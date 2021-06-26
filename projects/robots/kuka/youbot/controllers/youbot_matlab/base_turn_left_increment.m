function base_turn_left_increment
  global SPEED_INCREMENT;
  global MAX_SPEED;
  global robot_vx;
  global robot_vy;
  global robot_omega;
  robot_omega = robot_omega + SPEED_INCREMENT;
  if (robot_omega > MAX_SPEED)
    robot_omega = MAX_SPEED;
  end
  base_move(robot_vx, robot_vy, robot_omega);
end
