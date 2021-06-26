function base_init
  global robot_vx
  robot_vx = 0.0;
  global robot_vy
  robot_vy = 0.0;
  global robot_omega
  robot_omega = 0.0;
  global wheel
  for i=1:4
    wheel(i) = wb_robot_get_device(sprintf('wheel%d', i));
  end
end
