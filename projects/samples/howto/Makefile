# Copyright 1996-2021 Cyberbotics Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

TARGETS = binocular/controllers/binocular.Makefile \
	biped/controllers/biped.Makefile \
	center_of_mass/controllers/center_of_mass.Makefile \
	center_of_mass/controllers/rotation.Makefile \
	console/controllers/console.Makefile \
	custom_robot_window/plugins/robot_windows/custom_robot_window.Makefile \
	cylinder_stack/controllers/contact_points_supervisor.Makefile \
	force_control/controllers/force_control.Makefile \
	four_wheels/controllers/four_wheels.Makefile \
	gears/controllers/gears.Makefile \
	inverted_pendulum/controllers/inverted_pendulum.Makefile \
	mouse_events/controllers/mouse_events.Makefile \
	mouse_events/controllers/mouse_events_non_blocking.Makefile \
	omni_wheels/controllers/omni_wheels.Makefile \
	physics/plugins/physics/flying_mybot.Makefile \
	physics/controllers/physics.Makefile \
	sick_terrain_scanning/controllers/sick_terrain_scanning.Makefile \
	sick_terrain_scanning/controllers/sick_terrain_scanning_supervisor.Makefile \
	supervisor_draw_trail/controllers/supervisor_draw_trail.Makefile \
	texture_change/controllers/texture_change.Makefile \
	texture_change/controllers/texture_robot.Makefile \
	url/controllers/url.Makefile \
	vision/controllers/vision.Makefile \
	ziegler_nichols/controllers/ziegler_nichols.Makefile

.PHONY: release debug profile clean

release debug profile clean: $(TARGETS)

%.Makefile:
	+@echo "# make" $(MAKECMDGOALS) $(@:.Makefile=)
	+@make -s -C $(@:.Makefile=) $(MAKECMDGOALS)
