# Technical-Questions 2021

This is an archive of the `technical-questions` channel of the [Webots Discord server](https://discordapp.com/invite/nTWbN9m) for year 2021.

## January

##### RoboCoder 01/01/2021 00:01:23
I tried 3.9 and 3.7, same result both times.

##### KajalGada 01/01/2021 00:39:04
`@RoboCoder` I assume you are using Windows. What did you set your Python command in Webots preferences?

##### RoboCoder 01/01/2021 01:32:28
I tried a few things, but right now its set to "python"

##### Simon Steinmann [Moderator] 01/01/2021 11:08:41
`@RoboCoder` What PATH variables did you set exactly? You should add something like this:

C:\Users\USERNAME\AppData\Local\Programs\Python\Python38

C:\Users\USERNAME\AppData\Local\Programs\Python\Python38\Scripts


Unless you are on Linux or Mac, then it looks differently.

##### RoboCoder 01/01/2021 16:51:01
Should both of those be set in one PATH variable, or in two separate ones?

##### Simon Steinmann [Moderator] 01/01/2021 17:08:12
just add them beneath each other. Same variable

##### RoboCoder 01/01/2021 18:03:14
Okay. Does the variable name matter?

##### Simon Steinmann [Moderator] 01/01/2021 18:03:34
you are on windows right?


You have to change the "Path" variable [https://docs.alfresco.com/4.2/tasks/fot-addpath.html](https://docs.alfresco.com/4.2/tasks/fot-addpath.html)

##### RoboCoder 01/01/2021 18:08:16
Yep, on windows. I'll try that and get back to you.


Just wondering, does the version matter? I'm on 3.7 right now.


Okay, it's working now! I restarted the computer and it solved my issue. Thank you!

##### Yang 01/02/2021 11:50:31
Hi folks, I'm new to webots, just tried out a few examples on my PC, feels great! I'm wondering whether webots accepts urdf format files and mesh files (.stl etc.)?

##### Simon Steinmann [Moderator] 01/02/2021 12:37:40
`@Yang` you can directly import 3d meshes through the webots menu. In order to convert URDF to webot's PROTO format, use this:

[https://github.com/cyberbotics/urdf2webots](https://github.com/cyberbotics/urdf2webots)


I made a more detailed tutorial on how to do that:

[https://github.com/cyberbotics/urdf2webots/blob/master/docs/tutorial.md](https://github.com/cyberbotics/urdf2webots/blob/master/docs/tutorial.md)

##### Yang 01/02/2021 12:54:10
thank you Simon, I will check that out!

##### Nicolas Y 01/03/2021 14:30:49
Hi guys I'm also very new to Webots and I'm working on creating the 'Yamor Snake' but I can't figure out how to use 'Connectors' to get the pieces to stick together does anyone know how to get it done?

##### Simon Steinmann [Moderator] 01/03/2021 16:23:54
`@Nicolas Y` the hierarchy should look something like this:

Robot > children  > Hingejoint > endPoint Solid > children > Hingejoint > endPoint Solid > children > Hingejoint  ...etc

##### pnaraltnsk 01/03/2021 16:25:38
Hi, I am using webots for my graduation project and I am working on the Nao robot. I am trying to add pen node to my robot to mark the robot's walking path but for some reason, the pen node doesn't write. I added pen node to my robot's leftFootSlot and exactly like in pen.wbt example. Could you please help me out with this situation? Or do you know any other ways to mark the robot's walking path?

##### Simon Steinmann [Moderator] 01/03/2021 16:29:13
`@pnaraltnsk` Hi, make sure to follow the documentation: [https://cyberbotics.com/doc/reference/pen?tab-language=python](https://cyberbotics.com/doc/reference/pen?tab-language=python)

Make sure you activated the pen and that the parameters work for you. Also keep this in mind:

In order to be paintable, an object should be made up of a Solid node containing a Shape with a valid Geometry and an ImageTexture. The painted layer is applied over the texture without modifying it.

##### Nicolas Y 01/03/2021 16:30:16
`@Simon Steinmann` Thanks! I'll try it now 🙂

##### PowerYdRa 01/04/2021 05:21:22
hi guys, I am new in webots

I build this arena, my question is how to detect robot outside the arena? the idea is I want to make auto-referee for this arena, so which one outside first will be lose
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/795522239646662656/unknown.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 01/04/2021 06:59:01
A solid could have multiple contact point, so the `index` specifies which contact point info you want. The `index` should be included between 0 and the result of `getNumberOfContactPoints()`:

[https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python#wb\_supervisor\_node\_get\_contact\_point](https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python#wb_supervisor_node_get_contact_point)

Yes, the contact point can be returned only for a `Solid` (or `Robot`) node that doesn't have a direct parent `Solid` node, otherwise you will get NaN values as explained in the documentation.


The `time0to100` value is used to generate the object and compute the maximum acceleration value. So, yes, you can assume that the maximum acceleration is set only in the initialization. Given that the car is a procedural PROTO node, even if you should change the `time0to100` value, the whole robot node would be recreated and the controller will be restarted.


Hi, for your problem the best option would probably be to add a Supervisor node in your world and write a supervisor controller that monitors the status and the positions of the robots. Given that you know the size of the arena, you can get the robot position and check if it is outside:

[https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python#wb\_supervisor\_node\_get\_position](https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python#wb_supervisor_node_get_position)

##### PowerYdRa 01/04/2021 07:33:10
sorry to reply my own question, while waiting the answer from the other, I do some test and the answer is **Supervisor**

just add robot node and enable Supervisor in properties and make some program to get the position of the robot
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/795555408290775070/unknown.png)
%end


thanks for reply, just test and its very easy to get robot position, many thanks for your reply

##### Stefania Pedrazzi [Cyberbotics] 01/04/2021 07:37:07
You're welcome!

##### PowerYdRa 01/04/2021 07:38:02
the next is, how to make robot falling if outside from the arena

any idea how to build proper arena for sumo competition?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/795556632985600010/unknown.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 01/04/2021 07:40:02
Does your robot have Physics enabled? If yes, and if the floor `boundingObject` of  your arena corresponds to the graphical round area, then robot should automatically fall once outside.

##### PowerYdRa 01/04/2021 07:40:24
wait, check it first


this robot is e-puck, so default physics is enable, I think the problem is how I build the arena because its square floor with plane bounding, so I need to change bounding yes?

##### Stefania Pedrazzi [Cyberbotics] 01/04/2021 07:44:05
Yes, a plane in `boundingObject` has infinite size. You could use a Cylinder or a Box instead

##### PowerYdRa 01/04/2021 07:44:32
ok thanks, will try it and will give report here


wow great, thanks
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/795560824860311603/unknown.png)
%end

##### Simon Steinmann [Moderator] 01/04/2021 08:24:47
`@PowerYdRa`  A very simple way to check for "falling off" is to check the height of the robots. If the robot is below the arena surface, it loses. That's like 3 lines of code 🙂

##### PowerYdRa 01/04/2021 08:58:07
yes, thats the idea, but how to show some notification in the arena (not in console) ? any suggestion?

##### Simon Steinmann [Moderator] 01/04/2021 09:07:55
`from controller import Supervisor



\# disqualify height

y\_min = 0



supervisor = Supervisor()

timestep = int(supervisor.getBasicTimeStep())

robot\_1 = supervisor.getFromDef('Robot\_1')

robot\_2 = supervisor.getFromDef('Robot\_2')



root = supervisor.getRoot()

rootChildren = root.getField('children')

nNodes = rootChildren.getCount()

robotList = []

for i in range(nNodes):

    node = rootChildren.getMFNode(i)

    if node.getBaseTypeName() == 'Robot' and node.getDef() != 'referee':

        robotList.append(node)

nRobots = len(robotList)    

isActive = [1] *  nRobots

print(str(nRobots) + " Robots found")





while supervisor.step(timestep) != -1:

    for i in range(nRobots):

        pos = robotList[i].getPosition()

        if pos[1] < y\_min:

            isActive[i] = 0

        if sum(isActive) <= 1:

            print('Fight over!')

            `


`@PowerYdRa` this is a little example of a more general approach of the referee. This way it does not matter how many robots you add and what their DEF is. The only thing that matters is, that the supervisor Robot has the DEF "referee"

##### PowerYdRa 01/04/2021 09:08:52
wow nice, thanks for the code, will try it

##### Simon Steinmann [Moderator] 01/04/2021 09:09:11
let me know if you have questions

##### PowerYdRa 01/04/2021 09:09:24
okay, thanks for your help

##### Simon Steinmann [Moderator] 01/04/2021 09:09:29
robot\_1 = supervisor.getFromDef('Robot\_1')

robot\_2 = supervisor.getFromDef('Robot\_2')



wups, these two lines are not needed

##### Olivier Michel [Cyberbotics] 01/04/2021 09:12:27
You may use supervisor labels to display notifications in the 3D view: [https://www.cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_set\_label](https://www.cyberbotics.com/doc/reference/supervisor#wb_supervisor_set_label)

##### Simon Steinmann [Moderator] 01/04/2021 09:13:06
ahhh that's what I was looking for :p

##### Olivier Michel [Cyberbotics] 01/04/2021 09:13:20
Or a Display device (which may be a child of your supervisor Robot): [https://www.cyberbotics.com/doc/reference/display](https://www.cyberbotics.com/doc/reference/display)

##### PowerYdRa 01/04/2021 09:14:55
noted, thanks for your reply

will try it after fix the arena view, its look weird only show white 😆

##### Simon Steinmann [Moderator] 01/04/2021 09:38:20
`@PowerYdRa` added the display
> **Attachment**: [sumo\_referee.py](https://cdn.discordapp.com/attachments/565154703139405824/795586909321560084/sumo_referee.py)


even changes the color from green to red, when a robot is out 😄

##### Master.L 01/04/2021 09:47:58
Hi I have a question. I want to rotate the coordinates of the anchor. How can you do it?

##### Simon Steinmann [Moderator] 01/04/2021 09:48:26
What do you mean by anchor?


you mean in a hingejoint?

##### Master.L 01/04/2021 09:49:34
I want to rotate the anchor in the joint parameters of the hinge joint.

##### Simon Steinmann [Moderator] 01/04/2021 09:50:02
it is relative to the parent


so you need to change the parent orientation, or add a transform node inbetween

##### Master.L 01/04/2021 09:50:53
I want to add a joint at a specific location. But I can't hold the axis of rotation in the direction I want.


Where should I add the transform node?

##### Simon Steinmann [Moderator] 01/04/2021 09:52:26
The easiest solution would be to position the hingejoint with a parent transform node.


Or you position the location with the anchor, and change the axis value. but this will be less intuitive, as the axis wont be along either the x, y or z axis any more

##### Master.L 01/04/2021 10:34:14
Thank you `@Simon Steinmann`. Thanks to that, the problem was solved.

##### PowerYdRa 01/04/2021 10:53:21
auto referee for sumo competition
> **Attachment**: [sumo\_robot.mp4](https://cdn.discordapp.com/attachments/565154703139405824/795605787815968768/sumo_robot.mp4)


forget to give auto stop simulation, 😀

##### yash 01/04/2021 11:02:42
> A solid could have multiple contact point, so the `index` specifies which contact point info you want. The `index` should be included between 0 and the result of `getNumberOfContactPoints()`:

> [https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python#wb\_supervisor\_node\_get\_contact\_point](https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python#wb_supervisor_node_get_contact_point)

> Yes, the contact point can be returned only for a `Solid` (or `Robot`) node that doesn't have a direct parent `Solid` node, otherwise you will get NaN values as explained in the documentation.

@Stefania Pedrazzi#5404  understood , but for example a two finger gripper to grasp an object , so I will have multiple contact points, and I get them by specifying the index , then how can I identify through this index that which solids is in contact !?

##### Stefania Pedrazzi [Cyberbotics] 01/04/2021 12:25:03
You cannot get information about the other solid which is in contact with the current node from the Supervisor API. If you need this advanced information, then you should implement a physics plugin and retrieve this information from the `webots_physics_collide` callback function:

[https://www.cyberbotics.com/doc/reference/physics-plugin?tab-language=python](https://www.cyberbotics.com/doc/reference/physics-plugin?tab-language=python)

However, if you just need to check if point belongs to the contact between the two finger, then there are maybe other simpler solutions, for example to compare the returned contact points of finger 1 and finger 2.

##### yash 01/04/2021 12:48:17
Alright understood , just one more thing , we have to specify a node to use the function  -

def getNumberofContactpoints() , how to do that?

##### Stefania Pedrazzi [Cyberbotics] 01/04/2021 12:53:05
There are many different ways to retrieve a node, here is the link to the documentation:

[https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python#wb\_supervisor\_node\_get\_from\_def](https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python#wb_supervisor_node_get_from_def) (and all the other functions in this section)

[https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python#wb\_supervisor\_field\_get\_mf\_node](https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python#wb_supervisor_field_get_mf_node)

[https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python#wb\_supervisor\_field\_get\_mf\_node](https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python#wb_supervisor_field_get_mf_node)

You can find an example in the sample simulation:

[https://www.cyberbotics.com/doc/guide/samples-devices#supervisor-wbt](https://www.cyberbotics.com/doc/guide/samples-devices#supervisor-wbt)

##### PowerYdRa 01/04/2021 13:45:59
can webots save the world with lock mode? I mean object in the world can't be replace, add or remove until we insert some password or passcode?

##### yash 01/04/2021 13:46:05
> There are many different ways to retrieve a node, here is the link to the documentation:

> [https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python#wb\_supervisor\_node\_get\_from\_def](https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python#wb_supervisor_node_get_from_def) (and all the other functions in this section)

> [https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python#wb\_supervisor\_field\_get\_mf\_node](https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python#wb_supervisor_field_get_mf_node)

> [https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python#wb\_supervisor\_field\_get\_mf\_node](https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python#wb_supervisor_field_get_mf_node)

> You can find an example in the sample simulation:

> [https://www.cyberbotics.com/doc/guide/samples-devices#supervisor-wbt](https://www.cyberbotics.com/doc/guide/samples-devices#supervisor-wbt)

`@Stefania Pedrazzi`  thanks a lot !

##### PowerYdRa 01/04/2021 13:50:34
this is because, I want to share the world to student, so they cant change everything except coding the robot only

##### Darko Lukić [Cyberbotics] 01/04/2021 14:02:48
`@PowerYdRa` Something like that is not possible from Webots. It should be handled by your OS. For example, you can set the world file permissions to read-only mode, so only the administrator can edit it.

##### PowerYdRa 01/04/2021 14:04:29
if I setup server like in this tutorial [https://cyberbotics.com/doc/guide/web-simulation?tab-language=python](https://cyberbotics.com/doc/guide/web-simulation?tab-language=python)

can other people do some programming for robot?

##### Darko Lukić [Cyberbotics] 01/04/2021 14:05:48
Yes, in that case it would be similar to:

[https://robotbenchmark.net/](https://robotbenchmark.net/)

##### Simon Steinmann [Moderator] 01/04/2021 14:15:39
`@PowerYdRa` you can have them export their robot as a .wbo file and give you their controller as well


you can then load them into your world

##### yash 01/04/2021 14:41:58
I have set  SUPERVISOR field to TRUE, so to use  .getDevice() functions for sensors and motors ,  will robot.getDevice() be applicable ?

##### Nicolas Y 01/04/2021 15:23:14
`@Simon Steinmann` Hi Simon, thanks for you're help earlier but I can't seem to figure out how to 'bound' them so that all eight 'Yamors' move as one bigger whole

##### Stefania Pedrazzi [Cyberbotics] 01/04/2021 15:32:42
Yes

##### Simon Steinmann [Moderator] 01/04/2021 15:32:52
You have to put a "Robot" node as your base node `@Nicolas Y`

##### Nicolas Y 01/04/2021 15:33:01
I did

##### Simon Steinmann [Moderator] 01/04/2021 15:33:13
and add motors in the device section of the hingejoints


what exactly is your problem?

##### Nicolas Y 01/04/2021 15:35:06
I'm not sure how to get the components to stick
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/795676689534812170/unknown.png)
%end

##### Simon Steinmann [Moderator] 01/04/2021 15:35:37
Post your world file here


one thing I can see from the screenshot is, that you have not added JointParameters


double click on them and add a Parameter node


The individual Solids also need a physics node and a bounding Geometry

##### Nicolas Y 01/04/2021 15:40:17
This is my world file
> **Attachment**: [YamorSnake1.wbt](https://cdn.discordapp.com/attachments/565154703139405824/795677996596985856/YamorSnake1.wbt)

##### Simon Steinmann [Moderator] 01/04/2021 15:41:45
ohhh


you added all the segments as children of the robot


you have to put each next joint into the children node of the Solid before

##### Nicolas Y 01/04/2021 15:42:32
ohhh


I'll try that now

##### Simon Steinmann [Moderator] 01/04/2021 15:43:01
you can add boundingObject and physics by simply doubleclicking it


ohh wait, all the Yamor are individual robots?

##### Nicolas Y 01/04/2021 15:43:34
yes

##### Simon Steinmann [Moderator] 01/04/2021 15:43:55
are they supposed to be individually controlled?

##### Nicolas Y 01/04/2021 15:44:09
no I want them to work as one

##### Simon Steinmann [Moderator] 01/04/2021 15:44:09
Because I'm not sure about a chain of robot nodes

##### Nicolas Y 01/04/2021 15:44:37
oh?

##### Simon Steinmann [Moderator] 01/04/2021 15:45:43
Perhaps the devs know the best solution for that. I'm not sure about Robots containing other Robots

##### Nicolas Y 01/04/2021 15:47:03
oh


that complicates things

##### Simon Steinmann [Moderator] 01/04/2021 15:48:34
`@Olivier Michel` How does a hierarchy of robots work? Do the child robots act as devices if they don't have a controller assigned?

##### Olivier Michel [Cyberbotics] 01/04/2021 15:49:23
No, a robot can access only the devices it contains. If a robot contains another robot, the devices of the child robot are not available to the parent robot.

##### Simon Steinmann [Moderator] 01/04/2021 15:50:36
`@Nicolas Y` I think you are supposed to add one Yamor, then doubleclick on 'extensionSlot' and add another

##### Nicolas Y 01/04/2021 15:51:22
`@Olivier Michel` in which case how can one replicate this?
%figure
![yamor.png](https://cdn.discordapp.com/attachments/565154703139405824/795680785843748954/yamor.png)
%end

##### Olivier Michel [Cyberbotics] 01/04/2021 15:57:41
In this example (included in Webots), each module is an independent robot (there is no hierarchy).


Robots attach to each other with a Connector node.

##### Nicolas Y 01/04/2021 15:59:55
`@Olivier Michel` okay thanks I'll try that

##### Olivier Michel [Cyberbotics] 01/04/2021 16:17:39
I would recommend you to start from the yamor demo and modify it bit-by-bit to ensure you are not breaking anything.

##### Nicolas Y 01/04/2021 16:33:35
`@Olivier Michel` sorry but despite checking the website I can't seem to figure out how to use the 'Connector' node to Combine the two 'Yamor' nodes

##### Olivier Michel [Cyberbotics] 01/04/2021 16:34:45
I would recommend you to open the world file in a text  editor.


Yes, in the scene tree of Webots, but it may be easier to edit from a text editor (but this is a question of personal preferences).

##### pnaraltnsk 01/04/2021 20:28:39
Hi, I followed the documentation and checked everything else. Everything looks fine and the pen should have been working but it didn't. I am still struggling to find what's wrong.

##### TerryWr1st 01/05/2021 00:19:20
Hi all, new to this channel. I'm developing my thesis in Webots, which consists of a number of epucks at the start of the simulation. During runtime, I then need to spawn additional epucks and delete the old epucks which have served their purpose in the simulation. Is this possible in Webots? And is it achieved through a supervisor function, or some other method?

##### DDaniel [Cyberbotics] 01/05/2021 00:47:52
Yes you can do it using the supervisor, you can add and remove the nodes of the robots from the scene tree: [https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_field\_import\_mf\_node\_from\_string](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_field_import_mf_node_from_string)

##### TerryWr1st 01/05/2021 00:50:29
Ah ok, brilliant, thank you. Will take a look.

##### mayank.kishore 01/05/2021 03:35:29
Has anyone successfully implemented RRT for a robot to get to a waypoint?

##### Alessia Valle 01/05/2021 08:54:35
Hi, I'm working on my Master Thesis and I need your help if possible. I have imported my four-wheeled robot CAD model into webots and I need to interface it with ROS (not ROS2) to implement SLAM algorithm. The robot is equipped with a Lidar sensor. I have gone through webots and ROS documentation but I am still stuck.

I have seen that to use slam\_gmapping, my mobile robot needs to provide odometry data and I need to publish two different transforms, base\_laser->base\_link and base\_link->odom. Since I am novice to ROS and I have no experience in interfacing it with webots, here are my questions:

- how to make my robot publish its odometry? my idea is to use Position Sensors, but I don't know how to turn the values from the position sensors into the standard nav\_msgs/Odometry used in ROS. I think I am missing some steps. 

- since I need to publish transforms of odom, base\_link and base\_laser to use with gmapping, do I need to create the URDF of my robot? 

Thank you in advance

##### Master.L 01/05/2021 09:11:28
Excuse me, I have a question.

I am implementing a delta robot model in webots.

There is an upper leg that moves by a hinge joint and a lower leg that is connected to the upper leg by a ball joint.

Since the hinge joint is moved after starting the simulation, the ball joint continues to rotate and the position value increases. Therefore, the part connected to the ball joint continues to rotate. How can I fix it?

##### Simon Steinmann [Moderator] 01/05/2021 09:15:59
`@Alessia Valle` a Position sensor measures the angle of a joint / motor. What you need is the Pose

[http://docs.ros.org/en/melodic/api/nav\_msgs/html/msg/Odometry.html](http://docs.ros.org/en/melodic/api/nav_msgs/html/msg/Odometry.html)


this is the nav\_msgs definition

##### Darko Lukić [Cyberbotics] 01/05/2021 09:16:39
Hello `@Alessia Valle`,

- For odometry, see the example here: [https://github.com/cyberbotics/webots\_ros2/blob/9682e06932929ea3bfab5a6a826f6fe67471aef6/webots\_ros2\_core/webots\_ros2\_core/webots\_differential\_drive\_node.py#L128-L197](https://github.com/cyberbotics/webots_ros2/blob/9682e06932929ea3bfab5a6a826f6fe67471aef6/webots_ros2_core/webots_ros2_core/webots_differential_drive_node.py#L128-L197)

(it is for ROS2 but there is almost no difference for ROS)

- You don't have create URDF to publish transforms. If your model is simple you can publish them manually, see an example for a physical robot:

[https://github.com/cyberbotics/epuck\_ros2/blob/master/epuck\_ros2\_driver/src/driver.cpp#L167-L232](https://github.com/cyberbotics/epuck_ros2/blob/master/epuck_ros2_driver/src/driver.cpp#L167-L232)

(If you need URDF you can use [https://cyberbotics.com/doc/reference/robot#wb\_robot\_get\_urdf](https://cyberbotics.com/doc/reference/robot#wb_robot_get_urdf))

##### Simon Steinmann [Moderator] 01/05/2021 09:21:38
is the ball joint passive? If so. you have to add friction


so expand the balljoint and add a "JointParameters" node


there you can edit those values

##### yash 01/05/2021 09:34:26
I am getting some weird results when I play the simulation.
> **Attachment**: [test01.mp4](https://cdn.discordapp.com/attachments/565154703139405824/795948316768075806/test01.mp4)


what could be the issue ?

##### Simon Steinmann [Moderator] 01/05/2021 09:36:45
can you click on your robot and show a screenshot (with the white lines of the bounding box)


My first guess would be, that you used the visual mesh data as collision. And your shapes are not convex


Also it seems like it might be starting partially in the floor

##### yash 01/05/2021 09:39:27
this the bounding box
%figure
![Screenshot_from_2021-01-05_15-08-34.png](https://cdn.discordapp.com/attachments/565154703139405824/795949578347085854/Screenshot_from_2021-01-05_15-08-34.png)
%end


how could I rectify the problem ?

##### Simon Steinmann [Moderator] 01/05/2021 09:40:13
oh the grey metal part is not part of the robot?


and it looks like therre is a horizontal part, which has no visual

##### yash 01/05/2021 09:40:55
no it is not the part of the robot

##### Simon Steinmann [Moderator] 01/05/2021 09:41:11
but the robot is attached to it?


In general it is much better (for performance AND accuracy) to replace the boundingObject with simple geometries, such as boxes, spheres, cylinders and capsules

##### yash 01/05/2021 09:42:08
yes I have positioned the robot accordingly, but it is not attached to it.

##### Simon Steinmann [Moderator] 01/05/2021 09:45:33
well it is hard to say. What do you expect to happen?

##### Master.L 01/05/2021 09:45:42
Thanks for solving it!!

##### yash 01/05/2021 09:47:12
the 2 link manipulator should rotate about it's axis provided. With the metal plate at it's position only.

##### Simon Steinmann [Moderator] 01/05/2021 09:48:39
you have to make its axis static then


remove the physics node of the Robot


it still seems weird to me, that you have 3 segments in your bounding object


there is something weird there

##### yash 01/05/2021 09:50:19
oh okay, alright let me look into it, thanks !

##### nelsondmmg 01/05/2021 21:53:51
Could you cite a reference for the differential ratio calculated for the rear wheel of the vehicle? I'm assuming that a similar correction need to be made before applying the acceleration to the wheels of the vehicle, but I cannot find any reference that explains how this differential ratio is calculated. Thanks.

##### PowerYdRa 01/06/2021 00:22:35
hi guys, I get a problem,

just install numpy via pip in terminal, but when call in webots, its give error No Module named 'numpy', how to define library / module installed to webots?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/796171825926504448/unknown.png)
%end

##### DDaniel [Cyberbotics] 01/06/2021 00:41:20
Make sure in tools>preferences the python command is also python3

##### TerryWr1st 01/06/2021 00:49:02
Can Webots compile in x86, or only x64? I went through the visual studio setup both manually,  and with the wizard. The wizard sets the configuration properties as Release/x64, but this step isn't mentioned in the manual set up guide: [https://cyberbotics.com/doc/guide/using-your-ide](https://cyberbotics.com/doc/guide/using-your-ide)


Also, why does the wizard default to a Release configuration instead of debug?

##### Diego Rojas 01/06/2021 05:50:10
What would be the best way to simulate a ur5 robot on a 7th axis linear track using ROS2 and moveit2? I would like to control both the linear motion of the 7th axis track and the robot arm with moveit2. Using the follow\_joint\_trajectory action server, I was able to control the ur5 robot with ros2 control and moveit2, but now I want to attach the robot to a linear track. I have done this in the past in gazebo by creating a prismatic joint and using the follow\_joint\_trajectory controller to control it with ros\_control and moveit. Is this possible in webots also? How can I utilize the existing follow\_joint\_trajectory action server that is created in webots\_ros2 and create another follow\_joint\_trajectoy action server for a linear/prismatic joint with webots\_ros2?

##### Olivier Michel [Cyberbotics] 01/06/2021 07:12:43
Webots can compile controllers in either x64 or x86. However, by default it will build x64 binaries. If you want to build a x86 controller binary, you should be sure to link it with `WEBOTS_HOME\msys64\mingw32\bin\Controller.dll` instead of `WEBOTS_HOME\msys64\mingw64\bin\Controller.dll`.

##### yash 01/06/2021 07:38:12
I am working on a certain application that involves grasping a human foot. I imported the human foot CAD using .VRML format. And used the DEF geometry indexfaceset in the shape node to define the bounding object of the human foot. But the bounding object seems to be complex which is resulting in weird physics behavior.  How to address this issue such that the human foot remains stable ?
> **Attachment**: [2.mp4](https://cdn.discordapp.com/attachments/565154703139405824/796281451237736478/2.mp4)


Because when I replaced the foot with a simple cylindrical solid it worked fine since the bounding object of the cylinder is not complex.

##### Stefania Pedrazzi [Cyberbotics] 01/06/2021 07:48:08
The used formula comes from standard steering dynamics. Depending if the front or rear  wheels drive the car (or all four wheels) you need to compute the ratio for the left and right wheel because the do not turn using the same angle.

Here is the reference to a chapter about steering dynamics: [https://link.springer.com/chapter/10.1007/978-1-4614-8544-5\_7](https://link.springer.com/chapter/10.1007/978-1-4614-8544-5_7) but you can probably find other references on the web.

##### Olivier Michel [Cyberbotics] 01/06/2021 07:53:37
You should create a compound bounding object for the foot that is not a mesh, but a combination of simple primitives (spheres, capsules, cylinders and boxes). See this tutorial: [https://cyberbotics.com/doc/guide/tutorial-5-compound-solid-and-physics-attributes](https://cyberbotics.com/doc/guide/tutorial-5-compound-solid-and-physics-attributes) for an example of creating a compound bounding object.

##### yash 01/06/2021 08:01:33
alright ! but this wiil solve the issue right irrespective of the solid geometry ?

##### Olivier Michel [Cyberbotics] 01/06/2021 08:02:22
It should (as long as you don't include any mesh in the bounding object).

##### yash 01/06/2021 08:02:48
understood, thanks a lot !

##### Darko Lukić [Cyberbotics] 01/06/2021 08:48:56
Hello `@Diego Rojas`, I have never tried adding 7th axis linear track, but here are a few ideas. `TrajectoryFollower` can control any motor present in the robot. Therefore, you can add the linear track (built with `SliderJoint` which behaves in the same way as the prismatic joint) in the Webots robot model.



Alternatively, you can use `ConveyorBelt` but it is more complicated. You need to enable a `Supervisor` field of your robot and extend `trajectory_follower` to control `speed` field of `ConveyorBelt`. This will also require some basic integration to control the position of the belt as `ConveyorBelt` allows only a speed control.

##### Alessia Valle 01/06/2021 09:08:34
Hi `@Darko Lukić`, thank you again for the suggestions.  I also found on the ROS wiki website the diff\_drive\_controller  [http://wiki.ros.org/diff\_drive\_controller](http://wiki.ros.org/diff_drive_controller) but I don't know if it is useful for my purposes and if it can be used with Webots. Any idea? 

Moreover, I have another doubt about transforms and URDF. Actually I need the static transforms between the wheels, the base\_link and base\_laser to set up navigation. But how can I publish transforms without the URDF file? Do these frames take the name of the corresponding nodes on Webots?

##### Darko Lukić [Cyberbotics] 01/06/2021 09:20:51
You can use `diff_drive_controller` as well. Your objective is to publish `Odometry` topic and transforms that describe translation and orientation between `base_link` and `odom` frames. You can use `diff_drive_controller` or write one by yourself.



You don't need URDF to publish transforms. In general, `robot_state_publisher` uses URDF to publish `TransformStamped` ([http://docs.ros.org/en/melodic/api/geometry\_msgs/html/msg/TransformStamped.html](http://docs.ros.org/en/melodic/api/geometry_msgs/html/msg/TransformStamped.html)) messages. However, you can publish those messages by yourself.

##### mayank.kishore 01/06/2021 18:32:51
Hi! Trying to setup my environment for ros2 on my mac and am getting a file not found error, am I missing something? [https://index.ros.org/doc/ros2/Installation/Foxy/Linux-Development-Setup/#environment-setup](https://index.ros.org/doc/ros2/Installation/Foxy/Linux-Development-Setup/#environment-setup)


This is what my directory looks like:
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/796446454096068608/unknown.png)
%end


Hi! Were you able to solve this problem? Am running into the same issue

##### Darko Lukić [Cyberbotics] 01/06/2021 18:53:25
`@mayank.kishore` 

[https://index.ros.org/doc/ros2/Installation/Foxy/macOS-Install-Binary/](https://index.ros.org/doc/ros2/Installation/Foxy/macOS-Install-Binary/)

You should follow a tutorial for macOS

##### mayank.kishore 01/06/2021 19:00:00
~~I was following the tutorial, ran `tar xf ~/Downloads/ros2-release-distro-date-macos-amd64.tar.bz2` and am unsure why the next step of sourcing the ros2 file `. ~/ros2_foxy/ros2-osx/setup.bash` is trying to look for `local_setup.bash` in the directory above since the extract only puts everything into the `ros2-osx` folder as shown above~~



Figured it out needed to run `. ~/ros2_foxy/ros2-osx/setup.zsh`

##### Master.L 01/07/2021 03:02:30
Hi! Is there an inverse kinematics library for running delta-robots on webots?

##### Luiz Felipe 01/07/2021 08:33:45
Hello, I am trying to run a web simulation using a session and simulation server inside a docker container as explained in: [https://cyberbotics.com/doc/guide/web-simulation?tab-host=localhost](https://cyberbotics.com/doc/guide/web-simulation?tab-host=localhost)


I created the docker image, ran the docker container and inside the /usr/local/webots/resources/web/server i ran the './server.sh'



%figure
![Screenshot_from_2021-01-07_17-36-56.png](https://cdn.discordapp.com/attachments/565154703139405824/796658964321533992/Screenshot_from_2021-01-07_17-36-56.png)
%end


It seems to start successfully... but now I am having trouble testing if that works


As the documentation I am trying 'ws://localhost:1999/session?url=webots://github.com/cyberbotics/webots/branch/develop/projects/languages/python/worlds/example.wbt' in Connect to using the streaming viewer but i get streaming server error 1006

##### Simon Steinmann [Moderator] 01/07/2021 08:43:56
`@Master.L` [https://gist.github.com/hugs/4231272](https://gist.github.com/hugs/4231272) this looks like what you're looking for


[https://www.marginallyclever.com/other/samples/fk-ik-test.html](https://www.marginallyclever.com/other/samples/fk-ik-test.html) or this could help

##### Olivier Michel [Cyberbotics] 01/07/2021 08:46:17
Did you try to check the monitor page http://localhost:1999/monitor to check the status of your server?

##### Luiz Felipe 01/07/2021 09:35:08
Oh... If I check it, it appears the load:



%figure
![monitor.png](https://cdn.discordapp.com/attachments/565154703139405824/796673297160601600/monitor.png)
%end


But if i click the localhost/2000 I get 'This site can't be reached'... Hm...

##### Olivier Michel [Cyberbotics] 01/07/2021 09:38:04
It should be http://localhost:2000 instead.

##### Luiz Felipe 01/07/2021 09:39:42

%figure
![monitor-2.png](https://cdn.discordapp.com/attachments/565154703139405824/796674417806802994/monitor-2.png)
%end


Yes.. It works... it seems that I am doing something wrong in the Connect to part....

##### YCSU 01/07/2021 09:51:21
Hi, is there a way to move all the motors of the robot arm at the same time? For example, I would like to move the tool center point (tcp) of a ur5e in a straight line from x=0 m to x=5 m. Right now I call the  \_setPosition\_ function for each motor in a sequential order with the inverse kinematics result, and judging from the movement, the  tcp is not moving in a straight line. Especially at the start of the movement, the arm jitters. I suspect it's caused by the asynchronous movement of the motors.

##### Luiz Felipe 01/07/2021 09:57:00
The session server and simulation servers seems to be up and rnning... Is that correct to put 'ws://localhost:1999/session?url=webots://github.com/cyberbotics/webots/branch/develop/projects/languages/python/worlds/example.wbt' i the 'Connect to' part to test the web simulation?
%figure
![Screenshot_from_2021-01-07_18-55-34.png](https://cdn.discordapp.com/attachments/565154703139405824/796678771264192522/Screenshot_from_2021-01-07_18-55-34.png)
%end

##### Alessia Valle 01/07/2021 11:56:31
Hello everyone! My tutor wrote to me and he has changed his mind about my thesis. He asked me to implement SLAM algorithm on Webots without any integration with ROS. Do you have some resources/documentation about it? Is there any existing project in which SLAM has been implemented?

##### Darko Lukić [Cyberbotics] 01/07/2021 12:14:22
The only examples about SLAM and Webots include ROS and ROS2:

[http://wiki.ros.org/webots\_ros/Tutorials/Sample%20Simulations](http://wiki.ros.org/webots_ros/Tutorials/Sample%20Simulations)

[https://github.com/cyberbotics/webots\_ros2/wiki/SLAM-with-TurtleBot3](https://github.com/cyberbotics/webots_ros2/wiki/SLAM-with-TurtleBot3)



If you need a background in this topic you can check:

[https://pythonrobotics.readthedocs.io/en/latest/modules/slam.html](https://pythonrobotics.readthedocs.io/en/latest/modules/slam.html)

[https://github.com/kanster/awesome-slam](https://github.com/kanster/awesome-slam)

[https://www.youtube.com/playlist?list=PLgnQpQtFTOGQrZ4O5QzbIHgl3b1JHimN\_](https://www.youtube.com/playlist?list=PLgnQpQtFTOGQrZ4O5QzbIHgl3b1JHimN_)



I am not sure whether there is a good implementation of SLAM that you can easily integrate into your project without ROS. `cartographer` is a good implementation, but I am not sure how hard is to use it without ROS:

[https://github.com/cartographer-project/cartographer](https://github.com/cartographer-project/cartographer)

##### yash 01/07/2021 12:25:58
I want to use the supervisor function as well as the Robot node , therefore should I write ——

from controller import Supervisor, Robot



And should define as -

robot = Robot()

supervisor = Supervisor() 

 

Please help me with this !?

##### Darko Lukić [Cyberbotics] 01/07/2021 12:27:32
If you set the motor positions in the single step they all move at the same time. Your problem is therefore something else. For example, the motor in shoulder moves the endpoint faster than the motor in the wrist even though they rotate at the same speed (the motor in the shoulder is farther away from the endpoint than the motor in the wrist). One solution would be to find a lot of IK solutions along a straight line and apply the new IK solution in each step.


No. `robot = Supervisor()` is enough. It will give you methods from `Robot` and `Supervisor` nodes.

##### Alessia Valle 01/07/2021 12:34:06
Thank you. So do you think that I will have to write the SLAM algorithm from scratch on my own?

##### Darko Lukić [Cyberbotics] 01/07/2021 12:35:39
Yes, but make sure the port is correct.

##### yash 01/07/2021 12:37:14
> No. `robot = Supervisor()` is enough. It will give you methods from `Robot` and `Supervisor` nodes.

`@Darko Lukić`  thank you !

##### Darko Lukić [Cyberbotics] 01/07/2021 12:38:28
I don't know :/ Depends on your project.

##### Alessia Valle 01/07/2021 12:45:00
What do you mean by "it depends on your project?"😅

##### Darko Lukić [Cyberbotics] 01/07/2021 12:47:48
I guess if you have a good understanding of SLAM than you can implement it, but if you want a robust solution it is better to use something like `cartographer`.

##### Alessia Valle 01/07/2021 12:58:44
Ok, I will go through the documents you have shared! Thank you for your availability! 😀

##### josiehughes 01/07/2021 13:09:59
Hi - I'm wondering about the current e-puc hardware and compatability with Webots. Is the Epuc-2 compataible with Webots?

##### Luiz Felipe 01/07/2021 14:07:09
I get streaming server error... will check the debug flags of the session and simulation servers...

##### Darko Lukić [Cyberbotics] 01/07/2021 15:54:45
Hello `@josiehughes`, It is compatible

##### YCSU 01/07/2021 16:01:07
Got it. Thanks!

##### Ragemor 01/07/2021 17:43:11
Is there any Webots project about multi robot coordination using leader follower approach? I want to implement leader follower algorithm in my project but i cant understand how can i write code about that. Which nodes and api functions(or sensors) should i use? Do you have docs about leader follower and multi robots in webots?

##### adamadam333 01/07/2021 20:15:16
Hi, I am trying to rotate an epuck2 robot by 90 degrees, I tried using time but I couldn't measure exactly 90 degrees, is there a way to do it effectively not using time and angular speed?

##### adiagr 01/07/2021 21:36:03
The most effective way to turn an epuck2 is by using time and angular speed itself. You may want to have a look at the proto file and take into account the thickness of the wheels among the other things to get a more accurate value of axle length.

##### ArjunSadananda 01/07/2021 21:41:23
`@adamadam333` If you prefer, you could use encoders (and write a closed loop controller) instead of doing it by calculating the time (open loop controller).

##### Luiz Felipe 01/08/2021 06:08:10
In line 46 in [https://github.com/cyberbotics/webots/blob/master/resources/web/streaming\_viewer/index.html#L46](https://github.com/cyberbotics/webots/blob/master/resources/web/streaming_viewer/index.html#L46) of the new 2021a version the line it is not supposed to be:  `  <script src="[https://www.cyberbotics.com/wwi/R2021a/webots.min.js](https://www.cyberbotics.com/wwi/R2021a/webots.min.js)"></script>` ?

##### Darko Lukić [Cyberbotics] 01/08/2021 08:16:27
`@Luiz Felipe` There is `webots.min.js` distributed with Webots packages (`resources/web/wwi/webots.min.js`), but you can also use the one available on Cyberbotics servers.

##### Luiz Felipe 01/08/2021 08:21:10
Yes `@Darko Lukić` , I changed to use the one available on Cyberbotics servers because I am running the servers in a docker container. But the streaming server page seems a big buggy for the 2021a version in my computer... I am still having trouble following the Web Simulation documentation also, the session server and simulation server are fine, but I am not seem able to figure out how to open a simulation or send any information to the simulation server...


I just thought it is better for the default to be the one available on the Cyberbotics servers.

##### Olivier Michel [Cyberbotics] 01/08/2021 08:28:01
It should be the same anyhow.

##### yash 01/08/2021 08:33:19
Can I put touch sensor in children of solid node ? Since when the touch sensor collides with another solid, I want the contact point of the collision?

##### Luiz Felipe 01/08/2021 08:39:46
Yups... The streaming viewer however does not open... The 2020b version works fine...
%figure
![Screenshot_from_2021-01-08_17-37-04.png](https://cdn.discordapp.com/attachments/565154703139405824/797021720057741312/Screenshot_from_2021-01-08_17-37-04.png)
%end

##### Olivier Michel [Cyberbotics] 01/08/2021 08:43:19
Yes, you can. However to get the position of the contact point of the collision, you should use the supervisor API, not the touch sensor API.


I am sorry, but currently deploying a streaming server is not very easy, has some bugs and also is not very well documented. We are currently working on improving this, but it will take some time. Meanwhile, I am afraid, you will have to debug this kind of problem by yourself.

##### yash 01/08/2021 08:48:56
understood, thanks!

##### Luiz Felipe 01/08/2021 08:51:44
ok... no problem hehe

##### Master.L 01/08/2021 09:21:18
Excuse me, how can i get hinge joint Velocity?


Should I calculate by differentiating the position value?.

Can I get it through the sensor?

##### Simon Steinmann [Moderator] 01/08/2021 09:25:36
add PositionSensors


and yes, you have to calculate it by the change


vel = (pos\_new - pos\_old) / (timestep * 1000)


[https://cyberbotics.com/doc/reference/positionsensor?tab-language=python](https://cyberbotics.com/doc/reference/positionsensor?tab-language=python)


timestep = getBasicTimeStep()

so in ms, that's why you have to multiply by 1000

##### Master.L 01/08/2021 09:55:38
`@Simon Steinmann` Thank you. Succeeded in receiving the position value!!.

But I control the webots through ros. Is there a way to get time from ros topic or service?

##### josiehughes 01/08/2021 09:58:12
Thanks!

##### Master.L 01/08/2021 10:08:13
`@Simon Steinmann` Thank you. I found.👍

##### Ragemor 01/08/2021 10:14:59
Excuse me is there any Webots project about multi robot coordination using leader follower approach? I want to implement leader follower algorithm in my project but i cant understand how can i write code about that. Which nodes and api functions(or sensors) should i use? Do you have docs about leader follower and multi robots in webots?

##### Olivier Michel [Cyberbotics] 01/08/2021 10:19:44
No, we don't have any documentation about leader follower and I am not aware of any project about it.


Maybe this one can help? [http://www.diegoantognini.com/projects/dis/](http://www.diegoantognini.com/projects/dis/)

##### Ragemor 01/08/2021 14:00:31
Thank you. If anyone have an idea or doc about leader follower can share with me?

##### adamadam333 01/08/2021 14:23:25
could someone give me a hand with rotating an EPUCK2 robot by 90 degrees? Tried absolutely everything and nothing worked, I am a complete beginner so don't know much


and there are no resources online for it


would be very helpful


I could pay for a call teaching me on how to do it

##### Maheed 01/08/2021 14:40:49
Hi everyone, Im a webots beginner at the moment. I have 8 yamor modules connected together to look like a snake and I really need some help to program some controllers in C to make it do something such as move towards an a certain colour or avoid obstacles. Can anyone help me with this?

##### Simon Steinmann [Moderator] 01/08/2021 14:44:57
are you dead set on c?

##### Maheed 01/08/2021 14:46:30
Yep

##### Simon Steinmann [Moderator] 01/08/2021 14:47:47
Then I wont be able to help much. But my approach would be to create basic functionality as "move forward" "turn left" "turn right" etc. And then combine those into algorithms for obstacle avoidance etc.

##### Maheed 01/08/2021 15:27:37
Okay thanks


Is there anyone here good with C who can help me with this please?

##### linexc 01/08/2021 23:58:08
Hello everyone, I have a problem regarding Webots and NAO robot. I am trying to write a ROS server and client in order to make the NAO in Webots perform some gestures. But when I tried to simulate it, the error showed always like this. But I really don't know why the network address in already in use. If anybody can help me, I would be very grateful.



%figure
![error.png](https://cdn.discordapp.com/attachments/565154703139405824/797253462727327784/error.png)
%end

##### Simon Steinmann [Moderator] 01/09/2021 08:26:26
`@linexc` looks like a network problem. I have no experience with this exact thing, but usually you would use localhost


which should be 127.0.0.1

##### linexc 01/09/2021 10:40:19
thank you. I will try it now.

##### Chernayaten 01/09/2021 19:36:59
I am messing around with the pen.wbt example and I found that the floorSize and the floorTileSize have to match (for example 1 1 for both or 4 4) . Can someone explain why?


Also if anyone can help me figuring out how to make the robot(still using the one in pen.wbt) turn 90 degree angles..atm I am using setRotate(getTargetPosition() +/- Rotate) where Rotate = 2*math.pi +0.4. Not sure why 0.4 works, but it seems ok in general, except if I use a big time\_step and sometimes when I left the simulation run a while.



I feel like I should be able to use 1.0/3.0, 4.0/3.0 and stuff like that to get different angles, but I can't make it work

##### DrakerDG 01/10/2021 00:37:29
Hello everyone! Playing with lights and lighting in Webots.  I have placed three line follower robots using the same controller, I still don't understand how it works.  Can someone tell me how it works?

[https://youtu.be/jMzKwcVs\_08](https://youtu.be/jMzKwcVs_08)

##### Simon Steinmann [Moderator] 01/10/2021 12:37:11
`@DrakerDG` Hey, what part exactly don't you understand?

##### DrakerDG 01/10/2021 12:41:40
`@Simon Steinmann` hello! I don't understand how by duplicating or tripling the same robot (copy & paste) they can use the same controller separately.  What I mean is that I just copied and pasted them without changing anything and it worked.  Each robot can follow the line without affecting the other.  In the end I only changed the color of the chassis and nothing else.  I see that it works but I don't understand how


I understood that each robot should have its own controller, but apparently it is not entirely necessary

##### Simon Steinmann [Moderator] 01/10/2021 12:50:15
Ah okay, I understand the confusion. Each robot runs its own instance of that controller. In a separate process


does that make sense?

##### DrakerDG 01/10/2021 12:53:45
`@Simon Steinmann` yes, thank you 😁👍🏼


`@Simon Steinmann` like as running the same application several times


`@Simon Steinmann` If it were different robots if each one is configured with their controller, right?

##### Simon Steinmann [Moderator] 01/10/2021 12:56:16
yes


Usually a Controller is specific to one Robot, but you can have many instances of that Robot


you can take a look at your task-manager while running it. You should see the main Webots process, and then at least one per running controller.

##### DrakerDG 01/10/2021 12:58:23
`@Simon Steinmann` For example robots that play soccer use the same controller all, so

##### Simon Steinmann [Moderator] 01/10/2021 12:58:30
I mostly use Python and often need similar functionality. So I have sub-modules or scripts, which I dont change and can use in different controllers by simply importing them

##### DrakerDG 01/10/2021 12:59:09
Ok

##### Simon Steinmann [Moderator] 01/10/2021 13:00:01
If you have one type of robot player, then yes, it would make sense to use the same controller. Or at least a shared base-version for stuff like movement, sensors etc.


The roles for behavior, if they should differ, could be put on top of that


What Programming language do you use?

##### DrakerDG 01/10/2021 13:01:05
I will continue playing and I will try to make modules as you indicate.  It's very interesting


I am currently programming in C language, but I am learning in a course to start writing in Python


`@Simon Steinmann` My object is to use Webots to test robot designs, observe their operation, mechanical behavior, improve it and then physically build the robot

##### Simon Steinmann [Moderator] 01/10/2021 13:14:31
Python is definitely easier and quicker imho

##### DrakerDG 01/10/2021 13:17:40
`@Simon Steinmann`  definitely yes, the syntax is very friendly.  In the little that I have advanced I really like how simple it is


`@Simon Steinmann` Thank you very much for guiding me

##### AdityaDutt 01/10/2021 17:14:58
I opened up a new world in webots, and the console is flooded with errors that say [javascript] ReferenceError: Can't find variable: webots (undefined:1)



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/797876153640812614/unknown.png)
%end


would anyone know what the issue is? Thanks

##### adiagr 01/10/2021 17:30:24
The error for sure pertains to the robot window's javascript file. I'm also experiencing the same error with Webots 2021 version. Went through the changelog but couldn't find anything pertaining to webots.js

##### AdityaDutt 01/10/2021 17:30:50
should I stick with 2020 instead?

##### Darko Lukić [Cyberbotics] 01/10/2021 17:37:41
The robot window doesn't rely on webots.js. Could you tell me which world are you trying to open?

##### AdityaDutt 01/10/2021 17:39:01
I am using the erebus challenge world. [https://github.com/Shadow149/Erebus](https://github.com/Shadow149/Erebus). Under erebus-aura>game>worlds>GeneratedWorld.wbt


well i went back to 2020a rev 1 and the error is no longer there

##### Darko Lukić [Cyberbotics] 01/10/2021 21:00:41
`@AdityaDutt` , `@adiagr`  I have just checked and `MainSupervisorWindows.html` is not valid HTML file, so `webots` cannot be injected. The file content should be something like:

```html
<html>
<head>
    <script src="MainSupervisorWindow.js"></script>
</head>
<body>
...
</body>
</html>
```

##### adiagr 01/10/2021 21:03:17
Hey, in a robot window plugin, should the js file be explicitly included using the script tag?

##### Darko Lukić [Cyberbotics] 01/10/2021 22:03:29
Yes


You can check the e-puck example:

[https://github.com/cyberbotics/webots/blob/f6ebe9cf8ce64c30cf453655376a815deeb28744/projects/robots/gctronic/e-puck/plugins/robot\_windows/e-puck/e-puck.html#L76](https://github.com/cyberbotics/webots/blob/f6ebe9cf8ce64c30cf453655376a815deeb28744/projects/robots/gctronic/e-puck/plugins/robot_windows/e-puck/e-puck.html#L76)

##### Diego Rojas 01/10/2021 23:30:11
What are the steps of designing a model for webots simulation? What is the process of designing something in soldworks and making a PROTO from the mesh? This is likely a simple question, but I cannot seem to find a direct answer or demo.

##### DrakerDG 01/10/2021 23:40:04
`@Diego Rojas` check this play list: [https://youtube.com/playlist?list=PLbEU0vp\_OQkUwANRMUOM00SXybYQ4TXNF](https://youtube.com/playlist?list=PLbEU0vp_OQkUwANRMUOM00SXybYQ4TXNF)

##### Diego Rojas 01/11/2021 00:13:32
`@DrakerDG` Thanks!

##### Simon Steinmann [Moderator] 01/11/2021 09:15:52
Take a look at this first: [https://www.cyberbotics.com/doc/guide/tutorial-7-your-first-proto](https://www.cyberbotics.com/doc/guide/tutorial-7-your-first-proto)

In general, the simplest way in my opinion, would be to build up your model in webots, then export the robot as a .wbo


then you just need to add the .proto header (as described in the link)

##### JSK 01/11/2021 10:07:52
Hello Team Webots


i have a question related to webots


just visited the official website and then github and now i am here


Hi `@Darko Lukić`  how are you? remember me?

##### Darko Lukić [Cyberbotics] 01/11/2021 10:11:47
I am sorry, I cannot recognize your username 🙁

##### JSK 01/11/2021 10:12:22
i am Jamal Shams Khanzada, couple of days ago i had asked you a question on github and you helped me


and i am thankful to you


i am here for another problem to be solved.


can you still help me to solve it?


actually when i fetched gyro data recently, i started to make controller but it needs numpy

##### Darko Lukić [Cyberbotics] 01/11/2021 10:14:09
I recall now! I am glad I was able to help.


Feel free to ask a question and I, or somebody from the team, or the community, will probably be able to help you

##### JSK 01/11/2021 10:18:34
so when i imported numpy it says "ModuleNotFoundError: No module named 'numpy'"


although i have installed python latest version


you can see here
%figure
![Screenshot_from_2021-01-11_15-20-43.png](https://cdn.discordapp.com/attachments/565154703139405824/798134541603831808/Screenshot_from_2021-01-11_15-20-43.png)
%end


this is the error in webots terminal
%figure
![Screenshot_from_2021-01-11_15-22-51.png](https://cdn.discordapp.com/attachments/565154703139405824/798134959138930698/Screenshot_from_2021-01-11_15-22-51.png)
%end

##### Darko Lukić [Cyberbotics] 01/11/2021 10:23:46
You installed `numpy` for Python2, not Python3


You need to `pip3 install numpy`

##### JSK 01/11/2021 10:24:31
ok i am installing


but how can i know that `numpy` is for python2 or python 3?


installed it


rebooting is necessary or not?

##### Darko Lukić [Cyberbotics] 01/11/2021 10:26:11
No, just restart the controller


In general (not necessary), if you install it with `pip3` it is for Python3, if you install it with `pip` it is for Python2.

##### JSK 01/11/2021 10:27:33
ok


i restaretd the controller but the problem is still there

##### Darko Lukić [Cyberbotics] 01/11/2021 10:30:08
Do you get an error if you call?

```py
python3 -c 'import numpy'
```

##### JSK 01/11/2021 10:30:28
let me check it

##### Stefania Pedrazzi [Cyberbotics] 01/11/2021 10:30:57
`@JSK` did you maybe install Webots from the snap?

##### JSK 01/11/2021 10:31:31
`@Stefania Pedrazzi`  yes i installed it from snap

##### Stefania Pedrazzi [Cyberbotics] 01/11/2021 10:32:02
Ok, then you should use extern controller:

[https://www.cyberbotics.com/doc/guide/installation-procedure#extern-controllers](https://www.cyberbotics.com/doc/guide/installation-procedure#extern-controllers)

##### JSK 01/11/2021 10:32:52
negative... i got no error
%figure
![pip3.png](https://cdn.discordapp.com/attachments/565154703139405824/798137348495245312/pip3.png)
%end

##### Darko Lukić [Cyberbotics] 01/11/2021 10:34:13
If you installed Webots as `snap` it will not work. As `@Stefania Pedrazzi` suggested you should use external controller or install Webots as a Debian package.

##### JSK 01/11/2021 10:35:26
i checked that site. i have been reading it for two days but i was actually avoiding external controller. but now i think i have to switch here on external controller


i think i should go for Debian package


isn't it necessary that Debian should be installed on latest version of Ubuntu (20.04) i am using (18.04)


i have started downloading Debian Version.


and until it is downloaded i think i should reread the documentation `@Stefania Pedrazzi` mentioned me

##### Stefania Pedrazzi [Cyberbotics] 01/11/2021 10:42:32
There is a debian package version for Ubuntu 18.04: [https://github.com/cyberbotics/webots/releases/download/R2021a/webots-R2021a-x86-64\_ubuntu-18.04.tar.bz2](https://github.com/cyberbotics/webots/releases/download/R2021a/webots-R2021a-x86-64_ubuntu-18.04.tar.bz2)

##### JSK 01/11/2021 10:43:55
Thank you . I am so thankful to you. 🙂

##### Stefania Pedrazzi [Cyberbotics] 01/11/2021 10:44:16
You're welcome

##### JSK 01/11/2021 10:44:20
aaaa one last question please


after the file is downloaded, how to install it? i mean should i extract the `tar.bz2` extension file or something else?

##### Stefania Pedrazzi [Cyberbotics] 01/11/2021 10:45:58
Oops.. I linked to the tarball instead of the debian package

##### JSK 01/11/2021 10:46:00
please don't mind i am a novice

##### Stefania Pedrazzi [Cyberbotics] 01/11/2021 10:46:42
The tarball just needs to be extracted and then you can launch it from the directory where you extracted it


Here is the link to the debian package: [https://github.com/cyberbotics/webots/releases/download/R2021a/webots\_2021a\_amd64.deb](https://github.com/cyberbotics/webots/releases/download/R2021a/webots_2021a_amd64.deb)

Double clicking on it should open the Software Application window to install it

##### JSK 01/11/2021 10:47:34
alright that sounds good

##### Simon Steinmann [Moderator] 01/11/2021 10:48:29
`@JSK` You may also have to set environment variables, especially when using conda

##### JSK 01/11/2021 10:49:42
yeah i read about environment variables in the documentation that `@Stefania Pedrazzi`  mentioned

##### Simon Steinmann [Moderator] 01/11/2021 10:51:02
you can launch webots from a terminal. it will have the variables of that terminal

##### JSK 01/11/2021 10:51:44
to be honest i tried to set the variables but when i started to search `usr/local/webots` i was not able to find that and hence i came on this platform for help. i am trying this for 3 days

##### Simon Steinmann [Moderator] 01/11/2021 10:52:11
you can edit your .bashrc in your home folder to add variables

##### JSK 01/11/2021 10:53:05
what does "edit" mean.


?

##### Simon Steinmann [Moderator] 01/11/2021 10:53:17
open it in a text editor and add the variables


[https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers?tab-os=linux&tab-language=python](https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers?tab-os=linux&tab-language=python)


here is more detail

##### JSK 01/11/2021 10:54:51
alright

##### Simon Steinmann [Moderator] 01/11/2021 10:57:26
you can add this at the end of your .bashrc

`export WEBOTS\_HOME="/home/simon/webots"

export LD\_LIBRARY\_PATH="${LD\_LIBRARY\_PATH}:${WEBOTS\_HOME}/lib/controller"

export PYTHONPATH="${PYTHONPATH}:${WEBOTS\_HOME}/lib/controller/python38"

export PYTHONIOENCODING="UTF-8"`

keep in mind to change the first line to the correct Path of your installation, and change the python version to the correct one


`python3 --version`  to get your python3 version

##### JSK 01/11/2021 11:00:44
thanks let me do it

##### Simon Steinmann [Moderator] 01/11/2021 11:00:45
the .bashrc file gets sourced every time you open a new terminal. So you have to open a new terminal for it to take effect. Then you can launch webots from the terminal, and it will have those variables

##### JSK 01/11/2021 11:03:18
can you tell me how to access .bashrc file? i cant find it

##### Simon Steinmann [Moderator] 01/11/2021 11:03:28
it's hidden


ctrl + h to see hidden files, or `gedit ~/.bashrc` in the terminal

##### JSK 01/11/2021 11:04:05
found it


updated .bashrc as you mentioned `@Simon Steinmann` .

##### Simon Steinmann [Moderator] 01/11/2021 11:07:37
now you can open a new terminal and launch webots from there. the variables should be set


Instead of setting those variables in the .bashrc, you could set them in `sudo gedit /etc/profile`. That should set them system wide

##### JSK 01/11/2021 11:08:47
error still exists

##### Alessia Valle 01/11/2021 11:09:48
Hello everyone! I would like to write a controller that allows my robot to keep the lane. What sensors do you suggest me to use?

##### MartinG 01/11/2021 11:10:33
Use a camera sensor and look into canny edge detection and hough line transform


I don't know your programming language preference, but I strongly recommend you use python since OpenCV makes it a much easier task than it seems.

##### Darko Lukić [Cyberbotics] 01/11/2021 11:11:27
Something like this?

[https://github.com/lukicdarkoo/webots-example-lane-follower](https://github.com/lukicdarkoo/webots-example-lane-follower)


It is not robust, but it is very simple

##### Alessia Valle 01/11/2021 11:16:30
Yes, but I have never used OpenCV


Are there any references on Cyberbotics website?

##### MartinG 01/11/2021 11:19:03
I used a really solid (imo) guide that has some hand holding but also allows for you to figure the problems out by yourself, you can find it here [https://towardsdatascience.com/deeppicar-part-1-102e03c83f2c](https://towardsdatascience.com/deeppicar-part-1-102e03c83f2c)


Parts 3 and 4 are what you'll probably find the most useful.


And it does put up a paywall after 3 visits to the site I believe, but a bit of incognito browsing never hurt anyone.


I have a question of my own, using reset simulation crashes the whole program. The world is quite complex, however even leaving it to run for a long while after hitting reset doesn't seem to help.

##### Simon Steinmann [Moderator] 01/11/2021 11:23:12
Perhaps try removing things from the world one by one and hit reset after. Try to find out what is causing the crash

##### Alessia Valle 01/11/2021 11:25:21
Thank you!

##### MartinG 01/11/2021 11:26:05
I'm glad to help, I'll be around if you have some more specific questions.


Yeah, that makes sense. Thanks.


Anyone with some deeper knowledge of arctan2 and trigonometry in general want to help me out a bit?

##### Simon Steinmann [Moderator] 01/11/2021 14:16:06
`@MartinG` whaz do you want to k ow?


I got a pretty good handle on trigonometry and linear algebra, especially in the context of robotics and positions / orientations

##### MartinG 01/11/2021 14:20:37
Awesome.


So I'm trying to do a GPS guided navigation for the Tesla in Webots.


I do this by taking 5 points along the path, then converting them into a lot of points using a bspline path program.


The problem is figuring out how to calculate the angle the car needs to turn using arctan2

##### Simon Steinmann [Moderator] 01/11/2021 14:24:26

%figure
![220px-Atan2definition.png](https://cdn.discordapp.com/attachments/565154703139405824/798195623940718622/220px-Atan2definition.png)
%end

##### MartinG 01/11/2021 14:24:42
I've seen that graphic many times ahahah.

##### Simon Steinmann [Moderator] 01/11/2021 15:09:39
btw, this link can be very useful for checking calculations etc.

[https://www.andre-gaschler.com/rotationconverter/](https://www.andre-gaschler.com/rotationconverter/)

##### AdityaDutt 01/11/2021 16:42:55
Hi again! I am trying to create a robot class with the motors, position sensors, and other stuff included in it. I am using this code:

```C++
class myRobot {
  public:
    Robot *robot;
    Motor *left;
    Motor *right;
    PositionSensor *LEnc;
    PositionSensor *REnc;
    int timeStep;
};
```

It says that 'Robot' does not specify a type, and same with 'Motor' and 'PositionSensor'. I suspect this is because something in Robot.hpp is not public.


I'm sort of a noob with C++ and OOP, so I'm not really sure what the issue is

##### Simon Steinmann [Moderator] 01/11/2021 16:44:57
[https://www.cyberbotics.com/doc/guide/controller-programming?tab-language=c++](https://www.cyberbotics.com/doc/guide/controller-programming?tab-language=c++)


this should give a bit of guidance


and perhaps this

[https://www.cyberbotics.com/doc/guide/cpp-java-python?tab-language=c++](https://www.cyberbotics.com/doc/guide/cpp-java-python?tab-language=c++)

##### AdityaDutt 01/11/2021 16:47:08
thanks!

##### Chernayaten 01/11/2021 21:40:04
Would you perhaps be able to help me with the following? I want my robot to make a 90 degree clockwise turn which. I use setPosition to increment/decrement and thus move/turn the robot. I use a position sensor to properly stagger my commands so they don't overlap.



With my subpar trigonometry skills I figured that to turn the robot 90 degrees I have to inc/dec the position by (math.pi * robot\_radius)/ (2*wheel\_radius) . My simulations say I am completely wrong. Can you help me out? I can show how I reached the formula if you want

##### AdityaDutt 01/11/2021 21:53:28
funny I was doing the same thing today. This is the formula I landed on, and it works:

```
theta(in radians) = 
(angle(in degrees) * robot_radius * pi)/(360 * wheel_radius)
```


so if your robot radius(something you sort of have to play with to get accurate results) is 32.5, and wheel radius is 9.5, then your setPosition should be(in radians) (90 * 32.5 * PI)/(360*9.5)


pretty sure you forgot to multiply your angle and it should be 360*wheel\_rad

##### Chernayaten 01/11/2021 22:01:55
What do you mean it is something that I have to play around to get accurate results? Why?


Your formula translates to my\_formula / 2 since 90/360 = 1/4. Unfortunately the turn my robot does is still wrong

##### AdityaDutt 01/11/2021 22:14:02
it's not actually supposed to be the robot radius, it's meant to be the distance between the centers of the wheels, which is usually smaller than the robot radius given on the webots website


what does it do then?

##### Chernayaten 01/11/2021 22:15:37
Ahh.. yeah, I agree. The center of my wheels is right on the edge of the robot, in which case the distance should be the robot radius

##### AdityaDutt 01/11/2021 22:16:01
if it is extremely far off then there's something wrong with the formula


but if instead of 90 degrees it goes 80 or 100 degrees, then just mess with the robot radius


basically what my formula does is the following(90 degrees as an example)

1. 90/360 =1/4 shows that you are going 1/4 of the circumference.

2. calculate circumference through robot\_diameter*pi, and multiply by 1/4(or whatever fraction you got from step 1). This gives you the distance that each wheel has to go in terms of cm.


then the radians that each wheel has to turn is distance/wheelRadius


putting all that together gives the final formula

##### Chernayaten 01/11/2021 22:22:44
1/4 gives me a ~43.6 turning angle, 1/2 gives me a ~45.2 angle and 1/1 (so a full circle supposedly) gives me ~62 angle

##### AdityaDutt 01/11/2021 22:23:01
ooh


yeah


that's not right


this is what I have:

##### Chernayaten 01/11/2021 22:23:29
I do agree with your reasoning on how it works though

##### AdityaDutt 01/11/2021 22:23:34
```
(angle*WHEELBASE*PI)/(360*WHEELRAD)
```


WHEELBASE is the robot diameter


try that


and angle is in degrees

##### Chernayaten 01/11/2021 22:26:14
This is what I am trying. btw my result was in radians, I just converted it to angle so that it makes more sense. At least for me, angles feel far more natural when it comes to visualizing them

##### AdityaDutt 01/11/2021 22:26:15
*wheelbase is robot \_diameter\_, not radius


in case that was a matter of confusion


ok what's your wheelbase(\_diameter\_) and wheel radius

##### Chernayaten 01/11/2021 22:30:20
I'm pretty sure my wheelbase is the same as my robot diameter which is 0.9 and my wheel radius is 0.025.


My wheels are located on 0.045 to the left and the right of my robot, wheelbase is calculated as the distance from the center of the wheels which makes that 0.09


I'm using the robot from the pen.wbt world in the Devices Tour


I'll try playing around with the values to see if I can make it work. Thanks for taking the time, at least I know that my logic is not too far off

##### Luiz Felipe 01/12/2021 07:11:18
Just reporting. I think I found a bug (not sure), but receiving the message from the simulation server, when starting the Stream class another bar '/' is inserted in line 6 and the link becomes 'localhost:2001//mjpeg' ([https://github.com/cyberbotics/webots/blob/f6ebe9cf8ce64c30cf453655376a815deeb28744/resources/web/wwi/stream.js#L6](https://github.com/cyberbotics/webots/blob/f6ebe9cf8ce64c30cf453655376a815deeb28744/resources/web/wwi/stream.js#L6)) . For the x3d option the problem does not happen weirdly.


I am solving that by changing the message sent from the simulation server. I will check if it will also work with 'x3d' option or if using this solution only solves the 'mjpeg' but starts the error when using 'x3d'.

##### Olivier Michel [Cyberbotics] 01/12/2021 07:19:17
Thank you for this contribution! Once you have implemented and tested it, please open a new PR to submit your change, so that it will be integrated into Webots).

##### yash 01/12/2021 07:39:03
How can I extract the position moved by a manipulator link ? I have used a hinge joint along with position sensor .

So when a manipulator link hits a solid it stops, at this moment I want to know the position of the link .?

##### Olivier Michel [Cyberbotics] 01/12/2021 07:51:50
Can't you use the position sensor for that?

##### yash 01/12/2021 08:11:01
> Can't you use the position sensor for that?

`@Olivier Michel`  ye it’s done ! Was thinking in a wrong direction... thanks !

##### Luiz Felipe 01/12/2021 08:37:38
If I git clone the webots repository the dependencies are not downloaded right? How can I download also with the dependencies? (I cloned but the /wwi/dependencies folder does not exist)


I guess I should use --recurse-submodules... will test it!

##### Olivier Michel [Cyberbotics] 01/12/2021 08:45:02
Yes.

##### Simon Steinmann [Moderator] 01/12/2021 08:45:23
Use the build from source instructions on github


They give you a step for step process, including all dependencies

##### Olivier Michel [Cyberbotics] 01/12/2021 08:46:04
Please follow strictly the instructions at [https://github.com/cyberbotics/webots/wiki/Linux-installation](https://github.com/cyberbotics/webots/wiki/Linux-installation) (assuming you are on Linux)

##### Luiz Felipe 01/12/2021 08:49:05
Thanks 🙂 doing that now

##### Simon Steinmann [Moderator] 01/12/2021 08:51:44
`@AdityaDutt` `@Chernayaten`  [http://www.hmc.edu/lair/ARW/ARW-Lecture01-Odometry.pdf](http://www.hmc.edu/lair/ARW/ARW-Lecture01-Odometry.pdf) Take a look at this lecture presentation. Especially slide 23


delta-Theta is the angle change of the robot (you want 90° (pi/2)

delta-s  is the distance traveled by the right / left wheel
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/798475224294096907/unknown.png)
%end


2L is the distance between wheels. (as a note here, yesterday I found more accurate results using the outside edge to outside edge distance distance of the BoundingObject of the wheels, not the wheel center)


So solving for the difference in angles/position of your wheels, the formula should be like this:

`delta_pos = delta_theta * wheelBase / wheelRadius`


Now you can put in any theta angle in radians and get the delta\_pos


delta\_pos is how much more you have to turn one wheel over the other


I recommend staying with radians all the times. The only exception maybe being when printing a message. Then just multiply an angle by 180/pi  to get degrees

##### Luiz Felipe 01/12/2021 09:22:18
I followed the page, re-build webots, also installed the optional dependencies, but still nothing on the wwi/dependencies folder... should I download the dependencies in that folder manually?

##### Simon Steinmann [Moderator] 01/12/2021 09:23:19
what are you trying to achieve? Webots built successfully?

##### Luiz Felipe 01/12/2021 09:24:25
yups.. webots built successfully, i can open v2021a


i am trying to check a possible bug in the multimedia\_client.js file

##### Simon Steinmann [Moderator] 01/12/2021 09:24:56
okay... and what is the issue?

##### Darko Lukić [Cyberbotics] 01/12/2021 09:27:01
`@Luiz Felipe` Can you check whether you have node/npm installed, `npm --version`. If there is a missing dependency `make` skips compiling the corresponding part. Node should be included in the optional dependencies, but just to make sure whether it is properly installed

##### Luiz Felipe 01/12/2021 09:28:28
Probably that is the reason `@Darko Lukić` (npm is installed correctly but probably make skipped that when i ran the command)


does make clear works to re-build one more time?

##### Darko Lukić [Cyberbotics] 01/12/2021 09:29:20
Yes, `make clean` works

##### Olivier Michel [Cyberbotics] 01/12/2021 09:29:39
`make clean`

##### Darko Lukić [Cyberbotics] 01/12/2021 09:29:59
Correct, sorry

##### Luiz Felipe 01/12/2021 09:30:15
Thanks =), I will re-build

##### Olivier Michel [Cyberbotics] 01/12/2021 09:30:18
Or even more powerful `make cleanse` (will remove the dependencies).

##### Luiz Felipe 01/12/2021 09:31:38
For the web part, is there a list of bugs or features that need help? (I dont know much of javascript but I feel that part is really breakthrough work in  webots)


Now I got the dependencies `@Darko Lukić` . Thank you very much. I got an error during make regarding the npm but all the dependencies seems to be in the /dependencies folder.

##### Darko Lukić [Cyberbotics] 01/12/2021 09:44:24
What is the error?

##### Luiz Felipe 01/12/2021 09:47:20

%figure
![Screenshot_from_2021-01-12_18-40-36.png](https://cdn.discordapp.com/attachments/565154703139405824/798488275685605396/Screenshot_from_2021-01-12_18-40-36.png)
%end


but it works

##### Darko Lukić [Cyberbotics] 01/12/2021 09:51:21
Maybe to clean the npm cache `npm cache clean --force` and repeat the compilation. To clean the project you can also use `git clean -dfx`.

##### Luiz Felipe 01/12/2021 09:56:25
Ok, tomorrow I will try to compile again. I guess the bug is in this line: [https://github.com/cyberbotics/webots/blob/f6ebe9cf8ce64c30cf453655376a815deeb28744/resources/web/wwi/multimedia\_client.js#L77](https://github.com/cyberbotics/webots/blob/f6ebe9cf8ce64c30cf453655376a815deeb28744/resources/web/wwi/multimedia_client.js#L77) ... I will try to find a solution to send a PR for that


(giving the right url it works for the mjpeg also ;))

##### Apashe 01/12/2021 10:49:21
Hello everyone! recently started working with webots and I wanted to know how to get the current coordinates of the robot and point it to some point so that it gets there (I'm currently working with a robot "iRobot's Create")

how can this be done and is there an example?

now I want the robot to be able to drive from one current coordinate to another (without obstacles) on Rectangle Arena

##### Chernayaten 01/12/2021 10:50:09
GPS can get you current coordinates


Assuming you know the direction of your robot you can easily calculate the distance and the angle to your target

##### Apashe 01/12/2021 11:01:14
there are examples of how to use GPS

##### Chernayaten 01/12/2021 11:03:10
Yes, there's a gps.wbt file under the devices section.



GPS is simple to use, you can also check out its section oncthe webots site to see all the methods in your preferred language

##### owongcho 01/12/2021 16:39:37
hello! what type of simulation Webots generates? For example, is the type of simulation an emulation, discrete event simulation, or something else?

##### Olivier Michel [Cyberbotics] 01/12/2021 16:41:49
I would say emulation. The time is simulated and there is a fixed time step.

##### owongcho 01/12/2021 17:43:03
I see. thank you `@Olivier Michel`

##### Chernayaten 01/12/2021 19:14:50
Thanks for the information. I haven't looked at the lecture yet, but I will do so soon. I actually came to the same result as you, the outside distance seems to work better for some reason.



I also suspect that the initial anchor positions from the given example are wrong, unless I am mistaken. From Tutorial 6 on the 4 wheeled robot, the anchor field of the HingeJointParameters node should have the same values as the Solid translation one, but the robot of the pen.wbt world is missing the x values.



I'll review your information and hopefully get a good result. Thanks again (:

##### AdityaDutt 01/13/2021 00:49:06
I have the pioneer 3dx robot in between two walls. What would be a good way to ensure that the robot is straight/aligned with the wall? I tried checking two side sensors and making sure they return very similar values, or adjusting until they do, but the distance sensors are very noisy and not consistent much.



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/798715662129823805/unknown.png)
%end


How do I make sure the robot is straight?

##### TerryWr1st 01/13/2021 01:37:07
Have you specified a noise value in the sensor lookup table? Otherwise they should be accurate. 



Also, when your robot is in the above orientation, the distance sensor rays will be sent out on an angle (not perpendicular to walls as desired), which will increase/decrease your sensor values. So you would need to know your robot orientation for this approach as well

##### AdityaDutt 01/13/2021 01:37:32
wdym specify noise value?


I can pick how little/much noise I want?


and oh yeah I didn't think about the angle thing thanks

##### TerryWr1st 01/13/2021 01:41:31
[https://cyberbotics.com/doc/reference/distancesensor](https://cyberbotics.com/doc/reference/distancesensor)



Scroll down to the lookup table section

##### AdityaDutt 01/13/2021 01:42:37
wait so I can set my own noise stuff?


in code?


But if some competition is using webots that i'm participating in, what stops me from just setting 0 noise


how do I change the lookuptable values

##### TerryWr1st 01/13/2021 02:06:16
If the competition hasn't mentioned a restriction on sensor accuracy then dont set a noise. 



From the documentation, first column is sensor input, second column is what you want the input to represent, third column is the noise value. 



You can change these values in the scene tree (expand the fields of your distance sensor), or open the wbt file with a text editor

##### AdityaDutt 01/13/2021 03:05:56
got it thanks

##### Wasabi Fan 01/13/2021 04:52:17
I have a small projectile (on the order of 1cm diameter in the real world) that I'd like to simulate at moderate-to-fast speeds (tens of m/s) and detect contact with a particular square surface on a target robot. This is currently using a `TouchSensor`. Unfortunately, even with a projectile a few times larger and speeds a fraction of real-world, the simulation time step is too large to consistently (or, at higher speeds, essentially at all) have a time slice in which the projectile is actually intersecting the target. In other words, the collision is never detected. This is true even if I up the simulation speed to thousands of Hz, which isn't practical anyway. As far as I can gather, TouchSensor collision detection is purely based on intersection after a discrete time step.



One solution which seems technically possible but perhaps not practical is some form of intra-step interpolation (e.g., a ray intersection check which approximates my projectile as a point -- my understanding is that ODE supports this in some form).  What are others' thoughts here? And is there actually a practical means of implementation for the above in my Webots sim, with or (ideally) without a physics plugin? Alternately, this seems to be approaching the scale at which doing a purely mathematical hit-test without ever simulating the projectile may make more sense -- however, I \_do\_  need to simulate the time-of-flight, and I'm not sure what API would be sensible for computing those collisions, if any.

##### Luiz Felipe 01/13/2021 04:56:55
`@Wasabi Fan` , does decreasing the time step does not work even implementing a collision detector using a physics plugin following: [https://cyberbotics.com/doc/reference/introduction-of-the-physics-plugins](https://cyberbotics.com/doc/reference/introduction-of-the-physics-plugins) ?

##### Wasabi Fan 01/13/2021 05:01:28
I tried that once with a "normal" time-step (~60Hz) and it did not seem to change the results. Setting up the toolchain to compile physics plugins is a bit of extra work (which I preferred to not introduce into my general setup process), which is why I haven't done a lot of experimenting with it, but it's not out of the question. However, it's not clear to me what that sample plugin is intended to do -- is there an expectation that the joint-attachment collision handling is better in a scenario like this?

##### Olivier Michel [Cyberbotics] 01/13/2021 07:11:38
A possible solution (without a physics plugin) would be to turn the bullet into a Robot with the supervisor field set to TRUE. Then, add a distance sensor to the bullet looking backwards. At each time step, you should get the current position of the bullet and compute the vector between the current position and the previous position. You will use this vector to change the orientation of the distance sensor so that it looks exactly at the previous position and make a sensor measurement in the same time step. If the distance measured is smaller than the distance between the current and previous position, then a collision occurred and you should set the position of the bullet at the place of collision (which can easily be computed along the distance sensor ray).

##### Wasabi Fan 01/13/2021 07:16:36
Oh wow, that's very inventive and \_sounds\_ workable. I think there are some details I'd need to work out but I shall try that in the next few days and report back if I run into issues. Thanks!

##### Olivier Michel [Cyberbotics] 01/13/2021 07:17:28
I would truly appreciate your feedback on this.

##### Moha 01/13/2021 08:38:00
hi guys

when I run external controller for nao\_demo\_python using pycharm after mentioned configuration in related link, I see according errors in pycharm, anybody knows the solution ?



C:\Users\MoHa\AppData\Local\Programs\Python\Python39\python.exe C:/Users/MoHa/Desktop/0/nao/controllers/nao\_demo\_python/nao\_demo\_python.py

Traceback (most recent call last):

  File "C:\Users\MoHa\Desktop\0\nao\controllers\nao\_demo\_python\nao\_demo\_python.py", line 18, in <module>

    from controller import Robot, Keyboard, Motion

  File "C:\Users\MoHa\AppData\Local\Programs\Webots\lib\controller\python39\controller.py", line 11, in <module>

    webots\_home = os.environ['WEBOTS\_HOME']

  File "C:\Users\MoHa\AppData\Local\Programs\Python\Python39\lib\os.py", line 679, in \_\_getitem\_\_

    raise KeyError(key) from None

KeyError: 'WEBOTS\_HOME'



Process finished with exit code 1

##### Simon Steinmann [Moderator] 01/13/2021 08:40:21
[https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers?tab-language=python&tab-os=linux](https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers?tab-language=python&tab-os=linux)


you have to set your environment variablres

##### Moha 01/13/2021 08:41:58
in pycharm or windows environment variables ?

##### Simon Steinmann [Moderator] 01/13/2021 08:42:39
usually windows



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/798834582328967208/unknown.png)
%end


like this


also add the Path variables

##### Moha 01/13/2021 08:44:18
👍

##### yash 01/13/2021 11:00:50
Hi !  I want to use this supervisor function -  def addForce(self, force, relative). Therefore what exactly should I specify in the relative argument .?

##### Simon Steinmann [Moderator] 01/13/2021 11:23:34
You're right, the documentation is not 100% clear here. When you look at the C or C++ code, you see the type is BOOL


so I'd assume setting it to True = force relative to Solid, False = force relative to world


`@Olivier Michel` We should update the documentation

##### yash 01/13/2021 11:26:15
okay , let me try that.

##### Olivier Michel [Cyberbotics] 01/13/2021 11:28:03
It seems clear to me: [https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_add\_force](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_add_force)

##### Simon Steinmann [Moderator] 01/13/2021 11:28:31
when looking at the Python code, it is not clear that it is boolean

##### Olivier Michel [Cyberbotics] 01/13/2021 11:28:45
Feel free to suggest some change here: [https://github.com/cyberbotics/webots/edit/released/docs/reference/supervisor.md](https://github.com/cyberbotics/webots/edit/released/docs/reference/supervisor.md)

##### Simon Steinmann [Moderator] 01/13/2021 11:28:56
already at it 😉


[https://github.com/cyberbotics/webots/pull/2646](https://github.com/cyberbotics/webots/pull/2646)

##### Chernayaten 01/13/2021 11:34:43
From Tutorial 6 on the 4 wheeled robot, the anchor field of the HingeJointParameters node should have the same values as the Solid translation one. Is this correct?

##### Olivier Michel [Cyberbotics] 01/13/2021 11:37:19
Reviewed.


Yes.

##### Chernayaten 01/13/2021 11:41:13
There's a discrepancy in the pen.wbt device example

##### Olivier Michel [Cyberbotics] 01/13/2021 11:43:27
What is the problem?

##### Chernayaten 01/13/2021 11:44:30
The anchor field of the HingeJointParameters node is 0 0.025 0 whereas the Solid node (for the wheel) is -0.045 0.025 0 (and 0.045 for the other wheel)


This is in the Pen device example

##### Olivier Michel [Cyberbotics] 01/13/2021 11:49:28
This is correct, but is it not essential as the anchor may be anywhere on the rotation axis. However, you are right it is better to have it as close as possible to the rotating part (the wheel in this case) for stability reasons. May I let you propose a patch here: [https://github.com/cyberbotics/webots/edit/master/projects/samples/devices/worlds/pen.wbt](https://github.com/cyberbotics/webots/edit/master/projects/samples/devices/worlds/pen.wbt) ?

##### Chernayaten 01/13/2021 11:53:48
Done

##### Olivier Michel [Cyberbotics] 01/13/2021 11:55:40
Thank you. I approved it. You should be able to merge it once the CI tests are complete.

##### Chernayaten 01/13/2021 13:00:44
" Only those with write access to this repository can merge pull requests. "

##### Olivier Michel [Cyberbotics] 01/13/2021 13:06:56
OK, I merged it. Thank you.


I sent you a GitHub invitation to become "Committer" on this repo with write access.

##### AngelAyala 01/14/2021 12:53:23
Hi, I'm currently using a Reinforcement Learning algorithm with webots, and therefore it ia necessary to reset the simulation to its initiql state. A strange thing happens sometimes whe I'm testing the algorithm but webots start to use a huge memory amount reaching 5GB and it increases when the simulation is reset. I'm currently running three robots nodes and one is the supervisor as the agent interface, anyone else had been experiencing this or maybe you have any idea why it is happening?


On robot includes uses two display nodeeO


The other one is controlling the Mavic drone

##### Darko Lukić [Cyberbotics] 01/14/2021 12:57:13
Are you sure that Webots uses all that memory and not the controllers?


It would be great to isolate the issue and report it:

[https://github.com/cyberbotics/webots/issues/new?assignees=&labels=&template=bug\_report.md](https://github.com/cyberbotics/webots/issues/new?assignees=&labels=&template=bug_report.md)

##### AngelAyala 01/14/2021 12:58:49
In the system monitor it appears assigned to the webots - bin process


Any controller is executed isolated from main process right?

##### Darko Lukić [Cyberbotics] 01/14/2021 12:59:18
Yes

##### AngelAyala 01/14/2021 13:01:13
Ok I will poste there

##### Kirlin 01/14/2021 19:44:08
Hi, can someone help me about boundingObject Box ? I'm using a Free 3D Butter Stick model in a simulation (that will be part of a robotics immersion week in college), but the mesh has probably some internal conflicts, so if I use it as the boundingObject Box the Solid will actually tilt out of the screen.

My solution is use a box shape as the bounding box, since I dont need the object to be precisely cut. Anyway, when I put the Box as the boundingObject Box, it wont go to the center of the object, and, no matter how I change the dimensions of the Box, it wont fit the object as wished. So, here is my question: Can I move the "center" (in relation to the object)  of a boundingObject Box ?


here is the pic of the uncentered boundingObject Box
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/799363474299420732/unknown.png)
%end


PS: I'm on Ubuntu

##### Darko Lukić [Cyberbotics] 01/14/2021 19:55:04
You can add the `Box` node in a `Transform` node.

##### Kirlin 01/14/2021 19:56:45
Can you describe more how I do this ? I've never used a Transform node

##### Darko Lukić [Cyberbotics] 01/14/2021 19:59:54
It should be `boundingObject > Transform > children > Box`



[https://cyberbotics.com/doc/reference/transform](https://cyberbotics.com/doc/reference/transform)

##### Kirlin 01/14/2021 20:06:42
Wow, this is so useful, helped me pretty out, thanks so much!!

##### Wasabi Fan 01/15/2021 02:32:14
I'm taking a look at this in more depth, and now realize one part of my desired functionality is that I know \_what\_ the projectile hit. There is a particular surface on the robot (at the moment, I'm enclosing it in a TouchSensor boundary) which is being aimed at, and other parts of the robot (or elements of the surroundings) \_aren't\_ something I want to detect. In fact, at these speeds, there's some risk of going \_through\_ the robot and seeing the other target on the back of that robot.



I think this could be solved if there is a way to know what object the distance sensor is seeing -- is this a possibility? One option we see is to add a `Recognition` on a camera, with small FOV, pointing in the same direction as the distance sensor, and use that to disambiguate. I think it \_should\_ work, minus approximation error for the bounding boxes and any perf hit. Is there a more sensible way?

##### Olivier Michel [Cyberbotics] 01/15/2021 08:19:14
Yes, this should work but it looks a bit overkill. Instead of this, you could add `TouchSensor` nodes in the walls and when you move back the bullet into the wall, this touch sensor would be activated so that you know which wall was hit.

##### MartinG 01/15/2021 08:37:13
`@Wasabi Fan` I'd recommend taking a more mathematical approach to this, it would execute quite a bit faster than using all kinds of sensors and what not. If you can get the current position of both the projectile and your robot and you know the size of both of them, it's as easy as a couple if conditions to check for a collision.


There's a ton of information on collision detection online, however if you have any specific issue, I'd be glad to help.

##### Simon Steinmann [Moderator] 01/15/2021 08:58:34
Perhaps try running it without the displays. That is a lot of memory. Sounds to me like a lot of visual data is cumulatively buffered. You could also share your project (you can pm me too) and I'll have a look.


Oh and I have some experience with RL and Webots too, just fyi 🙂

##### yash 01/15/2021 09:02:55
this happens when I use the function---  addTorque()
%figure
![Screenshot_from_2021-01-15_14-31-59.png](https://cdn.discordapp.com/attachments/565154703139405824/799564262326272000/Screenshot_from_2021-01-15_14-31-59.png)
%end

##### Simon Steinmann [Moderator] 01/15/2021 09:08:49
In the Webots source code there is CCD functionality (Continuous collision detection). This would probably solve this problem. `@Olivier Michel` Is it possible to use that collision detection method? Either through a plugin or custom compile?

##### MartinG 01/15/2021 09:13:26
Where can I find it? I'd like to look through it.

##### Simon Steinmann [Moderator] 01/15/2021 09:14:13
[https://github.com/cyberbotics/webots/tree/master/src/ode](https://github.com/cyberbotics/webots/tree/master/src/ode)


libccd is the library


[https://github.com/danfis/libccd](https://github.com/danfis/libccd)

##### Olivier Michel [Cyberbotics] 01/15/2021 09:14:35
This might be possible but would certainly require a significant development effort.

##### MartinG 01/15/2021 09:14:54
Thanks.

##### Simon Steinmann [Moderator] 01/15/2021 09:17:42
I suspected as much. I hope something along these lines is somewhere on the roadmap. This is one of the few areas, where Webots is behind its competition: The low level control of physic engine settings and choice of different collision detection methods.


But I understand that it is not the highest priority and a lot of work

##### DrakerDG 01/15/2021 09:27:45
Hey, I tried to install webots on ubuntu but it tells me that it can't download the .deb file, apparently it can't find it. Does anyone know how to solve this problem?
%figure
![Screenshot_from_2021-01-15_03-23-15.png](https://cdn.discordapp.com/attachments/565154703139405824/799570511645376522/Screenshot_from_2021-01-15_03-23-15.png)
%end

##### Simon Steinmann [Moderator] 01/15/2021 09:30:02
what command did you use?


did you follow these steps? [https://cyberbotics.com/doc/guide/installation-procedure#installing-the-debian-package-with-the-advanced-packaging-tool-apt](https://cyberbotics.com/doc/guide/installation-procedure#installing-the-debian-package-with-the-advanced-packaging-tool-apt)

##### DrakerDG 01/15/2021 09:32:55
Yes:



wget -qO- [https://cyberbotics.com/Cyberbotics.asc](https://cyberbotics.com/Cyberbotics.asc) | sudo apt-key add -

sudo apt-add-repository 'deb [https://cyberbotics.com/debian/](https://cyberbotics.com/debian/) binary-amd64/'

sudo apt-get update

sudo apt-get install webots

##### mjc87 01/15/2021 09:34:28
`@DrakerDG` I noticed that there is a mistake in the install script it should be releases/download/R2021a/webots\_2021a\_amd64.deb I think

##### Simon Steinmann [Moderator] 01/15/2021 09:34:48
maybe download it manually. The version just changed to 2021. **[https://github.com/cyberbotics/webots/releases/tag/R2021a](https://github.com/cyberbotics/webots/releases/tag/R2021a)**


`@Olivier Michel` I think you should take a look and make sure the script is correct

##### DrakerDG 01/15/2021 09:35:43
Ok, I will try

##### MartinG 01/15/2021 09:36:28
What's the exact command you entered to download it? Can you copy it from your console?

##### Simon Steinmann [Moderator] 01/15/2021 09:36:42
he did

##### MartinG 01/15/2021 09:36:53
Oh, sorry I didn't see.

##### Olivier Michel [Cyberbotics] 01/15/2021 09:48:37
Which script are you referring to?

##### Simon Steinmann [Moderator] 01/15/2021 09:52:04
I dont know how apt works, but It looks for the wrong file. Read the last 4 lines of his console screenshot

##### Olivier Michel [Cyberbotics] 01/15/2021 09:53:27
Yes, I saw that, but there is no script about it.

##### Simon Steinmann [Moderator] 01/15/2021 09:53:57
what could cause this issue?

##### Olivier Michel [Cyberbotics] 01/15/2021 09:54:10
I don't know. I am investigating...

##### mjc87 01/15/2021 09:54:45
Yes, thats what I was referring, I also have very little knowledge of apt

##### yash 01/15/2021 09:55:35
I am facing an issue where my controller is crashing when I use the supervisor function - addTorque()
%figure
![Screenshot_from_2021-01-15_14-31-59.png](https://cdn.discordapp.com/attachments/565154703139405824/799577517596868608/Screenshot_from_2021-01-15_14-31-59.png)
%end

##### MartinG 01/15/2021 09:56:06
I think seeing your code and world would be helpful to get a better idea of what might be wrong.

##### yash 01/15/2021 09:56:38
sure



> **Attachment**: [force\_control.py](https://cdn.discordapp.com/attachments/565154703139405824/799578002249089053/force_control.py)


this is the controller code

##### Olivier Michel [Cyberbotics] 01/15/2021 09:58:21
Are you on Ubuntu 20.04?

##### MartinG 01/15/2021 10:00:58
I think that the problem is in your last line of code, f=Node.addForce(hf,force,relative=False)

##### yash 01/15/2021 10:01:15
yes but how to rectify it

##### MartinG 01/15/2021 10:01:22
You're calling addForce from the Node object instead of on the node that you want it to be executed on.


actually no


try changing f to hf


I saw that you defined the foot as hf before the while loop


That is most likely your problem.

##### yash 01/15/2021 10:04:06
so should I define hf=robot.getFromDef("human\_foot")   in the while loop ?

##### MartinG 01/15/2021 10:04:31
No, in your last line just put an h before f


hf=Node.addForce(hf,force,relative=False)


I’m not sure that will work, but f doesn’t exist prior to that so it’s a likely cause.

##### yash 01/15/2021 10:05:46
its the same issue still, didnt work

##### MartinG 01/15/2021 10:06:05
Okay, give me five minutes.


I'll message you privately to not crowd the chat here.

##### Olivier Michel [Cyberbotics] 01/15/2021 10:38:38
I believe the problem is now fixed (it was due to a misconfiguration of our debian server). Can you try again and report and success or failure?

##### DrakerDG 01/15/2021 10:46:47
`@Olivier Michel` Ok, at the moment you are downloading the binary file

##### mjc87 01/15/2021 10:46:57
`@Simon Steinmann` I've seen you've done some domain randomisation in the past, is it possible to introduce randomness to world descriptions so that blocks appear in a different place each time a simulation is started?

##### Olivier Michel [Cyberbotics] 01/15/2021 10:56:24
Great, let me know if that finally works.

##### Simon Steinmann [Moderator] 01/15/2021 10:57:59
`@mjc87` [https://drive.google.com/file/d/1Z2332YFU1Um1NCwx3PoTcOE2jdu6QUxP/view?usp=sharing](https://drive.google.com/file/d/1Z2332YFU1Um1NCwx3PoTcOE2jdu6QUxP/view?usp=sharing) I did this a while back. Domain Randomization to create cv training dataset

##### mjc87 01/15/2021 10:59:55
Thanks, please could you give me a pointer how you implemented it? I'm trying to generate a student robot challenge where some 'targets' are randomly placed

##### Simon Steinmann [Moderator] 01/15/2021 11:00:40
let me see if I can find that project


`@mjc87` [https://drive.google.com/file/d/1vp4ZzyA44JpcGbI5NEUsCNZ7eRFrYd4g/view?usp=sharing](https://drive.google.com/file/d/1vp4ZzyA44JpcGbI5NEUsCNZ7eRFrYd4g/view?usp=sharing) take a look at this


the large size is due to the textures

##### DrakerDG 01/15/2021 12:22:32
It is works 😆 thank you)



%figure
![Screenshot_from_2021-01-15_05-53-32.png](https://cdn.discordapp.com/attachments/565154703139405824/799614996047986708/Screenshot_from_2021-01-15_05-53-32.png)
%end



%figure
![Screenshot_from_2021-01-15_05-55-28.png](https://cdn.discordapp.com/attachments/565154703139405824/799615195914960906/Screenshot_from_2021-01-15_05-55-28.png)
%end

##### Olivier Michel [Cyberbotics] 01/15/2021 13:03:56
Nice! Thank you for the feedback.

##### R\_ 01/15/2021 13:57:14
[https://stackoverflow.com/questions/64875476/reinforcement-learning-webots-simulator](https://stackoverflow.com/questions/64875476/reinforcement-learning-webots-simulator)

##### valeria 01/15/2021 14:49:49
Hello, can anyone help me out? This is my very first time using webots and I'm using VS to write my Java code, however when I saved the file some "import errors" appeared. I have installed JDK and I use macOS then I don't understand why I have this. I also tried to clean the workspace but didn't work. Does anyone if I need to install something else in order to make it work?
%figure
![Screenshot_2021-01-15_at_15.48.25.png](https://cdn.discordapp.com/attachments/565154703139405824/799651562208755773/Screenshot_2021-01-15_at_15.48.25.png)
%end

##### Simon Steinmann [Moderator] 01/15/2021 18:27:02
`@valeria` [https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers?tab-os=macos&tab-language=java](https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers?tab-os=macos&tab-language=java) This may help. You have to set up some environment variables when using extern controllers (your own IDE like VC)

##### devDbb 01/16/2021 15:58:04
I am trying to create a keyboard controller to my robot but it says INFO: my\_controller3: Starting controller: python.exe -u my\_controller3.py

WARNING: my\_controller3: The process crashed some time after starting successfully.

WARNING: 'my\_controller3' controller crashed.
%figure
![Cnt1.JPG](https://cdn.discordapp.com/attachments/565154703139405824/800031127715708938/Cnt1.JPG)
%end



%figure
![cnt2.JPG](https://cdn.discordapp.com/attachments/565154703139405824/800031139582574592/cnt2.JPG)
%end


Can anyone help me

##### Apashe 01/16/2021 17:59:25
Hello, What is the diameter of the wheels at iRobot's Create??

##### Chernayaten 01/16/2021 18:56:12
" Note [Python]: In C++, Python and Java the keyboard functions are in a dedicated class called Keyboard. In order to get the Keyboard instance, you should call the getKeyboard function of the Robot class. "


This is what I read about the Keyboard class, whereas in your code you do kb = Keyboard()


I believe it is 0.062

##### Apashe 01/17/2021 14:04:46
in general, where can I see all the data on the robot iRobot's Create as on the E-puck robot?

Can I have a link?

##### Chernayaten 01/17/2021 15:22:10
Robots are located under \Webots\projects\robots . From there you can navigate to the robot you can and find the protos folder (if there is one) and find the proto file you want. So for example the Create robot proto file for me is located under "C:\Program Files\Webots\projects\robots\irobot\create\protos" and the e-puck one under " C:\Program Files\Webots\projects\robots\gctronic\e-puck\protos" 



You can read some more on the PROTO mechanism here: [https://cyberbotics.com/doc/reference/proto](https://cyberbotics.com/doc/reference/proto)


`@Apashe`

##### Tahir [Moderator] 01/18/2021 11:00:32
Hello,

How can we access individual value inside the SFVEC3F in proto files?


For example:

position IS frontRightLeg.x

Or

position IS frontRightLeg[0]


These both gave errors

##### Olivier Michel [Cyberbotics] 01/18/2021 11:02:58
You cannot based on the `IS` keyword. Instead, you should rely on Lua scripting to extract the individual components of a `SFVec3f`.

##### Tahir [Moderator] 01/18/2021 11:04:34
OK than instead 

position IS frontRightLegHip


Where frontRightLegHip is SFFLOAT


This should work


?

##### Olivier Michel [Cyberbotics] 01/18/2021 11:08:55
Yes, see an example of this page: [https://cyberbotics.com/doc/reference/procedural-proto-nodes](https://cyberbotics.com/doc/reference/procedural-proto-nodes)


In particular `fields.stepSize.value.x` to extract X component from `SFVec2f stepSize`.

##### Tahir [Moderator] 01/18/2021 11:19:30
OK thanks alot

##### ahforoughi 01/18/2021 11:58:50
Hi guys, How can we use openai gym in this platform? i want to simulate and train a robotic arm

##### MartinG 01/18/2021 11:59:48
I'll go ahead and guess that the Supervisor functionality is going to be your best friend for that.

##### Darko Lukić [Cyberbotics] 01/18/2021 13:04:02
You may find the following project relevant:

[https://github.com/aidudezzz/deepbots](https://github.com/aidudezzz/deepbots)

##### John520 01/18/2021 17:11:34
Hi guys, would it be possible to create a crop field, for example a canola field, and a tractor can go inside the field to harvest? Thank you very much for your help!

##### Simon Steinmann [Moderator] 01/18/2021 17:13:46
Probably depends on how visually accurate you want it. The simplest method I can imagine, is using a texture of the field, and then paining with the Pen node on top of that picture, where the tractor has harvested


[https://cyberbotics.com/doc/reference/pen](https://cyberbotics.com/doc/reference/pen)


that would probably have the least performance impact

##### John520 01/18/2021 17:26:36
Thank you very much for your help, `@Simon Steinmann` ! Except for creating a field of canola, I would also like to use a Lidar sensor to measure the height of the canola so that the harvesting header in front of the tractor can adjust its height accordingly for better harvesting. Is it possible to do so? Thanks a lot.


In other words, could the canola field be created with different heights?

##### Simon Steinmann [Moderator] 01/18/2021 17:31:01
[https://cyberbotics.com/doc/reference/elevationgrid?tab-language=python](https://cyberbotics.com/doc/reference/elevationgrid?tab-language=python)


perhaps this


I dont know how you would harvest it though

##### John520 01/18/2021 17:40:51
Thank you, `@Simon Steinmann`. Can the tractor go inside the field created by the elevationGrid? Can a lidar sensor detect the field?

##### Simon Steinmann [Moderator] 01/18/2021 18:45:07
it should be possible. You can disable collision and only have the visual Grid

##### John520 01/18/2021 20:56:29
Thank you `@Simon Steinmann`. Sounds great! I am going to try it out. Would it be possible to model a simple header (like a rectangular shape) and attach it to the tractor?

##### Homeralf 01/18/2021 23:56:27
Hi guys,



I would like to thank the team for this great simulator, the API and documentation are so great. 

I'm a hobbyist trying to develop a muscle-based robot and I've encountered a couple of issues.



I want to carry a simulation from a single python process.

As far as I know, each robot's external controller must be contained in its own process, so I spawn multiple processes from the main process (with the corresponding environment variables).

The actual problem comes when sharing data among processes. The objects I try to share can't be pickled (serialized) because they contain SwigPyObjects. I can solve this but is cumbersome and not idiomatic.



So I have a couple of questions I hope you can help me with:

Is there a way to run controllers without spawning additional processes?

I guess the serialization problem can't be solved because of Swig, however, I think an update on the python API is on the roadmap. It is possible this may solve this issue?





Thanks in advance!

##### Simon Steinmann [Moderator] 01/19/2021 08:19:41
`@Homeralf` Taking a step back, why do you want it to run from the same process? What data do you want to share?


In general [https://docs.python.org/3/library/multiprocessing.html](https://docs.python.org/3/library/multiprocessing.html) can work really well for parallel processing and data sharing. Then there is middlewares such as ROS. Webots itself has the Transmitter and Receiver node, which you can use to send data between Robots

##### Darko Lukić [Cyberbotics] 01/19/2021 08:45:08
Hello `@Homeralf`. I am glad you like Webots. As you mentioned, the objects cannot be serialized because of SWIG. If you saw PR with objective to move away from SWIG in favor of Python native bindings then I believe that will not solve your problem. Maybe we can do something like this:

[https://stackoverflow.com/questions/9310053/how-to-make-my-swig-extension-module-work-with-pickle](https://stackoverflow.com/questions/9310053/how-to-make-my-swig-extension-module-work-with-pickle)


You cannot run the controllers in the same process

##### Simon Steinmann [Moderator] 01/19/2021 09:29:02
You also might want to take a look at pyro5. Could be exactly what you need

##### Homeralf 01/19/2021 09:42:57
Hello `@Simon Steinmann`



I want to run it from the same process because among other things I want to automate multiple simulations based on evolutionary algorithms.

The controllers must receive input from higher abstractions like the activations of the muscles, where the robot should be headed, the target speed of a solid, etc.



Yes, I was using the BaseManager from multiprocessing to share the data. However, it wouldn't work because the objects can't be serialized.

I would like to keep the project as lean as possible and I think using ROS may be a little bit overkill for now but I will consider it if things get complicated (I may be wrong).

Transceiver and receiver nodes seem like a good solution among robots.

Didn't know about pyro5, looks promising! I will give it a try!



Thanks for your time Simon, really appreciated it.


Hi `@Darko Lukić`,



Yes, that was the PR I saw.

Nice, looks exactly like the solution. I will try to implement it as soon as I get back from work.



Thanks for answering the questions, you've helped me a lot.

##### gaitt 01/19/2021 10:20:48
Hi guys, 

I'm building a tutorial for our software. The tutorial use Webots and ROS2. So far, so good. 

Basically, I have a scene with a UR5e robot and some objects. We execute some trajectories with the followJointTrajectory action.

I would like now to go further and to do so, I need to know the position of some Webots objects in ROS2. For example, the robot move a bottle and I would like to get the bottle position to be published in ROS2 through a geometry\_msgs::Pose. 

Any idea how I can achieve this using C++/ROS2 API?

##### Darko Lukić [Cyberbotics] 01/19/2021 10:29:20
`@gaitt` You can use Supervisor:

[https://www.cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_get\_position](https://www.cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_get_position)



Do you use the `webots_ros2` package or you created your own interface?

##### Simon Steinmann [Moderator] 01/19/2021 10:31:16
If you want positions of objects relative to others (your robot for example), then you can use this script I made. Or you can at least take inspiration from it and create it in c++
> **Attachment**: [get\_relative\_position.py](https://cdn.discordapp.com/attachments/565154703139405824/801036050230673468/get_relative_position.py)

##### gaitt 01/19/2021 10:31:26
Yes I already use the  `webots_ros2` package.

##### Simon Steinmann [Moderator] 01/19/2021 10:32:11
The supervisor only gives you absolute (world) coordinates. For inverse kinematics you usually need it realtive to the robot base


If you end up creating something like this in c++, it would be nice if you could share

##### gaitt 01/19/2021 10:38:43
Thanks `@Simon Steinmann`, well absolute position is what I need.


I was thinking there was an easier solution that writing a new ROS2 node.

##### Simon Steinmann [Moderator] 01/19/2021 10:40:46
You need a supervisor controller (can be your ur5e). It can then get and publish the positions of anything in the simulation

##### yash 01/19/2021 11:39:11
Hi ! I am using wb\_motor\_get\_torque\_feedback(),  to get torque of motors for a 2R manipulator. But the function returns me the same value of torque for all the motors ? What could be the issue ?

##### Simon Steinmann [Moderator] 01/19/2021 11:47:41
can you post your code or controller file?

##### yash 01/19/2021 11:48:01
alright



> **Attachment**: [force\_control.py](https://cdn.discordapp.com/attachments/565154703139405824/801055822753955870/force_control.py)

##### Simon Steinmann [Moderator] 01/19/2021 11:53:24
line 104-108


you use the same motor


that might be it 😉

##### yash 01/19/2021 11:58:30
shit yes ! thanks a lot , very silly thing.

##### Simon Steinmann [Moderator] 01/19/2021 11:58:50
it happens :p

##### yash 01/19/2021 11:58:57
I would like to plot these values , how can it be done ?

##### Simon Steinmann [Moderator] 01/19/2021 11:59:24
double click on the robot, it should be plotted there


the robot window

##### yash 01/19/2021 12:06:07
No, but these are values that give torque feedback and doesn’t take any sensor into Accout


account*

##### Simon Steinmann [Moderator] 01/19/2021 12:06:56
then you have to either make a custom robot window, or plot it yourself with a python library


or use external IDEs

##### yash 01/19/2021 12:08:01
understood, I am using matplotlib, but I plot them with respect to time right i.e robot.step(timestep)?

##### Simon Steinmann [Moderator] 01/19/2021 12:08:51
time = robot.getTime()

##### yash 01/19/2021 12:09:06
oh okay

##### Simon Steinmann [Moderator] 01/19/2021 12:09:17
that is the simulation time you see in Webots


which is increased by the timestep amount every step

##### yash 01/19/2021 12:11:29
ok, thank you very much

##### Ishi\_Senpai 01/19/2021 14:59:32
hello is there any tutorials on how to creat a snake like robot in webots as i know you have the yamor but i need to make an orrigonal one but its my first time using the software]

##### gaitt 01/19/2021 15:07:29
Can I retrieve a Supervisor from an already running instance of webots?

##### Simon Steinmann [Moderator] 01/19/2021 15:23:29
A supervisor is used like the robot node. So you have to start a controller of a robot, which has supervisor = true checkmark in the scene tree


And instead of robot =Robot() you do robot = Supervisor()

##### gaitt 01/19/2021 15:37:11
Ok I managed to get it! It look likes, if webots is running I can't get the handle. I need first to create the supervisor and then launch webots to get the handle properly. Now let see how to parse the rootNode ...

##### Simon Steinmann [Moderator] 01/20/2021 08:49:55
`@gaitt` A supervisor needs to be initialized. For that a robot controller needs to be launched. Usually all robots controllers are launched, when the simulation starts. Through the use of a supervisor controller, you can start and stop other controllers. But I dont think you can start your first supervisor controller later on in the simulation.


You should also always have this open in a tab: [https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python](https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python)

You'll have to reference it constantly. Luckily it is very detailed. Also remember, that the `Supervisor() `also has all the functionality of `Robot()`. So you dont have to change any of your existing code, should you chose to turn your ur5e into a supervisor. All motor / sensor controls work the same

##### Majanao 01/20/2021 11:31:45
Hi all, I would like to balance the camera of the DJI Mavic. Is it possible to rotate it through a python script or do I have to add two motors to the drone model?

##### Olivier Michel [Cyberbotics] 01/20/2021 11:33:53
Sure, this is possible. You will have to edit the drone model (PROTO file) to add this capability.


In fact, you don't need to edit the drone model. Instead you should create a pan tilt camera proto and add it in the `cameraSlot` of the Mavic2Pro model so that it is used instead of the default `Camera` node.

##### Majanao 01/20/2021 11:53:47
Thanks a lot. I'll try that.

##### gaitt 01/20/2021 17:15:06
`@Simon Steinmann` Ok I see. Thanks a lot. 

Another question: can we add other sensors to a predefined robot. I use the predefined UR5e and so the controller is external. 

My goal would be to add a fixed rangefinder to the UR5e base and access it from my ROS2 node to process the depth image.


BTW, is there somewhere a piece of code to transform a rangefinder depth image to 3d points in world coordinate frame?

##### Apashe 01/20/2021 17:28:43
Hello, what is the distance between the wheels and the radius of the wheels themselves for the robot E-puck?

I did self.wheel\_radius = 0.021 and self.distance\_between\_weels = 0.052

but I don't have the full turn angle converge (I do it through the values PositionSensor)

##### Darko Lukić [Cyberbotics] 01/20/2021 17:31:46
`@Apashe` This may help:

[https://github.com/cyberbotics/webots\_ros2/wiki/Tutorial-E-puck-for-ROS2-Beginners#differential-drive-calibration](https://github.com/cyberbotics/webots_ros2/wiki/Tutorial-E-puck-for-ROS2-Beginners#differential-drive-calibration)


Yes, you can add any node to `toolSlot`


```py
def depth2pointcloud(depth_image, k_matrix):
    inv_fx = 1.0 / k_matrix[0, 0]
    inv_fy = 1.0 / k_matrix[1, 1]
    ox = k_matrix[0, 2]
    oy = k_matrix[1, 2]
    image_height, image_width = depth_image.shape
    points = np.zeros((image_width * image_height, 3), dtype=np.float32)
    counter = 0
    for y in range(image_height):
        for x in range(image_width):
            dist = depth_image[y, x]
            # This follows the coordinate system of the gripper
            points[counter, 1] = -np.float32((x - ox) * dist * inv_fx)
            points[counter, 2] = -np.float32((y - oy) * dist * inv_fy)
            points[counter, 0] = np.float32(dist)
            counter += 1
    return points[:counter].astype(np.float32)


def get_calibration_matrix_from_range_finder(range_finder):
    image_width = range_finder.getWidth()
    image_height = range_finder.getHeight()
    focal_length = 0.5 * image_width * (1 / math.tan(0.5 * range_finder.getFov()))
    k_matrix = np.array([
        [focal_length, 0, image_width / 2],
        [0, focal_length, image_height / 2],
        [0, 0, 0]
    ])
    return k_matrix


def get_point_cloud_from_range_finder(range_finder):
    depth_image = np.asarray(range_finder.getRangeImage(), dtype=np.float32)
    depth_image = depth_image.reshape((-1, range_finder.getWidth()))


    k_matrix = get_calibration_matrix_from_range_finder(range_finder)
    return depth2pointcloud(depth_image, k_matrix)
```

This snippet creates a point cloud in the range finder's reference frame.

##### gaitt 01/20/2021 17:38:51
Thanks a lot, much appreciated.

##### Apashe 01/20/2021 17:39:00
I don't use ros

##### Darko Lukić [Cyberbotics] 01/20/2021 17:39:55
I know, but the principle is the same. You rotate the robot and move it along the line to calibrate it

##### Apashe 01/20/2021 17:40:19
I used the standard example for PositionSensor from Webots and [https://www.youtube.com/watch?v=WSjTWcTojHg&t=536s](https://www.youtube.com/watch?v=WSjTWcTojHg&t=536s)

##### Darko Lukić [Cyberbotics] 01/20/2021 17:41:01
`@Apashe` 

```py
DEFAULT_WHEEL_RADIUS = 0.02
DEFAULT_WHEEL_DISTANCE = 0.05685
```

You can use these values

##### gaitt 01/20/2021 17:42:35
I did try `toolSlot` but as I expected it moves when the robot moves, I would like my rangefinder to be in a fixed frame

##### Apashe 01/20/2021 17:42:36
Thank you I'll go try it

##### Darko Lukić [Cyberbotics] 01/20/2021 17:43:33
I see, in that case you can add it in directly in the world

##### gaitt 01/20/2021 17:53:08
I'm not sure how, could you elaborate? Device node must belong to a robot node, right?

##### Darko Lukić [Cyberbotics] 01/20/2021 19:13:51
Yes, you would have two robot and two controllers, one of them just publishing the point cloud


Alternately, you can add the range finder to the UR PROTO directly

##### gaitt 01/21/2021 08:23:49
Ok thanks, I will try that!

##### MartinG 01/21/2021 10:22:45
Anyone have some tips on getting started with Lidars in Webots?

##### Simon Steinmann [Moderator] 01/21/2021 10:46:21
Read the API documentation and just try them out :)

##### Tej 01/21/2021 14:55:02
good afternoon fellow robots

##### Apashe 01/21/2021 17:54:39
sending the robot coordinates from the supervisor to E-puck robot, but the data is not coming in

message = struct.pack('>7f', robot\_coords[0][0], robot\_coords[0][1], robot\_coords[0][2], robot\_coords[1][0], robot\_coords[1][1], robot\_coords[1][2], robot\_coords[1][3])

    emitter.send(message)



what could be the problem?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/801872405399601182/unknown.png)
%end

##### fowzan 01/21/2021 17:59:09
Hi guys


Is there any simulation links available for Spot mini ?


`@Darko Lukić`


`@Olivier Michel`


I would love to simulate spot mini on my terminal , could you share links if available

##### Apashe 01/21/2021 18:26:40
I realized that the error in

self.receiver = self.getDevice('receiver')

if convert root to to base node and add receiver (with your hands and delete the old one)



everything works

why so?


the robot also has receiver, why doesn't it work?


how to connect to receiver without convert root to base node

##### Darko Lukić [Cyberbotics] 01/21/2021 19:06:06
[https://cyberbotics.com/doc/guide/spot](https://cyberbotics.com/doc/guide/spot)

##### fowzan 01/22/2021 08:42:53
Thank you


Is there any simulation for fleet management systems `@Darko Lukić`

##### Darko Lukić [Cyberbotics] 01/22/2021 08:45:02
Something like this:

[http://www.diegoantognini.com/projects/dis/](http://www.diegoantognini.com/projects/dis/)

##### Apashe 01/22/2021 08:45:11
Help me, please

##### Darko Lukić [Cyberbotics] 01/22/2021 08:47:12
I don't understand the issue. Can you send us a minimal example of the project so we can reproduce the problem?

##### Apashe 01/22/2021 08:52:58

> **Attachment**: [my\_odometry.7z](https://cdn.discordapp.com/attachments/565154703139405824/802098472752513044/my_odometry.7z)

##### fowzan 01/22/2021 08:54:23
What are the commands to move spot mini in webots ? I’m unable to navigate it
%figure
![image0.jpg](https://cdn.discordapp.com/attachments/565154703139405824/802098830870839366/image0.jpg)
%end


`@Darko Lukić`


Kindly share the commands

##### Darko Lukić [Cyberbotics] 01/22/2021 08:59:08
Your emitter and receiver don't use the same channel. Either change the `receiver_channel` value in the e-puck node, or `Emitter > channel` in the supervisor node.


[https://cyberbotics.com/doc/reference/nodes-and-api-functions](https://cyberbotics.com/doc/reference/nodes-and-api-functions)



But please go through the tutorials first:

[https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials)



Here is an example for Spot:

[https://github.com/cyberbotics/webots/blob/master/projects/robots/boston\_dynamics/spot/controllers/spot\_moving\_demo/spot\_moving\_demo.c](https://github.com/cyberbotics/webots/blob/master/projects/robots/boston_dynamics/spot/controllers/spot_moving_demo/spot_moving_demo.c)

##### Apashe 01/22/2021 09:07:10
thanks

##### fowzan 01/22/2021 09:08:08
Where should I change this ? Is it in the script


I tried but apart from raising the legs it is not navigating `@Darko Lukić`


Kindly help me out

##### MartinG 01/22/2021 09:49:48
getRangeImage and getLayerRangeImage both return the same amount of elements, the documentation says that getLayerRangeImage should only return the values of the requested layer.

##### Darko Lukić [Cyberbotics] 01/22/2021 10:02:11
`@MartinG` Can you confirm that `getRangeImage` is not equal to 1

##### MartinG 01/22/2021 10:06:37
I can now, just checked.

##### Darko Lukić [Cyberbotics] 01/22/2021 10:18:49
It indeed seems like a bug (I can reproduce it). Could please report the issue:

[https://github.com/cyberbotics/webots/issues/new?assignees=&labels=&template=bug\_report.md](https://github.com/cyberbotics/webots/issues/new?assignees=&labels=&template=bug_report.md)

##### MartinG 01/22/2021 10:19:11
Will do. Thanks.

##### gaitt 01/22/2021 10:36:59
Hey Darko, so I have tried the first approach. I added a 2nd robot with only the kinect in it. The robot controller is set to extern, supervisor is true for all robot. But when I launch the "webots\_robotic\_arm\_node", it seems to me that the joint state publisher get stuck! My custom ROS2 node seems to behave correctly and is loaded as controller for the 2nd robot. Can we have multiple supervisor, 1 per robot?

##### Darko Lukić [Cyberbotics] 01/22/2021 10:40:21
`@gaitt` Check this example on how to have more than 1 robot in a simulation:

[https://github.com/cyberbotics/webots\_ros2/blob/master/webots\_ros2\_demos/launch/armed\_robots.launch.py#L31-L45](https://github.com/cyberbotics/webots_ros2/blob/master/webots_ros2_demos/launch/armed_robots.launch.py#L31-L45)



The ROS2 interface will not handle two robots by itself as it cannot know how to assign controllers.

##### JSK 01/22/2021 10:49:48
hi fellows


how does the saveExperimentData() works in webots controller terminating?


It says  "NameError: name 'saveExperimentData' is not defined"

##### Darko Lukić [Cyberbotics] 01/22/2021 10:53:29
The `saveExperimentData` function is a placeholder function, you should implement it by yourself, it is just an example.

##### JSK 01/22/2021 11:00:23
ok

##### msoyer34 01/22/2021 22:44:43
Hey guys, I would like to ask a question



I am adding my own obj data to make a conveyor belt simulation but i fail to bound objects.



WARNING: DEF Simulation Robot > DEF stand\_pulley2 Solid > USE stand\_pulley2 > Shape > IndexedFaceSet: Mass properties computation failed for this IndexedFaceSet: the corresponding added mass defaults to 1kg, the added inertia matrix defaults to the identity matrix.Please check this geometry has no singularities and can suitably represent a bounded closed volume. Note in particular that every triangle should appear only once with its 'outward' orientation.



I am getting this error. What should i do ? 



Thank you



I will add another question regarding to first problem, my object does not stand on the floor it passes, anyone knows anything about it?

##### Simon Steinmann [Moderator] 01/23/2021 08:24:52
<@132121297277812736> your mesh file seems to be faulty. How did you generate it? There is tools and programs that can fix meshes. Webots told you the things that might be wrong


And for boundingObject, it is always better to approximate it with a simple geometric shape, such as a sphere, box, cylinder etc. It will make the simulation more stable and much faster


Should fix your second issue too

##### babaev1 01/23/2021 08:56:10
I am sorry for stupid question. Why Y axis stands for vertical direction? This I find in most of sample worlds. Conventionally Z stands for vertical. How much troubles I can encounter if I create world with Z vertical ?

##### Simon Steinmann [Moderator] 01/23/2021 09:30:41
In world info you can change the coordinate system from nue to enu (or create a new world through the wizard with that)

##### msoyer34 01/23/2021 13:56:04
Thank you Simon for your responses. I am using NX for my design and I export my design as OBJ. But it seems it doesnt work properly in webots.


`@Simon Steinmann`

##### Simon Steinmann [Moderator] 01/23/2021 15:36:39
make sure the mesh is watertight

##### babaev1 01/23/2021 16:15:06
Thank you for answer. This feature is available in latest version of Webots therefore this  pushed me to update. Still I experience some inconvenience because after changing from nue to enu all protos comes imported in wrong orientation. Saving world  and re-loading world doesn’t help. Wizard doesn’t help to set up viewpoint and background light for enu. They must be changed by hands. May be I made something wrong with consequence of steps.

##### ptrepag 01/23/2021 19:58:55
Our robot has a set of omniwheels attached to encoders.  Each omniwheel can only rotate in one direction, but moves freely in the other direction.  This makes them ideal for capturing actual robot movement in the X and Y direction (we use this for position localization).  Does anyone have a suggestion for how best to model this in webots?    I could add a position sensor to a cylinder, but I worry that the drag in the other direction would mess things up.  Any assistance would be greatly appreciated.

##### Cyber Police Officer 01/24/2021 16:49:34
Hello. I tried to search this in the documentation, but found no results. 



What does it mean when the "bounding objects" turn from white into purple?



(At t=0 some of the bounding objects are white but turn purple after a few seconds)



Thank you

##### Stefania Pedrazzi [Cyberbotics] 01/25/2021 07:21:27
The red/pink bounding objects means that this objects is colliding with some other objects. Then, if the object doesn't move any longer, after a while the boundingObject will turn blue. Otherwise if there is no collision the boundingObject color is white.

[https://www.cyberbotics.com/doc/guide/the-3d-window#selecting-an-object](https://www.cyberbotics.com/doc/guide/the-3d-window#selecting-an-object)

##### yash 01/25/2021 08:32:49
Hi ! I am using def getTorqueFeedback() function to compute torque of a mani[pulator link. I set the max available torque by using the function setAvailableTorque() to 2N.  Then I supply a velocity to the motors. So when I note down the values from the torque feedback, the values are not constant ? Since providing constant velocity to the motor, the torque should also be constant right ?


Please help me regarding this.

##### Cyber Police Officer 01/25/2021 09:25:31
Thank you!

##### Simon Steinmann [Moderator] 01/25/2021 10:29:08
Hold something in a stretched out arm at your side and lift it up until you hold it horizontally. The torque required at your shoulder changes constantly, even if you move at constant speed. When accelerating or compensating external forces (stretched out arm), you need more torque. Without load and a constant speed, you need no torque (or just a little bit to compensate for friction)

##### yash 01/25/2021 10:35:02
therefore , since the link has mass, so when actuating it at a constant velocity the torque will change ?

##### Bitbots\_Jasper [Moderator] 01/25/2021 13:54:21
if a link has no mass it theoretically requires no torque to accelerate, does any child of the link have any mass?

##### yash 01/25/2021 14:13:00
Yes the link has mass , so definitely it will require torque

##### Bitbots\_Jasper [Moderator] 01/25/2021 14:13:33
ah sorry i misread your message

##### yash 01/25/2021 14:14:04
I give constant velocity, so the torque should also be constant right ?


Given by gettorquefeedback() function

##### Bitbots\_Jasper [Moderator] 01/25/2021 14:16:36
lets assume there is no gravity, at the beginning of the motion you will need some torque to accelerate the joint, when the joint has reached its desired velocity, you might need some torque to overcome friction, at the end of the motion you will require some torque in the opposite direction to decelerate the joint


if you now have gravity, the joint has to apply torque to overcome that gravity pulling on the center of mass of the attached link, depending on the angle between the gravity vector pointing downwards and the angle of the joint to the link it will require more or less torque to overcome this force

##### yash 01/25/2021 14:21:31
Therefore as per your your statement , there will always be increase or decrease in the torque .?

##### Bitbots\_Jasper [Moderator] 01/25/2021 14:22:05
it depends on what external forces are acting on the link

##### yash 01/25/2021 14:22:24
Only the gravity

##### Simon Steinmann [Moderator] 01/25/2021 14:22:28
Just to make sure, do you understand what forces, torques, velocities and accelerations are? And how they interact with one another?

##### yash 01/25/2021 14:23:33
Yes.

##### Simon Steinmann [Moderator] 01/25/2021 14:25:00
Okay, so lets assume an instance, where there is no gravity, and no friction. In this case, you only need torque to accelerate or decelerate. Remaining still,  or at constant velocity, requires no torque.


Is this clear, or is there something you want me to explain in more detail?

##### yash 01/25/2021 14:28:08
Oh okay , well understand !

##### Simon Steinmann [Moderator] 01/25/2021 14:29:13
Now we turn gravity on. It will pull on the link and joint. This creates torque in the joint, assisting or fighting against the motor. So the motor torque has to change

##### yash 01/25/2021 14:30:56
Alright , because of the gravity or some exeternal force !

##### Simon Steinmann [Moderator] 01/25/2021 14:31:45
Just imagine, you would suddenly turn the motor off, and it would just be a joint without friction. Would anything fall down, slow down, speed up? If the answer is yes, the motor would have to apply a torque to counter that

##### yash 01/25/2021 14:32:55
Understood, 👍

##### Simon Steinmann [Moderator] 01/25/2021 14:33:07
[https://en.wikipedia.org/wiki/Newton%27s\_laws\_of\_motion](https://en.wikipedia.org/wiki/Newton%27s_laws_of_motion) Newtons laws in action 😄


Robotics is great. You actually need all the Maths and Physics you learned in school and at university

##### yash 01/25/2021 14:33:59
> Robotics is great. You actually need all the Maths and Physics you learned in school and at university

`@Simon Steinmann`  true !!!

##### Simon Steinmann [Moderator] 01/25/2021 14:34:46
[https://en.wikipedia.org/wiki/Rotation\_around\_a\_fixed\_axis](https://en.wikipedia.org/wiki/Rotation_around_a_fixed_axis) this might be worth a read too.

##### yash 01/25/2021 14:35:21
Thank you !

##### Krish 01/25/2021 14:45:56
Is it possible for Webots arena/world to have pictures, animations and GIFs?

If so, how to add them?

##### Bitbots\_Jasper [Moderator] 01/25/2021 14:47:08
for images refer to [https://cyberbotics.com/doc/reference/imagetexture](https://cyberbotics.com/doc/reference/imagetexture)


you can also use normal and occlusion maps with PBRAppearance [https://cyberbotics.com/doc/reference/pbrappearance](https://cyberbotics.com/doc/reference/pbrappearance)


[https://cyberbotics.com/doc/reference/appearance](https://cyberbotics.com/doc/reference/appearance) might be the simples approach though

##### Cyber Police Officer 01/25/2021 14:59:52
Hello. Sorry to bother again with questions:



I'm trying to do a parallel manipulator, similar to the stewart platform, and i'm trying to close the kinematic loop. Like the stewart mechanism, this manipulator has a fixed platform and a mobile platform.



What i'm trying to do is defining the mobile platform. The mobile platform pose in space relative to the fixed is quite simple, but relative to the last joint is fairly complicated.

Is there any way to generate the mobile platform solid relative to the base (basically just making it a children of the robot node) while being a movable solid?



If I create the mobile platform as a children of the robot node, it won't move no matter how much force the actuators make. Also, if i remove all the links, the mobile platform just stays afloat, ignoring gravity. All solids in question have a physics node. On the other hand, if i create it as a child of the last joint, it behaves properly.



I tried to follow the stewart platform example, where they generate the mobile platform as a child of the last joint, but i was trying to avoid this path.



Thank you

##### alejanpa17 01/25/2021 17:26:50
Hi everyone! Hope you can help me with this, I have the 3d model of a specific traffic cone and I'm trying to put it some physics but im stuck with the bounding object.

First, I tried to use the mesh of the 3d model but when I run a sim im getting this error: "Your world may be too complex." that's obvious because the mesh got many faces.

Then I thought in using the cone geometry shape but apparently webots doesn't support the cone as a Bounding Object.

What do you suggest? Is there any way I can modify the default traffic cone of webots (because this one has physics)?

This is one of the cones I wanted to introduce in the sim:



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/803314977511440404/unknown.png)
%end

##### Simon Steinmann [Moderator] 01/25/2021 17:28:49
Webots supports cones
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/803315455736414208/unknown.png)
%end


a cone plus box at the bottom should do the trick

##### alejanpa17 01/25/2021 17:34:58
It does supports cones but not as a  Bounding Object.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/803317001136242708/unknown.png)
%end


link: [https://cyberbotics.com/doc/reference/node-chart](https://cyberbotics.com/doc/reference/node-chart)

##### Simon Steinmann [Moderator] 01/25/2021 17:37:53
hmm you are right

##### AdityaDutt 01/25/2021 18:58:57
in my simulation, my simulation window is drifting around in a circular shape without me doing anything. Any ideas why?

##### DrakerDG 01/26/2021 06:50:47
<@671921336385273856> check the view menu: The Follow Object submenu allows you to switch between a fixed (static) viewpoint and a viewpoint that follows a mobile object (usually a robot). If you want the viewpoint to follow an object, first you need to select the object with the mouse and then check one of the items of the submenu depending on the following behavior you want... 

[https://cyberbotics.com/doc/guide/the-user-interface](https://cyberbotics.com/doc/guide/the-user-interface)

##### R\_ 01/26/2021 06:52:22
Is there a method Webots to create a 'ghost' robot (pure pose transformation through space without the effects of gravity and collisions affecting it?). This is shown in the video below. The robot in the darker shade is present in the physical world, whereas the other one is a ghost [https://www.youtube.com/watch?v=aiWxIjtMMFI](https://www.youtube.com/watch?v=aiWxIjtMMFI)

##### Olivier Michel [Cyberbotics] 01/26/2021 06:56:38
Yes, this possible by creating a robot without physics and animating it from a supervisor controller.

##### JSK 01/26/2021 07:40:41
Hello  there team


i am programming a controller for drone.


it takes of at 68.5


but i have given it more than 100 but it is not flying


motors are moving i.e. showing rotation


but the drone is not taking off


what could be the possible reason?


i am stuck here. need help

##### Olivier Michel [Cyberbotics] 01/26/2021 07:45:06
How did you create your drone model? Did you create it from the Mavic example?

##### JSK 01/26/2021 08:13:09
yes


Excuse me!

##### Olivier Michel [Cyberbotics] 01/26/2021 08:29:10
I would recommend you to revert back to the original drone model to check if it works with your controller and make changes step-by-step to identify where did the problem come from.

##### JSK 01/26/2021 09:30:03
Actually that drone is just imported in my\_first\_Simulation. it has nothing but just a drone. it is flying with "mavic2pro.c" controller but its not flying for mine. although  i have supplied rotor inputs. here is a screenshot of my envoirment.
%figure
![Screenshot_from_2021-01-26_14-01-08.png](https://cdn.discordapp.com/attachments/565154703139405824/803557358000930826/Screenshot_from_2021-01-26_14-01-08.png)
%end

##### Simon Steinmann [Moderator] 01/26/2021 11:42:51
Perhaps share your project. It's hard to tell otherwise

##### Cyber Police Officer 01/26/2021 11:46:01
Is there a way to chain transform nodes in a bounding object? The bounding object allows a transform node with a shape child, I was wondering if there's a way to chain multiple transform nodes, where the last one has a shape child.


Or if there's a way to automatically compute the equivalent transform

##### Simon Steinmann [Moderator] 01/26/2021 11:48:08
With just translations you can simply add them. Otherwise you have to do matrix multiplication


I'm sure there is online calculators for that. Basically you want to do forward kinematics


You can also see the translation and rotation of a node relative to others in webots

##### Cyber Police Officer 01/26/2021 11:55:08
Yeah, i have the transform on matlab. But i was trying to dynamically generate the manipulator, for given vector of design parameters, without having to manually input the transforms


Thank you

##### Simon Steinmann [Moderator] 01/26/2021 12:07:31
You may be able to simply chain them. I don't know if it works or not

##### Cyber Police Officer 01/26/2021 12:14:26
Its a chain of 3 transform with both rotation and translation. I could indeed multiply the matrices to obtain the equivalent transform, and then extract the translation and rotation, but i was wondering if there was a tool, in the PROTO, to automatically compute the chain of transforms. Or if there was a way to allow the chain to be interpreted as one transform in the bounding object

##### Simon Steinmann [Moderator] 01/26/2021 12:15:19
try it out

##### Cyber Police Officer 01/26/2021 12:16:25
I tried tried to chain them, but webots says it only allows a transform with a shape/geometry child, not a chain of transforms with a shape/geometry child:

"A Transform node inside a 'boundingObject' can only contain one Shape or one Geometry node. The child node is ignored.!"

##### Bitbots\_Jasper [Moderator] 01/26/2021 12:21:20
I think the structure of the proto you want is something like this:

```
Robot {
  Solid {
    translation ...
    rotation ....
    boundingObject Transform {
      translation ...
      rotation ...
      children [
        Box {
           size ...
        }
      ]
    ...
    }
    children [
      Solid {
      next link in the kinematic chain
      }
    ]
  }
}
```


the transform of the bounding object is relative to the solid

##### Cyber Police Officer 01/26/2021 12:30:35
Exactly, my problem is that, given the kinematics, i have to describe the bounding object with a chain of Transforms, like:

```
Robot {
  Solid {
    translation ...
    rotation ....
    boundingObject Transform {
      translation ...
      rotation ...
      children [
        Transform {
          translation ...
          rotation ...
          children [
            Transform {
              translation ...
              rotation ...
              children [
                Box {
                 size ...
                }
              ]
            }
          ]
        }
      ]
    ...
    }
    children [
      Solid {
      next link in the kinematic chain
      }
    ]
  }
}
```


And he doesn't allow this chain of transforms

##### Bitbots\_Jasper [Moderator] 01/26/2021 12:32:34
how are your solids connected?

##### Cyber Police Officer 01/26/2021 12:33:58
with ball and hingejoints


When i try to chain transform it says:

 A Transform node inside a 'boundingObject' can only contain one Shape or one Geometry node. The child node is ignored.

##### Bitbots\_Jasper [Moderator] 01/26/2021 12:34:23
but there are multiple bounding boxes in each link?

##### Cyber Police Officer 01/26/2021 12:35:11
No, each link only has one bounding object

##### Bitbots\_Jasper [Moderator] 01/26/2021 12:36:02
then the bounding box for each link should be inside that link (or solid how it is called in webots)


otherwise they will not move if the robot's joints move

##### Cyber Police Officer 01/26/2021 12:38:36
So the bounding shape must be a child of the solid and called to be used in the bounding object?

##### Bitbots\_Jasper [Moderator] 01/26/2021 12:40:25
i am not quite sure what you mean by "called to be used in the bounding object"


but yes the bounding object should be a child of the solid

##### Simon Steinmann [Moderator] 01/26/2021 12:41:11
A solid node has the field BoundingObject, that's where you add the collision geometry

##### Cyber Police Officer 01/26/2021 12:41:27
```DEF MOBILE_PLATFORM Solid {
                          translation 0 0 0
                          rotation 1 0 0 0
                          name "mobile_plat"
                          children[
                             DEF MOBILE_PLATFORM_SHAPE Transform {
                              translation 0 %{=-d/2}% 0
                              rotation %{=up_k['x']}% %{=up_k['y']}% %{=up_k['z']}% %{=-alphap_k}%
                              children[
                                Transform {
                                translation 0 %{=-h}% 0
                                rotation 0 0 1 %{=-beta_k}%
                                  children [
                                    Transform {
                                      translation %{=-b_k["x"]}% %{=-b_k["y"]}% %{=-b_k["z"]+z}%
                                      rotation 1 0 0 1.5708
                                      children [
                                        Shape {
                                        appearance PBRAppearance {
                                          baseColor 0.12 0.56 1
                                          roughness 1
                                          metalness 0
                                        }
                                        geometry Cylinder {
                                          radius IS r_m
                                          height IS arm_r
                                          subdivision 48
                                        }
                                      }
                                ]}
                               ]}
                             ]}
                          ]
                          boundingObject USE MOBILE_PLATFORM_SHAPE
                          physics NULL
                        }
```


This is what i'm trying to do

##### Simon Steinmann [Moderator] 01/26/2021 12:41:40
the visual shape you add in the children field

##### Cyber Police Officer 01/26/2021 12:41:48
Sorry for the horrible indentation


But it doesn't allow me, because the MOBILE\_PLATFORM\_SHAPE has a chain of Transforms. If it was only one transform it would work

##### Simon Steinmann [Moderator] 01/26/2021 12:43:22
the Shape has to have the DEF

##### Cyber Police Officer 01/26/2021 12:44:53
Yeah, but if i do that the boundingObject will be in the wrong place, because it loses the transforms


I could start with the chain of transforms, and define the solid has a child of it. But this is a end-effector of a joint and it only allows the end-effector to be a solid

##### Simon Steinmann [Moderator] 01/26/2021 12:46:37
why do you need that chain of transforms anyways?

##### Cyber Police Officer 01/26/2021 12:50:49
Basically i'm trying to describe where the blue platform is in space. And i don't know where it is relative to the green joint, but i do know where it is relative to the red plate.



So i'm transforming the coordinate system from the green joint referential into the red plate referential.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/803607880410333194/unknown.png)
%end


I could know where the blue plate is relative to the green joint, but the direct kinematics are fairly complicated and i was trying to avoid that path

##### Bitbots\_Jasper [Moderator] 01/26/2021 12:52:55
you should start by defining your kinematic chain as solid->joint->....->solid

where the solid after a joint is in the endPoint field of the joint and a joint after a solid is a child to the solid

you also put in the translations and rotations between the solids in the translation and rotation field of the joint, make sure to also set the joint parameters of the joint correctly

then you can put the bounding objects into the solids where the transform you set it to is relative to the solid it belongs to

##### Cyber Police Officer 01/26/2021 12:58:57
Thank you. I'm doing that, i started by following the Stewart platform example

##### Bitbots\_Jasper [Moderator] 01/26/2021 12:59:03
the pose of the blue table is based on the pose of green rods, therefore it has to be defined relative to the green rods

##### Cyber Police Officer 01/26/2021 13:00:05
Yes, the problem is that implies solving the direct kinematics, which are nontrivial in parallel manipulators


For instance, in that manipulator it implies solving a system of 6 nonlinear equations


Thank you for the help!

##### Bitbots\_Jasper [Moderator] 01/26/2021 13:09:24
yes you are right, it's really a lot more difficult for parallel manipulators..

##### alejanpa17 01/26/2021 15:20:37
Is there any way I can modify the default traffic cone of webots with my model in order to have his physics?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/803645582152433705/unknown.png)
%end

##### Simon Steinmann [Moderator] 01/26/2021 15:21:46
of course, just add a new object and load in your model for the visual shape and Bounding object

##### alejanpa17 01/26/2021 15:28:13
the problem is that my model has too many faces so the sim is giving me "Your world may be too complex."

##### Simon Steinmann [Moderator] 01/26/2021 15:29:10
you should approximate the bounding object with simple geometries


you can also specify the inertia matrix yourself.

##### jejay 01/26/2021 15:47:32
Hey guys I have a few questions: There is a kinematic mode mentioned (no physics, just animations) in the docs but it is not clear to me how to use it. Can I import animated objects (e.g. a car+trajectory animation) that I exported from blender? is there curves, interpolation etc?


and the other question extends this: is there a way to import humans + skeletal animation, again in kinematics mode?

##### Olivier Michel [Cyberbotics] 01/26/2021 15:51:46
The `TrafficCone.proto` has a `physics` field which is set to `NULL`. Simply adding a `Physics` node in there should add physics to it.


The `boundingObject` of the `TrafficCone.proto` is approximated with a `Box` and 3 `Capsule` primitives.


Simply removing the `Physics` nodes will disable physics in the simulation. Then, you will be able to animate the model from a supervisor controller by changing the `translation` and `rotation` fields of the objects you want to move.


As for human, see `WEBOTS_HOME/projects/humans/pedestrian` for some simple robotic human models.


Webots also support skeletal animation, see `WEBOTS_HOME/projects/samples/rendering/worlds/animated_skin.wbt`. However we do not distribute human model with skeletal animation, but it was tested and works.

##### jejay 01/26/2021 16:00:08
interesting, thanks!


so there is no simple way to play back a prerecorded animation or one that has been included e.g. in a collada file other than implementing it on your own step by step?

##### alejanpa17 01/26/2021 16:05:41
Thank you so much  `@Simon Steinmann` `@Olivier Michel`  😀

##### jejay 01/26/2021 16:06:30
the motion functions ([https://cyberbotics.com/doc/reference/motion-functions](https://cyberbotics.com/doc/reference/motion-functions)) are for physics/actuators only? I dont understand how you record them and where the motion editor is that is mentioned in the docs

##### Olivier Michel [Cyberbotics] 01/26/2021 16:09:39
The motion editor doesn't exist any more. The motion functions work for both physics and non-physics actuators.


We have some skin animation code in the works: [https://github.com/cyberbotics/webots-doc/pull/96](https://github.com/cyberbotics/webots-doc/pull/96) but it didn't yet reached the release.


The motion editor is actually deprecated and provided only for the Robotis-OP2 robot: [https://cyberbotics.com/doc/guide/robotis-op2#motion-editor](https://cyberbotics.com/doc/guide/robotis-op2#motion-editor)


But you can create a motion file from a simple text editor (the format is human readable).

##### Simon Steinmann [Moderator] 01/26/2021 16:15:27
you can also make a simple controller and use a supervisor to set jointagnles or right out the position and orientation of solids


and just read your trajectory or whatever you have and execute it in a loop

##### jejay 01/26/2021 16:16:02
thanks, yeah I guess thats kind of equivalent, thanks


Thanks a lot!

##### Simon Steinmann [Moderator] 01/26/2021 16:21:11
let us know if you have further questions


[https://cyberbotics.com/doc/reference/supervisor?tab-language=python](https://cyberbotics.com/doc/reference/supervisor?tab-language=python)


you'll probably need this


This trajectory follower script for ROS could help you out perhaps


[https://github.com/cyberbotics/webots/blob/master/projects/robots/universal\_robots/resources/ros\_package/ur\_e\_webots/src/ur\_e\_webots/trajectory\_follower.py#L199](https://github.com/cyberbotics/webots/blob/master/projects/robots/universal_robots/resources/ros_package/ur_e_webots/src/ur_e_webots/trajectory_follower.py#L199)


has cubic interpolation

##### jejay 01/26/2021 16:25:57
I am actually an "expert" to help students in a robotics practical at  UoEdinburgh that sadly this year is conducted virtually, hence most student will use webots. I might come back if I have any other questions. Thanks a lot again!


thats sounds good 🙂

##### Simon Steinmann [Moderator] 01/26/2021 16:26:52
you're welcome, and good luck 🙂

##### jejay 01/26/2021 16:37:17
ah maybe actually one final thing. If I stay in physics mode and take for example the bmw scene. Now I turn on manual mode and drive around. Can I record this into a motion file to replay the physics control in a later simulation? or would I need to write a controller that records my actions and then write those into a motion file (which in then could also be my own file format)?

##### Simon Steinmann [Moderator] 01/26/2021 16:38:12
you can record and replay anything you want. You could log the position of the whole model for example, and the then just replay that


with a supervisor you can set anything really

##### jejay 01/26/2021 16:38:46
yeah but i need to code that supervisor, right

##### Simon Steinmann [Moderator] 01/26/2021 16:39:04
very simple

##### jejay 01/26/2021 16:39:07
that was my question, there is nothing out of the box


yeah makes sense


yeah I see that this is simple 🙂

##### Simon Steinmann [Moderator] 01/26/2021 16:39:34
you get a handle to the node or field you want to change, and change it


instead of motors, you can directly change the joints

##### jejay 01/26/2021 16:40:52
👍 thanks again

##### chungshan 01/27/2021 06:00:25
Hi, can I get the force in the Hing2Joint?

I can get the torque by adding the rotational motor, but I also want to know how to get the force in the Hing2Joint

##### Darko Lukić [Cyberbotics] 01/27/2021 07:42:59
Hello `@chungshan`, the Motor node returns a force feedback as well:

[https://cyberbotics.com/doc/reference/motor#wb\_motor\_get\_force\_feedback](https://cyberbotics.com/doc/reference/motor#wb_motor_get_force_feedback)

##### Simon Steinmann [Moderator] 01/27/2021 10:03:31
`@chungshan` What exactly do you mean by the force in the joint? A joint can only have a torque. Only linear actuators can have forces. Perhaps you can clarify 🙂

##### DrVoodoo [Moderator] 01/27/2021 11:44:16
When creating new scenery proto objects, is there an additional step to make them pickable in the viewer? For example:


```PROTO Example [
  field SFVec3f     translation            0 0 0
  field SFRotation  rotation               0 1 0 0
  field SFString    name                   "example"
]
{
  Transform {
    translation IS translation
    rotation IS rotation
    children [
      CardboardBox {
        name %{= "\"" .. fields.name.value .. " top\"" }%
        translation 0 0 1
      }
      CardboardBox {
        name %{= "\"" .. fields.name.value .. " bottom\"" }%
      }
    ]
  }
}
```


Will not be selectable in the 3D viewer although it is still selectable in the scene tree

##### Stefania Pedrazzi [Cyberbotics] 01/27/2021 12:43:24
`@DrVoodoo` only `Solid` nodes are pickable from the 3D view

##### DrVoodoo [Moderator] 01/27/2021 12:58:24
`@Stefania Pedrazzi`  excellent, thanks. I was banging my head on that one

##### mayank.kishore 01/28/2021 01:27:09
Does anyone have experience creating an overhead map with a camera? And potentially stitching multiple images together to create an encompassing overhead map?

##### Laojiang 01/28/2021 06:35:05
How can I make a object whose the shape is a bowl?

##### Darko Lukić [Cyberbotics] 01/28/2021 07:53:16
In Webots? You can add a camera to a Supervisor robot or drone and take the images. Then, add the images to a software such as Pix4d (but probably there is a free alternative) to create a map.

##### Bitbots\_Jasper [Moderator] 01/28/2021 08:42:52
you can create a 3d model (many formats such as .stl, .obj, .dae and .blend are supported)


an example how to create a bowl model in open scad can be found here: [http://edutechwiki.unige.ch/en/OpenScad\_beginners\_tutorial#Simple\_CSG\_examples](http://edutechwiki.unige.ch/en/OpenScad_beginners_tutorial#Simple_CSG_examples)


then export it as stl (file->export) in open scad and import it in webots (file->import 3D model)

##### yash 01/29/2021 10:05:23
Sorry for the message before. Please ignore it.      I would like to know that if we have a structure like this -

          While robot.step(timestep)!=1

                    if (condition)                                                                                                                                                                                                                                                                                                                     for i in ----

   In this case when the execution is in the for loop will the robot.step(timestep) be updated ?

##### Simon Steinmann [Moderator] 01/29/2021 10:06:37
no, only when the robot.step() is executed, which is every iteration of the while loop

##### JSK 01/29/2021 10:06:50
Nop.

##### Simon Steinmann [Moderator] 01/29/2021 10:06:56
you could add an extra robot.step() inside the for loop

##### yash 01/29/2021 10:07:13
understood !! thanks

##### Lynerea 01/29/2021 10:43:53
Hi everyone, I recently downloaded Webots and when I went to make my own world, it comes up with an error saying that it can't find 'wbmath' and when I look at the file directory the files aren't there in any capacity - does anyone have any suggestions on how to solve that issue, would it just be a matter of reinstalling the program?

##### Bitbots\_Jasper [Moderator] 01/29/2021 10:45:11
how did you install webots?

##### Lynerea 01/29/2021 10:45:58
Via the cyberotics website using the newest Windows version it had available

##### Bitbots\_Jasper [Moderator] 01/29/2021 10:47:30
I'm sorry I don't know much about windows, maybe someone else can help with that.

##### Lynerea 01/29/2021 10:48:04
That's fine, thank you for trying anyways 🙂

##### Olivier Michel [Cyberbotics] 01/29/2021 10:48:59
Can you run the other demos of Webots (e.g., guided tour) without any problem?

##### Lynerea 01/29/2021 10:49:43
I opened the introductory demos, all of them I believe, with no issues at all

##### Olivier Michel [Cyberbotics] 01/29/2021 10:50:48
So what is particular with your world file? Did you add any specific object?

##### Lynerea 01/29/2021 10:52:10
I created a new project Directory via the wizard and ticked all the boxes when given the option but then the 'rectangle arena' caused an error but the rest of the world loaded successfully


I added a model of BB-8 after I created the world and the floor didn't work

##### Olivier Michel [Cyberbotics] 01/29/2021 11:33:29
I tried, but cannot reproduce this problem (tested on Windows 10).

##### Lynerea 01/29/2021 11:34:53
I don't understand how, after no tampering on my part, that the file was deleted, I don't know how it happened and I doubt I could reproduce the issue but reinstalling the program seems to have solved the issue for now

##### smasud98 01/30/2021 05:29:13
Hi everyone! I am new to webots and not sure if this is the right place to be asking this question. I am considering getting the Macbook with the M1 chip which uses ARM. I was wondering if anyone knows whether Webots will work well with this. Thanks

##### Krish 01/30/2021 06:03:49
Wait for this year's lineup.

##### Simon Steinmann [Moderator] 01/30/2021 14:23:48
`@smasud98` This is generally the right place to ask any questions regarding Webots. I think you should be fine with the macbook. While Webots is an x86 program, Apple's Rosetta 2 - x86 emulation seems to be very good and powerful. In general you should ask yourself the question though, whether a macbook is the right tool. If you want to dive deep into programming and development, engineering applications and linux, then it might be more advisable to get an x86 machine.  Perhaps you can tell me what you want to use your laptop for in general. Whether you are willing to switch to linux and/or linux is another big factor.

##### Krish 01/30/2021 15:24:37
Yeah well said.

He told me that he is more interested in getting a Mac than a Linux machine, he says that he has the 2015 Macbook and webots runs very slowly in that.

Also, not all the apps and programming languages are configured to run on the M1.



So he says if Webots does not run on it, he will just return and take the refund back.

##### Simon Steinmann [Moderator] 01/30/2021 15:39:30
The new macbook is a completely new hardware & software product. It probably works really well for a pure consumer, but as soon as you start development and go outside of MacOS, I foresee a lot of issues. Linux can run on arm, but by far not all software and programs.  I have a Lenovo Legion laptop. It is basically a gamer laptop, that looks more professional. I bought it for robotic simulation and Reinforcement Learning. I dont see macbooks being a good choice for that. For AI you kinda need a Nvidia GPU, and a powerful x86 CPU is a godsent when compiling and running lots of parallelized code / many instances.

##### PymZoR [Premier Service] 01/30/2021 15:46:55
Hi everyone, i'm having an issue with a basic HingeJoint for a wheel


I have a differential robot, and when it's turning in place, le wheels are "moving sideways"


like on a bad bike


By looking at the tree I see small  values in the `translation` field of the wheel although the joint is configured to only allow rotation on an axis

##### Simon Steinmann [Moderator] 01/30/2021 15:51:18
can you share your world or a screenshot of your hingjoint? with JointParameters expanded in the scene tree

##### PymZoR [Premier Service] 01/30/2021 15:54:41

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/805103704910200854/unknown.png)
%end



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/805103881205841930/unknown.png)
%end

##### Simon Steinmann [Moderator] 01/30/2021 15:58:32
it is always good practice to have the same anchor as translation of the endPoint solid.  currently it rotates around the y-axis of in the 2nd image


move the anchor to where you want the center of rotation to be

##### PymZoR [Premier Service] 01/30/2021 15:59:18
Oh, ok


On top of my Hinge I have a Transform


I can remove the Transform, and instead use anchor & translation on the endpoint solid right ?

##### Simon Steinmann [Moderator] 01/30/2021 16:00:19
the transform is fine.


send me your world, i can have a quick look


the issue is, if you have a translation of the endpoint, which is NOT along the rotational axis

##### PymZoR [Premier Service] 01/30/2021 16:07:04
The translation of the enpoint was the result of the bug, I did not manually edit these  values



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/805106901393604679/unknown.png)
%end


I just tried this, deleting the Transform above the Joint, no success

##### Simon Steinmann [Moderator] 01/30/2021 16:08:58
the translation of the endPoint changes with when the joint rotates. So at the start, the anchor and translation have to be the same (assuming you want the endPoint to rotate around its origin)

##### PymZoR [Premier Service] 01/30/2021 16:09:47
which is the case on my last screenshot right ?

##### Simon Steinmann [Moderator] 01/30/2021 16:09:56
yes

##### PymZoR [Premier Service] 01/30/2021 16:10:30

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/805107686977830952/unknown.png)
%end

##### Simon Steinmann [Moderator] 01/30/2021 16:10:47
this should work too

##### PymZoR [Premier Service] 01/30/2021 16:10:53
Which is also the case when the transform is done above; I have the same endpoint translation and anchor


Still have the problem though 😄

##### Simon Steinmann [Moderator] 01/30/2021 16:11:06
what problem?

##### PymZoR [Premier Service] 01/30/2021 16:11:50
When the robot is turning in place, the wheels are slightly translating


instead of only rotating around the joint axis

##### Simon Steinmann [Moderator] 01/30/2021 16:12:54
I would have to see the whole thing. But just stick with the solution that works


that is the 'proper' way to do it

##### PymZoR [Premier Service] 01/30/2021 16:13:06
No solution works atm haha

##### Simon Steinmann [Moderator] 01/30/2021 16:13:21
share your world file


can also pm if you want

##### ljmanso 01/31/2021 15:14:48
Hello everyone. I am new to Webots and I have a quick question that I haven't seen in the documentation. Is there any way to simulate virtual humans moving around?

##### Bitbots\_Jasper [Moderator] 01/31/2021 15:31:43
There is the pedestrian proto (model) and a controller for it.

##### ljmanso 01/31/2021 19:15:06
Thanks! I will try to find more information about it 👍

##### babaev1 01/31/2021 20:20:54
Hello, can’t figure out why two solids pass through each other like non-material objects during simulation running. Bounding objects are defined and physics is added for both solids. Could it be due to that dencity is -1, but mass has positive value? Could it be due to that inertia matrix is not added? Could it be due to that center of mass is not added for solids?

##### paperwave 01/31/2021 22:33:56
`@babaev1` When you click on the boundingObject node you should see the bounding region surrounding the Solid, if you don't see it, it may be covered from your graphical view.

##### babaev1 01/31/2021 22:35:44
Bounding region covers solid and I can see it

##### Bitbots\_Jasper [Moderator] 01/31/2021 22:37:09
the density field is used to calculate the mass by using the volume of the bounding object, if a mass is specified it should be -1


are the two solids that should collide part of the same proto?

##### babaev1 01/31/2021 22:38:07
Solids are designed by myself from primitives

##### Bitbots\_Jasper [Moderator] 01/31/2021 22:39:22
what i am trying to ask is if the two solids are defined in the same file?

##### babaev1 01/31/2021 22:39:41
Yes

##### Bitbots\_Jasper [Moderator] 01/31/2021 22:40:09
in the robot node, there is a parameter selfCollision, you might need to enable that


[https://cyberbotics.com/doc/reference/robot](https://cyberbotics.com/doc/reference/robot)

##### babaev1 01/31/2021 22:43:06
It works!


Thank you!

##### Bitbots\_Jasper [Moderator] 01/31/2021 22:43:39
you're welcome 👍

##### babaev1 01/31/2021 22:44:11
👏

##### paperwave 01/31/2021 22:49:14
Question, I'm working on the tutorials (on 6) but I'm getting a compile error because some nodes' name field is the same, is there a way in the IDE to change that? double click on name isn't doing anything


compile error
%figure
![wheel_not_found.PNG](https://cdn.discordapp.com/attachments/565154703139405824/805570707551027250/wheel_not_found.PNG)
%end


[https://cyberbotics.com/doc/reference/solid#unique-solid-name](https://cyberbotics.com/doc/reference/solid#unique-solid-name) indicates the IDE should make a unique name hmmm


wait those are warnings, so it should run, I just want to use better descriptive names

##### Bitbots\_Jasper [Moderator] 01/31/2021 22:55:09
you can right click on the robot in the node view on the left and edit the proto file(view protofile) , control+f for name and find the names that are overlapping

##### paperwave 01/31/2021 22:58:07
I don't see that, I right clicked on the robot's root node, "Robot", I'll keep looking for a view protofile option somewhere

##### Bitbots\_Jasper [Moderator] 01/31/2021 23:00:31

%figure
![jo.png](https://cdn.discordapp.com/attachments/565154703139405824/805573259717246996/jo.png)
%end

##### paperwave 01/31/2021 23:04:38
Here's what I'm seeing, I made it with Base nodes > Robot



%figure
![missing_settings.PNG](https://cdn.discordapp.com/attachments/565154703139405824/805574353571938334/missing_settings.PNG)
%end


I see, it doesn't have that because it was a base node.


looking for the wbt file, maybe that'll do it


It's my second day of webots so I know it's something simple

##### Bitbots\_Jasper [Moderator] 01/31/2021 23:10:47
The problem is that it is probably save in a location where you can not edit it (because you need sudo or admin). Copy the proto file from your webots installation to your own proto folder


maybe this helps to understand how you should structure your webots projects: [https://cyberbotics.com/doc/guide/the-standard-file-hierarchy-of-a-project](https://cyberbotics.com/doc/guide/the-standard-file-hierarchy-of-a-project)

##### paperwave 01/31/2021 23:13:19
moving the protos folder now


Looks like this part of the tutorial only uses base nodes so there was no proto on it however, I restarted Webots and was able to rename the wheels which is great. Fixing something else I messed up present, thanks for the help

## February

##### まちこ (Bailey) 02/01/2021 05:12:30
I'm trying to change the fields of an object, but it looks like this. What do I need to do so that I can edit them? (This is my first time using Webots, sorry if it's a dumb question!)
%figure
![Screen_Shot_2021-01-31_at_9.11.53_PM.png](https://cdn.discordapp.com/attachments/565154703139405824/805666868773781504/Screen_Shot_2021-01-31_at_9.11.53_PM.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 02/01/2021 07:06:18
There is a dedicated window to edit the field, but it seems that you hide it.

It is located just below this scene tree.

So to show you can click on the sidget separator (indicated by the three lines on the bottom of your screenshot) and drag it up, or restore the default application layout from the `Tools > Restore Layout`

##### まちこ (Bailey) 02/01/2021 07:15:46
Thank you!

##### iagsav 02/01/2021 12:31:47
Hi! Please tell me how you can read the data from the compass of the Firebird 6 robot?

##### Simon Steinmann [Moderator] 02/01/2021 12:35:42
`@iagsav` if you right-click on the robot, you can select "view PROTO source"


in there you will find this section, with the sensors and their names
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/805778505736519750/unknown.png)
%end

##### iagsav 02/01/2021 12:40:56
Thank you!


I wrote this code


cmp = robot.getDevice('compassXY\_01')

print(cmp)


but it returns 'None'

##### Simon Steinmann [Moderator] 02/01/2021 12:42:36
for i in range(robot.getNumberOfDevices()):

    device = supervisor.getDeviceByIndex(i)

    print(i, '  -  ', device.getName(), '   - Model:', device.getModel(),'   - NodeType:', device.getNodeType())


try running this


it should print all devices


also a good way to make lists of motors, sensors etc. by appending a list, if the device is of the type you want

##### iagsav 02/01/2021 12:44:35
should I set the supervisor field to true?



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/805780673922859038/unknown.png)
%end

##### Simon Steinmann [Moderator] 02/01/2021 12:45:12
oh, do it robot.getDeviceByIndex


supervisor is not needed


took it from one of my controllers


where I use a supervisor for other stuff

##### iagsav 02/01/2021 12:46:15
Thanks you! it worked



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/805781066035757117/unknown.png)
%end

##### Simon Steinmann [Moderator] 02/01/2021 12:46:47
hmm model doesnt seem to work for some reason


sensors = []

minPositions = []

maxPositions = []

for i in range(robot.getNumberOfDevices()):

    device = robot.getDeviceByIndex(i)

    print(i, '  -  ', device.getName(), '   - Model:', device.getModel(),'   - NodeType:', device.getNodeType())

    # if device is a rotational motor (uncomment line above to get a list of all robot devices)

    if device.getNodeType() == 54:  

        motors.append(device)

        minPositions.append(device.getMinPosition())

        maxPositions.append(device.getMaxPosition())

        sensor = device.getPositionSensor()

        try:

            sensor.getName()

            sensors.append(sensor)

            sensor.enable(timeStep)

        except Exception as e:

            print('Rotational Motor: ' + device.getName() + ' has no Position Sensor')


\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_

this is an example, how I initialize robotic arms. It automatically creates a list of all rotational motors, their position sensors and the corresponting min and max positions


motors = [] missing in the first line


This works regardless of what robotic arm I use. I'm sure you can adjust it to your needs

##### iagsav 02/01/2021 12:52:23
Thank you! I will try!


It is very strange, but when I run e-puck obstacle avoidance tutorial, I get such result



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/805786926274576474/unknown.png)
%end


I reinstall python 3.7.9 (64) twice


and restart webots too

##### Simon Steinmann [Moderator] 02/01/2021 13:10:37
ps[i] is empty in this case

##### iagsav 02/01/2021 13:11:33
I understand, but why? I use standard world file

##### Simon Steinmann [Moderator] 02/01/2021 13:11:58
show me the controller?

##### iagsav 02/01/2021 13:12:18

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/805787617240154132/unknown.png)
%end


I take it from tutorial


[https://cyberbotics.com/doc/guide/tutorial-4-more-about-controllers?tab-language=python](https://cyberbotics.com/doc/guide/tutorial-4-more-about-controllers?tab-language=python)

##### Simon Steinmann [Moderator] 02/01/2021 13:13:32
put a "print(i)" in front of the ps[i].enable

##### iagsav 02/01/2021 13:14:12

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/805788096221544488/unknown.png)
%end

##### Simon Steinmann [Moderator] 02/01/2021 13:15:19
weird, that error is new now


you are using the correct robot?

##### iagsav 02/01/2021 13:15:32
I found similar issue here: [https://github.com/cyberbotics/webots/issues/2570](https://github.com/cyberbotics/webots/issues/2570)


I use file e\_puck.wbt, I think it is correct

##### Simon Steinmann [Moderator] 02/01/2021 13:16:56
you are on the newest version of webots?


you could also try running that loop I showed you earlier

##### iagsav 02/01/2021 13:17:16
I download it now


Now i install newest webots version


It is very strange, but all works: I get compass in fire bird 6

##### Simon Steinmann [Moderator] 02/01/2021 13:39:06
great 🙂

##### iagsav 02/01/2021 13:42:06
yee!!! Thank you!!!

##### PRVG 02/01/2021 15:33:51
Hello i would like to know if anyone as implemented a optical flow sensor in Webots. Looking through the documentation i do not think there is one available in the sensor nodes. Thank you very much.

##### Olivier Michel [Cyberbotics] 02/01/2021 15:39:16
You are right, there is no such sensor. But basically, this can be implemented at the controller level from a standard camera flow, not inside Webots.

##### PRVG 02/01/2021 15:41:49
Thank you for the reply. I was gonna try implementing  that in matlab just wanna make sure there was no sensor for it already implemented and if someone had any experience with it so they could give some pointers. Thank you

##### Chernayaten 02/01/2021 16:39:29
I'm trying to make k-team's hemisson robot rotate around itself using the setPosition command in Python (with getTargetPosition +/- rotate\_value). However whether I try to increase the position or decrease it the robot always moves forward


I'm also facing an issue with its pen node which simply doesn't work at times. I've made a few tests starting it from random positions and it might work or it might not work. At times when it is not working, if I start manually  moving the robot around while the simulation is running it might write a bit and then stop or it might start writing normally

##### Simon Steinmann [Moderator] 02/01/2021 18:34:50
the setPosition most likely changes the wheel angle, not the robot rotation. So to rotate, you have to turn one clockwise and the other ccw

##### Chernayaten 02/01/2021 18:39:45
That is what I am doing. I use the motor.getTargetPosition() to get the position at that specific moment and then increase one wheel / decrease the other to make it turn. I've already used this code on a robot I made and it has been working without issues


I've performed the following simple test. Set both wheel positions at 100 and run it. The robot moves forward. Reset and set both wheel positions at -100. Robot moves forward again, even though it should be moving backwards

##### Simon Steinmann [Moderator] 02/01/2021 18:53:34
for wheels you should probably use velocity control


and change the angles incrementally. Also remember, that the values are radian


so 100 is a lot

##### Chernayaten 02/01/2021 18:56:41
I've been trying to make the robot turn exactly 90 degrees (and then other angles) which is why I have been using position instead. I am aware values are radian,  the 100 is just an example to show it is not working as it should


Weirdly enough, if I do use setVelocity with +/- on the wheels it does work as it should. I really don't want to have to re-work my code though. The setPosition should be working as well which is what I am trying to solve

##### PymZoR [Premier Service] 02/01/2021 19:05:13
Hi everyone, I was wondering if someone tried to use asyncio for a python controller ?

##### Simon Steinmann [Moderator] 02/01/2021 20:02:38
if your starting position is -200, then it will drive forward in both cases, setting the target to -100 and 100

##### Chernayaten 02/01/2021 20:38:15
That's why I said in my first message that I use the .getTargetPosition() method. The increase/decrease is always done in regards to the wheel position at that moment. That means that if my starting position was -200 then one wheel would increase to -100 and the other would decrease to -300

##### Simon Steinmann [Moderator] 02/02/2021 10:57:01
`@Chernayaten`  Why are you using .getTargetPosition() instead of using the position sensor to get the current position?


positionSensor = motor.getPositionSensor()

positionSensor.enable(timestep)

pos = positionSensor.getValue()


I suspect that the issue lies here


.getTargetPosition()  gives you the last command issued with setPosition() and NOT the current position

##### Chernayaten 02/02/2021 13:12:48
I am doing it this way because I do not care about the current position of the motor. My code is working correctly in my own robot, it works correctly on a different robot I randomly picked (Lego's Mindstorms), but it doesn't work correctly on the Hemisson robot.


while self.step(self.TIME\_STEP) != -1:

    left.setPosition(left.getTargetPosition() - 10)

    right.setPosition(right.getTargetPosition() + 10)


Regardless of whether this is the smart way to do it, the following code should make the robot constantly rotate around itself. Hemisson robot doesn't do that and I can't figure out why. At the same time the other two robots I mentioned do

##### alireza\_9 02/02/2021 18:39:19
Hello everyone

dose anyone knows how can I train robot using pytorch in webots?

##### Simon Steinmann [Moderator] 02/02/2021 18:59:26
[https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers](https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers)


use an extern python controller

##### John520 02/02/2021 20:25:42
Hi, I'd like to create a crop field for lidar detection simulation. I found out that Webots provides Trees and Plants objects. My questions are: 1. Can the tree and plant objects (shape and leaves) be detected by a lidar in the simulation? 2. Is there a way to create a crop object by myself? Thank you!

##### LucasW 02/03/2021 00:55:54
Hi all, I'm a student working on a project using Webots for the first time. This is a pretty basic question but I'm implementing an approximation of the VL53L1X tof sensor see [https://www.st.com/en/imaging-and-photonics-solutions/vl53l1x.html#overview](https://www.st.com/en/imaging-and-photonics-solutions/vl53l1x.html#overview) for details, or [https://www.pololu.com/product/3415/specs](https://www.pololu.com/product/3415/specs) for a quick specs overview. I was wondering what units resolution is measured in/should I just make it infinite and I don't understand what response values I should map the distances too? Any help would be appreciated thanks.

##### John520 02/03/2021 15:03:45
Hi guys, I'd like to create a crop field for a lidar detection simulation. I found out that Webots provides Trees and Plants objects. My questions are, 1. Can the Tree and Plant objects (shape and leaves) be detected by a lidar in the simulation? 2. Is there a way to create a crop object by myself? Thank you!

##### Darko Lukić [Cyberbotics] 02/03/2021 15:07:50
Hello `@John520`, 1. Yes. You can create a simple robot, add a Lidar, and visualize the point cloud (option in the View menu). 2. Yes. Create a terrain, add some plants, trees...



There are a few examples on YouTube (created in a very old version of Webots):

[https://www.youtube.com/watch?v=X7YFyVZc1Eg](https://www.youtube.com/watch?v=X7YFyVZc1Eg)


Hello `@LucasW`, the returned values from the DistanceSensor node are in meters.

##### John520 02/03/2021 15:13:24
`@Darko Lukić` Thank you very much for your reply. Would be possible to create a crop or plant that is not included in the Trees and Plants objects by myself. For example, can I create a cereal object somewhere and then import it into Webots? Thank you!

##### Olivier Michel [Cyberbotics] 02/03/2021 15:14:47
Yes, of course. It is just a matter of creating a new proto file.

##### gaitt 02/03/2021 15:16:22
Hello, I'm wondering how do I retrieve the camera behind a DEF. I'm using C++ API, supervisor->getNodeFromDef(CAMERA\_DEF) which seems to return valid pointer on a Node. But since a Device/Camera does not inherit from node I can't cast it to a camera ... any clue?

##### John520 02/03/2021 15:16:59
Oh sounds great! Thank you `@Olivier Michel`. Would you have any tutorial about creating a new proto file? Could you share a link, please? Thank you!

##### Olivier Michel [Cyberbotics] 02/03/2021 15:17:15
You should use the `getCamera` method instead.


[https://cyberbotics.com/doc/guide/tutorial-7-your-first-proto](https://cyberbotics.com/doc/guide/tutorial-7-your-first-proto)

##### John520 02/03/2021 15:20:07
Thank you very much `@Olivier Michel` for your help!!!

##### gaitt 02/03/2021 15:25:44
Well did not find such method in the documentation, on which class the method is available?

##### Olivier Michel [Cyberbotics] 02/03/2021 15:26:52
on Robot (from which Supervisor inherits).

##### Simon Steinmann [Moderator] 02/03/2021 15:28:21
[https://drive.google.com/drive/folders/1rI7xeOP5CCi\_IFTOmZu37nEZDxR-Uj6\_?usp=sharing](https://drive.google.com/drive/folders/1rI7xeOP5CCi_IFTOmZu37nEZDxR-Uj6_?usp=sharing) this could be useful

##### gaitt 02/03/2021 15:33:29
Ok thanks, I found it in the .hpp but it definitely not in the doc [https://cyberbotics.com/doc/reference/robot](https://cyberbotics.com/doc/reference/robot)

##### Olivier Michel [Cyberbotics] 02/03/2021 15:34:06
It's there: [https://cyberbotics.com/doc/reference/robot?tab-language=c++#wb\_robot\_get\_device](https://cyberbotics.com/doc/reference/robot?tab-language=c++#wb_robot_get_device)


The C function is called `wb_robot_get_device`.

##### gaitt 02/03/2021 15:35:17
My bad, I was searching for the C equivalent *\_get\_camera

##### John520 02/03/2021 17:25:14
Hi `@Olivier Michel`, I am looking into the Cypress.proto [https://github.com/cyberbotics/webots/blob/released/projects/objects/trees/protos/Cypress.proto](https://github.com/cyberbotics/webots/blob/released/projects/objects/trees/protos/Cypress.proto). Would you please advise how to define the point values for "geometry IndexedFaceSet"? There are lots of point values there. Thank you!

##### LucasW 02/03/2021 17:30:19
Thanks `@Darko Lukić` but I don't think I was very clear. I'm a confused by the lookup table response values. I've read the Webots documentation but still not sure how you set the response values based on different sensors. For example in the webots\_ros2/webots\_ros2\_epuck/protos/E-puck\_enu.proto there is a tof sensor. Which approximates the VL53L0X sensor and I don't understand why those corresponding response values were chosen, I've looked at the sensor documentation and the Webots documentation and am still not sure why?

##### iagsav 02/03/2021 17:38:45
Hi! please explain how you can add a compass to the e-puck robot?

##### Krish 02/03/2021 18:49:01
`@John520` Did you forget to leave the voice channel?


Its been a while

##### John520 02/03/2021 19:18:15
`@Krish` Thanks. I forgot about it.

##### Olivier Michel [Cyberbotics] 02/04/2021 07:04:37
IndexedFaceSet are usually designed as meshes in a 3D modelling software (like Blender or other) and then exported to VRML97, copy/pasted in to the PROTO file.

##### Stefania Pedrazzi [Cyberbotics] 02/04/2021 07:20:32
You should add it in the `turretSlot` filed of the e-puck.

##### Darko Lukić [Cyberbotics] 02/04/2021 08:06:40
Ok, I see. It is possible that GCtronic integrated API that returns scaled values. However, if you don't have a strong reason to use the `lookupTable` parameter, you can skip modeling that part. Without the lookup table you will get actual distance. The other parameters, `numberOfRays`, `aperture`, and`resolution` are more important for modeling a realistic sensor. Also, you can use the lookup table without scaling the values, only imposing a noise at different distances.

##### Iris230 02/04/2021 08:47:29
hello guys, I'm wondering how could Logitech G29 get connected to webots and control the car  just like this?



%figure
![Screenshot_20210204_163925_com.google.android.youtube.jpg](https://cdn.discordapp.com/attachments/565154703139405824/806808172630507539/Screenshot_20210204_163925_com.google.android.youtube.jpg)
%end


I opened a sample world called 'village center' and I added a BMW node. But I can't change to this point of view, like you are sitting in this car. 🥲😭

##### Olivier Michel [Cyberbotics] 02/04/2021 08:54:20
A sample controller for interfacing a G29 (and other racing wheels) is available in `WEBOTS_HOME/projects/vehicles/controllers/racing_wheel/`.

##### Iris230 02/04/2021 08:58:04
wow thank you so much😆

##### iagsav 02/04/2021 11:03:37
Thank you! It works!!!

##### LucasW 02/04/2021 11:20:24
Thank you very much `@Darko Lukić`

##### John520 02/04/2021 18:09:32
Thank you `@Olivier Michel` again. I am looking for a wheat model online. Do you think if I can convert the wheat field models to proto files so that I can use them in Webots? The wheat field models are in Blender formats, please check [https://www.turbosquid.com/3d-models/section-wheat-field-3d-model-1572937#](https://www.turbosquid.com/3d-models/section-wheat-field-3d-model-1572937#) and [https://www.turbosquid.com/3d-models/wheat-field-3d-model-1175816](https://www.turbosquid.com/3d-models/wheat-field-3d-model-1175816). Or I need to work on the model conversion for single wheat instead of a wheat field?

##### yash 02/04/2021 18:22:11
Hi ! This warning pops up in middle of the program running.....whats the reason behind it ?
%figure
![Screenshot_from_2021-02-04_23-50-28.png](https://cdn.discordapp.com/attachments/565154703139405824/806952763439054878/Screenshot_from_2021-02-04_23-50-28.png)
%end

##### Darko Lukić [Cyberbotics] 02/04/2021 18:23:45
It means that the controller has crashed. Try to simplify it

##### yash 02/04/2021 18:26:18
oh okay ! any hints you could give me about how to do it if there's any general solution, or it depends on the way the controller code is written ?

##### Darko Lukić [Cyberbotics] 02/04/2021 18:26:48
It depends a lot on the controller implementation

##### yash 02/04/2021 18:27:40
okay thanks a lot, let me try it out then.

##### Troy 02/05/2021 03:33:04
Hi everyone! I have a question about Fluid. I looked at the 'floating\_geometries' example, and in my simulation, I set the sane fluid properties, like I set shape, velocity, and boundingObj, and build a solid, set its immersionProperties, but it doesn't work, this solid doesn't even flow in the fluid.


`@Darko Lukić` I added a solid box in the 'floating\_geometries' example, and added shape, immersionProperties, boundingObj, damping, as other geometries, but this solid won't flow with the water. Can you help me? I don't understand why.



%figure
![test.jpg](https://cdn.discordapp.com/attachments/565154703139405824/807094784840826891/test.jpg)
%end


As you can see from the picture, it won't flow with the water


This happens in my another simulation. I don't know how to fix this problem.

##### Darko Lukić [Cyberbotics] 02/05/2021 08:45:36
`@Troy` Please check this minimal example:

```vrml
#VRML_SIM R2021a utf8
WorldInfo {
  coordinateSystem "NUE"
}
Viewpoint {
  orientation 0.9947291283851613 0.0980963256805878 0.029850829637807465 5.68943875583334
  position -0.4920553249349089 5.987256278548413 8.041045316575675
}
TexturedBackground {
}
TexturedBackgroundLight {
}
DEF STILL_WATER Fluid {
  translation 0 0.2 0
  children [
    DEF S Shape {
      appearance PBRAppearance {
        baseColor 0 0.501961 1
        transparency 0.5
        roughness 1.1102230246251565e-16
        metalness 0
      }
      geometry Cylinder {
        height 0.4
        radius 1.8
        subdivision 24
      }
    }
  ]
  name "swimming pool"
  boundingObject USE S
}
DEF THIN_CYLINDER Solid {
  translation -0.474938 1.52181 0.0751348
  rotation 0.28982509088350455 0.7070662121554249 0.6450261919663017 4.07769
  children [
    DEF S Shape {
      appearance PBRAppearance {
        baseColor 0.501961 0.337255 0.2
        roughness 1.1102230246251565e-16
        metalness 0
      }
      geometry Cylinder {
        height 0.5
        radius 0.12
      }
    }
  ]
  immersionProperties [
    DEF SWIMMING_POOL_IMMERSION_PROPERTIES ImmersionProperties {
      fluidName "swimming pool"
      dragForceCoefficients 0.1 0 0
      dragTorqueCoefficients 0.001 0 0
      viscousResistanceTorqueCoefficient 0.005
    }
  ]
  boundingObject USE S
  physics Physics {
    density 500
    damping Damping {
      linear 0.5
      angular 0.5
    }
  }
}
```


Notice that `ImmersionProperties > fluidName`  and `Fluid > name` have to match.

##### Welsh\_dragon64 02/05/2021 11:35:16
I have been trying to code the robotis darwin-op\_2 to walk using python. I have been trying to access its packages through managers provided by webots, but i keep getting errors and some errors doesnt quite make sense. I have minor experience in python.



> **Attachment**: [robotis\_code\_.txt](https://cdn.discordapp.com/attachments/565154703139405824/807213002692493322/robotis_code_.txt)

##### John520 02/05/2021 15:10:56
Thank you `@Olivier Michel` again. I am looking for a wheat model online. Do you think if I can convert wheat field models to proto files so that I can use them in Webots? The wheat field models are in Blender formats, please check [https://www.turbosquid.com/3d-models/section-wheat-field-3d-model-1572937#](https://www.turbosquid.com/3d-models/section-wheat-field-3d-model-1572937#) and [https://www.turbosquid.com/3d-models/wheat-field-3d-model-1175816](https://www.turbosquid.com/3d-models/wheat-field-3d-model-1175816). Or Do I need to work on the model conversion for single wheat instead of a wheat field? Thanks a lot.

##### Bitbots\_Jasper [Moderator] 02/05/2021 18:23:02
I am a bit confused as to how some of the supervisor funtions work, maybe someone can explain or point me some documentation. The supervisor functions allow me to retrieve nodes in the world either with ID or DEF ([https://www.cyberbotics.com/doc/reference/supervisor?tab-os=linux&tab-language=c++#supervisor-functions](https://www.cyberbotics.com/doc/reference/supervisor?tab-os=linux&tab-language=c++#supervisor-functions)),



 is this the same DEF that is used in the proto file for the DEF-USE mechanism ([https://www.cyberbotics.com/doc/guide/tutorial-2-modification-of-the-environment#def-use-mechanism](https://www.cyberbotics.com/doc/guide/tutorial-2-modification-of-the-environment#def-use-mechanism)) or is it something different?

##### Simon Steinmann [Moderator] 02/05/2021 18:23:59
`@Bitbots_Jasper` in the scene tree, you can give any node a DEF. You can then retrieve that node with the supervisor


nodes inside of proto files are a bit more tricky

##### Bitbots\_Jasper [Moderator] 02/05/2021 18:24:35
those are the ones I'm trying to reach

##### Welsh\_dragon64 02/05/2021 18:25:00
guys i have a question



how do i enable the robot window using roboits darwin\_op2 in webots using python?

##### Simon Steinmann [Moderator] 02/05/2021 18:25:02
what exactly are you trying to do?

##### Bitbots\_Jasper [Moderator] 02/05/2021 18:25:24
finding the position of certain solids in world coordinates

##### Simon Steinmann [Moderator] 02/05/2021 18:26:06
what robot?

##### Bitbots\_Jasper [Moderator] 02/05/2021 18:26:19
my own 😄


[https://github.com/bit-bots/wolfgang\_robot/blob/master/wolfgang\_webots\_sim/protos/Wolfgang.proto](https://github.com/bit-bots/wolfgang_robot/blob/master/wolfgang_webots_sim/protos/Wolfgang.proto)

##### Simon Steinmann [Moderator] 02/05/2021 18:26:41
you can expose fields in a proto


like you do line 8 -> line 21


you can do that for any field


ahh.. you want the node so you can get the position


should work the same

##### Bitbots\_Jasper [Moderator] 02/05/2021 18:29:44
i read that there is wb\_supervisor\_node\_get\_from\_proto\_def or getFromProtoDef in python, should that not work as well?


[https://www.cyberbotics.com/doc/reference/supervisor?tab-os=linux&tab-language=python#description](https://www.cyberbotics.com/doc/reference/supervisor?tab-os=linux&tab-language=python#description)

##### Simon Steinmann [Moderator] 02/05/2021 18:30:13
ohh right


that shouuld work


I only used fields inside of protos so far

##### Bitbots\_Jasper [Moderator] 02/05/2021 18:30:53
sorry this link is correct: [https://www.cyberbotics.com/doc/reference/supervisor?tab-os=linux&tab-language=python#wb\_supervisor\_node\_get\_from\_proto\_def](https://www.cyberbotics.com/doc/reference/supervisor?tab-os=linux&tab-language=python#wb_supervisor_node_get_from_proto_def)

##### Simon Steinmann [Moderator] 02/05/2021 18:33:23
baseNode = supervisor.getSelf()

node1 = baseNode.getFromProtoDef("defName")

pos = node1.getPosition()


I think that should work

##### Bitbots\_Jasper [Moderator] 02/05/2021 18:38:29
awesome, it works, i only have to go back into my proto and add a DEF to each solid :D, or is there a option to do it using urdf2webots, otherwise i can work on implementing it

##### Simon Steinmann [Moderator] 02/05/2021 18:39:14
urdf files dont have DEF names. they have names


I guess you could add a parsable option, to turn urdf link-names into DEF names for the solids, not just solid names

##### Bitbots\_Jasper [Moderator] 02/05/2021 18:40:58
that is what i meant, so that they can be retrieved using the getFromProtoDef function

##### Simon Steinmann [Moderator] 02/05/2021 18:41:57
[https://github.com/cyberbotics/urdf2webots/blob/master/urdf2webots/writeProto.py#L101](https://github.com/cyberbotics/urdf2webots/blob/master/urdf2webots/writeProto.py#L101)


you would have to add it in this line


link.name contains the link-name

##### Bitbots\_Jasper [Moderator] 02/05/2021 18:42:43
thanks so much for your help `@Simon Steinmann` , i'll work on it

##### Simon Steinmann [Moderator] 02/05/2021 18:43:06
[https://github.com/cyberbotics/urdf2webots/blob/master/urdf2webots/importer.py#L190](https://github.com/cyberbotics/urdf2webots/blob/master/urdf2webots/importer.py#L190) here you have to add a parsable argument


you can parse a bool to the writeProto script like this: [https://github.com/cyberbotics/urdf2webots/blob/master/urdf2webots/importer.py#L64](https://github.com/cyberbotics/urdf2webots/blob/master/urdf2webots/importer.py#L64)

##### Welsh\_dragon64 02/05/2021 18:47:09
`@Simon Steinmann`
%figure
![Robotis_op2.PNG](https://cdn.discordapp.com/attachments/565154703139405824/807321432954634260/Robotis_op2.PNG)
%end


can you look at this error plz?


i cant seems to display the robot window?

##### Simon Steinmann [Moderator] 02/05/2021 18:48:20
is the library at the specified path?

##### Welsh\_dragon64 02/05/2021 18:48:33
yea


for some reason it cant seem to load it


the library is in the same the file i am using to work on this robot simulation


i am trying to remotely control the robot using my laptop

##### Bitbots\_Jasper [Moderator] 02/05/2021 19:03:41
`@Simon Steinmann`  i made a pull request, thanks again for your help [https://github.com/cyberbotics/urdf2webots/pull/107](https://github.com/cyberbotics/urdf2webots/pull/107)


I think it depends on your use case, to convert the model to a proto you can use [https://github.com/cyberbotics/blender-webots-exporter](https://github.com/cyberbotics/blender-webots-exporter) as far as i know. If you want to very precisely place every little wheat plant you need individual models, if you only want to place it roughly you can probaly use the tiles they provide in the models you linked to. You need to pay a little attention to the amount of polygons you are using in the end, the models you linked to are quite high resolution and it might cause some issues if there is a gigantic amount of them on the screen.


if they do not come as a tile, you can create your own tile using single wheat plants. for that [https://www.cyberbotics.com/doc/reference/def-and-use](https://www.cyberbotics.com/doc/reference/def-and-use) is probably useful

##### Simon Steinmann [Moderator] 02/05/2021 19:44:39
`@Bitbots_Jasper` PR looks good to me, havent tested it though. Please also add your changes to the readme [https://github.com/cyberbotics/urdf2webots/blob/master/README.md#arguments](https://github.com/cyberbotics/urdf2webots/blob/master/README.md#arguments)

##### John520 02/05/2021 21:18:25
Thank you `@Bitbots_Jasper` for your detailed instruction! I will give it a try.

##### babaev1 02/05/2021 21:42:20
Is there anybody familiar with backlashes in joints of robot?  Is there any method to define value of backlash in HinjeJoint or in RotationalMotor?

##### Bitbots\_Jasper [Moderator] 02/05/2021 22:29:52
done


can you give some more information on your setup? have you checked if the file that is missing `robotis-op2_window.dll` is not in your filesystem, as a last resort you could try to reinstall webots

##### Welsh\_dragon64 02/05/2021 23:16:52
`@Bitbots_Jasper` turns out some files were missing such as transfer,remote control and checkstart position. i just copied paste the webot orignal file to the file i am editing and that fixed the issue i had.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/807389310331519056/unknown.png)
%end


`@Bitbots_Jasper` `@Simon Steinmann` I am trying to work on the object detection on Robotis\_op2 on webots. I would like to know which is better YOLO or Open CV. I would also appreciate any open source codes available

##### Anand 02/06/2021 00:49:15
Hey! I have set all the env variables correctly. Yet when I type "webots" in the terminal, I get an error saying "Command not found, but can be installed with snap". Any idea how to fix this?

##### Simon Steinmann [Moderator] 02/06/2021 10:55:52
`@Anand` you have to add an entry in the .bash\_aliases launching the executable


`alias webots='/home/simon/webots/webots'`  something like this

##### Bitbots\_Jasper [Moderator] 02/06/2021 11:37:01
It depends on what you want to detect. If it is just a red circle on a flat background open cv is fine, if you want to detect different object classes you might want to use yolo, but it could be overkill

##### Simon Steinmann [Moderator] 02/06/2021 11:39:07
I've only dabbled in computer vision, but I think going opencv first is the best approach. You can add YOLO to opencv


`@Welsh_dragon64` If you want to use domain randomization for training your cv, I can probably give you a starting point. I played around with it last year [https://drive.google.com/file/d/1Z2332YFU1Um1NCwx3PoTcOE2jdu6QUxP/view?usp=sharing](https://drive.google.com/file/d/1Z2332YFU1Um1NCwx3PoTcOE2jdu6QUxP/view?usp=sharing)

##### Iris230 02/07/2021 09:52:18
hi guys, I'm wondering what does this error mean?
%figure
![CHVBGQG0CMSPSG0WS0K.png](https://cdn.discordapp.com/attachments/565154703139405824/807911611695890432/CHVBGQG0CMSPSG0WS0K.png)
%end


I just opened the 'highway' sample world, and I changed the controller to the 'racing\_wheel'

##### Usama 02/07/2021 17:43:24
hi guys, i am new to webot, i am trying to add a lidar sensor to my robot but i cannot seem to find it in the base nodes.  in fact i can not find any sensor. how can i add the sensors to the node library?

##### Simon Steinmann [Moderator] 02/07/2021 17:44:13
Sensors have to be children of Solids, Robots or Transforms


and they only work in a robot

##### Usama 02/07/2021 17:45:04
okay thanks

##### Stefania Pedrazzi [Cyberbotics] 02/08/2021 09:35:49
Hi, to recompile the `racing_wheel` controller on Windows you can add the following line in the `Makefile` just after the `ifeq($OSTYPE),windows)` line:

```
LIBRARIES += -L"$(WEBOTS_HOME)/msys64/mingw64/bin"
```

##### yash 02/08/2021 12:21:30
Hi ! Are there any functionalities to model under-actuated behaviour ?  for example a  single motor drives multiple hinge joints.

##### bertlangton 02/08/2021 12:28:17
I have an issue starting up WeBots; it opens, but immediately shuts down after a couple of seconds. I've already tried: 

-Updating graphics card drivers (I have an integrated AMD card)

-Starting in Safe Mode 

-Older versions 

What could be the issue? My computer isn't exactly new, but I believe it does have the required minimum specifications for the program to run.

##### Stefania Pedrazzi [Cyberbotics] 02/08/2021 12:30:30
Is this the first time you start Webots or at some point it was starting?

##### Park Jeong Woo 02/08/2021 12:34:09
Hi, I was trying to do stuff with physics ODE plugin. I wonder is there any way to create solid object that doesn't physically interact with objects but still detects collision?

##### Stefania Pedrazzi [Cyberbotics] 02/08/2021 12:34:48
Currently it is not possible to control multiple joints with the same motor, but this functionality is in our TODO list:

[https://github.com/cyberbotics/webots/issues/1365](https://github.com/cyberbotics/webots/issues/1365)

##### Park Jeong Woo 02/08/2021 12:34:57
ex) ball collides with box but box doesn't get pushed by the ball. Still collision is detected by the box.

##### Stefania Pedrazzi [Cyberbotics] 02/08/2021 12:36:20
Yes, you should define the `boundingObject` field of a `Solid` node but not the `physics` field.

##### Park Jeong Woo 02/08/2021 12:37:00
I tried setting physics with null, but when I moved that object around, it still interacted with other objects.

##### Stefania Pedrazzi [Cyberbotics] 02/08/2021 12:41:02
Ok, so you want to detect the collision to do your stuff but then let the physics engine ignoring it.

This is possible from the physics plugin `webots_physics_collide` function. If this function returns 1 or 2, then ODE will not handle the collision:

[https://www.cyberbotics.com/doc/reference/callback-functions#int-webots\_physics\_collidedgeomid-dgeomid](https://www.cyberbotics.com/doc/reference/callback-functions#int-webots_physics_collidedgeomid-dgeomid)

##### Park Jeong Woo 02/08/2021 12:42:56
Thanks

##### bertlangton 02/08/2021 12:43:50
Nope, it never lasted more than a few seconds

##### Stefania Pedrazzi [Cyberbotics] 02/08/2021 12:50:06
Which OS are you using?

##### bertlangton 02/08/2021 13:24:39
Windows 10

##### Stefania Pedrazzi [Cyberbotics] 02/08/2021 13:47:01
It would be helpful if you could open the Command Prompt, navigate to the Webots installation directory, type the following instructions and post the output:

```
cd msys64\mingw64\bin
webots.exe --sysinfo
```

##### bertlangton 02/08/2021 15:21:53
sure, here it is:



System: Windows 10 64-bit

Processor: Intel(R) Core(TM) i3-6100U CPU @ 2.30GHz

Number of cores: 2

OpenGL vendor: ATI Technologies Inc. (0x1002)

OpenGL renderer: AMD Radeon (TM) R5 M430 (0x6660)

OpenGL version: 4.5.13469 Compatibility Profile Context 21.19.512.4

##### Olivier Michel [Cyberbotics] 02/08/2021 15:28:18
Did you try to update your graphics driver?

##### bertlangton 02/08/2021 15:38:29
Yup.


After that, it stayed open for a few seconds longer, but other than that it didn't help

##### Stefania Pedrazzi [Cyberbotics] 02/08/2021 15:47:52
The OpenGL version seems rather new. I'm checking in our telemetry database to see if other users could successfully run Webots with this OpenGL version and similar setup. But for the moment it seems that most of the users that enabled the telemetry are using older OpenGL versions (usually 3.3, for example `3.3.13596 Core Profile Forward-Compatible Context`)


Maybe it could also be an option to try to downgrade the drivers to check if it works.

##### Iris230 02/08/2021 16:05:00
thank you! It works😆

##### Stefania Pedrazzi [Cyberbotics] 02/08/2021 16:07:02
Thank you for reporting this bug! I updated the Webots Makefile and the fix will be included in the next release of Webots.

##### alejanpa17 02/08/2021 16:38:47
Hey guys, i need some help here. I have DEF this boundingObject but when i want to use it in other solid It doesnt appear. Am I doing something wrong?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/808376295079018548/unknown.png)
%end



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/808376461911261244/unknown.png)
%end

##### Olivier Michel [Cyberbotics] 02/08/2021 16:50:00
You can include a USE of this DEF only at a lower position in the scene tree. Where are you trying to include a USE of it?

##### alejanpa17 02/08/2021 16:52:41
For example in this one
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/808379791899099136/unknown.png)
%end


I just found out that you can include a USE only upper the DEF (in the way closer to the root of the scene tree). So if it is at the same level is impossible?

##### Olivier Michel [Cyberbotics] 02/08/2021 17:09:52
Yes, it should be possible at the same level, but the DEF has to come first (upper).


If you save your world file and search for the USE/DEF name, you should first find the DEF, then the USE.

##### alejanpa17 02/08/2021 17:26:24
Look, I have it the DEF there but the solids where I'm pointing It doesn't appear the USE
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/808388279052533810/unknown.png)
%end


I think is kind of a bug because I did a boundingObject DEF of a Box for testing and It works with the solids under it, just like you said


Its happening when you have this type of structure of the boundingObject DEF: Group>>Transform>>Box
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/808396782168965120/unknown.png)
%end


You can try yourself

##### Stefania Pedrazzi [Cyberbotics] 02/08/2021 19:29:02
`@alejanpa17` The DEF "test" node can only be used in `boundingObject` fields because this structure (without a `Shape` node between the `Transform` and the `Box` nodes) is a particular simplified structure not valid for graphical objects.

If you also want to use it outside the `boundingObject` to define the a graphical object, then I would suggest you to declare the DEF node for the graphical object and then the USE for the boundingObject.

##### alejanpa17 02/08/2021 20:40:19
Just found out this is working!! It turns out that you need a Shape node between Transform and Box node. Thank you!!
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/808437079990206534/unknown.png)
%end

##### AJ121 02/09/2021 06:51:52
Which sensor in webots can differentiate between colors?

##### Olivier Michel [Cyberbotics] 02/09/2021 07:04:37
A standard RGB camera.


Note: an infra-red distance sensor is also affected by colors of obstacles.

##### Bitbots\_Jasper [Moderator] 02/09/2021 07:05:39
if you just want a simple color sensor you can probably set the resolution to 1x1 pixels

##### AJ121 02/09/2021 07:06:00
I want to differentiate between black and white paths in a line follower

##### Olivier Michel [Cyberbotics] 02/09/2021 07:07:00
In that case, it is simpler and more efficient to use a DistanceSensor with type set to "infra-red". You will measure a larger value for the white and a lower value for the black.

##### AJ121 02/09/2021 07:07:47
Somehow the reading of my infrared come just the opposite for white it is <1000 and for black it is 1000

##### Olivier Michel [Cyberbotics] 02/09/2021 07:10:18
That may depend on the values set in lookupTable of the DistanceSensor.

##### AJ121 02/09/2021 07:11:31
Ok thanks !


I need a bit of help with my line following algorithm

##### popo74013 02/09/2021 09:05:00
i have a question related to the camera that works as rgb


i try to use the camera to read rgb but i am not getting the write value


my camera is close to an ir sensor, deos this affect it?

##### Olivier Michel [Cyberbotics] 02/09/2021 09:07:09
No, it shouldn't.

##### Bitbots\_Jasper [Moderator] 02/09/2021 09:07:17
you can try to visualize the cameras viewframe using view->show optional renderings->show camera frustum


maybe you also need to start your camera [https://cyberbotics.com/doc/reference/camera#wb\_camera\_enable](https://cyberbotics.com/doc/reference/camera#wb_camera_enable)

##### popo74013 02/09/2021 09:09:41
i did use enable command


but i am reading the wrong value


is it possible to go on a audio call with anyone?

##### Bitbots\_Jasper [Moderator] 02/09/2021 09:10:45
did you try this?

##### popo74013 02/09/2021 09:10:56
doing it now


it doesnt make any difference


i am quite sure that there is an issue related  to code


def colour():

    #identify colour

    cam.enable(TIME\_STEP)

    cameraData = cam.getImage()

    # get the red component of the pixel (5,10)

    red = cam.imageGetRed(cameraData, cam.getWidth(), 1, 1)

    green = cam.imageGetGreen(cameraData, cam.getWidth(), 1, 1)

    blue = cam.imageGetBlue(cameraData, cam.getWidth(), 1, 1)

    # red = pixel[0]

    # green = pixel[1]

    # blue = pixel[2]

    cam.disable()

    return (red,green,blue)


this is the function to read the colour


but the camera most of the time reads a certain colour and only at some random moments it reads a couple of more colours

##### Bitbots\_Jasper [Moderator] 02/09/2021 09:21:16
enable your camera once and then let it run, dont try to enable and disable it at every simulation step

##### popo74013 02/09/2021 09:22:47
compass = robot.getDevice('compass')

compass.enable(TIME\_STEP)


i have added these bits outside of the function

##### Bitbots\_Jasper [Moderator] 02/09/2021 09:24:06
i think it takes one simulation step to get the image from the simulation into the frame buffer of the camera


to test you can put a step simulation between the cam.enable and the cam.getImage

##### popo74013 02/09/2021 09:27:46
how can i do this


and why does reads red when there is nothing red in front of the robot?

##### Bitbots\_Jasper [Moderator] 02/09/2021 09:33:09
```
simulation -> camera buffer -> your code
            1                2
```


1 only happens when you step the simulation


and you are therefore doing 2 before 1


and there might be anything in the camera buffer before it is properly initialized


that is the nature of RAM


it should be something like controller.step(TIME\_STEP)

##### popo74013 02/09/2021 09:40:16
i think i figured it out


when you click on the camera which is located at the children file of the robot


i moved the pink pyramid shape of the camera in the front and now it reads the right colour


what is actually the pink pyramid?

##### Bitbots\_Jasper [Moderator] 02/09/2021 09:42:32
it is called the camera frustum


basically what the camera sees

##### popo74013 02/09/2021 09:42:45
ohhh


thank you! now it works perfectly and i get the rgb values


thank you!

##### nelsondmmg 02/09/2021 13:42:16
Hi, I'm would like to write a sh file that executes multiple webots simulations (sequentially), changing the arguments of the controllers used inside the simulation. Can I modify such parameters from the command line directly or do I need to modify the .wbt file before each execution? Also, is it possible to register videos from the simulation using the command line (if possible without opening the webots window). How can I automatize these simulations .In the documentation the "Starting webots" page only gives details about launching the application. Thanks.

##### Simon Steinmann [Moderator] 02/09/2021 13:44:16
`@nelsondmmg` you could use extern controllers and start those seperately, parsing arguments with python for example

##### Darko Lukić [Cyberbotics] 02/09/2021 13:44:17
`@nelsondmmg` You can add a Supervisor node which changes the arguments and restarts the simulation. Same for videos, you can use the Supervisor node to record videos.

##### Simon Steinmann [Moderator] 02/09/2021 13:46:24
you can also load the world from a python script



> **Attachment**: [launch\_webots\_world.py](https://cdn.discordapp.com/attachments/565154703139405824/808695365048991764/launch_webots_world.py)


on the bottom of the script is the example usage under \_\_main\_\_()


example on how to start your python controller
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/808697787134771261/unknown.png)
%end


this will launch the world, and make the extern controller connect to the correct webots instance

##### nelsondmmg 02/09/2021 14:06:13
Ok, but since I'm already working with C++ I would like to use it instead of python (but it's nice to know that there is an alternative). Now, if I create a supervisor node to manage the simulation (keeping in mind that all the objects in the simulation are also supervisors, since everyone recovers the position from the others directly from the node) how can I launch the other controllers (reset the controllers and the simulation and change the arguments can be done using the supervisor functions, but I do not see a start function). Also, if there a way to known that a controller ended execution, or some sort of message needs to be sent to the supervisor ?

##### popo74013 02/09/2021 14:11:23
hello, i have a question realated to motors


i try to se their position but they do some funny moves


i think it might be an issue with the reference position

##### Simon Steinmann [Moderator] 02/09/2021 14:12:57
`@nelsondmmg` If it is the same world and robots, just turn the controllers into extern controllers. After a reset or world load, the simulation will only start, if all extern controllers have been started. So you can just launch them with the specific args you want


`@popo74013` you have to give us more details than that. Perhaps show us your controller code and explain exactly what it is doing, and what it SHOULD do

##### popo74013 02/09/2021 14:17:07
i made a robot and i used 2 motors to control the gripping mechanisms


and once the robot moves and detects an object it is supposed to grip it


but when i set the motors' position at an angle, they move randomy


i think it is because they are not oriented correctly

##### Simon Steinmann [Moderator] 02/09/2021 14:19:08
Make sure the axis of the jont is defined properly


also the anchor and 'translation' field of the endPoint Solid for each joint


usually you want them to be identical

##### popo74013 02/09/2021 14:20:09
ohhh, thank you


i will try it and i will let you know!

##### nelsondmmg 02/09/2021 14:21:50
Is there some documentation about extern controllers? Maybe is a new feature and I'm not familiarized, but since there is some dependency related to the control of objects in the code (in my case I need to initialize the driver object to control vehicles) I should not start these controllers outside webots and then attach then to the webots process.

##### Simon Steinmann [Moderator] 02/09/2021 14:22:27
[https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers](https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers)

##### nelsondmmg 02/09/2021 14:40:59
Ahh thanks, now it is much more clear.

##### Olivier Michel [Cyberbotics] 02/09/2021 15:51:37
It could be very nice if you could share your experience using Webots with ROS at [https://discourse.ros.org/t/why-is-robotics-simulation-hard/15888/11](https://discourse.ros.org/t/why-is-robotics-simulation-hard/15888/11)

##### popo74013 02/10/2021 10:02:13
hello


is there a way to make a motor act as a servo?


i am trying to make a really simple grabbing mechanism and i just want to make 2 beams to rotate from 0 to 90 degrees

##### Darko Lukić [Cyberbotics] 02/10/2021 10:03:53
Hello `@popo74013`, Yes, if you control the motor using `wb_motor_set_position` it acts as a servo.

##### popo74013 02/10/2021 10:04:15
grips = []

gripsNames = ['left\_motor', 'right\_motor']

for i in range(2):

    grips.append(robot.getDevice(gripsNames[i]))

    grips[i].setPosition(3.14/2)        

    grips[i].setVelocity(0.0)


i am writing in python but this doesnt seem to work

##### Simon Steinmann [Moderator] 02/10/2021 10:39:36
`@popo74013` dont set the velocity

##### popo74013 02/10/2021 10:39:56
ohh i think it works now


it was an issue in the design


the right grip was working fine but the left not


so i modified the right motor and beam to look like the left and now it works


thank you!

##### Simon Steinmann [Moderator] 02/10/2021 10:41:21
[https://cyberbotics.com/doc/reference/motor?tab-language=python#force-and-torque-control](https://cyberbotics.com/doc/reference/motor?tab-language=python#force-and-torque-control)


this table tells you exactly what the commands do. Motors can be in position-, velocity-, and torque control

##### Saud 02/10/2021 11:16:36
Hello, I have created a sphere on a new world. I want to know if and how i can increase the size of this shpere with respect to time as the simulation runs? i am very new to webots

##### Simon Steinmann [Moderator] 02/10/2021 11:22:30
`@Saud` The easiest would be to use a supervisor controller. Get a handle to the radius Field of the sphere, and change the value

##### bingdong 02/10/2021 11:52:53
Hi! New to Webots and Blender. As I understand both support python programming. But is it the case that python scripts can be used in blender to create animations and in Webots to build robot controllers? 

I found a simple legged robot blender file online which makes it move when a python script. When I exported it to Webots, the model doesn't move. I'm guessing that the same script won't work for controller? What can I do to make the robot move then?

##### Simon Steinmann [Moderator] 02/10/2021 12:11:05
You will have to write a controller [https://www.cyberbotics.com/doc/guide/controller-programming](https://www.cyberbotics.com/doc/guide/controller-programming)


<@&568329906048598039> Is there still separate animation functionality? Not sure about that

##### Stefania Pedrazzi [Cyberbotics] 02/10/2021 12:17:40
No, Webots doesn't have any built-in functionality for animations.

For the moment the way to go is to write a controller program that reads the animation file and using the Supervisor API moves the different robot parts accordingly.

##### bingdong 02/10/2021 12:34:31
`@Simon Steinmann` `@Stefania Pedrazzi` Thanks for the guidance. I'll try it out!

##### Simon Steinmann [Moderator] 02/10/2021 12:48:51
is there an example for this? I remember this question coming up 3 times this winter already

##### Stefania Pedrazzi [Cyberbotics] 02/10/2021 12:50:57
Not yet, but it is in our short-term TODO list to add an example for the human animations (and release the experimental Skin node).

##### ChristinaP 02/10/2021 12:51:28
Hello everyone! I'm just starting using webots to simulate an autonomous vehicle. Is there an option to alter the input of the sensors (camera, lidar and gnss) while the simulation is running in order to figure out what's gonna happen at the vehicle in case one of those sensors malfunctions? Do those sensors affect the simulation on runtime?

##### csnametala 02/10/2021 12:56:31
Hello everyone! I'm using a PEN function on the e-puck robot, it leaves a trail that doesn't erase. Can I define a distance from this trail? For it to erase as the robot walks

##### Olivier Michel [Cyberbotics] 02/10/2021 13:13:31
You can, at the controller level, add an extra layer that will alter sensor data before passing then to your control algorithm. Also, in Webots, depending on the sensor, you can add different types of noise. See the specific sensor descriptions in the documentation to understand what kind of noise can be added.


Yes, you can use WorldInfo.inkEvaporation, see [https://cyberbotics.com/doc/reference/worldinfo#worldinfo](https://cyberbotics.com/doc/reference/worldinfo#worldinfo)

##### csnametala 02/10/2021 13:24:46
OK, thank you!


I found the problem I was having. I should control the evaporation of the pen in the inkEvaporation field of the WorldInfo node. thank you again.

##### waleed 02/10/2021 15:00:16
Can someone help on how to build a connection between the webots drone and kinect sensor

##### Simon Steinmann [Moderator] 02/10/2021 15:01:35
Can you be a bit more specific? What exactly do you want? Physically attach the kinect to the drone?

##### waleed 02/10/2021 15:02:31
So i am using a physical kinect sensor and a simulated drone that is available in the webots software


So the drone is not physical


I want to develop a communication between the physical kinect sensor and the drone available in webots software

##### Simon Steinmann [Moderator] 02/10/2021 15:04:11
[https://www.cyberbotics.com/doc/reference/emitter](https://www.cyberbotics.com/doc/reference/emitter)


you can use emitter and receiver nodes in Webots


basically simulates radio communication

##### waleed 02/10/2021 15:05:58
I see that so in my case i could send signals and use the kinect sensor to receive the nodes

##### Simon Steinmann [Moderator] 02/10/2021 15:06:22
what exactly do you want to do?


send movement commands to the drone depending on what the kinect sees?

##### waleed 02/10/2021 15:06:59
Yes exactly


Like that

##### Simon Steinmann [Moderator] 02/10/2021 15:07:17
then add a receiver to the drone, and an emitter to the kinect


both have to be defined as a robot. With a controller each

##### waleed 02/10/2021 15:08:57
Just one more clarification the code for my kinect is written in C# and i have used visual community in windows so do i have to change my code into C++ to make it possible

##### Simon Steinmann [Moderator] 02/10/2021 15:09:46
I'm a python and ROS guy, so I dont know. But that seems right.


I'd recommend going through some of the documentation of Webots and having a look at some sample worlds, in order to get a better base understanding of Webots

##### waleed 02/10/2021 15:11:02
Okay fair enough! But#bw to run the testing of the physical kinect using webots


To test ny gestures on a physical kinect sensor is it possible


?

##### Simon Steinmann [Moderator] 02/10/2021 15:11:36
wait, do you want to simulate the kinect, or use the physical kinect to control the simulated drone?

##### waleed 02/10/2021 15:11:54
Yes use the physical kinect to control the simulated drone

##### Simon Steinmann [Moderator] 02/10/2021 15:12:02
ohhhh okay, that is different


you're using the mavic2Pro in Webots?

##### waleed 02/10/2021 15:12:25
Yes mavic 2 pro

##### Simon Steinmann [Moderator] 02/10/2021 15:12:46
the default controller there is in c


you can take a look at it. You simply have to get your kinect data in there


how you do it, does not really matter


or you write your own in any of the supported languages

##### waleed 02/10/2021 15:14:29
Yes i have gone through the default controller snd the only part i dont get is how to get the kinect data in there

##### Simon Steinmann [Moderator] 02/10/2021 15:15:03
I have no experience with c#


<@&568329906048598039> Does anyone know how communicate between c# and c/c++?

##### waleed 02/10/2021 15:16:39
Well i would change my code from C# to C++ so webots can support that language


It is just the communication bit that needs to be sorted between a physical kinect and a simulated drone

##### HANEEN AL ALI 02/10/2021 15:20:16
Hi, I am building a manipulator and I want the first joint to move 180 along the x axis.  How i can do that? Do I need to change the motor parameters or is it related to the code?

##### Simon Steinmann [Moderator] 02/10/2021 16:54:37
Linear or rotational motor?


The type of movement is defined in the joint

##### HANEEN AL ALI 02/10/2021 17:54:36

%figure
![SmartSelect_20210210-175423_Samsung_Notes.jpg](https://cdn.discordapp.com/attachments/565154703139405824/809120151077257266/SmartSelect_20210210-175423_Samsung_Notes.jpg)
%end


Rotational motor like pic above . I want link 1 to move from -x to x

##### Simon Steinmann [Moderator] 02/10/2021 18:12:03
ohh, like a cartpole?


well you have to set up the hingejointParameters correctly


with the axis defined the way you want

##### AJ121 02/10/2021 18:16:14
Why is it that the code run on my bot with my world runs differently on someone else's computer with same world and same bot ?

##### Chernayaten 02/10/2021 18:23:30
Differently how? If it's the speed then that's most likely due to difference in the CPU/GPU capabilities of your computers



[https://www.cyberbotics.com/doc/guide/speed-performance](https://www.cyberbotics.com/doc/guide/speed-performance)

##### AJ121 02/10/2021 18:25:04
Ok ill look to it thanks


I need a brief syntax of switching leds on and off in webots ...please help

##### Stefania Pedrazzi [Cyberbotics] 02/11/2021 07:27:24
Here is the LED API: [https://www.cyberbotics.com/doc/reference/led#wb\_led\_set](https://www.cyberbotics.com/doc/reference/led#wb_led_set)

##### Darko Lukić [Cyberbotics] 02/11/2021 07:44:08
`@waleed` You want to send images from your physical Kinect to the simulated drone? In that case, you don't need C#-C/C++ communication, you can read the images directly in your C/C++ controller:

[https://homes.cs.washington.edu/~edzhang/tutorials/kinect2/kinect1.html](https://homes.cs.washington.edu/~edzhang/tutorials/kinect2/kinect1.html)


In general, to write a controller in non-supported language, you can develop a third-party support like it is done for Haskell:

[https://github.com/cyberbotics/HsWebots](https://github.com/cyberbotics/HsWebots)



or use TCP/IP:

[https://cyberbotics.com/doc/guide/interfacing-webots-to-third-party-software-with-tcp-ip](https://cyberbotics.com/doc/guide/interfacing-webots-to-third-party-software-with-tcp-ip)

##### nelsondmmg 02/11/2021 10:03:12
Hi, is there some way to find if a controller for a robot is in execution? From the supervisor that launch the extern controllers (with the system call that detach this new process from the supervisor itself). Thanks

##### Saud 02/11/2021 10:45:34
Hi i just wanted to ask that this is the correct way to print something out right. I am using C++. I have also included iostream but nothing actually prints out
%figure
![Screenshot_2021-02-11_at_10.44.30.png](https://cdn.discordapp.com/attachments/565154703139405824/809374568019656704/Screenshot_2021-02-11_at_10.44.30.png)
%end


Am I doing something wrong

##### Stefania Pedrazzi [Cyberbotics] 02/11/2021 10:47:29
In C++ you also need to print `std::endl` at the end of the log to flush the buffers.

[http://www.cplusplus.com/reference/ostream/endl/](http://www.cplusplus.com/reference/ostream/endl/)

##### Saud 02/11/2021 10:50:04
hmm right i thought that was mostly for a new line. just tried it. nothing

##### Mohannad Kassar 02/11/2021 10:50:10
hello guys, I'm taking webots tutorial and I want to build my senior project using webots. I want to ask you if I can implement AI algorithms that can manipulate a robotic arm using a bayesian neural network that get the distination as an input and the output is the angles of the actuators of this arm. is this possible ?

##### Stefania Pedrazzi [Cyberbotics] 02/11/2021 10:52:19
Are you running the simulation step-by-step? In this case the logs are currently printed one step later in the Webots console.

##### Bitbots\_Jasper [Moderator] 02/11/2021 10:53:22
it sounds like you want to teacha bayesian neural network to do inverse kinematics, is that correct?

##### Mohannad Kassar 02/11/2021 10:53:40
exactly

##### waleed 02/11/2021 10:54:09
Hi — thanks for the help and i will look into it. But in my case i was going to do live gestures and in my opinion if i were to send images that would produce a lot error in controlling the simulated drone

##### Bitbots\_Jasper [Moderator] 02/11/2021 10:55:05
you dont really need a simulation for that, you only need forward kinematics to get the error between the pose that the manipulator would be in if you applied the joint angels from the network and the pose you wanted it to be in

##### Saud 02/11/2021 10:55:19
yes but this is out of the while loop at the moment just to test. the actual thing i want to print is in the while loop. i put it out to see if it actually prints something out

##### Bitbots\_Jasper [Moderator] 02/11/2021 10:56:34
what is the purpose of using a neural network rather than something like fastIK or trac\_ik

##### Mohannad Kassar 02/11/2021 10:56:52
yes but I proposed the neural networks methods in my proposal for the project. fell like I'm in a real trouble 🥲

##### Stefania Pedrazzi [Cyberbotics] 02/11/2021 10:57:59
It is tricky to do it within Webots because the simulation doesn't run until all the extern controllers are started. You may try to modify the world from your robot controller and check for the modification from the supervisor controller (for example setting the `Robot.customData` field).

It seems better to check which processes are running on the system using standard libraries (for example psutil for Python, etc.)

##### Bitbots\_Jasper [Moderator] 02/11/2021 10:59:56
for inverse kinematics tasks neural networks usually dont work that well as far as i know, there is an evolutionary ik called bioIK that we use and works pretty well

##### Mohannad Kassar 02/11/2021 11:00:16
I didn't get you dear

##### Stefania Pedrazzi [Cyberbotics] 02/11/2021 11:00:34
Try to run it in realtime mode instead of step-by-step. In any case the instruction seems correct and it should work if you don't have other issues in your controller.

You can also check the C++ example in `projects/languages/cpp/worlds/example.wbt` where many messages are printed from the controllers.

##### Bitbots\_Jasper [Moderator] 02/11/2021 11:01:00
why do you want to use a neural network to solve the problem of inverse kinematics rather than an interative algorithm?

##### Mohannad Kassar 02/11/2021 11:02:49
I can use anything that works, I want the robotic arm to reach a destination of a surface when it is detected using a smart camera

##### Saud 02/11/2021 11:02:57
sorry yes i mean that i am running it on realtime. and also i should say that i am using supervisor mode so i hope that doesn't make a difference

##### Bitbots\_Jasper [Moderator] 02/11/2021 11:06:22
it is definitely possible to simulate a robotic arm and a regular rgb camera as well as a deph camera (maybe you can elaborate on what you mean by smart camera?)  in webots if that is what you meant to ask


but moving an arm from a to b is not as simple as it might seem, if you do jointspace interpolation, meaning just turn all the joints to the target position, you will have unexpected movements and possibly collision

##### Mohannad Kassar 02/11/2021 11:09:42
iw webots I found a smart camera with object recognition, you can check it in the user manual


I will do my best to work

##### Bitbots\_Jasper [Moderator] 02/11/2021 11:11:24
ah ok, just found the recognition node, did not know about that

##### Mohannad Kassar 02/11/2021 11:11:39
called mobileye

##### Bitbots\_Jasper [Moderator] 02/11/2021 11:16:17
i'm not sure that i understand your use case well enough, but I would recommend some motion planning framework such as moveit if you want to achive any complex movements

##### Mohannad Kassar 02/11/2021 11:17:21
this is a software ?

##### Bitbots\_Jasper [Moderator] 02/11/2021 11:17:43
[https://moveit.ros.org/](https://moveit.ros.org/)

##### Mohannad Kassar 02/11/2021 11:18:46
I'm also creating a 3D model using blender, would it work using moveit ?

##### Bitbots\_Jasper [Moderator] 02/11/2021 11:25:39
you need a urdf (basically a description of your robots kinematics) and a sdf (basically a semantic description of your robot) for moveit to work


how complex is your manipulator?

##### Mohannad Kassar 02/11/2021 11:26:34
is 3 Degrees of Freedom an answer for your question ?

##### Bitbots\_Jasper [Moderator] 02/11/2021 11:28:04
then maybe moveit is a bit overkill, it depends on how complex your environment is but with only 3 DoF you can probably use some jointspace interpolation


i thought you had something like a 6 DoF robot arm

##### Mohannad Kassar 02/11/2021 11:29:24
no it is only 3. anyway i had a look in moveit, it is very complicated as my first impression

##### Bitbots\_Jasper [Moderator] 02/11/2021 11:30:26
yes it is quite complicated, but for more complex motion planning tasks with more degrees of freedom some motion planning framework is usually required

##### Mohannad Kassar 02/11/2021 11:32:59
I have 3 main algorithms in this project need to be implements, I'm thinking to make something on GitHub so maybe I can got some help if needed

##### Stefania Pedrazzi [Cyberbotics] 02/11/2021 11:33:05
No, there is no difference if you use the Supervisor.

##### Mohannad Kassar 02/11/2021 11:33:10
implemented*

##### nelsondmmg 02/11/2021 12:58:13
Hmm got it, I'm using c++ but I'm gonna try to find some function to check the pid from the controller process. Thanks!


I actually created a isCtrlRunning variable into the object proto as a flag, easier that to do a system call every step. Thanks 🙂


Is there a way to record a movie in full-screen without the window warning the change to full-screen appearing? I'm launching the movie from code and each time it blocks the execution.

##### John520 02/11/2021 15:45:06
Hi guys, I have a question about adding the physics to the potted tree. When the simulation starts, the potted tree with physics added sinks a bit into the floor. Could you please advise me on this issue?
%figure
![Kazam_screenshot_00005.png](https://cdn.discordapp.com/attachments/565154703139405824/809449947904737290/Kazam_screenshot_00005.png)
%end

##### Simon Steinmann [Moderator] 02/11/2021 15:58:30
`@John520` you can give then a seperate contact material and decrease the soft\_cfm value


you can also decrease the cfm value of the whole world, but collisions might become "too hard"

##### John520 02/11/2021 16:10:41
`@Simon Steinmann` Thank you for your direction. I added a contact material for TruckWheels before. Would it be similar to add one for the potted tree?
%figure
![Kazam_screenshot_00006.png](https://cdn.discordapp.com/attachments/565154703139405824/809456385867251722/Kazam_screenshot_00006.png)
%end


By the way, why it does not work if the material2 is "TruckWheel"?

##### Simon Steinmann [Moderator] 02/11/2021 16:15:02
what does not work?


and yes, would be the same


you can add as many contact properties as you like

##### John520 02/11/2021 16:32:39
`@Simon Steinmann`. Oh, I added a truck and a contact property for it. The truck will sink a bit into the floor if the material2 is "TruckWheel".


But if the material2 is "TruckWheels", the truck will not sink and it looks very good.

##### Simon Steinmann [Moderator] 02/11/2021 16:37:32
contact properties adjust the interaction between 2 contact materials


so it depends on what material you have in your wheels

##### John520 02/11/2021 16:47:38
Thank you `@Simon Steinmann`. Should I check the Proto file to know what the material it is?

##### Simon Steinmann [Moderator] 02/11/2021 17:01:37
in the scene tree, just change the contactmaterial name of your wheels

##### John520 02/11/2021 17:19:08
Thank you very much. I found the contactMaterial name of the truck in the TruckWheel.proto.


But I could not find the contactMaterial name of the PottedTree.proto. Please see [https://github.com/cyberbotics/webots/blob/released/projects/objects/plants/protos/PottedTree.proto](https://github.com/cyberbotics/webots/blob/released/projects/objects/plants/protos/PottedTree.proto)

##### Simon Steinmann [Moderator] 02/11/2021 17:22:26
you could convert it to basenodes and add it yourself


or you edit the proto


if something is not included, it uses the default values

##### John520 02/11/2021 17:27:42
Thank you very much for your help! It works!


Is there a setting in the PottedTree to allow a truck to go through it? It is like a tractor going through in the crop field.

##### Simon Steinmann [Moderator] 02/11/2021 17:42:18
disable the bounding Object

##### vinwan 02/11/2021 17:52:16
I tried running the latest webots build on computer which has intel core i5 2520M which ha intel HD 3000 graphics with directX 12 but no dedicated GPU . After Webots was installed it opened and quit on me after 2 seconds or soo i only saw the loading screen .could someone help me out on this?

##### John520 02/11/2021 17:52:32
Thank you `@Simon Steinmann` . I first convert the PottedTree to the base node and then reset its boundingObject Group to default value. But the PottedTree will sink and disappear under the floor.

##### Simon Steinmann [Moderator] 02/11/2021 17:53:22
then disable physics to make them static objeccts

##### John520 02/11/2021 17:57:20
Thanks a lot for your help, `@Simon Steinmann` !

##### KajalGada 02/11/2021 19:42:44
Has anyone used the Youbot in Webots? I tried to run each wheel with different velocity and they don't behave like a typical Mecanum wheel. I could only get the robot to drive forward, backward and turn while driving ahead. I wasn't able to get it to drive right/left while facing forward. Anyone faced similar problems?

##### bellino 02/11/2021 22:44:42
Has anyone here ever set up an extern controller and run it as a ROS node? I've set up both ROS and webots and installed ros-noetic-webots-ros but it keeps throwing an error at the first line (an import statement) of my python file

##### Bitbots\_Jasper [Moderator] 02/11/2021 22:45:37
you probably need to set your environment variables correctly


we use this: [https://github.com/bit-bots/wolfgang\_robot/blob/master/wolfgang\_webots\_sim/scripts/setenvs.sh](https://github.com/bit-bots/wolfgang_robot/blob/master/wolfgang_webots_sim/scripts/setenvs.sh)


then just `source setenvs.sh` and try to start your controller in the same terminal

##### bellino 02/11/2021 22:49:03
hm I have set most of them, I put them in my .bashrc file, except what's on line 5. What does that do exactly?

##### Bitbots\_Jasper [Moderator] 02/11/2021 22:49:35
dont really know to be honest


what exactly fails to import?

##### bellino 02/11/2021 22:50:12
Also, I just put my files in src in my catkin\_ws and I start the node using rosrun packagename filename (as opposed to roslaunch webots\_ros and so on, like they do in the docs)


this is the error message I get: line 1: import: command not foundfrom: can't read /var/mail/controllers


so I guess it is the import statement itself?

##### Bitbots\_Jasper [Moderator] 02/11/2021 22:51:29
you forgot your shebang


add this as the first line of your code


`#!/usr/bin/env python3`

##### bellino 02/11/2021 22:53:20
ooh nice, now it just tells me that it doesn't know controller, which is what I first had when my environment variables were set up wrongly. Thank you!

##### Bitbots\_Jasper [Moderator] 02/11/2021 22:54:20
but you know how to fix it?

##### bellino 02/11/2021 22:56:14
okay I take back what I said, this is not the same error because running the external controller outside of ROS still works

##### Bitbots\_Jasper [Moderator] 02/11/2021 22:57:00
can you start your controller using `python controller.py`


without rosrun

##### bellino 02/11/2021 22:57:37
yep, that works just fine

##### Bitbots\_Jasper [Moderator] 02/11/2021 22:58:48
we start our ros controller for webots using a shell script: [https://github.com/bit-bots/wolfgang\_robot/blob/master/wolfgang\_webots\_sim/scripts/start.sh](https://github.com/bit-bots/wolfgang_robot/blob/master/wolfgang_webots_sim/scripts/start.sh)


which is called in a launch file


basically only line 8 in [https://github.com/bit-bots/wolfgang\_robot/blob/master/wolfgang\_webots\_sim/launch/simulation.launch](https://github.com/bit-bots/wolfgang_robot/blob/master/wolfgang_webots_sim/launch/simulation.launch)

##### bellino 02/11/2021 23:00:25
thank you, I will take a look!


just as a little fyi and also because I cannot help shaking my head, there was no problem... the import that failed was from a file of mine that was in a folder called controllers. Thank you `@Bitbots_Jasper` for your help!

##### vinwan 02/12/2021 07:31:37
I tried running the latest webots build on computer which has intel core i5 2520M which ha intel HD 3000 graphics with directX 12 but no dedicated GPU . After Webots was installed it opened and quit on me after 2 seconds or soo i only saw the loading screen .could someone help me out on this?

##### Darko Lukić [Cyberbotics] 02/12/2021 07:56:41
Can you try a safe mode:

[https://www.cyberbotics.com/doc/guide/starting-webots#safe-mode](https://www.cyberbotics.com/doc/guide/starting-webots#safe-mode)

##### vinwan 02/12/2021 08:33:40
`@Darko Lukić` Thank you for replying I will try this out and let you know of the outcome

##### nelsondmmg 02/12/2021 14:28:39
Hi, I have a question about the usega of extern controllers. During a simulation the step is executed only after all robots executed the step() function. How can I make a simulation continue if one of these extern controller finished its execution (and therefore does not execute step() anymore)? 

EDIT: Just found out that I just needed to set synchronization to false (but the pedestrians in my version, 2020a, did not have this attribute so I created).

##### Olivier Michel [Cyberbotics] 02/12/2021 14:54:59
Did you run the youbot demo distributed with Webots? The source code of the controller shows you how to make all these movements, including the side movement. Also, when the demo is over, you should be able to control the robot from the keyboard on all these motions.

##### vinwan 02/12/2021 15:03:25
Unfortunately it didn't workout webots still quits on me

##### Saud 02/12/2021 16:28:16
I am getting this error and i am quite confused why : 

fatal error: 'webots/robot.h' file not found

##### Irunit 02/12/2021 17:22:33
Hi! I was writing custom scenarios for simulation with the help of a script. But when I reset the simulation the floor node is not removed and can be seen in the scene tree whereas other nodes (entities) are removed. Can anyone please help me with this?

##### Krish 02/12/2021 17:45:49
Can there be a feature added in Webots that can make a drop-down wherever we declare a function or write if-elif-else anywhere in the code so that we can minimize it by pulling it up with the arrow, if we dont need to see it that much.

This will surely decrease the time to scroll up & down, and is also found in most editors.

##### Saud 02/12/2021 19:21:24
I am doing one of the tutorials and i tried to add the supervisor. and got this error. does anyone know why ?
%figure
![Screenshot_2021-02-12_at_19.20.10.png](https://cdn.discordapp.com/attachments/565154703139405824/809866771536412682/Screenshot_2021-02-12_at_19.20.10.png)
%end

##### waleed 02/12/2021 20:06:53
Shello


Can someone guide me on how to use the kinect sensor in webots


Does it have its own controller code and can it do live skeletal gesture tracking

##### vinwan 02/13/2021 07:40:26
Can something be done about this?
%figure
![IMG-20210213-WA0001.jpg](https://cdn.discordapp.com/attachments/565154703139405824/810052752037380116/IMG-20210213-WA0001.jpg)
%end

##### Simon Steinmann [Moderator] 02/13/2021 16:56:12
`@Saud` you initialize the robot class, and then the supervisor. The supervisor has all the robot functionality plus extra supervisor ones. So when your robot is a supervisor, don't use robot. Only use supervisor. You can use it in the very same wa as the robot


You can keep the same variable name as before, so you only need to change line 32 and not the rest of your code


`@vinwan` have you checked the documentation mentioned in the error message? And are the newest graphics drivers installed for your gpu?


`@AJ121` I'll answer here, as it seems more like a technical question. Of course you can set the speed higher, but for very hogg speeds you mat have to lower the basic timestep. If you are at 32ms, that's about 30 steps per second. Can you provide more info? What error are you getting? Are you using velocity or position control? What are the parameters of your motor?

##### vinwan 02/13/2021 17:22:13
`@Simon Steinmann` this was attempted with safe mode in both 2019 version and 2021Ra both had the same outcome . This error message is from the 2019 version . This computer is core i5 2nd generation without a dedicated gpu. I understand that this computer doesnt have the required openGL drivers  but webots is running on intel 3rd generation computers perfectly soo can there be a way to work this out?


`@Simon Steinmann` I think an older version of webots such as webots 8,7 will work but unfortunately those require licensing . This not for me this for a few high school students im teaching webots to

##### Simon Steinmann [Moderator] 02/13/2021 17:28:29
Problem is, that 2nd gen Intel integrated gpus only support opengl 3.1,nit 3.2


Perhaps look for a dirt cheap low end dedicated gpu?


<@&568329906048598039> this is not my expertise, but is it possible to run webots somehow with opengl3. 1?  I think docker uses CPU graphics

##### Olivier Michel [Cyberbotics] 02/13/2021 17:31:57
No, it's a no go unfortunately.


Webots in CI uses software rendering (super slow). It might be possible to replace your Windows OpenGL libraries with some software only rendering, i.e., Mesa, but I have no experience with this and the performance will be very low.

##### vinwan 02/13/2021 17:33:14
I thought of this but these are laptops and im not sure if this can be done

##### Olivier Michel [Cyberbotics] 02/13/2021 17:33:37
Unfortunately, with a laptop you're stuck and cannot upgrade the GPU...

##### Simon Steinmann [Moderator] 02/13/2021 17:35:15
Is it just one machine or multiple?


10 year old is ancient when it comes to computers

##### vinwan 02/13/2021 17:35:44
4 machines so far

##### Olivier Michel [Cyberbotics] 02/13/2021 17:36:00
Can you tell me what is the model of your Intel GPU?

##### Simon Steinmann [Moderator] 02/13/2021 17:36:12
Sandybridge


Zou need Intel HD 4000 or newer


So you need Intel 3rd gen or newer

##### vinwan 02/13/2021 17:37:44
I know but this is the best they've got

##### Simon Steinmann [Moderator] 02/13/2021 17:38:14
And they are all laptops?


Are you a high school teacher?

##### Olivier Michel [Cyberbotics] 02/13/2021 17:39:08
You might be able to work out something with this: [https://www.reddit.com/r/archlinux/comments/8stpmt/how\_can\_i\_get\_opengl\_33\_with\_glsl\_33\_support/](https://www.reddit.com/r/archlinux/comments/8stpmt/how_can_i_get_opengl_33_with_glsl_33_support/)


(you will have to install Linux)

##### Simon Steinmann [Moderator] 02/13/2021 17:39:54
Perhaps you can ask around for people who could donate an old laptop

##### vinwan 02/13/2021 17:39:55
I think most of them are but if some have desktops ill tell them to try out a old gpu


No im a college student from Sri Lanka teaching a few students at the high school i attended so that they may learn some robotics during this pandemic


Ill try thank you


Ill check it out thank you

##### Simon Steinmann [Moderator] 02/13/2021 17:43:01
Awesome! Thx for doing that. Perhaps you can ask the school faculty or even just the other students. I bet there are people with older machines. We are talking 2012 or newer

##### vinwan 02/13/2021 17:45:33
The school can't help us because schools are closed so we are on our own. Thank you for this insight into webots ill keep these requirements in mind  for next time

##### Olivier Michel [Cyberbotics] 02/13/2021 17:51:23
The only two solutions I see are:

- Try this: [https://dhern023.wordpress.com/2020/05/15/update-intel-sandy-bridge-opengl-compatibility-to-3-3/](https://dhern023.wordpress.com/2020/05/15/update-intel-sandy-bridge-opengl-compatibility-to-3-3/)

- Install Linux on the laptop and use Webots from Linux.


But in any case, you will get a slow performance (you should probably disable all advanced OpenGL features from the Webots preferences).

##### vinwan 02/13/2021 18:18:09
Thank you will look into this

##### Vangiel 02/14/2021 10:11:39
Hello everyone, I am quite new in Webots and I am having a problem I cannot solve. I load a world with just the basic nodes and a supervisor robot. From the controller of that robot I load elements to the scenario using the python api (pedestrians, floor, robot, objects...). My problem is that when I reset the simulation all the nodes I added disappear but some of them remain (like the floor and one of the pedestrians). How can I do a clean reset? I have tried to use supervisor.worldReload but the world keep reloading all the time. I hope you can help me, thanks.

##### Bitbots\_Jasper [Moderator] 02/14/2021 10:13:10
Make sure that you save the world after you have added all the nodes


There also is a difference between resetting the world and reloading it.

##### Vangiel 02/14/2021 10:17:10
The problem with saving the world after I add the nodes is that it will save all of it in the wbt file right? and I want to load random scenarios each time

##### Simon Steinmann [Moderator] 02/14/2021 16:14:29
`@Vangiel` what exactly do you want to happen? Remove everything? World reload seems like the easiest clean option. I'm not sure what exactly doesn't work for you there. Otherwise, you could simply remove all the nodes using the supervisor api


Then do a simulation reset


Feel free to pm me, I can take a look. Kinda curious about your project. Worked on domain randomization before, but not randomized environments

##### Qwerty23 02/14/2021 20:07:37
`@Simon Steinmann` Was wondering on how to move the Robotis OP2's arms in terms of Pitch, Yaw, and Roll, to let the robots arm close in, python


controlling it individually

##### Darko Lukić [Cyberbotics] 02/15/2021 08:04:47
The reset feature is documented here:

[https://www.cyberbotics.com/doc/guide/the-user-interface#file-menu](https://www.cyberbotics.com/doc/guide/the-user-interface#file-menu)

(scroll to `Note: In order to reset...`)



However, according to your description it seems that the reset doesn't work properly. Could you please open an issue with a minimal example:

[https://github.com/cyberbotics/webots/issues/new?assignees=&labels=&template=bug\_report.md](https://github.com/cyberbotics/webots/issues/new?assignees=&labels=&template=bug_report.md)

##### Siliconlad 02/15/2021 08:05:43
Good morning! I'm currently trying to create an external C++  ROS noetic controllers for our robot. So in my `.cpp` file I have include statements of the form `#include "webots/Robot.hpp"` alongside ROS include statements like `#include "ros/ros.h"`. Is there a way to link the webots cpp api using CMake so that I can run `catkin build` in my workspace and then just run the controllers (using `rosrun` or via a launch file) and have them connect to the simulator?

##### Darko Lukić [Cyberbotics] 02/15/2021 10:03:56
Here is a CMake example for ROS 2, but for ROS 1 should be very similar:

[https://github.com/cyberbotics/webots\_ros2/blob/5af9fe3b25e4f5be9b584b4e64f62078b063eaad/webots\_ros2\_cpp/CMakeLists.txt#L17-L26](https://github.com/cyberbotics/webots_ros2/blob/5af9fe3b25e4f5be9b584b4e64f62078b063eaad/webots_ros2_cpp/CMakeLists.txt#L17-L26)



Note: Make sure the `WEBOTS_HOME` environment variable is configured.

##### Tahir [Moderator] 02/15/2021 12:35:26
I am trying to teleop a diff drive robot with in Webots but the nodes does not communicate. The controller runs fine but the robot does not move when teleoperated. 

I am not able to get where is the problem. Anyone have any idea?


Here is my ROS specific controller which is running in Webots
> **Attachment**: [teleop\_robot.py](https://cdn.discordapp.com/attachments/565154703139405824/810852035628367892/teleop_robot.py)


Update:

rosnode info /teleop\_node 

--------------------------------------------------------------------------------

Node [/teleop\_node]

Publications: 

 * /rosout [rosgraph\_msgs/Log]



Subscriptions: 

 * /cmd\_vel [unknown type]



Services: 

 * /teleop\_node/get\_loggers

 * /teleop\_node/set\_logger\_level





contacting node http://rosPC:33375/ ...

ERROR: Communication with node[http://rosPC:33375/] failed!


The above rosnode info gives communication error whereas everything is running on same PC

##### Darko Lukić [Cyberbotics] 02/15/2021 12:58:19
```
 * /cmd_vel [unknown type]
```

This looks bad. It doesn't look directly related to Webots.



Which ROS distribution do you use?

##### Tahir [Moderator] 02/15/2021 13:00:11
No No this is when I am not publishing the cmd\_vel topic


after publishing this topic is recognised


I am having ROS melodic so that's Python2

##### Darko Lukić [Cyberbotics] 02/15/2021 13:02:26
[https://github.com/cyberbotics/webots\_ros/blob/master/scripts/ros\_python.py](https://github.com/cyberbotics/webots_ros/blob/master/scripts/ros_python.py)

Does this example work for you?

##### Tahir [Moderator] 02/15/2021 13:05:38
Let me just try


Nope its not working from me


Can you tell me the above mentioned controller is for which robot file I will try to debug


where is the problem


I have found the word


world*

##### Darko Lukić [Cyberbotics] 02/15/2021 13:26:29
[https://github.com/cyberbotics/webots\_ros/blob/master/launch/webots\_ros\_python.launch](https://github.com/cyberbotics/webots_ros/blob/master/launch/webots_ros_python.launch)

You should be able to launch this file

##### Tahir [Moderator] 02/15/2021 13:27:30
Yes this example is working

##### Darko Lukić [Cyberbotics] 02/15/2021 13:33:19
Your class `TeleopRobot` looks good, but maybe I am missing something. I would suggest to you to try rebuilding your controller from the example I sent to you

##### Tahir [Moderator] 02/15/2021 13:59:27
I've got things working but don't know whats happening and what was wrong. 

The script which I've uploaded `teleop_robot.py` worked when I just added(`rospy.loginfo("")` before `self.sendWheelVelocitties(self.left_wheel_velocity, self.right_wheel_velocity)`)

In this part the real working is happening where I am sending the commands to the robot to move. Before adding the above `rospy.loginfo` there was no movement but after adding its working fine both in form of a ros package as `@Darko Lukić`  suggested with above mentioned example package and the standalone controller in `Robot controller` field.


I know `rospy.loginfo` is triggering something but what that I am not able to understand

##### Darko Lukić [Cyberbotics] 02/15/2021 14:37:00
Strange... Thank you for sharing details with us!

##### alejanpa17 02/15/2021 15:14:20
Hey guys, quick question, is there any way I can add a parameter of downforce to a car?

##### Darko Lukić [Cyberbotics] 02/15/2021 16:32:17
No, but you can simulate it. You can use `wb_supervisor_node_get_velocity` ([https://www.cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_get\_velocity](https://www.cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_get_velocity)) to get car's velocity and `wb_supervisor_node_add_force` ([https://www.cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_add\_force](https://www.cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_add_force)) to apply the downforce as a function of the velocity.

##### alejanpa17 02/15/2021 17:02:24
Thanks :))

##### Callum98 02/15/2021 17:12:34
Hey guys, can anyone help me program a camera using Java? I'm unsure what commands I need for the image to be shown: its currently just a black screen

##### Simon Steinmann [Moderator] 02/15/2021 17:13:39
[https://www.cyberbotics.com/doc/reference/camera](https://www.cyberbotics.com/doc/reference/camera)

##### Bitbots\_Jasper [Moderator] 02/15/2021 17:14:03
make sure to enable your camera using  camera.enable(sampling\_peroid)

##### Simon Steinmann [Moderator] 02/15/2021 17:14:06
you have to enable most sensors before you can use them


in the api documentation (link I posted before) you can see all functions and commands, in all the different programming languages

##### Callum98 02/15/2021 17:29:23
I think I've enabled it, does the line camera.enable(timeStep); work? Hasnt done anything


Ive had a look at the api documentation but Im new to programming so finding it a bit confusing

##### Siliconlad 02/15/2021 18:04:08
This worked a treat! Thank you very much 🙂

##### Bitbots\_Jasper [Moderator] 02/15/2021 18:09:31
i dont really know much about java programming but there is a tutorial for using java an webots here: [https://cyberbotics.com/doc/guide/using-java](https://cyberbotics.com/doc/guide/using-java)


you can also go through the webots tutorials here: [https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials)

##### Callum98 02/15/2021 18:18:25
thank you, Ill have a look trhough

##### R\_ 02/15/2021 18:26:49
I have an Nvidia GPU, but I'm getting this error. Any suggestions to mitigate this error?😊

I'm on Ubuntu 20.04 and installed it with `sudo snap install webots`

My Nvidia Config is setup to 'NVIDIA On-Demand'
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/810940196996513853/unknown.png)
%end

##### Simon Steinmann [Moderator] 02/15/2021 18:39:37
on linux, integrated gpu + dedicated can be tricky sometimes


try disabling your integrated GPU in BIOS


or explicitly launching webots with Nvidia gpu

##### R\_ 02/15/2021 18:42:09
how do I do this?

##### Simon Steinmann [Moderator] 02/15/2021 18:42:40
what operating system?

##### R\_ 02/15/2021 18:43:24
Ubuntu 20.04

##### Simon Steinmann [Moderator] 02/15/2021 18:43:53
disable your iGPU or google it

##### R\_ 02/15/2021 18:49:18
But it is showing this
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/810945854043193394/unknown.png)
%end

##### Simon Steinmann [Moderator] 02/15/2021 18:51:07
Did you install the latest nvidia drivers?

##### R\_ 02/15/2021 18:58:40
yes

##### Simon Steinmann [Moderator] 02/15/2021 19:40:38
I just disabled the iGPU in bios, and everything works fine

##### modymu 02/16/2021 06:38:38
Use Editor instead!!

##### Steven37 02/16/2021 10:02:51
I'm trying to keep increasing the size of a box, as well as changing its color. In the image attached, it is my code to do it. I also have set the supervisor field to True. However, when I ran the simulation, the box didn't have any changes. Can someone help point out where I went wrong, please? Thank you!
%figure
![3Y_Iproject_supervisor_testCode.JPG](https://cdn.discordapp.com/attachments/565154703139405824/811175758138572860/3Y_Iproject_supervisor_testCode.JPG)
%end

##### Olivier Michel [Cyberbotics] 02/16/2021 10:21:12
1. You check that every `WbNodeRef` and `WbFieldRef` you retrieve is not NULL.

2. It's a bad idea to use `Box` for a DEF name as it is a reserved VRML keyword for the `Box` node.

3. Instead of changing  the `scale` field of a `Solid` node, I would suggest you to change the `size` field of the `Box` node.

4. The `baseColor` is not a field of a `Solid` node: you should get it from a `PRBAppearance` node instead.

##### Steven37 02/16/2021 10:47:54
`@Olivier Michel` I have followed your suggestions but it still doesn't work yet. Is there anything more I can do about this? Thank you!

##### nelsondmmg 02/16/2021 11:21:00
Hi, is there a way to continue a simulation given that one robot is already finished while maintaining the synchronization on?

##### oroulet [Premier Service] 02/16/2021 12:34:08
How can I run webots in the background to be used for functional testing? I could not find any command line arguments. Maybe some environment variable?

##### Olivier Michel [Cyberbotics] 02/16/2021 12:34:42
You should use the `--batch` command line option.

##### oroulet [Premier Service] 02/16/2021 12:35:16
but it still starts a window. Can we run it without window at all? or do I need to use framebuffer and co

##### Olivier Michel [Cyberbotics] 02/16/2021 12:36:53
You may add the `--minimize` command line option to have the window minimized at startup. It is not possible to avoid the creation of the window because Webots need to be connected to the OpenGL context via a window.


On Linux you can use a virtual framebuffer to avoid completely showing-up any window.

##### oroulet [Premier Service] 02/16/2021 12:37:54
OK. My goal was to use webots on gitlab/github CI. So then I probably need to set up some fake display


ja exactly. I will try. thanks

##### Olivier Michel [Cyberbotics] 02/16/2021 12:38:10
Yes.


We do use Webots in github CI and it works nicely.


See [https://github.com/cyberbotics/webots/tree/master/.github/workflows](https://github.com/cyberbotics/webots/tree/master/.github/workflows)

##### oroulet [Premier Service] 02/16/2021 12:39:35
Thanks. I just need to find out where in this files the framebuffer is set 😉

##### Olivier Michel [Cyberbotics] 02/16/2021 12:40:06
[https://github.com/cyberbotics/webots/blob/master/.github/workflows/test\_suite\_linux.yml#L102](https://github.com/cyberbotics/webots/blob/master/.github/workflows/test_suite_linux.yml#L102)


Or [https://github.com/cyberbotics/webots/blob/master/.github/workflows/test\_suite\_linux.yml#L140-L141](https://github.com/cyberbotics/webots/blob/master/.github/workflows/test_suite_linux.yml#L140-L141)

##### oroulet [Premier Service] 02/16/2021 12:41:12
thanks . found the lines now. I think I have done something similar earlier. I will try

##### Olivier Michel [Cyberbotics] 02/16/2021 12:43:19
You can either make that the robot will not actually quit, but execute `wb_robot_step()` in an endless loop, or you could simply replace the robot controller with an empty or void controller from a supervisor program.

##### oroulet [Premier Service] 02/16/2021 12:45:47
ahah. Last comment, made me think I need a way to stop webots if I make it run in the background. We use an external controller and that one will finish, but we need to stop webots too. Can we make webots quit from the external controller using some supervisor command?

you know we are the strange guys using webots without physics 😉

##### Olivier Michel [Cyberbotics] 02/16/2021 12:47:05
I remember 😄 . You need to use this function: [https://www.cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_simulation\_quit](https://www.cyberbotics.com/doc/reference/supervisor#wb_supervisor_simulation_quit)

##### oroulet [Premier Service] 02/16/2021 12:47:45
great . thanks

##### nelsondmmg 02/16/2021 14:17:19
Thanks. Is there a way to remove the fullscreen warning when the movie recording is started by code? This step block the start of execution each time.

##### Olivier Michel [Cyberbotics] 02/16/2021 15:05:36
Did you try the `--batch` mode?

##### Callum98 02/16/2021 15:40:12
Hi guys, do any of the prebuilt robots have working cameras programmed in java that you know of?

##### Olivier Michel [Cyberbotics] 02/16/2021 15:41:29
Yes, many of them have a camera and all of them can be programmed in Java.

##### Callum98 02/16/2021 15:42:44
But are any of them written in java? Im trying to create a colour recognition robot as part of a coursework but I dont know how to get the camera working so wanna use one as an example

##### Olivier Michel [Cyberbotics] 02/16/2021 15:44:12
The Java example in projects/languages/java features robots with camera and Java controllers.

##### Callum98 02/16/2021 15:45:59
Sorry where is this?


Im new to this and programming haha

##### Olivier Michel [Cyberbotics] 02/16/2021 15:47:24
File menu / Open Sample World

##### Callum98 02/16/2021 15:48:02
Ah got it, thank you!

##### nelsondmmg 02/16/2021 16:25:04
Ahh thanks I did not saw this option before

##### bellino 02/16/2021 17:29:17
Hello all, this may be a trivial question but I have been stuck since yesterday. I want to use ros2 due to the nav2 package but with webots as my simulator. I am struggling to figure out how to exactly use this package because I suppose I need to use different topics like cmd\_vel to publish speed etc. to. Does this package only tell me by how much to rotate my robot/ where to go or can it handle the actual joint movement?

##### Darko Lukić [Cyberbotics] 02/16/2021 19:03:02
Have you checked `webots_ros2_turtlebot`:

[https://github.com/cyberbotics/webots\_ros2/tree/master/webots\_ros2\_turtlebot](https://github.com/cyberbotics/webots_ros2/tree/master/webots_ros2_turtlebot)



It implements support for navigation:

[https://github.com/cyberbotics/webots\_ros2/wiki/Navigate-TurtleBot3](https://github.com/cyberbotics/webots_ros2/wiki/Navigate-TurtleBot3)

##### bellino 02/16/2021 21:00:43
thank you! I also have another question. My robot has two wheels with a motor and one wheel for stabilization, I am turning by rotating the wheels in the opposite direction and using position sensors to track by how far each wheel has rotated, but the rotation is not exact. For instance, when I want to rotate by pi/2, I rotate by 1.58 instead. Is there a better way to deal with this?

##### Darko Lukić [Cyberbotics] 02/16/2021 21:33:37
You probably need to calibrate the axle length and wheel radius. Try with different values until you get a more accurate rotation.



However, if you robot is aware it got rotated by 1.58 then you have problem with the control, you need calibrate PID coefficients.



In general, error of 0.01 radians is not bad.

##### bellino 02/16/2021 21:41:58
right, I suppose it's very difficult to get 100% accurate rotations

##### Daniel Honerkamp 02/16/2021 21:50:44
Hi, I'm following the examples to use moveit in [https://github.com/cyberbotics/webots/tree/released/projects/robots/universal\_robots/resources/ros\_package/ur\_e\_webots](https://github.com/cyberbotics/webots/tree/released/projects/robots/universal_robots/resources/ros_package/ur_e_webots). 

This includes the following:

```
from controller import Robot
robot = Robot()
```

using python 2.7. But when instantiating the Robot() the script simply terminates without any error messages. Any help appreciated. My pythonpath and ld\_library\_path should be set (the import succeeds) and include `/usr/local/webots/lib/controller/python27` and `${WEBOTS_HOME}/lib/controller` respectively

`

##### bellino 02/16/2021 23:36:21
Okay I followed the driver tutorial here [https://github.com/cyberbotics/webots\_ros2/wiki/Tutorial-Write-ROS2-Driver](https://github.com/cyberbotics/webots_ros2/wiki/Tutorial-Write-ROS2-Driver) and I think it works so far but I am not sure where I would put the code that actually moves the joints. I can't seem to find where that's in the turtlebot repo. Where would the controller be specified and how does it interact with the driver. From my understanding, the driver is the interface between ros2 and webots publishing webots data for ros2, correct?


Okay a little edit, I am now using WebotsDifferentialDriveNode and it says an external controller has been started. How would I go about actually moving the robot now? Do I have to publish something to some topic?

##### Darko Lukić [Cyberbotics] 02/17/2021 07:42:39
Hello Daniel, that sounds as a good approach. Can you run a Python2 controller from Webots UI? Make sure `Tools > Preferences > Python command` is set to `python2`


Yes, see this:

[https://github.com/cyberbotics/webots\_ros2/wiki/Tutorial-E-puck-for-ROS2-Beginners#velocity-control](https://github.com/cyberbotics/webots_ros2/wiki/Tutorial-E-puck-for-ROS2-Beginners#velocity-control)



Also, you can use `teleop_twist_keyboard`

##### Daniel Honerkamp 02/17/2021 08:04:13
This does seem to work in the webots UI itself


Looking at the environment variables set in the UI, there are numerous webots variables. Defining WEBOTS\_TMP\_PATH and WEBOTS\_SERVER I now at least get an error message from running `Robot()`. But copying the values from the UI won't work as they refer to tmp paths. Is there a guide somewhere which of these and how I have to set them?

##### Darko Lukić [Cyberbotics] 02/17/2021 09:16:18
What you did is enough to run an external controller:

```
export PYTHONPATH=${WEBOTS_HOME}/lib/controller/python27
export LD_LIBRARY_PATH=${WEBOTS_HOME}/lib/controller:${LD_LIBRARY_PATH}
```



Furthermore, those environment variables are automatically configured:

[https://github.com/cyberbotics/webots/blob/4027aa8c7ef7c0a7f4dd0692fdf89437cd99546f/projects/robots/universal\_robots/resources/ros\_package/ur\_e\_webots/launch/ur\_controller.launch#L4-L5](https://github.com/cyberbotics/webots/blob/4027aa8c7ef7c0a7f4dd0692fdf89437cd99546f/projects/robots/universal_robots/resources/ros_package/ur_e_webots/launch/ur_controller.launch#L4-L5)



So you don't need configure them manually.



I am trying to reproduce your problem, but it works fine for me with Noetic (I don't have Melodic)

##### Daniel Honerkamp 02/17/2021 09:26:23
Hmm. Outside of the webots UI it's as simple as running robot=Robot()  for me and the python shell immediately terminates. Happens in both python2.7 and python3. I'm using ros melodic. Maybe I'm missing a step that would configure these things when not using the webots UI? I actually don't think it is related to ROS

##### Darko Lukić [Cyberbotics] 02/17/2021 09:37:47
If you run only `robot = Robot()` the controller exists as there is no more things to do. You have have to add something like: `while robot.step(32) != -1: pass` to make it run.



If your controller crashes the problem is most likely in `LD_LIBRARY_PATH`. Try to print it in the controller running from UI and the external controller to see whether it clashes with some other shared library.

##### Daniel Honerkamp 02/17/2021 10:33:31
I have now set it to exactly `PYTHONPATH=/usr/local/webots/lib/controller/python27` and `LD_LIBRARY_PATH=/usr/local/webots/lib/controller`, so nothing else in those paths and still the same thing happens


In the UI I have this path: `LD_LIBRARY_PATH: /home/honerkam/Downloads/my_project/controllers/my_controller:/tmp/webots-26335-Ic2kFC//lib;/opt/ros/melodic/lib;/opt/ros/melodic/lib/x86_64-linux-gnu;/usr/local/cuda-10.2/lib64;/home/honerkam/.mujoco/mujoco200/bin;/usr/local/webots/lib/controller`. So the main difference in there is this tmp path `/tmp/webots-26335-Ic2kFC/`, but I assume I don't have to set this manually


And again, if I do define these two WEBOTS\_TMP\_PATH and WEBOTS\_SERVER I at least get an error about the server path being wrong. But I don't know if that means that I got further or less far in instantiating the Robot()

##### Darko Lukić [Cyberbotics] 02/17/2021 10:40:11
Are you sure the controller is crashing? Maybe it simply finished.

##### Daniel Honerkamp 02/17/2021 10:41:05
I'm just running a python shell at this point:

$python

>>from controller import Robot()

>>robot=Robot()

and then the python shell crashed

##### Darko Lukić [Cyberbotics] 02/17/2021 10:41:26
In your LD\_LIBRARY\_PATH you are using `;` and `:` as separator. I am not sure you can use `;`, it is usually `:`


But since you are overriding `LD_LIBRARY_PATH` it shouldn't make a difference

##### Daniel Honerkamp 02/17/2021 10:42:18
This is the path that it prints out in the webots UI where it works, not the one on my shell

##### Darko Lukić [Cyberbotics] 02/17/2021 11:00:57
It is really strange. Just to confirm a few things. You use Ubuntu 18.04 and you installed Webots R2021a from a Debian package? The `python` command comes from Ubuntu, not from Conda or similar?



1. Try creating a simple external C controller. If it crashes with no additional info then go to 2.

2. Compile Webots from source in debug mode: [https://github.com/cyberbotics/webots/wiki/Linux-installation](https://github.com/cyberbotics/webots/wiki/Linux-installation)

##### Daniel Honerkamp 02/17/2021 11:08:29
Ubuntu18.01, webots R202a installed through apt-get. Python is the system python27 interpreter


I was kind of hoping not to have to do that. I will have to see if I find time for that later this week


And there are really not other steps I'm missing than this to be configured correctly?

1. start a webots world with the robot

2. set paths

3. start a python console

##### Darko Lukić [Cyberbotics] 02/17/2021 11:15:00
The robot in the world has to have the  `controller` field equal to `<extern>`


Otherwise it looks good


If I get some idea why it doesn't work I will let you know

##### Steven37 02/17/2021 11:17:27
I am trying to get familiar with the camera in webots but I am wondering is there any function where I can get the width, height and color of only the recognized object from the camera instead of the image as a whole?

##### Olivier Michel [Cyberbotics] 02/17/2021 11:30:46
You should probably use the recognition module of the camera device, see [https://cyberbotics.com/doc/reference/camera#wb\_camera\_recognition\_enable](https://cyberbotics.com/doc/reference/camera#wb_camera_recognition_enable)

##### Steven37 02/17/2021 11:50:02
`@Olivier Michel` thank you!


I want to ask again to make sure my code is correct for me to know the colors of the object I want in the camera?
%figure
![3Y_Iproject_camera_testingCode.JPG](https://cdn.discordapp.com/attachments/565154703139405824/811570798238236722/3Y_Iproject_camera_testingCode.JPG)
%end


Because I'm not sure if the wb\_camera\_get\_width and wb\_camera\_get\_height functions will take the width and height of the whole image or is it just the recognised object?

##### Stefania Pedrazzi [Cyberbotics] 02/17/2021 12:15:34
The code is wrong because the `wb_camera_recognition_get_object` function doesn't return an image but a list of objects (`WbCameraRecognitionObjects`):

[https://www.cyberbotics.com/doc/reference/camera#camera-recognition-object](https://www.cyberbotics.com/doc/reference/camera#camera-recognition-object)


You can find an example in the `projects/samples/devices/worlds/camera_recognition.wbt` simulation.

Here is the source of the sample robot controller:

[https://github.com/cyberbotics/webots/blob/master/projects/samples/devices/controllers/camera\_recognition/camera\_recognition.c](https://github.com/cyberbotics/webots/blob/master/projects/samples/devices/controllers/camera_recognition/camera_recognition.c)

##### Saud 02/17/2021 13:41:27
where can i find a list of devices that i can use on the DJI MAVIC?

##### Stefania Pedrazzi [Cyberbotics] 02/17/2021 13:44:28
You can find the complete list of devices in the documentation page of the robot: on the menu on the right of the 3D viewer of the robot, all the devices names are listed grouped by type

[https://www.cyberbotics.com/doc/guide/mavic-2-pro](https://www.cyberbotics.com/doc/guide/mavic-2-pro)

##### Saud 02/17/2021 13:57:54
oh great thank you

##### Daniel Honerkamp 02/17/2021 14:00:43
Seems this is where I was going wrong. It wasn't clear to methat instantiating a Robot() object equates to having an external controller and instead used the `ros` controller in webots. With <external> I get it to work. (Might be helpful to add some check to the Robot() initialiser in case that's possible). Thanks for the help

##### alejanpa17 02/17/2021 20:00:44
Hi again guys! Is there any way I can make the tires of the car robot slippery?? Or is that too much complicated?

##### Chernayaten 02/17/2021 23:38:38
`@alejanpa17` I believe you have to establish ContactProperties between your tires and the floor and set the coulombFriction = 0 [https://www.cyberbotics.com/doc/reference/contactproperties](https://www.cyberbotics.com/doc/reference/contactproperties)

##### alejanpa17 02/18/2021 00:56:51
Thanks ^^

##### Bitbots\_Jasper [Moderator] 02/18/2021 11:51:13
I am trying to start multiple extern controllers as described here: [https://cyberbotics.com/doc/guide/running-extern-robot-controllers?tab-language=python#single-simulation-and-multiple-extern-robot-controllers](https://cyberbotics.com/doc/guide/running-extern-robot-controllers?tab-language=python#single-simulation-and-multiple-extern-robot-controllers)



I have multiple robots with the same proto but a different DEF. Is it possible to start an external robot controller and connect it to a Robot Node with a certain DEF? Otherwise, should I modify my proto files such that they have different names?

##### Darko Lukić [Cyberbotics] 02/18/2021 11:53:53
Hello `@Bitbots_Jasper`, usually the `name` field should be exported. For example:

[https://github.com/cyberbotics/webots/blob/794d40381d6179faaadf97cb8c890116cea5ce90/projects/robots/gctronic/e-puck/protos/E-puck.proto#L13](https://github.com/cyberbotics/webots/blob/794d40381d6179faaadf97cb8c890116cea5ce90/projects/robots/gctronic/e-puck/protos/E-puck.proto#L13)

[https://github.com/cyberbotics/webots/blob/794d40381d6179faaadf97cb8c890116cea5ce90/projects/robots/gctronic/e-puck/protos/E-puck.proto#L1137](https://github.com/cyberbotics/webots/blob/794d40381d6179faaadf97cb8c890116cea5ce90/projects/robots/gctronic/e-puck/protos/E-puck.proto#L1137)

##### Bitbots\_Jasper [Moderator] 02/18/2021 11:54:23
ohh, thank you so much I oversaw that

##### danielvicente 02/18/2021 16:03:14
Hi everyone, 

is there a way that, if as a supervisor I have the node of a camera on a different robot, to get the image data of that camera?

##### Darko Lukić [Cyberbotics] 02/18/2021 18:08:54
It is not possible. You have to implement some kind of communication between those two controllers to transfer the images.

##### yanan 02/18/2021 18:10:18
Hi everyone, it seems like it is impossible to run both a robot like dji and a vehicle at the same time within an environment?  is that right?

##### Darko Lukić [Cyberbotics] 02/18/2021 18:10:50
It is possible, what error do you get?

##### yanan 02/18/2021 18:12:32
thanks for your reply. actually, I am trying to use both dji and a tesla within an environment.  it seems like I have to use ' wb\_robot\_init();

     wbu\_driver\_init();'   and it reports  'Error: Only nodes based on the 'Car' node can used the car library.'

##### Darko Lukić [Cyberbotics] 02/18/2021 18:17:05
There should be two controllers. One controller with `wb_robot_init` for the drone, and another one with `wbu_driver_init` for the car.

##### yanan 02/18/2021 18:24:25
thanks. something like this ?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/812026757955059743/unknown.png)
%end

##### DDaniel [Cyberbotics] 02/18/2021 18:24:56
two separate files


one for the "controller" field of the dji, and another for the controller field of the tesla

##### yanan 02/18/2021 18:27:48
danke schoen. I will give it a try

##### danielvicente 02/18/2021 18:51:07
okay, thanks

##### mayank.kishore 02/18/2021 20:58:46
Would anyone be willing to chat with me on how to create a custom launch file? Am essentially trying to us `@Darko Lukić`'s navigation package and use it on a different environment with multiple robots: 



[https://github.com/cyberbotics/webots\_ros2/wiki/Navigate-TurtleBot3](https://github.com/cyberbotics/webots_ros2/wiki/Navigate-TurtleBot3)



The environment:

[https://github.com/virtualclone/formation\_uavs](https://github.com/virtualclone/formation_uavs)

##### h.sciascia 02/19/2021 08:09:32
Hello everyone ! 



I would like to import a 3D model and define it as collision zones for my robot. However, I would like to define this model as one shape/solid/form. 



Currently, when I import a 3D model, I get a multitude of Nodes (Transform, Solid, Shape, Group, etc) distinct from each other. So, I don't want to retrieve the ID of the respective boundingObjects of each of these shapes to compare them with my robot's boundingObject, that would be way too long...



So is there a way to merge each of these shapes of the 3D model (mesh?) In order to get only one object, with a single ID?



Thank you in advance !

##### Olivier Michel [Cyberbotics] 02/19/2021 08:12:10
Yes can either merge these object using a 3D modeling software, like Blender, or put them all together inside a Group node.

##### h.sciascia 02/19/2021 08:14:04
Ok I will try that, because I want only one boundingObject


Hello again ! 

Is that possible to launch Webots in command line (no graphical interface) ?

##### Olivier Michel [Cyberbotics] 02/19/2021 13:22:06
`webots --batch --minimize` should do it.

##### h.sciascia 02/19/2021 13:42:32
Thanks i will try that !

##### Callum98 02/19/2021 13:59:31
I anyone able to help me with programming in java?

##### bizzard 02/19/2021 15:30:50
I want to implement a multi robot controller, for example to realize some algorithm like: conflict based search(CBS), how can I do this?Are there any example controller?

##### bellino 02/19/2021 16:17:01
Hello, what unit is used for shape sizes in webots? I thought m at first but it doesn't seem quite right

##### mayank.kishore 02/19/2021 16:20:26
Do I need to have a share directory to create a launch file?

##### Olivier Michel [Cyberbotics] 02/19/2021 16:20:28
Yes, it's meters.

##### bellino 02/19/2021 18:19:32
Hello I have two questions, first of all, is it somehow possible to get a body's rotation in the world the way it's shown in the scene tree? (I.e. without using sensors) Also, I am trying to implement rotation for my robot using wheel encoders but I found that it is not very accurate (i.e. when turning by pi the closest values I have gotten are 1.56999 and 1.581). I tried playing around with the axle length and wheel radius but this is as close as I have gotten. The robot has two wheels and rotates by spinning the wheels in opposite directions.

##### Bitbots\_Jasper [Moderator] 02/19/2021 19:32:54
There is some documentation on how to do that here: [https://cyberbotics.com/doc/guide/running-extern-robot-controllers#single-simulation-and-multiple-extern-robot-controllers](https://cyberbotics.com/doc/guide/running-extern-robot-controllers#single-simulation-and-multiple-extern-robot-controllers)


it is not quite clear to me what your question is. What launch files are you talking about? ROS launch files?


to get the position and orientation of nodes in world coordinates you need to use a supervisor controller (see here: [https://cyberbotics.com/doc/reference/supervisor](https://cyberbotics.com/doc/reference/supervisor))  you can make the controller that is turning your robot into a supervisor controller by replacing the creation of your Robot by a creation of a Supervisor object (Supervisor inherits from robot so all functions should still work)

then you can call `getSelf()` (or the equivalent in the programming language of your choice) to get the robot node. On the robot node you can get the translation and rotation field using `getField("translation")` or `getField("rotation")`.  on the translation field you want to call `getSFVec3f()` to get an array of coordinates, on the rotation field you want to call `getSFRotation()` to get an array representing a rotation in the axis angle notation

##### bellino 02/19/2021 20:28:24
what about the rotation, it is even possible to have an 100% accurate rotation?

##### JMarple 02/19/2021 20:33:03
Hello all, 



I have a PROTO file with a `robot` that contains a set of 8 identical `robot` from another PROTO file.  I have a controller on the highest level robot and want to access the motors on each of the 8 identical robots.  Normally this would be done using `getDevice`, however at the highest level robot, `robot.getNumberOfDevices()` returns 0.  



Is there a proper way to go about this?  I also tried to adding a device parameter to my PROTO and explicitly defining a device list at the highest level, but I still can't seem to get access to this. 



For reference, this is what the topology roughly looks like: 

```
- Robot
  - controller
  - children
    - Robot slot0
       - HingeJoint
    - Robot slot1
       - HingeJoint
    - Robot slot2
       - HingeJoint
    - etc.etc
```

 

Thanks

##### Chernayaten 02/19/2021 20:55:43
`@bellino`  I recently programmed a two-wheeled robot that rotates on its axis. There's a formula for the calculation you want which takes the base of the rotation as the distance between the center of the wheels. What I found out, and a member of this server helped me confirm, is that the distance of the outside part of the wheel is more accurate. 



However it was still not enough. Say I do a 90 degree turn a bunch of times, I will eventually be (visibly) off course. This problem becomes even worse when trying to do different angles (presumably, a 45 degree turn is half the 90 degree, but this was far from true for me). 



I tried playing around by slowly increasing/decreasing the rotation amount and it works to an extend, if you want to do a couple angles only it may be worth trying.



Your best solution would be to use the supervisor to change the angle (or for a more realistic turn, let the robot rotate and when it is "done", set it to the correct angle, essentially fixing the small mistakes that happen)

##### Bitbots\_Jasper [Moderator] 02/19/2021 21:01:04
what you are essentially doing is called dead reckoning ([https://en.wikipedia.org/wiki/Dead\_reckoning](https://en.wikipedia.org/wiki/Dead_reckoning)) and it is inaccurate in simulation as well as in the real world. In simulation it can be caused by a variety of factors such as numerical inaccuracies or explicitly injected noise in the simulated sensor data and many others. In the real world it is cause by wheel slippage, which almost always occurs. 

I am not sure how much wheel slippage occurs in simulation as well but I imagine it does. Maybe someone can correct me here or give some further information?


what is normally done in robotics is to have some kind of localization system such as SLAM or AMCL. The localization is normally based on the odometry (that is the pose the robot based on the turning of the wheels) and some sensor such as a Lidar or a camera


the solution proposed by `@Chernayaten` can work, but depending on the context might be considered "cheating"


The robot node is derived from Solid and not from Device, therefore it is not returned by getDevice


where did you get the idea to put multiple robot nodes in a single proto file, i have not seen that before

##### JMarple 02/19/2021 21:13:23
In real life, I have 8 motors on 8 different devices which are controlled by one controller.  My hope is instead of having to copy/paste 8 proto files into one massive file, I can re-use them.  Additionally, it'll more closely mimic how the system is architected IRL.

##### bellino 02/19/2021 21:14:54
I am building the robot for one of my university courses, so I don't think I should be using the supervisor for this. I think I will have to go with SLAM in this case (was trying to avoid it in order to keep it as simple as possible). Thank you very much for the information, I will probably come back once I have read more about the topic!

##### JMarple 02/19/2021 21:21:51
Oop I had a bit of a breakthrough, I changed the topology to:

```
- Robot
  - controller
  - children
    - Solid slot0
       - HingeJoint
    - Solid slot1
       - HingeJoint
    - Solid slot2
       - HingeJoint
    - etc.etc
```

And I'm getting some devices back.  Don't know which is which yet, but I'm sure I can figure that out. Thanks!

##### cooolwhip14 02/19/2021 21:24:59
Hi guys im making a rover and have tried to implement wheel turning through hinge joints perpendicular to my wheel planes. When suspending the rover in air the wheels turn and respond to the position change in the coding but when moving the rover along the floor the wheels dont turn, implying theres a confliction between the wheels and the floor. Is there any settings i can alter to fix this? Thanks


I feel like it has something to do with the wheels sinking into the floor but not sure

##### Bitbots\_Jasper [Moderator] 02/19/2021 21:27:43
another option you have is to have 8 separate robots with a controller each and combine these in some master controller


you can try comparing your proto to the one of a different wheeled robot such as the pioneer [https://github.com/cyberbotics/webots/blob/master/projects/robots/adept/pioneer2/protos/Pioneer2.proto](https://github.com/cyberbotics/webots/blob/master/projects/robots/adept/pioneer2/protos/Pioneer2.proto)

##### Chernayaten 02/19/2021 21:44:33
Yeah, I quickly came to the conclusion that accuracy is a fickle thing for what I wanted to do. 



The default noise value is 0, which ofc can be changed. You can set ContactProperties which have fields such as coulombFriction and forceDependentSlip which should introduce some slippage, but I never tried them out cause it was not necessary for my needs.

##### cooolwhip14 02/19/2021 21:52:54
Hi thanks for the suggestion. The main difference i can see is the pioneer just uses the one set of hinge joints for wheels to turn, and then uses wheel PositionSensors in the joint device section. I dont have this setup as I have a hinge joint allowing turning movement for the wheels , and then a wheel hingejoint connected to this for wheel spinning. (Similar to sojourner proto in nasa files)


Yet i cannot see what the sojourner proto does much different to my model, The main difference i do observe is that my wheels sink into the surface

##### Chernayaten 02/19/2021 21:53:52
For PROTO nodes you should be able to use **wb\_supervisor\_node\_get\_from\_proto\_def** by giving DEF names to your nodes.


I would second jgueldenstein's suggestion to use 8 separate robots though


proto files are made to be re-used so why would you have to copy-paste them?

##### cooolwhip14 02/19/2021 22:14:06
Has anyone had any experience with wheels sinking into the surface and better yet how to fix it?

##### DDaniel [Cyberbotics] 02/19/2021 22:19:18
you can reduce the basicTimeStep in the worldinfo node and/or reduce the softCFM in the contact properties

##### cooolwhip14 02/19/2021 22:20:14
Ive set the timestep to 4 but no avail , softCFM?


changed CFM from 1e-5 -> 1e-10 didnt seem to work

##### DDaniel [Cyberbotics] 02/19/2021 22:22:22
worldinfo > contactproperties > softCFM

##### cooolwhip14 02/19/2021 22:25:08
No luck but im not even sure i have contact properties set up correctly

##### DDaniel [Cyberbotics] 02/19/2021 22:31:22
also doublecheck you didn't add very heavy parts by mistake (by setting a wrong density/mass or things like that)

##### cooolwhip14 02/19/2021 22:33:41
I havent actually defined any of the masses yet just all parts set to the default density of 1000


Is that a problem?

##### Bitbots\_Jasper [Moderator] 02/19/2021 22:34:33
if you have a bounding object and a density, it automatically calculates the mass

##### DDaniel [Cyberbotics] 02/19/2021 22:37:08
That's normal, isn't that then unless it's a huge robot

##### cooolwhip14 02/19/2021 22:44:13
Um id say its about 2.5m length 2m width 1m height


1m width 2m height*

##### Bitbots\_Jasper [Moderator] 02/19/2021 22:46:51
well, that makes your robot 2.5 * 1* 2 * 1000kg = 5000 kg heavy


just to test, try to set the density to 1

##### cooolwhip14 02/19/2021 22:48:37
When im next on my pc il give it a try and let you know. Cheers!


And is that to set the density of each bounding object i have on my rover to 1?


Nvm yes it would be

##### Bitbots\_Jasper [Moderator] 02/19/2021 22:50:22
you have basically two options, either you set the density to something that is realistic or you set the mass to something that is realistic. I think its generally easier to estimate the mass of each component than the density


if you set the mass you should set the density to -1, see here: [https://cyberbotics.com/doc/reference/physics#field-summary](https://cyberbotics.com/doc/reference/physics#field-summary)

##### cooolwhip14 02/20/2021 06:23:40
hi , I tried reducing the mass, i even reduced the scale in combination with the mass yet no avail, however when i did the wheels etc extremely light they would all go out of control and spin in random directions at the start of the sim. Anyone got any ideas?

##### Vangiel 02/20/2021 10:21:22
Hello, how can I rotate a solid around one axis without affecting the other axis (using python)? Thanks in advance.

##### DDaniel [Cyberbotics] 02/20/2021 12:14:55
put the solid in the endPoint of a hingeJoint and a rotationalmotor in the device field, then from the controller you access the device and control the motor


you can use the tutorial as reference: [https://www.cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot](https://www.cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot)

##### Bitbots\_Jasper [Moderator] 02/20/2021 14:45:34
can you share your proto file? it might be a lot easier to help you then.

##### Vangiel 02/20/2021 18:56:42
I will try that, thank you very much

##### Icy\_Flurry 02/20/2021 20:21:58
how can i set indentation to tabs in the webots text editor?

##### modymu 02/20/2021 20:29:51
+ Is there any add ons for webots text editor??

##### Icy\_Flurry 02/20/2021 20:36:41
if this isnt possible, how can i use webots with clion? the cmake instructions did not work

##### modymu 02/20/2021 20:37:58
can you compile the code directly from webots?

##### Icy\_Flurry 02/20/2021 20:38:11
yes

##### modymu 02/20/2021 20:40:24
I don't know about Clion but try to change where the executable file goes directory

##### Icy\_Flurry 02/20/2021 20:42:54
it compiles, but shows this message:

```
/media/<username>/Acer/Users/<username>/CLionProjects/webots-proj/cmake-build-debug/webots-proj: error while loading shared libraries: libController.so: cannot open shared object file: No such file or directory
```

##### bertlangton 02/21/2021 09:30:19
I keep on getting a "python.exe was not found" error when running WeBots, I've changed the Path variable and set the preference, but it doesn't help; what am I doing wrong？


I have windows 10

##### Olivier Michel [Cyberbotics] 02/21/2021 09:51:26
Which python version did you install?

##### bertlangton 02/21/2021 09:51:54
3.9.1


Ah, managed to clear it up... but now it doesnt seem to find numpy, though I installed it with anaconda

##### Olivier Michel [Cyberbotics] 02/21/2021 09:58:25
Anaconda is not supported out of the box, you should use python from python.org and pip to install numpy. Alternatively, you can recompile the python bindings to work with anaconda python. See details on this in the user guide.

##### bertlangton 02/21/2021 10:02:41
Ah, now it works; thanks

##### modymu 02/21/2021 22:11:38
It seems like Clion didn't know the libraries used by webots to compile your controller

##### Harris 02/21/2021 22:12:52
Hi, I am trying to put a imported gripper solid node in the toolslot of universal robot node. Here's the problem: when I drive the universal robot's motor to make some movement, the gripper cannot move well and the universal robot was stuck together if I turn on the boundingObject of each solid children node. Because I have to do collision detection later on so I am wondering is there anyway to solve this problem without turning off the boundingObject?

##### modymu 02/21/2021 22:13:27
I had this problem also in pylint in vs code it dose not recognized the libraries import at the beginning of the code

##### Icy\_Flurry 02/21/2021 23:19:38
yep, i fixed it by setting LD\_LIBRARY\_PATH in the environment variables section of the config

##### h.sciascia 02/22/2021 08:15:46
Hello ! I've installed Webots in Windows 10 and also in a docker in W10.

I would like to run a world created under W10 with Docker Command line . Is that possible ?

##### Darko Lukić [Cyberbotics] 02/22/2021 09:41:38
If you include the world file in your Docker image there should not be any problem. Do you want to Webots UI in Docker as well?

##### h.sciascia 02/22/2021 09:45:31
Ok (I'm new to Docker thanks)

No UI only command line interface, I try to explore all the possibilities to speed-up simulations

##### Darko Lukić [Cyberbotics] 02/22/2021 09:47:02
If you want to get a good performance in Docker make sure you use nvidia-docker (if you have NVIDIA GPU)


Also, check this:

[https://www.cyberbotics.com/doc/guide/speed-performance](https://www.cyberbotics.com/doc/guide/speed-performance)

##### h.sciascia 02/22/2021 09:48:19
A graphic card is required ?

##### Darko Lukić [Cyberbotics] 02/22/2021 09:49:22
It is not required, but it will boost performance, especially if you use LiDARs, cameras, range finders and similar

##### h.sciascia 02/22/2021 09:49:39
Ok thanks a lot ! 🙂


Ok it's working (for a really small basic STL exemple) using the option "using mesh for boundingObject" during the import process

##### MrPie 02/22/2021 11:49:02
Hi, I just downloaded webots and the UI is really small.


I am on Linux and I have a 4k screen


Can you guys tell me how I can scale the UI properly? I have had a look in the docs but couldn't find anything.

##### Darko Lukić [Cyberbotics] 02/22/2021 12:36:33
Check this:

[https://github.com/cyberbotics/webots/pull/2631](https://github.com/cyberbotics/webots/pull/2631)


You can either download the nightly build:

[https://github.com/cyberbotics/webots/releases/tag/nightly\_19\_2\_2021](https://github.com/cyberbotics/webots/releases/tag/nightly_19_2_2021)



or set the environment variable:

```
QT_ENABLE_HIGHDPI_SCALING = 1
```

##### cindy 02/22/2021 13:57:22
Hi guys for ultrasonic sensors, in the actual code, is there a way of editing the angle range at which rays will be returned from 45 to something else?

##### Olivier Michel [Cyberbotics] 02/22/2021 14:19:33
Do you want to change the `DistanceSensor.aperture` value at run-time?

##### cindy 02/22/2021 14:19:59
Yes, basically. Because we need to detect lots of small objects from a distance

##### Olivier Michel [Cyberbotics] 02/22/2021 14:20:18
But if you change it in the model, wouldn't that be sufficient?

##### cindy 02/22/2021 14:21:00
Ah how do you do that


I have not got much webots experience sadly

##### Olivier Michel [Cyberbotics] 02/22/2021 14:21:30
See [https://cyberbotics.com/doc/reference/distancesensor](https://cyberbotics.com/doc/reference/distancesensor)

##### cindy 02/22/2021 14:22:39
Oh right aperture

##### Chernayaten 02/22/2021 14:22:43
From the Scene Tree (on the left most likely), go to your Robot, find the distance sensor and then edit the field

##### cindy 02/22/2021 14:22:47
thank you

##### h.sciascia 02/22/2021 15:46:14
Hello again ! 

Do you know if there is a way to convert an URDF file to a 3D model file (obj, stl or other) ?

I want to use this model as my boundingObject because i do not have a mesh file included to my urdf model..

What do you think? I do not find an answer on the web

##### Simon Steinmann [Moderator] 02/22/2021 15:48:52
a urdf file does not contain any 3D model definitions itself


it just defines, how links are connected through joints


what exactly do you want to achieve?


there is always this converter: [https://github.com/cyberbotics/urdf2webots](https://github.com/cyberbotics/urdf2webots)


it converts urdf files into proto files, which you can directly load into Webots. May I ask, which urdf file you want to convert?

##### h.sciascia 02/22/2021 15:52:12
I want to find a way to "resolve"/get around this issue [https://github.com/cyberbotics/webots/issues/2584](https://github.com/cyberbotics/webots/issues/2584)

##### Simon Steinmann [Moderator] 02/22/2021 15:55:13
I'm not sure how your urdf comes into play here


you can use the shapes of your model as your bounding objects

##### h.sciascia 02/22/2021 15:56:54
Not for boundingObject Group nodes..

##### Simon Steinmann [Moderator] 02/22/2021 15:57:37
share your world file (project dir) and urdf pls


you can pm me if you dont wanna post public

##### h.sciascia 02/22/2021 16:00:54
Ok I send you a pm in the evening

 `@Darko Lukić` must see what i mean

##### Simon Steinmann [Moderator] 02/22/2021 16:21:59
I dont quite understand the issue to be honest

##### Steven37 02/22/2021 17:22:49
I am using a Pioneer robot with a camera attached to take a picture of the trees on the sides like in the video. Then I want it to print out some of the characteristics of those trees but nothing is printed. I have successfully tried with one tree, but it didn't work when I put several trees at the same time like that. Can someone tell me why, please? Thank you!
> **Attachment**: [3Y\_Iproject\_Pioneer3AT\_cameraTesting.mp4](https://cdn.discordapp.com/attachments/565154703139405824/813460803847454823/3Y_Iproject_Pioneer3AT_cameraTesting.mp4)

##### Simon Steinmann [Moderator] 02/22/2021 17:23:43
Perhaps is something as simple as the trees not having unique names

##### Steven37 02/22/2021 17:23:47
Also, here's my code to print it out. I can still print "Hello World" but there are problems with my camera somewhere.
%figure
![3Y_Iproject_Pioneer_cameraTesting.JPG](https://cdn.discordapp.com/attachments/565154703139405824/813461049537200189/3Y_Iproject_Pioneer_cameraTesting.JPG)
%end

##### Simon Steinmann [Moderator] 02/22/2021 17:26:56
did you enable object recognition?


[https://cyberbotics.com/doc/reference/camera?tab-language=c#wb\_camera\_has\_recognition](https://cyberbotics.com/doc/reference/camera?tab-language=c#wb_camera_has_recognition)

##### Steven37 02/22/2021 17:28:21
Must all the trees have a different name? So why doesn't it print the figure for a tree in it?

##### Simon Steinmann [Moderator] 02/22/2021 17:28:38
was just a suggestion. can you post your entire code?

##### Steven37 02/22/2021 17:29:37
yes, i think so
%figure
![Pioneer_cameraEnable.JPG](https://cdn.discordapp.com/attachments/565154703139405824/813462518147710986/Pioneer_cameraEnable.JPG)
%end

##### Simon Steinmann [Moderator] 02/22/2021 17:31:09
> Only Solids whose recognitionColors field is not empty can be recognized by the camera.

##### Steven37 02/22/2021 17:35:52

%figure
![3Y_pioneer_code1.JPG](https://cdn.discordapp.com/attachments/565154703139405824/813464088075173888/3Y_pioneer_code1.JPG)
%end


that's my code
%figure
![3Y_pioneer_code2.JPG](https://cdn.discordapp.com/attachments/565154703139405824/813464160930889759/3Y_pioneer_code2.JPG)
%end

##### Simon Steinmann [Moderator] 02/22/2021 17:36:40
do the trees have recognition colors assigned?

##### Steven37 02/22/2021 17:36:57
and this is my solid scene tree
%figure
![Plant_sceneTree.JPG](https://cdn.discordapp.com/attachments/565154703139405824/813464360722235432/Plant_sceneTree.JPG)
%end


Is there any problem?

##### Simon Steinmann [Moderator] 02/22/2021 17:40:12
I have never used this functionality


can you share your project? I can take a look


you can pm me


ohhh


you only execute the while loop AFTER you moved forward and then stopped


You should run that loop while you are moving

##### Steven37 02/22/2021 17:43:26
how can I?


I have successfully tried with one tree before

##### Simon Steinmann [Moderator] 02/22/2021 17:44:29
just set the velocities and then execute the while loop


you can have an if statement in the loop, that aborts it or sets the speed to 0 after a certain time

##### Steven37 02/22/2021 17:48:48
I removed the stop section but still didn't work

##### Simon Steinmann [Moderator] 02/22/2021 17:49:11
the 140*timestep is the issue

##### Steven37 02/22/2021 17:49:12
how can I share my project to you?


Why?

##### Simon Steinmann [Moderator] 02/22/2021 17:49:27
just zip the project folder


well you execute 140 timesteps, BEFORE you start your object recognition

##### Steven37 02/22/2021 17:55:11
I have changed it but still the same

##### Simon Steinmann [Moderator] 02/22/2021 17:55:27
zip your project and send it to me


just drag & drop in discord

##### Steven37 02/22/2021 17:55:44
this is the world
> **Attachment**: [Pioneer3AT.wbt](https://cdn.discordapp.com/attachments/565154703139405824/813469090978332732/Pioneer3AT.wbt)

##### Simon Steinmann [Moderator] 02/22/2021 17:56:04
the whole directory, with the controllers folder and the worlds folder

##### Steven37 02/22/2021 17:56:17
this is my controller
> **Attachment**: [MyPoineer3AT\_controller.c](https://cdn.discordapp.com/attachments/565154703139405824/813469228480856085/MyPoineer3AT_controller.c)


sorry but is it still ok?

##### Simon Steinmann [Moderator] 02/22/2021 17:57:15
whole directory is much easier for me


I dont have to manually create and name all the folders

##### Steven37 02/22/2021 18:03:00

> **Attachment**: [3Y\_Webots\_Pioneer.rar](https://cdn.discordapp.com/attachments/565154703139405824/813470919791476776/3Y_Webots_Pioneer.rar)

##### Simon Steinmann [Moderator] 02/22/2021 18:03:32
of course this does not work


you are executing all the movement BEFORE you start your loop


dont execute robot steps outside of your loop

##### Steven37 02/22/2021 18:06:51
still the same when i put those code into the loop

##### Simon Steinmann [Moderator] 02/22/2021 18:07:04
show me that code

##### Steven37 02/22/2021 18:08:08
i just cut all the movement code above and paste them inside the while loop

##### Simon Steinmann [Moderator] 02/22/2021 18:08:20
dont call the timestep


that is your issue


you tell it to just exectue 9 seconds of simulation without doing anything

##### Steven37 02/22/2021 18:08:42
yes, i have changed it as wll


as well

##### Simon Steinmann [Moderator] 02/22/2021 18:09:07
works perfectly for me

##### Steven37 02/22/2021 18:09:26
can you show me, please?


What have you changed exactly?

##### Simon Steinmann [Moderator] 02/22/2021 18:10:30

> **Attachment**: [MyPoineer3AT\_controller.c](https://cdn.discordapp.com/attachments/565154703139405824/813472806300942376/MyPoineer3AT_controller.c)


as I said. removed all the extra robot steps

##### Steven37 02/22/2021 18:16:45
So if I don't just go straight now but I want my robot to turn at some points, what can I do? I think I must use wb\_robot\_step () to do that.

##### Simon Steinmann [Moderator] 02/22/2021 18:20:25
Do it like this
> **Attachment**: [MyPoineer3AT\_controller.c](https://cdn.discordapp.com/attachments/565154703139405824/813475300750524416/MyPoineer3AT_controller.c)


I added an example for stopping after 8 seconds


this is probably a cleaner solution
> **Attachment**: [MyPoineer3AT\_controller.c](https://cdn.discordapp.com/attachments/565154703139405824/813478288496001074/MyPoineer3AT_controller.c)

##### Steven37 02/22/2021 18:33:18
I wrote the code like this so the robot can move the way I want, but this time it didn't move at all.
%figure
![3Y_pioneer_movement.JPG](https://cdn.discordapp.com/attachments/565154703139405824/813478543127609344/3Y_pioneer_movement.JPG)
%end

##### Simon Steinmann [Moderator] 02/22/2021 18:34:25
check the newest example I uploaded


it's easier to implement and cleaner

##### Steven37 02/22/2021 18:44:35
it's perfect now


I don't know what can be said to thank you


Thank you, love you so much!

##### Simon Steinmann [Moderator] 02/22/2021 18:45:33
just pass on your knowledge once you gathered more expertise 🙂


and you're welcome!

##### Steven37 02/22/2021 18:47:49
Of course, I also want to show this kindness to anyone I can like what I was helped by you.


Thank you!

##### SeanLuTW 02/23/2021 04:51:30
How can I set the initial joints' angle in my UR5e PROTO correctly? I added six float fields in the built-in UR5e PROTO file and imported it to my world. I set these joints to the values I wanted and everything seems all right. After reloaded the world, the arm recovered to the original pose while the position sensors reported the value I had set.

##### Bitbots\_Jasper [Moderator] 02/23/2021 09:54:40
If you open your world file in a text editor, there should be a list of items similar to `hidden rotation_1 0 0 1 2.4306877183167175e-01`, you can modify the parameter to the desired initial position


there might be an easier way to do it though 😄


I also have a question. I am looking to adjust the maximum velocity of a motor from my (non supervisor) controller. I had the idea to export the field from the proto, but this a) requires a supervisor and b) does not work without reloading the model. Does anybody have an idea? I am basically looking for an alternative to the function setMaxVelocity() (which does not exist)

##### Olivier Michel [Cyberbotics] 02/23/2021 10:09:50
Are you applying position, velocity or force control?

##### Bitbots\_Jasper [Moderator] 02/23/2021 10:10:01
position control

##### Olivier Michel [Cyberbotics] 02/23/2021 10:15:58
Did you try to use `wb_motor_set_velocity` followed by `wb_motor_set_position`? Does it take the specified velocity to achieve the requested position?

##### Bitbots\_Jasper [Moderator] 02/23/2021 10:16:17
I'll test it


That seems to work, I assumed, that `wb_motor_set_velocity` was only for velocity control, but it seems as they are used as upper limits according to ` The current values for velocity, acceleration and motor torque/force are taken into account.` here: [https://cyberbotics.com/doc/reference/motor#wb\_motor\_set\_velocity](https://cyberbotics.com/doc/reference/motor#wb_motor_set_velocity)


thank you for your help `@Olivier Michel`

##### Olivier Michel [Cyberbotics] 02/23/2021 10:23:37
I believe it is clear also from this table: [https://cyberbotics.com/doc/reference/motor#force-and-torque-control](https://cyberbotics.com/doc/reference/motor#force-and-torque-control)

##### Bitbots\_Jasper [Moderator] 02/23/2021 10:25:59
yes that table makes it quite clear

##### Steven37 02/23/2021 11:32:31
I want to use my robot to continuously record the colors of a box where I used my monitor to continuously change its color like in the video but why can't the camera recognize that box?
> **Attachment**: [3Y\_caemera\_boxChangeColor.mp4](https://cdn.discordapp.com/attachments/565154703139405824/813735037726883870/3Y_caemera_boxChangeColor.mp4)


I also have enabled the recognitionColors like this
%figure
![3Y_recogniseEnable.JPG](https://cdn.discordapp.com/attachments/565154703139405824/813735134649122836/3Y_recogniseEnable.JPG)
%end

##### Harris 02/23/2021 12:14:47
Hi, I am pretty new to webots. I am wondering if there's anyway to transfer list type data from a supervisor to a robot controller in Python? I use emmitter/reciever function(emmitter.send) but can only transfer string, thank you lol.

##### Olivier Michel [Cyberbotics] 02/23/2021 12:20:59
`recognitionColors` is not meant for that: it is to assign a color to each object so that the object gets segmented with that color. If you want to recognize the color of the cube, you should read the pixels of the camera image instead.


You could serialize your data into a string and use the Emitter/Receiver. Otherwise, you can use any IPC (Inter Process Communication) system or possibly simply a file shared between your robot and supervisor and synchronize the access to it with Emitter/Receiver messages.

##### Srivastav\_Udit 02/23/2021 12:33:20
Can someone please suggest a pick and transport algorithm using a camera?

##### Steven37 02/23/2021 12:50:30
Can you explain to me in a little more detail, please? I don't really get that yet.

##### Olivier Michel [Cyberbotics] 02/23/2021 13:44:24
You should look at this example to understand how to recognize the color of an object from a camera image: [https://cyberbotics.com/doc/guide/samples-devices#camera-wbt](https://cyberbotics.com/doc/guide/samples-devices#camera-wbt)

##### iagsav 02/23/2021 13:51:39
Hi! I try to start "Rats Life" world, but get such error:


Native code library failed to load. See the chapter on Dynamic Linking Problems in the SWIG Java documentation for help.

java.lang.UnsatisfiedLinkError: E:\Users\Xuser\AppData\Local\Programs\Webots\lib\controller\java\JavaController.dll: Can't load AMD 64-bit .dll on a IA 32-bit platform

WARNING: 'Rat0' controller exited with status: 1.


please tell me how it can be fixed?

##### DDaniel [Cyberbotics] 02/23/2021 13:54:52
did you you through the java and java compiler installation process? [https://www.cyberbotics.com/doc/guide/using-java](https://www.cyberbotics.com/doc/guide/using-java), specifically this looks like your error: [https://www.cyberbotics.com/doc/guide/using-java](https://www.cyberbotics.com/doc/guide/using-java)#troubleshooting-the-java-installation

##### iagsav 02/23/2021 13:56:45
Thank you!

##### John520 02/23/2021 15:09:09
Hi guys, I'd like to import a CAD vehicle model into Webots. I am happy that the latest Webots release already supports the direct importation of .stl and .obj model. My question is how to make the imported vehicle run, just like the vehicles here [https://cyberbotics.com/doc/automobile/car](https://cyberbotics.com/doc/automobile/car). Thanks a lot.

##### Olivier Michel [Cyberbotics] 02/23/2021 15:11:21
Importing meshes isn't enough to model a robot. You should follow the tutorial about robot modeling: [https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials) (at least until tutorial 7).

##### John520 02/23/2021 15:51:25
Thank you `@Olivier Michel`. I went through the tutorial before. Am I right that I will need to add joints and motors to the wheels for the imported vehicle to run?

##### Olivier Michel [Cyberbotics] 02/23/2021 15:53:12
Yes.

##### John520 02/23/2021 15:54:31
Thank you `@Olivier Michel`. I will look into it. Thank you for your help!

##### h.sciascia 02/23/2021 15:57:30
Hello ! 

I try to use Docker in W10 for running webots without GUI 



I did this command to be able to get Windows files inside my docker : 

`docker run -it -v C:\Users\h.sciascia\Documents\Webots\:/usr/local cyberbotics/webots:latest`



But when my container is opened in command line, there is no webots sources file 



If I do only `docker run -it cyberbotics/webots:latest` , "webots" command is known and i'm able to run webots examples 



Do you had this issue ? 



Thank in advance !

##### Darko Lukić [Cyberbotics] 02/23/2021 16:14:59
Why do you want to mount the Windows folder to `/usr/local`?



The Webots installation is placed in `/usr/local`, so once you mount the Windows folder you override the Webots installation. Note that the Windows version of Webots cannot run inside of the Linux container.

##### h.sciascia 02/23/2021 16:21:58
Oh ok .. 

I just want to run my Webots developped under Windows in a Docker container (to see if the simulation speed up without GUI)

##### Darko Lukić [Cyberbotics] 02/23/2021 16:24:03
Then you have to mount only the Webots project folder (from Windows)

##### h.sciascia 02/23/2021 16:24:12
It's ok now with this : 

`docker run -it -v c:/Users/h.sciascia/Documents/Webots:/data cyberbotics/webots:latest ls /data`

##### Darko Lukić [Cyberbotics] 02/23/2021 16:24:49
Looks better

##### h.sciascia 02/23/2021 16:24:51
Thanks, i did not know that webots was at /usr/local

##### Darko Lukić [Cyberbotics] 02/23/2021 16:25:33
But your simulation will not run faster in Docker because the Docker image uses a virtual display. In the best case, it will run at the same speed.

##### Simon Steinmann [Moderator] 02/23/2021 16:25:55
`@Darko Lukić` Is Webots actually running headless in docker, when not using any visual based sensors?

##### Darko Lukić [Cyberbotics] 02/23/2021 16:26:42
No, it never uses truly headless mode.

##### Simon Steinmann [Moderator] 02/23/2021 16:26:51
For parallel simulation, this is the main thing holding Webots back compared to Pybullet

##### Darko Lukić [Cyberbotics] 02/23/2021 16:28:23
We should implement something like this:

[https://developer.nvidia.com/blog/egl-eye-opengl-visualization-without-x-server/](https://developer.nvidia.com/blog/egl-eye-opengl-visualization-without-x-server/)

##### Simon Steinmann [Moderator] 02/23/2021 16:29:37
It would probably be a very tall order, but the Server-Client setup of gazebo and PyBullet makes a lot of sense. Wish Webots would support that

##### Darko Lukić [Cyberbotics] 02/23/2021 16:29:49
Do you have some analysis of that. I would like to know what was the bottleneck for you, VRAM or RAM?

##### Simon Steinmann [Moderator] 02/23/2021 16:30:07
both


If I remember correctly, Vram was always at 800 MB, even when minimized and not rendering anything

##### Darko Lukić [Cyberbotics] 02/23/2021 16:30:40
How much RAM/VRAM usage, how many instances, and what is the world complexity?

##### Simon Steinmann [Moderator] 02/23/2021 16:30:56
it was a very simple world with just a robotic arm


and rectangle arena


was like 5 month ago

##### h.sciascia 02/23/2021 16:31:42
So there is no way to run faster the simulation using Windows 10 ? :/

##### Simon Steinmann [Moderator] 02/23/2021 16:31:46
webots also used I think 1.1 GB of ram minimum. However, now on windows, I see it uses below 370MB


I did parallel RL, that quicly saturated my VRAM and RAM


it was cpu only simulation, so it kinda sucked to spend resources on something that is not needed. I can test things again. that was on 2020b


`@h.sciascia` There is many things you can do to speed up the simulation. I have quite some experience with that.


What is your goal, what are you simulating?

##### Darko Lukić [Cyberbotics] 02/23/2021 16:35:35
I have just tested, it is 330MB with `pioneer3at.wbt` (relatively big world + LiDAR)

##### Simon Steinmann [Moderator] 02/23/2021 16:35:52
windows or linux?

##### h.sciascia 02/23/2021 16:39:05
I cannot explain a lot for confidentiality reason but I use an URDF robot which explore all of its envrionment to detect the collisions (with a personalised physics plugin), go through them and save each collision in a CSV file

##### Simon Steinmann [Moderator] 02/23/2021 16:39:25
Yeah, on windows I get 360MB RAM and 220MB VRAM. That is already MUCH better. I still wish it would be possible to run without any gui at all

##### Darko Lukić [Cyberbotics] 02/23/2021 16:39:33
Linux

##### Simon Steinmann [Moderator] 02/23/2021 16:39:59
mobile robot or robotic arm?

##### h.sciascia 02/23/2021 16:40:11
So I did simulation for 1-2-3-4 DOF, but with 5 DOF it's becoming really really long (should take 2-3 days to explore all the possibilities)

##### Simon Steinmann [Moderator] 02/23/2021 16:40:34
did you profile your code?


IK can take a significant portion of time


I recommend using IKFast, if you are not already


also, increase the step size of your simulation and use simple bounding boxes

##### h.sciascia 02/23/2021 16:41:49
No IK , just iterative imbricated loop for be sure to run trhough all the possibilities


yep i did the increase for step size of the motors and use cubes

##### Simon Steinmann [Moderator] 02/23/2021 16:42:29
what is your realtime factor?

##### Darko Lukić [Cyberbotics] 02/23/2021 16:47:48
`@h.sciascia`  As Simon mentioned, you should profile your controller and the physics plugin. The controller is usually cause for the slow simulations.

##### MrPie 02/23/2021 16:48:34
Hi, just created my robot and I wanted to add a rgbD camera (RFID camera ) But I haven't had any luck finding one. Could you guys help me with that?


RgbD is a depth camera

##### Darko Lukić [Cyberbotics] 02/23/2021 16:49:11
You should combine the camera node and the range finder node

##### MrPie 02/23/2021 16:49:41
Oh OK, will have a look into that


Thanks so much

##### Darko Lukić [Cyberbotics] 02/23/2021 16:50:39
Just use the same pose for both nodes and the same intrinsic parameters (e.g. FoV)


I discovered a bug:

[https://github.com/cyberbotics/webots/issues/2776](https://github.com/cyberbotics/webots/issues/2776)

##### Simon Steinmann [Moderator] 02/23/2021 16:51:27
let me verify


for me on windows it garbage collects most of it, but not all. about 60MB extra

##### hugos 02/23/2021 16:58:43
(always me with personnal account) 

Ok thanks I will try that, thanks a lot for your time both of you 👍👍👍

##### Srivastav\_Udit 02/23/2021 18:12:29
Can someone please help me resolve this error?



Error: ignoring illegal call to wb\_differential\_wheels\_set\_speed() in a 'Robot' controller.

Error: this function can only be used in a 'DifferentialWheels' controller.

##### DDaniel [Cyberbotics] 02/23/2021 18:21:30
`@Srivastav_Udit`  The differential wheels node has been deprecated so you shouldn't be using it unless you really need to. You can achieve the same with a classic Robot node setting a hinge to either side and setting the left and right motor accordingly

##### Srivastav\_Udit 02/23/2021 18:33:27
Thank you so much Daniel! I'm trying to use the wb\_differential\_wheels\_get\_left\_encoder(); function with the epuck. Can you recommend another way to achieve this function on the epuck?

##### DDaniel [Cyberbotics] 02/23/2021 18:36:40
You can add also PositionSensors to the hinges, same way you add the motors

##### Srivastav\_Udit 02/23/2021 18:37:33
I'll try that, thanks again!

##### DDaniel [Cyberbotics] 02/23/2021 18:40:38
there's an epuck sample world already to see how it's done: file > open sample world > robot > gctronic > epuck

##### Srivastav\_Udit 02/23/2021 18:43:57
That's exactly what I was looking for, thanks a lot!

##### Chernayaten 02/23/2021 20:07:57
I think the robot tutorial on the webots website might help you as well: [https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot](https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot)

##### Srivastav\_Udit 02/23/2021 20:24:26
Has anyone worked on a simple object transportation algorithm using a camera with obstacle avoidance?


I'm essentially trying to search for an object and transport it to a "nest" in the fastest way possible avoiding any obstacles

##### Simon Steinmann [Moderator] 02/23/2021 20:26:19
what do you mean by "transport" ?


Are we talking abstract 3d movement, or a roboter manipulating it?

##### Srivastav\_Udit 02/23/2021 20:27:46
I've created a robot that is supposed to detect a ball in an arena and transport it to a nest

##### Simon Steinmann [Moderator] 02/23/2021 20:28:01
can you share a screenshot?

##### Srivastav\_Udit 02/23/2021 20:29:41

%figure
![PXL_20210223_202905478.jpg](https://cdn.discordapp.com/attachments/565154703139405824/813870218840375306/PXL_20210223_202905478.jpg)
%end

##### Simon Steinmann [Moderator] 02/23/2021 20:30:21
oh, is this your actual physical setup?

##### Srivastav\_Udit 02/23/2021 20:30:49
Yes, the robot has a gripper that can hold the ball

##### Simon Steinmann [Moderator] 02/23/2021 20:31:02
are you using ROS?


to control the bot

##### Srivastav\_Udit 02/23/2021 20:31:46
No, I'm not very familiar with ROS yet

##### Simon Steinmann [Moderator] 02/23/2021 20:32:19
are you using an overhead camera? or onboard?


and how do you control the bot?

##### Srivastav\_Udit 02/23/2021 20:34:16
I have a camera attached to the bot. It essentially spins till it detects the ball and then attempts to move towards it and grips the ball if it is within the reach  of the gripper.


I'm trying to work out how it can find the best path to the nest avoiding obstacles. I also need to understand how I can release the ball in the nest and then go search the arena again.

##### Simon Steinmann [Moderator] 02/23/2021 20:35:45
sorry, I dont have experience with this


you might want to look into "SLAM", but that is more advanced


but I think you need to implement odometry of some sort, so the robot knows where it is


simplest solution would probably trying to drive directly towards the nest and to just drive around obstacles

##### Bitbots\_Jasper [Moderator] 02/23/2021 20:37:48
mobile robot navigation is a relatively well researched topic with a lot of algorithms already implemented well. I think it is easier to learn ROS and use the navigation stack of ros than to reimplement these algorithms


the navigation stack may be overkill though if you want to find a "hacky" solution


but if you have lots of obstacles in the environment something like slam is definitely required


otherwise you could get away with some simple object detection and maybe potential field navigation


but if you want to get into robotics, it's really useful to know ROS

##### Srivastav\_Udit 02/23/2021 20:39:35
Do you have any algorithm/sample project I could use for reference?

##### Simon Steinmann [Moderator] 02/23/2021 20:39:53
no, sorry

##### Srivastav\_Udit 02/23/2021 20:40:49
Can you give me a reference point to get started with ROS? I don't have a lot of obstacles in my environment but I do have to attempt a swarm implementation later on.

##### Simon Steinmann [Moderator] 02/23/2021 20:43:20
ROS takes a while to wrap your head around

##### Bitbots\_Jasper [Moderator] 02/23/2021 20:43:43
the ROS wiki is your friend [https://index.ros.org/doc/ros2/](https://index.ros.org/doc/ros2/). In my opinion it is better to start using ROS2 right away since ROS 1 is nearing end of life (2025)


yes `@Simon Steinmann`  is right, it takes quite a bit of time to get to know it


there is also the ROS2 controller for webots [https://github.com/cyberbotics/webots\_ros2](https://github.com/cyberbotics/webots_ros2)

##### Simon Steinmann [Moderator] 02/23/2021 20:45:06
[https://www.theconstructsim.com/](https://www.theconstructsim.com/) I did this online academy. Yes it cost a bit of money, but I learned in 2 weeks much more than I did in the 2-3 moth before, trying to figure it out myself

##### Bitbots\_Jasper [Moderator] 02/23/2021 20:45:52
it is a bit difficult to estimate for your use case if it is better to learn ROS, which will benefit you a lot in the future if you continue doing robotics, or to implement your own solution

##### Srivastav\_Udit 02/23/2021 20:47:08
This might be a silly question but if I were to write a controller without ROS and had multiple robots to collect and drop these balls in a nest (probably like the hacky solution you mentioned) - what would you recommend ?

##### Bitbots\_Jasper [Moderator] 02/23/2021 20:48:17
if you want to stay inside webots there is the emitter and receiver: [https://cyberbotics.com/doc/reference/emitter](https://cyberbotics.com/doc/reference/emitter), [https://cyberbotics.com/doc/reference/receiver](https://cyberbotics.com/doc/reference/receiver)

##### Srivastav\_Udit 02/23/2021 20:49:50
Would this involve a supervisor since there are multiple robots?

##### Bitbots\_Jasper [Moderator] 02/23/2021 20:52:24
no, you dont need a supervisor controller to do that

##### Srivastav\_Udit 02/23/2021 20:54:50
Okay, thanks!

##### SeanLuTW 02/24/2021 02:55:44
I think I found the problem. After reload world or reset the simulator, the `hidden` descriptions in the world file were eliminated, which caused the angle offset. (`hidden` tells link how many angle to rotate, and `position` tells the position sensor current angle)


How can I avoid eliminating the `hidden` descriptions after reset the controller?

##### h.sciascia 02/24/2021 08:13:53
Hello ! Didn't see this message my bad



What do you mean by realtime factor ? Are you talking about the speed of the simulation ? Or the timeStep in my application ?

##### Bitbots\_Jasper [Moderator] 02/24/2021 08:56:14
did you properly save your world?

##### SeanLuTW 02/24/2021 08:58:45
Yes, I saved my world, and I can see the `hidden` in the file. But after I clicked reset simulator, the robot just went back to original pose.


I read the document from [https://cyberbotics.com/doc/reference/proto-hidden-fields](https://cyberbotics.com/doc/reference/proto-hidden-fields) . Maybe after clicking reset button, the `hidden` description just been ignored by the simulator?


If I used reload world, everything seems alright. But the world is pretty completed, it spend lots of time to load

##### Bitbots\_Jasper [Moderator] 02/24/2021 09:18:03
to clarify this a bit more, you changed the value of the position field in the JointParameter node?

##### SeanLuTW 02/24/2021 09:20:11
Yes, I defined a `SFFloat` variable to specify the initial angle in PROTO, and use this variable to set the `position` under `HingeJointParameters`

##### Vangiel 02/24/2021 09:21:10
hello everyone. I have two simple questions that I don't get to solve. Using python: How can I get the basictimestep of the simulation? How can I get the simulation time? Thanks

##### SeanLuTW 02/24/2021 09:26:40
`getBasicTimeStep` and `getTime`in  `Robot`  Here is the document [https://cyberbotics.com/doc/reference/robot?tab-language=python#wb\_robot\_get\_time](https://cyberbotics.com/doc/reference/robot?tab-language=python#wb_robot_get_time)

##### Vangiel 02/24/2021 09:27:02
Thanks a lot

##### Bitbots\_Jasper [Moderator] 02/24/2021 09:44:13
I think the problem with changing the position field is described in the field description of the JointParameters Node: [https://cyberbotics.com/doc/reference/jointparameters#field-summary](https://cyberbotics.com/doc/reference/jointparameters#field-summary)


`The position field represents the current position of the joint, in radians or meters. For an hinge or ball, it is the current rotation angle in radians. For a slider, it is the magnitude of the current translation in meters. When changing the position field from the Webots scene tree, Webots also changes the corresponding rotation (for a hinge or ball) or translation (for a slider) field in the endPoint solid of the parent joint node to keep the consistency of the model. Similarly, when changing the position field of a JointParameters node in a text editor, you should take care of also changing the corresponding rotation or translation field accordingly.`


so you only change the position of the joint but not the transformation to the endPoint


this kind of makes it hard to set different initial positions of a single proto model


and for each joint you need to calculate original\_rotation\_matrix x rotation\_introduced\_by\_joint (if these are rotation matrices)

##### SeanLuTW 02/24/2021 09:57:44
Maybe the best solution is to change the PROTO into base node 😦

##### Callum98 02/24/2021 11:26:10
Hey guys, I have two working cameras on my robot but they both seem blurry and out of focus, how do I fix this?

##### Bitbots\_Jasper [Moderator] 02/24/2021 11:27:31
could you share the definition of the camera node in your proto?

##### Callum98 02/24/2021 11:28:40
Sorry what do you mean by that?

##### Bitbots\_Jasper [Moderator] 02/24/2021 11:30:15
In the proto file, the file that defines your robot, there should be a camera definition that looks somewhat like this:

```Camera {
  fieldOfView 0.5
  ....
}
```

##### Callum98 02/24/2021 11:33:24
Oh the lines I've used in my code?

##### Bitbots\_Jasper [Moderator] 02/24/2021 11:33:59
not your controller code


what robot are you using, did you define it yourself, or did you use a preconfigured one?

##### Callum98 02/24/2021 11:34:56
Its one I'm making myself for a coursework, sorry I'm new to this only been doing it for about 2 weeks

##### Bitbots\_Jasper [Moderator] 02/24/2021 11:36:37
dont worry 😄

you should have created a `robot.proto`file, to define the robot. can you share that?

 if you dont want it publicly, you can also pm me

##### Simon Steinmann [Moderator] 02/24/2021 11:39:42
realtime factor is the speed your simulation runs at. If you had an absolutely perfect 1x realtime factor, the simulation time and real time would progress at the same rate. From what you describe, you should be able to get realtime factors in the area of 10x-200x. Of course this also heavily depends on your system, in particualr the single core speed of your CPU. What are your system specs?

##### Callum98 02/24/2021 11:41:08
I don't think I have a proto file, the protos folder is empty. Don't worry they aren't too bad, should be good enough for my robot to work, thank you for the help anyway

##### Simon Steinmann [Moderator] 02/24/2021 11:41:30
`@Callum98` how did you add the cameras?


[https://cyberbotics.com/doc/reference/camera?tab-language=python](https://cyberbotics.com/doc/reference/camera?tab-language=python)


perhaps you specified a focus value


look at your robot node in the scene tree on the left. What settings do you have there?

##### Callum98 02/24/2021 11:42:37
In robot children clicked the add button and specified camera, creating a shape as something to see

##### Simon Steinmann [Moderator] 02/24/2021 11:43:05
the default resolution is 64x64

##### Callum98 02/24/2021 11:43:55
How do I change the resolution so I can show camera image bigger? without it losing focus

##### Simon Steinmann [Moderator] 02/24/2021 11:44:37
expand the camera node


you should see all the settings there


use the documentation link I sent, to adjust the settings

##### Bitbots\_Jasper [Moderator] 02/24/2021 11:46:03
There is a difference between an image being really low resolution and being out of focus

##### Callum98 02/24/2021 11:49:57
Like this, I just want the image to be a bit clearer
%figure
![Camera.png](https://cdn.discordapp.com/attachments/565154703139405824/814101812394852372/Camera.png)
%end

##### Simon Steinmann [Moderator] 02/24/2021 11:50:20
increase the resolution

##### Callum98 02/24/2021 11:55:45
Got it, thanks man!

##### h.sciascia 02/24/2021 12:36:20
Yes, there is two mean time, when I saved nothing simulation can go 300-400x, and when I save in csv x70-80 max



Very old specs : 4GB Ram, Intel Core i5 4310U (2-2.6 GHz) , no graphic card

##### Simon Steinmann [Moderator] 02/24/2021 12:39:34
that is already really good. I dont think you'll get much faster than that. But for your task you might want to look into different solutions, as you dont require physics

##### h.sciascia 02/24/2021 12:40:37
Yes I will try ROS , maybe it's better for my app

##### Simon Steinmann [Moderator] 02/24/2021 12:41:01
just rviz or moveit could work

##### h.sciascia 02/24/2021 12:41:37
Ok I will try them, thanks a lot ! 🙂

##### Simon Steinmann [Moderator] 02/24/2021 12:42:03
[https://github.com/BerkeleyAutomation/python-fcl](https://github.com/BerkeleyAutomation/python-fcl)


or you give this a look

##### h.sciascia 02/24/2021 12:43:36
Ok it's noted , thanks !

##### John520 02/24/2021 15:17:48
Hi `@Olivier Michel`, I'd like to follow up on the question I asked yesterday. In order to run the imported vehicle, I will need to add joints and motors to the wheels. In the tutorial [https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot?tab-language=c++](https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot?tab-language=c++), a controller is used to control the motors. My question is, could I use ROS to control the motors? Thank you!

##### Olivier Michel [Cyberbotics] 02/24/2021 15:20:48
Yes, of course. There are many online tutorials explaining how to use ROS and ROS 2 with Webots.

##### John520 02/24/2021 15:30:08
`@Olivier Michel` Thanks a lot. I guess I would need to follow this [https://cyberbotics.com/doc/reference/motor?tab-language=ros](https://cyberbotics.com/doc/reference/motor?tab-language=ros) to control the motors. I used ROS to control the truck you provides in Webots successfully before.

##### yanan 02/24/2021 16:15:29
Hi there, I would like to generate a  dataset by frequently setting the position of tesla in the environment,  I am using this codes which dose not work and it always report : initializer element is not constant. is there anything wrong with my first line of code?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/814168637875027998/unknown.png)
%end

##### Olivier Michel [Cyberbotics] 02/24/2021 16:24:00
It looks good to me. The error is probably somewhere else...

##### Bitbots\_Jasper [Moderator] 02/24/2021 16:29:55
can you post a screenshot of the scene tree (the list of elements on the left in webots)?

##### yanan 02/24/2021 16:39:12
Hi, thanks for your replies. there are two robots and two controllers, one is Mavic2pro the another is TeslaModel3.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/814174605505593374/unknown.png)
%end


in tesla controller, I put something like this, the compiling is OK. but it dose not change the position of telsa
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/814174846929600582/unknown.png)
%end

##### Bitbots\_Jasper [Moderator] 02/24/2021 16:40:33
you need to set the DEF of the TeslaModel3 to be able to access it, click on the tesla in the scene tree and below there should be a text field


set that to TeslaModel3


or you can use wb\_supervisor\_node\_get\_self

##### yanan 02/24/2021 16:44:41
thanks a lot. it works. I forget to put TeslaModel3 as the DEF although the in scene tree it displays 'TeslaModel3', which is a little bit confusing

##### Bitbots\_Jasper [Moderator] 02/24/2021 16:45:28
yeah, what you are seeing is the name of the robot as defined in the PROTO file of the robot

##### yanan 02/24/2021 16:46:31
understood.  thanks

##### Bitbots\_Jasper [Moderator] 02/24/2021 16:49:37
good to hear it works 👍

##### yanan 02/24/2021 17:47:39
sorry to bother again. is there any way to share variables or communication between two controllers, such as mavic2pro and Tesla. Actually, I checked the Emitter and Receiver, it seems like it is for robots,  not robot and vehicle?

##### Olivier Michel [Cyberbotics] 02/24/2021 18:14:00
A vehicle in Webots is a Robot. So, it should work with Emitter/Receiver.

##### yanan 02/24/2021 18:14:46
thanks, I will have a try


just a feedback,  you are right, it works.  thanks

##### Kirlin 02/24/2021 22:06:53
Hello everyone, I am migrating the simulation of my robot project to Webots and, to be as close to reality as possible, I would like to use a controller based on ros topics, not services like the premade controller on Webots\_ros package. I tried use the Custom Ros Controller tutorials on the webots website itself, but I didn't get any results in compiling webots and ros together. Do you suggest any tutorial or material bigger or more complete than the site?

##### Darko Lukić [Cyberbotics] 02/25/2021 07:51:07
Hello, here is a custom ROS controller example:

[https://github.com/cyberbotics/webots\_ros/blob/master/scripts/ros\_python.py](https://github.com/cyberbotics/webots_ros/blob/master/scripts/ros_python.py)



If possible, I would highly recommend you to go with ROS 2, as our ROS 2 bridge creates a ROS 2 interface that is much closer to one on the real robots + the behavior is highly customizable:

[https://github.com/cyberbotics/webots\_ros2](https://github.com/cyberbotics/webots_ros2)

##### dA$H 02/25/2021 13:12:46
Hi.

How can i stop conveyorBelt in python?

##### Darko Lukić [Cyberbotics] 02/25/2021 13:20:15
You can use Supervisor to get the speed field and set it to 0:

[https://cyberbotics.com/doc/reference/supervisor](https://cyberbotics.com/doc/reference/supervisor)

##### Krish 02/25/2021 13:44:36
Is there a way that I can put mirrors in Webots?

I am planning to build a a kaleidoscope.



I know it sounds weird, but I just wanted to give it a try.

##### Darko Lukić [Cyberbotics] 02/25/2021 13:46:45
[https://cyberbotics.com/doc/guide/object-mirror](https://cyberbotics.com/doc/guide/object-mirror)

##### yanan 02/25/2021 14:26:36
Hi all, I am trying to move the vehicle into different postions in a loop, do you know why it only shows the last position I set?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/814503623235993620/unknown.png)
%end

##### Darko Lukić [Cyberbotics] 02/25/2021 14:32:16
You cannot use `sleep(1)` you need `wb_robot_step(1024)` instead

##### yanan 02/25/2021 14:41:45
wow, you are right. it works.  thank you so much.

##### dA$H 02/25/2021 14:42:12
I am trying to get conveyorBelt speed but it's not work
%figure
![conveyor.jpg](https://cdn.discordapp.com/attachments/565154703139405824/814507547192852580/conveyor.jpg)
%end

##### yanan 02/25/2021 14:43:17
you saved my afternoon. danke schoen

##### Darko Lukić [Cyberbotics] 02/25/2021 14:44:35
If you use `getFromDef` then you have to match DEF of the ConveyorBelt

##### Chernayaten 02/25/2021 14:45:06
You probably need to do getSFFloat  to get the speed, and you should also be doing that inside the while, otherwise the value will not update

##### Darko Lukić [Cyberbotics] 02/25/2021 14:46:18

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/814508581600755724/unknown.png)
%end


Also this. Thanks `@Chernayaten`

##### dA$H 02/25/2021 15:18:14
thank you
%figure
![empty_00_00_00-00_00_30.gif](https://cdn.discordapp.com/attachments/565154703139405824/814516615411793970/empty_00_00_00-00_00_30.gif)
%end

##### Le Fromage 02/25/2021 15:28:24
does anyone know how to set the point of rotation for a shape?

##### Simon Steinmann [Moderator] 02/25/2021 15:28:55
what do you mean by point of rotation?

##### Le Fromage 02/25/2021 15:31:37
im pretty new, im basically trying to make 4 legs for an animal of some sort


so the rotational motor would need to be at the top of the leg

##### Simon Steinmann [Moderator] 02/25/2021 15:34:01
a motor is always inside of hingejoint. And a hingejoint connects two Solids


Solid > children > hingejoint > endPoint Solid


the solids have translation and rotation fields, where you can position them


to make things easier, make sure that the translation of the endpoint solid is the same as the anchor of the hingejoint


[https://cyberbotics.com/doc/reference/hingejoint?tab-language=python](https://cyberbotics.com/doc/reference/hingejoint?tab-language=python)

##### Le Fromage 02/25/2021 15:39:18
thanks a bunch seems to work now

##### Steven37 02/25/2021 16:01:29
I am wanting to use the camera to analyze the colour of the image. I have coded it based on a sample camera code in this link: [https://github.com/cyberbotics/webots/blob/master/projects/samples/devices/controllers/camera/camera.c](https://github.com/cyberbotics/webots/blob/master/projects/samples/devices/controllers/camera/camera.c). But I don't understand why when I apply it to my project I get this error.
%figure
![3Y_project_cameraError.JPG](https://cdn.discordapp.com/attachments/565154703139405824/814527499685068880/3Y_project_cameraError.JPG)
%end

##### DDaniel [Cyberbotics] 02/25/2021 16:04:52
do you initialize (blue = 0) the variable blue somewhere before these lines?

##### Bitbots\_Jasper [Moderator] 02/25/2021 16:14:03
If I have multiple simulations running at the same time, I can use the environment variable WEBOTS\_PID to tell an external controller to which simulation it should connect. I am running a python external controller. Does someone by chance know if the state of this environment variable is relevant at the time of import or at the time of instantiation of the Robot object?

##### alejanpa17 02/25/2021 16:24:45
Hi, is it possible to give an object a malleability property?


And other question, is it possible to limit the render distance in order to optimize the simulation performance?

##### Darko Lukić [Cyberbotics] 02/25/2021 16:27:10
You can start a Webots instance and external controller pair with the same `WEBOTS_TMPDIR`:

[https://cyberbotics.com/doc/guide/running-extern-robot-controllers#running-extern-robot-controller-with-the-snap-version-of-webots](https://cyberbotics.com/doc/guide/running-extern-robot-controllers#running-extern-robot-controller-with-the-snap-version-of-webots)



That way you are sure multiple Webots instance/controller pairs are isolated from each other.

##### Steven37 02/25/2021 16:31:32
yes, i have for all green, blue and red. I don't know why only green is right but the other two are wrong.

##### Darko Lukić [Cyberbotics] 02/25/2021 16:31:58
No, Webots is a rigid body physics simulator


No, but check this:

[https://www.cyberbotics.com/doc/guide/speed-performance](https://www.cyberbotics.com/doc/guide/speed-performance)



The performance improvements can be done in another ways

##### DDaniel [Cyberbotics] 02/25/2021 16:38:33
`@Steven37` hard to say why like this, can you provide a snippet of the code?

##### Steven37 02/25/2021 16:44:42
This is my controller. The code may quite long but I hope you can help me check it out as I don't know where I could  go wrong. Thank you!
> **Attachment**: [controllerForTesting.c](https://cdn.discordapp.com/attachments/565154703139405824/814538375361069106/controllerForTesting.c)

##### BeastFromTheEast 02/25/2021 16:48:43
Hi, I am having issues with the compiler as I keep recieving " No rule to make target 'build/release/BotClass.h', needed by 'build/release/dez1.exe'.  Stop." I added the extra files I wanted to be compiled to the makefile already

##### DDaniel [Cyberbotics] 02/25/2021 16:54:41
`@Steven37` Line 112 bracket shouldn't be there and you're missing one on line 176

##### Steven37 02/25/2021 17:02:10
thank you so now that error gone


However, I want to read the color and detect the box and the tree as shown in my video but there isn't anything printed in my console. Can anyone show me why, please?
> **Attachment**: [3Y\_cameraTesting.mp4](https://cdn.discordapp.com/attachments/565154703139405824/814543350560194641/3Y_cameraTesting.mp4)

##### dA$H 02/25/2021 17:09:21
how can i see the value of "range Slider" in cyberbotic website?
%figure
![range.jpg](https://cdn.discordapp.com/attachments/565154703139405824/814544579268313138/range.jpg)
%end

##### Vangiel 02/25/2021 18:27:17
Hello, is there any implementation to check if a collision between two entities has occurred? (Using python)

##### Darko Lukić [Cyberbotics] 02/25/2021 19:01:29
You can use the touch sensor:

[https://cyberbotics.com/doc/reference/touchsensor](https://cyberbotics.com/doc/reference/touchsensor)



Alternatively, you can add a Physics plugin:

[https://www.cyberbotics.com/doc/reference/callback-functions#int-webots\_physics\_collidedgeomid-dgeomid](https://www.cyberbotics.com/doc/reference/callback-functions#int-webots_physics_collidedgeomid-dgeomid)

and send details about the collision to your Python controller.

##### yanan 02/25/2021 21:57:58
Hi, my Emitter sends a series of packs, how can my Receiver only get the latest pack instead of the next one?

##### Vangiel 02/26/2021 08:56:18
Thank you, I will give it a try

##### yanan 02/26/2021 09:33:09
Hello, I am using two controllers, one for Mavic2Pro with an emitter, the other for TeslaModel3 with a receiver.  The Tesla is receiving the position information from the drone. However, it seems like the receiver can not real-timely get the latest info from the emitter, it can only get a pack one by one hence resulting in a delay to get the latest info from the emitter.

##### Darko Lukić [Cyberbotics] 02/26/2021 09:34:07
How do you receive the messages?

##### yanan 02/26/2021 09:35:11

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/814792671423758346/unknown.png)
%end



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/814792782798913546/unknown.png)
%end

##### Darko Lukić [Cyberbotics] 02/26/2021 09:39:40
If you send N messages in the single step you receive only 1?

##### yanan 02/26/2021 09:40:45
yeah, that's true.  and that 1 is not latest


is there any synchronization mechanism between two controllers using emitter and receiver?  or do I have to use same amount of sending packs and receiving packs?

##### Darko Lukić [Cyberbotics] 02/26/2021 09:43:50
That is strange. Can you send us a minimal example?


>  or do I have to use same amount of sending packs and receiving packs?

But you are receiving the packets in the loop which means that you should receive all the packets you have sent.

##### yanan 02/26/2021 09:50:13
this is a simplified version of two controllers
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/814796456417820692/unknown.png)
%end


it might also because the emitting frequency is different from receiver's.

##### Darko Lukić [Cyberbotics] 02/26/2021 09:51:39
But you are sending only one message per step


Can you send a project file (including the world file), so I can test with the same configuration

##### yanan 02/26/2021 09:54:10
Sure,  much appreciated for that.


[https://uob-my.sharepoint.com/:u:/g/personal/yl17692\_bristol\_ac\_uk/EfF90tkuB4xIr6Vq4wXFvW4Bor\_inj1V4Rqj7udHVXFR3Q?e=qyv4Jd](https://uob-my.sharepoint.com/:u:/g/personal/yl17692_bristol_ac_uk/EfF90tkuB4xIr6Vq4wXFvW4Bor_inj1V4Rqj7udHVXFR3Q?e=qyv4Jd)


I am using Windows 10 with the latest version of Webots

##### Darko Lukić [Cyberbotics] 02/26/2021 10:13:10
Inside of `while (wb_receiver_get_queue_length(receiver) > 0)` you are essentially doing:

```
int step = 3;
double rot_step = M_PI/4;
for(int i = -step; i <= step; i++ )
{
  for(int j = -step; j <= step; j++)
  {
     for(float r = 0; r <= M_PI; r +=rot_step)
     {
          wb_robot_step(64);
     }
  }         
}
```


You have a lag of like 300 steps per single drone step

##### yanan 02/26/2021 10:39:06
thanks for pointing that, which is very helpfu;

##### h.sciascia 02/26/2021 10:43:36
Hello ! 

How can I get a 4x4 matrix describing the rotation and translation of the end-effector of URE10 ?

where R is the 3×3 submatrix describing rotation and T is the 3×1 submatrix describing translation. (I want to do some forward kinematics using Denavit Hartenberg)
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/814809890202845204/unknown.png)
%end

##### Simon Steinmann [Moderator] 02/26/2021 10:56:20
`@h.sciascia` what language are you using?

##### h.sciascia 02/26/2021 10:56:28
Python

##### Simon Steinmann [Moderator] 02/26/2021 10:56:33
use numpy then

##### h.sciascia 02/26/2021 10:57:09
I cannot get it with a node ? Like a GPS ?

##### Darko Lukić [Cyberbotics] 02/26/2021 10:57:14
[https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_get\_position](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_get_position)

##### Simon Steinmann [Moderator] 02/26/2021 10:57:18
take a look at this python module I made for getting relative positions
> **Attachment**: [get\_relative\_position.py](https://cdn.discordapp.com/attachments/565154703139405824/814813337358434325/get_relative_position.py)


self.ee\_pose[:3,:3] = target\_rot

self.ee\_pose[:3,3] = target\_pos


you can fill a 4x4 array like this

##### h.sciascia 02/26/2021 10:59:00
Thanks, I will try the two solutions ! 🙂

##### Simon Steinmann [Moderator] 02/26/2021 10:59:14
if you want a working ik / fk solution, let us know 😉

##### h.sciascia 02/26/2021 10:59:42
Yes I find one with PyRobotics for FK

##### Simon Steinmann [Moderator] 02/26/2021 10:59:57
do you need FK only, or also IK?

##### Steven37 02/26/2021 11:29:22
Is it possible for me to use supervisor to remove an object and adding a new object to the same position?

##### Simon Steinmann [Moderator] 02/26/2021 11:29:36
yes


remove and insert Field


[https://cyberbotics.com/doc/reference/supervisor?tab-language=python#wb\_supervisor\_field\_remove\_mf](https://cyberbotics.com/doc/reference/supervisor?tab-language=python#wb_supervisor_field_remove_mf)

##### Steven37 02/26/2021 11:39:38
If possible, is there any sample code on how to use those functions to do that?

##### Simon Steinmann [Moderator] 02/26/2021 12:03:35
this is an example to spawn a node in a field
> **Attachment**: [spawn\_target.py](https://cdn.discordapp.com/attachments/565154703139405824/814830020979064832/spawn_target.py)

##### h.sciascia 02/26/2021 12:32:08
FK only, I use IKPY for IK

##### Simon Steinmann [Moderator] 02/26/2021 12:37:57
ikpy has fk


ikpy for IK is really slow and has less than ideal results


I made a generik ikpy controller a while ago
> **Attachment**: [generic\_inverse\_kinematic\_.zip](https://cdn.discordapp.com/attachments/565154703139405824/814840204552699934/generic_inverse_kinematic_.zip)


you should be able to simple add it, and it works


has fk included too

##### h.sciascia 02/26/2021 12:44:32
oh ok... I will try with your's


thanks !

##### Bitbots\_Jasper [Moderator] 02/26/2021 13:35:35
Thanks for your response, I have not used `WEBOTS_TMPDIR` but again the PID and I figured out the response to my question through some testing. The environment variable `WEBOTS_PID` is read at the time when the robot controller is created (in python that is when you do `r = Robot()`) and not at import time. The same also holds for `WEBOTS_ROBOT_NAME`.

The original confusion came from rospy checking the parameter `ROS_NAMESPACE`at the time when it is imported and not at the time when a node is initialized.

##### SirLambda 02/26/2021 14:46:13
Hello, is it possible that I use different contact materials or textures for certain vertices in an IndexedFaceSet for example?

##### Olivier Michel [Cyberbotics] 02/26/2021 14:49:24
No: you will have to create two IndexedFaceSet nodes for that.

##### SirLambda 02/26/2021 14:52:43
Ok, thank you for the quick answer 🙂

##### Icy\_Flurry 02/26/2021 14:58:21
what units do the pioneer 3dx distance sensors use?

##### Darko Lukić [Cyberbotics] 02/26/2021 15:31:18
They use the following lookup table:

```
lookupTable [
  0 1024 0.01,
  5 0 0.01
]
```

See more about lookup tables here:

[https://cyberbotics.com/doc/reference/distancesensor#lookup-table](https://cyberbotics.com/doc/reference/distancesensor#lookup-table)

##### Icy\_Flurry 02/26/2021 15:33:39
thanks, ill look at that!


i dont quite understand how i would use that to convert the readings into mm/cm/m

##### Darko Lukić [Cyberbotics] 02/26/2021 17:20:54
The lookup table emulates the raw readings from the sensor


To get the actual distance from the particular lookup table you should do something like this:

```
distance_in_meters = 5 - (5 * value_from_sensor) / 1024
```

##### John520 02/26/2021 23:24:02
Hi guys, I am trying to import a harvester into Webots. I imported the green body and the wheels separately. And I follow the tutorial [https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot](https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot) to set up the HingeJoints between wheels and the body. But when I run the simulation, the harvester does not seem right. Could you please see the video and advise me? Thanks a lot!
> **Attachment**: [Kazam\_screencast\_00006.mp4](https://cdn.discordapp.com/attachments/565154703139405824/815001259899486269/Kazam_screencast_00006.mp4)

##### tbt24 02/27/2021 04:09:19
Hi, i was just wondering if anyone could tell me how to get a light sensor to differentiate between a green and a red spotlight

##### Krish 02/27/2021 06:14:10
Did you try performing image processing by a camera provided in the robot(if any) and using color detection algorithms, differentiate between colors.

##### Olivier Michel [Cyberbotics] 02/27/2021 07:57:14
Did you try to set a `colorFilter` to the `LigthSensor` node? [https://www.cyberbotics.com/doc/reference/lightsensor](https://www.cyberbotics.com/doc/reference/lightsensor)

##### tbt24 02/27/2021 11:30:48
Unfortunately I only have a light sensor available to me

##### Krish 02/27/2021 11:31:59
What does a light sensor actually do?

I have played with color sensors before, but not a light sensor.

##### tbt24 02/27/2021 11:35:30
The light sensor will just return the intensity of light. Then, when a colour filter is applied, ideally the light intensity will be multiplied by some value between 0 and 1 for RGB depending on the colour. However, the light sensor currently does not detect an 'LED' spotlight in my world

##### Olivier Michel [Cyberbotics] 02/27/2021 12:34:55
What about using two light sensors? One with a red filter and the other one with a green filter?

##### moeonethego 02/28/2021 12:23:52
If anybody can point me to the problem I've made a tracked robot , and added the left track with 7 wheels, defined the bounding Objects to include the wheels, but I get a strange shape as shown :
%figure
![strange-track-belt.JPG](https://cdn.discordapp.com/attachments/565154703139405824/815559899102576711/strange-track-belt.JPG)
%end


it seems like the problem with the animatedGeometry , or the definition of the bounding objects. waiting for your kind reply

## March

##### mayank.kishore 03/01/2021 01:52:56
Is it possible to base64 the camera.getImage() function?

##### Stefania Pedrazzi [Cyberbotics] 03/01/2021 06:47:29
Did you check that the wheels are all defined in the correct order?

As explained in the documentation the wheels have to be defined in clockwise order starting from the one with the smallest x-axis component:

[https://www.cyberbotics.com/doc/reference/track#geometries-animation](https://www.cyberbotics.com/doc/reference/track#geometries-animation)


`Camera.getImage()` returns the image raw data. In you controller program you can encode in base64 the raw data returned by `Camera.getImage()` using any image library specific to your programming language.

##### moeonethego 03/01/2021 12:01:56
thanks Stefania , this was the problem as you mentioned. , the order wasn't right
%figure
![track_belt_convex.png](https://cdn.discordapp.com/attachments/565154703139405824/815916767729156156/track_belt_convex.png)
%end

##### Krish 03/01/2021 12:04:30
The representation of the wheels looks awesome

##### moeonethego 03/01/2021 12:53:46
that's straight from the documentation link : [https://www.cyberbotics.com/doc/reference/track#geometries-animation](https://www.cyberbotics.com/doc/reference/track#geometries-animation)

##### Krish 03/01/2021 12:58:32
Oh 😂

##### h.sciascia 03/01/2021 14:53:44
Hello ! Where can I find the src/webots/engine/WbSimulationCluster.cpp in windows ? I would like to apply the updates for this issue [https://github.com/cyberbotics/webots/issues/2584](https://github.com/cyberbotics/webots/issues/2584)

##### John520 03/01/2021 14:58:13
Hi guys, I am trying to import a harvester into Webots. I imported the green body and the four black wheels separately. And I follow the tutorial [https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot](https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot) to set up the HingeJoints between wheels and the body. But when I run the simulation, the harvester does not seem right. Could you please see the short video and advise me? Thanks a lot!
> **Attachment**: [Kazam\_screencast\_00006.mp4](https://cdn.discordapp.com/attachments/565154703139405824/815961131314839582/Kazam_screencast_00006.mp4)

##### Stefania Pedrazzi [Cyberbotics] 03/01/2021 14:58:33
It is not enough to change the file, but you need to recompile Webots (clone the GitHub repo, modify the source file and compile Webots).

Otherwise you can download the latest nightly build of R2021a-rev1 that already contains the fix:

[https://github.com/cyberbotics/webots/releases/tag/nightly\_26\_2\_2021](https://github.com/cyberbotics/webots/releases/tag/nightly_26_2_2021)

##### h.sciascia 03/01/2021 14:59:11
Ok i will do that ! thanks !

##### Stefania Pedrazzi [Cyberbotics] 03/01/2021 15:05:00
From the video, it seems that the `JointParameter.anchor` may not be correctly aligned with the center of the wheel.

Usually, this value should correspond to the "endPoint" `Solid.translation`.

The joint axes can be visualized in the 3D scene by enabling the "Optional Rendering" > "Show Joint Axes" option  in the "View" menu. It may also be needed to increase the `WorldInfo.lineScale` value (for example to 2).

##### h.sciascia 03/01/2021 15:29:06
Not working

I have an URDF robot with multiple geometry for a joint and I go collide even if my physics plugin is intended to disable collisions

And now all my simulations collide 😢

##### John520 03/01/2021 15:33:07
`@Stefania Pedrazzi` Thank you very much for your help. I made the JointParameter.anchor and the Solid.translation the same. But I am seeing the Joint Axes now, I think I messed up the coordinates. I am trying to fix it and will let you know later. Thank you again!

##### Stefania Pedrazzi [Cyberbotics] 03/01/2021 15:40:51
Are you ignoring all the collisions or just some of them? Are you sure that the bodies to be ignored are detected correctly?

##### h.sciascia 03/01/2021 15:45:51
Hm I reload the world and the problem disapear , so works very well well !


Thanks a lot for this update !! 🥳

##### John520 03/01/2021 17:20:01
Hi `@Olivier Michel`, when I am running the simulation, the positions of axis of the wheels change. Could you please see the short video and advise me? Thank you very much.
> **Attachment**: [Kazam\_screencast\_00010.mp4](https://cdn.discordapp.com/attachments/565154703139405824/815996817367498812/Kazam_screencast_00010.mp4)

##### Bitbots\_Jasper [Moderator] 03/01/2021 17:38:10
I would like to start two controllers for two robots in the same process. Is there a way to do this or do they have to be separate processes? I currently get this error message: `Only one instance of the Robot class should be created`

##### Olivier Michel [Cyberbotics] 03/01/2021 18:02:12
This is unfortunately not possible.

##### Bitbots\_Jasper [Moderator] 03/01/2021 18:03:09
I was talking about external controllers


Thanks for your answer, we will do some inheritance magic then 😋

##### Vangiel 03/01/2021 18:30:13
Hello, anyone knows how can I access the lidar sensor in the tiago iron robot using python? Thanks.

##### Bitbots\_Jasper [Moderator] 03/01/2021 18:31:26
`robot.getDevice(lidar_sensor_name)`

##### Vangiel 03/01/2021 18:33:04
yes, I tried that, but because it is a proto it says that class Node doesn't have getDevice method. It seems that it doesn't recognise as a Robot node

##### Bitbots\_Jasper [Moderator] 03/01/2021 18:33:51
you need to do something like `robot = Robot()` in the beginning


if you are using a supervisor like `s = Supervisor()` then you need to call the function `getDevice`on the supervisor

##### Vangiel 03/01/2021 18:36:53
The thing is that I am using a robot supervisor node (without shape or anything) to control the whole simulation. So I get the robot node with the lidar sensor from the supervisor. For making it work I should call the getDevice in the controller of the robot with the lidar? Can't I do it from the supervisor robot?

##### Bitbots\_Jasper [Moderator] 03/01/2021 18:38:26
From your explanation it is not clear to me if you are using two different controllers or one controller with supervisor privileges

##### Vangiel 03/01/2021 18:39:01
I am using two controllers, one for the robot with the lidar and another one with supervisor privileges


And what I want is to get the lidar sensor from the one with supervisor privileges if possible

##### Bitbots\_Jasper [Moderator] 03/01/2021 18:39:48
can you post the code of the robot controller, private message is also ok if you dont want to share it publicly

##### Vangiel 03/01/2021 18:40:08
sure, give me a moment

##### Bitbots\_Jasper [Moderator] 03/01/2021 18:41:28
ah, ok, thats something different than I expected, I have an idea on how this could work but I need to test it, give me a moment.

##### Vangiel 03/01/2021 18:43:10
ok perfect, if you still need the code I can send it to you.

##### Bitbots\_Jasper [Moderator] 03/01/2021 18:49:36
I tested how I think it could have worked, but unfortunately it did not. I think devices are only accessible from the robots own controller and I am not aware that there is any method to do otherwise.


do you need to have the actual sensor data in the supervisor controller or can you do the required calculations in the robot controller and pass them to the supervisor controller using either a emitter receiver combo or the `customData` field?

##### Vangiel 03/01/2021 18:52:38
Ok thanks for the help. I can try to do the calculations in the robot controller. I know about the emitter receiver but what is the customData field?

##### Bitbots\_Jasper [Moderator] 03/01/2021 18:54:30
they explain it better than I could: [https://cyberbotics.com/doc/reference/robot#field-summary](https://cyberbotics.com/doc/reference/robot#field-summary)

##### Saud 03/01/2021 18:57:17
Hi, I want to use a lidar on the Mavic. but since it does not have one I have imported a Sick Lidar in the bodyslot but its quite big. Any Other suggestions ?

##### Bitbots\_Jasper [Moderator] 03/01/2021 18:58:36
have you checked here? [https://cyberbotics.com/doc/guide/lidar-sensors](https://cyberbotics.com/doc/guide/lidar-sensors)

##### Saud 03/01/2021 18:59:07
yes so i have decided to use SICK LMS 291

##### Vangiel 03/01/2021 18:59:45
Thank you very much

##### Saud 03/01/2021 19:00:13
Like i did not realise that this is huge. 😂
%figure
![Screenshot_2021-03-01_at_6.59.58_pm.png](https://cdn.discordapp.com/attachments/565154703139405824/816022032663314532/Screenshot_2021-03-01_at_6.59.58_pm.png)
%end


any way of scalling it down or something ?

##### Bitbots\_Jasper [Moderator] 03/01/2021 19:02:15
[https://github.com/cyberbotics/webots/blob/master/projects/devices/hokuyo/protos/HokuyoUtm30lx.proto](https://github.com/cyberbotics/webots/blob/master/projects/devices/hokuyo/protos/HokuyoUtm30lx.proto) is about 6x6x8.7cm large

##### Saud 03/01/2021 19:03:05
ok thanks let me have a look at that


i am not the greatest at webots so do excuse my wrongness if it is but i added the lidar and I am trying to use it in code. C++. I enabled the other devices using this but this one does not work. I am gonna guess because the others were already in mavic. But i have added the lidar to the bodySlot. Did it do this in a wrong way?
%figure
![Screenshot_2021-03-01_at_7.17.28_pm.png](https://cdn.discordapp.com/attachments/565154703139405824/816026397625679883/Screenshot_2021-03-01_at_7.17.28_pm.png)
%end

##### Bitbots\_Jasper [Moderator] 03/01/2021 19:28:06
for getting devices from the robot, you usually use the methods described here: [https://cyberbotics.com/doc/reference/robot?tab-language=c++#wb\_robot\_get\_device](https://cyberbotics.com/doc/reference/robot?tab-language=c++#wb_robot_get_device)


so you would have something like:

```Lidar *l;
l = robot->getLidar(lidar\_name)
```


or try:

`Lidar *l = new Lidar(lidar_name`


but i dont know if the second one works

##### Saud 03/01/2021 19:30:38
oh yes right ok let give that a go thanks

##### Welsh\_dragon64 03/01/2021 19:50:43
`@Bitbots_Jasper` `@Simon Steinmann` I tried implementing opencv on webots using python on the robotis darwin op2. No errors were presented on the code , although the issue i am facing is that it is not detecting the robotis camera but instead it detects my default laptop webcam
%figure
![problem.PNG](https://cdn.discordapp.com/attachments/565154703139405824/816034742113271849/problem.PNG)
%end



> **Attachment**: [opencv\_code.txt](https://cdn.discordapp.com/attachments/565154703139405824/816034897868881920/opencv_code.txt)


this is the attached code

##### Bitbots\_Jasper [Moderator] 03/01/2021 19:54:26
yes because you told it to do so in line 56 and 62

##### Welsh\_dragon64 03/01/2021 19:55:40
can you provide me your insight on this?

i am still new to webots and opencv

##### Bitbots\_Jasper [Moderator] 03/01/2021 19:55:45
you want to use camera.getImage() instead of that

##### Welsh\_dragon64 03/01/2021 19:56:02
is that reserved variable?

##### Bitbots\_Jasper [Moderator] 03/01/2021 20:01:45
you create your camera in line 40 and enable it in line 41, then you need to use the camera object in your while loop to get the image using `img = camera.getImage()`


the webots image is `BGRA`encoded though


I don't know what you mean by reserved variable.

##### Welsh\_dragon64 03/01/2021 20:09:29
I dont understand where to  place img = camera.getImage() in the while loop.

##### Bitbots\_Jasper [Moderator] 03/01/2021 20:10:09
instead of line 62

##### Welsh\_dragon64 03/01/2021 20:11:29
got this error in line 62, after i modified it.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/816039965699211294/unknown.png)
%end

##### Bitbots\_Jasper [Moderator] 03/01/2021 20:13:15
too many values to unpack means that you expect more return values than there are, you say you want 2 values, success and img but camera.getImage() only returns one value

##### Welsh\_dragon64 03/01/2021 20:15:51
I removed the success so it can return one value and now i got this new error.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/816041065470885908/unknown.png)
%end

##### Bitbots\_Jasper [Moderator] 03/01/2021 20:17:24
you need to somehow transform the array of pixel data into a opencv image but I dont know how to do that


but you can check the documentation of the webots to see what its format is [https://cyberbotics.com/doc/reference/camera?tab-language=python#wb\_camera\_get\_image](https://cyberbotics.com/doc/reference/camera?tab-language=python#wb_camera_get_image)


scroll a bit down to the python note from the link

##### Simon Steinmann [Moderator] 03/02/2021 11:25:45
`@Welsh_dragon64` 

`def getFrame():

    data = camera.getImage()

    frame = numpy.frombuffer(data, numpy.uint8).reshape((camera.getHeight(), camera.getWidth(), 4))

    return frame`



Try this code


this should be a valid cv2 frame / image

##### bingdong 03/02/2021 12:47:48
Hi, I was following the export tutorial for blender files on GitHub. Could someone let me know how I can access the RotationalMotor panel to move individual motors during simulation? Closed it and can't seem to find it now.
%figure
![Screenshot_20210302-181104__01.jpg](https://cdn.discordapp.com/attachments/565154703139405824/816290698084352010/Screenshot_20210302-181104__01.jpg)
%end

##### Darko Lukić [Cyberbotics] 03/02/2021 13:16:21
Double click on the robot


Make sure you are running the simulation and that your robot has a controller (it can be `void`).

##### bingdong 03/02/2021 14:19:39
`@Darko Lukić` Thanks a bunch!

##### Callum98 03/02/2021 15:32:22
Hey guys, I've got my robot with a camera that recognises the colour red but I want it to send a message, for example 'Red object ahead', when it comes into its vision. Is this possible?

##### Chernayaten 03/02/2021 15:46:01
You can use an emitter and a receiver 

[https://www.cyberbotics.com/doc/reference/emitter](https://www.cyberbotics.com/doc/reference/emitter)

[https://www.cyberbotics.com/doc/reference/receiver](https://www.cyberbotics.com/doc/reference/receiver)

##### Callum98 03/02/2021 16:03:37
Im looking at the sample camera robot and that does it with a camera but Im using java so having difficulty transferring it across

##### Chernayaten 03/02/2021 16:08:35
In the camera.wbt? That robot is not sending a message somewhere, it is simply printing it

##### Callum98 03/02/2021 16:09:37
Oh ok, I still don't see an emitter or receiver on the robot

##### Chernayaten 03/02/2021 16:11:21
The robot is not sending a message that can be received by someone else. It is simply printing it. What exactly are you having an issue with?

##### Callum98 03/02/2021 16:13:05
Yeah that's what I want. I will send a picture one sec but I want my robot to print out 'Red object detected' as there is a red object being outlined by the camera



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/816342416470573056/unknown.png)
%end


So since the robot detects that red object I want it to print out saying its found it

##### Chernayaten 03/02/2021 16:15:09
> if (redObjectDetected): print("Red object ahead") 

If you want to print it why don't you print it then?

##### Callum98 03/02/2021 16:15:56
Is that the only line of code needed? Sorry I dont really know how to code thats the problem 😦

##### Chernayaten 03/02/2021 16:17:27
My guess is that you're asking the wrong thing and you want to know how to detected the red object, not how to print a message saying you did

##### Callum98 03/02/2021 16:19:02
Its already detected that's why there's a box around it

##### Chernayaten 03/02/2021 16:19:27
The lines of code that do this in camera.wbt are 120-139. The wb\_camera\_image\_get\_red (and blue/green) can be found here and you can choose the function in your language: [https://cyberbotics.com/doc/reference/camera?tab-language=c#wb\_camera\_image\_get\_red](https://cyberbotics.com/doc/reference/camera?tab-language=c#wb_camera_image_get_red)

##### John520 03/02/2021 16:20:24
Hi guys, I am working on a HingeJoint but am having an issue that some parameter values change by themselves when the simulation starts.


Please take a look at the below two figures. The translation and rotation of the endPoint Solid change after the simulation starts. So the robot cannot go straight because of this. The first figure shows the original setting, while the second one shows the values after the simulation starts.
%figure
![Kazam_screenshot_00016.png](https://cdn.discordapp.com/attachments/565154703139405824/816344257194754048/Kazam_screenshot_00016.png)
%end



%figure
![Kazam_screenshot_00017.png](https://cdn.discordapp.com/attachments/565154703139405824/816344294322339880/Kazam_screenshot_00017.png)
%end

##### Callum98 03/02/2021 16:21:58
Pk Ill have a look thank you man

##### Chernayaten 03/02/2021 16:22:00
If the object is already detected (make sure it detects the color as well and not just the object), then you can use a boolean variable to print your message

##### Callum98 03/02/2021 16:22:34
Ok sure thank you

##### Simon Steinmann [Moderator] 03/02/2021 16:37:06
Take a look at this:

[https://cyberbotics.com/doc/reference/hingejoint?tab-language=python](https://cyberbotics.com/doc/reference/hingejoint?tab-language=python)


The axis and anchor is relative to the parent, so the translation and rotation of the endpoint Solid will change of course


Or at least, only rotating around the hingejoint axis

##### John520 03/02/2021 16:40:58
`@Simon Steinmann` Thanks a lot for your explanation. Is it possible to make the translation of the endPoint Solid not change?

##### Simon Steinmann [Moderator] 03/02/2021 16:41:50
it can be much easier to translate the parent to where you want it, and only translating the anchor and endPoint along the joint axis


in your case, only in x - direction

##### John520 03/02/2021 16:42:15
SInce other wheels have the same changes, the vehicle is not able to go straight.

##### Simon Steinmann [Moderator] 03/02/2021 16:44:30
You can add Transforms or Solid nodes as a parent of the hingejoint


position them  xyz, where they should be (would be your current anchor and translation). Then you set the anchor to 0 0 0, and the translation to 0 0 0

##### John520 03/02/2021 16:48:49
`@Simon Steinmann` I see, I am trying it. Thank you so much for your help! I really appreciate it.

##### Elizaveta\_Potemkina 03/02/2021 16:53:30
Hi guys,

I'm trying to select and place sensors on a robot that's meant to search for small blocks in an arena and go pick them up. Currently thinking of using ultrasonics to look for possible/clustered blocks, and then infrared readings while turning towards what it thinks is a block to actually get a bearing, because IR sensors seem to have a very small viewing angle at mid-range

My problem is, I don't understand why IR sensors are modelled to give an average distance reading with a gaussian weighting of the rays in Webots. Is that actually realistic to how an IR would work? I've looked at datasheets and I can't tell. I'm worried it would mess up the readings if blocks/a block and a wall are close in bearing but at different distances to the robot. Would it be reasonable to ignore the IR sensor presets on Webots and just model it to return the nearest distance with a small viewing angle?

##### Olivier Michel [Cyberbotics] 03/02/2021 17:36:33
No, IR sensors models in Webots are as realistic as possible. Because they reflect IR light, the return value depends on the amount of reflected light. If only one small area (represented by a few rays) reflects light, the measured value will be small, which is what you can observe in real IR sensors.


If you need the nearest value, you should set the type to "laser" or "sonar".

##### Elizaveta\_Potemkina 03/02/2021 17:39:05
I see. I've found a proto in the default Webots files for the exact IR I'd have access to IRL for this project. (Sharp GP2Y0A02YK0F). It's made by the manufacturer, and I can see that it's modeled as having only 1 ray. Should I just chalk that up to the manufacturer being optimistic about their own product?

##### Olivier Michel [Cyberbotics] 03/02/2021 17:40:48
Having only one ray is probably a performance / accuracy compromise.


If you need more accuracy, you should add more rays, but keep the aperture small enough to match the datasheet of the real sensor.

##### Welsh\_dragon64 03/02/2021 17:42:57
[https://cyberbotics.com/doc/guide/using-python#libraries](https://cyberbotics.com/doc/guide/using-python#libraries)



i cant seem to find the file at the location provided by the link above.



It say its located at WEBOTS\_HOME/projects/web/visual\_tracking  where the sample simulation uses opencv and numpy packages using python,but i cant find it anywhere. Can someone help me with this.

##### Elizaveta\_Potemkina 03/02/2021 17:43:38
That's one of the irritating things, they don't list an angle range on the datasheet at all. Best I've found is buried in a technical details file for a bunch of similar products giving something like a 5 degree viewing angle at the mid-high end of its range. I guess that's something to consider programming in

##### Pancha 03/02/2021 21:09:17
I have a general question with regards to Webots. We have a rather complex series of links that would be too difficult/time consuming to simulate. These links would extend out from a movable object in a straight line (essentially floating in air due to internal structures/tension). These links can extend and retract. My question then is;

  - Is it possible to make shapes that has ``boundingObject`` set but not ``physics`` (set to NULL)? This allows for the object to interact with others (collide) but still floats in the air. The moment I set ``physics`` to ``Physics`` it falls to the ground, but without ``physics`` set it does not collide.

  - Is it possible to make some shapes/solids collide (links colliding with wall for instance) whereas some solid/shapes does not (links not colliding with eachother / allows for them to be located inside each other)?

I really do hope there's a solution here 🙂

##### Bitbots\_Jasper [Moderator] 03/02/2021 21:11:20
you can disable gravity in webots using a supervisor controller if that does not interfere with the rest of your simulation

##### Pancha 03/02/2021 21:13:46
Hmm, that would interfere with the rest of the simulation as it depends on gravity. I guess I can do it the "easy way" and just refrain from using bounding objects on them and instead have some fictive distance sensors on the links emulating a "hit" on walls for instance 🙂

##### Bitbots\_Jasper [Moderator] 03/02/2021 21:18:06
I think the file has moved here: [https://github.com/cyberbotics/webots/blob/master/projects/samples/robotbenchmark/visual\_tracking/controllers/visual\_tracking/visual\_tracking.py](https://github.com/cyberbotics/webots/blob/master/projects/samples/robotbenchmark/visual_tracking/controllers/visual_tracking/visual_tracking.py)

##### Laojiang 03/03/2021 07:46:51
when I linked Pycharm with webots, I can't use numpy in Pycharm. That's the error image.
%figure
![16147575711.png](https://cdn.discordapp.com/attachments/565154703139405824/816577349012815952/16147575711.png)
%end


And If I don't use numpy, I can use Pycharm to control webots normally. Anybody could help me?

##### Tosidis 03/03/2021 08:54:48
Hey, I am using a display attached to my robot's camera to draw rectangles around objects detected by a object detector. Can I somehow delete the rectangles ,drawn on one frame , the next frame? Draw rectangle seems permanent on the display module.

##### Stefania Pedrazzi [Cyberbotics] 03/03/2021 08:58:30
Yes you can delete them by redrawing a transparent rectangle over the previously drawn objects.

Here is a sample code from the built-in Webots projects/samples/devices/worlds/display.wbt simulation:

[https://github.com/cyberbotics/webots/blob/master/projects/samples/devices/controllers/display/display.c#L111:L114](https://github.com/cyberbotics/webots/blob/master/projects/samples/devices/controllers/display/display.c#L111:L114)

##### Tosidis 03/03/2021 09:24:36
Thanks!

##### Darko Lukić [Cyberbotics] 03/03/2021 10:04:19
Check this out:

[https://cyberbotics.com/doc/guide/using-python#use-an-alternative-python-version](https://cyberbotics.com/doc/guide/using-python#use-an-alternative-python-version)

##### Tosidis 03/03/2021 10:06:33
I have a small bug, rectangle gets deleted, but when i draw the new one, it isn't a complete rectangle, some of the lines miss


example
%figure
![Screenshot_from_2021-03-03_12-06-55.png](https://cdn.discordapp.com/attachments/565154703139405824/816612751215689738/Screenshot_from_2021-03-03_12-06-55.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 03/03/2021 10:08:08
If you could provide a small example (here or opening an issue in GitHub [https://github.com/cyberbotics/webots/issues/new?assignees=&labels=&template=bug\_report.md](https://github.com/cyberbotics/webots/issues/new?assignees=&labels=&template=bug_report.md)) we will look at it

##### Tosidis 03/03/2021 10:16:57
I will try to report it later when I finish this. It happens due to resizing (display box inside world window is smaller than actual size and some lines won't render correctly. If I resize the window to the original size it shows the rectangles correctly)

##### Stefania Pedrazzi [Cyberbotics] 03/03/2021 10:19:32
Ok, thank you. Good to know that the Display functions are working correctly and it is just a resizing issue. By the way I'm not sure it could be fixed: when making an image smaller it is normal that some pixel information is missing.

##### Tosidis 03/03/2021 10:23:43
True, didn't think of it right away cause with openCV and ImageNew worked fine (much much slower though)

##### Welsh\_dragon64 03/03/2021 11:43:11
thnx

##### h.sciascia 03/03/2021 12:41:46
Hello everyone ! how to find the optimal thread count for the worldInfo settings ?

##### Stefania Pedrazzi [Cyberbotics] 03/03/2021 12:47:22
The optimal thread count strictly depends on the simulation. So the only way to find the best value is by manually changing the value and check the simulation speed.

But note that only the physics engine is using multi-threading, so you will only get some benefits by setting the thread count > 1 if you have multiple robots or dynamic objects that doesn't interact or if you have distinct groups of interacting objects. In the latter case, a good threads count could be the number of distinct groups of robots/objects.

##### h.sciascia 03/03/2021 12:48:19
Oh okok thanks

##### Simon Steinmann [Moderator] 03/03/2021 13:24:21
`@h.sciascia` in most circumstances 1 is the best

##### h.sciascia 03/03/2021 13:25:17
Ok :/ Because we are doing a lot of calculations and save with an reinforcement learning IA

##### Simon Steinmann [Moderator] 03/03/2021 13:26:11
you can do multiple instances of webots

##### h.sciascia 03/03/2021 13:26:30
for a single simulation?

##### Simon Steinmann [Moderator] 03/03/2021 13:26:36
yes


there is RL algorithms that you can train in parallel

##### h.sciascia 03/03/2021 13:27:12
how to do multiple instances ?

##### Simon Steinmann [Moderator] 03/03/2021 13:27:22
[https://drive.google.com/file/d/1C4ICDz30GjKs5PkuYxNW8\_mFCcxEJlPs/view?usp=sharing](https://drive.google.com/file/d/1C4ICDz30GjKs5PkuYxNW8_mFCcxEJlPs/view?usp=sharing)


`@Darko Lukić` `@h.sciascia` Darko, perhaps you can show him the new python api. This would make this much easier

##### h.sciascia 03/03/2021 13:29:01
Ok but the purpose is to train only one RL agent

##### Simon Steinmann [Moderator] 03/03/2021 13:29:20
what algorithm are you using?

##### h.sciascia 03/03/2021 13:29:26
td3



> **Attachment**: [Motion\_Planning\_of\_Robot\_Manipulators\_for\_a\_Smoother\_Path\_Using\_a\_Twin\_Delayed\_Deep\_Deterministic\_Po.pdf](https://cdn.discordapp.com/attachments/565154703139405824/816663760780918855/Motion_Planning_of_Robot_Manipulators_for_a_Smoother_Path_Using_a_Twin_Delayed_Deep_Deterministic_Po.pdf)

##### Simon Steinmann [Moderator] 03/03/2021 13:30:40
that one does not support multi Processing

##### h.sciascia 03/03/2021 13:31:30
So it's not possible


thanks

##### Darko Lukić [Cyberbotics] 03/03/2021 13:31:41
We did some initial testings with TD3 and you should first verify whether your algorithm is bottleneck. If TD3 takes a lot of time calculate time than Webots multiprocessing will not help.

##### h.sciascia 03/03/2021 13:32:04
lot of time yes

##### Simon Steinmann [Moderator] 03/03/2021 13:32:27
run a profiler and find out, what takes how long


if the simulation is only 5% of runtime, then there is little to optimize

##### Darko Lukić [Cyberbotics] 03/03/2021 13:33:12
How much time does the `step()` function from the OpenAI Gym takes?

##### h.sciascia 03/03/2021 13:34:06
we did not try it on OpenAI Gym Env because there is no environment that corresponds to our needs

##### Simon Steinmann [Moderator] 03/03/2021 13:34:46
what are you using? stable-baselines? or some custom solution?

##### h.sciascia 03/03/2021 13:35:26
i built the algo using pytorch

##### Simon Steinmann [Moderator] 03/03/2021 13:36:11
stable-baselines3 is based on pytorch. You might save yourself A LOT of work using that


[https://github.com/DLR-RM/stable-baselines3](https://github.com/DLR-RM/stable-baselines3)

##### Darko Lukić [Cyberbotics] 03/03/2021 13:36:24
You can easily integrate Webots with OpenAI Gym:

[https://www.cyberbotics.com/doc/guide/samples-howto?version=master#openai\_gym-wbt](https://www.cyberbotics.com/doc/guide/samples-howto?version=master#openai_gym-wbt)

##### h.sciascia 03/03/2021 13:36:46
thank you this is helpful

##### Simon Steinmann [Moderator] 03/03/2021 13:36:58
[https://github.com/DLR-RM/rl-baselines3-zoo](https://github.com/DLR-RM/rl-baselines3-zoo) this can be easily combined with stable-baselines3

##### Darko Lukić [Cyberbotics] 03/03/2021 13:38:01
How much time does your algorithm take to calculate an action from the given state?

##### h.sciascia 03/03/2021 13:38:06
my problem is mainly the huge time of simulation needed to train the agent


i didn't mesure it. it's a 2 hidden layers neural network (actor: 600x400)  so not much


the function that takes a lot of a time is the learning function


that updates the neural networks


along side another function that check for collision with obstacles present in the environment

##### Simon Steinmann [Moderator] 03/03/2021 13:40:02
maybe have a look at this paper: [https://www.groundai.com/project/towards-simplicity-in-deep-reinforcement-learning-streamlined-off-policy-learning/2](https://www.groundai.com/project/towards-simplicity-in-deep-reinforcement-learning-streamlined-off-policy-learning/2)


SAC could be a better choice, maybe.

##### h.sciascia 03/03/2021 13:40:48
okay thank you

##### Darko Lukić [Cyberbotics] 03/03/2021 13:41:30
Could you please benchmark all those functions so we better understand the bottleneck?

##### Simon Steinmann [Moderator] 03/03/2021 13:41:56
also, you should use HER together with TD3 or SAC, if you want to significantly increase sample efficiency

##### h.sciascia 03/03/2021 13:42:09
yeah i use HER

##### Simon Steinmann [Moderator] 03/03/2021 13:42:22
good 🙂

##### h.sciascia 03/03/2021 13:42:27
i will

##### Darko Lukić [Cyberbotics] 03/03/2021 14:32:29
`@h.sciascia` Please let us know the results. We are trying to optimize Webots for DRL, so your feedback will be very useful to us

##### h.sciascia 03/03/2021 15:36:14
Yes I will try to do this as fast as possible, lot of work in the company I cannot garantee you a date because I have others functions

##### Darko Lukić [Cyberbotics] 03/03/2021 15:48:37
Sure, no problem. Thank you!

##### Cyber Police Officer 03/03/2021 18:57:26
A quick question, is it possible to load a solid exported as .wbo into a solid node?


Forgot do specify: In a proto file. I know I can import in the GUI, but was wondering if it was possible in a proto file

##### Stefania Pedrazzi [Cyberbotics] 03/04/2021 07:20:16
There is no built-in function for this.  But you have different options:

1) turn the WBO node into a PROTO node so that it can easily be used in other PROTO files and nodes

2) import the WBO object from the Supervisor controller at the very beginning of the simulation

3) read and import the WBO file directly in the procedural PROTO file using Lua statements (a WBO object can be inserted as-is without any modifications to the file content other than removing the header line).

##### Master.L 03/04/2021 08:38:22
Hello! I have a question.

I am trying to create a simulation using webots and ros.

I am trying to control the joints by placing a motor on each joint of the webots robot by calling a service from ros.

In addition, I am trying to give values to a total of 3 joints by calling rosservice, but at the same time, a delay occurs because values are entered sequentially instead of entering each joint.

How can I send values at the same time?

##### Darko Lukić [Cyberbotics] 03/04/2021 08:41:36
If the `synchronization` field of your robot is set to `TRUE` then all service calls should be performed in the single timestep:

[https://cyberbotics.com/doc/reference/robot](https://cyberbotics.com/doc/reference/robot)


In addition, you should use the `--synchronize` argument:

[https://cyberbotics.com/doc/guide/using-ros](https://cyberbotics.com/doc/guide/using-ros)

to ensure that all the service calls are performed in the single timestep

##### Master.L 03/04/2021 09:19:58
`@Darko Lukić` Is there any example that I can refer to?


I currently use the controller as ros and create a ros package and run it externally.

##### Darko Lukić [Cyberbotics] 03/04/2021 09:25:23
Unfortunately, there is no example. You should configure the robot as in picture
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/816964535080321064/unknown.png)
%end


And you should get the `time_step` service exposed that you have to call at each step

##### Master.L 03/04/2021 09:48:00
`@Darko Lukić` Should I call time\_step before calling set\_position to each joint?

##### Darko Lukić [Cyberbotics] 03/04/2021 09:49:03
After. The `time_step` service calls the `wb_robot_step` function:

[https://cyberbotics.com/doc/reference/robot#wb\_robot\_step](https://cyberbotics.com/doc/reference/robot#wb_robot_step)

##### Master.L 03/04/2021 10:00:14
Thank you `@Darko Lukić` . I will try it and ask you again if there is a problem.

##### DNANA 03/04/2021 12:30:51
Does this work with Vex?

##### Cyber Police Officer 03/04/2021 15:21:39
Thank you! 1st one worked really well

##### Bitbots\_Jasper [Moderator] 03/04/2021 18:28:40
Here: [https://www.cyberbotics.com/doc/guide/tutorial-3-appearance#add-a-texture-to-the-ball](https://www.cyberbotics.com/doc/guide/tutorial-3-appearance#add-a-texture-to-the-ball) it says "Textures are mapped onto Geometry nodes according to predefined UV mapping functions described in the Reference Manual. A UV mapping function maps a 2D image representation to a 3D model."



I can not seem to find the documentation of which UV mapping is applied when `texCoordIndex` is empty in [https://cyberbotics.com/doc/reference/indexedfaceset](https://cyberbotics.com/doc/reference/indexedfaceset)


does somebody know where to find this information?

##### Olivier Michel [Cyberbotics] 03/04/2021 21:27:18
It's inherited from VRML97: [https://tecfa.unige.ch/guides/vrml/vrml97/spec/part1/nodesRef.html#IndexedFaceSet](https://tecfa.unige.ch/guides/vrml/vrml97/spec/part1/nodesRef.html#IndexedFaceSet)



%figure
![IFStexture.png](https://cdn.discordapp.com/attachments/565154703139405824/817146332371222579/IFStexture.png)
%end

##### Bitbots\_Jasper [Moderator] 03/05/2021 07:01:06
`@Olivier Michel` perfect, thanks for the info

##### h.sciascia 03/05/2021 09:03:37
Hello everyone ! 



How can I modify the initial coordinate system of a solid without modifying the position and orientation of this one ?



Thanks

##### Darko Lukić [Cyberbotics] 03/05/2021 09:13:30
`@h.sciascia` You have to change the solid so it matches the new coordinate system.

##### h.sciascia 03/05/2021 09:15:42
Okok thanks !

##### Darko Lukić [Cyberbotics] 03/05/2021 09:18:40
For example, if you are converting from NUE to ENU then you have rotate all solid's children and bounding objects by (-0.57, 0.57, 0.57, -2.09)

##### h.sciascia 03/05/2021 09:46:11
Thanks a lot ! 

It's to compare some results between my Denavit Hartenberg outside webots and the orientation/position given by Webots supervisor

##### Callum98 03/05/2021 14:02:02
Hi guys, I want my robot to turn for a specified amount of time before stopping, is there a command that tells the robot to perform an action for a set period of time?

##### Krish 03/05/2021 14:05:47
robot.step(time in ms)



Example - robot.step(1000)

Runs the commands above for 1 second.

##### Callum98 03/05/2021 14:12:43
Yeah that worked thank you man

##### Krish 03/05/2021 14:22:11
Welcome 😁

##### Yaksa 03/05/2021 20:17:26
hello everyone, I have a question how should I do tutorial 8 on windows, I've installed ROS packages, but these commands cannot use in windows
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/817491014948356166/unknown.png)
%end

##### Harris 03/06/2021 06:07:29
Hi, maybe a simple question, is there any way to change the center point of imported FBX models? Each models' center points are automatically set to the corner.

Also for the orientation? Is it possible to adjust them after importing models?

##### bingdong 03/06/2021 07:57:59
Hey, is there any way to use Blender keyframes for motion tracking of model animation and reusing it to create simulation on Webots?

##### moeonethego 03/06/2021 08:41:17
use the render options to view the rotation axis , follow this video , I had the same issue before but this developer made an extra step to adjust the rotation axis of the hingejoint : [https://www.youtube.com/watch?v=mX8p07a7K30](https://www.youtube.com/watch?v=mX8p07a7K30)

##### F\_Nadi 03/06/2021 08:42:44
Hi everyone, I have installed Webots R2021a and Matlab 2020a in Windows 10. Matlab is in my system path (as shown in the attached figure) but still Webots gives me this warning and cannot start my controller: 'WARNING: Unable to find the 'matlab' executable in the current PATH. Please check your matlab installation. It should be possible to launch matlab from a terminal by typing 'matlab'. It may be necessary to add the matlab bin directory to your PATH environment variable. More information about the matlab installation is available in Webots' User guide.' (I can start Matlab from a terminal by typing 'matlab' and Mingw-w64 compiler also has been installed using Add-Ons of Matlab software). would you please help me?



%figure
![Variables.PNG](https://cdn.discordapp.com/attachments/565154703139405824/817678740406009856/Variables.PNG)
%end

##### BeastFromTheEast 03/06/2021 11:45:03
Hi, I'm having an issue where my robot does not go in a straight line even though I have set both wheel velocities to the same speed and to my knowledge the wheels are parallel. Can anyone advice how I would look to eliminate such an issue>


?


Thanks

##### Krish 03/06/2021 12:48:18
It would happen due to external factors like friction and slipping.


To eliminate this maybe instill a PID control where you utilize the Gyro readings to make it go in a straight line.

##### benj3110 03/06/2021 18:07:33
Hi, I'm having alot of issues with solids going through eachother. Happens with robots and solids made in webots and with imported solids and I don't really know why. I've tried some of the suggestions on here [https://cyberbotics.com/doc/guide/modeling](https://cyberbotics.com/doc/guide/modeling) but nothing seems to work. Thanks

##### Bitbots\_Jasper [Moderator] 03/06/2021 18:15:48
is the problem that the center of the fbx file is not where you want it or is it imported incorrectly into webots? To test you could try to export it in a different format, .obj for example, and reload. You can add a Transform node around the import of the model to change translation or orientation (in axis angle convention).


do you have bounding objects for your solids? If not, add some. Otherwise, if the solids are part of the same robot you might need to enable self collision in the robot node

##### benj3110 03/06/2021 18:20:37
yea they are all bounded with physics and they are different solids.

##### Bitbots\_Jasper [Moderator] 03/06/2021 18:22:24
can you share your world file? pm is fine too

##### Darko Lukić [Cyberbotics] 03/08/2021 08:00:52
We fixed some issues regarding the MATLAB path (that may affect your problem as well):

[https://github.com/cyberbotics/webots/pull/2624](https://github.com/cyberbotics/webots/pull/2624)



Could you please try the Webots R2021a rev1 nightly build:

[https://github.com/cyberbotics/webots/releases](https://github.com/cyberbotics/webots/releases)

##### F\_Nadi 03/08/2021 08:07:06
Thanks a lot🙏 , I will do.


`@Darko Lukić`, my issue has been fixed. Thanks.

##### Vial 03/08/2021 18:05:28
Hi Webots community,

We are working on an automous robot competition where robots can't be connected to external PC

and usually runs on STM32 or dspic microcontrolers.

For debug purposes, we would like to display real sensor data/ point cloud/ pathfinding stuff from 

a C/C++ node connected to our robot to the Webots environement.

I think usually the robot community uses ROS/rviz to do so, but for differents internal reasons, we would like

to not use ROS and there must be a way to do everything from Webots anyway.

So I dive in the Supervisor programming and try to display everything we need but I really feel like I'm doing it wrong.

Is there a proper way to display real time point cloud/ path in webots ?



So far I'm working with a Shape/ geometry / PointSet and I feel like I can do something from this but it's quiet messy, 

it always feels like I have to find workarounds. 

I need to create a Node somewhere in the tree to store point cloud, clean the previous points in the coord/color fields if

some points already exist but I can't get the exact amount of points in

the pointSet from supervisor so I have to delete/recreate the node (there is no endpoints to get the the number of points you have in a field).

At the moment I still have a few bugs/undersirable behavior but it seems that I can at least generate a point cloud (All the points are white though, I need to reload the whole world to see the 

corect color).

Am I doing it right and it's just a matter of experience/knowledge with the supervisor API or is it beyond webots scope at the moment ?

Just to be clear, I'm not just criticize the API, I'm really enthousiastic about this project. I'm just trying to improve the development sharing my user experience.



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/818545029400100904/unknown.png)
%end

##### Simon Steinmann [Moderator] 03/08/2021 18:22:39
`@Vial`  think it is as simple as "ctrl + 8"
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/818549292040978512/unknown.png)
%end


[https://cyberbotics.com/doc/reference/lidar?tab-language=c++](https://cyberbotics.com/doc/reference/lidar?tab-language=c++) this might help too


oh, but the data is not from inside the simulation right, it's from the actual robot


ROS and Rviz really are simple solution for this. lightweight too, as it only displays and not simulates anything


`@Vial` I think you are overthinking your solution


this is my scene tree
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/818554789364760646/unknown.png)
%end


if you want to have the same color for all the points, just add a normal appearance node to the shape, and define the emissive color


Then you just set the Shape > pointSet > Coordinate to your pointcloud


you can give the Coordinate node a DEF "PointCloudCoord" in my screenshot
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/818555492405608468/unknown.png)
%end


then you can use the supervisor "getFromDef" functionality


i might be easier to import the whole thing as a string



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/818558629325701180/unknown.png)
%end


instead of line 36, you could generate a string form your pointcloud, with the structure of my screenshot


every point being separated by a comma


this might be faster, as only one call will be made to Webots, instead of one call per point


I'm not good with c++, so I dont know how to construct that string


`@Darko Lukić` is there a way to "setMF" a whole MF, without having to do it index  by index?


Something like 

`point.setMFVec3f( [[1, 1, 1], [2, 2, 2]] )`


or is that what 

`insertMFVec3f()` does?


or can it be only done via the importMFNodeFromString() function

##### Vial 03/08/2021 19:55:15
Hi `@Simon Steinmann` , 

Thanks a lot for your insights.

I should definitely try the string or file approach.

I just figured out that only some type of nodes can be exported to .wbo files.

I can try to save a template of the whole "shape pointCloud" as a .wbo file, and delete it/load it first and then update it as I can keep track on what I'm doing with this new node.

I'll try tomorow, thanks a lot for your time 🙂

##### Olivier Michel [Cyberbotics] 03/09/2021 07:11:46
You approach seems globally correct. If you need to reload the world to see the correct color, it might be a bug in Webots. Please test it with the latest Webots R2021a-rev1 nightly build where the bug might be already fixed. If not fixed, please open a bug report on GitHub with a simple source code example and we will look into it.

##### Darko Lukić [Cyberbotics] 03/09/2021 07:32:44
As far as I know you have to combine `wb_supervisor_field_set_mf_vec3f`, `wb_supervisor_field_insert_mf_vec3f`, and `wb_supervisor_field_remove_mf`.

##### Saud 03/09/2021 10:11:08
Hello, I am trying to add the lidar to the Mavic. I have added it on and enabled the lidar and the point cloud. But I am confused as to how to proceed further and start to get values and to get the points of the lidar to be seen in the simulation. I have enabled show lidar ray paths and point cloud. Also when how to store the values from the lidar?

##### Bitbots\_Jasper [Moderator] 03/09/2021 10:13:05
you need to write a controller for your robot, I recommend checking out the tutorials in the user manual [https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials)

##### Saud 03/09/2021 10:20:29
thank you which tutorial is best for lidar. I have gone through them


What am I doing wrong? quite lost and confused
%figure
![Screenshot_2021-03-09_at_11.12.47_am.png](https://cdn.discordapp.com/attachments/565154703139405824/818803666945179648/Screenshot_2021-03-09_at_11.12.47_am.png)
%end

##### Welsh\_dragon64 03/09/2021 12:33:32
I am trying to use <external> controller on the ROBOTIS OP 2 using Pycharm and got the following error. How do i set the content and path from webots to Pycharm???
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/818823824992763914/unknown.png)
%end

##### DDaniel [Cyberbotics] 03/09/2021 12:52:04
`@Saud` getPointCloud returns a const LidarPoint, not a LidarPoint

##### Saud 03/09/2021 12:57:10
ok great thanks i managed to change that. i am quite confused one how to represent the data which i get from the getPointCloud. Where do i save the array? do i need to use matlab to represent data?

##### DDaniel [Cyberbotics] 03/09/2021 12:59:29
To visualise it you can activate the rendering of the cloud: view > optional rendering >  show lidar point cloud

##### Darko Lukić [Cyberbotics] 03/09/2021 13:01:10
[https://cyberbotics.com/doc/guide/using-your-ide](https://cyberbotics.com/doc/guide/using-your-ide)

##### Deleted User 03/09/2021 14:20:25
Hello. Is it possible to integrate gamepad to webots?

##### Saud 03/09/2021 14:43:39
I have enabled this setting but all i get at the lines that you can see in the picture above

##### DDaniel [Cyberbotics] 03/09/2021 14:48:35
`@Saud` is it pointed in the right direction? (you can change the tilt angle of the lidar)

##### Saud 03/09/2021 14:54:20
I think that it is, or am I wrong?
%figure
![Screenshot_2021-03-09_at_2.53.47_pm.png](https://cdn.discordapp.com/attachments/565154703139405824/818859255246749726/Screenshot_2021-03-09_at_2.53.47_pm.png)
%end


I am also trying to get the point cloud but having trouble.
%figure
![Screenshot_2021-03-09_at_3.27.18_pm.png](https://cdn.discordapp.com/attachments/565154703139405824/818867588897570866/Screenshot_2021-03-09_at_3.27.18_pm.png)
%end

##### DDaniel [Cyberbotics] 03/09/2021 16:42:00
`@Saud` `const LidarPoint *points = lidar->getPointCloud()`. It depends what you're trying to do, have you taken a look at the 

sample world for lidar? Could help you understand how it works (file > open sample world > samples > devices > lidar)

##### Saud 03/09/2021 17:56:40
Yes I have essentially what I am trying to do is get lidar values to calculate the volume of the boxes

I have had a look at the sample but i dont know how its using the values and i cant see the lidar points in my simulation for some reason


ok i have managed to get values now but when i get the number of layers of the lidar it only returns 1 which means there are only 2 layers? quite confused why this is. i guess it explains the two lines. Currently i am using Hokuyo UTM-30LX. What i want is the drone to make a scan of the ground like a scanner would. Is this the correct lidar for that?

##### Simon Steinmann [Moderator] 03/09/2021 19:46:50
fieldOfView 4.71238

  verticalFieldOfView %{= 4.18879 / fields.resolution.value }%

  horizontalResolution IS resolution

  numberOfLayers 1

  spherical TRUE

  minRange 0.1

  maxRange 30

  noise IS noise


this is the settings of the UTM-30LX


it is one layer


it is a 2D-plane between the two lines you see

##### Uanuan 03/10/2021 01:35:20
Good day everyone, I'm new to webots, sorry if I'm asking a naive question. I'm wondering if it's possible to have an external program (let's say python) that modifies the attributes/properties of a robot while the simulation is running? For instance, let's say the hexapod is executing in webots, and I want it to "grow" an additional leg, or alter the shape or sensors on the bot dynamically. Could you give me some pointers as to how to start doing that? Thanks in advance.

##### SeanLuTW 03/10/2021 02:48:19
Hello, can I get object identity a connector currently connected to?

##### Olivier Michel [Cyberbotics] 03/10/2021 07:10:25
Yes, this is possible: you should use the supervisor API to add/remove nodes while the simulation is running.

##### Uanuan 03/10/2021 07:27:49
Sweet, thank you Olivier, appreciate it. I'll try that out 🙂

##### Harris 03/10/2021 09:52:42
Hi, I am trying to use connector function to simulate gripping hardware by a sucker. I already successfully connected the hardware with the gripper’s connector, and I try to move the end position, while the hardware doesn’t move together!(the lock state is still True) How can I fix this?



%figure
![image0.jpg](https://cdn.discordapp.com/attachments/565154703139405824/819145926610255882/image0.jpg)
%end



%figure
![image0.jpg](https://cdn.discordapp.com/attachments/565154703139405824/819146021317640192/image0.jpg)
%end


BTW, Thank for the previous help! Appreciate it.

##### AndyPandyO 03/10/2021 14:50:51
I am having trouble with my translation in endPoint solid not staying in place when i rotate an object. new to this, if anyone can help that would be great
> **Attachment**: [Recording\_1.mp4](https://cdn.discordapp.com/attachments/565154703139405824/819220768713080882/Recording_1.mp4)


I cannot get it to rotate around the axes point. any help? cheers

##### Stefania Pedrazzi [Cyberbotics] 03/10/2021 14:54:19
You should change the `JointParameters.anchor` value so that it matches the initial `endPoint Solid` translation.


You can also enable the visualization of the joint axes from the `View > Optional Rendering > Show Joint Axes` to make that it is correctly set.

##### AndyPandyO 03/10/2021 15:01:17
I did that, as you can see here. but it still resets the endPoint soldid translation
> **Attachment**: [Recording\_2.mp4](https://cdn.discordapp.com/attachments/565154703139405824/819223393395933204/Recording_2.mp4)

##### Stefania Pedrazzi [Cyberbotics] 03/10/2021 15:02:56
At position 0, both the endPoint translation and joint anchor should have the same value.

If the endPoint position is reset, then you can copy and paste the anchor value in the translation field.

##### AndyPandyO 03/10/2021 15:06:21
I did exactly that, same result. Thanks for the speedy response
> **Attachment**: [Recording\_3.mp4](https://cdn.discordapp.com/attachments/565154703139405824/819224668275736607/Recording_3.mp4)

##### Stefania Pedrazzi [Cyberbotics] 03/10/2021 15:12:08
Did you try to save and revert the simulation after fixing the `anchor` value?

##### AndyPandyO 03/10/2021 15:14:53
Yes! that's it, thanks so much

##### bellino 03/10/2021 16:01:22
Hello, I have a question about the charger and battery. I have a robot whose battery field is set to [51840, 51840, 2.4] and a charger whose battery field is set to the same values. When I move the robot to the charger the first field in the charger immediately goes to zero. I had a look at the example battery.wbt but besides the recharge speed and cpuConsumption (I am using the default value), I could not spot a difference. Any guess as to what I could be doing wrong?

##### Stefania Pedrazzi [Cyberbotics] 03/10/2021 16:24:05
Note that also in the example `battery.wbt` the first value of the `Charger.battery` field immediately goes to 0, but given that the recharge speed is higher, the robot gets recharged.


I'm reviewing the charging code to check if this is really the wanted behavior.

##### bellino 03/10/2021 16:56:18
higher then what? Do you mean the recharge time of the robot? I tried setting the recharge time of the robot to 4.8 but it didn't work, is this what you mean?

##### Stefania Pedrazzi [Cyberbotics] 03/10/2021 17:15:44
Yes. But you should try at least with 30 or similar (in the `battery.wbt` example).


By the way, the Charger behavior seems wrong and should be fixed ([https://github.com/cyberbotics/webots/issues/2843](https://github.com/cyberbotics/webots/issues/2843))

##### bellino 03/10/2021 17:39:41
thank you very much, it sort of works now with a higher recharge value!

##### cooolwhip14 03/10/2021 18:26:34
Hi , Im trying to make a python code to switch spotlights turn on when light goes below a certain value, however Im struggling as i cant see a way to call the spotlights name as the Spotlight device doesnt actually have a ‘name’ section to call. Anyone know a way around this?


light measured from a light sensor that is

##### Simon Steinmann [Moderator] 03/10/2021 19:21:04
give them a DEF


and use a supervisor controller

##### EmzG 03/10/2021 19:41:21
Hi, I’m trying to get my robot to go forward and avoids walls but stop and pick up a small object. I am struggling to get it to do both. If I have a small object it stops like it’s supposed to but for the wall it starts to turn then it stops. I’m sure it’s something so small but hoping I could get suggestions on how to allow the robot to do what it to do. I have uploaded 2 photos of my code to see if there are any mistakes. I am using C++. Thank you



%figure
![image0.jpg](https://cdn.discordapp.com/attachments/565154703139405824/819293980885778462/image0.jpg)
%end



%figure
![image0.jpg](https://cdn.discordapp.com/attachments/565154703139405824/819294033511186493/image0.jpg)
%end

##### Simon Steinmann [Moderator] 03/10/2021 19:50:35
`@EmzG` can you upload actual screenshots or the files themselves?


win + shift + s  to take a screenshot

##### EmzG 03/10/2021 19:55:45
this is a notpad file of my code
> **Attachment**: [controller\_help.txt](https://cdn.discordapp.com/attachments/565154703139405824/819297500069494834/controller_help.txt)

##### Simon Steinmann [Moderator] 03/10/2021 20:19:26
your code is a bit confusing. What is the exact behavior you want?


what is `obstacle_p` supposed to mean?


`avoidObstacleCounter` you decrease by 1 every step, but obstacle\_p gets set to 100, but never lowered back down

##### EmzG 03/10/2021 20:23:21
obstacle\_p is supposed to be another counter for the object sensor. this sensor is the one that will stop at small object.

##### Simon Steinmann [Moderator] 03/10/2021 20:23:35
you are using the same name for object and distance sensor


"ds"


try this
> **Attachment**: [obstacle.cpp](https://cdn.discordapp.com/attachments/565154703139405824/819305034164469790/obstacle.cpp)


changed ds to os for object sensors


also changed the indentation a bit. was hard to read

##### EmzG 03/10/2021 20:27:49
right is that what's the problem. cause i initially started with two file one which just had the two left and right sensors then the other file had the object sensor and both work fine independently. but combine them and wont work, i will give it a try. thank you

##### Simon Steinmann [Moderator] 03/10/2021 20:28:37
also, the way you have set it up, it will never move again, once it detected an object and stops

##### EmzG 03/10/2021 20:34:57
these errors seem to come up. that os is not declared
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/819307365581062174/unknown.png)
%end

##### Simon Steinmann [Moderator] 03/10/2021 20:35:23
yeah, you have to adjust your initialization


not sure why you do it in a loop anyways, when it is a single sensor


DistanceSensor *os;


or something


you can remove the loop


try this. I'm not very good with c++, but this might work
> **Attachment**: [obstacle.cpp](https://cdn.discordapp.com/attachments/565154703139405824/819308723154714704/obstacle.cpp)

##### EmzG 03/10/2021 20:49:07
thank you so so much that works perfect now. so all i needed to do was initialise it better . I was doing my nut trying to work out what was wrong the last few day and it was an oversite like that. you are amazing.

##### Simon Steinmann [Moderator] 03/10/2021 20:49:26
welcome to programming 😄


you're welcome

##### cooolwhip14 03/10/2021 21:01:03
What does the supervisor controller allow?

##### Simon Steinmann [Moderator] 03/10/2021 21:01:47
getting a handle to the light node and to manipulate it


Take a look at this project I did. It uses a supervisor controller to randomly change light sources
> **Attachment**: [domain\_randomization\_noTextures.zip](https://cdn.discordapp.com/attachments/565154703139405824/819315248133701655/domain_randomization_noTextures.zip)


just open the world and it should run automatically

##### cooolwhip14 03/10/2021 21:07:08
Great il check it out thanks

##### Harris 03/10/2021 21:52:53
Hi, I am trying to use connector function to simulate gripping hardware by a sucker. I already successfully connected the hardware with the gripper’s connector, and I try to move the end position, while the hardware doesn’t move together!(the lock state is still True) How can I fix this?



%figure
![image0.jpg](https://cdn.discordapp.com/attachments/565154703139405824/819327126552707162/image0.jpg)
%end

##### Simon Steinmann [Moderator] 03/10/2021 21:55:45
Can you try to rephrase that or show with a video? (webots has included video capture)


what do you want the hardware to do? what is it doing instead?

##### danny!! 03/10/2021 22:55:28
Hi kind of related to the above question. I am working with some arms kind of like square rods (solid object) connected to motors. In real life the motors would apply a constant force so that there is friction and the block is held tightly. How do I model this in webots? I used setForce and it just pinged the robot halfway across the arena

##### Master.L 03/11/2021 04:47:09
Hi! I have a question.

I am currently using a GPS sensor, but the values come in differently because the coordinate axis of the robot and the coordinate axis of the world are not the same. 

How can i align the axes??

##### Harris 03/11/2021 04:51:11

> **Attachment**: [pick\_up\_video1.mp4](https://cdn.discordapp.com/attachments/565154703139405824/819432244228915210/pick_up_video1.mp4)


I want the hardware to move with the gripper, and it is not moved

##### evoo 03/11/2021 09:00:53
I’m using Webots on MacOS. I created a controller using Python. When I run it I get ImportError: No module named pygame


Any idea on how to fix this?


Nvm I found the issue. I went to Preferences and then typed Python3 for “Python command”

##### Darko Lukić [Cyberbotics] 03/11/2021 09:13:44
The GPS sensor returns position in the global coordinate system. The coordinate system of your robot is not relevant. Can you provide more details about the issue?


[https://cyberbotics.com/doc/guide/moose#moose\_demo-wbt](https://cyberbotics.com/doc/guide/moose#moose_demo-wbt)

There is a Moose example. Press `A` to turn off the autopilot and use the arrow keys for teleoperation.

##### cindy 03/11/2021 10:10:07
Our code is written in C++ and it is crashing at different times every time we run it. It is a simple one robot controller for a uni project and we aren't entirely sure if it has anything to do with running on laptops that don't use the supported graphics cards (worth noting that if I close as many things as possible in Task Manager, the time of crash increases)? Would an indexing problem cause this sort of issue in multi-threading? And if so, how do I localise the issue if I can't use print statements?

##### Olivier Michel [Cyberbotics] 03/11/2021 10:11:13
See [https://cyberbotics.com/doc/guide/debugging-c-cpp-controllers](https://cyberbotics.com/doc/guide/debugging-c-cpp-controllers)

##### Elizaveta\_Potemkina 03/11/2021 11:32:59
When using Python for controller code, does .getLookupTable for sensors return a list with all the numbers in order just reading from one row to the next?

##### Lifebinder (tsampazk) 03/11/2021 12:05:26
Hello everyone, just a quick question. Is there a way to detect when self-collision is happening in a robot?

##### DDaniel [Cyberbotics] 03/11/2021 12:30:44
`@Lifebinder (tsampazk)` not programmatically but visually you can if you enable boundingObjects rendering,  when a collision occurs the color will change


view > optional rendering > show all bounding objects

##### Lifebinder (tsampazk) 03/11/2021 12:39:53
`@DDaniel` thank you very much for the quick answer!

##### alejanpa17 03/11/2021 13:49:19
Hey guys Im having troubles while displaying the lidar point cloud, I've already activate the View / Optional Rendering / Show Lidar Point Cloud but nothing shows up, is there something that I should have in consideration?


Also mention that I have the Lidar and Point Cloud enable in the robot window

##### Mohannad Kassar 03/11/2021 13:54:36
Hello, I have a question if a webots robot can be controller using a mobile application for example or a web page, any one can help ?

##### Darko Lukić [Cyberbotics] 03/11/2021 13:56:20
Yes, you can add a WebSocket server or web server in the robot controller


Which world are you trying to run? You may be affected with some bug resolved in R2021a rev1:

[https://github.com/cyberbotics/webots/releases](https://github.com/cyberbotics/webots/releases)

##### James Le Poidevin 03/11/2021 13:59:23
Hello, I have a problem when i create my own protos. The video below explains my probleme the best but I've been stuck for 2 days and can't figure it out.
> **Attachment**: [Kazam\_screencast\_00000.webm](https://cdn.discordapp.com/attachments/565154703139405824/819570203649507372/Kazam_screencast_00000.webm)


I add a proto. I save and then reload and they are gone. I tried on another PC and the proto isn't removed. If you have any idea why it would help alot thatnks

##### Darko Lukić [Cyberbotics] 03/11/2021 14:01:36
Are you getting any warnings in the console? What are the `LeftSide` and `RightSide` fields?

##### James Le Poidevin 03/11/2021 14:03:01
No Warnings, errors or even message
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/819571118162575410/unknown.png)
%end

##### Darko Lukić [Cyberbotics] 03/11/2021 14:05:59
Can you send the project to me, so I can test it

##### James Le Poidevin 03/11/2021 14:06:33
a ZIP ok ?



> **Attachment**: [RoverExomars\_GinestetJB.zip](https://cdn.discordapp.com/attachments/565154703139405824/819572690813583370/RoverExomars_GinestetJB.zip)


The world is TEST.wbt

##### Darko Lukić [Cyberbotics] 03/11/2021 14:11:58
It should look like this?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/819573372399779880/unknown.png)
%end

##### James Le Poidevin 03/11/2021 14:12:33
yes it's the front bogie of a rover

##### Darko Lukić [Cyberbotics] 03/11/2021 14:12:57
Just rename the `World` folder to `worlds`

##### James Le Poidevin 03/11/2021 14:14:29
Brilliant thanks ! (sorry for taking up your time)

##### Master.L 03/11/2021 17:39:21
Thank you `@Darko Lukić` 

I would like to know the position of the end-effector relative to the robot base through the sensor in the simulation. How can I do it?

##### Darko Lukić [Cyberbotics] 03/11/2021 17:53:05
Typical way of calculating end-point pose (in the real-world) is by putting an encoder in each joint of the arm. Then, you just calculate a forward kinematics given the encoder values and the robot model. You can do the same thing in the simulation.


If you are allowed to "cheat" then you can use:

[https://www.cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_get\_orientation](https://www.cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_get_orientation)

(see the `Python Example: ` subsection)


There is a Webots example using `ikpy`:

[https://cyberbotics.com/doc/guide/irb4600-40#inverse\_kinematics-wbt](https://cyberbotics.com/doc/guide/irb4600-40#inverse_kinematics-wbt)

You can change it to calculate the forward kinematics:

[https://ikpy.readthedocs.io/en/latest/chain.html#ikpy.chain.Chain.forward\_kinematics](https://ikpy.readthedocs.io/en/latest/chain.html#ikpy.chain.Chain.forward_kinematics)

##### Matzah 03/11/2021 18:06:00
hi ive been trying to use webots for school but the world doesnt play when i hit the play button, the timer just stays at 0. any ideas of what could be causing this?

##### Darko Lukić [Cyberbotics] 03/11/2021 18:06:22
Is there any warning in the console?

##### Simon Steinmann [Moderator] 03/11/2021 18:06:58
you need a running controller, which periodically calls robot.step()

##### Matzah 03/11/2021 18:08:43
i think i fixed it actually, nvm. thanks though!

##### Welsh\_dragon64 03/11/2021 19:48:54
thnx

##### YCL 03/12/2021 01:51:28
Hi Webots experts!


I met with a Webots problem recently. Could you please help me with it? Thank you so much!



My Webots can not work with my old files - which works well before. Recently I face up a problem that I use Webots 2019b and 2021a, which are different from what I used before. 



When I build the controller, the error shows "Makefile:74: D:/Users/Liu/AppData/Local/Programs/Webots /resources/Makefile.include: No such file or directory

make: *** No rule to make target 'D:/Users/Liu/AppData/Local/Programs/Webots /resources/Makefile.include'.  Stop." 



 

Then I did the following tries.



1. I checked my computer did include 'D:/Users/Liu/AppData/Local/Programs/Webots /resources/Makefile.include'



2. Then I delete my old "Makefile"  in the related controller file, and compile the controller in Webots, the Webots creates a new  "Makefile"  in the related controller file. The error above is solved but "WARNING: SL\_AGD\_Ini\_Star: The process crashed sometime after starting successfully.

WARNING: 'SL\_AGD\_Ini\_Star' controller crashed."



Thank you very much!



Have a nice day and keep healthy!


OK. I solved it now. Thank you so much!

##### Harris 03/12/2021 03:29:42
Hi, I am still struggling of this problem, can anyone plz help me to solve this connector problem? Thank you.

##### Darko Lukić [Cyberbotics] 03/12/2021 08:00:56
It is very time consuming to debug your simulation. Could you please create MRE:

[https://github.com/cyberbotics/webots/wiki/Webots-User-Support#how-to-ask-a-question](https://github.com/cyberbotics/webots/wiki/Webots-User-Support#how-to-ask-a-question)

##### Harris 03/12/2021 08:52:02
Here's my MRE project, press G to grip and press B to move. Thanks for your time if you can help!



> **Attachment**: [Webots\_MRE.7z](https://cdn.discordapp.com/attachments/565154703139405824/819855346155126795/Webots_MRE.7z)

##### Darko Lukić [Cyberbotics] 03/12/2021 08:55:12
Could you please make it simpler, just the world file is 10MB


> Never send the simulation you are working on. Your simulation is typically complex and it will cause confusion.

You should isolate the problem by creating a simulation from scratch

##### Harris 03/12/2021 09:11:40
Ok, sorry for that

##### pnaraltnsk 03/12/2021 12:35:58
Hi, I am trying to calculate the degree difference between my nao robot and a target point. My ultimate goal is to turn my robot in that direction and to move towards that direction. 

            angle = round(math.atan2(-(relativeX), relativeZ), 4)



            degrees = round(angle * (180 / math.pi), 4)    

This is how I calculate the degree between the robot and the target point. I subtract this from the robots angle and I rotate the robot according to result. But my robot doesn't stop rotating. I think that the problem is that robots position doesn't change so it keeps rotating and doesn't move forward. Do you have any ideas of how I can solve this problem.

##### Darko Lukić [Cyberbotics] 03/12/2021 13:16:47
You can check this example as a reference:

[https://github.com/cyberbotics/webots/blob/9a71190bb2084fec04246bd1008d9d311bef656f/projects/robots/clearpath/moose/controllers/moose\_path\_following/moose\_path\_following.c#L149-L190](https://github.com/cyberbotics/webots/blob/9a71190bb2084fec04246bd1008d9d311bef656f/projects/robots/clearpath/moose/controllers/moose_path_following/moose_path_following.c#L149-L190)



The robot in the example follows the provided checkpoints

##### Harris 03/12/2021 15:18:53
I just solve it, thank you for the reply and instruction!

##### Gregory Rasputin 03/12/2021 16:10:31
How can I access (set/get) a Device field? In particular I want to set the colorFilter field of the LightSensor

##### Darko Lukić [Cyberbotics] 03/12/2021 16:11:21
You can use the Supervisor node:

[https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_get\_field](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_get_field)

##### Gregory Rasputin 03/12/2021 16:12:27
Perfect. Never touched a supervisor before. Guess now's the time. Thanks 👍

##### Saud 03/12/2021 17:17:51
Hello, Is there a way to clear the console in webots using C++?

##### Master.L 03/12/2021 18:47:15
I am currently using ros controller for both robot and conveyor. However, there is no response when sending service to the conveyor's ros controller. Can you see why?

##### dA$H 03/12/2021 19:00:59
hello.

how can i get my robot translation in controller?

##### Simon Steinmann [Moderator] 03/12/2021 19:01:37
Either through a gps sensor, or you "cheat" by using a supervisor controller


[https://cyberbotics.com/doc/reference/supervisor](https://cyberbotics.com/doc/reference/supervisor)

##### dA$H 03/12/2021 19:05:59

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/820009748451688448/unknown.png)
%end

##### Simon Steinmann [Moderator] 03/12/2021 19:07:13
use the checkbox "Supervisor" in your scene tree


you robot needs to be a supervisor


you can also use the getPosition() function


and if you are using the field method, it is `Field.getSFVec3f()`

##### Zändi 03/13/2021 11:18:54
Hello together! 🙂

I would like to program an agv so that it travels between three stations and avoids obstacles. I would like to implement this with a laser scanner. can you help me with this? 

Do you have any ideas for the implementation, is there anything similar already?

##### enbakom ghebremichael 03/13/2021 18:57:46
I was simulate  a wall follower epuck hawever, the robot couldnt reach all rooms but it can follow the walls inside one room . could some one help me to improve my codes ?

##### Simon Steinmann [Moderator] 03/13/2021 23:45:24
You will have to give us more details than that. Perhaps show us in a video what it is doing and provide your controller code. Try to narrow down the issue yourself and try to ask a specific question 🙂

##### bingdong 03/14/2021 08:20:43
Hi, is there a simple example of how I can use the path stored in an external file to transform a solid/robot link in the simulation? I guess it would be using the supervisor property but not sure where to begin.

##### Zändi 03/14/2021 13:38:04
Hi ya 🙂

I am struggling with a code for an E-puck which should following a line. Can someone help me out with a code in C++?

##### Gregory Rasputin 03/14/2021 13:40:53
Have a look at my teaching materials especially section 5 [http://colin-price.wbs.uni.worc.ac.uk/Courses\_2020\_21/Comp2403/Workshops.htm](http://colin-price.wbs.uni.worc.ac.uk/Courses_2020_21/Comp2403/Workshops.htm)

##### Zändi 03/14/2021 13:45:27
oh thanks! Do you have more for Laserscanner or Infrared instead of using a camera?

##### Gregory Rasputin 03/14/2021 13:46:16
Nope. 😭 But camera is cool.

##### mayank.kishore 03/14/2021 21:54:41
Hi! Am working with the Moose robot and the right motors are not working? Says the velocity for each one is set at 10 but do not see any movement? Left motors work as expected, any ideas why?

##### Simon Steinmann [Moderator] 03/14/2021 22:54:11
`@mayank.kishore` perhaps you can give us more information. Share your project folder. Are you sure you spelled the motor names correctly?

##### mayank.kishore 03/15/2021 01:26:06
Identified the issue as not setting the inital position to infinity

##### bingdong 03/15/2021 07:28:33
Hi, is there a simple example of how I can use the path stored in an external file to transform a solid/robot link in the simulation? I guess it would be using the supervisor property but not sure where to begin.

##### Darko Lukić [Cyberbotics] 03/15/2021 07:38:06
Check out this code:

[https://github.com/lukicdarkoo/webots-example-visual-tracking/blob/master/controllers/ball\_supervisor/ball\_supervisor.py](https://github.com/lukicdarkoo/webots-example-visual-tracking/blob/master/controllers/ball_supervisor/ball_supervisor.py)



It calculates a position and sets the position of the ball. You can read the position from the external file instead of calculating it at each timestep.

##### Stefania Pedrazzi [Cyberbotics] 03/15/2021 07:58:05
Actually I was wrong. It is possible to clear the console, here is the documentation:

[https://cyberbotics.com/doc/guide/controller-programming#console-output](https://cyberbotics.com/doc/guide/controller-programming#console-output)

And here you can find an example: [https://github.com/cyberbotics/webots/blob/released/projects/samples/howto/controllers/console/console.c](https://github.com/cyberbotics/webots/blob/released/projects/samples/howto/controllers/console/console.c)

##### sanindu 03/15/2021 09:21:18
How can I solve this?
> **Attachment**: [screen-capture\_4\_3.webm](https://cdn.discordapp.com/attachments/565154703139405824/820949773908836362/screen-capture_4_3.webm)

##### Darko Lukić [Cyberbotics] 03/15/2021 09:22:17
Try starting Webots in a safe mode:

[https://cyberbotics.com/doc/guide/starting-webots#safe-mode](https://cyberbotics.com/doc/guide/starting-webots#safe-mode)

##### Rody El Hamod 03/15/2021 09:22:22
Hello, i tried today to create two processes in python


the simulation was blocked


any ideas?

##### Darko Lukić [Cyberbotics] 03/15/2021 09:24:06
Can you share your controller?

##### Rody El Hamod 03/15/2021 09:25:29

> **Attachment**: [robot\_controller.py](https://cdn.discordapp.com/attachments/565154703139405824/820950825538945024/robot_controller.py)

##### sanindu 03/15/2021 09:32:07
`@Darko Lukić` It's not working😕

##### Darko Lukić [Cyberbotics] 03/15/2021 09:33:44
This doesn't look related to Webots. Your `p1` process is accessing to the `robot` instance from the main process. You need something like queues to synchronize them and exchange data. Note that multiprocessing is different from multithreading.

##### sanindu 03/15/2021 09:33:54
same
> **Attachment**: [screen-capture\_5.webm](https://cdn.discordapp.com/attachments/565154703139405824/820952945683726397/screen-capture_5.webm)


while I did it
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/820953043067600896/unknown.png)
%end


`@Darko Lukić` What should I do now?

##### Rody El Hamod 03/15/2021 09:37:31
if you noticed i have added a print() before the processes start. But console does not display it


before the nested for loop

##### James Le Poidevin 03/15/2021 09:39:57
Hello, I'm having a problem with webots the last couple of days and I keep getting this error:



```Warning: QStandardPaths: XDG_RUNTIME_DIR not set, defaulting to '/tmp/runtime-root'
AL lib: (WW) alc_initconfig: Failed to initialize backend "pulse"
Warning: QProcess: Destroyed while process ("/usr/local/webots/resources/projects/controllers/void/void") is still running.
/usr/local/bin/webots: line 88: 25429 Bus error               (core dumped) "$webots_home/bin/webots-bin" "$@"
```

I'm running webots for a docker (so have to use --no-sandbox), I don't know if that could be the error.

And also the bus error changes but the rest of the error is the same.

##### sanindu 03/15/2021 09:42:19
<@&568329906048598039> please help me

##### Darko Lukić [Cyberbotics] 03/15/2021 09:44:33
That is strange


Can you try simplifying the code? It stopped working as soon as you introduced the multiprocessing?

##### sanindu 03/15/2021 09:46:24
`@Darko Lukić` It's not working...😟


😕

##### Rody El Hamod 03/15/2021 09:47:45
yes i have a working version with no processes. I tried to add the two processes in order to parallylise two functions and speed up the simulation

##### Stefania Pedrazzi [Cyberbotics] 03/15/2021 09:49:21
Did you try the official Docker image for Webots? Does it work?

Here you can find the environment setup we use:  [https://github.com/cyberbotics/webots-docker/blob/master/Dockerfile](https://github.com/cyberbotics/webots-docker/blob/master/Dockerfile)

##### Darko Lukić [Cyberbotics] 03/15/2021 09:50:40
[https://cyberbotics.com/doc/guide/starting-webots#windows](https://cyberbotics.com/doc/guide/starting-webots#windows)

What do you get when you run Webots with the `--sysinfo` flag?

##### James Le Poidevin 03/15/2021 09:52:09
thanks, I'm using my companies docker package for webots but try your version.

##### sanindu 03/15/2021 09:52:44
What's meant by --sysinfo flag?

##### Darko Lukić [Cyberbotics] 03/15/2021 09:58:26
The `print()` should be visible only after the `robot.step()` is called. Make sure `robot.step()` is called after the `print()` function.

##### sanindu 03/15/2021 09:59:41
What's meant by *--sysinfo* \_flag\_?


please help me...

##### DrVoodoo [Moderator] 03/15/2021 10:04:38
Do not use the everyone tag

##### sanindu 03/15/2021 10:05:13
ok

##### DrVoodoo [Moderator] 03/15/2021 10:06:19
Regarding the --sysinfo flag.


It returns various bits of information regarding the hardware and OS that you are currently running webots on


If you are unclear how to run programs from the command line in windows


[https://www.wikihow.com/Run-a-Program-on-Command-Prompt](https://www.wikihow.com/Run-a-Program-on-Command-Prompt)

##### bellino 03/15/2021 13:14:41
What is the unit of the gaussianWidth field in the DistanceSensor? Can't seem to find this in the docs

##### Stefania Pedrazzi [Cyberbotics] 03/15/2021 13:29:16
`gaussianWidth` doesn't have any unit, it is just a factor applied to the DistanceSensor aperture to compute the standard deviation of the gaussian distribution of the sensors ray weights.

##### bellino 03/15/2021 13:29:44
right that makes sense, thank you!

##### nelsondmmg 03/15/2021 13:48:05
Hi, I'm trying to put a pedestrian crossing in my environment but the strips do not show. How can I make the texture appear? Thanks
%figure
![pedStrips.png](https://cdn.discordapp.com/attachments/565154703139405824/821016913459019886/pedStrips.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 03/15/2021 13:56:06
Hi, it seems there is an issue with the `PedestrianCrossing` PNG texture (with transparency) that is not correctly displayed. Could you please open an issue on GitHub (

[https://github.com/cyberbotics/webots/issues/new?assignees=&labels=&template=bug\_report.md](https://github.com/cyberbotics/webots/issues/new?assignees=&labels=&template=bug_report.md)) ? We will then have a look at it as soon as possible.

##### nelsondmmg 03/15/2021 14:01:42
Done!

##### AleBurzio 03/15/2021 16:01:51
Hi everyone! I am trying to make a pothole detection system for an autonomous vehicle and I was wondering if there was a way to model the road directly to add the potholes (maybe applying an ElevationGrid to a road segment? But I'm not too sure about how to do that)

Does anyone have an idea?

##### Olivier Michel [Cyberbotics] 03/15/2021 16:20:33
You should probably create your own `RoadWithPothole.proto` file that could use a ElevationGrid or several Boxes to model the pothole...

##### AleBurzio 03/15/2021 16:42:03
Thank you very much! Is there any sample world that has a road proto that uses an elevation grid I could use as reference? I tried looking at the CH\_Vens world and I don't think the road is modeled that way there, but I might be looking in the wrong place

##### Zändi 03/15/2021 17:44:20
Hey 

Where can I translate a Phyton Code into C++?

##### Gregory Rasputin 03/15/2021 17:45:02
In your mind

##### Zändi 03/15/2021 17:45:39
Can you help me please? I am a beginner 😅


its a really short one

##### Gregory Rasputin 03/15/2021 17:46:54
"Short one" - explain - perhaps show the python code


I suggest you start by "reading code". Look at the samples provided in Webots and try to understand how the code works.

##### Zändi 03/15/2021 17:47:58
left\_ir\_value = left\_ir.getValue();

    right\_ir\_value = right\_ir.getValue(); 

    

    void print ("left: {} right: {}".format(left\_ir\_value, right\_ir\_value));

    left\_speed = max\_speed;

    right\_speed = max\_speed;

    

    if (left\_ir\_value > right\_ir\_value) and (6 < left\_ir\_value 15);

      print ("Go left");

      left\_speed = -max\_speed

    else (right\_ir\_value > left\_ir\_value) and (6 < left\_ir\_value 15);

      print("Go right");

      right\_speed = -max\_speed;

      

     left\_motor.setVelocity(left\_speed);

     right\_motor.setVelocity(right\_speed);

##### Gregory Rasputin 03/15/2021 17:49:04
OK I will share my teaching resources with you. These use C and start from the beginning. Here we go ... [http://colin-price.wbs.uni.worc.ac.uk/Courses\_2020\_21/Comp2403/Workshops.htm](http://colin-price.wbs.uni.worc.ac.uk/Courses_2020_21/Comp2403/Workshops.htm)

##### Zändi 03/15/2021 17:50:14
Alright, thank you Ill try

##### Gregory Rasputin 03/15/2021 17:50:34
Good luck my friend 👍

##### Master.L 03/16/2021 01:24:08
Hello! I have a question.

I want to control the speed of the end-effector of the robot arm, not the speed of each joint. What should I do?

##### Simon Steinmann [Moderator] 03/16/2021 01:24:33
You have to use inverse kinematics


what robot and what operating system are you on?

##### Master.L 03/16/2021 01:28:31
I am currently working on a project on Linux-ROS.

We are also using a delta robot, and we have solved the inverse kinematics of the delta robot to move the end-effector to the desired position.

When moving, I want to control the speed of the end-effector like a motion profile.


I want to include acceleration/deceleration in the speed.

##### Simon Steinmann [Moderator] 03/16/2021 01:35:38
use moveit with ROS for exact motion control


but velocity control for the end effector in cartesian coordinates is not easy. You can do it with jacobian matrices, but you'll get a


an error drift


I managed to do a clean ik velocity control, but it requires IKFast, so you would have to compile a solver for your robot

##### Master.L 03/16/2021 02:20:47
Currently, I have created my own motion profile.

And it calls the position to each joint by rosservice as much as the resolution of the motion profile.

The time\_step is progressed in units of 32mm/s, and the project proceeds by calling time\_step after the joint position rosservice call.

There are several problems with this.

1. For 32mm/s, I cannot check whether each joint has moved to the desired position.

2. Simulation does not move in the middle of execution.

Checking other examples, it was a method of giving the first position once and continuing the time\_step without giving a position every time\_step.

Am I wrong the way I do it?

##### Simon Steinmann [Moderator] 03/16/2021 02:26:21
use the trajectory follower example


publish a joint trajectory


[https://github.com/cyberbotics/webots/tree/master/projects/robots/universal\_robots/resources/ros\_package/ur\_e\_webots/src/ur\_e\_webots](https://github.com/cyberbotics/webots/tree/master/projects/robots/universal_robots/resources/ros_package/ur_e_webots/src/ur_e_webots)

##### Master.L 03/16/2021 02:55:01
Thank you!! but I want to control webots with ros without moveit. Could you please give me an example on how to do this?

##### Elizaveta\_Potemkina 03/16/2021 11:53:28
hey guys, is there a way in webots (using Python) to make the robot wait between executing one function and another? I have a rotational motor that needs to close a claw around an object before a sensor takes a reading. It's called using setPosition, and takes some time to actually get there, but the next line in the code is already to read the sensor, so it doesn't manage to close the claw. I believe time.sleep() wouldn't work, since I've tried to use that for some previous testing but it just stopped the controller for some reason, and I need setPosition to still be executing while the robot waits

##### Chernayaten 03/16/2021 12:02:32
I used a positionSensor to do the exact thing you're trying to do and had it loop through a while function until the diff was small enough. There might be a better way to do it though

##### Olivier Michel [Cyberbotics] 03/16/2021 12:33:41
Simply call the Robot.step() method with the number of milliseconds you need to wait.

##### Elizaveta\_Potemkina 03/16/2021 12:41:14
I ended up using a while loop to just iterate +=1 on an integer variable for a few timesteps. Will probably switch to robot.step(milliseconds) to be more efficient

##### h.sciascia 03/16/2021 15:38:47
Hello Everyone ! Can I deactivate completely the PID of my motors ?

##### Olivier Michel [Cyberbotics] 03/16/2021 15:49:24
Yes, if you control the motor in torque, Webots will not use it's built in PID controller.

##### madberg 03/16/2021 17:03:57
Is the world used for the example recognition segmentation image [0] available somewhere? 🙂



[0] [https://www.cyberbotics.com/doc/reference/recognition#](https://www.cyberbotics.com/doc/reference/recognition#)!

##### Darko Lukić [Cyberbotics] 03/16/2021 17:05:21
Yes, you can find it here:

[https://github.com/cyberbotics/webots/blob/master/projects/samples/devices/worlds/camera\_segmentation.wbt](https://github.com/cyberbotics/webots/blob/master/projects/samples/devices/worlds/camera_segmentation.wbt)

##### madberg 03/16/2021 17:05:50
Thanks!

##### reinaldobianchi 03/16/2021 19:21:44
Hello! Has anyone ever had a "Shader compilation fail" when opening Webots?



%figure
![Captura_de_Tela_2021-03-16_as_15.53.01.png](https://cdn.discordapp.com/attachments/565154703139405824/821463407995650079/Captura_de_Tela_2021-03-16_as_15.53.01.png)
%end


I Have just donwnloaded it to a new computer...


GPU ATI Radeon HD 4350...

##### Darko Lukić [Cyberbotics] 03/16/2021 19:39:41
[https://www.cyberbotics.com/doc/guide/verifying-your-graphics-driver-installation](https://www.cyberbotics.com/doc/guide/verifying-your-graphics-driver-installation)

##### reinaldobianchi 03/16/2021 19:41:09
But why a compiler error?

##### Olivier Michel [Cyberbotics] 03/16/2021 19:41:57
Because the graphics card is too old and doesn't support recent versions of OpenGL and corresponding shading language.

##### reinaldobianchi 03/16/2021 19:42:35
Usually I get the message "This computer does not support OpenGL 2..."


After installation.


This message was very criptic 🙂

##### Olivier Michel [Cyberbotics] 03/16/2021 19:43:27
AMD was not very good at being fully OpenGL compliant. This driver may in theory support OpenGL 3.3, but not in practice...

##### reinaldobianchi 03/16/2021 19:44:02
That's why there is no error after installation?


I am a teacher and this semester I am using Webots with 80+ students... I am facing errors I have never seen before 🙂

##### Simon Steinmann [Moderator] 03/16/2021 20:12:20
`@reinaldobianchi` Webots requires 3rd gen intel cpus. For more detail, you can check this link. [https://www.katsbits.com/codex/blender-opengl-compatibility/](https://www.katsbits.com/codex/blender-opengl-compatibility/) OpenGL 3.3 has to be supported

##### reinaldobianchi 03/16/2021 20:14:10
Yes... What happened is that I trusted that the GPU was OK when no message of error or warning appeared after I installed it. Usually old or incompatible GPUs are screened out really fast...


This one wasn't.

##### Simon Steinmann [Moderator] 03/16/2021 20:14:41
ATI Radeon HD 4350 should be supported. Perhaps the driver needs an update

##### reinaldobianchi 03/16/2021 20:15:46
Already did that... bus it is an old GPU...

##### Simon Steinmann [Moderator] 03/16/2021 20:17:10
OpenGL 3.3 is installed? and it still does not work? Are you sure no integrated GPU might be accidentally used? Especially on Linux dual graphics can be tricky at times


what CPU are you using?

##### reinaldobianchi 03/16/2021 20:17:26
Windows... 😦


OpenGL installed...


single gpu

##### Simon Steinmann [Moderator] 03/16/2021 20:18:00
which one?


CPU I mean

##### reinaldobianchi 03/16/2021 20:19:09
i5


not sure which one


have to check with the student

##### Simon Steinmann [Moderator] 03/16/2021 20:19:35
I bet it's 1st or 2nd gen intel

##### reinaldobianchi 03/16/2021 20:19:40
probably

##### Simon Steinmann [Moderator] 03/16/2021 20:19:52
it might try to use the integrated graphics, which dont support the openGL 3.3 version

##### reinaldobianchi 03/16/2021 20:19:55
old computer, as the one he was using fried...

##### Simon Steinmann [Moderator] 03/16/2021 20:20:07
you could try disabling integrated gpu in bios

##### reinaldobianchi 03/16/2021 20:21:19
I will do that also...

##### Simon Steinmann [Moderator] 03/16/2021 20:21:25
good luck!

##### reinaldobianchi 03/16/2021 20:21:27
Thanks!

##### Srivastav\_Udit 03/16/2021 22:21:06
Can someone please help me resolve this error: fatal error: webots/servo.h: No such file or directory

##### Simon Steinmann [Moderator] 03/16/2021 23:06:00
Is the library present? servo.h is not a standard library of webots


I take it you based your controller code on some other? Make sure you provide all include libraries

##### Srivastav\_Udit 03/16/2021 23:17:58
Yes, is there anyway I can get the servo.h file?

##### Simon Steinmann [Moderator] 03/16/2021 23:19:01
where did you get your code from? That code will surely have it

##### Srivastav\_Udit 03/16/2021 23:24:14
I'll have another look, thanks!

##### avinoob 03/17/2021 04:37:40
Hi could some one guide me on how to run my virtual environment in python controller.

##### xjs 03/17/2021 06:34:19
hi,I have encountered some troubles. After I joined the tactile sensor in the simulation of quadruped robot, I want to realize the robot to move backward for one movement cycle and then move forward after touching the obstacle. But I don't know how to write a program to realize it. Can anyone give me some suggestions? Thank you👀

##### Tahir [Moderator] 03/17/2021 14:35:27
any suggestions on how to do test driven development with Webots ?


running Webots from script in background testing everything and shuting it down


thanks for any advice in advance

##### Darko Lukić [Cyberbotics] 03/17/2021 15:28:10
Something like this?

[https://www.youtube.com/watch?v=CDOrTKQAOqs](https://www.youtube.com/watch?v=CDOrTKQAOqs)


We are doing it in our CI:

[https://github.com/cyberbotics/webots/tree/master/tests](https://github.com/cyberbotics/webots/tree/master/tests)



You can add a Supervisor controller to your simulation which will perform the tests and close the simulation in the end.

##### James Le Poidevin 03/17/2021 15:47:05
Hello, Do you know what could be causing this error ?

```
AL lib: (EE) ALCplaybackAlsa_open: Could not open playback device 'default': Device or resource busy
ALSA lib pcm_dmix.c:1052:(snd_pcm_dmix_open) unable to open slave
```

##### Darko Lukić [Cyberbotics] 03/17/2021 16:02:32
Webots has problem accessing the playback device. It is not critical for the simulation, only the sound will not work.

##### James Le Poidevin 03/17/2021 16:02:52
Ok thanks

##### Tahir [Moderator] 03/18/2021 07:51:57
Great thanks I was looking for something similar

##### Götz 03/18/2021 16:10:34
Good afternoon! Can a Supervisor control all robots in a world? I'm trying to have one supervisor that opens an API port to control a small fleet of robots....


I've been through the docs, to me it looks like no. But I might very well have overlooked something...

##### Darko Lukić [Cyberbotics] 03/18/2021 17:36:50
You cannot control motors or read sensors from the other robots, but you can change everything in the scene tree (for example translation/rotation of the robots). You can use emitter/receiver to send and receive data from the other robots:

[https://www.cyberbotics.com/doc/reference/emitter](https://www.cyberbotics.com/doc/reference/emitter)

[https://www.cyberbotics.com/doc/reference/receiver](https://www.cyberbotics.com/doc/reference/receiver)

##### Götz 03/18/2021 17:37:53
Okay, thanks a lot!


And another one... 😉

I'm running a world with 10 robots which are controlled via REST API calls on an AWS GPU instance. I'd like to give access to the visualization of the simulation to students who **control their robots** but **not to the full Webots** interface. I've tried the streaming server and it gives me very bad performance and nearly eats up all of my laptops CPU. Using a VNC connection to the AWS instance works, but then I have to stream the simulation to the students using web conferencing software.

Is there any other way to get the simulation from a headless server to viewers?

I've read in the news section a new web UI is coming up, would this help in my case?

##### Darko Lukić [Cyberbotics] 03/18/2021 17:49:16
In general, the web streaming should produce a minimal overhead. Did you use `x3d` or `mjpeg` streaming?

[https://cyberbotics.com/doc/guide/web-streaming](https://cyberbotics.com/doc/guide/web-streaming)

##### Götz 03/18/2021 17:49:29
X3D


Didn't get mjpeg to work...


I was a bit surprised myself about the performance.

##### Darko Lukić [Cyberbotics] 03/18/2021 17:55:22
Just to check, everything is fine on the server, but your laptop has performance issues just by showing the simulation in a web browser?

##### Götz 03/18/2021 17:59:17
Yup, this is how it looks to me. So the client should not get a lot of load during streaming?

##### Darko Lukić [Cyberbotics] 03/18/2021 18:00:33
No, I don't think we have ever had similar problem

##### Götz 03/18/2021 18:00:37
I better test with another client then...

##### Darko Lukić [Cyberbotics] 03/18/2021 18:01:06
What about this simulation:

[https://lukicdarkoo.github.io/webots-example-visual-tracking/master/#visual\_tracking](https://lukicdarkoo.github.io/webots-example-visual-tracking/master/#visual_tracking)

Do you experience performance issues with that simulation?


Maybe your simulation is too complex


If the issue persists please report it here:

[https://github.com/cyberbotics/webots/issues/new?assignees=&labels=&template=bug\_report.md](https://github.com/cyberbotics/webots/issues/new?assignees=&labels=&template=bug_report.md)

so we can investigate it.

##### Götz 03/18/2021 18:03:03
I took the Student Robotics Competition world as a base because it looks quite basic. I‘m afk right now, let me check. Would be really cool to get this working.


Thanks, will get back!

##### Hoplite 03/18/2021 23:09:56
Hello everyone, I have just begun using Webots and I have a question about transferring code to a robot. I read the documentation on the website, but it wasn't very clear. My group and I are trying to build a small drone and a robotic arm. We are probably going to use an arduino and I was wondering how can I cross-compile a webots controller code into arduino code (I think it is c++)


Also, does it matter if our webots controller is in C++ or Python ??

##### Olivier Michel [Cyberbotics] 03/19/2021 08:00:48
It all depends if your arduino board support C++ and Python. The idea is to write a wrapper between the Webots API and your hardware.


See [https://cyberbotics.com/doc/guide/transfer-to-your-own-robot](https://cyberbotics.com/doc/guide/transfer-to-your-own-robot)

##### James Le Poidevin 03/19/2021 08:24:51
Hello, I just wanted to know if there was anything integrated in webots that allows me to projected the FOV of a camera onto the floor or will i have to pas directly by openGL ?(For example for a NavCams or LocCams on a rover)

##### Olivier Michel [Cyberbotics] 03/19/2021 08:45:03
Not sure to understand your question... Do you want to see the intersection of the camera frustum with the floor?


Did you check the View menu, Optional Rendering, Show Camera Frustum?

##### James Le Poidevin 03/19/2021 09:02:12
Something like this
%figure
![FOV-airborne-image.png](https://cdn.discordapp.com/attachments/565154703139405824/822394517906653234/FOV-airborne-image.png)
%end


But only show like a square on the floor showing what the robot can see

##### Olivier Michel [Cyberbotics] 03/19/2021 09:19:07
Well, I believe this is not directly possible, but the View menu, Optional Rendering, Show Camera Frustum option will let you see what the robot see at the top of the frustum.

##### James Le Poidevin 03/19/2021 09:21:40
Ok thank you i'll look at OpenGL to see if it's possible . The Camera Frustum is good but the Frustum doesn't touch the ground so it won't work for what i need.


Hello again, I have a question about implementing a bogie (like what is on the sojourner), I've go the exact same hinge joint config as on the sojourner but mine messes up sometime. It often works fine but on some occasions it's as if the joint goes solid. (cf video)
> **Attachment**: [Bogie-test.mp4](https://cdn.discordapp.com/attachments/565154703139405824/822455077415026708/Bogie-test.mp4)

##### Olivier Michel [Cyberbotics] 03/19/2021 13:14:19
It looks like the left and right joints are connected, isn't it?

##### James Le Poidevin 03/19/2021 13:15:29
Yes it's 1 solid from the wheel on the left to the one on the right (of the bogie)

##### Olivier Michel [Cyberbotics] 03/19/2021 13:21:12
Then depending on the constraint on the other side, the behavior you get may be normal, isn't it?


If you would like to see the same behavior as the sojouner, you should probably decouple the left and right hand sides.

##### James Le Poidevin 03/19/2021 13:30:45
I have the same config as the Sojouner 1 Solid -> hinge joint(with no devices) -> 1 solid connected to the two wheels
%figure
![Capture_decran_du_2021-03-19_14-29-23.png](https://cdn.discordapp.com/attachments/565154703139405824/822462101540175902/Capture_decran_du_2021-03-19_14-29-23.png)
%end


The image doesn't show much but it explains what i am trying to reproduce.

##### Götz 03/19/2021 17:46:03
Just a heads up: making sure the AWS GPU is actually used by Webots 😁 and using mjpeg for the stream helped immensely, thanks.

##### pnaraltnsk 03/19/2021 18:20:32
Hi, I have implemented the code and the calculations worked fine but when I try to rotate my robot, since the angle values changes unexpectedly, it keeps turning. For example, angle value changes from 44 to -110. Is there a way to fix this? What am I doing wrong? Thank you in advance.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/822535027258687528/unknown.png)
%end

##### Seedow 03/19/2021 22:07:47
Hey everyone!

I am trying to put together a robot with a distance sensor on it, for navigation and mapping purposes. I ran into a problem when I try to import 3D models for walls (tryed .stl and .obj files, exported from Autodesk Inventor or Blender).

As you can see on the attached video, the distance sensor returns the distance to the furthermost wall of the mesh that's in range. I also tried it with the Webots box shape, and it's working properly. The mesh is in the shape node of a Solid, and the bounding object of the solid is referenced to the shape. I tried with laser and generic sensor types.

Did anybody have any similar experience importing meshes? Any idea how can I overcome it?
> **Attachment**: [robot\_8.mp4](https://cdn.discordapp.com/attachments/565154703139405824/822592215268065300/robot_8.mp4)

##### Icy\_Flurry 03/19/2021 22:15:31
how do i add an image texture to a Wall or Solid?


i dont see the pbrappearance option

##### DDaniel [Cyberbotics] 03/19/2021 22:18:43
<@288787843042770945> This problem was solved already for indexedFaceSets, are you using the latest version of webots? Otherwise what might be causing the issue is if the normals of the faces aren't pointing in the right direction (they should point out of the solid, not in. You can visualize them from the optionalrendering under View menu)


`@Icy_Flurry` did you add a Shape children to the solid?

##### Icy\_Flurry 03/19/2021 22:20:21
no, ill try that


that did it, thanks!


wait, i cant see the image


do i need to do something to make it visible?

##### DDaniel [Cyberbotics] 03/19/2021 22:38:16
`@Icy_Flurry` set the image in the baseColorMap and define a geometry for the object in the Shape

##### Icy\_Flurry 03/19/2021 22:40:55
what should the geometry be defined as?

##### DDaniel [Cyberbotics] 03/19/2021 22:43:58
Depends what you're trying to do, for a wall you can use a Box

##### Icy\_Flurry 03/19/2021 22:44:33
that worked, thanks

##### Seedow 03/20/2021 00:30:10
This is how it looks like after I render the normals. Is this how they should look like? I've just updated to the R2021a version this week.
%figure
![robot_1.png](https://cdn.discordapp.com/attachments/565154703139405824/822628047491694622/robot_1.png)
%end


I installed the newest R2021b nightly build, it seems like it solved the problem. I was affraid to use nightly, because it might be unstable. How bad is it actually?

##### Iris230 03/20/2021 09:11:44
hello guys, I'm wondering that how can I fix this error? I just want to follow this step in the document
%figure
![error.png](https://cdn.discordapp.com/attachments/565154703139405824/822759303177568296/error.png)
%end



%figure
![step.png](https://cdn.discordapp.com/attachments/565154703139405824/822759358566891530/step.png)
%end

##### Darko Lukić [Cyberbotics] 03/20/2021 10:36:16
We have dozens of automated tests running in our CI, testing most of the API functions, physics, parser, rendering... We make sure they all pass before the nightly builds. Stable releases are tested manually in addition to the automated tests.



A few important things to note about the nightly builds:

- There are revisions and new versions. In the revisions, we are trying not to break the API, but only to fix the bugs. In the new versions, we are introducing new features and changes.

- A documentation targeting the new revision is available with the `?version=master` suffix, while documentation targeting the new version is available with the `?version=develop` suffix. For example [https://www.cyberbotics.com/doc/reference/supervisor?version=develop](https://www.cyberbotics.com/doc/reference/supervisor?version=develop)

- By using the nightly builds you are helping us by reporting bugs before the release.

- If you need a specific bug fixed and you need to be on the safe side it is a good idea to pick a nightly build that works well with your simulation and stick with it.

##### danielvicente 03/20/2021 11:31:31
Hi guys, I'm trying to run webots with an extern controller but I'm getting this error "ModuleNotFoundError: No module named 'controller'".  Did I do anything wrong in the env variables?
%figure
![Screenshot_2021-03-20_at_11.30.54.png](https://cdn.discordapp.com/attachments/565154703139405824/822794481522311188/Screenshot_2021-03-20_at_11.30.54.png)
%end

##### Darko Lukić [Cyberbotics] 03/20/2021 11:51:35
Delete the `add` prefix

##### danielvicente 03/20/2021 11:53:55
Still get the same error

##### Darko Lukić [Cyberbotics] 03/20/2021 11:54:54
What happens if you `ls ${PYTHONPATH}`?

##### danielvicente 03/20/2021 11:55:43

%figure
![Screenshot_2021-03-20_at_11.55.31.png](https://cdn.discordapp.com/attachments/565154703139405824/822800570942750780/Screenshot_2021-03-20_at_11.55.31.png)
%end


It's working now with python 3.7 instead of 3.8

##### bellino 03/20/2021 15:24:44
hello, I'm trying to run a webots simulation in my browser but it doesn't seem to work. I follow the quickstart tutorial here: [https://www.cyberbotics.com/doc/guide/web-simulation](https://www.cyberbotics.com/doc/guide/web-simulation) and after I start the servers using the given command it tells me: "session server created on port x; simulation server created on port x+1". So far so good, but when I try to connect to ws://localhost:5924 (5924 is where the simulation server is running) I get the error shown in the image. When I click on "here" it just resets but next time I try to connect the same thing happens.  Am I missing a step?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/822853172468908082/unknown.png)
%end

##### ms71 03/20/2021 15:45:39
Hi, i am playing around with "Tutorial-E-puck-for-ROS2-Beginners", is there a feature, that the robot is just driving around and discover the full area.

##### aslisevil 03/20/2021 16:33:15
Hello, I am trying to calculate the angle between a target point and Nao robot. I tried to do vectoral angle calculation but it does not give correct result. Any ideas of how to do this? Sorry if the question seems a bit odd I am new to webots.

##### Simon Steinmann [Moderator] 03/20/2021 21:50:17
`@aslisevil` I assume you only need the 2D angle (Thetha) around the vertical axis


for that use atan2()


in python it should be:

from math import atan2

theta = atan2(x1-x2, y1 - y2)

##### xjs 03/21/2021 06:59:15
Hello, what kind of objects can be identified in camera recognition, except in the example; how to add the recognition objects that users like

##### watchdogs132 03/21/2021 08:23:04
Hello . I wanted to know , in the coulomb friction field , do I have to set the coefficient of friction between material 1 and 2 ?

##### Max\_K 03/21/2021 13:55:04
Hello. I am working on a Gripper mounted on a robot. Does someone know why my fingers of the Gripper does not collide with any objects while i move the robot. And why do my fingers disconnect from my model and hanging around at the original position when i moved the robot and let an object collide with the fingers?

##### Simon Steinmann [Moderator] 03/21/2021 17:32:06
`@Max_K` which gripper on which robot? how did you attach it?


`@watchdogs132` under world info > contact properties, you can specify the friction between two materials. All these settings only apply to interactions between those two materials

##### Max\_K 03/21/2021 17:39:31
Thanks for the reply. It's an self-desigend 3F-Gripper which grep objects from above on a robotino3. I exported the gripper from blender as .dae and created some hingejoints with the finger as EndPoint. I solved it meanwhile by applying first a Transform->Connector->and then Hingejoint.

Now I am trying to pick a puck or small cylinder, but it just falls to the ground. Is there something like a glue?

##### Simon Steinmann [Moderator] 03/21/2021 17:41:44
you only need a connector node if you want to connect and disconnect something dynamically


I'd recommend you open the ure.wbt sample world and right click on one of the robots and convert it to base nodes


and have a look how they are structured. it's the universal robotics arms with a 3f gripper attached to them

##### Max\_K 03/21/2021 17:45:21
ok there is an connector node for picking with 3f gripper the bottles?

##### Simon Steinmann [Moderator] 03/21/2021 17:45:45
no, you should use the fingers and the physics interactions

##### aslisevil 03/21/2021 18:59:54
thank you for the feedback but unfortunately it did not work. To calculate I took robot's rot vec and target's translation vector. Results were incoherent.

##### Simon Steinmann [Moderator] 03/21/2021 19:33:35
`@aslisevil` the x and y values in my example were the both the translation values


[https://en.wikipedia.org/wiki/Inverse\_trigonometric\_functions](https://en.wikipedia.org/wiki/Inverse_trigonometric_functions) take a look at that picture on the right side.


so you should use the atan2 function, and put in the distances in the 2 horizontal axis


that should be the values from the translation vector of both


Of course this only works, if both are in the same coordinate system (world)


`@aslisevil` what programming language do you use?

##### Zändi 03/21/2021 20:33:36
hi guys 🙂

how can I average 3 output values from the robot E-Puck and use this for further processing?

##### Simon Steinmann [Moderator] 03/21/2021 20:37:02
`@Zändi` just get all 3 values, and then do with those values whatever you want.  The simplest would be` v_avg = (v1 + v2 + v3)/3`

##### Zändi 03/21/2021 20:41:40
thanks, how can I declare v\_avg? 

my basic is: 

double left\_ir\_value = left\_ir->getValue();

double right\_ir\_value = right\_ir->getValue();



i want the average of the left\_ir\_value (infrared sensors for line following)


double left\_ir\_avg;

 left\_ir\_avg = (left\_ir\_value + left\_ir\_value + left\_ir\_value)/3;

##### Simon Steinmann [Moderator] 03/21/2021 20:44:56
that is programming basics

##### Zändi 03/21/2021 20:45:00
does this make sense?

##### Simon Steinmann [Moderator] 03/21/2021 20:45:07
i'm not good with c and c++

##### Zändi 03/21/2021 20:45:13
I am a beginner, sorry

##### Simon Steinmann [Moderator] 03/21/2021 20:45:50
with Python I can help, but this is not webots specific. Just google online how to set and manipulate variables


and if you are a total beginner, you might want to research a bit, on what language to learn


Unless you are not required to do C or C++ (which one do you use?), I would recommend Python, as it is much easier to understand and more user friendly

##### Zändi 03/21/2021 20:47:30
Thank you. I must use c++

##### Simon Steinmann [Moderator] 03/21/2021 20:50:44
then use google a lot. stack-overflow etc. are your friend

##### Zändi 03/21/2021 20:51:47
alright, thanks

##### pnaraltnsk 03/21/2021 22:30:04
Hi, I am trying to rotate my robot around Z-axis using this code.  I followed exactly the same steps on this resource [https://matthew-brett.github.io/transforms3d/reference/transforms3d.axangles.html](https://matthew-brett.github.io/transforms3d/reference/transforms3d.axangles.html) .According to this everything should work fine but Nao robot doesn't rotate as it should. When I write rotate(-180), it rotates 143 degrees or rotate(10) it rotates 154 degrees. Any ideas of what is wrong in the code? Thank you in advance.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/823322601974661170/unknown.png)
%end

##### Simon Steinmann [Moderator] 03/21/2021 22:37:13
all those calculations use Radians


you have to convert your rot\_degree into radians


rot\_radian = rot\_degree * pi / 180

##### pnaraltnsk 03/21/2021 23:09:03
Thank you, it worked!

##### aslisevil 03/21/2021 23:32:46
thank you for the response. Currently, I am able to calculate the angle correctly. When Nao starts with 1.57 rad angle, it rotates correctly towards the target point. But when it starts with a different angle it fails to rotate correctly. 'target\_angle' is the angle that is calculated with atan2. I tried to subtract target\_angle from nao\_angle which is the angle in axis-angle. But it didn't work. 😦
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/823338380301566052/unknown.png)
%end

##### Simon Steinmann [Moderator] 03/21/2021 23:41:54
is the nao angle correct? This code can only work, if the axis in the rotation field points straight up


and why do you take -y?


just use the relativeY in atan2


and remove the `if target_angle` The angle given is still in radians


I'd really recommend using radians only. Degrees screw things up


pi = 180 degrees. 2*pi is a full circle.

##### Stefania Pedrazzi [Cyberbotics] 03/22/2021 06:59:17
Any `Solid` object whose `recognitionColors` field is set can be recognized from the `Camera.Recognition` functionality:

[https://www.cyberbotics.com/doc/reference/recognition](https://www.cyberbotics.com/doc/reference/recognition)

Some of the distributed objects (cars, street furniture, etc.) already have the `recognitionColors` field set, but if not you can set it yourself just by modifying the PROTO. Here is an example:

[https://github.com/cyberbotics/webots/blob/master/projects/objects/balls/protos/SoccerBall.proto#L47:L49](https://github.com/cyberbotics/webots/blob/master/projects/objects/balls/protos/SoccerBall.proto#L47:L49)


Which version of Webots are you using? Do you get any errors in the Webots console?

Did you try first to stream directly from Webots without starting a session server and a simulation server, i.e. as explained in the `Web Streaming`([https://www.cyberbotics.com/doc/guide/web-streaming](https://www.cyberbotics.com/doc/guide/web-streaming)) section?

##### Timcampy 03/22/2021 08:18:12
Hi, i am new to webots and i need help. The controller that i want to compile, does not want to compile. The error is makefile: no such file in directory. Does anyone know how to edit the INCLUDE variable from the Makefile to link to the correct directory?

##### Stefania Pedrazzi [Cyberbotics] 03/22/2021 09:53:41
Hi, you can find some documentation in the header of `Makefile.include` that should be linked in your controller `Makefile`:

[https://github.com/cyberbotics/webots/blob/released/resources/Makefile.include](https://github.com/cyberbotics/webots/blob/released/resources/Makefile.include)

If your header and source files are not located in the root folder of the controller, then you have to set the `INCLUDE` and `C_SOURCES`/`CXX_SOURCES` so that they contain your files.

Here there is an example:

[https://github.com/cyberbotics/webots/blob/released/projects/robots/kuka/youbot/libraries/youbot\_control/Makefile](https://github.com/cyberbotics/webots/blob/released/projects/robots/kuka/youbot/libraries/youbot_control/Makefile)

##### Timcampy 03/22/2021 10:31:28
`@Stefania Pedrazzi` Thx. I already solved it.

##### Götz 03/22/2021 11:33:23
Hi all, I'm trying to build some kind of very simple battle arena. The robots are controlled via a REST API. What I've build so far looks like this:
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/823519729435803668/unknown.png)
%end


It is actually way to slow, running on a AWS GPU instance. And I get the "...your world may be to complex..."


I've stripped down as far as I can, no shadows and so on. I've tried working the basic timestep, but below 16 nothing is moving anymore, works with 24 or 32... but really slow.


Is there any hope this can be done in the first place or is having 12 robot instances just too much? It works pretty nicely with, say, 4 robots. But even than I get the "too complex" warning...


Any hints would be highly appreciated!



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/823520716213321818/unknown.png)
%end

##### Timcampy 03/22/2021 11:50:51
Hi, i was writing some basic code for my homework on robot. I encounter weird issue for coding part. I write same if block and it execute differently. I don't know how to fix the issue. First If execute, reset the distance equal to 0 and match how i want it to be. However, other second if block that has same exact code execute differently.


the second code didn't execute properly and freeze
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/823524610346975262/unknown.png)
%end


I comment block one if block to execute other one

##### Darko Lukić [Cyberbotics] 03/22/2021 13:13:25
Your simulation seems to be relatively simple. In general, it should work just fine. Can you try deleting the nodes to identify what causes it work slowly? Try putting the `controller` field to `void` to isolate the impact of the controllers on the performance.

##### DDaniel [Cyberbotics] 03/22/2021 13:16:15
`@Timcampy` they're not the same, the second one has `oldX = Y` and the first one `oldY = Y`. Also it freezes because of the infinite loop, you don't change `Theta` so `newTheta` is always bigger than it.

##### Götz 03/22/2021 13:30:53
Hi Darko, do you mean reducing the number of robots? Or other non-robot nodes?

##### Darko Lukić [Cyberbotics] 03/22/2021 13:33:06
Both. Start by choosing a simple controller just move them around and then deleting the nodes, one by one, and comparing the performances

##### yanan 03/22/2021 14:36:14
Hi,  does anyone know whether we can get the mask of an image directly (from a camera on the drone) about one object (a car for example)?  thanks
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/823565743484174366/unknown.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 03/22/2021 14:42:08
Hi, you should have a look at the `Camera.recognition` node:

[https://www.cyberbotics.com/doc/reference/recognition](https://www.cyberbotics.com/doc/reference/recognition)

This node has a segmentation functionality to automatically generate this kind of images. If you just need to get the car, then you could unset the `Solid.recognitionColors` of the other nodes.

Here is an example: [https://github.com/cyberbotics/webots/blob/master/projects/samples/devices/worlds/camera\_segmentation.wbt](https://github.com/cyberbotics/webots/blob/master/projects/samples/devices/worlds/camera_segmentation.wbt)

##### yanan 03/22/2021 14:43:02
thank you very much, Stefania, I'll have a look

##### Dorteel 03/22/2021 15:54:08
Hi! Is there an easy way to get the position and orientation of the Tool Center Point of a manipulator (UR-3) relative to the base frame? Tried using Transforms, but got stuck with it

##### danielvicente 03/22/2021 15:57:58
Hello, is the coordinate system in meters?

##### Dorteel 03/22/2021 16:01:52
Yes it is. And I saw that the transform node has a Position tab where I can see the relative position to the frame, but I'm not sure how to access it from a Supervisor node

##### Darko Lukić [Cyberbotics] 03/22/2021 16:05:25
Hello `@Dorteel`, you have to calculate it:

[https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_get\_orientation](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_get_orientation)

search for `Python Example`.



From the Supervisor node, you can either get the absolute pose of the a node or relative pose to the first parent node. Anything else you have to calculate.

##### Dorteel 03/22/2021 16:07:02
Thank you very much `@Darko Lukić` !

##### Götz 03/22/2021 17:15:36
Sigh... seems to be my controller, sim runs fine with a very basic collision avoidance Python controller even with 12 robots. My controller is actually a Python Tornado app going into a loop, listening for API commands and executing them. Finding the issue might be well beyond my dev capabilities, are there any things that come to your mind I could look for?  😔

##### Simon Steinmann [Moderator] 03/22/2021 17:58:25
`@Götz` "world may be too complex" warning has nothing to do with the GPU. The limit will be the CPU. And it is usually limited by the speed of a single core. This makes servers less suited, as they have many weak cores.


It would be good to find out where the bottleneck actually lies.


Is every controller running in its own process (12 controllers for 12 robots)? or is it one controller running 12 robots?


I can take a look if you want

##### Götz 03/22/2021 18:00:38
Every robot is starting an instance of the python controller with customData=<portnumber>, the controller is then opening this port for the robot.

##### Simon Steinmann [Moderator] 03/22/2021 18:01:34
send me your project and I can take a look

##### Götz 03/22/2021 18:01:52
The controller is working fine but is still more like a proof of concept. Just saying so it's not getting to embarrassing when you look at it. 😉

##### Simon Steinmann [Moderator] 03/22/2021 18:02:35
😄 dont worry about it. Code usually looks messy until it's done


you can pm me, if you dont want to share publically

##### Götz 03/22/2021 18:02:51
[https://github.com/cloud-native-robotz-hackathon/robot-sim/blob/master/simulations/crash\_derby\_sim/controllers/srobo\_remote\_tornado/srobo\_remote\_tornado.py](https://github.com/cloud-native-robotz-hackathon/robot-sim/blob/master/simulations/crash_derby_sim/controllers/srobo_remote_tornado/srobo_remote_tornado.py)


Hey, I'm working for the worlds largest Open Source software company, sharing is caring... 😂


And BTW I'd be more then happy to have one controller controlling the robots... but I couldn't figure it out and a supervisor can't do it... if I'm correct...

##### Simon Steinmann [Moderator] 03/22/2021 18:05:56
it would probably be slower too

##### watchdogs132 03/22/2021 18:19:13
Thank you for your reply .  I want to know do I have to set the friction coefficient between the surfaces in that field ?. In the sample world 'track' the coulomb friction is set at 300. However when it comes to coefficient of friction usually the values are like 0.5 0.6 something like that .

##### Simon Steinmann [Moderator] 03/22/2021 18:37:09
I assume the friction was set to something that high as a workaround, to ensure non slip. If nothing is defined, default values are used


`@watchdogs132` [https://cyberbotics.com/doc/reference/contactproperties?tab-language=python](https://cyberbotics.com/doc/reference/contactproperties?tab-language=python) these are the default values

##### aslisevil 03/22/2021 20:55:12
thank you it worked.

##### Srivastav\_Udit 03/23/2021 02:39:12
What does the controlPID field do for rotational motors? I'm not seeing any changes when I'm setting different values for PID..


I'm essentially trying to draw a square but it isn't drawing a perfect one because of real world constraints, so I assumed changing values in the controlPID field would improve the robot's performance but that's not the case..

##### Simon Steinmann [Moderator] 03/24/2021 02:23:44
`@Srivastav_Udit` Can you show us the issue in more detail? And you might want to read up on the documentation: [https://cyberbotics.com/doc/reference/motor](https://cyberbotics.com/doc/reference/motor)


but in general you should not have to change the PID values.  Webot's default method of controlling motors is very stable. Perhaps you can show a video of you project

##### main 03/24/2021 03:15:21
In contact properties, what goes in ‘material 1 ‘ and ‘material 2’ the PBR appearance name, or the name of the solids that are making contact?

##### Simon Steinmann [Moderator] 03/24/2021 03:15:51
`@main` solids have a field called "contact material"

##### main 03/24/2021 03:18:28
Oh wow thank you

##### Master.L 03/24/2021 08:43:28
Hello I have a question.

I am trying to implement a vacuum gripper in webots.

I know you can use connectors to make vacuum grippers.

You need to add connectors for both the gripper and the box.

If so, should I add the connector to the child after converting the box to convert to base node?

##### rschilli 03/24/2021 08:44:17
Hey guys. In robocup we are using simspark in 3d-simulation league and we have a DRL framework with stable-baseline/ppo to learn kicks/walking etc. For the virtual competition this year I've implemented a external controller and connect it to our DRL, so I am able to learn in webots as well. Now I want to paralize  this process by using multiple simulation instances, but here's the problem: either the docker approach nor native (starting multiple webots instances and define WEBOTS\_PID to select instance\_X) is working. With docker I can't connect to the running instance inside the container (extern controller running locally/outside), with native webots instances it seems that the WEBOTS\_PID is ignored/not parsed correctly, cause it is always connecting to the same instance. I've seen some posts of solutions but no documentation is working. Would be nice if someone could help me? Further details: DRL implemented in Python, Controller/Behavior logic in Java (Client-Server communication between both)

##### Olivier Michel [Cyberbotics] 03/24/2021 08:46:06
Although it kinda works, the Connector is not the ideal way to model vacuum grippers. See details on this issue: [https://github.com/cyberbotics/webots/issues/2889](https://github.com/cyberbotics/webots/issues/2889)

##### Master.L 03/24/2021 09:02:09
Thank you `@Olivier Michel`. But I don't understand the content of the link you sent me.

Can you make it in a different way now than the way I said?

##### Olivier Michel [Cyberbotics] 03/24/2021 09:08:10
We should actually modify Webots to extend the connector node, so that it can connect with any solid node and works like a suction cup. Using a Connector node as it is currently implement doesn't work well to simulate a vacuum gripper.


I believe the modification of Webots is not very difficult, but is not on our roadmap yet. Any contribution (including from you) is welcome.

##### Darko Lukić [Cyberbotics] 03/24/2021 09:13:57
You can set a  `WEBOTS_TMPDIR` for each Webots instance and controller pair. See about the `WEBOTS_TMPDIR` here:

[https://cyberbotics.com/doc/guide/running-extern-robot-controllers#running-extern-robot-controller-with-the-snap-version-of-webots](https://cyberbotics.com/doc/guide/running-extern-robot-controllers#running-extern-robot-controller-with-the-snap-version-of-webots)

##### Srivastav\_Udit 03/24/2021 14:34:26
I'm having the same issue as this video [https://youtu.be/CDOrTKQAOqs](https://youtu.be/CDOrTKQAOqs) from 11:40 to 11:55

##### rschilli 03/24/2021 14:45:43
thanks for the hint, this brings me a bit closer to the solution. now I'm able to start multiple webot instances and connect to it with separated extern java controllers. But with the current approach (start ThreadPool and each thread create a new controller to specific webot instance) it crashes, since only 1 robot class should be created. So may I have to look for other controller-creation logic 🙂


sry, wrong answer link, thanks goes to `@Darko Lukić`  😉

##### Darko Lukić [Cyberbotics] 03/24/2021 15:20:12
In Python, we use the multiprocessing library to create multiple controllers. Probably there is something similar in Java

##### John520 03/24/2021 16:28:53
Hi guys, I'd like to connect the part on the left to the three links on the right. But I have no idea how to do it. Could you give me any advice? It is a closed-loop mechanism. By the way, I read through this tutorial:[https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot](https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot) and I think it is an open-loop system. I tried it myself and had no problem.
%figure
![Kazam_screenshot_00013.png](https://cdn.discordapp.com/attachments/565154703139405824/824318870398566410/Kazam_screenshot_00013.png)
%end


The local coordinate of the part on the left is not on the joints.

##### Darko Lukić [Cyberbotics] 03/24/2021 17:07:47
Check the Stewart Platform example, it is the closed-loop mechanism:

[https://cyberbotics.com/doc/guide/samples-demos#stewart\_platform-wbt](https://cyberbotics.com/doc/guide/samples-demos#stewart_platform-wbt)

##### John520 03/24/2021 17:19:32
Thank you, `@Darko Lukić`. I checked this model before. Do I need to move the part on the left to the correct pose so that the anchors of the three HingeJoints can be defined correctly?


Additionally, if the parts are connected correctly, when I control the rotation of the top link, will the lower two links (without control) rotate accordingly?


Thank you!

##### main 03/24/2021 17:39:03
What exactly is a supervisor controller/node?

##### Isha 03/24/2021 18:11:09
Hi! In the Webots, is it possible for two epuck robots to communicate via Bluetooth using python apis?

##### Simon Steinmann [Moderator] 03/24/2021 23:18:49
`@Isha` Webots includes a transmitter and emitter node, which can be used for communication.


`@rschilli` I used stable baselines with parallel webots instances in the past. I'll pm you 🙂


`@main` [https://cyberbotics.com/doc/reference/supervisor](https://cyberbotics.com/doc/reference/supervisor) if a robot controller is a supervisor controller, it has all the normal robot controller functions, but expands its capabilities to do pretty much anything. Read the documentation to get an overview of all its functions

##### Olivier Michel [Cyberbotics] 03/25/2021 07:10:33
Beware that when you start Webots on Linux, there are two process created, one is named `webots-bin` and the other one is the launcher, called `webots`. The `WEBOTS_PID` variable correspond to the first one. You can check it by listing the content of the`/tmp/` folder which should contain a sub-folder with named `webots-PID` where `PID` is the value you should set in `WEBOTS_PID`. Let me know if that helps.

##### rschilli 03/25/2021 07:59:20
Thanks for this additional hint. Meanwhile I can connect to multiple instances, as well with the `WEBOTS_TMPDIR` solution of `@Darko Lukić` as also by reading the `WEBOTS_PID` from the subfolders you mentioned (even if modifying env-vars is not straight forward inside a java runtime..). But as already told, I have the problem with multiple robots per thread, so its only working with separated JVMs for each robot. So currently I am looking to switch to multiprocessing instead of multithreading in java or even switch our controller logic to Python to use its multiprocessing library and re-write our communication between java-python for the learning process

##### danielvicente 03/25/2021 09:10:12
Hello, I'm trying to run webots on fast mode but it doesn't do anything and runs like normal, I tried doing it with the menu option simulation > fast and also by running webots with the flag --mode=fast , none of the options worked. Anyone knows how to fix this? thank you

##### Olivier Michel [Cyberbotics] 03/25/2021 09:26:40
Normally the speedometer should indicate a value above 1 if your simulation is not too complex. Otherwise, it should display a value below 1 and the fast mode makes indeed no difference with the run mode in this case.

##### danielvicente 03/25/2021 09:27:24
the speedometer remains about the same in both modes, is that normal?

##### Olivier Michel [Cyberbotics] 03/25/2021 09:27:37
Yes, if it's below 1.


It means you are running Webots at the maximum speed.

##### danielvicente 03/25/2021 09:28:13
okay, thanks


I thought the 3d window was supposed to be black with fast mode enables

##### Olivier Michel [Cyberbotics] 03/25/2021 09:29:21
You can disable the rendering in the 3D rendering in the main window by pressing the button next to the fast button (the wireframe box).

##### danielvicente 03/25/2021 09:30:42
is there any way to do that through the command line?

##### Olivier Michel [Cyberbotics] 03/25/2021 09:30:53
Yes.


`webots --help` will help you.

##### danielvicente 03/25/2021 09:32:37
okay thank you

##### Master.L 03/25/2021 10:20:10
Hello, how can I increase the range of the distance sensor?

##### DDaniel [Cyberbotics] 03/25/2021 10:40:10
`@Master.L` you have to modify the look-up table accordingly, specifically the last row. [https://cyberbotics.com/doc/reference/distancesensor#lookup-table](https://cyberbotics.com/doc/reference/distancesensor#lookup-table)

##### Master.L 03/25/2021 10:59:15
Thank you `@DDaniel` !

##### Dorteel 03/25/2021 11:51:59
Hi! I'm trying to get the contact points of the UR3 arm to detect collisions, but I can't seem to get the contact points.  `getNumberOfContactPoints() `just gives 0, even through there are many contact points. I tried looking at the example with the cylinder\_stack world, but even that doesn't give the right contact points (it gives 4 contact points when the red cylinder is just in freefall). Can anybody explain how these points work or how they could be calculated?

##### Götz 03/25/2021 12:04:32
I'm getting into the InertialUnit to have my robots turn more exactly. So me not having an engineering background... when I test, I get these values (right are IU readings): 
```Right turn

0 rad / 0 Deg = 0.05
1.5708 rad / 90 Deg = 1.57
3.14159 rad / 180 Deg = 3.14
4.71239 rad / 270 Deg = -1.57
6.28319 rad / 360 Deg = 0.05
```

So first thing: what is the smartest way to convert the values  into degrees from 1-360? Or even rad, but consecutive...


And then: when I use an IU to control turns, I do have to turn a bit, get yaw value, turn a bit... until I reach the desired angle. Right? So I would have to call robot.step on every loop run? Does this make sense perfomance-wise?


Thanks!

##### Darko Lukić [Cyberbotics] 03/25/2021 12:09:56
You can measure difference between two steps, if it is bigger than let's say 0.3 rad that means it means the robot has crossed 0/2pi


That's how a controller works in general


Did you set `includeDescendants` to `true`?

##### James Le Poidevin 03/25/2021 12:19:24
Hello, Is it possible to fill in the polygons on an imported mesh? (like the right screen on meshlab)
%figure
![Capture_decran_de_2021-03-25_13-17-57.png](https://cdn.discordapp.com/attachments/565154703139405824/824618473782902814/Capture_decran_de_2021-03-25_13-17-57.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 03/25/2021 12:29:59
What is displayed in the Webots with white lines is the boundingObject and it is never "filled" but just displays the wireframe.

However the graphical object should automatically be displayed (as "filled"). If it is not, then you should first check if maybe the faces viewed from the back instead of the front. The IndexedFaceSet.ccw field lets you specify if faces are defined clockwise or counter-closkwise.

If this still doesn't solve the issue you should check if any warning is printed in the Webots console.

##### James Le Poidevin 03/25/2021 12:35:58
Sorry when i meant "filled" i mean that items could not pass in between the wires. I have a mesh in meshlab that i export in .stl (or .dae ...) and i want to use it as a floor for my simulation but the objects fall though it.

##### Olivier Michel [Cyberbotics] 03/25/2021 12:37:49
Did you set the boundingObject for the Solid node?

##### James Le Poidevin 03/25/2021 12:37:57
yes

##### Olivier Michel [Cyberbotics] 03/25/2021 12:38:38
Is it an IndexedFaceSet or an ElevationGrid?

##### James Le Poidevin 03/25/2021 12:39:05
indexedFaceSet

##### Olivier Michel [Cyberbotics] 03/25/2021 12:39:39
Collision detection with IndexedFaceSet nodes and small or large objects is sometimes buggy...

##### James Le Poidevin 03/25/2021 12:40:21
Ok It is a 1km floor so that could be it

##### Olivier Michel [Cyberbotics] 03/25/2021 12:40:57
Indeed.


Try to use an ElevationGrid instead, it should be more robust against this kind of bug.

##### James Le Poidevin 03/25/2021 12:43:59
I've spent the last 2 days trying to convert my .tif file to an elevation grid but there is often more than a million heights and so far they have all been very ugly(cf photo) to that's why i tried with an indexedface
%figure
![elevation.png](https://cdn.discordapp.com/attachments/565154703139405824/824624659990708244/elevation.png)
%end

##### Götz 03/25/2021 12:56:19
0/2pi...? TBH I still don't get it. How do I handle the IU value going up and then at 3.14 rad turning negative and going down? But hey, I'm perfectly aware you can't provide engineering 101 support here... I'll try to figure it out. Thanks!

##### Simon Steinmann [Moderator] 03/25/2021 19:16:00
`@Götz` Take this image. If you turn counter clock wise, at 180° you have 3.14 (pi) radians. But instead of going up from there to describe the angle around the origin, you can start at 0 ° / 0 rad, and go clockwise. The values will become negative: -90° (- pi / 2) until you reach -180° (-pi). A Full circle is 360° or 2*pi. but you can just move it and define it from -180° to 180° and -pi to pi.
%figure
![xsAa3.png](https://cdn.discordapp.com/attachments/565154703139405824/824723314315755571/xsAa3.png)
%end


So if you were going counterclockwise increasing the angle more and more, once you pass 180° (pi), you would immediatly jump to -180° (-pi) and keep going from there


This jump is the major reason why Euler angles (roll, pitch, yaw) are usually not used for describing absolute orientations. Doing calculations with those can lead to issues around that jump. That's where axis angles, quaternion and Rotation matrices are far superior. But they are much harder to understand and less intuitive. For your current purpose euler angles should be enough

##### Westin 03/25/2021 20:44:19
Does anyone have advice for modeling a joint that is actuated using a linear actuator between the two segments? I made the lower and upper parts connected with the hinge, but I'm not sure how to make a slider joint also connect. It kind of ruins the robot tree since it makes a loop. I tried using DEF, but the node wouldn't show up when I was selecting the other end of the slider. I guess if this isn't possible, I could just use a normal motor and model the differences in the controller.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/824745537420918894/unknown.png)
%end

##### Joanna 03/25/2021 20:51:58
Hello, everyone I followed wiki on [https://github.com/cyberbotics/webots/wiki/Docker](https://github.com/cyberbotics/webots/wiki/Docker). But when access the localhost:1999 or 2000 for simulation stream server in dicker. I see no simulation : can only upgrade to WebSocket message. Can someone help me in that?


Secondly, we tried to run webots using gpu in docker following this repository [https://github.com/cyberbotics/webots-docker](https://github.com/cyberbotics/webots-docker). We use vnc(not headless) and we can see the gpu inside the container by nvidia-smi. But when we launch the webbots simulation, the webbots doesn't detect any gpu. Does someone has experience in that?

##### Westin 03/25/2021 21:45:25
I found the SolidReference in the documentation and that is what I was missing. My joint still isn't working, but I suspect I have joint parameters that aren't in agreement about which way things are going to go.

##### Simon Steinmann [Moderator] 03/25/2021 22:55:41
`@Westin` closed loops are very difficult. it is often easier to 'fake' the linear actuation by putting a motor into the joint.

##### Westin 03/25/2021 22:59:08
Okay, thanks for the tip. I'm not totally sure I'm able to do what I want, but I think I will be using a joint like this in a variety of scenarios with different configurations, and I'd like to get it right and parameterize it in a proto file. In the long term, that may be easier than faking it each time a bit different.


Is this what the tree should look like? I'm unsure whether the two hinge joints on the right side are required. I could see that going either way. I think I have all of my joints facing the right way, but I get no movement as I move the linear motor. The top and left solids are the two segments of the joint.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/824780857847709786/unknown.png)
%end

##### 이 거북이 03/26/2021 02:57:58
I dont understand how to make the Hoap2 robot to move can anyone help?

##### Uanuan 03/26/2021 06:35:46
Hello, I am trying out the supervisor and robot API after completing tutorials. I have my script based largely on the materials from tutorial 6 (the 4-wheelers bot). However, I'm a bit stuck with what I need to do to get it to import a new wheel to attach to the bot, set the name, modify the hingeJoint parameters and such since I noticed that some of the parameters are immutable (such as the Device type which only has get\_name). I want to change the name for the imported new wheel so that I don't have duplicates in case I want to import additional wheels from the exported wheel prototype "HingeJoint.wbo." I also tried to modify the field directly (such as name) with the code "wheel.name = 'somename'" but to no avail. Could someone give me some pointers as to how to accomplish this in python?


Sorry for double posting, here's a screenshot of what I'm trying to do: the bot at the center top has one of the wheels deleted at the start of the simulation, whereas the center bottom image shows the bot gained a new wheel as a result of running my code. I tried various ways based on the doc to try to change a new name and modify the hingejoint and such while the simulation's running, but to no avail:
%figure
![Untitled.png](https://cdn.discordapp.com/attachments/565154703139405824/824894922473340958/Untitled.png)
%end

##### Laprase13 03/26/2021 08:13:26
Hi, how do I run Webots using the dedicated GPU of my laptop? It is an NVIDIA GPU, but the NVIDIA control panel doesn't show the application

##### Darko Lukić [Cyberbotics] 03/26/2021 08:13:53
You can use the `wb_supervisor_field_import_mf_node_from_string` function instead:

[https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_field\_import\_mf\_node\_from\_string](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_field_import_mf_node_from_string)



Before passing the string to the function simply replace the name and everything else you need.

##### Uanuan 03/26/2021 08:17:25
Thanks Darko, I've just got it working with the following (gonna post it here for others): 



def experimentAddWheel(robotHandle):

    node = robotHandle.getSelf()

    children = node.getField("children")

    # 0 corresponds to position: 0 - beginning of children

    children.importMFNode(-1,"HingeJoint.wbo")

    wheel = children.getMFNode(-1)

    #print("wheel is {0}".format(wheel.getField("device")))

    devicename = wheel.getField("device").getMFNode(-1).getField("name")

    #print("before change: {0} ".format(devicename.getSFString()))

    devicename.setSFString("blahblahblah")

    print("Type--- {0}".format(type(wheel)))

    #print("after change: {0} ".format(devicename.getSFString()))

    

    #field = node.getField("translation")

    #field.setSFVec3f([0.1,0.1,0.1])



    endpoint = wheel.getField("endPoint")

    name = endpoint.getSFNode().getField("name")

    name.setSFString("somenamehahaha")

    #print(name.getSFString())



    #test code for existing wheel2

    #wheel = children.getMFNode(2)

    #devicename = wheel.getField("device").getMFNode(0).getField("name")

    #print("before change: {0} ".format(devicename.getSFString()))

    #devicename.setSFString("blahblahblah2")

    #print("after change: {0} ".format(devicename.getSFString()))



Unfortunately, I ran into another problem. The "wbo" file has a predefined name "wheel1", so when I run the sim with a new name "blahblahblah" for that particular importMFNode motor, for some reason, the robot only shows the old name "wheel1". It doesn't seem to "refresh" the name of devices when I use getDeviceByIndex(). here's my code and output:
> **Attachment**: [my\_controller.py](https://cdn.discordapp.com/attachments/565154703139405824/824919964221308938/my_controller.py)


And output: Sorry that this is a bit messy since I'm still learning the thingy so there are a lot of prints 😛  total number of devices: 5 (before adding a new motor from the WBO file)

    Device 0 : <controller.Motor; proxy of <Swig Object of type 'webots::Motor *' at 0x7f7acd5c68d0> >

    Device 1 : <controller.Motor; proxy of <Swig Object of type 'webots::Motor *' at 0x7f7acd52c330> >

    Device 2 : <controller.Motor; proxy of <Swig Object of type 'webots::Motor *' at 0x7f7acd4cfed0> >

    Device 3 : <controller.DistanceSensor; proxy of <Swig Object of type 'webots::DistanceSensor *' at 0x7f7acd52c510> >

    Device 4 : <controller.DistanceSensor; proxy of <Swig Object of type 'webots::DistanceSensor *' at 0x7f7acd4cfe70> >



total number of devices: 6 (After adding the wbo file)

    Device 0 : <controller.Motor; proxy of <Swig Object of type 'webots::Motor *' at 0x7f7acd5c6db0> >

    Device 1 : <controller.Motor; proxy of <Swig Object of type 'webots::Motor *' at 0x7f7acd5c68d0> >

    Device 2 : <controller.Motor; proxy of <Swig Object of type 'webots::Motor *' at 0x7f7acd52c180> >

    Device 3 : <controller.DistanceSensor; proxy of <Swig Object of type 'webots::DistanceSensor *' at 0x7f7acd4cfde0> >

    Device 4 : <controller.DistanceSensor; proxy of <Swig Object of type 'webots::DistanceSensor *' at 0x7f7acd4cfe70> >

    Device 5 : <controller.Motor; proxy of <Swig Object of type 'webots::Motor *' at 0x7f7acd52c2d0> >

My name is: wheel2

My name is: wheel3

My name is: wheel4

My name is: ds\_right

My name is: ds\_left

My name is: wheel1 <--- it's supposed to be "blahblahblah" based on my\_controller.py, but it still stays as wheel1

##### Darko Lukić [Cyberbotics] 03/26/2021 08:26:19
You should restart the controller if you change the name like that

##### Uanuan 03/26/2021 08:28:09
thanks, how should I do that? I did try supervisor.\_\_init\_\_() but it seems to get stuck in an infinite loop. I think it reruns my\_controller.py again and again while trying to add wheel1 into the children node, which then generated an error with a message like: wheel1 already exists.

##### Darko Lukić [Cyberbotics] 03/26/2021 08:34:05
You can use the `wb_supervisor_node_restart_controller` function:

[https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_restart\_controller](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_restart_controller)



Before adding the joint and restarting the controller make sure that the new joint doesn't exist in the robot. That way you will prevent the infinite loop.

##### Uanuan 03/26/2021 08:34:55
Great, thanks again Darko, appreciate it. I'll try out the method later 🙂

##### Ruchi 03/26/2021 13:19:42
Hello!!

Can we have communication between webots simulator being the laptop and any bot in environment.

Because as far as I know two epucks can communicate with emitter and receiver node but not sure about the case I mentioned.

##### main 03/26/2021 18:11:47
what does a red bounding box usually mean?


Are objects with colliding bounding boxes allowed to move together in space? say a robot leg made of multiple shapes with overlapping bounding boxes

##### DDaniel [Cyberbotics] 03/26/2021 18:27:02
`@main`  they turn red when a collision occurs. Consecutive solids (two solids linked by a joint) don't trigger it, but if there's a third layer, that one can collide with the first. Internal collisions like these only occur if the selfCollision field at the level of the robot is enabled, otherwise all of them are ignored.

##### Simon Steinmann [Moderator] 03/26/2021 19:26:48
Can you elaborate on what exactly you want to achieve? In general a Supervisor controller can get and set any information in a simulation.

##### Pancha 03/27/2021 17:41:31
Is there some way of simulating a suction cup (a type of gripper)? I have an arm with a suction cup on the end which would work in the same fashion as a pick-and-place robot.

##### Simon Steinmann [Moderator] 03/27/2021 17:51:44
`@Pancha` there is the connector node, this could be used for suction gripping, however, you can only attach and detach two connectors, not grip an object at any spot you like

##### Pancha 03/27/2021 18:52:02
I'll take a look at this. Wouldn't be the most optimal solution, but it pushes me in the correct direction. Cheers 🙂


One more question, which seems rather simple, but I haven't found a proper solution for it. Is there any way to rearrange the order of nodes under a children other than cut --> pasting? This seems to be an issue when I'm using DEF and other nodes inherits from this one. When I add a new node (or copy-pastes) it appears on top of the hierarchy and can therefore not inherit from a DEF further below.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/825455526938804274/unknown.png)
%end

##### Simon Steinmann [Moderator] 03/28/2021 00:03:44
`@Pancha` you can select any of the children (so it is highlighted). Pasting or adding a node, adds it underneath the highlighted child

##### Pancha 03/28/2021 00:05:04
Wow! Why didn't I think of that 😅  Thanks again 😉

##### yash 03/28/2021 13:25:57
I want to use Model predictive control -MPC for trajectory tracking , but Webots already runs a PID controller , so how can I neglect the Webots  PID controller to design my own MPC controller ?

##### Olivier Michel [Cyberbotics] 03/28/2021 16:17:43
You can control your motor in torque and Webots won't use the built-in PID controller as documented in the table here: [https://cyberbotics.com/doc/reference/motor#force-and-torque-control](https://cyberbotics.com/doc/reference/motor#force-and-torque-control)

##### yash 03/28/2021 16:19:26
So but then I will have to drive the motors in torque mode right .?> You can control your motor in torque and Webots won't use the built-in PID controller as documented in the table here: [https://cyberbotics.com/doc/reference/motor#force-and-torque-control](https://cyberbotics.com/doc/reference/motor#force-and-torque-control)

`@Olivier Michel`  So but then I will have to drive the motors in torque mode right .?

##### Olivier Michel [Cyberbotics] 03/28/2021 16:19:47
Yes.

##### yash 03/28/2021 16:20:10
Alright thank you 👍

##### Chinwei.Chang 03/29/2021 00:59:47
How to experience VR tour using a virtual reality headset (HTC Vive) to view the simulation? I have installed steam and steamVR on windows. However, it doesn't work! My webots  version is 2021a.

##### Simon Steinmann [Moderator] 03/29/2021 01:08:12
[https://cyberbotics.com/doc/guide/computer-peripherals](https://cyberbotics.com/doc/guide/computer-peripherals)


did you do the room setup?

##### Chinwei.Chang 03/29/2021 01:20:54
yes, room setup is done!

##### Stefania Pedrazzi [Cyberbotics] 03/29/2021 06:24:45
Usually, from the NVIDIA control panel, you can add any program manually selecting the application from the file manager.

##### Laprase13 03/29/2021 06:30:39
Yes, I have checked the list of programs, but Webots doesn't seem to be there

##### Stefania Pedrazzi [Cyberbotics] 03/29/2021 06:34:07
When adding new programs to the list, the file manager opens. Then you have to navigate to the installation directory. Did you do it? or did you simply checked in the list of applications already available by default in the NVIDIA control panel programs list?


Another options, but I don't remember exactly if it works, would be to set the NVIDIA GPU as default for all the programs, launch Webots and then check again if Webots is in the list of programs in the NVIDIA control panel.

##### Laprase13 03/29/2021 06:43:42
I haven't checked the Add button and Webots was there! Thanks for the help!

##### xjs 03/29/2021 08:07:58
Does anyone know what the delay function of C language in webots is

##### AleBurzio 03/29/2021 11:48:48
Hello, I have a question on ros2 + webots for vehicles. 



I have tried the webots\_ros2\_tesla package and it works fine (even though lane\_follower crashes whenever the yellow line is not detected, e.g. in crossroads because a division by zero happens somewhere), but when I tried applying the extern controller on a car in a different world it starts running significantly slower (about x0.2-0.3 vs tesla world x0.8-0.9) as soon as I start acquiring camera data. 



I thought the problem was the basic time step in my world being a lot smaller but they're both set to 10, so I'm not exactly sure what could be causing this issue? Is there a setting for camera frames per second? (camera resolution is the same)

##### Darko Lukić [Cyberbotics] 03/29/2021 11:53:40
Hello, it is a simple example, not robust, but it should not crash, we should fix it.



> I tried applying the extern controller on a car in a different world it starts running significantly slower



Check the camera resolution and the frequency at which you are acquiring and sending the images. I noticed that the bottleneck is on the ROS side, but hopefully it will be better in the future thanks to the FastDDS zero-copy feature.

##### AleBurzio 03/29/2021 11:59:55
thanks for the fast reply, I'll send you a quick video to reproduce the crash



how can I check how fast the image is being acquired? Is this a ROS problem or is there some setting that can force a certain acquisition speed for the camera directly in webots?

##### Darko Lukić [Cyberbotics] 03/29/2021 12:07:17
You change the camera resolution in the Webots world and the camera publish rate through the ROS 2 driver, see an example:

[https://github.com/cyberbotics/webots\_ros2/blob/d0f772b358252cda6b9e56768e5ec5cf7454a5ff/webots\_ros2\_epuck/webots\_ros2\_epuck/driver.py#L72](https://github.com/cyberbotics/webots_ros2/blob/d0f772b358252cda6b9e56768e5ec5cf7454a5ff/webots_ros2_epuck/webots_ros2_epuck/driver.py#L72)



In your case it should be something like:

```python
self.start_device_manager({'camera_name': {'timestep': 128}})
```



See the available ROS 2 configurations for the sensor devices:

[https://github.com/cyberbotics/webots\_ros2/wiki/API-Devices#sensordevice](https://github.com/cyberbotics/webots_ros2/wiki/API-Devices#sensordevice)

##### AleBurzio 03/29/2021 12:09:14
Thank you! That's what I was looking for 🙂


This is a video of the crash, should I open an issue on GitHub?
> **Attachment**: [lane\_follower\_crash.mp4](https://cdn.discordapp.com/attachments/565154703139405824/826067795573866556/lane_follower_crash.mp4)

##### Darko Lukić [Cyberbotics] 03/29/2021 12:19:12
Yes, please. Thank you!


Feel free to open a PR if you know how to fix it 🙂

##### AleBurzio 03/29/2021 12:46:51
So I have found a quick fix which doesn't make the program crash, but the car still crashes after a while so I'm not sure how good of a fix it is 🥲

##### benj3110 03/29/2021 12:54:42
is there a way to to set an acelerometers to measure acceleration in the worlds coord sys rather than the objects?

##### ClBaze 03/29/2021 12:58:39
Hello, I am controlling a robotic arm in velocity. I want to switch to position control for the last point of the trajectory. Is it enough to use setPosition(pos) to switch back to that mode ?


The documentation says that using setPosition() when controlling in torque will switch to position control mode but it doesn't say when controlling in velocity


[https://cyberbotics.com/doc/reference/motor#force-and-torque-control](https://cyberbotics.com/doc/reference/motor#force-and-torque-control)

##### Stefania Pedrazzi [Cyberbotics] 03/29/2021 13:43:00
Yes, it is enough to set the position to switch to position control. Velocity control is only enabled when the position is set to infinity.

##### Darko Lukić [Cyberbotics] 03/29/2021 14:52:11
You can stop the car when there is no lane. It is an example, we are not making autonomous cars 😄

##### enbakom ghebremichael 03/29/2021 15:58:57
Haw can I change the  objects size in the world. I add a bed hawever I couldnt change the size of the bed. could you help me with that?

##### Darko Lukić [Cyberbotics] 03/29/2021 16:24:10
You cannot change the size of the bed. You can import different 3D model though.



Website with free 3D models:

[https://free3d.com/](https://free3d.com/)



Search for the `Import 3D Model...` feature:

[https://cyberbotics.com/doc/guide/the-user-interface#file-menu](https://cyberbotics.com/doc/guide/the-user-interface#file-menu)

##### Westin 03/29/2021 20:17:54
Does physics need to be enabled for a robot to have a functional mechanical loop?

##### Olivier Michel [Cyberbotics] 03/29/2021 20:22:37
Yes.

##### Westin 03/29/2021 20:24:31
`@Olivier Michel` Maybe that is my problem. Does this tree drawing look right for modeling a joint controlled by a linear motor?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/826190107853848617/unknown.png)
%end



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/826190144650084362/unknown.png)
%end

##### Olivier Michel [Cyberbotics] 03/29/2021 20:32:03
Yes, that looks good.

##### Westin 03/29/2021 20:38:48
`@Olivier Michel` Great, thank you. It made sense to me but nothing would move. I'll try again with physics enabled for the robot.


`@Olivier Michel` do you think the two hinges noted in the tree are required? Does the slider require that the points it attaches to stay at the same angle throughout movement or can they rotate freely without a hinge to explicitly allow it to?

##### SeanLuTW 03/30/2021 02:20:02
In the document, it mentioned that `the duration parameter must be a multiple of the WorldInfo.basicTimeStep`. What if, say, I have `basicTimeStep` 16 and try to call `robot.step(8)`?

##### Simon Steinmann [Moderator] 03/30/2021 02:21:43
that is an issue


you can only call 16, 32 etc.

##### SeanLuTW 03/30/2021 02:44:55
In my simulation with `basicTimeStep` set to 16, I called `robot.step(1000)` and it just worked fine. I wonder what will happen if the duration is non-integer multiple of `basicTimeStep`

##### Simon Steinmann [Moderator] 03/30/2021 02:45:12
it probably rounds

##### Srivastav\_Udit 03/30/2021 03:02:22
I'm using the same controller on three robots in the same world. I need them to have the ability to update the same int array. Is it possible via the Supervisor?

##### SeanLuTW 03/30/2021 03:26:21
Maybe you can add a PROTO supervisor robot with `MFFloat` field, and use your controller to access/change the data?

##### Srivastav\_Udit 03/30/2021 03:32:43
Could you direct me to some documentation/example I could use?

##### James Le Poidevin 03/30/2021 09:25:30
Hello, Is it possible to publish GPS and RangeFinder data to a ros2 topic ? I can't find any examples.

##### Bitbots\_Jasper [Moderator] 03/30/2021 11:26:54
I have a question about terminology. what are the fields in the beginning of a proto which can be overwritten in the world file called? exported fields, exposed fields?

##### Olivier Michel [Cyberbotics] 03/30/2021 11:27:19
exposed fields.

##### Bitbots\_Jasper [Moderator] 03/30/2021 11:27:25
thanks


is the second column of a LookupTable ([https://cyberbotics.com/doc/reference/distancesensor#lookup-table](https://cyberbotics.com/doc/reference/distancesensor#lookup-table)) an integer value and will the function returning the current value of a sensor also only return an integer value when a LookupTable is used?

##### [DER]Mad[MAN] 03/30/2021 13:46:32
Hello to all! I am developing a basic simulator to be able to develop an autonomous navigation system. I'm working with your simulator, but the prototyping of the system goes through Ros2 Foxy. I wrote a controller to drive my robot model and manage its sensors. I want to use an IMU/GPS couple to generate a perfect odometry at first. Unfortunately, I have difficulties to activate the IMU and read its data. The controller is written in Python. The IMU is named in the simulation : inertial\_unit, and I load it via the getDevice() method when initializing my object. In a function belonging to my object, I then try to read the quaternion with the getQuaternion() method. However, the IMU informs me that I am trying to read data from a disabled device.



```
        self.gps = self.robot.getDevice("gps")
        self.imu = self.robot.getDevice("inertial_unit")
        self.odometry_topic = '/odom'
        self.odometry_frame = 'odom'
        self.robot_base_frame = 'base_link'
        self.gps_sampling_period = 10
        self.imu_sampling_period = 10
        self.gps.enable(self.gps_sampling_period)
        self.imu.enable(self.imu\_sampling\_period)
```



```def position_read(self):
        self.get_logger().info('IMU sampling period is : "%f"' % self.imu.getSamplingPeriod())
        self.get_logger().info('GPS sampling period is : "%f"' % self.gps.getSamplingPeriod())
        orientation = self.imu.getQuaternion()
        position = self.gps.getValues()
        stamp = self.get\_clock().now().to\_msg()
```



```[sylvester_controller-3] [INFO] [1617110834.248941546] [sylvester_controller]: IMU sampling period is : "0.000000"
[sylvester_controller-3] [INFO] [1617110834.249945361] [sylvester_controller]: GPS sampling period is : "10.000000"
[sylvester\_controller-3] Error: wb\_inertial\_unit\_get\_quaternion() called for a disabled device! Please use: wb\_inertial\_unit\_enable().
```



I don't understand what I'm doing wrong. Is it a bug?

##### briiiii 03/30/2021 14:02:36
Hi, Just wondering does anyone know how to draw in Webots? Im trying to draw a rectangle or a straight line but I have some errors and here's my code. Thank you so much and have a great day



robot = Robot()



\#make this the output for drawing

ground\_display = robot.getDevice("ground\_display")

 

\#make the output floor white

ground\_display.setColor(0x000000)

 

\#draw rectangle with corner at (0,0), width of 30 and height of 30

ground\_display.fillRectangle(0,0, 30, 30)





And my error
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/826456383947669514/unknown.png)
%end

##### Olivier Michel [Cyberbotics] 03/30/2021 14:04:22
You need to equip your Robot with a Display device named "ground\_display"...

##### briiiii 03/30/2021 15:21:08
Hi Olivier, I did but I still have the error
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/826476145507696700/unknown.png)
%end

##### Olivier Michel [Cyberbotics] 03/30/2021 15:22:03
It's not a Display, it's a Solid node...

##### Darko Lukić [Cyberbotics] 03/30/2021 15:22:57
Have you tried?

[https://github.com/cyberbotics/webots\_ros2](https://github.com/cyberbotics/webots_ros2)

##### briiiii 03/30/2021 15:25:40
I see thanks Olivier!

##### [DER]Mad[MAN] 03/30/2021 15:25:52
yes I use webots\_ros2

##### John520 03/30/2021 15:30:29
Hi guys, I am looking for an inclinometer in Webots but could not find one in Nodes. Does Webots have a sensor that works similarly to an inclinometer? Thank you!

##### Darko Lukić [Cyberbotics] 03/30/2021 15:31:45
GPS and IMU data should be automatically published, that doesn't work for you?


You can use the IMU instead:

[https://cyberbotics.com/doc/reference/inertialunit](https://cyberbotics.com/doc/reference/inertialunit)

##### John520 03/30/2021 15:34:23
Ahh, thank you `@Darko Lukić` !

##### [DER]Mad[MAN] 03/30/2021 15:37:25
I tried several way. The one that I post here, GPS works. But I can't read data from the IMU. I use this one  [https://cyberbotics.com/doc/reference/inertialunit?tab-language=python#wb\_inertial\_unit\_enable](https://cyberbotics.com/doc/reference/inertialunit?tab-language=python#wb_inertial_unit_enable). I tried to load the IMU with the device\_manager. But when I tried to echo topic's data, the controller stoped

##### Darko Lukić [Cyberbotics] 03/30/2021 15:39:18
Sounds like a bug. Can you report it:

[https://github.com/cyberbotics/webots\_ros2/issues/new?assignees=&labels=&template=bug\_report.md](https://github.com/cyberbotics/webots_ros2/issues/new?assignees=&labels=&template=bug_report.md)

##### [DER]Mad[MAN] 03/30/2021 15:40:07
Okay !

##### Darko Lukić [Cyberbotics] 03/30/2021 15:42:18
BTW, the device manager automatically disables a device when not used. See here how to enable it all the time:

[https://github.com/cyberbotics/webots\_ros2/blob/d0f772b358252cda6b9e56768e5ec5cf7454a5ff/webots\_ros2\_epuck/webots\_ros2\_epuck/driver.py#L60](https://github.com/cyberbotics/webots_ros2/blob/d0f772b358252cda6b9e56768e5ec5cf7454a5ff/webots_ros2_epuck/webots_ros2_epuck/driver.py#L60)

##### [DER]Mad[MAN] 03/30/2021 15:45:43
I will check with that


this is my device\_manager call : 

```
self.start_device_manager({
            'robot': {'publish_base_footprint': True},
            'LDS-01' : {'topic_name' : '/sylvester/scan'},
            'inertial_unit' : {'always_publish': True, 'topic_name': '/sylvester/IMU'}
        })
```

I get this error :

```
AttributeError: 'InertialUnit' object has no attribute 'getValues'
```

But I never call getValues() method in my controller.



either I'm not using the IMU correctly or the problem comes from calling a function that doesn't seem to exist in the IMU driver

##### Darko Lukić [Cyberbotics] 03/30/2021 16:08:51
I see, we fixed that in the `develop` branch, but it targets Webots R2021b:

[https://github.com/cyberbotics/webots\_ros2/blob/dce214b403edee795fee63d926cb70d015fffefc/webots\_ros2\_core/webots\_ros2\_core/devices/imu\_device.py#L90](https://github.com/cyberbotics/webots_ros2/blob/dce214b403edee795fee63d926cb70d015fffefc/webots_ros2_core/webots_ros2_core/devices/imu_device.py#L90)

##### [DER]Mad[MAN] 03/30/2021 16:12:46
Yes I run R2021a. Do I still must open bug report ? (I'm junior and new in debugging world ^^') Or do something else ?

##### Darko Lukić [Cyberbotics] 03/30/2021 16:15:49
Yes please. No worries, it is good that you noticed the bug

##### Simon Steinmann [Moderator] 03/30/2021 18:57:37
video capture does not work. ubuntu 20. 2021a (.ros install)
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/826530625141342238/unknown.png)
%end


sudo apt-get install libx264-dev

sudo apt-get install libfdk-aac1

Is installed


reinstalled ffmpeg with "sudo apt install ffmpeg"


now I get this error: 

ffmpeg: error while loading shared libraries: libvo-amrwbenc.so.0: cannot open shared object file: No such file or directory


FIX:  running `linux_runtime_dependencies.sh` fixed the issue


<@&568329906048598039> perhaps you should check, if the automatic ros install takes the dependencies into account. Might just be some other issue on my system though.

##### Westin 03/30/2021 20:18:22
Is there a way to change the size of the joint axes?

##### Simon Steinmann [Moderator] 03/30/2021 21:07:15
change the size?


the way it is displayed or the actual size?

##### Westin 03/30/2021 21:25:36
`@Simon Steinmann` The way it is displayed. I'm talking about the xyz triad that is visible when the "Show Joint Axes" option is enabled.

##### Simon Steinmann [Moderator] 03/31/2021 00:36:26
`@Westin` you can use "lineScale" under Worldinfo to change the size of optionally rendered lines such as the joint axes

##### Olivier Michel [Cyberbotics] 03/31/2021 06:14:57
How did you install Webots? From the .deb package, snap or tarball? Depending on the installation method, in particular with the tarball, some additional steps are needed to get ffmpeg properly installed to allow Webots to make movies. This is normally fully documented here: [https://cyberbotics.com/doc/guide/installation-procedure#installing-the-tarball-package](https://cyberbotics.com/doc/guide/installation-procedure#installing-the-tarball-package)

##### SeanLuTW 03/31/2021 06:17:43
Can I get the status of `movieStopRecording`? I want to reload and restart my world after the video is saved, currently I use sleep to wait the process, but the duration is hard to decide

##### Olivier Michel [Cyberbotics] 03/31/2021 06:19:44
Did you try to use movieIsReady()? See [https://www.cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_movie\_is\_ready](https://www.cyberbotics.com/doc/reference/supervisor#wb_supervisor_movie_is_ready)

##### SeanLuTW 03/31/2021 06:31:31
`@Olivier Michel`  Oh! I misunderstood its meaning, I will try. Thank you!

##### James Le Poidevin 03/31/2021 09:37:26
Hello, I'm using webots with ros2 and using the start\_device\_manager(), it correctly detects that i have 18 motors (6 wheel Protos with 3 motors per wheel with different names each time). The problem is that when I echo joint\_states there is just 6 times the same three motors (should have Wheelmotor1\_1,1\_2 ...). Does anyone have any idea why?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/826752040771387422/unknown.png)
%end

##### Darko Lukić [Cyberbotics] 03/31/2021 12:17:22
Is it possible that you have multiple position sensors for a single motor? This is how the joint state publisher is discovering the motors:

[https://github.com/cyberbotics/webots\_ros2/blob/d0f772b358252cda6b9e56768e5ec5cf7454a5ff/webots\_ros2\_core/webots\_ros2\_core/joint\_state\_publisher.py#L50-L53](https://github.com/cyberbotics/webots_ros2/blob/d0f772b358252cda6b9e56768e5ec5cf7454a5ff/webots_ros2_core/webots_ros2_core/joint_state_publisher.py#L50-L53)

##### James Le Poidevin 03/31/2021 12:18:27
Ok thanks I'll try that

##### Srivastav\_Udit 03/31/2021 17:53:34
How can I change the colour of a solid after a particular event?

##### benj3110 03/31/2021 19:16:07
hey guys sry to ask again but is there a way to set the measurement of an accelerometer to the world coordinate sys rather than the objects? or should i use a GPS node and then work out the acceleration?

##### Simon Steinmann [Moderator] 03/31/2021 20:35:15
`@benj3110` you can get the orientation rotation 3x3 matrix (R) of the robot and transform your acceleration vectors into those coordinates


a\_world = R\_object * a\_object


* being the dot product


you have to use the supervisor functionality to change the color field of the solid

##### Canberk Arıcı 03/31/2021 21:13:34
Hello, I imported a map from OSM and added mavic drone to this map but drone cannot take off


What can I do?


If take drone up and leave, only this time I can turn drone to left or right but other buttons don't work

##### Srivastav\_Udit 03/31/2021 22:07:31
Is it possible to make the recognitionColors field for a solid null after a certain point in the supervisor?

##### Simon Steinmann [Moderator] 03/31/2021 22:07:57
you can edit any field in the supervisor

##### Canberk Arıcı 03/31/2021 22:11:58
Even if I edit drone controller code, I cannot see the change in simulation. Can anybody help?

##### Srivastav\_Udit 03/31/2021 22:15:52
Could you guide me to some sample world/code that does this?

##### Simon Steinmann [Moderator] 03/31/2021 22:16:14
[https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python](https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python)


get a node reference, thena field ref


then change that field


robot = Supervisor

node= robot.getFromDef(DEF\_target)

field = node.getField('fieldName')


field.set....


you have to set the correct data type

##### Canberk Arıcı 03/31/2021 22:20:46
Can't I change controller of drone?

##### Simon Steinmann [Moderator] 03/31/2021 22:20:55
you can


change the controller or edit the current one


you have to reset the simulation for it to go into effect


if it's c, c++ or java, you have to compile the changes first

##### Canberk Arıcı 03/31/2021 22:21:53
Okay, I'll try this, thank you

##### Simon Steinmann [Moderator] 03/31/2021 22:22:10
F7 in webots I believe.. check the menu

##### Canberk Arıcı 03/31/2021 22:22:28
Okay, thank you very much

##### Welsh\_dragon64 03/31/2021 22:55:35
I was trying to remotely connect the webots simulation to my actual robot, Dawrin robotisop2 and in the robot console i got the following message :Warning: File `/robotis/Framework/src/minIni/minIni.c' has modification time 2.2e+04 s in the future and this message been going in a infinite loop for a long time, has anyone one of you experienced this?

## April

##### benj3110 04/01/2021 12:00:15
Can you elaborate on how to get R\_object?


found the formula but should it not be the inverse if R\_object? i was thinking R\_object*a\_world = a\_object, if R\_object is the rotation matrix for the object?

##### Shyam 04/01/2021 13:18:20
I am trying to integrate Webots using TCP/IP ,initially I would like to add some common things like LIDAR, IMU, CAMERA, MOTOR, etc from Webots. I have gone through API  and TCP/IP documentation of Webots.

I would like to know on which ports does Webots publish data of different sensors? 

It would be grate if anyone can share some additional resources for this.

##### Joanna 04/01/2021 18:53:30
Hello,everyone. Do anyone have a link or model of an environment for warehouse for simulation of mobile robot?

##### DDaniel [Cyberbotics] 04/01/2021 18:58:01
`@Joanna` the closest available might be the factory environment, you can adapt it to your needs if it doesn't suit as is. [https://cyberbotics.com/doc/guide/samples-environments#factory-wbt](https://cyberbotics.com/doc/guide/samples-environments#factory-wbt)

##### Joanna 04/01/2021 19:01:17
Yes,I am aware of factory environment. But anyway, thanks Daniel. Was just kinda sure that somebody has already sucha Kinda environment "typical warehouse " since most of mobile robotics work is done in warehouses . Will see if somebody has it maybe.

##### briiiii 04/01/2021 19:51:49
Hi Olivier, So I changed the Solid node to a Display, and the ground supposed to be white but it showing a different color. Im just wondering If you know how to draw a simple rectangle in webots. Thank you so much Olivier



Here's my code

robot = Robot()



\#make this the output for drawing

ground\_display = robot.getDevice("ground\_display")

 

\#make the output floor white

ground\_display.setColor(0x000000)

 

\#draw rectangle with corner at (0,0), width of 30 and height of 30

ground\_display.fillRectangle(0,0, 30, 30)

##### Simon Steinmann [Moderator] 04/01/2021 21:14:08
you are right, you should do the inverse


rot\_base = np.transpose(rot\_base)


`rot_base = np.transpose(np.array(base.getOrientation()).reshape(3, 3))`


as one line to get R


base being the robot (copied it from some of my code)


np being numpy. For other languages you gonna have to figure it out yourself 😉

##### deleted 04/01/2021 22:14:14
hey, i want to make a robot that has leg joints controlled by servos


but i dont want it to collapse while unpowered


what would be my solution to this?

##### Simon Steinmann [Moderator] 04/01/2021 23:29:13
you are asking inside of webots?

##### deleted 04/01/2021 23:50:03
no, physically

##### Simon Steinmann [Moderator] 04/02/2021 00:10:50
this is the webots discord, I'm sure there is robotics servers and forums 🙂


perhaps adding spring tension or something

##### deleted 04/02/2021 00:30:26
yeah i just assumed that people here would know that.. since its something you would have to consider while designing a robot lol

##### Simon Steinmann [Moderator] 04/02/2021 00:48:25
most robots I know are collapsed when unpowered

##### KNTRL [Premier Service] 04/02/2021 10:42:27
Hi guys, I'm absolutely new and just finished the tutorial up to the ROS part (I need to get to know ROS first), and I'll be writing my master thesis with Webots + ROS, so I'll be more active here for the next months probably.



I have a question already: I observed that the software has some troubles with heavy objects (example a 1x1x1m box that falls off of a meter, etc..) that will sink through a plane floor for example, and stop at a certain point. Now I am aware that there might be some heavy numerical operations to solve which can easily lead to unwanted results, but what can I generally do to know that my object will most likely remain above the surface? Is there some kind of a rule of thumb, or is it a pure trial & error for anybody else too?

Cheers, and I'm looking forward to the further input in here.

##### DDaniel [Cyberbotics] 04/02/2021 11:29:38
`@KNTRL` when you notice interpenetration between solids like you mention it's usually that the `basicTimeStep` (in worldInfo node) being used is too big. This is especially noticeable for heavy objects. The physics engine will detect the violation and progressively correct it in the following steps but  the fact remains that the timestep being used isn't suitable. Another factor is the contactProperties (also in worldInfo) between the two objects, you can lower the `softCFM` parameter to get better results. [https://cyberbotics.com/doc/reference/worldinfo](https://cyberbotics.com/doc/reference/worldinfo)

##### KNTRL [Premier Service] 04/02/2021 11:35:10
<@787796043987025941> thank you, appreciate the info!

##### Shyam 04/02/2021 15:52:43
I am trying to integrate Webots using TCP/IP ,initially I would like to add some common things like LIDAR, IMU, CAMERA, MOTOR, etc from Webots. I have gone through API  and TCP/IP documentation of Webots.

I would like to know on which ports does Webots publish data of different sensors? 

It would be grate if anyone can share some additional resources for this.

##### satyar 04/03/2021 07:18:14
Hello everyone,

I have problem for working Webots with MacOS .

I installed python 3.9 and set python command “python3”

I tested the code and this world with windows pc that completely work 😕

Can anyone help!?



I have M1 MacBook with 11.2.3 macOS bigsur
%figure
![image0.jpg](https://cdn.discordapp.com/attachments/565154703139405824/827804172610175016/image0.jpg)
%end

##### Olivier Michel [Cyberbotics] 04/03/2021 07:28:53
Did you try with Python 3.8 from python.org?

##### Yi Zhou 04/03/2021 07:35:43
Hello everyone, I have a question about webots. When I try to integrated a kinect camera into panda robot, the kinect was attached perfectly, but the image windows show nothing, I checked some demo about camera, it look like I need  some code to make kinect work. So, does anyone know how to make kinect work?

##### Darko Lukić [Cyberbotics] 04/03/2021 13:57:04
You have to enable the camera:

[https://cyberbotics.com/doc/reference/camera#wb\_camera\_enable](https://cyberbotics.com/doc/reference/camera#wb_camera_enable)

##### satyar 04/03/2021 13:58:44
I tried but show error the code crash 😢 and didn’t work

##### benj3110 04/03/2021 14:19:20
Thank you! really big help!😁

##### Ali\_1 04/05/2021 03:32:29
Hi, 

How can I move a solid object (with physics) on a path in the  supervisor node?  Since physics is enabled if falls when I simply change its position.

##### DDaniel [Cyberbotics] 04/05/2021 08:28:38
`@Ali_1` if the object is supposed to be entirely moved by the supervisor you need to manually set it's translation/rotation field at every timestep. If instead you want the object to have physics but "float"  then you can insert the solid object as a child of the robot, then give physics to the object itself but not to the parent (the robot)

##### Yi Zhou 04/05/2021 09:33:15
Hello, thanks for your reply, I get my kinect camera by using "getFromDEF" because the kinect camera was defined as DEF. But there is no enable function

##### Pancha 04/05/2021 15:55:19
I've seen that there's plenty of people trying to get a vacuum/suction gripper to work, but have anyone succeeded and have an example? I'm new to Webots, but I am really struggling with getting this to work. Programming in `Python` for this project.



The idea I have been working around is for an arm to extend out and for it to pick a box by the use of a suction gripper (or electromagnet). This seemed simple enough, but I'm just unable to get the gripper and the box to connect.



So far I've created the arm with a `Connector` node as the end effector on the `Robot` node. In order to allow for the suction gripper to be  locked/unlocked  (`lock()` / `unlock()`) to the box on demand, each box also had to be a `Robot` node with a `Connector` node. So far this works as intended. When extending or retracting the robot over the box I'm printing out `getPresence()` for each connector and I see them change from `0` to `1` when they're in the correct position based on the tolerance of the connector nodes. The issue I then have, which I seem to be unable to work around is to actually connect them together. When moving the robot arm the box doesn't follow.



I've followed various online resources and the `connector.wbt` sample world (programmed in C) which have lead me where I am today.



Hope someone has an idea nudging me in the correct direction 🙂

##### tudor 04/05/2021 16:10:01
Hello Webots community, I have a question regarding proper usage of Supervisor.simulationReset() function. My .wbt file initially cotains only a supervisor robot node with a non-void controller (SimulationController.py). The pseudocode of this controller reads as follows:



    for usecase in usecases:

        Supervisor.simulationReset() # step 1.

        Supervisor.step(64)  # step 2. basicTimestep is 64 as well. This, as far as I understand, is required for SimulationManager.py to communicate the simulation reset request to webots. Without this the robots are never loaded successfully

        load\_robots(usecase) # step 3 . Here I am loading various robots from string. Robots have non-void controllers



        #The following loop lets the simulation run until a preset MyTimeout. After MyTimeout is reached, simulation should reset

        while not self.step(self.timestep) == -1 and now <= MyTimeout:

            now = self.getTime()



This loop runs succesfully for the first iteration, then crashes with a segmentation fault during the second iteration, during step 1. Output messages vary sporadically:

Message 1:

munmap\_chunk(): invalid pointer

Message 2:

Assertion 'newindex >= 0 && newindex < (int) flist->size' failed at pulsecore/flist.c:93, function stack\_push(). Aborting.

Or simply:

./webots: line 90: 30204 Aborted                 (core dumped) "$webots\_home/bin/webots-bin" "$@"





My questions are :

1. Am I doing something obviosuly wrong with the way I am trying to reset my simulation?

2. The documentation seems to me a bit vague in specifying exactly what happens to controllers of programatically created robot nodes when calling simulationReset. Am I assuming correctly, that in my case, when calling simulationReset, the controllers of the robots spawned in load\_robots should be killed, whereas the SimulationController.py controller should continue running?



Thanks for reading!

##### reinaldobianchi 04/05/2021 19:35:46
Dear Friends. I have a doubt about the Sensors on the Pioneer AT3. Is the figure on page [https://cyberbotics.com/doc/guide/pioneer-3at](https://cyberbotics.com/doc/guide/pioneer-3at) wrong?


It looks like the sensors that are in front should be in the back...


Sensors 0 to 7 should be in the back, and sensors 8 to 15 in the front, no?


I also think that the Front and Back views of the robot are switched...


I think it is also wrong for the Pioneer 3 DX

##### enbakom ghebremichael 04/05/2021 19:53:08
anyone have  Idea LIGHT\_THRESHOLD = 500

NOSE\_SENSITIVITY = 0.25



%figure
![epuck_con.PNG](https://cdn.discordapp.com/attachments/565154703139405824/828719645582164018/epuck_con.PNG)
%end


what is LIGHT\_THRESHOLD = 500 ?

NOSE\_SENSITIVITY = 0.25   ?  Where can I get any explanation about  that?

##### Simon Steinmann [Moderator] 04/05/2021 20:47:54
look lower in the code and see how it is used

##### enbakom ghebremichael 04/05/2021 21:58:16

%figure
![Capture_11.PNG](https://cdn.discordapp.com/attachments/565154703139405824/828750413578502184/Capture_11.PNG)
%end



%figure
![Capture_12.PNG](https://cdn.discordapp.com/attachments/565154703139405824/828750449691328532/Capture_12.PNG)
%end


what is LIGHT\_THRESHOLD = 500 ?

NOSE\_SENSITIVITY = 0.25   ?  Where can I get any explanation about  that?

##### Simon Steinmann [Moderator] 04/05/2021 22:05:51
it is using those variables to comprehend the distance sensor values


top\_wall and side\_wall are boolean (True or false).

##### enbakom ghebremichael 04/05/2021 22:52:52
Still not clear to me .  LIGHT\_THRESHOLD = 500 ? what is 500 ?  or what is 0.25 ?

##### Simon Steinmann [Moderator] 04/05/2021 22:54:54
values that get compared to the sensor values

##### James Le Poidevin 04/06/2021 13:18:24
Hello, Is it possible to use the supervisor API with ros2 to move an object during a simulation ?

##### Darko Lukić [Cyberbotics] 04/06/2021 13:25:02
Hello `@reinaldobianchi`. Could you please provide a more details?



For example, the `s04` sensor is in the front (the wide side with a black sticker) in the 3D robot model.



This picture matches it:

[https://raw.githubusercontent.com/cyberbotics/webots/released/docs/guide/images/robots/pioneer-3at/wheels.png](https://raw.githubusercontent.com/cyberbotics/webots/released/docs/guide/images/robots/pioneer-3at/wheels.png)



And the following picture as well:

[https://raw.githubusercontent.com/cyberbotics/webots/released/docs/guide/images/robots/pioneer-3at/sonars.png](https://raw.githubusercontent.com/cyberbotics/webots/released/docs/guide/images/robots/pioneer-3at/sonars.png)



It is possible I am missing something though.

##### reinaldobianchi 04/06/2021 13:25:28
Hi.


When using this robot, if you move the viewpoit to the front, you will se the robot's back.


Alsi, in this image, sensor so3 is said to be on the front, but it is on the back.


if you move the robot with positive velocity on the wheels, it will move to the real front of the robot.


but if you use sensor s03 as a front sensor, distance to a wall will increase...


DX and AT

##### Darko Lukić [Cyberbotics] 04/06/2021 13:30:06
Position of the `so3`
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/828984920038178856/unknown.png)
%end

##### reinaldobianchi 04/06/2021 13:30:40
try running this code...


"""bate\_e\_volta controller."""



\# You may need to import some classes of the controller module. Ex:

\#  from controller import Robot, Motor, DistanceSensor

from controller import Robot



\# create the Robot instance.

robot = Robot()



\# get the time step of the current world.

timestep = int(robot.getBasicTimeStep())



motor\_FL = robot.getDevice('front left wheel')

motor\_FR = robot.getDevice('front right wheel')

motor\_BL = robot.getDevice('back left wheel')

motor\_BR = robot.getDevice('back right wheel')



motor\_FL.setPosition(float('inf'))

motor\_FR.setPosition(float('inf'))

motor\_BL.setPosition(float('inf'))

motor\_BR.setPosition(float('inf'))



motor\_FL.setVelocity(6.28)

motor\_FR.setVelocity(6.28)

motor\_BL.setVelocity(6.28)

motor\_BR.setVelocity(6.28)





sensor\_Front = robot.getDevice('so11')

sensor\_Front.enable(timestep)

sensor\_Back = robot.getDevice('so3')

sensor\_Back.enable(timestep)





\# Main loop:

\# - perform simulation steps until Webots is stopping the controller

while robot.step(timestep) != -1:

   valor\_F = sensor\_Front.getValue()

   valor\_B = sensor\_Back.getValue()

   print ("Sensor frontal: %f" % valor\_F)

   print ("Sensor traseiro: %f" % valor\_B)



   pass



\# Enter here exit cleanup code.


And see the values of the frontal and back sensor.


Frontal = front


traseiro = back


using the AT


Pioneer 3 AT

##### Darko Lukić [Cyberbotics] 04/06/2021 13:32:53
Yes, you can extend the `WebotsNode`, see an example:

[https://github.com/cyberbotics/webots\_ros2/blob/99fa4a1a9d467e4ba71eff17ddf4e82444c78938/webots\_ros2\_examples/webots\_ros2\_examples/khepera\_driver.py#L69-L108](https://github.com/cyberbotics/webots_ros2/blob/99fa4a1a9d467e4ba71eff17ddf4e82444c78938/webots_ros2_examples/webots_ros2_examples/khepera_driver.py#L69-L108)

##### Dorteel 04/06/2021 13:34:12
Hi, I have a question regarding supervisor's getNumberofContactPoints() function. I'm trying to do Reinforcement Learning, where the episode ends when the robot collides with itself. But when I use simulationReset() the  number of contact points does not reset but stays the same, so the robot gets stuck in an endless cycle of simulation resets. Does anyone have a workaround for that problem?

##### James Le Poidevin 04/06/2021 13:45:54
I'm not sure I explained properly what i wanted. i want to use the function setSFVec3f(self, values) on a a class that inherited from WebotsNode.

##### Darko Lukić [Cyberbotics] 04/06/2021 13:52:47
I have just properly checked and the orientation is correct. The confusion may come from the fact that the Pioneer robot uses a lookup table for the distance sensors:

[https://github.com/cyberbotics/webots/blob/ab8b93ff30523825a8193327db756f44f12d390c/projects/robots/adept/pioneer3/protos/Pioneer3DistanceSensor.proto#L191-L194](https://github.com/cyberbotics/webots/blob/ab8b93ff30523825a8193327db756f44f12d390c/projects/robots/adept/pioneer3/protos/Pioneer3DistanceSensor.proto#L191-L194)



See details:

[https://cyberbotics.com/doc/reference/distancesensor#lookup-table](https://cyberbotics.com/doc/reference/distancesensor#lookup-table)



Closer the obstacle the higher value is. This is done to imitate the real hardware, although I am not sure how accurate it is at this point.



Feel free to open a pull request and propose changes.

##### reinaldobianchi 04/06/2021 13:55:45
Just another question: why the views are wrong?


if you move to the viewpoint "Front", you will see the back of the P3 AT


And vice versa.

##### Darko Lukić [Cyberbotics] 04/06/2021 13:57:14
Maybe a better example:

[https://github.com/cyberbotics/webots\_ros2/blob/master/webots\_ros2\_tesla/webots\_ros2\_tesla/tesla\_driver.py](https://github.com/cyberbotics/webots_ros2/blob/master/webots_ros2_tesla/webots_ros2_tesla/tesla_driver.py)



So, you would have something like:

```python
class YourDriver(WebotsNode):
    def __init__(self, args):
        super().__init__('tesla_driver', args, controller_class=Supervisor)
        self.start_device_manager()
        field = self.robot.getSelf().getField(...)
        field.setSFVec3f(...)
```

##### James Le Poidevin 04/06/2021 13:58:16
Brilliant thanks 👍

##### Darko Lukić [Cyberbotics] 04/06/2021 14:00:20
`Front View` refers to the world, not to the robot

##### reinaldobianchi 04/06/2021 14:03:21
Ok.


Thanks.

##### Harris 04/07/2021 06:11:53
Hi, maybe a simple question but struggling this~ I am trying to use pycharm to run webots controller in Ubuntu. I follow the tutorial of "Using your IDE" chapter, but it returns out this error

ImportError: libCppController.so: cannot open shared object file: No such file or directory

How can I solve this problem?

##### notjfarhan 04/07/2021 06:13:37
Hello , I wanted to know if there was a webots model for the intel realsense camera .

##### Darko Lukić [Cyberbotics] 04/07/2021 07:10:13
Make sure the `LD_LIBRARY_PATH` is pointing to the correct directory

##### Harris 04/07/2021 07:28:14
Yes it is pointing to the correct one
%figure
![2021-04-07_01-27-56_.png](https://cdn.discordapp.com/attachments/565154703139405824/829256238565949460/2021-04-07_01-27-56_.png)
%end



%figure
![2021-04-07_01-27-34_.png](https://cdn.discordapp.com/attachments/565154703139405824/829256254685708319/2021-04-07_01-27-34_.png)
%end

##### Darko Lukić [Cyberbotics] 04/07/2021 07:37:10
You can check whether the configuration works independent from your IDE:

```bash
export WEBOTS_HOME=/usr/local/webots
export LD_LIBRARY_PATH=${WEBOTS_HOME}/lib/controller
export PYTHONPATH=${WEBOTS_HOME}/lib/controller/python38
python3 UR5e_1.py
```

If it works, then the problem is isolated and you can focus on configuring your IDE.

##### Harris 04/07/2021 08:22:21
Do u mean type these in the IDE terminal? Programs can be started in the terminal but it still not work when I press run. Sorry I am new to PyCharm as well..
%figure
![2021-04-07_02-19-35_.png](https://cdn.discordapp.com/attachments/565154703139405824/829269859526246431/2021-04-07_02-19-35_.png)
%end

##### Darko Lukić [Cyberbotics] 04/07/2021 08:26:13
That's a good sign. Now, you just need to configure PyCharm. Unfortunately, I have close to zero experience with PyCharm.



This tutorial should help:

[https://cyberbotics.com/doc/guide/using-your-ide#pycharm](https://cyberbotics.com/doc/guide/using-your-ide#pycharm)



If you discover a problem in the tutorial feel free to open a PR.

##### Harris 04/07/2021 08:34:27
This is what I followed lol. Actually I once worked this in Windows so maybe Sth goes wrong with Linux tutorial? Anyway thx for your time!

##### James Le Poidevin 04/07/2021 09:59:42
Hello could someone explain this error to me. 
```[ERROR] [driver-3]: process has died [pid 5427, exit code 1, cmd '/workspace/planetary-simulator/external_ros_controleur/install/webots_ros2_rosalind_franklin/lib/webots_ros2_rosalind_franklin/driver --webots-robot-name  --webots-node-name webots_driver --ros-args --params-file /tmp/launch_params_glzi3e0_'].
[seram_0_2.wbt" --batch --no-sandbox --mode=realtime-2] /usr/local/webots/webots: line 88:  5434 Bus error               (core dumped) "$webots_home/bin/webots-bin" "$@"
[ERROR] [seram_0_2.wbt" --batch --no-sandbox --mode=realtime-2]: process has died [pid 5425, exit code 135, cmd '"/usr/local/webots/webots" "/workspace/planetary-simulator/external_ros_controleur/install/webots_ros2_rosalind_franklin/share/webots_ros2_rosalind_franklin/worlds/seram_0_2.wbt" --batch --no-sandbox --mode=realtime'].
[INFO] [robot_state_publisher-1]: sending signal 'SIGINT' to process[robot_state_publisher-1]
[robot_state_publisher-1] [INFO] [1617789338.941998681] [rclcpp]: signal_handler(signal_value=2)
[INFO] [robot_state_publisher-1]: process has finished cleanly [pid 5423]
```

It happens after there is a mistake in my external controleur(i.e forgot a " or indentation error). After the mistake i cannot use webots again unless i reboot my PC.

##### [DER]Mad[MAN] 04/07/2021 12:14:26
Hello there ! I have a question about a detail from Webots ros2 sensors messages. It seems that they are not reliable, I need to switch rviz2 topic subscription to best\_effort to be able to visualize pointcloud (for example). Is this normal? Is there a way to get reliable messages?

##### Darko Lukić [Cyberbotics] 04/07/2021 12:22:02
The newest version of `webots_ros2` (available in the master branch) uses `qos_profile_sensor_data` QoS for all sensors.


I suppose your ROS 2 controller has crashed and it crashed the Webots instance

##### [DER]Mad[MAN] 04/07/2021 12:26:26
Okay, thanks. I'll check, but I thought I had the latest version of the package

##### Westin 04/07/2021 22:14:23
Is there a way to batch import STL files?

##### Olivier Michel [Cyberbotics] 04/08/2021 06:02:46
You can use the Mesh node to import STL files, see [https://cyberbotics.com/doc/reference/mesh](https://cyberbotics.com/doc/reference/mesh)

##### Pancha 04/08/2021 09:06:54
I've seen that there's plenty of people trying to get a vacuum/suction gripper to work, but have anyone succeeded and have an example? I'm new to Webots, but I am really struggling with getting this to work. Programming in `Python` for this project.



The idea I have been working around is for an arm to extend out and for it to pick a box by the use of a suction gripper (or electromagnet). This seemed simple enough, but I'm just unable to get the gripper and the box to connect.



So far I've created the arm with a `Connector` node as the end effector on the `Robot` node. In order to allow for the suction gripper to be  locked/unlocked  (`lock()` / `unlock()`) to the box on demand, each box also had to be a `Robot` node with a `Connector` node. So far this works as intended. When extending or retracting the robot over the box I'm printing out `getPresence()` for each connector and I see them change from `0` to `1` when they're in the correct position based on the tolerance of the connector nodes. The issue I then have, which I seem to be unable to work around is to actually connect them together. When moving the robot arm the box doesn't follow.



I've followed various online resources and the `connector.wbt` sample world (programmed in C) which have lead me where I am today.



Hope someone has an idea nudging me in the correct direction 🙂

##### Darko Lukić [Cyberbotics] 04/08/2021 09:17:01
If you are comfortable with C/C++ you can try to implement it in the Webots core:

[https://github.com/cyberbotics/webots/issues/2889#issuecomment-804073941](https://github.com/cyberbotics/webots/issues/2889#issuecomment-804073941)



Another idea is to use the Supervisor robot. On each timestep you measure a distance between an object and the suction cup. Once it gets close enough, you stick the object to the suction cup by changing the position/orientation of the object at each timestep depending on the position of the suction cup.

##### Pancha 04/08/2021 09:18:14
Cheers, I’ll try this out 😁👍

##### tudor 04/08/2021 10:26:40
Could anyone please clarify: What happens to controllers of robots created during the simulation, when using simulationReset? Am I right to assume that the get killed by the webots main process?

##### Darko Lukić [Cyberbotics] 04/08/2021 10:50:58
Nothing, you have to reset each controller explicitly:

[https://cyberbotics.com/doc/reference/supervisor?version=develop#resetreload-matrix](https://cyberbotics.com/doc/reference/supervisor?version=develop#resetreload-matrix)

##### tudor 04/08/2021 11:12:35
Thanks for the prompt response. Don't you find this behaviour a bit weird ? Say the supervisor adds a robot node with a supervisor at t=10s (using importMFNodeFromString). At t=100s the supervisor decides to reset the simulation, ie bring it to t=0s, where there is no added robot node yet. But the controller of that robot which is not existing anymore is still running? Even if I reset this controller manually from the supervisor, what will it do running from t=0, when it has no robot node to operate on(as again, supervisor will try to create that robot only at t=10s)?

##### Yi Zhou 04/08/2021 11:14:27
Hello everyone, I have got the image array by using python function "getImageArray" from camera, but when I try to show image array by using Pillow, it did not show the image that camera shows on webots, does anyone know what's the reason?

##### Darko Lukić [Cyberbotics] 04/08/2021 11:20:10
That function returns list of lists in Python. You should use `getImage` instead, see here:

[https://github.com/lukicdarkoo/webots-example-lane-follower/blob/3ca866b0c41b65faa01d3d998de062c941680708/controllers/lane\_follower/lane\_follower.py#L17-L18](https://github.com/lukicdarkoo/webots-example-lane-follower/blob/3ca866b0c41b65faa01d3d998de062c941680708/controllers/lane_follower/lane_follower.py#L17-L18)

##### Yi Zhou 04/08/2021 11:21:36
Thank you so much, I'll try👍

##### Stefania Pedrazzi [Cyberbotics] 04/08/2021 11:33:05
Yes, if a robot is created during the simulation, then it is deleted on simulation reset and its controller is automatically killed by Webots.

Controllers of robots that are part of the original world and won't be deleted on simulation reset continue to run.

##### tudor 04/08/2021 12:07:34
Thank you very much, makes sense! As a followup question, am I right to assume that when the supervisor is calling wb\_simulation\_reset, the reset is actually happening only after also calling the step(basicTimestep), because only then the supervisor controller communicates to the main webots process?

##### Stefania Pedrazzi [Cyberbotics] 04/08/2021 12:16:33
The `wb_supervisor_simulation_reset()` request is sent immediately to the Webots main process but it is executed at the end of the ongoing simulation step. So if your robots have the `synchronization` field set to TRUE, yes you can assume that the reset happens when all the controllers call the step(basicTimestep) instruction.

##### Westin 04/08/2021 16:42:18
Can the mesh node only be used in a PROTO file? I'm not seeing it in the scene tree nodes
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/829758062306525204/unknown.png)
%end

##### DDaniel [Cyberbotics] 04/08/2021 16:46:51
`@Westin` need to add a Shape child first

##### jasonc1025 04/09/2021 01:25:33
Hi, this is my first time posting where I could use your help.  Yet as a JrHi/SrHi teacher, I'm excited about Webots as a solution in providing Robotics Education... online esp. during this pandemic season.  I've used a few sim tools (e.g. VRT, RVW) in the past, yet I appreciate the open-source, real-time responsiveness, and web-based features (e.g. RobotBenchmark.net) of Webots.  I need your advise as best practice on sending my students a Zipped File of the 'controllers/protos/worlds' directories - so they can open the .wbt file to test/mod that lesson's sim (kindly see a screenshot of a simple prototype below).  Yet I'm finding that what I create on Win does not load correctly on a Mac, and vice-versa (e.g. a 'Matlab' path issue: 'WARNING: To run Matlab controllers, you need to install Matlab 64-bit and ensure it is available from the DOS CMD.EXE console. ...  Warning: ... failed to start:  !).   What seemed to work is exporting VRML97 - as well as - copy/paste the Controller code into 'File: New World' for that respective Win/Mac platform.  Thus, I need to have *both*  a Win and Mac version for every zip-file-sim-lesson.  Any recommended process to have just 'one' zip file, yet be cross-platform friendly?  Sorry for the long text.  I'm such a newb.  😉
%figure
![2021-04-08_17-40-46.png](https://cdn.discordapp.com/attachments/565154703139405824/829889744669507604/2021-04-08_17-40-46.png)
%end

##### thonk enthusiast 04/09/2021 05:52:12
Ah so the wbt file doesn’t work cross-platform?

##### Olivier Michel [Cyberbotics] 04/09/2021 06:24:36
Everything should work cross-platform, especially if your controllers are interpreted language (MATLAB or Python) or Java. For C and C++ controllers, you will need to recompile them from the source, but that's very easy from the Webots IDE.

##### thonk enthusiast 04/09/2021 06:30:34
`@jasonc1025` could you post what errors/warnings you get when you try and open it?

##### Pancha 04/09/2021 14:13:34
Is there a way through code to reset the console if `Reset Simulation` button is pressed in Webots except for manually right clicking the console and selecting `Clear Console`? 🙂

##### AleBurzio 04/09/2021 14:22:57
Hello! Is there any way I can acquire the simulation time, aside from counting the simulation steps and multiplying by the world time step?

##### Darko Lukić [Cyberbotics] 04/09/2021 14:24:04
[https://cyberbotics.com/doc/reference/robot#wb\_robot\_get\_time](https://cyberbotics.com/doc/reference/robot#wb_robot_get_time)

There is the `wb_robot_get_time()` function


You can use the `CTRL` + `K` shortcut instead of the clicking

##### jasonc1025 04/09/2021 20:08:10
Hi Olivier `@Olivier Michel` and '! Thonk enthusiast' `@thonk enthusiast` , Thanks for the helpful, timely responses.  Oliver's advice helped tremendously, as the C-based controllers do need a rebuild - via the convenient 'Build' Button - esp. when switching between Win & Mac.  



I only ran into one 'corner case' where the solution may involve a little more TLC, since the sample 'MyBot' - that I'm learning from - features multi-directory controllers: 1) Simple, 2) Camera and 3) Physics.  The first two rebuilt ok, yet the third had difficulty finding the desired controller file for re-building.  To minimize this post from becoming a 'thousand words', a screenshot/picture is being posted.  Thx again for any supportive time on this. 🙂
%figure
![2021-04-09_11-49-11.png](https://cdn.discordapp.com/attachments/565154703139405824/830172259682877480/2021-04-09_11-49-11.png)
%end

##### rendermite\_ 04/09/2021 20:11:17
<:tSmile:816483891275759656>

##### thonk enthusiast 04/09/2021 20:51:00
uh, what exactly do you mean by 'TLC'?

##### jasonc1025 04/09/2021 20:51:19
Here is a second follow-up where I could use Olivier's and anyone else's help... 🙂



Question 1: Thx `@Olivier Michel`  to suggest that Python could be a solution to bypass this extra-step of manual-rebuilding, esp. when trying to provide a zipped .wbt package to students with either Win or Mac.   I like the idea, yet are there any Python Samples to learn from - similar to the following C Samples: 1) MyBot (nice & simple .wbt & controller file), 2) 4\_Wheels\_Robot (another nice & simple .wbt & controller file), and/or 3) Surveyor (nice tele-op via keyboard - since kids enjoy remote-control action - just as much as autonomous-mode ;).  



Question 2: For my younger students who are still using 'block' programming, would it be advised that I just provide two downloadable zipped-packages - Win and Mac - and they just download, unzip and double-click the .wbt file (assuming they already have Webots installed)?   These are the same students who love the one-URL/Browser approach of RobotBenchmark.net (of which I'm also excited about and could use some help later - exploring the idea of sharing teacher-developed missions via the same platform, as a full curriculum-unit).  Believe me, teaching online under this pandemic has been draining - and I would celebrate the opportunity of not having to worry/support downloadable apps among a hundred+ JrHigh/SrHigh students with various computer types (Win, Mac, iOS, Android, etc.). 😉



Our budget is limited - as we *also* seek to reach out to 'under-resourced' students (feel free to verify our authenticity as I serve as Lead for open-source-based RoboQuest Robotics (second-listed program on [https://questforspace.com/quest-portal/](https://questforspace.com/quest-portal/)), yet I'll be happy to seek my Upper Admin to insure a win-win for everyone.  (Thx for anyone's support towards such a long post.  Now, off to more curriculum-dev.)  😉


TLC = Tender Loving Care... 😉

##### thonk enthusiast 04/09/2021 20:52:56
Ahh, ok.

##### rendermite\_ 04/09/2021 20:54:27
so is this two separate programs? or is this one program for the same set of students?

##### OwOcelot 04/09/2021 20:55:48
What do you mean by “python”


o7

##### jasonc1025 04/09/2021 21:04:26
Thx `@rendermite_` & `@OwOcelot` (cc: `@Olivier Michel`) for asking.  If I understand the question properly, any one given class can have students with a Win, Mac, or iPad (iOS).   Since I don't have enough skill-set to leverage the super-cool 'Web-Sim', we'll need the students to download the Webot desktop on their respective device, as well as download our Sim-Lessons per Zipped-.wbt Package (of which I'm seeking Olivier and others for best-practices advice, such as using *Python* language as a more turn-key solution to 'C' for younger students [no need to rebuild]).   Hope this makes sense. 🙂

##### thonk enthusiast 04/09/2021 21:05:46
Also,
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/830186756765646939/unknown.png)
%end


did you mean "Microsoft"? `@jasonc1025`

##### OwOcelot 04/09/2021 21:06:45
`@meww` please dont joke around, this is a serious discord server


I think its a documentary

##### rendermite\_ 04/09/2021 21:08:10
ah, so the following class has students that have windows, IPADOS and Mac. In that case i think its better to give the two downloadables only if the staff can successfully manage to keep the class running smoothly even with multiple operating systems. Hope that helps <:tWink:821238815078219826>


ipad os = IOS for the ipad after IOS 13 I belive


[https://en.wikipedia.org/wiki/IPadOS](https://en.wikipedia.org/wiki/IPadOS)

##### OwOcelot 04/09/2021 21:09:33
Yeah, ipad os but how about ipa dos

##### rendermite\_ 04/09/2021 21:10:23
np <a:ANIwelcome:736304466080301176>

##### jasonc1025 04/09/2021 21:13:57
`@thonk enthusiast` Nice observation.  Amazingly enough, we have many volunteer-students helping with our non-profit company - such as web-dev.  So it appears that they haven't gotten to our spelling-correction request. 😉  I'll follow-up with them again.  🙂

##### thonk enthusiast 04/09/2021 21:15:34
> web-sim

I actually tried setting up the Web Simulation the other day, it wasn't *too* hard but the reverse proxy (for SSL + production deployments) was a bit tricky too


[https://cdn.discordapp.com/emojis/693723538552782888.png?size=64](https://cdn.discordapp.com/emojis/693723538552782888.png?size=64)


your experience may vary though

##### rendermite\_ 04/09/2021 21:17:34
alright. also, could you tell more about your "project?". I'm passionate about helping elementry and JH peoples, and want to see if I can help with this is any way

##### OwOcelot 04/09/2021 21:17:43
`@meww` i think you sent the wrong emojis for your reactions, you might have misclicked

##### thonk enthusiast 04/09/2021 21:18:25
He called his project "Roboquest" which is the second row on that link he sent to questforspace


It looks interesting

##### rendermite\_ 04/09/2021 21:18:40
hmm


I have experience helping out others in Robotics in my organization as well


`@thonk enthusiast` mind if we go into DM's?

##### thonk enthusiast 04/09/2021 21:20:06
Uhh yeah sure


`@rendermite_` Check your friend requests

##### rendermite\_ 04/09/2021 21:20:40
aight thxx

##### OwOcelot 04/09/2021 21:20:48
I think you have to click alt+f4


No need to get angry


Its the... (guys, whats the altf4 equivalent on mac)

##### onecelerysticc 04/09/2021 21:22:14
I don’t use macs a lot, perhaps you should ask someone else?


Thats not very nice, I do not have experience in this field, perhaps you should listen to Ow Ocelot


Oh but I saw you fixed it? I guess you don’t need help anymore

##### OwOcelot 04/09/2021 21:25:43
<@&605343389285744640> whats with the trolls lately? Pls fix


Oh, im sorry


Please take a cookie as a sign of my apology 🍪

##### onecelerysticc 04/09/2021 21:26:45
Perhaps you should go to a doctor?

##### rendermite\_ 04/09/2021 21:27:52
`@meww` this is a serious server, please leave if you are not here to contribute to the webot community, thank you

##### OwOcelot 04/09/2021 21:28:32
Your*


Please stop trolling

##### onecelerysticc 04/09/2021 21:29:30
I’m not too sure with doctor terminology, but I think you could try a podiatrist? I’m not too sure if thats the correct one though

##### rendermite\_ 04/09/2021 21:30:40
says the person who doesnt have one

##### onecelerysticc 04/09/2021 21:30:48
:0

##### thonk enthusiast 04/09/2021 21:31:34
`@rendermite_` You shouldn't engage with the trolls they'll target you

##### rendermite\_ 04/09/2021 21:31:42
youre right


<@&605343389285744640> please do something about this situation

##### OwOcelot 04/09/2021 21:32:32
Just block them guys

##### rendermite\_ 04/09/2021 21:33:09
i already did that

##### thonk enthusiast 04/09/2021 21:33:10
Yeah I did that already

##### OwOcelot 04/09/2021 21:33:19
Kk


The moderation staff on this server really needs to step up and stop the trolls

##### thonk enthusiast 04/09/2021 21:34:22
I mean they probably don't have to deal with trolls often, this is a kinda niche community

##### rendermite\_ 04/09/2021 21:34:27
yeah the mods here are pretty incompetent


yeah


the server seems dead as well

##### thonk enthusiast 04/09/2021 21:34:46
not to discredit them but


[https://cdn.discordapp.com/emojis/693723538552782888.png?size=64](https://cdn.discordapp.com/emojis/693723538552782888.png?size=64)

##### Simon Steinmann [Moderator] 04/09/2021 21:38:51
`@meww` This is a technical support server for Webots. Please refrain from using profane language, insulting people and try to conduct yourself in a respectful manner. Otherwise we'll have to ban you.


oh and `@rendermite_` this was literally the first instance ever I have seen of a troll here, for the year I have been active here

##### rendermite\_ 04/09/2021 21:54:48
oh wow ok

##### thonk enthusiast 04/09/2021 21:54:57
oh wow

##### rendermite\_ 04/09/2021 21:55:03
yeah i never expected trolls in this place

##### thonk enthusiast 04/09/2021 21:55:06
+1

##### rendermite\_ 04/09/2021 21:55:21
hopefully this doesnt happen again

##### thonk enthusiast 04/09/2021 21:57:00
Yeah probably not

##### jasonc1025 04/10/2021 00:08:40
Given `@Olivier Michel`  'scope-of-responsibility' in Webots, I appreciate your valuable time  - even via your kind 'thumb-up' emoji - to confirm that I should be in the right direction re: Rebuilding for Win/Mac Controller Code in C.  Now to hopefully learn more about the Python option. 😉


After further browsing, I ran into this exciting news 'The sample players as well as the "automatic referee" are implemented in *Python*, which should allow for easily updating the code to match the rules and *avoid any compilation issues*.' from [https://robocupjuniortc.github.io/rcj-soccer-sim/](https://robocupjuniortc.github.io/rcj-soccer-sim/).  Perhaps this is a good model to learn how to code in Python for Webots, plus I've been searching for an alternative robotics competition for my students (trying to wean off of FLL [Lego EV3]). 😉  Any other suggested Webots Sims in Python to explore?

##### diluccockballs 04/10/2021 00:14:38
very interesting


maybe you ought to implement a *rules channel*

##### Olivier Michel [Cyberbotics] 04/10/2021 08:49:54
For graphical programming of robots in Webots, you might be interested to check this out: [https://www.stormingrobots.com/prod/dev2.html](https://www.stormingrobots.com/prod/dev2.html) (especially the first project with Blockly programming in Webots).

##### Stormtrooper 04/10/2021 12:22:02
(Sorry for posting multiple times, i posted the wrong picture at first)

I'm having trouble with measuring how much the robot's CG is shifting, i need this data to measure the robot's stability margin. I've "borrowed" the code from the  center\_of\_mass.wbt sample from the 'how to' page, but the data produced is very small, even after multiplying by 1000 (i need the data in mm). The data output does seem to make sense though, since it is changing proportionally to the change in my robot's direction.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/830417340512206878/unknown.png)
%end

##### alireza1992 04/10/2021 13:47:34
hi, 

I have a humanoid robot, op3;

I want to know its joint's velocity.

I know that we can read the angular position of each joint from 'getPositionSensor("motor's name") ' ; I want what like this.

I don't want to use gyro sensor in my joints, because gyro get angular velocity respect to global coordinate, so I should do some summations and subtractions to compute the relative angular velocity.

how can I do it?

thanks 🙏

##### Bitbots\_Jasper [Moderator] 04/10/2021 13:49:49
The motor device has a function called getVelocity()


[https://cyberbotics.com/doc/reference/motor?tab-language=python#wb\_motor\_get\_velocity](https://cyberbotics.com/doc/reference/motor?tab-language=python#wb_motor_get_velocity)

##### alireza1992 04/10/2021 13:51:50
Thank you for your quick response 🙏 🙏 🙏 .

I need to get current velocity of a joint. However getVelocity() returns the velocity that is specified by setVelocity() method or the maxVelocity parameters in Proto file.

##### Bitbots\_Jasper [Moderator] 04/10/2021 15:53:52
you can do getPosition at each timestep and calculate the difference in position/timestep to get the velocity

##### jasonc1025 04/10/2021 18:02:12
Thx again `@Olivier Michel` for your precious time to support the students/families of which I serve (btw, in 'Silicon Valley', CA).  First Python, and now Blockly... sweet.  As an appreciation for this highly supportive community - as led by Olivier and Crew - I ran into an amazing tutorial for the RoboCupJunior Soccer Sim in Webots... using Python!  This is the best Python tutorial that I've seen so far, where even the Supervisor concept is introduced.  (Based on the educational-quality of this tutorial, perhaps this was developed in-house by Webots?)  😉  And what's so cool about this Webots Soccer Sim... it works turnkey style - unlike other Sim Engines that requires so much more setup/dependencies 'TLC'.  😉  Here is the link: [https://robocupjuniortc.github.io/rcj-soccer-sim/how\_to/](https://robocupjuniortc.github.io/rcj-soccer-sim/how_to/)  Cheers.  🙂

##### thonk enthusiast 04/10/2021 19:11:13
So, if I’ve gotten this correct, you’re planning a competition style simulation with Blockly and python using Webots?

##### jasonc1025 04/10/2021 19:37:42
It's a key way to get Jr Highers (younger and older) motivated to learn, yet we wish also emphasize team-vs-team (so no solo lone rangers). 😉

##### rendermite\_ 04/10/2021 19:40:06
thats pretty nice, since Junior Highers like competition. I think its a great idea to motivate students and make the learning process more enjoyable <:tWink:821238815078219826>

##### jasonc1025 04/10/2021 19:57:05
just for fun, no obligation, yet it's kind of nice to learn more about the community here.  I'm from 'Silicon Valley' (South Bay) in CA, USA.  Where are you `@thonk enthusiast`  and `@rendermite_`  from(?), assuming i'm not just talking to 'smart bots'.  😉  I assume that we're from all parts of this world?  🙂

##### diluccockballs 04/10/2021 21:23:43
[https://cdn.discordapp.com/emojis/771407331489087569.png?size=64](https://cdn.discordapp.com/emojis/771407331489087569.png?size=64)

##### rendermite\_ 04/11/2021 04:38:33
I’m a student residing in the United States

##### thonk enthusiast 04/11/2021 04:39:48
Yeah, I'm also a student


`@rendermite_` Where in the US? If you're in the west coast go to DMs

##### Ali\_1 04/11/2021 05:08:50
Hi, is it possible to change the cursor style in 3D view?

##### rendermite\_ 04/11/2021 05:11:06
oh you live in west coast as well? Aight check dms

##### Supernova 04/11/2021 08:45:10
Hey! Is it possible to use path planning in webots?

##### umairnaseer21 04/11/2021 14:49:17
Hi, how can I import a 3D model in webot along with joints and other stuff?

##### Bitbots\_Jasper [Moderator] 04/11/2021 14:51:13
webots itself does not have path planning algorithms implemented but you can either implement them yourself or (preferably) use existing implementations

this may be a good refernce: [https://github.com/AtsushiSakai/PythonRobotics/tree/master/PathPlanning](https://github.com/AtsushiSakai/PythonRobotics/tree/master/PathPlanning)

and if you are already using ros then the ros navigation stack is a good option


what format is your model in?

##### umairnaseer21 04/11/2021 14:51:53
Secondly any material for coding. Actually i am working on project and don't have any idea about coding


Its in stl filw


File


There is a tool, robotic arm pick it and put it in AGV, then AGV moves and again robotic arm pick it and put it on convayer belt.


This is the case scenario.

##### Bitbots\_Jasper [Moderator] 04/11/2021 14:54:09
firstly, check if your robot is already included in webots [https://cyberbotics.com/doc/guide/robots](https://cyberbotics.com/doc/guide/robots)

##### umairnaseer21 04/11/2021 14:55:05
I think we can use universal robotic system.


For AGV, i have designed an AGV

##### Bitbots\_Jasper [Moderator] 04/11/2021 14:55:58
creating a robot model is quite a long task and an stl does not have all the information that a robot model needs such as the pose of the joints

##### umairnaseer21 04/11/2021 14:56:33
No i want to instal the tool only


And AGV

##### Bitbots\_Jasper [Moderator] 04/11/2021 14:57:20
agv = autonomous guided vehicle ? so just a mobile robot?

##### umairnaseer21 04/11/2021 14:57:31
Yes


Universal robot is already present


In library

##### Bitbots\_Jasper [Moderator] 04/11/2021 14:58:20
do you have a urdf for the mobile robot?

##### umairnaseer21 04/11/2021 14:58:21
Universal robotic arm.


No

##### Bitbots\_Jasper [Moderator] 04/11/2021 14:58:49
do you have a CAD model?

##### umairnaseer21 04/11/2021 14:58:50
Its again designed in fusion 360


Yes

##### Bitbots\_Jasper [Moderator] 04/11/2021 15:01:15
if the model is quite complex it can be a good idea to automate the export, you could use [https://github.com/syuntoku14/fusion2urdf](https://github.com/syuntoku14/fusion2urdf) in combination with [https://github.com/cyberbotics/urdf2webots](https://github.com/cyberbotics/urdf2webots) to get a proto model


if it is not that complex it is probably quicker to model it yourself using the proto language

##### umairnaseer21 04/11/2021 15:02:21
Proto language?

##### Bitbots\_Jasper [Moderator] 04/11/2021 15:02:44
there is a tutorial [https://cyberbotics.com/doc/guide/tutorial-7-your-first-proto](https://cyberbotics.com/doc/guide/tutorial-7-your-first-proto) and an extensive documentation [https://cyberbotics.com/doc/reference/proto](https://cyberbotics.com/doc/reference/proto) on how to create these proto models


proto is the format used by webots for robot models

##### umairnaseer21 04/11/2021 15:03:59
Secondly i need to write a script in python. Any source of help?


<@321244789527805964>  thankyou soo much Man.


I was bit worried because i don't have much time. But you really helped me alot.

##### Bitbots\_Jasper [Moderator] 04/11/2021 15:08:04
This seems to be a quite complex scenario you are trying to build. Could you elaborate a bit more on how robust this has to be? should the arms just perform preprogrammed motions or do you need to do adaptive motions with some sensing of the part that is being picked up?

##### umairnaseer21 04/11/2021 15:09:59
It is like one cube is placed on table and robot will pick it and put it in AGV.  That AGV travels and reach a place where that part will be picked up by robotic arm.


I need to install a gripper as well


If i could get some tutorial related to this seen. Then it will be very easy. But i could not find on youtube.

##### Bitbots\_Jasper [Moderator] 04/11/2021 15:45:11
"should the arms just perform pre-programmed motions or do you need to do adaptive motions with some sensing of the part that is being picked up?" is really the important question, if you have to do motion planning it becomes much more complex

##### umairnaseer21 04/11/2021 15:45:56
No, its just simple motion


Without sensors

##### Beginner26798 04/11/2021 15:46:27
Does anyone have the UR10e robotic arm code please

##### Bitbots\_Jasper [Moderator] 04/11/2021 15:48:01
there are some written tutorials in the documetation, I believe they are a good starting point


is this what you are looking for: [https://github.com/cyberbotics/webots/blob/master/projects/robots/universal\_robots/controllers/ure\_can\_grasper/ure\_can\_grasper.c](https://github.com/cyberbotics/webots/blob/master/projects/robots/universal_robots/controllers/ure_can_grasper/ure_can_grasper.c) ?

##### umairnaseer21 04/11/2021 15:49:00
I think yes


Thankyou soo much.


I will start with this.

##### Beginner26798 04/11/2021 15:50:25
Can someone help me please

##### Bitbots\_Jasper [Moderator] 04/11/2021 15:50:49
i just replied to you

##### umairnaseer21 04/11/2021 15:51:11
This is for you as well.

##### Beginner26798 04/11/2021 15:51:12
Oh sorry i didn't see that thanks

##### Stormtrooper 04/11/2021 15:52:01
Does anyone know how to obtain the distance traveled by a robot's COM during turning/movement (relative to its current position)?

##### Bitbots\_Jasper [Moderator] 04/11/2021 15:55:14
what sensors do you have available? do you want to use a supervisor controller (something that you will not have in the real world and is therefore "cheating")

##### Stormtrooper 04/11/2021 15:58:47
Thanks for replying, the robot has Lidar, a distance sensor, accelerometers, GPS, and IMU. And yes i am willing to use a supervisor controller. i miight have it already as i "borrowed" the code from the center\_of\_mass.wbt sample from the how to page. but theres not that much documentation so i dont really understand whats going on that well

##### Bitbots\_Jasper [Moderator] 04/11/2021 16:04:10
you have a few options: 

1. for the position you can use the GPS sensor [https://cyberbotics.com/doc/reference/gps](https://cyberbotics.com/doc/reference/gps), for orientation you can use your IMU [https://cyberbotics.com/doc/reference/inertialunit](https://cyberbotics.com/doc/reference/inertialunit)

2. you could also calculate the odometry [https://en.wikipedia.org/wiki/Odometry](https://en.wikipedia.org/wiki/Odometry) based on how much you wheels have turned (if you have a wheeled robot?)

3. you can request the fields "translation" and "rotation" from the robot node using the supervisor controller [https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_get\_field](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_get_field)

##### Stormtrooper 04/11/2021 16:18:31
Thanks again for replying, i dont think i quite understand how these options may help me. sorry but I may have phrased the question wrongly. i meant to ask for the distance traveled by a robot's CG from its COM during turning in mm. As in, the robot's CG should be on top of its COM during constant velocity movement, but when turning the CG should be offset from the COM by a certain distance. i meant to ask how to find this distance?

##### Maurice Faraj 04/11/2021 16:21:23
hello everyone


I need a ur10e code with an attached camera on its endpoint can anyone help me please

##### Bitbots\_Jasper [Moderator] 04/11/2021 16:22:16
ok i assumed you have a mobile robot with a static center of mass, could you specify what kind of robot you are using?

##### Stormtrooper 04/11/2021 16:28:26
I am using a custom made robot, its meant to simulate a robot ill be building soon.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/830841736188002364/unknown.png)
%end

##### Bitbots\_Jasper [Moderator] 04/11/2021 16:29:41
it seems like the com is not moving relative to the robot so you can take the reading of the imu and gps at the beginning of the motion and at the end and the difference between them is how much your robot has moved

##### Maurice Faraj 04/11/2021 16:33:52
I'm trying to control this can any one help me please its a ur10e that have a camera in the place of the last link
%figure
![camera.png](https://cdn.discordapp.com/attachments/565154703139405824/830843104097468436/camera.png)
%end

##### Beginner26798 04/11/2021 16:36:06
Nice Idea 👍 help him please

##### Stormtrooper 04/11/2021 16:47:22
sorry that was a unrepresentative screenshot of the data, here is a better representation of the data. it shows the CG movement data while in uniform travel, straight line braking, and turning. but the data output is very small? though the change does seem to correlate with the change in direction
%figure
![2fyp4.png](https://cdn.discordapp.com/attachments/565154703139405824/830846503006240788/2fyp4.png)
%end

##### Supernova 04/11/2021 17:17:36
thank you, i will look into that ❤️

##### jasonc1025 04/11/2021 19:26:03
Thanks also `@Bitbots_Jasper` for the Python lead.  For the WeBots Team: might there be plans to build a 'simple' version of path-planning, within WeBots API set, as an educational tool for Jr/Sr High Students & even Adults - like myself - to experiment & learn from?  I understand this is a tall-order, since as an indie-game-developer, path-planning for enemy-bots is a bonus, advanced feature to experience.  So no rush in replying to this   😉


`@Bitbots_Jasper` wow, you rock :).  Your 'Tutorial-7-Your-First-Proto' lead brought me to the earlier lesson:  [https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot?tab-language=python](https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot?tab-language=python), which helps me convert an existing C-Coded Controller into Python, to avoid the need for rebuilding in a Win/Mac cross-platform classroom-environment (a topic I had posted earlier).  Even all 5 languages are *clearly* featured.  Can't wait to have my Jr/Sr High students to start looking at this.  Super-rocking.  🙂
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/830894796981403679/unknown.png)
%end

##### alireza1992 04/12/2021 08:35:13
Hi everybody;

I have an OP3 robot, I want to access to one of its link with supervisor, for example its shoulder, is it possible? how can I do it?

How can I use "supervisor.getFromProtoDef()" in python?

##### Darko Lukić [Cyberbotics] 04/12/2021 09:26:29
The `getFromProtoDef` is a member method of a node, so you should get the node first. For example, `supervisor.getFromDef('MY_ROBOT').getFromProtoDef(...`

##### alireza1992 04/12/2021 10:02:39
Thank you🙏

##### Harris 04/12/2021 12:46:05
Hi, I am doing collision check in a specific situation. Stage 1 is detect a moving robot arm is collided with other objects or not, and stage 2 is that detect collision of a moving robot with a connected solid(use connector). I use touch sensor function which has to be called with a robot controller. Is there any way to do stage 2 collision check without adding a robot node/controller to the connected solid?

##### Yi Zhou 04/12/2021 14:40:29
Hello everyone, I have tried to use ikpy package in my python script, and there is a demo about how to use it in webots, but when I try to use getUrdf to get my link, the number of link I got is incorrect. Could anyone tell me if you have better way to use ikpy in webots?

##### Darko Lukić [Cyberbotics] 04/12/2021 14:47:47
Hello, you can define links manually, see:

[https://github.com/cyberbotics/webots/blob/d7ecc8022d19c00242781a0edb173c6b494c8b84/projects/robots/abb/irb/controllers/inverse\_kinematics/inverse\_kinematics.py#L31-L68](https://github.com/cyberbotics/webots/blob/d7ecc8022d19c00242781a0edb173c6b494c8b84/projects/robots/abb/irb/controllers/inverse_kinematics/inverse_kinematics.py#L31-L68)



Do you get less or more links with `getUrdf`? The link is exported if the corresponding Webots solid node has a name.

##### Yi Zhou 04/12/2021 14:48:38
Hi, actually, I got less link

##### Darko Lukić [Cyberbotics] 04/12/2021 14:52:09
Hello, maybe this can help:

[https://cyberbotics.com/doc/reference/supervisor?tab-language=python](https://cyberbotics.com/doc/reference/supervisor?tab-language=python)



Alternatively, you can create a physics plugin:

[https://www.cyberbotics.com/doc/reference/physics-plugin](https://www.cyberbotics.com/doc/reference/physics-plugin)


You should be able to solve it by adding more solid nodes

##### Yi Zhou 04/12/2021 14:54:59
Could you please tell me how I can add more solid nodes?

##### Darko Lukić [Cyberbotics] 04/12/2021 14:56:39
Did you model the robot in Webots?

##### Yi Zhou 04/12/2021 15:03:20
the robot can show by webots


Actually, I have 7 link solid nodes

##### Beginner26798 04/12/2021 17:14:57
Hi , I'm trying to inegrate a camera and I'm using the code posted on the webots site,but i m getting this error what should i do
%figure
![20210412_201230.jpg](https://cdn.discordapp.com/attachments/565154703139405824/831215832293703710/20210412_201230.jpg)
%end


// File:          discordtest.cpp

// Date:

// Description:

// Author:

// Modifications:



// You may need to add webots include files such as

// <webots/DistanceSensor.hpp>, <webots/Motor.hpp>, etc.

// and/or to add some other includes

\#include <webots/Robot.hpp>

\#include <webots/Camera.hpp>

// All the webots classes are defined in the "webots" namespace

using namespace webots;



// This is the main program of your controller.

// It creates an instance of your Robot instance, launches its

// function(s) and destroys it at the end of the execution.

// Note that only one instance of Robot should be created in

// a controller program.

// The arguments of the main function can be specified by the

// "controllerArgs" field of the Robot node

int main(int argc, char **argv) {

  // create the Robot instance.

  Robot *robot = new Robot();

  namespace webots {

    class Camera : public Device {

      virtual void enable(int samplingPeriod);

      virtual void disable();

      int getSamplingPeriod() const;

      // ...

    }

  }



  // get the time step of the current world.

  int timeStep = (int)robot->getBasicTimeStep();



  // You should insert a getDevice-like function in order to get the

  // instance of a device of the robot. Something like:

  //  Motor *motor = robot->getMotor("motorname");

  //  DistanceSensor *ds = robot->getDistanceSensor("dsname");

  //  ds->enable(timeStep);



  // Main loop:

  // - perform simulation steps until Webots is stopping the controller

  while (robot->step(timeStep) != -1) {

    // Read the sensors:

    // Enter here functions to read sensor data, like:

    //  double val = ds->getValue();



    // Process sensor data here.



    // Enter here functions to send actuator commands, like:

    //  motor->setPosition(10.0);

  };



  // Enter here exit cleanup code.



  delete robot;

  return 0;

}


This is my code

##### KNTRL [Premier Service] 04/12/2021 17:17:38
Hi guys,

So I am trying to figure out all of the sensors and while trying to work on the range-finder, I have my doubts that the sample-file is working as intendet for me? I also tried to play around in a simple project, but the results on the data-array are either minrange or maxrange values, no matter what I do
> **Attachment**: [range\_finder\_example.mp4](https://cdn.discordapp.com/attachments/565154703139405824/831216507798290492/range_finder_example.mp4)

##### Maurice Faraj 04/12/2021 18:14:00
Me to I’m having the same error if anyone can help us please I well be so grateful

##### DDaniel [Cyberbotics] 04/12/2021 19:13:20
`@KNTRL` Hi, which version of Webots are you using? Just tested the sample world on R2021a and it appears to be working

##### Beginner26798 04/12/2021 19:47:01
I have rhe 2020b

##### KNTRL [Premier Service] 04/12/2021 20:01:12
<@787796043987025941> I'm on 2021a too, and I haven't touched the file before simulating.

##### Beginner26798 04/12/2021 20:30:29
Can you please send the link of the latest version of webots maybe that's the problem

##### DDaniel [Cyberbotics] 04/12/2021 20:58:51
`@Beginner26798` you can't simply copy-paste the code snippet from the documentation and expect it to work like you did here 

``` 
namespace webots {
    class Camera : public Device {
      virtual void enable(int samplingPeriod);
      virtual void disable();
      int getSamplingPeriod() const;
      // ...
    }
  }
```

but instead 

```
Robot *robot = new Robot();
Camera *camera;
camera = robot->getCamera("name_of_the_camera");
camera->enable(TIME_STEP);
```



If you're not comfortable with c++ there's other options, and I suggest you follow the tutorials to begin with.


`@KNTRL` very odd, what OS are you on?

##### smasud98 04/12/2021 23:38:05
Hi guys

I am very new to Webots so apologies if this question is very basic or has been answered already. I want to make a monitor/global controller that would keep track of the positions of all the objects in the environment. I was wondering if it is possible to extract the translation, size, and rotation attributes of all of the objects in the environment. Thanks

##### Baziyad 04/13/2021 00:05:20
Hello everyone


I am trying to remotely control my OP2 robot using Webot, but I am getting the following error:

[sudo] password for robotis: [sudo] password for robotis: sudo: /robotis/Linux/project/webots/remote\_control/remote\_control: command not found



 



What could be the problem and how to solve?



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/831319292950872164/unknown.png)
%end


Thanks in advance 🙂 Really enjoying Webot

##### OwOcelot 04/13/2021 00:35:04
I have no idea

##### thonk enthusiast 04/13/2021 01:10:04
I think you've either linked it incorrectly or using the wrong path
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/831335399329759273/unknown.png)
%end


If I remember how `ln` works correctly


You linked /robotis/Linux/project/demo/demo to /robotis/Linux/project/webots*/default*


but you didn't specify default in your remote control command path


try this one


```bash
echo 111111 | sudo -S /robotis/Linux/project/webots/default/remote_control/remote_control 2 2
```

##### Baziyad 04/13/2021 01:38:15
Thank you very much for your kind reply

##### thonk enthusiast 04/13/2021 01:38:52
You're welcome

##### Baziyad 04/13/2021 01:40:46
But Webot generates automatically the code and installs it on OP2 robot.

I think the directory is correct, when I navigate to:

 /robotis/Linux/project/webots/remote\_control/ 



I find this folder, but I find any  remote\_control file inside


but I don't find*


Webot did not install it on the OP2 robot?



%figure
![WhatsApp_Image_2021-04-13_at_5.41.54_AM.jpeg](https://cdn.discordapp.com/attachments/565154703139405824/831343540071825409/WhatsApp_Image_2021-04-13_at_5.41.54_AM.jpeg)
%end


Webot is trying to run the follwoing:

/robotis/Linux/project/webots/remote\_control/remote\_control



Which there is no file named "remote\_control" inside remote\_control folder as shown in the image.

Looks like Webot is not installing all the required files? How to solve?

##### thonk enthusiast 04/13/2021 01:47:17
[https://cdn.discordapp.com/emojis/771407331489087569.png?size=64](https://cdn.discordapp.com/emojis/771407331489087569.png?size=64)


apparently someone else had this problem


this one


The recommended solution was to uninstall webots + reinstall it

##### Baziyad 04/13/2021 01:48:18
I already did 😭

##### thonk enthusiast 04/13/2021 01:48:39
uhhh


try running make?


I see a makefile in the directory


maybe it didn't compile ¯\\\\_(;-;)\_/¯

##### Baziyad 04/13/2021 01:53:01
I get: 

no makefiles found. Stop.

##### thonk enthusiast 04/13/2021 01:54:13
uhh try "make -f Makefile.robotisop2" or "make -f Makefile.robotis-op2"


just try pointing the make to the makefile

##### Baziyad 04/13/2021 01:57:46
I get:

no rule to make the make target Makefile.robotis-op2. Stop.


no rule to make the target Makefile.robotis-op2. Stop.


Btw: in the remote window in Webot, I get an error before:



[sudo] password for robotis: LinuxNetwork.cpp:225:70: error: no ‘const Robot::LinuxServer& Robot::LinuxServer::operator<<(const string&) const’ member function declared in class ���Robot::LinuxServer’

LinuxNetwork.cpp:235:62: error: no ‘const Robot::LinuxServer& Robot::LinuxServer::operator<<(const int&) const’ member function declared in class ‘Robot::LinuxServer’

LinuxNetwork.cpp:248:64: error: no ‘const Robot::LinuxServer& Robot::LinuxServer::operator>>(std::string&) const’ member function declared in class ‘Robot::LinuxServer���

LinuxNetwork.cpp:258:46: error: no ‘void Robot::LinuxServer::accept(Robot::LinuxServer&)’ member function declared in class ‘Robot::LinuxServer’

LinuxNetwork.cpp:266:58: error: no ‘bool Robot::LinuxServer::send(unsigned char*, int)’ member function declared in class ‘Robot::LinuxServer’

LinuxNetwork.cpp:271:57: error: no ‘int Robot::LinuxServer::recv(unsigned char*, int)’ member function declared in class ���Robot::LinuxServer’

make[1]: *** [LinuxNetwork.o] Error 1

make: *** [darwin.a] Error 2


The file LinuxNetwork.cpp is dropping errors



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/831348185888587826/unknown.png)
%end


Another error while scrolling in the console:



Note that here it is trying to run "make" in the directory "remote\_control", but "LinuxNetwork.cpp" is dropping error
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/831348945288167424/unknown.png)
%end


I think "LinuxNetwork.cpp" is causing the problems! 

This file is specific to the OP2 robot


Is there any workaround to run a Webot world file code in the OP2 robot rather than the Transfer window in Webot?

##### Olivier Michel [Cyberbotics] 04/13/2021 05:43:12
Yes, using a supervisor.

##### aja\_discord 04/13/2021 06:04:18
Hi Guys, I'm trying to make a self balancing robot that learns from a genetic algorithm. But I'm unable to restore the position of the robot properly..From my observations it seems the supervisor and controller are out of sync...meaning that when the supervisor resets the robots position the robot controller still keeps on running it motors hence loosing its balance early on, unlike what I want...I don't understand how to get this fixed...Here are the links to the supervisor and controlloer...Thanks in advance...The functions in concern are  **restore\_robot\_position(), run\_optimization(), evaluate\_genotype() ** ***Links :***[https://pastebin.com/H0jnEjN5](https://pastebin.com/H0jnEjN5)     [https://pastebin.com/eyzcPrey](https://pastebin.com/eyzcPrey)

##### KNTRL [Premier Service] 04/13/2021 06:41:07
<@787796043987025941> I'm on Windows 10. I'll try a new installation later today

##### h.sciascia 04/13/2021 06:45:55
Hello everyone !

I would like to create a world with a robot (6 DOF) which responds as close as from the reality. The most important item I would like is the deceleration of the robot. For exemple, if the robot is doing a very large arm movement (high speed and acceleration), and the robot need to stop, how can I simulate the deceleration from the moment I said stop to the robot ? 

In other terms, how can I get the "real" paramaters torque and velocity (maybe controlPID and acceleration) for each motor ?

Thank in advance

##### bingdong 04/13/2021 08:38:36
Hello!

How can I take text input from user while running a Python-based controller? 

When using the input() function, the console just hangs.

##### Master.L 04/13/2021 09:42:50
hello!

I am trying to do reinforcement learning on webots using deepbots.



What is the difference between emitter-receiver method and supervisor method?

##### DDaniel [Cyberbotics] 04/13/2021 09:46:13
`@bingdong` if you want to control the robot using the keyboard you can use the keyboard functions [https://cyberbotics.com/doc/reference/keyboard?tab-language=python](https://cyberbotics.com/doc/reference/keyboard?tab-language=python)

##### bingdong 04/13/2021 10:05:30
From what I understand, the Keyboard Functions are for key press input. I want to  be able to give a text input of a list according to which the robot simulates.

##### aja\_discord 04/13/2021 10:06:22
supervisor is something like another controller but it has special functions, more that that of the robot class.You can think of it like adding the presence of a human to your simulation.Emitter\_Reciever method is like adding a radio module or infrared module to your robot.You are esentially opening a communication link for the robot...Hope this helps

##### DDaniel [Cyberbotics] 04/13/2021 10:27:16
I don't see that working well, but maybe I'm misunderstanding. If you have multiple robots that have the same controller but want them to behave differently you can do so using controller arguments [https://cyberbotics.com/doc/guide/controller-programming#using-controller-arguments](https://cyberbotics.com/doc/guide/controller-programming#using-controller-arguments) 

Alternatively, you can create a supervisor that communicates with the robot through an emitter/receiver and sends out that information to the robot at the appropriate time.


The difference and use-cases is explained in the deepbots repository: [https://github.com/aidudezzz/deepbots/blob/dev/README.md](https://github.com/aidudezzz/deepbots/blob/dev/README.md)

##### Maurice Faraj 04/13/2021 10:31:03
Hello everyone hope you’re fine I’m facing a problem in my webots project I’m trying to enable the camera on I’m used several code to do this but the camera did not function and nothing wrong with the code . if any one can help me please I will be so grateful and thank you all

##### DDaniel [Cyberbotics] 04/13/2021 10:34:39
`@aja_discord` The supervisor `simulationReset` function doesn't actually restart the controllers. If you want it to, you need to manually do so [https://www.cyberbotics.com/doc/reference/supervisor?tab-language=c++#wb\_supervisor\_node\_restart\_controller](https://www.cyberbotics.com/doc/reference/supervisor?tab-language=c++#wb_supervisor_node_restart_controller)

##### bingdong 04/13/2021 10:59:43
Controller Arguments was what I needed, yes. Thank you!

##### KNTRL [Premier Service] 04/13/2021 11:20:08
<@787796043987025941> I installed the R2021a new and I am still having the same problem (Windows 10, 64 Bit), but here comes the interesting part: I also have a 2020b Version on the machine where it seems to work.


If I take the working file from the R2020b folder and try to open it with R2021a version, it won't work (like in the previously posted video)



Furthermore I noticed that when I start Webots R2021a and it autostarts the range-finder sample project, it throws an



ERROR: Shader compilation failed! Filename: encode\_depth.frag

0(5) : error C1059: non constant expression in initialization

##### Baziyad 04/13/2021 12:22:03
Hello everyone,



Can someone help me with my problem?

##### Johan2021 04/13/2021 12:41:27
Hi everyone, I am trying to model a Physics plugin that would perform some action depending on the value of a Connector node's `isLocked` value ([https://cyberbotics.com/doc/reference/connector#wb\_connector\_is\_locked](https://cyberbotics.com/doc/reference/connector#wb_connector_is_locked)). I found that ODE provides functions such as `dBodyGetPosition()` to directly retrieve information about the robot, but I was wondering whether there is some functionality that would allow to retrieve (and not necessarily change) the `isLocked` field that is part of the Connector node - being governed by the controller.



It should be possible to send the boolean value of `isLocked` from the controller using the `dWebotsReceive()` function in the plugin and the `wb_emitter_send()` function of an Emitter node - as explained here [https://www.cyberbotics.com/doc/reference/utility-functions#dwebotssend](https://www.cyberbotics.com/doc/reference/utility-functions#dwebotssend). But for design reasons I was hoping to avoid this approach. Does someone have more knowledge or ideas on this? Greatly appreciated!

##### Sutirtha38 04/14/2021 06:39:18
Hi All,

I  am trying a NAO  robot to react to a video, I am unable to include the video source into the environment. Any Help? 

Cheers,

Suti

##### Darko Lukić [Cyberbotics] 04/14/2021 06:59:41
Hello, Webots creates a fixed joint between two connectors when locked:

[https://github.com/cyberbotics/webots/blob/180777b4f9114e336eb39f70f8feda42a7ffc552/src/webots/nodes/WbConnector.cpp#L486](https://github.com/cyberbotics/webots/blob/180777b4f9114e336eb39f70f8feda42a7ffc552/src/webots/nodes/WbConnector.cpp#L486)



Maybe, you can exploit it in the physics plugin


You can set an image to the Display node:

[https://stackoverflow.com/questions/58286019/webots-displaying-processed-numpy-image-opencv-python/58298221#58298221](https://stackoverflow.com/questions/58286019/webots-displaying-processed-numpy-image-opencv-python/58298221#58298221)

So, maybe you can do it continuously from a controller.



One more idea (that I am not sure about) is to use `wbr_camera_set_image`:

[https://github.com/cyberbotics/webots/blob/114ef740e47613c8235022716d7cf0782e383f3a/projects/robots/gctronic/e-puck/plugins/remote\_controls/e-puck\_bluetooth/EPuckInputPacket.cpp#L127](https://github.com/cyberbotics/webots/blob/114ef740e47613c8235022716d7cf0782e383f3a/projects/robots/gctronic/e-puck/plugins/remote_controls/e-puck_bluetooth/EPuckInputPacket.cpp#L127)

and then `wb_display_attach_camera`:

[https://cyberbotics.com/doc/reference/display#wb\_display\_attach\_camera](https://cyberbotics.com/doc/reference/display#wb_display_attach_camera)



In any case, you need a controller that reads frame by frame from a video file and sends it to Webots.

##### Uanuan 04/14/2021 08:46:34
Hi all, I'm trying (hopefully) to create a socket server that will eventually control a bunch of bots simulated in the webots environment, so I have setup a python server (separated from the main controller thread), and in turn, it listens to connection requests from other programs. One thing I noticed is that the main thread appears to be stuck at 0 when I start the simulation (the timer stays at 0), and it will only spit out a bunch of text when I  terminate the simulation (which kills the server as well). Although the client/server interactions appear to be correct based on my test drivers, I'm obviously doing something very wrong. Can you give me some pointers on how to fix this problem? Here's how the controller code looks like: 

def main():

    robot = Robot()

    

    #sets up the server thread and start it

    server = SocketServer(server\_address, serverStatusCallback, PORT, maxConns)

    server.start()

whereas SocketServer class looks like the following and I've called threading.Thread.\_\_init\_\_(self) beforehand to start the server, so I ddin't get any errors from Python =>

class SocketServer(threading.Thread): <--- when run() is called, it just enters into a loop to listen() and spawn additional threads to handle client connections. In addition, I've also set all of the print to console with flush=True option.

##### James Le Poidevin 04/14/2021 09:26:49
Hello, Is there an example of a ros2 package that doesn't launch a world file but uses one already open (either by another pkg or just by hand)?

##### Darko Lukić [Cyberbotics] 04/14/2021 09:29:20
No, but you should be able to exclude the Webots world from the launch files and then run only controller

##### Stefania Pedrazzi [Cyberbotics] 04/14/2021 09:40:16
You should make sure that in your controller program you have the simulation main loop that calls `robot.step(timestep)` otherwise if the Robot has the `synchonization` field set to true the simulation won't advance because it will wait for this controller:

[https://www.cyberbotics.com/doc/guide/controller-programming#controller-programming](https://www.cyberbotics.com/doc/guide/controller-programming#controller-programming)

##### James Le Poidevin 04/14/2021 09:48:10
I'm using an example of the robot_launch.py file from github and i keep getting the error 
```  RuntimeError: Included launch description missing required argument 'world' (description: 'Path to Webots' world file. Make sure `controller` field is set to `<extern>`.'), given: [package, executable, robot_name]

``` 

Do i even need to use the `webots = IncludeLaunchDescription(...)` part ?

##### Darko Lukić [Cyberbotics] 04/14/2021 11:00:02
You cannot use `robot_launch.py`, but you can take inspiration from it:

[https://github.com/cyberbotics/webots\_ros2/blob/master/webots\_ros2\_core/launch/robot\_launch.py](https://github.com/cyberbotics/webots_ros2/blob/master/webots_ros2_core/launch/robot_launch.py)



For example, to run the controller you need only this part:

[https://github.com/cyberbotics/webots\_ros2/blob/57f8f48121243f24f35675ba1b54b886da8690f4/webots\_ros2\_core/launch/robot\_launch.py#L108-L122](https://github.com/cyberbotics/webots_ros2/blob/57f8f48121243f24f35675ba1b54b886da8690f4/webots_ros2_core/launch/robot_launch.py#L108-L122)



And to run the simulation:

[https://github.com/cyberbotics/webots\_ros2/blob/57f8f48121243f24f35675ba1b54b886da8690f4/webots\_ros2\_core/launch/robot\_launch.py#L101-L105](https://github.com/cyberbotics/webots_ros2/blob/57f8f48121243f24f35675ba1b54b886da8690f4/webots_ros2_core/launch/robot_launch.py#L101-L105)

[https://github.com/cyberbotics/webots\_ros2/blob/57f8f48121243f24f35675ba1b54b886da8690f4/webots\_ros2\_core/launch/robot\_launch.py#L142-L147](https://github.com/cyberbotics/webots_ros2/blob/57f8f48121243f24f35675ba1b54b886da8690f4/webots_ros2_core/launch/robot_launch.py#L142-L147)

##### James Le Poidevin 04/14/2021 12:05:06
Ok thank you i'll give it a try


Thank you  i got it to work just needed to remove the 'webots' from the launch

##### bingdong 04/14/2021 15:13:17
Hi!

Is it the case that I can't run an infinite while loop inside the main loop in robot controller like this?

while robot.step(timestep)!=-1:

    #Commands

     while True:

         # Commands

##### Darko Lukić [Cyberbotics] 04/14/2021 15:14:36
`@bingdong` You can, but nothing from the loop will be executed until you call `robot.step()`

##### Nick R 04/14/2021 17:19:46
Hi all, I'm very new to Webots and evaluating it against a few other simulators for use at my company. I'm trying to test a number of sensors (eg a few Velodyne LIDARs and a handful of mono cameras) in a reasonably-complex world. However, I'm struggling to understand how I can extend any of the sample robot/car model PROTOs to add these sensors. `@Olivier Michel` suggested I use the extension slots on the BmwX5 model (thank you VERY much for all your help yesterday by the way!) but it would be very helpful if there's some example model I can follow which has a sensor added to a slot.



Alternatively, if there is a simpler way to evaluate a number of sensors please do let me know. Mainly looking to test real-time performance and understand processing limitations. Thank you!

##### baby\_droid\_yoda 04/14/2021 18:00:05
Hi, How can I make YOLOv3 with Webots? Webots' controller give me image parameters 4 (BGRA) and Yolov3 is getting image array parameters 3(BGR).

##### Simon Steinmann [Moderator] 04/14/2021 19:38:33
`@Nick R` You can simply add a Robot base node and add all the sensor as children. as controller select 'Void'. run the simulation and right click on the robot in the scene tree -> view Robot Window. Here you can enable the sensors and view them. You can move the robot in the 3d view when selected. And you can move the sensors relative to the robot when you select them.


`@baby_droid_yoda` [https://cyberbotics.com/doc/reference/camera?tab-language=python#wb\_camera\_get\_image](https://cyberbotics.com/doc/reference/camera?tab-language=python#wb_camera_get_image) I suggest you study the api documentation. It really depends on what language you use, and what exactly you want and need

##### Beginner26798 04/14/2021 20:41:03
Good evening everyone ... i need a code to control the ur10e robotic hand please

##### Simon Steinmann [Moderator] 04/14/2021 21:51:13
`@Beginner26798` which hand? the Robotiq 3f gripper?


and what language?

##### Beginner26798 04/14/2021 21:52:51
If ther is a code to this hand without a gripper i will replace it with a camera.

Using C language

##### Simon Steinmann [Moderator] 04/14/2021 21:53:07
what exactly do you want to achieve?

##### Beginner26798 04/14/2021 21:54:05
I am working of a controlled hand that moves due to color recognition caused by the camera


If you want more details i can dm you

##### Simon Steinmann [Moderator] 04/14/2021 22:12:11
look at the documentation, motors are very simple to control

##### Emiliano Borghi 04/15/2021 00:24:26
Hello, I'm doing some tests with Pioneer with ROS. I was able to compile the official controller  ([https://github.com/cyberbotics/webots\_ros/blob/master/src/pioneer3at.cpp](https://github.com/cyberbotics/webots_ros/blob/master/src/pioneer3at.cpp)) and run it as "ros", but when I set the world option as "<extern>", it's not able to find it.



Webots terminal is not showing any useful data. How can I see more useful logs?

##### Simon Steinmann [Moderator] 04/15/2021 00:28:08
pretty sure you have to run the ros controller in webots, not the extern

##### Uanuan 04/15/2021 01:15:26
Cool, thanks a lot. I'm going to try that out. I'm also wondering if there's a way to reference multiple (different) robot supervisor in the world? is there such a thing: bot1 =Supervisor("some id") then bot2 = Supervisor("another id") in the API?

##### Nick R 04/15/2021 03:15:52
Thanks a bunch! I was able to do this and get a number of sensors running simultaneously and streamed to ROS2 using the webots\_ros2\_core launch file robot\_launch.py, however I have the issue that the transform tree in ROS2 is empty. I get launch errors like the following, seems a link is not unique, anyone have an idea where to look to solve this? The 'solid\_37725' is not referenced anywhere in the .wbt that I can see, not sure where this comes from...



[robot\_state\_publisher-1] Error:   link 'solid\_37725' is not unique.

[robot\_state\_publisher-1]          at line 134 in /tmp/binarydeb/ros-foxy-urdfdom-2.3.3/urdf\_parser/src/model.cpp

[robot\_state\_publisher-1] [WARN] [1618456336.778552563] [robot\_state\_publisher]: Unable to initialize urdf::model from robot description

##### Stefania Pedrazzi [Cyberbotics] 04/15/2021 06:23:59
An independent controller instance is run for each Robot/Supervisor, so you can only create one supervisor instance per controller and use the Supervisor API with the robot associated with the controller.

But you can use the same controller for multiple robots (multiple instances will be started) and identify and use for example the `Robot.name` value to identify it (see [https://www.cyberbotics.com/doc/reference/robot#wb\_robot\_get\_name](https://www.cyberbotics.com/doc/reference/robot#wb_robot_get_name)).

##### Darko Lukić [Cyberbotics] 04/15/2021 06:53:19
Webots doesn't require solid names to be unique, so we add a suffix to the solid name if there is a solid with the same name. However, it is strange that you are getting the error. Is it a custom robot?

##### Uanuan 04/15/2021 08:32:33
Thanks again. I got the timestep thingy in place, and everything is working fine atm. Really appreciate your help on this, I'm going to experiment with the Robot.name in my python code later 🙂

##### Maurice Faraj 04/15/2021 09:07:17
Hello everyone hope you’re doing well I’m facing some problems to controle a ure10 hand that contain a camera on its periferic and the camera by recongnize color should order the ure10 to move if anyone can help me please how to controle it and thank you everyone

##### Baziyad 04/15/2021 09:38:37
Dear `@Darko Lukić`, thanks for your efforts.

I am facing the following error when  I try to remotley connect Webot  to OP2 robot:

Any clues?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/832188154474790912/unknown.png)
%end

##### Johan2021 04/15/2021 10:13:12
Hi `@Darko Lukić` , thanks a lot for your reply! In this case, the fixed joint would only be created when a compatible peer connector is present - if I understood correctly - which would not necessarily be the case for me. Would there be a way to declare the `isLocked` value as a "global variable" to the simulator or scene tree that could then be read out from the plugin (I am still new with Webots)?

##### Darko Lukić [Cyberbotics] 04/15/2021 10:55:30
Yes, you can write the start in e.g. `customData` robot field:

[https://cyberbotics.com/doc/reference/robot](https://cyberbotics.com/doc/reference/robot)



And update it using:

[https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_field\_set\_sf\_string](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_field_set_sf_string)

and read it using:

[https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_field\_get\_sf\_string](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_field_get_sf_string)

##### KNTRL [Premier Service] 04/15/2021 11:05:59
Can someone explain the way the RadarTarget list is being sorted? I don't seem to figure out the way it gets sorted after a new target is added.

##### Stefania Pedrazzi [Cyberbotics] 04/15/2021 11:18:24
At simulation load the RadarTarget list is initialized using the order in the scene tree. If you then add new targets after the simulation load, the new target is appended to the end of the list.

##### Johan2021 04/15/2021 11:19:27
This is great! Just to make sure; this would mean that the supervisor class and its functions are available to the `plugin` (reading out the value) as well as to the `controller` (setting the value), after declaring the robot node as supervisor?

##### Darko Lukić [Cyberbotics] 04/15/2021 11:39:52
Unfortunately, I don't think you can access these fields from the Physics plugin. I afraid you need to fallback to your initial idea.

##### Emiliano Borghi 04/15/2021 12:01:54
But it's possible according to the documentation but I haven't found an example in webots\_ros package.

##### aslisevil 04/15/2021 12:40:30
hello, is it possible to create a motion file automatically for Nao robot using some program? I am trying to add a box lifting motion to it.

##### Iris230 04/15/2021 12:49:18
guys, I just added the controller' racing\_wheel' to a BMW5. 

But when I keep my foot on the gas, the speed is so low. Is there someone knowing what the problem is? :smiling\_face\_with\_tear:
%figure
![68b2c851b73d6578.png](https://cdn.discordapp.com/attachments/565154703139405824/832236141054132245/68b2c851b73d6578.png)
%end
%figure
![-1add8774e3bb647a.png](https://cdn.discordapp.com/attachments/565154703139405824/832236141318635520/-1add8774e3bb647a.png)
%end


sorry It's a BmwX5

##### Nick R 04/15/2021 14:14:01
Yes, this is a custom "robot" created as a Robot base node with two LIDAR sensors as children. The LIDAR names are unique, so I'm not sure what else is preventing this from working. Unfortunately since the robot\_state\_publisher won't run, I can't access /tf and visualize all the data in map frame (which I assume should work if launched successfully)

##### Steven37 04/15/2021 15:12:06
I am trying to create a function to control my 4-wheeled robot by continuously changing the speed of 4 wheels so that the robot can move in random directions. Then, I want to print out the robot's position every 3 seconds using the supervisor. I have coded for that function as shown in the image I attached. However, I did not see the robot's position printed out. Can someone help me, please?
%figure
![3Y_project_CodeForExperiment11.JPG](https://cdn.discordapp.com/attachments/565154703139405824/832272078747992084/3Y_project_CodeForExperiment11.JPG)
%end

##### DDaniel [Cyberbotics] 04/15/2021 16:45:43
`@Steven37` the condition for it to print doesn't do what you want it to do, variable `t` doesn't change. The function you're thinking of is probably [https://cyberbotics.com/doc/reference/robot?tab-language=c++#wb\_robot\_get\_time](https://cyberbotics.com/doc/reference/robot?tab-language=c++#wb_robot_get_time) (still need to fix the condition though)

##### ~E 04/15/2021 16:49:57
Hi all,



I am using the e-puck sample bot with the collision detection and pulling accelerometer data for further data analysis. I am using matlab for the code since my school gets paid A LOT of money to use them, and therefore cannot use Python like any other sensible person would (only joking a little bit ;).



I can post my code, but would like to limit how much I post since this is a project for school, and I do not want classmates to find this and plagiarize.



> **Attachment**: [E-Puck\_jerky\_movement.mp4](https://cdn.discordapp.com/attachments/565154703139405824/832297321935929434/E-Puck_jerky_movement.mp4)

##### Darko Lukić [Cyberbotics] 04/15/2021 17:14:12
Is it possible to send the robot model to me so I can investigate it? Also, we did some improvements to the URDF exporter in R2021b. Therefore, if it doesn't take too much time then you can try R2021b:

[https://github.com/cyberbotics/webots/releases](https://github.com/cyberbotics/webots/releases)

##### ~E 04/15/2021 17:25:27
I just spoke to my professor, and apparently we aren't even supposed to make the robot collide with anything. I can still DM the files to it if you're curious

##### Nick R 04/15/2021 17:36:22
Sure, I can send it, but I'll try to use 2021b instead first (currently on 2021a). Thanks!

##### ~E 04/15/2021 19:24:57
Can someone confirm for me that if your basicsteptime = 4, that means that each step is equal to 0.004 seconds (4ms)?

##### Simon Steinmann [Moderator] 04/15/2021 19:29:23
yes


4 is quite small. Usually 16 works quite well


webots is very stable compared to other simulators

##### ~E 04/15/2021 19:30:29
I just have some REALLY high jerk in starting my movement, and my prof wants us to integrate to find velocity


Also, I hate that matlab starts arrays at 1

##### Simon Steinmann [Moderator] 04/15/2021 20:18:49
😆  I feel you


oh integrate acceleration to fin vel?


I guess when you are only allowed to use the accelerometer. Wheel motors seem more logical


can you show an example of the jerk in starting movement? It might just be a very high torque setting (unrealisticly high).


you can always speed up in a slower and controlled manner

##### geflowers 04/16/2021 04:06:15
Hey everyone, I'm pretty new with working on webots and had a quick question on how to obtain a certain movement with a rotational motor


So the idea is to have this arm begin at this initial position and after a timestep, have the rotational motor kind of "jerk" the arm back to this horizontal position and repeatedly do that
%figure
![Concept_art_-_Function.PNG](https://cdn.discordapp.com/attachments/565154703139405824/832467213767671828/Concept_art_-_Function.PNG)
%end


Was recommended to base it off PID control but kinda struggling on getting the rotational motor to move using the created controller


Any ideas or resources I could use as reference to do this?

##### Max\_K 04/16/2021 07:13:58
Hey, is it possible to run a opc ua server inside webots and are there any examples?

##### KNTRL [Premier Service] 04/16/2021 08:21:21
What could cause that the point cloud randomly disappears during the simulation? This was done on Win10, R2021a, and it is the same with R2021a on Ubuntu 18.04.5.
> **Attachment**: [lidar\_samplefile\_win10\_R2021a.mp4](https://cdn.discordapp.com/attachments/565154703139405824/832531097040191528/lidar_samplefile_win10_R2021a.mp4)

##### DDaniel [Cyberbotics] 04/16/2021 08:24:28
`@KNTRL` was a bug, [https://github.com/cyberbotics/webots/pull/2666](https://github.com/cyberbotics/webots/pull/2666) but should be fixed in the nightly version of webots

##### bingdong 04/16/2021 15:45:48
Hi, I seem to have done something in the properties which is why I'm seeing this in the rendering screen. Can someone let me know how I can fix this?
%figure
![Screenshot_from_2021-04-16_21-05-30.png](https://cdn.discordapp.com/attachments/565154703139405824/832642948872011806/Screenshot_from_2021-04-16_21-05-30.png)
%end

##### Nick R 04/16/2021 15:45:52
The nightly version seems to have fixed my issue with URDF export/robot description generation to be used in building the tf tree. Thank you for the suggestion!! Maybe worth making a note that there may be an issue with 2021a

##### DDaniel [Cyberbotics] 04/16/2021 15:51:05
`@bingdong` not sure as the pic isn't super clear but try F11 or view > plain rendering, might be that

##### bingdong 04/16/2021 16:05:49
Yes, right. I'd switched it to wireframe rendering somehow. Thank you!

##### Nick R 04/16/2021 17:26:44
When simulating a VLP-16 LIDAR, I can run the simulation in fast mode at >10x speed. However, if I subscribe to the ROS2 topic for the LIDAR data in any way (for example viewing it in RViz2, or even just echoing the topic), then the speed cuts in half. Is this expected due to the overhead of ROS2 communication? Anything that can be done about it?

##### Darko Lukić [Cyberbotics] 04/16/2021 20:25:25
By default, the Lidar publishes data every 128ms as data is very large:

[https://github.com/cyberbotics/webots\_ros2/blob/b37b9916f002503148171279a9a4a1051cd1092d/webots\_ros2\_core/webots\_ros2\_core/devices/lidar\_device.py#L44](https://github.com/cyberbotics/webots_ros2/blob/b37b9916f002503148171279a9a4a1051cd1092d/webots_ros2_core/webots_ros2_core/devices/lidar_device.py#L44)



P.S. FastDDS recently published a zero-copy feature that should significantly improve transfer speeds between nodes. This should allow publishing lidar data at higher rates.

##### Nick R 04/16/2021 20:52:22
Would the zero-copy feature work to reduce overhead from webots to ROS2 though? I'm new to ROS2, but that sounds like the concept of nodelets in ROS1


Also, I experimented a bit more and the RTF (realtime factor) actually decreases much more when subscribing to a single camera stream (800x800) than a single VLP16, which I was surprised by. Unfortunately I want to stream several of each of those sensors at once, but the RTF goes down below 1.0 in that case 😦

##### Simon Steinmann [Moderator] 04/16/2021 21:05:57
`@Nick R` This is a lot of data. 800 * 800 * 4 Byte = 2.5 MB per frame


if you have 30 frames per second (32ms timestep), that is 75MB/s


this can easily saturate a network connection


keep in mind, that this image data is uncompressed


you might want to consider publishing at a lower rate, or turning it into a service or action, only publishing on request

##### Nick R 04/16/2021 21:10:12
Thanks, it does make sense then. I'm actually not sure the output rate, it's the default for the Camera sensor, but I forgot to check on the ROS2 side. Can the data be sent as a compressed ROS2 msg?

##### Simon Steinmann [Moderator] 04/16/2021 21:10:32
I'm actually googling that right now :p

##### Nick R 04/16/2021 21:10:42
I was suprised because in Gazebo, I was able to run at RTF=1.0 for a large number of camera sensors. They must use some zero-copy mechanism under the hood, I should dig further

##### Simon Steinmann [Moderator] 04/16/2021 21:10:52
[http://wiki.ros.org/compressed\_image\_transport](http://wiki.ros.org/compressed_image_transport)

##### Nick R 04/16/2021 21:11:28
Thanks, yeah I'm aware of this for ROS1, not sure about ROS2, and if webots side would need to be modified in either case

##### Simon Steinmann [Moderator] 04/16/2021 21:11:30
the question is, where exactly the bottleneck lies. It might be inside webots or in ros


do you have python code?

##### Nick R 04/16/2021 21:12:45
I just use the webots\_ros2\_core launch file robot\_launch.py to create the ROS2 interface, with my simple "robot" having some sensors and the controller set to <extern>

##### Simon Steinmann [Moderator] 04/16/2021 21:12:55
oh, what gpu do you have? and is it being utilized properly?


could you pm me your project? wanna do some digging and analysis

##### alireza1992 04/17/2021 10:38:10
Hi everyone;

I would like to change the position of robot's links immediately after taking command, that is I don't want to use from joints and motors of robot because they need to spent several time step.

Probably one way is that define several field corresponding to every link and change their values by supervisor. Is there any other way? 

Can you help me? 

Thank you.

##### Beginner26798 04/17/2021 20:36:19
Good evening ... i need a code to change the shoulder and elbow angle of the ure10 robot please


Continously


`@Darko Lukić` can you please help me

##### Simon Steinmann [Moderator] 04/18/2021 01:20:12
`@Beginner26798` use the motors to change the position. The  position is the angle in radian. Take a look at the example world ure.wbt

##### tomala 04/18/2021 22:58:50
Hi! I came across some problems while I was working on my robot. 



I defined some additional parameters on the top of the proto and used them in the file with command IS. However webots shows me the error which says that my parameter has no matching IS field.



Next problem occured when I was adding HingeJointWithBacklash. I think that everything is written correctly but I still see error that HingeJointWithBacklash is skipped.



Do you have any ideas what I could have done wrong?
%figure
![errory_backlash.PNG](https://cdn.discordapp.com/attachments/565154703139405824/833476700058026064/errory_backlash.PNG)
%end



%figure
![definicja_nie_dziala.PNG](https://cdn.discordapp.com/attachments/565154703139405824/833476775463223306/definicja_nie_dziala.PNG)
%end



%figure
![hingejointwithbacklash.PNG](https://cdn.discordapp.com/attachments/565154703139405824/833476855330897940/hingejointwithbacklash.PNG)
%end

##### Simon Steinmann [Moderator] 04/18/2021 23:22:25
`@tomala`  try avoiding the dashes, that could be it

##### tomala 04/18/2021 23:27:50
Unfortunately, it didn't help

##### Simon Steinmann [Moderator] 04/18/2021 23:46:00
What is HingeJointWithBacklash?


did you actually add this node or is it a dev build node?


[https://cyberbotics.com/doc/reference/hingejointparameters?tab-language=python](https://cyberbotics.com/doc/reference/hingejointparameters?tab-language=python)


these fields dont exist in hingjointparams

##### main 04/19/2021 03:32:20
Anyone know why when STL files are used to import models they end up so huge?


I imported a wheel as an stl file but its bigger than the entire scene (should only be a few hundred millimeters by a few hundred) does anyone know how this scaling works?


As i can scale it down to say 0.001 making it small , but i dont know how much to scale it so that the dimensions are identical to the real dimensions of the stl file (hope that makes sense?)

##### SeanLuTW 04/19/2021 06:08:04
Is it possible to take screenshot by the supervisor API when the simulator is running?

##### Stefania Pedrazzi [Cyberbotics] 04/19/2021 06:09:30
Yes with the `wb_supervisor_export_image` function [https://www.cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_export\_image](https://www.cyberbotics.com/doc/reference/supervisor#wb_supervisor_export_image)

##### SeanLuTW 04/19/2021 06:11:17
Oh, thanks!

##### Stefania Pedrazzi [Cyberbotics] 04/19/2021 06:11:31
There are just two ways to move joints to specific position:

1) use the Motor API to simulate real motor behavior

2) use the Supervisor API to change the joint position field


You could use a position sensor to detect when the joint reaches the desired position. Note that there is the gravity force acting on the joint end solid so you have to make sure that your motor has enough torque to keep the position.

Alternatively, if you don't want to reproduce a real motor behavior, you can use the Supervisor API to immediately set the joint position field.

Here are some examples:

- [https://www.cyberbotics.com/doc/guide/samples-devices#position\_sensor-wbt](https://www.cyberbotics.com/doc/guide/samples-devices#position_sensor-wbt)

- [https://www.cyberbotics.com/doc/guide/samples-devices#motor-wbt](https://www.cyberbotics.com/doc/guide/samples-devices#motor-wbt)

##### geflowers 04/19/2021 06:20:40
Thank you so much! Really appreciate the help

##### Stefania Pedrazzi [Cyberbotics] 04/19/2021 06:31:33
It depends on the scale. Webots scale unit is meters, when exporting STL you should make sure that the meters scale unit is used as well. Usually graphical editors like Blender give the option to choose the scale unit in the export dialog.

Otherwise, you can simply scale the Mesh node by setting the `Transform.scale`. The correct scale value depends on the scale unit used in the STL file.

##### DDaniel [Cyberbotics] 04/19/2021 06:34:34
`@tomala` Hi, which version of Webots are you using? The `HingeJointWithBacklash` proto is available only in the develop branch of Webots, so if you're using anything else it won't find it. Also, did you follow the hierarchy described here for your project? [https://cyberbotics.com/doc/guide/the-standard-file-hierarchy-of-a-project#the-root-directory-of-a-project](https://cyberbotics.com/doc/guide/the-standard-file-hierarchy-of-a-project#the-root-directory-of-a-project). As for the other error, I assume you're making your own robot proto. From what you've showed the call to `HingeJointWithBacklash`  looks good to me, could you post more (or send via PM) about the robot proto itself? Might be easier to pin point.

##### Johan2021 04/19/2021 07:43:03
Hi everyone, I am writing a Physics plugin that is supposed to exert a force onto a body at every timestep, but the simulation crashes at start-up pointing to `dBodyAddForce()` or to `dBodyAddRelForce()` when I replace the former. I found an old issue ([https://cyberbotics.com/doc/discord/technical-questions-2020?tab-language=c++&tab-os=macos#troy-04242020-01-27-23](https://cyberbotics.com/doc/discord/technical-questions-2020?tab-language=c++&tab-os=macos#troy-04242020-01-27-23)), but was this resolved in any way -- other than by using a Supervisor?

##### Master.L 04/19/2021 07:50:18
Hi.

I would like to implement a force/torque sensor in webots.

In the webots documentation, I found a force sensor that came with a 3-axis force value, but couldn't find a sensor that could get the torque value.

How can I get the torque value?

##### Olivier Michel [Cyberbotics] 04/19/2021 07:53:14
This is currently not implemented. I believe the way to go is to implement a new type of touch sensor that would be called "force-6d" and would return both the force and torque.

##### B0Bhead 04/19/2021 07:55:10
Is there currently a SLAM version that runs on the RoboMaker Pro Kit?

##### Stefania Pedrazzi [Cyberbotics] 04/19/2021 07:58:17
Did you check that the body you are using is valid and correctly initialized?

Here are a couple of Webots sample projects using the `dBodyAddRelForce` function that are working correctly:

[https://www.cyberbotics.com/doc/guide/blimp#blimp\_lis-wbt](https://www.cyberbotics.com/doc/guide/blimp#blimp_lis-wbt)

[https://www.cyberbotics.com/doc/guide/samples-howto#physics-wbt](https://www.cyberbotics.com/doc/guide/samples-howto#physics-wbt)

##### Master.L 04/19/2021 08:17:30
Thanks `@Olivier Michel` !

I'll ask you one more question!

There was a way to implement a new type of sensor that returns both force and torque called "force-6d", but I don't know what to do.

Can you give me some advice?

##### Olivier Michel [Cyberbotics] 04/19/2021 09:19:44
It should be implemented in C++ into this file: [https://github.com/cyberbotics/webots/blob/develop/src/webots/nodes/WbTouchSensor.cpp](https://github.com/cyberbotics/webots/blob/develop/src/webots/nodes/WbTouchSensor.cpp) and possibly other files, properly documented and tested.

##### Yi Zhou 04/19/2021 09:30:07
Hello everyone, does anyone know if there are some example about control robot grasp object in webots?

##### DDaniel [Cyberbotics] 04/19/2021 09:33:48
`@Yi Zhou` Hi, you can find one on `file > open sample world > robots > k-team > khepera3_gripper.wbt`

##### Yi Zhou 04/19/2021 09:34:11
Thank you so much!

##### alireza1992 04/19/2021 09:48:35
Thanks for your reply,

I am working on a reinforcement learning project in which I need to reset a humanoid robot position and gesture randomly at the start of each episode. Is there any way to set the joint positions before simulation started?

##### Stefania Pedrazzi [Cyberbotics] 04/19/2021 09:59:53
Usually for reinforcement learning, you use a Supervisor controller to set the world and robot and the Supervisor API has a reset function to prepare the world to the next episode.

Depending on your needs, you can dynamically add the robot from the Supervisor controller so that it is already inserted in the world with the desired pose, or use the Supervisor API to change just the joints position.

Note that the Supervisor API also has options to restart the robot controller in case it is needed after setting it to the desired position.

##### alireza1992 04/19/2021 10:25:47
Thank you;

But "reset" takes a lot of time, I want a faster process like defining fields for position of links and use from "node.getField()", but I have some problem with it
> **Attachment**: [RobotisOp3.proto](https://cdn.discordapp.com/attachments/565154703139405824/833649575390478336/RobotisOp3.proto)

##### Troy 04/19/2021 13:15:14
Hi guys, is there anyway to get a distance feedback in webots controller? I want to detect the distance between my robot and a solid, and get real time feedback, how can i do it?


And the solid will be random subject on the ground

##### Olivier Michel [Cyberbotics] 04/19/2021 13:20:53
Use a distance sensor.

##### Troy 04/19/2021 13:23:19
can i get lidar feedback in webots? not using ros?

##### Olivier Michel [Cyberbotics] 04/19/2021 13:27:45
Yes.

##### Srivastav\_Udit 04/19/2021 13:43:15
Are there any default properties for a solid in Webots? (for example, Young's Modulus etc.)


I'm trying to figure out what the properties of a solid are if the physics is enabled. What material would it represent? Plastic, metal, etc.

##### satyar 04/19/2021 16:25:13
Hi guys ,

Which version of Webots work completely with M1 processor MacBook !? Or dose webots work with m1 Mac !?

##### Saud 04/19/2021 18:36:49
Hello, I have collected lidar points by attaching LIDAR on DJI. exported points into a csv file. Now i want to represent it in 3D in maybe matlab. But it wants a PLY file. Can anyone give any suggestions on what i should do?

##### Simon Steinmann [Moderator] 04/19/2021 21:05:55
I think it should work through rosetta 2, like any other x86 program


Webots is a rigid body simulator, internal material stresses and deformations are not calculated. Bodies are rigid and only their kinematics and interaction is simulated.


you can convert / save the data as .ply files. [http://paulbourke.net/dataformats/ply/](http://paulbourke.net/dataformats/ply/) this might give you some insight in the format. Not that different from .obj files. What language are you using for your python controller?


You can also read the csv file in matlab and assign it manually: [https://www.mathworks.com/help/vision/ref/pcplayer.view.html](https://www.mathworks.com/help/vision/ref/pcplayer.view.html)

##### Nick R 04/19/2021 22:04:33
Can anyone tell me whether there's a quick way to 'killall webots' which will take care of anything hanging on after I close the GUI? I see a process lingering which is supposedly taking up half my GPU memory...

##### Saud 04/19/2021 22:06:42
right. thank you. i will have a look at this. I am using C++ btw for the controller of the robot

##### Simon Steinmann [Moderator] 04/19/2021 22:08:01
`@Saud` [https://github.com/ddiakopoulos/tinyply](https://github.com/ddiakopoulos/tinyply) this might help

##### Olivier Michel [Cyberbotics] 04/20/2021 05:33:30
I assume you are on Linux. Normally, killing Webots with SIGTERM should clean-up all the resources it uses.

##### Tosidis 04/20/2021 08:45:07
Hello, I just installed the latest nightly build because i was having problems with Supervisor.simulationReset() (webots kept crashing), but I still face the same issue. Are there any workarounds?

##### Asger Printz 04/20/2021 09:16:09
Hello Webots community. I have a problem with the control of the UR3 model. Whenever I give it a positive velocity it doesn't allow me to go into a negative angular position. I have changed it by converting it to base notes and removed the "boundingObject Group" how most of the links and joints and I have added touch sensors to them. Maybe that has messed something up? I have tried to mess around with minStop and maxStop of the hinge joint parameters, but nothing seems to help.

##### Emiliano Borghi 04/20/2021 15:30:25
Is it possible to have multiple ros controllers in simulation?

##### Olivier Michel [Cyberbotics] 04/20/2021 15:30:43
Yes.

##### Emiliano Borghi 04/20/2021 15:31:56
I tried setting  different --name as argument but one of the controller crashes without any explanation.


No worries, I was able to make it work. I chime in too early.

I'll post my results soon!

##### Nick R 04/20/2021 17:14:39
Just curious, is this for multi-robot simulation controlled externally from ROS? I'm interested in doing the same if so

##### alejanpa17 04/20/2021 22:01:59
Hello, I was trying to match the objects detected by the camera recognition and the measure distance by the point cloud of the lidar. Is there anyway you can make than sensor fusion/calibration in webots?

##### tomala 04/21/2021 00:06:33
Hi! I have problem with applying lua to proto. Even though lua output says red, if and elseif conditions are skipped. I was following lua documentation and everything seems ok, but I might be wrong
%figure
![if_not_working.PNG](https://cdn.discordapp.com/attachments/565154703139405824/834218518718316654/if_not_working.PNG)
%end

##### Olivier Michel [Cyberbotics] 04/21/2021 06:17:23
Did you try to declare the color and number variables non-local (e.g., remove the `local` keyword)?

##### DDaniel [Cyberbotics] 04/21/2021 06:25:19
`@tomala` also, you don't need to create full nodes for all three variants if you plan on using just one, can do something like this

```
Shape {
  appearance PBRAppearance {
    %{ if color == "red" then }%
      baseColor 0 0 0
    %{ elseif color == "blue" then }%
      baseColor 1 1 1
    %{ else }%
      baseColor 0.5 0.5 0.5
    %{ end }%
      transparency 0
      roughness 1 
      ...
  }
  geometry Mesh {
   ...
  }
}
```

##### Yi Zhou 04/21/2021 06:26:59
Hello guys, does anyone know how to use ikpy in webots? I found a example, but I cannot get all my link follow the example

##### Dorteel 04/21/2021 07:36:56
which robot are you using it with? It worked for us with the UR3

##### Yi Zhou 04/21/2021 07:37:31
franka panda robot

##### Tosidis 04/21/2021 10:36:43
Any updates for supervisor.simulationReset() (python) ? It still crashes webots, even with nightly build

##### Stefania Pedrazzi [Cyberbotics] 04/21/2021 11:05:45
Did you already provide an example where the crash is reproducible? Are you using Webots R2021a rev 1 or R2021b?

##### Tosidis 04/21/2021 11:48:18
webots\_2021a (stable) and webots\_2021a-rev1, happens in both. I am gonna create a very simple example (i suppose it will do the same) , so its minimal and easy to reproduce

##### Stefania Pedrazzi [Cyberbotics] 04/21/2021 11:49:35
Yes, having an simple example we will be able to inspect the issue. Because I tested and in my project it is working just fine, the simulation is reset and Webots doesn't crash.

##### Tosidis 04/21/2021 11:49:58
which version?

##### Stefania Pedrazzi [Cyberbotics] 04/21/2021 11:50:33
I tested both on the master (R2021a rev1) and develop (R2021b) branches.

##### Tosidis 04/21/2021 11:51:14
doing it now and i will post the code

##### alejanpa17 04/21/2021 11:58:58
why am I getting these fake recognition?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/834397800916582420/unknown.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 04/21/2021 12:01:30
We fixed some issues with the camera recognition functionality recently.

I would suggest you to try the latest nightly build of Webots R2021a rev1 and check if this issue is still reproducible:

[https://github.com/cyberbotics/webots/releases/tag/nightly\_19\_4\_2021](https://github.com/cyberbotics/webots/releases/tag/nightly_19_4_2021)

##### Tosidis 04/21/2021 12:32:48
Working on the mavic2 controller (translated to python)



robot = Supervisor()

root = robot.getRoot()

children = root.getField('children')



while robot.step(timestep) != -1:

    if robot.getTime() > 10:

        robot.simulationReset()

        human\_model = 'human.wbo'

        children.importMFNode(-1, human\_model)


if i dont import the node after each reset it works fine, but if I import the object it crashes (after a couple of resets)

version: R2021a rev1

Another thing happening, is that the drone won't get to the initial state after a couple of resets either (2 of the 4 motors stop moving)

##### younes 04/21/2021 12:40:09
hi guys i am using webots2021a and Matlab2018, i am trying to command an epuck2 using webot-matlab , while creating the world in webot , i'm getting this error :


remote control initialisation failed

[matlab] Error: Cannot load the "C:/Webots/projects/robots/gctronic/e-puck/plugins/remote\_controls/e-puck\_bluetooth/e-puck\_bluetooth.dll" remote control library.

##### Stefania Pedrazzi [Cyberbotics] 04/21/2021 12:51:49
A simple workaround to the crash that works for me is to change the import index for example from `-1` to `2` or any other index value that is valid after reset.

I will inspect why the `-1` is not correctly translated to valid index.

##### tomala 04/21/2021 13:11:42
It did work! Thank you very much! `@DDaniel` thank you too. I have been working long hours recently so I hadn't thought of simplifying code. Thank you all!

##### Yi Zhou 04/21/2021 13:15:35
Hello guys, does anyone know how to fix the robot position? I import a robot proto file, but when I run, the robot arm will fall down

##### Olivier Michel [Cyberbotics] 04/21/2021 13:21:16
Simple: remove the Physics node from the base Robot.

##### Yi Zhou 04/21/2021 13:23:06
Thank you, could you tell me how to remove? from webots? or from robot proto file?

##### Olivier Michel [Cyberbotics] 04/21/2021 13:23:32
From proto file.

##### Yi Zhou 04/21/2021 13:37:28
Thank you so much! Do you know how I can get the robot by python code?

##### alejanpa17 04/21/2021 14:50:26
Solved thanks ^^

##### Maurice Faraj 04/21/2021 21:08:01
Hello everyone hope all is fine I’m trying to combine a robot with. An robotic hand every part of this robot work prefectly alone but when I merge the 2 robot only the robotic hand work any help know how to solve this problem please

##### Stefania Pedrazzi [Cyberbotics] 04/22/2021 06:18:22
I would suggest you to start by following this tutorial: [https://www.cyberbotics.com/doc/guide/tutorial-4-more-about-controllers](https://www.cyberbotics.com/doc/guide/tutorial-4-more-about-controllers)


You could use a sensor, for example the Compass, to measure the orientation of the robot or a PositionSensor to measure how much the the motor turned

[https://www.cyberbotics.com/doc/reference/compass](https://www.cyberbotics.com/doc/reference/compass)

[https://www.cyberbotics.com/doc/reference/positionsensor](https://www.cyberbotics.com/doc/reference/positionsensor)

##### JSK 04/22/2021 07:58:41
Hello there. Any one know the working of compas in webots?


i want to know how does the values of -1 to 1 in compas deals with angle?


i mean if the compas value is 0.89 then what is it in degrees? is it a cosine relation?

##### Stefania Pedrazzi [Cyberbotics] 04/22/2021 08:06:27
The Compass device returns the vector direction pointing from the Compass center to the world north.

In the documentation you can find a sample code to compute the bearing angle based on this vector:

[https://www.cyberbotics.com/doc/reference/compass#compass-functions](https://www.cyberbotics.com/doc/reference/compass#compass-functions)

##### JSK 04/22/2021 08:35:17
Thanks Stefania..!! it worked

##### Haladin 04/22/2021 08:41:37
Hi.  I am getting an empty page when I click on the documentation links such as [https://www.cyberbotics.com/doc/reference/positionsensor](https://www.cyberbotics.com/doc/reference/positionsensor).  Is anyone else getting this?

##### Stefania Pedrazzi [Cyberbotics] 04/22/2021 08:44:09
Yes, the documentation is currently down.


Alternatively you can open the offline documentation from the Webots > Help > Offline Documentation > Reference Manual

##### Haladin 04/22/2021 08:45:44
Thanks Stefania.  I will check out the offline documentation.

##### Stefania Pedrazzi [Cyberbotics] 04/22/2021 08:45:53
or open the documentation from GitHub: [https://github.com/cyberbotics/webots/blob/released/docs/reference/positionsensor.md](https://github.com/cyberbotics/webots/blob/released/docs/reference/positionsensor.md)

##### Dorteel 04/22/2021 11:41:48
Hi there! I'm trying to replace robot.setVelocity(velocity) with robot.setPosition(currentPosition + velocity*timestep/1000), but getting different rates, setting it with position is about 3x slower. Is there a quick fix to this?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/834755869735387186/unknown.png)
%end

##### DDaniel [Cyberbotics] 04/22/2021 11:48:01
`@Dorteel` if you set a position that isn't infinity, it switches from velocity control to position control as described in this table: [https://cyberbotics.com/doc/reference/motor#force-and-torque-control](https://cyberbotics.com/doc/reference/motor#force-and-torque-control), the functions have different effects depending on which type of control you are doing

##### Dorteel 04/22/2021 12:10:56
Thanks  `@DDaniel`  I'll have a look at the table!

##### AndreiSPb 04/22/2021 14:38:30
Hello! Can someone explain how should I treat 2nd string for moments of inertia in physics of Solid? What Input should I make? I dont really get why the matrix is not 3x3... I can get values of 3x3 matrix from CAD.

##### Maël Wildi [Cyberbotics] 04/22/2021 14:42:34
The first vector is the diagonal [I11, I22, I33] of the matrix and the second one is for [I12, I13, I23] ([https://cyberbotics.com/doc/reference/physics](https://cyberbotics.com/doc/reference/physics))

##### AndreiSPb 04/22/2021 14:50:25
Well yeah I read that in docs and it seems not absolutely correct. Matrix can be not symmetrical with [I12, I13, I23] not equal to [I21, I31, I32]. Or am I missing something?

##### Maël Wildi [Cyberbotics] 04/22/2021 14:55:13
In the case of matrices of inertia I think they are the same because these terms are the products of inertia with respect to a plane, for instance I12 is for the XY plane = YX plane.

##### AndreiSPb 04/22/2021 19:03:14
Thank you for your answers!

In that case, what coordinate system is used in webots to define this matrix, how does it change during the movements and should I provide any coordinate transform to ensure the correctness of the matrix?

##### Simon Steinmann [Moderator] 04/22/2021 20:18:01
`@AndreiSPb` The inertia matrix is 3x3, but since the lower left corner is symetrical to the upper right corner, it is common to express with the diagonal, plus the upper right corner, which is precisely how it is done in Webots. Hope that helps
%figure
![slide_8.png](https://cdn.discordapp.com/attachments/565154703139405824/834885780269826068/slide_8.png)
%end

##### AndreiSPb 04/22/2021 20:47:44
Thank you!

What I was struggling with is that CAD gives Principal moments and axis in model frame:
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/834893258286432266/unknown.png)
%end


So I had to make some calculations
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/834893553552851014/unknown.png)
%end

##### Simon Steinmann [Moderator] 04/22/2021 21:10:10
what CAD program are you using?

##### AndreiSPb 04/22/2021 21:21:08
that was spaceclaim

##### Simon Steinmann [Moderator] 04/22/2021 21:44:25
`@AndreiSPb` [https://forum.ansys.com/discussion/13650/mass-concentreted-moment-of-inertia](https://forum.ansys.com/discussion/13650/mass-concentreted-moment-of-inertia) this might help

##### Welsh\_dragon64 04/22/2021 23:32:52
i uploaded a video of the robotisOp2 trying to walk as it picks up an item.
> **Attachment**: [pick\_and\_walk\_fail.mp4](https://cdn.discordapp.com/attachments/565154703139405824/834934816914276372/pick_and_walk_fail.mp4)


`@Simon Steinmann` `@Bitbots_Jasper`  i would appreciate any help

##### Simon Steinmann [Moderator] 04/22/2021 23:36:06
well for one you are setting positions that are outside of the range of motion for the robot


does the robot work without picking up the box first?

##### Welsh\_dragon64 04/23/2021 00:20:48
no it doesnt work


its falls down with or without picking up the box

##### Simon Steinmann [Moderator] 04/23/2021 00:35:05
well you have to get your walking motion in order then


try to avoid all those errors coming up


stay within the motor limits

##### thonk enthusiast 04/23/2021 05:22:22
Hello,

I was trying to use robotbenchmark tonight (not sure if that falls under webots support) but it seems like your session/simulation server's certificates have expired.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/835022772128055316/unknown.png)
%end

##### JSK 04/23/2021 06:13:08
Hello there. I have mess with the coordinates of the world and shifted it to ENU but the direction of gravity has changed to left, so whenever I start the simulation it drops to the left. I wanna say that it  does not fall on the ground rather it falls on left
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/835035545167069214/unknown.png)
%end


It should have to fall on square mainhole but it is not


kindly help me to change the gravity. i want to make north on y-axis, east on x-axis, and up on z-axis

##### Olivier Michel [Cyberbotics] 04/23/2021 06:43:00
Thanks for reporting this. It is now fixed. Our crontab script for the certificate renewal was not called properly.

##### Dorteel 04/23/2021 14:31:19
Hi! Does anyone has any suggestions how to get an image processed with opencv onto a display? I tried a stackoverflow solution from 2020 february ([https://stackoverflow.com/questions/58286019/webots-displaying-processed-numpy-image-opencv-python](https://stackoverflow.com/questions/58286019/webots-displaying-processed-numpy-image-opencv-python)) But that doesn't seem to work anymore (bytes object has no attribute 'tolist'). Also tried a sample code provided by `@Olivier Michel` ([https://github.com/cyberbotics/webots/pull/1033](https://github.com/cyberbotics/webots/pull/1033)) but that doesn't run either, even if I replace the deprecated function names with the newer ones. Does anyone have any idea how to solve this?

##### Bitbots\_Jasper [Moderator] 04/23/2021 14:40:08
python or c++?

##### Dorteel 04/23/2021 14:40:55
It's python

##### Bitbots\_Jasper [Moderator] 04/23/2021 14:42:57
```python
img_raw = self.camera.getImageArray()
img = np.array(img_raw, dtype=np.uint8)
img = np.swapaxes(img, 0, 1)
cv2.imshow("window", img)
cv2.waitKey(0)
```


something like this worked for us

##### Dorteel 04/23/2021 14:44:45
Thanks, that also works for me, but I need to display it on a display within the simulator, which doesn't seem to work

##### Bitbots\_Jasper [Moderator] 04/23/2021 14:47:39
sorry I misunderstood you question then, I dont have any experience with the webots display unfortunately

##### Dorteel 04/23/2021 14:49:19
This is the closest I got to it without error, but when I try to increase the resolution of the image simply nothing appears
%figure
![error.png](https://cdn.discordapp.com/attachments/565154703139405824/835165446240075786/error.png)
%end


No problem, thanks for trying to help!

##### Joanna 04/23/2021 18:29:35
Hello everybody,

I tried to run extern controller (following this tutorial [https://cyberbotics.com/doc/guide/running-extern-robot-controllers?tab-os=linux&tab-language=c++](https://cyberbotics.com/doc/guide/running-extern-robot-controllers?tab-os=linux&tab-language=c++)) from ros2 component in cpp to contol the ur5 simulation ([https://github.com/cyberbotics/webots\_ros2/wiki/Example-Universal-Robots](https://github.com/cyberbotics/webots_ros2/wiki/Example-Universal-Robots)). The controller in simulation is already extern and set to true. But I have this error in webots console. 

(WARNING: Failed to attach extern robot controller: no available "<extern>" robot controller found). Any help?

##### Alejandro Pescathore 04/23/2021 21:28:07
Hello everyone!! i'm modeling a robot arm in webots using HingeJoints and rotational motors as devices, how can do move all the joints of the robot at the same time? when i use the motor set position funcion only can move one by one joint

##### DDaniel [Cyberbotics] 04/23/2021 21:36:39
`@Alejandro Pescathore` you gave all rotational motors the same name? If so then yes, only one will move. The possibility of controlling multiple motors at once is being worked on at the moment, but for now you'll have to give each their own unique name, and in the controller get the corresponding `WbDeviceTag` and give the position command to each

##### dA$H 04/24/2021 19:19:59
hello.

how can i add a camera in webot world.

no in robot node


i want add camera here
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/835596573093920778/unknown.png)
%end

##### Simon Steinmann [Moderator] 04/24/2021 19:23:40
A camera has to be a child of a robot


so add a robot node, and then a camera to its children node

##### dA$H 04/24/2021 19:24:54
I did this but I could not get the camera on the controller

##### Simon Steinmann [Moderator] 04/24/2021 19:26:07
Is that controller launched by the robot node containing the camera?


And is the camera called "robot"?

##### dA$H 04/24/2021 19:28:04

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/835597984262520893/unknown.png)
%end

##### Simon Steinmann [Moderator] 04/24/2021 19:29:38
you have to use the name, not the DEF


[https://www.cyberbotics.com/doc/reference/robot?tab-language=python#wb\_robot\_get\_device](https://www.cyberbotics.com/doc/reference/robot?tab-language=python#wb_robot_get_device)


and this only works if it's the same robot


are you trying to get the camera from a different robot?

##### dA$H 04/24/2021 19:30:19
yes

##### Simon Steinmann [Moderator] 04/24/2021 19:30:55
it might be easier to add the camera to those robots, with a transform from the base (which is static I assume)


then you dont have to fiddle with extra controllers


A device is not the same as a node, and a controller only has access to the devices of the robot that started the controller

##### aslisevil 04/24/2021 19:38:23
Hello, I am currently making my Nao robot hold a box as exhibited in the photo. But my problem is that when I make it rotate with supervisor rotation function, it drops the box. Is there a way of fixing this problem. Your answers would be much appreciated.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/835600581388402688/unknown.png)
%end

##### Simon Steinmann [Moderator] 04/24/2021 19:38:54
how exactly do you rotate it?

##### aslisevil 04/24/2021 19:40:14
With setSFRotation function.


It drops the box while rotating with this function.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/835602230484271125/unknown.png)
%end

##### Simon Steinmann [Moderator] 04/24/2021 19:58:06
of course it does, you forgoe the physics simulation by simply changing the state


if you dont want it to drop, you have to move the box accordingly


use motors and joints to simulate with proper physics


As an ugly fix, you could perhaps try moving in very small increments and a lower timestep, but no guarantee that it works.

##### aslisevil 04/24/2021 20:09:08
okey thank you so much.

##### main 04/24/2021 20:28:52
When hinge joints unexpectedly turn by themselves is setting the static friction higher to prevent this good practice?

##### Dinossauro.Bebado 04/24/2021 20:53:31
hey guys, i am trying to import a robot file from solidworks to webots.Right now


its only import the solid


there is a easier way to solve this issue ?

##### Simon Steinmann [Moderator] 04/24/2021 20:54:33
try to export as an urdf file and import the urdf in webots

##### Dinossauro.Bebado 04/24/2021 20:55:40
will try, tks for the fast response

##### Yi Zhou 04/25/2021 07:31:56
Hello guys, does anyone know the reason that positionSensor value is nan?

##### DDaniel [Cyberbotics] 04/25/2021 11:14:24
`@Yi Zhou` hard to say, how do you get the position value? Can you show a snippet of the code?

##### tabousoud 04/25/2021 13:02:48
Hey everyone, noob question. I'm trying to build a robot from imported mesh geometry and can't figure out how to import a mesh as a bounding obj. When I import the mesh directly (from url) as a bounding object and try to assign physics, I get a warning that "boundingObj"  is  not defined and I can't assign mass or inertia. However, it *seems* to work if I first import the mesh using file -> Import 3D Model then create a DEF for the mesh shape to use in the robot. Is this the way it's supposed to work or am I missing something?

##### Simon Steinmann [Moderator] 04/25/2021 15:20:18
`@gus` [https://www.hmc.edu/lair/ARW/ARW-Lecture01-Odometry.pdf](https://www.hmc.edu/lair/ARW/ARW-Lecture01-Odometry.pdf) slide #23 has the formula


After some algebra, you are left with this formula:

`delta_motor_pos = theta_turn * axle_length / wheel_radius`


theta\_turn would be the turn angle in radians


delta\_motor\_pos is the difference in angle for the left and right wheel.


so you could turn one wheel + delta\_motor\_pos/2 and the other - delta\_motor\_pos/2

##### Sérgio 04/25/2021 19:33:12
Hi

##### main 04/26/2021 02:03:39
When using scaling on a robot, is the mass also scaled with this? e.g if my robot goes from 1 down to 0.5 scale, will my entered mass go from 10kg to 5kg in theory (If 10kg is entered in the physics node) Or will the mass still be 10kg, just the robot will be smaller?

##### Stefania Pedrazzi [Cyberbotics] 04/26/2021 06:06:38
Which  Device method are you using to load the image into the display?Because from the screenshot it seems that the format is wrong (for example 3 channels instead of 4 channels).

If you are using `wb_display_image_new`, you should make sure that the format that you pass to the Display API corresponds to the format of your image (e.g., RGB, ARGB, BGRA, etc.).

If you are using a Webots version older than R2021a, I would also suggest you to upgrade because we fixed issues with the Display image format.


This is probably a bug. I will inspect this.

However it is not recommended to use complex mesh geometries as boundingObjects because it will very likely cause instabilities in the physics computation. Instead, it is suggested to approximate the mesh using simple geometries (sphere, box, cylinder, capsule).


Yes, after scaling the mass is updated but not linearly as you would expect. When you scale a robot, its bounding objects are scaled as well. If all the Physics nodes of the robot define the density value (and not the mass value), then the actual mass (and resulting inertia matrix) of the robot should be scaled appropriately as well. However, if one of the Physics nodes of the robot defines a mass value explicitly, then, this mass will remain unscaled. For these reasons it is not recommended to scale robots because the physical behavior could no longer correspond to the originally unscaled robot.

##### ChristmasLights 04/26/2021 08:21:26
Hey everyone, is there anyone with the experience of using webots controller to control a web simulation? I have several motion files that I can run on the robot while using desktop application. But not sure about how can i manage to use this controller to control a web simulated robot.

##### Stefania Pedrazzi [Cyberbotics] 04/26/2021 09:45:02
It depends on which kind of web simulation are you using. If you refer to [Web Simulation]([https://www.cyberbotics.com/doc/guide/web-simulation](https://www.cyberbotics.com/doc/guide/web-simulation)) described in the User Guide, then a Webots instance and the controller is executed locally on a machine and the simulation updates are sent to the client browser.

##### ChristmasLights 04/26/2021 09:46:51
Yes, this is the one I am referring to. I am trying to use an external controller (written in Python) to run some motion files on a web simulation.

##### Stefania Pedrazzi [Cyberbotics] 04/26/2021 09:48:39
Once you start the Webots instance, and given that it is external, the controller, then the simulation should be streamed automatically. Note that in any case the controller is run locally on your machine/server and not on the web.

##### ChristmasLights 04/26/2021 09:51:50
Should I make any changes to my controller that works locally? I've followed this link for the local one: [https://www.cyberbotics.com/doc/guide/controller-programming](https://www.cyberbotics.com/doc/guide/controller-programming)

##### tabousoud 04/26/2021 09:52:19
Thank you for the follow up and advice!

##### Stefania Pedrazzi [Cyberbotics] 04/26/2021 09:54:19
No because the controller will in any case run locally.


`@ChristmasLights` I think that it is not clear what you want to achieve. Do you want to stream your simulation + controller on the web or do you want to modify the controller from the web simulation?

##### ChristmasLights 04/26/2021 09:59:26
I want to use an external controller on a web simulation (no need to modify it), have backend written in django. What I'm trying to achieve is, trigger some motion files depending on the state (assuming there are multiple states with multiple motion files are attached to). Like if situation 1 happens, my external controller should run the specific motion file on the simulation.

##### Stefania Pedrazzi [Cyberbotics] 04/26/2021 10:01:47
Ok, so I don't see where is the issue. Once you can run it locally, it can also be streamed on the web.

##### ChristmasLights 04/26/2021 10:39:41
When I try to run the local controller, it gives the following error: Webots doesn't seems to be ready yet: (retrying in 1 second). But its up and running and i can see the robot on localhost.

##### Yi Zhou 04/26/2021 10:43:52
Hello guys, I try to use getRangeImage to get the depth image of kinect, but the value is None, kinect works but no image data get, do anyone know the reason?

##### Stefania Pedrazzi [Cyberbotics] 04/26/2021 11:08:49
The external controller should run on the same computer as the Webots instance. Here are the instructions to use external controllers:

[https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers](https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers)

##### yash 04/26/2021 13:22:47
Hello ! Is the robot translation field updated at every time step .?

##### DDaniel [Cyberbotics] 04/26/2021 13:24:43
`@yash` Hi, if it changes yes.

##### yash 04/26/2021 13:26:15
Okay , thanks 👍


The time step at which the loop -

while robot.step(timestep)!=-1 is updated is as per the basic time step specified in the world info right .?

##### DDaniel [Cyberbotics] 04/26/2021 13:41:14
`@yash` don't know if I understand your question right, but the value you provide as argument (`timestep`) is the duration in milliseconds that will be simulated before the function is called again. Which ideally should be a multiple of `basicTimeStep` in worldInfo. So you can simulate the world very accurately, but sense/control at lower frequency.

##### yash 04/26/2021 13:52:35
> `@yash` don't know if I understand your question right, but the value you provide as argument (`timestep`) is the duration in milliseconds that will be simulated before the function is called again. Which ideally should be a multiple of `basicTimeStep` in worldInfo. So you can simulate the world very accurately, but sense/control at lower frequency.

`@DDaniel`  oh okay , I have understood a bit . Will try and implement. Thanks 👍

##### Stefania Pedrazzi [Cyberbotics] 04/26/2021 14:52:17
Did you enable the "kinect range" device of the Kinect PROTO node?

You can find here a sample C controller using the Kinect range image. The workflow is the same in each programming languages:

[https://www.cyberbotics.com/doc/guide/pioneer-3dx#pioneer3dx\_with\_kinect-wbt](https://www.cyberbotics.com/doc/guide/pioneer-3dx#pioneer3dx_with_kinect-wbt)

##### Yi Zhou 04/26/2021 14:53:17
Yes, I enable the camera

##### Stefania Pedrazzi [Cyberbotics] 04/26/2021 14:53:56
It is not enough to enable the camera device `kinect color`. You also have to enable the range finder device `kinect range`.

##### Yi Zhou 04/26/2021 14:55:26
Sure, I enable this, I can see image from webots, but I cannot get it

##### Stefania Pedrazzi [Cyberbotics] 04/26/2021 14:56:20
What code do you use to get the range finder data?


And the range finder device

##### Yi Zhou 04/26/2021 14:58:49
img\_array = self.kinect\_range.getRangeImage()    self.kinect\_range = self.getDevice("kinect range")

##### Stefania Pedrazzi [Cyberbotics] 04/26/2021 15:02:25
Here is a sample Python controller that works just fine in printing the Kinect range finder image:

```
from controller import Robot

robot = Robot()
timestep = int(robot.getBasicTimeStep())
kinect_range = robot.getDevice("kinect range")
kinect_range.enable(timestep)

while robot.step(timestep) != -1:
    print(kinect_range.getRangeImage())
```

##### Yi Zhou 04/26/2021 15:03:55
Thanks. I'll try😀

##### Fareed 04/26/2021 17:06:15
Hi there! I'm a student from New Rochelle High School in the Advanced Robotics class. While using WeBots we ran into several glitches which left me and my classmates stumped. Such examples are as followed: The robot itself glitching through the floor, the robot flinging itself while holding the ring, some portion of the robot being left behind while moving up or down on the Z-axis, and the robot falling through the floor even though physics are enabled. Some of these issues were resolved after shutting down and reopening WeBots but later persisted. If you have a fix to these problems please let me know.


Heres a picture of the part remaining afloat yet when we move it it will move back to its original positioning
%figure
![Screenshot_2021-04-26_130719.png](https://cdn.discordapp.com/attachments/565154703139405824/836287863640817704/Screenshot_2021-04-26_130719.png)
%end

##### ChristmasLights 04/26/2021 20:39:25
For now, I am running the web server locally (on the same computer with the controller), but still getting the mentioned error. When I use the controller with the desktop app, there is no problem. I am using the web simulation with the docker, settings are mentioned as in the link. Maybe there is something I am missing with external controller. Is there any setting that I should change in the dockerfile in order to use my external controller on the web simulation? As long as I know, the webots is using the default nao controller instead of the external.

##### fero taraba 04/26/2021 21:33:15
Hello, my HTML export does not contain labels created via "set\_label" method. Should it be there or is it currently not supported?

##### Simon Steinmann [Moderator] 04/26/2021 22:55:08
`@Fareed` I suggest you fix the warnings that Webots shows you.

##### Yi Zhou 04/27/2021 06:52:21
Hi, the camera can be enabled, but both color image and depth image cannot show, is it possible because of the kinect camera?

##### Johan2021 04/27/2021 09:18:45
Hi all, I would have to detect a collision/distance threshold between a dynamic body and any other body in the world, and subsequently align these 2 bodies - very much like the connector snap functionality (which I would not be using). For the collision I would implement an IR sensor. However, I am not sure how to align these 2 bodies/retrieve their orientation - since it is not known a priori which body the other body should align with. Would anyone know more about this?

##### tabousoud 04/27/2021 10:12:43
Hi all, is there a way to construct other simple shapes in Webots beyond the basic primitives? For example, truncated cones (drafted cylinder) or prism-type shapes? Thanks!

##### DDaniel [Cyberbotics] 04/27/2021 10:25:18
`@tabousoud` Hi, not directly but you can create them with other software and import them: [https://cyberbotics.com/doc/reference/mesh](https://cyberbotics.com/doc/reference/mesh)

##### tabousoud 04/27/2021 10:33:21
`@DDaniel` Hi Daniel, thanks for the feedback. I want to use these shapes for bounding object geometry and it seems that the recommendation is to avoid imported mesh geometry for that, so I was trying to use standard shapes in Webots. Does that same caveat also apply for simple meshes?

##### DDaniel [Cyberbotics] 04/27/2021 10:44:59
Yes, generally it is suggested to avoid very complex shapes (typically, using the mesh of the piece itself, with potentially hundreds or thousands of faces is overkill) for boundingObjects as it makes the collision detection computationally very intensive and at times also less reliable, but if none of the basic shapes (or a combination of them) fits the bill for your application you can still use something else. As for the primitive shapes, keep in mind you can group more of them together

##### tabousoud 04/27/2021 10:46:41
`@DDaniel` Got it, many thanks!

##### Dorteel 04/27/2021 12:16:40
Thanks! I managed to get it work, but it's a pretty ugly solution:



`       cameraData = camera.getImage()

        image = ... bunch of processing with openCV

        # Displaying the camera image directly

        #ir = display\_score.imageNew(cameraData, Display.BGRA, camera.getWidth(), camera.getHeight())

        # Displaying the processed image

        cv2.imwrite('tmp.jpg', image)

        ir = display\_score.imageLoad('tmp.jpg')

        display\_score.imagePaste(ir, 0, 0, False)

        display\_score.imageDelete(ir)`



The commented line displays the right image directly from the camera, but I couldn't get to display the processed image with the `imageNew` function

##### Venkat 04/27/2021 12:52:25
Hello All,


Hello All, I am looking to integrate a LIDAR sensor from SICK, that is not already available in the model database of webots. I am looking for the information regarding using the laser scanners that arent available within webots yet. If anyone have information, kindly help !!!

##### Darko Lukić [Cyberbotics] 04/27/2021 12:55:40
You can always use the generic Lidar node:

[https://cyberbotics.com/doc/reference/lidar](https://cyberbotics.com/doc/reference/lidar)

and parameterize it so it has the same characteristics as your Lidar.

##### Hamed 04/27/2021 13:27:22
Hello, I want to make a matlab controller, but it doesn't work and it could not start matlab.

Matlab is run correctly with the command window



%figure
![IMG-20210427-WA0003.jpg](https://cdn.discordapp.com/attachments/565154703139405824/836594553770213396/IMG-20210427-WA0003.jpg)
%end

##### Darko Lukić [Cyberbotics] 04/27/2021 13:38:34
You are probably affected by this bug:

[https://github.com/cyberbotics/webots/issues/2608](https://github.com/cyberbotics/webots/issues/2608)



which is fixed in (R2021a rev1):

[https://github.com/cyberbotics/webots/pull/2624](https://github.com/cyberbotics/webots/pull/2624)



Unfortunately, our Windows CI doesn't work at the moment and we don't R2021a rev1 nightly build:

[https://github.com/cyberbotics/webots/releases](https://github.com/cyberbotics/webots/releases)



Until the nightly builds are back you can install MATLAB in a `matlab` folder.

##### James Le Poidevin 04/27/2021 13:41:49
Hello, I saw on a ROS forum that webots is debating using URDF files instead of proto files ([https://discourse.ros.org/t/urdf-with-webots/19665](https://discourse.ros.org/t/urdf-with-webots/19665)).  Do you have any idea how you are going to proceed for indexedFaces seeing that webots doesn't not keep a reference of the original mesh file(stl,dae ...)  once imported. I ask the question because i'm currently try to convert a robot containing mainly meshs and i would like to create a urdf for this robot.

##### Darko Lukić [Cyberbotics] 04/27/2021 13:43:04
In Webots, you can include meshes or reference them. See the Mesh node:

[https://cyberbotics.com/doc/reference/mesh](https://cyberbotics.com/doc/reference/mesh)

##### James Le Poidevin 04/27/2021 13:46:57
OK thank you.


But they still are exported in the urdf even as a mesh ?

##### Darko Lukić [Cyberbotics] 04/27/2021 14:26:25
The URDF export is limited, it replaces visual meshes by bounding objects:

[https://cyberbotics.com/doc/reference/robot#wb\_robot\_get\_urdf](https://cyberbotics.com/doc/reference/robot#wb_robot_get_urdf) (check the note)



Regarding the Discourse post, we concluded that we want to allow URDF importation (the other way around).

##### James Le Poidevin 04/27/2021 14:27:45
Ok I see, Thank you.

##### Simon Steinmann [Moderator] 04/27/2021 19:56:44
`@James Le Poidevin` I've written a tool that extracts the meshes from proto files. So using the webots -> urdf export, and a bit of manual adjusting, you can get a full urdf file with all visual meshes included


[https://github.com/cyberbotics/webots/blob/master/scripts/proto\_splitter/proto2mesh.py](https://github.com/cyberbotics/webots/blob/master/scripts/proto_splitter/proto2mesh.py)

##### Affonso 04/27/2021 20:10:39
Do Webots have a native delay function on the controller with C?

##### Simon Steinmann [Moderator] 04/27/2021 20:11:51
You can use a normal C delay function?!


Webots controllers only continue the simulation when you explicitly call the robot step function

##### Affonso 04/27/2021 20:13:00
We are using this


void delay(int time, int atual)

{

    while (wb\_robot\_step(TIME\_STEP) != -1) {

      if (wb\_robot\_get\_time() > time + atual){

        break;

       }

  }

}


Is there an error?

##### Simon Steinmann [Moderator] 04/27/2021 20:13:40
what do you want to achieve?

##### Affonso 04/27/2021 20:15:10
My robot needs make a reverse curve when the infra sensor detects the line

##### Simon Steinmann [Moderator] 04/27/2021 20:16:19
so you want a function that simply executes the current commands for a certain amount of time?

##### Affonso 04/27/2021 20:16:52
exactly


Now we are calling a function that curves and interrupts the code using delay

##### Simon Steinmann [Moderator] 04/27/2021 20:18:03
if (wb\_robot\_get\_time() < t\_simulate){

        continue;


you could put that at the start of your loop


and when you want the main loop to skip any logic, you set the t\_simulate to the current sim time + the duration


as soon as wb\_robot\_get\_time() is bigger than that t\_sim, it will execute the rest of your loop


also, time is double, not int

##### Affonso 04/27/2021 20:21:53
Ok, but i need use this logic of delay more times in the code that's why I create the function above

##### Simon Steinmann [Moderator] 04/27/2021 20:22:20
void delay(double duration)

{

    double time\_end = wb\_robot\_get\_time() + duration;

    while (wb\_robot\_step(TIME\_STEP) != -1) {

      if (wb\_robot\_get\_time() > time\_end ){

        break;

       }

  }

}

##### Affonso 04/27/2021 20:22:29
You think that will works?

##### Simon Steinmann [Moderator] 04/27/2021 20:22:33
not a c programmer, but perhaps like this?

##### Affonso 04/27/2021 20:22:45
yeah


thanks

##### Simon Steinmann [Moderator] 04/27/2021 20:23:08
you're welcome 🙂


make sure to use the documentation [https://www.cyberbotics.com/doc/reference/robot?tab-language=c#wb\_robot\_get\_time](https://www.cyberbotics.com/doc/reference/robot?tab-language=c#wb_robot_get_time)


it has all variable types and functions explained

##### main 04/27/2021 21:35:51
`@Stefania Pedrazzi` Thanks, so say if i was to make a leg 30kg and scale it to 0.5, it will remain at 30kg (just in a smaller bounding box area?)

##### Simon Steinmann [Moderator] 04/27/2021 21:38:59
`@main` it depends on whether you define the weight through mass or density


if you define the mass as 30kg, it will remain the same

##### tabousoud 04/27/2021 21:55:56
Hi All, 

Quick question: For a camera node, which axis aligns with the camera's line of vision? I looked in the docs but can't seem to find it.

##### Ragemor 04/27/2021 23:07:56
ı have different colored objects in my simulation. and i want to move my robots to different colored objects. how can i perform that? basicly a leader follower concept with camera recognition. I try to use objects[].colors[] but i cant made what i want. my english is not pretty good. anyone can help me about that?


this is my simulation. reds first and greens behind them. this is working for a while but if green object robots see another colored robot it stopped moving.
%figure
![webbb.jpg](https://cdn.discordapp.com/attachments/565154703139405824/836740552145764412/webbb.jpg)
%end


and this is my code [https://discord.com/channels/565154702715518986/565155651395780609/836732423982415883](https://discord.com/channels/565154702715518986/565155651395780609/836732423982415883)

##### nslinco 04/28/2021 02:04:51
Hello all! I'm currently working with the Neuronics IprHd6ms180 and the gripper only opens partially. I'm using a python controller and the following line to alter the motor position: "motors[len(motors)-1].setPosition(1.22171)". Does anyone have experience with a similar problem?


Altering the value passed to setPosition affects the correct motor, but it only opens a fraction of the amount it should


Nvm

##### Joanna 04/28/2021 06:03:16
`@Darko Lukić` Hello everybody,

I tried to run extern controller (following this tutorial [https://cyberbotics.com/doc/guide/running-extern-robot-controllers?tab-os=linux&tab-language=c++](https://cyberbotics.com/doc/guide/running-extern-robot-controllers?tab-os=linux&tab-language=c++)) from ros2 component in cpp to contol the ur5 simulation ([https://github.com/cyberbotics/webots\_ros2/wiki/Example-Universal-Robots](https://github.com/cyberbotics/webots_ros2/wiki/Example-Universal-Robots)). The controller in simulation is already extern and set to true. But I have this error in webots console. 

(WARNING: Failed to attach extern robot controller: no available "<extern>" robot controller found). Any help?

##### fero taraba 04/28/2021 15:46:37
Hello, my HTML export does not contain labels created via "set\_label" method. Should it be there or is it currently not supported?

##### Benjamin Délèze [Cyberbotics] 04/28/2021 15:57:42
Hello, sorry labels are currently not supported in HTML exports

##### Dorteel 04/28/2021 19:07:11
Hi there! Is it possible to change the mass of the object through supervisor? I have troubles getting to the Physics node of an object in the scene tree without using a specific DEF tag on the Physics node

##### Dizz 04/29/2021 02:32:53
would anyone happen to know how to adjust the robots rotation in code? currently programming in python.

##### Simon Steinmann [Moderator] 04/29/2021 02:33:31
using a supervisor controller


[https://cyberbotics.com/doc/reference/supervisor?tab-language=python](https://cyberbotics.com/doc/reference/supervisor?tab-language=python)


from controller import Supervisor



robot = Supervisor()

robotNode = robot.getSelf()

robotNode.setOrientation()


check the documentation, that is from what I remember, might be wrong

##### Dizz 04/29/2021 02:36:02
Thank you for your help!

##### Simon Steinmann [Moderator] 04/29/2021 02:36:16
oh and you have to make the robot a supervisor. Set it to true in the scene tree

##### bingdong 04/29/2021 06:57:02
Hi, can Python multiprocessing be run in the controller script?

##### SeanLuTW 04/29/2021 09:23:54
Hi, I'm writing a script to run and record the simulation. My controller add several PROTOs into my world during simulator running and the cursor will highlight the added object. So in my video, the object's coordinate (three axes with RGB) will be recorded. How can I avoid this?

##### Stefania Pedrazzi [Cyberbotics] 04/29/2021 09:33:58
One option is to hide the handles to move the object (the three axes with RGB and arrows) from the "View" > "Scene Interactions" > "Disable Object Move". Note that handles are only disabled if the node is selected from the 3D scene and not from the scene tree.

##### jasonc1025 04/29/2021 11:41:06
Thx `@Stefania Pedrazzi`  for helping me with my attempted "Web Simulation: Quick Start: 'Simulation starting' " frozen status.   (I've move my question-thread over to Webots, to be topic-consistent.)  ; )  Unfortunately, I'm not that good w/ PHP for Website Hosting.  



Just curious re: my 'Quick Start: Simulation starting...' frozen status, how important do my ports need to be opened? (1999 and 2000 are opened, yet my Comcast-xFinity is having issues opening 443, 2001+ as I read somewhere as being needed.) Btw, Is there a 'LocalHost' approach to see if it can work w/in my own LAN? If this could help, a 'localhost:2000/monitor' has been attached. I would be happy to post my session & simulation output.log, if needed.  Thx  for anyone's kind support.  : ) (Thx Stefania for the earlier reply: [https://discord.com/channels/595546443498913822/595550130019631105/837270555132166164](https://discord.com/channels/595546443498913822/595550130019631105/837270555132166164))
%figure
![Screenshot_from_2021-04-29_02-59-22.png](https://cdn.discordapp.com/attachments/565154703139405824/837292407228858438/Screenshot_from_2021-04-29_02-59-22.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 04/29/2021 11:51:23
Ports 1999, 2000 are used by the web simulation server, ports 2001+ are used by the Webots instances. So you have to make sure that at least ports to 1999 to 2005 are open. Note that this range of port is configurable as described in documentation.

Port 443 is required to use HTTPS so it will probably be open by default.


All what is described in the documentation runs on localhost.

If you want to access from another computer in the same network, then you should use the IP address.

##### jasonc1025 04/29/2021 11:54:42
`@Olivier Michel` and related Colleagues, as program manager... my supervisors have been very impressed with Webots and we would be honored to be annual subscribers if we can get Web Simulation working, as we're looking for an open-source Robotics Sim platform to develop EdTech curriculum with our partner NASA ISS.  Yet our server experience is more node.js and python (flask-based), vs. PHP & Java.  We're based in Silicon Valley, CA USA ([https://questforspace.com/](https://questforspace.com/)), so feel free to verify us as needed.  ; )


`@Olivier Michel` , `@Stefania Pedrazzi`, et al,  re: Web Simulation, due to our limited server skill-sets, e.g. running Docker would be cool, yet we need just simple copy & paste lines to 'mindlessly' create and run the Docker image.  Hope this makes sense.  ; )


Thx `@Stefania Pedrazzi`, I'll see what I can do to get those ports opened.  : )

##### Stefania Pedrazzi [Cyberbotics] 04/29/2021 12:00:21
You can find here some instructions to run the simulation server locally on a Docker container:

[https://github.com/cyberbotics/webots/wiki/Docker#run-streaming-server-in-docker-container](https://github.com/cyberbotics/webots/wiki/Docker#run-streaming-server-in-docker-container)

Note that to use it in production and make it available online you need additional steps.

##### jasonc1025 04/29/2021 12:03:11
Wow, thx `@Stefania Pedrazzi`, that's way cool.  My staff and I will 'hack away' at the helpful Github Docker recipe.  ; )

##### Stefania Pedrazzi [Cyberbotics] 04/29/2021 12:04:14
In any case, yes with paid support we will help you setting up your web simulation server.

You can ask for an offer at sales@cyberbotics.com.

##### jasonc1025 04/29/2021 12:08:03
Thx for such timely and helpful responses.   Great, I can relay this helpful info back to my supervisors, to see what we can do for a hopeful long-term partnership.  : )

##### Venkat 04/29/2021 15:43:40
`@Darko Lukić` Thank you. I was able to do so. With a 270 degree scanning angle for the lidar, I am able to visualize the "Lidar Ray Path" exactly the way being defined. But I am unable to see the "Lidar Point cloud" as expected. The point cloud visualization displays light around few areas, but not for the whole area (270 degrees). I tried slowing the simulation by reducing the basicTimeStep, but still the display remains the same.

##### havilesa 04/29/2021 16:33:15
Hi everyone, I am new to this forum, my robot do not seem to respond to setPositition request. I'm using Python and sojourner. Any help is appreciated: """usodecamarasojourner controller."""





back\_left\_bogie = 0

front\_left\_bogie = 1

front\_left\_arm = 2

back\_left\_arm = 3

front\_left\_wheel = 4

middle\_left\_wheel= 5

back\_left\_wheel = 6 

back\_right\_bogie = 7

front\_right\_bogie = 8 

front\_right\_arm = 9

back\_right\_arm = 10

front\_right\_wheel = 11

middle\_right\_wheel = 12

back\_right\_wheel = 13



part\_name = ['BackLeftBogie','FrontLeftBogie',

'FrontLeftArm','BackLeftArm', 'FrontLeftWheel','MiddleLeftWheel',

'BackLeftWheel','BackRightBogie', 'FrontRightBogie',

'FrontRightArm','BackRightArm', 'FrontRightWheel', 'MiddleRightWheel',

'BackRightWheel']



joints = []

for i in range(len(part\_name)):

  joints.append(robot.getDevice(part\_name[i]))

  joints[i].setPosition(float('inf'))

  joints[i].setVelocity(0.0)

  

VELOCITY = 0.6  

def move\_6\_wheels(v): 

  joints[middle\_right\_wheel].setAvailableTorque( 2.0)

  joints[middle\_left\_wheel].setAvailableTorque( 2.0)



  joints[front\_left\_wheel].setVelocity( v * VELOCITY)

  joints[middle\_left\_wheel].setVelocity( v * VELOCITY)

  joints[back\_left\_wheel].setVelocity( v * VELOCITY)

  joints[front\_right\_wheel].setVelocity( v * VELOCITY)

  joints[middle\_right\_wheel].setVelocity( v * VELOCITY)

  joints[back\_right\_wheel].setVelocity( v * VELOCITY)



def turn\_wheels\_right(): 

  joints[front\_left\_arm].setPosition(0.4)

  joints[front\_right\_arm].setPosition(0.227)

  joints[back\_right\_arm].setPosition(-0.227)

  joints[back\_left\_arm].setPosition(-0.4)





def turn\_wheels\_left(): 

  joints[front\_left\_arm].setPosition(-0.227)

  joints[front\_right\_arm].setPosition(-0.4)

  joints[back\_right\_arm].setPosition(0.4)

  joints[back\_left\_arm].setPosition(0.227)

##### Skev 04/29/2021 19:11:34
Hello, my base nodes list when I click on "add a node" doesn't seem to be complete. Is there something wrong with my version or the cameras, lightsensors... are somewhere else ?

##### DDaniel [Cyberbotics] 04/29/2021 19:15:07
`@Skev` hi, only the compatible nodes you can put in the spot you're currently selecting will show up. Add a Robot node first and then select the children field before clicking the (+) button


`@havilesa` hi, how do you do it? Can you show a snippet of the relevant code?

##### Skev 04/29/2021 19:19:19
Oh thank you very much !

##### devPRS 04/29/2021 20:30:25
Hello everyone, I have recently implemented radars in my webots world. I noticed however that I do not get multiple detections for a large building, instead I just get one single hit. I have used radars quite a bit in the real world and the simulated data was not quite what I expected.


Is there anything I can do about this?

##### DDaniel [Cyberbotics] 04/29/2021 21:45:27
`@havilesa` By doing this 

```
joints[i].setPosition(float('inf'))
joints[i].setVelocity(0.0)
```

you're setting the motors for Velocity control, but afterwards with this `joints[front_left_arm].setPosition(-0.227)` you're switching them to Position control and in Position control what you provided using the setVelocity (0) specifies the maximal velocity, so they won't move. Take a look at this table [https://cyberbotics.com/doc/reference/motor#motor-control-summary](https://cyberbotics.com/doc/reference/motor#motor-control-summary)

##### Darth Jon 04/30/2021 01:10:34
Hi there, I have a question I couldn't an answer anywhere still: Does webots support machining simulation, like milling or cutting by a robotic end effector?

##### Simon Steinmann [Moderator] 04/30/2021 01:12:37
`@Darth Jon` No, Solids are "indestructible". You could probably find a complicated way to make it work, but it does not support such a feature.

##### Darth Jon 04/30/2021 01:13:00
Tyvm Simon!

##### Simon Steinmann [Moderator] 04/30/2021 01:13:24
May I ask what exactly it is you want to accomplish?

##### Darth Jon 04/30/2021 01:14:35
I'm deciding which platform I'm going to use in my PhD, I'm comparing the features and limitations of each

##### Simon Steinmann [Moderator] 04/30/2021 01:15:03
What are your requirements? What exactly do you want to simulate?

##### Darth Jon 04/30/2021 01:17:06
I'll develop the digital twin of an automation laboratory of university and develop a CPS on top of it and simulating all the factory processes may be interesting from a research point of view


I'm in the preliminary stages so may need it or not


not sure still


but once I start, there's no way back kinda


wont migrate all work after months of development

##### Simon Steinmann [Moderator] 04/30/2021 01:19:12
You wont find a robotic simulator who can actually simulate machining.


like FEM and CFM can't be done properly in real time in a rigid body physics engine


The robot simulators such as Gazebo, Webots, PyBullet, Mujoco etc. are rigid body simulators


you can simulate all the movement, but not the actual cutting process

##### Darth Jon 04/30/2021 01:25:10
It probably doesn't use FEM but I found one that allows mesh manipulation in real time, so the robots can simulate cut and milling but it has another limitations like very small robot library

##### Simon Steinmann [Moderator] 04/30/2021 01:25:23
which one?

##### Darth Jon 04/30/2021 01:26:02
Coppeliasim (new name for V-REP)

##### Simon Steinmann [Moderator] 04/30/2021 01:26:54
it's proprietary. I'd stick with open source and free if I were you


also has lots of other drawbacks

##### Darth Jon 04/30/2021 01:28:03
yeah, it can be used for PhD cause it has an educational free license but yeah it has weakness too that's why I'm studying the case

##### Simon Steinmann [Moderator] 04/30/2021 01:28:26
I did an academic paper on robotic simulators, pm me if you like. We shouldnt spam the channel here

##### Darth Jon 04/30/2021 01:28:45
right

##### Darko Lukić [Cyberbotics] 04/30/2021 06:54:46
If a LiDAR measurement is outside the provided range then the measurement is considered invalid and thus, it is not displayed

##### L'etranger 04/30/2021 11:59:04
Hey everyone. I would appreciate some assistance. I'm working with the built-in "iRobot's Create" world but I've run into an issue. 



Within the controller for the robot (create\_avoid\_obstacles.c), a turn() function is specified that takes an angle in Radians as input. 



I've messed around with the function, giving it specific instead of random values, like 90 ° (M\_PI / 2) but I notice that it's extremely inaccurate and imprecise.



For example, if I give 180°, it only turns by about 140°. If I give 90°, it only turns by about 60. Can anyone help me with this?

##### Beginner26798 04/30/2021 14:21:25
Hi guys im attaching a ure hand to an irobot vacum cleaner but when the irobot move and the hand rotate , the hand detached from the robot how can i fix this problem

##### Markita 04/30/2021 14:58:46
Hi everyone! Does anybody know how can I change the contact properties so that they are "glued" together? I just can't figure it out

##### sa33 04/30/2021 16:36:39
Hi guys, I am working on a robot attached with 2 solid frames getting rotated independently. outer frame will rotate along one axis and the inner frame is connected to outer with a motor and rotates in another axis. so the inner has to rotate along its own axis with outer frame. I have anchored the frames same as the endpoint translation. outer is working fine but inner frame is not rotating properly. Inner frame's anchoring is not working properly and its anchor position is changing while rotation.  Can anyone help me to solve this issue?

##### Beginner26798 04/30/2021 20:02:58
Anyone??!!

##### Simon Steinmann [Moderator] 04/30/2021 21:35:45
`@Beginner26798` can you post a screenshot of the problem?


There is no adahesion. You can increase the friction by a lot, so they wont move, but it will still require some force to press the two objects together. If you want them to be actually stuck together, you either have to link them with a transform node, or use connector nodes


[https://cyberbotics.com/doc/reference/connector?tab-language=python](https://cyberbotics.com/doc/reference/connector?tab-language=python)

##### Nick R 04/30/2021 22:23:30
Can anyone explain why the Velodyne HDL-32 sensors don't have concentric rings where they hit the floor, but star-like patterns - is this intentional? The VLP-16 (superimposed in white) has circular rings as I would expect. [https://imgur.com/a/zcl5RCT](https://imgur.com/a/zcl5RCT)

##### Simon Steinmann [Moderator] 04/30/2021 22:23:40
Your problem is hard to visualize. Can you share a screenshot or quick video?


It seems like it has 6 sensors aranged in a circle pointing outward


if you overlay their waves, that would result in the pattern you see


hmm no, that does not seem to be it

##### Nick R 04/30/2021 22:29:39
Based on real data ([https://velodynelidar.com/products/hdl-32e/](https://velodynelidar.com/products/hdl-32e/)) something looks quite off

##### Simon Steinmann [Moderator] 04/30/2021 22:29:47
ahh, it is rotating. Try lowering the timestep and see if the pattern changes

##### Nick R 04/30/2021 22:31:24
Thanks, still confused about how to lower the timestep. As with my tests before, this is running as an external controller... launched with the ROS2 interface

##### Simon Steinmann [Moderator] 04/30/2021 22:31:40
in world info, you set the timestep lower


looking at the HDL-32 proto file, you can see the properties:

`tiltAngle -0.19896287

  horizontalResolution 4500  # 360 / 0.08

  fieldOfView 1.570795       # 6.28318 / 4

  verticalFieldOfView 0.468

  numberOfLayers 64

  minRange 0.8

  maxRange 120

  spherical TRUE

  type "rotating"

  noise 0.000167 # 0.02 / 120

  defaultFrequency 5

  minFrequency 5

  maxFrequency 15`


so it rotates with 5 hz


at 32ms timestep, your simulation runs at about 30 hz


so one rotation only has about 6 positions that get simulated


hence the star pattern


Does that make sense?

##### Nick R 04/30/2021 22:34:19
Yes it does, thank you 🙂 If I set the WorldInfo timestep lower then they approach circular rings. But of course the RTF takes a hit

##### Simon Steinmann [Moderator] 04/30/2021 22:34:23
If you were to lower the timestep to 16ms, one rotation of the lidar would have 12 positions and so on


yes, that is the issue


you could fake it and make a surround lidar with 360° FOV

##### Nick R 04/30/2021 22:41:40
Does this amount to using the basic Lidar and setting type to "fixed" ? Any way to override the rotation of the HDL-32 or I have to go back to using the basic LIDAR if I want to achieve this?

##### Simon Steinmann [Moderator] 04/30/2021 22:42:50
A basic lidar I assume. You can also duplicate the proto file and adjust it


changed it to fixed and 360°


try this
> **Attachment**: [VelodyneHDL-32E\_360.proto](https://cdn.discordapp.com/attachments/565154703139405824/837821899840552991/VelodyneHDL-32E_360.proto)


the file name and the proto name (line:8 in the file) have to be identical, otherwise Webots does not open it

## May

##### Steven37 05/01/2021 11:10:33
I have a robot like in the video and when it moves it will recognize the objects it has passed and read the color of that object. However, as you can see in the camera image, the colors of these objects will be read again and again. So is there a way for me to get the camera to only recognize each object once and which object has already read its color then won't read it again? Thank you so much!
> **Attachment**: [3YProject\_SagaRobot\_ask\_recognise\_1\_object.mp4](https://cdn.discordapp.com/attachments/565154703139405824/838009497606815744/3YProject_SagaRobot_ask_recognise_1_object.mp4)

##### dA$H 05/01/2021 12:54:12
Does anyone know why my controller stops?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/838035582629576704/unknown.png)
%end


Prints two link.names and then stops


I'm trying to implement inverse kinematic on ure10


According to a example in webot.

inverse\_kinematic.wbt

##### Dorteel 05/01/2021 15:10:50
The error message says "timeStep not defined", correcting "timeStep" to "timestep" with lower s should solve the issue

##### mileny 05/01/2021 17:17:47
Hi everyone, is it possible to create a sticky wheel on the webots?

##### Mohannad Kassar 05/01/2021 17:24:26
Hello, I just finished a 3D model using solid-works, I export the VRML file (.wrl), how can I import it to webots to program it ?

##### Markita 05/01/2021 17:44:34
Thank you `@Simon Steinmann`  !!


I think my question was pretty much the same, check the answer just above, maybe it helps you

##### mileny 05/01/2021 18:03:08
I will try that, thank you!

##### dA$H 05/01/2021 18:07:45
thanks man.


Does anyone know the cause of this error?

Robot: URE10

Inverse kinematic
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/838124628006010900/unknown.png)
%end

##### Simon Steinmann [Moderator] 05/01/2021 18:48:49
arrays start with 0. So an array with size 8 goes from 0-7

##### dA$H 05/01/2021 18:50:37
Where is more than 7?


len(motors) = 6


line 166: ikResults = armChain.inverse\_kinematics([x, y, z], max\_iter=IKPY\_MAX\_ITERATIONS, initial\_position=initial\_position)

##### Gustavo Cruz 05/01/2021 19:59:17
Helo! I am trying to create a web simulation service, but I have a problem. I start the session and simulation servers normally, but when I try to start a new simulation by the client an error appears in the command prompt of the servers and the client is in the state of starting simulation. Does anyone know how to solve this problem? Thank you very much!
%figure
![Erro2.PNG](https://cdn.discordapp.com/attachments/565154703139405824/838142556395143188/Erro2.PNG)
%end

##### Maurice Faraj 05/01/2021 20:17:01
Hello everyone hope all is fine I’m facing a problem in my project that my ure10 robotic arm pass through solides  and the floor can anyone help me please to fix it and thank you every one

##### Bitbots\_Jasper [Moderator] 05/01/2021 20:32:44
Your solids probably need a bounding object

##### Maurice Faraj 05/01/2021 20:44:20
Ok thank you

##### dA$H 05/01/2021 20:53:16
Does anyone know why the robot does not go exactly to the ball?
> **Attachment**: [empty\_2.mp4](https://cdn.discordapp.com/attachments/565154703139405824/838156143490236476/empty_2.mp4)


I wrote the controller according to a example in Webot (irb inverse\_kinematic)

##### Simon Steinmann [Moderator] 05/01/2021 21:44:59
self collision probably. Also, ikpy is not very good. Slow and unstable. I'd recommend ikfast

##### dA$H 05/01/2021 21:46:35
Is there an example of ikfast in webot?

##### Simon Steinmann [Moderator] 05/01/2021 21:47:17
you can compile it yourself


[https://github.com/cyberbotics/pyikfast](https://github.com/cyberbotics/pyikfast)


are you using the ur10e?

##### dA$H 05/01/2021 21:48:00
yes

##### Simon Steinmann [Moderator] 05/01/2021 21:52:30
I don't have the solver on this machine


hold on


what os are you on?

##### dA$H 05/01/2021 21:54:23
windows 10

##### Simon Steinmann [Moderator] 05/01/2021 21:54:58
ur10e needs lapack installed


I tried doing so without success on win10


Try to install this with "pip install ."
> **Attachment**: [pyikfast\_ur10e.zip](https://cdn.discordapp.com/attachments/565154703139405824/838171882162880512/pyikfast_ur10e.zip)


but it will most likely fail. You need the lapack library. Maybe you manage to get it to compile


let me know if you do

##### dA$H 05/01/2021 22:01:49
Gave an error


Why ikpy works well with irb but has problems with ur10e?

##### Simon Steinmann [Moderator] 05/01/2021 22:04:28
ur10e is more complex and requires the linear algebra library "lapack"


on Linux I can tell you how to solve it. WIndows I was not able to do it. However it may have been due to an outdated version. Just recently updated to version 20H2


[https://www.google.com/search?client=firefox-b-d&q=lapack+windows+10](https://www.google.com/search?client=firefox-b-d&q=lapack+windows+10)

##### tabousoud 05/02/2021 13:55:28
Hello everyone, I'm trying to model a gripper similar to this one in the pic and can't figure out how to use `SolidReference` to close the loop. 



Using the right  linkage as an example, I first attempted:

body -> joint j1 -> endpoint L1 -> j2 -> L2 -> ... -> j5 -> endpoint = SolidReference(body)

but this didn't seem to work. 

I tried a different approach with L3 as the SolidReference node closing the inner (L4) and outer links (L1, L2)  where L3 is an endpoint for j3 and a SolidReference endpoint for j4.



Can someone please advise the way to define this linkage? Thanks!
%figure
![gripper_linkage.png](https://cdn.discordapp.com/attachments/565154703139405824/838413387477286912/gripper_linkage.png)
%end

##### DDaniel [Cyberbotics] 05/02/2021 13:56:33
`@tabousoud` search for the khepera3 gripper sample, it's very similar and uses solid reference

##### tabousoud 05/02/2021 13:58:09
Thanks `@DDaniel` , I'll take a look at it

##### DDaniel [Cyberbotics] 05/02/2021 13:59:53
also don't forget to enable physics in all intermediary endpoints otherwise it won't work

##### tabousoud 05/02/2021 14:01:04
Got it, thanks again!

##### DDaniel [Cyberbotics] 05/02/2021 14:11:30
`@tabousoud` or you can check this one instead, it's basically like the khepera one but has much less stuff so might be easier to understand
> **Attachment**: [gripper.zip](https://cdn.discordapp.com/attachments/565154703139405824/838417421290831902/gripper.zip)

##### tabousoud 05/02/2021 14:22:36
Yes, this looks more straightforward. Many thanks!

##### Simon Steinmann [Moderator] 05/02/2021 19:28:36
[https://github.com/cyberbotics/community-projects/tree/master/devices/robotiq/protos](https://github.com/cyberbotics/community-projects/tree/master/devices/robotiq/protos) `@tabousoud` try this one I did

##### tabousoud 05/02/2021 20:03:38
Thanks for sharing! Yes, I actually downloaded both the `Robotiq2f.proto` and `Robotiq85Gripper.proto` to see if I could use those. I noticed that those models used rotational motors for every joint, so was trying to see if I could build a model with a single actuator motor and utilize the linkages to model the grip. So far, it's not going very well...
> **Attachment**: [stacking\_env\_DEV01.mp4](https://cdn.discordapp.com/attachments/565154703139405824/838506037626667048/stacking_env_DEV01.mp4)

##### Simon Steinmann [Moderator] 05/02/2021 20:13:07
I think it is easier to "fake" it. I have done so sucessfully in the past. Also had a controller for it, but can't find it


But please post, if you manage to do it successfully

##### tabousoud 05/02/2021 20:18:00
For sure. If I get it to work I'll share what I did. Thanks!

##### Simon Steinmann [Moderator] 05/02/2021 20:27:35
`    def set\_robotiq\_2f\_gripper\_pos(self, gripperPos):

        pos = self.gripper\_minPos[0] + gripperPos * (self.gripper\_maxPos[0]-self.gripper\_minPos[0])

        self.gripper\_motors[0].setPosition(pos)     

        self.gripper\_motors[1].setPosition(-pos)

        self.gripper\_motors[2].setPosition(pos)

        self.gripper\_motors[3].setPosition(pos)

        self.gripper\_motors[4].setPosition(-pos)

        self.gripper\_motors[5].setPosition(pos)`


I found the function. This controls the gripper nicely doing it the "fake" way


I'm sure you can initialize the class variables yourself, they should be self explanatory

##### tabousoud 05/02/2021 20:32:10
Perfect, this is very helpful `@Simon Steinmann` . Thanks again!

##### Stefania Pedrazzi [Cyberbotics] 05/03/2021 06:07:56
The simulation server needs to create an isolated folder in `/tmp/webots/instances` where to store the projects file requested by the client each time a client connects and asks to start a simulation.

You should first make sure that this folder can be created and accessed by `simulation_server.sh` script.

##### Milacuk 05/03/2021 06:49:03
hello,maybe somebody can help me with this problem,i am using makefile which is included into webots
%figure
![2021-05-03_09.45.16.png](https://cdn.discordapp.com/attachments/565154703139405824/838668463022276668/2021-05-03_09.45.16.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 05/03/2021 06:53:18
Here there is some confusion. The robot controller file `distance.c` should contain a C code to control the robot. But your file contains a world definition that should be saved in a `.wbt` file and cannot be compiled.

I would strongly suggest you to read the tutorials to understand how Webots works:

[https://www.cyberbotics.com/doc/guide/tutorials](https://www.cyberbotics.com/doc/guide/tutorials)

##### Johan2021 05/03/2021 10:46:41
Hi everyone, I am modeling a robot that is locked to a surface (alternating between its 2 feet) when in motion. To lock the feet, I exert a force onto the foot body perpendicular to the bottom surface of the foot, but this yields some weird behavior (going into a rigid body) during simulation -- anyone from Webots who could help me on this?
> **Attachment**: [force\_approach\_bad.mp4](https://cdn.discordapp.com/attachments/565154703139405824/838728264637218887/force_approach_bad.mp4)

##### Stormtrooper 05/03/2021 12:51:54
Hello everyone, i have a quick question...how can i tell how often my robot is printing data onto the console? im aware TIME\_STEP is used to get data from a sensor after that specified period of time, but is it also the time taken for the software to go through the main's while loop once?

##### Gustavo Cruz 05/03/2021 12:52:37
The simulation server is creating the folder in /tmp /webots /instances, but it is not creating any files in it when I try to start a simulation. I have already tried to change some permissions on the folder, but the problem continues.
%figure
![Instances.PNG](https://cdn.discordapp.com/attachments/565154703139405824/838759957679308830/Instances.PNG)
%end

##### Stefania Pedrazzi [Cyberbotics] 05/03/2021 13:38:17
Did you check in the logs (default path: `$WEBOTS_HOME/resources/web/server/log/simulation/` and `$WEBOTS_HOME/resources/web/server/log/session/`) if other errors are printed?

If not, I would suggest you to debug the `simulation_server.py` script and in particular the `setup_project` function to better understand why the project files are not copied.

##### Yannnick3 05/03/2021 19:33:55
Hello everyone ! I'm implementing a VIO module in Webots using ROS. The module has a camera, an accelerometer and a gyroscope. I enable them using the corresponding ROS services. Once activated, each sensor publishes its values on its topic (example: /Module/camera/image for the camera). My problem is the following: I can enable them with any sampling period, the different sensors always publish their value on the topics with a period corresponding to the basicTimeStep of the Webots world. For example, for my camera to publish images at a frequency of 40hz, I enable it with a period of 25ms. Despite this, the camera topic receives an image every 5ms, which is the period of my Webots world. Is there a way to obtain the correct publishing frequency on the topics? Thanks in advance ! 🙂

##### Gustavo Cruz 05/03/2021 20:06:13
I checked the simulation server logs and found that the problem was with the setup\_project\_from\_github function.

The function was checking for the presence of the word "tag" or "branch" in the project link on github, but it was not checking the word "blob".

Thanks for helping me.
%figure
![TrechoProblema.PNG](https://cdn.discordapp.com/attachments/565154703139405824/838869079141384312/TrechoProblema.PNG)
%end


Unfortunately, even correcting the problem, the simulation is not yet started and the client keeps displaying the message "Starting simulation ...".



No error is shown and this is the siling server log at the moment:
%figure
![NovoLog.PNG](https://cdn.discordapp.com/attachments/565154703139405824/838869287291846656/NovoLog.PNG)
%end

##### Boichu 05/04/2021 00:00:23
Hello, I have a problem, I created a test 6-wheeled robot, and wanted to check if the engines are powerful enough for different types of terrains, I've added contact properties and changed Coulomb friction for all of the different surfaces, but the speed of the robots on each of the surface is same, what did i do wrong? 😦
> **Attachment**: [problem.mp4](https://cdn.discordapp.com/attachments/565154703139405824/838928008391426068/problem.mp4)

##### Simon Steinmann [Moderator] 05/04/2021 01:37:13
`@Boichu` That means that there is most likely no slippage. Set the friction to 0 for one of them as a sanity check


wait.. I think I misunderstood your question. Why would they move at different speeds in your opinion?

##### Boichu 05/04/2021 01:48:44
Shouldn't there be some resistance to motion on different friction surfaces? I might be wrong though please don't judge too hard.

I did check on 0, no movement, so it's fine

##### Simon Steinmann [Moderator] 05/04/2021 01:49:33
You have wheels, as long as they dont slip, there is almost no resistance


You would be right if you were to drag a block across those surfaces, but you are rolling them


There is rolling friction, but i'm not sure it is simulated here, and it very small compared to other sources of friction or drag


Just think of rolling a rubber toy wheel across sandpaper. Rolls without any issues right? Now try to drag it across without spinning. Almost impossible

##### Boichu 05/04/2021 02:02:59
I guess you are right, was thinking about slipping like car on ice, or slowing down on sand, but i'm not sure if it should appear here, thank you for your help

##### Simon Steinmann [Moderator] 05/04/2021 02:05:47
the slipping on ice can be simulated. Sand is more tricky, as it is not a solid surface

##### Boichu 05/04/2021 02:09:59
I will try tommorow to create a ramp 😄 I think it can be created from box + rotation right? since i didn't see triangle shape or anything like that

##### Darko Lukić [Cyberbotics] 05/04/2021 06:44:47
The period should be a multiple of the basic timestep:

[https://github.com/cyberbotics/webots/blob/dde08ba5aa324cf2d9d8efdfc53f0363d00cf47e/projects/default/controllers/ros/RosSensor.cpp#L85](https://github.com/cyberbotics/webots/blob/dde08ba5aa324cf2d9d8efdfc53f0363d00cf47e/projects/default/controllers/ros/RosSensor.cpp#L85)

##### Eduardo Tavares 05/04/2021 07:47:09
Hi guys, I'm a newbie and wanted to develop a DH table simulation of a simple puma 560 style robot can you help me?

##### Yannnick3 05/04/2021 08:11:59
Thanks for your answer. In my case the sampling period is 25ms, while the basic timestep is 5ms. But the sensors still sample values every 5ms (topic frequency is 200hz instead of 40hz). The get\_sampling\_time service returns the correct sampling period though. Am I missing something here ?

##### Darko Lukić [Cyberbotics] 05/04/2021 08:19:39
It seems we are not increasing the `mStep` here:

[https://github.com/cyberbotics/webots/blob/73fe51ccc30a82fede130c67a8d983eec96589a1/projects/default/controllers/ros/Ros.cpp#L491-L492](https://github.com/cyberbotics/webots/blob/73fe51ccc30a82fede130c67a8d983eec96589a1/projects/default/controllers/ros/Ros.cpp#L491-L492)



Is it possible for you to debug it?

[https://github.com/cyberbotics/webots/wiki/Linux-installation](https://github.com/cyberbotics/webots/wiki/Linux-installation)

##### Yannnick3 05/04/2021 11:54:17
Yes that's the source of the problem ! I added the mStep incrementation. I also needed to replace mStepSize by mRobot->getBasicTimeStep() in the following line, because mStepSize is always equal to 1 and not to the actual duration of a step: [https://github.com/cyberbotics/webots/blob/73fe51ccc30a82fede130c67a8d983eec96589a1/projects/default/controllers/ros/Ros.cpp#L483](https://github.com/cyberbotics/webots/blob/73fe51ccc30a82fede130c67a8d983eec96589a1/projects/default/controllers/ros/Ros.cpp#L483)



Thanks a lot for your help, it works perfectly now !

##### Nexos 05/04/2021 12:25:34
Hello! Is it possible to retrieve the global coordinates (wrt the origin of the world) of an object detected through a robot's Camera? I have used the CameraRecognitionObject.get\_position() function and then followed the instructions on the documentation (calculating p' = R * p + T) but the result is way off.

##### ~E 05/04/2021 13:14:46
Does anyone know of a good, in-depth place to learn about PID controllers? I want to be able to design on from scratch in matlab

##### Simon Steinmann [Moderator] 05/04/2021 18:45:09
[https://www.youtube.com/watch?v=wkfEZmsQqiA](https://www.youtube.com/watch?v=wkfEZmsQqiA)

##### Beginner26798 05/04/2021 21:59:44
Good evening what application can we use to do a 3D model and import it to webots

##### tabousoud 05/04/2021 22:07:00
Webots supports a lot of 3D formats, here is a list of what it can import. There are many CAD and modeling programs that can export these file formats.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/839261861785763910/unknown.png)
%end

##### Deleted User 05/05/2021 04:56:58
Hello, does anybody knows how to change the icon size of Webots UI?

##### Stefania Pedrazzi [Cyberbotics] 05/05/2021 06:08:43
Next step to debug is try to run the exact same instruction to start Webots from a terminal and check if the simulation is started.

##### Darko Lukić [Cyberbotics] 05/05/2021 06:32:05
It's great you found a solution. Can you share the implementation in a GitHub pull request?

##### Olivier Michel [Cyberbotics] 05/05/2021 07:41:25
I assume you are on a retina display, right? Then, setting an environment variable like `QT_ENABLE_HIGHDPI_SCALING=1` should help. See [https://github.com/cyberbotics/webots/pull/2631](https://github.com/cyberbotics/webots/pull/2631). Otherwise, you may try to edit `WEBOTS_HOME/resources/stylesheet.*.qss` with a text editor and see whether you can change icon sizes in there.

##### Max\_K 05/05/2021 12:56:22
Hello, does anybody know if artags or apriltag are available in webots?

##### tabousoud 05/05/2021 13:03:09
Hello everyone! I'm trying to figure out how to implement a robot extension slot within a PROTO. 



From what I've seen looking at the existing protos and some of the examples, the way to do this is to add the `MFFloat` field instantiated with an empty array and reference this at the associated node (e.g. endpoint -> solid -> transform -> `children IS extensionSlot`).  I modeled a manipulator and gripper, and used this approach to specify an expansion slot on the manipulator robot. The associated protos for both are in the same project directory and all devices (motors, sensors, etc.) on the robot and gripper have unique names. When using the models, I can add the gripper to the slot as expected. However, I get a "device not found" error when running the controller and referencing a device from the gripper (e.g. `robot.getDevice("left_finger_actuator")`). 



This doesn't seem to be an issue when working with existing robots and devices in Webots. My guess is I need to add a path to the child proto but have no clue where/how to do this. Any pointers? Thanks!

##### Darko Lukić [Cyberbotics] 05/05/2021 13:12:38
The name of your motor (inside the PROTO) is `left_finger_actuator`?

##### tabousoud 05/05/2021 13:15:08
Yes, that is the name in the PROTO (in this example, at least).

##### Darko Lukić [Cyberbotics] 05/05/2021 13:19:40
If you `(Right click on the PROTO) > Convert to Base Node` you should be able to see a generated robot structure inside the scene tree. Try to run `robot.getDevice("left_finger_actuator")` then. If that fails, try to find the `left_finger_actuator` node in the scene tree.

##### tabousoud 05/05/2021 13:23:27
Cool, I'll give that a shot. Just to clarify: Is it the child (i.e. gripper) PROTO that I should convert?

##### Gustavo Cruz 05/05/2021 13:40:41
I executed the instruction on a terminal. The Webots program started but then closed quickly. And then a message appeared at the terminal. I think the problem is because he is trying to open the file that is in the path '/tmp/webots/instances/140192500708640/worlds/empty.wbt' but the right path is' / tmp / webots / instances / 140192500708640 / WebotsWorld / worlds /empty.wbt '
%figure
![OpenFile.PNG](https://cdn.discordapp.com/attachments/565154703139405824/839496830605066300/OpenFile.PNG)
%end

##### tabousoud 05/05/2021 14:26:11
`@Darko Lukić` Thanks, I have it working now. The problem is that I tried to convert the Robot node I made for the gripper into a Solid node by manually editing its PROTO and removing some fields. I noticed when using `Convert to Base Node` that it still shows up as a `Robot` node. I reversed the process by converting the Robot node into a solid (from the scene tree) first and then adding back the actuators - which get lost when converting Robot to Solid - manually in the PROTO. This fixed the issue.

##### milacuk 05/05/2021 14:31:01
hello friends,can somebody explain me why i could't buid and simulate  a project
%figure
![2021-05-05_15.21.04.png](https://cdn.discordapp.com/attachments/565154703139405824/839509497348620318/2021-05-05_15.21.04.png)
%end

##### DDaniel [Cyberbotics] 05/05/2021 14:38:15
`@milacuk` Hi, what error do you get?

##### Yannnick3 05/05/2021 14:45:43
Yes of course ! I made the pull request just now.

##### milacuk 05/05/2021 14:49:32
Actually ,i cant build it
%figure
![2021-05-05_17.48.29.png](https://cdn.discordapp.com/attachments/565154703139405824/839514158197637130/2021-05-05_17.48.29.png)
%end

##### DDaniel [Cyberbotics] 05/05/2021 14:51:02
`@milacuk` you're using a python controller, you don't need to build it, only to save it

##### milacuk 05/05/2021 14:55:59
ok, but when i run the simulation nothing happens

##### DDaniel [Cyberbotics] 05/05/2021 14:57:26
because you're not giving any instructions to the robot, the controller you're showing is an empty loop


`@milacuk` replace it with this and it will do stuff: [https://cyberbotics.com/doc/guide/tutorial-4-more-about-controllers?tab-language=python#the-controller-code](https://cyberbotics.com/doc/guide/tutorial-4-more-about-controllers?tab-language=python#the-controller-code)

##### milacuk 05/05/2021 15:01:00
ok, i will try tnx a lot

##### Gustavo Cruz 05/05/2021 17:32:24
I executed the instruction in the terminal and the simulation started in Webots, but in the browser it still continues "Starting simulation..."
%figure
![Open.PNG](https://cdn.discordapp.com/attachments/565154703139405824/839555143850524693/Open.PNG)
%end

##### main 05/05/2021 17:56:42
Is the max torque on webots the Stall Torque?

##### Mohannad Kassar 05/05/2021 19:10:49
Hello Every one, please any one who tried to import a VRML file to webots and worked reply to me.


I builded a model using solidwork and exported the VRML file and when importing it it gives many erros of this type: ERROR: 'C:/Users/ASUS/Desktop/finall/bag holder.wrl':2:1: error: Skipped unknown 'Separator' node or PROTO.


and I tried to import only one part of the simulation, for the fact this error may be from the complexity of the model, and even this part didn't work and gave the same error


any solution ?

##### Simon Steinmann [Moderator] 05/05/2021 22:24:15
`@Mohannad Kassar` As the error says, the file has Separator nodes, which do not exist in Webots.


`@main` It is the maximum torque the motor can utilize. It is not dependent on rotational speed

##### Stefania Pedrazzi [Cyberbotics] 05/06/2021 06:04:55
About the browser, if you start the simulation manually then, you should pay attention because on the browser the use simulation server port may not match.

But if the simulation starts from a terminal then it should also start directly from the script (if you are using the exact same instructions). You should debug what is the difference and why the simulation server doesn't work.

##### Majanao 05/06/2021 10:31:49
Hi everyone, I'm trying to control the motors of a Robotique2F gripper attached to a Kinova Gen3 through a tool slot with 'getDevice('xxx')'. It finds all the motors of the Gen3 but none of the gripper. Can you tell me what I'm doing wrong?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/839811690233462824/unknown.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 05/06/2021 10:47:14
Note that each `Robot` node is an independent system with each own robot controller and can only access its own devices. So you cannot access devices from a descendant robot from the parent robot controller.

in practice, if the `Robotiq2f` PROTO is of type `Robot`, then you should add a dedicated controller in the `Robotiq2f.controller` field to control its devices.

Alternatively, you should replace the `Robot` node with a `Solid` node in the `Robotiq2f` PROTO definition

##### bingdong 05/06/2021 10:55:43
Hi, how can I add an LCD screen on my robot in simulation?

##### DDaniel [Cyberbotics] 05/06/2021 11:01:53
`@bingdong` hi, you can add a display: [https://cyberbotics.com/doc/reference/display](https://cyberbotics.com/doc/reference/display)

##### Gustavo Cruz 05/06/2021 12:30:35
I did some tests here and found that the problem is in that part of the image code. The simulation server is never receiving the "open" message from the webots. The condition "if line.startswith ('open')" is never true and is always in the loop.
%figure
![Open.PNG](https://cdn.discordapp.com/attachments/565154703139405824/839841575865155624/Open.PNG)
%end

##### Stefania Pedrazzi [Cyberbotics] 05/06/2021 12:32:05
Which Webots version are you using?

I just tested on R2021a and everything works correctly.


You should probably also make sure that the Webots version you are using is the same as the session and simulation server.

##### Gustavo Cruz 05/06/2021 12:41:26
I am using a version R2021a-x86-64.

##### Stefania Pedrazzi [Cyberbotics] 05/06/2021 12:45:51
Note that the "open" message is only sent if Webots is started with the streaming `monitorActivity` option.


The original `simulation_server.py` code already enables this option by default.

##### Gustavo Cruz 05/06/2021 13:00:39
Webots are being started with the streaming monitorActivity option here.

I think I will try to reinstall Webots or install on another version of Ubuntu.



Thanks for helping me. I'm sorry for the inconvenience.

##### Seedow 05/06/2021 13:06:27
Hi! I'm using Webots R2021a revision 1 Nightly 19/3/2021 and Matlab R2020a. If I want to run a controller in extern mode, at first time it works well, but after I stop it or it crashes, after the second start (after resetting the simulation) Matlab crashes completely. Has anyone run into this issue? Should I upgrade my Matlab to a newer version?

##### Majanao 05/06/2021 14:13:41
Thanks a lot `@Stefania Pedrazzi`! I set it to Solid and it work just fine.

##### bingdong 05/06/2021 14:21:39
@<@787796043987025941> Thanks a lot!

##### alejanpa17 05/06/2021 16:11:30
Hello everyone, Im trying to get the absolute position of an object given the relative position from the camera recognition any clue of the formulas i have to apply?

##### bingdong 05/06/2021 16:12:09
<@787796043987025941> I actually wanted to have the display as a 2D texture on a shape inside of an overlay. The functions described worked for the overlay but I'm not sure how to use them for texture.  

Display>children>Shape>Appearance>baseColorMap>ImageTexture>url 

Is there a display function for making changes to the url or will I have to use Supervisor?

##### Simon Steinmann [Moderator] 05/06/2021 18:52:00
`@alejanpa17` [https://cyberbotics.com/doc/reference/supervisor?tab-language=python#wb\_supervisor\_node\_get\_position](https://cyberbotics.com/doc/reference/supervisor?tab-language=python#wb_supervisor_node_get_position) take a look at the python example


If you are working in python, I can give you a easy to implement module

##### alejanpa17 05/06/2021 18:56:18
Awesome, i get the idea thank you!

##### jasonsteakham4 05/06/2021 22:22:23
hey, could anyone detail how to go about saving a camera image through python? I can't seem to make sense of the code in the doccumentation

##### Simon Steinmann [Moderator] 05/06/2021 22:54:15
`@jasonsteakham4` you want to save it as an image file?


if you use opencv (cv2), then you can use this: [https://www.geeksforgeeks.org/python-opencv-cv2-imwrite-method/](https://www.geeksforgeeks.org/python-opencv-cv2-imwrite-method/)

##### jasonsteakham4 05/06/2021 22:55:29
Yes ideally

##### Simon Steinmann [Moderator] 05/06/2021 22:55:31
otherwise look for general python documentation.

##### jasonsteakham4 05/06/2021 22:55:39
Thanks!

##### ~E 05/08/2021 00:37:56

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/840387007431966740/unknown.png)
%end


does anyone know how to fix these weird wireframe things?

##### DDaniel [Cyberbotics] 05/08/2021 00:39:01
`@~E` Hi, View > Plain rendering

##### chika 05/08/2021 08:46:31
Hi. I am using webots 2021 and I'm trying to get the color of a color patch on ground using webots camera node. I program with C++.

I came across 2 problems. 

1. When I reload the world and simulate for the first time, it take some time for the camera to start capturing (about 2 seconds) and by that time the color patch is already passed. But when I reset the world (without reloading) and simulate again, camera start capturing right from the beginning (just the way I want it to). 

How can I solve this issue?

2. When the camera gets disabled through code after serving it's purpose, the camera keep disabled even in next simulations (even after I reload the world the camera is still disabled). Why?

##### aslisevil 05/08/2021 12:07:09
hello, I have a question. I use the supervisor rotate function in my simulation. But when I do rotation, my robot starts to walk with a curve instead of walking in a straight line. And sometimes it starts shifting out of nowhere after rotation. Do you know why this might be happening?

##### Beginner26798 05/08/2021 12:36:53
Hi guys ... i need please anything that helps me to make a robotic arm i don't want to use the samples in webots

##### Donny\_234 05/08/2021 14:48:56
Hello I am a beginner in Webots. I wanted to ask whether it is possible to control the motion of a robotic arm using hand gestures as live input from the computer's webcam?

##### pnaraltnsk 05/08/2021 17:45:27
Hello, is there any way of changing the angular velocity magnitude of nao robot?

##### Simon Steinmann [Moderator] 05/08/2021 23:03:21
It is possible, but sounds very complicated. You would need some form of computervision algorithms recognizing and translating the video input into movement commands. That is not included in Webots


Dont use the supervisor for controlling a robot. It "cheats" or "teleports", which screws up the physics. You should only control your robot with motor commands


A custom arm or an existing one? For existing ones you can convert urdf into Webot's proto format


otherwise I suggest you add an existing arm to your webots world, right-click > convert to base nodes


then take a look at the structure and create your own arm accordingly

##### main 05/09/2021 03:46:11
Any appropriate way to add delays in your script?


(to have a delay between code continuously executing)

##### Simon Steinmann [Moderator] 05/09/2021 03:49:08
`@main` just add waiting loops or pauses. You can either use simulation time or computer time

##### main 05/09/2021 03:49:31
As in the python sleep function?

##### Simon Steinmann [Moderator] 05/09/2021 03:49:42
yes


or you do a check on what simulation time it is


and only execute your code when enough sim time has elapsed

##### main 05/09/2021 03:51:03
The sleep function would be better for the scenario, however I always have trouble defining it?

##### Simon Steinmann [Moderator] 05/09/2021 03:51:47
what exactly do you want to accomplish?

##### main 05/09/2021 03:53:27
Ive got distance sensors on my offroad truck , and im trying to make it so the distance isnt being repeatedly checked


so theres a bit of timing in between distance sensor measurments

##### Simon Steinmann [Moderator] 05/09/2021 03:54:31
enable the sensor with a different frequency then


(should be a multiple of basicTimeStep

##### main 05/09/2021 03:55:00
Thats where ive been going wrong with the multiple of time step, thanks


Its in milliseconds right? so 2048 is around 2s

##### Simon Steinmann [Moderator] 05/09/2021 03:57:14
yes

##### SeanLuTW 05/09/2021 17:25:13
I made some customized revised of `UR5e.proto` and `Kinect.proto` and put them in the `protos` in my project directory. When I try to import them, I found that only `UR5e (Robot)` under `PROTO nodes (Current Project)`  and couldn't find Kinect. Does anyone know why?

##### aslisevil 05/09/2021 18:25:49
hello, I was wondering if it's possible to disable distance sensor for one specific object only.

##### mmoralesp 05/09/2021 19:33:34
Hello Everyone!,  I am working on a project for my university on ROS and Webots, I am trying to control two epuck in the same Webots environment with ROS but I am not able to make them work, as if I can do it with only one epuck. I would like to know if anyone has tried to do this and if you could help me thanks.

##### Simon Steinmann [Moderator] 05/09/2021 20:39:26
`@mmoralesp` you need one controller per robot, but they can be "dumb" controllers, simply subscribing and publishing topics. All your logic can be done from the same script / node


The name of the proto file and the name of the proto node inside the file have to be exactly the same.  Post the file and I can take a look

##### SeanLuTW 05/10/2021 00:54:51
If I named the file which doesn't match with the name of the PROTO node, the console will show `'FILE_TO_PROTO': error: PROTO identifier does not match filename`. But if I fixed it, it still not shown.

##### Simon Steinmann [Moderator] 05/10/2021 00:55:15
post the file here, i'll take a look


it is in the protos folder of your project?

##### SeanLuTW 05/10/2021 00:55:38
Here's my PROTO
> **Attachment**: [Kinect.proto](https://cdn.discordapp.com/attachments/565154703139405824/841116239266971689/Kinect.proto)


Yes, I put them into the protos subdirectory
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/841118431172624424/unknown.png)
%end

##### Simon Steinmann [Moderator] 05/10/2021 01:04:37
not showing up for me either, i'm investigating


`@SeanLuTW` figured it out. It only shows up when you add it as a child node of a robot


or a handslot etc of a robot


only if "devices" show up under Webots Projects, can your device show up as well
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/841120213969010688/unknown.png)
%end


It contains sensors, any device needs a robot as a parent

##### SeanLuTW 05/10/2021 01:13:25
Yes, it show up when I add it as a child of a robot


Thanks! I forgot it is a `Solid` instead of a `Robot`

##### Simon Steinmann [Moderator] 05/10/2021 01:15:03
if you want to simply place a kinect "in the room" you have to add a base Robot node and add it as a child


it does not need any other solids or shapes

##### Luiz Felipe 05/10/2021 09:38:39
Hello, is it possible to stream a simulation to the browser in 'run' mode? (Running as fast as possible but also able to see the graphics) It seems I can only run in real time or fast mode without graphics during training.

##### h.sciascia 05/10/2021 09:54:17
Hello everyone, 

Is that possible to do a check collision with a Physics Plugin if the robot and environment are not moving ?


I can only detect collision when colliding, not when my robot and environment are already in collision

##### Stefania Pedrazzi [Cyberbotics] 05/10/2021 10:50:25
Which version of Webots are you using? Are you using X3D or MJPEG streaming?

I just checked and the run mode with the X3D streaming works correctly on Webots R2021a after enabling the `webots.showRun = true;` option in the streaming viewer.

##### Luiz Felipe 05/10/2021 10:52:01
Oh... I am testing with Webots 2020b and MJPEG mode `@Stefania Pedrazzi`... Does it work only with the R2021a and X3D mode?

##### Stefania Pedrazzi [Cyberbotics] 05/10/2021 10:54:12
It works on R2020b too, but with the MJPEG mode you need to enable the graphics rendering in Webots.

If you disabled them in Webots, then the MJPEG streaming cannot work.


To speed up the physics computations, objects are "disabled" if they do not move or don't have new collisions. These "idle" objects are represented in Webots by changing the color of the bounding object from white to blue.

One thing you could try in your physics plugin is to check if the body is enabled using the function `bool dBodyIsEnabled(dBodyID)` and if not try to enabled it manually using  `void dBodeEnable(dBodyID)` and check again if you can detect the collisions.

You can find additional information in the ODE physics engine manual:

[http://ode.org/wiki/index.php?title=Manual#Collision\_handling](http://ode.org/wiki/index.php?title=Manual#Collision_handling)

##### Luiz Felipe 05/10/2021 11:07:00
Locally I can run the three modes (real-time, run, and fast) in 2020b. Yups... it seems the 3 types are supported by the webots class... when I run the 'real-time' mode with MJPEG it works fine with graphics in the stream... If I run the 'run' or 'fast' mode the simulation run as fast as possible but both of them without graphics in the stream...


If i get the graphics in 'real-time' it means the graphics rendering in Webots is enabled right?

##### Stefania Pedrazzi [Cyberbotics] 05/10/2021 11:09:16
Yes. The MJPEG won't work when running with the FAST mode, but should work running the simulation in RUN mode.

If it doesn't, then I would suggest you to upgrade to the latest Webots R2021a.

##### Luiz Felipe 05/10/2021 11:10:30
Okkz! I will test again... because the websocket call changes to R2021a right?

##### Stefania Pedrazzi [Cyberbotics] 05/10/2021 11:16:00
In R2021a, the "run" and "fast" mode have been changed in Webots: only the "fast" remains and the graphics rendering has been added as separate option. In the streaming communication nothing changed.

The main reason to upgrade is that we won't be able to fix bugs, if any, in the old Webots R2020b release.

##### Luiz Felipe 05/10/2021 11:19:42
ok!

##### h.sciascia 05/10/2021 11:33:17
Ok thanks i will try that

##### Iris230 05/10/2021 13:19:22
guys I'm wondering how can I use the vehicle libraries in pycharm?

##### Darko Lukić [Cyberbotics] 05/10/2021 13:19:56
[https://cyberbotics.com/doc/guide/using-your-ide#pycharm](https://cyberbotics.com/doc/guide/using-your-ide#pycharm)

##### Iris230 05/10/2021 13:23:36
thank you, I've seen this😂 but  what exactly value of path should I type?

##### Darko Lukić [Cyberbotics] 05/10/2021 13:25:32
Check this:

[https://cyberbotics.com/doc/guide/running-extern-robot-controllers#environment-variables](https://cyberbotics.com/doc/guide/running-extern-robot-controllers#environment-variables)

##### Iris230 05/10/2021 13:28:18
but there is no vehicle libraries🥲



%figure
![IMG_20210510_212543.jpg](https://cdn.discordapp.com/attachments/565154703139405824/841305700470882354/IMG_20210510_212543.jpg)
%end


here is the error🥲

##### Darko Lukić [Cyberbotics] 05/10/2021 13:31:10
That means that your `LD_LIBRARY_PATH` is not properly configured. What do you get when you do `echo $LD_LIBRARY_PATH`?


Oh, my bad, you are in Windows

##### Iris230 05/10/2021 13:34:01
yes,I'm in windows😂

##### Darko Lukić [Cyberbotics] 05/10/2021 13:34:08
What is your PATH?

##### Iris230 05/10/2021 13:35:45
sorry give me a second



%figure
![IMG_20210510_213810.jpg](https://cdn.discordapp.com/attachments/565154703139405824/841308208903356446/IMG_20210510_213810.jpg)
%end

##### Darko Lukić [Cyberbotics] 05/10/2021 13:40:37
Can you check whether those directories exist?

##### Iris230 05/10/2021 13:42:30
they do exist, cause I just copied and add them

##### Darko Lukić [Cyberbotics] 05/10/2021 13:48:40
Can you run the controller from a CLI using these instructions?

[https://cyberbotics.com/doc/guide/running-extern-robot-controllers#environment-variables](https://cyberbotics.com/doc/guide/running-extern-robot-controllers#environment-variables)

##### Iris230 05/10/2021 13:56:30
it seems like there is no instruction about running the controller from a  CLI?

##### Darko Lukić [Cyberbotics] 05/10/2021 13:57:48
You should be able to run `python3 /path/to/your_controller.py` once all environment variables are set

##### Iris230 05/10/2021 14:12:25
nothing happened… it still can't control my car



%figure
![IMG_20210510_221258.jpg](https://cdn.discordapp.com/attachments/565154703139405824/841316923597258802/IMG_20210510_221258.jpg)
%end


I couldn't see anything🥲

##### Darko Lukić [Cyberbotics] 05/10/2021 14:19:06
You use Python from Anaconda :/

[https://cyberbotics.com/doc/guide/using-python#use-an-alternative-python-version](https://cyberbotics.com/doc/guide/using-python#use-an-alternative-python-version)

##### Iris230 05/10/2021 14:23:57
oh thank you very much

##### Luiz Felipe 05/11/2021 00:54:21
Just as a feedback `@Stefania Pedrazzi` , with the R2021a it works fine... Thank you!

##### h.sciascia 05/11/2021 06:35:05
Hello everyone, 

I am doing a simulation with a personalized physics plugin.



My code is generating a lot of random configuration of a 6 dof arm robot and realize them. 

For each configuration I setPosition to the motors, and then erase the previous configuration with a new random one. 



But my simulation is freezing about 5-6 secs after i launch the process.



Have you any idea for the resolution of my problem ? 

Because even if I reset the physics plugin and erase all of the components of my environment to only keep the robot arm the simulation freeze...



Thanks in advance 😉

##### Darko Lukić [Cyberbotics] 05/11/2021 06:45:15
The Physics plugin is usually a critical part, you may have a blocking code there. Is it possible for you to test without the plugin?

##### yash 05/11/2021 14:20:55
Hi ! I want to know the radius of the wheels of iRobot’s create that’s available in WEBOTS.



I couldn’t find it ... please help me if there’s any way to find out.

##### harrymcc 05/11/2021 15:00:22
wb\_motor\_set\_position(left\_motor, 1);

 wb\_motor\_set\_position(right\_motor, 1); i have been going through the tutorials and this code according to it makes the robot come to a stop after 1 rotation of the wheel but it just keep going. Any clue why it does this. Even tried a straight copy and paste of the tutorial code and still nothing

##### Majanao 05/11/2021 16:58:15
Hi, is someone able to use the Blender import function. I always get the same error even with different Blender models.... WARNING: Invalid data, please verify mesh file (bone weights, normals, ...): BLEND: Expected at least one object with no parent

##### KajalGada 05/12/2021 00:08:25
From my understanding, any set position or set velocity command once given will continue to be in that state and not a one off.



If you want it stop after a rotation, you can add an if condition to check current position of wheel and once reached to 1, stop it.

##### Simon Steinmann [Moderator] 05/12/2021 02:02:36
`@harrymcc` `@KajalGada` wb\_motor\_set\_position(right\_motor, 1) gives the motor the command to reach the position 1. Once it reaches that position, it stops and holds it. The value is in radian (2*pi = 6.28...) is one rotation. So if your current motor position is 0 and you turn to 1, the robot will only roll a bit. However, if your current position is -100, it will roll quite a bit.


`@Majanao` what type of file did you try to import? Perhaps try to export it as an .obj file and import that


It sounds like you want to export a scene or special structure.

##### Darko Lukić [Cyberbotics] 05/12/2021 08:15:16
Hello `@cahyadiryan30`, I had remove your post as it contained images that make hard for users to debug your simulation. Please post the code to services like Gist ([https://gist.github.com/](https://gist.github.com/)) or use a proper tag ([https://support.discord.com/hc/en-us/articles/210298617-Markdown-Text-101-Chat-Formatting-Bold-Italic-Underline-](https://support.discord.com/hc/en-us/articles/210298617-Markdown-Text-101-Chat-Formatting-Bold-Italic-Underline-)).

##### KajalGada 05/12/2021 14:29:14
That makes sense.

##### harrymcc 05/12/2021 14:42:45
okay thanks for help turns out my bot must have been well in the minuses and just wasn't stopping

##### Max\_K 05/12/2021 16:17:18
Hello, i am trying to use a camera in Webots for the detection of apriltags in ros2 foxy. I tried with the 'Webcam for Robotino3' and the custom camera, but the /camera\_info topic in ros2 is empty, there is only one message at start. 

 Do i have to fill the camera\_info msgs myself or do i miss something?

##### Darko Lukić [Cyberbotics] 05/12/2021 16:18:27
Hello, the CameraInfo topic is latched, so you should verify QoS of your subscriber

##### Max\_K 05/12/2021 16:29:01
Okay, so its not possible to echo the camera\_info via cmd without QoS settings? I also do not see the image in rviz2 when subscribing to the /image\_raw, are the QoS setting also reason for that?

##### Darko Lukić [Cyberbotics] 05/12/2021 16:49:27
You can use echo, but:

[https://answers.ros.org/question/371521/ros2-foxy-qos-durability-transient\_local-rclpy-to-echo-robot\_description/](https://answers.ros.org/question/371521/ros2-foxy-qos-durability-transient_local-rclpy-to-echo-robot_description/)



For image in RViz2 yes, it may be the reason, but you can change QoS in RViz2.

##### Max\_K 05/12/2021 17:00:42
ok thanks! i m just getting one msgs when echo to the CameraInfo topic with QoS settings, but that helps.

##### Alejandro Pescathore 05/12/2021 22:18:53
Hello there! 

Is it possible to record a simulation with audio?  And if it is possible, how can I do it?

##### Simon Steinmann [Moderator] 05/12/2021 23:02:36
Do you have audio in the simulation?

##### Alejandro Pescathore 05/12/2021 23:03:26
Yes, i ise the speakers

##### Simon Steinmann [Moderator] 05/12/2021 23:04:13
I dont have experience with it, but if it is not implemented, that would be a good feature request

##### pewpewpew 05/13/2021 02:54:31
Are the left and right sensor values getting printed to the console in decimeters? Thanks in advance.
%figure
![Screenshot_1447.png](https://cdn.discordapp.com/attachments/565154703139405824/842233318715686932/Screenshot_1447.png)
%end

##### Simon Steinmann [Moderator] 05/13/2021 03:12:15
radians. They are motors, so it's angles in radians

##### pewpewpew 05/13/2021 04:37:47
it says that these sensors "allow the robot to know the distance traveled so far",  how would I convert two meters to radians?

##### DDaniel [Cyberbotics] 05/13/2021 08:40:52
For a full rotation of the motor (2pi radians)  the robot will advance roughly by a distance equal to the circumference of the wheel

##### pewpewpew 05/13/2021 08:57:19
thanks! I eventually figured that out, now I'm just trying to get the robot to follow the square path correctly

##### bingdong 05/13/2021 09:12:10
Hi, not sure how the motor.setPosition() function works. I initialised the rotational motor in scene tree and python script, then called motor.setPosition(PositionValue) in the main loop. From what I understand, the controller should calculate velocity according to set PID values to reach position. But the motor doesn't seem to be rotating.

##### DDaniel [Cyberbotics] 05/13/2021 09:15:09
`@bingdong` did you set motor.setVelocity(0) somewhere by any chance? In position control this command specifies the maximal velocity, if you did it won't move.

##### bingdong 05/13/2021 09:17:10
Ah okay. Thought it would change the setVelocity() value while running. Got it, thanks!

##### DDaniel [Cyberbotics] 05/13/2021 09:18:53
`@bingdong` no, only in velocity control it would do that, in position control it specifies the maximal velocity. Keep in mind that the motor API functions have different meanings depending on which type of control you're making (position control, velocity control, torque control). It's summarised in this table: [https://cyberbotics.com/doc/reference/motor#motor-control-summary](https://cyberbotics.com/doc/reference/motor#motor-control-summary)

##### bingdong 05/13/2021 09:27:12
Will go through it. Thank you.

##### raerae 05/13/2021 09:42:06
Hi guys, I'm new here. I just want to ask, is there a possible method to export Solidworks model to Webots that will include its original appearance?

##### alejanpa17 05/13/2021 18:40:10
Hi, im getting this error while compiling the racing\_wheel controller. Any clue whats happening?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/842471300152950804/unknown.png)
%end

##### Suke 05/13/2021 20:02:28
We are trying to figure out if we can implement a conveyor with teeth shown in the attached figure with the track node. We tried to use group that includes both track and box (teeth) in the bounding geometry. Is it possible or do we need to implement this as slider joints to mimic this behavior?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/842492011101683722/unknown.png)
%end


Go to "File>Save as" and select VRML (.wrl). Click option and select version "VRML 97". Still You may loose some apperance, but this is the best way so far for me.

##### Supernova 05/13/2021 20:37:46
Hey, can anyone tell me why do I have a message: "Connect to SUMO... This operation may take a few seconds." that well, never dissapears? My vehicle never connects to sumo and the simulation just stops the moment it starts, for some reason.

##### Simon Steinmann [Moderator] 05/13/2021 22:39:52
Can you give more information? What did you run, where did the error show?

##### Supernova 05/14/2021 05:57:48
Im running my own autonomus vehicle controller. Its tesla with a compass, gps, lidar and seven distance controllers. But most of the code is commented - i only wanted to test a tiny function i prepared for distance sensors, so the only think the car does is going forward and checking if anything is too close, and this happends. Its not even an error, just an information - nothing else gets printed out. So im just trying to ask If its my fault and i messed up the code, or is it just some form of a bug...?

##### Darko Lukić [Cyberbotics] 05/14/2021 06:30:46
You probably have a `SumoInterface` PROTO somewhere in your world. Deleting it should fix the error:

[https://cyberbotics.com/doc/automobile/sumo-interface](https://cyberbotics.com/doc/automobile/sumo-interface)


It seems you are missing the Qt library in your PATH

##### Dorteel 05/14/2021 11:49:08
Hi everyone! Can anybody give me an advice on how to align connectors? I'm trying to grasp, and lock when there is a presence of a connector, but the can just teleports to the side after locking. The connector in the robot is in the middle of the tool center point, and the connector in the can is in the middle as well. There is a distance tolerance of 0.02. Any help is greatly appreciated, have ran out of ideas...
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/842730251021058088/unknown.png)
%end

##### alejanpa17 05/14/2021 16:31:13
isn't that supposed to be included in webots when installing it? I didnt touch anything in that path

##### antehc 05/14/2021 20:10:23
Hi guys, I'm new here. I just want to ask, is there a way to have some fire or explotion effect in webots?

##### Simon Steinmann [Moderator] 05/14/2021 20:51:44
`@alejanpa17` [https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers#environment-variables](https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers#environment-variables) you have to set environmental variables


`@Dorteel` can you share a project so we can take a closer look?


Fire or explosions are not included. Perhaps you could try a point light, which you change the properties of with a script

##### mmoralesp 05/14/2021 21:35:20
Ok, but how i set in the launch file the controller or where i set this? Thanks!!

##### Simon Steinmann [Moderator] 05/14/2021 21:39:08
`@mmoralesp` just select the ros controller for each robot. It should automatically create topics and action servers / services to control the robot


all controllers get started automatically when the simulation starts

##### mmoralesp 05/14/2021 21:40:57
but the controller is external so where i set that

##### Simon Steinmann [Moderator] 05/14/2021 21:41:17
in webots


you can still start your external controller

##### mmoralesp 05/14/2021 21:45:01
I launch the world from the terminal with the ros command and then the ros controller to set the velocity


so i dont know how to launch this from webots

##### Simon Steinmann [Moderator] 05/14/2021 21:45:51
what command? Is it your own world?

##### mmoralesp 05/14/2021 21:47:11
ros2 launch webots\_ros2\_epuck robot\_launch.py (Robot\_launch.py is where is configured the world, executable...)

##### Simon Steinmann [Moderator] 05/14/2021 21:48:50
ohh you're using ros2. You did not mention that

##### mmoralesp 05/14/2021 21:49:01
sorry mate


yes im using ros2

##### Simon Steinmann [Moderator] 05/14/2021 21:49:10
not very experienced with ros2

##### mmoralesp 05/14/2021 21:49:44
if i can do this with ros, i dont mind to change


but i like to use the publish and subscription

##### alejanpa17 05/15/2021 11:49:25
Environmental variables were good. I fixed doing this: [https://github.com/cyberbotics/webots/issues/2717](https://github.com/cyberbotics/webots/issues/2717) I have the version nightly\_13\_5\_2021

##### dA$H 05/15/2021 14:33:05
hi.

can we use "input()" in  webot's console?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/843133895607975936/unknown.png)
%end

##### alejanpa17 05/15/2021 17:59:11
Hi, im trying to calibrate the engineFunctionCoefficients of a car node but I have no clue on how to do it, the rest of the configuration is on the image. I tried other values but the rpm never go higher than 900 and I dont know why
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/843185763084271656/unknown.png)
%end

##### abhicv 05/16/2021 04:51:52
Hi, how to visualize things like fluid being sprayed by a robotic arm on objects?

##### [BU] Heinrich Mellmann 05/16/2021 10:00:39
Hi everybody, I have a problem with using the `Pen`. it works perfectly on a floor with a single texture tile, but stop working as soon as there are more texture tiles. To reproduce the issue:



1. open `projects/samples/devices/worlds/pen.wbt`

2. start => pen works

3. change `RectangleArena->floorSize->x = 2` 

4. start => pen is double

5. change `RectangleArena->floorSize->y = 2`

4. start => pen doesn't work



`@DDaniel` I adjusted the test scenario: you have to change the `floorSize` to make sure there are several tiles. As long as there is only one tile everything works fine, but there seems to be a problem as soon as the floor consists of several tiles. Alternatively you can set `floorTileSize->x=0.5`, `floorTileSize->y=0.5`

##### Beginner26798 05/16/2021 10:18:58
Hi guys ... i'm using a regular wheel not a differential one so i need another sentence to get the encoder can you help please and give me another sentence than this in the photo below



%figure
![20210516_131647.jpg](https://cdn.discordapp.com/attachments/565154703139405824/843432378797260820/20210516_131647.jpg)
%end

##### DDaniel [Cyberbotics] 05/16/2021 10:54:26
`@[BU] Heinrich Mellmann` I can't reproduce the bug on 2021a, what version are you using?


`@Beginner26798` As explained here [https://www.cyberbotics.com/doc/reference/differentialwheels#differentialwheels](https://www.cyberbotics.com/doc/reference/differentialwheels#differentialwheels), you can replace the differential wheel with a hingejoint + rotational motor + position sensor. You need to use Motor API [https://www.cyberbotics.com/doc/reference/motor](https://www.cyberbotics.com/doc/reference/motor) and Position Sensor API [https://www.cyberbotics.com/doc/reference/positionsensor](https://www.cyberbotics.com/doc/reference/positionsensor) to interact with them


`@dA$H` Hi, haven't checked but depending on what you need to do there might be better ways to do it. If you want to control a robot using the keyboard you can use the Keyboard API [https://www.cyberbotics.com/doc/reference/keyboard](https://www.cyberbotics.com/doc/reference/keyboard), which is available by default for Robot nodes (useful for 1 stroke inputs typically, like steering). If you have multiple robots that all have the same controller but want them to behave differently you can do using using controller arguments [https://cyberbotics.com/doc/guide/controller-programming#using-controller-arguments](https://cyberbotics.com/doc/guide/controller-programming#using-controller-arguments). Lastly, you can use a supervisor to send out unique commands to each robot using emitters/receivers.

##### [BU] Heinrich Mellmann 05/16/2021 11:34:28
Thanks for the fast reply! I'm using the same version. You are right, I mistyped a variable name in my test case: for the bug to appear you have to change `floorSize` to make sure your floor texture has more than one tile. I also adjusted the test case description above.

##### DDaniel [Cyberbotics] 05/16/2021 12:16:54
`@[BU] Heinrich Mellmann` I see, I can reproduce it as well. It's due to the fact that the `RectangleArena` PROTO doesn't handle the mapping by itself when changes occur, so when you change the `floorSize` you also need to change the `floorTileSize` accordingly as reported here: [https://github.com/cyberbotics/webots/issues/1529#issuecomment-615062207](https://github.com/cyberbotics/webots/issues/1529#issuecomment-615062207), but I do agree that it can be misleading

##### Beginner26798 05/16/2021 12:23:29
I have used the position sensors it is giving me this error



%figure
![20210516_152222.jpg](https://cdn.discordapp.com/attachments/565154703139405824/843463786090594334/20210516_152222.jpg)
%end


<@787796043987025941>

##### DDaniel [Cyberbotics] 05/16/2021 12:27:05
`@Beginner26798` you only need to pass `ps0`, `WbDeviceTag` is the type. I suggest you do this tutorial before: [https://cyberbotics.com/doc/guide/tutorial-4-more-about-controllers](https://cyberbotics.com/doc/guide/tutorial-4-more-about-controllers)

##### Beginner26798 05/16/2021 14:33:04
I used this code to initialize the position sensors but it is giving me the same error can you please tell me what the specific code that i should add ... and thanks for helping <@787796043987025941>
%figure
![Screenshot_20210516-173046_Chrome.jpg](https://cdn.discordapp.com/attachments/565154703139405824/843496279316889611/Screenshot_20210516-173046_Chrome.jpg)
%end



%figure
![20210516_173313.jpg](https://cdn.discordapp.com/attachments/565154703139405824/843496357373280276/20210516_173313.jpg)
%end


What's wrong with this
%figure
![20210516_173918.jpg](https://cdn.discordapp.com/attachments/565154703139405824/843498094838284338/20210516_173918.jpg)
%end

##### DDaniel [Cyberbotics] 05/16/2021 15:18:06
`@Beginner26798` like I said you need to drop the `WbDeviceTag` and just call `wb_position_sensor_get_value(ps0)` as shown in the tutorial. Also, is `ps0` a global variable? Otherwise your function `compute_odometry` won't know what that is.

##### Daemongear 05/16/2021 17:21:43
Hey there. I'm trying to connect ROS with WeBots. I already have an Rviz launch file, the urdfs, protos, worlds setup, but I can't figure out how exactly I can connect RViz to the motor sensor feedbacks.


I figured that there should be a way to pass them to the joint\_state\_publisher, but I'm not sure how


as far as I have found, I still need to write a node to connect the two, where I simply take the results from the respective topics (whose names change each time) and plug them into their joint\_state.name and .position, then publish the results

##### Simon Steinmann [Moderator] 05/16/2021 20:50:55
your robot's controller has to get the sensor values and publish them

##### Beginner26798 05/16/2021 21:28:46
Any i dea on what can cause these errors ??
%figure
![20210517_002742.jpg](https://cdn.discordapp.com/attachments/565154703139405824/843600892355805204/20210517_002742.jpg)
%end

##### Simon Steinmann [Moderator] 05/16/2021 21:31:44
which errors are you referring to? The lower half are only warnings


and it's hard to say without seeing your code


can you upload your controller file here?

##### Beginner26798 05/16/2021 21:39:19

> **Attachment**: [final\_one.c](https://cdn.discordapp.com/attachments/565154703139405824/843603549739810856/final_one.c)


`@Simon Steinmann` this is the file

##### Simon Steinmann [Moderator] 05/16/2021 21:39:44
`@Daemongear` are you using ROS or ROS2? If using ROS, did you use the default ROS controller

##### Beginner26798 05/16/2021 21:40:29
I'm trying to make a obstacle avoider robot with color recognition

##### Simon Steinmann [Moderator] 05/16/2021 21:40:31
you get the errors when building?

##### Beginner26798 05/16/2021 21:40:39
Yes

##### Simon Steinmann [Moderator] 05/16/2021 21:40:52
and can you post the whole output, without any old console messages

##### Beginner26798 05/16/2021 21:44:11

%figure
![20210517_004335.jpg](https://cdn.discordapp.com/attachments/565154703139405824/843604772379951124/20210517_004335.jpg)
%end

##### Simon Steinmann [Moderator] 05/16/2021 21:44:28
Okay, so first of all, you have wrong variable types. Check the reference manual: [https://www.cyberbotics.com/doc/reference/positionsensor#wb\_position\_sensor\_get\_value](https://www.cyberbotics.com/doc/reference/positionsensor#wb_position_sensor_get_value)

get\_value is double, not int


`@Beginner26798` I suggest you orient yourself on an example controller such as this one
> **Attachment**: [e-puck.c](https://cdn.discordapp.com/attachments/565154703139405824/843605350808027146/e-puck.c)


change and expand it


understand its functionality

##### DDaniel [Cyberbotics] 05/16/2021 21:47:01
```
static double compute_odometry() 

{
    WbDeviceTag ps[2];
    char ps_names[2][10] = { "ps0", "ps1"};

       for (i = 0; i < 2; i++) {
        ps[i] = wb_robot_get_device(ps_names[i]);
        wb_distance_sensor_enable(ps[i], TIME_STEP);
}

    int l =  wb_position_sensor_get_value('ps0');
    
    int  r =  wb_position_sensor_get_value('ps1');
    
    
    // distance covered by left wheel in meter
    double dl = l / ENCODER_RESOLUTION * WHEEL_RADIUS;
    
    // distance covered by right wheel in meter
    double dr = r / ENCODER_RESOLUTION * WHEEL_RADIUS;
    
    // delta orientation
    double da = (dr - dl) / AXLE_LENGTH;

    X = X + (((dr + dl) / 2) * cos(da));
    Y = Y + (((dr + dl) / 2) * sin(da)); 
    
    // printf("X: %3.3f m | Y: %3.3f m | D: %3.3f\n", X, Y, da);

    // printf("estimated change of orientation: %g rad.\n", da);
    
    return da;
}
```

the brackets `{}` here make no sense, and you can't access the array `ps`, using `'ps0'`, but instead using `ps[0]`. Like I said before already, most of what you want to do is in the tutorial-4

##### Beginner26798 05/16/2021 21:47:46
I'm using this one actually and my robot is just going forward


Ok I'll see what I'll get sorry for bothering I'm new to webots

##### Simon Steinmann [Moderator] 05/16/2021 21:48:37
Tutorials are your friend :). Try to do them first and understand them

##### DDaniel [Cyberbotics] 05/16/2021 21:48:50
if you're not comfortable with c, you can use other languages like python or matlab

##### Beginner26798 05/16/2021 21:49:39
The issue that I'm low on time and I'm using webots to simulate my university project that's why I'm not taking my time in reading tutorials

##### Simon Steinmann [Moderator] 05/16/2021 21:50:14
Is c as a programming language demanded by the professor?


You'll save lots of time using Python. Much quicker and easier to learn than c.

##### Beginner26798 05/16/2021 21:51:19
No but i started with it and i don't have the time to restart using another language

##### Simon Steinmann [Moderator] 05/16/2021 21:51:41
believe me, you'll be faster with python

##### Beginner26798 05/16/2021 21:52:31
Actually i tried python first but i didn't now how to run it


Know*

##### Simon Steinmann [Moderator] 05/16/2021 21:52:39
no compiling, easier syntax

##### Beginner26798 05/16/2021 21:54:24
Is it possible that you can help me to do the code personally?

##### Simon Steinmann [Moderator] 05/16/2021 21:54:44
pm me

##### Daemongear 05/16/2021 21:55:52
hey `@Simon Steinmann` sorry for the delay, I didn't notice the ping. I'm using ROS, and currently I'm using the standard ROS controller, as provided in the tutorials. I've advanced a bit in my search, and if I'm not mistaken, I should be able to remap the same controller that I have for RViz to the webots data. (I am also using an auto generated package from a Fusion360-urdf exporter, which provides launch files for RViz, including a basic controller and such.)

##### Simon Steinmann [Moderator] 05/16/2021 21:58:11
`@Daemongear` when the simulation is running, do a` rostopic list` and see what is published. If i'm not mistaken, the standard ROS controller should automatically publish all sensors. You just have to select the appropiate topic in Rviiz

##### Daemongear 05/16/2021 23:08:36
yeah, but is rename enough to connect them to RViz if I model it right in  the launch file?

##### Simon Steinmann [Moderator] 05/16/2021 23:43:54
verify first that everything gets published


then just open rviz, make all the changes and save the config

##### Daemongear 05/17/2021 00:36:12
did that just fine, played around with the webots controller as well, I can get movement and feedback from the terminal via the rosservices


I do have to enable all the motor sensors first, then I can see them in the rostopic list, as expected


I have a launch file for RViz, with config and the proper project, but the launch file that I have declares a static joint\_state\_publisher source, or rather as I understand it, declares one but leaves it unconnected


so sorry to be responding so late, i have been in a meeting up until now

##### Simon Steinmann [Moderator] 05/17/2021 00:38:03
just launch rviz. once it is open, add all the visualization you want. Then save the config

##### Daemongear 05/17/2021 00:38:12
did that too

##### Simon Steinmann [Moderator] 05/17/2021 00:38:22
what is the issue then?

##### Daemongear 05/17/2021 00:38:32
having what I do in webots translate to rviz 🙂

##### Simon Steinmann [Moderator] 05/17/2021 00:38:46
what exactly do you want to visualiize?

##### Daemongear 05/17/2021 00:39:11
the robot's internal movements (relative to itself, not an environment)

##### Simon Steinmann [Moderator] 05/17/2021 00:39:25
so just the joints moving?

##### Daemongear 05/17/2021 00:39:29
the joints being replicated, yeah


Hello, so as Simon (many thanks btw) asked, I'm rephrasing my question, maybe someone can help with advice. I want to publish the WeBots joints to the ROS joint\_state\_publisher. I am using the standard, tutorial ROS controller

so far the only options I've found were A) rewrite parts of the standard controller (which is heavily distributed of course) to get the joints directly published to the right topic or B) write my own node (much simpler) to subscribe to the default topics after enabling the motor sensors, and publish them to the joint\_state\_publisher. this would of course induce some delay.

Any other ideas?

##### harrymcc 05/17/2021 11:41:49
in a tutorial i have seen the code robot.getTime() but would be an equivalent in c?

##### Stefania Pedrazzi [Cyberbotics] 05/17/2021 11:43:49
Yes, the C function is `wb_robot_get_time()`. All the API functions are documented in the Reference Manual:

[https://www.cyberbotics.com/doc/reference/robot#wb\_robot\_get\_time](https://www.cyberbotics.com/doc/reference/robot#wb_robot_get_time)

##### harrymcc 05/17/2021 11:45:04
sorry, i dont know how i missed that. thank you

##### h.sciascia 05/17/2021 14:46:50
Hello everyone ! 

I have two robots with the same architecture and node names. 

For exemple, I have two RotationalMotor called "motor 0", two "motor 1", two "motor 2" etc



I would like to create two list which refers to the id of my two robots motors architecture and be sure that I do not get the same motor



I would like to do something like this :



`Robot1 = supervisor.getFromDef("Robot1")

Robot2 = supervisor.getFromDef("Robot2")



for link in armChain1.links:

    if 'moteur ' in link.name:

        motor = Robot1.getDevice(link.name)

        motors1.append(motor)

for link in armChain2.links:

    if 'motor ' in link.name:

        motor = Robot2.getDevice(link.name)

        motors2.append(motor)`



In the documentation we can use getDevice for the robot but i get the error : "AttributeError: 'Node' object has no attribute 'getDevice'"

Thank in advance !

##### Stefania Pedrazzi [Cyberbotics] 05/17/2021 15:14:59
First, from a controller you can only access devices in the current robot and not devices from other robots.

Then, the `getDevice` is a method of the `Robot` object but your `Robot1` retrieved using the Supervisor API has the `Node` type.

So you should rather call `supervisor.getDevice` to retrieve the devices defined in the Robot node associated with the current controller.

##### h.sciascia 05/17/2021 15:17:27
Ok so I cannot use one controller for two same robots okok

Thanks

##### Stefania Pedrazzi [Cyberbotics] 05/17/2021 15:18:32
You can use the same controller for multiple robots, but multiple instances of the same controller will be started

##### h.sciascia 05/17/2021 15:20:06
ok I will do two controller then if I cannot use something like Node.getDevice("#")

Thx

##### Stefania Pedrazzi [Cyberbotics] 05/17/2021 15:23:27
You don't need the Supervisor API to identify the robot, you could for example set a unique name or set some value in the `Robot.customData` field or use the Supervisor simply to get the DEF name of the `supervisor.getSelf().getDef()` node but without using the returned node for getting the devices.

##### h.sciascia 05/17/2021 15:24:19
Oh okok I will try that ! Great

##### jmarsik 05/17/2021 16:22:01
Hello guys, is there any way to have a distance sensor that interacts with Solids (not their bounding boxes) and does not take into account the color (like the infra-red type)?

##### Simon Steinmann [Moderator] 05/17/2021 20:08:11
`@jmarsik` [https://www.cyberbotics.com/doc/reference/distancesensor#distancesensor-types](https://www.cyberbotics.com/doc/reference/distancesensor#distancesensor-types)


I believe they all use the shape, not bounding box, as they are visual sensors and use GPU


but i'm not 100% sure

##### harrymcc 05/17/2021 20:26:15
WARNING: Forward: The process crashed some time after starting successfully.

WARNING: 'Forward' controller crashed. Is this crash likely to do dodgy C code. Just in the console it didnt come up any issues

##### DDaniel [Cyberbotics] 05/17/2021 20:29:34
`@jmarsik` Laser/sonar and generic interact with `boundingObject`, infra-red with the solid itself. But since you can make the `boundingObject` as arbitrarily complex as you need it to be, the distinction shouldn't matter much. Can you explain more why you need it to interact with the solid instead of `boundingObject`? As for colored surfaces, only the infra-red is affected by it.


`@harrymcc` remembered to put `wb_robot_init();` and `wb_robot_cleanup();` ?

##### harrymcc 05/17/2021 20:39:05
yeah all thats in.  These lines seem to cause the issue   (not in order of code                                                                                                      

 
``` int linear_velocity = wheel_radius * MAX_SPEED; 
  int duration_side = length_side/linear_velocity;                  if (current_time > start_time + duration_side) {
 left_speed = 0;
 right_speed = 0;
 } 
```

##### DDaniel [Cyberbotics] 05/17/2021 20:44:32
hard to say from that, can you post the file?


`@harrymcc` `duration_side` isn't defined


or rather, `linear_velocity`, at least in the full code

##### harrymcc 05/17/2021 20:53:02
yeah tbh  i have really confused my self messing about. That what i sent wasnt the code that would compile and crash. Sorry for messing you about


is it okay if i upload it again. Got to same point i was on about

##### DDaniel [Cyberbotics] 05/17/2021 21:11:15
sure

##### harrymcc 05/17/2021 21:12:09
thank you
> **Attachment**: [Forward.c](https://cdn.discordapp.com/attachments/565154703139405824/843959101787996170/Forward.c)

##### DDaniel [Cyberbotics] 05/17/2021 21:25:59
`@harrymcc` `wheel_radius` should be a double, if you set it as int it assigns it zero, not 0.025. Hence, also `linear_velocity` becomes zero and `duration_side` is infinity. Similarly, `wb_robot_get_time()` also returns a double, not a int. You should recheck all the types of your variables

##### harrymcc 05/17/2021 21:28:32
Okay thanks for the help I'll take a look. Got it going now youre a life saver. Need to go back to c basics. Doing 1st year electrical engineering so i have only known C for last few months on and off

##### SeanLuTW 05/18/2021 02:53:27
I have a `MFFloat` field with 6 values, how can I access the 0th index in my PROTO?

##### Olivier Michel [Cyberbotics] 05/18/2021 06:17:52
You should use `wb_supervisor_node_get_proto_field` to get a handler to your field and then `wb_supervisor_field_get_mf_float` with `index=0` on it.

##### SeanLuTW 05/18/2021 06:21:44
I want to use these values in my PROTO instead of controller.


I found some examples here. ([https://github.com/cyberbotics/webots/tree/master/tests/protos/protos](https://github.com/cyberbotics/webots/tree/master/tests/protos/protos))

##### Olivier Michel [Cyberbotics] 05/18/2021 06:24:16
I see. You should then be able to access it as a Lua array. Note however that the first index in a Lua array is 1, not 0.

##### SeanLuTW 05/18/2021 06:26:02
Yes, the problem solved! But I wonder which language is PROTO file used, VRML or Lua?

##### Olivier Michel [Cyberbotics] 05/18/2021 06:27:23
It's the PROTO format (inspired from VRML97) to which you can apply Lua templating.

##### TheGiant 05/18/2021 07:29:01
Hello everyone,

I'm working with hydraulic actuators, which are commanded using a velocity. Their are really strong, hence their is two behavior : 1) when a velocity is requested, it's reached almost immediately (their is a internal regulation in it, which is very good, and I don't want to simulate it). I've tweaked the P[ID] value to get something nice, and also change the max force to something very large, which is what the actuator could do. If the requested velocity is positive (resp negative), then whatever the load the actuator will not output a negative (resp positive) speed. At worst it will not move.

2) When the velocity requested is 0, then the actuator will never move, (independently of the load), because the circuit in which the oil flow is closed.

For the moment, I'm using a linear motor, which seems to work fine, but not for a speed of 0 (the actuator retract because of the gravity and the load), which is actually logical.

Do you have any thought on that ?

I thought about using connector to "lock" the actuator when speed == 0, but its not pretty.

I thought also about adding a "lock" option to "transform" the motor in mechanical link when speed == 0.

##### Olivier Michel [Cyberbotics] 05/18/2021 07:33:24
Why not switching to position control instead of velocity control when velocity is 0?

##### TheGiant 05/18/2021 07:36:21
Yes, but I don't know which position should be used, and I'm not huge fan of adding a position sensor just for that (which don't exist in reality). Additionally, it will continue to cost time, because the solver will continue to regulate if some perturbation push it out of position, is it ?

##### Olivier Michel [Cyberbotics] 05/18/2021 07:39:13
Yes...

##### jmarsik 05/18/2021 08:46:16
That's right, but complex bounding objects really slow down physics computation and also make it unstable sometimes. Or at least that's my experience. But I will try it because it seems like the only option right now. Maybe another is to create a pull request and try to add a property like `red color sensitivity` that would tune this sensitivity by a factor (0..1) for infra-red sensors? But I am not confident if I have enough experience to do that.

##### TheGiant 05/18/2021 08:48:52
`@Olivier Michel` fyi, devil strike again : I'm been stuck on "Finalizing node" (oscillating weirdly between 19% and 99%) when creating (and so opening) a new project directory...Add to reboot my PC.


Is their any logs produced during file opening ?


And here we go again, but this time, stuck on "Opening world file" at 99%. SIGKILL send, htop confirm webots shutdown, but back in gui the windows is still here, screen frozen, with mouse cursor moving but messed up... 

I'm using a webots version built with "debug", but I don't expect to be the issue...

##### Olivier Michel [Cyberbotics] 05/18/2021 09:09:21
Did you try to debug it with gdb?

##### TheGiant 05/18/2021 09:25:15
I'm not, actually, that would be interesting.


I'm currently rebuilding it on master with "release" makefile, and see if it change anything, I wil give gdb a try next


Well, that didnt go according to the plan :/ Master + release => same issue (actually, I don't even see the "loading world" popup)

Rebuild with "debug", on the branch makefile-improvement (to get -ggdb flags) did nothing more... gdb doesn't seems to catch anything. (but I can see m gdb output, cause screen freeze...)

I think I will reinstall ubuntu, and give it another try. Also I continu to also blame my pc on that one, I've got very strange stuff in the past 

At least it's consistant : run webots from command line with a fresh empty world, cause it to crash each time

##### jmarsik 05/18/2021 12:24:36
Ok, I tried to implement it and submitted a PR 🙂 [https://github.com/cyberbotics/webots/pull/3077](https://github.com/cyberbotics/webots/pull/3077)

##### ShuffleWire 05/19/2021 08:24:20
I've made progress ! 🙂 Trying to reinstall Ubuntu doesn't change the issue (however, now I could say that python3-dev and python-is-python3 are mandatory to build Webots (and docs) on a fresh Ubuntu install)

I've added info to the bug, it's nicely reproducible,  and I found the faulty line. Updated to related issue [https://github.com/cyberbotics/webots/issues/3065](https://github.com/cyberbotics/webots/issues/3065) . Hope it might help...

##### Olivier Michel [Cyberbotics] 05/19/2021 13:09:57
Unfortunately, since we cannot reproduce this bug on our machines, you will have to continue to investigate on yours.

##### h.sciascia 05/19/2021 14:32:35
Hello everyone ! 

I  have a problem when I try to use PyQt5 with webots

The application seems to do not find PyQt 



If I uninstall PyQt everthing is ok



Do you have any clue ?

##### ftsell 05/20/2021 20:42:18
Hey folks,



I currently have the problem that webots starts lagging like crazy after resetting a world.

- Everything runs completely fine and smooth before the reset

- I don't have any controllers running

- I use this world: [https://github.com/bit-bots/wolfgang\_robot/blob/master/wolfgang\_webots\_sim/worlds/1\_bot.wbt](https://github.com/bit-bots/wolfgang_robot/blob/master/wolfgang_webots_sim/worlds/1_bot.wbt) (you can set the controllers to void)

- I was able to debug that our custom robot proto: [https://github.com/bit-bots/wolfgang\_robot/blob/master/wolfgang\_webots\_sim/protos/Wolfgang.proto](https://github.com/bit-bots/wolfgang_robot/blob/master/wolfgang_webots_sim/protos/Wolfgang.proto) seems to somehow be causing the problem because the simulation survives a reset when the robot is not in it.

- I am using `Webots version: R2021b Nightly Build 11/5/2021 1585fa928cccee28dbab882d925361a21369e727`



If anyone has any idea on how to debug this further I'd be happy to! Although imho it seems to be at least partially a webots internal problem since the problem only occurs after resetting the simulation.

##### mmoralesp 05/20/2021 21:45:03
Hi everyone, Im using the epuck robot to simulate an enviroment and i want to obtain the position on the enviroment via ROS or other way, does anybody know how to do this? Thanks

##### Simon Steinmann [Moderator] 05/20/2021 21:45:47
`@mmoralesp` use supervisor functionality or gps sensor


I downloaded your entire webots\_sim folder and there is many errors.


probably because i'm not on develop

##### Gowtham Sridhar 05/21/2021 08:52:18
I am using webots in ubuntu I am facing this error. I have added circle floor but its not rendering. were sample files are rendering excellently.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/845222461141155850/unknown.png)
%end

##### DDaniel [Cyberbotics] 05/21/2021 08:56:14
`@Gowtham Sridhar` Hi, it's because there's no light source on your world. Add the `Background` node, or `TexturedBackgroundLight` & `TexturedBackground` nodes for example, or something analogous to it

##### Gowtham Sridhar 05/21/2021 08:58:00
yeah thanks it works.

##### Maurice Faraj 05/21/2021 10:44:04
Hello everyone I’m facing a problem while I’m enabling the camera in my robots can anyone help me please



%figure
![image0.jpg](https://cdn.discordapp.com/attachments/565154703139405824/845251260944744508/image0.jpg)
%end
%figure
![image1.jpg](https://cdn.discordapp.com/attachments/565154703139405824/845251261347004416/image1.jpg)
%end

##### gian 05/21/2021 14:58:53
hey guys, im having a problem to align the wheels in my proto. they are running, but with a little pull to the right (the side that is misaligned)! does someone know how to make them with the same alignment on both sides, please?

##### DDaniel [Cyberbotics] 05/21/2021 19:22:57
Can you post the relevant part of the proto? Hard to say without

##### ftsell 05/21/2021 19:26:25
your controller is not valid python syntax. that's why it exits with an error exit code (1 in this case) and throws indentation errors.



If you want to have functions which don't have any implementation yet you can use the `pass` keyword like the following:

```
class MyClass:
    def my_function(self):
        pass
```

##### gian 05/21/2021 22:04:23

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/845421798257328158/unknown.png)
%end

##### スカイツー 05/22/2021 06:25:11
Hello everyone, Have you completely solved the camera lens distortion problem?



It seems that mainly radial distortions (radialCoefficients) have not been resolved.



[https://github.com/cyberbotics/webots/pull/2961](https://github.com/cyberbotics/webots/pull/2961)

##### Skev 05/22/2021 23:01:43
Hello, is it possible to move an object in the scene (like a can) that is not directly connected to a robot, programmatically, from the robot controller ? (for a project using object detection)

##### Simon Steinmann [Moderator] 05/22/2021 23:02:24
yes, using the supervisor functionality


[https://cyberbotics.com/doc/reference/supervisor?tab-language=python](https://cyberbotics.com/doc/reference/supervisor?tab-language=python)


the robot needs to be set to supervisor = True in the scene tree for it to work

##### Skev 05/22/2021 23:03:12
ok thanks I'm going to try that


Yes it works thank you

##### Seedow 05/23/2021 22:29:52
Hi!

Is there a way to programmatically build a world? Preferably from MATLAB

##### DDaniel [Cyberbotics] 05/23/2021 22:31:00
<@288787843042770945> Hi, you can use a supervisor to spawn the objects of the world yes

##### Seedow 05/23/2021 22:34:27
Thanks for your reply. Is it possible to do that without running the whole program? I wanted to generate a world file that could be later used for simulation.

##### Max\_K 05/24/2021 13:33:33
Does anyone have an idea why i get just one camera\_info message on the rostopic when starting webots? I m trying to use apriltags in webots with ros2. I m using robotino3 and his webcam. In the header the stamp is also missing. I know its a latched topic but i don't know how to use it.

header:

  stamp:

    sec: 0

    nanosec: 0

  frame\_id: ''

height: 480

width: 640

distortion\_model: plumb\_bob

d:

##### harrymcc 05/24/2021 14:13:35
double current_time = wb_robot_get_time(); is there anyway i can reset this current_time to 0 with code? 
``` if ( current_time > start_time + time + duration_side){
 left_speed = MAX_SPEED;
 right_speed = 0;
 }
 if ( current_time > start_time + time * 2){
  left_speed = MAX_SPEED;
 right\_speed = MAX\_SPEED;
``` currently im * with each turn but i want to find a way to turn it into a function that can be repeated instead of timesing

##### DDaniel [Cyberbotics] 05/24/2021 14:23:57
Not without resetting the entire simulation. What you can do instead is to track the number of timesteps that have elapsed (at every call of `wb_robot_step`) instead of relying on simulation time

##### harrymcc 05/24/2021 14:28:27
okay thanks, so would basically change the current\_time to what ever i declare wb\_robot\_step as

##### DDaniel [Cyberbotics] 05/24/2021 14:38:55
Depends what you're making, there's multiple options. You can keep a counter that you increase at every call of `wb_robot_step` (in the infinite loop for example) and change the program flow based on the counter value, or you call `wb_robot_step` by providing as argument the duration it has to similate, like `wb_robot_step(XX)` which will simulate the next XX ms.

##### harrymcc 05/24/2021 14:39:43
its just a simple 1 metre square


```double current_time = wb_robot_step(432);
 left_speed = MAX_SPEED;
 right_speed = MAX_SPEED;
 
 if (current_time > start_time + duration_side){
 left_speed = MAX_SPEED;
 right_speed = 0;}
 if (current_time > start_time + duration_side + turn_time  ){
  left_speed = MAX_SPEED;
 right\_speed = MAX\_SPEED;
``` so something like that?

##### DDaniel [Cyberbotics] 05/24/2021 14:49:02
No, like 

```
action A

wb_robot_step(duration_action_A)

action B

wb_robot_step(duration_action_B)
```

##### harrymcc 05/24/2021 15:31:33
```  while (wb_robot_step(TIME_STEP) != -1) {
    left_speed = MAX_SPEED;
 right_speed = MAX_SPEED;
 wb_robot_step(5000);
 left_speed = MAX_SPEED;
 right_speed = 0;
  wb\_robot\_step(10000);
``` think i have got hang of it nearly however one last question if you dont mind so after 15 second in spins on the spot but it doesnt do the first action. Am i meant to separate these further

##### DDaniel [Cyberbotics] 05/24/2021 15:35:02
Why did you keep the infinite while loop? Doesn't make much sense to mix the two.

##### Ilya Ryakin 05/24/2021 15:35:17
Hello!

##### harrymcc 05/24/2021 15:37:07
tbf im not sure im just doing some messing about because i have never really used the robot\_step and forgot aha

##### DDaniel [Cyberbotics] 05/24/2021 15:38:23
Or  you want to have it run around the square multiple times? If so then sure you can have a `while(1)` around  the sequence

##### harrymcc 05/24/2021 15:40:20
no sorry it literally needs to go around once. What i mean is so when i added 1 timestep it did after 5 seconds i added the second set of instruction which was 10 seconds but instead of doing it in order it added the 5 seconds and 10 seconds together and only did the last step which has confused me abit

##### DDaniel [Cyberbotics] 05/24/2021 16:02:50
Sorry I didn't understand much, but it could be due to the while loop messing things up. From what I understood you want to do, all you really need is something like:

```
while(1){ // or just do 4x times

action_go_straight

wb_robot_step(how_long_go_straight)

action_turn_on_itself

wb_robot_step(how_long_turn)
}
```

##### harrymcc 05/24/2021 16:04:12
Right that looks alot better actually. I just couldn't get my head around it at all. Thank you for the help anyways I  appreciate it alot

##### insaanimanav 05/25/2021 06:38:11
hi , so I had a .wbo file which was an object made by another team member of mine


how do I import this object into my enviroment and use it ?

##### Darko Lukić [Cyberbotics] 05/25/2021 07:38:38
You can use the `wb_supervisor_field_import_mf/sf_node` Supervisor functions:

[https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_field\_import\_mf\_node](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_field_import_mf_node)

or simply copy/paste the content of the .wbo file into your world (.wbt) file.

##### insaanimanav 05/25/2021 09:03:22
also I had another small question , I am trying to use the distance sensor in my robot


but whenever I try see the value of the distance sensor by using `print(ds.value())`


I get nan


how else do I see the current value being read by the sensor ?


This is my entire controller code that I am using for a robot which just has one child , distance sensor 
```from controller import Robot
from controller import LED
import time 

robot = Robot()

ds = robot.getDevice("distance sensor")
ds.enable(64)
print((ds.getValue()))
```

##### Darko Lukić [Cyberbotics] 05/25/2021 09:10:25
You are missing the `step()`:

```py
from controller import Robot

robot = Robot()

ds = robot.getDevice("distance sensor")
ds.enable(64)
robot.step(64)
print(ds.getValue())
```

##### insaanimanav 05/25/2021 09:10:58
shit yes


thanks


saved the day

##### h.sciascia 05/25/2021 11:37:22
Hello ! Does anyone use a personalised physics plugin ? 

I would like to get the position of my contact point with something like this : dWebotsConsolePrintf("%f, %f, %f", &contact[i].geom.pos[0], &contact[i].geom.pos[1], &contact[i].geom.pos[2]);

But it return only 0.000000, 0.000000, 0.00000 ...



If you know how can I get those position between a robot and the environnement it would be perfect !



Have a nice day 😉

##### Tahir [Moderator] 05/25/2021 13:08:03
Hello I am trying to simulate a tracked robot, imported from Blender
%figure
![robot.png](https://cdn.discordapp.com/attachments/565154703139405824/846736374548004884/robot.png)
%end


Everything so far seems OK but I am stuck on how ti create joints in Webots?


Track of the robots looks like this:
%figure
![track.png](https://cdn.discordapp.com/attachments/565154703139405824/846736631075700796/track.png)
%end


The motor(in below figure) powering the robot on each side is connected to the yellow parts in the above figure.
%figure
![motor_selected.png](https://cdn.discordapp.com/attachments/565154703139405824/846736877301006336/motor_selected.png)
%end


This motor rotates, which is coupled to the yellow part of the above figure. The assembly of the robot rotates(in the figure below) which rotates the tracks(second figure from above)


There are two adjustments wheels to tighten the tracks as desired. They rotate normally with the tracks.
%figure
![support_wheels.png](https://cdn.discordapp.com/attachments/565154703139405824/846737540723900456/support_wheels.png)
%end


I am not able to convert this mechanism to Webots anyone who can comment on how to create joints so that the tracks with all other assmebly and adjustments wheels rotates?


Thank you so much in advance for any suggestions

##### DDaniel [Cyberbotics] 05/25/2021 17:13:46
`@Tahir` Hi, have you looked into the sample world `track.wbt`? You will need to use the `Track` and `TrackWheel` nodes. [https://cyberbotics.com/doc/reference/track](https://cyberbotics.com/doc/reference/track)

##### Blindera 05/26/2021 00:51:15
Hello i have this issue idk how can i solve.. i'm using linux nightly build version and im new about linux too
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/846913339787313172/unknown.png)
%end

##### Simon Steinmann [Moderator] 05/26/2021 02:12:51
you are trying to change a default controller


just hit "OK", and it will create a copy of the controller in another location

##### Blindera 05/26/2021 02:17:24
But if i create a copy i cant simulate anymore.. these controller was created for my robo team and my teamates build The project like this and o cant do at The same way

##### Simon Steinmann [Moderator] 05/26/2021 02:18:17
just copy the project to your documents and open the world there

##### Blindera 05/26/2021 02:18:54
I Will try thank u

##### James Le Poidevin 05/26/2021 07:39:30
Hello, I am trying to simulate large terrain (multiple Km) using precise elevation grids and i was wondering if it's possible to load 'chunks' of the elevation grid(like in minecraft) around a robot to help the simulation?

##### oroulet [Premier Service] 05/26/2021 13:44:38
box\_node.getPosition() returning (nan, nan, nan) in nightly, does that say anything to anyone? same code return the position in stable release


same with getOrientation()


is that a nightly bug or an API change?

##### Stefania Pedrazzi [Cyberbotics] 05/26/2021 14:11:40
What is the `box_node` a node visible in the scene tree, a PROTO internal node or a base node?


Which nighlty are you testing? R2021b or R2021a-rev1?


We fixed an issue in R2021b with the `getPosition` function. It would be good if you could provide an example where this function is not working as you expect.

##### Darko Lukić [Cyberbotics] 05/26/2021 14:36:27
We added `wb_supervisor_node_get_pose`:

[https://github.com/cyberbotics/webots/pull/2932](https://github.com/cyberbotics/webots/pull/2932)

but we the `wb_supervisor_node_get_position` should still work.


You mean levels, something like this?

[https://ignitionrobotics.org/api/gazebo/2.10/levels.html](https://ignitionrobotics.org/api/gazebo/2.10/levels.html)

##### James Le Poidevin 05/26/2021 14:39:18
Yes i can't see the images but it sounds like that


Is it already in webots ?

##### bingdong 05/26/2021 16:14:16
Hi! For the LED node that is non-gradual (gradual parameter is False and multiple colours can be assumed by the Light node), can the intensity be modified from the controller? Documentation specifies it can be done for a monochromatic LED, but nothing is mentioned in case of multicolor LED.

##### Darko Lukić [Cyberbotics] 05/26/2021 16:28:44
Unfortunately not, but you can use Supervisor to delete and elements in the scene

##### Matt Estrada 05/26/2021 16:58:43
Hello! I am interested in aquatic simulation of an amphibious robot we are developing in the Biorobotics Lab at EPFL. We have implemented this in the past via `Fluid Nodes` and `Immersion Properties` . 



We are curious about a more complex interaction than  a single direction of fluid flow-- perhaps to implement flow along a "river" that might curve and simulate "flow" of this fluid in different directions, which could lead to some interesting motion planning problems. 



I notice `streamVelocity` as part of a [Fluid]([https://cyberbotics.com/doc/reference/fluid](https://cyberbotics.com/doc/reference/fluid)) node seems to implement a single direction of flow. Do you know if anyone has tried to vary this streamVelocity, or any node's property, while the simulation was running? If so, this might allow us imitate a water flow that changes with respect to the robot's position in the map. 



Not sure if I'm missing a higher level API for changing node properties in general that I might be able to use to attain this desired effect...

##### robert.hobbs 05/26/2021 18:12:56
Matt,  I was just reviewing the discord channel before asking the same question.  I need to model an AFV in a stream and it would be beneficail to include at least varying water velocities if not turbulent flows. I will follow your thread with interest and let you know if I find an answer beforehand.  Thanks

##### mclzc 05/26/2021 19:03:03
Hi, I was hearing this interview

[https://www.theconstructsim.com/webots-robot-simulator-ros-olivier-michel/](https://www.theconstructsim.com/webots-robot-simulator-ros-olivier-michel/)



and at minute 9:30 Oliver talks about preconfigured sensors. Is there an available list of these pre-configured sensors? I'd like to know of there are rgbd cameras configured like the Intel Realsense ones.

##### pnaraltnsk 05/26/2021 19:29:49
Hi, I am using Nao robot’s distance sensor and I want to disable the sensor for a specific object. Is that possible? Do you have any suggestions of how to do it?

##### mclzc 05/26/2021 21:21:20
Where could I find more information on how to translate Gazebo URDF to PROTO files? 



I know and have used the `xacro2proto` script (which underneath uses `urdf2webots`),  but I don't really know what did it do and to what extent did it work. I see the Gazebo robot that I'm using being rendered in Webots... but I'm completely clueless of if it's a complete and successful translation into the PROTO format. The first question that comes to my mind is, "are the sensors correctly defined in the PROTO file now?"



In this Github question ([https://github.com/cyberbotics/urdf2webots/issues/112](https://github.com/cyberbotics/urdf2webots/issues/112)) I have found the first piece of information that I have not seen around in the Webots tutorials that I've come across: `Why not introduce a <webots> xml tag instead of parsing the <gazebo> tags?`. Now I know that the script is indeed gazebo-tags aware and will look into the code, but would be nice to know what is it capable of.

##### Olivier Michel [Cyberbotics] 05/27/2021 05:50:36
Here the list of pre-configured sensors in Webots: [https://cyberbotics.com/doc/guide/sensors#commercially-available-sensors](https://cyberbotics.com/doc/guide/sensors#commercially-available-sensors). You can use a Kinect if you need a RGBD camera, or implement a Intel Realsense from basic sensors:  RGB camera + range finder camera: [https://cyberbotics.com/doc/reference/camera](https://cyberbotics.com/doc/reference/camera) + [https://cyberbotics.com/doc/reference/rangefinder](https://cyberbotics.com/doc/reference/rangefinder).


You can use the supervisor API to change any field of any node (including the `streamVelocity` field). See this example to change a field from a supervisor: [https://cyberbotics.com/doc/guide/supervisor-programming#setting-the-position-of-robots](https://cyberbotics.com/doc/guide/supervisor-programming#setting-the-position-of-robots)


Turn that object into a Transform (instead of a Solid)?


The best way is probably to study the source code the `urdf2webots` script, study the content of the URDF and PROTO files to determine how successful the conversion was. This `urdf2webots` script is constantly evolving and improving, but still has some limitations for some specific cases, you are welcome to contribute to improve it if you spot any problem.

##### bingdong 05/27/2021 06:22:30
Hi! For the LED node that is non-gradual (gradual parameter is False and multiple colours can be assumed by the Light node), can the intensity be modified from the controller? Documentation specifies it can be done for a monochromatic LED, but nothing is mentioned in case of multicolor LED.

##### Olivier Michel [Cyberbotics] 05/27/2021 06:24:11
Yes, simply setting a RGB color with less intensity (lighter or darker) should work.

##### bingdong 05/27/2021 06:25:37
But these changes would be to colour and not the intensity parameter?

##### Olivier Michel [Cyberbotics] 05/27/2021 06:25:53
Yes.

##### bingdong 05/27/2021 06:26:06
Alright, thanks!

##### oroulet [Premier Service] 05/27/2021 07:00:32
using 2021b

the node is an instanciated proto.

getting the relative position works fine with box\_node.getField("translation").getSFVec3f()

`

getPose() works also (What is getPose btw, the same as getPosition AND getOrientation()?)


box\_node.getPosition()

[nan, nan, nan]

(Pdb) box\_node.getPose()

[1.0, 0.0, 0.0, 1.455, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, -1.5, 0.0, 0.0, 0.0, 1.0]

##### Stefania Pedrazzi [Cyberbotics] 05/27/2021 07:02:22
Ok, could you please open an issue on the Webots GItHub posting your PROTO node? This way we will be able to inspect the issue

[https://github.com/cyberbotics/webots/issues/new?assignees=&labels=&template=bug\_report.md](https://github.com/cyberbotics/webots/issues/new?assignees=&labels=&template=bug_report.md)

##### oroulet [Premier Service] 05/27/2021 07:04:26
I will try but as usual our model is big so we need to simplify it before sending


`@Stefania Pedrazzi` what is node.getPose() also the global position? I cannot find it in documentation

##### Stefania Pedrazzi [Cyberbotics] 05/27/2021 07:06:14
Yes, it is the global world position and orientation of the node. Here is the R2021b documentation:

[https://www.cyberbotics.com/doc/reference/supervisor?version=develop#wb\_supervisor\_node\_get\_pose](https://www.cyberbotics.com/doc/reference/supervisor?version=develop#wb_supervisor_node_get_pose)

##### oroulet [Premier Service] 05/27/2021 07:06:51
ah OK this is a new one. Good

##### Greemfy 05/27/2021 09:33:00
Hi guys👋  I hope you can help me with my (probably far too easy to solve) question. I´m using python and got my Lidar sensor working. Right now I calculate the coordinates of every point with the values of getRangeImage and the angular position of the lidar. It´s painfull 😂  To my suprise there is the easy solution getting the coordinates from LidarPoint.*. But I can´t figure out how to access those values. In the WB doc theres something written about @property. Never worked with that. If I want to get one coordinate of a point it says "<property object at 0x000002990325B9A0>". Thank you so much!

##### Darko Lukić [Cyberbotics] 05/27/2021 11:27:12
Can share the part of the code in which you are accessing the point data?

##### James Le Poidevin 05/27/2021 13:23:19
OK i thought i would have to try that. Do you think it would be possible to use one elevation grid or will i have to use multiple one that i load one by one ?

##### Darko Lukić [Cyberbotics] 05/27/2021 13:57:03
It would probably be easier to have multiple elevation grids

##### mclzc 05/27/2021 14:53:12
Thanks for your answers `@Olivier Michel`

##### Jhon mathew 05/27/2021 16:50:57
hello, I have used camera recognition in webots using python. I did everything according to the resources but I have 2 major issue. 1. values is updating only 0  

2.  It says the index exceeds the list....







I need some basic pre-build code in *Python*  atleast to get id using camera recognition... Other things I can manage with that reference.

##### Matt Estrada 05/27/2021 19:00:17
Thank you for the very quick and helpful response! That should accomplish what I am looking for!

##### AmorFati 05/27/2021 19:53:30
I have an environment where i need to recognize an object (Wooden Box) and reach that location. I am using E-Puck for my project. I got how to recognize the object which is inbuilt. Not sure what logic should i follow once i detect it.

##### Simon Steinmann [Moderator] 05/27/2021 23:07:16
`@AmorFati` [https://cyberbotics.com/doc/reference/camera?tab-language=python#camera](https://cyberbotics.com/doc/reference/camera?tab-language=python#camera)


you can enable recognition

##### Jhon mathew 05/28/2021 02:13:25
please someone give basic code in python..  I need to get information about recognised object.

##### Simon Steinmann [Moderator] 05/28/2021 02:14:16
recognitionObjects = camera.getRecognitionObjects()

    if len(recognitionObjects) > 0:

        recognitionPosition = recognitionObjects[0].get\_position()

        recognitionSize = recognitionObjects[0].get\_size()


` camera.getRecognitionObjects()` returns a list, so when there is no recognized objects, the list is empty


objects need their recognition color field defined in order to be detected

##### Jhon mathew 05/28/2021 03:07:57
thank you. ill try that now


Object is detecting by the camera. I have done some minor mistake.. thank you

##### AmorFati 05/28/2021 04:31:44
Thanks Simon, but I think Epuch does not has Recognition node. Because when i do camera.hasRecognition(), it gives false. Can you guide me how to add one.

##### Simon Steinmann [Moderator] 05/28/2021 04:32:19
you have to enable recognition mode


use the api documentation


camera = robot.getDevice('camera')

camera.enable(timeStep)

camera.recognitionEnable(timeStep)


something like that

##### AmorFati 05/28/2021 05:03:46
Yeah i tried that but its giving me below error: Error: wb\_camera\_recognition\_enable() called on a Camera without Recognition node.


I tried this


camera = robot.getDevice("camera")

    camera.enable(time\_step)

    camera.recognitionEnable(time\_step)


where robot is EPuck robot

##### Simon Steinmann [Moderator] 05/28/2021 05:22:53
you might have to check the camera node, or maybe add a recognition color to the bot and or the camera

##### Cyber Police Officer 05/28/2021 10:19:58
I've tried to search the documentation, but didn't find any information about this. Is it possible, using the propeller node, to make a propeller have a different thrust coefficient depending on the rotation direction? 



I know i can workaround using two propellers, facing in opposite directions, having each the correct thrust coefficient, but was wondering if there were a cleaner way.



Thank you

##### Beginner26798 05/28/2021 13:06:47
Hi.. is there any helpful tutorials to help me if i want to make a robot that follows the recognised object


Python language


??

##### Darko Lukić [Cyberbotics] 05/28/2021 14:51:46
If you set a negative velocity it doesn't invert the thrust?


[https://github.com/lukicdarkoo/webots-example-visual-tracking](https://github.com/lukicdarkoo/webots-example-visual-tracking)

##### Beginner26798 05/28/2021 15:10:00
Thank you


What's the solution of this
%figure
![20210528_181021.jpg](https://cdn.discordapp.com/attachments/565154703139405824/847854388370079752/20210528_181021.jpg)
%end

##### Cyber Police Officer 05/28/2021 15:20:26
It does. But some propellers have different thrust coefficients depending on the thrust direction

##### Beginner26798 05/28/2021 17:00:52
`@Darko Lukić` ?

##### AmorFati 05/28/2021 18:18:22
Can anyone tell how to add recognition colors to the WoodenBox in webots

##### Simon Steinmann [Moderator] 05/28/2021 21:36:47
`@AmorFati` you have to convert it to Base Nodes (right click). Then you can add the colors

##### AmorFati 05/28/2021 21:37:14
Thanks simon. I will try that.


And one more thing I have got the object recognized. And I am not able to understand what is the difference between position and position\_on\_image in the API?

##### Simon Steinmann [Moderator] 05/28/2021 21:39:07
read the text under the commands. It explains it


are you working on the vacuum project as well?

##### AmorFati 05/28/2021 21:39:44
Yeah i read it but not able to understand what it says exactly. I am getting x and y values in that


No I am working on my own college Project

##### Simon Steinmann [Moderator] 05/28/2021 21:40:17
> The position\_on\_image and size\_on\_image can be used to determine the bounding box of the object in the camera image, the units are pixels

##### AmorFati 05/28/2021 21:40:57
Does it give the center of the bounding box of the recognised object?

##### Simon Steinmann [Moderator] 05/28/2021 21:41:06
I would assume so

##### AmorFati 05/28/2021 21:41:11
Okay


and what does just Position gives ?

##### Simon Steinmann [Moderator] 05/28/2021 21:41:41
it gives the relative position of the recognized object to the camera


as a sidenote: get\_size() has a bug in the current release, where it returns a integer instead of a float. So you will get the size rounded to the next full meter. Install the the latest nightly build if that is the case for you

##### AmorFati 05/28/2021 21:44:22
Thanks Simon. This was super helpful

##### notjfarhan 05/29/2021 04:05:05
```webots/projects/default/controllers/ros/ros: error while loading shared libraries: libroscpp.so: cannot open shared object file: No such file or directory

WARNING: 'ros' controller exited with status: 127.
```

All I did was create a world , get a robot in and attach the generic 'ros' controller. Is there anything else I have to do ?


hello ?

##### insaanimanav 05/29/2021 08:39:29
How do I create sensors of my own ?


For example I want to create a simple sensor that reads some value of a node in the world , I couldn't find anything in the docs for that

##### Bitbots\_Jasper [Moderator] 05/29/2021 08:47:45
If you have ROS installed try running webots from a terminal where ROS is sourced


What exactly are you trying to read? If you want to know some information about some object in your simulation have a look at the supervisor API. That might already fulfill your requirements

##### bingdong 05/29/2021 08:51:41
`@Beginner26798` You'll have to install the cv2 library to your system. If you have pip installed, open command prompt on windows and type: pip install opencv-python

 [https://stackoverflow.com/questions/19876079/cannot-find-module-cv2-when-using-opencv](https://stackoverflow.com/questions/19876079/cannot-find-module-cv2-when-using-opencv)

##### insaanimanav 05/29/2021 08:55:45
Also one small question how can I add a custom property to something ?


For example I want to have solid which has the custom property of let's say Ph


Maybe humidity too

##### notjfarhan 05/29/2021 09:40:29
Thanks I got it.

##### R K gupta 05/29/2021 15:24:12
Does there is any way to add some extra sensors like temperature and PH sensors in webots to analyse the environment around it !like how  do  we get temperature of a particular thing  like environment in  webots ?

##### SHAHRRUCK 05/30/2021 05:18:01
Is there any way to create water pump in webots for irrigation purposes ??

##### viorel\_gheorghe 05/30/2021 21:36:19
Hi guys!


Maybe is an already answer question, but I have not found the proper response 🙂


Is there a way to save the sensor data from a robot (position, gyro, accelerometer) and plot them in excel?

##### Simon Steinmann [Moderator] 05/31/2021 01:11:46
`@viorel_gheorghe` you should google that for the programming language that you use. This is not Webots specific

##### viorel\_gheorghe 05/31/2021 05:05:44
`@Simon Steinmann`, I'm using Python. I thought there is an in-built feature of the controller since it can print the evolution on the sensors  if selected


But we learn and learn 🙂

##### hanm 05/31/2021 10:28:46
hi, I have met some problem about webots, a create a env which have a ur10e robot and a conveyor belt. I set the ur10e  as a supervisor and I want the ur10e controller can enable a kinect which is a child in the conveyor belt, but I failed to enable it.


my code is :


ur10erobot = Supervisor()



\# Get the time step of the current world.

timestep = int(ur10erobot.getBasicTimeStep())



conveyor\_belt = ur10erobot.getFromDef("CONVEYOR\_BELT")

assert(conveyor\_belt != None)

conveyor\_kinectcolor = conveyor\_belt.getDevice("kinect color")

conveyor\_kinectcolor.enable(int(1000/24))


the error report is :


conveyor\_kinectcolor = conveyor\_belt.getDevice("kinect color")

  File "/usr/local/webots/lib/controller/python27/controller.py", line 1756, in <lambda>

    \_\_getattr\_\_ = lambda self, name: \_swig\_getattr(self, Node, name)

  File "/usr/local/webots/lib/controller/python27/controller.py", line 96, in \_swig\_getattr

    raise AttributeError("'%s' object has no attribute '%s'" % (class\_type.\_\_name\_\_, name))

AttributeError: 'Node' object has no attribute 'getDevice'

##### Stefania Pedrazzi [Cyberbotics] 05/31/2021 10:46:11
It is not possible to control device of another robot, i.e. in your case you cannot controller the conveyor belt devices from the ur10e controller if they are two distinct robots.

Many different solutions exists depending of the specific use case.

One option could to use the Emitter/Receiver mechanism to send a message from the ur10e controller to the conveyor belt controller notifying that the Kinect should be enabled.

##### hanm 05/31/2021 10:55:03
thanks, by the way, I want to know how can  I set two robot 's controller as `extern`  and control it by two progress?

##### Stefania Pedrazzi [Cyberbotics] 05/31/2021 10:55:58
here is the documentation:

[https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers#single-simulation-and-multiple-extern-robot-controllers](https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers#single-simulation-and-multiple-extern-robot-controllers)

##### hanm 05/31/2021 10:57:30
thanks~

##### mmoralesp 05/31/2021 17:40:44
Does anybody know how to output in ros2 the odometry info?

##### R K gupta 05/31/2021 18:28:48
Does any one have any idea how we add some extra sensors like temperature and PH sensors in webots to analyse the environment around it !like a farm environment in  webots ?

##### SHAHRRUCK 05/31/2021 20:33:17
How to display/play videos in webots ??


I am trying Motion class in webots


But I am getting Unexpected end of file as error, How Should I solve this


`from controller import Supervisor`motion = `Motion("C:/Users/Shahrruck/Downloads/water2.mp4")

 video = Motion.play(motion)`

## June

##### Simon Steinmann [Moderator] 06/01/2021 01:38:22
Adding "true" extra sensors is difficult, but may be achieved with physics plugins [https://cyberbotics.com/doc/reference/physics-plugin](https://cyberbotics.com/doc/reference/physics-plugin) . You can also try using existing sensors. Camera with object recognition comes to mind. You could store information such as PH and Temp in the recognition color values (RGB, so you have 3 values) [https://cyberbotics.com/doc/reference/camera?tab-language=python#camera-recognition-object](https://cyberbotics.com/doc/reference/camera?tab-language=python#camera-recognition-object)

##### hanm 06/01/2021 03:06:41
hi, I meet a problem when I get camera image in python


the code is :



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/849121847232299008/unknown.png)
%end


but the image  in 23 line is None

##### Simon Steinmann [Moderator] 06/01/2021 03:08:22
you have to execute a step before. You can't get an image before you simulated the first step

##### hanm 06/01/2021 03:09:43
it work! thanks 🙂

##### mclzc 06/01/2021 03:10:38
Hi, are you planning to support the parsing of SDF tags, for example <gps>?

##### Darko Lukić [Cyberbotics] 06/01/2021 07:23:41
We plan only to parse what is specified by URDF and for the missing components we plan to introduce a `<webots />` tag:

[https://github.com/cyberbotics/urdf2webots/issues/112](https://github.com/cyberbotics/urdf2webots/issues/112)

##### viorel\_gheorghe 06/01/2021 07:31:50
`@Darko Lukić` If I have a parallel robot, and I need the position and orintatio  of the moving platform (yellow one). I must use an IMU, or there is some other things that I can do


parallel robot
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/849188543557140490/unknown.png)
%end

##### Darko Lukić [Cyberbotics] 06/01/2021 07:33:17
You can use `wb_supervisor_node_get_position` and `wb_supervisor_node_get_orientation`:

[https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_get\_position](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_get_position)

##### viorel\_gheorghe 06/01/2021 07:33:59
sexy! it's just a RTFM thing!


thank you, it seems that the supervisor is the way! 👍

##### Ayk 06/01/2021 11:35:23
Hey there,

I'm working with webots and Nao and trying to get it to move for x distance. 

What do you think is the best approach to do this (im new to webots).

Just using the motion file and doing the forward motion for a certain time (dependend on my distance)? Or is there a better approach ?

##### Simon Steinmann [Moderator] 06/01/2021 22:18:16
`@Ayk` you can turn the wheels an exact angle. Together with its radius, you can calculate the distance.

##### Kirlin 06/02/2021 02:35:13
Hi everyone, can you help me with a question ?

I have a custom controller linked with ros, that use as "extern", and I'm trying to make a Supervisor, so it will be more simple to reset the position of the robot when it falls, but I've read all the documentation and tried a lot of things, but I had no progress.

Do you guys have any suggestion ?

##### Simon Steinmann [Moderator] 06/02/2021 02:35:51
The robot has to be a supervisor (checkbox in the scene tree)


`from controller import Supervisor` instead of Robot

##### Kirlin 06/02/2021 02:36:52
yep, I've done that


but I capture the robot node like self.natasha = Robot()


and then I operate self.natasha with all the functions


when I import Supervisor


how can I capture the Robot node ?

##### Simon Steinmann [Moderator] 06/02/2021 02:37:57
self.natasha = Supervisor()


it has all the Robot functionality

##### Kirlin 06/02/2021 02:39:10
wow


I have tried so many things


I was trying self.supervisor = Supervisor and then self.natasha = supervisor.getFromDef('Natasha') or getSelf()


so now I can just operate self.natasha with the Robot functions and the Supervisor functions ?


thank you so much

##### Simon Steinmann [Moderator] 06/02/2021 02:40:44
you have to do that if you want a handle to the robot node, for doing supervisor manipulations


you're welcome :)=

##### Kirlin 06/02/2021 02:42:15
like set translation or rotation ?

##### Simon Steinmann [Moderator] 06/02/2021 02:43:45
yes

##### mclzc 06/02/2021 03:06:20
Hi, Do you have a way of outputting `sensor_msgs/msg/PointCloud2` from a `RangeFinder`? I see that the `range_finder_device.py` file creates a publisher for `sensor_msgs/Image`.

##### Darko Lukić [Cyberbotics] 06/02/2021 07:25:52
No, only the Lidar publishes `sensor_msgs/msg/PointCloud2`. Can you contribute `sensor_msgs/msg/PointCloud2` for the range finder? Although I am not sure whether it is better to implement the point cloud generation in the Webots core. Please open the issue about that so we can discuss.

##### AloneLeader 06/02/2021 14:18:47
Hey, i just was looking for a tutorial and end up with this channel. I need to create a robotic simulation for my term project. Is there anyone who like to help a newbie?

