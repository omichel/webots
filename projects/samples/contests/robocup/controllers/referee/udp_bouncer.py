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

import socket
import threading
import json
import sys
import time

try:
    from queue import SimpleQueue
except ImportError:
    # python < 3.7
    from queue import Queue as SimpleQueue


def log(message):
    global log_file
    if log_file:
        real_time = int(1000 * (time.time() - log.real_time)) / 1000
        log_file.write(f'[{real_time:08.3f}] {message}\n')
        log_file.flush()


log_file = open("bouncing_log.txt", "w")
log.real_time = time.time()

# Port we receive messages from the GameController
UDP_GC_LISTEN_PORT = 3839

# Port we forward GameController messages to
UDP_GC_SEND_PORT = 3838

# Port we receive messages from robots
UDP_TEAM_LISTEN_PORT = 3737

# Port we forward robot messages to
UDP_TEAM_SEND_PORT = 3737

# IP of this UDP server
SERVER_IP = "0.0.0.0"

# Buffer size
BUFFER = 1024

# Setting up a Queue to handle incoming packages
package_queue = SimpleQueue()

# List of registered clients to send the information to
clients = []
robots_blue = []
robots_red = []


# Sends a given package to a given IP on the UDP_SEND_PORT
def send_message_to_client(ip, port, package):
    send_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP
    send_sock.sendto(package, (ip, port))


# Main function of the server that handles packages from clients and adds them to a queue
def receive_input_from_client(port):
    # Binding the server that listens to incoming UDP packages
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP
    sock.bind((SERVER_IP, port))
    log(f"Binding receive on {SERVER_IP}:{port}")

    while True:
        data, addr = sock.recvfrom(BUFFER)
        package_queue.put([time.time(), addr[0], port, data])
        time.sleep(0.005)


# Function that listens to queue updates and sends the messages out to the individual clients
def on_queue_update():
    while True:
        if not package_queue.empty():
            current_package = package_queue.get()
            if current_package[2] == 3839:  # we received a GC update message
                for client in clients:
                    client_thread = threading.Thread(target=send_message_to_client, args=[client, UDP_GC_SEND_PORT,
                                                                                          current_package[3]])
                    client_thread.start()
            else:  # we received an update from one of the teams
                log(current_package)
                if current_package[1] in robots_blue:
                    for client in robots_blue:
                        if client != current_package[1]:
                            client_thread = threading.Thread(target=send_message_to_client,
                                                             args=[client, UDP_TEAM_SEND_PORT, current_package[3]])
                            client_thread.start()
                elif current_package[1] in robots_red:
                    for client in robots_red:
                        if client != current_package[1]:
                            client_thread = threading.Thread(target=send_message_to_client,
                                                             args=[client, UDP_TEAM_SEND_PORT, current_package[3]])
                            client_thread.start()
                else:
                    log("We received a message on the team communication port from a robot not registered with one of the"
                        " teams. This should not happen.")
        time.sleep(0.005)


# Loads all client IP addresses from the game.json config file
def add_clients_from_config(config):
    with open(config, 'r') as game_json:
        config = json.load(game_json)
        clients.append(config['host'])
        for blue_robot in config["blue"]["hosts"]:
            if blue_robot != "127.0.0.1":
                clients.append(blue_robot)
                robots_blue.append(blue_robot)
        for red_robot in config["red"]["hosts"]:
            if red_robot != "127.0.0.1":
                clients.append(red_robot)
                robots_red.append(red_robot)


def start_bouncing_server(game_config):
    log("Initializing UDP Server")

    add_clients_from_config(game_config)
    log("Successfully read in %s" % game_config)
    log("List of clients registered with the server is %s" % clients)
    log("Robots in team blue are %s" % robots_blue)
    log("Robots in team red are %s" % robots_red)

    queue_thread = threading.Thread(target=on_queue_update)
    queue_thread.start()
    log("Queue thread started")

    server_thread = threading.Thread(target=receive_input_from_client, args=[UDP_GC_LISTEN_PORT])
    server_thread.start()
    log("Server thread for GC port started")

    server_thread = threading.Thread(target=receive_input_from_client, args=[UDP_TEAM_LISTEN_PORT])
    server_thread.start()
    log("Server thread for Robot port started")

    log("Setup completed")


if __name__ == "__main__":
    start_bouncing_server(sys.argv[1])
