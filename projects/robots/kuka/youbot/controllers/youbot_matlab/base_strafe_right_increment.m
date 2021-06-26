function base_strafe_right_increment
  global SPEED_INCREMENT;
  global MAX_SPEED;
  global robot_vx;
  global robot_vy;
  global robot_omega;
  robot_vy = robot_vy - SPEED_INCREMENT;
  if (robot_vy < -MAX_SPEED)
    robot_vy = -MAX_SPEED;
  end
  base_move(robot_vx, robot_vy, robot_omega);
end
