function base_forwards_increment
  global SPEED_INCREMENT;
  global MAX_SPEED;
  global robot_vx;
  global robot_vy;
  global robot_omega;
  robot_vx = robot_vx + SPEED_INCREMENT;
  if (robot_vx > MAX_SPEED)
    robot_vx = MAX_SPEED;
  end
  base_move(robot_vx, robot_vy, robot_omega);
end
