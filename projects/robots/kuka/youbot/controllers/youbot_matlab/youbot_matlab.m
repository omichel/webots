% Description: MATLAB controller example for Webots

% uncomment the next two lines if you want to use
% MATLAB's desktop and interact with the controller
%desktop;
%keyboard;

TIME_STEP = 64; % time in [ms] of a simulation step

% Constants
global MAX_SPEED;
MAX_SPEED = 0.30;
global SPEED_INCREMENT;
SPEED_INCREMENT = 0.05;
global WHEEL_RADIUS;
WHEEL_RADIUS = 0.05;
global LX; % lateral distance from robot's COM to wheel [m].
LX = 0.158;
global LY; % longitudinal distance from robot's COM to wheel [m].
LY= 0.228;
global HEIGHTS;
HEIGHTS = ["ARM_BACK_PLATE_LOW", "ARM_BACK_PLATE_HIGH", "ARM_RESET", "ARM_FRONT_CARDBOARD_BOX", ...
           "ARM_HANOI_PREPARE", "ARM_FRONT_PLATE", "ARM_FRONT_FLOOR"];
global HEIGHTS_ID;
HEIGHTS_ID = containers.Map(HEIGHTS, [1:length(HEIGHTS)]);
global ORIENTATIONS;
ORIENTATIONS = ["ARM_BACK_RIGHT", "ARM_RIGHT", "ARM_FRONT_RIGHT", "ARM_FRONT", "ARM_FRONT_LEFT", "ARM_LEFT", ...
                "ARM_BACK_LEFT"];
global ORIENTATIONS_ID;
ORIENTATIONS_ID = containers.Map(ORIENTATIONS, [1:length(ORIENTATIONS)]);

% Robot State
global robot_vx; % forwards speed [m/s]
global robot_vy; % lateral speed [m/s]
global robot_omega; % angular speed [rad/s]
global current_height;
global current_orientation;

% Initialization
arm_init
base_init
gripper_init

% Short Demo
passive_wait(2.2)
automatic_behavior

% Keyboard control
previous_key = 0;
wb_keyboard_enable(1);

wb_console_print(sprintf(['\n \nControl commands:\n', ...
  '  Arrows:         Move the robot\n', ...
  '  Page Up/Down:   Rotate the robot\n', ...
  '  +/-:            (Un)grip\n', ...
  '  Shift + arrows: Handle the arm\n', ...
  '  Space:          Reset']), WB_STDOUT);

while wb_robot_step(TIME_STEP) ~= -1
  key = double(wb_keyboard_get_key());
  if ((key > 0) && key ~= previous_key)
    switch key
      case WB_KEYBOARD_UP
        base_forwards_increment
      case WB_KEYBOARD_DOWN
        base_backwards_increment
      case WB_KEYBOARD_LEFT
        base_strafe_left_increment
      case WB_KEYBOARD_RIGHT
        base_strafe_right_increment
      case WB_KEYBOARD_PAGEUP
        base_turn_left_increment
      case WB_KEYBOARD_PAGEDOWN
        base_turn_right_increment
      case {WB_KEYBOARD_END, ' '}
        wb_console_print('Reset', WB_STDOUT)
        base_reset
        arm_perform_reset
      case {'+', 388, 65585}
        wb_console_print('Grip', WB_STDOUT)
        gripper_grip
      case {'-', 390}
        wb_console_print('Ungrip', WB_STDOUT)
        gripper_release
      case {332, 65851, WB_KEYBOARD_UP | WB_KEYBOARD_SHIFT}
        wb_console_print('Increase arm height', WB_STDOUT)
        arm_increase_height
      case {326, 65853, WB_KEYBOARD_DOWN | WB_KEYBOARD_SHIFT}
        wb_console_print('Decrease arm height', WB_STDOUT)
        arm_decrease_height
      case {330, 65852, WB_KEYBOARD_RIGHT | WB_KEYBOARD_SHIFT}
        wb_console_print('Increase arm orientation', WB_STDOUT)
        arm_increase_orientation
      case {328, 65850, WB_KEYBOARD_LEFT | WB_KEYBOARD_SHIFT}
        wb_console_print('Decrease arm orientation', WB_STDOUT)
        arm_decrease_orientation
      otherwise
        wb_console_print('Wrong keyboard input', WB_STDERR)
    end
  end
  previous_key = key;
end
