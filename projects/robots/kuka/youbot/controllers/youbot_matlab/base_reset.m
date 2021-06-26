function base_reset
  speeds = [0.0, 0.0, 0.0, 0.0];
  base_set_wheel_speeds_helper(speeds)
  global robot_vx;
  robot_vx = 0.0;
  global robot_vy;
  robot_vy = 0.0;
  global robot_omega;
  robot_omega = 0.0;
end
