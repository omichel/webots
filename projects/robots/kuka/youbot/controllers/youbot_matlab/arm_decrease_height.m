function arm_decrease_height()
  global HEIGHTS_ID;
  global HEIGHTS;
  global current_height;
  
  new_height_id = HEIGHTS_ID(current_height) -1;
  
  % Prevents from going beyond index.
  if (new_height_id < 1)
    new_height_id = 1;
  end
  
  new_height = HEIGHTS(new_height_id);  
  arm_set_height(new_height);
end
