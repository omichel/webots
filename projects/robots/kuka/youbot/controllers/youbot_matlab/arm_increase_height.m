function arm_increase_height()
  global HEIGHTS_ID;
  global HEIGHTS;
  global current_height;
  global current_orientation;
  
  new_height_id = HEIGHTS_ID(current_height) + 1;
  
  % Prevents from going beyond index.
  if (new_height_id > HEIGHTS_ID.Count)
    new_height_id = HEIGHTS_ID.Count;
  end
  
  new_height = HEIGHTS(new_height_id);
  
  % Prevents self-colliding poses.
  if (strcmp(new_height, 'ARM_FRONT_FLOOR'))
    if (strcmp(current_orientation, 'ARM_BACK_LEFT') || strcmp(current_orientation, 'ARM_BACK_RIGHT'))
      new_height = current_height;
    end
  end
  
  arm_set_height(new_height);
end
