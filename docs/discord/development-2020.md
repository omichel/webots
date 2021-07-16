# Development 2020

This is an archive of the `development` channel of the [Webots Discord server](https://discordapp.com/invite/nTWbN9m) for year 2020.

## January

##### Shanaka 01/02/2020 08:15:57
I'm shanaka currently working as a research engineer in the department of electrical and electronic engineering, University of Peradeniya, Sri Lanka. These days, I engaged to design simulation using PUMA 560 robot manipulator in WEBOT and note that I'm very new to webot. 



I modified the end-tool of the manipulator by replacing the Pioneer gripper



I  control this entire manipulator (with new gripper) by modifying puma560.motion file and puma560.c file. 



But I need to add dynamics and individual joint controllers to this simulation with the idea of implementation in real world. So i check puma560.c file and there I found  functions called 



wbu\_motion\_new("puma560\_1.motion");

wbu\_motion\_play(motion);

but I was not able to find the definition of these functions anywhere. ( I checked motion.h Motion.hpp Motion.cpp files.) Although I can find the function definitions inside the above-mentioned files, still I was not able to find function definitions and how the given angles fed to motors in the manipulator by wbu\_motion\_play() function. 



The final target is to implement the controller in the real world using Raspberry pi or FPGA microcontrollers. So by using wbu\_motion\_play() functions how can I do it? Where can I find the function description? If this method is impossible, it will be great if you can give guidance to make it a success. 



your response will be highly appreciated !!
%figure
![image.png](https://cdn.discordapp.com/attachments/565155651395780609/662207440632414248/image.png)
%end

##### David Mansolino [Cyberbotics] 01/03/2020 07:32:26
`@Shanaka` instead of using motion file you should control directly the motor individually  (in speed, position or torque/force) using the motor API: [https://cyberbotics.com/doc/reference/motor#motor-functions](https://cyberbotics.com/doc/reference/motor#motor-functions)

You can easily get the name of the individual motors by double-clicking on the robot to open the robot-window.

IF you didn't already did, I strongly recommend to follow our tutorial (which explains how to control the motors individually): [https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials)

##### rbhuva 01/15/2020 02:09:57
anyone has Breve Simulation Environment?

##### David Mansolino [Cyberbotics] 01/15/2020 06:54:13
`@rbhuva`, no we are using the Webots simulation environment 😉

##### rbhuva 01/15/2020 23:33:21
sorry guys I am asking very silly question but I am stuck

I downloaded and try various versions of the Webot software but after installing it always shows the error of unexpected stop working webots on windows. I need the software please give some solution.

##### David Mansolino [Cyberbotics] 01/16/2020 06:51:32
`@rbhuva`, what is exactly the error you get?

PLease make sure to update the driver of your GPU.


Using the safe mode might help: [https://cyberbotics.com/doc/guide/starting-webots#safe-mode](https://cyberbotics.com/doc/guide/starting-webots#safe-mode)

##### rbhuva 01/16/2020 06:52:29
Thanks `@David Mansolino`

##### David Mansolino [Cyberbotics] 01/16/2020 06:52:50
You're welcome

##### Angerbot 01/17/2020 14:28:29
Apologies for the continuous questions - lines here have broken down and unable to download just yet. Is there a drag and drop interface for simple movement design? Thanks

##### David Mansolino [Cyberbotics] 01/17/2020 14:29:55
No sorry, Webots doesn't provide such drag and drop interface

##### machinekoder 01/23/2020 10:05:11
I have acquired an EZgripper ([https://sakerobotics.com/](https://sakerobotics.com/)) for my robot. What's the best way to simulate such an under actuated gripper? I've written a small Python controller, but I can't really make it grasp properly. Any ideas? I don't really need the physics aspect of the grasping itself, just being able to move objects around.

##### Fabien Rohrer [Moderator] 01/23/2020 10:06:42
Hi,


Generally all our grippers have physics, collision  shapes and joints.


This is certainly the best way to implement this.


Other solutions looks like hacks to me.


Does this answer your question?

##### machinekoder 01/23/2020 10:10:52
Not really. The example grippers seem to have actuated joints. However, the ezgripper has two under-actuated joints, meaning they are not driven by a motor. What's the best way to simulate this behavior. Or shall I just actuate them in the sim? I tried to mimic the real behavior using touch sensors, but my approach wasn't successful.


[https://youtu.be/240b1ubUwMA](https://youtu.be/240b1ubUwMA)

##### Fabien Rohrer [Moderator] 01/23/2020 10:48:08
I think you need to actuate the Webots joints of the underactuated joints, yes.


I had similar issues for the PR2 simulation.


[https://www.youtube.com/watch?v=Lm0FhXAxkXg](https://www.youtube.com/watch?v=Lm0FhXAxkXg)


The most important thing was to control the gripper in force control rather than in position control.


In such case, you can master the grasping force precisely, and that matters 🙂

##### machinekoder 01/23/2020 10:58:35
`@Fabien Rohrer` Thanks, I'll try that. Is the PR2 model in the examples?

##### Fabien Rohrer [Moderator] 01/23/2020 10:59:05
Yes, here it is:


[https://www.cyberbotics.com/doc/guide/pr2](https://www.cyberbotics.com/doc/guide/pr2)

##### Hannah 01/23/2020 13:38:38
Hello, is there any way to get MES(Manufacturing Execution System) data in webots? I want to simulate a production line, and I need to work with the MES data generated by the machines in the line.

##### David Mansolino [Cyberbotics] 01/23/2020 13:40:09
Hi `@Hannah` not out of the bxox, but I am pretty sure that you can implement your own interface to take these MES data as input of your robot controller.

##### Hannah 01/23/2020 13:42:27
Thanks `@David Mansolino` , I wanted to know if it is already implemented or not.

##### David Mansolino [Cyberbotics] 01/23/2020 13:43:48
Unfortunately it is not.

##### Hannah 01/23/2020 13:49:07
Thanks 🙂

##### David Mansolino [Cyberbotics] 01/23/2020 13:49:31
You're welcome

##### ZoRal 01/23/2020 14:12:18
Thanks

##### nitrow 01/25/2020 19:18:45
I would suggest moving the CoM a little in front of the turtlebot3 model to counteract it being very unstable when accelerating. I worked a lot better when I did this. I don't know the correct CoM, but I'm sure the real turtlebot is not that prone to doing wheelies! 😄

##### David Mansolino [Cyberbotics] 01/27/2020 07:11:37
Hi `@nitrow`, thank you for the suggestion, I will check in the datasheet if I find the CoM and fix it if there is some issues.


`@nitrow`, just to let you now that this is now fixed in [https://github.com/cyberbotics/webots/pull/1300](https://github.com/cyberbotics/webots/pull/1300) and will be included in the next release (and the nightly builds).

##### nitrow 01/28/2020 10:18:03
`@David Mansolino` Great, works a lot better now!

##### David Mansolino [Cyberbotics] 01/28/2020 10:19:07
`@nitrow` perfect, thank you for the feedback!

## February

##### rbhuva 02/07/2020 16:51:33
Hi there

I want to make one model in which I want multiple sources and number of bots. And I want to see that how the bots will distribute towards the various sources.

Can anyone please help me how can I achieve that?

##### luoyu2014 02/09/2020 07:44:42
Hi, I want to know where I can download the Webots 2018b version, I need this version. Thank you.

##### David Mansolino [Cyberbotics] 02/10/2020 06:36:21
Hi `@rbhuva` what do you mean by sources? Source of robots? Of light? Of energy?


Hi `@luoyu2014` you can download all the old version fo webots from here: [https://github.com/cyberbotics/webots/releases/tag/R2019a](https://github.com/cyberbotics/webots/releases/tag/R2019a)

##### junjihashimoto 02/13/2020 08:30:48
Hello, I'm Junji Hashimoto, Gree Inc. I've developed webots-bindings for haskell.  [https://hackage.haskell.org/package/HsWebots](https://hackage.haskell.org/package/HsWebots)

##### David Mansolino [Cyberbotics] 02/13/2020 08:32:15
Hi `@junjihashimoto`, very nice!!!


just for the note, the link to the github project seems wrong (it seems you forgot te replace the 'githubuser') 😉

##### Fabien Rohrer [Moderator] 02/13/2020 08:34:11
That's a great news! You should post it in the <#568354695513374730> chanel for a better visibility.

##### junjihashimoto 02/13/2020 08:44:39
`@David Mansolino` Thank you for your feedback. I've fixed it.

##### David Mansolino [Cyberbotics] 02/13/2020 08:46:14
You're welcome, thank you for sharing this with us.

##### junjihashimoto 02/13/2020 08:54:12
`@David Mansolino` I want to check what fields the nodes have. Does webots have the api finding fields?

##### David Mansolino [Cyberbotics] 02/13/2020 08:55:49
Yes, you can use the supervisor api to get the fields of a node:

[https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_get\_field](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_get_field)

##### junjihashimoto 02/13/2020 09:00:15
Thx! If the field does not exit, It returns NULL. So I should check NULL or not here. [https://github.com/junjihashimoto/HsWebots/blob/master/src/Webots/Supervisor.hs#L144](https://github.com/junjihashimoto/HsWebots/blob/master/src/Webots/Supervisor.hs#L144)

##### David Mansolino [Cyberbotics] 02/13/2020 09:02:39
Both should work, as testing:

```
if (X == NULL)
```

Is equiavlent to:

```
if (!X)
```

If X = NULL.

##### junjihashimoto 02/13/2020 10:07:37
The fix is done. [https://github.com/junjihashimoto/HsWebots/blob/master/src/Webots/Supervisor.hs#L148](https://github.com/junjihashimoto/HsWebots/blob/master/src/Webots/Supervisor.hs#L148)

##### junjihashimoto 02/14/2020 04:11:57
`@David Mansolino` Thank you for inviting me on github.


Can I move HsWebots-project into cyberbotics? If you have any concerns, please feel free to contact me.


BTW, I want to do CI on both linux and macos. I know linux has  a deb package of webots. So I can install it by apt. How about macos?


Do you provide binary-tar-ball for macos?

##### David Mansolino [Cyberbotics] 02/14/2020 07:43:43
> Can I move HsWebots-project into cyberbotics? If you have any concerns, please feel free to contact me.

`@junjihashimoto` 

Thanks for this, we will discuss this internally ant let you know.


Adding a CI is a very good idea and sign of quality.

For macOS unfortunately we do not provide any binary-tar-ball, however it should be feasible to install the mac package from the command line.


Jsut out of curiosity, which CI are you planning to use ?


It would also be very nice if you could include in the README the version of Webots and the supported OS.

##### junjihashimoto 02/14/2020 08:06:40
I will use github-actions, because it has 8GB memory and long running time.

##### David Mansolino [Cyberbotics] 02/14/2020 08:08:09
Ok, interesting.


`@junjihashimoto` we just had a discussion and you are welcome to transfert your HsWebots-project  🙂

##### junjihashimoto 02/14/2020 09:47:18
`@David Mansolino` Thx! I'm grad to hear that. I will do it.


Setup CI is done.

##### David Mansolino [Cyberbotics] 02/14/2020 13:15:00
Perfect !

##### Axel M 02/21/2020 09:28:05
Some issue I ran into when using urdf2webots:

- The resulting model in webots is laying on its right side (-90 deg roll). This seems to be related to axes differences between ROS (Z upward) and webots (Y upward)

- Large meshes are merged into a single PROTO file, which results in a (very) big file not suited to manual modifications in a text editor



I'm eager help on those issues

##### Olivier Michel [Cyberbotics] 02/21/2020 09:35:46
Regarding point 2 (large meshes), we are considering implementing support for meshes in Webots in different formats (DAE, OBJ, STL) by introducing a new node called Mesh that would replace the IndexedFaceSet node by referring to a mesh file (DAE, OBJ, STL, etc.) instead of listing a large number of coordinates and coordinate indices. I will open an issue about it to explain our design ideas. You are very welcome to contribute by proposing an implementation.


See [https://github.com/cyberbotics/webots/issues/1396](https://github.com/cyberbotics/webots/issues/1396)

##### Axel M 02/21/2020 09:51:54
Thats great, in the meantime I was considering modifying urdf2webots in order to generate PROTO files containing only the IndexedFaceSet of the parsed mesh (that's what i've done manually on my model). In the light of the above issue, does that seems still relevant to you ?

##### Olivier Michel [Cyberbotics] 02/21/2020 10:07:30
Yes, that's a very good idea as well.

##### rbhuva 02/24/2020 15:14:28
In my project, I would be changing the number of light sources considering them as a food source for the robot (treated like animals). The hypothesis of doing so is to observe the behaviour of an individual animal and swarm of the animals when they have different food sources available at different locations. Moreover, it is also assumed that it is possible to simulate the behaviour of the animals towards the food origin of different quality. That is, they might turn to the light(food) source of the higher intensity even though the less intense light source is nearer to them

To carry out these experiments, I will consider the intensity of the sources, number of sources, number of vehicles as my independent variables.


can any one help me to achieve this...

##### Olivier Michel [Cyberbotics] 02/24/2020 15:21:47
Yes, Cyberbotics can provide you with paid support if needed. You can ask for an offer at sales@cyberbotics.com. Otherwise if you have some quick question, we may answer you here.

##### rbhuva 02/24/2020 16:48:16
okay thank you


i want code for the bot such that it senses the light and go towards that


i am ready to use either khepera-1 or epuck


because i try a lot but failed in coding


and i have very less time


I want the behaviour such that my robot go in direction of light and then stop near the light 



if someone can help me with that code I am very thankful...

##### David Mansolino [Cyberbotics] 02/25/2020 06:36:38
`@rbhuva` you can find an example of robot going in the direction of the light here: [https://cyberbotics.com/doc/guide/samples-devices#light\_sensor-wbt](https://cyberbotics.com/doc/guide/samples-devices#light_sensor-wbt)

The e-puck sensor model already have light sensors so using this robot model should be quite easy to do what you want

##### cnaz 02/25/2020 08:36:14
I have a project to link a algorithm of rei forcément learning and Webots. My programming learning is python. But the problem is I don't know where to do it. Is someone want to realize the project with me or help, I'll be grateful. Thx you guys (my operating system is Ubuntu 18)

##### Olivier Michel [Cyberbotics] 02/25/2020 08:44:52
Hi `@cnaz`, I would recommend you to start with simple Python controllers (see for example Webots/projects/languages/python/worlds/), define a task to be learnt by the robot and use reinforcement learning techniques in your Python controller.


I would recommend you to visit [https://robotbenchmark.net](https://robotbenchmark.net) for a series of Python-based robotics challenges that may involve reinforcement learning.


All these benchmarks are also included in Webots in the Webots/projects/samples/robotbenchmark folder.

##### cnaz 02/25/2020 11:43:03
Hi `@Olivier Michel` I already do the simple python controllers, and it works but with an algorithm of reinforcement learning I don't see where to star and what Ill do. I'll visite robotbenchmark

##### DrVoodoo [Moderator] 02/25/2020 11:45:08
Hello `@cnaz` , what approach were you planning on using for reinforcement learning?

##### David Mansolino [Cyberbotics] 02/25/2020 11:45:42
`@cnaz` if you are not familliar with this page, I would suggest to have a look at it: [https://www.cyberbotics.com/doc/guide/using-numerical-optimization-methods](https://www.cyberbotics.com/doc/guide/using-numerical-optimization-methods)

##### cnaz 02/25/2020 11:56:27
`@DrVoodoo` I plan to use Q learning


`@David Mansolino` thx you, I'll look

##### rbhuva 02/26/2020 01:21:11
how can i configure proximity sensor in khepera-1?

and how to add it into code...?

Or how can I avoid collision of robot with wall of circle arena...

##### David Mansolino [Cyberbotics] 02/26/2020 06:21:10
Hi `@rbhuva` the distance sensors of the khepera-1 are already present in the model: [https://cyberbotics.com/doc/guide/khepera1](https://cyberbotics.com/doc/guide/khepera1)

To use them, I strongly recommend following this tutorial: [https://cyberbotics.com/doc/guide/tutorial-4-more-about-controllers](https://cyberbotics.com/doc/guide/tutorial-4-more-about-controllers)

It uses the e-puck robot, but it works the exact same way for the khepera robot (excepts that the device names are different)

##### rbhuva 02/26/2020 06:26:57
Thank You so much

##### David Mansolino [Cyberbotics] 02/26/2020 06:42:40
You're welcome

##### rbhuva 02/26/2020 06:45:11
i have attached a GPS at extension of the Khepera-1 can anyone give me the code to store the GPS co-ordinates?

##### David Mansolino [Cyberbotics] 02/26/2020 06:45:38
[https://cyberbotics.com/doc/reference/gps](https://cyberbotics.com/doc/reference/gps)

##### rbhuva 02/26/2020 23:06:51
any other solution beside it?

##### David Mansolino [Cyberbotics] 02/27/2020 06:19:17
Beside using a GPS ?

##### rbhuva 02/27/2020 15:36:37
thank you so much...

##### elkelkmuh 02/28/2020 12:16:35
Hi `@David Mansolino`   I saw your pricing list. On the event you can give special support. Could you give FSR support for real robot. If you give support. How much is it?

##### David Mansolino [Cyberbotics] 02/28/2020 13:03:24
Hi `@elkelkmuh` yes of course we can give support for such kind of things, however, we would need to have access to the real robot with FSR to develop this.

##### elkelkmuh 02/28/2020 13:06:11
`@David Mansolino`  How much is it?

##### David Mansolino [Cyberbotics] 02/28/2020 13:19:24
May I ask you to send your request to support@cyberbotics.com (precising exactly what you need (e.g. real robot only, or both real and simulated, remote-control only or cross-compilation too, which version of the robot, etc.)), I am not able to answer you here directly.


May I ask you to send your request to support@cyberbotics.com .

##### elkelkmuh 02/28/2020 13:43:16
Ok  thank you

##### KyleM 02/28/2020 23:28:34
Howdy folks!


I'd really like to use webots + ros for my autonomous vehicle project


I built a custom Car PROTO and I can drive it around with a python controller using the Driver class


However...I can't figure out how to compile the ros\_automotive to connect it my some ros nodes


Is there any documentation or projects I can study to figure how to control my robot my ros?


Was able to solve my own problem by running 'make' on the main /projects folder. It doesn't build cleanly on 16.04 Ubuntu by the way


I had comment most of the child make(s) and just compile the vehicles folder

## March

##### David Mansolino [Cyberbotics] 03/02/2020 06:39:28
`@KyleM` Welcome!

for vehicle and ROS, there is a specific controller making the bridge between Webots and ROS1 it should work out of the box: [https://github.com/cyberbotics/webots/tree/master/projects/vehicles/controllers/ros\_automobile](https://github.com/cyberbotics/webots/tree/master/projects/vehicles/controllers/ros_automobile)

If you have issue recompiling Webots, here is the guide: [https://github.com/cyberbotics/webots/wiki/Linux-installation](https://github.com/cyberbotics/webots/wiki/Linux-installation)

(It should work for Ubuntu 16.04 as I am using it on a daily basis)

##### elkelkmuh 03/02/2020 07:11:20
`@David Mansolino`  I sent an email.

##### David Mansolino [Cyberbotics] 03/02/2020 07:11:56
Hi `@elkelkmuh` thank you I saw it, but haden't time to answer all my emails from the week-end yet, I will answer for sure today.

##### elkelkmuh 03/02/2020 13:20:34
Hi `@David Mansolino`   thank you for answer. I sent  an email again

##### rbhuva 03/12/2020 19:12:45
Hello!! Is it possible to measure the exact intensity of the point light source at the floor? I want to know it because I am changing the height of the fixed intense source from the floor!

##### David Mansolino [Cyberbotics] 03/13/2020 06:31:37
Hello `@rbhuva`, you might use a LightSensor node: [https://www.cyberbotics.com/doc/reference/lightsensor](https://www.cyberbotics.com/doc/reference/lightsensor)

##### taeyoung 03/18/2020 10:15:13
Hi! Can I put the pointlight or spotlight over the ball? I mean make the pointlight keep the height while it follows the ball. When I put the pointlight in the children of the ball with location 0 1 0,  as ball rolls the pointlight goes down and up.

##### David Mansolino [Cyberbotics] 03/18/2020 10:56:25
HI `@taeyoung`, this is unfortunately not possible out of the box, however, the solution is to use the Supervisor API, using the supervisor API you can at each step monitor the position of the ball and move the light accordingly.

Here is an example where a Supervisor moves a light at a predefined location: [https://cyberbotics.com/doc/guide/samples-devices#supervisor-wbt](https://cyberbotics.com/doc/guide/samples-devices#supervisor-wbt)

##### taeyoung 03/18/2020 12:14:23
Ok Thank you

##### David Mansolino [Cyberbotics] 03/18/2020 12:14:40
You're welcome

##### Jesusmd 03/23/2020 04:32:31
Hi, I need to read the LidarPoint properties with the CluodPointEnable, but I just have one list of object pointers, so my questions is how I can set all properties with LidarPoint class imported? I mean set all values of each point into a object made by LidarPoint class.

##### David Mansolino [Cyberbotics] 03/23/2020 06:19:27
HI `@Jesusmd` which lanugag are you using?

Why do you want to change the properties of the lidar points? These value should be seen are readonly value as they represent the measure of the sensor.

##### Jesusmd 03/23/2020 07:31:22
Hi, `@David Mansolino` I using python language. The idea is using the measure of Lidar to make an csv file that contains measures and corresponding angles. so I need to manipulate the data


`@David Mansolino` the global idea is make lidar to grid map

##### David Mansolino [Cyberbotics] 03/23/2020 07:37:30
In that case you should be able to make a loop and modify the LidarPoint objects, somthing like (untested):

```Python
for point in lidar.getPointCloud():
    # do something with the point
```

##### Jesusmd 03/23/2020 08:20:27
`@David Mansolino`  the loop is correct but for some reason my x,y and z values give 0,  I tried something like this : Lidar\_h\_r = Lidar1.getHorizontalResolution()

    print("the getHorizontalResolution is:", Lidar\_h\_r)

    Layer = Lidar1.getNumberOfLayers()

    Layer\_id = Lidar1.getLayerPointCloud(Layer)

    for i in range(Lidar\_h\_r):

        Point = Layer\_id[i]

        print("point:",Point.z,Point.y,Point.z)


`@David Mansolino`  ho my fault, I see it is points.x or some other proprieties, thanks!

##### David Mansolino [Cyberbotics] 03/23/2020 08:55:47
You're welcome 😉

##### mint 03/25/2020 12:28:12
Hello, I have been develping with webot and encountered a issue with setVelocity function for a node  in supervisor. The setVelocity function takes the angular velocity in form of rotation aroud respective x y z rotation, but my calculation for angular velocity would give the result in form of axis-angle (normal vector and the magnitude of rotation around it). I'm now troubled with converting w in axis-angle form to the x y z rotation form. Is there any formula, functions or library supported by webot?


Oh, its setVelocity function at supervisor

##### David Mansolino [Cyberbotics] 03/25/2020 12:31:30
Hi `@mint` unfortunately, Webots API does not provide such function. But I am sure you can find plenty of external libraries that do this. Which language are you using ?

##### mint 03/25/2020 12:32:24
I have been using python 3..

##### David Mansolino [Cyberbotics] 03/25/2020 12:33:50
In Python I personnally often use the ``transforms3d``module (available on PIP: [https://pypi.org/project/transforms3d](https://pypi.org/project/transforms3d)) which allows to convert between many 3D representations, you will probably find what you need in the doc: [http://matthew-brett.github.io/transforms3d/](http://matthew-brett.github.io/transforms3d/)

##### mint 03/25/2020 12:37:37
wow.. Thank you I couldn't find that even after hours of search. You saved a lot of time for me 🙂

##### David Mansolino [Cyberbotics] 03/25/2020 12:39:30
You're welcome 😉

##### Mistral 03/27/2020 01:40:47
Can we program and control the NAO robot in webots now?


I didn't find APIs

##### David Mansolino [Cyberbotics] 03/27/2020 06:21:02
Hi yes of course, in Webots you can use the regular Webots API to control the simulated robot, you will find some example controllers here: [https://github.com/cyberbotics/webots/tree/master/projects/robots/softbank/nao/controllers](https://github.com/cyberbotics/webots/tree/master/projects/robots/softbank/nao/controllers)

##### Mistral 03/27/2020 07:47:41
Thanks!😁

##### David Mansolino [Cyberbotics] 03/27/2020 07:49:44
You're welcome.

## April

##### angelicaba23 04/01/2020 17:12:59
Hey, does someone has a code for a light follower?

##### David Mansolino [Cyberbotics] 04/01/2020 17:18:23
Hi `@angelicaba23`, this sample show a very simple light follower: [https://cyberbotics.com/doc/guide/samples-devices#light\_sensor-wbt](https://cyberbotics.com/doc/guide/samples-devices#light_sensor-wbt)

##### angelicaba23 04/04/2020 16:23:50
> Hi `@angelicaba23`, this sample show a very simple light follower: [https://cyberbotics.com/doc/guide/samples-devices#light\_sensor-wbt](https://cyberbotics.com/doc/guide/samples-devices#light_sensor-wbt)

`@David Mansolino`  thanks!!


Hey, I hope you’re doing good!! Where can I find a tutorial to change the appearance of my robot?

##### DrVoodoo [Moderator] 04/04/2020 16:51:03
Shape or colour/texture?


If shape [https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot](https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot)

##### Sanket Khadse 04/09/2020 12:35:55
Hey, I want to use a ship mast in my simulation,  which basically has a tower like structure and a radar (dish),  which continuously moves around in a 90° angle.  

So the problem is,  I dont know how to make the part moving in the manner I need.  

There's another interesting problem,  that the whole mast should move in a sinusoidal manner (basically, the way it moves in the sea). How could these things be done?! 

Thank you!

##### David Mansolino [Cyberbotics] 04/09/2020 12:37:49
About the first issue, you mean you don't now how to make the radar moving ?


About the second issue, I would use the Supervisor API to move the whole mast: [https://cyberbotics.com/doc/reference/supervisor](https://cyberbotics.com/doc/reference/supervisor)

##### Sanket Khadse 04/09/2020 12:43:17
`@David Mansolino`,  in the first question, after I imported the model into webots,  how should I define the movement of that radar part as needed?

##### David Mansolino [Cyberbotics] 04/09/2020 12:48:38
You can either use the Supervisor API to change it's rotation field either mount the radar on a controllable joint.


Did you follow our tutorials? If not I would strongly advice to follow tutorials 1 to 6: [https://cyberbotics.com/doc/reference/supervisor](https://cyberbotics.com/doc/reference/supervisor)

##### Sanket Khadse 04/09/2020 12:49:57
Sure,  I will look into them and let you know if it helps! 😊


Hey, I tried importing a door hinge as a VRML97 file, which was converted from Solidworks model. 

The problem is, it is considering each part of the hinge, like screws, doors as separate individual. (As in the scene tree, it is showing each part differently with same name - "transform") 

Neither it is considered as other objects in the world's, because while I tried to click on the hinge, it won't click and show it's origin and all. 

What must be the problem here?

##### David Mansolino [Cyberbotics] 04/14/2020 05:21:51
Hi `@Sanket Khadse`, the VRML import does indeed import only the visual meshes of the object, you then have to recreate the structure of the object yourself. I would recommend to follow these tutorials to create an objet (1 to 7): [https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials)


Note also that a customizable door is already available in Webots: [https://cyberbotics.com/doc/guide/object-apartment-structure#door](https://cyberbotics.com/doc/guide/object-apartment-structure#door)

##### 🍎小苹果🍎 04/14/2020 15:06:31
hello! sorry for disturbing again. I don't know why but my simulation looks like lagging (I don't know if its correct word to explain but ) it's happens in some sample projects too I tried changing fps and time step but it didn't change too much thing in simulation.is there any other way to solve this ?

##### David Mansolino [Cyberbotics] 04/14/2020 15:08:50
Hi, you may try reducing the OpenGl features to speed up the simulation speed: [https://cyberbotics.com/doc/guide/preferences#opengl](https://cyberbotics.com/doc/guide/preferences#opengl)


You can also find some tips to speed up your simulation here: [https://cyberbotics.com/doc/guide/speed-performance](https://cyberbotics.com/doc/guide/speed-performance)

##### 🍎小苹果🍎 04/14/2020 15:09:24
okay thank you so much :)

##### David Mansolino [Cyberbotics] 04/14/2020 15:13:36
You're welcome

##### Aan 04/16/2020 07:07:35
Hi, I am trying to soccer ball object recognition using camera\_recognition.c in project samples. When I add soccerball, camera does not recognize the ball but it can recognize apple, can, oil barrel. How camera can recognize the soccer ball?

##### David Mansolino [Cyberbotics] 04/16/2020 07:09:05
Hi, this is because the recognition color of the soccer ball is not set, let me fix this.


Here is the  fix:

[https://github.com/cyberbotics/webots/pull/1548/files](https://github.com/cyberbotics/webots/pull/1548/files)

It will be available in the next release of Webots. But in the meantime, you might apply it locally to your Webots installation files.

##### Aan 04/16/2020 07:14:58
thank you 😀

##### David Mansolino [Cyberbotics] 04/16/2020 07:15:29
You're welcome

##### Dorteel 04/18/2020 05:48:34
Hi, I have a question regarding the Nao robot. I saw WeBots used to be able to interface with Choreographe, I was wondering if the motions for the Nao that are built-in in Choreographe can somehow be imported into WeBots?

##### Stefania Pedrazzi [Cyberbotics] 04/20/2020 06:14:54
`@Dorteel`, no there is no automatic procedure to import Choregraphe built-in motions in Webots. But if you have the motion joints values, then you should be able to reproduce it in Webots.

##### Dorteel 04/21/2020 09:13:59
Thank you `@Stefania Pedrazzi` ! 🙂

##### angelicaba23 04/22/2020 20:20:17
Any advice about how to get the epuck moves randomly in this world
%figure
![image0.jpg](https://cdn.discordapp.com/attachments/565155651395780609/702614779767947394/image0.jpg)
%end

##### David Mansolino [Cyberbotics] 04/23/2020 05:44:20
Hi `@angelicaba23` you might simply use the braitenberg controller.

##### fdvalois 04/23/2020 07:34:46
You just look braitenberg controller online?

##### David Mansolino [Cyberbotics] 04/23/2020 07:37:52
A breaitenberge already compatible with many robot is distributed with Webots, here is the code:  [https://github.com/cyberbotics/webots/tree/master/projects/default/controllers/braitenberg](https://github.com/cyberbotics/webots/tree/master/projects/default/controllers/braitenberg)

##### fdvalois 04/23/2020 07:38:11
Thank you!

##### David Mansolino [Cyberbotics] 04/23/2020 07:38:41
You're welcome

##### mint 04/26/2020 14:38:09
One question while developing.. what is the convention webot's using for the matrix returned by wb\_supervisor\_node\_get\_orientation function?

is it Euler Angles, Rotation Matrix, or Tait–Bryan angles?

##### Olivier Michel [Cyberbotics] 04/26/2020 14:43:52
It's a rotation matrix (as in OpenGL).


See [https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_get\_orientation](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_get_orientation)

##### mint 04/26/2020 14:54:05
Thank you Olivier!

##### Psyka 04/28/2020 13:51:04
Hey again,


I've got an other issue :


WARNING: DEF GROUND Solid > Shape > PBRAppearance > ImageTexture: 'textures/ground.jpg' not found.

A resource file can be defined relatively to the worlds directory of the current project, relatively to the worlds directory of the default project, relatively to its protos directory (if defined in a PROTO), or absolutely.


but the file is int the current directory of the current project


and it's not working

##### David Mansolino [Cyberbotics] 04/28/2020 13:52:11
Is it correctly in a ``textures``  folder?

##### Psyka 04/28/2020 13:52:43
textures


ground


copy pasted

##### David Mansolino [Cyberbotics] 04/28/2020 13:53:08
And where is it in your project directory exactly?

##### Psyka 04/28/2020 13:53:44
I've got a directory named "Created World" where there is all my .wbt worlds using this


and inside I've got a directory named "textures"


wbt directory


haaa sorry I'm loosing it


so all my project are in E:\...\...\Project\wbt\


there I've got a directory "controllers" for controllers and "Created World" with in it "textures" and then the "ground.jpg"


Create\_Webots\_World\wbt\Created\_World\textures\ground.jpg

##### David Mansolino [Cyberbotics] 04/28/2020 13:59:47
Ok, this is the problem, the folder in which are the world can't be named 'Created World'.

You should have:

```
Create_Webots_World\wbt\
                     -> controllers
                     -> worlds
                           -> textures
                                    -> ground.jpg
```

##### Psyka 04/28/2020 13:59:59
exactly


and all my .wbt are in worlds


hooo why that can't it be ?

##### David Mansolino [Cyberbotics] 04/28/2020 14:00:36
But that's not what you said:

> Create\_Webots\_World\wbt\Created\_World\textures\ground.jpg

##### Psyka 04/28/2020 14:00:46
yes


Create\_Webots\_World\wbt\Created\_World\textures\ground.jpg


is a copy past of my path

##### David Mansolino [Cyberbotics] 04/28/2020 14:01:36
because the names in the project folder should respect a strict convention (you can name the project folder as you want but not the folders inside):

   [https://cyberbotics.com/doc/guide/the-standard-file-hierarchy-of-a-project](https://cyberbotics.com/doc/guide/the-standard-file-hierarchy-of-a-project)

##### Psyka 04/28/2020 14:03:05
it's "Created\_World" and not "Created World" I just writte it wrong

##### David Mansolino [Cyberbotics] 04/28/2020 14:03:18
It's still problematic 😉

##### Psyka 04/28/2020 14:03:45
it was working fine like last week


how should I name that directory ?


worlds


?


ho ok

##### David Mansolino [Cyberbotics] 04/28/2020 14:04:24
yes indeed

##### Psyka 04/28/2020 14:04:33
I understand now what you mean


so I just have to rename it ?

##### David Mansolino [Cyberbotics] 04/28/2020 14:04:40
You should have:

> Create\_Webots\_World\wbt\worlds\textures\ground.jpg

##### Psyka 04/28/2020 14:06:34
very nice thank's 🙂


it working fine

##### David Mansolino [Cyberbotics] 04/28/2020 14:09:41
You're welcome

##### Shubham Dahatre 04/29/2020 22:30:02
I am trying to create some solid using transform and shapes.

I am not able to set the objects at desired angle.

How should I set the parameters if I need specific angle.

Or is their any simple way to convert normal vector direction to euler angles

##### Luftwaffel [Moderator] 04/29/2020 22:31:07
[https://www.andre-gaschler.com/rotationconverter/](https://www.andre-gaschler.com/rotationconverter/)

## May

##### Jesusmd 05/10/2020 06:29:26
Is there an quickly way to get the attribute instead of  predefined values as for example in  Keyboard class.?  I would preffer enum as in c or c++

##### David Mansolino [Cyberbotics] 05/11/2020 06:43:54
> Is there an quickly way to get the attribute instead of  predefined values as for example in  Keyboard class.?  I would preffer enum as in c or c++

`@Jesusmd` which language are you using?

##### Luftwaffel [Moderator] 05/13/2020 16:02:55
<@&568329906048598039> Btw, I'm currently working on a python program, that publishes the PoseStamped of one or more nodes into a rostopic. It also allows to publish the Pose relative to a specific node.


Still a bit crude with little error correction, but it works 🙂


Btw, is there a way to get the quaternion orientation directly from webots? That would make things much simpler

##### Olivier Michel [Cyberbotics] 05/13/2020 16:08:03
No, you have to get it as an axis-angle representation, but that's super easy to translate into quaternion.


```python
def axis_angle_to_quaternion(axis, theta):
    axis = numpy.array(axis) / numpy.linalg.norm(axis)
    return numpy.append([numpy.cos(theta/2)], numpy.sin(theta/2) * axis)
```

##### Luftwaffel [Moderator] 05/13/2020 16:09:36
how do we get axis angles? I have the 3x3 matrix

##### Olivier Michel [Cyberbotics] 05/13/2020 16:10:25
[https://www.cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_field\_get\_sf\_rotation](https://www.cyberbotics.com/doc/reference/supervisor#wb_supervisor_field_get_sf_rotation)


(gives relative rotation in axis-angle notation)


If you need absolute rotation (in axis-angle notation), use:  [https://www.cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_get\_orientation](https://www.cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_get_orientation)

##### Luftwaffel [Moderator] 05/13/2020 16:12:12
but that returns the 3x3 matrix

##### Olivier Michel [Cyberbotics] 05/13/2020 16:12:56
Yes, you are right. Sorry, my bad.


OK, so you need to convert this 3x3 rotation matrix to a quaternion then.

##### Luftwaffel [Moderator] 05/13/2020 16:14:33
The thing is, I want it to be able to run in the native webots environment, so no extra packages. I got it to work using their code:


[https://nipy.org/nibabel/reference/nibabel.quaternions.html](https://nipy.org/nibabel/reference/nibabel.quaternions.html)


specifically this:


[https://sscc.nimh.nih.gov/pub/dist/bin/linux\_gcc32/meica.libs/nibabel/quaternions.py](https://sscc.nimh.nih.gov/pub/dist/bin/linux_gcc32/meica.libs/nibabel/quaternions.py)


Problem is, the link to their liscense is broken 😅

##### Olivier Michel [Cyberbotics] 05/13/2020 16:15:26
Oops...

##### Luftwaffel [Moderator] 05/13/2020 16:16:25
how big of a deal would it be to add the 'tf' package to the webots ROS environment?


it is almost a must if working with ROS and robots

##### Olivier Michel [Cyberbotics] 05/13/2020 16:18:13
If you use Webots with ROS, you need to install ROS, and you should get 'tf' for free, isn't it?

##### Luftwaffel [Moderator] 05/13/2020 16:19:08
yes, but it requires runtime.ini edits. Would be nice if people can have it run as an example out of the box

##### Olivier Michel [Cyberbotics] 05/13/2020 16:20:18
Yes, if you have an idea to achieve this... Now you know how to open a pull request 😉

##### Luftwaffel [Moderator] 05/13/2020 16:20:54
oh lordy 😅


I'll give it a try

##### Sanket Khadse 05/13/2020 16:43:42
Hey, could you tell me how to multi-select things in scene-tree? 

I can't find a way to. I imported a VRML97 model into Webots, the model was quite large, and it's difficult to select, cut and paste each 'transform' object into a robot node's children attribute.

##### Olivier Michel [Cyberbotics] 05/13/2020 16:44:11
Unfortunately, this is not possible.

##### Sanket Khadse 05/13/2020 16:45:26
Oh, thank you for letting me know!

Keep it as a suggestion for the next update. 😄

##### Luftwaffel [Moderator] 05/13/2020 16:47:12
Perhaps add a 'Group' base node, put all your nodes in, and copy paste that

##### Sanket Khadse 05/13/2020 17:11:36
<@239507682111913984> that is what my problem is about. The nodes I have to copy paste one by one are in "hundreds".

##### Luftwaffel [Moderator] 05/13/2020 18:04:16
`@Sanket Khadse`  perhaps direct .proto file edit can help

##### David Mansolino [Cyberbotics] 05/14/2020 05:34:23
`@Luftwaffel` instead of relying on ROS, if you are using Python you ca probably use the `transforms3d` python package which allows for example to convert from a rotation matrix to quaternions: [https://matthew-brett.github.io/transforms3d/reference/transforms3d.quaternions.html#transforms3d.quaternions.mat2quat](https://matthew-brett.github.io/transforms3d/reference/transforms3d.quaternions.html#transforms3d.quaternions.mat2quat)

##### PymZoR [Premier Service] 05/14/2020 09:08:34
`@Luftwaffel` Although working with ROS, we decided to split the simulation projects with minimal dependencies over ROS other than communication. For 3D Math in Cpp, we're using Eigen, and in python either numpy or numpy + transformations.py (which is standalone of tf)


Regarding your example of getting relative position between two nodes, that can be easily achieved with Eigen in cpp (3x3 Matrix -> Quaternion, quaternion / vector product)


The integration with ROS is super easy too with [http://wiki.ros.org/eigen\_conversions](http://wiki.ros.org/eigen_conversions) 🙂

##### Luftwaffel [Moderator] 05/14/2020 12:48:46
> `@Luftwaffel` Although working with ROS, we decided to split the simulation projects with minimal dependencies over ROS other than communication. For 3D Math in Cpp, we're using Eigen, and in python either numpy or numpy + transformations.py (which is standalone of tf)

`@PymZoR` 



Awesome, thank you so much. That makes things easier

##### Jesusmd 05/15/2020 01:49:44
`@David Mansolino` Hi, I am using python, I would like to work with several distance sensors at the same time and comparing their data in console. But instead of obtain the type, I got a number.

##### Sanket Khadse 05/15/2020 03:02:49
Hey, may I know, why does a drone, (let it be DJI Mavic 2 Pro, which is already available into Webots, or a custom made) moves up and down and shifts constantly in one direction even if no such behaviour is defined in the controller code?

##### David Mansolino [Cyberbotics] 05/15/2020 04:55:43
`@Jesusmd` you want to get the type of distance sensor? If so you should use the ``getType`` function: [https://cyberbotics.com/doc/reference/distancesensor?tab-language=python#wb\_distance\_sensor\_get\_type](https://cyberbotics.com/doc/reference/distancesensor?tab-language=python#wb_distance_sensor_get_type)


`@Sanket Khadse` the controller of the drone is very simple, it doesn't do any feedback using the GPS position or any inertial unit, it might therefore easily drift.

##### Alfian 05/21/2020 04:09:02
There is no controller for lock position of the DJI Mavic 2 Pro

##### ChapoGuzman 05/21/2020 23:41:00
Hi guys im using the urdf package to convert my files between solidworks and webots


But i have this error



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/713174651731181749/unknown.png)
%end


I did a good install because I tried the "Human" file and it worked correctly


Thanks in advance

##### Olivier Michel [Cyberbotics] 05/22/2020 06:48:43
Please open an issue on [https://github.com/cyberbotics/urdf2webots/issues/new](https://github.com/cyberbotics/urdf2webots/issues/new) and upload your URDF file as a ZIP file.

##### Sanket Khadse 05/22/2020 13:18:06
Hey, I was trying to simulate a world on a HTTP web page. But I couldn't link the controller to the world. I can only move the viewpoint. How should I link the controllers to that file? So that the robots in my simulation should work.

##### Olivier Michel [Cyberbotics] 05/22/2020 14:27:44
If your robots move in Webots, they should also move in the web view. Don't they?

##### Sanket Khadse 05/22/2020 15:28:54
`@Olivier Michel` you see, no other option or window opens. neither the drone moves. But in webots, it moves. Any suggestions?
%figure
![Screenshot_from_2020-05-22_20-57-46.png](https://cdn.discordapp.com/attachments/565155651395780609/713413087914491904/Screenshot_from_2020-05-22_20-57-46.png)
%end

##### Olivier Michel [Cyberbotics] 05/22/2020 15:31:32
That's because you exported to HTML but you are not streaming the simulation.


Do you want to export an animation or to stream a live simulation?


Please refer to the user guide: [https://cyberbotics.com/doc/guide/web-interface](https://cyberbotics.com/doc/guide/web-interface)

##### Sanket Khadse 05/22/2020 15:37:09
Yes, I have already read it. I need to simulate my world on a web browser using HTML file. I mean anyone with the file can open the simulation on the web. Server streaming or already recorded animation isn't the case.

##### Olivier Michel [Cyberbotics] 05/22/2020 15:41:18
Exporting a to HTML will simply export a static scene (no motion), so you probably want to either export an animation or stream a live simulation?

##### Sanket Khadse 05/22/2020 15:45:40
So could you tell me how can I make this static scene to moving? (by using my controller code of course).

No, as I said, its none of these two cases.

##### Olivier Michel [Cyberbotics] 05/22/2020 15:46:11
This is not possible.

##### Sanket Khadse 05/22/2020 15:50:50
`@Olivier Michel` , would you please take a look at this. So it will be easy to help me out.

[http://www.aerialroboticscompetition.org/assets/downloads/simulation\_challenge\_rules\_1.1.pdf](http://www.aerialroboticscompetition.org/assets/downloads/simulation_challenge_rules_1.1.pdf)

Go for " Challenge Rules - 1 ".

##### Olivier Michel [Cyberbotics] 05/22/2020 16:00:06
Are you a competitor or an organizer/developer of this contest?

##### Sanket Khadse 05/22/2020 16:01:57
I am a competitor, sir.

##### Olivier Michel [Cyberbotics] 05/22/2020 16:02:54
OK, then you should probably run Webots in the cloud and stream your simulation to the web.

##### Sanket Khadse 05/22/2020 16:03:44
Alright, thank you for your understanding!

##### hrsh12 05/24/2020 21:21:56
Is there a way of modifying the usual steering system used in car models?

##### David Mansolino [Cyberbotics] 05/25/2020 06:25:16
You can either convert your car node to base node (right click on the node in the scene-tree) and then you will be able to change the structure, either change directly the `Car.proto` file.

##### PymZoR [Premier Service] 05/27/2020 16:42:51
I ported the Atom PROTO extension to VSCode : [https://marketplace.visualstudio.com/items?itemName=pymzor.language-proto-webots](https://marketplace.visualstudio.com/items?itemName=pymzor.language-proto-webots)


Review / PR / issues are welcome 🙂

##### Clara Cardoso Ferreira 05/27/2020 18:21:49
[https://pgda.gsfc.nasa.gov/products/54](https://pgda.gsfc.nasa.gov/products/54)

Im trying to retrieve height maps from the above data and import it to Webots to create a custom terrain. I am not sure if this is possible and easy to do. Please provide me some guidance.



I was thinking float img files formats would work best since I could import it to Opend CV if necessary.

##### David Mansolino [Cyberbotics] 05/28/2020 05:24:05
One possible solution would be to create a procedural PROTO ([https://www.cyberbotics.com/doc/reference/procedural-proto-nodes](https://www.cyberbotics.com/doc/reference/procedural-proto-nodes)) which will read the float image and generate an ElevationGrid ([https://www.cyberbotics.com/doc/reference/elevationgrid](https://www.cyberbotics.com/doc/reference/elevationgrid)) from it.


You will find an example of this here: [https://gist.github.com/DavidMansolino/46d7e0df4c48bbaac4611b3872878347](https://gist.github.com/DavidMansolino/46d7e0df4c48bbaac4611b3872878347)

##### AngelAyala 05/28/2020 06:15:18
Is there anyway to catch the runtime errors in python in order to stop and reset the simulation? I can control the execution of the simulation with Supervisor node

##### David Mansolino [Cyberbotics] 05/28/2020 06:18:04
you can always use a  `try` and `except` on the sensitive part

##### AngelAyala 05/28/2020 06:18:55
I'm currently using, but when appears this error Error: wb\_gps\_enable(): invalid device tag, the code is unable to catch it


is maybe a especial ExceptionError?

##### David Mansolino [Cyberbotics] 05/28/2020 06:19:43
this s indeed just a warning which doesnt  raise any exception


but you can simply check the return value

##### AngelAyala 05/28/2020 06:20:33
oh greats, thanks

##### David Mansolino [Cyberbotics] 05/28/2020 06:20:42
You're welcome

##### AngelAyala 05/28/2020 06:50:43
now, how can I access to the GPS node inside the Mavic2Pro PROTO using the supervisor node?


is there anyway to do this?


using python

##### Stefania Pedrazzi [Cyberbotics] 05/28/2020 06:52:23
Is the Supervisor controller different from the Mavic2Pro controller?

##### AngelAyala 05/28/2020 06:52:31
yes

##### Stefania Pedrazzi [Cyberbotics] 05/28/2020 06:52:49
if yes, then you cannot read the Mavic2Pro GPS device values from another robot/supervisor.


This is like in real context.


What you can do is to send the GPS data from the Mavic2Pro controller to the supervisor controller using for example the Emitter/Receiver devices

##### AngelAyala 05/28/2020 06:54:21
ok I get it, both running in different controllers


I wasn't considering as real context, thanks

##### Clara Cardoso Ferreira 05/28/2020 21:34:42
> One possible solution would be to create a procedural PROTO ([https://www.cyberbotics.com/doc/reference/procedural-proto-nodes](https://www.cyberbotics.com/doc/reference/procedural-proto-nodes)) which will read the float image and generate an ElevationGrid ([https://www.cyberbotics.com/doc/reference/elevationgrid](https://www.cyberbotics.com/doc/reference/elevationgrid)) from it.

`@David Mansolino`


My guess would be that his solution would work, but it would probably be extremely slow since you would have to generate each individual slide of the terrain at every render pass

##### Iguins 05/28/2020 22:10:01
I was facing that same problem

##### David Mansolino [Cyberbotics] 05/29/2020 05:03:20
Will the terrain change at each step ?

##### Clara Cardoso Ferreira 05/29/2020 05:04:20
For my case, it changes every meter. I am trying to import the map of the moon to Webots


I could import the whole moon, but I plan to have at least sections of the moon


so that my robot can learn how to navigate in unknown terrain


with real data

##### David Mansolino [Cyberbotics] 05/29/2020 05:06:52
For the whole moon, it is indeed going to be slow, but if you import only a reasonable sub-section, the performance should be ok. See this example: [https://cyberbotics.com/doc/automobile/ch-vens](https://cyberbotics.com/doc/automobile/ch-vens)

##### Clara Cardoso Ferreira 05/29/2020 05:20:52
Ill give it a try, thank you David!

##### Iguins 05/29/2020 05:31:28
I wanted the robots to modify an uneven terrain

##### David Mansolino [Cyberbotics] 05/29/2020 05:32:59
In that case the robots should be Supervisors and they will be able to change the height values.

##### Iguins 05/29/2020 05:33:39
nice thank you!

##### David Mansolino [Cyberbotics] 05/29/2020 05:33:58
You're welcome

##### Iguins 05/29/2020 05:35:32
do you think that instead of a heightmap I could use a custom mesh to also change the x and z values?

##### David Mansolino [Cyberbotics] 05/29/2020 05:36:28
You can, but heightmap is much more efficient and stable than random indexedFaceSet.

##### Iguins 05/29/2020 05:36:59
I was afraid of that


thank you

##### Wesst 05/30/2020 16:24:35
does anyone have implemented some kind of pathfinding and can help me ? pls dm

##### Ans 05/30/2020 20:39:57
Hi! Webots noob here, could someone tell me how to add an odor plume in the simulation and make the robot follow it? (Kinda trying to recreate this : [https://en.wikibooks.org/wiki/Webots\_Odor\_Simulation](https://en.wikibooks.org/wiki/Webots_Odor_Simulation))

##### Deleted User 05/31/2020 21:22:36
How can i run simulator using Qt Creator?


is there any depth camera that i can use for skeletal tracking?

## June

##### David Mansolino [Cyberbotics] 06/02/2020 05:51:17
> does anyone have implemented some kind of pathfinding and can help me ? pls dm

`@Wesst` you will find some examples here: [https://en.wikibooks.org/wiki/Cyberbotics%27\_Robot\_Curriculum/Advanced\_Programming\_Exercises#Path\_planning\_](https://en.wikibooks.org/wiki/Cyberbotics%27_Robot_Curriculum/Advanced_Programming_Exercises#Path_planning_)[Advanced]


> Hi! Webots noob here, could someone tell me how to add an odor plume in the simulation and make the robot follow it? (Kinda trying to recreate this : [https://en.wikibooks.org/wiki/Webots\_Odor\_Simulation](https://en.wikibooks.org/wiki/Webots_Odor_Simulation))

`@Ans` this is simply emulated from a Supervisor controller that imports shapes to visualize the plume.


The supervisor also send plume information to every robot using emitter-receivers.


> How can i run simulator using Qt Creator?

`@Deleted User` do you want to start Webots from Qt Creator, or use Qt in your controller?


> is there any depth camera that i can use for skeletal tracking?

`@Deleted User` yes of course, you can use range-finders: [https://www.cyberbotics.com/doc/reference/rangefinder](https://www.cyberbotics.com/doc/reference/rangefinder)

##### lojik 06/02/2020 10:48:45
Hello everyone, I do not know if someone else have the same problem as me but it is easy to verify.



I am on ubuntu 18.04, using webots R2020a-revision1. When I right-click to add new features --> search in the 'find' box and type an underscore '\_' Webots crashes.



Do someone has the same issue?

##### David Mansolino [Cyberbotics] 06/02/2020 10:50:28
Hi, yes this is indeed a known bug, it was fixed very recently, you can already download a beta of R2020a-revision2 which incudes the fix here: [https://github.com/cyberbotics/webots/releases](https://github.com/cyberbotics/webots/releases)

##### lojik 06/02/2020 10:51:08
Perfect, thank you 👍

##### David Mansolino [Cyberbotics] 06/02/2020 10:51:52
You're welcome

##### hrsh12 06/04/2020 21:23:54
Hello everyone, could someone tell me if we can implement a rack and pinion system for steering in Car. For the jointsI'm trying to recreate a  steer-by-wire system. Thanks

##### David Mansolino [Cyberbotics] 06/05/2020 05:15:24
Hi `@hrsh12`, this is not possible directly, however you can combine several base joint to make such system.


We plan to add a new feature which will allow to link joint together and which might help you doing this ([https://github.com/cyberbotics/webots/issues/1365](https://github.com/cyberbotics/webots/issues/1365)).

##### hrsh12 06/09/2020 11:33:40
Hi, `@David Mansolino` , To make a rack and pinion arrangement, i require more kind of joints than those that are available in the documentation. Can i model my own basic joints?

##### David Mansolino [Cyberbotics] 06/09/2020 11:35:20
You can either use physics plugins: 

[https://cyberbotics.com/doc/reference/physics-plugin](https://cyberbotics.com/doc/reference/physics-plugin)

Either extend Webots with new nodes:

[https://github.com/cyberbotics/webots/blob/master/CONTRIBUTING.md](https://github.com/cyberbotics/webots/blob/master/CONTRIBUTING.md)

##### lojik 06/10/2020 16:46:38
Hello there, I imported a 3d object in webots by using webots 2020b. It happen frequently that an object is suddently not takent into account with contacts. It seems that there is some problems with the bounding object.



Could it be a bug on this nightly version or is it a problem on my side?



%figure
![Screenshot_from_2020-06-10_18-47-22.png](https://cdn.discordapp.com/attachments/565155651395780609/720318368837533766/Screenshot_from_2020-06-10_18-47-22.png)
%end


Here you have the kind of objects I imported. These are kind of sinus waves. I put a bit of each pieces down the floor to have smaller mountains.

##### David Mansolino [Cyberbotics] 06/11/2020 04:57:15
Hi `@lojik` what is exactly the problem you have with the bounding object,?We can't see them on this picture.

##### lojik 06/11/2020 08:37:32
Hi `@David Mansolino` sorry, I did not explain well...  My problem is that I actually use the objects shown in the previous picture. With the bounding object as their own shape. But when my robot has to climb them, sometimes the wheel go through the object instead of climb it. It seems that the bounding object is not here.



I actually use webots nightls R2020b 9\_6\_2020



%figure
![Screenshot_from_2020-06-11_10-34-54.png](https://cdn.discordapp.com/attachments/565155651395780609/720557335977787433/Screenshot_from_2020-06-11_10-34-54.png)
%end


As seen on this picture, the wheel on the left is inside the object and the bounding object (in white) should be in red if it is well detected by the wheel.



On the right, the wheel is correctly on top of the object.


And this effect is worst when I would like to record a video. The object are never well detected.


I just try now to begin recording the video after that the robot well detected my objects. In that case it works fine, but I have to play simulation until the robot detects my objects and then begin to record video.

##### David Mansolino [Cyberbotics] 06/11/2020 08:44:01
This is because the bounding object created automatically when imported are based on the actual mesh, this is known to be not very accurate and stable, for better collisions, you should re-create the key bounding objects using sets of basic geometries (e.g. boxes, spheres, etc.)

##### lojik 06/11/2020 08:45:02
Would it be better if I have a finner mesh? Or worst?


Because I would like to import custom models to have more freedom on the form of obstacles I have. It would not be a good deal if I have to redo the bounding object after importing them..

##### David Mansolino [Cyberbotics] 06/11/2020 08:46:16
it all depends on the complexity of the mesh, but there is a tradeoff (which is not always easy to find).

But maybe a simpler solution for recording the video is to decrease the WeorldInfo.timestep.

##### lojik 06/11/2020 08:47:57
I work with blender to do these meshes. My goal is to have well designed unflat terrain to benchmark my robot in that condition.


I already have a basicTimeStep set to 1ms 😅

##### David Mansolino [Cyberbotics] 06/11/2020 08:51:10
I am sorry but there is no magic solution for this, using meshes as bounding object is not the ideal solution.

##### lojik 06/11/2020 08:58:23
If I understand correctly it is actually not possible to create more complexe shapes with their own bounding object? If I would like to have a ramp which look like an ellipsoid more than a circle. I will always have to approximate the bounding object with a circle or take time too find a good threshold with the mesh to make it works?

##### David Mansolino [Cyberbotics] 06/11/2020 09:00:04
> I will always have to approximate the bounding object with a circle

Not exaclty, you can use several geometries to approximate the shape (such as for example several circles to approximate an ellipsoid).

##### lojik 06/11/2020 09:02:53
Ok, well, I did not expected that. I have some good results with my meshes. But from one simulation to an other one the results are not the same so I was wondering where it was wrong.



Thank you for your answer, I will have a deeper look to find a good solution between approximation geometries and find a good mesh.

##### David Mansolino [Cyberbotics] 06/11/2020 09:03:22
You're welcome

##### lojik 06/11/2020 10:33:03
Ok, so after playing a bit with my meshes I have a first intuition. I realize that your cylinders are composed with only parallel lines. So I did the same in blender for my sinusoid ground and it seems to work much better. Now it is detected every time instead of 1/3 run.



Unfortunately it will keep to have triangles by exportations. But since there is a huge number of parallels in  my mesh, it works fine now.



I do not know if I explain well enough .. Here is my final mesh :



%figure
![Screenshot_from_2020-06-11_12-32-27.png](https://cdn.discordapp.com/attachments/565155651395780609/720586430224793600/Screenshot_from_2020-06-11_12-32-27.png)
%end


PS: The first run after importing such an object will provoc a webots' crash.

##### David Mansolino [Cyberbotics] 06/11/2020 11:45:25
The mesh looks indeed to be very clean like this. Just for the note, for the cylinder, the parallel lines are just for visualization, for the physics it is a perfect cylinder.

##### lojik 06/11/2020 11:48:58
Ok thank you, so my intuition is not right, but it works much better with this mesh 👍

##### David Mansolino [Cyberbotics] 06/11/2020 11:51:11
OK, it make sense, for we are using similar meshes to create roads and it works quite well. Note that you might be interested by the road PROTO as it allows to create similar meshes (when varying the height of the road): [https://cyberbotics.com/doc/guide/object-road](https://cyberbotics.com/doc/guide/object-road)

##### lojik 06/11/2020 11:59:14
Yes, exactly, you use quite the same idea behind the road PROTO. So I will keep in mind this way to create meshes. Thank you !


I also see that uneven terrain seems to use a symetric grid mesh as a bounding object, is it right?

##### David Mansolino [Cyberbotics] 06/11/2020 12:01:48
Uneven terrai is using an EleveationGrid as mesh, this mesh is way more stable/efficient than IndexedFaceSet for collision but has more constraints ([https://www.cyberbotics.com/doc/reference/elevationgrid](https://www.cyberbotics.com/doc/reference/elevationgrid))

##### lojik 06/11/2020 12:29:35
Okay, thank's 👍

##### webotspro9999 06/20/2020 22:52:55
hello all


I'm new in Webots and I want to develop speech recognition applications using Softbank Robotics NAO. Is this feature available in Webots?

##### David Mansolino [Cyberbotics] 06/22/2020 05:27:14
hello `@webotspro9999`, this is unfortunately not available out of the box, you will have to implement your own speach recognition algorithm or use a library implementign this.

##### aalmanso 06/22/2020 15:36:42
Hi everyone,

I want to place E-puck randomly inside arena every time I start or reload the simulation, I need that to test some behaviour. So, i wander how can I find the help about using random numbers and 2D coordinates in webots simulator?

##### Olivier Michel [Cyberbotics] 06/22/2020 15:38:09
You can simply write a supervisor controller that will move the robot to a random position at the beginning of the simulation.

##### webotspro9999 06/22/2020 22:02:26
> hello `@webotspro9999`, this is unfortunately not available out of the box, you will have to implement your own speach recognition algorithm or use a library implementign this.

`@David Mansolino` thanks for your reply, does the NAO in Webots have sound listening functions? For example, when I say hello can the NAO hear this and take it as an input data?

##### David Mansolino [Cyberbotics] 06/23/2020 05:01:36
No, Webots does not simulate microphones. But you micreate an interface to get the audio from your computer michrophone.

##### black\_hammer\_67 06/23/2020 16:33:46
Can somebody help me to figure out how to reset the simulation without having to stop the controller, actually I want every time a condition is true in the controller to reset the robots initial position only, not the simulation time

##### Clara Cardoso Ferreira 06/23/2020 23:23:31
I think so`@black_hammer_67` , check the supervisor API and look for position


Does anyone know if it's possible to reset the terrain in the proto for every simulation and set up the robot's wheels to touch the ground to start the simulation?


Also, what is the benefit of using the supervisor vs a sensor (like the GPS) to get states such as position?

##### Andrei 06/23/2020 23:34:32
I guess it depends what you want to do. I wanna make a pathfinder so I wanna know the whole map when I determine the path I take not just what sensors can give me.

##### David Mansolino [Cyberbotics] 06/24/2020 05:38:33
> Can somebody help me to figure out how to reset the simulation without having to stop the controller, actually I want every time a condition is true in the controller to reset the robots initial position only, not the simulation time

`@black_hammer_67` yes, you can use the `wb_supervisor_simulation_reset` Supervisor function: [https://www.cyberbotics.com/doc/reference/supervisor?tab-language=c++#wb\_supervisor\_simulation\_reset](https://www.cyberbotics.com/doc/reference/supervisor?tab-language=c++#wb_supervisor_simulation_reset)


> Does anyone know if it's possible to reset the terrain in the proto for every simulation and set up the robot's wheels to touch the ground to start the simulation?

`@Clara Cardoso Ferreira` to reset the terrain from the proto file is indeed possible, unfortunately it is not possible to move the robot from there, but you can move it then at the first step of the simulation with a Supervisor controller.


> Also, what is the benefit of using the supervisor vs a sensor (like the GPS) to get states such as position?

`@Clara Cardoso Ferreira` no benefit, if you are trying to get your own position, GPS is the way to go, if you are trying to get the position of another node supervisor is the way to go.

##### black\_hammer\_67 06/24/2020 13:59:32
hello, is there a way to detect robot colisions in the suppervisor mode ?

##### David Mansolino [Cyberbotics] 06/24/2020 14:00:20
Yes, you can get the number and location of the contact points of a node:  [https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_get\_contact\_point](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_get_contact_point)

##### black\_hammer\_67 06/24/2020 14:01:48
ok so if I have a pioneer 3dx robot the default contact points are 3 , these that the wheels have with the floor and if I detect 4 or more points I can assume this is a colision right ?

##### David Mansolino [Cyberbotics] 06/24/2020 14:02:59
That's unfortunately not so simple, the number of contact points of the wheel can vary, you should rather check the location of the contact points.

##### black\_hammer\_67 06/24/2020 14:04:12
ok i see, so I am looking for points that they have a certain value on y axis > 0.1 for example ?

##### David Mansolino [Cyberbotics] 06/24/2020 14:04:52
Yes, that sounds good.

##### black\_hammer\_67 06/24/2020 14:06:02
thank you clarifying that 😁

##### David Mansolino [Cyberbotics] 06/24/2020 14:06:16
You're welcome 😉

##### Clara Cardoso Ferreira 06/24/2020 19:00:41
> `@Clara Cardoso Ferreira` to reset the terrain from the proto file is indeed possible, unfortunately it is not possible to move the robot from there, but you can move it then at the first step of the simulation with a Supervisor controller.

`@David Mansolino` 



Your answer somewhat helps. I understand I can reset the terrain, but would I have to reset it manually or is there a way to make a loop so that a different terrain is used in every simulation iteration? I imagine I would have to connect the supervisor code to the proto somehow



Another big question, I forgot a simulation running overnight on my computer and the next morning, it would not turn on. It may have been a coincidence that my computer failed even though it is under a year of use. Would you have any suggestions of what may have gone wrong?

##### David Mansolino [Cyberbotics] 06/25/2020 04:53:53
You can do this directly from the proto itself, you simply have to set the 'randomSeed' field to 0 as explained here: [https://www.cyberbotics.com/doc/guide/object-floors#uneventerrain-field-summary](https://www.cyberbotics.com/doc/guide/object-floors#uneventerrain-field-summary)


About your other question, was your controller writing to a file? Would it be possibl that it saturated the hard drive?

##### Clara Cardoso Ferreira 06/29/2020 15:04:28
Thanks I'll try that when I get my computer back.

My controller was not writing anything at the moment it crashed, but it was probably storing data. Yeah, it may have been the harddrive

##### Marian 06/30/2020 09:29:18
Hi, I'm trying to create some simple cylinders in WeBots. It works but unfortunatly it only creates a wireframe but no texture. What is missing here?
%figure
![webots_cylinderr.png](https://cdn.discordapp.com/attachments/565155651395780609/727455716390076437/webots_cylinderr.png)
%end

##### lojik 06/30/2020 09:32:56
Your "shape" node should be in the "children" one and not "boundingObject". Then you can select it in "boundingObject" because bounding object is just the virtual shape taken into account when performing contact simulation. It is not the visual rendering of the object.

##### Marian 06/30/2020 09:45:52
Ok, thanks a lot for the explanation. It works!

## July

##### Clara Cardoso Ferreira 07/03/2020 23:17:42
> Thanks I'll try that when I get my computer back.

> My controller was not writing anything at the moment it crashed, but it was probably storing data. Yeah, it may have been the harddrive



I got my computer back and it was in fact a hardrive issue. It was just a coincidence that I was running an extensive Webots  simulation

Thanks for the tips

##### henry10210 07/04/2020 16:38:53
I set the WEBOTS\_HOME to the root of the source code folder (where I ran the ./webots command).  I wonder if this is an incorrect WEBOTS\_HOME


Is it possible to use just python3 with webots?  Could not build the python2 language binding due to my libpython2 missing symbols, and since ROS2 is all python3, figured I would not need it.  But finding python2 dependenceis in webots\_ros2 package

##### Luftwaffel [Moderator] 07/05/2020 09:34:48
`@henry10210` just select extern as the controller and run your controller outside in your own environment

##### starstuff\_0903 07/05/2020 10:26:14
i need to create a 3d environment from a 2d image..Is it possible

##### mint 07/06/2020 04:59:09
Hello, is there any ways of writing a proto file  from an external design tool like CAD? What  kind of language does it belong to?


nevermind, I just  figured out that it is VRML97. my bad XD

##### David Mansolino [Cyberbotics] 07/06/2020 05:39:15
> I got my computer back and it was in fact a hardrive issue. It was just a coincidence that I was running an extensive Webots  simulation

> Thanks for the tips

`@Clara Cardoso Ferreira` you're welcome! thank you for the feedback!


> I set the WEBOTS\_HOME to the root of the source code folder (where I ran the ./webots command).  I wonder if this is an incorrect WEBOTS\_HOME

`@henry10210` yes this is correct.


> Is it possible to use just python3 with webots?  Could not build the python2 language binding due to my libpython2 missing symbols, and since ROS2 is all python3, figured I would not need it.  But finding python2 dependenceis in webots\_ros2 package

`@henry10210` which python2 dependencies do you see in the webots\_ros2 package?


> i need to create a 3d environment from a 2d image..Is it possible

`@starstuff_0903` can you tell us more, what does represent your image? The height of the terrain ?

##### Luftwaffel [Moderator] 07/06/2020 14:32:47
`@David Mansolino` hi david, could you take a quick look at this issue? If I know how to solve it, I can write up a short tutorial


[https://github.com/cyberbotics/webots/issues/1841](https://github.com/cyberbotics/webots/issues/1841)

##### David Mansolino [Cyberbotics] 07/06/2020 15:35:44
Yes, sure I will answer it tomorrow morning

##### Luftwaffel [Moderator] 07/06/2020 17:30:54
`@David Mansolino` I seem to have figured it out. This shows joint 1 and 3 being changed in percent of their valid range. Looks correct now. Do you want me to make a PR with the new, 'correct' robotiq gripper?
> **Attachment**: [3f\_gripper.mp4](https://cdn.discordapp.com/attachments/565155651395780609/729751244406521887/3f_gripper.mp4)

##### zeynepp 07/06/2020 20:10:30

%figure
![1.JPG](https://cdn.discordapp.com/attachments/565155651395780609/729791407081914508/1.JPG)
%end



%figure
![2.JPG](https://cdn.discordapp.com/attachments/565155651395780609/729791411846774824/2.JPG)
%end


I am trying to write the kinematics for the lrb robot for ipr but it is not directed to the target


what do u think



> **Attachment**: [empty\_.mp4](https://cdn.discordapp.com/attachments/565155651395780609/729792436452196372/empty_.mp4)


where is the problem

##### Sanket Khadse 07/07/2020 04:53:42
I was trying to see what's inside this - "Visual\_tracking.wbt" sample world inside Webots. Unfortunately I didn't found a github link to the file for reference, but this screenshot should be helpful.

My question was, how does the RubberDuck move? I noticed it has an immersion properties node, with fluid name - "water". But I don't see any fluid node in the scene tree. Also there's no controller code, for it's movement.

Any explanation please?
%figure
![Screenshot_from_2020-07-07_10-17-01.png](https://cdn.discordapp.com/attachments/565155651395780609/729923074715025518/Screenshot_from_2020-07-07_10-17-01.png)
%end

##### David Mansolino [Cyberbotics] 07/07/2020 05:52:59
> I was trying to see what's inside this - "Visual\_tracking.wbt" sample world inside Webots. Unfortunately I didn't found a github link to the file for reference, but this screenshot should be helpful.

> My question was, how does the RubberDuck move? I noticed it has an immersion properties node, with fluid name - "water". But I don't see any fluid node in the scene tree. Also there's no controller code, for it's movement.

> Any explanation please?

`@Sanket Khadse` here is the link: [https://github.com/cyberbotics/webots/tree/master/projects/samples/robotbenchmark/visual\_tracking](https://github.com/cyberbotics/webots/tree/master/projects/samples/robotbenchmark/visual_tracking)

The duck is moved by with a Supervisor, here is the controller of the Supervisor: [https://github.com/cyberbotics/webots/blob/master/projects/samples/robotbenchmark/visual\_tracking/controllers/visual\_tracking\_benchmark/visual\_tracking\_benchmark.py](https://github.com/cyberbotics/webots/blob/master/projects/samples/robotbenchmark/visual_tracking/controllers/visual_tracking_benchmark/visual_tracking_benchmark.py)


> `@David Mansolino` I seem to have figured it out. This shows joint 1 and 3 being changed in percent of their valid range. Looks correct now. Do you want me to make a PR with the new, 'correct' robotiq gripper?

`@Luftwaffel` yes sure if you have any correction to bring to the model any PR is highly appreciated!

##### Luftwaffel [Moderator] 07/08/2020 15:55:06
I had a weird issue with urdf2webots. I converted the kinova gen3 6dof arm and everything worked. However when implementing it with moveit, I noticed that several hingeJoints were rotating in the wrong direction. I had to switch the axis manually to negative (or positve) to change direction.  It might have something to do with the urdf using the z-axis and the proto using the y-axis for the joint.

##### David Mansolino [Cyberbotics] 07/09/2020 05:33:58
Hi, just to make sur I understood correctly, the joint axes were correct, but the direction wrong right? Is the error similar to [https://github.com/cyberbotics/urdf2webots/issues/42](https://github.com/cyberbotics/urdf2webots/issues/42) ?

##### Luftwaffel [Moderator] 07/09/2020 08:43:40
Yes, that's exactly the same issue. For some joints the direction of rotation is flipped. I fixed it manually, by flipping the rotational axis (0 1 0 --> 0 -1 0)


I think I had to flip 4/6 joints. Inverse kinematics was broken before, now it works. This should be looked into though in my opinion. It takes deeper knowledge to be able to figure out the issue and fix it. Would be great if it works out of the box and control + IK code can be directly used from the existing urdf based repositories.


Btw, successfully created a working GEN3 kinova arm. Even got IK through moveIt to run. Any news on the 'unofficial' robot model repo?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/730719273768452136/unknown.png)
%end

##### David Mansolino [Cyberbotics] 07/09/2020 09:41:27
> I think I had to flip 4/6 joints. Inverse kinematics was broken before, now it works. This should be looked into though in my opinion. It takes deeper knowledge to be able to figure out the issue and fix it. Would be great if it works out of the box and control + IK code can be directly used from the existing urdf based repositories.

`@Luftwaffel` ok thank you for the feedback, we will try to fix this in the URDF exporter directly.


> Btw, successfully created a working GEN3 kinova arm. Even got IK through moveIt to run. Any news on the 'unofficial' robot model repo?

`@Luftwaffel` Very nice! No news yet (several of us are in holidays this week, so we will wait next week to discuss about this), but I will for sure let you know, it would be nice to include your model 🙂

##### Luftwaffel [Moderator] 07/09/2020 09:44:14
I might make a kinova\_webots git. A bunch of the launch files have to be altered in order for the IK to work. Plus it needs a custom ROS controller for webots.


On that node, I highly modified the universal\_ROS controller you guys created. Made it extern and modified the trajectory and pose\_publisher slightly. Now all it takes to switch from ur10e to kinova-Gen3, is to change the joint names and the same controller works.


A universal(common, not the company) trajectory-arm controller might be something interesting


For our simulation benchmark project, we have to decide on a  version of webots. Could you give a short explanation on how your versions work?  For example, what is 2020a vs 2020b? When does the official version get updated? And lastly, which version would you recommend? I know that I needed to install a nightly build at the end of mai, to fix the simulation reset issue, and that some proto file updates are essential for our benchmark.


Should I just get the latest nightly build of the master branch?

##### David Mansolino [Cyberbotics] 07/14/2020 14:16:37
We usually do 2 version per year, first version a, then aroudn the middle of the year we create version b, then in between these version if needed we create patch release (e.g. a-revision1, a-revision2, etc.). If you need the patch, I would stick to the version b nightly, we will soon release an official version of R2020b when this is the case, I woudl stick to this official and stable R2020b.

##### Luftwaffel [Moderator] 07/14/2020 14:17:20
version b is the develop branch?

##### David Mansolino [Cyberbotics] 07/14/2020 14:17:25
yes

##### Luftwaffel [Moderator] 07/14/2020 14:18:33
do nightly build files stay up indefinetely?

##### David Mansolino [Cyberbotics] 07/14/2020 14:18:49
no, we keep only the one of the last 3 days.

##### Luftwaffel [Moderator] 07/14/2020 14:19:05
hmm

##### David Mansolino [Cyberbotics] 07/14/2020 14:19:20
But once we will release the official stable version of R2020b, this one will stay up indefinetely.

##### Luftwaffel [Moderator] 07/14/2020 14:19:30
is there a ETA?


collegues are setting up their systems and environments, we need to be able to install the same version, and preferably not have to change it frequently

##### David Mansolino [Cyberbotics] 07/14/2020 14:21:22
ok, in that case, only the official stable versions will be kept (e.g. latest R2020a-rev1).

##### Luftwaffel [Moderator] 07/14/2020 14:21:39
that one doesnt have the simulation reset fix included yet right?

##### David Mansolino [Cyberbotics] 07/14/2020 14:22:25
Unfortunately not.


But we expect to release the official version of R2020b in ~3 weeks from now.

##### Luftwaffel [Moderator] 07/14/2020 14:23:02
hmm, perhaps we have to copy and save the setup file ourselves then


thanks for the infos 🙂

##### David Mansolino [Cyberbotics] 07/14/2020 14:31:19
You're welcome

##### Emiliano Borghi 07/22/2020 19:33:46
Hey, I saw you merged this PR: [https://github.com/cyberbotics/webots/pull/1643](https://github.com/cyberbotics/webots/pull/1643)

I would recommend you to make a post in ROS Discourse because it will help the ROS community with their simulations (included myself).



Thanks for merging it BTW!

##### Olivier Michel [Cyberbotics] 07/23/2020 05:33:41
You are welcome. We plan to release Webots R2020b (including this feature) in the next couple of days. We will make sure we announce it (including this ROS-friendly feature and a few others) on ROS Discourse.

##### JSON Derulo 07/23/2020 08:54:46
Hi, does WeBots have to the ability to be integrated with by planners?

##### Darko Lukić [Cyberbotics] 07/23/2020 08:57:02
`@JSON Derulo` Yes, you can check this example: [https://www.youtube.com/watch?v=gO0EXKv8x70](https://www.youtube.com/watch?v=gO0EXKv8x70)



In general, Webots is compatible with ROS and ROS2, therefore you can use ROS packages. A few more links:

- Webots with ROS1 tutorial: [https://cyberbotics.com/doc/guide/tutorial-8-using-ros](https://cyberbotics.com/doc/guide/tutorial-8-using-ros)

- Webots support for ROS2: [http://wiki.ros.org/webots\_ros2](http://wiki.ros.org/webots_ros2)

##### JSON Derulo 07/23/2020 08:57:35
Awesome, thanks

##### csnametala 07/27/2020 16:11:10
Hello everyone!

I have an e-puck robot in Webots simulations, and I wanted to detect when it hits a wall. How can I solve this?

##### Luftwaffel [Moderator] 07/27/2020 16:30:05
read out the appropiate sensor

##### Lukulus 07/29/2020 10:33:21
Hello I am trying to use multiple threads for different tasks for an robot. 

But it seems like calling the robot->step() funktion in diffenrent threads leads to stop the Simulation.

Sometimes it runs, sometimes time stops and also the simulation.

So can you tell me if its possible to use different threads in a robot controller and how to use them in a right way?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/737981083752202241/unknown.png)
%end

##### David Mansolino [Cyberbotics] 07/29/2020 10:43:48
Hi `@Lukulus`, Webots APi supports well multi-threading, however, it is strongly recommended to call the step function from one thread only, doing is contrary is extremely error-prone.

##### Lukulus 07/29/2020 10:46:56
ok, thank you 🙂

##### David Mansolino [Cyberbotics] 07/29/2020 10:48:29
You're welcome

##### Yanick Douven 07/31/2020 07:57:06
Hi everyone!

I'm trying to implement a ROS controller for a 3D lidar (like the Velodyne). Does anything like this already exist?

Thank you!

##### David Mansolino [Cyberbotics] 07/31/2020 08:25:34
hi, for ROS 1 or 2?

##### Yanick Douven 07/31/2020 08:28:12
Hi David,

For ROS 1

##### David Mansolino [Cyberbotics] 07/31/2020 08:29:32
Ok, yes in that case the default ros controller does implement an interface for any kind of lidars: 

  - [https://www.cyberbotics.com/doc/guide/using-ros#standard-ros-controller](https://www.cyberbotics.com/doc/guide/using-ros#standard-ros-controller)

  - [https://www.cyberbotics.com/doc/reference/lidar?tab-language=ros#lidar-functions](https://www.cyberbotics.com/doc/reference/lidar?tab-language=ros#lidar-functions)

##### Yanick Douven 07/31/2020 08:44:34
Thanks! I didn't know there was a standard ROS controller. I'm currently writing a custom one in Python, since I need some non-generic interfacing with other components. Is it possible to combine the two? I.e. run the standard and custom ROS controllers side by side?


Or, if not, is there some Python implementation of the generic lidar controller?

##### David Mansolino [Cyberbotics] 07/31/2020 08:45:16
Yes and no, you can split your robot into two robots (one been a child of the other) and assign one controller per robot.


> Or, if not, is there some Python implementation of the generic lidar controller?

`@Yanick Douven` unfortunately not, but you should be able to convert it quite simply, here is the source code: [https://github.com/cyberbotics/webots/blob/master/projects/default/controllers/ros/RosLidar.cpp](https://github.com/cyberbotics/webots/blob/master/projects/default/controllers/ros/RosLidar.cpp)

##### Yanick Douven 07/31/2020 08:46:31
Perfect, thank you very much!

##### David Mansolino [Cyberbotics] 07/31/2020 08:46:40
You're welcome

## August

##### Luftwaffel [Moderator] 08/04/2020 15:17:17
Found a strange behavior. Running EXACTLY the same simulation, I get several of these errors:

WARNING: The current physics step could not be computed correctly. Your world may be too complex. If this problem persists, try simplifying your bounding object(s), reducing the number of joints, or reducing WorldInfo.basicTimeStep.



if the timeStep=1    Timestep 4-16  are fine

##### Stefania Pedrazzi [Cyberbotics] 08/04/2020 15:46:43
As reported by the warning message, the ODE physics engine has issues in computing the simulation step due to the complexity of the world.

Reducing the time step also reduces the complexity of the computations. Other suggestions to improve the stability of the simulation and avoid this error can be found here:

[https://www.cyberbotics.com/doc/guide/modeling#my-robotsimulation-explodes-what-should-i-do](https://www.cyberbotics.com/doc/guide/modeling#my-robotsimulation-explodes-what-should-i-do)

##### Luftwaffel [Moderator] 08/04/2020 15:47:22
the issue is, that I get these errors at timestep 1, which is much smaller than let's say 8, where everythign runs fine

##### Stefania Pedrazzi [Cyberbotics] 08/04/2020 15:54:36
it depends on the simulation and the collisions between the objects at the computation time. Maybe in your case increasing the time step makes skipping some problematic contact or object's intersection.

##### Luftwaffel [Moderator] 08/04/2020 15:56:09
a simple stacking task
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/740236647781630022/unknown.png)
%end


with cylinder objects, and gripper tips are boxes


at something like 4 or 8 timestep, the simulation outcome is also exactly the same. At like 32 there is a bit of variance, which is expected. I find it weird that at timestep 1 it has issues

##### Stefania Pedrazzi [Cyberbotics] 08/04/2020 15:58:04
it's a simple task, but it is complex from the physical computation

##### Luftwaffel [Moderator] 08/04/2020 15:59:52
is there a way to get more detailed info? where exactle the issue is?

##### Stefania Pedrazzi [Cyberbotics] 08/04/2020 16:01:05
unfortunately not, ODE doesn't give any details. But you can find it by simplifying your simulation until the problem disappears.

##### Luftwaffel [Moderator] 08/04/2020 16:04:35
I just dont understand how decreasing the timestep can cause issues. That's how you're supposed to get rid of these issues

##### Olivier Michel [Cyberbotics] 08/04/2020 16:05:46
In general yes, but your case seems tricky...

##### Luftwaffel [Moderator] 08/04/2020 16:16:42
Suuuper weird issue. So I run the exact same task several times with increasing timesteps (4, 8, 16, 32). If everything is set correctly, it takes 43 seconds. I use the supervisor to reset the simulation after each run, and set the timestep before each run. Now here is the strange part. If the timestep is 4 or smaller, the time for each run is the same. However, if reset the controller and manually put the timestep to 32 BEFORE i start my script, it uses 32ms increments for the simulation time, even if the steps are actually smaller. This results in times of like 5min, 2,5 min 1,2 min and then finally 43 seconds when actually using 32 timestep again.


There seems to be a something strange going on with the sim\_time and timestep


Okay I figured out the issue:

robot.getBasicTimeStep()

This always returns the timestep that a controller was initialized with. Changing the timestep does NOT change the return of this function.


looking at the source code, the robot would have to be re-initialized. I think this function should get this value directly from the world instance instead from a value that get's defined on controller initialization

##### Buzzer 08/05/2020 15:57:51
Hi, how can I get a node position


?


I want to built a bot to go to a specific node location


there is something like GetNode(Node*)


?

##### Luftwaffel [Moderator] 08/05/2020 15:58:21
[https://cyberbotics.com/doc/reference/supervisor?tab-language=python#wb\_supervisor\_node\_get\_orientation](https://cyberbotics.com/doc/reference/supervisor?tab-language=python#wb_supervisor_node_get_orientation)


there is. The controller has to be a supervisor though

##### Buzzer 08/05/2020 15:58:49
Ow nice, thanks

##### Luftwaffel [Moderator] 08/05/2020 16:00:36
there is a python example I wrote, on how to get the position of an object relative to another. That might come in handy
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/740600155841101904/unknown.png)
%end


because the function will only return values relative to the world

##### Buzzer 08/05/2020 16:03:09
thanks `@Luftwaffel`, I will follow this

##### Luftwaffel [Moderator] 08/06/2020 11:19:38
Since I have done quite a few urdf2webots conversions lately, I might as well create a tutorial for it, as the process can be complicated if one has not much experience. Is there any robotic arm that people want to see added to the community projects?

##### Marko G 08/06/2020 11:23:39
Hey! I'm just in the middle of trying to wrap my head around urf2webots and a tutorial would be of great help to me! The arms I would like to use are KUKA LBR iiwa and Kinova Jaco and I belive that they would be a good addition as they are quite popular. BTW right now reading older messages as I see that you've had similar problems 🙂

##### Luftwaffel [Moderator] 08/06/2020 11:24:34
Yesterday I fixed a huge issue in the converter. The joints should not be wonky anymore, so make sure you pull the newest version of urdf2webots


you got a link to the kuka repo?

##### Marko G 08/06/2020 11:25:49
OK, thanks a lot 😄 You've just saved me countless hours hahah :)

I found a github repo with the kuka arm which I'm trying to get to work

##### Luftwaffel [Moderator] 08/06/2020 11:26:00
link?

##### Marko G 08/06/2020 11:26:05
just a sec


[https://github.com/ros-industrial/kuka\_experimental](https://github.com/ros-industrial/kuka_experimental)

The repo has other arms as well, all in separate folders

##### Luftwaffel [Moderator] 08/06/2020 12:09:16
`@David Mansolino` is the new fixed version of urdff2webots already available on pip install? because I get this error:

importer.py: error: no such option: --multi-file

##### David Mansolino [Cyberbotics] 08/06/2020 12:09:32
Not yet.


I will create it right now 😉

##### Luftwaffel [Moderator] 08/06/2020 12:09:58
hehe good


making a  tutorial right now


would be easier with pip. please ping me when it's done

##### David Mansolino [Cyberbotics] 08/06/2020 12:11:46
I just launched the process, now Github Action will create and upload the pip package


It's now live (it reflects the current state of master): [https://pypi.org/project/urdf2webots/#history](https://pypi.org/project/urdf2webots/#history)

##### Luftwaffel [Moderator] 08/06/2020 13:43:39
It worked, but needed to specify no-cache. For anyone else who wants to use the new and fixed version of urdf2webots, install it like this:

pip install --no-cache-dir --upgrade urdf2webots


I ran into an issue, where 

robot.getBasicTimeStep()


returns the Timestep on robot init, not the actual timestep of the simulation


is there already an issue related to this? My suggestion would be to add a function:

robot.setBasicTimeStep

##### Olivier Michel [Cyberbotics] 08/07/2020 13:34:10
Yes, this is a known issue.

##### Luftwaffel [Moderator] 08/07/2020 13:34:36
or add it to the supervisor and have 

robot.getBasicTimeStep()

grab the value directly from the world instance

##### Olivier Michel [Cyberbotics] 08/07/2020 13:34:39
It's pretty rare to dynamically change the time step of a simulation. Why do your need to do that?

##### Luftwaffel [Moderator] 08/07/2020 13:35:21
benchmark runs at different timesteps. I worked around it, by not using 

robot.getBasicTimeStep()  for steps, but the manually set interval

##### Olivier Michel [Cyberbotics] 08/07/2020 13:35:55
A solution could be to read the "basicTimeStep" field of the WorldInfo node from a supervisor process.

##### Luftwaffel [Moderator] 08/07/2020 13:36:18
that would be great


Supervisor function to set and get the BasicTimeStep


because setting it is a bit clonky


want me to open request?

##### Olivier Michel [Cyberbotics] 08/07/2020 13:37:27
No, I believe we won't implement it.

##### Luftwaffel [Moderator] 08/07/2020 13:37:52
ohh I know what you mean

##### Olivier Michel [Cyberbotics] 08/07/2020 13:37:57
Because this is a rare use case and a workaround exists using a supervisor.

##### Luftwaffel [Moderator] 08/07/2020 13:45:50
I could add this example code to the robot.getBasicTimeStep documentation. In case someone else runs into that issue
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/741291016308523018/unknown.png)
%end

##### Olivier Michel [Cyberbotics] 08/07/2020 13:47:32
Yes, that would be great.

##### Luftwaffel [Moderator] 08/12/2020 13:40:49
When I include images in a tutorial, where do I put them? I'm currently creating a 'tutorial.md' for urdf2webots. My plan is to have it in the base directory and link it in the README.md.


I could create a new directory called 'docs' and put everything, including the images there

##### David Mansolino [Cyberbotics] 08/12/2020 13:42:14
Yes, a 'docs' folder with an 'images' folder inside look good.


Like it is currently the case for the 'webots' repository: [https://github.com/cyberbotics/webots/tree/master/docs](https://github.com/cyberbotics/webots/tree/master/docs)

##### Luftwaffel [Moderator] 08/12/2020 13:46:01
sounds good


PR is ready for review [https://github.com/cyberbotics/urdf2webots/pull/68](https://github.com/cyberbotics/urdf2webots/pull/68)

##### Olivier Michel [Cyberbotics] 08/12/2020 15:43:19
Reviewed.

##### Luftwaffel [Moderator] 08/12/2020 16:07:54
can't commit the last one


outdated?!

##### Olivier Michel [Cyberbotics] 08/12/2020 16:16:35
Probably...

##### Luftwaffel [Moderator] 08/12/2020 16:17:16
i'll do it manually

##### Olivier Michel [Cyberbotics] 08/12/2020 16:17:30
OK, thank you.

##### Luftwaffel [Moderator] 08/12/2020 16:21:17
okay done. Accepted all changes except 2 minor ones

##### Olivier Michel [Cyberbotics] 08/12/2020 16:58:51
Thanks. I will check it tomorrow.

##### MarioAndres7 08/12/2020 23:14:33
Hello Im new. Does anyone knows if exists and example of a movile robot? i want to give it 

coordinates

##### David Mansolino [Cyberbotics] 08/13/2020 05:17:10
Hi `@MarioAndres7`, welcome? Do you mean mobile robot?

##### Luftwaffel [Moderator] 08/13/2020 06:55:32
converting the kuka repo atm. Getting these errors:

WARNING: KukaLbrIiwa14R820 (PROTO) > HingeJoint > Solid > HingeJoint > Solid > HingeJoint > Solid > HingeJoint > Solid > HingeJoint > Solid > HingeJoint > Solid > HingeJoint > Solid > Solid: Undefined inertia matrix: using the identity matrix. Please specify 'boundingObject' or 'inertiaMatrix' values.

WARNING: KukaLbrIiwa14R820 (PROTO) > Solid: Undefined inertia matrix: using the identity matrix. Please specify 'boundingObject' or 'inertiaMatrix' values.



I'm pretty sure, these are due to the two 'empty' links in the urdf:

<link name="tool0"/>

<link name="base"/>


Question is, how should empty links be handled properly? And can we automate this

##### David Mansolino [Cyberbotics] 08/13/2020 06:58:39
We should probably improve the importer to handle this exception and avoid exporting the physis in this case.

##### Luftwaffel [Moderator] 08/13/2020 06:59:14
I agree. giving them mass, doesnt seem logical

##### David Mansolino [Cyberbotics] 08/13/2020 06:59:43
Please open an issue on the urdf2webots repo about this.

##### Luftwaffel [Moderator] 08/13/2020 07:00:02
will do

##### David Mansolino [Cyberbotics] 08/13/2020 07:00:07
thank you

##### Luftwaffel [Moderator] 08/13/2020 07:05:30
done, opened issue

##### David Mansolino [Cyberbotics] 08/13/2020 07:06:45
perfect, thank you

##### Luftwaffel [Moderator] 08/13/2020 07:25:26
DEF link\_4\_0 IndexedFaceSet: Cannot create IndexedFaceSet because: "'coordIndex' is empty.".
> **Attachment**: [link\_4\_0Mesh.proto](https://cdn.discordapp.com/attachments/565155651395780609/743369609666625587/link_4_0Mesh.proto)


The file looks correct. I have no idea why this error comes up

##### David Mansolino [Cyberbotics] 08/13/2020 07:30:05
To me it works fine, without any error in the console
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/743370782125719562/unknown.png)
%end

##### Luftwaffel [Moderator] 08/13/2020 07:30:26
uploading to community, one sec


[https://github.com/cyberbotics/community-projects/pull/9](https://github.com/cyberbotics/community-projects/pull/9)


the kr5 robot


I can load the link itself as well, but for some reason, it doesnt work loading it into the robot link

##### David Mansolino [Cyberbotics] 08/13/2020 07:39:11
ok, that's strange, will have a look as soon as I have the time to.

##### Luftwaffel [Moderator] 08/13/2020 07:42:29
removing and adding the hidden tag again seemed to have fixed the issue


[https://tenor.com/view/elmo-shrug-gif-5094560](https://tenor.com/view/elmo-shrug-gif-5094560)

##### David Mansolino [Cyberbotics] 08/13/2020 07:43:14
That's strange, I will try thsi too if I can reproduce the problem

##### Luftwaffel [Moderator] 08/13/2020 08:14:03
another strange issue. This is the kr5 robot, loaded from the community projects folder
%figure
![Screenshot_from_2020-08-13_10-09-47.png](https://cdn.discordapp.com/attachments/565155651395780609/743381846217457694/Screenshot_from_2020-08-13_10-09-47.png)
%end


simply copy pasting the meshes folder into my current project's protos folder, fixes all the issues and looks like this:
%figure
![Screenshot_from_2020-08-13_10-10-20.png](https://cdn.discordapp.com/attachments/565155651395780609/743381994280321034/Screenshot_from_2020-08-13_10-10-20.png)
%end

##### David Mansolino [Cyberbotics] 08/13/2020 08:48:53
I am currently reviewing your PR and checking this.

##### Luftwaffel [Moderator] 08/13/2020 08:49:28
There is also a issue with the standard maxTorque  of 100 being too low


on the kr5 I changed it to maxTorque 1000, and it fixes it (not pushed yet)

##### David Mansolino [Cyberbotics] 08/13/2020 08:49:52
the maxtorque is not defined in the urdf ?

##### Luftwaffel [Moderator] 08/13/2020 08:49:57
no it is not

##### David Mansolino [Cyberbotics] 08/13/2020 08:50:12
ok, and do you know what is the default maxtorque in URDF ?

##### Luftwaffel [Moderator] 08/13/2020 08:50:22
nope


not defined

##### David Mansolino [Cyberbotics] 08/13/2020 08:50:49
ok, maybe if we find it, we can export to the standard urdf maxtorque if not specified in the urdf file.

##### Luftwaffel [Moderator] 08/13/2020 08:51:17
what do you mean?

##### David Mansolino [Cyberbotics] 08/13/2020 08:54:08
In the urdf2webtos, if the maxtorque is not specified in th eurdf file, we just do not specify it in the PROTO file, Webots will then use the fedault value  which is quite small and probably doesn't correspond to the URDF default value. Instead, if the maxtorque is not defined in the URDF file, we can still export the maxTorque to a higher value (ideally corresponding to the URDF default maxTorque).

##### Luftwaffel [Moderator] 08/13/2020 08:54:45
ahhh I understand


from my understanding, if not defined in urdf, it is handled as not having a limit


I mean, Gazebo handles it that way

##### David Mansolino [Cyberbotics] 08/13/2020 08:59:06
> simply copy pasting the meshes folder into my current project's protos folder, fixes all the issues and looks like this:

`@Luftwaffel` for me it works fine from the commuity project without any changes too. Maybe you need to close and re-open Webots? It is possible that if you made some changes to the 'extern project' files they are updated only next time you restart Webots.

##### Luftwaffel [Moderator] 08/13/2020 08:59:31
restarting webots didnt fix the issue weirdly enough

##### David Mansolino [Cyberbotics] 08/13/2020 08:59:57
> from my understanding, if not defined in urdf, it is handled as not having a limit

`@Luftwaffel`ok then in that case, if not defined we should probably set a very high value (e.g. 10'000)


Ohhh I can reprocude the issue with the `lrb_iiwa` but not with the `kr5` is it the same for you?

##### Luftwaffel [Moderator] 08/13/2020 09:01:23
hold on

##### David Mansolino [Cyberbotics] 08/13/2020 09:02:47
Ok, I think I found the reason. The problem is that both robot are defining the same PROTO names, and therefore in the case of the `lrb_iiwa` some meshes of the `kr5` are used.

##### Luftwaffel [Moderator] 08/13/2020 09:03:06
yes, same

##### David Mansolino [Cyberbotics] 08/13/2020 09:03:12
We should probably improve the name of the sub-PROTO to include as prefix the name of the robot.

##### Luftwaffel [Moderator] 08/13/2020 09:03:56
ohhhh, it doesnt start searching at the location of the proto file, but at the base level


okay, then names have to be unique


fix should be easy enough

##### David Mansolino [Cyberbotics] 08/13/2020 09:04:54
Yes, that shouldn't be too dificult to include this prefix

##### Luftwaffel [Moderator] 08/13/2020 09:05:56
but perhaps it would be better to (or in addition), to change the mesh-lookup algorithm. So it starts in the same directory as the main proto file

##### David Mansolino [Cyberbotics] 08/13/2020 09:09:05
We are are going to re-work this indeed (that's on our medium term plan), but in any case avoiding duplcating PROTO names is a good practice (think for example if a user then move the files to put them all in the same folder).

It should be sufficient to prepend the name of the robot to the `name` variable here: [https://github.com/cyberbotics/urdf2webots/blob/master/urdf2webots/writeProto.py#L461](https://github.com/cyberbotics/urdf2webots/blob/master/urdf2webots/writeProto.py#L461)

Would you give a try? Or I can do this if you want.

##### Luftwaffel [Moderator] 08/13/2020 09:09:34
already at it 😄

##### David Mansolino [Cyberbotics] 08/13/2020 09:09:47
Perfect, thank  you 🙂

##### Luftwaffel [Moderator] 08/13/2020 09:29:45
[https://github.com/cyberbotics/urdf2webots/pull/70](https://github.com/cyberbotics/urdf2webots/pull/70)


implemented and tested. works 🙂

##### David Mansolino [Cyberbotics] 08/13/2020 09:33:28
That looks all good, I will try and update the test.

##### Luftwaffel [Moderator] 08/13/2020 09:36:21
I can add the torque fix while we are at it.


already tested it. change 1 indet + change 2 lines

##### David Mansolino [Cyberbotics] 08/13/2020 09:36:50
(I was going to do it ^^, but if you can do it while I am testing it's cool

##### Luftwaffel [Moderator] 08/13/2020 09:39:21
added it


now we just have to remove the physics nodes from epmty liks

##### David Mansolino [Cyberbotics] 08/13/2020 09:39:37
Perfect, I will check it just after lunch

##### Luftwaffel [Moderator] 08/13/2020 10:09:27
implemented the physics node thing too. Not sure it is the cleanest solution, but at least it only needs 4 lines of extra code

##### David Mansolino [Cyberbotics] 08/13/2020 11:06:55
I just finished, if you're fine with the changes feel free to merge it.

##### Luftwaffel [Moderator] 08/13/2020 11:25:35
max torque implementation doesnt work for me


the torque limits are still 0.0, not 10000

##### David Mansolino [Cyberbotics] 08/13/2020 11:29:32
oups, will try

##### Luftwaffel [Moderator] 08/13/2020 11:30:43
line 825 in parserURDF needs a condition I think


jup, adding this line works:


if float(limitElement.getAttribute('effort')) != 0:


pushed it


merged


... Sorry to do this, but one final PR 😄


[https://github.com/cyberbotics/urdf2webots/pull/71](https://github.com/cyberbotics/urdf2webots/pull/71)


after this I'll be happy, I swear


No manual edits needed after converting

##### David Mansolino [Cyberbotics] 08/13/2020 12:13:26
> No manual edits needed after converting

`@Luftwaffel` 🎉

##### Luftwaffel [Moderator] 08/13/2020 12:13:46
well at least for robot arms 🙂

##### David Mansolino [Cyberbotics] 08/13/2020 12:14:29
That's already a good start

##### Luftwaffel [Moderator] 08/13/2020 12:26:03
done


converted and added bunch of KUKA arms:

[https://github.com/cyberbotics/community-projects/pull/11](https://github.com/cyberbotics/community-projects/pull/11)

##### David Mansolino [Cyberbotics] 08/13/2020 14:02:09
Can you also update the one of [https://github.com/cyberbotics/community-projects/pull/9](https://github.com/cyberbotics/community-projects/pull/9) ?

##### Luftwaffel [Moderator] 08/13/2020 15:45:04
Oh I didn't see that one still being open. I replaced all files. pull/9 can be discarded


Can this be an issue? The base node of every robot is called 'base\_link'. Could this lead to problems?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/743863005908959352/unknown.png)
%end

##### David Mansolino [Cyberbotics] 08/17/2020 06:10:33
Oh yes indeed, we should open the 'name' field too.


Just opened an issue about this: [https://github.com/cyberbotics/urdf2webots/issues/72](https://github.com/cyberbotics/urdf2webots/issues/72)

##### Luftwaffel [Moderator] 08/17/2020 17:25:40
I created a universal IK controller. only parameters that need to be changed are the urdf filename and which axis points out of the toolSlot. Works really well


I wonder if the link-setup information could be extracted straight from the protofile. Then it would truly be an universal controller, that wouldn't need any adjustments between robot arms



> **Attachment**: [ik3.mp4](https://cdn.discordapp.com/attachments/565155651395780609/744971358995349504/ik3.mp4)

##### Albens Gladius 08/17/2020 19:31:23
> I wonder if the link-setup information could be extracted straight from the protofile. Then it would truly be an universal controller, that wouldn't need any adjustments between robot arms

`@Luftwaffel` where did you upload the controller?

##### Luftwaffel [Moderator] 08/17/2020 19:36:55
`@Albens Gladius` have not uploaded it yet


do you need help with IK?

##### David Mansolino [Cyberbotics] 08/18/2020 05:47:49
That looks very cool, I have been trying to do the same using IKPY last week ([https://github.com/cyberbotics/webots/blob/master/projects/robots/abb/irb/controllers/inverse\_kinematics/inverse\_kinematics.py](https://github.com/cyberbotics/webots/blob/master/projects/robots/abb/irb/controllers/inverse_kinematics/inverse_kinematics.py)) but I am not 100% happy with the result, I would be very interested to see which library you are using and how.


> I wonder if the link-setup information could be extracted straight from the protofile. Then it would truly be an universal controller, that wouldn't need any adjustments between robot arms

`@Luftwaffel` what about using the new `wb_robot_get_urdf` function?

##### Luftwaffel [Moderator] 08/18/2020 07:33:02
😮 that exists? That would be awesome, I'll check it out later when I have time

##### David Mansolino [Cyberbotics] 08/18/2020 07:34:51
Yes, that's new in R2020b 😉


(and improved in the nightly)

##### Luftwaffel [Moderator] 08/18/2020 07:49:13
the version I have uses the sensor names for the joints. has this been fixed in the new nightlies?

##### David Mansolino [Cyberbotics] 08/18/2020 07:50:52
No, it uses indeed the sensor as names, but you should be able to easily switch from one to the other (removing the 'sensor' postfix).

##### Luftwaffel [Moderator] 08/18/2020 07:51:22
particular reason it does this?


oh and can you link me the IK controller you're working on?

##### David Mansolino [Cyberbotics] 08/18/2020 07:51:53
No, it was either the motor or sensor name, we decided sensor name.


Sure, here it is: [https://github.com/cyberbotics/webots/blob/master/projects/robots/abb/irb/controllers/inverse\_kinematics/inverse\_kinematics.py](https://github.com/cyberbotics/webots/blob/master/projects/robots/abb/irb/controllers/inverse_kinematics/inverse_kinematics.py)

##### Luftwaffel [Moderator] 08/18/2020 07:52:41
converting urdf2webots uses joint names as motor names, that's why I'd assume the other way around would do the same

##### David Mansolino [Cyberbotics] 08/18/2020 07:53:02
Makes sense indeed.

##### Luftwaffel [Moderator] 08/18/2020 07:53:16
if you were to convert it back, the motors would be called 'joint\_sensor' and the sensors 'joint\_sensor\_sensor'

##### David Mansolino [Cyberbotics] 08/18/2020 07:54:21
indeed good point, I should admit that the name of the joint was chosen without particularly thinking about this, it need indeed a bit more thinking about this.

##### Luftwaffel [Moderator] 08/18/2020 07:56:31
glad to be of service 😄


In the controller I sent you, there is the extra file 'get\_relative\_position.py'. I think this is a much needed functionality for the supervisor. Could look something like this:

supervisor.getPositionRelative(self, node)


Doing all this linear algebra manually is a big obstacle for many people. Took me a while to figure out too. First time I needed linear algebra since uni pretty much 😄

##### David Mansolino [Cyberbotics] 08/18/2020 08:11:04
It is indeed useful, but we somehow have to keep the number of API functions low (as to keep the quality level high it requires maintenance time) so we have to be careful when adding function to get information that can already be extracted from other API functions.

##### Luftwaffel [Moderator] 08/18/2020 08:13:33
true I guess, still would be usefull. I might update the documentation and link to a implementation. Or we update the irb inverse kinematics world with this function, showing a general implementation on how to get relative positions and orientationis


universal IK controller nearly done :D. already requires no urdf import (used your armChain creation). only thing left is to figure out, which axis points out of the robot


automatically


is there a easy way to get translation and orientation of a specific solid in a proto file?


I especially need the rotation matrix, urdf only has rpy

##### Olivier Michel [Cyberbotics] 08/18/2020 10:09:24
Yes, you should be able to do this from a supervisor controller.


See [https://www.cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_get\_from\_proto\_def](https://www.cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_get_from_proto_def)


And [https://www.cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_get\_position](https://www.cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_get_position)

##### Luftwaffel [Moderator] 08/18/2020 10:10:25
but  the solid needs to have a defined DEF right?

##### Olivier Michel [Cyberbotics] 08/18/2020 10:10:30
Yes.

##### Luftwaffel [Moderator] 08/18/2020 10:10:35
damn

##### Olivier Michel [Cyberbotics] 08/18/2020 10:11:05
If an upper parent has a DEF name, this may be workable though.

##### Luftwaffel [Moderator] 08/18/2020 10:11:23
I would have to go through the whole tree right?

##### Olivier Michel [Cyberbotics] 08/18/2020 10:11:29
Yes.

##### Luftwaffel [Moderator] 08/18/2020 10:11:32
problem is, I need it for the last solid 😄

##### Olivier Michel [Cyberbotics] 08/18/2020 10:11:54
A fairly simple loop should do the job.

##### Luftwaffel [Moderator] 08/18/2020 10:12:08
trying to make a universal-IK controller for you guys


could you help me with that loop?

##### Olivier Michel [Cyberbotics] 08/18/2020 10:12:41
Which programming language are you using?

##### Luftwaffel [Moderator] 08/18/2020 10:12:47
python

##### Olivier Michel [Cyberbotics] 08/18/2020 10:13:06
OK, let me have a look.

##### Luftwaffel [Moderator] 08/18/2020 10:16:30
thank you

##### Olivier Michel [Cyberbotics] 08/18/2020 10:18:38
How will your recognize the node you want to get the orientation/position? By its name, any other field?

##### Luftwaffel [Moderator] 08/18/2020 10:19:02
i guess name

##### Olivier Michel [Cyberbotics] 08/18/2020 10:19:08
OK.

##### David Mansolino [Cyberbotics] 08/18/2020 10:19:30
> I especially need the rotation matrix, urdf only has rpy

`@Luftwaffel` can't you convert the rpy into matrix? (in python e.g. `transforms3d` allows to do this: [https://matthew-brett.github.io/transforms3d/reference/transforms3d.euler.html#terms-used-in-function-names](https://matthew-brett.github.io/transforms3d/reference/transforms3d.euler.html#terms-used-in-function-names))

##### Luftwaffel [Moderator] 08/18/2020 10:19:51
we might have to update all official robotic arms with the new urdf2webots convention


would have liked to prevent more libraries, but that is an option as well of course


but the supervisor loop would be great in general, and perhaps preferable here


okay, so the ur robots will have to be created again.


the structure of the proto is not like the official urdf. mainly the tool link is missing.

##### David Mansolino [Cyberbotics] 08/18/2020 10:33:30
We can indeed add the tool link, but we should be carreful to not break compatibility (e.g. changing motor/sensor names) when updating it.

##### Luftwaffel [Moderator] 08/18/2020 10:34:57
those are correct. the issue will be the tool slot. with the webots model, the y-axis points out, with the urdff model, the x-axis points out. But we can adjust that to be the same, like the old webots model

##### Olivier Michel [Cyberbotics] 08/18/2020 10:35:29
```python
function getNode(node, name):
    nameField = node.getField('name'):
    if nameField:
        if nameField.getSFString() == name:
            return node
    children = node.getField('children')
    if children:
        for i in range(children.getCount()):
                child = children.getMFNode(i)
                n = getNode(child, name)
                if n:
                    return n
    return None

proto = supervisor.getFromDef('MY_PROTO_ROBOT')
root = proto.getFromProtoDef('MY_ROBOT_ROOT')
node = getNode(root, 'my_searched_name')
```


Something like this should do the job (not tested).

##### Luftwaffel [Moderator] 08/18/2020 10:37:03
thx I will test it

##### David Mansolino [Cyberbotics] 08/18/2020 10:38:01
I think there is an issue:

n = getSFNode(child, name)  = > n = getNode(child, name)

##### Luftwaffel [Moderator] 08/18/2020 10:44:32
root = proto.getProtoFromDef('MY\_ROBOT\_ROOT') 

this is not a valid function


what would be 'MY\_ROBOT\_ROOT'


is the getProto a new develop-branch function?

##### David Mansolino [Cyberbotics] 08/18/2020 10:45:39
no, it is in R2020b

##### Luftwaffel [Moderator] 08/18/2020 10:46:00
oh, running a

##### David Mansolino [Cyberbotics] 08/18/2020 10:46:08
But the code is probably wrong:

=> root = supervisor.getProtoFromDef('MY\_ROBOT\_ROOT')

##### Luftwaffel [Moderator] 08/18/2020 10:52:47
would have to install 2020b, With the tar install, I just have to extract and change environment variable right?

##### David Mansolino [Cyberbotics] 08/18/2020 10:53:05
Yes exactly

##### Luftwaffel [Moderator] 08/18/2020 10:54:40
for ubuntu 18.04 and up I take 

webots-R2020b-rev1-x86-64.tar.bz2 

right?

##### David Mansolino [Cyberbotics] 08/18/2020 10:55:13
For ubuntu 18.04 you can take the webots-R2020b-rev1-x86-64\_ubuntu-16.04.tar.bz2


For up you can take webots-R2020b-rev1-x86-64.tar.bz2

##### Luftwaffel [Moderator] 08/18/2020 10:55:45
can or should? :p

##### David Mansolino [Cyberbotics] 08/18/2020 10:55:50
That's indeed not clearly said


*should 😉

##### Luftwaffel [Moderator] 08/18/2020 10:56:00
okay, thx ^

##### David Mansolino [Cyberbotics] 08/18/2020 10:56:23
You're welcome

##### Luftwaffel [Moderator] 08/18/2020 12:28:27
is there a quick way to set max and min position of a motor from code? or to ignore limits

##### David Mansolino [Cyberbotics] 08/18/2020 12:29:15
No, you can only get the min and max, but the robot is not able to change them (except using the Supervisor API).

##### Luftwaffel [Moderator] 08/18/2020 12:29:47
node from device is not implemented in the main build yyet right?

##### Olivier Michel [Cyberbotics] 08/18/2020 12:30:40
I believe it is implemented in the nightly build of R2021a.

##### David Mansolino [Cyberbotics] 08/18/2020 12:30:45
Only on the develop branch

##### Luftwaffel [Moderator] 08/18/2020 12:31:05
oh man, ikpy uses >= for limits, webots uses just >


WARNING: UR10e (PROTO) > HingeJoint > Solid > HingeJoint > Solid > HingeJoint > Solid > HingeJoint > Solid > HingeJoint > RotationalMotor: too low requested position: -6.28319 < -6.28319

WARNING: UR10e (PROTO) > HingeJoint > RotationalMotor: too low requested position: -6.28319 < -6.28319

WARNING: UR10e (PROTO) > HingeJoint > Solid > HingeJoint > RotationalMotor: too low requested position: -6.28319 < -6.28319

WARNING: UR10e (PROTO) > HingeJoint > RotationalMotor: too low requested position: -6.28319 < -6.28319

WARNING: UR10e (PROTO) > HingeJoint > Solid > HingeJoint > RotationalMotor: too low requested position: -6.28319 < -6.28319

WARNING: UR10e (PROTO) > HingeJoint > RotationalMotor: too low requested position: -6.28319 < -6.28319

WARNING: UR10e (PROTO) > HingeJoint > Solid > HingeJoint > Solid > HingeJoint > Solid > HingeJoint > RotationalMotor: too big requested position: 6.28319 > 6.28319

WARNING: UR10e (PROTO) > HingeJoint > Solid > HingeJoint > Solid > HingeJoint > Solid > HingeJoint > Solid > HingeJoint > RotationalMotor: too low requested position: -6.28319 < -6.28319

WARNING: UR10e (PROTO) > HingeJoint > Solid > HingeJoint > Solid > HingeJoint > Solid > HingeJoint > Solid > HingeJoint > RotationalMotor: too low requested position: -6.28319 < -6.28319

WARNING: UR10e (PROTO) > HingeJoint > Solid > HingeJoint > Solid > HingeJoint > Solid > HingeJoint > RotationalMotor: too big requested position: 6.28319 > 6.28319


😄


finished with the universal IK controller


can take it for a spin
> **Attachment**: [webots\_IK\_controller2.py](https://cdn.discordapp.com/attachments/565155651395780609/745258824138489926/webots_IK_controller2.py)


only requires the robot to be supervisor, and a target node, where it gets position and orientation



> **Attachment**: [lbr\_iiwa\_IK.zip](https://cdn.discordapp.com/attachments/565155651395780609/745260355437199380/lbr_iiwa_IK.zip)


full world, easier to test 🙂


that urdf export function is awesome!


perhaps adding it to the gui would be good. Right click robot - export - as urdf


...testing with irb... the whole calling the joints after the sensors is really screwing with the ik controller here, because it is not using the convention of the sensor being named 'motorname\_sensor'


is there a way to get the motor name, given the sensorname?

##### David Mansolino [Cyberbotics] 08/18/2020 13:20:11
Yes: [https://cyberbotics.com/doc/reference/motor#wb\_motor\_get\_position\_sensor](https://cyberbotics.com/doc/reference/motor#wb_motor_get_position_sensor)

##### Luftwaffel [Moderator] 08/18/2020 13:20:54
[https://tenor.com/view/yay-market-yellow-man-gif-6195464](https://tenor.com/view/yay-market-yellow-man-gif-6195464)


omg it works!!!

##### David Mansolino [Cyberbotics] 08/18/2020 13:30:32
🙂

##### Luftwaffel [Moderator] 08/18/2020 13:31:04
added a fix for singularities, and generating the active link mask is now independant of the naming scheme

##### Olivier Michel [Cyberbotics] 08/18/2020 13:31:59
I just fixed a couple of errors in my code snippet (it should be slightly more correct now).

##### Luftwaffel [Moderator] 08/18/2020 13:32:42
is it compatible with 2020a? I'm not running it, but would be nice for other people to be able to use it

##### Olivier Michel [Cyberbotics] 08/18/2020 13:33:01
No. It requires R2020b.

##### Luftwaffel [Moderator] 08/18/2020 13:33:14
can you paste it again?

##### Olivier Michel [Cyberbotics] 08/18/2020 13:33:52
```python
def getNode(node, name):
    nameField = node.getField('name')
    if nameField:
        if nameField.getSFString() == name:
            return node
    children = node.getField('children')
    if children:
        for i in range(children.getCount()):
            child = children.getMFNode(i)
            n = getNode(child, name)
            if n:
                return n
    return None

proto = supervisor.getFromDef('MY_PROTO_ROBOT')
root = proto.getFromProtoDef('MY_ROBOT_ROOT')
node = getNode(root, 'my_searched_name')
```

##### Luftwaffel [Moderator] 08/18/2020 13:38:29
what is MY\_ROBOT\_ROOT?

##### Olivier Michel [Cyberbotics] 08/18/2020 13:40:02
It's the DEF node inside your proto definition.


The one that contains the node you are looking for.

##### Luftwaffel [Moderator] 08/18/2020 13:43:32
oh so the DEF of the robot with the supervisor controller?


would that not be the same as MY\_PROTO\_ROBOT?

##### Olivier Michel [Cyberbotics] 08/18/2020 13:45:57
No, it's not the same. In your world file, you will have:


```
DEF MY_PROTO_ROBOT MyRobot { ... }
```


And in your `MyRobot.proto` file:


```
PROTO MyRobot [ ... ] { DEF MY_ROBOT_ROOT Robot { ... } }
```

##### Luftwaffel [Moderator] 08/18/2020 13:48:50
there is no DEF in most proto files



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/745278094939783168/unknown.png)
%end

##### Olivier Michel [Cyberbotics] 08/18/2020 13:49:48
You should add one. Or use one if any at a lower level.

##### Luftwaffel [Moderator] 08/18/2020 13:53:11
trying to make it work for general and existing protos


the screenshot above is your model 😉

##### Olivier Michel [Cyberbotics] 08/18/2020 13:54:19
I know. Then we probably need another API function to get the root proto node even if it has no DEF...

##### Luftwaffel [Moderator] 08/18/2020 13:55:14
requireing no argument would be good, as we arleady specify which node.

##### Olivier Michel [Cyberbotics] 08/18/2020 13:57:28
Yes, something like `wb_supervisor_node_get_from_proto_root`.

##### Luftwaffel [Moderator] 08/18/2020 14:00:12
exactly 🙂


\# ikpy includes the bounds as valid, In Webots they have to be less than the limit

        new\_lower = armChain.links[i].bounds[0] * 0.9999999

        new\_upper = armChain.links[i].bounds[1] * 0.9999999        

        armChain.links[i].bounds = (new\_lower, new\_upper)


😄


problem fixed :p still makes me smile

##### Olivier Michel [Cyberbotics] 08/18/2020 14:01:40
It's maybe less dangerous to do:

##### Luftwaffel [Moderator] 08/18/2020 14:03:49
hmm, so my IK controller now works flawlessly with all the robots (at least kuka) in the community projects. Is it possible to add a controller on a higher abstraction level and not a per-world basis?

##### Olivier Michel [Cyberbotics] 08/18/2020 14:03:54
```python
        new_lower = armChain.links[i].bounds[0] - 0.0000001 if armChain.links[i].bounds[0] > 0 else armChain.links[i].bounds[0] + 0.0000001
        new_upper = armChain.links[i].bounds[1] - 0.0000001 if armChain.links[i].bounds[1] > 0 else armChain.links[i].bounds[1] + 0.0000001
        armChain.links[i].bounds = (new_lower, new_upper)
```

##### Luftwaffel [Moderator] 08/18/2020 14:04:30
i'm not dividing, so there should be no issues?!


ohhhhh


because of 0

##### Olivier Michel [Cyberbotics] 08/18/2020 14:04:50
Yes, I would recommend you to add such a controller in a folder named "generic\_inverse\_kinematic" or the like.

##### Luftwaffel [Moderator] 08/18/2020 14:04:50
yeah

##### Olivier Michel [Cyberbotics] 08/18/2020 14:05:20
Yes, there might be rounding problems with small values.

##### Luftwaffel [Moderator] 08/18/2020 14:05:27
where would that folder be? I added the whole comm.proj. git to extra paths


new\_lower = armChain.links[i].bounds[0] + 0.0000001

        new\_upper = armChain.links[i].bounds[1] - 0.0000001       

        armChain.links[i].bounds = (new\_lower, new\_upper)

this should be enough, since lower is always smaller than upper

##### Olivier Michel [Cyberbotics] 08/18/2020 14:07:55
Probably we should create a `controllers` folder at the level and put it in there.

##### Luftwaffel [Moderator] 08/18/2020 14:08:13
that would be good

##### Olivier Michel [Cyberbotics] 08/18/2020 14:08:31
Like [https://github.com/cyberbotics/community-projects/tree/master/controllers/generic\_inverse\_kinematic/](https://github.com/cyberbotics/community-projects/tree/master/controllers/generic_inverse_kinematic/)

##### Luftwaffel [Moderator] 08/18/2020 14:08:46
404

##### Olivier Michel [Cyberbotics] 08/18/2020 14:08:54
Sure, you need to create it.


Feel free to open a PR to do so.

##### Luftwaffel [Moderator] 08/18/2020 14:11:58
does not find the controller

##### David Mansolino [Cyberbotics] 08/18/2020 14:13:38
> Like [https://github.com/cyberbotics/community-projects/tree/master/controllers/generic\_inverse\_kinematic/](https://github.com/cyberbotics/community-projects/tree/master/controllers/generic_inverse_kinematic/)

`@Olivier Michel` this does not respect the Webots project hierarchy

##### Luftwaffel [Moderator] 08/18/2020 14:14:10
wait, it doesnt find anything in the extra path now

##### David Mansolino [Cyberbotics] 08/18/2020 14:14:16
You should rather put it in somethig like 'default/controllers/generic\_inverse\_kinematic'

##### Luftwaffel [Moderator] 08/18/2020 14:15:14
that was it


putting controller in the base directory, removes everything

##### Olivier Michel [Cyberbotics] 08/18/2020 14:15:37
OK, in that case, I would prefer something like `generic/controllers/inverse_kinematic`

##### David Mansolino [Cyberbotics] 08/18/2020 14:16:27
That's fine too, I was suggesting 'default' to be like in Webots ([https://github.com/cyberbotics/webots/tree/master/projects](https://github.com/cyberbotics/webots/tree/master/projects)), but it is not mandatory to be exactly the same.

##### Luftwaffel [Moderator] 08/18/2020 14:16:58
I like consistency


but up to you guys

##### David Mansolino [Cyberbotics] 08/18/2020 14:17:18
And as we already have a n'inverse\_kinematic' controller in Webots looks safer to avoid duplicating it.

##### Olivier Michel [Cyberbotics] 08/18/2020 14:17:59
OK, so let's go for the solution proposed by `@David Mansolino`.

##### Luftwaffel [Moderator] 08/18/2020 14:19:43
perhaps that target sphere can be generated by the controller. that would make the implementation even simpler

##### Olivier Michel [Cyberbotics] 08/18/2020 14:20:29
Yes, that could be a nice demo.

##### Luftwaffel [Moderator] 08/18/2020 14:20:50
i can't upload to github through the webpage 😦

##### Olivier Michel [Cyberbotics] 08/18/2020 14:21:01
(it may be optional, triggered by a controllerArg)


You need to create a new branch

##### Luftwaffel [Moderator] 08/18/2020 14:21:55
usually I can upload and crerate a new branch from those files


you guys do it 😉
> **Attachment**: [default.zip](https://cdn.discordapp.com/attachments/565155651395780609/745286472424489010/default.zip)

##### Olivier Michel [Cyberbotics] 08/18/2020 14:22:43
Did you checked out locally the community-projects repo?

##### Luftwaffel [Moderator] 08/18/2020 14:23:10
I dont wanna pull, because I have many working changes, guess I could clone again for upload

##### Olivier Michel [Cyberbotics] 08/18/2020 14:24:13
I see. So from GitHub web page, you should be able to create a branch and upload your controller files on this branch (creating the folder structure at the same time).


1. Go on the "Code" tab.


2. Click on the "master" pop-up menu and create a branch (give it a name)


3. On this branch, click "Add files" popup menu.


4. Upload your two python files.

##### Luftwaffel [Moderator] 08/18/2020 14:26:11
still something wrong


doesnt let me


Something went really wrong, and we can’t process that file.

##### Olivier Michel [Cyberbotics] 08/18/2020 14:26:21
Yes, I see.


You should create a temporary dummy file.


Click "Add file" / "Create new files".


Enter "default/controllers/generic\_inverse\_kinematics/dummy.txt"

##### Luftwaffel [Moderator] 08/18/2020 14:28:06
that seems to work

##### Olivier Michel [Cyberbotics] 08/18/2020 14:28:18
Then, you will be able to upload your files and delete the dummy.txt file.

##### Luftwaffel [Moderator] 08/18/2020 14:28:55
[https://github.com/cyberbotics/community-projects/pull/12](https://github.com/cyberbotics/community-projects/pull/12)


how do I revert commits?

##### Olivier Michel [Cyberbotics] 08/18/2020 15:26:02
Simply commit the previous version of the file.

##### Luftwaffel [Moderator] 08/18/2020 15:42:15
finally done 😄


how would I spawn that target sphere?

##### Olivier Michel [Cyberbotics] 08/18/2020 15:44:20
Probably with `importSFNodeFromString`: [https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python#wb\_supervisor\_field\_import\_sf\_node\_from\_string](https://www.cyberbotics.com/doc/reference/supervisor?tab-language=python#wb_supervisor_field_import_sf_node_from_string)

##### Luftwaffel [Moderator] 08/18/2020 15:46:14
as a string I can just use the world file part of the node then?

##### Olivier Michel [Cyberbotics] 08/18/2020 15:46:37
Yes.

##### Luftwaffel [Moderator] 08/18/2020 15:46:44
gonna try 🙂


how can I get the number of children? I wanna insert the target node on the bottom

##### Olivier Michel [Cyberbotics] 08/18/2020 15:58:13
Get the root node, its children field and get the number of items (see my code sample)


`children.getCount()`

##### Luftwaffel [Moderator] 08/18/2020 15:59:13
just saw, thx 🙂


okay works beautifully


can I link textures relative to the controller? or would it be easier to copy the files into the world folder?

##### Olivier Michel [Cyberbotics] 08/18/2020 16:06:54
It's better to copy them in the worlds folder under a textures subfolder.

##### Luftwaffel [Moderator] 08/18/2020 16:07:39
how can I get the path to the world?

##### Olivier Michel [Cyberbotics] 08/18/2020 16:09:38
There must be a project path environment variable... Let me search...


Can you try to getenv and print WEBOTS\_PROJECT environment variable? Then append "/worlds" to it should do the trick.


No, that won't work...

##### Luftwaffel [Moderator] 08/18/2020 16:13:36
it's not a system variable

##### Olivier Michel [Cyberbotics] 08/18/2020 16:14:29
Otherwise, you can assume it's `../../worlds/` relative to your controller.

##### Luftwaffel [Moderator] 08/18/2020 16:14:49
well, using it for the generic controller 😉

##### Olivier Michel [Cyberbotics] 08/18/2020 16:17:17
Unfortunately, I don't see any better way to do this...

##### Luftwaffel [Moderator] 08/18/2020 16:17:36
shucks


I think I'll just remove the texture and make the sphere yellow


is there a conflict, if I inialize a supervisor in my main controler, and also in a imported module?


meaning, do I have to parse the supervisor for the module-class \_\_init\_\_ function or can it just import it directly


figured it out, have to parse it. Can't init supervisor twice


btw, figured out the world path problem



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/745346993257185430/unknown.png)
%end

##### David Mansolino [Cyberbotics] 08/18/2020 19:25:45
> Can you try to getenv and print WEBOTS\_PROJECT environment variable? Then append "/worlds" to it should do the trick.

`@Olivier Michel` `@Luftwaffel`  we have an API function for this: Robot.getWorldPath()

##### Luftwaffel [Moderator] 08/18/2020 19:26:29
what I used 😉


that returns the worldpath including the filename though, that's why I needed the next 2 lines to remove it

##### David Mansolino [Cyberbotics] 08/18/2020 19:27:23
> what I used 😉

`@Luftwaffel` just saw it (I was reading messages from top to bottom)

##### Luftwaffel [Moderator] 08/18/2020 19:28:29
it works super well now. you just have to assignt the controller and do nothing else. If there is no target node, it adds it for you. Even tells you to enable supervisor, if not done so already

##### David Mansolino [Cyberbotics] 08/18/2020 19:29:52
> that returns the worldpath including the filename though, that's why I needed the next 2 lines to remove it

`@Luftwaffel` a simplest solution would be:

```Python
worldPath = os.path.dirname(supervisor.getWorldPath())
```

##### Luftwaffel [Moderator] 08/18/2020 19:30:08
that's cleaner

##### David Mansolino [Cyberbotics] 08/18/2020 19:30:11
> it works super well now. you just have to assignt the controller and do nothing else. If there is no target node, it adds it for you. Even tells you to enable supervisor, if not done so already

`@Luftwaffel` looks very nice 🙂

##### Luftwaffel [Moderator] 08/18/2020 19:35:45
but we will have to check and recompile several official robot arms, as they dont include the last link, when converting to urdf. Good idea anyways, using multi-file, so viewing and editing PROTO-source is less laggy


`@alxy` you can merge your PR

`@Olivier Michel` `@David Mansolino` I created a new PR for the Mirobot, already including alxy's changes:

[https://github.com/cyberbotics/community-projects/pull/14](https://github.com/cyberbotics/community-projects/pull/14)

##### alxy 08/20/2020 18:09:14
Did you check if it works if the bounds are not set?

##### Luftwaffel [Moderator] 08/20/2020 18:09:30
the bounds get exported


I now found an issue, where attached things to the toolSlot fall down 😄


added a bit of code that improves the singularity handling
> **Attachment**: [ik\_module.py](https://cdn.discordapp.com/attachments/565155651395780609/746068745415557170/ik_module.py)

##### alxy 08/20/2020 18:11:08
Ok, so I will now merge my PR, however I guess it doesnt really matter....

##### Luftwaffel [Moderator] 08/20/2020 18:11:46
you can attach grippers without falling down?

##### alxy 08/20/2020 18:11:56
yes?


you can check my world in my repo, my gripper works...

##### Luftwaffel [Moderator] 08/20/2020 18:45:22
figured it out and fixed it


you can check out my model in the PR above. I'd recommend using it, as it is super lightweight and easy to edit, due to the meshes being in seperate files

##### alxy 08/20/2020 19:38:05
yeah, will see if I can do that tomorrow. I also plan to export my gripper to a proto file again

##### Luftwaffel [Moderator] 08/21/2020 10:54:52
`@David Mansolino` Could you tell me why [https://github.com/cyberbotics/urdf2webots/blob/master/urdf2webots/writeProto.py#L462](https://github.com/cyberbotics/urdf2webots/blob/master/urdf2webots/writeProto.py#L462) this condition is needed? I created a batch-convertion, automatically converting a whole nested directory of urdf files (with options defined in .json configs). And for some reason, this line stops SOME, but not all Mesh protos to be created. I spent 5 hours yesterday trying to figure that out.  Setting this line to 

if True: 

fixes this issue. My question is, why is it needed?

##### David Mansolino [Cyberbotics] 08/21/2020 11:40:50
This is part of the automatic DEF-USE mechanism to avoid defining twice the same mesh, if you set the condition to True, then it will an any case export the mesh and disable DEF-USE mechanism

##### Luftwaffel [Moderator] 08/21/2020 11:43:39
Once we are through with the current PR (just updated it), I'll upload the batch conversion. Perhaps you can then take a look. The issue is super weird


the issue came up, when I rant the batch conversion inside a class. removed the class and just used normal functions. Seems to work now... super strange

##### alxy 08/21/2020 12:48:55
What do you guys think? Is it somehow possible to interface the original firmware ( [https://github.com/wlkata/WLkata-Mirobot-Firmware](https://github.com/wlkata/WLkata-Mirobot-Firmware) ) for the microcontroller of the real robot with webots? I have no idea if this is could even somehow be possible, but I imagine much more realistic movements and trajectories of the robot, as it uses the original algorithms to calculate motor positons...

##### Luftwaffel [Moderator] 08/21/2020 12:50:05
of course it's possible, but not easy. Feel free to do it 😉


I'm a python guy. c scares me 😅


I can read it if I absolutely have to, but I can't code in it

##### alxy 08/21/2020 12:52:03
Yeah, Im on the same side, but some C code I can really hardly read


the steps would be to first kind of build the program and see how to map input and output I assume?


On a first glance it appears these guys who coded the firmware in C are wizards 😄

##### Luftwaffel [Moderator] 08/21/2020 12:55:35
`@David Mansolino` do you have the urdf files and meshes of the robots you created?

##### David Mansolino [Cyberbotics] 08/21/2020 12:55:50
Which ones?

##### Luftwaffel [Moderator] 08/21/2020 12:56:02
as many as possible 😉

##### David Mansolino [Cyberbotics] 08/21/2020 12:56:37
Unfortunately, most (almost all) of the robots are designed from scratch without using the URDF files 😕

##### Luftwaffel [Moderator] 08/21/2020 12:56:45
ohh okay


right now, I can convert the entire community projects repo, plus universal robots ur3, 5 and 10 in like 10 seconds


making improvements to the convertion tools, allows to reconvert and update all robots

##### David Mansolino [Cyberbotics] 08/21/2020 12:58:06
That sounds very nice 😄

##### Luftwaffel [Moderator] 08/21/2020 12:58:11
The difference with mutli-file for the user-experrience is HUGE

##### David Mansolino [Cyberbotics] 08/21/2020 12:58:28
> The difference with mutli-file for the user-experrience is HUGE

`@Luftwaffel` yes indeed, it much simpler for the maintenance too!

##### Luftwaffel [Moderator] 08/21/2020 12:58:31
editing a 5MB proto file is just so horrible


approved the changes

##### David Mansolino [Cyberbotics] 08/21/2020 13:03:31
Cool, remains just to update:

  - [https://github.com/cyberbotics/urdf2webots/blob/Simon-Steinmann-nameFixes%26more/README.md](https://github.com/cyberbotics/urdf2webots/blob/Simon-Steinmann-nameFixes%26more/README.md)

  - [https://github.com/cyberbotics/urdf2webots/blob/Simon-Steinmann-nameFixes%26more/docs/tutorial.md](https://github.com/cyberbotics/urdf2webots/blob/Simon-Steinmann-nameFixes%26more/docs/tutorial.md)

##### Luftwaffel [Moderator] 08/21/2020 13:03:46
😅  oh yeah right


pushed update doc

##### David Mansolino [Cyberbotics] 08/21/2020 13:37:00
Just a small typo.

##### alxy 08/21/2020 13:44:49
Ok, after googling around it appears more or less impossible for me 😄

##### Luftwaffel [Moderator] 08/21/2020 13:45:17
i'm sure the real robot has position control right?


if your max vel is low enough, it should be pretty much 1 to 1


where you run into differences is when using IK. Most real robots also have cartesian control, using IK in their controller. That might be quite different

##### alxy 08/21/2020 13:46:42
position control as in your provide coordinates and it moves?

##### Luftwaffel [Moderator] 08/21/2020 13:46:54
position control is joint angles

##### alxy 08/21/2020 13:47:13
ah, yeah you can do that, obviously that is something we dont use at all 😄

##### Luftwaffel [Moderator] 08/21/2020 13:47:15
cartesian control is xyz for the tool


but you can use your own ik, use it for both, the simulation and the real robot


and control the robot by adressing the joint angles directly

##### alxy 08/21/2020 13:48:15
that sounds good

##### Luftwaffel [Moderator] 08/21/2020 13:50:31
where did you get the mirobot vel limits?


and are there torque limits too?

##### alxy 08/21/2020 13:51:08
I linked the document in the PR


[https://uploads-ssl.webflow.com/5edc22ea8e58c13e2883e09b/5efd245acdbcfb27deb727ce\_WLKATA%20Mirobot%20User%20Manual%20V1.2.pdf](https://uploads-ssl.webflow.com/5edc22ea8e58c13e2883e09b/5efd245acdbcfb27deb727ce_WLKATA%20Mirobot%20User%20Manual%20V1.2.pdf) page 55


but I have to say the docuemtnation is not entirely correct, I have already found some quite major errors

##### Luftwaffel [Moderator] 08/21/2020 13:53:10
like what?

##### alxy 08/21/2020 13:53:15
They certainly do have torque limits, but they are not specified

##### Luftwaffel [Moderator] 08/21/2020 13:53:34
yeah ofc they have. they are NOT 200Nm, that would be very impressive 😄

##### alxy 08/21/2020 13:53:49
They use gcodes to communicate, and there is one gcode to open the gripper, but it actually closes it


return values of the firmware to the serial connection are not correct


or not documented


or entirely different from what is documented

##### Luftwaffel [Moderator] 08/21/2020 13:57:37
well i'm gonna add the velocities to the .urdf, so they get converted automatically

##### alxy 08/21/2020 13:57:52
Nice, thanks!

##### Luftwaffel [Moderator] 08/21/2020 14:00:28
works, gets converted automatically now 🙂

##### alxy 08/21/2020 14:01:05
you have taken the converter to a whole new level! 🙂

##### Luftwaffel [Moderator] 08/21/2020 14:01:43
really have over the past month. It was the reason I didnt switch a year earlier. Getting your own robot into the simulator is so important

##### alxy 08/21/2020 14:02:17
what did you use earlier?

##### Luftwaffel [Moderator] 08/21/2020 14:02:22
Gazebo

##### alxy 08/21/2020 14:02:48
Ok, for me its my first experience with a robot simulation tool, or robotics in general 😄

##### Luftwaffel [Moderator] 08/21/2020 14:04:02
you picked a good one. For me the main advantages of Webots are:

- excellent and detailed documentation

- native API in multiple languages (not incomplete and slow wrappers)

- this discord, where you get answers immediatley

##### alxy 08/21/2020 14:04:32
yeah, the support is really impresive

##### Luftwaffel [Moderator] 08/21/2020 14:04:54
pllus it's easy to get changes implemented if you make them 😄


wooo, PR merged


`@alxy` `@David Mansolino` [https://github.com/cyberbotics/urdf2webots/pull/74](https://github.com/cyberbotics/urdf2webots/pull/74)


please check it out and see if it works for you


was interesting creating this, super fun. Sat at it till from like 4pm to 5am 😅


FINALLY!!!!!!!!!!! figured out the issue with some meshes not being created. Collision and Material references get added. Have to manually reset them:

importer.urdf2webots.parserURDF.Geometry.reference = {}

importer.urdf2webots.parserURDF.Material.namedMaterial = {}


so the <line> part in collada meshes dont get correctly converted.  It tries to create a triangle mesh, which of course fails (coordIndex is empty)
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/746425729868103701/unknown.png)
%end


fixed it, assuming that LineSets cannot get properly imported into Webots:


[https://github.com/cyberbotics/urdf2webots/pull/75](https://github.com/cyberbotics/urdf2webots/pull/75)

##### alxy 08/22/2020 14:47:49
yeah, the PRs definetly need fixing 😄


I'd also suggest to use your own fork for the PRs, otherwise the main repo gets cluttered with all these branches


`@Luftwaffel` Sorry to bug you again, but I have found the original gripper for Mirobot in github as step (stp) file: [https://github.com/CBerauer/Mirobot-CAD-Files/tree/master/Gripper](https://github.com/CBerauer/Mirobot-CAD-Files/tree/master/Gripper) I just installed a viewer and it indeed looks good, however I am now struggling how to convert that to webots? Is it possible?

##### Luftwaffel [Moderator] 08/22/2020 15:57:10
you gonna have to convert it to .obj, stl or .dae for proto conversion. Or check the import in webots itself. it supports a bunch of formats

##### alxy 08/22/2020 16:03:56
Mh, exported to stl and was able to successfully import, but nothing shows up. It created a bunch of new elements under a solid node though, but I dont see the gripper

##### Luftwaffel [Moderator] 08/22/2020 16:06:05
having a urdf file or something that dictates the structure would be good

##### alxy 08/22/2020 16:07:15
yeah, I have just realized it really only contains the 3d mockup, no joints defined


So I cannot really use it, sadly 😦

##### Luftwaffel [Moderator] 08/22/2020 16:08:40
what is the name of that gripper?


you could try to manually assemble them

##### alxy 08/22/2020 16:09:56
I dont think it has a dedicated name?

##### Luftwaffel [Moderator] 08/22/2020 16:10:23
except the one you linked, I find no mirobot package containing a gripper

##### alxy 08/22/2020 16:10:25
It comes with Mirobot in the satndard package


yeah, they did not release the urdf for the gripper, just for the arm itself


Not sure where that guy got the 3d model from, maybe he has assembled it on his own

##### Luftwaffel [Moderator] 08/22/2020 16:12:19
well, having the parts, it should not be too complicated assembing it

##### alxy 08/22/2020 16:12:55
given I have no experience with any cad/3d software, I can imagine it is very hard 😄

##### Luftwaffel [Moderator] 08/22/2020 16:13:26
no, in webots, or with urdf.

##### alxy 08/22/2020 16:14:31
yeah that might be possible, but not if it gets created as a signle IndexedFaceSet, the parts are not separate nodes in webots

##### Luftwaffel [Moderator] 08/22/2020 16:15:40
that would not be ideal :p


perhaps you can convert it to another format, which includes data for the different components

##### alxy 08/22/2020 16:16:17
I checked out the other file (catpart) in inventor, that looks more promising


do you know which formats preserve the structure?

##### Luftwaffel [Moderator] 08/22/2020 16:17:10
collada (.dae) does. But i'm not an expert in this


.dae has the advantage to be readable in a text editor

##### alxy 08/22/2020 16:19:36
of course that is not available as an export option 😄

##### Luftwaffel [Moderator] 08/22/2020 16:20:20
A new Mesh node was added to support the inclusion of a large variety of external 3D mesh files: 3D Studio mesh, Blender, Biovision Hierarchy, Collada, Filmbox, STL, Wavefront, X3D. It is also possible to import such files as Webots primitives.

##### alxy 08/22/2020 16:21:09
Yeah, I checked the list on the import wizzward with the list of available export options


I try .obj now


lol, scales dont seem to match:
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/746766447845310506/unknown.png)
%end


can you find my mirobot 😄

##### Luftwaffel [Moderator] 08/22/2020 16:25:13
nope :p


perhaps it uses millimeters as its unit


so that would be 1000x


perhaps you can change the unit somewhere before exporting?

##### alxy 08/22/2020 16:26:03
yeah, its in that black whole facing the front. You can see it


but the problem persists with the obj, it is just one single node in webots then

##### Luftwaffel [Moderator] 08/22/2020 16:29:18
what program did you load the file into?

##### alxy 08/22/2020 16:30:08
autodesk inventor


and I can see the hierarchy of different parts there


so it is somehow encoded in the file

##### Luftwaffel [Moderator] 08/22/2020 16:30:43
can you export individual parts?

##### alxy 08/22/2020 16:30:49

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/746768350914281633/unknown.png)
%end


No idea, as said I have no prior experience with CAD programs. The interface looks rather complicated to me

##### Luftwaffel [Moderator] 08/22/2020 16:32:43
great opportunity to learn 😉

##### alxy 08/22/2020 16:33:20
Its a bit far from what I wanted to achieve originally 😄

##### Luftwaffel [Moderator] 08/22/2020 16:33:35
can never hurt

##### alxy 08/22/2020 16:34:02
probably not, if just time permits to do all the things... 😄


the easier thing is to first try all the other available cad programs and see if they can export to other formats (I will try blender now)

##### Luftwaffel [Moderator] 08/22/2020 16:36:36
fusion 360 might be a good optioni

##### alxy 08/22/2020 16:44:45
[https://github.com/rjvallett/URDF-Converter](https://github.com/rjvallett/URDF-Converter) that looks promising, but hasnt been updated in a while and lacks any documentation

##### Luftwaffel [Moderator] 08/22/2020 16:45:56
try it 🙂


here we go
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/746773437116186765/unknown.png)
%end


used this: [https://cadexchanger.com/downloads](https://cadexchanger.com/downloads)

to convert it to a .dae file. It needed some editing, but managed to import it

##### alxy 08/22/2020 17:05:47
but is it in separate parts?


I mean importing was not an issue, that worked with either of the format


it is a sigle IndexedFacedSet for me


`@Luftwaffel`

##### Luftwaffel [Moderator] 08/22/2020 17:16:03
seperate parts


I'm adding jonits and motors atm

##### alxy 08/22/2020 17:16:21
wow, thanks 🙂

##### Luftwaffel [Moderator] 08/22/2020 17:16:48
never done it that way, learning opportunity 😉

##### alxy 08/22/2020 17:17:48
I really dont know why the manufacturer does not provide the files for the gripper. It is basically useless to do simulations without the gripper I assume

##### Luftwaffel [Moderator] 08/22/2020 17:18:02
[https://tenor.com/view/elmo-shrug-gif-5094560](https://tenor.com/view/elmo-shrug-gif-5094560)


if you're curious, we could zoom and I show you what i'm doing

##### alxy 08/22/2020 17:18:36
sure, are you from germany btw?

##### Luftwaffel [Moderator] 08/22/2020 17:18:41
jep

##### alxy 08/22/2020 17:19:10
ok, so let me install zoom then

##### Luftwaffel [Moderator] 08/22/2020 19:10:02
preliminary proto for the gripper. names for meshes are still bonked, and of course motors have no limits
> **Attachment**: [MirobotGripper.proto](https://cdn.discordapp.com/attachments/565155651395780609/746808418400206853/MirobotGripper.proto)


you owe me a beer once rona is over 😄
> **Attachment**: [for\_alex.zip](https://cdn.discordapp.com/attachments/565155651395780609/746859366459899914/for_alex.zip)


`@alxy` wrote a very simple and easy to include controller too

##### alxy 08/23/2020 08:02:43
Ok, yeah, sure. Will test it now 🙂


The Mirobot.proto is the current one from the community project?

##### Luftwaffel [Moderator] 08/23/2020 08:04:58
either that or a newly converted, but with correct values


it doesnt contain the mirobot, so it is the same model you have

##### alxy 08/23/2020 08:08:24
Nice, works very well 🙂


but it really contains a lot of motors 😄

##### Luftwaffel [Moderator] 08/23/2020 08:10:03
uses 3 virtual motors per gripper side, instead of 1


but all 6 gripper motors can be moved with the same position command to have it open and close correctly

##### alxy 08/23/2020 08:12:44
yeah, thats indeed vbery easy to use like that. I had thought ahout a rather complex controller script when you told me about the virtual motors yesterday...


I am very curious how good it matches the real bot in the end. It looks really close now

##### Luftwaffel [Moderator] 08/23/2020 08:25:07
you gonna have to adjust the torques perhaps


the problem is, that the motors should be exactly the same value


perhaps use torque feedback

##### alxy 08/23/2020 08:27:29
ok, will see, I needed to do that for the old gripper as well to properly grab the wooden boxes

##### Luftwaffel [Moderator] 08/23/2020 08:28:04
I have a script somewhere to do that. I can check later, when I'm on linux


basically I use high torque on the motors, but with torque feedback, and they incrementally close each timestep, as long as the torque is lower than a threshhold

##### alxy 08/23/2020 08:29:26
why dont you set it too some lower, more realistic torque?

##### Luftwaffel [Moderator] 08/23/2020 08:29:28
then they all stop


because you have 3 virtual motors, they have to have the EXACT same value, for them to be parallel


and they have different level action. so torques will be different


you could manually synchronize all motors to let's say on main motor, but then all the 'slave' motors will be one timestep behind


perhaps you can come up with a better solution 😄


if you do, let me know 😉

##### alxy 08/23/2020 08:33:59
Im not entirely sure if I understand the problem, but I will see what happens. I am going to integrate the new gripper with my IP based controller first and then go on to see if I can grab something 🙂

##### Luftwaffel [Moderator] 08/23/2020 08:35:10
good luck. Let me know if you have any changes or improvements for the model. We should probably add it to community projects

##### alxy 08/23/2020 08:36:48
yeah, I can do it if you want me to, but as its your work, you should probably do it 😄

##### Luftwaffel [Moderator] 08/23/2020 08:57:32
well, let me know if you have any improvements or feedback, so I can improve the model and then upload it

##### alxy 08/23/2020 09:01:59
I think the wrist motor is not necessary and not in the real robot as well, as the last joint of the arm has the same axis and can thus be used to achieve the same functionality. Maybe be usedfull if used with another arm though

##### Luftwaffel [Moderator] 08/23/2020 09:28:17
I think you're right


[https://www.robotshop.com/media/catalog/product/cache/image/1350x/9df78eab33525d08d6e5fb8d27136e95/w/l/wlkata-6-axis-mini-robot-arm-mirobot-education-kit-2.jpg](https://www.robotshop.com/media/catalog/product/cache/image/1350x/9df78eab33525d08d6e5fb8d27136e95/w/l/wlkata-6-axis-mini-robot-arm-mirobot-education-kit-2.jpg)


you can try to do it yourself. Otherwise I'll do it later

##### alxy 08/23/2020 09:29:33
its not an issue, I just dont initialize or use this motor

##### Luftwaffel [Moderator] 08/23/2020 09:29:37
little tip. You can convert it to basenodes, adjust it, then save the world. If you open the world file, you have the .proto definition in there


(open in text editor)

##### alxy 08/23/2020 09:33:24
but you are right, in this case its not enoug to simply change the troque limits, teh gripper is not able to grab like that


right\_1 is the actual motor which is actuated in the real robot?


> WARNING: Mirobot (PROTO) > MirobotGripper (PROTO) > Solid > HingeJoint > Solid > HingeJoint > RotationalMotor: wb\_motor\_enable\_torque\_feedback(): cannot be invoked for a Joint whose end point has no Physics node.


mh, simply adding Physics nodes to all solides makes it look a bit weird 😄

##### Luftwaffel [Moderator] 08/23/2020 10:28:10
I already did that


my work in progress
> **Attachment**: [for\_alex2.zip](https://cdn.discordapp.com/attachments/565155651395780609/747039918563262504/for_alex2.zip)


torque feedback almost done

##### alxy 08/23/2020 12:11:52
Ok, I think I understand the code, but getTorqueFeedback() appears to always return nan? Why is that?

##### Luftwaffel [Moderator] 08/23/2020 12:12:24

> **Attachment**: [MirobotGripper\_controller.py](https://cdn.discordapp.com/attachments/565155651395780609/747065705785983046/MirobotGripper_controller.py)


line 15 and 16 were wrong


have to enable the sensors, was late last night ^^


should work now

##### alxy 08/23/2020 12:14:52
yes, nice 🙂


thanks a lot!


I just think about rewriting the controller so that the gripperPosTorqueLimited does not call supervisor.step(), will see if I can get it working...

##### Luftwaffel [Moderator] 08/23/2020 12:17:51
that is intentional


I guess you could add another function, that moves it closer to the target location for one increment

##### alxy 08/23/2020 12:19:22
I was thinking about a watcher pattern, that just monitors the torque at each step and fixes the positions to the current positions once a threshold is reached


I do something similar for the joint angles to determine if the robot is busy or idle

##### Luftwaffel [Moderator] 08/23/2020 12:20:19
you will have to call it every time though. the current implementation, you only have to call once


sorry, typo
> **Attachment**: [MirobotGripper\_controller.py](https://cdn.discordapp.com/attachments/565155651395780609/747070845209280572/MirobotGripper_controller.py)


this one

##### alxy 08/23/2020 13:34:47
will see tomorrow how I can integrate it with my code. I was jsut a bnit concerned about having blocking calls inside the main controller loop, as I have other things which have to continuously work inside the loop, like the tcp/ip server

##### Luftwaffel [Moderator] 08/23/2020 13:35:34
I added a function at the bottom, that only moves it incrementally, and you call the timestep in your main loop

##### alxy 08/23/2020 14:37:00
Minor issue, if the gripper only needs to move a very tiny bit, itnervals might be zero, hwich leads to an error (division by zero). I'll make sure to check if intervals > 0 🙂

##### Luftwaffel [Moderator] 08/23/2020 14:38:01
they should not be, as i'm using math.ceil(), which always rounds up


perhaps there needs to be an abs() in there.

##### alxy 08/23/2020 14:38:35
then probably pos\_end exactly matched pos\_start

##### Luftwaffel [Moderator] 08/23/2020 14:38:52
unlikely but possible

##### alxy 08/23/2020 14:38:56
if you call the function again with the exact same position


in your test code the sin always gives a different value for each time step


but if intervals > 0: really fixes the problem, so everything alright

##### Luftwaffel [Moderator] 08/23/2020 14:42:01
rahter add a min(1, ...)

##### alxy 08/23/2020 14:42:37
do you mean max()  ?

##### Luftwaffel [Moderator] 08/23/2020 14:42:46
oh yeah

##### alxy 08/23/2020 14:43:40
I have by the way now done it the following way. Added a target\_pos attribute to the class and the open and close actions set that, and in each iterationf og the controller loop I call this function: ``def run(self):

        self.gripperMoveIncrement(self.target\_pos, 2)``

##### Luftwaffel [Moderator] 08/23/2020 14:45:23
works too

##### alxy 08/23/2020 14:47:31
yeah like that I can just use your GripperXController class in my main controller and just pass the calls for the gripper to that instance 🙂

##### Dave 08/24/2020 10:05:38
Hello everyone,

I would need some guidance please.



I am currently simulating en EPUCK on Webots with Python.



As I understand, the ground sensors functions are only available on C language but not on Python because it is an "extension" and it is not implemented in the API.



[https://cyberbotics.com/doc/guide/epuck](https://cyberbotics.com/doc/guide/epuck)



`In particular, the ground sensors module extension of the real e-puck robot is modelled using the "E-puckGroundSensors.proto" PROTO in Webots to provide 3 optional infra-red sensors pointing to the ground in front of the robot. `



How can I add the python functions for the ground sensors in my code please ? 



Thank you in advance for your help !



I have been searching for a solution, but it seems that I need to go trough PROTO ? Not sure what is Proto

##### David Mansolino [Cyberbotics] 08/24/2020 10:06:29
HI `@Dave`, the ground sensor is available for Python too in simulation.


Here is an introduction about PROTO: [https://cyberbotics.com/doc/reference/proto](https://cyberbotics.com/doc/reference/proto)

##### Dave 08/24/2020 10:06:57
I don't understand how I can import in Python 😓

##### David Mansolino [Cyberbotics] 08/24/2020 10:08:44
In any language, but you have to make sure that the 'groundSensorsSlot' field contains an 'E-puckGroundSensors' node.


Then they names of the ground sensors devices are 'gs0', 'gs1' and 'gs2'


they can be get the same way as the other distance sensors.

##### Dave 08/24/2020 10:10:03
Ok thank you! I will try again and be more persistence!


Thank you! At least I know that it is already implemented 😁

##### David Mansolino [Cyberbotics] 08/24/2020 10:10:33
You're welcome

##### alxy 08/24/2020 16:35:04
lol, I now tried to figure out why the gripper isnt working in the greater setup for like an hour. Turns out I have used an old version of he gripper there without the Physics node, meaning torque is alsways nan


interestingly, it does not throw an error when compared to a number

##### Luftwaffel [Moderator] 08/24/2020 16:36:55
doh

##### alxy 08/24/2020 16:42:50
Yeah, adding it an everything works as expected. I was really starting to become desperate 😄


`@Luftwaffel` Whoops, I added the physics nodes and now I see the following phenomenon when moving the arm. What could that be?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/747497736135376906/unknown.png)
%end

##### Luftwaffel [Moderator] 08/24/2020 16:49:32
what model are you using?

##### alxy 08/24/2020 16:50:01
The second one from yesterday, I just made the changes to the Solid proto node as well (adding the physics nodes)

##### Luftwaffel [Moderator] 08/24/2020 16:50:10
i added physics nodes

##### alxy 08/24/2020 16:50:10
As they were only present in the Robot proto

##### Luftwaffel [Moderator] 08/24/2020 16:50:32
did you convert it to base nodes?

##### alxy 08/24/2020 16:51:06
nope, I copied the children attribute of the robot node directly to the other proto file


[https://github.com/alxy/webots-mirobot/commit/209eae53e1233c5e59491447de03029aeb713093#diff-5f0cbd16163d674dc1f6dd994a25cf05](https://github.com/alxy/webots-mirobot/commit/209eae53e1233c5e59491447de03029aeb713093#diff-5f0cbd16163d674dc1f6dd994a25cf05)


ver weird that the preview is my own portrait

##### Luftwaffel [Moderator] 08/24/2020 16:55:07

> **Attachment**: [MirobotGripper.zip](https://cdn.discordapp.com/attachments/565155651395780609/747499242016473188/MirobotGripper.zip)


both, the robot and the solid version have physics nodes


The issue with the meshes is, they are all in reference to the origin of the wrist mesh, so I needed to add transform nodes before the mesh nodes

##### alxy 08/24/2020 16:56:46
Thanks! I will check this version out


mh, same issue. It contains the same file I was using, just with one additional max position set

##### Luftwaffel [Moderator] 08/24/2020 16:59:53
send me your project as a zip

##### alxy 08/24/2020 17:01:03
[https://github.com/alxy/webots-mirobot/archive/master.zip](https://github.com/alxy/webots-mirobot/archive/master.zip)


The controller wont work as it has a lot of dependencies, but the same happens when just using the manual motor control


mirobot\_gripper\_original is my world, I need to definetly clean up the projext once it is working, sorry for that


I can create a more minimal example if you want

##### Luftwaffel [Moderator] 08/24/2020 17:03:31

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/747501356654526505/unknown.png)
%end

##### alxy 08/24/2020 17:03:53
as said, just use void controller

##### Luftwaffel [Moderator] 08/24/2020 17:04:42
looks fine to me

##### alxy 08/24/2020 17:05:27
so if you move joint1 (which rotates the base) the gripper moves along with the arm?

##### Luftwaffel [Moderator] 08/24/2020 17:05:40
ohh I see


it does, but turning wrist of gripper screws it

##### alxy 08/24/2020 17:06:07
I think its the wrist joint somehow not connected to the rest of the gripper

##### Luftwaffel [Moderator] 08/24/2020 17:06:42
lol


lemme investigate


fixed it. I removed the extra layered Solid node (now wrist is first solid) and added a physics node to it
> **Attachment**: [Mirobot.proto](https://cdn.discordapp.com/attachments/565155651395780609/747506528566902874/Mirobot.proto)

##### alxy 08/24/2020 17:26:04
So you needed to change the Mirobot?


Not the gripper?

##### Luftwaffel [Moderator] 08/24/2020 17:26:13
wups


wrong file



> **Attachment**: [MirobotGripper.proto](https://cdn.discordapp.com/attachments/565155651395780609/747507112984313996/MirobotGripper.proto)


😅

##### alxy 08/24/2020 17:27:24
Yeah I have the same issue, way too many files with the same or similar names 😄


uh yeah, very nice, a big thanks again! Definetly worth at least one beer, if we ever meet in person 🙂

##### Luftwaffel [Moderator] 08/24/2020 17:31:18
heh, I'll hold you to it 😉


but interesting project, learned a lot doing it

##### alxy 08/24/2020 17:33:21
I eventually want to use it for my job, as we have the mirobot there, but its really time consuming to test the control code. Right now its only a weekend/after work project, but I hope it will really help to speed up development


Looks very promising now

##### Luftwaffel [Moderator] 08/24/2020 18:07:07
take a look at this
> **Attachment**: [Mirobot\_noAttachment.zip](https://cdn.discordapp.com/attachments/565155651395780609/747517360113451048/Mirobot_noAttachment.zip)

##### Dave 08/24/2020 19:29:54
> You're welcome

`@David Mansolino` Finally did it (haha thank you again for the help !) Indeed I did not added in the field of the groundSensorSlot.



Wish you a nice evening !

##### David Mansolino [Cyberbotics] 08/25/2020 05:14:42
`@Dave` you're welcome, have a nice day too.

##### Luftwaffel [Moderator] 08/25/2020 12:56:20
Is there a reason that the inertia matrix from a urdf file does not get used in the converter? I've noticed that no .proto model has those values. Instead it get's calculated from the boundingObject, which, in almost all cases, is a box.


I tested it, and the calculated inertia matrices are quite different from the ones in the urdf file.  It might not be an issue for pure simulation, but I think for the transfer of Sim-to-real, this could be a factor


But you might have additional info that i'm missing

##### David Mansolino [Cyberbotics] 08/25/2020 13:05:19
It seems th einertia get parsed here: [https://github.com/cyberbotics/urdf2webots/blob/master/urdf2webots/parserURDF.py#L601](https://github.com/cyberbotics/urdf2webots/blob/master/urdf2webots/parserURDF.py#L601)


But then there is a problem when exporting the Physics: [https://github.com/cyberbotics/urdf2webots/blob/master/urdf2webots/writeProto.py#L158](https://github.com/cyberbotics/urdf2webots/blob/master/urdf2webots/writeProto.py#L158)

##### Luftwaffel [Moderator] 08/25/2020 13:07:08
exactly, that's the area I found

##### David Mansolino [Cyberbotics] 08/25/2020 13:07:19
It seems there is a misunderstanding there betwen center of mass and inertia

##### Luftwaffel [Moderator] 08/25/2020 13:08:07
inertia is never actually added

##### David Mansolino [Cyberbotics] 08/25/2020 13:08:07
This seems indeed to be a bug (not sure if it was always like this or if a wrong conflict merge introduced it).


Yes and it is used to check if center of mass should be set (which is wrong).

##### Luftwaffel [Moderator] 08/25/2020 13:08:35
I found it due to the error message, when the inertia had 'rpy'


in that case it could just be multiplied by the rotation matrix derived from the rpy I think


I might make a PR later if I have some time

##### David Mansolino [Cyberbotics] 08/25/2020 13:13:31
Ok, thank you

##### Luftwaffel [Moderator] 08/25/2020 16:08:56
`@David Mansolino` I implemented the fix, you can try it out with this urdf file. [https://github.com/cyberbotics/urdf2webots/pull/76](https://github.com/cyberbotics/urdf2webots/pull/76)
> **Attachment**: [UR10e\_urdf.zip](https://cdn.discordapp.com/attachments/565155651395780609/747850006773366814/UR10e_urdf.zip)

##### David Mansolino [Cyberbotics] 08/26/2020 05:24:39
Perfect, I will try it, thank you

##### Luftwaffel [Moderator] 08/26/2020 12:04:18
Hey, I want your opinion. If no mass or inertia is specified, wouldn't it be better to use a density calculation instead of assuming a mass of 1 kg? Especially for very small or very large objects, this seems like a much better approach

##### David Mansolino [Cyberbotics] 08/26/2020 12:20:32
That sounds good indeed.

##### Olivier Michel [Cyberbotics] 08/26/2020 12:21:18
Sure, by default Webots assumes a density of 1000 kg/m^3 for objects (based on the volume of the bounding object). This is default value of the Physics node.


Maybe you can re-use this default value?

##### Luftwaffel [Moderator] 08/26/2020 12:25:19
Okay so this gets into specific cases, where urdf-links have collision values, but no inertia tag. We dont want empty links (which are common) to get a physics node


So if collision exists, and mass == None, we add an empty physics node, which should calculate the mass of the bounding object with a default density of 1000 right?

##### Olivier Michel [Cyberbotics] 08/26/2020 12:26:36
Yep.

##### Luftwaffel [Moderator] 08/26/2020 12:27:01
and the center of Mass gets automatically taken from the bounding box, or does it have to be specified?

##### Olivier Michel [Cyberbotics] 08/26/2020 12:28:23
No, if left empty, the center of mass will be automatically computed from the bounding object.

##### Luftwaffel [Moderator] 08/26/2020 12:28:33
ok great

##### Olivier Michel [Cyberbotics] 08/26/2020 12:29:00
This is documented here: [https://cyberbotics.com/doc/reference/physics](https://cyberbotics.com/doc/reference/physics)

##### Luftwaffel [Moderator] 08/26/2020 12:29:19
yeah read that yesterda, just double checking 🙂


`@David Mansolino` consolidated all the changes [https://github.com/cyberbotics/urdf2webots/pull/79](https://github.com/cyberbotics/urdf2webots/pull/79)


[https://github.com/cyberbotics/urdf2webots/pull/76](https://github.com/cyberbotics/urdf2webots/pull/76) this should be done right?


reverted the .gitignore change

##### David Mansolino [Cyberbotics] 08/27/2020 14:47:14
👍 checking it right now.


Perfect, accepted!

Once merged it would be nice to update [https://github.com/cyberbotics/urdf2webots/pull/74](https://github.com/cyberbotics/urdf2webots/pull/74) with master as this branch is getting a bit out of sync.

##### Luftwaffel [Moderator] 08/27/2020 14:49:53
will do


`@Olivier Michel`  quick question: how to do you transform rotation matrices into axis angles in webots? Does it by any chance use the same math\_util function as urdf2webots? Because I found it to be faulty yesterday in certain circumstances

##### Olivier Michel [Cyberbotics] 08/27/2020 17:03:14
No, it's not a Python, but a C++ method doing this. So, it's likely not the same. But I will check your fix against our C++ method. Did you already publish it somewhere?

##### Luftwaffel [Moderator] 08/27/2020 17:04:45
line 110 is the old one, line 120 my implementation
> **Attachment**: [math\_utils.py](https://cdn.discordapp.com/attachments/565155651395780609/748588831690981507/math_utils.py)


in some edge cases the current implementation produced wrong values


rotationFromMatrix([-1, 0, 0, 0, 0, 1, 0, 1, 0]) 

this was my test, or where I noticed an issue


axis angle should be:

{ [ 0, 0.7071068, 0.7071068 ], 3.1415927 }

##### Olivier Michel [Cyberbotics] 08/27/2020 17:20:11
Thanks. I'll check it tomorrow.

##### Luftwaffel [Moderator] 08/27/2020 17:20:32
no problem 🙂


just something I found in a ridiculous project


can be Webots launched with the ExtraProjectPath set as a variable?

##### David Mansolino [Cyberbotics] 08/27/2020 18:23:37
> can be Webots launched with the ExtraProjectPath set as a variable?

`@Luftwaffel` no, but since this is saved in the settings file, you might edit this file from within a script (on Linux, this file is located at `$HOME/.config/Cyberbotics`).

##### Luftwaffel [Moderator] 08/27/2020 18:24:28
ah good to know. FYI, just pushed the batch-conversion if you're bored. and seperately pushed the sources to community-projects

##### David Mansolino [Cyberbotics] 08/27/2020 18:33:49
Perfect, I will have a look tomorrow (it is already late here 😉).

##### Luftwaffel [Moderator] 08/27/2020 18:34:23
I'm currently in Germany, so yeah I know ;), hence the 'if you're bored'

##### Olivier Michel [Cyberbotics] 08/28/2020 07:14:55
`@Luftwaffel`: so we have something quite different from your implementation (in webots/src/webots/maths/WbRotation.cpp):

```C++
void WbRotation::fromMatrix3(const WbMatrix3 &M) {
  // Reference: https://www.geometrictools.com/Documentation/RotationRepresentations.pdf

  const double theta = acos((M(0, 0) + M(1, 1) + M(2, 2) - 1) / 2);

  if (theta < WbPrecision::DOUBLE_EQUALITY_TOLERANCE) {
    // If `theta == 0`
    mX = 1;
    mY = 0;
    mZ = 0;
    mAngle = 0;
  } else if (theta < M_PI - WbPrecision::DOUBLE_EQUALITY_TOLERANCE) {
    // If `theta in (0, pi)`
    mX = M(2, 1) - M(1, 2);
    mY = M(0, 2) - M(2, 0);
    mZ = M(1, 0) - M(0, 1);
    normalizeAxis();
    mAngle = theta;
  } else {
    // If `theta == pi`
    if (M(0, 0) > M(1, 1) && M(0, 0) > M(2, 2)) {
      mX = sqrt(M(0, 0) - M(1, 1) - M(2, 2) + 0.5);
      mY = M(0, 1) / (2 * mX);
      mZ = M(0, 2) / (2 * mX);
      mAngle = theta;
    } else if (M(1, 1) > M(0, 0) && M(1, 1) > M(2, 2)) {
      mY = sqrt(M(1, 1) - M(0, 0) - M(2, 2) + 0.5);
      mX = M(0, 1) / (2 * mY);
      mZ = M(1, 2) / (2 * mY);
      mAngle = theta;
    } else {
      mZ = sqrt(M(2, 2) - M(0, 0) - M(1, 1) + 0.5);
      mX = M(0, 2) / (2 * mZ);
      mY = M(1, 2) / (2 * mZ);
      mAngle = theta;
    }
  }
}
```


Well, I am not getting the same results as you:

```c++
  WbMatrix3 m(-1, 0, 0,
               0, 0, 1,
               0, 1, 0);
  WbRotation r(m);
  qDebug() << r.toString(WbPrecision::DOUBLE_MAX);  // "0 0.4082482904638631 1.224744871391589 3.141592653589793"
```


According to [https://www.andre-gaschler.com/rotationconverter](https://www.andre-gaschler.com/rotationconverter), your result seems correct.

##### David Mansolino [Cyberbotics] 08/28/2020 08:22:48
> ah good to know. FYI, just pushed the batch-conversion if you're bored. and seperately pushed the sources to community-projects

`@Luftwaffel` I don't see your PR on community-projects.

##### Luftwaffel [Moderator] 08/28/2020 09:08:45
That is exactly how I stumbled across this issue. It is the rotation to rotate a cylinder that is defined with the z-axis as it's height, so it points into the y-axis direction.  It most likely is rarely an issue, as webots uses rotation matrices I'd assume

##### Olivier Michel [Cyberbotics] 08/28/2020 09:09:06
Thanks `@Luftwaffel`: we could fix a bug in our C++ matrix3 to axis-angle thanks to you! See [https://github.com/cyberbotics/webots/pull/2155](https://github.com/cyberbotics/webots/pull/2155)

##### Luftwaffel [Moderator] 08/28/2020 09:09:11
However, when converting the axis angles back to rotation matrix, it is different to the original one


Happy I could be of help. Really getting good at troubleshooting and linear algebra diving into this stuff 😄


should I make a PR for the urdf2webots?


the math\_utils.py

##### Olivier Michel [Cyberbotics] 08/28/2020 09:12:46
Yes, please.

##### Luftwaffel [Moderator] 08/28/2020 10:07:13
Is there an automated tool to make code pep8 compliant?

##### David Mansolino [Cyberbotics] 08/28/2020 10:07:51
Probably, but I am personnally not using it, but I am using a linter just to display where my code is not pep8 compliant

##### Luftwaffel [Moderator] 08/28/2020 10:08:28
linter?

##### David Mansolino [Cyberbotics] 08/28/2020 10:09:39
Here are the instructions if you use atom editor (probably similar linter exists for other editors): [https://github.com/cyberbotics/webots/wiki/Python-Coding-Style#cs100-use-pep8](https://github.com/cyberbotics/webots/wiki/Python-Coding-Style#cs100-use-pep8)

##### Luftwaffel [Moderator] 08/28/2020 10:10:07
thx, I'll have a look

##### David Mansolino [Cyberbotics] 08/28/2020 10:10:14
A linter just hightlights issue in your code while you are writting

##### Luftwaffel [Moderator] 08/28/2020 11:33:19
How do I resolve merge conflicts. I just wanna take the master, and change to files and push that to my  branch. I pulled the master, checked out my branch and did 'git merge master'. But that creates conflicts


managed to do it with some extra steps. The batch conversion should no be 'clean' and ready to test:

[https://github.com/cyberbotics/urdf2webots/pull/74](https://github.com/cyberbotics/urdf2webots/pull/74)

##### David Mansolino [Cyberbotics] 08/28/2020 11:51:25
> How do I resolve merge conflicts. I just wanna take the master, and change to files and push that to my  branch. I pulled the master, checked out my branch and did 'git merge master'. But that creates conflicts

`@Luftwaffel` [https://www.atlassian.com/git/tutorials/using-branches/merge-conflicts](https://www.atlassian.com/git/tutorials/using-branches/merge-conflicts) 😉

##### Luftwaffel [Moderator] 08/28/2020 13:09:25
The automatic-conversion pull request failes the check with this error:

[https://travis-ci.com/github/cyberbotics/urdf2webots/jobs/378767198](https://travis-ci.com/github/cyberbotics/urdf2webots/jobs/378767198)



I dont quite understand what that means

##### David Mansolino [Cyberbotics] 08/28/2020 13:11:47
The test is converting the files in the source directory ([https://github.com/cyberbotics/urdf2webots/tree/master/tests/sources](https://github.com/cyberbotics/urdf2webots/tree/master/tests/sources)) with these arguments: [https://github.com/cyberbotics/urdf2webots/blob/master/tests/test\_export.py#L13-L34](https://github.com/cyberbotics/urdf2webots/blob/master/tests/test_export.py#L13-L34)

And comparing them with the references: [https://github.com/cyberbotics/urdf2webots/tree/master/tests/expected](https://github.com/cyberbotics/urdf2webots/tree/master/tests/expected)

If something difeers, it will then fail.


I saw that you managed to fix the merge issue 🙂


Maybe can we speak about this part here?

[https://github.com/cyberbotics/urdf2webots/pull/74/files#diff-68c204ae057e777029c1e7dbd77d1f03L157-L162](https://github.com/cyberbotics/urdf2webots/pull/74/files#diff-68c204ae057e777029c1e7dbd77d1f03L157-L162)

##### alxy 08/28/2020 13:25:55
`@Luftwaffel` Regarding git workflow, I do sync with the remote (pull), create new feature branch from remote/master, do changes, commit, push

##### Luftwaffel [Moderator] 08/28/2020 13:40:40
`@alxy` the issue was, that the master changed, since I did that

##### alxy 08/28/2020 13:57:28
[https://imgs.xkcd.com/comics/git.png](https://imgs.xkcd.com/comics/git.png)


It's shockingly true

##### Luftwaffel [Moderator] 08/28/2020 15:46:08
Any idea how I could only get the urdf of the robot arm, without including things  attached in the toolSlot field?


i'm so far that I can get the node added:

toolSlotNode = self.supervisor.getSelf().getField('toolSlot').getMFNode(0)


but since it's not a robot node, I cant get a urdf from it, nor can I get devices


is there a way to copy nodes, remove them and then insert them again? I got so far as to removing them, but how can I insert a node


Can I somehow convert a node to string? So I can insert with string

##### alxy 08/29/2020 11:37:53
I'd say you need an optional aprameter in your ik\_module to pass a list of joints/motors you want in the chain


You can then just create the entire chain from the urdf from the supervisor, and later remove the joints that dont actually belong to the arm by comparing to that passed list of names

##### Luftwaffel [Moderator] 08/29/2020 13:55:03
The issue is, that some inlcude a end link, and some don't

##### alxy 08/29/2020 14:09:48
yeah, thats why I mean make it optional, if it is None for example use all joints


else, use the ones specified


I mean there might even be robots where an arm is mounted on top of some kind of platform with wheel, to move around, so that would introduce even more motors

##### Luftwaffel [Moderator] 08/30/2020 18:39:31
Do you guys have a benchmark to test collision detection performance?

##### David Mansolino [Cyberbotics] 08/31/2020 05:59:12
Hi `@Luftwaffel`, not really (or at least not maintained). However, you might fidn in our CI some tests checking that the behavior of the physics and the collisions is the expected one: [https://github.com/cyberbotics/webots/tree/master/tests/physics/worlds](https://github.com/cyberbotics/webots/tree/master/tests/physics/worlds)

##### Luftwaffel [Moderator] 08/31/2020 10:38:19
Thank you


Is there any work on adding more native support for motion planning? Something like OMPL?

##### David Mansolino [Cyberbotics] 08/31/2020 10:40:27
I am currently investigating the use of IKFast for a private project (the current result doesn't look very promising), if I have something nice I will let you know.

##### Luftwaffel [Moderator] 08/31/2020 10:40:45
I was looking into that 1-2 weeks ago

##### David Mansolino [Cyberbotics] 08/31/2020 10:40:58
OMPL is indeed the next alternative I will try if I can't have nice result with IKFast

##### Luftwaffel [Moderator] 08/31/2020 10:41:14
I managed to compile it with docker, but not getting it to run with a wrapper


how far are you with IKFast?

##### David Mansolino [Cyberbotics] 08/31/2020 10:42:23
Currently it fails generating the solution for my model after hours of computation (I am still not 100% sure the problem is on my model definition or just that IKFast is not able to find a solution for this kind of robot).

##### Luftwaffel [Moderator] 08/31/2020 10:42:59
I had that issue too... gosh I dont remember what I did to fix it


hold on, lemme try to find it


[http://docs.ros.org/kinetic/api/moveit\_tutorials/html/doc/ikfast/ikfast\_tutorial.html](http://docs.ros.org/kinetic/api/moveit_tutorials/html/doc/ikfast/ikfast_tutorial.html)


the part here:

Often floating point issues arise in converting a URDF file to Collada file, so a script has been created to round all the numbers down to x decimal places in your .dae file.


rounding the decimal places. That did the trick

##### David Mansolino [Cyberbotics] 08/31/2020 10:46:54
Thank you, unfortunately, I am following this exact same tutorial and already did the rounding trick, but it doesn't seem to fix the problem (but my arm is not 'conventional' as it contains a slider joint, I suspect this is causing issues to the solver).

##### Luftwaffel [Moderator] 08/31/2020 10:49:14
that might be. Never used IK with sliding joints. Is it supposed to be supported?


what error are you getting? I got "maximum recursion depth reached"

##### David Mansolino [Cyberbotics] 08/31/2020 10:51:23
> that might be. Never used IK with sliding joints. Is it supposed to be supported?

`@Luftwaffel` from a theoretical point a view, slider should be simpler to solve than rotational joints, but since they are less commun I suspect they are less supported, but IKFast doesn't complains about it, it just fails with something like 'Could not resolve the system' (don't remember exactly the error message, I will in any case retry later this week).

##### Luftwaffel [Moderator] 08/31/2020 10:53:09
let me know if you make any progress. Especially implementing and using the compiled file would be very interesting. There is a python wrapper library, but I failed to understand how to add my own model to it


I used it in the past with the already included irb planner, and successfully controlled the arm in webots. This would be a fantastic solution and addition, if we manage to establish an implementation workflow

##### David Mansolino [Cyberbotics] 08/31/2020 11:50:35
> I used it in the past with the already included irb planner, and successfully controlled the arm in webots. This would be a fantastic solution and addition, if we manage to establish an implementation workflow

`@Luftwaffel` was the motion better than with IKPY ? Especially, was the orientation working fine?


> let me know if you make any progress. Especially implementing and using the compiled file would be very interesting. There is a python wrapper library, but I failed to understand how to add my own model to it

`@Luftwaffel` Yes sure, currently we are preparing the release if Webots R2020b-rev1, I am quite busy with this, but right after the release I will continue on this topic.

##### Luftwaffel [Moderator] 08/31/2020 12:22:41
I got ikpy rotation to work with my latest iteration of the generic IK controller. I cannot make a good judgement about accuracy, however, the speed difference is HUGE. ikpy you cannot run at high frequency. IKFast is very fast

##### David Mansolino [Cyberbotics] 08/31/2020 12:23:11
Yes that's indeed what I read that IKFast is much more efficient.

##### Luftwaffel [Moderator] 08/31/2020 12:23:57
it is a analytical instead of numerical solver. The big drawback is, that every robot and task needs to be compiled

##### David Mansolino [Cyberbotics] 08/31/2020 12:24:39
Yes, for making something generic this is indeed a huge drawback!

##### Luftwaffel [Moderator] 08/31/2020 12:26:24
[https://github.com/sebastianstarke/BioIK](https://github.com/sebastianstarke/BioIK) this looks interesting as well... it just seems so difficult to find a proper ik solution, that can be implemented without moveit


perhaps moveIt 2 with ROS2 could be implemented with webots.

##### David Mansolino [Cyberbotics] 08/31/2020 12:32:38
Yes I saw indeed that there are many many alternative to implement IK.


Unfortunately, I would like to avoid using ROS/ROS2 in my project. But for sure integrating MoveIt2 is on the roadmap (once it is out of beta).

##### Luftwaffel [Moderator] 08/31/2020 12:35:14
what are some alternatives?

##### David Mansolino [Cyberbotics] 08/31/2020 12:37:14
IKPY, IKFAST, TheComet/ik, OMPL, KDL, etc.

##### Luftwaffel [Moderator] 08/31/2020 12:41:02
Comet is new to me, gonna check it out

##### David Mansolino [Cyberbotics] 08/31/2020 12:41:56
> Comet is new to me, gonna check it out

`@Luftwaffel` not sure about the maturity of this one, I haven't tested it yet.

##### Luftwaffel [Moderator] 08/31/2020 12:41:57
but it's not just the solvers, the implementation has to be doable too. There ikpy is 'simple', even if the documentation is very confusing and misleading at times


it uses the Fabrik solver, reading up on it right now 🙂

##### David Mansolino [Cyberbotics] 08/31/2020 12:42:18
Yes, in the ease of use, ikpy is clearly the best

##### Luftwaffel [Moderator] 08/31/2020 12:42:32
comet seems to be similar to ikpy, but for C


Do you guys have more detailed documentation on your multithreading work for ODE? Was it based on this paper?

[https://link.springer.com/chapter/10.1007/978-3-642-23397-5\_20](https://link.springer.com/chapter/10.1007/978-3-642-23397-5_20)

I went into a deep rabbit hole yeserday, reading countless papers till 2am in the morning, around collision detection, mesh optimizations and anything related. I'm curious as to how your branch of ODE differs. Did you parallelize the collision detection portion? The dynamics simulation? both, or even more than that? Also I'm curious as to how collisions are parsed to ODE. From what `@Olivier Michel` told me, I gather that you use hierachical (nested) spaces.


From my paper-reading spree yesterday, I concluded that there are several different options on how to structure collision detection, and that this can have large performance impacts. For example one method can be very effective and fast when having nested collision spaces with the same number of levels and approximately same numper of objects / complexity per level. But the same method can be slow or imprecise when comparing spaces of different depth or complexity


If you have some reading or implementations or documentations on your side you can point me too, I'd be appreciative

##### Olivier Michel [Cyberbotics] 08/31/2020 13:14:11
I am afraid there is little documentation about it. However, it was performed during a master project if I remember well, so there might be a master thesis about it...

##### Luftwaffel [Moderator] 08/31/2020 13:15:42
you wouldnt happen to have it? or the name of the author?

##### Olivier Michel [Cyberbotics] 08/31/2020 13:16:26
Basically, the idea behind parallelization was simply to create islands of objects which are mechanically isolated from each other (no contact, no joint). And to run these islands in parallel. However these islands have to be dynamically updated as the simulation evolves and object may move from one island to another.


I will send you the report privately.

##### Luftwaffel [Moderator] 08/31/2020 13:17:02
okay, thank you 🙂

##### alxy 08/31/2020 13:42:31
Wouldnt have thought inverse kinematics is such a hard problem

##### Luftwaffel [Moderator] 08/31/2020 15:05:29
I GOT IKFAST RUNNING ON THE UR10E!!!!!


when computing an ik-solution every 10 timesteps, ikpy runs at 0.9-1.2 realtime factor, ikfast at 90-120x!!!!!

##### David Mansolino [Cyberbotics] 08/31/2020 15:09:56
Oh cool!!

##### Luftwaffel [Moderator] 08/31/2020 15:11:47
once things are compiled, the implementation is actually way easier than ikpy


but getting there... oh boy


okay, ikfast isn't even the bottleneck anymore. now it runs at 185x . The only thing I changed, was instead of creating a new transformation matrix every time, I just init it once, and override it with each call.... wow

##### David Mansolino [Cyberbotics] 08/31/2020 15:20:37
Yes indeed at this speed the Webots step is probably the bootlneck

##### Luftwaffel [Moderator] 08/31/2020 15:23:11
Any request for a robotic arm, to do next? Gotta create a workflow that can be reproduced

##### alxy 08/31/2020 15:24:45
is it using a c++ controller then or does it work from python as well?

##### Luftwaffel [Moderator] 08/31/2020 15:25:06
I got it to work through python

##### alxy 08/31/2020 15:25:14
nice work

##### Luftwaffel [Moderator] 08/31/2020 15:26:09
this is all it takes to implement it
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/750013567855493251/unknown.png)
%end


There might be some work to be done, which solution to pick (it returns all possible solutions)

##### alxy 08/31/2020 15:27:43
not bad, and it handels orientation and position correctly and fast?

##### Luftwaffel [Moderator] 08/31/2020 15:27:51
yes

## September


so I made a ikfast controller for ur3, 5 and 10e


7DOF kuka failed. But I could make it for every 6DOF arm


it always requires a compiled file, the rest of the ik controller for webots can be reused


how would you guys like it for an official implementation?


have it seperate for every supported robot?


or have a general one, that picks the correct file when available. And perhaps use ikpy when not

##### David Mansolino [Cyberbotics] 09/01/2020 14:34:41
Very nice, I think personnally that having it for one of the ur robot along with a small README file explaining how you did generate it (so that users can reproduce this for other robots) would be very nice and usefull!

##### Luftwaffel [Moderator] 09/01/2020 14:36:33
The process of generating these files is not easy though. I think providing it for the already supported ones would be great


If anyone wants to test the ikfast implementation. Let me know if it works out of the box
> **Attachment**: [ur10e\_ik.zip](https://cdn.discordapp.com/attachments/565155651395780609/750399802331758684/ur10e_ik.zip)


pip install numpy Cython


you might have to install those two packages


`@alxy` can you test this world?

##### alxy 09/01/2020 18:59:05
sorry, want do anything more today :p


*wont

##### Luftwaffel [Moderator] 09/01/2020 18:59:24
okay, tomorrow perhaps 🙂


okay, so I got velocity control to work quite nicely as well


no accumulated error, only moves if valid (no flipping wrists etc)


I think building up a proper demo would be nice


keyboard control would be nice


Is there a good example for controlling your robot via keyboard?

##### David Mansolino [Cyberbotics] 09/02/2020 14:28:33
You can check for example this one: [https://cyberbotics.com/doc/guide/samples-demos#moon-wbt](https://cyberbotics.com/doc/guide/samples-demos#moon-wbt)

##### Luftwaffel [Moderator] 09/02/2020 14:50:13
How much work would it be, to add visual export to the urdf-export?


I tried compiling an ikfast solver for  a webots model, using the extracted urdf. But I think openrave doesnt handle multiple collision cylinders and boxes per link that well


A visual trimesh could be converted to a convex collision mesh. There is easy to use libraries for that. But for that I need those meshes

##### David Mansolino [Cyberbotics] 09/02/2020 14:58:59
Exporting the full visual part of the model to URDF is complex in the sense that it requires exporting the IndexedFaceSet as separated stl or collada files, but of course this is feasible.

##### Luftwaffel [Moderator] 09/02/2020 15:00:15
that's kinda what I thought. urdf files usually link to mesh files anyways

##### David Mansolino [Cyberbotics] 09/02/2020 15:00:35
Exactly.

##### Luftwaffel [Moderator] 09/02/2020 15:44:46
Can you link me to the urdf-exporter code?

##### David Mansolino [Cyberbotics] 09/02/2020 15:45:58
This is unfortunately deep inside the core of Webots, but let me find you a pointer

##### Luftwaffel [Moderator] 09/02/2020 15:46:05
thx

##### David Mansolino [Cyberbotics] 09/02/2020 15:48:18
Here is how the current visual is exported using the definition of the bounding objects: [https://github.com/cyberbotics/webots/blob/master/src/webots/nodes/WbSolid.cpp#L3012](https://github.com/cyberbotics/webots/blob/master/src/webots/nodes/WbSolid.cpp#L3012)

I would recommend starting by this part.

##### Luftwaffel [Moderator] 09/02/2020 15:51:17
thx, i'll have a look

##### David Mansolino [Cyberbotics] 09/02/2020 15:51:59
You're welcome

##### Luftwaffel [Moderator] 09/02/2020 19:26:46
is it possible to export a .wrl with just the selected object in it?

##### David Mansolino [Cyberbotics] 09/03/2020 05:46:22
This is unfortunately not possible, but if you are interested, this should be quite simple to implement.

##### Luftwaffel [Moderator] 09/03/2020 11:39:23
can you point me to that portion of code?

##### David Mansolino [Cyberbotics] 09/03/2020 11:41:11
Sure, let me look for it


Actually, I just found that this is possible 😂 

You have to right click on the node either in the scene-tree either in the 3D view, then change the extension of the filename to save to '.wrl', and that's it 🙂

##### Luftwaffel [Moderator] 09/03/2020 11:57:36
haha that actually works 😄


can you still link me to the code?

##### Stefania Pedrazzi [Cyberbotics] 09/03/2020 13:58:49
here is the export method:

[https://github.com/cyberbotics/webots/blob/master/src/webots/scene\_tree/WbSceneTree.cpp#L1464-L1497](https://github.com/cyberbotics/webots/blob/master/src/webots/scene_tree/WbSceneTree.cpp#L1464-L1497)


`@David Mansolino` is improving it to add the wrl extension support: [https://github.com/cyberbotics/webots/pull/2201](https://github.com/cyberbotics/webots/pull/2201)

##### Luftwaffel [Moderator] 09/03/2020 13:59:41
oh boy, at some point I really need to get into c++ properly


i'm now writing a python script, which will hopefully extract the shapes from a vrml file, and convert them to collada for example, and create a convex hull for the individual parts


with that, it is hopefully possible to compile a ikfast solver


or do you guys know of any way to convert a .wrl file into a ORDERED structure of meshes? Everything I found so far (for example blender), does not keep the structure, so all the meshes are on the same level with usually arbitrary names. hard to tell, which shape belongs to which part

##### David Mansolino [Cyberbotics] 09/03/2020 14:13:39
> oh boy, at some point I really need to get into c++ properly

`@Luftwaffel` for the core of webots, it is indeed better to have some good C++ knowledge 😉


> or do you guys know of any way to convert a .wrl file into a ORDERED structure of meshes? Everything I found so far (for example blender), does not keep the structure, so all the meshes are on the same level with usually arbitrary names. hard to tell, which shape belongs to which part

`@Luftwaffel` I am surprised that blender does not support structure. It may not support joints indeed, but it should at least supper hierarchy.

##### Luftwaffel [Moderator] 09/03/2020 14:17:04
I have like 0 blender experience, but this looks like it's all on the same level. Perhaps one of you can point me in the right direction?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/751083346427379762/unknown.png)
%end

##### David Mansolino [Cyberbotics] 09/03/2020 14:17:23
Indeed, it's all at the same level

##### Luftwaffel [Moderator] 09/03/2020 14:17:32
basically I want to export the meshes in groups, corresponding to individual solids


or merge them, doesnt matter

##### David Mansolino [Cyberbotics] 09/03/2020 14:17:50
let me check if the problem is at the webots export level or not

##### Luftwaffel [Moderator] 09/03/2020 14:23:29
CAD Exchange reads the structure fine, but it's only a 30day trial period 😦

##### David Mansolino [Cyberbotics] 09/03/2020 14:26:38
I think you can use the 'limited' version for unlimited time

##### Luftwaffel [Moderator] 09/03/2020 14:26:53
really?

##### David Mansolino [Cyberbotics] 09/03/2020 14:26:55
I am using it for a few months at least

##### Luftwaffel [Moderator] 09/03/2020 14:26:58
I love that software

##### David Mansolino [Cyberbotics] 09/03/2020 14:27:15
I am speaking about the online version: [https://cloud.cadexchanger.com/app/files/my](https://cloud.cadexchanger.com/app/files/my)

##### Luftwaffel [Moderator] 09/03/2020 14:34:46
oh, i installed one


let me check this one out


hmm the online version seems to also convert the whole thing, cant export individually

##### David Mansolino [Cyberbotics] 09/03/2020 14:44:16
Argh, that's a shame that the behavior is not the same in the native and online version 😦

##### Luftwaffel [Moderator] 09/03/2020 23:03:22
I managed to write a script, that extracts the trimeshes from a webots-export .wrl file, merges all meshes for a Solid together and creates a visual .dae file, and a convexHull   .stl file for collision. I checked with the ur10e, and the converted files are pretty much identical to the ones from Universal Robots


I made it... successfully compiled ikfast-solver purely from webots exports (.wrl and .urdf combined)


🥳


now I really need some sleep xD


You guys should really include a dropdown option when selecting 'Export' in webots. I tested it and changing the extension to .urdf already works flawlessly. People just need to know it 🙂

##### David Mansolino [Cyberbotics] 09/04/2020 09:52:20
This is what I did yesterday, will be able in the next version of Webots: [https://github.com/cyberbotics/webots/pull/2201](https://github.com/cyberbotics/webots/pull/2201)

##### Luftwaffel [Moderator] 09/04/2020 09:52:35
awesome !


can someone test this ikfast-controller implementation? [https://drive.google.com/file/d/1MMFf481v79ypq-vKLCTsC5dEqCU2caum/view?usp=sharing](https://drive.google.com/file/d/1MMFf481v79ypq-vKLCTsC5dEqCU2caum/view?usp=sharing)


I want to know if it works without system dependencies


just opening the world should be all you have to do. You can move the target sphere around, to have the robot arms follow it

##### David Mansolino [Cyberbotics] 09/04/2020 10:16:47
Works perfectly out of the box for me (and it is indeed way way faster than ikpy), but only on linux and with python 3.7 (which seems to make sense since it contains a pre-compiled library).

##### Luftwaffel [Moderator] 09/04/2020 10:18:30
these compiled libraries dont take long. like 20s on my system. They basically add a python wrapper to the ikfast-solver, which takes 10min -1h for every robot


but that solver only has to be done once

##### David Mansolino [Cyberbotics] 09/04/2020 10:19:09
looks good

##### Luftwaffel [Moderator] 09/04/2020 10:20:27
okay, i'll try to automate the workflow a bit more. Sadly it requires a fairly large docker image I made (2.4 GB or so).  As it requires a very specific ubuntu, ROS and moveIt configuration


but maybe i'll be able to automate it to a point, where you jusp export the .wrl and .urdf file and run  1-2 commands


While i'm at it, I can very easily add mesh conversions and calculations, such as generation of convex hulls or convex decompositions (such as this [https://github.com/kmammou/v-hacd](https://github.com/kmammou/v-hacd))


Is there interest in this?

##### David Mansolino [Cyberbotics] 09/04/2020 10:24:42
Not sure to understand what it is exaclty used for?

##### Luftwaffel [Moderator] 09/04/2020 10:25:08
collision detection


generally, collision detection only works properly with convex geometries


the simples of which is a box, containing the mesh


but it can be broken up into multiple convex shapes, representing the actual mesh in a more accurate way


while still immensly decreasing the complexity and number of vertices

##### David Mansolino [Cyberbotics] 09/04/2020 10:27:59
Ok, makes sense indeed, in that case that is indeed probably usefull!

##### Luftwaffel [Moderator] 09/04/2020 10:29:45
since ODE supports nested collision spaces, this could be a nice way of having multiple levels of collision detail. Which could improve the accuracy of complexer part collisions. I already did some work on that, also with automated box and cylinder bounding box generation


it's not too far fetched to be able to automate this. This would allow one to skip the tedious step of manually creating bounding objects


But first the IKFast stuff 🙂


Found a mistake in the urdf exporter. It takes the endpoint Solid translation for the urdf-joints, NOT the Hingjoint anchors. With P-Rob 3 there is a discrepency for example

##### David Mansolino [Cyberbotics] 09/07/2020 05:56:16
That's interesting, I will check if I can reproduce this.

##### Luftwaffel [Moderator] 09/07/2020 09:56:02
After lots of work I managed to write a script, that 1. Converts a proto file into xml format, 2. Turns this xml proto into an urdf, extracting meshes for each link.


Once I'm on my computer I'll show you the correct joint origin calculation


correct origin of a joint is:

anchor - anchor (previous joint) + translation (previous endpoint)

##### David Mansolino [Cyberbotics] 09/07/2020 10:24:05
Ok thank you, I will try

##### Luftwaffel [Moderator] 09/07/2020 10:30:24
This has been created with the webots model exported as proto (Export -> change .wbo to .proto) and then using my tools to convert it to urdf with extracted visual and collisions
> **Attachment**: [P-Rob3\_ik.mp4](https://cdn.discordapp.com/attachments/565155651395780609/752475856311550012/P-Rob3_ik.mp4)


and compiling an ikfast solver of course, which I have automated a lot too

##### David Mansolino [Cyberbotics] 09/07/2020 10:32:22
That looks really really stable!

##### Luftwaffel [Moderator] 09/07/2020 10:32:24
The code and documentation is not pretty yet, but I'll share soon. Some tests from you guys would be good to validate

##### David Mansolino [Cyberbotics] 09/07/2020 10:32:48
Yes sure (I might even use it for a personnal project 😉 )

##### Luftwaffel [Moderator] 09/07/2020 10:32:49
yeah, IKFast is amazing. it's really fast. it calculates in microseconds


once this project is properly running, I'll try to implement OMPL ([https://ompl.kavrakilab.org/](https://ompl.kavrakilab.org/)) with webots. Allowing for motion planning with collision checking and different solvers (also sample based ones, requireing no solver compiling).

##### David Mansolino [Cyberbotics] 09/07/2020 12:15:39
> correct origin of a joint is:

> anchor - anchor (previous joint) + translation (previous endpoint)

`@Luftwaffel` I can indeed reproduce the issue, we will open an issue and try to fix this soon.

##### Luftwaffel [Moderator] 09/07/2020 13:12:34
Alright, can someone test my conversion tool + ikfast generator?

[https://github.com/Simon-Steinmann/webots\_ikfast\_generator](https://github.com/Simon-Steinmann/webots_ikfast_generator)

you simply should have to run the setup.sh, and then the generate\_ikfast\_solver.sh


it will download a docker image, which has like 2GB, just as a warning


Okay, awesome!! I just compiled the solution for the Puma560, and it literally took 5 minutes, containing about 10 clicks and pressing 'y' a few times

##### David Mansolino [Cyberbotics] 09/07/2020 13:28:54
Sure, as soon as I have finished fixing the bug with urdf joint I will test it, I assume the setup.sh should be run on linux?

##### Luftwaffel [Moderator] 09/07/2020 13:29:04
yes


perhaps everything could pe put into a docker eventually


or the steps are just done manually. But the shell script makes it very convenient


first we gotta make sure it works properly though 🙂


oh, and atm it's only for 6DOF arms.



> **Attachment**: [Puma560\_ik.mp4](https://cdn.discordapp.com/attachments/565155651395780609/752524358622249020/Puma560_ik.mp4)


is there a simple way to get all the default values of node types?


or is it safe to assume, that these wont change due to backwards compatibility?

##### David Mansolino [Cyberbotics] 09/07/2020 13:44:31
You mean the default value of the fields of the basic nodes?

##### Luftwaffel [Moderator] 09/07/2020 13:44:32
some of the older official models are quite wonky in their setup and often dont have parameters, when they are default.


yeah


like anchor, translation etc.


that caused quite some headache last night 😄

##### David Mansolino [Cyberbotics] 09/07/2020 13:45:26
Sure, if not specified in the world/PROTO, the default value used are the one defined in the node definition: [https://github.com/cyberbotics/webots/tree/master/resources/nodes](https://github.com/cyberbotics/webots/tree/master/resources/nodes)

##### Luftwaffel [Moderator] 09/07/2020 13:45:34
instead of putting tons of exceptions in the urdf creator, I rather do a pass over the proto2xml converter and add default values


those values should not change right?


so I dont have to dynamically link them

##### David Mansolino [Cyberbotics] 09/07/2020 13:46:22
We try not to change them as this will break compatibility, so it is extremely rare that we change one of them.

##### Luftwaffel [Moderator] 09/07/2020 13:46:53
okay good. then I'll bake those values into the script. only concerns a few nodes anyways


Solid, Transform and Group can all be used to translate and group other nodes right?


or are there more?


because some models use them for positioning shapes... makes it much more complicated 🙄

##### David Mansolino [Cyberbotics] 09/07/2020 14:07:27
Basically all the descandant of 'Group' can do this: [https://cyberbotics.com/doc/reference/node-chart](https://cyberbotics.com/doc/reference/node-chart)

##### Luftwaffel [Moderator] 09/07/2020 14:08:06
okay, for robot links, only those 3 are important though

##### David Mansolino [Cyberbotics] 09/07/2020 14:08:12
yes

##### Luftwaffel [Moderator] 09/07/2020 14:09:06
do you think it makes more sense to turn groups and transforms into links, or to take their translation and rotation into account and changing the origin of the meshes


the former is easier, but the latter produces much cleaner urdf files

##### David Mansolino [Cyberbotics] 09/07/2020 14:11:45
The second one is indeed cleaner (in my opinion its always good to go for the cleaner as at the end it will make you save time on the long term)

##### Luftwaffel [Moderator] 09/07/2020 14:13:13
yeah, should be possible. this, and converting primitive geometry as well are still on my to-do list. So far it only converts trimeshes, and turns those into a convex and simplified collision mesh. But some models have boxes etc. for their visuals. And those are inside of Transform nodes


how can I hook into the renderer of webots? I would like to add a pybullet cloth simulation within webots

##### David Mansolino [Cyberbotics] 09/09/2020 05:54:20
🤔 That's not going to be an easy task (I have to warn you), and this should be intergrated in the core of Webots and therefore in C++


Here is the implementation of WREN (Webots-Rendering-ENgine): [https://github.com/cyberbotics/webots/tree/master/src/wren](https://github.com/cyberbotics/webots/tree/master/src/wren)

You should probably first start by understanding how it works.


Note that Webots has an experimental 'skin' node that allows to simulation (just for rendering) deformable meshes, you can find an example of this in the 'projects/samples/rendering/worlds/animated\_skin.wbt' simulation ([https://www.cyberbotics.com/doc/guide/samples-rendering#animated\_skin-wbt](https://www.cyberbotics.com/doc/guide/samples-rendering#animated_skin-wbt)).

##### Luftwaffel [Moderator] 09/09/2020 09:04:35
thx for the links, I'll check them out


orther question: with what precision can proto files be written. I found an issue when converting the JACO2 arm from kinova. the fingertips are tiny, so the inertia matrices are very small 10e-7 and e-8 territory. This gets turned into 0 on conversion


which then throws an error

##### David Mansolino [Cyberbotics] 09/09/2020 09:06:49
Our physics engine uses double precision so the values should be able to be extremely small.


Maybe it is the python converter that is somehow rounding the value

##### Luftwaffel [Moderator] 09/09/2020 09:07:26
the problem is the conversion, writing the proto


yeah, i'll investigate more


created a new PR for urdf2webots, small but important fix. 

[https://github.com/cyberbotics/urdf2webots/pull/81](https://github.com/cyberbotics/urdf2webots/pull/81)


a quick check and integration would be great, I need to implement this in my batch conversion, which I would like to finalize soon 🙂

##### David Mansolino [Cyberbotics] 09/09/2020 12:06:57
Cheking it right now 😉

##### Luftwaffel [Moderator] 09/09/2020 14:23:25
alright, added the init-pos feature


even remembered to do the readme 😄


[https://github.com/cyberbotics/urdf2webots/pull/82](https://github.com/cyberbotics/urdf2webots/pull/82)


also, I would really like to get this through:

[https://github.com/cyberbotics/urdf2webots/pull/74](https://github.com/cyberbotics/urdf2webots/pull/74)

now we are already 2 commits ahead again

##### David Mansolino [Cyberbotics] 09/09/2020 14:51:35
Very nice, will have a look when I have the time

##### Luftwaffel [Moderator] 09/09/2020 14:52:03
finding some small bugs, in 15min it should be good


should be correct now


Did you guys already create a script that turns a old proto into a multifile proto?. Should not be too hard to make and I think all the models should be converted. The user experience is 100x better with multifiles

##### David Mansolino [Cyberbotics] 09/10/2020 12:09:03
Not yet no.

##### Luftwaffel [Moderator] 09/10/2020 12:09:17
I'll have a crack at it


how does USE and DEF work with multifile?

##### David Mansolino [Cyberbotics] 09/10/2020 14:12:04
You can't USE DEF are local to the file

##### Luftwaffel [Moderator] 09/10/2020 14:12:31
urgh, well that makes it much uglier


proto inclusion only works on nodes like Shape? Including a geometry node does not work

##### David Mansolino [Cyberbotics] 09/10/2020 14:42:55
No, any PROTO can include any other PROTO as long as it respect the hierarchy rules.

##### Luftwaffel [Moderator] 09/10/2020 14:44:23
it throws an error..

##### David Mansolino [Cyberbotics] 09/10/2020 14:44:32
Which one?

##### Luftwaffel [Moderator] 09/10/2020 14:44:44
ERROR: '/home/simon/Downloads/my\_first\_simulation/ikFast\_test/textile\_manipulation\_training/protos/UR10e.proto':36:21: error: Skipped unknown 'UR10e\_0Mesh' field in Shape node.

ERROR: '/home/simon/Downloads/my\_first\_simulation/ikFast\_test/textile\_manipulation\_training/protos/UR10e.proto':36:33: error: Expected field name or '}', found '{'.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/753627026820431933/unknown.png)
%end

##### David Mansolino [Cyberbotics] 09/10/2020 14:45:17
your are missing the 'geometry' keyword before 'UR10e\_0Mesh'.

##### Luftwaffel [Moderator] 09/10/2020 14:45:40
okay, and I remove it in the mesh file then right?


od do I keep it there


geometry IndexedFaceSet {

vs

 IndexedFaceSet {

##### David Mansolino [Cyberbotics] 09/10/2020 14:46:29
> IndexedFaceSet {

👍🏻

##### Luftwaffel [Moderator] 09/10/2020 14:50:39
alright, I created the script. It extracts all the indexfaceset geometries.

##### David Mansolino [Cyberbotics] 09/10/2020 14:50:56
Very cool!

##### Luftwaffel [Moderator] 09/10/2020 14:51:08
you can just run it, a file select window opens
> **Attachment**: [proto2multi.py](https://cdn.discordapp.com/attachments/565155651395780609/753628633867747459/proto2multi.py)

##### David Mansolino [Cyberbotics] 09/10/2020 14:51:18
If you want, we have a script folder here where it could fit: [https://github.com/cyberbotics/webots/tree/master/scripts](https://github.com/cyberbotics/webots/tree/master/scripts)

##### Luftwaffel [Moderator] 09/10/2020 14:51:47
sure

##### David Mansolino [Cyberbotics] 09/10/2020 14:52:05
Looks good!

##### Luftwaffel [Moderator] 09/10/2020 14:52:07
maybe I should add an option to crawl through a folder structure and convert every proto


replacing it

##### David Mansolino [Cyberbotics] 09/10/2020 14:52:39
That would be nice yes. But if you prefeer this could be added in a second step

##### Luftwaffel [Moderator] 09/10/2020 14:52:55
my batch converter already does it, can reuse the code


okay, every robot in the webots project directory converted in like 3 seconds 😄


take t for a spin. Start it with '--all' to do a batch conversion. it creates new folder in the selected one, with all conversions inside, same structure and all
> **Attachment**: [proto2multi.py](https://cdn.discordapp.com/attachments/565155651395780609/753641339677048953/proto2multi.py)


On a different note. My ik-fast generator is working really well now, and I created solvers for all kinova, kuka, universal robots + irb + P-Rob3 + puma robot. My generik IK-Fast controller also works really well, automatically picking the correct solver. I think this would be a fantastic tool for people to have, or to be more deeply implemented. The issue is, that it requires a ikfastpy compiled file, which is OS dependant


and I have no experience compiling on Mac or windows.

##### David Mansolino [Cyberbotics] 09/10/2020 15:54:40
If I have time I will have a look and try to handle the mac and Windows part but this is unfortunately probably not going to be this week, as I am still very busy with this URDF exportation bug: [https://github.com/cyberbotics/webots/issues/2212](https://github.com/cyberbotics/webots/issues/2212)

##### Luftwaffel [Moderator] 09/10/2020 15:55:28
oh I fixed that issue in my own converter 😄

##### David Mansolino [Cyberbotics] 09/10/2020 15:56:54
I know 😉 Bu I am trying to fix it directly in the core of Webots, and this is surprisingly complex, I fixed the 'P-Rob3' case, but then it broke another robot 🙄

##### Luftwaffel [Moderator] 09/10/2020 15:57:23
did you try the thing with enpoint-anchor discrepency I told you?

##### David Mansolino [Cyberbotics] 09/10/2020 15:58:23
Yes, that's what I did and it fixed the P-Rob case nicely, and all the joint-link are now correct in all the cases, but in some specific case the position of some visual/collision nodes are broken

##### Luftwaffel [Moderator] 09/10/2020 15:59:07
most likely those inside an extra transform node


are you setting an additional origin in the urdf for those?

##### David Mansolino [Cyberbotics] 09/10/2020 16:00:09
exactly, the problem seems when the joint anchor does not matches the Solid origin + the bounding object uses an extra transform layer.

##### Luftwaffel [Moderator] 09/10/2020 16:00:35
show me to the code good sir


in my mesh creation, I implemented that discrepancy too


this might be of use. This is my proto to urdf conversion
> **Attachment**: [xml2urdf.py](https://cdn.discordapp.com/attachments/565155651395780609/753646330173456424/xml2urdf.py)


with the proto being in xm format

##### David Mansolino [Cyberbotics] 09/10/2020 16:01:54
Here is my current non-fully-working patch (but it is in C++): [https://github.com/cyberbotics/webots/pull/2214/files](https://github.com/cyberbotics/webots/pull/2214/files)



and here is the result:


😂
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/753646494862802995/unknown.png)
%end

##### Luftwaffel [Moderator] 09/10/2020 16:02:18
you dropped something 😄

##### David Mansolino [Cyberbotics] 09/10/2020 16:02:26
Clearly


> this might be of use. This is my proto to urdf conversion

`@Luftwaffel` thanks will check where is the difference.


Is it working fine with both the tiago and p-rob3 robots?

##### Luftwaffel [Moderator] 09/10/2020 16:03:52
let me try


how do you view the urdf btw?


which tiago?

##### David Mansolino [Cyberbotics] 09/10/2020 16:05:08
tiago++


> how do you view the urdf btw?

`@Luftwaffel` the simplest and powerfull solution is to use the `urdf_tutorial` package:

```
roslaunch urdf_tutorial display.launch model:=./tiago.urdf
```

You can even move joints with sliders

##### Luftwaffel [Moderator] 09/10/2020 16:38:00
man that file is evil


full of duplicate names and stuff 😛

##### David Mansolino [Cyberbotics] 09/10/2020 16:38:17
Which one ?

##### Luftwaffel [Moderator] 09/10/2020 16:38:23
Tiago

##### David Mansolino [Cyberbotics] 09/10/2020 16:38:44
ah yes indeed, this is why it's my test case, it is a very complex robot!

##### Luftwaffel [Moderator] 09/10/2020 16:46:56
oh boy, my script is definitely not yet up to the task :p

##### David Mansolino [Cyberbotics] 09/10/2020 16:47:23
I am kind of re-assured that I am not the only one having issues with this crazy robot 😉

##### Luftwaffel [Moderator] 09/10/2020 16:50:45
ohhhh sliderjoints may be an issue for me

##### David Mansolino [Cyberbotics] 09/10/2020 16:51:49
There is indeed a few of them in this robot, they indeed need to be handled, but they are more easy to handle than hingeJoints

##### Luftwaffel [Moderator] 09/10/2020 16:52:09
my script turns them into fixed ones atm

##### David Mansolino [Cyberbotics] 09/10/2020 16:52:34
Better than nothing 😉

##### Luftwaffel [Moderator] 09/10/2020 17:06:25
this is too much for me atm, but you have to add an <origin> node to the visual (and probably collision) part in the urdf


the correct origin is:

origin1 = anchor(parent joint)

origin2 = translation of current solid/link

origin = origin2 -  origin1


end result should look something like this
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/753663512378605568/unknown.png)
%end

##### David Mansolino [Cyberbotics] 09/10/2020 17:10:31
Yes, I have something like this, it is almost working, just have a final issue with the gripper now!

##### Luftwaffel [Moderator] 09/10/2020 17:11:31
do you by any chance have pybullet experience?

##### David Mansolino [Cyberbotics] 09/10/2020 17:12:35
Unfortunately no.

##### Luftwaffel [Moderator] 09/11/2020 18:22:44
I think I found a way to install ikfast solvers with pip. And it should be possible to have them natively in c++ too. With that I'd need some help though.

##### JMRMEDEV 09/12/2020 05:05:17
Hi guys! I'm working in a project to program an external controller to the UR5e. I'm still getting familiar with Webots. I already followed simple tutorials. Like how to control NAO with external controller. Does each device uses specific libraries or all come in webots/something?


Can anyone help with an existing example or documentation about this matter?

##### Luftwaffel [Moderator] 09/12/2020 11:12:41
`@JMRMEDEV` can you elaborate on what exactly you are trying to do?


what programming language are you using? Do you need inverse kinematics?

##### JMRMEDEV 09/12/2020 15:21:54
`@Luftwaffel` well, I guess I don't need inverse Kinematics. I'm intending to control the UR5 with Kinect. So, I guess the dyanmic would be if I move my right arm, the first motor, if I take my forearm so will do the next section of the UR5 and so. I've programmed the UR5 with inverse Kinematics and Matlab before, but as I said, I'm not sure it would be the need. So, my main goal is to know how to build an external controller for the UR5. I want to do it in C. As far as I understood in the documentation, with an external controller, I have the effects that I want in programming outside Webots. I don't know if this is clear enough.


*dynamic


I already tried the external controller for the NAO and take a look to the code. And I see there are several includes without an explicit reference to NAO itself, but the Webots API.

##### Luftwaffel [Moderator] 09/12/2020 15:23:37
I'm a python guy, so I can't help you much, but follwoing this: [https://www.cyberbotics.com/doc/guide/installation-procedure#extern-controllers](https://www.cyberbotics.com/doc/guide/installation-procedure#extern-controllers) should lead you to success


and probably this too:

[https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers](https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers)

##### JMRMEDEV 09/12/2020 15:25:26
Yup, I already looked at that info.


Like a puthon guy....


Have you programmed an external controller?

##### Luftwaffel [Moderator] 09/12/2020 15:26:05
yes, constantly use them, but only python, and only on linux


did you set your environment variables?


as described in the second link?


that is the key

##### JMRMEDEV 09/12/2020 15:26:47
I get that. I don't actually know python programming, but I guess you have to use something like includes, using or any other references, right?

##### Luftwaffel [Moderator] 09/12/2020 15:27:10
yep, "import <package\_name>"

##### JMRMEDEV 09/12/2020 15:27:13
Yes, I already set the enviroment variables. And controlled the simulated NAO with the external controller example.


So my actual question is related with which dependencies should I use.


That I'm not sure if there are specific ones for each robots or are like "universal".

##### Luftwaffel [Moderator] 09/12/2020 15:28:19
\#include <webots/robot.h>


and if you are using a supervisor robot, then probably

\#include <webots/supervisor.h>

##### JMRMEDEV 09/12/2020 15:29:06
Okay. So they are indeed universal, right?

##### Luftwaffel [Moderator] 09/12/2020 15:29:13
yes

##### JMRMEDEV 09/12/2020 15:29:19
For example, in the NAO controller, I see these includes:


\#include <assert.h>

\#include <stdio.h>

\#include <stdlib.h>

\#include <string.h>

\#include <webots/accelerometer.h>

\#include <webots/camera.h>

\#include <webots/distance\_sensor.h>

\#include <webots/gps.h>

\#include <webots/gyro.h>

\#include <webots/inertial\_unit.h>

\#include <webots/keyboard.h>

\#include <webots/led.h>

\#include <webots/motor.h>

\#include <webots/robot.h>

\#include <webots/touch\_sensor.h>

\#include <webots/utils/motion.h>

##### Luftwaffel [Moderator] 09/12/2020 15:29:40
[https://cyberbotics.com/doc/reference/robot](https://cyberbotics.com/doc/reference/robot) this is your friend

##### JMRMEDEV 09/12/2020 15:29:48
And I see there's nothing like <webots/nao.h>


So the specific applications, extends 'robot' as I understood.


But well, I guess you already clarified my doubt.


Thank you.

##### Luftwaffel [Moderator] 09/12/2020 15:31:34
in python you just import the robot. everything else should be pretty much a descendent of that


[https://cyberbotics.com/doc/reference/node-chart](https://cyberbotics.com/doc/reference/node-chart)

##### JMRMEDEV 09/12/2020 15:32:07
Thank you once more.

##### Luftwaffel [Moderator] 09/12/2020 15:50:09
np 🙂


I think I found a way to install ikfast solvers with pip. And it should be possible to have them natively in c++ too. With that I'd need some help though.


can some try to build this. In linux it's 

pip install .

inside of the directory
> **Attachment**: [ikfast\_pybind.zip](https://cdn.discordapp.com/attachments/565155651395780609/755028033076723742/ikfast_pybind.zip)


with this world and controller you should be able to test it
> **Attachment**: [generik\_ikfast.zip](https://cdn.discordapp.com/attachments/565155651395780609/755028933589860442/generik_ikfast.zip)


it should automatically work with all the URe robots, puma, p-rob3, all 6 dof kukas


`@Olivier Michel` `@David Mansolino` Having created many protos now from urdf, and handling the already included robots in webots, I noticed a discrepency, which I think should be addressed and made consistent. The toolSlot of severral webots models has the y-axis pointing out, while almost every urdf model and also some webots models, have the z-axis pointing out. It would be great, if that is standardized, and I'm leaning towards the z-axis convention, as it is most common and compatible with most user-urdf conversions


also, the current toolSlot implementations in webots, create an issue when extracting the urdf from webots, as they are no solid


also the default rotation should be adjusted, so that things are oriented correctly by default (robot spawning upright, not on its side)

##### Olivier Michel [Cyberbotics] 09/14/2020 12:53:44
I agree with your proposal regarding orientation of the toolSlot. It would be good to open an issue about it and list which protos have a toolSlot orientation different from z-axis out. This will allow us to evaluate the amount of work to fix this.

##### Luftwaffel [Moderator] 09/14/2020 12:54:04
this is, what my 'ideal' robot arm model looks like. It's basically an automated conversion, but I took your custom  boudning objects
> **Attachment**: [UR10e.zip](https://cdn.discordapp.com/attachments/565155651395780609/755048724740178001/UR10e.zip)


with my scripts I can probably automate a lot of these things.


also, I think my kuka conversions are pretty much good enough for official inclusion. The only thing might be the collision boxes, which could be split up. But they are already pretty damn good


same with gen3 kinova

##### Olivier Michel [Cyberbotics] 09/14/2020 13:19:44
In that case, feel free to open a PR to add these models to the main distribution of Webots. In addition to what you mention, we would also require some documentation pages and a working demo world.

##### Luftwaffel [Moderator] 09/14/2020 13:30:28
what is your opinion on python wrapper for the ik\_solvers. If they are compiled for windows, linux and mac, they should all work right? Considering the amazing performance, it would be very cool, if that could be included

##### David Mansolino [Cyberbotics] 09/14/2020 13:32:12
Yes, but ideally we should not commit compiled binaries on Github, but rather provide the sources + compilation procedure.

##### Luftwaffel [Moderator] 09/14/2020 13:34:47
For all those robotic arms, I would like to include these ik-controllers. Would it be possible to include the files and then prompt the user to compile, and that being exectued from within webots?


on linux, pip does everything automatically, but I dont know how it is on other OS

##### David Mansolino [Cyberbotics] 09/14/2020 13:40:41
Not sure, the best would be to try, if you want you can open the PR and make sure it works well for linux and we will try for other OS.

##### Luftwaffel [Moderator] 09/14/2020 13:43:21
Say we kept the solvers in cpp, could it be possible to use the plugin functionality somehow?

##### David Mansolino [Cyberbotics] 09/14/2020 13:45:23
I am not sure to understand what you mean by plugin exactly?

##### Luftwaffel [Moderator] 09/14/2020 13:45:39
physics plugin

##### David Mansolino [Cyberbotics] 09/14/2020 13:45:58
You want to integrate the ik solver in the physics plugin?

##### Luftwaffel [Moderator] 09/14/2020 13:46:14
maybe something could be adjusted to create a plugin, that does the ik

##### David Mansolino [Cyberbotics] 09/14/2020 13:47:07
The physics plugin is probably not the best way to go, a simple first step would maybe to create a dynamical library that you can load from any controller.

##### Luftwaffel [Moderator] 09/14/2020 13:48:32
you mean like creating a new functionality for the robot node for example?

##### David Mansolino [Cyberbotics] 09/14/2020 13:49:23
yes, somethign like what is done with the driver library that extends the robot API for 4wheeled vehicle: [https://github.com/cyberbotics/webots/tree/master/projects/default/libraries/vehicle](https://github.com/cyberbotics/webots/tree/master/projects/default/libraries/vehicle)

##### Luftwaffel [Moderator] 09/14/2020 13:49:52
uh that looks promising


how does it work, being able to communicate with c, cpp, java and python?

##### David Mansolino [Cyberbotics] 09/14/2020 13:51:11
The library is written in C, and then we use wrappers from the other languages to call the c one.

##### Luftwaffel [Moderator] 09/14/2020 13:51:34
but arent these precompiled?

##### David Mansolino [Cyberbotics] 09/14/2020 13:51:47
Once distributed, but not on Github

##### Luftwaffel [Moderator] 09/14/2020 13:51:55
> Yes, but ideally we should not commit compiled binaries on Github, but rather provide the sources + compilation procedure.

`@David Mansolino`  cough cough


😄


i mean on github of course. but for the version 'normal' people download, stuff like this is precompiled right?

##### David Mansolino [Cyberbotics] 09/14/2020 13:54:15
Yes sure

##### Luftwaffel [Moderator] 09/14/2020 13:54:26
sorry, to be annoying, just wanna understand how the stuff works, so I can implement my projects better


I'm confused, with the newest nightly build, a robot arm with z pointing up, gets aligned automatically, if the rotation is 0? In the past we had to specify the default rotation field. So now, all my robots are lying down, isntead of standing up. What is the new mechanism?


how does it know, what 'up' is for a proto model?


I'm guessing this has to do with automatic world coordinate calculations

##### David Mansolino [Cyberbotics] 09/14/2020 14:48:27
Which nightly are you using? R2020b-rev2 or R2021a ?

##### Luftwaffel [Moderator] 09/14/2020 14:48:48
b-rev2

##### David Mansolino [Cyberbotics] 09/14/2020 14:49:54
And before you where using the official release right?

##### Luftwaffel [Moderator] 09/14/2020 14:51:24
the latest nightly


webots-R2020b-rev2-x86-64\_ubuntu-16.04.tar.bz2


11-9-2020

##### David Mansolino [Cyberbotics] 09/14/2020 14:52:18
ok and now, which date ?

##### Luftwaffel [Moderator] 09/14/2020 14:53:16
11-9-2020

##### David Mansolino [Cyberbotics] 09/14/2020 14:53:47
Ok, but then compared to which version the behavior changed?

##### Luftwaffel [Moderator] 09/14/2020 14:54:26
I have the old webots folder, where can I check the version/date?

##### David Mansolino [Cyberbotics] 09/14/2020 14:55:13
can you try 'webots --version' ?

##### Luftwaffel [Moderator] 09/14/2020 14:55:34
the webots file has the modified date of:

Mo 03 Aug 2020 23∶58∶00 CEST

##### David Mansolino [Cyberbotics] 09/14/2020 14:56:46
Oh ok, then probably you were using revision 1 ?

##### Luftwaffel [Moderator] 09/14/2020 14:56:58
2021a nightly build 3/8/2020


oh was using 2021


which version is the one I should use for testing and latest features?

##### Olivier Michel [Cyberbotics] 09/14/2020 15:01:21
For the latest features, you should use R2021a nightly.

##### Luftwaffel [Moderator] 09/14/2020 15:02:40
alright, downloading


question, how important are those modeled screws you guys have on the universal robot models?

##### David Mansolino [Cyberbotics] 09/15/2020 05:12:18
They are not particularly important, just to make the robot look slightly nicer.

##### Luftwaffel [Moderator] 09/15/2020 14:23:18
`@David Mansolino` I reworked the multi-file script. Should adress everything now. Read the instructions (they changed). [https://github.com/cyberbotics/webots/pull/2226](https://github.com/cyberbotics/webots/pull/2226)

##### David Mansolino [Cyberbotics] 09/15/2020 14:37:38
Perfect, I will retry it

##### Luftwaffel [Moderator] 09/15/2020 17:43:22
is it possible to have a exposed field in a proto hidden? So it can be addressed with a supervisor, but does not show up in the scene tree?

##### David Mansolino [Cyberbotics] 09/15/2020 18:23:11
Yes it is, we are using this mechanism for example for the kinematic vehicles to rotate the wheels, the angular velocity is an open field which is hidden because the user don't care about it, but the supervisor is getting it and changing its vaue: [https://github.com/cyberbotics/webots/blob/master/projects/vehicles/protos/bmw/BmwX5Simple.proto#L21](https://github.com/cyberbotics/webots/blob/master/projects/vehicles/protos/bmw/BmwX5Simple.proto#L21)

##### Luftwaffel [Moderator] 09/15/2020 18:29:15
awesome! thank you, exactly what I was looking for

##### David Mansolino [Cyberbotics] 09/15/2020 18:49:26
You're welcome

##### Luftwaffel [Moderator] 09/15/2020 23:08:23
Automated convex collision mesh creation. Visual .dae to collision .obj  

Less vertices than the usual .stl files and much more accurate and closer to the model. Based on v-hacd ([https://github.com/kmammou/v-hacd](https://github.com/kmammou/v-hacd))
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/755565709655932928/unknown.png)
%end


added multithreading for the computation too, so it doesnt take very long.


TODO:

add feature to urdf2webots updfparser.py, so it does not merge meshes on  .obj import. Should behave more like collada import


I also managed to get automatically aligned minimal bounding boxes and cylinders to work (in contrast to axis aligned). Both methods combined could also greatly improve box-collision approximations

##### David Mansolino [Cyberbotics] 09/16/2020 05:09:03
That looks really good, looking forward to test this.

##### Luftwaffel [Moderator] 09/16/2020 14:16:16
you can test this. --input=<.urdf file>
> **Attachment**: [urdf\_v\_hacd\_collision.zip](https://cdn.discordapp.com/attachments/565155651395780609/755794188418940998/urdf_v_hacd_collision.zip)


should create a new urdf in the same folder, with collision meshes replaced and everything in absolute paths


you can try this version of parserURDF.py, if you want to have the .obj collision as individual convex bounding objects
> **Attachment**: [parserURDF.py](https://cdn.discordapp.com/attachments/565155651395780609/755796417297252502/parserURDF.py)


wups, this was nut updated
> **Attachment**: [requirements.txt](https://cdn.discordapp.com/attachments/565155651395780609/755797102046740510/requirements.txt)

##### David Mansolino [Cyberbotics] 09/16/2020 14:30:09
Can you please create a PR with this on the urdf2webots repo (we don't have the time to test thi right now)

##### Luftwaffel [Moderator] 09/16/2020 14:30:44
it requires pybullet and trimesh though ( the mesh optimization)


but the .obj as multiple files I can do a PR


[https://github.com/cyberbotics/urdf2webots/pull/83](https://github.com/cyberbotics/urdf2webots/pull/83)


would you want the collision mesh optimization also as port of the urdf2webots repository?


it's just a single script, but it has dependencies, namely pybullet. Which can be installed via pip easily, but still

##### David Mansolino [Cyberbotics] 09/17/2020 05:52:38
Let's start with the obj improvements first, as it seems already a new feature in itself.

##### Luftwaffel [Moderator] 09/17/2020 12:35:41
both PRs should be cleaned up and working

##### David Mansolino [Cyberbotics] 09/17/2020 12:36:05
Just approved the first one, I am testing the second one

##### Luftwaffel [Moderator] 09/17/2020 12:37:12
🥳


hmmm export as urdf does not work, when trying to export the gripper in the toolslot of a robot. only the whole robot can be exported

##### Darko Lukić [Cyberbotics] 09/17/2020 12:46:04
`@Luftwaffel` You mean `get_urdf()`?

##### Luftwaffel [Moderator] 09/17/2020 12:46:26
right-click -> export -> change extension to .urdf


oh most likely only robots can be exported?


since it is a Solid

##### David Mansolino [Cyberbotics] 09/17/2020 12:48:29
> oh most likely only robots can be exported?

`@Luftwaffel` yes

##### Darko Lukić [Cyberbotics] 09/17/2020 12:48:46
You can create Robot node, add the solid inside, and then export it as URDF

##### David Mansolino [Cyberbotics] 09/17/2020 12:48:50
You should get a warning if it is not a solid

##### Luftwaffel [Moderator] 09/17/2020 12:49:23
oh, terminal was hidden :p


would be nice if it were to work with other types too, but it's not the end of the world

##### David Mansolino [Cyberbotics] 09/17/2020 12:50:31
I am not 100% familiar with urdf, but not sure it supports something else than robot?

##### Luftwaffel [Moderator] 09/17/2020 12:50:56
[https://tenor.com/view/elmo-shrug-gif-5094560](https://tenor.com/view/elmo-shrug-gif-5094560)


devices are usual defined as robotst in urdf, but in webots they need to be solids

##### David Mansolino [Cyberbotics] 09/17/2020 12:51:19
Since URDF stands for 'Unified **Robot** Description Format', I would expect it to be for robot only.

##### Luftwaffel [Moderator] 09/17/2020 12:51:41
would be nice if one could export a gripper directly

##### David Mansolino [Cyberbotics] 09/17/2020 12:51:53
> devices are usual defined as robotst in urdf, but in webots they need to be solids

`@Luftwaffel` yes you're right, in that case you should probably use the workaround `@Darko Lukić` proposed

##### Luftwaffel [Moderator] 09/17/2020 12:52:09
or export only part of a robot


let's say we have a mobile robot with arms. would be nice to select the base solid of the arm and export as urdf

##### JMRMEDEV 09/17/2020 23:48:07
Hi guys, I'm developing a controller in C++.


But I get this message when tryng to set motor positions:


No symbol file loaded for Controller.dll


Thoughts?

##### David Mansolino [Cyberbotics] 09/18/2020 06:09:54
Hi, is it only with the wb\_motor\_set\_position function?

Which version of Webots are you using on and which OS?

Are you using regular controllers or extern controllers?

##### JMRMEDEV 09/18/2020 06:11:54
Well happened only with set\_position. In an external controller, Windows 10 OS.


I just noticed that for external controllers, the main function is the one that has to use the webots namespace and includes.


I tried to create a header with the webots namespace, logic and includes.


And by using as a header is when I got the error.


I re-designed my app to rely on webots as main, but that makes a pain modular programming. But anyway, so far I managed to do what I wanted, even if is not the way I wanted.


Thanks for the reply.

##### David Mansolino [Cyberbotics] 09/18/2020 06:14:57
> I re-designed my app to rely on webots as main, but that makes a pain modular programming. But anyway, so far I managed to do what I wanted, even if is not the way I wanted.

`@JMRMEDEV` yes that's indeed not very nice. Normally extern controller should compile and run the same way as normal controller.

Can you reproduce this with one of the sample controllers provided with Webots using the set\_position function?

##### JMRMEDEV 09/18/2020 06:16:05
Well, there is not a sample directly in C++. Only in C. But if I find the time, I will.


I mean, I will "translate" the sample from C to C++ implementing classes the way I tried.

##### David Mansolino [Cyberbotics] 09/18/2020 06:16:57
You can find one C++ sample in projects/languages/cpp


I just checked it uses setPosition:

[https://github.com/cyberbotics/webots/blob/master/projects/languages/cpp/controllers/slave/slave.cpp#L65](https://github.com/cyberbotics/webots/blob/master/projects/languages/cpp/controllers/slave/slave.cpp#L65)

##### JMRMEDEV 09/18/2020 06:18:04
🤔 I asked this before, but just to be sure... All the functions are compatible with all the robots, right?


> Well, there is not a sample directly in C++. Only in C. But if I find the time, I will.

`@JMRMEDEV` I mean, I'm writing a controller for URe. That's the robot where there's only a C controller sample.

##### David Mansolino [Cyberbotics] 09/18/2020 06:18:37
Yes (as long as the robot has the device).

##### JMRMEDEV 09/18/2020 06:19:08
But I'll try one of the samples that you referred.


Taking advantage of the topic... I tried to make a Dynamic Library (DLL) making reference to webots. It didn't work. I noticed that I do must build an executable application for it to work.


Are any implementations of Webots as a reference in a DLL?

##### David Mansolino [Cyberbotics] 09/18/2020 06:23:27
Yes, this is possible, we are using this for example with the darwin-op robot: [https://github.com/cyberbotics/webots/tree/master/projects/robots/robotis/darwin-op](https://github.com/cyberbotics/webots/tree/master/projects/robots/robotis/darwin-op)

We have created here a library that uses the Webtos API and can then be used directly from the controller: [https://github.com/cyberbotics/webots/tree/master/projects/robots/robotis/darwin-op/libraries/managers](https://github.com/cyberbotics/webots/tree/master/projects/robots/robotis/darwin-op/libraries/managers)

##### JMRMEDEV 09/18/2020 06:24:05
I'll take a look. Thank you so much!

##### David Mansolino [Cyberbotics] 09/18/2020 06:24:27
You're welcome

##### Luftwaffel [Moderator] 09/18/2020 11:39:31
[https://github.com/cyberbotics/webots/pull/2226](https://github.com/cyberbotics/webots/pull/2226) should be finally done now 🙂

##### David Mansolino [Cyberbotics] 09/18/2020 12:13:56
I will have a look in a moment 🙂

##### Luftwaffel [Moderator] 09/20/2020 18:27:58
I noticed something: when having a world with <extern> controller and setting the mode to 'run', the process uses 100% of its thread. Even though the simulation is not running. Whichever loop is running in the background, waiting for an extern controller to connect, should have a limited frequency

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 06:59:21
`@Luftwaffel` Yes, Webots uses 100% because it tries to run as fast as possible but it is blocked by the missing extern controller.  In this case, the simulation IS running internally. In fact if you click on the pause button, then the process usage immediately decrease. We will check if there is way to improve it. Please open an issue on GitHub ([https://github.com/cyberbotics/webots/issues/new/choose](https://github.com/cyberbotics/webots/issues/new/choose)).

##### XZbot 09/21/2020 07:01:19
Hi, I am trying to use the latest Webots docker image within Kubernetes. I put 

        image: cyberbotics/webots:lastest

inside the container spec of a yaml file for a job or a pod. However, it shows the pod/job status as ErrImagePull or ImagePullBackOff. Could you please give me some advice on how to debug this error? 



Besides, I'd like to run Webots in the CI without GUI. Then still inside the yaml file for a pod or job, should I put 

        command: ["/bin/bash", "-c"] 

        args: ["xvfb-run webots --stdout --stderr --batch --mode=fast /path/to/persistentVolumeClaim/worldFile"]

there?



Many thanks!



(`@Stefania Pedrazzi`  I have deleted my message in the news channel and reposted here. Sorry about that.)

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 07:02:40
> Hi, I am trying to use the latest Webots docker image within Kubernetes. I put 

>         image: cyberbotics/webots:lastest

> inside the container spec of a yaml file for a job or a pod. However, it shows the pod/job status as ErrImagePull or ImagePullBackOff. Could you please give me some advice on how to debug this error? 

> 

> Besides, I'd like to run Webots in the CI without GUI. Then still inside the yaml file for a pod or job, should I put 

>         command: ["/bin/bash", "-c"] 

>         args: ["xvfb-run webots --stdout --stderr --batch --mode=fast /path/to/persistentVolumeClaim/worldFile"]

> there?

`@XZbot`  Yes, these are the options to use. There is no other option to completely run Webots without GUI.


I will check for the problem with the Docker image

##### XZbot 09/21/2020 07:06:34
> I will check for the problem with the Docker image

`@Stefania Pedrazzi` Thanks for checking 🙂

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 07:39:55
I checked and the Docker image works fine for me. We never tested the Webots docker image within Kuberbetes, so we cannot help you very much with this.

But I quickly checked and you can find many threads on the web regarding ErrImagePull or ImagePullBackOff errors. Probably looking at them you should be able to debug and identify what could be the issue.

##### XZbot 09/21/2020 09:51:55
> I checked and the Docker image works fine for me. We never tested the Webots docker image within Kuberbetes, so we cannot help you very much with this.

> But I quickly checked and you can find many threads on the web regarding ErrImagePull or ImagePullBackOff errors. Probably looking at them you should be able to debug and identify what could be the issue.

`@Stefania Pedrazzi` Using docker to pull and run the Webots image also works for me. Actually there was no issue of pulling your Webots image within Kubernetes two to three weeks ago, before you released the updates. I did not change the way I called the image in the yaml file for a job/pod. The Kubernetes cluster being used is running without any problem. So there is not much I can do from my end. It would be quite important and helpful to confirm that your Webots docker image can work in a CI, since some Webots projects (like ours) do require a lot of computing resources for necessarily complex world environment and for many concurrent simulation runs. For the past few months, we've tried on local desktops with 1 to 2 great GPUs and fast simulations without visualization in Webots, but still quite slow. That's why we are switching to cluster resources. Since the Webots docker image (both R2019b and the latest version) could no longer be pulled by the Kubernetes after your updates but other docker images being used by our group still work well within the same cluster, I think there is highly chance that you could do some slight modifications on your docker image and let it work again within Kubernetes. Thank you for your assistance!

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 10:05:30
We can look at it and inspect the issue as soon as possible and inspect the issue. But it will probably take some time.

In the meantime, here are the instructions to build your own docker image of Webots:

[https://github.com/cyberbotics/webots/wiki/Docker#create-your-own-image](https://github.com/cyberbotics/webots/wiki/Docker#create-your-own-image)

These are the instructions we used previously to build the docker image and have not been updated since June, so there is a chance that this will work for you.

##### XZbot 09/21/2020 10:06:35
> We can look at it and inspect the issue as soon as possible and inspect the issue. But it will probably take some time.

> In the meantime, here are the instructions to build your own docker image of Webots:

> [https://github.com/cyberbotics/webots/wiki/Docker#create-your-own-image](https://github.com/cyberbotics/webots/wiki/Docker#create-your-own-image)

> These are the instructions we used previously to build the docker image and have not been updated since June, so there is a chance that this will work for you.

`@Stefania Pedrazzi` Ok. Thanks!

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 10:07:10
You're welcome

##### XZbot 09/21/2020 10:17:09
> You're welcome

`@Stefania Pedrazzi` I just clicked on the link and noticed that actually I tried these steps on the same webpage to build my own docker image a month ago. But after starting a Ubuntu 16.04 image in the bash mode and during the process of installing the Webots dependencies, I encountered the following errors. I tried to update the node version but still did not work. Then I quit creating my own Webots image and switched to work on other project stuff. I wonder if you have encountered similar errors during this image creating process. Thanks.
%figure
![image.png](https://cdn.discordapp.com/attachments/565155651395780609/757545950951637032/image.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 11:44:59
I just checked again the isntructions on Ubuntu 18.04 and it works without any error. We are continuously fixing this kind of issues in the GitHub repo, so it is very likely that it would work now for you as well. Otherwise you could just skip the compilation of `web/wwi` that it is only needed for running Webots on the web


I can reproduce the same issue on the Ubuntu 16.04 docker image. If possible it would be better to use Ubuntu 18.04 or 20.04

##### XZbot 09/21/2020 12:27:39
> I just checked again the isntructions on Ubuntu 18.04 and it works without any error. We are continuously fixing this kind of issues in the GitHub repo, so it is very likely that it would work now for you as well. Otherwise you could just skip the compilation of `web/wwi` that it is only needed for running Webots on the web

`@Stefania Pedrazzi` Great. Thank you. I will try to recreate my own Webots docker image on Ubuntu 18.04 tomorrow. Just now still within the Kubernetes cluster, I created new pods and tested again / for the first time all your Webots image versions available (R2019b-rev1 and all on [https://hub.docker.com/r/cyberbotics/webots/tags](https://hub.docker.com/r/cyberbotics/webots/tags)). Now a pod with the image R2019b-rev1 still generates the ErrImagePull status. But a pod with one of the other five images does work now, including the latest version which did not work a few days ago. Not sure why though... Anyways, I can again use the latest version now 🙂
%figure
![images_work_or_not.JPG](https://cdn.discordapp.com/attachments/565155651395780609/757578794335862794/images_work_or_not.JPG)
%end

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 12:29:54
Thank you for the update!

##### XZbot 09/21/2020 12:30:23
No prob. Thank you for the help!

##### Luftwaffel [Moderator] 09/21/2020 21:37:47
Reinforcement learning with Webots
> **Attachment**: [simplescreenrecorder-2020-09-21\_23.31.34.mp4](https://cdn.discordapp.com/attachments/565155651395780609/757717239729225798/simplescreenrecorder-2020-09-21_23.31.34.mp4)



> **Attachment**: [simplescreenrecorder-2020-09-21\_23.32.36.mp4](https://cdn.discordapp.com/attachments/565155651395780609/757717245324558427/simplescreenrecorder-2020-09-21_23.32.36.mp4)


how fast it trains, and speed with realtime factor of 1


using DDPG with HER (hindsight experience replay)


this was about after 20 minutes of training

##### Veverest 09/22/2020 06:46:39
Do you have a github repo for this? I'm sure a lot of us would love to take a look

##### Luftwaffel [Moderator] 09/22/2020 08:00:05
Not yet, but I'm working on a clean workflow and implementation. Based on stable-baselines and rl-baselines-zoo


[https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers#multiple-concurrent-simulations](https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers#multiple-concurrent-simulations)  the webots PID, is that the webots PID, or webots-bin PID?



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/757943039766429776/unknown.png)
%end

##### Olivier Michel [Cyberbotics] 09/22/2020 12:36:38
I believe it's Webots PID.

##### Luftwaffel [Moderator] 09/22/2020 12:37:22
I'll try. I hope it is, because getting the PID of the webot-bin will be much more complicated


continuing here, as it's more development...


here the huge memory heap of webots really starts to become a problem


it's not structured and clean enough for a repo, but will come soon. It uses my ikfast solvers, which is a huge part of the incredible speed


yesterday I have played around with valgrind, Massif and all that, but I'm just starting to scratch the surface. Perhaps you can help with that.

##### Olivier Michel [Cyberbotics] 09/22/2020 16:08:49
I am not sure what can we make to reduce the heap used by Webots...

##### Luftwaffel [Moderator] 09/22/2020 16:08:55
Ideally, it would be great, if webots could be started in a mode without gui, and only the bare minimums loaded


from my understanding, memory heap is not necessarily all used. just allocated

##### Olivier Michel [Cyberbotics] 09/22/2020 16:09:56
I believe the GUI doesn't consume that much of heap... Did you try opening a very simple world file (no textures,  a simple cube object). Does it make a significant heap reduction?

##### Luftwaffel [Moderator] 09/22/2020 16:10:37
no matter what I load, the memory usage is at min. 1.1GB

##### Olivier Michel [Cyberbotics] 09/22/2020 16:10:49
If not used, it shouldn't be a problem as it will go the swap...

##### Luftwaffel [Moderator] 09/22/2020 16:10:54
which is quite a bit, when wanting to have multiple instances loaded


ohhhhh wait... the now compiled version only uses 550MB

##### Olivier Michel [Cyberbotics] 09/22/2020 16:12:21
Did you compile with debug symbols (make debug) or without (make release)?

##### Luftwaffel [Moderator] 09/22/2020 16:12:40
debug


nope, nvm, back at 1,1GB

##### Olivier Michel [Cyberbotics] 09/22/2020 16:13:23
release should produce a more optimized executable (in terms of speed and memory footprint)

##### Luftwaffel [Moderator] 09/22/2020 16:13:38
let me try


could you merge that fix to develop branch real quick?

##### Olivier Michel [Cyberbotics] 09/22/2020 16:14:09
I first need to merge it to master and then merge master into develop.

##### Luftwaffel [Moderator] 09/22/2020 16:14:24
I'll change it by hand until then

##### Olivier Michel [Cyberbotics] 09/22/2020 16:14:30
And I need a review of my colleagues for each stage.


If you approve the PR at [https://github.com/cyberbotics/webots/pull/2260](https://github.com/cyberbotics/webots/pull/2260), it may speed-up a little bit the process.

##### Luftwaffel [Moderator] 09/22/2020 16:16:37
done


recompiling developer branch with those 2 lines changed. will test that as well


is there any good way to see, what exactly takes up the memory space? I tried Massif, but I couldnt make sense of the information

##### Olivier Michel [Cyberbotics] 09/22/2020 16:21:39
I don't know...


Note that you have to recompile Webots, e.g., from `cd webots/src/webots ; make -j 8 release`

##### Luftwaffel [Moderator] 09/22/2020 16:22:22
just did


still a heap of 1.1GB


but the pid fix works 🙂


are you familiar with pybullet?

##### Olivier Michel [Cyberbotics] 09/22/2020 16:24:43
No.

##### Luftwaffel [Moderator] 09/22/2020 16:25:40
it can import and load directly from python. for the Simulation benchmark, we are doing, it takes like 150MB. Doesn't have a full fletched gui, but for RL it is very convenient


there must be ways to drastically decrease the memory footprint of webots, when not everything is needed


`@Stefania Pedrazzi` I'm getting this error with valgrind. seems to be some issue with multithreating
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/758025109008285706/unknown.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 09/23/2020 06:36:21
`@Luftwaffel` I just tested running Webots from the develop branch with valgrind (`valgrind-3.16.1`) and it works correctly for me. Here is command I used:

```
valgrind --log-file=valgrind.out --smc-check=all bin/webots-bin
```

##### Luftwaffel [Moderator] 09/23/2020 07:55:33
Can you try if running valgrind tool=massif works for you?

##### Stefania Pedrazzi [Cyberbotics] 09/23/2020 07:57:23
This works fine as well

`valgrind --tool=massif --log-file=valgrind.out --smc-check=all bin/webots-bin`

##### Luftwaffel [Moderator] 09/23/2020 07:57:39
Is webots actually launching?

##### Stefania Pedrazzi [Cyberbotics] 09/23/2020 07:57:48
yes

##### Luftwaffel [Moderator] 09/23/2020 07:58:06
You you post the output file of massif?

##### Stefania Pedrazzi [Cyberbotics] 09/23/2020 07:59:03
Sorry but I have a meeting now, I will do it later

##### Luftwaffel [Moderator] 09/23/2020 10:20:08
that would be great. Perhaps you can run this too?

`valgrind --tool=massif --depth=5  --alloc-fn=g_malloc --alloc-fn=g_realloc --alloc-fn=g_try_malloc  --alloc-fn=g_malloc0 --alloc-fn=g_mem_chunk_alloc --smc-check=all bin/webots-bin `

##### Stefania Pedrazzi [Cyberbotics] 09/23/2020 10:22:52
Sorry but I have lot of work and unfortunately I cannot debug it for you.

If valgrind doesn't work on your machine it could be that the environment is not properly set or that you have a different version of valgrind.

##### Luftwaffel [Moderator] 09/23/2020 10:29:37
would be nice to just have the output file, should be fairly small and not take long. The one you ran earlier is fine too

##### Stefania Pedrazzi [Cyberbotics] 09/23/2020 11:00:11

> **Attachment**: [massif.out.1006](https://cdn.discordapp.com/attachments/565155651395780609/758281559358046229/massif.out.1006)



> **Attachment**: [massif.out.1049](https://cdn.discordapp.com/attachments/565155651395780609/758281566073651250/massif.out.1049)


Here are the massif log files

##### Luftwaffel [Moderator] 09/23/2020 11:00:27
thx 🙂


just out of curiosity, what does your task manager say about the memory usage of webots-bin, when it is running?


and which version of valgrind are you running? I really want to find out, where that huge heap allocation is coming from. 1,1GB for me

##### Stefania Pedrazzi [Cyberbotics] 09/23/2020 11:27:36
valgrind version is `valgrind-3.16.1`

##### Luftwaffel [Moderator] 09/23/2020 11:28:15
thx

##### Stefania Pedrazzi [Cyberbotics] 09/23/2020 11:31:01
> just out of curiosity, what does your task manager say about the memory usage of webots-bin, when it is running?

`@Luftwaffel` currently it is 1.2 GB

##### Luftwaffel [Moderator] 09/23/2020 11:31:34
that is about the same as mine. I dont quite understand why massif only sees up to 280MB


First preview of parallel RL learning with webots. Still work in progress and lots of optimization to be done though. Still... something satisfying about it 😄
> **Attachment**: [simplescreenrecorder-2020-09-23\_20.44.50.mp4](https://cdn.discordapp.com/attachments/565155651395780609/758410945290043462/simplescreenrecorder-2020-09-23_20.44.50.mp4)


I have bit of a more detailed question. I ran a c-profler on my webots controller, and I noticed that getting position data from object in the scene took very long. Here is a list of the times per call:

node.getPosition() = 23 851 ns

node.getOrientation() = 21 896 ns

Field.getSFVec3f() = 22 563 ns


If we compare that to a sensor:

PositionSensor.getValue() = 450 ns


these timings might seem short, but when having to perform 50 of those supervisor calls at a time, that takes about 1.1 ms. If I were to retrieve this data every timestep, it would be slower than the simulation itself


Is there a way, to get all the data at once? Or any other fast method? I would really like to avoid to turning every little object into a robot with a gps sensor (the timing of which I have not measured).

##### Olivier Michel [Cyberbotics] 09/25/2020 08:12:31
The problem with these calls is that they require sending a request to Webots and waiting for the answer, which takes long.


To speed-up things, we could implement the same mechanism as with sensors where we subscribe to a position/orientation service and get faster batched results.


In practice, that would require to add some API functions like `node.enablePosition(ms)` where ms would be the refresh rate of positions expressed in milliseconds (like in `PositionSensor.enable(ms)`).

##### Luftwaffel [Moderator] 09/25/2020 09:25:26
that would be great


internally the values get retrieved from node fileds right?


would it be feasible to define a list of fields to and get all values at once?


But your suggestion alone would already be huge. Can I help with the implementation?

##### Olivier Michel [Cyberbotics] 09/25/2020 09:28:36
Sure, you are very welcome.


Basically, I would suggest that you open an issue, exposing the problem and proposing a resolution (i.e., based on what I suggested or if you have a better idea, please propose it). Your resolution should include a detailed description of what API functions would need to be implemented and how we would document them. Then, once we agree on the design, we will help to get started with the implementation if needed.

##### Luftwaffel [Moderator] 09/25/2020 09:31:46
sounds good. I will think about it and make a proposal


[https://github.com/cyberbotics/webots/issues/2269](https://github.com/cyberbotics/webots/issues/2269)


I created a feature request, with a proposal on how the API could look like


I have a question regarding the integraded PID controllers, especially for the rotational motors (position control). The default values are 10, 0, 0. But it behaves incredibly stable, with basically no zero state error, even though Ki = 0. How can this be? Is there some secret sauce behind the scenes? Is there some higher frequency calculations, or is it tied to the timestep? Because a typical timestep of 32 or 16 ms, would only mean about 30-60hz, which is very slow for a PID controller.

##### Olivier Michel [Cyberbotics] 09/29/2020 11:05:30
Note that the PID controls the velocity, not the position of the motor which may explain why it is so stable. The position is controlled internally by ODE from the velocity and force and the magic probably is there, inside ODE.

##### Luftwaffel [Moderator] 09/29/2020 11:05:56
ohhhh


so the whole PID controller is inside of ODE?

##### Olivier Michel [Cyberbotics] 09/29/2020 11:07:49
Yes for controlling the position from the velocity.

##### Luftwaffel [Moderator] 09/29/2020 11:07:49
Or is  the translation from position control to velocity controll done by webots?

##### Olivier Michel [Cyberbotics] 09/29/2020 11:08:28
No, everything is computed inside ODE and I am no sure which tricks they use to make it super stable.

##### Luftwaffel [Moderator] 09/29/2020 11:08:49
my brain hurts... So when chaning the PID for a motor node, we change the ODE PID values


but they do not correspond to the mode we are in (position, velocity, torque)

##### Olivier Michel [Cyberbotics] 09/29/2020 11:09:31
No: you change the PID used to control the velocity only.


Not the position.

##### Luftwaffel [Moderator] 09/29/2020 11:09:54
Could you point me to the ODE documentation of the function you are using?

##### Olivier Michel [Cyberbotics] 09/29/2020 11:10:49
See [https://cyberbotics.com/doc/reference/motor#position-control](https://cyberbotics.com/doc/reference/motor#position-control) for the PID controlling velocity in Webots.

##### Luftwaffel [Moderator] 09/29/2020 11:11:05
thank you 🙂

##### Olivier Michel [Cyberbotics] 09/29/2020 11:18:43
Motors in ODE can be controlled either in velocity or in force (resp. torque). See [https://ode.org/ode-latest-userguide.html](https://ode.org/ode-latest-userguide.html). Controlling them in velocity makes the PID control much more stable than controlling then in force.

##### Luftwaffel [Moderator] 09/29/2020 11:27:16
thank you for the information. We are trying to get the motor control consisten across different simulations. This certainly helped


I dont know why I just noticed this, but webots uses a maximum of 8% of my 6 core cpu. I do have hyperthreating enabled, but shouldn't it still be able to use a full core (16%), if no other heavy task is running?


I'm on Ubuntu 18.04. Do you have experience with this?


this is a HUGE performance difference

##### Olivier Michel [Cyberbotics] 09/29/2020 13:40:05
6 cores with hyperthreading means 12 virtual cores, which correspond to 8% of CPU power.

##### Luftwaffel [Moderator] 09/29/2020 13:40:44
On Windows (as far as I remember), the two treads per core can be scaled. the performance per thread is not halfed


per core*

##### Olivier Michel [Cyberbotics] 09/29/2020 13:40:56
Webots runs on a single virtual core unless your simulation can be split into mechanically isolated islands and you enable multi-threading.


The controllers run on different cores, so if they are CPU intensive it will scale well on multi-core CPUs.


The is no fundamental difference between Linux and Windows with respect to thread distribution.

##### Luftwaffel [Moderator] 09/29/2020 13:43:18
having smt enabled should usually not cause such drastic differences. A single thread should still be able to use (almost) a full core, shouldn't it?

##### Olivier Michel [Cyberbotics] 09/29/2020 13:56:15
Yes, if you use the fast mode.

##### Luftwaffel [Moderator] 09/29/2020 13:58:02
hmm it might just be, that the cpu utilization is not reported correctly. Do you know of any tool, that can show you the actual cpu usage of physical cores? Even with smt enabled?


alright, i just tested it. It indeed is the way, system utilization is displayed


turning off smt, does not change the performance


Webots can't be run without a x-server?


we are trying to put everything in a dockercontainer, and it's creating problems

##### Olivier Michel [Cyberbotics] 09/29/2020 14:17:41
No, it needs an X server.


But it's easy to have it in a docker container, even including hardware acceleration. We even provide docker images.


See [https://github.com/cyberbotics/webots/wiki/Docker](https://github.com/cyberbotics/webots/wiki/Docker)

##### Luftwaffel [Moderator] 09/29/2020 14:19:09
thank you 😮


🙂

##### Olivier Michel [Cyberbotics] 09/29/2020 14:19:46
See also [https://github.com/cyberbotics/webots-docker](https://github.com/cyberbotics/webots-docker)


And [https://hub.docker.com/r/cyberbotics/webots](https://hub.docker.com/r/cyberbotics/webots)

##### Luftwaffel [Moderator] 09/29/2020 14:23:36
thx


Quck question for general understanding, is the following correct?:



1. webots/src/Controller/api  -- this is the core functionality of webots

2. webots/src/webots/nodes  -- this is the core functionality for any api (the functionality of nodes api in the documentation gets defined here)

##### Olivier Michel [Cyberbotics] 09/30/2020 10:27:17
1. contains the Webots API (e.g., function that you can call from your controller program)


2. is part of the source code of webots.exe binary that implements nodes in Webots and also communicates with the libController for exchanging messages through a communication pipe.

##### Luftwaffel [Moderator] 09/30/2020 10:28:16
Do you have some sort of flow chart, visualizing, how webots works? 😅


what exactly is the libController. I wanna understand how the basic structure works, and what the components are

##### Olivier Michel [Cyberbotics] 09/30/2020 10:29:13
Unfortunately, we don't have any chart.


libController is a shared library (DLL on Windows, .so file on Linux) that is linked with a controller program to allow the controller (which is a self-standing executable) to communicate with Webots.

##### Luftwaffel [Moderator] 09/30/2020 10:31:05
okay, so src/webots is the core functionality. src/Controller  is the controller, communicating with src/webots?


So, in order to create a new functionality, I have to start with the src/webots/nodes part to create the underlying functionality. Then I have to create the api interface in src/Controller/api

##### Olivier Michel [Cyberbotics] 09/30/2020 10:37:29
Yes. You can start by either end (libController or Webots). At some point they will need to communicate with each other.


I would however recommend you to start on the libController side to implement the API function you need (and which will do nothing at the beginning).

##### Luftwaffel [Moderator] 09/30/2020 10:39:09
hmm okay


am I correct in assuming, thath this here, is the data stream of sensors? And that I have to implement something similar for the fields?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/760813340834136064/unknown.png)
%end

##### Olivier Michel [Cyberbotics] 09/30/2020 10:42:38
Yes, exactly, you will see that the WbSupervisor class has the same methods into which you will have to add your new message and handle it.

##### Luftwaffel [Moderator] 09/30/2020 10:42:52
oh boi... I will try 😄

## October


I have some questions about webots, especially regarding docker


is the xvfb (x virtual frame buffer) only needed for the gui, and is not used, if we run the simulation in fast mode and have no visual sensors?


because xvb is cpu based


Also, in our docker, we get this error:

```
Starting WeBots...
WeBots started with PID=49
Warning: could not connect to display
Info: Could not load the Qt platform plugin "xcb" in "/usr/local/webots/lib/webots/qt/plugins" even though it was found.
Fatal: This application failed to start because no Qt platform plugin could be initialized. Reinstalling the application may fix this problem.
Available platform plugins are: xcb.
/usr/local/webots/webots: line 88:    55 Aborted                 (core dumped) "$webots_home/bin/webots-bin" "$@"
Option f requires an argument
Error: type -h for help
```

##### Olivier Michel [Cyberbotics] 10/01/2020 10:14:10
xvfb is needed in any case.

##### Luftwaffel [Moderator] 10/01/2020 10:16:26
but there should be no gpu usage, if no visual based sensors are included right?


it's merely for the gui


in that case


`@Olivier Michel` Any ideas on the QT error?

##### Stefania Pedrazzi [Cyberbotics] 10/01/2020 10:28:42
usually this Qt error is due to some issues in the path environment variables

##### Luftwaffel [Moderator] 10/01/2020 10:28:55
on my system, I get exactly the same error, when trying to launch bin/webots-bin directly, instead of using the launcher


might be related

##### Stefania Pedrazzi [Cyberbotics] 10/01/2020 10:29:34
Yes, this is because you need to set the LD\_LIBRARY\_PATH and include `<WEBOTS_HOME>/lib/webots`

##### Luftwaffel [Moderator] 10/01/2020 10:29:43
it is set

##### Stefania Pedrazzi [Cyberbotics] 10/01/2020 10:30:16
> on my system, I get exactly the same error, when trying to launch bin/webots-bin directly, instead of using the launcher

`@Luftwaffel` The launcher automatically sets the LD\_LIBRARY\_PATH

##### Luftwaffel [Moderator] 10/01/2020 10:30:21

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/761173153388429322/unknown.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 10/01/2020 10:31:02
To debug this issue, it could be useful to set the `QT_DEBUG_PLUGINS` variable before starting webots


this usually gives more details about why the plugin cannot be loaded

##### Luftwaffel [Moderator] 10/01/2020 10:34:36
error with debug
> **Attachment**: [message.txt](https://cdn.discordapp.com/attachments/565155651395780609/761174222495809566/message.txt)


sorry, first one had html formatting

##### Stefania Pedrazzi [Cyberbotics] 10/01/2020 10:38:27
do you have other versions of Qt installed on the system?

##### Luftwaffel [Moderator] 10/01/2020 10:39:18
I tried solving this error many times, sometimes successfully. But I think either ROS or something else might install it as a dependency


the thing is, it works when using the launcher

##### Stefania Pedrazzi [Cyberbotics] 10/01/2020 10:39:58
When setting manually the LD\_LIBRARY\_PATH do you prepend or append the webots lib folder?

##### Luftwaffel [Moderator] 10/01/2020 10:40:17
right now It is the only one


```
(base) simon@simon-Legion-Y540:~/webots$ echo $LD_LIBRARY_PATH
/home/simon/webots/lib/controller
```

##### Stefania Pedrazzi [Cyberbotics] 10/01/2020 10:41:35
You should also add ``/home/simon/webots/lib/webots``

##### Luftwaffel [Moderator] 10/01/2020 10:41:49
let me try

##### Stefania Pedrazzi [Cyberbotics] 10/01/2020 10:42:55
The `lib/controller` is needed for external controllers.

To run webots core GUI you need the `lib/webots` folder in the path

##### Luftwaffel [Moderator] 10/01/2020 10:42:59
that does it


but it can't be run without gui right?

##### Stefania Pedrazzi [Cyberbotics] 10/01/2020 10:44:18
no, the GUI is part of the Webots core application

##### Luftwaffel [Moderator] 10/01/2020 10:44:39
okay, but thx, this helped. gonna test if it works in docker too

##### Justin Fisher 10/02/2020 14:08:37
Currently Webots seems to restrict each controller to reading only those sensors that are descended from its own robot node.  This makes sense for ordinary controllers.  But supervisor controllers are near-omniscient.  Is there any reason not to let supervisors also read the other sensors in the world? 



(Simon and I were talking about this as another potential solution to the challenge of giving controllers quicker access to certain information, taking advantage of how pre-enabled sensors buffer information for quick controller assess, whereas supervisor field.getX operations are comparatively slow because they require slow messages to be piped back and forth between the API and Webots.  If supervisors could read every sensor, that would be pretty much equivalent to letting them "subscribe" to have quick access to whatever information those sensors detect.)

##### Olivier Michel [Cyberbotics] 10/02/2020 14:13:05
It is basically a design choice for robots: a robot should not be able to access the devices of another robot, which makes sense with respect to real robots. Now, since supervisors are a special kind of robot, we didn't yet implemented the possibility for a supervisor to access the sensor of another robot, but I see no objection to this. It just has to be implemented. Also, since a simulation often contains several robots which have the same sensor names, the supervisors should access devices in a different way, e.g., passing the robot name and the device name as the device name only wouldn't suffice. So, that means some API changes...

##### Justin Fisher 10/02/2020 14:21:45
After the simulation runs for a timestep, I think it iterates over the various sensors, giving them the opportunity to pipe out their current sensor readings to the API so that these readings will then be available for quick access by controllers.  Simon and I think we found the place where position sensors pipe out their current readings, but haven't yet found the place where the iteration is done that triggers them to do so.  Can you point us to that?



(If Simon goes ahead with the plan to make some "subscribed" fields also pipe out their values to make them quickly accessible, that should probably happen at about the same time as this iteration.  Or if we were to give supervisors the ability to read other sensors, we'd probably have to ensure that those other sensors were included in this iteration.)

##### Olivier Michel [Cyberbotics] 10/02/2020 14:25:13
It's basically here: [https://github.com/cyberbotics/webots/blob/master/src/webots/control/WbController.cpp#L917](https://github.com/cyberbotics/webots/blob/master/src/webots/control/WbController.cpp#L917)


This method will collect the answer for each sensor (line 941) and send the buffer of sensor data to the libController (line 951).

##### Justin Fisher 10/02/2020 14:34:41
Thanks.  One issue we'd considered is whether it would make sense to have sensor devices that aren't descended from robots, e.g., a GPS device attached to a freely moving object in the world whose position a supervisor wants to be able to track quickly.  It looks like this code assumes that each sensor will be in a robot, and wouldn't end up collecting answers from sensors that aren't in robots?

##### Olivier Michel [Cyberbotics] 10/02/2020 14:38:25
Yes. Another approach would be to have a supervisor node that includes the whole scene it wants to control, including child robots. That would allow for a cleaner access to robot devices...

##### Justin Fisher 10/02/2020 14:41:15
It hadn't occurred to me that we might want one robot to contain another, but since we're thinking of bending lots of other old "rules" here, that one's probably a good candidate for bending too!  (Regardless, you're right that a solution to the problem of GPS-tagging independent objects would be to make those objects be part of a supervisor.)


Does Webots already allow for nested robots?

##### Olivier Michel [Cyberbotics] 10/02/2020 14:42:15
I believe we worked on that a while ago, let me check the current status.

##### Justin Fisher 10/02/2020 14:43:23
(I could easily imagine wanting to set up a rolling platform with an arm attached, where the platform was one robot with a controller, the arm was descended from it, but was also a separate robot with its own controller)

##### Olivier Michel [Cyberbotics] 10/02/2020 14:45:51
Yes, it is indeed possible.

##### Justin Fisher 10/02/2020 14:46:37
Then it might already be possible for a supervisor to read another robot's sensors, so long as it includes that other robot as a descendant?

##### Olivier Michel [Cyberbotics] 10/02/2020 14:47:08
I am not sure about it, but it would certainly be the easiest to implement.

##### Justin Fisher 10/02/2020 14:51:24
That was my thinking too, which is why I'd suggested this as an alternative to Simon.  Why go to all the work of duplicating the long channel by which enabled sensors pass information out for quick access by controllers, if you could instead take advantage of the fact that sensors are already set up to make such information available quickly, so all we really need to do is let the supervisor read all the sensors?  



And maybe that'll just be as simple as making the supervisor contain everything else.  We'll try it and see!

##### Olivier Michel [Cyberbotics] 10/02/2020 14:57:36
This currently doesn't work. Although a Robot can have another Robot as a child, the parent Robot cannot access the devices of the child Robot. They are just mechanically linked to each other, but not logically linked. However, it would possible to implement it.

##### Justin Fisher 10/02/2020 15:04:45
I guess if it requires a change in implementation, I'd probably suggest that the change should be that every supervisor gets the potential to read every sensor, regardless of where it is in the scene tree (rather than a more limited change, e.g., to allow supervisors to read sensors within descendant robots).  I'd probably also let supervisors send commands to arbitrary devices.  Sure this could conflict with what those devices' own controllers command, but since we're already trusting supervisors with the power to delete these devices at will, it doesn't seem to be any worse to trust them not to send bad commands to them.

##### Olivier Michel [Cyberbotics] 10/02/2020 15:05:30
I agree.

##### Justin Fisher 10/02/2020 15:07:14
So it seems like the biggest change this would require would be in the getDevice command the supervisor uses?  It would need to be allowed to search more broadly to find devices, and maybe also should accept restrictions on where it'll search (e.g., within a particular robot, if there are two clone robots whose devices have the same names).

##### Olivier Michel [Cyberbotics] 10/02/2020 15:07:55
Yes, ideally, it should be "robot\_name.device\_name" or so.

##### Justin Fisher 10/02/2020 15:12:07
It's odd that Webots does have this functionality with DEF-names, and getNodeFromDef but not with device names and getDevice.  Is there a good reason for why these seem to duplicate a lot of each others' functionality?    If I were doing it from scratch, I'd probably do away with device names, and just make getDevice look up DEF-names, allowing for pathing just like getNodeFromDef does.

##### Olivier Michel [Cyberbotics] 10/02/2020 15:16:53
Yes, that's a good remark.

##### Justin Fisher 10/02/2020 15:27:20
Ok, I'll mention to Simon this an alternative way he might try to go to get the quicker access he wants for his supervisor: it may turn out that giving supervisors access to more sensors is a lot easier than trying to get arbitrary "subscribed" fields to pipe out copies of their values for quick access...


Thanks!

##### Olivier Michel [Cyberbotics] 10/02/2020 15:35:34
🤔 I am not sure that will be easier to implement... As it will require to rework some fundamental part inside Webots (like currently a device has a single owner, and this won't be the case any more, and this will induce many changes to fix conflicts here and there...). So this would be actually much more complicated to implement than a subscription-based system...

##### Justin Fisher 10/02/2020 15:36:17
ok, i'll warn him that too


i'd worried about whether there was some point where devices presume they have a single owner


it seems like it would be possible to implement it such that the API device is just there, ready to respond to inquiries from any controller, not just the one that "owns" it


but i have no idea whether it actually is implemented that way!


I think most of the things he wanted quick access to were world objects not situated inside of robots, so it may be that an easy solution to his particular problem is just to put those objects inside his supervisor and attach GPS devices to them for his supervisor to read.  The place where things get tricky is if you want a supervisor to be able to have quick access to what's going on inside another robot, since it currently can't read that robot's sensors and using field.getX functions is slower than Simon wanted.

##### Westin 10/09/2020 18:45:36
I just ran into an issue that I thought I would let you know about. I tried to use the VR view and it caused both SteamVR and Webots to crash. I wonder if it was too much for the GPU to handle. The GPU is at 100% just showing its home page. I then started Webots up again and it crashed immediately while loading the world without without the chance to turn off VR mode. I then disconnected the headset and tried again with the same issue. This makes me suspect the issue was not due to the GPU being overutilized. My solution was to change the SteamVR configuration files such that it uses the null headset, start Webots, then turn off VR mode. Its Webots R2020b and a Pimax headset. The Pimax drivers seem sketchy, so I'm sure that is at least partly to blame.

##### Justin Fisher 10/10/2020 03:45:49
I don't know much about getting VR to work, but another way you might have been able to get Webots running long enough for you to change the settings is by using the command line option to have Webots start with the simulation paused.  (I had to learn that to escape a different insta-crash loop I was stuck in!)

##### Lukulus 10/12/2020 07:59:48
Hello,

Is there a way to get like a world controller, which contains all the elements of a world and the positions?

##### David Mansolino [Cyberbotics] 10/12/2020 08:18:52
Hello, what you are looking for is called the Supervisor API: [https://www.cyberbotics.com/doc/reference/supervisor](https://www.cyberbotics.com/doc/reference/supervisor)


> I just ran into an issue that I thought I would let you know about. I tried to use the VR view and it caused both SteamVR and Webots to crash. I wonder if it was too much for the GPU to handle. The GPU is at 100% just showing its home page. I then started Webots up again and it crashed immediately while loading the world without without the chance to turn off VR mode. I then disconnected the headset and tried again with the same issue. This makes me suspect the issue was not due to the GPU being overutilized. My solution was to change the SteamVR configuration files such that it uses the null headset, start Webots, then turn off VR mode. Its Webots R2020b and a Pimax headset. The Pimax drivers seem sketchy, so I'm sure that is at least partly to blame.

`@Westin` We never tried with the Pimax headset, but have you tried reducing the image resolution in SteamVR (before startign Webots)?

##### Lukulus 10/12/2020 10:51:54
Is there a way to get the bounding box of a node in the supervisor, for calculate the distance between two nodes?

##### Olivier Michel [Cyberbotics] 10/12/2020 10:56:25
Yes, this is possible from the Supervisor API. You should retrieve the `boundingObject` field of the node you want to inspect.

##### Lukulus 10/12/2020 11:02:24
thank you! And is there also a class documentation where I can find class functions for the boundingObject like get int getDistance(boundingObject  destiny, this)?

##### Stefania Pedrazzi [Cyberbotics] 10/12/2020 11:21:03
Currently Webots doesn't provide any specific built-in functions for `boundingObjects` nodes.


Also note that the `boundingObjects` nodes are normal nodes. So you should be able to compute the distance in your controller program using the Supervisor API

##### Westin 10/12/2020 13:13:53
`@Justin Fisher` Thanks for the tip.

`@David Mansolino` If I get a chance, I'll try it out.

##### Justin Fisher 10/13/2020 01:28:07
`@Lukulus` If you're using a simple bounding object, like a sphere shape, the bounding object itself will be centered on the center of the solid that it is the bounding object for.  Suppose that solid has DEF name SOLID42, and that you called your supervisor robot sup.  Then, if your Supervisor is written in Python, you can get that solid's center, in global coordinates, with sup.getFromDef("SOLID42").getPosition().  (If you use some other language than Python, you can look up the equivalent functions for your language in Webots Supervisor docs -- I just gave a Python example because that's what I use.)  You can have more complex bounding shapes, e.g., ones starting with a group or translation field with many children.  In that case, you'd probably want to get an appropriate node from among those children and getPosition() from it.



You can similarly use getPosition() to find the position of the other solid you're interested in, and then you can compute the distance between those two positions with the good old fashioned Pythagorean theorem. 



If you're most interested in whether the objects are *colliding*, you may find it useful to use the supervisor get\_contact\_point functions, though I think those only tell you *where* on your solid the collision happened, not *which* other solid it collided with.

##### Götz 10/19/2020 10:36:38
Hi there, hope this is the right place for this question, let me know if not. I just started to look into Webots and it seems to be what we're looking for: a great simulator that could replace our raspi-based robots in a robot hackathon event we have to convert to virtual now. Sigh. So my question: Our hackathon was based on robots students would control via RESTful API calls. I gave the tcpip controller a test and then tried Python with Flask as a controller to implement a REST interface. But as Flask is not returning (because it's listening itself) the process is killed. Is there a way to implement a RESTful controller API on a higher level directly in Webots I'm overlooking? Or would I have to implement it on a socket-devel base ("low-level" like tcpip controller example) and then interface it with a REST service running "outside" of Webots?

##### Olivier Michel [Cyberbotics] 10/19/2020 10:41:19
Hi `@Götz`, yes this is possible and should be very easy to implement as a Python controller. However, your controller should remain responsive and call the Robot.step() function regularly. This is configurable with most Python HTTP servers, like in tornado for example. I don't have any experience with Flask, but I doubt it wouldn't be possible. Alternatively, you may run the REST server in a separate thread of your Python controller.

##### Götz 10/19/2020 11:07:41
Hey `@Olivier Michel` thanks for the quick reply, sounds good! So I just have to make sure to call Robot.step() regularly from Flask of whatever and it would not get killed? Nice, I'll give it a try. I'd like to avoid getting into thread development, programming not really being in my main skill set... 😉

##### black\_hammer\_67 10/20/2020 16:03:54
I have a quastion about webots debuging, how can I debug my code, I write in python and usually I use pdb but in controller scripts it doesn't seem to work


the programm stops when pdb.set\_trace() is called


but I cannot type to the terminal

##### Olivier Michel [Cyberbotics] 10/20/2020 16:17:11
I usually rely on the print() function to debug my Python controllers. However, it may be possible to use pdb if you start the Python controller yourself with pdb. In that case, you should use an extern Python controller instead of a regular Webots Python controller.


[https://cyberbotics.com/doc/guide/running-extern-robot-controllers?tab-os=linux&tab-language=python](https://cyberbotics.com/doc/guide/running-extern-robot-controllers?tab-os=linux&tab-language=python)

##### black\_hammer\_67 10/20/2020 16:18:58
ok thank you Mr

##### Götz 10/20/2020 17:10:25
I'll give up with getting my RestAPI embedded in a Python controller to work. Last try, maybe somebody can spot anything obviously wrong  here. 
```def robotstep():
    robot.step(32)

if __name__ == '__main__':
    app = Application()
    app.listen(3000)
    tornado.ioloop.PeriodicCallback(robotstep, 16).start()
    tornado.ioloop.IOLoop.instance().start()
```


It's working and I get the "OK" back from here 
```class ForwardHandler(tornado.web.RequestHandler):
    def get(self):
        leftMotor.setVelocity(3)
        rightMotor.setVelocity(3)        
        self.write('OK')
```


But the robot is not doing anything like it's not picked up by robot.step()


Sigh.

##### Olivier Michel [Cyberbotics] 10/20/2020 17:47:10
Can you write some print() statement in the robotstep() function to assess it actually is called?

##### Götz 10/20/2020 17:53:32
Good point... just gave it a try, gets called a lot. 🙂


And when I change robotstep() to 
```def robotstep():
    leftMotor.setVelocity(3)
    rightMotor.setVelocity(3)
    robot.step(32)
    print("robotstep")
```


The robot starts to move forward. But not when left/rightMotor are called from tornados RequestHandler like above.


I just had a session with Simon, the RestAPI controller works for him. So it looks like the issue is somewhere outside of Webots, OS or Python versions. Thanks, Simon!


Looks like the issue was somehow connected to Fedora (tornado version or whatever), works perfectly on Ubuntu 20.04. So my Rest API Controller is on the way. 😃

##### DrVoodoo [Moderator] 10/29/2020 11:08:53
What is the best way to get the Node associated with a given Device? The ultimate aim is to get the orientation of a RangeFinder. Alternatively (and probably better) is for me to use an InertialUnit but I'm not sure how to go from the readings on an InertialUnit to a global orientation

##### David Mansolino [Cyberbotics] 10/29/2020 11:11:06
In Webots R2021a we introduced a new function doing exactly this: ``wb\_supervisor\_node\_get\_from\_device ``, you can already get a beta of this version from the nighly builds: [https://github.com/cyberbotics/webots/releases](https://github.com/cyberbotics/webots/releases)


Here is the documentation: [https://www.cyberbotics.com/doc/reference/supervisor?version=develop#wb\_supervisor\_node\_get\_from\_device](https://www.cyberbotics.com/doc/reference/supervisor?version=develop#wb_supervisor_node_get_from_device)

##### DrVoodoo [Moderator] 10/29/2020 11:12:09
oooh thanks, ok I'll give that a go

##### David Mansolino [Cyberbotics] 10/29/2020 11:12:19
You're welcome 😉

##### DrVoodoo [Moderator] 10/29/2020 11:13:14
but i am still a interested in understanding the inertialunit readings


oh hang on, just had a thought


nope, not that


so if i get the node of an inertialunit and grab the rotation matrix, the inertialunit rollpitchyaw readings appear to be different

##### David Mansolino [Cyberbotics] 10/29/2020 11:20:01
Probably that you have to check that the inertial unit orientation in the robot is correct.

##### DrVoodoo [Moderator] 10/29/2020 11:21:15
x forward, y up, z side to side?


ok will double check

##### Luftwaffel [Moderator] 10/29/2020 14:56:04
Question: How many iterations does webots use for ODE collision detection? Where are the default ODE parameters, which cannot be changed by the api, defined?

##### David Mansolino [Cyberbotics] 10/29/2020 15:04:32
Here is where the ODE step function is called:[https://github.com/cyberbotics/webots/blob/d019ad4906e44a34a94fbea4e110d71af7172c07/src/webots/engine/WbSimulationCluster.cpp#L101](https://github.com/cyberbotics/webots/blob/d019ad4906e44a34a94fbea4e110d71af7172c07/src/webots/engine/WbSimulationCluster.cpp#L101)


And here is where ODE is initialize: [https://github.com/cyberbotics/webots/blob/d019ad4906e44a34a94fbea4e110d71af7172c07/src/webots/ode/WbOdeContext.cpp](https://github.com/cyberbotics/webots/blob/d019ad4906e44a34a94fbea4e110d71af7172c07/src/webots/ode/WbOdeContext.cpp)

##### Luftwaffel [Moderator] 10/29/2020 18:11:22
Webots uses "world step" not "quick step" right?

##### David Mansolino [Cyberbotics] 10/29/2020 18:12:43
Exactly

##### Luftwaffel [Moderator] 10/29/2020 18:13:11
This cannot be changed? (besides changing source code?)

##### David Mansolino [Cyberbotics] 10/29/2020 18:13:44
No, this is currently not open.

##### Luftwaffel [Moderator] 10/29/2020 18:15:36
okay thank you 🙂


(would be cool, if things like this could be changed by the user in the future)

##### David Mansolino [Cyberbotics] 10/29/2020 18:18:19
Feel free to open a feature request 😉

[https://github.com/cyberbotics/webots/issues/new?template=feature\_request.md](https://github.com/cyberbotics/webots/issues/new?template=feature_request.md)

##### Luftwaffel [Moderator] 10/29/2020 18:18:53
will do ^^


[https://github.com/cyberbotics/webots/blob/master/src/ode/ode/src/objects.cpp#L71](https://github.com/cyberbotics/webots/blob/master/src/ode/ode/src/objects.cpp#L71)


am I correct in assuming, that these are the default values for min\_depth and max\_vel?

##### David Mansolino [Cyberbotics] 10/29/2020 20:31:06
Yes you are.

##### Luftwaffel [Moderator] 10/30/2020 15:58:31
I noticed something curious, 2020b runs over twice as fast as 2021a. Any ideas as to why this might be?

##### David Mansolino [Cyberbotics] 10/30/2020 16:01:32
Did you compiled them from sources?

##### Luftwaffel [Moderator] 10/30/2020 16:03:23
2021a I did, 2020b is tarball I'm pretty sure

##### David Mansolino [Cyberbotics] 10/30/2020 16:04:01
Ok, did you by any chance compiled 2021a in debug mode?

##### Luftwaffel [Moderator] 10/30/2020 16:04:27
should be in release. Can i check that after the fact?

##### David Mansolino [Cyberbotics] 10/30/2020 16:05:05
the simplest is to try to recompile in release, if it is doing nothing it means it was already in release

##### Luftwaffel [Moderator] 10/30/2020 16:08:02
jup, was in release


jup, it's just 2-3 times slower

##### David Mansolino [Cyberbotics] 10/30/2020 16:11:07
Can you reproduce this on every simulations (let's say of the guided tour), or only on specific ones?

##### Luftwaffel [Moderator] 10/30/2020 16:14:50
I'll investigate more later


ure sample world. 2020b is 3x faster
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/771775850189291610/unknown.png)
%end


I pulled and recompiled 2021a


and it is purely the "robot.step()" function


that I can gather from our benchmark


some internal physics calculation may have changed

##### Olivier Michel [Cyberbotics] 10/30/2020 16:45:21
From the changelog at [https://github.com/cyberbotics/webots/blob/develop/docs/reference/changelog-r2021.md](https://github.com/cyberbotics/webots/blob/develop/docs/reference/changelog-r2021.md), I cannot see what could be the cause of this performance drop... 🤔

##### Luftwaffel [Moderator] 10/30/2020 16:49:42
perhaps you can do the test yourself, open the ure.wbt world on 2020b and 20201a

##### Olivier Michel [Cyberbotics] 10/30/2020 17:00:00
OK, I just tested it and for me there is no significant difference between 2020b revision 1 (release) and the current develop branch (R2021a).

##### Luftwaffel [Moderator] 10/30/2020 17:00:20
on linux?

##### Olivier Michel [Cyberbotics] 10/30/2020 17:00:30
Instead of using the version you compiled yourself for R2021a, can you try one of the nightly builds?


No, I am on Windows.


I can check on Linux as well...

##### Luftwaffel [Moderator] 10/30/2020 17:01:59
downloading the tar

##### Olivier Michel [Cyberbotics] 10/30/2020 17:07:58
Are you on Ubuntu 20.04?

##### Luftwaffel [Moderator] 10/30/2020 17:08:10
18.04

##### Olivier Michel [Cyberbotics] 10/30/2020 17:08:23
OK, same for me.

##### Luftwaffel [Moderator] 10/30/2020 17:13:52
the tar version runs faster again

##### Olivier Michel [Cyberbotics] 10/30/2020 17:22:13
Good news. This means the problem may come from the way you compiled Webots... Can you try to type "make cleanse" at the top level and "make release" again to recompile the whole thing in optimization mode?

##### Luftwaffel [Moderator] 10/30/2020 17:27:16
I'm currently doing everything fresh, clean pull etc.


I accidentally built the master branch. good news, that one works perfectly


okay, compiled the develop branch. works now too 🙂

##### Olivier Michel [Cyberbotics] 10/30/2020 21:22:25
It could be that you compiled some part in debug mode (either Webots, ODE, Wren or libController)...

##### Luftwaffel [Moderator] 10/30/2020 21:22:59
that's my guess too. I did compile it at some point in debug mode

## November

##### DrVoodoo [Moderator] 11/02/2020 14:09:14
Is anyone able to advise on going from the rpy readings of an inertial unit to the equivilant rotation matrix? Assuming that both the inertial and the device (say a rangefinder) are located at the same position?


So if

```
double roll = inertial->getRollPitchYaw()[0]; 
double pitch = inertial->getRollPitchYaw()[1];
double yaw = inertial->getRollPitchYaw()[2];
```


I think the matrix for that should be

` R = Ry(yaw) * Rx(pitch) * Rz(roll)`


```Ry = 
[ cos(y)    0     sin(y)
    0       1      0
 -sin(y)    0     cos(y) ]
```


```Rx = 
[   1       0      0
    0     cos(p) -sin(p)
    0     sin(p)  cos(p) ]
```


```Rz = 
[ cos(r) -sin(r)   0 
  sin(r)  cos(r)   0
    0       0      1 ]
```


and that should give me the following matrix but I just cannot figure out where its going wrong


```
[ sin(p)sin(r)sin(y) + cos(r)cos(y)    sin(p)cos(r)sin(y) - sin(r)cos(y)    cos(p)sin(y)
  cos(p)sin(r)                         cos(p)cos(r)                         -sin(p)
  sin(p)sin(r)cos(y) - cos(r)sin(y)    sin(p)cos(r)cos(y) + sin(r)sin(y)    cos(p)cos(y)
]
```


it's tantilisingly close, but if I give an example of the resulting matrices


this is the output from the above


```rpyMatrix = 
0.878361 -0.005254 0.477970
-0.062284 0.990156 0.125343
-0.473923 -0.139866 0.869387
```


and this is matrix as pulled off the device using the supervisor


```devicematrix = 
0.487706 0.079376 -0.869392
0.062286 0.990156 0.125342
0.870783 -0.115281 0.477962
```

##### Darko Lukić [Cyberbotics] 11/02/2020 14:31:53
Hello `@DrVoodoo` , I think you are very close, I think it should be:

```
R = Ry(yaw) * Rz(pitch) * Rx(roll)
```


Here is a working example:

```Python
import transforms3d
from controller import Supervisor

robot = Supervisor()

timestep = int(robot.getBasicTimeStep())

imu_device = robot.getInertialUnit('inertial unit')
imu_device.enable(timestep)

imu_node = robot.getFromDef('IMU_SOLID')

while robot.step(timestep) != -1:
    # Print from InertialUnit
    rpy = imu_device.getRollPitchYaw()
    matrix = transforms3d.euler.euler2mat(rpy[2], rpy[1], rpy[0], axes='ryzx')
    # matrix = transforms3d.euler.euler2mat(rpy[0], rpy[1], rpy[2], axes='sxzy')
    
    print(matrix)

    # Print as supervisor
    print(imu_node.getOrientation())
```

##### DrVoodoo [Moderator] 11/02/2020 14:52:13
`R = Rz(pitch) * Ry(yaw) * Rx(roll)` still doesnt work for me but I'll have a look at the python in greater depth and see if I can spot where I'm going wrong


thanks

##### Darko Lukić [Cyberbotics] 11/02/2020 14:59:13
I found a rotation for which the Python example doesn't work. Let me fix it


`@DrVoodoo`, you can check the Python example now, both conventions (extrinsic and intrinsic) work

##### DrVoodoo [Moderator] 11/02/2020 15:26:06
ok but we cant compare the rotation matrix on the intertial unit with the matrix we get from the rpy values because inertial units are meant to be positioned with the x axis along the forward direction


but devices like rangefinders look backwards along the z axis

##### Darko Lukić [Cyberbotics] 11/02/2020 15:30:20
Why not rotating the inertial unit to match the rotation of the range finder?

##### DrVoodoo [Moderator] 11/02/2020 15:30:42
that might be the simplest solution


but the documentation does state that it should be positioned so forward is along the x


and it does mean that the RollPitchYaw would actually be PitchRollYaw (i think)


Thanks `@Darko Lukić` , can't do it now but I'll try just rotating the inertial so it matches the rangefinder and see if that does it

##### Darko Lukić [Cyberbotics] 11/02/2020 16:24:45
> but devices like rangefinders look backwards along the z axis



`@DrVoodoo` It seems inconsistent, but it is done to better match OpenCV convention which is important in case of Camera, Lidar and RangeFinder.

##### Srivastav\_Udit 11/03/2020 10:13:02
Hi! I'm new to using Webots and I am supposed to import a robot (.STL file) from Fusion360. I managed to import the 3D model but I am unsure on how I should proceed to define the joints and write the controller to get the robot moving.

##### DrVoodoo [Moderator] 11/03/2020 10:35:21
Right.


Firstly thanks to `@Darko Lukić` for their help yesterday


Ignoring the documentation and just rotating the inertialunit to the same orientation as the rangefinder was definitely the simplest solution


for the sake of avoiding [https://xkcd.com/979/](https://xkcd.com/979/) here's the solution I ended up with


So I ended up with `R = Ry(yaw)Rz(roll)Rx(pitch)`


which gives
%figure
![CodeCogsEqn.png](https://cdn.discordapp.com/attachments/565155651395780609/773134534190301234/CodeCogsEqn.png)
%end


or in code form


```struct RPY { double pitch, roll, yaw; };
const RPY s { sin(inertial->getRollPitchYaw()[0]),
              sin(inertial->getRollPitchYaw()[1]),
              sin(inertial->getRollPitchYaw()[2]) };
const RPY c { cos(inertial->getRollPitchYaw()[0]),
              cos(inertial->getRollPitchYaw()[1]),
              cos(inertial->getRollPitchYaw()[2]) };

const std::array<double,9> matrix 
{
    c.roll*c.yaw,
    s.pitch*s.yaw - c.pitch*s.roll*c.yaw,
    s.pitch*s.roll*c.yaw + c.pitch*s.yaw,

    s.roll,
    c.pitch*c.roll,
    s.pitch*(-c.roll),
    
    (-c.roll)*s.yaw,
    c.pitch*s.roll*s.yaw + s.pitch*c.yaw,
    c.pitch*c.yaw - s.pitch*s.roll*s.yaw      
};
```


`@Srivastav_Udit` can you give some more details. what sort of robot, have you imported as a single object or as individual parts?

##### Srivastav\_Udit 11/03/2020 10:56:57
`@DrVoodoo` It's a small robot aimed at swarm robotic applications. I'm importing it as a single object but all the joints connecting the individual components are already defined in Fusion360 before saving as a .STL file

##### ChristopherD 11/03/2020 11:06:21
`@Srivastav_Udit` i can't remember how i've imported cad. pretty sure you can just drop the stl in and avoid all intermediate softwares (like in this tutorial they use blender).



read up on what stl actually is, it is simply a file format which saves your geometry from cad as a mesh of triangles. hence it doens't contain any information about your joints. you have to rig your model with joints, mass's, bounding box, physics ect in webots. See the for help



[https://www.youtube.com/watch?v=s151UClAnEk](https://www.youtube.com/watch?v=s151UClAnEk)

##### Srivastav\_Udit 11/03/2020 11:09:29
`@ChristopherD` Thanks! I'll have a look and let you know how it goes!

##### Luftwaffel [Moderator] 11/03/2020 14:29:19
`@DrVoodoo`  For future transformation issues, I find these two things really help:

Transforms3d library

[https://matthew-brett.github.io/transforms3d/](https://matthew-brett.github.io/transforms3d/)

and this to quickly convert and double check specific orientations:

[https://www.andre-gaschler.com/rotationconverter/](https://www.andre-gaschler.com/rotationconverter/)

##### Daniele 11/07/2020 20:20:34
Hi everyone I don't know if it's the right place to write it so tell me if I'm wrong. So I have a code which is supposed to make e-puck robot avoid obstacles and follow the line. It follows the line as it should but when it approaches an obstacle it changes its direction but don't get back to the line. This is the code:


code
> **Attachment**: [test.py](https://cdn.discordapp.com/attachments/565155651395780609/774730160465379368/test.py)


Any help would be appreciated.

##### DrakerDG 11/09/2020 21:55:24
Hello, I am learning Python, what courses or books do you advise me to continue learning?  To be able to program in webots using phyton, since I have only been able to program in C code 😬🙏

##### DrVoodoo [Moderator] 11/11/2020 18:22:14
"Automate the boring stuff with python" is good, highlights where python excels

##### DrakerDG 11/12/2020 05:25:52
`@DrVoodoo` thanks a lot 😁👍🏼

##### csnametala 11/12/2020 20:41:08
Hello everyone!

I am using more than one e-puck robot, and at some point some determined to stop for some reason while the others continue their process, so the problem is that when one is on hold, everyone else is stopping in the simulation.


Please, does anyone have any idea what it could be?

##### Lukulus 11/13/2020 10:48:16
Hi everyone, is there a project on git where a scissor lift is implemented? I am trying to simulate a scissor lift, but I have a lot struggle with fixed connection between two elements..

Is there a way to connect two solids at one point? And do you think webots is appropriate to simulate a sissor lift?

Thanks in advance.

##### Olivier Michel [Cyberbotics] 11/13/2020 10:57:39
I am not aware of such a model implemented in Webots, but I believe you could take inspiration from the Stewart Platform example provided in Webots: [https://cyberbotics.com/doc/guide/samples-demos#stewart\_platform-wbt](https://cyberbotics.com/doc/guide/samples-demos#stewart_platform-wbt) [https://www.youtube.com/watch?v=ddrtiwjKAaY](https://www.youtube.com/watch?v=ddrtiwjKAaY)

##### Luftwaffel [Moderator] 11/13/2020 11:01:55
`@Lukulus` it really depends on what you want to simulate. Does it simply have to act and look correct? Or do you actually have to simulate the mechanics and torque/force transmissions? If it just has to behave correctly, you can simplify the functionality by using a slider joint and linear motor. You can add the "scissors" separately, not being connected to the platform, but moving in unison


I'm not aware of any rigid body simulators that can simulate closed joint loops

##### Lukulus 11/13/2020 11:05:20
if it act and looks like a scissor lift would be sufficient

##### Luftwaffel [Moderator] 11/13/2020 11:05:46
Then I think this will be your best approach

##### Lukulus 11/13/2020 11:06:00
ok, thank you:)

##### Luftwaffel [Moderator] 11/13/2020 11:06:17
let us know if you have further questions about how to do it 🙂


and perhaps post a result, once you're done. I'm sure others are interested too!


<@&568329906048598039> I had an idea the other day and wanted to ask if something like this is already implemented or in the works:

Wizard for creating generic robots (arm, vehicle, legged)


For example, select Tpye: "arm", DOF "6"


it then creates a robot with the correct nodes

##### Lukulus 11/13/2020 11:20:03
`@Luftwaffel` my problem is now, that I need two slider joints. the first lift the platform, the second bring the lower scissor together.

Do you have a idea how to simulate this?


so while the scissor go up, the scissor must slide togehter

##### Luftwaffel [Moderator] 11/13/2020 11:21:00
can you share a screenshot?

##### Olivier Michel [Cyberbotics] 11/13/2020 11:25:37
`@Luftwaffel`: the wizard you describe is neither implemented nor in the works.


If you want to contribute it, we would be happy to review such a PR.

##### Luftwaffel [Moderator] 11/13/2020 11:27:16
I could only feasably do something like that in python. More akin to the urdf2webots tool


However, it would be great, to have it in the GUI like the world wizard


or conroller wizard

##### Lukulus 11/13/2020 11:28:18
this is how i want it to simulate
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/776770415791702016/unknown.png)
%end


slide 2 moves the platform

##### Luftwaffel [Moderator] 11/13/2020 11:30:20
only 1 "scissor" ?

##### Lukulus 11/13/2020 11:30:37
to make it easy for the beginning

##### Luftwaffel [Moderator] 11/13/2020 11:32:10
the easiest would be to ignore the middle fixed point, an turn the other two fixed points into hinge joints with rotational motors, that turn the scissor-blade with the correct rate compared to the vertical slide joint

##### Lukulus 11/13/2020 11:33:05
nice, this shound work 🙂 thank you

##### Luftwaffel [Moderator] 11/13/2020 11:33:19
with simple trigonometry it should be easy to have the angles correlate to the vertical distance

##### Lukulus 11/13/2020 13:16:56
Is it Posible to add more then one Joint to an endPoint of another joint?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/776797750574579742/unknown.png)
%end

##### Luftwaffel [Moderator] 11/13/2020 13:17:17
yes

##### Lukulus 11/13/2020 13:17:18
so if I only use one joint there is no problem


but if i add the second the device isn't found

##### Luftwaffel [Moderator] 11/13/2020 13:18:12
a joint itself is not a device, the motor and sensor are devices


they need unique names too


if you want to have them at different locations, you can put the hingejoints in a transform node

##### Lukulus 11/13/2020 13:20:16
so here it says : Warning: "scissor\_2" device not found.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/776798589896359936/unknown.png)
%end


ohh ok


this i missed


ty!

##### Luftwaffel [Moderator] 11/13/2020 13:20:47
You're welcome!

##### Lukulus 11/13/2020 13:38:19
I get the same warning: Warning: "scissor\_2" device not found:/
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/776803133187555369/unknown.png)
%end


If I only use one joint here it works perfectly, with two: one of them isn't fount


and the controller is crashed

##### Luftwaffel [Moderator] 11/13/2020 13:40:32
hard for me to tell from here


can you share your project? then I can take a look

##### Lukulus 11/13/2020 13:45:31
Das sind die relevanten Stellen.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/776804944258007100/unknown.png)
%end


but if you want I can make a short example project and share

##### Luftwaffel [Moderator] 11/13/2020 13:47:26
the worldfile would be enough

##### Lukulus 11/13/2020 13:59:54
hmm restarted it, and then it worked🤦‍♂️  Sorry for spamming so much ^^

##### Luftwaffel [Moderator] 11/13/2020 14:00:04
np, glad you figured it out 🙂


reloading the world also can help in many cases

##### evoo 11/13/2020 23:46:38
Hi! If I have two robots in my world how do I differentiate one from the other in the controller code? I know how to instantiate one like “ robot = Robot() “ but how about more than one?

##### DrakerDG 11/14/2020 00:09:36
`@evoo` I'm not sure but I think that in the controller code it can be easily differentiated

##### Luftwaffel [Moderator] 11/14/2020 11:39:22
`@evoo` Every controller has the "controller" field. You can have 2 of the same robots, with the same controller. It then get's loaded for each robot separately.


If you are using extern controllers, refer to this: [https://cyberbotics.com/doc/guide/running-extern-robot-controllers#single-simulation-and-multiple-extern-robot-controllers](https://cyberbotics.com/doc/guide/running-extern-robot-controllers#single-simulation-and-multiple-extern-robot-controllers)

##### evoo 11/14/2020 22:09:09
thank u

##### davidzjeh 11/15/2020 10:36:42
Hello, I have a question about how can I control the easiest way the Universal Robots to a specified point in the world?(to catch an object which translation is fix)

##### Darko Lukić [Cyberbotics] 11/16/2020 08:00:07
Hello `@davidzjeh` , you can check the example with ABB IRB 4600:

[https://github.com/cyberbotics/webots/blob/master/projects/robots/abb/irb/controllers/inverse\_kinematics/inverse\_kinematics.py](https://github.com/cyberbotics/webots/blob/master/projects/robots/abb/irb/controllers/inverse_kinematics/inverse_kinematics.py)

##### davidzjeh 11/16/2020 08:08:39
ahh thanks, can I implement this in C? cuz i saw that is using the ikpy python module that don't exist in C

##### Olivier Michel [Cyberbotics] 11/16/2020 08:46:37
For C++, you can use ikfast instead of ikpy. `@Luftwaffel` has implemented it successfully and it's really fast compared to ikpy.

##### davidzjeh 11/16/2020 09:34:32
in this code detail, how can I define the bounds vector for UR robots? I look the UR5e.proto file, I find the rest, but i don't know this vector or is it neccessary for ikpy?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/777828948918403112/unknown.png)
%end

##### Darko Lukić [Cyberbotics] 11/16/2020 09:40:06
[https://github.com/cyberbotics/webots/blob/fb5cd3df739e4773aa677d9445eebd2ab849493f/projects/robots/abb/irb/controllers/inverse\_kinematics/inverse\_kinematics.py#L42-L45](https://github.com/cyberbotics/webots/blob/fb5cd3df739e4773aa677d9445eebd2ab849493f/projects/robots/abb/irb/controllers/inverse_kinematics/inverse_kinematics.py#L42-L45)

You should be able to extract those bounds automatically from the model using `wb_robot_get_urdf`

##### davidzjeh 11/16/2020 10:16:57
||[inverse\_kinematics] Traceback (most recent call last):

[inverse\_kinematics]   File "inverse\_kinematics.py", line 45, in <module>

[inverse\_kinematics]     file.write(supervisor.getUrdf().encode('utf-8'))

[inverse\_kinematics] AttributeError: 'Supervisor' object has no attribute 'getUrdf'

WARNING: 'inverse\_kinematics' controller exited with status: 1.||


I just want to run the example code

##### Darko Lukić [Cyberbotics] 11/16/2020 10:18:16
Which version of Webots do you use?

##### davidzjeh 11/16/2020 10:20:00
R2020a rev1

##### Darko Lukić [Cyberbotics] 11/16/2020 10:20:48
`getUrdf()` is introduced in R2020b

[https://cyberbotics.com/doc/blog/Webots-2020-b-release#ros-2--webots](https://cyberbotics.com/doc/blog/Webots-2020-b-release#ros-2--webots)

##### davidzjeh 11/16/2020 10:22:11
meh 😄 okay, thanks your help, iI will install it


I think I installed it when it's released but the simulation totally lagged and install back the r2020a becasuse in this no lagg, smooth run


now start the simulation and the webots closes down all time..


this happened with my project but on the 2020a version it's fine, but i don't know in 2020b version why it's crash all time without any error message..

##### Darko Lukić [Cyberbotics] 11/16/2020 12:47:45
Can you identify a cause of the crash? It happens when you start `/projects/robots/abb/irb/worlds/inverse_kinematics.wbt`?

[https://github.com/cyberbotics/webots/blob/master/projects/robots/abb/irb/worlds/inverse\_kinematics.wbt](https://github.com/cyberbotics/webots/blob/master/projects/robots/abb/irb/worlds/inverse_kinematics.wbt)

##### Luftwaffel [Moderator] 11/16/2020 13:39:45
`@davidzjeh` what operating system are you on?


I can help you with the ik implementation. I made a very successful and fast implementation, however I only tested it on Linux

##### davidzjeh 11/16/2020 13:42:29
> `@davidzjeh` what operating system are you on?

`@Luftwaffel` win10


it's bad? 😄

##### Luftwaffel [Moderator] 11/16/2020 14:03:42
I havent tried it :p


ikfast has to be compiled. I dont develop on Windows


if you have some experience with c++ compiling on windows, we could give it a shot


`@davidzjeh` If you are interested to try, install this: [https://github.com/yijiangh/ikfast\_pybind](https://github.com/yijiangh/ikfast_pybind)


I can then give you the files needed for your robot

##### davidzjeh 11/16/2020 14:29:27
okey, i will try but now need to debug my world to work fine..

##### Luftwaffel [Moderator] 11/17/2020 12:19:05
[https://github.com/cyberbotics/webots/pull/2460](https://github.com/cyberbotics/webots/pull/2460)


PR done for install documentation

##### Olivier Michel [Cyberbotics] 11/17/2020 12:45:10
Approved! Thank you.

##### Luftwaffel [Moderator] 11/17/2020 12:53:30
nice, much easier for newcomers now 🙂


Out of curiosity, how much would it take, to separate the webots core from the gui? Perhaps in a server / client setup. I see this as a crucial step for wider spread adoption of Webots, especially in the commercial sector, where HPC computers are used for simulation.

##### Olivier Michel [Cyberbotics] 11/20/2020 13:34:10
This is tricky as Webots relies a lot on OpenGL for cameras, range-finders, lidars, etc. So a connection to an X server with OpenGL capability is required. The GUI itself is not an issue as the X server is anyhow mandatory. Webots runs perfectly well in the cloud, on HPC computers and in CI machines as long as a possibly virtual X server is setup (we often use xvfb). We are currently working on providing facilities for deploying Webots simulations in the cloud, including HPC systems. So, this is all coming soon...

##### Luftwaffel [Moderator] 11/20/2020 13:37:03
That's good to hear. I hope your approach is modular, so only things that are needed can be loaded. For example, when not relying on any visual sensors, it would be great not having to load an X server

##### Olivier Michel [Cyberbotics] 11/20/2020 13:46:24
Yes. However, we are not yet at this point, but that's on the roadmap.

##### Luftwaffel [Moderator] 11/20/2020 13:50:01
Awesome! Happy to hear that 🙂

##### Srivastav\_Udit 11/24/2020 14:14:13
Hi, I have imported a robot from Fusion360 to Webots but I am having difficulty defining hinge joints and getting it to move using my controller code. I had defined rigid joints and revolute joints in Fusion360 as well.


The joints haven't been transferred from Fusion360 to Webots as I am importing it in .obj format.


I am facing this issue even after defining the hinge joint in Webots.

##### Olivier Michel [Cyberbotics] 11/24/2020 14:18:37
I would recommend you to start building the joint structure of your robot first in Webots (or with a text editor) and add the 3D parts, one-by-one, in a second step. You should probably follow the Webots tutorials from the user guide to understand how to build the joint structure.

##### Srivastav\_Udit 11/24/2020 14:25:37
Could you please direct me to this tutorial?

##### Olivier Michel [Cyberbotics] 11/24/2020 14:27:37
[https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials) from 1 to 7 at least.

##### Srivastav\_Udit 11/24/2020 14:28:29
Thanks, I'll go through these and get back to you.

## December

##### Luftwaffel [Moderator] 12/01/2020 20:03:00
Question: Are you guys aware that the robot window (where one can monitor devices) 1. takes a huge performance hit, and 2. the performance penalty persists, even when closed. The world has to be restarted to fix it.

##### Olivier Michel [Cyberbotics] 12/02/2020 07:18:12
Yes for 1. Not sure for 2. Maybe 2 would deserve an issue so that we won't forget to investigate it and try to fix it, as it should fixable.

##### Luftwaffel [Moderator] 12/02/2020 10:42:38
`@Olivier Michel` Okay, I'll open an issue.


Would it be feasible to have a user adjustable polling rate in a multiple of timesteps? That could vastly increase performance


Also I suspect the root of the performance is the unbuffered supervisor calls to retrieve fields.

##### Olivier Michel [Cyberbotics] 12/02/2020 10:46:44
Sure, you can use any polling rate. It's better that the polling rate (wb\_robot\_step value / wb\_sensor\_enable value) is a multiple of the World.basicTimeStep for performance.

##### Luftwaffel [Moderator] 12/02/2020 10:46:55
On another note, the "add node" window can be very slow when searching / expanding. This is especially true when having a extra project paths like the community projects. I suspect that a search is done every time. Perhaps this could be once and buffered on world load

##### Olivier Michel [Cyberbotics] 12/02/2020 10:47:46
Yes, it is a known problem. But we have plans to improve this.

##### Luftwaffel [Moderator] 12/02/2020 10:47:59
okay great. so I dont have to open an issueß


?

##### Olivier Michel [Cyberbotics] 12/02/2020 10:48:12
Not for this one.

##### Luftwaffel [Moderator] 12/02/2020 10:48:26
good 🙂


Last question. I managed to get my ikfast controller to work on windows as well. I added all included robot arms of webots. I think this would be a great addition to webots as a standard included controller. The solvers could be precompiled for the different versions and included in the webots/lib/controller/pythonxx path


about 2MB


(per OS / version)


It is vastly superior to ikpy, not just in performance, but also quality

##### Darko Lukić [Cyberbotics] 12/02/2020 11:00:20
`@Luftwaffel` Users can generate the `ikfast` Python library (including C++ library) simply by navigating to a folder with the exported URDF and executing a single command:

```bash
docker run -v ${PWD}:/output cyberbotics/pyikfast [base_link] [effector]
```

(see [https://github.com/cyberbotics/pyikfast](https://github.com/cyberbotics/pyikfast))

##### Luftwaffel [Moderator] 12/02/2020 11:04:20
holy crap you implemented it 😮


pardon my french :p


it is ikfast though? So it will return all possible solutions at once?


My python controller takes these solutions and has algorithms to select the best solution and I also created a cartesian velocity control without any error drift


I think it would be great to have a very simple working solution for the included robots, with a guide / steps to create it for your own robot

##### Darko Lukić [Cyberbotics] 12/02/2020 11:11:31
Yes, it returns a few solutions and a user can pick one.

##### Olivier Michel [Cyberbotics] 12/02/2020 11:13:20
I don't like the idea of having many robot-specific libraries in the webots/lib/controller/pythonxx folder. But a single nice C++ example of using ikfast with some specific robot could be great if it is sufficiently documented in README.md file so that users can generalize it to other robots.

##### Luftwaffel [Moderator] 12/02/2020 11:15:12
I'll take a look at you guy's compiled solvers and see if it is compatible with my controller. Perhaps something can be done here

##### Darko Lukić [Cyberbotics] 12/02/2020 11:16:22
If you can the merge cartesian velocity control and the algorithm that picks the best solution it would be a great contribution

##### Luftwaffel [Moderator] 12/02/2020 11:17:41
Yeah, these work truly great (kinda proud of that). I'll try to flush them out and clean them up

##### Darko Lukić [Cyberbotics] 12/02/2020 11:18:01
Sounds great 🙂

##### Luftwaffel [Moderator] 12/02/2020 12:13:35
[https://github.com/cyberbotics/webots/issues/2493](https://github.com/cyberbotics/webots/issues/2493) robot window bug


`@Darko Lukić` I tried your docker compile for the ur10e. I ran into the same issue as I did with my initial solvers with it requiring lapack and throwing errors because of incorrect header definitions


These issues I only had with the universal robot arms so far

##### Darko Lukić [Cyberbotics] 12/02/2020 15:39:08
We tested only with ABB' IRB 4600. How did you solve the issue?

##### Luftwaffel [Moderator] 12/02/2020 15:39:59
[https://github.com/yijiangh/ikfast\_pybind/tree/master/src/ur5](https://github.com/yijiangh/ikfast_pybind/tree/master/src/ur5)


I copied the setup of the ur5 here and changed the DH parameters to match the ur3e ur5e and ur10e robots


irb4600, p-rob3 and puma work with the ikfast from urdf compiler


(I basically did what you did a few month back)


although your implementation is much cleaner 😄


I did get it to compile on linux after days of troubleshooting, but not on windows

##### Darko Lukić [Cyberbotics] 12/02/2020 15:43:10
That looks too specific to UR robots, maybe we should try to find a better solution

##### Luftwaffel [Moderator] 12/02/2020 15:44:44
I quite like the ikfast\_pybind repo. That's how I implemented my solvers

##### Darko Lukić [Cyberbotics] 12/02/2020 15:44:51
I will not have time the following week, but after that I can look at it

##### Luftwaffel [Moderator] 12/02/2020 15:44:56
and the universals are already added

##### Darko Lukić [Cyberbotics] 12/02/2020 15:51:25
We can improve `pyikfast`. I checked the other solutions but they lacked generalization and they are overcompilcated for users. `pyikfast` generates a Python library in one command. We have to improve support for the UR robots though

##### Luftwaffel [Moderator] 12/02/2020 15:52:19
I think the easiest would be to include the solvers for the existing ones, so just the python wrapper has to be compiled


and or we figure out that lapack / blas issue


maybe we can even put my velocity control and "best solution picker" into the c++ file. That could speed things up


Anyways, in the meantime i'll make an example of my controller with the irb

##### Darko Lukić [Cyberbotics] 12/02/2020 15:55:47
I would prefer to solve the lapack issue as a user may want to build a robot similar to e.g. UR5


That would be better. In that case we can benefit from the velocity control and "best solution picker" in C++ controllers as well

##### Luftwaffel [Moderator] 12/02/2020 15:59:23
Sounds good. Hit me up when you want to tackle the issues. I can show you what I figured out and exact problems exist

##### Darko Lukić [Cyberbotics] 12/02/2020 16:00:05
Nice, thanks, I will 🙂

##### Luftwaffel [Moderator] 12/03/2020 17:16:44
`@Darko Lukić` First version using your pyikfast and my controller.
> **Attachment**: [irb4600\_ikfast\_sample.zip](https://cdn.discordapp.com/attachments/565155651395780609/784105857281425438/irb4600_ikfast_sample.zip)


the controller contains a folder with all files needed to create the python module. To get it to work on Windows I had to change a header.


`pip install .`

should be all you have to do inside the "pyikfast" folder


oh and you'll need transforms3d


`pip install transforms3d`

##### Darko Lukić [Cyberbotics] 12/03/2020 17:24:03
Fantastic! Thank you! Let me discuss with `@Olivier Michel` where is the best place to put it. I believe we should include a nice tutorial that follows the example.

##### Luftwaffel [Moderator] 12/03/2020 17:25:55
there is still some work to do and i'd like to move some functionality into the c++ portion. But it certainly works well  🙂


`@Darko Lukić` I really like your ikfast creation with docker, however, I think it would be good to create seperate python modules for different robots. So people can create several modules. I suggest that instead of naming it "robot.urdf" and having a "pyikfast" module, a custom robot name can be used (or just the default from webots).


For example:

`irb4600_40.urdf `turns into` pyikfast_irb4600_40`  modoule

##### Darko Lukić [Cyberbotics] 12/04/2020 13:26:58
`@Luftwaffel` You can choose the library extension:

[https://github.com/cyberbotics/pyikfast/blob/1b45a9d4401ec4e63a0a2ca1d15ca20e19da315c/entrypoint.bash#L5](https://github.com/cyberbotics/pyikfast/blob/1b45a9d4401ec4e63a0a2ca1d15ca20e19da315c/entrypoint.bash#L5)



So you call:

```
docker run -v ${PWD}:/output cyberbotics/pyikfast base_link solid_12208 _my_robot
```

and the library gets named `pyikfast_my_robot`.

##### Luftwaffel [Moderator] 12/04/2020 13:27:09
Also I noticed, that the "handslot" is not generated in the urdf. Thus there is a mismatch in orientation between the solver and handslot


oh okay, great. It still wants the "robot.urdf" though?

##### Darko Lukić [Cyberbotics] 12/04/2020 13:28:35
Yes, but we can change it

##### Luftwaffel [Moderator] 12/04/2020 13:28:40
I see it in the script


should be fine I think, the module name is what matters

##### Darko Lukić [Cyberbotics] 12/04/2020 13:28:59
Feel free to propose a better approach and we can consider implementing it

##### Luftwaffel [Moderator] 12/04/2020 13:29:23
more important is to adjust the ikfast.h, as it does not work under windows in its current version


a few lines need to be changed

##### Darko Lukić [Cyberbotics] 12/04/2020 13:29:35
Do you have a device or solid in that slot?

##### Luftwaffel [Moderator] 12/04/2020 13:29:56
no, empty slot. Just the robot extracted


I also found a lead for the lapack problem. Lapack library has to be manually linstalled/compiled and then linked. I'm a c++ novice, so I dont know how to do that


[http://openrave.org/docs/0.8.2/openravepy/ikfast/#using-generated-ik-files](http://openrave.org/docs/0.8.2/openravepy/ikfast/#using-generated-ik-files) here it is explained / mentioned

##### Darko Lukić [Cyberbotics] 12/04/2020 13:32:03
If there is no solid in the slot it doesn't make sense to extract it? That was our assumption


Great, I will take a look

##### Luftwaffel [Moderator] 12/04/2020 13:32:47
it should be extracted as an empty link, like many urdf files have


an endeffector transform


I tried adding a Solid in the handslot, however, it does not show up in the urdf. It seems to me, that only "revolute" joints get generated from hingeJoints, but no "fixed" joints from Solids

##### Darko Lukić [Cyberbotics] 12/04/2020 15:26:14
The fixed joints are generated from solids only if the solids have name configured

##### Luftwaffel [Moderator] 12/04/2020 15:41:35
hmm aparently not if a transform node is between two solids

##### Darko Lukić [Cyberbotics] 12/04/2020 16:01:29
It should work even if there are other nodes in between. Could you please submit a bug report?

##### Luftwaffel [Moderator] 12/04/2020 16:07:20
will do


Partially figured it out [https://github.com/cyberbotics/webots/issues/2503](https://github.com/cyberbotics/webots/issues/2503)

##### Steven37 12/06/2020 07:53:23
Hi, I just want to ask a simple question that how can I attach a sensor, for example, the lidar LMS 291 to a robot existing in Webots, Pioneer3 for instance? Can anyone show me, please?

##### Whizbuzzer 12/06/2020 07:59:16
Some robots have an additional spot for that e.g. E-puck has a turret slot where you can add additional nodes. Check out the Pioneer3 documentation

##### Steven37 12/06/2020 08:16:26
I saw this picture in the pioneer3's document so how can I attach the lidar like that?
%figure
![pioneer3at_avoidance.png](https://cdn.discordapp.com/attachments/565155651395780609/785057048277811200/pioneer3at_avoidance.png)
%end

##### Luftwaffel [Moderator] 12/06/2020 13:39:18
`@Steven37` select the extensionSlot, click on the plus on top and select whatever you want to attach
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/785138302314610698/unknown.png)
%end


you can use the "Find" function in the top right of the window to search for "lidar". Once added, use the the translation and rotation field to position the devide. Alternatively you can use the arrows as well
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/785138891249811456/unknown.png)
%end

##### Steven37 12/06/2020 13:50:21
`@Luftwaffel` thank you so so much!

##### Luftwaffel [Moderator] 12/07/2020 11:18:48
Hey `@Darko Lukić`, Could you take a look at the PR [https://github.com/cyberbotics/pyikfast/pull/2](https://github.com/cyberbotics/pyikfast/pull/2) I made for pyikfast? Would be helpful to have that merged for the tutorial. Also, I dont have write permission to merge PRs


Okay this is a bit multilayered:

- In order to address this issue for the included robotic arms [https://github.com/cyberbotics/webots/issues/2503](https://github.com/cyberbotics/webots/issues/2503)

- I plan to use this PR I made [https://github.com/cyberbotics/webots/pull/2513](https://github.com/cyberbotics/webots/pull/2513) (proto splitter update), to convert the existing arms into multi-proto files. This is long overdue, view proto source on the irb4600 freezes my webots for 2 minutes. In addition, I'll make the "toolSlot" name consistent, and turn it into a Solid node. This way it can get converted into URDF. This is also the same behaviour as the urdf2webots script has (since my PR spree on that repo a few month back)

##### Darko Lukić [Cyberbotics] 12/07/2020 13:07:37
`@Luftwaffel` You can also put a dummy node in  `toolSlot` ?

##### Luftwaffel [Moderator] 12/07/2020 13:15:21
do you mean adding a "Transform" as a child by default?

##### Darko Lukić [Cyberbotics] 12/07/2020 13:16:18
`Transform` or `Solid` should do the job

##### Luftwaffel [Moderator] 12/07/2020 13:23:04
I'm also adding names to the Solids


how do I add a Transform node to children, while still having it be the exposed field variable?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/785498357619490846/unknown.png)
%end

##### Darko Lukić [Cyberbotics] 12/07/2020 13:37:12
Sorry, `Transform` will not work. Here is an example:

```
UR10e {
  name "ur10e"
  toolSlot [
    Solid {
      children [
        Solid {
        }
      ]
      name "toolSlot"
    }
  ]
}
```

##### Luftwaffel [Moderator] 12/07/2020 13:37:47
I just added a Transform node as the default value of the MFNode field


works



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/785500409333547078/unknown.png)
%end


[https://github.com/cyberbotics/webots/pull/2516](https://github.com/cyberbotics/webots/pull/2516)


done and tested. No errors for me


`@Darko Lukić` can I have write access? I cannot merge the approved changes
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/785506389707194408/unknown.png)
%end

##### Olivier Michel [Cyberbotics] 12/07/2020 14:05:50
I just added write access to the cyberbotics committers group, so that you should be able to merge it now.

##### Luftwaffel [Moderator] 12/07/2020 14:06:22
works

##### Wesztman 12/22/2020 16:45:51
<#565155651395780609> 



I´ve added the following idea to the discussion on Github :)



[https://github.com/cyberbotics/webots/discussions/2596](https://github.com/cyberbotics/webots/discussions/2596)


`@Darko Lukić`

##### Darko Lukić [Cyberbotics] 12/22/2020 16:48:27
Nice, thank you!

##### Luftwaffel [Moderator] 12/30/2020 16:16:24
The import from url of the Mesh node needs some work. For example, it does not seem to import the creaseAngle of a x3d file. Also the geometry looks slightly different to when I import the same file through the webots gui as a solid.


left is original, right is using Mesh with .x3d files linked.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/793875355092910130/unknown.png)
%end


I exportet the tall middle link of both as a .wrl file. There is some obvious differences. Might point in the right direction. I'm guessing, that the automatic normal generation of the Mesh node has some issue



> **Attachment**: [Shape\_left.wrl](https://cdn.discordapp.com/attachments/565155651395780609/793877246863474688/Shape_left.wrl)



> **Attachment**: [Shape\_right.wrl](https://cdn.discordapp.com/attachments/565155651395780609/793877250230845494/Shape_right.wrl)

##### DrakerDG 12/30/2020 19:30:31
🥰 👍

