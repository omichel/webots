# Development 2019

This is an archive of the `development` channel of the [Webots Discord server](https://discordapp.com/invite/nTWbN9m) for year 2019.

## April

##### Thelm 04/25/2019 10:34:24
Hi, I don't know if this is the good place to post this, tell me if I'm wrong.

I'm actually trying to implement a full support for the e-puck's missing sensors in webots (especially for V2). 

After editing the proto file to add them in the simulation, I am now trying to implement the wifi communication by following this : [http://www.gctronic.com/doc/index.php?title=e-puck2\_PC\_side\_development#WiFi\_2](http://www.gctronic.com/doc/index.php?title=e-puck2_PC_side_development#WiFi_2)



and I'm facing two problems for now : in this piece of code (e-puck\_wifi/Wrapper.cpp) I don't understand the calibration values for the accelerometer so I can't transpose them to the gyro and the magnetometer



TripleValuesSensor *accelerometer = DeviceManager::instance()->accelerometer();

  if (accelerometer->isEnabled()) {

    const double calibration\_k[3] = {-9.81 / 800.0, 9.81 / 800.0, 9.81 / 800.0};

    const double calibration\_offset = -2000.0;

    double values[3];

    for (int i = 0; i < 3; i++) {

      short int acc = sensor\_data[2 * i] + 256 * sensor\_data[2 * i + 1];

      values[i] = calibration\_k[i] * (acc + calibration\_offset);

    }

    wbr\_accelerometer\_set\_values(accelerometer->tag(), values);

  }


also, on the wiki, I've seen that the accelerometer and the gyro are encoded on 6 bytes so 2 bytes for X, Y and Z. But the magnetometer is encoded on 12 bytes, which doesn't make sense to me as the raw values are between -32460 and 32760... So are there 3 axis encoded on 4 bytes, or am I missing something?

##### Olivier Michel [Cyberbotics] 04/25/2019 11:19:50
Hi, welcome.


I would propose you to open an issue on [https://github.com/omichel/webots/issues](https://github.com/omichel/webots/issues) to follow-up with this discussion.


Eventually, you will be able to open a pull request to merge your contribution.

## June

##### Arsalan.b.r 06/04/2019 07:42:35
hello

##### Fabien Rohrer [Moderator] 06/04/2019 07:42:51
Hi again ^^

##### Arsalan.b.r 06/04/2019 07:42:56
i want to model a Tractor trailer mobile robot


its a chained combination of wheeled robots


how can i do it simply?


is there such a robot in webots?

##### Fabien Rohrer [Moderator] 06/04/2019 07:45:40
First of all it's possible to do so. There are even several possibilities to achieve this.


Is it the same robot replicated several time? How is the link between the robots? A HingeJoint?


Do you have a screenshot of a real setup?

##### Arsalan.b.r 06/04/2019 07:47:13
a passive joint between carts

##### Fabien Rohrer [Moderator] 06/04/2019 07:47:48
I think the closest robot we have in Webots yet is the yamor:


[https://cyberbotics.com/doc/guide/yamor](https://cyberbotics.com/doc/guide/yamor)

##### Arsalan.b.r 06/04/2019 07:49:01

%figure
![tractor-trailer.JPG](https://cdn.discordapp.com/attachments/565155651395780609/585374436270800906/tractor-trailer.JPG)
%end

##### Fabien Rohrer [Moderator] 06/04/2019 07:49:15
The robot model is encapsulated in a PROTO node. The Robots are linked with Connector nodes:


[https://cyberbotics.com/doc/reference/connector](https://cyberbotics.com/doc/reference/connector)

##### Arsalan.b.r 06/04/2019 07:50:21
it should be wheeled

##### Fabien Rohrer [Moderator] 06/04/2019 07:50:29
I think you could achieve something by studying yamor.wbt

##### Arsalan.b.r 06/04/2019 07:50:30
like tractor and trailer


yamor is like snake?


do you have a picture of that?

##### Fabien Rohrer [Moderator] 06/04/2019 07:51:07
Yes, but adding wheels to the yamor module is not an issue


This other example may interest you:


[https://www.cyberbotics.com/doc/automobile/car#heavy-weights](https://www.cyberbotics.com/doc/automobile/car#heavy-weights)


The trucks trailer are probably closer from your expectations.

##### Arsalan.b.r 06/04/2019 07:54:14
yes


thanks again

##### Fabien Rohrer [Moderator] 06/04/2019 07:56:42
But first of all, please do our tutorial 😉 [https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials)

##### TH0 06/12/2019 13:54:02
i tried to build webots from source under windows 10, but i got this error:



nodes/utils/WbVirtualRealityHeadset.cpp:43:10: fatal error: openvr.h: No such file or directory

   43 | #include <openvr.h>

      |          ^~~~~~~~~~

compilation terminated.

make[1]: *** [Makefile:669: WbVirtualRealityHeadset.o] Error 1





i followed the installation guide step by step ( [https://github.com/omichel/webots/wiki/Windows-installation](https://github.com/omichel/webots/wiki/Windows-installation) )

any ideas? just cloning the valvesoftware openvr sdk does not seem to work. 



is it a bug in the webots tutorial or did i miss anything?

##### David Mansolino [Cyberbotics] 06/12/2019 13:55:04
this dependency should be downloaded automatically. Can you check the content of your 'dependencies' folder ?

##### TH0 06/12/2019 13:57:29
$ ls dependencies/

Cyberbotics.Webots.Mingw64.Libraries.manifest  libpico.zip  lua-gd-windows.zip  Makefile.mac      openvr-1.0.7

libOIS.zip                                     lua-5.2.3    Makefile.linux      Makefile.windows

##### David Mansolino [Cyberbotics] 06/12/2019 13:58:15
Ok, perfect, you have it the includes are located in the 'openvr-1.0.7' directories.


Just to make sure, you did follow all the instructions step by step from [https://github.com/omichel/webots/wiki/Windows-installation](https://github.com/omichel/webots/wiki/Windows-installation) right ?

##### TH0 06/12/2019 13:58:48
yes


and there where no errors until the one i pasted here

##### David Mansolino [Cyberbotics] 06/12/2019 13:59:16
Ok, let's first try something simple, can you do a 'make cleanse' from the 'webots' folder?


(this will clean the 'dependencies' folder so that you ca retry from a clean install)

##### TH0 06/12/2019 14:00:15
done


should i send the output in the chat?

##### David Mansolino [Cyberbotics] 06/12/2019 14:00:36
Ok, can you retry to do a 'make'


if they was no error its not needed to send the output

##### TH0 06/12/2019 14:03:46
ok, same error after make -j6


is there some trick in discord to send code in a more compact way in the chat? 😄


like a block or something like that

##### David Mansolino [Cyberbotics] 06/12/2019 14:04:52
Not really, but you can send gist link: [https://gist.github.com/](https://gist.github.com/)

##### TH0 06/12/2019 14:05:53
ok, how ever, is python and npm needed or only optional?

##### David Mansolino [Cyberbotics] 06/12/2019 14:06:32
python is higly recommended (even so it is not required to compile the core of Webots).


Npn is completely optionnal


if you want to have npn install (and some other optionnal dependencies) you can install them by running './src/install\_scripts/msys64\_installer.sh --dev' from the webots directory

##### TH0 06/12/2019 14:07:33
ok, but i think, its not the solution for the openvr problem, right?

##### David Mansolino [Cyberbotics] 06/12/2019 14:08:13
It should not impact openvr, but it is worse trying


what is the output of your make -j6 command ?

##### TH0 06/12/2019 14:09:46
first of all:

Thomas@DESKTOP-O5VO47I MSYS /C/msys64/home/Thomas/webots

$ find . -name openvr.h

./dependencies/openvr-1.0.7/headers/openvr.h

./include/openvr/headers/openvr.h



Thomas@DESKTOP-O5VO47I MSYS /C/msys64/home/Thomas/webots

$


[https://0bin.net/paste/DeUwrdi11b19n91y#XDrDCDC3pOVgGQ3a1vqIJNM6k7hJ0j1e1qgjBpNhMQh](https://0bin.net/paste/DeUwrdi11b19n91y#XDrDCDC3pOVgGQ3a1vqIJNM6k7hJ0j1e1qgjBpNhMQh)

##### David Mansolino [Cyberbotics] 06/12/2019 14:15:06
The missing headers should be located in '/C/msys64/home/Thomas/webots/openvr-1.0.7/headers' after the make -j6


From the log you sent it seems they are there, can you confirm ?

##### TH0 06/12/2019 14:16:38
yes, its there


sorry for the confusion with the find command, i updated the message from above

##### David Mansolino [Cyberbotics] 06/12/2019 14:18:54
Ok, no problem, then it seems they are correctly downloaded and then copied in the 'include' fodler

##### TH0 06/12/2019 14:19:04
right


seems like an error in the makefile (missing include directory?)

##### David Mansolino [Cyberbotics] 06/12/2019 14:20:22
Yes, indeed, can you try changing in 'src/webots/Makefile:

OPEN\_VR\_INCLUDE = -isystem $(WEBOTS\_PATH)/include/openvr

Into

OPEN\_VR\_INCLUDE = -isystem $(WEBOTS\_PATH)/include/openvr/headers

and try to recompile ?

##### TH0 06/12/2019 14:20:46
😄


👌


look good so far

##### David Mansolino [Cyberbotics] 06/12/2019 14:26:49
Perfect, we will fix this in our repo !

##### TH0 06/12/2019 14:30:43
btw.: great that webots is open source now!


make is completed and was successful!

##### David Mansolino [Cyberbotics] 06/12/2019 14:31:54
Perfect !

##### TH0 06/12/2019 14:32:30
time for another question? 😉


i think the vr-function in webots is broken. i tried to view a simulation with the htc vive and steam vr but only a blue background appers in the view. when i deactivate position and rotation tracking in the webot gui, a coordinate system (webots style) is displayed in the vive in the center of the display, but no other objects from the scene)

##### David Mansolino [Cyberbotics] 06/12/2019 14:36:33
That's strange we did test it a few weeks ago and it was working fine. Unfortunately we don't have any VIve available right now to test.


Where you able to view the simulation in Webots moving when you were moving the headset ?

##### TH0 06/12/2019 14:37:41
i'm using the webots 2019a Rev 1 from some weeks ago. is there a newer version?

##### David Mansolino [Cyberbotics] 06/12/2019 14:39:55
Not yet, but we will release R2019b soon. Unfortunately I don't have the time right now, but if you test with R2019b when we will release it and the problem is still present, please do not hesitate to tell us.

##### TH0 06/12/2019 14:40:28
ok


when i move the headset, nothing happens


only a light blue/grey background color

##### David Mansolino [Cyberbotics] 06/12/2019 14:41:33
That's indeed not normal, but we changed some part of the VRHeadset code for the next release, so please try again.

##### TH0 06/12/2019 14:41:36
whats the world you tested the VR?

##### David Mansolino [Cyberbotics] 06/12/2019 14:42:11
Many of them, but I remember I tested I lot the 'robotis/soccer.wbt' world

##### TH0 06/12/2019 14:42:28
ok, thats the one i used too


i'm not sure, but isn't the version i just build from webots the 2019b you mean?

##### David Mansolino [Cyberbotics] 06/12/2019 14:48:16
It depends on which git branch you are ?

##### TH0 06/12/2019 14:50:14
on the develop


i just tested the vive in the 2019b, same problem: colored background but no objects


world file was "nao\_robocup.wbt"

##### David Mansolino [Cyberbotics] 06/12/2019 14:53:18
Then if you are using the 'develop' branch you are indeed very close to the R2019b version 😕

##### TH0 06/12/2019 14:54:44
ok. the color shown in the vive is the same as the "/Background/SkyColor" (i changed it to red to test it)


maybe a near-clipping problem because of different scales?

##### David Mansolino [Cyberbotics] 06/12/2019 15:01:22
At least if shows that the connexion with the Vive is correctly established

##### TH0 06/12/2019 15:01:42
exact

##### David Mansolino [Cyberbotics] 06/12/2019 15:02:05
Yes, maybe near-clip, or maybe a resolution issue. can you try setting the VIve resolution to 50% (you can set this in the preference of SteamVR). and then reboot Webots.

##### TH0 06/12/2019 15:04:17
set to 50%, same issue


interessting: when i click on some objects in the treeview, the coordinate systems are shown in the vive



%figure
![webotsVR01.png](https://cdn.discordapp.com/attachments/565155651395780609/588384559884926987/webotsVR01.png)
%end

##### David Mansolino [Cyberbotics] 06/12/2019 15:10:35
Can you see the handle in the headset too?

##### TH0 06/12/2019 15:11:17
no handles


and the position is not tracked, but the rotation from the vive


(but i set both options to true in the webots gui)


oh, and i can rotate the cameraview per mouse while vr is on

##### David Mansolino [Cyberbotics] 06/12/2019 15:23:28
that's very strange, you should see exactly the same in the Webots view and in one of the two eyes in the headset. Probably there is something not clean with OpenGL on our side.

##### TH0 06/12/2019 15:27:23
the vive preview in webots and what i see in the vive is the same

##### David Mansolino [Cyberbotics] 06/12/2019 15:27:56
Ok, so you can see the handle in the preview but not in the headset ?

##### TH0 06/12/2019 15:28:37
just to be sure: you mean with "handles" the two controllers from the vive?


i do not see the rendered vive controllers in both the webots preview and in the vive

##### David Mansolino [Cyberbotics] 06/12/2019 15:29:38
No sorry, I mean the red,green and blue arrows you can see in Webots.

##### TH0 06/12/2019 15:30:03
yes, i also see them in the vive (its exacly the same image)

##### David Mansolino [Cyberbotics] 06/12/2019 15:30:04
> i do not see the rendered vive controllers in both the webots preview and in the vive



that's normal we did not yet interfaced the vive controller with webots.

##### TH0 06/12/2019 15:30:26
sorry, english is not my mother language, i thought you mean the vive handles (controllers)

##### David Mansolino [Cyberbotics] 06/12/2019 15:30:31
Ok, than this make more sense, so the problem is really what is rendered in Webots

##### TH0 06/12/2019 15:30:40
yes


i think so

##### David Mansolino [Cyberbotics] 06/12/2019 15:30:44
> sorry, english is not my mother language, i thought you mean the vive handles (controllers)



No problem that was not clear from my side too.

##### TH0 06/12/2019 15:31:10
the good news: you can simulate the behaviour without a vive, right?

##### David Mansolino [Cyberbotics] 06/12/2019 15:31:11
Can you try playing with the Viewpoint, near and far value ?


> the good news: you can simulate the behaviour without a vive, right?



Kind of, but still the vive API in itself interfere with OpenGL and this is maybe the cause of the problem.

But as soon as I have time I will try to reproduve this.

##### TH0 06/12/2019 15:35:50

%figure
![webotsVR02.png](https://cdn.discordapp.com/attachments/565155651395780609/588391019444174858/webotsVR02.png)
%end



%figure
![webotsVR03.png](https://cdn.discordapp.com/attachments/565155651395780609/588391025089708196/webotsVR03.png)
%end


see difference far 0 (default) vs. far 1


that means for me, clipping is not the problem, right?

##### David Mansolino [Cyberbotics] 06/12/2019 15:36:30
Can you try increasing it to something like 100 ?

##### TH0 06/12/2019 15:36:44
same result with 100

##### David Mansolino [Cyberbotics] 06/12/2019 15:36:45
(value are in meters)


Ok, then yes it doesn't seems it is clipping the problem, but rather an issue with OpenGL


By the way what is your GPU ?

##### TH0 06/12/2019 15:37:26
0 means infinity?


P3200 mobile

##### David Mansolino [Cyberbotics] 06/12/2019 15:40:46
> 0 means infinity?



for far yes


We haven't tested it with this GPU, but it should works fine.

Do you know which OpenGL version you are using?

Are other application working fine with the Vive headset?

##### TH0 06/12/2019 15:44:12
yes other apps work fine and i use the vive often


OpenGL 4.6 (415 ext) - Quadro P3200/PCIe/SSE2

##### David Mansolino [Cyberbotics] 06/12/2019 15:50:20
Ok, then everything is perfect with your hardware. I will check when I have time and let you know

##### TH0 06/12/2019 15:51:23
ok, good luck!

##### David Mansolino [Cyberbotics] 06/12/2019 15:52:00
Thank you

##### BlackPearl 06/19/2019 08:50:43
I think we found a Bug?
%figure
![image0.jpg](https://cdn.discordapp.com/attachments/565155651395780609/590825780716634132/image0.jpg)
%end



%figure
![image0.jpg](https://cdn.discordapp.com/attachments/565155651395780609/590826051312025620/image0.jpg)
%end

##### Fabien Rohrer [Moderator] 06/19/2019 08:52:39
Hi


It seems your controller called SensorController crashes.


A priori, this bug is on your side.


Do you have any way to debug your controller?

##### BlackPearl 06/19/2019 18:15:12
Actually we tried today different controllers but always getting this error

##### David Mansolino [Cyberbotics] 06/20/2019 06:54:45
`@BlackPearl` do you get this error with the controllers distributed within Webots?

##### BlackPearl 06/20/2019 13:54:10
Yes


That’s why we created our own but still get this error

##### David Mansolino [Cyberbotics] 06/20/2019 14:05:24
`@BlackPearl` which version of Webots and which version of Java are you using ?

##### BlackPearl 06/21/2019 04:11:49
The latest (java 8 and webots R2019-R1)

##### David Mansolino [Cyberbotics] 06/26/2019 06:15:07
`@BlackPearl` can you please try with the latest version of Webots that we releeased yesterday: [https://github.com/omichel/webots/releases/latest](https://github.com/omichel/webots/releases/latest)

##### BlackPearl 06/28/2019 05:00:03
`@David Mansolino`  thanks, will try

## July

##### Deleted User 07/20/2019 10:21:57
`@BlackPearl` hello sir, are you using nao ?

## August

##### BlackPearl 08/21/2019 09:42:49
`@David Mansolino`  so we are using the new version but we got the controller crash again


Anything we need to pay attention?


It’s a java controller

##### Fabien Rohrer [Moderator] 08/21/2019 09:48:29
How did you installed Java? Your log seems to refer to OpenJDK while we recommend to use the Oracle JDK: [https://cyberbotics.com/doc/guide/using-java#windows](https://cyberbotics.com/doc/guide/using-java#windows)


(the java libcontroller is precompiled with the last 8 version of the Oracle JDK)

##### BlackPearl 08/21/2019 09:50:26
`@Fabien Rohrer`  oh ok thanks for the quick answer


We Will change to the oracle jdk

##### Fabien Rohrer [Moderator] 08/21/2019 09:51:44
oops, I think I'm wrong: I was sure you were on windows.

##### BlackPearl 08/21/2019 09:52:04
No we are running Linux


But yes we are using the openJDK

##### Fabien Rohrer [Moderator] 08/21/2019 09:52:20
on linux, OpenJDK is supposed to work,

##### BlackPearl 08/21/2019 09:53:19
Hm so why the error accurs

##### Fabien Rohrer [Moderator] 08/21/2019 09:56:07
not sure. could you give us more precisely your Java version (`java -version`)? It is supposed to be `1.8`. Does it matches with your `javac -version`?

##### BlackPearl 08/21/2019 09:57:46

%figure
![image0.png](https://cdn.discordapp.com/attachments/565155651395780609/613673091335323648/image0.png)
%end

##### Fabien Rohrer [Moderator] 08/21/2019 10:01:29
ok, this is certainly the issue


We are precompiling the Java library with OpenJDK 1.8.0\_222.


What OS are you using?  arch?


You should either downgrade your Java, or either recompile the library with your Java version (normally it's quite simple, just a matter to install `swig`, define `WEBOTS_HOME` and type `make` in `WEBOTS_HOME/resources/languages/java`)

##### BlackPearl 08/21/2019 10:07:58
Unbuntu


*ubuntu


Hm ok

##### Fabien Rohrer [Moderator] 08/21/2019 10:09:36
Let me know if you have troubles with this.

##### BlackPearl 08/21/2019 10:10:07
Ok thank you very much. We are downgrading

##### Fabien Rohrer [Moderator] 08/21/2019 10:10:23
(it's probably the simplest solution indeed)

##### MerySgh 08/21/2019 10:13:23
hello everyone !  has anyone have developed jamming attack in webot  before ?

##### Fabien Rohrer [Moderator] 08/21/2019 10:15:38
Hi, Webots is mainly a Desktop app. A jamming attack has not much sense to me. Do you speak about the streaming sever? Our websites?


Do you have something else in mind?

##### MerySgh 08/21/2019 10:20:57
mmm actually !! I'm supposed to develop  jamming attack in my project ... for more clarifying I have a project of autonomous vehicles and I have to make an accident with a passenger in the road according to jamming attack

##### BlackPearl 08/21/2019 10:21:14
`@Fabien Rohrer` it’s still not working ....

##### Fabien Rohrer [Moderator] 08/21/2019 10:21:46
`@MerySgh` haha makes more sense

##### MerySgh 08/21/2019 10:21:55
I  made the accident but how to develop the attack I do not know yet

##### Fabien Rohrer [Moderator] 08/21/2019 10:22:53
`@BlackPearl` so you did downgraded to java and javac 1.8 and clean/build your controller again?


`@MerySgh` are you using SUMO to manage the traffic?

##### BlackPearl 08/21/2019 10:24:53
`@Fabien Rohrer` one minute. We are restarting everything again

##### MerySgh 08/21/2019 10:25:45

> **Attachment**: [city\_2.mp4](https://cdn.discordapp.com/attachments/565155651395780609/613680130568617984/city_2.mp4)


sumo ? what do  you mean ?

##### Fabien Rohrer [Moderator] 08/21/2019 10:28:17
If I understand well, you need to simulate a traffic jam after the collision, right?


In Webots, you can simulate vehicle traffic using SUMO (it's an open-source application which simulates vehicle traffic and which can be interfaced with Webots):


Please take a look at this link and the movie: [https://cyberbotics.com/doc/automobile/sumo-interface](https://cyberbotics.com/doc/automobile/sumo-interface)


Could such traffic simulation meet your expectations?

##### MerySgh 08/21/2019 10:39:32
ok i'm going to see it .. i think i'm not clear enough ... ok the car made an accident with pedestrian because it's attacked from different attacks 

thanks to this attack that the car does not see the passenger crossing the road in my case the attack is jamming

##### BlackPearl 08/21/2019 10:42:31
It still doesn’t work `@fa`

##### Fabien Rohrer [Moderator] 08/21/2019 11:30:38
`@MerySgh` Ok, I understand more and more your scenario. In my knowledge, I'm not aware of similar projects in the Webots community. Do you have specific questions?


`@BlackPearl` Sorry to read this, the next step could be to test if the Java examples provided in Webots are working smoothly or not. Could you simply open WEBOTS\_HOME/projects/languages/java/worlds/example.wbt ?

##### MerySgh 08/21/2019 11:36:36
`@Fabien Rohrer` No thank you so much 🙂

## September

##### Derek 09/10/2019 01:23:04
How can I add some joints in a robot by GUI operation?

##### David Mansolino [Cyberbotics] 09/10/2019 06:06:06
Hi `@Derek`, I would recommend to follow our tutorial, in particular this one will explain how to add 4 joints (connected to 4 wheels) to a robot: [https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot](https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot)


Do you have a particular robot in mind ?

##### SimonDK 09/15/2019 14:31:33
What models for omni-directional mecanum wheel platforms have you seen/are there in Webots? How are the mecanum wheels simulated? I would like to develop my own simulation for a platform so are looking for some examples as a starting point.

##### Stefania Pedrazzi [Cyberbotics] 09/16/2019 06:10:23
Hi `@SimonDK`, the youBot robot has mecanum wheels [https://www.cyberbotics.com/doc/guide/youbot](https://www.cyberbotics.com/doc/guide/youbot)

The mecanum wheel is simulated by setting asymmetric friction  to the [ContactProperties]([https://www.cyberbotics.com/doc/reference/contactproperties](https://www.cyberbotics.com/doc/reference/contactproperties)) of the wheel.


You can find another example in samples/howto/omni\_wheels.wbt simulation:  [https://www.cyberbotics.com/doc/guide/samples-howto#omni\_wheels-wbt](https://www.cyberbotics.com/doc/guide/samples-howto#omni_wheels-wbt). In this case the wheel is simulated using two layers of joints and cylinders.

##### Anoop 09/25/2019 01:27:46
Does webots include this marine robots, exact this one? I can find salamander closest to it?
%figure
![FluidRobot.png](https://cdn.discordapp.com/attachments/565155651395780609/626228320996032512/FluidRobot.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 09/25/2019 06:13:39
Hi `@Anoop`, yes this robot is included in Webots. You can find it in the simulation `projects/samples/rendering/worlds/animated_skin.wbt`

[https://www.cyberbotics.com/doc/guide/samples-rendering#animated\_skin-wbt](https://www.cyberbotics.com/doc/guide/samples-rendering#animated_skin-wbt)


Just note that the `Skin` node used to implement the skin animation is still experimental, i.e. it can be used but it is not documentated as the functionality could still change.

##### Anoop 09/25/2019 17:45:20
Thanks `@Stefania Pedrazzi` for the information. Can I assume this as a soft robotics or more generally can SKIN node be used for generating soft robotics model?

##### Stefania Pedrazzi [Cyberbotics] 09/26/2019 06:20:06
`@Anoop` The `Skin` node is a pure graphical functionality that only modifies the mesh of the robot but doesn't affect the physics.

Webots cannot simulation soft robotics because ODE (the Webots physics engine) doesn't support soft body dynamics.

However there are some tricks to transform a soft robot model into an hard model that could help in simulating soft robotics application.

##### bart 09/29/2019 10:08:43
how to integrate webots with an ide in macOS?

##### Fabien Rohrer [Moderator] 09/30/2019 06:35:35
`@bart` Hi, I expect you mean to create a webots controller inside an IDE, such as XCode. Could you refer to this draft page of the documentation? [https://cyberbotics.com/doc/guide/using-your-ide?version=enhancement-ide-section](https://cyberbotics.com/doc/guide/using-your-ide?version=enhancement-ide-section)

##### David Mansolino [Cyberbotics] 09/30/2019 06:40:07
We also have an example with PyCharm here: [https://www.cyberbotics.com/doc/guide/using-pycharm-with-webots](https://www.cyberbotics.com/doc/guide/using-pycharm-with-webots)

## October

##### pavlos27t 10/17/2019 14:39:46
Hello Sir, i am trying to understand supervisor programming but i found only this example in C : [https://www.cyberbotics.com/doc/guide/supervisor-programming](https://www.cyberbotics.com/doc/guide/supervisor-programming) , could you give me something similar in python?I'm trying this on a robot controller: from controller import * ;robot = Robot();robot.getFromDef(name =  "robotname")  ---> and i get error robot object has no attribute getFromDef , but i have supervisor field :TRUE on the robot


if i change robot = Robot() --> robot = Supervisor() , then webots crashes when a supervisor unction called

##### Fabien Rohrer [Moderator] 10/17/2019 14:42:43
I confirm you should use Supervisor() instead of Robot() to access the Supervisor API


Are you sure that "robotname" matches with an existing DEF name?


>>> DEF robotname Robot {}

##### pavlos27t 10/17/2019 14:43:31
yes because it crashes on every function

##### Fabien Rohrer [Moderator] 10/17/2019 14:44:01
Ok. Could you post the crash error here?

##### pavlos27t 10/17/2019 14:44:38
sorry i don't know how to post a crash error

##### Fabien Rohrer [Moderator] 10/17/2019 14:45:24
Normally, when a Python controller crash, something is displayed in the Webots console. Simply copy-paste it in this chat.


(or a screenshot ? 😉 )

##### pavlos27t 10/17/2019 14:50:35
sorry it doesn't crash now , everything works fine


😅

##### Fabien Rohrer [Moderator] 10/17/2019 15:13:45
I'm happy to read this 😉

##### Hakkinein 10/20/2019 18:45:28
anybody ever try to use webots with ros?


and why use webots over gazebo?

##### SimonDK 10/20/2019 18:53:29
`@Hakkinein` Going to try Webots with ROS very soon. At least for me I have grown very tired of Gazebo. It always feels unstable, cumbersome to set up and we have to use many hacks to get it to work somewhat ok with Reinforcement Learning algorithms. I hope it will be easier to speed up simulations and build simulations with Webots. Let's see 🙂

##### Flo 10/20/2019 19:09:40
`@SimonDK`  I build a gym environnement for webots. Its not released yet but let me know. If you need it for your RL I can accelerate the release

##### Hakkinein 10/20/2019 19:11:53
anyway, is it possible to treat webots controller as ros node?

##### SimonDK 10/21/2019 04:53:16
`@Flo` sounds very interesting, will PM you

##### David Mansolino [Cyberbotics] 10/21/2019 06:53:31
`@Hakkinein`, Webots has several advantages compared to Gazebo, here is a non-exhaustive list:

  - Cross-platform [windows, linux, mac].

Stable physics engine.

  -An efficient rendering engine using Physically Based Rendering for realistic images.

  -A simple and intuitive user interface.

  -Wide range of simulated sensors and actuators available and ready to work.

  -Wide range of robot models available and ready to work.

  -Wide range of documented samples.


> anyway, is it possible to treat webots controller as ros node?



Yes of course, we have developped an interface with ROS1 which is a controller that acts as a generic bridge between Webots and ROS1: [http://wiki.ros.org/webots\_ros/](http://wiki.ros.org/webots_ros/)



And we are currently creating an interface with ROS2, for this new version we are using the Webots API directly in the ros nodes: [http://wiki.ros.org/webots\_ros2](http://wiki.ros.org/webots_ros2)



Let us know if you have any precise question


`@Flo`, that sounds very interesting, looking forward to see this too!


`@SimonDK` :

> Going to try Webots with ROS very soon. At least for me I have grown very tired of Gazebo. It always feels unstable, cumbersome to set up and we have to use many hacks to get it to work somewhat ok with Reinforcement Learning algorithms. I hope it will be easier to speed up simulations and build simulations with Webots. Let's see



For sur it will 😉 let us know if you have any issues doing so.

##### Hakkinein 10/23/2019 10:31:58
`@David Mansolino` thank you for the answer, but is it possible to create a custom robot in Webots? As my robot is custom made and not based on sample robot provided by the Webots simulator.



And what about the ROS controller provided by Webots? can i use it in my custom made robot or should i create custom made controller for my robot that work as bridge between ROS and Webots?.

##### Olivier Michel [Cyberbotics] 10/23/2019 10:33:18
Of course you can create your custom robot in Webots.


See this tutorial: [https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot](https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot)


You can use the ROS controller provided in Webots with your own new robot design. There is no need to modify it. You will simply have to figure out the topics and services published by it.


See details here: [https://cyberbotics.com/doc/guide/using-ros#standard-ros-controller](https://cyberbotics.com/doc/guide/using-ros#standard-ros-controller)

##### SimonDK 10/25/2019 17:24:39
`@Flo` I sent you a PM, did you receive it? /cheers

## November

##### Hakkinein 11/02/2019 00:48:58
how can i launch python controller in webots?

##### Stefania Pedrazzi [Cyberbotics] 11/04/2019 07:08:37
Hi `@Hakkinein`, usually controllers are automatically started by Webots when you run the simulation if the is used by any robot node, i.e. the Robot.controller field is set to your python controller name. Does this not work for you?

##### pavlos27t 11/04/2019 13:16:17
Hello Sir, i have a strange problem: i give equal speed on left and right wheel of e-puck robot but it doesn't move linear, i print my velocities to confirm it and they are equal ,so why my robot turns?

##### David Mansolino [Cyberbotics] 11/04/2019 13:17:21
Hi `@pavlos27t`, just to make sure you are speaking about simulation right? Does the robot collide with something? is the ground flat?

##### pavlos27t 11/04/2019 13:21:26
the ground is flat, but there is a possiblity for the e-puck robot to collide with another e-puch robot that i have in the simulation


i reset my robots like this [http://prntscr.com/ps9cqr](http://prntscr.com/ps9cqr)


if collision happens and forces applying should i reset the forces too?

##### David Mansolino [Cyberbotics] 11/04/2019 13:26:07
you probably want to reset the physics of the robot too indeed: wb\_supervisor\_node\_reset\_physics

##### pavlos27t 11/04/2019 13:33:23
right thanks i didn't know how to reset physics..

##### David Mansolino [Cyberbotics] 11/04/2019 13:33:39
You're welcome

##### pavlos27t 11/06/2019 18:54:05
hello again , i resetphysics but some forces still applying after collision



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/641711950677409813/unknown.png)
%end

##### David Mansolino [Cyberbotics] 11/07/2019 07:58:34
`@pavlos27t` you should make sure that the wheels are not colliding the floor when you move the robot. It is also recommended to check what is happenning with the contact points just before and just after the reset, you can see this in the view / Optional Rendering menu.

##### pavlos27t 11/07/2019 10:41:35
how can i make sure the wheels are not colliding the floor?

##### David Mansolino [Cyberbotics] 11/07/2019 16:16:08
By checking the contact points you can see if it does collide with the floor. If you don't see any contact points then it means that it is not colliding: [https://cyberbotics.com/doc/guide/the-user-interface#view-menu](https://cyberbotics.com/doc/guide/the-user-interface#view-menu)

##### pavlos27 11/07/2019 18:16:14
i see contact points with the floor but what that means?my robot can't be in the air


it will always colliding with the floor


i have reset the physics of the floor too, but still i have the same problem


i don't know what to do to fix that do you have any ideas?

##### Fabien Rohrer [Moderator] 11/11/2019 07:20:24
`@pavlos27` Would it be possible to translate it at the floor level, but to add a very small vertical offset, just in order to avoid collisions with the floor during the translation?

##### pavlos27 11/13/2019 11:20:07
unfortunately this didn't worked either

##### Fabien Rohrer [Moderator] 11/13/2019 12:29:50
Could you better describe what does not work?

##### pavlos27 11/13/2019 12:46:06
i have 2 robots and a rectangle arena . The robots are moving only in one linear direction  one behind the other and sometimes they are colliding . When i reset my robots to their initial position like this



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565155651395780609/644156174123532351/unknown.png)
%end

##### Fabien Rohrer [Moderator] 11/13/2019 12:47:25
it sounds very correct to me.


(the floor.resetPhysics() is certainly useless, because a floor has no physics)

##### pavlos27 11/13/2019 12:48:28
but still collisions forces are applied after reset


but this doesn't happen at first reset


this happens after 50-60 resets

##### Fabien Rohrer [Moderator] 11/13/2019 12:49:48
I fear that some error is accumulated.


did you try to revert or reload the whole simulation instead?

##### pavlos27 11/13/2019 12:52:34
no i will try

##### Fabien Rohrer [Moderator] 11/13/2019 12:54:00
I would go for this function first: [https://www.cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_simulation\_reset](https://www.cyberbotics.com/doc/reference/supervisor#wb_supervisor_simulation_reset)

##### Ella 11/19/2019 16:04:03
Hello~ I am a real beginner of Webot!!  I want to use a* algorithm  boxes as obstacles and find the path!! but it's really hard,,,, can you give me some tips where to start??

##### Fabien Rohrer [Moderator] 11/19/2019 16:05:16
`@Ella` Hi, I confirm this task may be hard. There is currently no example about such implementation.


If I were you, I would first use an existing library to deal with the algorithm, and a predefined map stored in the controller. Webots can retrieve easily the robot position in the map.


Does this answer your question?


The objects of a scene and their size may be retrieved using a supervisor, like this:


[https://gist.github.com/fabienrohrer/543c2a9751eefc6d352e6957e6e3dc90](https://gist.github.com/fabienrohrer/543c2a9751eefc6d352e6957e6e3dc90)


But using a predefined map to start is certainly simpler.

##### Ella 11/19/2019 16:16:53
`@Fabien Rohrer`  Thank you so much!!! Maybe I should try step by step!!!

##### Fabien Rohrer [Moderator] 11/19/2019 16:38:52
you're welcome, good luck with your project 🙂

##### Dorteel 11/20/2019 09:30:13
Hi guys! Does anybody here have a model for converting the two wheel velocities of a differential-drive robot to the robot's angular and linear velocity?

##### Fabien Rohrer [Moderator] 11/20/2019 09:31:16
`@Dorteel` Hi, it's a matter to compute the robot odometry, usually using simple trigonometric functions.

##### Dorteel 11/20/2019 09:32:31
Thanks `@Fabi`en! I think I found something online!

##### SimonDK 11/27/2019 17:01:08
Regarding the Webots graphical interface, I was wondering if there are any plans to have somethings like groups or folders in the Scene Tree, or being able to drag'n'drop to reorganize items? In big simulations you easily lose overview of all items 😀

##### David Mansolino [Cyberbotics] 11/28/2019 07:33:06
Yes we have some long-term plans to improve the scene-tree in Webots, but those are long term plans and will not be implemented in a near future (unfortunately we have currently more urgent things to improve), but if you want to try implementing this you are welcome to contribute 😉

##### SimonDK 11/28/2019 18:09:02
Great to hear it is in the long-term plans at least 😊 I get there are more urgent things. If time would allow it, I would give it a go 😁

##### Enger 11/30/2019 22:42:33
Hi Guys, I am new to Webots but I want to use it for a robot soccer simulation project. I would like to use the Robotis OP2 robot but I want to code in python. Is it possible?

## December

##### Tahir [Moderator] 12/01/2019 01:44:29
Hi 

`@Enger` Yes it is possible


You can see a sample soccer simulation in webots demos as well


[https://cyberbotics.com/doc/guide/samples-demos](https://cyberbotics.com/doc/guide/samples-demos)

##### Musuyaba 12/03/2019 01:37:29
Hi, i get this error, 
```   11 | #include <webots/servo.h>
      |          ^~~~~~~~~~~~~~~~ 
```

and i think i need servo library, anyone have it? thankyou

##### Stefania Pedrazzi [Cyberbotics] 12/03/2019 07:27:24
`@Musuyaba` the Servo node has been deprecated since many years now (starting from Webots 7.2). Please use the joint and motor nodes instead.


You can find some examples in the Webots samples library:

- samples/devices/motor.wbt ([https://cyberbotics.com/doc/guide/samples-devices#motor-wbt](https://cyberbotics.com/doc/guide/samples-devices#motor-wbt))

- samples/devices/motor2.wbt ([https://cyberbotics.com/doc/guide/samples-devices#motor2-wbt](https://cyberbotics.com/doc/guide/samples-devices#motor2-wbt))

- samples/devices/motor3.wbt ([https://cyberbotics.com/doc/guide/samples-devices#motor3-wbt](https://cyberbotics.com/doc/guide/samples-devices#motor3-wbt))

- samples/devices/linear\_motor.wbt ([https://cyberbotics.com/doc/guide/samples-devices#linear\_motor-wbt](https://cyberbotics.com/doc/guide/samples-devices#linear_motor-wbt))

##### Musuyaba 12/03/2019 08:40:33
I see, thank you, i will try it later

##### iloving 12/05/2019 01:29:59
Hi, I can not connect the Internet license server, and the site [https://www.cyberbotics.com/](https://www.cyberbotics.com/) is not attached. Anyone have it?

##### David Mansolino [Cyberbotics] 12/05/2019 07:21:00
`@iloving`, we are currenlty running some migration on our server. This might take some time, in the meantime you can use the latest version of Webots which doesn't require any internet license: [https://github.com/cyberbotics/webots/releases/tag/R2019b-rev1](https://github.com/cyberbotics/webots/releases/tag/R2019b-rev1)

##### juanrh 12/06/2019 20:00:55
Hi, I'm a newby trying to use webots with Python. I'm on Fedora 30 with webots installed with snap. I did export WEBOTS\_HOME=/var/lib/snapd/snap/webots/current export PYTHONPATH="${WEBOTS\_HOME}/usr/share/webots/lib/python37"

export PYTHONIOENCODING='UTF-8'

export LD\_LIBRARY\_PATH="${LD\_LIBRARY\_PATH}:${WEBOTS\_HOME}/lib" and when I launch `python3` and do `import controller` I get "ImportError: libCppController.so: cannot open shared object file: No such file or directory"


I don't find that .so file in my webots installation. I have [juanrh@juanydell hello\_webots\_py]$ ls $WEBOTS\_HOME/lib

bindtextdomain.so  systemd  udev  x86\_64-linux-gnu


any idea what might be the problem here?


ah ok, it looks I should use `export WEBOTS_HOME=/var/lib/snapd/snap/webots/current/usr/share/webots` instead

##### Fabien Rohrer [Moderator] 12/06/2019 20:08:59
Good job! 👍

##### luoyu 12/09/2019 02:43:09
Hi, I have a problem that wrong license password for my account. Is It caused by the migration on the server？Can I know how long it will take？

##### Fabien Rohrer [Moderator] 12/09/2019 04:40:59
`@luoyu` could you try with the « webots » password? [https://cyberbotics.com/doc/guide/general-faq#can-i-still-use-a-webots-version-before-the-r2019a-release](https://cyberbotics.com/doc/guide/general-faq#can-i-still-use-a-webots-version-before-the-r2019a-release)


Last week, we simply open the license server (dealing with Webots licenses before R2019) as Webots is open source since one year.

##### luoyu 12/09/2019 07:09:40
Thank you very much. The magical password is useful.

##### kawaiipotato2023 12/10/2019 07:59:17
Hello! I just downloaded webots and when I log in it tells me that I am not licensed to use the program. Does anyone know why this happens or how to resolve it?

##### David Mansolino [Cyberbotics] 12/10/2019 07:59:56
Hi `@kawaiipotato2023`, which version of Webots did you download?

##### kawaiipotato2023 12/10/2019 08:02:13
Webots 8 2.1.2 for windows

##### David Mansolino [Cyberbotics] 12/10/2019 08:02:43
Why not using the latest version ?

##### kawaiipotato2023 12/10/2019 08:03:49
This was the only version I found, do you have a link to where I can get the latest version?

##### David Mansolino [Cyberbotics] 12/10/2019 08:04:56
Yes of course, you can download the latest version from here: [https://github.com/cyberbotics/webots/releases/tag/R2019b-rev1](https://github.com/cyberbotics/webots/releases/tag/R2019b-rev1)

##### kawaiipotato2023 12/10/2019 08:05:24
Thank you!

##### David Mansolino [Cyberbotics] 12/10/2019 08:05:30
You're welcome

