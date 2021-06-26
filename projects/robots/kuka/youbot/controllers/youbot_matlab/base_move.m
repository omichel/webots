function base_move(vx, vy, omega)
  global WHEEL_RADIUS;
  global LX;
  global LY;
  speeds = [ 1 / WHEEL_RADIUS * (vx + vy + (LX + LY) * omega),
             1 / WHEEL_RADIUS * (vx - vy - (LX + LY) * omega),
             1 / WHEEL_RADIUS * (vx - vy + (LX + LY) * omega),
             1 / WHEEL_RADIUS * (vx + vy - (LX + LY) * omega)];
  base_set_wheel_speeds_helper(speeds);
  wb_console_print(sprintf('Speeds: vx=%.2f[m/s] vy=%.2f[m/s] ω=%.2f[rad/s]', vx, vy, omega), WB_STDOUT)
end
