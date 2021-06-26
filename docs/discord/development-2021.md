# Development 2021

This is an archive of the `development` channel of the [Webots Discord server](https://discordapp.com/invite/nTWbN9m) for year 2021.

## January

##### Simon Steinmann [Moderator] 01/05/2021 09:27:08
The conversation in technical-questions made me think: Wouldn't it be a nice feature, to have an 'odometry' device? Which retrieves the pose of it's parent. Of course this can be done manually with a supervisor, however that is more complicated for beginners and well, requires a supervisor.

##### Olivier Michel [Cyberbotics] 01/05/2021 09:47:22
Yes, but I believe that such an odometry device doesn't exists in real life...

##### Simon Steinmann [Moderator] 01/05/2021 09:56:13
no it doesnt, but it can be a good first step / validation tool. This is used all the time in ROS and Gazebo


It could be implemented in the Robot node perhaps. "getPosition", "getOrientation"

##### Olivier Michel [Cyberbotics] 01/05/2021 10:01:28
It is already there when you turn your robot into a supervisor.

##### Simon Steinmann [Moderator] 01/05/2021 10:10:39
well I guess that's true. Still could be useful to have a device one could add to Solids/Transforms, in order to retrieve the pose


Found a strange behavior, exporting a robot as URDF uses the boundingObject as the visual geometry, not the shape. Is this on purpose? Or simply because mesh extraction is not implemented yet?

##### Darko Lukić [Cyberbotics] 01/07/2021 08:00:15
Yes, it is described in the note under `wb_robot_get_urdf` documentation. We have a basic implementation of URDF exporter that can work for most use-cases (publish ROS transforms, calculate inverse kinematics, and similar), but only a basic visualization is possible.

##### Simon Steinmann [Moderator] 01/07/2021 10:01:47
`@Darko Lukić` launching the dynamic rviz with ros2, this Error comes up
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/796679973011718164/unknown.png)
%end



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/796680042905337867/unknown.png)
%end


this is part of the console output
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/796680283385888779/unknown.png)
%end


I suspect that the reason is rviz launching before webots and ros2 is fully loaded


loading the same config again once it is running, no errors come up

##### Darko Lukić [Cyberbotics] 01/07/2021 10:04:06
RViz2 is a bit buggy, just uncheck/check the RobotModel checkbox and it should work

##### Simon Steinmann [Moderator] 01/07/2021 10:04:29
you're right


is there a quick and easy way to include the world frame in the tf broadcast?

##### Darko Lukić [Cyberbotics] 01/07/2021 10:06:26
You can add a static transform publisher to your launch file:

```py
static_tf = Node(
  package='tf2_ros',
  executable='static_transform_publisher',
  name='static_transform_publisher',
  output='log',
  arguments=['0.0', '0.0', '0.0', '0.0', '0.0', '0.0', 'world', 'base_link']
)
```

##### Simon Steinmann [Moderator] 01/07/2021 10:07:00
what if the robot is mobile?

##### Darko Lukić [Cyberbotics] 01/07/2021 10:07:48
Than you have to publish the transforms manually :/

##### Simon Steinmann [Moderator] 01/07/2021 10:09:00
does that work alongside the robot\_state\_publisher?

##### Darko Lukić [Cyberbotics] 01/07/2021 10:10:07
Sure

##### Simon Steinmann [Moderator] 01/07/2021 10:10:37
maybe that should be a feature of the webots\_ros2\_core

##### Darko Lukić [Cyberbotics] 01/07/2021 10:11:42
We do it here for example:

[https://github.com/cyberbotics/webots\_ros2/blob/7aa84877e0dc5d668e923fd2f23ebd4acb865d90/webots\_ros2\_examples/webots\_ros2\_examples/khepera\_driver.py#L92-L105](https://github.com/cyberbotics/webots_ros2/blob/7aa84877e0dc5d668e923fd2f23ebd4acb865d90/webots_ros2_examples/webots_ros2_examples/khepera_driver.py#L92-L105)


The robot state publisher is in the core, what would you like to add in addition to it?

##### Simon Steinmann [Moderator] 01/07/2021 10:13:18
have an option (or by default) to pulbish the world - baselink transform


static robots are one thing, but especially for mobile ones, it is kinda essential.

##### Darko Lukić [Cyberbotics] 01/07/2021 10:14:52
But that is something that a user has to handle

##### Simon Steinmann [Moderator] 01/07/2021 10:15:37
Gazebo publishes it by default, people are used to simply selecting "world" in rviz

##### Darko Lukić [Cyberbotics] 01/07/2021 10:17:16
Gazebo publishes transform between `world` and `base_link` for mobile robots? That sounds like cheating


In the real-world transform between `world` and `base_link` is not available

##### Simon Steinmann [Moderator] 01/07/2021 10:18:02
maybe it is just static. Has been a while, dont remember

##### Darko Lukić [Cyberbotics] 01/07/2021 10:18:31
We can easily add it using Supervisor, but it is strange to have something like that

##### Simon Steinmann [Moderator] 01/07/2021 10:20:13
hmmm where exactly would I add that tf\_static node?


for example when using this: [https://github.com/cyberbotics/webots\_ros2/blob/master/webots\_ros2\_universal\_robot/launch/universal\_robot\_rviz\_dynamic.launch.py](https://github.com/cyberbotics/webots_ros2/blob/master/webots_ros2_universal_robot/launch/universal_robot_rviz_dynamic.launch.py)


line 43?


jup, works 🙂

##### Moha 01/13/2021 07:53:46
hi guys

is it possible to use "vscode" as my IDE?

##### Olivier Michel [Cyberbotics] 01/13/2021 07:55:10
Sure.

##### Ragemor 01/13/2021 08:06:20
[https://cyberbotics.com/doc/guide/using-your-ide](https://cyberbotics.com/doc/guide/using-your-ide)

##### Moha 01/13/2021 08:07:09
👍

##### Simon Steinmann [Moderator] 01/13/2021 08:44:39
I'm curious, wouldn't it be possible to set system environment variables during installation?


`@Darko Lukić` `@Olivier Michel` I've got moveit2 working with ros2 and webots. I also made some progress of creating an easy to use template or automatic setup for robotic arms


I think one important feature would be, to automatically extract the urdf from webots, including meshes.


I already did some work in that direction with my proto-splitter script. That one worked, extracting .obj or vrml meshes, but without smoothing

##### Olivier Michel [Cyberbotics] 01/13/2021 09:42:21
Probably yes on Windows, probably not possible on Mac and Linux...

##### Simon Steinmann [Moderator] 01/13/2021 09:44:28
ahh okay


Also I was wondering: could it be possible to launch a ROS1 or ROS2 launch file from within webots?

##### Olivier Michel [Cyberbotics] 01/13/2021 11:23:51
I am not sure this is a good idea as we want to be able to interact with ROS in the console...

##### Darko Lukić [Cyberbotics] 01/13/2021 15:44:30
It may be possible to implement events in the launch file (like here [https://github.com/cyberbotics/webots\_ros2/blob/38d88e01fe174a8a00731f554f1a8646b9127bd2/webots\_ros2\_demos/launch/armed\_robots.launch.py#L63](https://github.com/cyberbotics/webots_ros2/blob/38d88e01fe174a8a00731f554f1a8646b9127bd2/webots_ros2_demos/launch/armed_robots.launch.py#L63)), so if reset controllers in Webots it supposed to restart certain ROS2 nodes as well within the launch file. This would be useful feature and it would speed up development significantly (as you don't need to reload the world).

##### babaev1 01/16/2021 20:00:53
Dear Sirs, recently Organizing Commitee of Humanoid League of Robocup Federation decided to host Summer World Championship in virtual format with organizing games between teams within simulator. Currently simulator platforms are under consideration for choosing. There are going to be games between 2 teams , with each team providing 4 humanoid robots  equipped by 23-25DoF servomotors, 1-2 imus, 1-2 cameras. Imortant point is streaming of images from cameras to external controller softwares of teams. Currently built in webots camera streams images in non-compressed format. Supposing simultaneously streaming images from 8 cameras with resolution 640x480 RGB  with 30 fps simulation is expected to be dramatically slowing down.  Some of teams are expected to use stereo cameras, means number of cameras in this case will be doubled.


Do you plan to add compression to image streaming in near future, or there are some implementations which I am not familiar with?

##### cnbarcelo 01/17/2021 15:43:05
Hi! Where can I find the sensors visualization code? I'm mainly looking for the the lidar visualization.

##### Darko Lukić [Cyberbotics] 01/17/2021 17:59:21
`@cnbarcelo` You can use `View / Optional Rendering / Show Lidar Point Cloud` and `View / Optional Rendering / Show Lidar Rays Paths`:

[https://cyberbotics.com/doc/guide/the-user-interface](https://cyberbotics.com/doc/guide/the-user-interface)



Remember, to show the point cloud, you need to enable the point cloud first. You can do it from the robot window (double click on the robot) or from your controller:

[https://cyberbotics.com/doc/reference/lidar#wb\_lidar\_enable\_point\_cloud](https://cyberbotics.com/doc/reference/lidar#wb_lidar_enable_point_cloud)

##### cnbarcelo 01/17/2021 18:03:15
Yeah, I do know that, but I migrated from 2020 to 2021 and it's not working well. I wanted to check if something changed before creating a ticket.


If there's any bug to work on, I might be able to collaborate.

##### Darko Lukić [Cyberbotics] 01/17/2021 18:06:12
It works fine for me. Can you tell more details about your issue?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/800425762247278592/unknown.png)
%end

##### cnbarcelo 01/17/2021 18:07:38
The visualization blinks. It works for a couple of seconds, stop working, and starts working again, completely randomly.


It happens in my simulation and also in the lidar world example.


I'll record a video and share it here later on today.

##### Chernayaten 01/17/2021 18:08:54
Hey Darko, I also wrote a comment few weeks ago about this in <#565154703139405824> . I run the lidar.wbt example and although it shows the point cloud at first it just stops working correctly


In my simulation it doesn't work at all

##### cnbarcelo 01/17/2021 18:09:43
That's exactly what happens, will share a video later then, so you Darko can see it.


I didn't see your comment in <#565154703139405824> , that's way I came here with the issue.

##### Darko Lukić [Cyberbotics] 01/17/2021 18:10:38
It may be specific to GPU or OS. Could you please report the bug:

[https://github.com/cyberbotics/webots/issues/new?assignees=&labels=&template=bug\_report.md](https://github.com/cyberbotics/webots/issues/new?assignees=&labels=&template=bug_report.md)

##### Chernayaten 01/17/2021 18:12:43
What info would you like besides GPU and OS?

##### cnbarcelo 01/17/2021 18:16:21
I'm running Ubuntu and an Nvidia card.

I'm out of office now, will check if you already created the issue when I'm back, and will add the info of my system there, or will create it by my own.

##### Chernayaten 01/17/2021 18:24:03
[https://github.com/cyberbotics/webots/issues/2660](https://github.com/cyberbotics/webots/issues/2660)



I added a short video to show it

##### Darko Lukić [Cyberbotics] 01/17/2021 19:23:47
Thank you, we will check it out


It may be related to this PR:

[https://github.com/cyberbotics/webots/pull/2509](https://github.com/cyberbotics/webots/pull/2509)



It is possible that the visualization part fails to handle infinity values.

##### Chernayaten 01/17/2021 20:12:04
After a few tests it seems you are correct. The same issue presents when the robot gets close to objects because " If the range value is smaller than the minRange value then infinity is returned."

##### Olivier Michel [Cyberbotics] 01/18/2021 07:58:52
In Webots images are read from the GPU memory directly into a shared memory segment which is readable by both Webots and the local robot controller to optimize efficiency and avoid unnecessary image copy. In the context of RoboCup (and other frameworks, like ROS or ROS2), the local robot controller acts as a relay between the simulator and the actual robot controller which may run on a different machine over the network. Therefore, while implementing this relay controller, it is very easy to add JPEG compression to the camera image to be sent to the actual controller over the network. This is indeed exactly what happens when you use a ROS 2 controller with Webots to publish a camera image, it gets automatically compressed in JPEG (and you can even choose the compression ratio) before being sent to the controller.

##### babaev1 01/18/2021 09:23:07
Thank you for prompt reply. For me it is clear. I shall share your answer with Robocup community for possible definitions.

##### Moha 01/19/2021 09:49:08
hi guys

Is it possible to use spot robot SDK releasing from Boston dynamics in webots ?

##### Darko Lukić [Cyberbotics] 01/19/2021 10:01:18
Hello `@Moha`, Webots doesn't provide support for Spot SDK. It is not in the scope of Webots, but I advise you to create a discussion:

[https://github.com/cyberbotics/webots/discussions/categories/ideas](https://github.com/cyberbotics/webots/discussions/categories/ideas)

If there is a strong need (many requests), somebody from the community may create the integration.

##### Moha 01/19/2021 10:06:20
hi dear Darko

good idea, ok

but may I ask you how is it possible to request for it ?

##### Darko Lukić [Cyberbotics] 01/19/2021 10:08:39
You can create a discussion:

[https://github.com/cyberbotics/webots/discussions/new](https://github.com/cyberbotics/webots/discussions/new)



Explain why the integration is necessary, how big impact it will have on the community...

##### Moha 01/19/2021 10:14:43
I've understood what you mean in first reply, but I mean I want to know how other people can show their enthusiasm for it ?

##### Darko Lukić [Cyberbotics] 01/19/2021 10:15:56
They can join the discussion, like it, suggest new ideas

##### Moha 01/19/2021 10:52:14
ok thank you 👍

##### mayank.kishore 01/20/2021 05:59:29
What is the best way to locally edit a webots package?

##### Darko Lukić [Cyberbotics] 01/20/2021 07:30:40
You can compile as here:

[https://github.com/cyberbotics/webots\_ros2/wiki/Build-and-Install](https://github.com/cyberbotics/webots_ros2/wiki/Build-and-Install)

##### Simon Steinmann [Moderator] 01/20/2021 09:48:01
Is there documentation for custom robot window? I cannot seem to find it.

##### Olivier Michel [Cyberbotics] 01/20/2021 09:58:59
[https://cyberbotics.com/doc/guide/controller-plugin#robot-window](https://cyberbotics.com/doc/guide/controller-plugin#robot-window) (but it's very limited).


[https://cyberbotics.com/doc/guide/controller-plugin?version=master#robot-window](https://cyberbotics.com/doc/guide/controller-plugin?version=master#robot-window) (this is a newer version, with much more details)


(but refers to the master branch, not the released version)


(to be released in the next revision)

##### Simon Steinmann [Moderator] 01/20/2021 10:01:32
yeah, i'm excited for the new changes there. I'm gonna wait then until I dive back in

##### fowzan 01/21/2021 18:08:36
I would love to simulate spot mini on my terminal , could you share links if available

##### mayank.kishore 01/22/2021 16:58:40
Having trouble editing a package locally, please message me if you can help


Do you know why I am getting this error?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/802613647226044436/unknown.png)
%end

##### Bitbots\_Jasper [Moderator] 01/23/2021 19:02:50
you seem to be missiong ros-<version>-console-bridge

##### adiagr 01/24/2021 18:32:16
You can first threshold the image and then find contours in the image. Select the contour with the maximum area. Finally, find centroid of that contour and use it as an input to a PID controller or something similar to drive your robot

##### Simon Steinmann [Moderator] 01/25/2021 15:11:40
I got the suggestion from a user to add voice channels to the discord server.  Could be a nice addition

##### Olivier Michel [Cyberbotics] 01/25/2021 15:29:48
I just created one. Can you try it?

##### Simon Steinmann [Moderator] 01/25/2021 15:30:06
seems to work

##### Krish 01/25/2021 15:33:10
oh wow, already created.

##### mayank.kishore 01/25/2021 15:55:58
Does anyone have experience creating an overhead map with a camera? And potentially stitching multiple images together to create an encompassing overhead map?

##### babaev1 01/25/2021 16:04:06
There is good feature in Coppelia which I couldn’t find in Webots: position and orientation of each node can be measured and modified from GUI.

##### Simon Steinmann [Moderator] 01/25/2021 17:13:51
you can do the same in Webots in the scene tree

##### babaev1 01/25/2021 17:34:59
Yes, it is possible to read absolute position in scene tree, but modification of absolute position or orientation is not accessible there.

##### Simon Steinmann [Moderator] 01/25/2021 17:38:12
it is


just change the values
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/803317976853905448/unknown.png)
%end

##### babaev1 01/25/2021 17:43:41
In case if in children of parent Solid child Solid is added direct change of values of child Solid appears complex especially in case if in parent Solid rotation is not zero.


I try to compose humanoid robot with 6 DOF in each branch. Direct modification of absolute position of deep level child elements is really challenging.

##### Simon Steinmann [Moderator] 01/25/2021 17:50:44
that is the nature of kinematic chains


that is where you need inverse kinematics

##### babaev1 01/25/2021 17:56:54
In case if I select node in scene tree I can see absolute position and rotation of node, but values are not editable. Possibility of modification of absolute position and orientation through editing values could be helpful.


In Coppelia it is possible

##### Simon Steinmann [Moderator] 01/26/2021 09:11:26
`@babaev1` If I understand you correctly, you want to position a child node using absolute coordinates, which would automatically calculate the relative coordinates to the parent?

##### babaev1 01/26/2021 13:51:17
I am using Coppelia for teaching robotics in school since 2 years. There is functionally which I consider useful and I use it often. It is possible to call pop-up widow for reading and editing absolute coordinate of any node which you select on scene tree or directly on 3D rendering window. After giving new coordinate and position old link to parent node is replaced by new link. Position and orientation of parent node remain unchanged. Position and orientation of all child nodes changes together with selected node new position and orientation. In case if you want to protect complex structures from being destroyed by this action node can be protected from being selected in node properties. I started to study Webots 2 weeks ago and I didn’t find similar useful functionality.


window (not widow)

##### Simon Steinmann [Moderator] 01/27/2021 10:35:31
`@babaev1` So you want the reference frame selected here (in this case 'Absolute') to change the axis in the 3D view and the translation + rotation field in the scene tree. So they can be edited in the chosen frame. However, the saved definition is still the relative position to its parent.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/803936219175452702/unknown.png)
%end


`@Olivier Michel` this could be a nice feature and should not necessarily be too complex to include

##### Olivier Michel [Cyberbotics] 01/27/2021 10:41:53
Yes, I believe changing these position and rotation fields from read-only to read/write should be fairly easy to implement. However, I am not sure to understand why it seems useful. In general objects can be moved from the root-level translation/rotation. Moving an object inside its parent is not something we want to do from the global coordinate system. So, I am not sure to understand the use case for this...

##### babaev1 01/27/2021 10:43:34
This is useful during building new robot


In order to build 23 DOF robot from primitives I spent 2 days in Coppelia and about 1 week in Webots.

##### Olivier Michel [Cyberbotics] 01/27/2021 10:54:57
Ok, thank you for the explanation. Then, you should probably open an issue about this (or implement it by yourself if you want).

##### babaev1 01/27/2021 11:01:24
First variant is more realistic for my statement of amateur programmer.

##### Olivier Michel [Cyberbotics] 01/27/2021 11:09:23
I believe this really not so difficult to implement...

##### babaev1 01/28/2021 22:07:31
Dear Webots developers, do you have plan to add Russian language to variety of GUI languages in 2021? Together with pandemic and moving robotic lessons and competitions to virtual format there is higher interest to use simulation software in education. Unfortunately not many teachers and students are fluent with English in Russian schools.  Adding optional Russian language to GUI can give good advantage to Webots to be widely used in Russian schools.

##### Darko Lukić [Cyberbotics] 01/28/2021 22:15:23
Hello `@babaev1`, feel free to add translations, we will be happy to review your PR:

[https://github.com/cyberbotics/webots/tree/master/resources/translations](https://github.com/cyberbotics/webots/tree/master/resources/translations)

##### babaev1 01/28/2021 22:19:39
Thank you. Good to know that it is so easy. I shall arrange people at my side to add Russian translation.

##### Professor Felix 01/29/2021 04:31:27
does webots use git LFS? I'm just a student who cloned the repo to try and understand some of the internal codebase for educational purposes and I noticed the file size was over 5GB

##### Olivier Michel [Cyberbotics] 01/29/2021 07:48:43
You should not need git LFS to check out Webots as Webots doesn't contain any file larger than 5GB.

##### Professor Felix 01/29/2021 08:33:24
I know I just thought GitHub itself preferred to keep repositories under 2GB


But I think I heard wrong

##### Olivier Michel [Cyberbotics] 01/29/2021 08:34:33
Yes, there not such a limit in GitHub.

##### Simon Steinmann [Moderator] 01/29/2021 09:58:03
`@Darko Lukić` wouldn't it be possible to run a translator api over the language files and do a base translation? That seems like a fairly easy and quick way to at least offer more languages. Of course they may not be perfect, but might be bettter than nothing

##### Darko Lukić [Cyberbotics] 01/29/2021 10:08:53
I think it is possible, but I am concern about the quality of the translations.

##### Bitbots\_Jasper [Moderator] 01/29/2021 10:10:29
I think the quality would be rather poor since there is often only a small snippet of text and translation algorithms usually work much better with context, it might be worth a shot but a native or very fluent speaker would need to rework it

##### Ginzo1 01/29/2021 12:47:01
Hello, everyone! I've just recently pick up WeBots as I have an university project that requires to be simulated. It is a robot design project for which I will need a hydraulic scissors lift. Since I have a relatively basic knowledge on mechanics and kinematics but I am still really inexperienced with WeBots. Since the scissors lift is a quite common system, I was wondering if there is some information somewhere on how to create one on my own or see how it is supposed to be done from scratch? Thanks in advance!

##### Bitbots\_Jasper [Moderator] 01/29/2021 12:49:47
`@Ginzo1` this question has been asked a while back, maybe there is something useful for you there [https://cyberbotics.com/doc/discord/development-2020#lukulus-11132020-10-48-16](https://cyberbotics.com/doc/discord/development-2020#lukulus-11132020-10-48-16)

##### babaev1 01/30/2021 10:34:05
Dear Webots developers, it is more or less clear how to translate GUI to Russian language. Students in our University started to undertake this work. Separate story is how to translate and publish User Guide and Reference Manual which  is located in website of Cyberbotics. There are 2 variants: 1. Russian translation will be published in Cyberbotics site along with English. 2. In Russian GUI link to User Guide will be different, then we can undertake providing space on web for translated User Guide.

##### Simon Steinmann [Moderator] 01/30/2021 14:18:53
`@babaev1` Awesome! Thank you for the contribution. I'm sure the devs will do everything they can to support your effort!


[https://github.com/cyberbotics/webots/tree/master/docs](https://github.com/cyberbotics/webots/tree/master/docs) this is the documentation location of Webots. "guide" and "reference" are the 2 big documentation directories. I'm sure you can make a fork and start translating these. `@Olivier Michel` Perhaps we should slightly change the structure to accommodate any new translations.

##### babaev1 01/31/2021 11:10:23
Thank you, I shall look for opportunity to make this part to be translated too.

## February

##### Olivier Michel [Cyberbotics] 02/01/2021 10:52:38
`@babaev1`: thank you for undertaking this translation effort.

For translating the GUI, please follow the procedure at [https://github.com/cyberbotics/webots/tree/released/resources/translations](https://github.com/cyberbotics/webots/tree/released/resources/translations)

We will be happy to merge your contribution into Webots.

For translating the manuals, I believe the option 2 is the best for us: we will include in the user guide and reference manual links to community contributed translations.

##### Lumii 02/09/2021 10:36:01
is there any tutorial how i can start writing a program to actually move (UR) my robot in the simulation?

##### Bitbots\_Jasper [Moderator] 02/09/2021 10:37:40
[https://cyberbotics.com/doc/guide/tutorial-4-more-about-controllers](https://cyberbotics.com/doc/guide/tutorial-4-more-about-controllers)


especially the program the controller section


or if you want to do actual motion planning and more complicated motions like planned trajectories or constraints on those trajectories, I would recommend ROS with moveit, but that is quite a project to get into

##### Lumii 02/09/2021 10:45:31
ye i use to work with moveit but moveit2 currently only supports c++ 

and im tryin to create a ROS2 class for my university and it should be in python because the students are learing python in prev classes. So im lookin for an alternativ so that the students can still learn ros2 but with python instead of c++

##### Simon Steinmann [Moderator] 02/09/2021 11:31:47
`@Lumii` Cyberbotics and I also developed a standalone IKFast Python module


that makes ik super fast and easy to use in webots


[https://github.com/cyberbotics/pyikfast](https://github.com/cyberbotics/pyikfast)


This includes an example I made with the IRB4600 robot. You only have to recompile the pyikfast solver for your robot. Everything else should work out of the box (robot needs to be supervisor, And the init values for this example may have to be changed). I tried my best to comment the code. PM me if you have questions or suggestions
> **Attachment**: [irb4600\_ikfast\_sample\_2.zip](https://cdn.discordapp.com/attachments/565155651395780609/808666959016099840/irb4600_ikfast_sample_2.zip)

##### Lumii 02/09/2021 15:09:09
Oh thanks a lot I’ll look into it!

##### Chernayaten 02/10/2021 22:52:00
Double clicking a .wbt file to start Webots, from a path-file inside a Greek folder crashes the program. To be more specific, Webots starts with the following message showing on the console window for a split second before the program terminates automatically.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/809194994279645214/unknown.png)
%end


If I initiate the webots program from its shortcut I can normally load the world without any issues


I'd argue that besides the incorrect handling of the greek characters (and possibly other characters as well), the program shouldn't crash if it fails to load the world

##### Olivier Michel [Cyberbotics] 02/11/2021 06:36:27
Can you please open a bug report about it? [https://github.com/cyberbotics/webots/issues/new?assignees=&labels=&template=bug\_report.md](https://github.com/cyberbotics/webots/issues/new?assignees=&labels=&template=bug_report.md)

##### AJ121 02/12/2021 21:32:21
Can't I set a motor speed more than 10 in webots? It shows error ! My bot is too slow

##### Stefania Pedrazzi [Cyberbotics] 02/15/2021 07:11:58
`@AJ121` the maximum speed for a robot is defined in the `Motor.maxVelocity` field. You should check the value for your robot model and adjust it:

[https://www.cyberbotics.com/doc/reference/motor#field-summary](https://www.cyberbotics.com/doc/reference/motor#field-summary)

##### JMarple 02/16/2021 18:50:55
I wish I had more information (cause I know just saying something is broken isn't all that helpful) but I figure I'd atleast pass this along: 

Running webots on Mac OSX 10.15.6 on MBP 2020, I've been finding webots crashes quite regularly.  It can be just from starting a simulation, or moving a part within the UI, or doing nothing at all, it'll just randomly crash.  Maybe a frequency of once per hour or so. 



Is there any sort of verbose mode or *something* I can do to help track down these issues?

##### Darko Lukić [Cyberbotics] 02/16/2021 19:05:53
Hello `@JMarple`. You can compile Webots in debug mode and you should get more details about the crashes:

[https://github.com/cyberbotics/webots/wiki/macOS-installation](https://github.com/cyberbotics/webots/wiki/macOS-installation)



Please let us know if you have any problems compiling Webots.

##### Ariel 02/16/2021 20:50:48
Hello! I am writing a program in webots for an autonomous vehicle moving around a specific path and collecting data (images) of the environment. I added a fog node on the scene and the cameras that I had already can not see it. Is this the expected behaviour?

##### Darko Lukić [Cyberbotics] 02/16/2021 21:34:54
Hello Ariel, it sounds wrong. Can you open an issue about that:

[https://github.com/cyberbotics/webots/issues/new?assignees=&labels=&template=bug\_report.md](https://github.com/cyberbotics/webots/issues/new?assignees=&labels=&template=bug_report.md)

##### DrVoodoo [Moderator] 02/18/2021 15:05:40
`@Ariel` is the fog node declared before or after the camera in the world file? If it's after, try moving the Fog node to before the camera nodes.

##### Ariel 02/18/2021 19:48:53
I thought the fog node doesn't need to be declared at all?


in the controllers code file if this is what you mean

##### Srivastav\_Udit 02/18/2021 22:38:30
Hi, I wanted to know if there was any sample code available for a robot that has to detect, collect and drop an object at a particular location? I'm trying to work towards a swarm implementation eventually. Any help will be much appreciated. Cheers!

##### Darko Lukić [Cyberbotics] 02/23/2021 16:56:46
`@Simon Steinmann` Regarding the RAM usage. I have just tried add the UR5 in the empty world and uses like 850MB of RAM. It seems the complex models cause the high RAM usage.

##### Simon Steinmann [Moderator] 02/23/2021 16:57:58
340 -> 1.250 -> 520    (ram usage for your described scenario)


however after a while the 520 trickles down to a bit above 400


I used the UR10e back in the day for my RL testing


it sounds like there is lots of room for optimization. A Model that has 50MB at most in model data, should not need 500MB of RAM (at least in my uninformed opinion 😄 )

##### Darko Lukić [Cyberbotics] 02/23/2021 17:05:21
Let's say you have 8 core CPU with 16 thread. To reach the full CPU utilization you should run 8 instances (2 processes per instance, 1 Webots process + 1 controller process). Let's say your simulation uses 1GB of RAM. That means that Webots parallel simulations will use 8GB of RAM in total. Considering that typical 8 core PC is equipped with 16GB of RAM (and RAM is cheap) then running the parallel simulations should be fine?

##### Simon Steinmann [Moderator] 02/23/2021 17:06:38
If it were just the simulation, yes, but running deep neural networks takes lots of resources as well

##### Darko Lukić [Cyberbotics] 02/23/2021 17:08:18
True, in my tests, the RL controllers use like above 1GB of RAM per instance. But then, it is not fair to expect from simulation to use significantly less RAM.

##### Simon Steinmann [Moderator] 02/23/2021 17:08:21
plus, if we were to scale it up on server level, the amounts of RAM get huge. A partner company in our past research project had a 128 core 256 thread server.

##### Darko Lukić [Cyberbotics] 02/23/2021 17:10:07
I am trying to understand how import is to invest time in the RAM/VRAM optimization.

##### Simon Steinmann [Moderator] 02/23/2021 17:11:25
well, same simulation and setup in Pybullet were in the order of 100-300MB RAM

##### Darko Lukić [Cyberbotics] 02/23/2021 17:11:45
Per instance?

##### Simon Steinmann [Moderator] 02/23/2021 17:11:48
yes


i have to check if I find the detailed data


Overall RAM usage during the benchmark run: Webots 1811 MB, PyBullet 927 MB


dont have a more detailed breakdown, but I think around 600-700MB is the portion without the simulation, so Pybullet was 200-300MB


Webots has many great advantages for RL, but this is a major downside.


PyBullet has the advantage, that you only initialize what you need, and the simulation is run on a server, completely seperate from a client, which you can connect or not. This is not super userfriendly of course (where Webots shines). But for more complex training scenarios, this is quite important

##### Darko Lukić [Cyberbotics] 02/23/2021 17:20:53
But quality of the Webots model is much higher (pybullet model in the image)
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/813822706759499826/unknown.png)
%end



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/813822734861598720/unknown.png)
%end

##### Simon Steinmann [Moderator] 02/23/2021 17:21:27
we used the same model


directly urdf in PyBullet, conversion in Webots


the resulting protos is 9.282 KB

##### Darko Lukić [Cyberbotics] 02/23/2021 17:39:10
I have just done some tests with UR5e in an empty world:

- 850MB with initial setup,

- 400MB with all graphics enchantments turned off,

- 177MB with meshes removed, and

- probably less with textures disabled (haven't tried).



Does it make sense that we add a CLI argument to disable meshes and textures?

##### Simon Steinmann [Moderator] 02/23/2021 17:39:37
CLI argument?

##### Darko Lukić [Cyberbotics] 02/23/2021 17:40:05
Yes, something like, `webots --disable-meshes`

##### Simon Steinmann [Moderator] 02/23/2021 17:40:17
being able to run a simulation with all graphics turned off, would be fantastic. There is tons of simulation, where no visual sensors are needed


YES


please! THIS!

##### Darko Lukić [Cyberbotics] 02/23/2021 17:40:35
Nice! 😄


Can you make an issue about this?

##### Simon Steinmann [Moderator] 02/23/2021 17:41:30
you already described it better :p


but I can open it if you want

##### Darko Lukić [Cyberbotics] 02/23/2021 17:41:55
Ahh, ok, I will 🙂

##### Simon Steinmann [Moderator] 02/23/2021 17:42:01
thx ^^


what does this mean?


- 400MB with all graphics enchantments turned off,


I mean

##### Darko Lukić [Cyberbotics] 02/23/2021 17:43:34

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/813828413441441802/unknown.png)
%end

##### Simon Steinmann [Moderator] 02/23/2021 17:43:43
ohhhhhhhhhhh


yeah


perhaps it could be launched with no rendering? And only load those things, when a different mode is selected


but your suggestion is already super great 🙂

##### Darko Lukić [Cyberbotics] 02/23/2021 17:49:04
[https://github.com/cyberbotics/webots/issues/2777](https://github.com/cyberbotics/webots/issues/2777)

##### Simon Steinmann [Moderator] 02/23/2021 17:50:20
would this include the graphics settingsß


?


perhaps "no render" mode can turn those off, if it frees up ram

##### Darko Lukić [Cyberbotics] 02/23/2021 17:50:58
`--no-rendering` still renders meshes for the camera based devices, so it is not the same

##### Simon Steinmann [Moderator] 02/23/2021 17:51:14
ohhh makes sense

##### Darko Lukić [Cyberbotics] 02/23/2021 17:51:25
Add a comment 🙂

##### Simon Steinmann [Moderator] 02/23/2021 17:55:12
no textures and no meshes, does it still render? just with the bounding objects?

##### Darko Lukić [Cyberbotics] 02/23/2021 17:56:48
Yes

##### Simon Steinmann [Moderator] 02/23/2021 17:57:16
thx

##### Srivastav\_Udit 02/24/2021 14:06:04
Is there a way to detect an object only when it is in the middle of the field of view?

##### pnaraltnsk 02/24/2021 20:15:27
Hi I am trying to set a specific rotation to my humanoid robot but my robot gets into weird positions. For example, I want it to rotate around z-axis so I set [0,0,1, rad] but it rotates into weird positions. Could you please help me with this problem? I am using supervisor functions to rotate.

##### Darko Lukić [Cyberbotics] 02/25/2021 07:57:36
Hello, note that Webots used to use the NUE coordinate system by default:

[https://cyberbotics.com/doc/reference/worldinfo#worldinfo](https://cyberbotics.com/doc/reference/worldinfo#worldinfo)

That means that the Y axis is up. 



Could you share the code snippet that you use for the rotation?

##### pnaraltnsk 02/25/2021 11:24:35
I am trying to give the rotation angle in rad as a parameter expecting the robot to turn in that rotation according to the given angle. But it doesn't work.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/814457818715127878/unknown.png)
%end


Here the y-axis is in green and z-axis is in blue and that's why I was trying to rotate it in z-axis.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/814458435525410826/unknown.png)
%end

##### Simon Steinmann [Moderator] 02/25/2021 13:21:47
what is happening instead? Is the field changing? Or is it just not behaving as expected?

##### Darko Lukić [Cyberbotics] 02/25/2021 13:22:37
Also, what is the initial rotation of the robot?

##### bkpcoding 02/25/2021 17:22:37
Hi, you can try subscribe to the logical camera and use the coordinates to fit some range according to your speed to stop in middle, something like -0.5<camera.pose.position.x<0.5

##### pnaraltnsk 02/25/2021 20:00:04
It is not behaving as expected and the initial rotation of the robot is 1,0,0,-1.57

##### Darko Lukić [Cyberbotics] 02/25/2021 20:02:15
So your robot is already rotated around the X axis by -pi/2. That means, the method you wrote rotates the robot, by pi/2 around the X axis + `rad` around the Z axis. Is that what you want?


To rotate around the Z axis from the initial rotation you have to:

- take the rotation,

- convert the rotation to a rotation matrix or quaternion,

- multiply by a rotation matrix/quaternion that represents desired rotation around Z axis, and 

- convert back to the angle-axis representation.

##### Simon Steinmann [Moderator] 02/25/2021 22:22:13
I highly recommend this library for the conversions: [https://matthew-brett.github.io/transforms3d/](https://matthew-brett.github.io/transforms3d/)

##### Gonçalo Carrola Silva 02/26/2021 11:20:21
Hi everyone!!! I'm totally new to using WEBOTS so I have a simple (I hope) problem of development of my robot. I want to use inverse kinematics on PR2 robot (it has to be PR2) to move the right hand to a certain position of the world. For that, I tried to use the ikpy library just like in the ABB' IRB 4600/40 example, and then adapting that code to the PR2 robot, but it seems that it does not work. What do you guys think I am doing wrong? Is there any other way that you suggest to implement an inverse kinematics solution for the PR2 robot using Python?

##### Simon Steinmann [Moderator] 02/26/2021 11:21:10
[https://github.com/cyberbotics/pyikfast](https://github.com/cyberbotics/pyikfast)


this will be the best solution


once you generated your solvers, I can help you out implementing it


or you use ROS and MoveIt

##### Gonçalo Carrola Silva 02/26/2021 11:31:48
Simon, I do not know what to replace in [base\_link], [effector] and [module\_extension] parameters on the READ\_ME file. This is my udrf file:



> **Attachment**: [Pr2.urdf](https://cdn.discordapp.com/attachments/565155651395780609/814822069497888778/Pr2.urdf)


Note that I am using Windows, so I do not use ROS

##### Simon Steinmann [Moderator] 02/26/2021 12:05:23
hmm this file looks wrong


how did you extract it?


just right click on the robot, export and then select .urdf

##### Gonçalo Carrola Silva 02/26/2021 12:08:24
Yes, I did that, but the only option to save the file is .wbo. I think that is the problem

##### Simon Steinmann [Moderator] 02/26/2021 12:09:05
what version of webots do you use?


you need R2021a

##### Gonçalo Carrola Silva 02/26/2021 12:09:46
I think mine is R2020b

##### Simon Steinmann [Moderator] 02/26/2021 12:09:55
you gotta update that


in the meantime, this is the extracted file
> **Attachment**: [Pr2.urdf](https://cdn.discordapp.com/attachments/565155651395780609/814831657579839518/Pr2.urdf)


okay, the pr2 arm has 7 DOF. Ikfast only works with 6


so there has to be an inactive link

##### Gonçalo Carrola Silva 02/26/2021 12:17:57
So, in [base\_link] I put base\_link, in [effector] I put base\_footprint?

##### Simon Steinmann [Moderator] 02/26/2021 12:18:17
base\_link = "torso\_lift\_link"

effector = "r\_wrist\_flex\_link"


I think


module extension do "\_pr2"


see if that works


`@Darko Lukić` we should look into enabling options for DOF != 6


ikfast has options to set joints as inactive


or compile different solvers for DOF = 5 or 4

##### Gonçalo Carrola Silva 02/26/2021 12:28:17

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/814836233606922240/unknown.png)
%end

##### Simon Steinmann [Moderator] 02/26/2021 12:29:10
you need to install docker

##### Darko Lukić [Cyberbotics] 02/26/2021 13:17:56
I have never tried it, I believe it should fallback to avoid looking in the database and try to find a solution by itself. However, if it is not the case then we have to fix.

##### Gonçalo Carrola Silva 02/26/2021 14:33:36
I already installed docker, now the error that appears is this:



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/814867787960418324/unknown.png)
%end

##### Ginzo1 02/26/2021 14:56:57
Hello guys! I am new to webots and for the project I am working we require the robot to receive input arguments before the start of the simulation. Is there a way to receive those arguments before the start of the simulation? The robot has to receive predefined coordinates and move towards them.

##### DDaniel [Cyberbotics] 02/26/2021 15:05:09
`@Ginzo1` you can use a supervisor to send specific commands to each robot using emitter/receivers.


you can use file > open sample world > samples > curriculum > advanced\_genetic\_algorithm as reference on how to exchange info

##### Simon Steinmann [Moderator] 02/26/2021 15:13:07
`@Gonçalo Carrola Silva` make sure docker is running

##### Gonçalo Carrola Silva 02/26/2021 15:15:57
I don't know why docker is not connecting. The message says that the docker daemon is not running but I don't know why

##### Simon Steinmann [Moderator] 02/26/2021 15:16:12
did you launch docker? you're on windows right?

##### Gonçalo Carrola Silva 02/26/2021 15:16:30
I'm on Windows yes

##### Simon Steinmann [Moderator] 02/26/2021 15:16:53
make sure docker desktop is launched

##### Gonçalo Carrola Silva 02/26/2021 15:17:20

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/814878777585827840/unknown.png)
%end


I think that is. But I tried to run that container and the message error is the same

##### Simon Steinmann [Moderator] 02/26/2021 15:18:49
when docker is running, you should be able to open windows power shell


and then use the commands on the github


ofc you have to navigate to the correct directory first

##### Gonçalo Carrola Silva 02/26/2021 15:20:54
But the directory isn't the one where the .urdf file is?

##### Simon Steinmann [Moderator] 02/26/2021 15:21:16
you have to navigate there


in your folder press "ctrl + L", copy the path


then in the powershell:

cd "<paste path>"

##### Chernayaten 02/26/2021 15:23:32
I believe there is an arguments field or something similar for exactly that reason. Could you check the fields on your robot Node?

##### Gonçalo Carrola Silva 02/26/2021 15:27:57
The directory where the urdf file is: "C:\Users\gcarr> cd OneDrive\Ambiente de Trabalho\5º ano\Dissertação\WEBOTS>". I cannot put this on powershell because it has spaces in it. But I can navigate to the folder by the command line

##### Simon Steinmann [Moderator] 02/26/2021 15:28:13
you put it inside of quotation marks


cd ""


the path inbetween

##### Gonçalo Carrola Silva 02/26/2021 15:31:13

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/814882271290195998/unknown.png)
%end


I don't know what is happening


The first sentence of the error message is: "Invalid characters on the way"

##### Simon Steinmann [Moderator] 02/26/2021 15:32:26
try a path with no special characters


peprhaps just your desktop or standard document folder

##### Gonçalo Carrola Silva 02/26/2021 15:34:20
Ok, I am there. I will try to run that command on the github


Nope, same error

##### Simon Steinmann [Moderator] 02/26/2021 15:36:28
try running it as administrator

##### Gonçalo Carrola Silva 02/26/2021 15:36:48

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/814883676290285608/unknown.png)
%end

##### Simon Steinmann [Moderator] 02/26/2021 15:36:50
and I dont know what your path or your characters are


is it running?

##### Gonçalo Carrola Silva 02/26/2021 15:37:38
I don't think so


I think the problem is only in the docker. I cannot run docker\_machine either, it seems that it is not installed

##### Simon Steinmann [Moderator] 02/26/2021 15:39:29
I thought you installed docker?


[https://docs.docker.com/docker-for-windows/install/](https://docs.docker.com/docker-for-windows/install/)


pr2 is not the best candidate for this all. I had to heavily modify the urdf. I'm compiling a solver right now


use this urdf
> **Attachment**: [robot.urdf](https://cdn.discordapp.com/attachments/565155651395780609/814885845462548510/robot.urdf)


`docker run -v ${PWD}:/output cyberbotics/pyikfast torso_lift_link solid_363 _pr2` and this command

##### Gonçalo Carrola Silva 02/26/2021 15:50:36

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/814887150033305660/unknown.png)
%end


I already made all these steps to install docker, except step 5. Maybe the fact that my user is not the administrator is the reason that the docker is not running



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/814887572685717544/unknown.png)
%end


This is the message error when I run powershell as an administrator

##### Simon Steinmann [Moderator] 02/26/2021 15:52:47
oh yeah, you should run the shell as admin

##### Gonçalo Carrola Silva 02/26/2021 15:53:40
Yes, but running as admin it appears this error


`@Simon Steinmann` do I need to install docker toolbox?


I think I finally can run docker


The problem was that it misses the WSL2 container



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/814896647942832189/unknown.png)
%end


I am really sorry to be such a noob XD. I run the docker, and after the installation of many packages it appears this:



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/814921762495266896/unknown.png)
%end


I don't know if the reason was a bad introduction of the 3 arguments of base\_link, etc... or if it is something else


Then, I follow the rest of the github guidance, put "pip3 install . " command and then this appears:



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/814922508766543873/unknown.png)
%end


Although maybe both errors are correlated


Yet about yesterday, I think I made some progress. I finally can run docker but it seems that the file ikfast\_robot.cpp does not exist...



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/815203976470331392/unknown.png)
%end

##### Simon Steinmann [Moderator] 02/27/2021 12:50:21
you need python3 i'm pretty sure

##### Gonçalo Carrola Silva 02/27/2021 12:50:38
I have python3

##### Simon Steinmann [Moderator] 02/27/2021 12:50:43
can you show the command you entered?


i'm currently rerunning everything again. I had troubles doing the pip install afterwards

##### Gonçalo Carrola Silva 02/27/2021 12:51:35
docker run -v ${PWD}:/output cyberbotics/pyikfast torso\_lift\_link solid\_363 \_pr2

##### Simon Steinmann [Moderator] 02/27/2021 12:52:02
you were in the correct directory?

##### Gonçalo Carrola Silva 02/27/2021 12:52:16
yes. Absolutely sure


In the same directory as the urdf file

##### Simon Steinmann [Moderator] 02/27/2021 12:53:10
you have wsl2 installed?


and can you show the entire console output?

##### Gonçalo Carrola Silva 02/27/2021 12:53:44
I can't. It's too big

##### Simon Steinmann [Moderator] 02/27/2021 12:53:52
you can export as .txt file


oh I just got the same thing

##### Gonçalo Carrola Silva 02/27/2021 12:54:31
How?


Maybe is something missing in the urdf file?

##### Simon Steinmann [Moderator] 02/27/2021 13:02:03
`RuntimeError: maximum recursion depth exceeded`


that is the real issue


compiling the solver failed


I ran into that in the past... was a while ago though

##### Gonçalo Carrola Silva 02/27/2021 13:18:03
So, do you know an alternative way to do kinematics with Python for this robot or do you think that the best way is to define it manually? But doing manually maybe the problem maintains, because if I understand it correctly the real challenge is the urdf part.

##### Simon Steinmann [Moderator] 02/27/2021 13:19:24
inverse kinematics is not easy


pr2 is a very complicated robot


ROS and MoveIt are the most refined options


there is no good standalone solutions as far as I know. At least not in python


you could try ikpy. Perhaps it handles 7DOF somewhat well, but I always found it sluggish and difficult to work with in the past. But that was on 6DOF bots

##### row 02/27/2021 18:40:24
I just tried to run webots on a M1 MacBook Air. It runs flawlessly.

##### Krish 02/27/2021 18:45:03
Real or sarcastic?

##### row 02/27/2021 18:45:22
Real


I got a refurbished M1 Macbook Air base model

##### Krish 02/27/2021 18:45:40
Wow, Apple is catching up.


The 2 years promise would turn one and a half I guess.

##### row 02/27/2021 18:46:06
It's crazy because I think webot is translated in rosetta. it's not even running on native arm

##### Krish 02/27/2021 18:46:36
Ahh, really?

##### row 02/27/2021 18:47:03
Yeah.. I am gonna do more testings once I have the time. But it's mind blowing.


Hopefully we will see a native arm built in the future

##### Krish 02/27/2021 18:49:40
Tim Cook OP 😆

##### Whizbuzzer 02/27/2021 21:10:39
> Tim Cook OP 😆

`@Krish`  

🤣

##### Olivier Michel [Cyberbotics] 02/27/2021 22:41:32
Yes, this is in our plans. Would you be willing to beta test a native M1 version of Webots?

##### row 02/28/2021 01:18:23
`@Olivier Michel` Sure. 👍 Let me know how I can help

##### Ginzo1 02/28/2021 20:01:54
Hello everyone! I am sorry for the newbie question but I do not seem to understand how to do it. I've built a robot and I need it to go to a specific box, however, I do not know how to get the position of the box in the world. I've read about using the Supervisor but it does not seem to work properly. What would be the best way to get the coordinates of an object in the world? Also, is the origin (0,0,0) at the centre of my floor node? If it is not, how do I find where the origin is? Thanks!

##### Chernayaten 02/28/2021 20:04:07
The simplest way to find where your 0,0,0 is would be to set your robot (or any object's position) to those coordinates (via the Scene Tree). Similarly, if this is not an issue for you, you can hard code your object's coordinates in your code

## March

##### Stefania Pedrazzi [Cyberbotics] 03/01/2021 06:59:16
As you already read, the simplest and best way to get the coordinates of an object in the world is to use the Supervisor API. Here you can find some documentation: [https://www.cyberbotics.com/doc/doc/supervisor-programming](https://www.cyberbotics.com/doc/doc/supervisor-programming)

##### h.sciascia 03/01/2021 09:06:20
Hello ! How to set min and max motor limits to a negative value ?

##### Bitbots\_Jasper [Moderator] 03/01/2021 09:08:45
normally you should be able to just set the values in the Motor node in the PROTO file of your robot. The fields are described in more detail here: [https://cyberbotics.com/doc/reference/motor#motor-limits](https://cyberbotics.com/doc/reference/motor#motor-limits)


make sure that minPosition < maxPosition

##### h.sciascia 03/01/2021 09:25:46
I want a min at -180° and max at -45° , so minPosition<maxPosition


But I have the error : WARNING: DEF Robot3DOF Robot > HingeJoint > DEF axe0Solid Solid > HingeJoint > RotationalMotor: Invalid 'maxPosition' changed to 0. The value should be 0 or greater.


Oh sorry i'm in the wrong channel

##### Bitbots\_Jasper [Moderator] 03/01/2021 09:30:42
this is not really expected behavior but you can rotate your motor by 45° before to work around this. If you give me your proto I can quickly calculate the correct rotation

##### Stefania Pedrazzi [Cyberbotics] 03/01/2021 09:30:49
You should first modify the `minPosition` field, then modify the current joint position (probably set to 0) so that it will be in the new position range [-180°, -45°], and then modify the `maxPosition`. Otherwise if you don't modify the joint position, Webots won't let you set a range that doesn't contain the current joint position.

##### Bitbots\_Jasper [Moderator] 03/01/2021 09:31:38
I think this is a better solution 😆

##### h.sciascia 03/01/2021 09:33:14
Ok I will try to make some conversion, thank you 👍 and sorry for the wrong channel

##### Olivier Michel [Cyberbotics] 03/01/2021 09:36:32
Can you post a message to [https://github.com/cyberbotics/webots/discussions/2574](https://github.com/cyberbotics/webots/discussions/2574) mentioning that you have a M1 machine and are ready to make some tests? We will contact you be replying there (and you will be notified) when we have a beta version to test. Thank you!

##### Bitbots\_Jasper [Moderator] 03/01/2021 09:39:19
but only changing the position in the HingeJoint does not change the rotation of the end point. I just says what is the position of the joint at the pose you define in your proto. For that you need to change the roation field in the Node defined as the endPoint of your HingeJoint

##### h.sciascia 03/01/2021 09:42:25
Yes I did understood that I have to modify the "initial" position of the motor in my proto/urdf 🙂 Sorry for my english

##### row 03/02/2021 12:14:01
`@Olivier Michel` will do. Thanks! 👍

##### Srivastav\_Udit 03/03/2021 03:50:10
Hi! Has anyone worked with A* path planning for robots? I'm trying to understand how I can implement the algorithm and would really appreciate some guidance.

##### Darko Lukić [Cyberbotics] 03/03/2021 07:52:42
Hello `@Srivastav_Udit`. We have just created a new channel <#816577965285441567> for general robotics questions that are not strictly related to Webots. Feel free to post the question there 🙂

##### row 03/03/2021 21:10:27
`@Srivastav_Udit` There is an open source project PythonRobotics ([https://github.com/AtsushiSakai/PythonRobotics](https://github.com/AtsushiSakai/PythonRobotics)) and it implements many robotics related algorithms, including A*. Everything is written in Python so it's very easy to read and implement a study case.

##### cindy 03/06/2021 23:01:42
Hi I have a small coding issue that I can't figure out from the documentation alone


`LightSensor *light_sensor = robot->getLightSensor("TEPT4400");` gives 'error: 'light\_sensor' does not name a type'


though LightSensor is clearly the class name

##### Stefania Pedrazzi [Cyberbotics] 03/08/2021 07:03:20
Hi, are you programming in C++?

Did you include the `<webots/LightSensor.hpp>` header in your controller?

`#include <webots/LightSensor.hpp>`

##### cindy 03/08/2021 13:06:20
yes

##### Stefania Pedrazzi [Cyberbotics] 03/08/2021 13:09:52
Then, it should work. Are you sure that it is the line you posted that throws the error?

##### cindy 03/08/2021 13:10:19

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/818470690239021116/unknown.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 03/08/2021 13:11:26
Did you check that `light_sensor` is not NULL and that the device name is correct?

##### cindy 03/08/2021 13:12:08
Oh not yet. It might be a naming issue yeah


No, it shouldn't be null, I've checked the proto file of the robot

##### Stefania Pedrazzi [Cyberbotics] 03/08/2021 13:19:25
What I would suggest is to simplify it and remove everything else that it is not strictly needed (additional include statements, code, etc). Keeping only the LightSensor code you should be able to identify where does the error come from. Also you should check that in your Makefile the `USE_C_API` is not set to true.

##### cindy 03/08/2021 13:20:59
Aha that is the problem


It is set to true


Would changing it manually work?

##### Stefania Pedrazzi [Cyberbotics] 03/08/2021 13:22:55
Yes, you should comment or remove the line `USE_C_API` line and then recompile the controller.


Note that this variable tells the Makefile compilation system that in your C++ controller you will use the Webots C API. So if you are going to use Webots C++ API it seems there is no need to set it.

##### cindy 03/08/2021 13:36:12
i've deleted it completely and the issue is persisting

##### Simon Steinmann [Moderator] 03/08/2021 23:25:40
<@&568329906048598039> Is there a way to SET the values of a sensor? This would be very useful, allowing external sensors to be connected and to feed the simulation.  On a separate note: Is there a way to directly define an MF field, without having to set every item index by index. Every set call takes a certain time. When filling an array with hundreds of values, this is incredibly slow.

##### Olivier Michel [Cyberbotics] 03/09/2021 07:17:35
Yes, this is possible from the remote control library:

[https://cyberbotics.com/doc/guide/controller-plugin#remote-control-plugin](https://cyberbotics.com/doc/guide/controller-plugin#remote-control-plugin)

[https://github.com/cyberbotics/webots/blob/master/include/controller/c/webots/remote\_control.h](https://github.com/cyberbotics/webots/blob/master/include/controller/c/webots/remote_control.h)

The original purpose of this is to be able to switch a controller between the simulation and a remote controlled real robot (and display the sensor values of the remote controlled robot in Webots as it is was simulated sensors).


Regarding setting multiple MF fields in a row, it is not possible. If you need to set large amount of MF fields, it is probably better to import the whole node as a string, which includes all the MF fields.

##### Simon Steinmann [Moderator] 03/09/2021 20:25:28
`@Olivier Michel`  thanks, I will take a look


There is  a weird issue, where the pointcloud of a lidar is only shown, if the lidar is closer.
> **Attachment**: [lidar\_issue.mp4](https://cdn.discordapp.com/attachments/565155651395780609/818943268196450314/lidar_issue.mp4)


I figured it out: As soon as a ray has no collision (no obstacle within max range), NO point is shown at all. Even though there are valid points to be drawn

##### Olivier Michel [Cyberbotics] 03/09/2021 20:46:35
That's a known bug which was fixed recently.

##### Simon Steinmann [Moderator] 03/09/2021 20:46:56
Oh good 🙂

##### Olivier Michel [Cyberbotics] 03/09/2021 20:47:20
Did you try a nightly build?

##### Simon Steinmann [Moderator] 03/09/2021 20:47:41
no, official 2021a I think

##### Olivier Michel [Cyberbotics] 03/09/2021 20:47:58
It should be fixed in all nightly builds.

##### Simon Steinmann [Moderator] 03/09/2021 20:48:18
gonna check it out later


`@Darko Lukić` For working with ros2 and moveit, which version of Webots do I use? The nightly build, or the automatic .ros install (15. Dezember)

##### Darko Lukić [Cyberbotics] 03/09/2021 22:52:42
Any should work

##### Ginzo1 03/10/2021 23:15:11
Hello everyone! I need to draw lines on the floor that my robot will follow for a school project however I do not seem to find a proper way to do it. Could someone give me any ideas on how to do it? 🙂

##### KajalGada 03/11/2021 01:05:37
`@Ginzo1` check out this tutorial by DrakerDGRobotics on how he made his track using TinkerCAD. You can design your own style based on that.



[https://youtu.be/XUSCD7aYtQ8](https://youtu.be/XUSCD7aYtQ8)

##### Srivastav\_Udit 03/23/2021 07:12:09
Hi! I need to replace this line wb\_differential\_wheels\_set\_encoders(0, 0); with code that is accepted for Position Sensors by Webots. Are there any recommendations?

##### Stefania Pedrazzi [Cyberbotics] 03/24/2021 07:43:54
Hi, the `DifferentialWheels` node is replaced by `Motor` and `PositionSensor` nodes. `PositionSensor` just returns the position value of the motor. If you want to set the encoders, i.e., motor position, you should use the `Motor` API:

[https://www.cyberbotics.com/doc/reference/motor](https://www.cyberbotics.com/doc/reference/motor)

Depending if you want to control the motor using position directly, velocity or force/torque there are different functions to use:

* [https://www.cyberbotics.com/doc/reference/motor#position-control](https://www.cyberbotics.com/doc/reference/motor#position-control)

* [https://www.cyberbotics.com/doc/reference/motor#velocity-control](https://www.cyberbotics.com/doc/reference/motor#velocity-control)

* [https://www.cyberbotics.com/doc/reference/motor#force-and-torque-control](https://www.cyberbotics.com/doc/reference/motor#force-and-torque-control)

When converting the default Webots worlds using `DifferentialWheels` node we used the *velocity* control and thus replaced the `wb_differential_wheels_set_encoders(0, 0)` function with:

```
wb_motor_set_position(left_motor, INFINITY);
wb_motor_set_position(right_motor, INFINITY);
wb_motor_set_velocity(left_motor, 0.0);
wb_motor_set_velocity(right_motor, 0.0);
```

##### Ginzo1 03/25/2021 18:22:47
Hello everyone! I need to create a spraying device for my simulation - the form and size do not matter so I wondered if something similar's been done before and it is available to use. Also any suggestions for the implementation are more than welcome 🙂

##### Darko Lukić [Cyberbotics] 03/25/2021 18:52:23
You can use the Pen node:

[https://cyberbotics.com/doc/reference/pen](https://cyberbotics.com/doc/reference/pen)

##### Ginzo1 03/25/2021 19:33:18
Thank you!

##### Vial 03/25/2021 23:35:00
Hi, I'm trying to implement a new endpoint for lidars in remote control library but I have some questions about the API, I thought it would be much easier for everyone with screenshots. Hence the pdf file, it's mostly screenshots.

Thanks by advance for considering this question
> **Attachment**: [Question-remote-api.pdf](https://cdn.discordapp.com/attachments/565155651395780609/824788493434617886/Question-remote-api.pdf)

##### Darko Lukić [Cyberbotics] 03/26/2021 08:08:39
Hello Vial, It would be very nice to extend the remote library to support Lidars!  



Hopefully I managed to understand details of your problem.



> Why is c→image→data remains null in remote

> control mode (figure 4), where is the memory allocated

> for this field or should I calloc it myself

You should populate the Lidar image in your plugin:

```c
const unsigned char *lidar_data_from_robot = malloc(...);
wbr_lidar_set_image(camera->tag(), lidar_data_from_robot);
```



I would strongly advise you to open a PR and put the everything inside, so we can easily review it. Just mark it is as a `Draft` and keep iterating until you have a question or it is ready to be merged.

##### Spur 03/27/2021 05:05:26
dumb question but does anyone know what the lidar point cloud returns and how to access it?

##### Darko Lukić [Cyberbotics] 03/27/2021 13:58:42
It should be clear from the documentation:

[https://cyberbotics.com/doc/reference/lidar#wb\_lidar\_get\_point\_cloud](https://cyberbotics.com/doc/reference/lidar#wb_lidar_get_point_cloud)



Let us know if there is something unclear.

##### ouur 03/28/2021 15:49:44
Hello everyone. im using python and trying to get self node but it gives me error. Any idea?

TypeError: in method 'Supervisor\_getSelf', argument 1 of type 'webots::Supervisor const *'

##### Simon Steinmann [Moderator] 03/28/2021 18:30:49
`@ouur` show us your code

##### Srivastav\_Udit 03/30/2021 03:22:47
Thank you so much!

##### Simon Steinmann [Moderator] 03/30/2021 19:22:27
I found a very strange issue. The wheel seems to move the robot in the same direction, regardless of which way it is spinning
> **Attachment**: [simple\_crashderby\_1.mp4](https://cdn.discordapp.com/attachments/565155651395780609/826536875283447818/simple_crashderby_1.mp4)

##### Darko Lukić [Cyberbotics] 03/30/2021 19:26:39
What's that? Which world?

## April

##### Krish 04/01/2021 11:48:12
Is the other wheel pivoted?

##### Simon Steinmann [Moderator] 04/01/2021 21:13:50
it's due to kinematics mode (no physics). It works for velocity control, but not position control

##### Spur 04/02/2021 06:37:26
does getRecognitionObjects() return a pointer to an array of objects or just to one? and how do I access the id of one of those object?

##### Darko Lukić [Cyberbotics] 04/03/2021 13:59:20
See this example:

[https://github.com/cyberbotics/webots/blob/ab8b93ff30523825a8193327db756f44f12d390c/projects/samples/devices/controllers/camera\_recognition/camera\_recognition.c#L60-L75](https://github.com/cyberbotics/webots/blob/ab8b93ff30523825a8193327db756f44f12d390c/projects/samples/devices/controllers/camera_recognition/camera_recognition.c#L60-L75)

##### Simon Steinmann [Moderator] 04/04/2021 01:21:49
<@&568329906048598039> The webots docs directory is huge. I know that transparency is nice with the png files, but they are only included in white documents. These images could be compressed to jpg files, reducing the file size by about 10-20 times.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/828076864353730600/unknown.png)
%end

##### DDaniel [Cyberbotics] 04/04/2021 07:27:53
`@Simon Steinmann` it's in the works, the offline documentation might be removed entirely by default (with the option of downloading it if needed) among other things. [https://github.com/cyberbotics/webots/pull/2787](https://github.com/cyberbotics/webots/pull/2787)

##### Spur 04/08/2021 01:56:50
Does anyone know how the  'position' field of a Camera Recognition Object works? it says it has 3 values but I'm unsure what they represent

##### Olivier Michel [Cyberbotics] 04/08/2021 06:03:46
XYZ coordinates?

##### DrVoodoo [Moderator] 04/08/2021 16:06:45
I'm having some grief on a world a ENU world with an inertial unit


I have the inertial unit aligned as per the docs (x forward, y up, z right)


And I am using getQuaternion() to get the orientation (which I assume is returning in x,y,z,w order)


Now the robot is on a flat(ish) surface so that's giving me ( 0.000950936, -0.00466466, 0.980983, -0.194035 ) for example


No wait, I think I figured this out. I'm trying to present the data in PCL which is using NUE still


As always, you bang your head on the problem for an hour and realise the issue as soon as you start explaining it.

##### Bitbots\_Jasper [Moderator] 04/08/2021 16:45:20
have you tried the rubber ducky method ([https://en.wikipedia.org/wiki/Rubber\_duck\_debugging](https://en.wikipedia.org/wiki/Rubber_duck_debugging)) ? 😋

##### Spur 04/09/2021 00:12:04
oh ok thanks, do you know what the orientation field values represent, theres 4 so im very unsure

##### Simon Steinmann [Moderator] 04/09/2021 00:24:23
axis angles usually

##### Spur 04/09/2021 06:26:06
wont there only be 3 though? xy xz yz ?

##### Bitbots\_Jasper [Moderator] 04/09/2021 06:29:30
In the axis angle notation used in webots  the first three values specify the rotation axis and the fourth specifies the rotation in radians

##### Spur 04/09/2021 06:32:35
sorry i'm a bit confused, how do the three values describe the axis, isnt an axis by definition only 1 value, ie x axis y axis or z axis

##### Bitbots\_Jasper [Moderator] 04/09/2021 06:36:12
It is not necessarily the x y or z axis. You can think of the axis being specified as a line though the origin of the coordinate system and a point on the unit sphere specified by the three coordinates. If the point is (1,0,0) it is simply a rotation around the x axis but any axis is possible

##### Spur 04/09/2021 06:38:19
oh ok, forgive my ignorance but whats the point of the rotation value then? (4th value)

##### Drake P. 04/09/2021 06:53:55
The 4th value is the roll around the specified axis to my understanding

##### Spur 04/09/2021 06:54:21
oh gotcha thanks

##### Bitbots\_Jasper [Moderator] 04/09/2021 07:16:05
Yes, thanks `@Drake P.` for explaining

##### Gotcha97 04/09/2021 19:29:53
Is it possible to add a speaker device to an existing robot template (in my case a TIAGo Titanium)?

##### Olivier Michel [Cyberbotics] 04/09/2021 20:49:33
Yes, you should be able to insert it in one of the extension slots of the TIAGo proto.

##### Westin 04/14/2021 16:39:28
I found a typo.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/831931677990191204/unknown.png)
%end

##### Simon Steinmann [Moderator] 04/14/2021 19:51:46
<@&568329906048598039> [https://github.com/cyberbotics/webots/blob/develop/src/webots/nodes/WbLidar.cpp#L728](https://github.com/cyberbotics/webots/blob/develop/src/webots/nodes/WbLidar.cpp#L728) found it. it's supposed to be 'using' right?


it comes up several times. Doing a search in VC for webots, brings up more instances. Mostly in translation files.

##### Stefania Pedrazzi [Cyberbotics] 04/15/2021 06:09:49
Thank you! We will fix it immediately.

##### aja\_discord 04/15/2021 08:17:34
Hi Guys checkout my first own project on webots where I have made a self balancing robot which learns to balance itself using a genetic algorithm.Let me know your ideas. Thanks..LINK - [https://www.youtube.com/watch?v=l9BZQ9E5y6A](https://www.youtube.com/watch?v=l9BZQ9E5y6A)

##### Troy 04/20/2021 00:48:34
Hi sorry to bother again, we can use camera to recognize object, can we directly  get distance feedback between the camera and the object?

##### Olivier Michel [Cyberbotics] 04/20/2021 05:34:27
Yes, by adding a DistanceSensor node.

##### Troy 04/20/2021 21:18:50
Yeah, but what if the distance sensor is not pointing directly to the object?


for example, the camera on the robot recognizes an object on top right of the image, but the distance sensor is pointing in the middle, how can I get the distance feedback of the object?

##### Olivier Michel [Cyberbotics] 04/21/2021 06:14:42
Then, you should use a Lidar with the same position, orientation, field of view, resolution, etc. as the camera.

##### B0Bhead 04/22/2021 12:48:39
`@Olivier Michel` Thank you for linking that Master Thesis paper. I am also currently working on my Bachelor Thesis, doing research on SLAM. Is there a way to get in contact with you guys? Can I DM you on Discord?

##### Olivier Michel [Cyberbotics] 04/22/2021 12:56:28
For technical support, you should ask on the different Discord channel. For personal messages, yes, feel free to contact me in DM.

##### Simon Steinmann [Moderator] 04/22/2021 22:06:53
does anyone here have experience compiling webots for profiling? make has the `profile` and `debug` options. This page says i have to compile in debug [https://github.com/cyberbotics/webots/wiki/Valgrind](https://github.com/cyberbotics/webots/wiki/Valgrind). I remember not being able to successfuly use valgrind in the past. But I think the profile option did not exists back then. Any ideas how to proceed?

##### Olivier Michel [Cyberbotics] 04/23/2021 06:23:58
Simply type "make profile" in the `WEBOTS_HOME/src/webots` folder. Then, you should be able to use gprof to analyse the result of a run. See [https://sourceware.org/binutils/docs/gprof/](https://sourceware.org/binutils/docs/gprof/)

##### Bitbots\_Jasper [Moderator] 04/23/2021 13:56:53
The current way of modeling noise for gyros and accelerometers using a lookup table. The noise is given as a percentage of the response value (see [https://cyberbotics.com/doc/reference/distancesensor#lookup-table](https://cyberbotics.com/doc/reference/distancesensor#lookup-table)). In my opinion this does not make much sense as a model of noise for these sensors (opposed to distance sensors for which it does) since it causes that there is very little noise around zero. 

In gazebo using a gaussian around the signal with a fixed standard deviation and an optional bias. 

Is there a way to model the noise in this way in webots as well?

##### Olivier Michel [Cyberbotics] 04/23/2021 14:45:29
Well, if you increase the noise when getting close to the 0 zero value in the lookup table, you will have somehow a constant noise level. But I agree, this is not ideal. If you have any idea to improve this, feel free to open an issue.

##### Ragemor 04/27/2021 22:33:47
ı have different colored objects in my simulation. and i want to move my robots to different colored objects. how can i perform that? basicly a leader follower concept with camera recognition. I try to use objects[].colors[] but i cant made what i want. my english is not pretty good. anyone can help me about that?


this is my simulation. reds first and greens behind them. this is working for a while but if green object robots see another colored robot it stopped moving.
%figure
![webbb.jpg](https://cdn.discordapp.com/attachments/565155651395780609/836732246350233631/webbb.jpg)
%end



> **Attachment**: [code\_greens.txt](https://cdn.discordapp.com/attachments/565155651395780609/836732424238792774/code_greens.txt)

##### Nick R 04/29/2021 21:45:23
Very general question about Webots: was it free for academic use before becoming free/open source in 2019?

##### Olivier Michel [Cyberbotics] 04/30/2021 06:44:30
No. It was not free for academic use. It became open source in December 2018.

##### Nick R 04/30/2021 14:17:27
Thank you, was just wondering.

## May

##### ShuffleWire 05/05/2021 15:47:54
Hello ! 

I'm using gdb to play with webots, and additionally to the -g option, I had to use the -ggdb one. I've read about it that it generate more "gdb friendly" symbol (and there is additional version of this flag). Is it expected or did I miss something ?

##### Olivier Michel [Cyberbotics] 05/05/2021 17:18:52
I believe it is not included in the Makefiles of Webots, but it should be easy to add. What are the exact benefits of this flag?

##### ShuffleWire 05/05/2021 23:08:24
I'm using my IDE to have a nice GUI interface for GDB (Codeblock IDE), which work fine with this flag. Otherwise it work as well, but I don't get any function name in stack listing etc, which make the thing less useful...

It's seems related to Qt, which could cause trouble to GDB "as it" (ie only -g)

##### Olivier Michel [Cyberbotics] 05/06/2021 05:33:27
Did you try to add the `-ggdb` option here: [https://github.com/cyberbotics/webots/blob/master/src/webots/Makefile#L191?](https://github.com/cyberbotics/webots/blob/master/src/webots/Makefile#L191?)

##### ShuffleWire [Moderator] 05/06/2021 11:26:07
I've not, but should make a PR for that ? It's seems to be a so tiny fix...

##### Olivier Michel [Cyberbotics] 05/06/2021 11:26:58
Yes, please, tiny PRs are also very welcome!

##### ShuffleWire [Moderator] 05/06/2021 11:27:37
Allright, I might have some to do, then 🙂

##### Olivier Michel [Cyberbotics] 05/06/2021 11:28:36
You are very welcome.

##### Daemongear 05/17/2021 01:34:31
Hello, so as Simon (many thanks btw) asked, I'm rephrasing my question, maybe someone can help with advice. I want to publish the WeBots joints to the ROS joint\_state\_publisher. I am using the standard, tutorial ROS controller


so far the only options I've found were A) rewrite parts of the standard controller (which is heavily distributed of course) to get the joints directly published to the right topic or B) write my own node (much simpler) to subscribe to the default topics after enabling the motor sensors, and publish them to the joint\_state\_publisher. this would of course induce some delay.


Any other ideas?

##### Darko Lukić [Cyberbotics] 05/17/2021 06:33:06
Hello `@Daemongear`, you can use `ros_control`. As of Webots R2021b it is a part of the `ros` controller. Here is an example:

[https://github.com/cyberbotics/webots\_ros/blob/2bfc812411d82cf82427d0e414be3fc34a04e988/launch/tiago.launch#L18-L20](https://github.com/cyberbotics/webots_ros/blob/2bfc812411d82cf82427d0e414be3fc34a04e988/launch/tiago.launch#L18-L20)

##### Daemongear 05/17/2021 06:35:51
how does that work? do I need a launch file like that where I manually specify the joints, or is that a service I can enable or call?

##### Darko Lukić [Cyberbotics] 05/17/2021 06:38:08
Just set the `controller` parameter to `ros` and `controllerArgs` like this:

[https://github.com/cyberbotics/webots\_ros/blob/2bfc812411d82cf82427d0e414be3fc34a04e988/worlds/tiago.wbt#L539-L545](https://github.com/cyberbotics/webots_ros/blob/2bfc812411d82cf82427d0e414be3fc34a04e988/worlds/tiago.wbt#L539-L545)

It will start `ros_control` and then you follow documentation from `ros_control`:

[http://wiki.ros.org/ros\_control](http://wiki.ros.org/ros_control)

##### Daemongear 05/17/2021 06:42:09
oh that's awesome, I knew there must have been a generic publisher somewhere. I'll try it in a bit, thanks so much for the info!


out of reference-making interest (I'm writing my bachelor project report right now, as it's due today, gg). are you, by any chance, one of the devs of WeBots?

##### Darko Lukić [Cyberbotics] 05/17/2021 06:46:04
Yes 🙂

##### Daemongear 05/17/2021 06:47:03
is it alright if I use your name as a reference in a TAROS paper and my project? (my paper is about a comparison between CoppeliaSim and WeBots, as undergraduate viability, while my project is about a specific humanoid application)

##### Darko Lukić [Cyberbotics] 05/17/2021 06:49:18
Sure, but maybe it is better to use Cyberbotics as a reference

##### Daemongear 05/17/2021 06:50:28
it's more as an argument of "their team is significantly more responsive and community-involved", with the basis that CoppeliaSim has just one active dev who barely responds


good point, I'll ask my mentor to make sure it's appropriate then


R2021b is not published, as far as I can tell? the latest version available I could find is R2021a Revision1

##### DDaniel [Cyberbotics] 05/21/2021 06:17:07
`@Daemongear` 2021b will be release in roughly a month, but there are nightly builds of it available: [https://github.com/cyberbotics/webots/releases](https://github.com/cyberbotics/webots/releases)

##### Daemongear 05/21/2021 06:31:41
I've downloaded the entire webots\_ros and placed it in the default location, just to test the setup, and it doesn't seem to work


oh thanks!

##### Darko Lukić [Cyberbotics] 05/21/2021 06:32:43
The `webots_ros` contains only examples, the actual Webots ROS interface is implemented in Webots

##### Daemongear 05/21/2021 06:34:04
so basically I need the nightly build installed, right?

##### Darko Lukić [Cyberbotics] 05/21/2021 06:34:42
Yes

##### Daemongear 05/21/2021 06:35:45
ok, did that. is it compatible with the older one, or do I need to delete and refresh the ros thingies?

##### Darko Lukić [Cyberbotics] 05/21/2021 06:35:47
You need the Webots nightly build and the develop branch from `webots_ros` repository for examples


You can type:

```
webots --version
```

to see which version is active.

##### Daemongear 05/21/2021 06:37:45
it's the one you've linked before I believe, which is currently what I replaced my webots\_ros files with


the simulation seems to work now, it does publish to a heck of a lot of rostopics, altough it does throw an error "failed to load arm\_controller". but from here I should be able to develop what I needed


ok it does update the tf, but not the joint\_states and robot\_states

##### Darko Lukić [Cyberbotics] 05/21/2021 06:40:17
Do you have `ros_controller` installed?

##### Daemongear 05/21/2021 06:40:36
what does that installation require?

##### Darko Lukić [Cyberbotics] 05/21/2021 06:41:36
```
sudo apt install ros-noetic-ros-controllers
```

##### Daemongear 05/21/2021 06:42:01
same for melodic I assume

##### Darko Lukić [Cyberbotics] 05/21/2021 06:42:08
Yes


`rosdep` should install the required packages automatically

##### Daemongear 05/21/2021 06:43:51
I... umm... get an error I don't understand.



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/845190202107756564/unknown.png)
%end


after I've already installed it with apt


oh wait, that's not what rosdep does, is it *facepalm*

##### Darko Lukić [Cyberbotics] 05/21/2021 06:46:29
`rosdep` resolves dependencies based on `package.xml`, you typically run it when you install a package from source to resolve all system dependencies


Installing `ros_controller` from apt should be enough

##### Daemongear 05/21/2021 06:48:41
I've rerun roslaunch webots\_ros tiago.launch , it no longer gives an error on loading arm\_controller, just a warning for wait\_for\_srvice failed to contact, and an error for moveit\_ros\_move\_group, which I assume is a different package

##### Darko Lukić [Cyberbotics] 05/21/2021 06:50:29
Yes, the `moveit_ros_move_group` is a different package, but it should come with the standard MoveIt installation:

[https://moveit.ros.org/install/](https://moveit.ros.org/install/)

##### Daemongear 05/21/2021 06:53:28
well, it now identifies the new install. still gives of error "failed to find 3d sensor plugin param" and "exception while loading planning adapter plugin" after using planning interface OPML.


this is all fine since I am not at the point where I can use move-it yet anyway


how would I connect this to a standard gui joint\_state\_controller?

##### Darko Lukić [Cyberbotics] 05/21/2021 06:54:45
I am not sure about `exception while loading planning adapter plugin` but the other one is fine as Tiago doesn't have a 3D sensor

##### Daemongear 05/21/2021 06:55:25
that's a tomorrow problem for tomorrow me, as I am a long way from using that

##### Darko Lukić [Cyberbotics] 05/21/2021 06:55:26
Can you echo `joint_states`?

##### Daemongear 05/21/2021 06:57:15
yes, but doesn't give back anything. not sure it should tbh


oh I think I found the issue with that weird error from moveIt. upper in the log it says "could not find parameter robot\_description on parameter server" and suggests it's likely because of a remap of "robot\_description"


and after 2 retries it moves on and throws "semantic description is not specified for the same robot as the URDF"

##### Darko Lukić [Cyberbotics] 05/21/2021 07:00:49
It should pick the `robot_description` from a corresponding topic

##### Daemongear 05/21/2021 07:02:07
robot\_description should be a topic? or sth like robot\_state


(both missing)


ok so... good news and bad news. I've started an RViz display from a launch file, now the rostopic echo returns the current positions from Joint\_state, but they don't update in webots


what's worse is that now using rosservice call set\_position doesn't do anything in the webots window


although it does return success true


the simulation is running, but at 0.00x? is this an error or a new setting??

##### Darko Lukić [Cyberbotics] 05/21/2021 07:09:01
I can reproduce, let me check, I think we have a bug in the develop branch

##### Daemongear 05/21/2021 07:09:31
thank you ^\_^


it's part of the roslaunch package, not the webots version


from my testing, the issue seems to be in the ros controller, disabling it lets the simulation run just fine. maybe a launch service command?


umm I think I may have found the source of the bug. looking in the rosservice list output, all the motor fields are doubled.


I think this throws off an error with the services and blocks everything


never mind, that is a separate error, where closing webots and the process doesn't close the services and they get doubled


name/robot/ services are completely unresponsive, so a failed one is probably blocking the entire node

##### Darko Lukić [Cyberbotics] 05/21/2021 08:16:06
This PR has changed the behavior of the `ros` controller:

[https://github.com/cyberbotics/webots/pull/3040](https://github.com/cyberbotics/webots/pull/3040)



As a temporary fix you can  add one more argument `"--use-sim-time"` here:

[https://github.com/cyberbotics/webots\_ros/blob/2bfc812411d82cf82427d0e414be3fc34a04e988/worlds/tiago.wbt#L544](https://github.com/cyberbotics/webots_ros/blob/2bfc812411d82cf82427d0e414be3fc34a04e988/worlds/tiago.wbt#L544)



I have to verify what is the best long-term solution and to fix it


Let me know whether it works for you

##### Daemongear 05/21/2021 08:28:57
i'll load up the vm rightaway


ok so I have good news and bad news


the good news is that both my sim and tiago do work


the bad news is that I think it automatically requests some data?  tiago seems to be still stuck on not having a robot\_description file, since I'm not sure where that is, while my sim throws continuously errors as "rotational motor: too low requested position" and "too big" of +-pi as compared to 0. so i'm guessing it's also resetting the limits


progress update: (there are 2 services for each motor called "get\_min\_position" and max) which return the proper values, surprisingly enough. I've named some of the joints to track down the issue



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/845220870212288542/unknown.png)
%end

##### Darko Lukić [Cyberbotics] 05/21/2021 08:49:03
I cannot reproduce this, it works fine for me :/

##### Daemongear 05/21/2021 08:49:29
something keeps requesting that position, and I don't know what, since I am not using any controllers and even rviz is closed


I would think so, since I have only 3 motors out of 18 giving this issue. maybe I've exported something wrong last time, I'll keep checking.

##### Darko Lukić [Cyberbotics] 05/21/2021 08:51:18
You are launching the simulation like this?

```
roslaunch webots_ros tiago.launch
```

##### Daemongear 05/21/2021 08:51:21
what could possibly request those positions continuously?


this is not in tiago, tiago works fine

##### Darko Lukić [Cyberbotics] 05/21/2021 08:52:18
Oh, you are running your own robot?

##### Daemongear 05/21/2021 08:52:37
yes, I've updated its files at the same time with tiago

##### Darko Lukić [Cyberbotics] 05/21/2021 08:53:32
You are using ros\_control?

##### Daemongear 05/21/2021 08:53:43
I keep calling from the terminal set\_position as 0.0, but something else is trying to set it weirdly


yes


we can hop on a quick call if you have time


in any case, I've check with both RViz and the proto itself for the limits, they are right, but I don't know where the initial positions are stored?

##### Darko Lukić [Cyberbotics] 05/21/2021 08:55:23
Unfortunately no, it is a part of the Cyberbotics Premier Service

##### Daemongear 05/21/2021 08:55:49
I... have no idea what that is

##### Darko Lukić [Cyberbotics] 05/21/2021 08:56:16
[https://cyberbotics.com/](https://cyberbotics.com/)

Search for `Premier Service`

##### Daemongear 05/21/2021 08:56:56
ah lol niice, that's a really cool feature for the software. I understand

##### Darko Lukić [Cyberbotics] 05/21/2021 08:58:03
If a initial position is close to min/max then the joint may cross the range for a little and that confuses ros\_control

##### Daemongear 05/21/2021 08:58:23
no, it's off by pi/2 or more


and even so, when i call rosservice call /robot/LElbow/set\_position 0.0, it returns true, but the error and rostopic echo still stay it's at 2.1928

##### Darko Lukić [Cyberbotics] 05/21/2021 08:59:22
Also, ros\_control keeps setting joint positions at each timestep, so the issue may be in ros\_control

##### Daemongear 05/21/2021 09:00:05
hmm ok that makes a lot more sense, as it overwrites at each time step. so where does ros\_control have the inputs? or does it require a controller written?


if I publish something to joint\_states, will it take those values maybe>


?

##### Darko Lukić [Cyberbotics] 05/21/2021 09:00:44
If you delete the `"--use-ros-control"` flag it should be fine


If it is properly configured yes, but the joint\_states is correct topic for that

##### Daemongear 05/21/2021 09:01:50
it did solve the issue, yes.


where does ros-control take its initial config from then? that's probably the outdated package I need to modify

##### Darko Lukić [Cyberbotics] 05/21/2021 09:03:11
I am sorry, but this out of scope of the Webots simulation, it is more related to `ros_control`

##### Daemongear 05/21/2021 09:03:26
ooh I think I know. the proto must be generated from my previous description folder, and ros\_control takes the info from the ros package.


I'll update all the packages later and let you know if it solves the issue. it probably very much will


just to make sure I got this right: if I publish to joint\_states, it will update the webots simulation, right?

##### Darko Lukić [Cyberbotics] 05/21/2021 09:30:20
If you use `ros_control` then you the joint control goes through the `joint_trajectory_controller`:

[http://wiki.ros.org/joint\_trajectory\_controller](http://wiki.ros.org/joint_trajectory_controller)

(typical topic name `/command`)

But you have make sure that the controller is properly configured. This is a standard interface for controlling joints utilized by packages like MoveIt.



If you don't use `ros_control` then you can rely on the `set_position` service:

[https://cyberbotics.com/doc/reference/motor?tab-language=ros#wb\_motor\_set\_position](https://cyberbotics.com/doc/reference/motor?tab-language=ros#wb_motor_set_position)

##### Daemongear 05/21/2021 09:41:51
oh ok that clears a lot of things up. However, I still don't understand how to solve my very initial issue: setting up ROS so I can use sliders from RViz (joint\_state\_publisher with gui) to modify both positions in RVIZ and the ones in WeBots.

##### Darko Lukić [Cyberbotics] 05/21/2021 09:52:31
I don't have much experience with that, but the `joint_state_publisher` doesn't sound like something designed for control. As the name suggests, it publishes joint states, so it is not supposed to control them. It is fine for RViz as RViz is designed to visualize robot's state, but not to control a physical or simulated robot.


I believe, the correct way to control joints is through the`trajectory_msgs/JointTrajectory` topic or `control_msgs::FollowJointTrajectoryAction` action. Otherwise, you can a low-level access with the `set_position` service.

##### Daemongear 05/21/2021 09:56:32
I see🤔  thanks for all the great help!

##### Stephen 05/24/2021 00:49:30
Is anyone else having issues downloading SUMO when building webots from source? Is there another mirror somewhere for it?

I am setting up a CI/CD pipeline for our controller for robocup, which requires me to build webots from source on several computers. All of them are having issues downloading SUMO for linux64. When they get to the step where they decompress the tar archive, there is an unexpected EOF character - basically the tar archive hasn't downloaded properly. I have downloaded it manually, and decompressed it myself successfully, but that's really painful when it takes an hour to download sumo at 8 KB/s

##### Simon Steinmann [Moderator] 05/28/2021 21:38:37
<@&568329906048598039> Working with object recognition, I think it could be very useful to expose the "recognitionColors" field in all objects provided by Webots. What do you think? I can create a PR if you agree.

##### AmorFati 05/28/2021 21:39:12
Yeah totally agree.

##### Bitbots\_Jasper [Moderator] 05/29/2021 08:51:23
I think it's a good idea as well.

##### Olivier Michel [Cyberbotics] 05/29/2021 11:19:56
Yes, good idea. Please go ahead.

##### Simon Steinmann [Moderator] 05/29/2021 23:34:26
[https://github.com/cyberbotics/webots/pull/3108](https://github.com/cyberbotics/webots/pull/3108)


created the PR and did all the changes.

##### mmoralesp 05/31/2021 17:06:26
Does anybody know how to output in ros2 the odometry info?

##### mclzc 05/31/2021 20:31:30
Hi, regarding the urdf2proto conversion:

[https://github.com/cyberbotics/urdf2webots/blob/0136988774e7dde8d7d476c1b5e584a92d8f76df/urdf2webots/parserURDF.py#L991](https://github.com/cyberbotics/urdf2webots/blob/0136988774e7dde8d7d476c1b5e584a92d8f76df/urdf2webots/parserURDF.py#L991)



You are looking for something that starts with `libgazebo_ros_imu` which would match the `GazeboRosImu` based on the `ModelPlugin` but would as well match a `libgazebo_ros_imu_sensor`, based on the `SensorPlugin`. Is that intentional? Or did you only mean to match the first one. I don't really know the difference between both.

## June

##### Darko Lukić [Cyberbotics] 06/01/2021 07:15:43
You can get odometry data if you inherit the `WebotsDifferentialDriveNode` class. Check the e-puck example:

[https://github.com/cyberbotics/webots\_ros2/blob/55043633c37b6afb5b2c80b768ceb0aaa8b7afe7/webots\_ros2\_epuck/webots\_ros2\_epuck/driver.py#L64](https://github.com/cyberbotics/webots_ros2/blob/55043633c37b6afb5b2c80b768ceb0aaa8b7afe7/webots_ros2_epuck/webots_ros2_epuck/driver.py#L64)



We have recently released an alpha version of `ros2_control` integration, so you can use `DiffDriverController` or any other off-the-shelf controller that can output odometry. Check the TurtleBot3 Burger example:

[https://github.com/cyberbotics/webots\_ros2/blob/55043633c37b6afb5b2c80b768ceb0aaa8b7afe7/webots\_ros2\_turtlebot/resource/ros2control.yml#L11-L20](https://github.com/cyberbotics/webots_ros2/blob/55043633c37b6afb5b2c80b768ceb0aaa8b7afe7/webots_ros2_turtlebot/resource/ros2control.yml#L11-L20)

[https://github.com/cyberbotics/webots\_ros2/blob/55043633c37b6afb5b2c80b768ceb0aaa8b7afe7/webots\_ros2\_turtlebot/resource/turtlebot\_webots.urdf#L25](https://github.com/cyberbotics/webots_ros2/blob/55043633c37b6afb5b2c80b768ceb0aaa8b7afe7/webots_ros2_turtlebot/resource/turtlebot_webots.urdf#L25)


For GPS we use `libgazebo_ros_p3d`:

[https://github.com/cyberbotics/urdf2webots/blob/0136988774e7dde8d7d476c1b5e584a92d8f76df/urdf2webots/parserURDF.py#L1006-L1015](https://github.com/cyberbotics/urdf2webots/blob/0136988774e7dde8d7d476c1b5e584a92d8f76df/urdf2webots/parserURDF.py#L1006-L1015)



I couldn't find the `libgazebo_ros_gps_sensor` Gazebo plugin

##### mclzc 06/01/2021 13:07:06
`@Darko Lukić` I'm sorry, I made a typo mistake in that question. I didn't mean `libgazebo_ros_gps_sensor` but `libgazebo_ros_imu_sensor`. I've edited it.

##### Darko Lukić [Cyberbotics] 06/01/2021 15:29:34
The script looks only at `topicName` and `gaussianNoise` parameters which are common for both plugins, so I guess it is intentional. Do you experience some kind of problems?

##### mclzc 06/01/2021 15:43:03
No problems, I'm just trying to make my way through URDF, SDF and PROTO. Thanks `@Darko Lukić`

##### mmoralesp 06/01/2021 19:13:27
Hello, thanks for the response, but how i can get the position with de WebotsDifferentialDriveNode?

##### Darko Lukić [Cyberbotics] 06/02/2021 07:10:45
It will create an `odom` topic in which it publishes odometry data generated from wheels

