function arm_decrease_orientation()
  global ORIENTATIONS_ID;
  global ORIENTATIONS;
  global current_orientation;
  global current_height;
  
  new_orientation_id = ORIENTATIONS_ID(current_orientation) -1;
  
  % Prevents from going beyond index.
  if (new_orientation_id < 1)
    new_orientation_id = 1;
  end
  
  new_orientation = ORIENTATIONS(new_orientation_id);
  
  % Prevents self-colliding poses.
  if (strcmp(new_orientation, 'ARM_BACK_RIGHT'))
    if (strcmp(current_height, 'ARM_FRONT_FLOOR'))
    	new_orientation = current_orientation;
    end
  end
  
  arm_set_orientation(new_orientation);
end
