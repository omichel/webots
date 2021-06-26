# Copyright 1996-2021 Cyberbotics Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Adapted from from https://github.com/RoboCup-Humanoid-TC/GameController/blob/master/protocols/python/gamestate.py

from construct import Array, Byte, Bytes, Const, Enum, Flag, Int16sl, Int16ul, Struct
try:
    from construct import PaddedString
except ImportError:
    # construct < 2.9.39
    from construct import String as PaddedString

Short = Int16ul

RobotInfo = "robot_info" / Struct(
    # NONE                                        0
    # PENALTY_HL_KID_BALL_MANIPULATION            1
    # PENALTY_HL_KID_PHYSICAL_CONTACT             2
    # PENALTY_HL_KID_ILLEGAL_ATTACK               3
    # PENALTY_HL_KID_ILLEGAL_DEFENSE              4
    # PENALTY_HL_KID_REQUEST_FOR_PICKUP           5
    # PENALTY_HL_KID_REQUEST_FOR_SERVICE          6
    # PENALTY_HL_KID_REQUEST_FOR_PICKUP_2_SERVICE 7
    # MANUAL                                      15
    "penalty" / Byte,
    "secs_till_unpenalized" / Byte,
    "number_of_warnings" / Byte,
    "number_of_yellow_cards" / Byte,
    "number_of_red_cards" / Byte,
    "goalkeeper" / Flag
)

TeamInfo = "team" / Struct(
    "team_number" / Byte,
    "team_color" / Enum(Byte,
                        BLUE=0,
                        RED=1,
                        YELLOW=2,
                        BLACK=3,
                        WHITE=4,
                        GREEN=5,
                        ORANGE=6,
                        PURPLE=7,
                        BROWN=8,
                        GRAY=9),
    "score" / Byte,
    "penalty_shot" / Byte,  # penalty shot counter
    "single_shots" / Short,  # bits represent penalty shot success
    "coach_sequence" / Byte,
    "coach_message" / PaddedString(253, 'utf8'),
    "coach" / RobotInfo,
    "players" / Array(11, RobotInfo)
)

GameState = "gamedata" / Struct(
    "header" / Const(b'RGme'),
    "version" / Const(12, Short),
    "packet_number" / Byte,
    "players_per_team" / Byte,
    "game_type" / Byte,
    "game_state" / Enum(Byte,
                        STATE_INITIAL=0,
                        STATE_READY=1,      # go to start positions
                        STATE_SET=2,        # keep ready
                        STATE_PLAYING=3,    # start play
                        STATE_FINISHED=4),  # game over
    "first_half" / Flag,
    "kickoff_team" / Byte,
    "secondary_state" / Enum(Byte,
                             STATE_NORMAL=0,
                             STATE_PENALTYSHOOT=1,
                             STATE_OVERTIME=2,
                             STATE_TIMEOUT=3,
                             STATE_DIRECT_FREEKICK=4,
                             STATE_INDIRECT_FREEKICK=5,
                             STATE_PENALTYKICK=6,
                             STATE_CORNERKICK=7,
                             STATE_GOALKICK=8,
                             STATE_THROWIN=9,
                             STATE_DROPBALL=128,
                             STATE_UNKNOWN=255),
    "secondary_state_info" / Bytes(4),
    "drop_in_team" / Flag,
    "drop_in_time" / Short,
    "seconds_remaining" / Int16sl,
    "secondary_seconds_remaining" / Int16sl,
    "teams" / Array(2, "team" / TeamInfo)
)

GAME_CONTROLLER_RESPONSE_VERSION = 2

ReturnData = Struct(
    "header" / Const(b"RGrt"),
    "version" / Const(2, Byte),
    "team" / Byte,
    "player" / Byte,
    "message" / Byte
)
