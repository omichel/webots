% Description: MATLAB controller example for Webots

% uncomment the next two lines if you want to use
% MATLAB's desktop and interact with the controller
%desktop;
%keyboard;

TIME_STEP = wb_robot_get_basic_time_step();

right_shoulder_motor  = wb_robot_get_device('ShoulderR');
left_shoulder_motor   = wb_robot_get_device('ShoulderL');
right_upper_arm_motor = wb_robot_get_device('ArmUpperR');
left_upper_arm_motor  = wb_robot_get_device('ArmUpperL');
right_lower_arm_motor = wb_robot_get_device('ArmLowerR');
left_lower_arm_motor  = wb_robot_get_device('ArmLowerL');

right_shoulder_sensor  = wb_robot_get_device('ShoulderRS');
right_upper_arm_sensor = wb_robot_get_device('ArmUpperRS');
right_lower_arm_sensor = wb_robot_get_device('ArmLowerRS');

wb_motor_set_available_torque(right_shoulder_motor,0.0);
wb_motor_set_available_torque(right_upper_arm_motor,0.0);
wb_motor_set_available_torque(right_lower_arm_motor,0.0);

left_shoulder_min_position  = wb_motor_get_min_position(left_shoulder_motor);
left_shoulder_max_position  = wb_motor_get_max_position(left_shoulder_motor);
left_upper_arm_min_position = wb_motor_get_min_position(left_upper_arm_motor);
left_upper_arm_max_position = wb_motor_get_max_position(left_upper_arm_motor);
left_lower_arm_min_position = wb_motor_get_min_position(left_lower_arm_motor);
left_lower_arm_max_position = wb_motor_get_max_position(left_lower_arm_motor);

wb_position_sensor_enable(right_shoulder_sensor,TIME_STEP);
wb_position_sensor_enable(right_upper_arm_sensor,TIME_STEP);
wb_position_sensor_enable(right_lower_arm_sensor,TIME_STEP);

wb_console_print('-------Symmetry example of ROBOTIS OP2-------',WB_STDOUT);
wb_console_print('The right arm is free while the left one mimics it.',WB_STDOUT);
wb_console_print('In order to move the left arm, add a force to the right arm:',WB_STDOUT);
wb_console_print('keep alt pressed and select the right arm.',WB_STDOUT);
wb_console_print('Now you just have to move the mouse without releasing it.',WB_STDOUT);
wb_console_print('This example illustrates also the selfCollision which is activated by default',WB_STDOUT);
wb_console_print(sprintf('The basic time step is %g ms',TIME_STEP),WB_STDOUT);

while wb_robot_step(TIME_STEP) ~= -1

  left_shoulder_position  = -wb_position_sensor_get_value(right_shoulder_sensor);
  left_upper_arm_position = -wb_position_sensor_get_value(right_upper_arm_sensor);
  left_lower_arm_position = -wb_position_sensor_get_value(right_lower_arm_sensor);

  if (left_shoulder_position > left_shoulder_max_position)
   left_shoulder_position = left_shoulder_max_position;
  elseif (left_shoulder_position < left_shoulder_min_position)
   left_shoulder_position = left_shoulder_min_position;
  end

  if (left_upper_arm_position > left_upper_arm_max_position)
   left_upper_arm_position = left_upper_arm_max_position;
  elseif (left_upper_arm_position < left_upper_arm_min_position)
   left_upper_arm_position = left_upper_arm_min_position;
  end

  if (left_lower_arm_position > left_lower_arm_max_position)
   left_lower_arm_position = left_lower_arm_max_position;
  elseif (left_lower_arm_position < left_lower_arm_min_position)
   left_lower_arm_position = left_lower_arm_min_position;
  end

  wb_motor_set_position(left_shoulder_motor,left_shoulder_position);
  wb_motor_set_position(left_upper_arm_motor,left_upper_arm_position);
  wb_motor_set_position(left_lower_arm_motor,left_lower_arm_position);
end
