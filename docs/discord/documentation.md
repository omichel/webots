# Documentation

This is an archive of the `documentation` channel of the [Webots Discord server](https://discordapp.com/invite/nTWbN9m).

## 2019

##### Flo 05/17/2019 10:03:53
Dear All, is it possible in webots to make the floor reflect (like a mirror) ?

##### David Mansolino [Cyberbotics] 05/17/2019 10:04:43
Hi, have you looked at the mirror object? [https://www.cyberbotics.com/doc/guide/object-mirror](https://www.cyberbotics.com/doc/guide/object-mirror)


You can do something similar, the concept is quite simple, your floor need to be a robot node, have one camera and one display and to attache the camera to the display.


Alternatively you can simply set the roughness of the floor material to 0, but this will reflect only the background not dynamic object.

##### Flo 05/17/2019 10:07:16
ok thanks a lot David, I will check it out !

##### David Mansolino [Cyberbotics] 05/17/2019 10:07:25
You're welcome

##### Flo 05/17/2019 10:08:20
Will I be able to set  the display to be semi-transparent ? So that I can still see the floor under ?


something like they do here : [https://techcrunch.com/wp-content/uploads/2019/01/giphy-5.gif](https://techcrunch.com/wp-content/uploads/2019/01/giphy-5.gif)

##### David Mansolino [Cyberbotics] 05/17/2019 10:09:19
Yes, you simply need to set the 'transperency' of the display appearance to some non-null value

##### Flo 05/17/2019 10:09:25
perfect !

##### TH0 06/11/2019 18:12:50
Hi, there are some minor bugs in the webots tutorial java codes. how do you prefere reports about these mistakes?

##### AnnaLefay 06/11/2019 18:46:49
There are some in the C++ codes too I think.

##### David Mansolino [Cyberbotics] 06/12/2019 06:34:10
Hi `@TH0` and `@AnnaLefay`, please feel free to report them directly on our Github repository as a new issue: [https://github.com/omichel/webots/issues](https://github.com/omichel/webots/issues)

If you want you can also fix them directly by yourself, when you are on the doc you should have a small 'Found an error? Contribute on GitHub!' link on th etop of the page.

##### yacinedz6 06/28/2019 17:45:03
hi, how can i use or download the khepera 4 presentation in the photo to use it in presentation
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155720933146637/594221742008827921/unknown.png)
%end

##### Olivier Michel [Cyberbotics] 07/01/2019 06:29:15
Hi, you can make a screenshot of this page, you are free to use this for your presentations. Or you can link your presentation directly to this web page.

##### Luiz Felipe 07/10/2019 11:25:27
Hello everyone, the webots from the 2019 version is open source. Does a license is still required for older version of webots? Or they are also open source from nwo on?

##### David Mansolino [Cyberbotics] 07/10/2019 11:25:46
Hi


Webots is open source and does not require any license since the R2018a version


previous versions still require a license

##### Luiz Felipe 07/10/2019 11:26:25
thank you 😃

##### David Mansolino [Cyberbotics] 07/10/2019 11:26:32
You're welcome

##### HiguchiNakamura 08/30/2019 10:43:46
Hi, what is the Java variant for setMode function in Robot class?


Only getMode is listed


[https://cyberbotics.com/doc/reference/robot?tab=java#wb\_robot\_set\_mode](https://cyberbotics.com/doc/reference/robot?tab=java#wb_robot_set_mode)

##### David Mansolino [Cyberbotics] 08/30/2019 10:50:46
Hi `@HiguchiNakamura`, let me check.

##### HiguchiNakamura 08/30/2019 10:51:17
OK

##### David Mansolino [Cyberbotics] 08/30/2019 10:55:21
I just checked and the function `setMode` is indeed available in Java too, the Python and Java documentation are missing this fucntion, we will add it as soon as possible, thank you for noticing this.

##### HiguchiNakamura 08/30/2019 10:56:48
Okey, thank you

##### David Mansolino [Cyberbotics] 08/30/2019 10:58:05
You're welcome

##### NaoTeam28 09/23/2019 08:36:37
Hello Guys, 



we got a problem. 



public void setMode(int mode, SWIGTYPE\_p\_void arg);



what do we have to insert for SWIGTYPE\_p\_void arg? We want to switch from Simulated to remote control. We have a real NAO here so we want to run it in real life

##### Stefania Pedrazzi [Cyberbotics] 09/23/2019 09:17:25
Hi `@NaoTeam28` , as explained in the documentation ([https://www.cyberbotics.com/doc/reference/robot?version=fix-contact-properties-doc&tab=c#wb\_robot\_set\_mode](https://www.cyberbotics.com/doc/reference/robot?version=fix-contact-properties-doc&tab=c#wb_robot_set_mode)) `arg` is the argument that will be passed to the remote control `wbr_start` function.


As far as I know there is no default remote controller plugin in Webots for the NAO robot. But you can write your own: [https://www.cyberbotics.com/doc/guide/controller-plugin?tab=c#remote-control-plugin](https://www.cyberbotics.com/doc/guide/controller-plugin?tab=c#remote-control-plugin)

##### HiguchiNakamura 09/24/2019 12:24:54
Hi is there an example for other Robots so I can make it for nao in java

##### Fabien Rohrer [Moderator] 09/24/2019 12:25:12
Hi,


You may find a java example in WEBOTS\_HOME/projects/languages/java/worlds/example.wbt


You can find Nao examples in this directory\_ WEBOTS\_HOME/projects/robots/softbank/nao


There is currently no example of Nao controller written in Java.


But it's indeed possible to write a Java controller for the Nao.


Does this answer your question?

##### Stefania Pedrazzi [Cyberbotics] 09/25/2019 06:21:48
`@HiguchiNakamura` , if you are referring to the remote-control plugin, then it is not possible to write it in Java but only in C.

##### HiguchiNakamura 09/25/2019 12:23:29
`@Stefania Pedrazzi` for every robot only in c or is it just the case for NAO

##### David Mansolino [Cyberbotics] 09/25/2019 12:32:39
Yes, the remote control library should be written in C, but then the controller of the robot iteself can be written in any language.

Please find more information here:

  - [https://www.cyberbotics.com/doc/guide/transfer-to-your-own-robot#remote-control](https://www.cyberbotics.com/doc/guide/transfer-to-your-own-robot#remote-control)

  - [https://www.cyberbotics.com/doc/guide/controller-plugin#remote-control-plugin](https://www.cyberbotics.com/doc/guide/controller-plugin#remote-control-plugin)

##### HiguchiNakamura 09/25/2019 15:55:11
Seems like a lot to do. And there is realy no example for NAO? Not even code snippets? I am not that good in c/c++

##### elnaz 09/26/2019 16:25:23
Hi

does Webots work fine in macOS Mojave?

##### Fabien Rohrer [Moderator] 09/26/2019 19:02:56
Hi, yes it is tested every day in this OS.

##### SimonDK 10/01/2019 14:31:45
I use it in macos and it works great


In Tutorial 5 ([https://cyberbotics.com/doc/guide/tutorial-5-compound-solid-and-physics-attributes](https://cyberbotics.com/doc/guide/tutorial-5-compound-solid-and-physics-attributes)) the link to ODE ([http://ode-wiki.org/wiki/index.php?title=Manual](http://ode-wiki.org/wiki/index.php?title=Manual)), the website is full of advertisement and no documentation. One time I clicked it and it directly downloaded maccleaner which is a POS software. I think the link should be updated or removed.

##### David Mansolino [Cyberbotics] 10/01/2019 14:35:20
You are right, it seems this site is not the official one anymore. Thank you for the notice, we will correct this right now!

##### SimonDK 10/01/2019 14:40:20
Great, thank you for the fast reply `@David Mansolino`


I just remembered, in Tutorial 4 ([https://cyberbotics.com/doc/guide/tutorial-4-more-about-controllers](https://cyberbotics.com/doc/guide/tutorial-4-more-about-controllers)) where you make a new controller for simple obstacle avoidance, I believe the tutorial never mentions to actually add the new controller to the robot in the end. It just stops and concludes.

##### David Mansolino [Cyberbotics] 10/01/2019 14:45:48
You're welcome. The links have been updated here: [https://github.com/cyberbotics/webots/pull/945](https://github.com/cyberbotics/webots/pull/945)

They will be updated on the live version on the website in a few minutes.


> I just remembered, in Tutorial 4 ([https://cyberbotics.com/doc/guide/tutorial-4-more-about-controllers](https://cyberbotics.com/doc/guide/tutorial-4-more-about-controllers)) where you make a new controller for simple obstacle avoidance, I believe the tutorial never mentions to actually add the new controller to the robot in the end. It just stops and concludes.



Ok, I will check.


At the beginning, in the 'Hands on #1' it mentions:

>  Modify the controller field of the E-puck node in order to associate it to the new controller.



Is this what you were looking for ?

##### SimonDK 10/01/2019 15:24:08
Ah yes indeed, missed that. Fortunately I remembered how to do it from one of the previous tutorials 🙂 A suggestion. It would be more intuitive if all "Hands on" followed the style of numbered bullet lists instead of just a full paragraph text. Besides that, I think the tutorials are great, love them!

##### NaoTeam28 10/17/2019 09:27:37
The developers of the NAO specify an effective cone of 60°, but in the Webots documentary it is 45°. The DistanceSensor.getAperture() function returns the value 1.04 in radians, which is 60° in degrees.



What is meant by 45°?

##### Fabien Rohrer [Moderator] 10/17/2019 09:32:28
Hi, could you give us the link where you found 45°?


(The most recent doc about the Nao is there: [https://cyberbotics.com/doc/guide/nao](https://cyberbotics.com/doc/guide/nao) )

##### NaoTeam28 10/17/2019 09:34:49
[https://cyberbotics.com/doc/reference/distancesensor#sonar-sensors](https://cyberbotics.com/doc/reference/distancesensor#sonar-sensors)

##### Fabien Rohrer [Moderator] 10/17/2019 09:36:32
Ok I see...


There are 2 different angles in case of a sonar DistanceSensor.


The first one is given by the customizable DistanceSensor.aperture angle AT THE SENSOR POINT. In case of the Nao, it's 60°. This defines the opening angle of the cone of rays. Indeed a single sensor is modeled by several rays, shown in red if you enable the "View / Optional renderings / Show DistanceSensor rays".


The second one is specific to the sonar type, and is hard-coded in Webots to 45°. It defines the angle beyond which the rays are lost AT THE REFLECTION POINT.


It seems to match quite well the Nao specs. Do you understand better these 2 angles?

##### NaoTeam28 10/17/2019 09:46:45
is it rather the case that the NAO in webots, for example, only recognizes objects in these 45°?


The NAO has two such sensors. Do they together give about this 60°? It should overlap.

##### Fabien Rohrer [Moderator] 10/17/2019 09:49:49
Both NAO sonars have an aperture of 60°: [https://github.com/cyberbotics/webots/blob/revision/projects/robots/softbank/nao/protos/Nao.proto#L720](https://github.com/cyberbotics/webots/blob/revision/projects/robots/softbank/nao/protos/Nao.proto#L720)

##### NaoTeam28 10/17/2019 09:51:11
OK

##### Fabien Rohrer [Moderator] 10/17/2019 09:51:16
This means that at the sonar point, the rays are splitted along 60°.


The 45° occurs at another level, where the rays hit objects. It's like a property of the hit material.

##### NaoTeam28 10/17/2019 09:53:05
ok, thank you

##### BlackPearl 10/17/2019 10:04:56
`@Fabien Rohrer`  so can u explain what is the 45 degree now? Should we focus on 60 degree?

##### Fabien Rohrer [Moderator] 10/17/2019 10:08:17
I do my best to explain it 😉


A DistanceSensor in Webots is modeled internally as a set of rays.


These rays have an origin at the sensor location.


The maximum angle of these rays at the sensor location is given by the DistanceSensor.aperture field, in your case, 1.04 rad = 60°. This is a parameter defined in the Nao PROTO


When the rays hit objects, the rays are rejected if the angle between the ray and the hit material is above 45° => this is internal Webots stuff. I think you can forget this for now 😉


Is it more clear?

##### BlackPearl 10/17/2019 10:14:42
Ah ok, yes 

Thank you very much

##### HiguchiNakamura 10/27/2019 15:41:02
Hi. I think the inertial unit section  (function getRollPitchYaw) has wrong information. 



Some  axis are not correct at roll, pitch and yaw angle.


Pitch has something to do with y-axis, but in webots there are two axis mentioned. The first one is z-axis.


Yaw has something to do with z-axis but the documentation even doesn't include this


I'm not sure what's right now.

##### Stefania Pedrazzi [Cyberbotics] 10/28/2019 07:02:42
`@HiguchiNakamura`, you have to take into consideration that in Webots the up vector by default corresponds to the y-axis and not the z-axis as in other coordinate systems.


The "first" axis mentioned in the documentation corresponds to the rotation axis if the up (or gravity) vector is the default one (i.e. if the up vector corresponds to the y-axis)


then , for pitch and roll the description also specified that this is only true if the up (or gravity) vector is -y-axis

##### dreamerz 10/28/2019 07:08:03
How would you create a custom chassy

##### Stefania Pedrazzi [Cyberbotics] 10/28/2019 07:11:24
`@dreamerz` what do you mean exactly? a complete one including wheels or just a solid piece? 

Did you already look at the tutorials? [https://www.cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot](https://www.cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot)

##### dreamerz 10/28/2019 07:13:11
A complete one, is there a way to make tracks?

##### Stefania Pedrazzi [Cyberbotics] 10/28/2019 07:17:23
yes, here is the documentation of the Track node: [https://www.cyberbotics.com/doc/reference/track](https://www.cyberbotics.com/doc/reference/track)

##### dreamerz 10/28/2019 07:18:05
Thanks

##### Stefania Pedrazzi [Cyberbotics] 10/28/2019 07:18:31
and you can find a simple example in the distributed samples folder `projects/samples/devices/worlds/track.wbt`

##### JoanGerard 10/28/2019 17:25:02
Hello, I was trying to run an external robot controller but there is no <external> option while selecting the controller of my robot. I have the Webots R2019a version. 

Maybe this options is not available any more?
%figure
![Screen_Shot_2019-10-28_at_6.22.02_PM.png](https://cdn.discordapp.com/attachments/565155720933146637/638428024743657522/Screen_Shot_2019-10-28_at_6.22.02_PM.png)
%end

##### Tahir [Moderator] 10/28/2019 17:36:54
`@JoanGerard` this option is available in R2019b not in R2019a


check here [https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers](https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers)

##### JoanGerard 10/28/2019 17:49:50
Thanks `@Tahir` !

##### HiguchiNakamura 10/30/2019 16:47:51
Hello, I wanted to ask if the camera node of the NAO can be extended by a recognition node. But I don't find any children at NAO. So isn't it possible? When a Camera device has a Recognition node in its recognition field, it is able to recognize which objects are present in the camera image.

##### Fabien Rohrer [Moderator] 10/30/2019 16:48:52
`@HiguchiNakamura` Hi


Yes, it's possible.


But I think the simplest is to add a new Camera node (with the recognition feature) in the Nao.headSlot.


As-is, you will have a full control of the new camera fields.

##### HiguchiNakamura 10/30/2019 16:50:52
We want to stay as close as possible to the NAO (so no modifications). But this wouldn't be a mod?

##### Fabien Rohrer [Moderator] 10/30/2019 16:52:00
For sure this is a supplementary camera, but it could be defined exactly as the existing camera embedded in the Nao.


This is the simplest solution, but if this is problematic, you could also either:


- edit Nao.proto, and modify the Camera node as you wish:


[https://github.com/cyberbotics/webots/blob/revision/projects/robots/softbank/nao/protos/Nao.proto#L797](https://github.com/cyberbotics/webots/blob/revision/projects/robots/softbank/nao/protos/Nao.proto#L797)


- or explode the Nao.proto in your simulation (from the scene tree, right click on the Nao, and select "Convert to base nodes") Then the Camera node will be accessible. But the simulation modularity will be penalized 😉


Does this answer your question?

##### HiguchiNakamura 10/30/2019 16:56:48
I tried that out with "convert base node". The Controller wont work after that 😅 . It is planned to test it on a real NAO in the foreseeable future. If we modify, I can't tell if this work out in real life.


I think we're skipping the function.


But thanks

##### Fabien Rohrer [Moderator] 10/30/2019 16:59:34
Ok, so probably that modifying directly the Nao.proto is the best solution for your issue. (you can copy-paste the "protos" directory in your project to work on a local copy)

##### chamandana 11/07/2019 10:35:59
Can anyone give me an example to use RangeFinder in C?

##### David Mansolino [Cyberbotics] 11/07/2019 10:37:28
Hi `@chamandana` an example is distributed with Webots: [https://cyberbotics.com/doc/guide/samples-devices#range\_finder-wbt](https://cyberbotics.com/doc/guide/samples-devices#range_finder-wbt)

##### chamandana 11/07/2019 10:38:02
ah elaela


`@David Mansolino` Thanks, sorry for being ignorant lol

##### David Mansolino [Cyberbotics] 11/07/2019 10:41:35
`@chamandana` no problem, you're welcome

##### chamandana 11/07/2019 10:58:54
another problem, I've never worked with C on Webots. and printf() function doesn't seem to be working. (2019b).

##### Stefania Pedrazzi [Cyberbotics] 11/07/2019 11:06:07
`@chamandana` when using printf in C it is important to end the string with "\n" to force  flushing the buffer

##### chamandana 11/07/2019 11:08:02
many thanks.

##### thrilok emmadsietty 11/10/2019 23:48:26
i am new to this software where can i get the tutorials?

##### Hayden Woodger 11/10/2019 23:49:19
Hi there, I used and still use this website for interactive training 🙂 [https://robotbenchmark.net/](https://robotbenchmark.net/)


The first Tutorial on the Webots website is also a good one to learn  to get started 🙂 [https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials)

##### Marian 12/03/2019 13:00:16
hello

##### Fabien Rohrer [Moderator] 12/03/2019 13:00:37
`@Marian` Hi how could we help you?

##### nitrow 12/03/2019 14:26:37
Hello, it seems like [https://cyberbotics.com](https://cyberbotics.com) has been down for a while now, and thereby also the documentation. Maybe you can help me anyway.. I know that it is possible to download/install Webots through the terminal, can you tell me how?

##### David Mansolino [Cyberbotics] 12/03/2019 14:27:35
Hi `@nitrow`, your are right, we are currenlty updating our servers.


You can find a backup of the documentation here: [https://github.com/cyberbotics/webots/blob/revision/docs/guide/installation-procedure.md](https://github.com/cyberbotics/webots/blob/revision/docs/guide/installation-procedure.md)

##### nitrow 12/03/2019 14:28:22
Thanks a lot!

##### David Mansolino [Cyberbotics] 12/03/2019 14:28:28
You're welcome

##### nitrow 12/04/2019 18:56:32
This command doesn't seem to be working now that the servers are back up "curl -s -L [https://www.cyberbotics.com/Cyberbotics.asc](https://www.cyberbotics.com/Cyberbotics.asc) | sudo apt-key add -" Did it change to something else?

##### David Mansolino [Cyberbotics] 12/05/2019 07:18:43
Hi `@nitrow`, the server is back but we haven't restored everything yet.


For example this file is not available yet, but we are still working on it.


In the meantime you can download Webots directly here: [https://github.com/cyberbotics/webots/releases/tag/R2019b-rev1](https://github.com/cyberbotics/webots/releases/tag/R2019b-rev1)

##### laboris7440 12/05/2019 08:02:06
`@David Mansolino`  Thanks, just what I was looking for!

##### David Mansolino [Cyberbotics] 12/05/2019 08:03:06
`@laboris7440` you're welcome !

##### bsr.nur.bahadir 12/18/2019 19:40:43
Hello, need voice recognition documentation but I couldn't open it. And is there any example project for path planning? 🥺

##### David Mansolino [Cyberbotics] 12/19/2019 06:57:02
Hi `@bsr.nur.bahadir` which voice recognition documentation are you refeerig too? The Webots API provide text to speech but not voice recognition.

About path planninc, you can find some examples here: [https://en.wikibooks.org/wiki/Cyberbotics%27\_Robot\_Curriculum/Advanced\_Programming\_Exercises#Path\_planning\_](https://en.wikibooks.org/wiki/Cyberbotics%27_Robot_Curriculum/Advanced_Programming_Exercises#Path_planning_)[Advanced]

##### bsr.nur.bahadir 12/21/2019 17:51:01
I understand. Thank you so much 🙂

## 2020

##### David Mansolino [Cyberbotics] 01/03/2020 07:23:00
<@!647102363882225664>, unfortunately Webots doesn't support sound sensors for know.

But feel free to contribute to Webots to extend it and support sound sensors, here is the guidlines for adding new nodes (such as a microphone): [https://github.com/cyberbotics/webots/wiki/Adding-New-Node-and-API-Function](https://github.com/cyberbotics/webots/wiki/Adding-New-Node-and-API-Function)

##### TH0 01/05/2020 12:14:18
Hi, can you recommend a good webots tutorial for creating an own robot from 3d parts designed in a CAD programm (e.g. fusion 360)? I'm interessted if there is a good workflow for importing shapes and connecting all parts over joints without manually editing - for example the geometry details - in the proto files, because its so time consuming and not very efficient.

##### ClBaze 01/09/2020 10:47:35
There is a small mistake in the [https://cyberbotics.com/doc/guide/modeling#how-to-make-replicabledeterministic-simulations](https://cyberbotics.com/doc/guide/modeling#how-to-make-replicabledeterministic-simulations) doc.


"The number of threads used by the physics engine (ODE) can be changed either globally in the preferences or using the WorldInfo.basicTimeStep field"


I think it's WorldInfo.optimalThreadCount

##### Stefania Pedrazzi [Cyberbotics] 01/09/2020 10:54:19
`@ClBaze` Thank you for reporting this! I will fix it.

By the way you could also fix it directly in our GitHub repository: [https://github.com/cyberbotics/webots/blob/master/docs/guide/modeling.md](https://github.com/cyberbotics/webots/blob/master/docs/guide/modeling.md)

##### bsr.nur.bahadir 01/19/2020 14:26:48
Is there any instructions of using SUMO on Windows?

##### David Mansolino [Cyberbotics] 01/20/2020 06:57:55
Hi `@bsr.nur.bahadir` the sumo interface is described here: [https://cyberbotics.com/doc/automobile/sumo-interface](https://cyberbotics.com/doc/automobile/sumo-interface)


On Windows it should work the axact same way as on the other OS, do you have any specific issue?


It might be possibkle that the port used by default is not free on your computer, in that case you can simply change it by changign the `port` field of the `SumoInterface` node in the scene-tree.

##### bsr.nur.bahadir 01/20/2020 09:43:13
I was talking about SUMO Exporter  sorry but I solved thank you 🙂

##### 🍎小苹果🍎 01/21/2020 22:43:42
Hi I'm probably asking a really basic thing but I couldn't solve. I was trying to simulate a city I added roads etc everything was fine but when I added the car it's buries into the road. How can I solve it?

##### David Mansolino [Cyberbotics] 01/22/2020 07:01:55
Hi `@🍎小苹果🍎`, the first thing to check is to make sure that you did define the properties of the contact between the ground and the wheels of the car. You should have a contactProperties with a `softCFM` around  1e-05 and a `coulombFriction` around 8. You can for example simply copy paste the contectProperties from the city world: [https://github.com/cyberbotics/webots/blob/master/projects/vehicles/worlds/city.wbt#L13-L73](https://github.com/cyberbotics/webots/blob/master/projects/vehicles/worlds/city.wbt#L13-L73)

##### 🍎小苹果🍎 01/22/2020 08:00:31
Thank you 😃

##### David Mansolino [Cyberbotics] 01/22/2020 08:02:40
You're welcome

##### 🍎小苹果🍎 01/26/2020 20:04:42
I'm trying to make image progressing. I get data with << data = camera. getImage() >> but I can't display it with cv2.imshow( data )   what should I do?

##### David Mansolino [Cyberbotics] 01/27/2020 07:10:51
Hi `@🍎小苹果🍎`, Webots provides an example of controller in python using cv2 to process a camera images: [https://github.com/cyberbotics/webots/blob/master/projects/samples/robotbenchmark/visual\_tracking/controllers/visual\_tracking/visual\_tracking.py](https://github.com/cyberbotics/webots/blob/master/projects/samples/robotbenchmark/visual_tracking/controllers/visual_tracking/visual_tracking.py)

You can't simply use 'data' as a cv2 image, you have to convert it somehow (see for example: [https://stackoverflow.com/questions/17170752/python-opencv-load-image-from-byte-string?answertab=oldest#tab-top](https://stackoverflow.com/questions/17170752/python-opencv-load-image-from-byte-string?answertab=oldest#tab-top))

##### 🍎小苹果🍎 01/27/2020 07:51:22
Thank you so much 🙂

##### David Mansolino [Cyberbotics] 01/27/2020 07:51:38
You're welcome

##### 🍎小苹果🍎 01/30/2020 00:48:26
Hi! Sorry for disturbing you again. I converted the data and made the image progressing. I want to show it with display screen. I have something like 

<<

 img= np.array( array). tolist()  ref=display.imageNew(img,Display.RGBA,h,w)

 display.imagePaste(ref,0,0,blend=false)

 >> 

 when I use cv2 to show I can see the processed image  but when I use display it shows somewhere else in the simulation with 90° rotation where I'm doing wrong? And I'm really sorry for asking too many questions I really tried for days but couldn't solve.

##### David Mansolino [Cyberbotics] 01/30/2020 06:23:01
Hi `@🍎小苹果🍎` if I am not wrong cv2 inverts X and Y coordinate compared to Webots Displays, you should therefore inverse the x an y of your array before converting it to list.

Probably the `numpy.swapaxes` function can help you, with something like this (not tested):

```Python
img= np.array(np.swapaxes(array,0,1)). tolist()  ref=display.imageNew(img,Display.RGBA,h,w)
display.imagePaste(ref,0,0,blend=false)
```

##### 🍎小苹果🍎 01/30/2020 07:48:28
Thank you so much for your help @David Mansolino 😀

##### David Mansolino [Cyberbotics] 01/30/2020 08:02:43
You're welcome

##### nelsondmmg 01/30/2020 09:49:51
I'm trying to use setThrottle but webots return the message "called with an invalid force argument". Should I called a function before to change the vehicle mode to torque control? I'm already waiting 30ms to initiate all sensors

##### David Mansolino [Cyberbotics] 01/30/2020 09:59:36
What is the exact error message you have?

##### nelsondmmg 01/30/2020 10:00:20
Error: wb\_motor\_set\_force() called with and invalid force argument (NaN)


I'm passing 0 as argument, both to setThrottle and setBreakIntensity


I'm also using a Tesla model 3 as vehicle

##### David Mansolino [Cyberbotics] 01/30/2020 10:04:36
Did you engage a gear before calling setThrottle ?

##### nelsondmmg 01/30/2020 10:12:19
I didn't, I tried again with the gear 1 and the same error appears

##### David Mansolino [Cyberbotics] 01/30/2020 10:13:26
would it be possible for you to share this part of code with us so that we can try?

##### nelsondmmg 01/30/2020 10:14:38
I'm going to open a thread at StackOverflow, this app does not work in the company's network


So I'm on the cellphone

##### David Mansolino [Cyberbotics] 01/30/2020 10:15:51
Ok thank you, you can also send us a gist if you want: [https://gist.github.com/](https://gist.github.com/)

##### nelsondmmg 01/30/2020 10:21:41
Just posted on stack, thanks

##### David Mansolino [Cyberbotics] 01/30/2020 10:22:42
Thank you, I will have a look at this today.

##### Ptosidis\_opendr 02/11/2020 10:04:16
Hello, I am trying to convert the mavic2pro controller to python so I can run some experiments with it. (Just starting my webots interaction, so it might be a newbie question). 

I got the following problem: can't seem to find the camera roll device. I tried by camera\_roll\_motor = robot.getDevice('camera roll') but it doesn't seem to work. 

Any insights on how to find all devices by name, or better the names of all devices on a robot?

##### Stefania Pedrazzi [Cyberbotics] 02/11/2020 10:06:22
Hi `@Ptosidis_opendr` in Python controller you should use the method `robot.getCamera('camera roll')` [https://www.cyberbotics.com/doc/reference/robot?tab-language=python#getcamera](https://www.cyberbotics.com/doc/reference/robot?tab-language=python#getcamera)

##### Ptosidis\_opendr 02/11/2020 10:07:43
Hey Stefania, thank you for your fast reply! I might bombard you with question in the near future, so please be patient with me 🙂

##### Stefania Pedrazzi [Cyberbotics] 02/11/2020 10:08:36
No problem!

##### Ptosidis\_opendr 02/11/2020 11:03:35
Hey again, still having some problems.

`WbDeviceTag camera_roll_motor = wb_robot_get_device("camera roll"); `

should translate to


`camera_roll_motor = robot.getCamera('camera roll')`


but this one , even though i get no errors, returns None


nvm it's a Motor, getMotor works fine

##### Olivier Michel [Cyberbotics] 02/11/2020 11:06:26
Yes, you should use `getMotor` instead.

##### 🍎小苹果🍎 02/11/2020 15:31:50
Hi! Is there any example project for object recognition from camera data? I read that camera has recognition node but I couldn't understand too much. Is it possible to recognise a specific object with it ? Or is it possible to use Yolo for this? I'm totally noob someone please help me

##### Fabien Rohrer [Moderator] 02/11/2020 15:32:50
Hi, please take a look at this example: [https://cyberbotics.com/doc/guide/samples-devices#camera\_recognition-wbt](https://cyberbotics.com/doc/guide/samples-devices#camera_recognition-wbt)

##### 🍎小苹果🍎 02/11/2020 16:03:00
Thank you so much


Hi I came back again with my questions 😅  I kinda tried to use multiprocessing but it didn't work. it says no available extern robot controller found and when I try directly run it from simulation it just.. Nothing happens.

##### David Mansolino [Cyberbotics] 02/12/2020 13:10:52
did you revert the simulation befaore starting your controller?

##### 🍎小苹果🍎 02/12/2020 13:23:24
Yeah I did but it's showing it in loop when I close it shows up again until I pause the simulation (sorry for my bad English but I hope you understand me 😭   )

##### David Mansolino [Cyberbotics] 02/12/2020 13:26:16
Don't worry for your english, mine is not perfect too ;-)

Are you calling several time the wb\_robot\_init fucntion from different thread or so ?

##### 🍎小苹果🍎 02/12/2020 13:28:38
Thank you :) No I just call 2 different function 1 is for Lane detection and others for object recognition

##### David Mansolino [Cyberbotics] 02/12/2020 13:44:11
Ok


maybe can you describe your workflow so that we can better understand where can the problem come from

##### 🍎小苹果🍎 02/12/2020 13:53:40
In while loop I'm creating 2 process first one taking image data from camera1 finding lanes and displays it. Second one just taking image data from camera 2 and not doing anything for now cuz I didn't code it yet.  When I start it. It actually works I can see lanes on display but that no extern controller found alert shows up

##### David Mansolino [Cyberbotics] 02/12/2020 13:55:48
just to make sure, did you set the 'controller' field of the robot to <extern> ?


Have you tried with a simpler version without multi-processing to see if you can reproduce the issue?

##### 🍎小苹果🍎 02/12/2020 13:57:46
Yeah I set the controller <extern>. It was working fine before I try multi processing

##### David Mansolino [Cyberbotics] 02/12/2020 13:58:35
ok, can you identify when is the alert display? wich function cause this alert exactly?

##### 🍎小苹果🍎 02/12/2020 14:01:51
in the code when p1.start() to starting processes 1

##### David Mansolino [Cyberbotics] 02/12/2020 14:07:46
Are you calling the wb\_robot\_init before this?

##### 🍎小苹果🍎 02/12/2020 14:10:33
Yeah.

##### David Mansolino [Cyberbotics] 02/12/2020 14:24:40
That's very strange because this alert should only be raised when you initiliaze the robot.

##### 🍎小苹果🍎 02/12/2020 14:27:19
😭 😭  okay thank you so much for helping me. I'll read more probably I'm doing something wrong because I don't know too much about multiprocessing

##### David Mansolino [Cyberbotics] 02/12/2020 14:28:30
OK, good look, sorry that I am not able to help more. But do not desesperate, it is for sure feasible (I did something similar recently with extern controllers too and it was working).

##### 🍎小苹果🍎 02/12/2020 15:13:12
Don't say sorry thank you so much for helping me 🙂

##### David Mansolino [Cyberbotics] 02/12/2020 15:15:42
You're welcome 😉

##### 🍎小苹果🍎 02/12/2020 16:36:05
I found the problem . It was about Windows i needed to use if \_\_name\_\_=='\_\_main\_\_':. 😅 😅

##### Lars 02/14/2020 12:40:46
`@David Mansolino` We found an error in [https://github.com/cyberbotics/webots/blob/master/docs/guide/gripper-actuators.md](https://github.com/cyberbotics/webots/blob/master/docs/guide/gripper-actuators.md)

It says `palm_finger_1_joint_sensor` in both second and third row of the second column

Suggested change for third row, second column: Change to `finger_1_joint_1_sensor` (which is currently not mentioned in the table, but defined in the proto).

See [https://github.com/cyberbotics/webots/compare/master...LarsVaehrens:patch-1](https://github.com/cyberbotics/webots/compare/master...LarsVaehrens:patch-1) the fork

##### David Mansolino [Cyberbotics] 02/14/2020 13:12:39
`@Lars` your patch is completely correct, feel free to open a pull request to include it in Webots 🙂

##### Lars 02/14/2020 13:20:54
`@David Mansolino` Done, made a request to you. 😄

##### Lifebinder (tsampazk) 02/18/2020 11:41:54
Hello, i don't know if its appropriate to tell you this in this channel, but the tooltip of reset simulation (hover over tooltip) says hotkey is control shift F, but when i press that it pops the "Fullscreen mode" window



I just installed the 2020a rev1 version

##### David Mansolino [Cyberbotics] 02/18/2020 11:43:56
Hi `@Lifebinder (tsampazk)`, thank you for reporting this, I will check it.


The tooltip is indeed wrong, the actual hotkey is 'control + shift + t'. I am fixing the tooltip right now.

##### ClBaze 04/16/2020 12:40:37
Hello, I'm using Webots from the develop branch, I've noticed that the ROS service supervisor\_get\_from\_def doesn't work anymore


(oops this probably belongs to the development channel)

##### David Mansolino [Cyberbotics] 04/16/2020 12:42:38
Hi `@ClBaze` is it working on the master branch ?

##### ClBaze 04/16/2020 12:44:19
I haven't tested, but it works with the Webots R2020a revision1 .deb


It seems that the checksum of the get\_from\_def message has changed


`[ros] [ERROR] [1587023118.216799425, 4.456000000]: client wants service /tile_35_4366_pc047/supervisor/get_from_def to have md5sum ac26007a2c83bd1b38318cda0f4ce627, but it has 3f818aa8f7c2c60588c99f7ee189c5bd. Dropping connection.`

##### David Mansolino [Cyberbotics] 04/16/2020 13:03:30
Ok, thank you for reporting this.

May I ask you to open a bug report here: [https://github.com/cyberbotics/webots/issues/new?template=bug\_report.md](https://github.com/cyberbotics/webots/issues/new?template=bug_report.md)

So that we can log this and try to fix it?

Thank you.

##### ClBaze 04/16/2020 13:03:52
ok

##### David Mansolino [Cyberbotics] 04/16/2020 13:04:38
Which version of ROS are you using?

##### ClBaze 04/16/2020 13:04:43
kinetic

##### 🍎小苹果🍎 04/19/2020 09:28:24
May I ask something? I'm trying to change width of road lines for the making lane detection more easy but when I change only one side of the road is changing. there is only 1 dashed road line node exists I couldn't understand how to change other one.
%figure
![road.png](https://cdn.discordapp.com/attachments/565155720933146637/701363563998085170/road.png)
%end

##### David Mansolino [Cyberbotics] 04/20/2020 06:00:37
Hi, by default lines are dashed so the last line is not diplayed in the scene-tree, to be able to change its with you have to add one more `RoadLine` node to the `lines` field.

##### 🍎小苹果🍎 04/21/2020 09:26:51
Thank you so much 😊

##### David Mansolino [Cyberbotics] 04/21/2020 10:03:37
You're welcome

##### PymZoR [Premier Service] 04/21/2020 15:59:54
Hi ! Documentation content seems off since a few minutes

##### Olivier Michel [Cyberbotics] 04/21/2020 16:01:39
Yes, that's right. Thank you for reporting...


Well, it is very slow, but works for me...


Actually the problem is on github...


This URL doesn't load for me: [https://github.com/cyberbotics/webots/blob/master/docs/reference/propeller.md](https://github.com/cyberbotics/webots/blob/master/docs/reference/propeller.md)


And since the doc uses that...


Meanwhile, you can revert to the doc embedded inside Webots.


From the Help menu.

##### Simon Steinmann [Moderator] 04/30/2020 10:03:35
Okay so I finally got it working to quickly and easily calculate any position and orientation of a node relative to any other node. This should be added to the supervisor get\_position and get\_orientation documentation


[https://pastebin.com/k7kf4Ez5](https://pastebin.com/k7kf4Ez5)

##### Olivier Michel [Cyberbotics] 04/30/2020 10:07:41
That is great. Could you create a PR to add this contribution to the doc? [https://github.com/cyberbotics/webots/edit/master/docs/reference/supervisor.md](https://github.com/cyberbotics/webots/edit/master/docs/reference/supervisor.md)

##### Simon Steinmann [Moderator] 04/30/2020 10:08:41
Okay will do. Feel free to change or edit it btw.


in what way should I insert the code? Linked, directly in the description, or in a different way??


a 'expandable box' would be great, no idea how to implement that

##### Olivier Michel [Cyberbotics] 04/30/2020 10:16:01
Yes, that seems to be a good idea. Let me search how to do that...


Something like the **Reminder** and **Tips** here: [https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot#sensors](https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot#sensors) ?

##### Simon Steinmann [Moderator] 04/30/2020 10:18:40
I think I got it 🙂

##### Olivier Michel [Cyberbotics] 04/30/2020 10:19:11
Great. It's the `%spoiler` keyword.

##### Simon Steinmann [Moderator] 04/30/2020 10:24:45
added it


solved it differently

##### Olivier Michel [Cyberbotics] 04/30/2020 10:25:15
OK, looking forward to review it.

##### Simon Steinmann [Moderator] 04/30/2020 10:26:13
oh btw, it's kinda weird that get\_orientation returns a 1x9 list, instead of 3x3


but I guess changing that would break existing code

##### Olivier Michel [Cyberbotics] 04/30/2020 10:28:36
Yes, we may consider changing it on the develop branch.

##### Simon Steinmann [Moderator] 04/30/2020 10:33:59
perhaps adding a function? Would be the non destructive way


get\_orientation\_matrix


perhaps add get\_orientation\_quaternion while you're at it 😉


most commonly used in ROS and any 3D application. would really be helpfull

##### Olivier Michel [Cyberbotics] 04/30/2020 10:35:08
Yes, that seems to be a good idea.

##### Simon Steinmann [Moderator] 04/30/2020 10:36:03
A frustrated coder is full of good ideas 😄

##### Olivier Michel [Cyberbotics] 04/30/2020 10:37:09
😁 . But feel free to go ahead with these good idea and propose an implementation with a PR. That shouldn't be very difficult.

##### Simon Steinmann [Moderator] 04/30/2020 10:39:36
where would I propose that?


do you have something like a trello?

##### David Mansolino [Cyberbotics] 04/30/2020 10:47:24
No we are using directly the Github issue mechanism: [https://github.com/cyberbotics/webots/issues](https://github.com/cyberbotics/webots/issues)

In particular for feature request:

[https://github.com/cyberbotics/webots/issues/new?template=feature\_request.md](https://github.com/cyberbotics/webots/issues/new?template=feature_request.md)

##### Simon Steinmann [Moderator] 04/30/2020 10:54:50
submitted it

##### 🍎小苹果🍎 05/01/2020 16:33:48
hi !  in emitter-receivers how can I calculate aperture?I couldn't understand. from documentation I understand like when it's -1 it sends to 360 degree I want it  send to only 60

##### Olivier Michel [Cyberbotics] 05/01/2020 16:53:21
Then you should set the aperture field value to 1.0472 rad.


which corresponds to 60°

##### 🍎小苹果🍎 05/01/2020 17:31:51
Oh I understand now thank you so much

##### davisUndergrad 05/06/2020 17:21:50
Hello, I am trying to work with the Kuka youBot, and I am having trouble understanding where the origin of the coordinate frame used by the arm\_ik function provided in the arm.c 

library is located. Is this documented somewhere?

##### David Mansolino [Cyberbotics] 05/07/2020 04:53:34
Hi `@davisUndergrad`, it is located at '0.156 0 0' from the robot origin. To see it you have to convert the robot node to base node (right click on the youbot in the scene tree, and press 'Convert to Base Node(s)'.


The you have to select the arm and in the 'Position' tab of the field editor (on the bottom of the scene-tree) you can see the position relative to the robot.

##### davisUndergrad 05/08/2020 03:56:17
`@David Mansolino` thank you for the response!

##### David Mansolino [Cyberbotics] 05/08/2020 05:17:36
You're welcome

##### mgautam 05/12/2020 08:58:37
Hi! I was searching for urdf2webots docs. Does it have one?

##### David Mansolino [Cyberbotics] 05/12/2020 08:59:20
Hi, the documentation is directly in the README file: [https://github.com/cyberbotics/urdf2webots/blob/master/README.md](https://github.com/cyberbotics/urdf2webots/blob/master/README.md)


Let us know if you have any specific question

##### mgautam 05/12/2020 09:00:57
Thanks for fast reply. I was thinking to set up a continuous documentation pipeline for it using sphinx.


If it is needed.

##### David Mansolino [Cyberbotics] 05/12/2020 09:02:07
That would indeed be something very useful, if you want to contribute you are very welcome!

##### mgautam 05/12/2020 09:03:07
Thanks David 👍 I will make a PR on it soon

##### David Mansolino [Cyberbotics] 05/12/2020 09:04:01
You're welcome, looking forward to see your PR!

##### Chaytanya 05/12/2020 22:11:15
Hello everyone,My name is Chaytanya Sinha,I am an engineering student experienced in c/c++,javascript,nodejs,reactjs,html,css,python and kotlin. I am interested in contributing to webots's documentation.I have been following webots since long. I have experience of documentation as I am working on documentation of webpack v5. Please guide me how to proceed towards documentation of webots


I am interested in How-to Guides for Webots and How-to Guides for robotbenchmark projects to contribute

##### Olivier Michel [Cyberbotics] 05/13/2020 06:37:58
Hi `@Chaytanya`, please send an introductory e-mail to support@cyberbotics.com along with you CV and we will answer you.

##### Chaytanya 05/13/2020 06:46:52
Sure `@Olivier Michel`

##### 🍎小苹果🍎 05/13/2020 08:00:17
hi ! why setBrakeIntensity(0)  causes this warning ? How can I solve it ?
%figure
![Untitled.png](https://cdn.discordapp.com/attachments/565155720933146637/710038699077009408/Untitled.png)
%end

##### David Mansolino [Cyberbotics] 05/13/2020 08:03:17
let me check


which language are you using?

##### 🍎小苹果🍎 05/13/2020 08:05:14
Python

##### David Mansolino [Cyberbotics] 05/13/2020 08:09:14
I just tried and driver.setBrakeIntensity(0)  is not raising any warning for me, you may have another call to driver.setBrakeIntensity with a negative value somewhere in your code.

##### 🍎小苹果🍎 05/13/2020 08:10:36
but it says uses 0 instead and when I write setBrakeIntensity(1)  it gaves same warning with ,used 1 instead

##### David Mansolino [Cyberbotics] 05/13/2020 08:11:17
but are you sure you don't have any call with a value smaller than 0? Because for me when I call with 0 I don't have any warning at all.

##### 🍎小苹果🍎 05/13/2020 08:12:00
Okay I'll check again thank you so much :)

##### David Mansolino [Cyberbotics] 05/13/2020 08:12:06
You're welcome

##### lojik 05/13/2020 14:51:20
Hello everyone, how can we help to document webots ? I would be happy to take part to improve it.



I think there is a mistake on the following part : [https://www.cyberbotics.com/doc/reference/worldinfo](https://www.cyberbotics.com/doc/reference/worldinfo) on the basicTimeStep part it is written that the minimum value could be 0.001 (one microsecond), but in the table at the beginning, the variable should be [1, inf). If we put less than 1, the simulation does not start in the example accelerometer.

##### David Mansolino [Cyberbotics] 05/13/2020 14:52:55
Hi, thank you for pointing this out, you can edit this directly on github and create a PR for merging your changes, here is the editor (you need a Github account): [https://github.com/cyberbotics/webots/edit/master/docs/reference/worldinfo.md](https://github.com/cyberbotics/webots/edit/master/docs/reference/worldinfo.md)

##### lojik 05/13/2020 14:55:35
Thank you, I already have a github account so I will do that. The right way is to fork the repository and then purpose a pull request with my changes ?

##### David Mansolino [Cyberbotics] 05/13/2020 14:56:55
Yes exactly.

##### lojik 05/13/2020 15:06:06
Do you prefer to do the pull request directly in master or in an other branch ?

##### David Mansolino [Cyberbotics] 05/13/2020 15:07:12
For doc correction you can target master directly

##### Simon Steinmann [Moderator] 05/13/2020 15:22:20
`@David Mansolino`  I made a commit here [https://github.com/cyberbotics/webots/edit/master/docs/reference/supervisor.md](https://github.com/cyberbotics/webots/edit/master/docs/reference/supervisor.md)

##### David Mansolino [Cyberbotics] 05/13/2020 15:25:07
Ok perfect, can you then open a pull-request from the branch where you did the commit so that we can review and merge it?

##### Simon Steinmann [Moderator] 05/13/2020 15:26:19
I can't find my commit

##### David Mansolino [Cyberbotics] 05/13/2020 15:26:46
Did you fork the repo?


> I can't find my commit

`@Simon Steinmann` me neither

##### Simon Steinmann [Moderator] 05/13/2020 15:27:46
great 😩


I submitted it April 30th, any way to check all activity? perhaps I submited it to a weird branch. Not that familiar with github

##### David Mansolino [Cyberbotics] 05/13/2020 15:35:21
Let me check if I can find a stale branch, by the way what is your Github username?

##### Simon Steinmann [Moderator] 05/13/2020 15:37:33
simon-steinmann

##### David Mansolino [Cyberbotics] 05/13/2020 15:40:17
Thank you, I will check and let you know

##### Olivier Michel [Cyberbotics] 05/13/2020 15:53:33
`@Simon Steinmann`: It's here [https://github.com/cyberbotics/webots/compare/master...Simon-Steinmann:patch-1](https://github.com/cyberbotics/webots/compare/master...Simon-Steinmann:patch-1)


You have to click the "Create pull request" button.

##### Simon Steinmann [Moderator] 05/13/2020 15:55:41
`@Olivier Michel`  thank you so much <3. I created a pull request

##### Olivier Michel [Cyberbotics] 05/13/2020 15:58:07
Thank you. We will review it soon.

##### Prubhtej Singh 05/19/2020 19:10:22
I just had a small query.


When was documentation last updated ?

##### David Mansolino [Cyberbotics] 05/20/2020 04:50:52
Hi, the documentation is updated directly from our Github repository, you can see all the latest changes here: [https://github.com/cyberbotics/webots/tree/master/docs](https://github.com/cyberbotics/webots/tree/master/docs)

##### Prubhtej Singh 05/20/2020 05:31:12
Thanks for the reply. I'll definitely check it out.

##### David Mansolino [Cyberbotics] 05/20/2020 05:37:11
You're welcome

##### NitishGadangi 06/06/2020 18:47:41
Hello `@Olivier Michel` , This is regarding Season Of Docs 2020

I have mailed you the details about me and few doubts about the idea I am interested in.

Could you please check that out and ping me back

##### Olivier Michel [Cyberbotics] 06/08/2020 06:20:06
`@NitishGadangi`: Sure I will.

##### ana.dospinescu 06/16/2020 20:17:02
Hello!

Can somenone help me with the equivalent of functions in Webots R2020a version for:

wb\_differential\_wheels\_set\_speed

wb\_differential\_wheels\_enable\_encoders

wb\_differential\_wheels\_set\_encoders

##### David Mansolino [Cyberbotics] 06/17/2020 05:26:17
Hi, in R2020a, you should instead use the right and left motor instead.

```
wb_differential_wheels_set_speed
```

Becomes something like:

```
  WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);
  wb_motor_set_velocity(left_motor, 0.0);
  wb_motor_set_velocity(right_motor, 0.0);
```

And for the encoders, you should use instead the position sensors:

```
// get a handler to the position sensors and enable them.
WbDeviceTag left_position_sensor = wb_robot_get_device("left wheel sensor");
WbDeviceTag right_position_sensor = wb_robot_get_device("right wheel sensor");
wb_position_sensor_enable(left_position_sensor, TIME_STEP);
wb_position_sensor_enable(right_position_sensor, TIME_STEP);
double left_encoder_value = wb_position_sensor_get_value(left_position_sensor);
double right_encoder_value = wb_position_sensor_get_value(right_position_sensor);
```

##### ana.dospinescu 06/17/2020 09:41:50
`@David Mansolino` Thank you!

##### David Mansolino [Cyberbotics] 06/17/2020 09:42:18
You're welcome

##### Simon Steinmann [Moderator] 07/07/2020 11:41:29
`@David Mansolino` I created a PR [https://github.com/cyberbotics/webots/pull/1879](https://github.com/cyberbotics/webots/pull/1879)

##### David Mansolino [Cyberbotics] 07/07/2020 11:51:08
Thank you, we will have a look soon!

##### Laojiang 07/20/2020 02:21:35
Hello, how can I download the documentation like user guide?

##### David Mansolino [Cyberbotics] 07/20/2020 05:36:54
This is unfortunaely not possible. However you can view it offline in Webots from the 'Help' menu.

##### lance 08/19/2020 00:08:18
Hi, in a project I am trying to write a adaptive controller for the DJI  mavic 2 pro drone and, therefore, require some detailed parameters of the drone model. I have read the proto file and I am still confused about where to find parameters like moment of inertia around 3 axis, total mass of the drone, and distance from center of mass to the rotor. I am wondering where I can find those parameters? Thanks in advance！

##### David Mansolino [Cyberbotics] 08/19/2020 05:44:59
Hi `@lance`, you can get many of these information directly in Webots, if you select the drone, in the node editor (part below the scene-tree) you have a 'Mass' tab that allows you to retrieve many information. Then the simplest solution to get the rotor/propeller position, the simplest solution is to convert temporarily the PROTO node in Base node (right clik on the ndoe in the scene-tree => 'Convert to Base Node(s)') then you can select the propeller node and chek it's position (relatively to any parent node in the 'position' tab of the node editor.

##### lance 08/20/2020 00:11:27
Thank you `@David Mansolino` ! In the mass tab, it seems that moment of inertia is only shown with excluding descendants option, do I have to calculate the moment of inertia including descendants by myself?

##### David Mansolino [Cyberbotics] 08/20/2020 05:20:47
`@lance` what you can do is to convert the PROTO node to base node (right click on the node in the scene-tree => Convert to Base Node(s))


Then you will be able to select the children Solid nodes and get their inertia matrix too.

##### Kamil Kaya 08/21/2020 11:31:48
Hi, How can I use display node? When I add it to the child node of camera controller crashes.

##### David Mansolino [Cyberbotics] 08/21/2020 11:33:36
Hi, you should have a look at the examples provided within Webots, e.g. [https://cyberbotics.com/doc/guide/samples-devices#display-wbt](https://cyberbotics.com/doc/guide/samples-devices#display-wbt)

##### Kamil Kaya 08/21/2020 16:57:44
But Can I put the display node to camera as child node? I want to achieve the hough circle transform by taking frames from the camera, processing it and after than showing it to display screen.

##### David Mansolino [Cyberbotics] 08/24/2020 05:51:00
You don't need to put it in the camera as child to do this, you just need to retrieve the image of the camera, process it and then use the display functions to draw on it: [https://cyberbotics.com/doc/reference/display#display-functions](https://cyberbotics.com/doc/reference/display#display-functions)

##### Kamil Kaya 08/24/2020 10:15:49
> You don't need to put it in the camera as child to do this, you just need to retrieve the image of the camera, process it and then use the display functions to draw on it: [https://cyberbotics.com/doc/reference/display#display-functions](https://cyberbotics.com/doc/reference/display#display-functions)

`@David Mansolino` Thank you. I achieved it as you refer.

##### David Mansolino [Cyberbotics] 08/24/2020 10:16:57
You're welcome

##### Awadhut 09/15/2020 01:30:32
Hi guys. I have a query about use of sensors in webots. Specifically, I want to poll pointcloud data and publish it on a rostopic, just like one that could be obtained from a stereo camera like the intel realsense.



I am new to webots and found it hard to find resources to do this. Can somebody point me to some examples for achieving this?

##### David Mansolino [Cyberbotics] 09/15/2020 05:07:26
Hi `@Awadhut`, the device you are looking for is the Range-finder: [https://www.cyberbotics.com/doc/reference/rangefinder](https://www.cyberbotics.com/doc/reference/rangefinder)


If you use it together with the default 'ros' controller ([https://cyberbotics.com/doc/guide/using-ros](https://cyberbotics.com/doc/guide/using-ros)), it will publish a depth image: [https://www.cyberbotics.com/doc/reference/rangefinder?tab-language=ros#wb\_range\_finder\_get\_range\_image](https://www.cyberbotics.com/doc/reference/rangefinder?tab-language=ros#wb_range_finder_get_range_image)


If you want directly a ROS point cloud, an alternative is the lidar node: [https://www.cyberbotics.com/doc/reference/lidar?tab-language=ros#wb\_lidar\_get\_point\_cloud](https://www.cyberbotics.com/doc/reference/lidar?tab-language=ros#wb_lidar_get_point_cloud)


In any case, to get familiar with the Webots-ros interface, you should probably read this:

[http://wiki.ros.org/webots\_ros](http://wiki.ros.org/webots_ros)

[http://wiki.ros.org/webots\_ros/Tutorials/Sample%20Simulations](http://wiki.ros.org/webots_ros/Tutorials/Sample%20Simulations)

##### Simon Steinmann [Moderator] 09/21/2020 09:17:21
`@Stefania Pedrazzi` I created an issue about the high cpu usage on <extern> waiting


another question: I'm working on Reinforcement learning, and I'm wondering how much overhead there is, and if webots can be launched in a minimal way without GUI. Ultimateley, it would be great to be able to launch many simulation instances for parralelized learning


or would it be better to launch multiple robot instances in the same simulation to train

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 09:23:09
usually it is better to run multiple instances of Webots.

It is not possible to launch Webots without GUI, but there are options to avoid useless renderings:

- start webots with `--batch` and `--minimize` options

- run the simulation in `fast` mode (`--mode=fast` in starting option) to disable rendering of the main 3D view

##### Simon Steinmann [Moderator] 09/21/2020 09:37:08
webots has a fairly large memory footprint. 1.1GB with only a very small and simple simulation. Any methods to decrease that?


990MB of that is 'heap', only 113MB is the engine
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155720933146637/757536398948565033/unknown.png)
%end


is the heap shared between multiple instances?


I have very limited understanding of memory stacks, heaps and all that

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 09:43:43
Why are you comparing with the `libQtWebEngineCore`? Note that this is only a minimal part of the Webots application or even a Qt application that is just used for showing the documentation and robot window.


By the way I don't think that the heap is shared between different processes.


We regularly run successfully multiple Webots instances on power enough machines.

##### Simon Steinmann [Moderator] 09/21/2020 09:56:08
is there  documentation for multiple instances?

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 09:56:42
no. What kind of documentation do you need?

##### Simon Steinmann [Moderator] 09/21/2020 09:57:28
I remember reading somewhere, that you have to specify the PID of the webots process you want to connect to


let's say I wanna run 10 instances of the same simulation with the same robot


and the same controller

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 09:58:21
Are you using ROS or extern controllers?


Otherwise if the simulation quits itself, you don't need to know the PID of the process

##### Simon Steinmann [Moderator] 09/21/2020 10:02:16
[https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers#multiple-concurrent-simulations](https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers#multiple-concurrent-simulations)


found it


gonna try it out


for larger scale Reinforcement Learning training, it would be very beneficial, if webots could be launched in a minimal way. Maybe a bit like gazebo server. Where only the absolute base things get loaded. 1,1 GB is quite a bit, if we want to scale it up. Deep Neural Networks can eat up quite some memory too


can you point me to a way in the code, or documentation, where the different components get loaded. Or do you know of some tools, to inspect memory allocation and its sources?


I already got pretty far with webots over the weekend (based on my previous work). Where I can train a robotic arm in a openai style environment. And I got it really fast. One simulation step takes less than 1ms, including inverse kinematics, simulation, get\_obs and reward calculation


Plus webots excellent API support makes it an amazing tool


But I'm worried about scalability

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 10:10:20
The starting options I pointed out are the way to start Webots in the minimal way.

To inspect memory you could use `valgrind` [https://github.com/cyberbotics/webots/wiki/Valgrind](https://github.com/cyberbotics/webots/wiki/Valgrind)

##### Simon Steinmann [Moderator] 09/21/2020 10:11:00
I tried started webots like you suggested:

webots --batch --minimize --mode=fast

still 1,1GB memory per instance


thanks, I'll have a look at Valgrind


how do I compile webots in debug mode?


And which branch should I use? I always install the latest nightly with the tar method


which is the correct branch for R2021a?


develop?


[https://github.com/cyberbotics/webots/wiki/Linux-installation#build-webots](https://github.com/cyberbotics/webots/wiki/Linux-installation#build-webots) pretty sure that should say 

"For example, type make -j12 on a CPU with six cores and hyper-threading." and not "four cores"

##### R\_ 09/21/2020 10:48:04
Hi! is there an example for closed loop control of a robot in the sample worlds?

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 10:59:36
`@Simon Steinmann` to compile in debug mode simply type `make debug -jX`.  For R2021a you have to choose the `develop` branch . I will fix the number of cores in the documentation.

##### Simon Steinmann [Moderator] 09/21/2020 11:00:17
`@Stefania Pedrazzi` thanks! Please add the 'debug' as well in the documentation


it is 'debug' and not '-debug' or '--debug' ?

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 11:01:14
yes, it is `make debug`. This a standard Makefile command

##### Simon Steinmann [Moderator] 09/21/2020 11:01:21
thx 🙂


but please include it in the documentation. Not everyone is very familiar with compiling (points at myself) 😄

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 11:09:11
`@R_` there is an example of PID control here: [https://www.cyberbotics.com/doc/guide/samples-devices#position\_sensor-wbt](https://www.cyberbotics.com/doc/guide/samples-devices#position_sensor-wbt)

##### Justin Fisher 09/21/2020 11:09:14
> Hi! is there an example for closed loop control of a robot in the sample worlds?

`@R_` Many of the sample controllers probably count as "closed loop control".  E.g., one that comes to mind is the Lego Mindstorms sample ([https://cyberbotics.com/doc/guide/mindstorms](https://cyberbotics.com/doc/guide/mindstorms)) that maintains a brightness reading on its sensors to follow a line on the ground.   Or is that not what you were hoping for?

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 11:12:01
> but please include it in the documentation. Not everyone is very familiar with compiling (points at myself) 😄

`@Simon Steinmann` the wiki page already points out that you can get all the available targets (including for debugging) by running `make help`.

##### Simon Steinmann [Moderator] 09/21/2020 11:12:46
😅  reading would help


but I discovered a potential problem


cat scripts/install/bashrc.linux >> ~/.bashrc


this script assumes home/user/webots install


it will screw things up if people install it somewhere else

##### R\_ 09/21/2020 11:14:08
Thank you! `@Justin Fisher` `@Stefania Pedrazzi`

##### Simon Steinmann [Moderator] 09/21/2020 11:15:20

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155720933146637/757560595988873246/unknown.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 11:15:44
`@Simon Steinmann` you should properly set the `WEBOTS_HOME` environment variable

##### Simon Steinmann [Moderator] 09/21/2020 11:15:55
it is properly set



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155720933146637/757561010591629372/unknown.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 11:17:23
Did you adjust the bashrc values to match your environment?


for ros issue you should try to reset the `ROS_DISTRO` variable before compiling Webots

##### Simon Steinmann [Moderator] 09/21/2020 11:26:03
I fixed it, for some reason, resources/webots\_ros was modified. I reverted the change and  pulled again


`@Stefania Pedrazzi` valgrind failed. it is version 3.13.0
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155720933146637/757564208928784395/unknown.png)
%end


should I do the 3.12 fix, mentioned in the documentation?

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 11:31:03
you should first check that the LD\_LIBRARY\_PATH contains `$WEBOTS_HOME/lib/webots`

##### Simon Steinmann [Moderator] 09/21/2020 11:31:44
let me check, it seems the old installation is still interfering


I fixed the LD\_LIBRARY\_PATH to only include the new directory and recompiled. Still getting this error
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155720933146637/757569019434893372/unknown.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 11:51:36
this seems a configuration issue and not a valgrind issue.

do you get the same error if you start webots only with `bin/webots-bin` (without valgrind)?

##### Simon Steinmann [Moderator] 09/21/2020 11:53:38
that started fine. I'm now redoing everything in /home/webots


`@Stefania Pedrazzi` my apologies, the -j12 for a 4 core was correct. "make help" tells me to do -j18, as I have a 6 core multithreaded cpu. It's cpu-threads * 1.5


maybe mention in the documentation to just run "make help" to get the correct command

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 13:35:17
Yes, I didn't change it at the end, because the computation was correct.


> maybe mention in the documentation to just run "make help" to get the correct command

`@Simon Steinmann` it is already mentioned in the wiki page

##### Simon Steinmann [Moderator] 09/21/2020 13:36:35
In the end it doesnt matter much, but a note, that the number is different from your threadcount, could help

##### Stefania Pedrazzi [Cyberbotics] 09/21/2020 13:37:27
There is no correct or wrong number of threads to use. The one printed in the help message is just a suggestion.

##### Meenakshi Prabhakar 10/11/2020 15:50:33
is there any example code for a 4 wheeled robot with line following and obstacle avoidance ?......

##### Stefania Pedrazzi [Cyberbotics] 10/12/2020 06:25:39
`@Meenakshi Prabhakar` you could check the code of the `e-puck_line_demo.wbt` simulation: [https://www.cyberbotics.com/doc/guide/epuck#e-puck\_line\_demo-wbt](https://www.cyberbotics.com/doc/guide/epuck#e-puck_line_demo-wbt)

the e-puck has only two wheels, but you should be able to easily apply it to a 4-wheeled robot

##### Soft\_illusion 10/26/2020 00:15:02
`@Meenakshi Prabhakar` and `@Stefania Pedrazzi` have a look at [https://youtu.be/l0JuUM58nOs](https://youtu.be/l0JuUM58nOs) there is also a tutorial to make a custom 4 wheel robot in this series.

##### mø 11/04/2020 18:56:14
Where can I find component ratings like power usage etc. for the motors/sensors I used? Most of them are standard commercialized ones, so  how can i find them?

##### R\_ 11/07/2020 19:50:42
Is there an opensource library to test RL on Webots example robots, perhaps similar to this: [https://gist.github.com/mikko/424018819ba3cb10f5780cb7c74cbfb7](https://gist.github.com/mikko/424018819ba3cb10f5780cb7c74cbfb7)

##### Stefania Pedrazzi [Cyberbotics] 11/09/2020 07:30:58
`@mø` Which information do you need exactly?

You can find all the information about a Webots sensor/motor in the PROTO file that defines it or in the documentation page:

- [https://www.cyberbotics.com/doc/guide/actuators](https://www.cyberbotics.com/doc/guide/actuators)

- [https://www.cyberbotics.com/doc/guide/sensors](https://www.cyberbotics.com/doc/guide/sensors)

But usually Webots models doesn't contain power usage information,  so you should look at the web site of the motor/sensor producer to retrieve these values.

##### vinwan 11/19/2020 07:28:39
How can i use python multiprocessing module in webots?

##### Darko Lukić [Cyberbotics] 11/19/2020 07:58:25
Hello `@vinwan` , Webots should support the Python multiprocessing module out-of-the-box. Just make sure everything is synced with `step()`

##### vinwan 11/19/2020 09:18:48
Hello `@Darko Lukić`  Thank you for replying . I just started using webots and i don't know how to sync everything with step() . Is there any documentation i can read about it ?

##### Darko Lukić [Cyberbotics] 11/19/2020 10:53:34
`@vinwan` Welcome to Webots!



Yes, here is a reference for the `step()`  function:

[https://cyberbotics.com/doc/reference/robot#wb\_robot\_step](https://cyberbotics.com/doc/reference/robot#wb_robot_step)



And here is useful guide on how to write a cotroller:

[https://cyberbotics.com/doc/guide/controller-programming?tab-language=python](https://cyberbotics.com/doc/guide/controller-programming?tab-language=python)



Make sure you have gone through the tutorials first:

[https://cyberbotics.com/doc/guide/tutorials?tab-language=python](https://cyberbotics.com/doc/guide/tutorials?tab-language=python)

##### F\_Nadi 11/21/2020 11:50:37
Hello guys, I want to rotate my robot exactly 30 degrees to the right in Webots R2019b version. But sometimes my robot is rotated less than 30 degrees. Would you please help me to tackle this problem?

##### KajalGada 11/22/2020 02:57:28
I was about to ask for webot time step documentation. Curious question: is there a reason I see multiples of 32 for time step usage in examples?


`@F_Nadi` I have been working on rotation robots in webots. While I got it to rotate to a certain degree. It was not always exact. I saw error accumulating over time. My best guess is slight mismatch between timings in when sensor was measured (to get position of robot) and when motor commands were executed. Which in some ways represents real world - you can't have exact turns.



For your references, how I turn robot based on speed and timing: [https://youtu.be/CDOrTKQAOqs](https://youtu.be/CDOrTKQAOqs)

##### Olivier Michel [Cyberbotics] 11/23/2020 07:58:05
`@KajalGada`: there is actually a reason for using a multiple of 32 millisecond for the controller step: it is easy to divide by two several times. And that can be useful. Let's imagine you choose a `WorldInfo.basicTimeStep` of 32 milliseconds and a robot controller step of 32 milliseconds, so that both are in sync, which is optimal performance-wise. But, it turns out that the physics of your simulation is unstable. Then, dividing the `WorldInfo.basicTimeStep`, so that you get 16 should help improving the stability and will not affect the controller program as both will be in sync every two basic time step. If dividing by two is not enough, you can divide by two again and get 8, and continue with 4, 2 and 1. In every case, your controller will be in sync with the simulation physics step, which contributes to make the simulation efficient and stable.

##### F\_Nadi 11/24/2020 07:31:05
Thanks `@KajalGada`

##### KajalGada 11/25/2020 01:16:49
That is smart, thank you for the explaination Olivier 🙂

##### j-ub 12/03/2020 13:30:30
Hi there! I'm trying to control a quadricopter iin webots by using ROS and extern controller from VisualStudio code. I previously made the controller in C and runned it from the webots interface but I'm bit lost about how to face the new requirements (extern controller+ROS). I got how the . launch file is running the .wbt file but i don't know the way the .cpp node is joining with the quadricopter (mavic2pro) in the simulation and providing it of a controller. So.. I have few question i would be glad if someone could guide me a bit through.. 1st.- Is this the only way to work with webots+extern\_controller+ROS?.. It is necessary to use the .cpp file in src folder?, 2nd.- Could the .cpp file be replaced for a .py file so i won't have to C++ programming? and 3rd.- If the answer of 2nd question is a "no".. there is some documentation explaining the complete\_test.cpp file or at least the robot\_information\_parser file? The code looks bit wide and complex for me.. Thank in advance:)

##### Darko Lukić [Cyberbotics] 12/03/2020 13:48:12
Hello `@j-ub` 

>  Is this the only way to work with webots+extern\_controller+ROS

If you use ROS1, you can choose the `ros` controller instead of `<extern>` and it will create a ROS interface. In that way you avoid using the external controller.



> It is necessary to use the .cpp file in src folder?

No, you can use e.g. Python as well. Check this file out:

[https://github.com/cyberbotics/webots\_ros/blob/master/launch/webots\_ros\_python.launch](https://github.com/cyberbotics/webots_ros/blob/master/launch/webots_ros_python.launch)



Let me know if this covers 3rd question as well 🙂

##### j-ub 12/03/2020 14:10:37
well.. first of all thank you for your quick answer:) . Maybe I didin't explain myself good enought in the 3rd question xd, It could be changed for the following "Is it some documentation wider than the one that become inside the complete\_test and the robot\_information\_parser file?" But for the moment, I think I'm going to investigate deeper about how to use the ros controller instead the <extern> one and .py files so your answer is good enought for me. Thanks a lot!

##### Darko Lukić [Cyberbotics] 12/03/2020 14:38:40
Maybe these tutorials:

[http://wiki.ros.org/webots](http://wiki.ros.org/webots)

[http://wiki.ros.org/webots\_ros/Tutorials/Sample%20Simulations](http://wiki.ros.org/webots_ros/Tutorials/Sample%20Simulations)

[https://cyberbotics.com/doc/guide/tutorial-8-using-ros](https://cyberbotics.com/doc/guide/tutorial-8-using-ros)



And each Webots node has a `ROS` tab that serves as a ROS API reference, e.g.:

[https://cyberbotics.com/doc/reference/distancesensor?tab-language=ros#distancesensor-functions](https://cyberbotics.com/doc/reference/distancesensor?tab-language=ros#distancesensor-functions)



In case you choose to switch to ROS2 the documentation is here:

[https://github.com/cyberbotics/webots\_ros2/wiki](https://github.com/cyberbotics/webots_ros2/wiki)

##### j-ub 12/03/2020 15:33:48
I have already made the tutorials anyway I guess I should dedicate a bit more time understanding how those examples work.. The biggest problem I have I think is I don't really understand how all is "inter-connected". For example, could you tell me how the nodes ros\_controller.py and ros\_python.py work together and why in this case the controller is setted up as a extern controller? And.. sorry.., last question:) could I set in this example (webots\_ros\_python) the controller field as ros controller and make it work somehow? P.S.: I think I am fine with ROS1.


probably the questions need a wide explanation.. maybe due to my lack of knowledge in this subject.. anyway whatever info or ideas for a better understanding will be gladly taken. thanks again

##### Darko Lukić [Cyberbotics] 12/03/2020 15:41:06
> For example, could you tell me how the nodes ros\_controller.py and ros\_python.py work together

`ros_controller.py` is an example of a ROS node, not relavant to Webots. It just makes the robot do something.

 `ros_python.py` is a ROS driver for Webots. It "converts" a Webots API (a part of the API relavant to your robot) to a ROS interface.


> could I set in this example (webots\_ros\_python) the controller field as ros controller and make it work somehow?

Yes. Just make sure the ROS client libraries can be included (`rospy`, `std_msgs` and similar).


Under-the-hood it looks something like this:

```md
             `ros_controller.py`
                      |
               (TCPROS/UDPROS) 
                      |      
               `ros_python.py`
                      |
(Webots protocol based on a shared memory and pipes)
                      |
               Webots simulator
```

##### j-ub 12/03/2020 16:00:51
Ok, that was very revealing. thanks `@Darko Lukić` for clarifying those many doubts, now I have some work waiting for me 😉

##### Darko Lukić [Cyberbotics] 12/03/2020 16:03:23
Great, let us know if we can help you with something else 🙂

##### j-ub 12/03/2020 16:05:58
Sure, I will. Have a nice evening!

##### Darko Lukić [Cyberbotics] 12/03/2020 17:18:36
Thanks, have a nice evening!

##### j-ub 12/04/2020 14:18:04
Hello! I’m here again luckily faster than I guessed. I figured how to make it work as a .py node launched with ROS+ extern controller. I made a simple controller for the mavic2pro robot which is only setting the propellers velocity and getting the imu values which i want to print in the console (webots console or ubuntu terminal..)


The point is when I’m trying to print anything (before the main loop or inside the main loop) I can’t see it nowhere and I guess it is been printed somewhere.. Could I have some help about this matter?



Thanks in advance

##### Darko Lukić [Cyberbotics] 12/04/2020 15:31:29
The external Webots controller doesn't print to Webots console, but to the terminal from which it is launched. Then you should check whether the standard output is redirected to `screen`:

```
output="screen"
```

[http://wiki.ros.org/roslaunch/XML/node](http://wiki.ros.org/roslaunch/XML/node)

##### j-ub 12/04/2020 15:48:49
Oh! That is just way easier than the thing i was trying to do. Thanks again `@Darko Lukić`


I was trying to make the .launch file openning another .py script that could print all in a new terminal.. So.. thanks:)

##### Diego Rojas 12/07/2020 19:26:28
Is it possible to change a robot end effector dynamically during a running simulation? I currently have three end effectors for my robot and I need those three tools to complete a robot repair in simulation. I want to change the robot tool while the simulation is running to demonstrate a complete robotic repair. Currently, I have to restart the simulation and load a new end effector with controller every time I want to switch EE.

##### Olivier Michel [Cyberbotics] 12/08/2020 06:36:15
Yes, this is possible using a supervisor to remove the node corresponding to the tool from the tool slot and add a new node corresponding to the new tool in the tool slot. You will however need to use the latest nightly builds of Webots R2021a.

##### Diego Rojas 12/08/2020 06:43:07
`@Olivier Michel` Sounds great! This may be a silly question, but is Webots R2021a passing on Ros2 foxy? I have ros2, moveit2, working with the universal packages in webots R2019b. Are the ros2\_universal\_robot packages the same in R2021a?

##### Olivier Michel [Cyberbotics] 12/08/2020 06:46:09
Yes, exactly the same.

## 2021

##### pnaraltnsk 01/02/2021 21:48:12
Hi, I am using webots for my graduation project and I am working on the Nao robot. I am trying to add pen node to my robot to mark the robot's walking path but for some reason, the pen node doesn't write. I added pen node to my robot's leftFootSlot and exactly like in pen.wbt example. Could you please help me out with this situation? Or do you know any other ways to mark the robot's walking path?

##### Stefania Pedrazzi [Cyberbotics] 01/04/2021 07:12:22
`@pnaraltnsk` your question has already been answered in the `technical-questions` channel.

##### AdityaDutt 01/12/2021 15:37:04
are there any debugging tools available in webots? Like step through code, etc.

##### Olivier Michel [Cyberbotics] 01/12/2021 16:41:00
Yes, you can use gdb (although it's not fully integrated into Webots).

##### prophile 01/21/2021 22:58:17
Hello, your documentation currently looks like this:
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155720933146637/801948818329305088/unknown.png)
%end


we've reproduced in a number of different browsers and OSs

##### TheOrangeOne 01/21/2021 22:59:05
> Package size exceeded the configured limit of 50 MB. Try [https://github.com/cyberbotics/webots/tree/released/docs/css/webots-doc.css](https://github.com/cyberbotics/webots/tree/released/docs/css/webots-doc.css) instead.

##### prophile 01/21/2021 23:00:09
(Because [https://cdn.jsdelivr.net/gh/cyberbotics/webots@released/docs/css/webots-doc.css](https://cdn.jsdelivr.net/gh/cyberbotics/webots@released/docs/css/webots-doc.css) is returning a 403)


We are not entirely sure that this is intended behaviour


so thought we'd let you know

##### Wasabi Fan 01/22/2021 02:48:00
I'm seeing the same thing as above. The documentation is currently broken.


It seems (?) this has fixed itself since. 🎉


No, never mind, the reference is still definitely borked 🙁

##### Olivier Michel [Cyberbotics] 01/22/2021 08:29:30
I cannot reproduce this problem. For me [https://cdn.jsdelivr.net/gh/cyberbotics/webots@released/docs/css/webots-doc.css](https://cdn.jsdelivr.net/gh/cyberbotics/webots@released/docs/css/webots-doc.css) works. Can you try again?

##### TheOrangeOne 01/22/2021 13:04:34
Seems to be working on now, perhaps an issue with the CDN

##### Olivier Michel [Cyberbotics] 01/22/2021 13:41:31
Probably. Anyhow thank you for reporting it and for your feedback.

##### ahforoughi 01/26/2021 20:59:48
Hi guys is there any sample robotic arm in webots that use to train with a actor-critic algorithm!?

##### Darko Lukić [Cyberbotics] 01/27/2021 07:48:46
`@ahforoughi` 

Here is a tutorial about Actor-Critic algorithm and Open AI Gym:

[https://www.tensorflow.org/tutorials/reinforcement\_learning/actor\_critic](https://www.tensorflow.org/tutorials/reinforcement_learning/actor_critic)



and here is a middleware for Open AI Gym and Webots:

[https://github.com/aidudezzz/deepbots](https://github.com/aidudezzz/deepbots)

(paper: [https://link.springer.com/chapter/10.1007/978-3-030-49186-4\_6](https://link.springer.com/chapter/10.1007/978-3-030-49186-4_6))



Hope this helps. We will be happy to see your project that demonstrates Actor-Critic algorithm in Webots.

##### Luiz Felipe 02/02/2021 10:28:14
`@ahforoughi` I am using DDPG for continuous control with Webots... With the new fast mode it seems to work fine... My tip would be for you to use the input of a position controller or a torque controller as the action of your actor-critic algorithm

##### HANEEN AL ALI 02/04/2021 12:35:12
hello, i want to build end effector for the robot arm like the picture below. i added a hinge joint and i want to build the two finger. should i choose a group to build link1( shape box) or should i choose soild first??. Can u please help me by explaining how can i build these part?


This is the design i want to implement
%figure
![end.jpg](https://cdn.discordapp.com/attachments/565155720933146637/806865642158161920/end.jpg)
%end

##### Stefania Pedrazzi [Cyberbotics] 02/08/2021 07:11:32
Hi, link1 should be a Solid node.

Here is an example of gripper: [https://www.cyberbotics.com/doc/guide/khepera3#khepera3\_gripper-wbt](https://www.cyberbotics.com/doc/guide/khepera3#khepera3_gripper-wbt)

The gripper  model is saved in a PROTO file. But if you select it, tight-click on it to open the context menu and choose the "Convert to Base Node(s)" options then you will be able to check the internal structure directly from the Webots scene tree.

##### HANEEN AL ALI 02/10/2021 15:20:16
`@Stefania Pedrazzi` thank you so much for ur reply.  Can u explain why link 1 should be a solid node, pleased? I choose it as transform node

##### Stefania Pedrazzi [Cyberbotics] 02/10/2021 15:24:37
You can define link 1 in a `Transform` node, but between the `HingeJoint` node and the link 1 node you need a `Solid` node because the `HingeJoint.endPoint` field expects a `Solid` or derived node.

##### HANEEN AL ALI 02/10/2021 17:08:21
I went to khepera3 but I couldn't find the model for khepera3\_gripper. I just found Khepera3 robot with a two- wheeled.



Please please can u tell where I can find the model with the gripper?

##### Chernayaten 02/10/2021 18:20:25
`@HANEEN AL ALI`  You're looking in ..\Webots\projects\robots\k-team\khepera3\worlds and you can't find the gripper wbt file? I found it in my installation

You can probably copy missing files from here [https://github.com/cyberbotics/webots/tree/master/projects/robots/k-team/khepera3](https://github.com/cyberbotics/webots/tree/master/projects/robots/k-team/khepera3)

##### HANEEN AL ALI 02/10/2021 20:10:19
`@Chernayaten` ya, it is this one i am looking for. so i can have an idea how to build my design. however i do know how to add it to the robot node, do you have an idea on how to add it?

##### Chernayaten 02/10/2021 20:21:53
I do not know how to use grippers. If I wanted to learn them I would add a khepera with a gripper and then do what stefania suggested, convert to base nodes so that I can study how its done and mimic it

##### Stefania Pedrazzi [Cyberbotics] 02/11/2021 07:25:25
In the Webots samples library there is a world called `khepera3_gripper.wbt`. You can find it by openining the File > Open Sample World.. dialog and type the world name.

The gripper is specified in a separated PROTO called `Khepera3_Gripper`. You can added it to any Robot node:

1. Select a `children` or `*Slot` (in case of PROTO robot node) fields

2. Click the "Add node" button

3. Type in the "Find" text field "Khepera3\_Gripper" and select it

##### BeastFromTheEast 02/24/2021 14:52:33
Is documentation page down for anyone else atm?

##### DDaniel [Cyberbotics] 02/24/2021 14:52:50
yes, looking into it at the moment

##### BeastFromTheEast 02/24/2021 14:53:03
Thank you

##### DDaniel [Cyberbotics] 02/24/2021 14:58:32
`@BeastFromTheEast` should be back now

##### Elizaveta\_Potemkina 03/02/2021 17:18:46
Hi guys, I'm looking for the proto file for the Sharp GP2Y0A02YK0F sensor - where in the docs would I be able to find it?

##### Chernayaten 03/02/2021 17:33:07
Webots\projects\devices\sharp\protos is where mine is located


`@Elizaveta_Potemkina`

##### Elizaveta\_Potemkina 03/02/2021 17:33:23
Thanks, just found it actually!

##### Shyam 03/20/2021 19:58:06
is there something wrong with the documentation page on website?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155720933146637/822921967689662489/unknown.png)
%end

##### baby\_droid\_yoda 03/20/2021 20:03:57
Yes , same with me this documentation website

##### Olivier Michel [Cyberbotics] 03/22/2021 07:26:31
I believe this is now fixed.

##### Shyam 03/22/2021 10:37:54
Yes, thank you for the update `@Olivier Michel`

##### Westin 03/24/2021 15:49:09
Is anyone else having issues with the docs page?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155720933146637/824308871422017536/unknown.png)
%end

##### Olivier Michel [Cyberbotics] 03/24/2021 15:50:50
Can you check again now?

##### Westin 03/24/2021 15:51:01
Yes its working, thanks.


There is also an issue in the webots download link. Pressing the drop down button gets you there, but the main button gets a 404.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155720933146637/824309651982123028/unknown.png)
%end

##### Olivier Michel [Cyberbotics] 03/24/2021 15:53:14
Strange, I don't get any 404 with the main button...


From which browser are you trying this?


The "NaN undefined NaN - undefined" seems to indicate that our Javascript is unable to determine your platform (Window, macOS or Linux).


Which is your operating system?

##### Westin 03/24/2021 15:56:25
I'm not having the issue now. I tried on Edge and Chrome on Windows 10 and got the same result each time.

##### Olivier Michel [Cyberbotics] 03/24/2021 15:57:13
Do you mean the issue disappeared suddenly?

##### Westin 03/24/2021 15:59:40
Yes I loaded the page again after a few minutes and it displays properly.

##### Olivier Michel [Cyberbotics] 03/24/2021 15:59:56
Strange...

##### Westin 03/24/2021 16:00:39
Yep.

##### reinaldobianchi 04/06/2021 13:04:53
Hi. I have a doubt about the Sensors on the Pioneer AT3. Is the figure on page [https://cyberbotics.com/doc/guide/pioneer-3at](https://cyberbotics.com/doc/guide/pioneer-3at) wrong?

It looks like the sensors that are in front should be in the back...

Sensors 0 to 7 should be in the back, and sensors 8 to 15 in the front, no?

I also think that the Front and Back views of the robot are switched...


This is wrong for the Pioneer 3 DX and AT

##### Srivastav\_Udit 04/08/2021 06:46:44
Is there any particular reason why the sensors orientation on the epuck is the way it is?

##### Olivier Michel [Cyberbotics] 04/08/2021 06:47:18
It corresponds to the orientation on the real robot.

##### Stefania Pedrazzi [Cyberbotics] 04/08/2021 06:57:14
Hi, the figure on the documentation is correct: sonar sensors 0 to 7 are in the front. This robot was calibrated with the real one years ago.

But I checked again and all the datasheets that I could find about the Adept Pioneer 3 are still mentioning that sensors 0 to 7 are in the front and sensors 8 to 15 in the back.

Do you have a different robot datasheet mentioning a different sonar sensors position?

##### reinaldobianchi 04/08/2021 11:01:03
Hi. The datasheets indeed shows that sonars 0 to 7 are in front of the real robot. But using it in Webots, we get the data from the back ring. I think that in the model of the Pioneer 3 AT the sonar ring was rotated 180 degrees... just run the code I sent and you will see that...

##### Stefania Pedrazzi [Cyberbotics] 04/08/2021 11:14:13
I just checked the sensors position in the simulation and their response to object detection and sensors 0-7 correctly detect objects in the front of the robot. You can see it in this video.
> **Attachment**: [pioneer3at\_sensors.mp4](https://cdn.discordapp.com/attachments/565155720933146637/829675497473441802/pioneer3at_sensors.mp4)

##### reinaldobianchi 04/10/2021 20:46:00
Thanks! Now I finally understand how it works.

##### ShuffleWire 05/05/2021 23:49:55
In the coding style page [https://github.com/cyberbotics/webots/wiki/CPP-Coding-Style](https://github.com/cyberbotics/webots/wiki/CPP-Coding-Style), the first link in #CS300  [https://github.com/cyberbotics/webots/blob/master/.clang-format](https://github.com/cyberbotics/webots/blob/master/.clang-format) is dead. But I wasn't comfortable to open an issue just for that...

##### Olivier Michel [Cyberbotics] 05/06/2021 05:31:44
Thank you for reporting this. I just fixed it.

##### mmoralesp 05/07/2021 15:49:25
Hello Everyone!,  I am working on a project for my university on ROS and Webots, I am trying to control two epuck in the same Webots environment with ROS but I am not able to make them work, as if I can do it with only one epuck. I would like to know if anyone has tried to do this and if you could help me thanks.

##### Darko Lukić [Cyberbotics] 05/10/2021 09:20:53
It should work with multiple robots without problems. To each robot, the `ros` controller adds an unique prefix to all topics and services. What was the issue?

##### Azhagan 05/12/2021 14:42:49
hello


I am azhagan, and i am new to webots , how can i start with it

##### DDaniel [Cyberbotics] 05/12/2021 15:10:04
`@Azhagan` Hi, you can start by installing it [https://cyberbotics.com/doc/guide/installing-webots](https://cyberbotics.com/doc/guide/installing-webots), then follow the "getting started" guide to learn how to navigate the interface: [https://cyberbotics.com/doc/guide/getting-started-with-webots](https://cyberbotics.com/doc/guide/getting-started-with-webots), and finally do the tutorials: [https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials).

##### Azhagan 05/12/2021 15:52:12
thanks !!

##### Chrimo 05/12/2021 22:04:43
Hi all, is there any example for using GPS with ros2/webots available ? TIA

##### raerae 05/13/2021 09:47:08
Hi guys, I'm new here. I just want to ask, is there a possible method to export Solidworks model to Webots that will include its original appearance?

##### Darko Lukić [Cyberbotics] 05/14/2021 06:34:50
No, but if you include a GPS device in your PROTO you should be getting `NavSatFix` messages. I guess you tried the `webots_ros2` package:

[https://github.com/cyberbotics/webots\_ros2](https://github.com/cyberbotics/webots_ros2)

##### Affonso 05/14/2021 19:36:57
Hi, I have a doubt about HingeJoints, what parameters I use to set the position of joinParameters?

##### DDaniel [Cyberbotics] 05/14/2021 19:44:23
`@Affonso` Hi, the current position of the HingeJoint is defined by the field `position` inside the `HingeJointParameters` node. If you intend to manually change with a text editor (changing the world file directly) you need to remember to also change the translation/rotation of the corresponding `endPoint` Solid to keep things consistent. If instead you change it from the interface, it automatically updates the solid as well.

##### mmoralesp 05/14/2021 21:34:11
Hi, the problem is when I start the launch and then i start the publisher with the velocity, no robot moves, but when there is only one robot it moves.

##### Darko Lukić [Cyberbotics] 05/17/2021 06:42:39
Can you share more details? Can you list the topics (`rostopic list`)?

##### BlueShrapnel 05/17/2021 06:42:44
Okay

##### mmoralesp 05/17/2021 21:13:59
Hi, im using ros2, with only one robot (epuck) it works find, the robot moves on


This is the scenario
%figure
![Captura_de_pantalla_de_2021-05-17_23-21-31.png](https://cdn.discordapp.com/attachments/565155720933146637/843961774293385216/Captura_de_pantalla_de_2021-05-17_23-21-31.png)
%end


When i launch the vel publisher whit only one robot in the scenario it works well


but with two, it doesn't work

##### Darko Lukić [Cyberbotics] 05/18/2021 07:22:34
You can check this example:

[https://github.com/cyberbotics/webots\_ros2/blob/master/webots\_ros2\_demos/launch/armed\_robots.launch.py](https://github.com/cyberbotics/webots_ros2/blob/master/webots_ros2_demos/launch/armed_robots.launch.py)



Does your launch file looks similar?

##### TheGiant 05/18/2021 10:35:13
`@Olivier Michel` about : [https://github.com/cyberbotics/webots/issues/3075](https://github.com/cyberbotics/webots/issues/3075), I do actually enjoy offline doc 🙂 Maybe add a button to get it offline in one go ? And then open it/browse it in a regular browser. Additionally, I love the "Help..." button when right click on an item, which bring immediately to the right doc page 🙂

However, I understand that it's rather heavy, and removing the viewer could be a nice cleanup.

##### mmoralesp 05/18/2021 17:11:22
Hi, this is my launcher
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155720933146637/844260892518514758/unknown.png)
%end


For set the velocity i have another controller that i launch from another terminal


but only works when i had a unique robot

##### Darko Lukić [Cyberbotics] 05/19/2021 07:35:02
It should be `executable='driver'` for both robots and you should verify whether the robot names (`--webots-robot-name`) match the names in the world file.

##### mmoralesp 05/19/2021 15:27:43
Hi `@Darko Lukić` , thanks a lot for your help, it works for me and now the robots get the velocity and move

##### medrimonia 05/21/2021 13:00:05
Hi everyone, I am under the impression that using the function `wb_supervisor_field_import_mf_node_from_string` only support import of PROTO files listed in the `Webots Projects` and not from the PROTO nodes from the `Current Project`. Am I right and is it the expected behavior? I did not found anything related to that in the documentation and that was not very intuitive to me.

##### Maël Wildi [Cyberbotics] 05/21/2021 13:25:02
Hi `@medrimonia`, it should be possible to import PROTO from your `Current Project` using `wb_supervisor_field_import_mf_node_from_string`. Do you get any error on your side ?

##### medrimonia 05/21/2021 13:29:30
I systematically got the following error while it was located in the `protos` folder of my active project: `ERROR: Skipped unknown 'XXX' node or PROTO.`  (I tried several different robots for this). Moving the proto inside a project in the `WEBOTS_HOME` solved this.


To reduce the risk that my folder structure was inappropriate or something similar I just checked and I was able to see the PROTO file when adding the robot through the GUI (listed in current project)

##### DDaniel [Cyberbotics] 05/21/2021 13:35:44
Just to be sure, does the name of the file itself match the name inside the proto file?

##### medrimonia 05/21/2021 13:36:14
My bad... I noticed the tree structure of my project was still invalid... I had the world listed in a directory named `world` and not in a directory named `worlds` changing the name of the directory did fix the issue...


(sorry for the invalid comment then, I *thought* I did enough preliminary checks before asking it)

##### Maël Wildi [Cyberbotics] 05/21/2021 13:40:04
No problem!

##### KC 05/22/2021 17:17:52
Hi

Can anyone recommend my some good resources on how can I add grippers or something like that to an existing robot in Webots? (Like adding a robotic arm to a Khepera IV robot)

##### DDaniel [Cyberbotics] 05/24/2021 15:51:22
`@KC` Hi, the `khepera3_gripper` robot available in the sample worlds has a gripper, you can take inspiration from that I suppose

##### Affonso 06/01/2021 17:47:51
Hey, I'm working on a robot for a competition and we are using Webots to simulate that robot, as it is very important to control the speed of the robot, we want to know how to convert pwm to speed in the simulator

##### Darko Lukić [Cyberbotics] 06/02/2021 07:21:46
One approach would be to establish a relation between voltage (PWM) and torque of the motor. So, based on the real-world measurements you should get `f(voltage) -> torque`. Then you can simply control a torque of the motor using `wb_motor_set_torque`:

[https://cyberbotics.com/doc/reference/motor?version=develop#wb\_motor\_set\_torque](https://cyberbotics.com/doc/reference/motor?version=develop#wb_motor_set_torque)



Converting PWM to speed is probably not a good idea since a load on the motor may vary (except the PWM controls a set-point).

