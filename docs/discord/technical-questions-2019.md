# Technical-Questions 2019

This is an archive of the `technical-questions` channel of the [Webots Discord server](https://discordapp.com/invite/nTWbN9m) for year 2019.

## April

##### shridhar 04/18/2019 07:08:31
Hello, I have included camera and a Hingejoint in  children field of Robot and wrote a controller to spin a solid cylinder but when I run the controller even the cameras are rotating. so how to rotate the solid without rotating camera ( i tried with two separate Robots but still the result is same.

##### Stefania Pedrazzi [Cyberbotics] 04/18/2019 07:10:35
Hi, if you don't want to rotate  the camera, then the Camera node doesn't have to be a descendant of the HingeJoint node

##### shridhar 04/18/2019 07:13:08
ya the camera is not descendant of HingeJoint but both are children of Robot.

##### Stefania Pedrazzi [Cyberbotics] 04/18/2019 07:15:02
So then if you rotate the HingeJoint, the camera doesn't move. Please check your world because if it works differently then your are not moving the correct HingeJoint or your robot node hierarchy is wrong.

##### shridhar 04/18/2019 07:17:12
ok I will check it

##### Jiajun Wang 04/18/2019 08:00:06
[four\_wheel\_avoid\_matlab] Error using calllib

[four\_wheel\_avoid\_matlab] Parameter can not be converted to a character vector

[four\_wheel\_avoid\_matlab] 

[four\_wheel\_avoid\_matlab] Error in wb\_robot\_get\_device (line 6)

[four\_wheel\_avoid\_matlab] result = calllib('libController', 'wb\_robot\_get\_device', name);

[four\_wheel\_avoid\_matlab] 

[four\_wheel\_avoid\_matlab] Error in four\_wheel\_avoid\_matlab (line 23)

[four\_wheel\_avoid\_matlab]   ps(i) = wb\_robot\_get\_device(ps\_names(i));

[four\_wheel\_avoid\_matlab] 

[four\_wheel\_avoid\_matlab] Error in launcher (line 151)

[four\_wheel\_avoid\_matlab]   eval(WEBOTS\_CONTROLLER\_NAME);


how to slove this error

##### Fabien Rohrer [Moderator] 04/18/2019 08:01:15
Could you tell us your OS, Webots version and Matlab version?


Do you try to run a sample provided in Webots (which one?)? Or did you modified the controller?

##### Jiajun Wang 04/18/2019 08:02:04
tutorial  Tutorial 6: 4-Wheels Robot   when I use C++，no problem;   but use matlab , error


both in win10 & ubuntu18.04.2


in win10  matlab2018b  ;  in ubuntu  matlab2017b


I had try "WEBOTS\_HOME/projects/languages/matlab/worlds/e-puck\_matlab.wbt"  in win10  Matlab can't start

##### Fabien Rohrer [Moderator] 04/18/2019 08:05:52
Did you tried with Webots R2019a.rev1?

##### Jiajun Wang 04/18/2019 08:06:14
of course


while in ubuntu  :[matlab] Error: /usr/local/MATLAB/R2017b/bin/glnxa64/../../sys/os/glnxa64/libstdc++.so.6: version `GLIBCXX\_3.4.21' not found (required by /usr/local/webots/projects/robots/gctronic/e-puck/plugins/remote\_controls/e-puck\_bluetooth/libe-puck\_bluetooth.so) (dynamic library)

[matlab] Error: remote control initialisation failed

[matlab] Error: Cannot load the "/usr/local/webots/projects/robots/gctronic/e-puck/plugins/remote\_controls/e-puck\_bluetooth/libe-puck\_bluetooth.so" remote control library.


when I try  "WEBOTS\_HOME/projects/languages/matlab/worlds/e-puck\_matlab.wbt"  in ubuntu18

##### Fabien Rohrer [Moderator] 04/18/2019 08:08:19
Ok


On Ubuntu, you could try to remove the remoteControl library from the E-puck.proto


To do so, please try to remove the last lines of projects/robots/gctronic/e-puck/protos/E-puck.proto


%{ if v2 then }%

    remoteControl "e-puck\_wifi"

  %{ else }%

    remoteControl "e-puck\_bluetooth"

  %{ end }%


This would give us clues: is this library problematic for matlab?

##### Jiajun Wang 04/18/2019 08:12:22
And，what about the error in win10？

##### Fabien Rohrer [Moderator] 04/18/2019 08:13:13
it seems that at line 23 of your controller, you don't send a string.


could you check this by printing the ps\_names(i)?

##### Jiajun Wang 04/18/2019 08:16:41
ok，i will try later.         In win10, always show: Mathworks collapse ! and never success in simulation


even couldnot start

##### Fabien Rohrer [Moderator] 04/18/2019 08:18:50
I'm not sure why. It would deserve to be debugged. It would be extremely useful if you could determine at which point it fails precisely, and fill a bug report on our GitHub issues.

##### Jiajun Wang 04/18/2019 08:19:37
Yesterday , you suggested me to using matlab or python . so i try

##### Fabien Rohrer [Moderator] 04/18/2019 08:20:01
Sorry for this.

##### Jiajun Wang 04/18/2019 08:21:08
I'm not complaining.

##### Fabien Rohrer [Moderator] 04/18/2019 08:22:59
Python is much simpler to use IMO. If it's just a matter of plotting something, why not using [https://matplotlib.org/users/pyplot\_tutorial.html](https://matplotlib.org/users/pyplot_tutorial.html) ?

##### Jiajun Wang 04/18/2019 08:29:00

%figure
![win10_ERROR.PNG](https://cdn.discordapp.com/attachments/565154703139405824/568352267258953728/win10_ERROR.PNG)
%end


win10   always  show this

##### Fabien Rohrer [Moderator] 04/18/2019 08:31:34
I know Matlab has issues with UTF8 characters in path. Could you even run a .m file in Matlab without Webots?


(I cannot read the error message 😃 )

##### Jiajun Wang 04/18/2019 08:33:29
the words are Chinese, I am sorry .   It means "Mathworks Collapse Report procedure "


An internal error was encountered and needs to be closed

##### Fabien Rohrer [Moderator] 04/18/2019 08:36:13
We already had Chinese users complaining about Matlab. I think you are in this situation (without guarantee).

##### Jiajun Wang 04/18/2019 08:37:17
I can run a.m file  in Matlab~

##### Fabien Rohrer [Moderator] 04/18/2019 08:42:40
Sorry, I have no ideas to help you further. It seems Matlab doesn't like something, but it's impossible to say what without stack or clear error message. It crashes at startup. This is certainly a Matlab bug 😕

##### Jiajun Wang 04/18/2019 09:22:01
Now， I have the successful running in Ubuntu .  thank you Fabien

##### Fabien Rohrer [Moderator] 04/18/2019 09:22:21
Great, thank you for the update.

##### Jiajun Wang 04/18/2019 09:23:36
your  Tuturial has a lot of errors about the Matlab code😂

##### Fabien Rohrer [Moderator] 04/18/2019 09:23:47
oops


It would be awesome if you could contribute by creating a PR on Github fixing the issues 👍

##### Jiajun Wang 04/18/2019 09:29:44
INFO: four\_wheels\_avoid\_collision: Starting: ""C:\Program Files\MATLAB\R2018b\bin\win64\MATLAB.exe" -nosplash -nodesktop -minimize -r launcher"

[four\_wheels\_avoid\_collision] Assertion failed: 

[four\_wheels\_avoid\_collision] 

[four\_wheels\_avoid\_collision] Function: void \_\_cdecl `anonymous-namespace'::mwJavaAbort(void), file b:\matlab\src\jmi\jmi\javainit.cpp, line 1366

[four\_wheels\_avoid\_collision] #

[four\_wheels\_avoid\_collision] # A fatal error has been detected by the Java Runtime Environment:

[four\_wheels\_avoid\_collision] #

[four\_wheels\_avoid\_collision] #  EXCEPTION\_ACCESS\_VIOLATION (0xc0000005) at pc=0x00000000ec4be8fe, pid=5408, tid=0x0000000000001dfc

[four\_wheels\_avoid\_collision] #

[four\_wheels\_avoid\_collision] # JRE version: Java(TM) SE Runtime Environment (8.0\_152-b16) (build 1.8.0\_152-b16)

[four\_wheels\_avoid\_collision] # Java VM: Java HotSpot(TM) 64-Bit Server VM (25.152-b16 mixed mode windows-amd64 compressed oops)

[four\_wheels\_avoid\_collision] # Problematic frame:

[four\_wheels\_avoid\_collision] # C  0x00000000ec4be8fe

[four\_wheels\_avoid\_collision] #

[four\_wheels\_avoid\_collision] # Failed to write core dump. Minidumps are not enabled by default on client versions of Windows

[four\_wheels\_avoid\_collision] #

[four\_wheels\_avoid\_collision] # An error report file with more information is saved as:

[four\_wheels\_avoid\_collision] # C:\Users\Jason\AppData\Local\Temp\hs\_error\_pid5408.log

[four\_wheels\_avoid\_collision] #

[four\_wheels\_avoid\_collision] # If you would like to submit a bug report, please visit:

[four\_wheels\_avoid\_collision] #   [http://bugreport.java.com/bugreport/crash.jsp](http://bugreport.java.com/bugreport/crash.jsp)

[four\_wheels\_avoid\_collision] #

WARNING: 'four\_wheels\_avoid\_collision' controller exited with status: 3.

##### Fabien Rohrer [Moderator] 04/18/2019 09:31:21
Mmm, this is interesting. It looks like the issue is in-between Matlab and Java.

##### Jiajun Wang 04/18/2019 09:31:22
the win10  error log.  Have you ever saw like this


OK . I choose ubuntu then.

##### Fabien Rohrer [Moderator] 04/18/2019 09:32:27
[https://ch.mathworks.com/matlabcentral/answers/182412-matlab-access-violation-on-startup](https://ch.mathworks.com/matlabcentral/answers/182412-matlab-access-violation-on-startup)


It seems that starting java with "-nojvm" may help


Could you add in your controller a "runtime.ini" file?


Containing:


[matlab]


OPTIONS = -nojvm


(reference: [https://cyberbotics.com/doc/guide/controller-programming#languages-settings](https://cyberbotics.com/doc/guide/controller-programming#languages-settings) )

##### Jiajun Wang 04/18/2019 09:34:42
ok, let me have a try


solved~


haha , Thank you very much

##### Fabien Rohrer [Moderator] 04/18/2019 09:38:24
Great!

##### Jiajun Wang 04/18/2019 09:38:27
ERROR: Cannot initialize the sound engine: Cannot initialize OpenAL default device 'OpenAL Soft'


only  show this

##### Fabien Rohrer [Moderator] 04/18/2019 09:38:48
I'm wondering if we should add the "-nojvm" option by default everywhere.

##### Jiajun Wang 04/18/2019 09:39:26
what about this:   ERROR: Cannot initialize the sound engine: Cannot initialize OpenAL default device 'OpenAL Soft' ?


in  windows 10



%figure
![win10_ERROR.PNG](https://cdn.discordapp.com/attachments/565154703139405824/568370208000180234/win10_ERROR.PNG)
%end

##### Fabien Rohrer [Moderator] 04/18/2019 09:40:48
This error appear when the sound engine cannot be initialized smoothly.


Do you have issues with sound on your computer?


I think that if you "mute" the sound (from the top menu) this should disapear.


(not sure)

##### Jiajun Wang 04/18/2019 09:41:54
update driver? or  just Ignore this error?

##### Fabien Rohrer [Moderator] 04/18/2019 09:42:08
for sure this error may be ignored.

##### Jiajun Wang 04/18/2019 09:45:10
OK  .  yesterday  I find a way to programming C++ controller in the IDE ---Clion .  what should I do to programming *.m controller in Matlab Editor ?


For  the default Editor in Webots is not convenient enough, for syntactic

##### Fabien Rohrer [Moderator] 04/18/2019 09:48:17
Sorry, I don't have matlab right now. Couldn't you simply open the .m controller in the Matlab editor?

##### Jiajun Wang 04/18/2019 09:49:14
the matlab editor could not find : the function like : wb\_distance\_sensor\_get\_value(ps{i})

##### Fabien Rohrer [Moderator] 04/18/2019 09:52:11
I'm not expert in the Matlab GUI. I expect there is a way to open/target/link with the $WEBOTS\_HOME/lib/matlab directory!?

##### Jiajun Wang 04/18/2019 09:52:47
ok  , thank you a lot

##### Fabien Rohrer [Moderator] 04/18/2019 09:53:49
I'm happy that it's working for you. Plotting in Matlab is very convenient. This is the force of Matlab IMO.

##### Jiajun Wang 04/18/2019 09:54:39
😋

##### shridhar 04/18/2019 13:30:48
hi how to save image from weboots in opencv format (WbDeviceTag processed\_image\_display = wb\_robot\_get\_device("proc\_im\_display");)

##### Fabien Rohrer [Moderator] 04/18/2019 13:31:33
hi


you refer a Display node. Do you want to store the result of a Display node?

##### shridhar 04/18/2019 13:32:35
yes


either captured or displayed ..

##### Fabien Rohrer [Moderator] 04/18/2019 13:34:17
There is no way to access the Display texture data directly, but you could use the wb\_display\_image\_save() function ([https://www.cyberbotics.com/doc/reference/display#wb\_display\_image\_save](https://www.cyberbotics.com/doc/reference/display#wb_display_image_save)) and open it in opencv

##### shridhar 04/18/2019 13:35:08
ok then how to use opencv or read image..

##### Fabien Rohrer [Moderator] 04/18/2019 13:35:40
I can easily provide you these examples:


[https://cyberbotics.com/doc/guide/samples-howto#vision-wbt](https://cyberbotics.com/doc/guide/samples-howto#vision-wbt)


I recommend you to read the OpenCV doc to understand how to load an image. This is a basic task.

##### shridhar 04/18/2019 13:37:47
Thank you.. sorry for the simple question ..

##### Fabien Rohrer [Moderator] 04/18/2019 13:39:09
I think it would deserve to post this kind of general question (quite out of focus of a direct use of Webots), on stackoverflow with the Webots tag. We monitor these questions and answer them if we think it's interesting for the community:


[https://stackoverflow.com/questions/tagged/webots](https://stackoverflow.com/questions/tagged/webots)

##### shridhar 04/18/2019 13:40:09
ok sure.

##### Fabien Rohrer [Moderator] 04/18/2019 13:43:06
I did a quick search and I have unfortunately not better examples to provide you.

##### Thelm 04/18/2019 14:08:13
Hi, i'm trying to implement the missing sensors of the E-PUCK in the webots' simulation,

I am actually working on adding the ToF sensor of th V2 version by editing the prototype file but it returns an error:



    %{ if v2 then }%

      DEF EPUCK\_TOF E-puckDistanceSensor {

        translation 0 0.033 -0.035

        rotation 0 1 0 1.57

        name "ToF"

        lookupTable [

          0 0 0 4 4 0

        ]

      }

    %{ end }%







ERROR: 'S:/GDrive/Traineeship/prog/Webots/e-puck/maze/protos/E-puck.proto':367:7: error: Skipped unknown 'lookupTable' parameter in E-puckDistanceSensor PROTO.

##### Fabien Rohrer [Moderator] 04/18/2019 14:10:50
Quite difficult to understand: the E-puckDistance sensor is a PROTO (i.e. a façade) of a DistanceSensor node.


Its definition is there: projects/robots/gctronic/e-puck/protos/E-puckDistanceSensor.proto


[https://github.com/omichel/webots/blob/master/projects/robots/gctronic/e-puck/protos/E-puckDistanceSensor.proto#L1](https://github.com/omichel/webots/blob/master/projects/robots/gctronic/e-puck/protos/E-puckDistanceSensor.proto#L1)


the lookupTable is not an open field.

##### Thelm 04/18/2019 14:12:03
Actually I've just copied one of the other distance sensors and tried to modify it in order to have a different range


oh


I think I've found my error

##### Fabien Rohrer [Moderator] 04/18/2019 14:12:44
You could open the lookupTable by adding a line in the  E-puckDistanceSensor.proto file:

##### Thelm 04/18/2019 14:12:56
I have left e-puckDistanceSensor instead of just  DistanceSensor that's right?

##### Fabien Rohrer [Moderator] 04/18/2019 14:13:17
Yes, that's another approach.


You could directly use DistanceSensor there

##### Thelm 04/18/2019 14:13:58
but will the sensors added in the proto file be working with a real e-puck?

##### Fabien Rohrer [Moderator] 04/18/2019 14:14:15
Unfortunately no.

##### Thelm 04/18/2019 14:14:29
oh...

##### Fabien Rohrer [Moderator] 04/18/2019 14:14:35
If you do that, you only act on the simulated robot.


(which is the first step in any case)

##### Thelm 04/18/2019 14:15:14
in fact, I am trying to add a support of the sensors like the gyro, the magnetometer and the ToF sensor in webots...

##### Fabien Rohrer [Moderator] 04/18/2019 14:15:53
This is a good development 👍


Once added in the E-puck PROTO, you should also add the devices to the communication protocol between the e-puck controller and the real e-puck

##### Thelm 04/18/2019 14:16:54
and as I'm beggining with webots, I don't really know which file defines the sensors in the real e-puck

##### Fabien Rohrer [Moderator] 04/18/2019 14:16:55
You simply have to add this into the e-puck\_wifi library... somewhere 😃


You should take a look at this directory: [https://github.com/omichel/webots/tree/master/projects/robots/gctronic/e-puck/plugins/remote\_controls/e-puck\_wifi](https://github.com/omichel/webots/tree/master/projects/robots/gctronic/e-puck/plugins/remote_controls/e-puck_wifi)

##### Thelm 04/18/2019 14:18:03
If I successfully add these sensors I could send you the files if you want

##### Fabien Rohrer [Moderator] 04/18/2019 14:18:34
It would be awesome if you could create a GitHub Pull Request for this.


We could communicate there and help you to achieve.


To do so, just fork the webots repo, create a branch containing your patch, and you should be able to create the pull request.


For example, here is a pull request a user had submited recently:


[https://github.com/omichel/webots/pull/350](https://github.com/omichel/webots/pull/350)

##### Thelm 04/18/2019 14:21:47
okay thank you I'll take a look for completing th e-puck files then 😄

##### Fabien Rohrer [Moderator] 04/18/2019 14:22:07
I look forward to see this 😃

##### shridhar 04/19/2019 06:30:46
INFO: camer1: Starting: ""D:\webotsim\test4\controllers\camer1\camer1.exe""

[camer1] Vision module demo, using openCV.

WARNING: camer1: The process crashed some time after starting successfully.

WARNING: 'camer1' controller crashed.


the webots is crashing for simple image processing program .

##### el 04/19/2019 09:36:32
how to add opencv library to webots compiler mingw

##### Fabien Rohrer [Moderator] 04/19/2019 09:49:17
`@shridhar` it definitely comes from your controller. Could you check which is the problematic line?


`@el` we have a sample linked with opencv (embedded in Webots).


[https://cyberbotics.com/doc/guide/samples-howto#vision-wbt](https://cyberbotics.com/doc/guide/samples-howto#vision-wbt)

##### el 04/19/2019 09:51:24
thanks 😃

##### Fabien Rohrer [Moderator] 04/19/2019 09:52:27
`@el` `@shridhar` you are doing the same thing 😉

##### el 04/19/2019 10:01:00
sorry for the stupid question, so there is no need to download opencv library cause its already in webots?

##### Fabien Rohrer [Moderator] 04/19/2019 10:36:18
Exactly! Webots includes already the opencv library.

##### el 04/19/2019 10:45:09
great, but when i run the vision.wbt world it says this


INFO: vision: Starting: ""D:\Programs\Webots\projects\samples\howto\controllers\vision\vision.exe""

WARNING: vision: The process crashed some time after starting successfully.

WARNING: 'vision' controller crashed.


what is going wrong?

##### Fabien Rohrer [Moderator] 04/19/2019 11:23:47
Oops, this is not normal. Could I kindly ask you to open a GitHub issue? Because of the Easter holidays, we will certainly look at this issue only on next Tuesday.

##### el 04/19/2019 11:29:15
ok i will, only this  sample with opencv has this problem everything else worls

##### Fabien Rohrer [Moderator] 04/19/2019 11:37:09
Meanwhile I could only propose you to switch to Linux or macOS if you could.

##### el 04/19/2019 13:03:12
thanks

##### Lukas\_ 04/19/2019 13:47:30
Hello, I have question. In webots  -> generic robot window -> GPS i can see graph with all values during run. Can I somehow export all those data to external file? I am using C code and have printf for value at given time (step()) but what if i want to access all data as they are in the graph. Can i do it and if so, how?

##### Fabien Rohrer [Moderator] 04/19/2019 16:39:24
Instead of printf, you could use fprintf to write those values in a file. This file could then be imported in your favorite plotter (pyplot, Matlab, excel, harryplotter, etc. )

##### Lukas\_ 04/19/2019 16:42:58
well yes but printf fprintf only give me value in current time or after current step() but what if in the end of simulation i want to have all sensor data with TIME\_STEP. Am i missing something? Is this what i ask for possible?

##### Fabien Rohrer [Moderator] 04/19/2019 16:56:10
fprintf allows you to append data to an existing file, therefore to store the history. For example, you could create the file at the begining of the simulation (fprintf(..., “w”)) and then append to this file a line containing the data and the time per step (fprintf(f, “%g, %g, %g, %g\n“, time, v0, v1, v2))

##### Lukas\_ 04/19/2019 17:04:39
Yes i understand that but what you suggest is excatly what i dont want, save data at given time. The graph thats is made durring simulation i would like those data used in it. How can i access all of them? Not only data that i can save only at time but like in the graph data from time when sensor is enabled until the end of simulation.  They should be stored somewhere since i can change between time, xy, xz and yz. But how i can access them?

##### el 04/19/2019 17:33:41
libobencv\_imgprog401.dll not found , do you know maybe how can i solve it easily?

##### Fabien Rohrer [Moderator] 04/19/2019 18:38:42
`@Lukas_` the source code of the generic robot window is in this directory: [https://github.com/omichel/webots/tree/master/resources/projects/plugins/robot\_windows/generic](https://github.com/omichel/webots/tree/master/resources/projects/plugins/robot_windows/generic)


`@el` OpenCv is supposed to be installed from pacman here: [https://github.com/omichel/webots/blob/master/src/install\_scripts/msys64\_installer.sh#L28](https://github.com/omichel/webots/blob/master/src/install_scripts/msys64_installer.sh#L28)


But it seems to be not released: [https://github.com/omichel/webots/blob/master/src/packaging/files\_msys64.txt](https://github.com/omichel/webots/blob/master/src/packaging/files_msys64.txt)


We will create a fix soon


Meanwhile you could solve this by installing Webots from sources


[https://github.com/omichel/webots/wiki/Windows-installation](https://github.com/omichel/webots/wiki/Windows-installation)

##### Lukas\_ 04/19/2019 18:49:33
Wait, so i have to save data there? Basically i have to change webots simulation file on my PC?

##### Fabien Rohrer [Moderator] 04/19/2019 18:50:50
To be honest I still don’t understand why you cannot store the data from the controller 😅

##### Lukas\_ 04/19/2019 18:53:24
Well i want to have data with static time period difrence between each one and in code i have move comand then i wait, then end, after that different command with different wait, pause, sleep  so i am unable to have this period in between.


The sensor reads data at any given moment right? So is there any easy way to access this container, bulk, field of datas?

##### Fabien Rohrer [Moderator] 04/19/2019 19:00:14
The controller is by definition at a fixed time between to steps. You can get the simulated time there.


When the sensor API is used, it gives you the sensor feedback of the last discrete simulation step.

##### Lukas\_ 04/19/2019 19:07:49
But i would like to get values of every step. Like in matlab where you get all values after simulation and can plot them. This is what i need. Is it possible from controller?

##### Fabien Rohrer [Moderator] 04/19/2019 19:10:39
Yes sure. The Matlab sample is a controller, and the generic robot window uses the controller API only 😉

##### Lukas\_ 04/19/2019 19:12:18
So i must change the source of generic robot window to save all sensor data?

##### Fabien Rohrer [Moderator] 04/19/2019 19:12:41
You should document yourself on the way time is managed in Webots.


Not necessarily. It’s a bad idea IMO, because everything is directly accessible from the controller API.

##### Lukas\_ 04/19/2019 19:14:04
To be honest now i am lost 😅

##### Fabien Rohrer [Moderator] 04/19/2019 19:17:13
Haha sorry for this


If you create a simple controller, and write a line between each wb\_robot\_step call, this ensures you cannot miss any data.


This ensures you that you will have one data every 32 simulated milliseconds. (If WorldInfo.basicTimeStep = 32)


So this is perfect to plot a graph with a regular x-axis (time)


This is what we do in the generic window.


Sounds better like this?

##### Lukas\_ 04/19/2019 19:22:50
yes thank you


would it be problem if i use this function as a sleep?

static void passive\_wait(double sec) {

  double start\_time = wb\_robot\_get\_time();

  do {

    step();

  } while (start\_time + sec > wb\_robot\_get\_time());

}


i should change to something like that 

static void passive\_wait(double sec) {

  double start\_time = wb\_robot\_get\_time();

  do {

    step();

fprinft(mydata)

  } while (start\_time + sec > wb\_robot\_get\_time());

}

RIGHT?

##### Fabien Rohrer [Moderator] 04/19/2019 19:26:55
Both are possible.


If you choose solution 1. You should store also the simulated time when you acquire the data.


If you choose solution 2. you know that the time between two values are taken is constant.


It’s just something to take into account when you plot the data.

##### Lukas\_ 04/19/2019 19:30:50
So solution 2 it is for me thank, this was help full. I did not fully understood it before now now i know what i need to do. 👌  great job, thanks

##### Fabien Rohrer [Moderator] 04/19/2019 19:31:42
Great, you’re welcome!

##### ina 04/20/2019 02:46:02
I want to use "cin" to input from console. But it doesn't work???


"The Console menu item opens the Webots Console, which is a read-only console that is used to display Webots error messages and controller outputs."


......


how to debug the variable and  jump to the function using vs2017?

##### wail\_maghdoud 04/20/2019 09:35:38
Hi,

I installed Webots 8. In the dialogue window I type my e-mail address and password and then I get confirmation that my floating license exists but as soon as I confirme it I get a message saying I have no license rights.  How can i get the 30 days  trial licenece?

Thanks in advance,

##### Fabien Rohrer [Moderator] 04/20/2019 10:37:35
`@wail_maghdoud` webots 8 is obsolete (proprietary and closed source), please use R2019.rev1 (free and open-source, same features)


`@ina` indeed, controllers only deal with stdout and stderr, but not stdin. Please use another IPC


About VS, normally you should be able to attach the controller process from the VS interface when its running (at simulation step 1)


But to be honest I didn’t tried this since a while 😅

##### el 04/20/2019 11:48:49
do you know whats wrong? thats what i get when i Install the MSYS2 Dependencies everytime
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/569127330770059284/unknown.png)
%end


is the version of qt5 downloaded the problem?

##### ina 04/20/2019 15:20:08
I can do the debug, but I can't jump to function  or see variable's value.....


Does gdb works?(I am not used to it...)

##### el 04/20/2019 15:25:20
it says this        -->     error: failed retrieving file 'mingw-w64-x86\_64-qt5-5.10.1-2-any.pkg.tar.xz' fro                      m repo.msys2.org : transfer closed with 506632108 bytes remaining to read

##### Lukas\_ 04/20/2019 16:50:56
H iguys, how to i get time of simulation as a value and what format would ti have? ideal to me would be float. So like in 

printf("current time is %(what1)", time); 

and 

time = (what2);

##### Jiajun Wang 04/21/2019 06:33:47
I wonder if webots can simulate planar robots? `@Fabien Rohrer`


That is to say, how to set the planar constraints between robot body with the background/space? thx`@Fabien Rohrer`

##### Akash 04/21/2019 11:59:11
Is there any  kind of ray ( 'DistanceSensor' or 'Emitter')   that can be reflected back to the 'Receiver' (Infra red)' from a 'boundingObject' ?

##### Fabien Rohrer [Moderator] 04/21/2019 14:28:55
`@el` indeed. The qt version seems problematic. Probably because of an update of pacman. Could you open a Github issue with this?


`@ina` yes GDb works, please refer to [https://cyberbotics.com/doc/guide/debugging-c-cpp-controllers](https://cyberbotics.com/doc/guide/debugging-c-cpp-controllers)


`@Lukas_` please refer to wb\_robot\_get\_time() [https://www.cyberbotics.com/doc/reference/robot?tab=c++#wb\_robot\_get\_time](https://www.cyberbotics.com/doc/reference/robot?tab=c++#wb_robot_get_time)


It returns the simulated time as a double in seconds.


`@Jiajun Wang` not sure what you mean by a « planar robot ». A 2 wheeled robots on a planar ground?


`@Akash` distance sensors uses rays but do not ensure communication


Emitter/receiver can deal with occlusions. It’s more what you need. But not with reflexion.

##### Jiajun Wang 04/21/2019 15:31:20
`@Fabien Rohrer`

 A planar robot means that the robot\_body's DOF in y, roll & yaw are fixed. such that the robot can only move in the saggittal plane.  

like the Raibert's robot: hopper.


`@Fabien Rohrer`

 I mean a 2D bipedal robot.

##### Fabien Rohrer [Moderator] 04/21/2019 18:22:28
Ok I see. Basically Webots can simulate any robot which is made of a set of solids linked with joints (passive or motorized). To know how to do that, refer to our user guide tutorials. Once done, you could create a controller having access to the dynamics through the sensor API (gyro, inertial unit, etc)


So a hopper leg could be modeled as a sequence of rotational/linear motors.

##### redbagy 04/22/2019 08:42:50
Is there a way to know the time steps taken to execute a command?


like wb\_emitter\_send()


or can I assume that each action takes one time step for example?

##### Fabien Rohrer [Moderator] 04/22/2019 10:52:49
The simulated time is freezed in between two wb\_robot\_step() calls. Its these calls which let the simulation steps forward of a discrete amount of time.

##### redbagy 04/22/2019 10:54:01
Oh ok thank you!

##### Fabien Rohrer [Moderator] 04/22/2019 12:10:18
You’re welcome


`@el` I just reported your issue here: [https://github.com/omichel/webots/issues/376](https://github.com/omichel/webots/issues/376)

##### cctung 04/23/2019 14:57:13
Where to get these libraries in windows?  STATIC\_LIBS = -static -lpng -lz -ljpeg -ltiff -llzma -lzstd

##### Olivier Michel [Cyberbotics] 04/23/2019 15:00:02
They come from MSYS2.


If you followed the installation instructions, they should be there.


I am assuming you are recompiling Webots from the source, right?

##### cctung 04/23/2019 15:00:53
yes,

##### Olivier Michel [Cyberbotics] 04/23/2019 15:01:02
Did you follow these instructions: [https://github.com/omichel/webots/wiki/Windows-installation?](https://github.com/omichel/webots/wiki/Windows-installation?)

##### cctung 04/23/2019 15:01:16
yes


Seems a problem compiling 32 bit version of controller.dll

##### Olivier Michel [Cyberbotics] 04/23/2019 15:02:10
I see.

##### cctung 04/23/2019 15:02:23
I can't find 32 bit version of these libs.

##### Olivier Michel [Cyberbotics] 04/23/2019 15:02:39
Can you check if you have /mingw32/bin/linzstd.dll installed?


$ ls -l /mingw32/bin/libzstd.dll

##### cctung 04/23/2019 15:04:17
no such file

##### Olivier Michel [Cyberbotics] 04/23/2019 15:04:48
Ok, so can you install it?

##### cctung 04/23/2019 15:05:05
I try

##### Olivier Michel [Cyberbotics] 04/23/2019 15:05:13
pacman -S mingw-w64-i686-zstd

##### cctung 04/23/2019 15:06:16
installed ok!

##### Olivier Michel [Cyberbotics] 04/23/2019 15:07:30
So, does the libController links properly now?

##### cctung 04/23/2019 15:08:34
\# * libController *

\#

\# 'VISUAL\_STUDIO\_PATH' not set, skipping Controller.lib

\# linking Controller.dll and libController.a (32 bit)

D:/m64/mingw32/bin/../lib/gcc/i686-w64-mingw32/7.4.0/../../../../i686-w64-mingw32/bin/ld.exe: cannot find -lpng

D:/m64/mingw32/bin/../lib/gcc/i686-w64-mingw32/7.4.0/../../../../i686-w64-mingw32/bin/ld.exe: cannot find -ljpeg

D:/m64/mingw32/bin/../lib/gcc/i686-w64-mingw32/7.4.0/../../../../i686-w64-mingw32/bin/ld.exe: cannot find -ltiff

D:/m64/mingw32/bin/../lib/gcc/i686-w64-mingw32/7.4.0/../../../../i686-w64-mingw32/bin/ld.exe: cannot find -llzma

collect2.exe: error: ld returned 1 exit status

make[1]: *** [Makefile:145: ../../../msys64/mingw32/bin/Controller.dll] Error 1

make: *** [Makefile:108: webots\_target] Error 2

##### Olivier Michel [Cyberbotics] 04/23/2019 15:09:13
Do you have these libraries installed?

##### cctung 04/23/2019 15:09:42
Maybe no. (32 bit)

##### Olivier Michel [Cyberbotics] 04/23/2019 15:09:54
I am checking...


You should have /mingw32/lib/libpng.a for example.


If not, you can install from pacman -S mingw-w64-i686-libpng

##### cctung 04/23/2019 15:12:43
no. ok.

##### Olivier Michel [Cyberbotics] 04/23/2019 15:13:20
Normally, they should have been installed if you installed the optional packages.


With "msys64\_installer.sh --all"

##### cctung 04/23/2019 15:14:34
see. I can handle it.


Thank you  very much!

##### Olivier Michel [Cyberbotics] 04/23/2019 15:15:33
You are welcome.


I believe you installed gcc-32 bit, but not the other 32-bit dependencies of libController. Therefore the libController Makefile is trying to build the 32 bit version of libController but fails. Maybe we should also test for the availability of the 32 bit dependencies before attempting to build the 32 bit version of libController...

##### cctung 04/23/2019 15:27:04
yes

##### Olivier Michel [Cyberbotics] 04/23/2019 15:43:08
Done here: [https://github.com/omichel/webots/pull/383](https://github.com/omichel/webots/pull/383)


Can you review it and let me know if you believe it is correct?

##### cctung 04/24/2019 02:31:32
I believe it is correct!

##### Olivier Michel [Cyberbotics] 04/24/2019 06:26:46
Thanks!

##### 古了个鹏 04/24/2019 07:47:56
Hello,dear developers. I am controlling  a motor using :  motor[2]->setPosition(std::numeric\_limits<double>::infinity());

  motor[2]->setVelocity(0.2); but the motor moveing is not very smooth. I can clearly the velocity fluctuating. Is that correct?

##### Fabien Rohrer [Moderator] 04/24/2019 07:48:32
Hi


Yes this is the right way.


But it is affected by other parameters.


Principally, has the motor enough torque (force) to lift its weight?


To be sure of this, you could for example increase the RotationalMotor.maxTorque field: [https://www.cyberbotics.com/doc/reference/rotationalmotor](https://www.cyberbotics.com/doc/reference/rotationalmotor)

##### 古了个鹏 04/24/2019 07:50:41
I think it has enough torque. I set the torque into 100


Even I set it into 1000, it still cannot move smoothly~

##### Fabien Rohrer [Moderator] 04/24/2019 07:54:13
Could you just try to set it to 1000000 to be sure 😉 ?


(just the time to understand if it's the issue)

##### 古了个鹏 04/24/2019 07:55:58
it still moves step by step~


moves video
> **Attachment**: [rf\_leg.mp4](https://cdn.discordapp.com/attachments/565154703139405824/570518444412502016/rf_leg.mp4)

##### Fabien Rohrer [Moderator] 04/24/2019 07:57:13
ok, so forget this hypothesis.

##### 古了个鹏 04/24/2019 07:57:49
can you see the video I upload?

##### Fabien Rohrer [Moderator] 04/24/2019 07:57:54
yes

##### 古了个鹏 04/24/2019 07:58:01
Is that right?

##### Fabien Rohrer [Moderator] 04/24/2019 07:58:01
but I'm not sure to see the issue in the movie..


I have the feeling that the 2 velocities are constants.

##### 古了个鹏 04/24/2019 07:59:17
but in the simulation window, it moves step by step😂


Maybe it works in the right way.

##### Fabien Rohrer [Moderator] 04/24/2019 08:01:55
A movie is recording in real-time. In the 3D window, the time may fluctuate if some bottleneck is reached or if the computer is somehow saturated.

##### 古了个鹏 04/24/2019 08:03:37
Ok. Thanks~👌

##### Fabien Rohrer [Moderator] 04/24/2019 08:06:05
Do you use the "real-time" button? Could you check that your WorldInfo is set to something reasonable (like 8 or 16 for such robot)? You could also decrease OpenGL preferences, etc. => It's much more convenient to work in real-time and do in order that this is reached (speedometer is constantly reaching 1.0x without fluctuations)

##### 古了个鹏 04/24/2019 08:49:27
Yes, I use the real-time button. And the bacistimestep is 8.


The realtime speed, the simulation ratio is about 0.90 to 0.98x


I open some other example project. they can moves in very smoothly.


So I think there are something I do not set it up right


I meet this error: WARNING: Robot > DEF RFJoint HingeJoint > Solid > HingeJoint > RotationalMotor: too low requested position: -1 < 0


when I use c++ api setPosition


motor[1] ->setPosition(-1.0);


If I set it in a positive value, it works right.


How can I solve it?

##### Fabien Rohrer [Moderator] 04/24/2019 14:19:11
You control the motor in velocity mode, don't you?

##### 古了个鹏 04/24/2019 14:20:12
No, I don't define the working mode.



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/570615096217174037/unknown.png)
%end

##### Fabien Rohrer [Moderator] 04/24/2019 14:21:34
In your model, did you defined min/maxPosition or min/maxStop ?

##### 古了个鹏 04/24/2019 14:23:03
Yes, I define the maxPosition into 100


and the default minposition is 0.

##### Fabien Rohrer [Moderator] 04/24/2019 14:24:07
Ok, that's the issue. You defined the range from 0 to 100, and try to set a value of -1 😃


just change minPosition to -100 or so..

##### 古了个鹏 04/24/2019 14:24:59
OK, it works~ thank you. 😀

##### Fabien Rohrer [Moderator] 04/24/2019 14:25:06
great 😃

##### 古了个鹏 04/24/2019 14:30:04
And how can I make the simulation speed is the same as I planned? I planned 10001 datas in 10s. But when I read the planned data and setPosition into the motor. the simulation time is not the same as I planned.

##### Fabien Rohrer [Moderator] 04/24/2019 14:32:15
You should apply 1000 steps per second, so you should set WorldInfo.basicTimeStep to 1 (milliseconds). This should work precisely. To be sure, you can get the simulated time in your controller using the wb\_robot\_get\_time() function.


.. and your controller should call steps of 1 ms too: wb\_robot\_step(1)

##### 古了个鹏 04/24/2019 14:36:06
I write the controller into C++. Is there still the wb\_robot\_steo() like C?

##### Fabien Rohrer [Moderator] 04/24/2019 14:36:30
Yes, it's Robot.step(1) (it's the same function)


[https://cyberbotics.com/doc/reference/robot?tab=c++#wb\_robot\_step](https://cyberbotics.com/doc/reference/robot?tab=c++#wb_robot_step)

##### 古了个鹏 04/24/2019 14:37:27
👌

##### Fabien Rohrer [Moderator] 04/24/2019 14:38:14
It's generally a good idea to let match the controller step rate with WorldInfo.basicTimeStep. In most of my controllers, I'm using this pattern:


Robot.step((int) Robot.getBasicTimeStep())

##### 古了个鹏 04/24/2019 14:39:06
Ok, get it~thank you~

##### tilly 04/24/2019 15:25:09
hi, since updating to the 2019a version webots crashes when we use thymiosII (especially their groundsensors) is this a known bug?

##### Olivier Michel [Cyberbotics] 04/24/2019 15:25:45
Hi, no this is not a known bug.

##### tilly 04/24/2019 15:26:31
Are there any other bugs related to the thymio?

##### Olivier Michel [Cyberbotics] 04/24/2019 15:26:48
Not to my knowledge. Can you open an issue on [https://github.com/omichel/webots/issues](https://github.com/omichel/webots/issues) so that we can look into the problem?

##### tilly 04/24/2019 15:38:08
[https://github.com/omichel/webots/issues/386](https://github.com/omichel/webots/issues/386) do u need further information


?

##### Olivier Michel [Cyberbotics] 04/24/2019 15:40:33
Please write a clear description in the text (edit) instead of the default template, so that we can reproduce the problem. You may also upload your world file and controller files if needed.


If we don't get enough information to be able to reproduce the problem, we won't be able to investigate it.


Don't put a too long title, but put more information in the first comment (edit it).

##### tilly 04/24/2019 15:42:04
upsi im new to git hub sry

##### Olivier Michel [Cyberbotics] 04/24/2019 15:42:23

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/570635659115495434/unknown.png)
%end

##### tilly 04/24/2019 15:42:36
i found it


updated the issue if u need anything else please ask

##### Olivier Michel [Cyberbotics] 04/24/2019 16:05:09
That looks good. Thank you.

##### ditorom1984 04/25/2019 13:35:31
Hi all. I have a problem with Webots 8.6.2. I have a license purchased from Webots 8.6.2 PRO but need internet connection. For some academic tests, I need this version to work without internet connection. how can I do it?

##### Fabien Rohrer [Moderator] 04/25/2019 13:35:58
Please contact info@cyberbotics.com explaining your issue and giving your license ID. We will see what we can do.

##### el\_samu\_el 04/25/2019 13:51:59
Hello people! Does anybody know how run multiple robots or webots instances in parallel to speed to the learning process (for a reinforcement learning algorithm)?  What is the best way to do that in your experience?


I found this old thread, but they were talking about the PRO version (which doesn't exist anymore in my knowledge)


[https://www.cyberbotics.com/forum?message=5060](https://www.cyberbotics.com/forum?message=5060)

##### Fabien Rohrer [Moderator] 04/25/2019 13:53:58
Hi


Indeed, Webots PRO license doesn't exist anymore, Webots is now free and open-source: this is a good news to run as many Webots instances as you can 😉


I like the idea to have a script using Webots as a slave and running one Webots instance per experiment.


This is on the responsability of this script to create deal with parallelism.


This many advantages, this is very easy to setup.


The main drawback is that launching Webots takes time; this offset may be annoying.

##### el\_samu\_el 04/25/2019 13:57:58
I like that idea too, it is just not clear to me how the CPU ressources  are used in this way. And because there would be around 6-12 instances in parallel opening and closing webots GUIs every second.


Yes this offset is what I am worried about.


A single experiment runs in let's say  0.1s in fast forward mode.

##### Fabien Rohrer [Moderator] 04/25/2019 13:59:29
The CPU/GPU consumption depends a lot on the simulation.


Before fearing the offset to run Webots, you could give a try to optimize your simulation 😉


A simple test running a simulation sample on multiple instances may give you answers about your CPU load.


To avoid the offset, there is another approach:


to run several Webots slaves "forever", and the main script communicate with them (using any IPC). Webots Supervisor API allows to load another world, reload a world, etc.


A bit more difficult to setup, but it's more efficient in your case.


I hope this will help you.

##### el\_samu\_el 04/25/2019 14:13:00
Yeah It does, thanks! But how can the main script reload the world and read data from the supervisor controller? I have no experience setting up IPCs. I'll see if anybody else did something similar before, otherwise I'll try to figure out how to do that. Any tips are welcomes 😃

##### Fabien Rohrer [Moderator] 04/25/2019 14:14:13
A simple approach is the following:


... hum just a question: do you need to change the world, or it's just a matter of training a controller in the same world?

##### el\_samu\_el 04/25/2019 14:16:49
later on I will change the world, but for the most iterations it is the same world


it is about locomotion learning

##### Fabien Rohrer [Moderator] 04/25/2019 14:18:03
and in which language? Python?

##### el\_samu\_el 04/25/2019 14:18:49
Python yes. There will be a curriculum in which the terrain on which the robot walk becomes more challenging.


But on a single terrain (world) the learner needs around 1000-5000 iterations.


one iteration corresponds to walking until it falls, which is in real time from around 0.5 to 5 seconds

##### Fabien Rohrer [Moderator] 04/25/2019 14:27:57
A good approach would be that your main script starts an IPC server, and several instances of Webots on a single world containing a Supervisor listenening as a client on this IPC. The IPC can be a local TCPIP server, this is easy to setup in Python. There are many modules to do similar things. The main script may send events to the supervisor. Mainly it's about to evaluate some fitness, with data (neuron weights or so). In this case, the supervisor may reset the simulation typically by reseting the object position, or reloading the entire world (it's the purpose of a supervisor), and evaluate the fitness, and give the result back to the main script through the IPC.

##### el\_samu\_el 04/25/2019 14:30:45
That sounds like something I was looking for. I will take some time to set this up and if it works I'll share this approach on github.


Thanks!

##### Fabien Rohrer [Moderator] 04/25/2019 14:31:45
you're welcome. Sharing your approach is a good way to thanks us ^^

##### Thelm 04/26/2019 13:34:45
Hi, I'm trying to model a magnetic sensor in webots for e-puck, and I just realized that the output is only a vector whereas the magnetic sensor of e-pucks output a value in µT for each axis. Is a lookup table sufficient to convert the compass into a magnetometer? If not, how can I manage to do this in the proto file?

##### Olivier Michel [Cyberbotics] 04/26/2019 13:37:35
Hi, yes setting the lookupTable of the Compass node should be sufficient to calibrate your compass model in that case.

##### Thelm 04/26/2019 13:40:55
So as I need the value to be between -4912 and 4912, will the output be a 3D vector with a fixed length, or the 3 the output are independent?

##### Olivier Michel [Cyberbotics] 04/26/2019 13:41:42
The 3 output are independent.


They will match what you define in the lookup table.


Typically, your lookup table may look like this:


lookupTable [-1 -4912 0.1


1 4912 0.1 ]

##### Thelm 04/26/2019 13:50:08
so if I do this, it will measure a magnetic field of 4912µT in the webots world right?

##### Olivier Michel [Cyberbotics] 04/26/2019 13:52:37
I think so.


However in Webots, the Compass node gives a unit vector which is the direction of the north.


It's not a value expressed in µT.

##### Thelm 04/26/2019 13:56:30
I think for the lookup table it will be better to set [ -4912 -4912 0.1   4912 4912 0.1 ] that way, when I'll connect a real e-puck the measured values will be the good ones

##### Olivier Michel [Cyberbotics] 04/26/2019 13:57:04
Yes, you certainly need to calibrate it with a real e-puck2 robot.

##### Thelm 04/26/2019 13:57:58
Thank you, I'll wait to have a robot to try then


have a nice day 😄

##### Olivier Michel [Cyberbotics] 04/26/2019 14:02:42
Thank you. Have a nice day as well.

##### rittwiq 04/28/2019 11:11:42
Hey! Does Webot support controllers in Matlab for vehicles?

##### Fabien Rohrer [Moderator] 04/28/2019 11:56:29
Hi, unfortunately no. We created wrappers for C++, Python, Java and ROS, but not for Matlab yet.


I think that it should be fairly easy to do. Do you have  any resource for this development?

##### rittwiq 04/28/2019 12:46:04
No.... I was thinking of integrating with simulink... But if I was to use python or C++ do I need to make changes in my make file for vehicles?

##### Ahmad Ali 04/29/2019 01:54:04
Hi, I am trying to use a  C++ based external library in webots in Ubuntu and I got this error.

error: #error This file requires compiler and library support for the ISO C++ 2011 standard. This support must be enabled with the -std=c++11 or -std=gnu++11 compiler options.

how can I use support for the ISO CPP 2011 standard?

Regards

##### Fabien Rohrer [Moderator] 04/29/2019 04:06:44
`@rittwiq` it would be helpful to have a Matlab wrapper to start simulink. There are maybe other possibilities about to start Simulink directly.


To use Python and the vehicle library, you need to have a runtime.ini indicating to the controller where to find the vehicle library. You could copy it from any python example using the vehicle library.


For C++, you also need a custom Makefile and runtime.ini. There too, please simply refer to a released example.


`@Ahmad Ali` did you simply tried to add this option in your Makefile?


CFLAGS += -stdc++11

##### Ahmad Ali 04/29/2019 06:41:06
I tried "CFLAGS += -stdc++11"  in the Makefile, but I am still getting this error message,

g++: error: unrecognized command line option ‘-stdc++11’

/usr/local/webots/resources/Makefile.include:690: recipe for target 'build/release/UR5e\_SOMA.o' failed

##### Fabien Rohrer [Moderator] 04/29/2019 06:44:23
This is weird; what version of Ubuntu and g++ are you using?


An equal character is missing 😃


CFLAGS += -std=c++11

##### Shounak 04/29/2019 06:51:13
what is the python versuion  camera recognition ?

##### Ahmad Ali 04/29/2019 06:51:23
yes it is working, thanks Mr. Fabien

##### Shounak 04/29/2019 06:52:07
const WbCameraRecognitionObject *objects = wb\_camera\_recognition\_get\_objects(camera);

##### Stefania Pedrazzi [Cyberbotics] 04/29/2019 06:53:25
`@Shounak`  the camera recognition API is available both for Python 2.7 and Python 3

##### Shounak 04/29/2019 06:53:46
yes. I been trying to run it


its not working

##### Stefania Pedrazzi [Cyberbotics] 04/29/2019 06:54:30
do you get any error?

##### Shounak 04/29/2019 06:54:39
yeah


I can't read the  position value


Can you just send a the line I wrote earlier in python/


WbCameraRecognitionObject *objects = wb\_camera\_recognition\_get\_objects(camera);


sorry neew to the image recognition

##### Stefania Pedrazzi [Cyberbotics] 04/29/2019 06:57:19
here is the Python documentation: [https://www.cyberbotics.com/doc/reference/camera?tab=python](https://www.cyberbotics.com/doc/reference/camera?tab=python)


the Python call will be:


objects = camera.getRecognitionObjects()

##### Shounak 04/29/2019 06:58:42
[<controller.CameraRecognitionObject; proxy of <Swig Object of type 'webots::CameraRecognitionObject *' at 0x00000000039AE540> >, <controller.CameraRecognitionObject; proxy of <Swig Object of type 'webots::CameraRecognitionObject *' at 0x00000000039AE510> >]

##### Stefania Pedrazzi [Cyberbotics] 04/29/2019 06:58:42
note that a Recognition nodes needs to be present in the Camera node and that you first have to enable the recognition

##### Shounak 04/29/2019 06:58:55
I have done that


I need to read the position.

##### Stefania Pedrazzi [Cyberbotics] 04/29/2019 07:00:43
I will check it works in my example and let you know. But it could take some time

##### Shounak 04/29/2019 07:00:56
sure

##### Ahmad Ali 04/29/2019 07:08:07
I included this in Makefile:

include = -I "/usr/local/include" 

include = -I "/usr/include/eigen3/Eigen" 



But still I am getting this error:



In file included from /usr/local/include/ompl/base/StateSpace.h:43:0,

                 from /usr/local/include/ompl/base/SpaceInformation.h:43,

                 from UR5e\_SOMA.cpp:7:

/usr/local/include/ompl/base/ProjectionEvaluator.h:49:22: fatal error: Eigen/Core: No such file or directory

compilation terminated.

/usr/local/webots/resources/Makefile.include:690: recipe for target 'build/release/UR5e\_SOMA.o' failed

make: *** [build/release/UR5e\_SOMA.o] Error 1

##### Shounak 04/29/2019 07:08:54
Got it. It's working


Thanks man

##### Fabien Rohrer [Moderator] 04/29/2019 08:08:33
`@Ahmad Ali` It seems that some include files cannot be found. Your directories are certainly not well defined. Where is the "Core" file installed? You should try:


include = -I "/usr/include/eigen3

##### Stefania Pedrazzi [Cyberbotics] 04/29/2019 08:11:56
`@Ahmad Ali`  it also seems that you are overwriting the "include" variable instead of adding multiple paths. You should use "+=" instead of "="

##### Ahmad Ali 04/29/2019 08:28:23
Thanks Stefania and Fabien,

Path of the Core file is "/usr/include/eigen3/Eigen/Core.h"



I tried  "include = -I "/usr/include/eigen3" ,  but the same error appears.


when I run the source file in Ubuntu terminal, then "-I/usr/include/eigen3" is working fine. But when I use code in Webots, then it gives the error

##### Olivier Michel [Cyberbotics] 04/29/2019 08:34:05
Not sure if it makes a difference, but "include" should be written in capital letters, e.g., INCLUDE = -I /usr/include/eigen3

##### Ahmad Ali 04/29/2019 09:16:51
Thanks, its working now.

##### Olivier Michel [Cyberbotics] 04/29/2019 09:17:18
So, capital letters were needed, right?

##### Ahmad Ali 04/29/2019 09:18:41
yes, exactly.

##### Olivier Michel [Cyberbotics] 04/29/2019 09:18:57
Thanks for the feedback.

##### Shounak 04/29/2019 11:04:42
need help with camera recognition


The camera recognition is not working with Box , stairs


Is there any toolbox or code I am missing


I am trying to detect stair steps

##### Stefania Pedrazzi [Cyberbotics] 04/29/2019 11:14:40
only Solid nodes for which the 'recognitionColors' field is not empty can be detected. Please refer to the Solid and Camera documentation


[https://www.cyberbotics.com/doc/reference/solid](https://www.cyberbotics.com/doc/reference/solid)


[https://www.cyberbotics.com/doc/reference/recognition](https://www.cyberbotics.com/doc/reference/recognition)

##### Shounak 04/29/2019 11:22:30
Thank you. It works excellent

##### Stefania Pedrazzi [Cyberbotics] 04/29/2019 11:22:58
good!

##### Ahmad Ali 04/29/2019 13:33:39
Are there any Control tutorials using C++ in webots?

##### Fabien Rohrer [Moderator] 04/29/2019 13:34:43
Could you take a look at this page? [https://cyberbotics.com/doc/guide/cpp-java-python](https://cyberbotics.com/doc/guide/cpp-java-python)


I think it explains well how to use the C++ API.


We focus more on the C language in the tutorial. Migrating it to C++ should be almost direct.


[https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials)

##### Ahmad Ali 04/29/2019 13:38:36
Yes, it will help. In demo projects in Webots, all are C based. Thanks and Regards,

##### kim 04/29/2019 14:18:01
Can I know control the nao robot using python?

##### Olivier Michel [Cyberbotics] 04/29/2019 14:18:14
Hi Kim,

##### kim 04/29/2019 14:18:17
Hi

##### Olivier Michel [Cyberbotics] 04/29/2019 14:18:18
Yes of course.

##### kim 04/29/2019 14:18:31
can I know?

##### Olivier Michel [Cyberbotics] 04/29/2019 14:18:48
There are some examples I an searching for you...

##### kim 04/29/2019 14:18:48
I try to search so long time


Oh thank you


Oh


I wonder about using nao in webots

##### Olivier Michel [Cyberbotics] 04/29/2019 14:20:08
Look into the projects/samples/robotbenchmark/humanoid\_sprint/controllers/sprinter

##### kim 04/29/2019 14:20:13
I am doning Project for lab about reinforcement learning

##### Olivier Michel [Cyberbotics] 04/29/2019 14:20:24
There is a simple NAO controller in Python.


"sprinter.py"

##### kim 04/29/2019 14:21:01
have you tried to control using kinecT?


kinect

##### Olivier Michel [Cyberbotics] 04/29/2019 14:21:17
The corresponding world file is the projects/samples/robotbenchmark/humanoid\_sprint/worlds/humanoid\_sprint.wbt


Not personnaly, but I know others did.

##### kim 04/29/2019 14:21:54
really?


do you know how to use kinect ?


for nao in webots?

##### Olivier Michel [Cyberbotics] 04/29/2019 14:22:54
See [https://www.youtube.com/watch?v=QNtUm9SOGhU](https://www.youtube.com/watch?v=QNtUm9SOGhU)

##### kim 04/29/2019 14:23:02
Yeah


I saw


but I don't know how to do that guy

##### Olivier Michel [Cyberbotics] 04/29/2019 14:23:37
This one also: [https://www.youtube.com/watch?v=Ze5w1GUAigY](https://www.youtube.com/watch?v=Ze5w1GUAigY)


I don't know either, but it must be doable as several people did it.


If you want, we can implement it for you, we have some experience with Kinect and know Webots very well.


See also this one: [https://www.youtube.com/watch?v=KKmNfsFk1ho](https://www.youtube.com/watch?v=KKmNfsFk1ho)

##### kim 04/29/2019 14:25:35
you mean that you can do it ?

##### Olivier Michel [Cyberbotics] 04/29/2019 14:25:42
Yes.

##### kim 04/29/2019 14:25:53
really? do you have kinect?

##### Olivier Michel [Cyberbotics] 04/29/2019 14:25:58
Yes.

##### kim 04/29/2019 14:26:00
Oh


my god


please


help me !

##### Olivier Michel [Cyberbotics] 04/29/2019 14:26:47
Of course, we won't do it free of charge, we need to evaluate how much time it will take us to do it and then we can tell you the price.

##### kim 04/29/2019 14:26:48
I want to know to control nao in webots using kinect


umm


ok

##### Olivier Michel [Cyberbotics] 04/29/2019 14:28:33
If you are interested, please write us at sales@cyberbotics.com and describe exactly what you want us to develop for you. Then, we will send you an offer for that. We can give you all the source code and teach you how it works.

##### kim 04/29/2019 14:29:58
okay. I wonder how much money do I pay  ?  can you give me expectation

##### Olivier Michel [Cyberbotics] 04/29/2019 14:32:35
If the resulting code is released under an open source license, we will charge you CHF 1000 per day of development (currently slightly less than USD 1000), if you order one week or one month, you will get some discount of that price.

##### kim 04/29/2019 14:35:23
after I think about that I will send the message

##### Olivier Michel [Cyberbotics] 04/29/2019 14:35:37
All right.

##### kim 04/29/2019 14:35:46
Thanks!

##### Olivier Michel [Cyberbotics] 04/29/2019 14:35:51
You are welcome.

##### kim 04/29/2019 14:40:39
Do we use the naoqi in webots ?

##### Olivier Michel [Cyberbotics] 04/29/2019 14:40:57
Yes, this is possible but not recommended.


Because SoftBank robotics is not maintaining the naoqi simulator-sdk and there are some bugs and limitation that won't get fixed.

##### kim 04/29/2019 14:42:42
so when we control the nao robot in real  we don't use the naoqi ?


from naoqi import ALProxy

tts = ALProxy("ALTextToSpeech", "<IP of your robot>", 9559)

tts.say("Hello, world!")


this code

##### Olivier Michel [Cyberbotics] 04/29/2019 14:43:34
Yes, but that is the native version of naoqi. The simulated version of naoqi relies on the simulator-sdk from SoftBank Robotics.

##### kim 04/29/2019 14:45:32
how do i get the simulator-sdk from SoftBank Robotics?


do I have to buy?

##### Olivier Michel [Cyberbotics] 04/29/2019 14:46:00
No, it's free.


Follow the instructions here: [https://github.com/omichel/naoqisim](https://github.com/omichel/naoqisim)


(basically, it gets installed when you install naoqisim for Webots)

##### kim 04/29/2019 14:49:00
I installed the naoqi in this web site


[https://community.ald.softbankrobotics.com/en/resources/software/language/en-gb](https://community.ald.softbankrobotics.com/en/resources/software/language/en-gb)


do you knwo?


know about this website?

##### Olivier Michel [Cyberbotics] 04/29/2019 14:51:26
Yes, but it doesn't include the simulator-sdk.


The simulator-sdk was basically abandoned by SoftBank, this is why you cannot download it from their official site.


It is not maintained any more.


Hence this is why we don't recommend using it.


Instead you can program the simulated NAO robot in Webots directly in Python using the Webots API.

##### kim 04/29/2019 14:53:41
When I use the kinect also?

##### Olivier Michel [Cyberbotics] 04/29/2019 14:53:52
Yes.

##### kim 04/29/2019 14:54:47
have you been to use the webcam in webots ?

##### Olivier Michel [Cyberbotics] 04/29/2019 15:13:05
Yes.

##### kim 04/29/2019 15:20:15
can i know how to do?

##### Olivier Michel [Cyberbotics] 04/29/2019 15:21:29
Do you mean the camera of the real NAO robot?


Or the camera of the simulated NAO robot?


Or a standard computer webcam?

##### kim 04/29/2019 15:21:58
standard computer webcam

##### Olivier Michel [Cyberbotics] 04/29/2019 15:22:57
In that case, it's a standard programming question.


We can do it for you if needed.

##### kim 04/29/2019 15:23:34
should i pay for that?

##### Olivier Michel [Cyberbotics] 04/29/2019 15:23:55
Yep.

##### kim 04/30/2019 00:32:54
hello

##### Ahmad Ali 04/30/2019 08:06:11
Hi,

I am doing motion planning using OMPL (C++ based motion planning library) with Webots. But OMPL does not have its collision checking feature. In Webots, we can check collision using physics plug-in during the simulation. I am wandering if I can use collision checking feature of webots in motion planning (which will be outside the simulation loop).

##### Olivier Michel [Cyberbotics] 04/30/2019 08:07:11
Hi,


You can probably write a physics plugin in Webots that will send collision information to your robot controller using the Emitter / Receiver system. This information could then be used by OMPL which I guess is linked with your robot controller.

##### Ahmad Ali 04/30/2019 08:22:16
in fact , the planning part is offline, that is why,  the code for planning is in controller but outside the simulation loop with time\_step. Is it possible to call collision detection  function directly in Controller?

##### Olivier Michel [Cyberbotics] 04/30/2019 08:26:39
No, that's not possible. Instead you should actually run the simulation possibly with another controller that will be executed before your main controller and that would use run the simulation to perform only collision detection in the physics plug-in.

##### Ahmad Ali 04/30/2019 08:28:20
Thanks Mr Olivier for clearing my confusion.

##### Olivier Michel [Cyberbotics] 04/30/2019 08:28:29
You are welcome.

##### Ahmad Ali 04/30/2019 08:29:04
I will try to use some other collision checking library for collision checking in motion planning.

##### Olivier Michel [Cyberbotics] 04/30/2019 08:29:14
OK.

##### Ahmad Ali 04/30/2019 16:03:19
Hi, is there any way to get the world file as a 3D mesh model?

## May

##### Olivier Michel [Cyberbotics] 05/01/2019 06:05:27
Hi,


Yes, you can export it as VRML97.

##### 之之 05/01/2019 06:23:03
Hi, Why does my webot pause the simulation, but the memory used has been rising

##### Olivier Michel [Cyberbotics] 05/01/2019 06:24:31
Did you identify that it is the Webots process that eats up memory? It might be another program, including a Webots controller program.

##### 之之 05/01/2019 06:25:23
Thank you, I will confirm it.


But I don't have this problem with the same program on another computer.

##### Olivier Michel [Cyberbotics] 05/01/2019 06:37:13
On which OS are you running this?

##### 之之 05/01/2019 06:37:30
window10


Webot version 8.5.4

##### Olivier Michel [Cyberbotics] 05/01/2019 06:38:06
Is this memory leak specific to a certain simulation?


Oops... This version is not any more supported. I would recommend you to upgrade to Webots R2019a revision 1 as we won't fix version 8.5.4 even if you find a memory leak in there.

##### 之之 05/01/2019 06:39:40
May be a setting problem?


Okay, thank you

##### Olivier Michel [Cyberbotics] 05/01/2019 06:40:25
Yes, maybe... You are welcome.

##### nisuunn 05/01/2019 13:59:47
Hi, I have a question regarding elevationGrids.

##### Olivier Michel [Cyberbotics] 05/01/2019 14:00:07
Hi,

##### nisuunn 05/01/2019 14:00:35
I have an elevationGrid which I have defined and it works fine regarding collisions.

##### Olivier Michel [Cyberbotics] 05/01/2019 14:00:56
All right.

##### nisuunn 05/01/2019 14:01:02
I also have a supervisor with which I change the parameters of the elevation grid


which change the height values


once activated,


visually the height change can be seen


however, collision does not seem to work as desired,


when some other object e.g. robot collides with the new elevationgrid, the collision seems to still be working with the old version of the elevationgrid

##### Olivier Michel [Cyberbotics] 05/01/2019 14:02:34
I see.


That is likely a bug...

##### nisuunn 05/01/2019 14:02:46
I also noticed that before making the change, if I change the position of the elevationgrid, then the new elevationgrid with new heights works fine with respect to collision


I see


Aside from slightly changing the position of the elevation grid each time, is there any other way to get around this issue?

##### Olivier Michel [Cyberbotics] 05/01/2019 14:03:44
Yes, fixing the bug inside Webots.


Can you open an issue at [https://github.com/omichel/webots/issues](https://github.com/omichel/webots/issues) and describe this bug precisely so that we can try to reproduce it?

##### nisuunn 05/01/2019 14:04:32
I will do so

##### Olivier Michel [Cyberbotics] 05/01/2019 14:04:36
Thank you.

##### nisuunn 05/01/2019 14:04:46
I have another question regarding robot modelling

##### Olivier Michel [Cyberbotics] 05/01/2019 14:04:51
Yes.

##### nisuunn 05/01/2019 14:04:59
the robot's boundingobject parameter


Right now I have left it undefined, and instead I have defined the bounding object of all the individual parts of the robot (such as foot, body, etc.)


Can this be a problem?

##### Olivier Michel [Cyberbotics] 05/01/2019 14:06:58
It shouldn't be a problem. However, it is better to define a boundingObject for the robot. This bounding object usually represents the body of the robot (often the place where the CPU of the robot is) from which the other joints are connected to.

##### nisuunn 05/01/2019 14:07:31
I see, so in the case of a quadruped robot, should the main body torso be the boundingObject of the robot?

##### Olivier Michel [Cyberbotics] 05/01/2019 14:07:39
Yes.

##### nisuunn 05/01/2019 14:07:46
Thank you!

##### Olivier Michel [Cyberbotics] 05/01/2019 14:07:52
You are welcome.

##### sai 05/01/2019 19:11:58
Hello, is it possible to make webots vehicles start with some specific initial velocity instead of zero?

##### Fabien Rohrer [Moderator] 05/01/2019 19:13:31
I think this is possible thanks to the hidden fields.


Just save a world while vehicles are running, and observe how « hiddenField » are stored in the .wbt file.

##### sai 05/01/2019 19:15:19
Ohh thats's a good idea. I can try it out thanks!!

##### Fabien Rohrer [Moderator] 05/01/2019 19:15:42
Here is the doc: [https://cyberbotics.com/doc/reference/proto-hidden-fields](https://cyberbotics.com/doc/reference/proto-hidden-fields)


These parameters may be difficult to write manually.

##### sai 05/01/2019 19:20:29
Yeah that looks like it... Is there any source where I can understand how these parameters encode some specific velocity? Without that I could only try to check a couple of webots saved worlds with different velocities...

##### Fabien Rohrer [Moderator] 05/01/2019 19:26:44
Hidden fields are dealt in the WbSolid class: [https://github.com/omichel/webots/blob/931d04675d7dc69aa0599c4d525795bbbdd6193f/src/webots/nodes/WbSolid.cpp#L157](https://github.com/omichel/webots/blob/931d04675d7dc69aa0599c4d525795bbbdd6193f/src/webots/nodes/WbSolid.cpp#L157)

##### sai 05/01/2019 19:27:25
Thanks that could be helpful


Also, I have a question regarding the acceleration value being assigned infinity for a vehicle


What does it mean in a simulation? Does it make the vehicle instantaneously reach a specific velocity in cruise mode?

##### Fabien Rohrer [Moderator] 05/01/2019 19:35:57
The vehicles are supposed to be controlled using the libvehicle only which actually manage the acceleration

##### Sajjad 05/01/2019 20:41:40
hi, how I can my first project with Nao robot in webots?


Hi, I want to connect Webots to Choregraphe, how know about it? actually, ,I can't find nao\_qi !

##### sai 05/01/2019 23:45:22
Hello, does webots support thrustmaster tmx driving wheel. I want to control the vehicle using this thrustmaster wheel connected to pc

##### Olivier Michel [Cyberbotics] 05/02/2019 06:25:52
`@Sajjad` , you need to install naoqisim in addition to Webots. Installation instructions are here: [https://github.com/omichel/naoqisim](https://github.com/omichel/naoqisim)


`@sai`, Webots works with the Logitech driving wheel and Fanatec driving wheels. We never tested the Thurstmaster, but it is likely to work as well. If not, it shouldn't be difficult to adapt the buttons and axis references in the source code of Webots.

##### David Mansolino [Cyberbotics] 05/02/2019 07:19:06
To be completely correct about the thrustmaster tmx driving wheel, the buttons and axis references are handled on the controller side, they can easily be changed in the configuration file (*.ini): [https://github.com/omichel/webots/tree/master/projects/vehicles/controllers/racing\_wheel](https://github.com/omichel/webots/tree/master/projects/vehicles/controllers/racing_wheel)

##### AntiSquid 05/02/2019 11:08:47
greetings, how do i add boost to webots?

##### Fabien Rohrer [Moderator] 05/02/2019 11:35:00
Hi, Boost can be added as any other third party library: [https://www.cyberbotics.com/doc/guide/using-webots-makefiles#adding-an-external-library-ccp](https://www.cyberbotics.com/doc/guide/using-webots-makefiles#adding-an-external-library-ccp)

##### kim 05/02/2019 15:19:56
hi


I wonder nao robot can be controlled using python3

##### Fabien Rohrer [Moderator] 05/02/2019 15:20:37
Hi, yes it can


Webots supports Python 3 on every OS. The Webots API can be used to control the Nao.

##### kim 05/02/2019 15:22:08
Is the Webots API  installed when I install the weobots?

##### Fabien Rohrer [Moderator] 05/02/2019 15:22:22
Yes


You should be able to test this example out-of-the-box: [https://github.com/omichel/webots/blob/master/projects/robots/softbank/nao/controllers/nao\_demo\_python/nao\_demo\_python.py](https://github.com/omichel/webots/blob/master/projects/robots/softbank/nao/controllers/nao_demo_python/nao_demo_python.py)


Please refer to this doc to run Python controllers: [https://cyberbotics.com/doc/guide/using-python](https://cyberbotics.com/doc/guide/using-python)


(basically, you simply need to install Python 3 correctly)

##### kim 05/02/2019 15:25:10
What is the first code for?

##### Fabien Rohrer [Moderator] 05/02/2019 15:26:12
It's a demo controller for the Nao written in Python (2 or 3). A good starting point...

##### kim 05/02/2019 15:28:55
What is the Webots API exactly?

##### Fabien Rohrer [Moderator] 05/02/2019 15:29:58
It's simply the library (set of funtions) used to control the virtual robots: [https://cyberbotics.com/doc/reference/nodes-and-api-functions](https://cyberbotics.com/doc/reference/nodes-and-api-functions)

##### kim 05/02/2019 15:31:02
So you mean that we can control the Nao robot using Webots API?

##### Fabien Rohrer [Moderator] 05/02/2019 15:32:03
Yes, as in the controller shown above.

##### kim 05/02/2019 15:32:17
control the robot in reality  not virtual environment


is it right?

##### Fabien Rohrer [Moderator] 05/02/2019 15:32:34
No, in reality, this is another story.

##### kim 05/02/2019 15:33:11
So do you know that we can control the Nao robot in reality using python?

##### Fabien Rohrer [Moderator] 05/02/2019 15:33:55
You should refer to the softbank documentation for this. I think yes. Not sure at all about Python 3.


At some point, we were developing a system with softbank to control both the real and the virtual robot, but they dropped it:


[https://github.com/omichel/naoqisim](https://github.com/omichel/naoqisim)


You can give this a try, but we do not support this anymore.

##### kim 05/02/2019 15:35:26
ok thanks !

##### Fabien Rohrer [Moderator] 05/02/2019 15:35:44
you're welcome, good luck with this 😃

##### kim 05/02/2019 15:36:01
As I know we can control the nao robot in reality using python sdk naoqi


have you tried about that?

##### Fabien Rohrer [Moderator] 05/02/2019 15:36:42
Yes, we tried this.

##### kim 05/02/2019 15:36:43
using python 2.7


did you use python 2.7?

##### Fabien Rohrer [Moderator] 05/02/2019 15:37:08
yes, 2.7

##### kim 05/02/2019 15:38:10
Ok Thanks for the information


Oh


Have you been to  transfer human skeleton data  to robot?

##### Fabien Rohrer [Moderator] 05/02/2019 16:00:27
I never tried this ^^

##### DANNYWEBOTS 05/02/2019 16:06:34
hellow, hellow, I need activate the version test free for 30 days, Please. Urgent

##### David Mansolino [Cyberbotics] 05/02/2019 16:06:52
Hi

##### DANNYWEBOTS 05/02/2019 16:07:06
hi

##### David Mansolino [Cyberbotics] 05/02/2019 16:07:10
Since December 2018 Webots is completely free and open-source.



You can download Webots here: [https://www.cyberbotics.com/download](https://www.cyberbotics.com/download) and check out the source code here: [https://github.com/omichel/webots](https://github.com/omichel/webots)



We provide paid user support, training and consulting for users who need to quickly develop high-quality Webots simulations, see: [https://www.cyberbotics.com/buy](https://www.cyberbotics.com/buy)

##### DANNYWEBOTS 05/02/2019 16:08:00
Thank you

##### David Mansolino [Cyberbotics] 05/02/2019 16:08:06
You're welcome

##### DANNYWEBOTS 05/02/2019 16:09:28
Now, I need to install an older version, because my computer does not have an advanced graphics card, I need to install version 8.6.2 for windows, I may be able to use that version in open source


It is Posiible?

##### David Mansolino [Cyberbotics] 05/02/2019 16:10:03
No I am sorry but only version from R2019a are free and open source

##### DANNYWEBOTS 05/02/2019 16:10:18
I'm sorry my English is not good

##### David Mansolino [Cyberbotics] 05/02/2019 16:10:24
But in any case, even with older version of Webots we do recommend to use a decent GPU


What is your GPU ?

##### DANNYWEBOTS 05/02/2019 16:11:24
My version GPU is GeForce GT 525 M


I do not understand why the last version does not run


What can I do to get the latest version?

##### David Mansolino [Cyberbotics] 05/02/2019 16:14:27
Have you tried the safe mode?



[https://cyberbotics.com/doc/guide/starting-webots#safe-mode](https://cyberbotics.com/doc/guide/starting-webots#safe-mode)

##### AntiSquid 05/02/2019 18:31:04
webots gets stuck at "finalizing nodes" after i restarted my pc


tried to restart the program multiple times it keeps saying "not responding"


ok it works with safe mode

i still find it strange it worked flawlessly the very first time, but then kept crashing afterwards

##### Chin 05/02/2019 19:07:38
Hi, are there any step-by-step tutorials on Robotic Arm Simulations and how to write controllers for them?

##### David Mansolino [Cyberbotics] 05/03/2019 06:30:52
`@AntiSquid`, what is your GPU ? Have you been able to identify which OpenGL preference ([https://cyberbotics.com/doc/guide/preferences#opengl](https://cyberbotics.com/doc/guide/preferences#opengl))  cause the crash ?


`@Chin`  unfortunately not yet, however you can find generic step-by-step tutorials here: [https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials).

In these tutorials you will learn (amongst other) how to control motors which is what you need for controlling Robotic Arms.

By the way, do you have a specific robotic arm in mind?

##### AntiSquid 05/03/2019 11:43:52
`@David Mansolino` no idea what caused the crash. I am not sure it's the GPU (Intel® HD Graphics 4400)

i reset all the world settings and it worked again, there seems to be an issue with webots (for me) when i try to use physics, but it might be the physics dll i use is bugged

##### David Mansolino [Cyberbotics] 05/03/2019 12:13:59
That's indeed higly possible, has the physics dll is loaded by Webots directly, if there is a crash in the physics plugin, Webots will crash as well.

##### Thelm 05/03/2019 12:16:04
Hi, would it be possible to make multiple e-pucks communicating with each other (e.g. by WiFi or Bluetooth) with cross compliation?


I will use e-pucks v2

##### David Mansolino [Cyberbotics] 05/03/2019 12:17:15
Hi, this is not supported out of the box, if you are using remote-control you might use emitter-receiver. But with remote-control you will need to implement this by yourself.

##### Thelm 05/03/2019 12:27:41
So for now I have to leave webots connected to e-pucks. Also I've found some examples on forums, of people trying to use LibIrcom, which permit to send data by the IR sensors, but I can't find any examples of working codes. So I think I will give up the cross-compilation idea for now and stay with the remote control thing


because the cross compilation seems to be too incomplete for what I want to do

##### el 05/03/2019 20:11:33

%figure
![Selection_012.png](https://cdn.discordapp.com/attachments/565154703139405824/573964888288788481/Selection_012.png)
%end


why it says that there is  no highgui file? i also modified the makefile of the simulation project to include this library

##### 之之 05/03/2019 20:13:04
Try to drop it into a subdirectory?

##### el 05/03/2019 20:13:33
what subdirectory?

##### 之之 05/03/2019 20:14:01
Folder path


my english is not well,sorry

##### el 05/03/2019 20:44:23
as i see the opencv2 inside webots doestn't include highgui


/usr/bin/ld: warning: libopencv\_core.so.4.0, needed by //usr/local/lib/libopencv\_highgui.so, may conflict with libopencv\_core.so.2.4


i put 2.4 highgui.hpp in include webots files


in opencv2 of webots


and is says there are conflicts with the opencv i have outside of webots


how to solve the conflicts?

##### Akash 05/04/2019 21:40:04
How could i set a new offset value to the compass having a range of 0 to  359

##### Wei 05/05/2019 12:22:00
Hi all


I imported a wrl file, but there is no shadow?


But another wrl file has shadow...



> **Attachment**: [ok.WRL](https://cdn.discordapp.com/attachments/565154703139405824/574572017534238720/ok.WRL)

##### Fabien Rohrer [Moderator] 05/05/2019 12:42:17
`@Wei` Shadows is more a matter of enabling the Light.castShadow field: [https://cyberbotics.com/doc/reference/light](https://cyberbotics.com/doc/reference/light)


`@Akash` Did you look at the Compass.lookupTable field? [https://cyberbotics.com/doc/reference/compass](https://cyberbotics.com/doc/reference/compass)


`@el` Indeed, installing opencv is a good idea. If the OpenCV released in Webots is problematic, you could take a look at your LD\_LIBRARY\_PATH environment variable, and make sure your OpenCV appears first.


I also think that OpenCV is only used in some examples in Webots. So you could also simply remove the related files from Webots.

##### Leonidas Liakopoulos 05/05/2019 13:51:24
Hello, i have also problem linking my system`s opencv with webots, can anyone give a link or some steps of how to do this correctly? I use Ubuntu 18 and opencv4 installed with opencv.org instructions. Thanks in advance

##### janglee 05/05/2019 17:11:48
is there any good example of hexapod simulation made with webot?


like this one [https://www.youtube.com/watch?v=bmoGfBe63ZA](https://www.youtube.com/watch?v=bmoGfBe63ZA)

##### Fabien Rohrer [Moderator] 05/05/2019 18:13:58
The Mantis is certainly the most advanced hexapod we have in Webots: [https://cyberbotics.com/doc/guide/mantis](https://cyberbotics.com/doc/guide/mantis)


`@Leonidas Liakopoulos` you should simply install opencv and link it with your controller as described here [https://cyberbotics.com/doc/guide/using-webots-makefiles#adding-an-external-library-ccp](https://cyberbotics.com/doc/guide/using-webots-makefiles#adding-an-external-library-ccp)

##### Wei 05/06/2019 09:28:02
`@Fabien Rohrer` Thank you!

##### 之之 05/07/2019 14:54:24
My license may be occupied, can you help me?

##### Fabien Rohrer [Moderator] 05/07/2019 14:55:05
Hi,


Webots does not have license anymore since december. Could you use the free version of Webots?

##### 之之 05/07/2019 14:56:40
For some reasons, the version 8.5.4 is used.

##### Fabien Rohrer [Moderator] 05/07/2019 14:57:29
In this case, please send an e-mail to info@cyberbotics.com with your license ID, we will see how we could help you.

##### 之之 05/07/2019 14:58:02
thank you very much!

##### ༺Yatori༻ 05/07/2019 15:49:20
hey ! which sensor is the most fitting to get the value of  the position of a starting point A and the position of a target point B (so that i can tell my robot to go for example straight from A to B or things like that).

##### Fabien Rohrer [Moderator] 05/07/2019 15:50:04
Certainly the GPS which can retrieve an absolute position.

##### ༺Yatori༻ 05/07/2019 15:50:14
i see thank you !

##### Fabi 05/07/2019 19:28:35
Hi,  

I  am trying to run the python example  in (webots/projects/languages/python/worlds/example.wbt but I can not set the keyboard inputs to  control robot1.  Can anybody help me, please?

##### Olivier Michel [Cyberbotics] 05/08/2019 06:17:19
Hi Fabi, you need to click in the 3D view so that keyboard events can reach out the robot controller program.

##### ༺Yatori༻ 05/08/2019 07:00:41
Hey , does the epuck contain ultra sonic sensors ?

##### David Mansolino [Cyberbotics] 05/08/2019 07:02:21
Hi


No the epuck only contains 'infra-red' distance sensors.

##### ༺Yatori༻ 05/08/2019 07:20:54
I see thank you ,is it  possible to add it through a node or something like that?

##### CH\_KIM 05/08/2019 07:42:29
HI! I want to check sensor data by 2khz. but "duration" of "wb\_robot\_step" is "int" type.


How to get sensor data by high frequency.

##### David Mansolino [Cyberbotics] 05/08/2019 08:43:41
`@༺Yatori༻` yes you can add new node (e.g. DistanceSensor nodes) in the 'turretSlot' field of the E-puck node.


`@CH_KIM` the duration is in millisecond which means the biggest frequency you can have is 1khz (which is already quite high for a control loop).

##### CH\_KIM 05/08/2019 08:51:36
um... ok


not change the duration value, any solution about my problem.

##### Olivier Michel [Cyberbotics] 05/08/2019 08:55:06
There is currently no solution to go beyond 1 KHz.

##### CH\_KIM 05/08/2019 09:01:06
um ok!

##### AntiSquid 05/08/2019 09:44:06
where can i check the angular rotation of a robot, trying to adjust the torque

##### David Mansolino [Cyberbotics] 05/08/2019 09:45:26
Hi,  using the InertialUnit node ([https://www.cyberbotics.com/doc/reference/inertialunit](https://www.cyberbotics.com/doc/reference/inertialunit)) you can get the absolute orientation of the robot.


Is this what you need ?

##### AntiSquid 05/08/2019 09:46:55
not sure how to adjust the speed at which it turns

##### David Mansolino [Cyberbotics] 05/08/2019 09:47:11
what does your robot loooks like ?

##### AntiSquid 05/08/2019 09:48:29
like a box, a 3d rectangle

##### David Mansolino [Cyberbotics] 05/08/2019 09:48:52
ok and how do you control it? Does it has wheels ?

##### AntiSquid 05/08/2019 09:49:28
yes 2 wheels

##### David Mansolino [Cyberbotics] 05/08/2019 09:50:22
Then you can directly change the wheel rotation speed with the wb\_motor\_set\_velocity function: [https://www.cyberbotics.com/doc/reference/motor#wb\_motor\_set\_velocity](https://www.cyberbotics.com/doc/reference/motor#wb_motor_set_velocity)

##### AntiSquid 05/08/2019 09:51:36
right, thanks

##### David Mansolino [Cyberbotics] 05/08/2019 09:51:44
You're welcome

##### Avnish Sachar 05/08/2019 12:35:59
Hi, is there a way to print a custom message on the console even when the simulation is paused?

##### Fabien Rohrer [Moderator] 05/08/2019 12:38:14
Unfortunately, this is not possible.


Could you write your output into a file instead? Or use a debugger?

##### Avnish Sachar 05/08/2019 12:44:09
Yes, I can write into a file instead. Thanks!

##### Hussein 05/09/2019 09:34:16
Hello evryone. I'm new user

##### Fabien Rohrer [Moderator] 05/09/2019 09:34:32
Hi Hussein, welcome.

##### Hussein 05/09/2019 09:34:41
thanks

##### David Mansolino [Cyberbotics] 05/09/2019 09:38:15
Welcome 😃

##### Hussein 05/09/2019 12:42:08
Hi

Why webots close sudenlly afer luchuing

##### David Mansolino [Cyberbotics] 05/09/2019 12:42:25
Hi


what is your GPU? And are your GPU drivers up to date ?

##### Hussein 05/09/2019 12:44:38
Geforce m930. I have recent update

##### David Mansolino [Cyberbotics] 05/09/2019 12:46:34
Ok, please try to use Webots safe mode: [https://cyberbotics.com/doc/guide/starting-webots#safe-mode](https://cyberbotics.com/doc/guide/starting-webots#safe-mode)

##### Hussein 05/09/2019 12:47:23
Thanks

##### David Mansolino [Cyberbotics] 05/09/2019 12:49:30
You're welcome

##### kim 05/10/2019 06:53:23
Can i ask a question?

##### David Mansolino [Cyberbotics] 05/10/2019 06:53:31
Yes of course

##### kim 05/10/2019 06:54:36
Can I control the nao robot using the webot in choregraphe?


without using naoqi


umm... Can I contrl the real nao robot using the webot without naoqi?

##### David Mansolino [Cyberbotics] 05/10/2019 06:56:47
Yes you should use directly the Webots C/C++/Python/Java api.

##### kim 05/10/2019 06:57:13
Webots supports the openaigym?

##### David Mansolino [Cyberbotics] 05/10/2019 06:57:55
Not out of the box, but as Webots provide a Python interface you can integrate it yourself to openaigym

##### kim 05/10/2019 07:01:22
is there a tutorial about the using openaigym and webots?


and Webots Python api is in Webots program, is it right?

##### David Mansolino [Cyberbotics] 05/10/2019 07:02:45
No there is unfortunately no tutorial about openaigym and Webots. Yes the Python API is the API used to control your robot in Webots.


I strongly recommend to follow our tutorial (available in Python version) you will then better understand how the control of the robot works: [https://www.cyberbotics.com/doc/guide/tutorials?tab=python](https://www.cyberbotics.com/doc/guide/tutorials?tab=python)

##### kim 05/10/2019 07:24:17
Thanks

##### David Mansolino [Cyberbotics] 05/10/2019 07:27:14
You're welcome

##### fateme 05/11/2019 12:03:09
hi


I have a question . I want to replace my e-puck to a new place in environment after each episodes. Is there certain command for this transfer?

##### Fabien Rohrer [Moderator] 05/11/2019 12:08:25
Do you mean to set the robot position programatically?

##### fateme 05/11/2019 12:08:48
yes

##### Fabien Rohrer [Moderator] 05/11/2019 12:09:17
This is possible with the Supervisor API:

##### fateme 05/11/2019 12:09:44
I don not understand

##### Fabien Rohrer [Moderator] 05/11/2019 12:10:03
[https://www.cyberbotics.com/doc/guide/supervisor-programming#setting-the-position-of-robots](https://www.cyberbotics.com/doc/guide/supervisor-programming#setting-the-position-of-robots)


Please look at this doc page.

##### fateme 05/11/2019 12:10:43
Thanks

##### Fabien Rohrer [Moderator] 05/11/2019 12:10:53
You’re welcome 😉

##### fateme 05/11/2019 12:35:43
ignoring illegal call to wb\_supervisor\_node\_get\_from\_def() in a 'Robot' controller.


\#include <webots/supervisor.h> i add the library


why this error shows


this function can only be used in a 'Supervisor' controller.


I correct it


but when robot translate to new position, the speed is so high and rotate is it natural?

##### el 05/12/2019 12:12:26
hello 😃  well i want to take a camera frame and the next camera frame and implement a feature matching algorithm using opencv. but as seen in the above code i take the first frame 2 times cause i am in the same timestep .how to take 2 frames one after the other from 2 timwsteps?



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/577106042710917160/unknown.png)
%end


the feature matching algorythm works fine but i dont know how to take two frames one after the other

##### Amey 05/13/2019 01:46:12
Hi I want to enable GPS on e puck in python. Any link or reference that would help

##### Stefania Pedrazzi [Cyberbotics] 05/13/2019 06:18:21
> but when robot translate to new position, the speed is so high and rotate is it natural?



`@fateme` you should  reset the physics of your robot after translate it by calling wb\_supervisor\_node\_reset\_physics [https://www.cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_reset\_physics](https://www.cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_reset_physics)


`@Amey`  you should first add the GPS device in the 'turretSlot' of the e-puck because by default the e-puck model doesn't have it. The gps works exactly as all the other sensors. You can for example look at projects/robots/softbank/nao/worlds/nao\_demo.wbt for an example for the Nao robot that uses a GPS  or at projects/languages/python/worlds/example.wbt for a more simple and generic example of how to use sensors in Python

Here is the GPS documentation: [https://www.cyberbotics.com/doc/reference/gps?tab=python](https://www.cyberbotics.com/doc/reference/gps?tab=python)


`@el` in order to get two consecutive frames you have to call `wb_robot_step` function in between. So you have to store the previous camera image and use it in the next `while (wb_robot_step() != -1)` run.

##### el 05/13/2019 18:54:53
thank you very much ❤

##### Jeremy 05/14/2019 07:48:20
Hello, I am trying to get the position of my robot using a supervisor, but I don't quite get how to implement it. I have found an example in C on the website but do you know where I could find an example in C++ ?

##### Stefania Pedrazzi [Cyberbotics] 05/14/2019 08:13:53
Hi `@Jeremy` , yes you can find a C++ example in the driver.cpp controller file of the world projects/languages/cpp/worlds/example.cpp

##### Jeremy 05/14/2019 09:54:30
thank you !

##### Abdullah 05/14/2019 13:49:22
Hello, I just installed reboots on my mac and it seems that my GPU is of 1024 MB, however I can not see the world view, the 3D i mean.

##### Fabien Rohrer [Moderator] 05/14/2019 13:49:48
Hi


Do you have any error message?

##### Abdullah 05/14/2019 13:50:24
Hi Fabien,


I just restarted it and it worked ! sorry


there was some glitch maybe with my mac.

##### Fabien Rohrer [Moderator] 05/14/2019 13:51:00
glad to read that ^^

##### Abdullah 05/14/2019 13:51:22
Thank you. Have a good day

##### Fabien Rohrer [Moderator] 05/14/2019 13:51:36
Webots may have started on an empty world... Thank you, have a good day too.

##### Karl 05/14/2019 20:30:27
Do I really need to compile WeBots when making a custom remote control, or am I missing something?


Or, more generally: Does anyone know how to build a custom remote control with CMake?

##### Olivier Michel [Cyberbotics] 05/15/2019 06:24:06
Hi Karl,


You don't need to recompile Webots to create a custom remote control.


You just need to compile your own remote-control library.


A remote control library is a simple shared library (DLL on Windows, .so on Linux, .dylib on mac OS)


You can compile it with any C compiler, using CMake or any other build tool.


We provide examples of remote-control libraries built with gcc and Makefile, but you are free to use other tools.


See the documentation here: [https://www.cyberbotics.com/doc/guide/transfer-to-your-own-robot#developing-a-remote-control-plugin](https://www.cyberbotics.com/doc/guide/transfer-to-your-own-robot#developing-a-remote-control-plugin)

##### el\_samu\_el 05/15/2019 09:18:23
When I start ubuntu with ' webots --minimize --stdout' ion ubuntu, it still opens the GUI and prints partly some messages on terminal and on webots console. Is this a known bug or am I doing something wrong?

##### Karl 05/15/2019 09:18:49
Thank you `@Olivier Michel`! I got it to work with cmake.

##### el\_samu\_el 05/15/2019 09:18:51
When I start webots with ' webots --minimize --stdout' on ubuntu, it still opens the GUI and prints partly some messages on terminal and on webots console. Is this a known bug or am I doing something wrong?


**

##### Fabien Rohrer [Moderator] 05/15/2019 09:19:40
Could you try to add `--batch` option?

##### el\_samu\_el 05/15/2019 09:20:02
yes


does not change anything really

##### Fabien Rohrer [Moderator] 05/15/2019 09:20:26
Webots requires the GUI, it's currently not possible to run Webots without GUI

##### el\_samu\_el 05/15/2019 09:20:58
that's fine, but I need the output on the console

##### Fabien Rohrer [Moderator] 05/15/2019 09:21:07
But normally, most messages should be redirected  on the console.


which messages are not redirected?

##### el\_samu\_el 05/15/2019 09:21:21
because webots tends to print in the wrong order or does not at all if the program isn't terminating


error messages are shown in the webots console


while print statements are shown in the terminal

##### Fabien Rohrer [Moderator] 05/15/2019 09:23:38
Ok. Could I kindly ask you to fill a bug report here: [https://github.com/omichel/webots/issues](https://github.com/omichel/webots/issues) mentionning your OS, the Webots version, and an example of a message not working (screenshot)? We will help you there.

##### el\_samu\_el 05/15/2019 09:23:51
Okay!

##### Fabien Rohrer [Moderator] 05/15/2019 09:24:01
thank you

##### Olivier Michel [Cyberbotics] 05/15/2019 09:25:23
`@SamuelKopp`: did you try to use the --stdout and --stderr command line options of Webots? Is it what you need?

##### Fabien Rohrer [Moderator] 05/15/2019 09:25:40
👍

##### el\_samu\_el 05/15/2019 09:27:52
No, I missed stderr. Thanks! But that --minimize is not working is known, right?

##### Fabien Rohrer [Moderator] 05/15/2019 09:28:51
It's supposed to work. On which OS are you working?

##### el\_samu\_el 05/15/2019 09:29:04
Ubuntu 18.04

##### Fabien Rohrer [Moderator] 05/15/2019 09:32:29
This option is supposed to launch Webots but minimized. Do you see the window in such case?

##### el\_samu\_el 05/15/2019 09:34:33
yes, it opens in exactly the same position and layout as when I opened it the last time normally

##### Stefania Pedrazzi [Cyberbotics] 05/15/2019 09:34:57
I checked and I get the same issue on Ubuntu 18.04 with minimize.

##### Fabien Rohrer [Moderator] 05/15/2019 09:36:23
Yes, me too


I'm opening issue


[https://github.com/omichel/webots/issues/453](https://github.com/omichel/webots/issues/453)


`@el_samu_el` please follow this issue to be aware about its resolution.

##### el\_samu\_el 05/15/2019 10:00:43
So the other issue is that webots print statements are somewhat delayed in the execution order. If my program does not terminate, then they are not printed at all. This makes debugging really hard.

##### Olivier Michel [Cyberbotics] 05/15/2019 10:03:17
Do you get this even with the --stdout and --stderr options?

##### el\_samu\_el 05/15/2019 10:03:37
yes, the output is  the same as in the webots console

##### Olivier Michel [Cyberbotics] 05/15/2019 10:03:43
Did you try to call fflush(stdout) after printfs?

##### el\_samu\_el 05/15/2019 10:03:48
i am using python


will try

##### Olivier Michel [Cyberbotics] 05/15/2019 10:04:28
Did you try to set the PYTHONUNBUFFERED environment variable?


See [https://stackoverflow.com/questions/107705/disable-output-buffering](https://stackoverflow.com/questions/107705/disable-output-buffering)

##### el\_samu\_el 05/15/2019 10:04:48
no, I didn't read about that


thanks


flushing does not help, i'll try it with the env variable


setting PYTHONUNBUFFERED does not help either

##### Olivier Michel [Cyberbotics] 05/15/2019 10:15:53
Oops... Can you try writing into a file?

##### el\_samu\_el 05/15/2019 10:31:09
yeah that works perfectly

##### Olivier Michel [Cyberbotics] 05/15/2019 10:36:35
This is strange, I don't understand why you can't get the same on stdout/stderr...

##### el\_samu\_el 05/15/2019 10:49:52
it just outputs "controller\_name": Starting: "python3 -u "filename.py"


then it ignores the print statement in the first line  even with flush


keyboard interrupt doesn't help with showing what is printed

##### Olivier Michel [Cyberbotics] 05/15/2019 10:58:02
Do you mean the print in the Terminal (with --stderr --stdout) or in the Webots console?

##### el\_samu\_el 05/15/2019 10:58:20
both is equivalent

##### Olivier Michel [Cyberbotics] 05/15/2019 10:58:28
OK...

##### nisuunn 05/15/2019 12:45:55
Hi


I'm using a supervisor to create new objects at runtime.


Once they've spawned, how would I get a handle to these newly created objects?


I would use the getFromDef function, but if many of the same objects are spawned, they'd all have the same DEF,

##### David Mansolino [Cyberbotics] 05/15/2019 12:49:07
Hi


You can either define different def when spawning them or retrieve them with the  wb\_supervisor\_field\_get\_mf\_node function ([https://www.cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_field\_get\_mf\_node](https://www.cyberbotics.com/doc/reference/supervisor#wb_supervisor_field_get_mf_node)) , you can use '-1' as index to get the latest imported node.

##### nisuunn 05/15/2019 12:53:06
How do I define a different def when spawning a new object?

##### Olivier Michel [Cyberbotics] 05/15/2019 13:05:41
See example here: [https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_field\_import\_mf\_node\_from\_string](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_field_import_mf_node_from_string)


wb\_supervisor\_field\_import\_mf\_node\_from\_string(root\_children\_field, 4, "DEF MY\_ROBOT Robot { controller \"my\_controller\" }");

##### nisuunn 05/15/2019 13:07:57
Thank you!

##### Olivier Michel [Cyberbotics] 05/15/2019 13:09:21
You are welcome.

##### hunter[EU] 05/16/2019 19:01:23
We are havin issues attach libreries to the simulator

60448918\_416254715593013\_6479608462719320064\_n.png

60809016\_841252589585797\_12981491407519744\_n.png



|Regards ,



%figure
![60448918_416254715593013_6479608462719320064_n.png](https://cdn.discordapp.com/attachments/565154703139405824/578658470224592896/60448918_416254715593013_6479608462719320064_n.png)
%end



%figure
![60809016_841252589585797_12981491407519744_n.png](https://cdn.discordapp.com/attachments/565154703139405824/578658498192343040/60809016_841252589585797_12981491407519744_n.png)
%end

##### David Mansolino [Cyberbotics] 05/17/2019 06:31:55
Hi, you have to modify the makefile of your controller to add the includes and link with the library, please find more information about this here: [https://www.cyberbotics.com/doc/guide/using-webots-makefiles#adding-an-external-library-ccp](https://www.cyberbotics.com/doc/guide/using-webots-makefiles#adding-an-external-library-ccp)

##### Thelm 05/17/2019 11:12:32
Hi! I have a problem when connecting e-puck by bluetooth to webots as it says "wbr\_motor\_set\_position was called but not set". In the code the only time I call this method is in an init function to set the motors in velocity control mode :

wheels[i].setPosition(float('+inf'))

wheels[i].setVelocity(0)



but the program works just fine in the simulation

##### Olivier Michel [Cyberbotics] 05/17/2019 12:20:14
Hi, did you try to run the sample e-puck controllers with a remote controlled robot?


If so, did that work?

##### Thelm 05/17/2019 12:29:06
yes the sample crontrollers are working fine ....


but I'm progrmming in python

##### Olivier Michel [Cyberbotics] 05/17/2019 12:30:38
Strange... Did you try to make the same kind of motor control as in the C sample? E.g., control in speed rather than in position?

##### Thelm 05/17/2019 13:13:35
yes, actually, at first I tried to do a position control in python. then I had this messages so I tried to do a velocity control


I also tried to put a robot.set(3000) to give time to establish the connection

##### Olivier Michel [Cyberbotics] 05/17/2019 13:20:53
robot.step(3000) is probably useless. It should work fine with a robot.step(100) value or so.

##### Thelm 05/17/2019 13:36:20
yes but it's still not working fine..... I'll try to make the same controller in C it maybe will solve the problem

##### Olivier Michel [Cyberbotics] 05/17/2019 13:37:06
OK, let us know if that helps.

##### Jeremy 05/17/2019 16:18:19
Hello ! I have another question, how can I get the simulation time in c++ ?

##### Olivier Michel [Cyberbotics] 05/17/2019 16:19:23
See here: [https://cyberbotics.com/doc/reference/robot?tab=c++#wb\_robot\_get\_time](https://cyberbotics.com/doc/reference/robot?tab=c++#wb_robot_get_time)

##### Jeremy 05/17/2019 17:11:58
thank you !

##### Amey 05/19/2019 02:10:39
Hello, I just need some guidance on how I can move a robot and reach the destination by avoiding obstacles.

##### Jeremy 05/19/2019 14:41:23
Hello, I have two controllers, one for my robot and one for a supervisor. Is it possible  from the controller of the supervisor to stop/exit the controller of the robot ? I am working in C++.

##### Stefania Pedrazzi [Cyberbotics] 05/20/2019 06:23:42
Hi `@Amey` , ostacle avoidance is a research topic in robotics and there are many different ways to achieve it. You can find many examples in the provided simulations, I would suggest you to run the Webots Guided Tour and look at the different controller programs. 

We often use the Breitenberg algorithm ([https://en.wikipedia.org/wiki/Braitenberg\_vehicle](https://en.wikipedia.org/wiki/Braitenberg_vehicle)), for example in

- robots/gctronic/worlds/e-puck.wbt

- samples/robotbenchmark/obstacle\_avoidance/worlds/obstacle\_avoidance.wbt


Hi `@Jeremy` , you cannot stop it directly, but you can notify the robot controller to stop by the supervisor controller and handle the stop message in the robot controller. For example you can use the Emitter/Receiver ([https://www.cyberbotics.com/doc/reference/emitter](https://www.cyberbotics.com/doc/reference/emitter)) devices to send the stop message from the supervisor to the robot.

##### Thelm 05/20/2019 14:54:26
Hi! Could you help me to find out what's wrong in my code? I'm trying to remote control an e-puck with bluetoothand I get this error :

```
wbr_motor_set_position was called but not set
```



here is the main code :

```C
int main(int argc, char **argv)
{
  /* necessary to initialize webots stuff */
  wb_robot_init();
  WbDeviceTag left_motor, right_motor, left_sensor, right_sensor;
  left_motor = wb_robot_get_device("left wheel motor");
  right_motor = wb_robot_get_device("right wheel motor");
  printf("testm\n");
  wb_motor_set_position(left_motor, 0.0);
  wb_motor_set_position(right_motor, 0.0);
  wb_motor_set_velocity(left_motor, 2.0);
  wb_motor_set_velocity(right_motor, 2.0);
  
  wb_robot_step(2000);
  
  wb_motor_set_position(left_motor, 10.72);
  wb_motor_set_position(right_motor, 10.72);


  while (wb_robot_step(TIME_STEP) != -1) {
  };

  /* Enter your cleanup code here */

  /* This is necessary to cleanup webots resources */
  wb_robot_cleanup();

  return 0;
}
```

##### Fabien Rohrer [Moderator] 05/20/2019 14:55:33
Hi


It means that your controller is linked with a remote control library (cf. Robot.remoteControl field) where wbr\_motor\_set\_position should be set.


=> wbr\_motor\_set\_position is not defined in this library.

##### David Mansolino [Cyberbotics] 05/20/2019 14:57:49
For information position control for the e-puck in remote-control is not supported with the remote-control library distributed with Webots: [https://github.com/omichel/webots/blob/revision/projects/robots/gctronic/e-puck/plugins/remote\_controls/e-puck\_bluetooth/entry\_points.cpp#L31](https://github.com/omichel/webots/blob/revision/projects/robots/gctronic/e-puck/plugins/remote_controls/e-puck_bluetooth/entry_points.cpp#L31)

##### Thelm 05/20/2019 15:00:29
Okay thank you, i'll try to do without but I would be harder with just velocity control because of the wb\_robot\_step not properly workng with bluetooth

##### elefps 05/21/2019 13:28:09
which is the default focal length and  principle point of the epucks camera?

##### Fabien Rohrer [Moderator] 05/21/2019 13:29:39
Hi el


This is defined in the E-puck.proto file, let me check...


[https://github.com/omichel/webots/blob/master/projects/robots/gctronic/e-puck/protos/E-puck.proto#L20](https://github.com/omichel/webots/blob/master/projects/robots/gctronic/e-puck/protos/E-puck.proto#L20)


The field of view is by default 0.84 radians and this is an open field (it can be modified).


The focal length and the principal point are different concepts.


Focal length, principal point and field of view are related with this equation: [https://www.edmundoptics.fr/resources/application-notes/imaging/understanding-focal-length-and-field-of-view/](https://www.edmundoptics.fr/resources/application-notes/imaging/understanding-focal-length-and-field-of-view/)


But Webots allows to define the horizontal field of view only.


Does this answer your question?


Webots can also simulate depth of field blur. In this case, the focal distance can be set: [https://www.cyberbotics.com/doc/reference/focus](https://www.cyberbotics.com/doc/reference/focus)


This is not enabled by default on the e-puck

##### Mark - AgJunction 05/21/2019 21:43:31
I'm need to output my robot's GPS as CAN traffic.  I've been able to do so from inside the controller, but can someone tell me if using a Controller Plug-In is the correct way to do this?

##### David Mansolino [Cyberbotics] 05/22/2019 07:08:16
I would rather keep it on the controller side, but if you want to make it a bit modular you can wrap this in a library that you can call from each of your controller (so that you don't cuplicate the code in each controller).

##### Thelm 05/22/2019 16:04:04
Hi, I'm still with my position control problem for the e-puck, so I'm editing the library to implement a position control directly into the e-puck firmware, but now I'm stuck because I need an equivalent a events listeners in C... do someone know how can I do this?


I can't use simple while (!condition) because I need to execute other pieces of code at the same time

##### moon 05/23/2019 06:12:12
Hi, everyone! I just have started to study webots. And by following the tutorial, I have faced with next error : "ERROR: 'RectangleArena.proto': Lua error: 'floorSize' must contain positive values". Would you help with this? I've tried to change the values of the floorSize. But it doesn't helpful

##### David Mansolino [Cyberbotics] 05/23/2019 06:13:41
`@Thelm` since in remote-control you will communicate with the robot only during the step function, the only way to do this is to check the encoder position at each step. If you want something really precise, the only way is to implement position cotnrol directly in the robot library: [https://github.com/omichel/webots/blob/revision/projects/robots/gctronic/e-puck/transfer/library/motor\_led/advance\_one\_timer/e\_motors.c](https://github.com/omichel/webots/blob/revision/projects/robots/gctronic/e-puck/transfer/library/motor_led/advance_one_timer/e_motors.c)


`@moon`, in which tutorial did you encoutner this problem?

##### moon 05/23/2019 07:16:10
in the first. But right now everything is working, after an execution of build error was disappeared.

##### David Mansolino [Cyberbotics] 05/23/2019 07:19:11
Ok perfect!

##### nisuunn 05/23/2019 13:13:50
Hi!

##### David Mansolino [Cyberbotics] 05/23/2019 13:14:29
Hi

##### nisuunn 05/23/2019 13:15:26
I would like to import a robot (robot node) using a supervisor. I'm trying to use childField.importMFNode(index, robotPath), where childField is the children field of a transform node, however Webots does not let me do this as you cannot import robot nodes into children fields of transform fields


is there any way for me to spawn in a robot using the inmportMFNode(), or in any other way?

##### David Mansolino [Cyberbotics] 05/23/2019 13:16:09
Yes of course, let me explain


First, it is better to import you robot at the root level, you can get the root field with something like this:

supervisor.getRoot().getField('children')


Then you can import anything in this field

##### nisuunn 05/23/2019 13:17:53
ah okay, perfect, many thanks!

##### David Mansolino [Cyberbotics] 05/23/2019 13:18:04
You may also be interested by the 'importMFNodeFromString' function


you can do something like:

supervisor.getRoot().getField('children').importMFNodeFromString('Robot { translation 1 0 2 }')


You're welcome

##### nisuunn 05/23/2019 13:19:44
I see, thanks again!


I'm using supervisor.getRoot().getField('children')


and into that children field I spawn my desired robot


(i'm using importMFNode(index, path))


however, instead of spawning only one robot, it keeps spawning in the same robot forever, so that more and more of these robots are created in the same exact space, I think the code does not get past this spawning point.


Is there any way around this issue?

##### David Mansolino [Cyberbotics] 05/23/2019 13:38:16
Do you call the ` importMFNode(index, path)` function at each step ?

##### nisuunn 05/23/2019 13:40:08
no


in one function which is called once

##### David Mansolino [Cyberbotics] 05/23/2019 13:40:37
Does the robot that you spawn have the same controller as the one spawning the robot ?

##### nisuunn 05/23/2019 13:40:44
yes


the same supervisor

##### David Mansolino [Cyberbotics] 05/23/2019 13:41:36
Then this is probably the problem:

 step0: original robot spawn robot 1

step1: robot1 spawn robot2

step2: robot2 spawn robot3

...

##### nisuunn 05/23/2019 13:42:42
I see, many thanks!

##### David Mansolino [Cyberbotics] 05/23/2019 13:42:49
You're welcome

##### Fabi 05/23/2019 18:01:54
Hi

I am trying to build a ROS controller in python but  it returns  import error: no module  named rospy 

Any idea  how to fix this?

##### mi 05/23/2019 19:53:01
Hello



Has anyone ever worked with line follower with Webots, ROS and python at the same time?

##### David Mansolino [Cyberbotics] 05/24/2019 06:17:21
Hi, not in Python, but this example perform simple line following with the epuck robot using ROS in C++: [https://github.com/omichel/webots/blob/revision/projects/languages/ros/webots\_ros/src/e\_puck\_line.cpp](https://github.com/omichel/webots/blob/revision/projects/languages/ros/webots_ros/src/e_puck_line.cpp)

##### moon 05/24/2019 06:31:57
Hi everyone! I am studying this tutorial from youtube channel Cyberbotics  (link is [https://www.youtube.com/watch?v=xRvFJDuhx3Y&t=303s](https://www.youtube.com/watch?v=xRvFJDuhx3Y&t=303s)). So here after executing rosrun gmapping slam\_gmapping scan:=/pioneer3at/ command, rviz has to show map. But not my RViz. It shows a warning message "no map received". So after I have installed slam\_gmapping package, but it doesn't helpful. I have tried to write own launch file, but it seems like doesn't useful for pioneer3at, or I don't know. Anyway, it did not work. So, can you help me&

##### David Mansolino [Cyberbotics] 05/24/2019 06:36:15
Hi, in the terminal you launched gmapping do you have any error messages or can you see that it is updating the map?

##### moon 05/24/2019 06:48:43
no, there is no errors
%figure
![2.png](https://cdn.discordapp.com/attachments/565154703139405824/581372993784119307/2.png)
%end

##### David Mansolino [Cyberbotics] 05/24/2019 06:50:18
No error but nothing is printed, gmapping should print each time it is computing updates. Is the simulation running ?


If yes, does the robo move?


Is there any error in the webots console ?

##### moon 05/24/2019 07:01:19
webots also doesn't show any error
%figure
![4.png](https://cdn.discordapp.com/attachments/565154703139405824/581376164644257792/4.png)
%end

##### David Mansolino [Cyberbotics] 05/24/2019 07:01:57
But the camera doesn't seems enabled... Does the robot move?


Did you start the 'pioneer3at' ros node ?

##### moon 05/24/2019 07:05:16
camera and robot work
%figure
![6.png](https://cdn.discordapp.com/attachments/565154703139405824/581377160376221696/6.png)
%end

##### David Mansolino [Cyberbotics] 05/24/2019 07:05:55
Perfect and now if you start gmapping after that does it prints something (the order in which you start the node might be important)

##### moon 05/24/2019 07:18:24
no, the  same...
%figure
![7.png](https://cdn.discordapp.com/attachments/565154703139405824/581380465362075648/7.png)
%end

##### David Mansolino [Cyberbotics] 05/24/2019 07:19:09
Can you check if the sick is pucblishing laser\_scan ?

##### moon 05/24/2019 07:59:02
laser also works well.
%figure
![8.png](https://cdn.discordapp.com/attachments/565154703139405824/581390689565278208/8.png)
%end

##### David Mansolino [Cyberbotics] 05/24/2019 08:05:58
I am sorrry but I can't help you further, if the laser scan is correctly published from the Webots side gamping should produce the map, this is more a ROS problem than a Webots problem.

Maybe someone more familiar to ROS could help you.


Maybe you could also try to visualize the lascer\_scan in rviz just to check if they make sense

##### moon 05/24/2019 08:11:01
Thank you a lot! I've tried to visualize laser scan by RViz, and there were appearing little dots, outside of the grid,  which hard to see.
%figure
![9.png](https://cdn.discordapp.com/attachments/565154703139405824/581393708214124544/9.png)
%end

##### David Mansolino [Cyberbotics] 05/24/2019 08:13:12
Indeed they are hard to see, you should probably change the color of the dot and make sure the  size of the grid is similar to the size of the floor in Webots, then in Webots you can enable the optional rendering 'View->Optional Rendering->Show Lidar Point Cloud' and check if the shapes of the points are similar in Webots and RViz

##### moon 05/24/2019 08:23:38
I've already solved! I had to skip this step, I wrote " rosrun gmapping slam\_gmapping scan:=/pioneer3at/Sick\_LMS\_291/laser\_scan/layer0 \_xmax:=30 \_xmin:=-30 \_ymax:=30 \_ymin:=-30 \_delta:=0.2", as shown in the tutorial, and everything works! Thank you for your help!!

##### David Mansolino [Cyberbotics] 05/24/2019 08:31:37
Hi, ok good news!

##### TH0 05/24/2019 09:57:24
Hi!



i'm currently working on a more realistic version of the Nao-Robocup-Simulator. I already changed some materials, textures and other things in the proto files:
%figure
![webots1.png](https://cdn.discordapp.com/attachments/565154703139405824/581420477193650186/webots1.png)
%end



%figure
![webots2.png](https://cdn.discordapp.com/attachments/565154703139405824/581420495564439568/webots2.png)
%end


but i wonder if there is maybe a visual tool to edit the proto files instead of using a text editor? Because my current workflow is extremly ineffective and i am not able to edit the geometry of objects by myself except editing the raw coordinates in the proto files -\_-



Can you recommend a tool for editing proto files and create or change webots objects efficiently?

##### David Mansolino [Cyberbotics] 05/24/2019 10:00:06
Hi, very nice simulation !!

##### TH0 05/24/2019 10:00:22
thanks 😃 i will share it when its finished

##### David Mansolino [Cyberbotics] 05/24/2019 10:01:15
To edit the mesh we usually use Blender, we have created a plugin to be able to export directly from Blender to Webots (but you can also export in VRML from blender and import it in Webots): [https://github.com/omichel/blender-webots-exporter](https://github.com/omichel/blender-webots-exporter)


Very cool, looking forward to see it !

##### TH0 05/24/2019 10:02:24
ok, and how about importing already existing geometry (like the nao.proto) in blender ?

##### David Mansolino [Cyberbotics] 05/24/2019 10:02:51
Abotu editing the PROTO, it is sometimes difficult indeed to edit directly in text file, for this you can right click on the PROTO node in Webots and press on 'Convert to Base Node(s)' this will 'unprototize' the robot for you.


You can export your robot in vrml from Webots and then import it in Webots, you might loose some information in the precess but at least the meshes (which is what is interesting to edit in blender) should be fine.

##### TH0 05/24/2019 10:04:19
ok, cool, i will try that!

##### David Mansolino [Cyberbotics] 05/24/2019 10:04:41
Do not hesitate to ask us if you have problem with one specific step

##### TH0 05/24/2019 10:05:39
ok, thanks! i have a ton of other questions, but one after another 😄

##### David Mansolino [Cyberbotics] 05/24/2019 10:05:50
Good idea 😉

##### newcomer\_523 05/24/2019 12:52:21
Hello, can anybody tell me how i can change the type of a distance sensor from the  generic type to infra-red type?

##### David Mansolino [Cyberbotics] 05/24/2019 12:52:32
Hello


yes, you simply have to change the 'type' field of the corresponding DistanceSensor node.

##### newcomer\_523 05/24/2019 12:54:37
Thanks for this fast response 😃 Could you tell me how i can do this in C?  I just started using webots and programming with C.

##### David Mansolino [Cyberbotics] 05/24/2019 12:55:28
You should not do this from the controller, but manually in the Webots scene-tree.


I strongly recommend to do our tutorial (at least the first 7) to get familiar with Webots: [https://www.cyberbotics.com/doc/guide/tutorials](https://www.cyberbotics.com/doc/guide/tutorials)

##### newcomer\_523 05/24/2019 12:56:07
thank you very much 😄

##### David Mansolino [Cyberbotics] 05/24/2019 12:56:18
You're welcome

##### Noji 05/25/2019 04:10:58
Hello, I'm struggling now. 3D view is disappeared.


Could someone tell me how to show 3D view again?


No 3D view
%figure
![2019-05-25_13.13.14.png](https://cdn.discordapp.com/attachments/565154703139405824/581696384013697027/2019-05-25_13.13.14.png)
%end


I think this is a very basic problem. Sorry for bothering you.

##### David Mansolino [Cyberbotics] 05/27/2019 06:12:33
`@Noji`, the simplest solution is to use the 'Tools->Restore Layout' menu

##### Markacho 05/27/2019 08:36:31
Can someone tell me if the Boomer 3050 tractor model can be modified?

##### David Mansolino [Cyberbotics] 05/27/2019 09:22:40
Yes of course, you can modify any PROTO node distributed with Webots.

##### Markacho 05/27/2019 09:23:08
Thanks.  I just found it:  Tractor.proto

##### David Mansolino [Cyberbotics] 05/27/2019 09:23:21
You're welcome

##### Ya 05/27/2019 14:46:50
hi, how can i use the lidar with khepera IV and where can i fin th source code thank you

##### Fabien Rohrer [Moderator] 05/27/2019 14:48:11
Hi

##### Ya 05/27/2019 14:48:23
Hi

##### Fabien Rohrer [Moderator] 05/27/2019 14:48:40
You should start by running the khepera4.wbt example:

##### Ya 05/27/2019 14:48:54
yes i do it

##### Fabien Rohrer [Moderator] 05/27/2019 14:49:03
[https://cyberbotics.com/doc/guide/khepera4#khepera4-wbt](https://cyberbotics.com/doc/guide/khepera4#khepera4-wbt)

##### Ya 05/27/2019 14:49:47
also i modifaded the source code of khepera 4


you have an amazing simulator

##### Fabien Rohrer [Moderator] 05/27/2019 14:51:01
Then you should add a Lidar node to the Khepera4.turretSlot



%figure
![Capture_decran_2019-05-27_a_16.50.40.png](https://cdn.discordapp.com/attachments/565154703139405824/582581579042390027/Capture_decran_2019-05-27_a_16.50.40.png)
%end


save the simulation.


Now, the Lidar should be accessible in the controller, using the Lidar API:


[https://cyberbotics.com/doc/reference/lidar](https://cyberbotics.com/doc/reference/lidar)

##### Ya 05/27/2019 14:51:57
there is a source code to use the lidar ?

##### Fabien Rohrer [Moderator] 05/27/2019 14:52:44
The documentation above describes all the lidar functions.


You could refer to the lidar example:


[https://cyberbotics.com/doc/guide/samples-devices#lidar-wbt](https://cyberbotics.com/doc/guide/samples-devices#lidar-wbt)


The controller (lidar.c) of this example shows how to use the API in a working situation.


Thank you ^^

##### Ya 05/27/2019 15:03:41
all the best ^^

##### Noji 05/27/2019 15:27:34
Dear `@David Mansolino` Thanks a lot! I tried 'Tools->Restore Layout'. First, the window did not change, but after restarting Webots, 3D view was shown again.

##### David Mansolino [Cyberbotics] 05/27/2019 15:29:25
`@Noji`, you're welcome.

##### mehdi 05/28/2019 14:00:02
hi everybody


is there any one can tell me why the last version of webots crach at the startup in my computer..???

##### Olivier Michel [Cyberbotics] 05/28/2019 14:21:43
Hi, did you check the minimal requirements?


[https://cyberbotics.com/doc/guide/system-requirements](https://cyberbotics.com/doc/guide/system-requirements)

##### sai 05/29/2019 05:57:42
hello, we are looking for a warehouse environment in webots human-robot interaction experiments. I couldn't find much in the default environment samples. Is there an external source for such environments or do we have to build it from scratch?? Thanks in advance

##### Fabien Rohrer [Moderator] 05/29/2019 06:00:56
Hi, we have a set of predefined assets that can be easily put together to create such kind of environment:


For example here: [https://www.cyberbotics.com/doc/guide/object-factory](https://www.cyberbotics.com/doc/guide/object-factory)

##### sai 05/29/2019 06:01:57
Ok thanks, I will have a look

##### Fabien Rohrer [Moderator] 05/29/2019 06:03:59
If this is not sufficient, we could create new assets and a demo example, if you have some budget for this.

##### sai 05/29/2019 06:04:58
I see, I will have to discuss with my advisor. We will let you know if needed, thanks!!


I have another question. How flexible is the process of importing vrml designs into webots. I haven't tried it before but is it possible to import complex object like a factory machine or something designed in CAD, exported as a vrml format and importing it into webots?

##### Fabien Rohrer [Moderator] 05/29/2019 06:40:03
The Webots language is very close to VRML (subset + extension) and provides VRML import/export tools. Using blender at some point and its exporter plugin is often required [https://github.com/omichel/blender-webots-exporter](https://github.com/omichel/blender-webots-exporter) for example to create low poly meshes. Setting the physics parameters may also require supplementary passes.  Cyberbotics has a lot of expertise in this field.

##### sai 05/29/2019 06:43:32
That sounds great. thanks

##### Deleted User 05/29/2019 10:32:33
Hi, I'm trying to  run Webots remotely on a google cloud VM-Instance. However,  Im running into some issues with OpenGL. Specifically, its giving me these errors: libGL error: No matching fbConfigs or visuals found

libGL error: failed to load driver: swrast

libGL error: No matching fbConfigs or visuals found

libGL error: failed to load driver: swrast

Warning: Unrecognized OpenGL version


I was wondering if you might be able to help me out?


ERROR: Unable to load OpenGL functions!


It throws this error as well

##### Olivier Michel [Cyberbotics] 05/29/2019 10:34:13
You need to have a GPU enabled compute node or setup a virtual frame buffer (xvfb).


Or xf86-video-dummy (Xdummy).

##### Deleted User 05/29/2019 10:37:58
Thanks for the response! Do I need to do this on the VM-Instance or my own computer? Since, others are using the same server to run webots remotely and they do not run into this issue.

##### Olivier Michel [Cyberbotics] 05/29/2019 10:38:47
On the VM instance.


But wait, is the display of Webots redirected to your computer?

##### Deleted User 05/29/2019 10:39:06
yes it is

##### Olivier Michel [Cyberbotics] 05/29/2019 10:39:32
In that case, forget about the virtual frame buffer, you simply need to redirect the DISPLAY to your computer.


With export DISPLAY / xhost+

##### Deleted User 05/29/2019 10:40:58
I think it is already redirected to my computer


it starts xquartz and shows a webots window in xquartz


however after this it throws the opengl errors and crashes

##### Olivier Michel [Cyberbotics] 05/29/2019 10:42:05
Oops... Then, I am afraid I don't know how to help you with that...


Can run other OpenGL based applications? like glxgears?

##### Deleted User 05/29/2019 10:44:15
I have not tried this I'll test it and see what happens, thanks!


One more question regarding the xvfb


If I set it up on the VM-Instance running in the cloud, would I then be able to run webots completely on the VM-instance even though it doesnt have an actual display?

##### Olivier Michel [Cyberbotics] 05/29/2019 10:47:34
Yes.

##### Deleted User 05/29/2019 10:48:07
Okay, thank you very much!

##### Olivier Michel [Cyberbotics] 05/29/2019 10:48:24
Welcome

##### Lifebinder (tsampazk) 05/29/2019 15:39:36
Hello, is there a way to reset the position (translation) of a robot through a different controller (supervisor), without reverting the simulation, etc.? We are using python

##### David Mansolino [Cyberbotics] 05/29/2019 15:40:30
Hi, yes of course, this is exmplained in detail here: [https://www.cyberbotics.com/doc/guide/using-numerical-optimization-methods#using-the-wb\_supervisor\_field\_set\_-and-wb\_supervisor\_simulation\_reset\_physics-functions](https://www.cyberbotics.com/doc/guide/using-numerical-optimization-methods#using-the-wb_supervisor_field_set_-and-wb_supervisor_simulation_reset_physics-functions)


The example is in C, but you can find all the equivalent functions in Python

##### Lifebinder (tsampazk) 05/29/2019 15:52:14
follow up question,  we are able to get the translation field through "robotNode.getField('translation')" but then cant find the equivalent to "

wb\_supervisor\_field\_set\_sf\_vec3f(trans\_field, INITIAL\_TRANS);" from the example.

##### David Mansolino [Cyberbotics] 05/29/2019 15:54:58
The 'robotNode.getField('translation')' function will return a Field object, you therefore have to keep it and use it:

  translationField = robotNode.getField('translation')

  translationField.setSFVec3f(INITIAL\_TRANS)


([https://cyberbotics.com/doc/reference/supervisor?tab=python#wb\_supervisor\_node\_get\_field](https://cyberbotics.com/doc/reference/supervisor?tab=python#wb_supervisor_node_get_field) and [https://cyberbotics.com/doc/reference/supervisor?tab=python#wb\_supervisor\_field\_set\_sf\_vec3f](https://cyberbotics.com/doc/reference/supervisor?tab=python#wb_supervisor_field_set_sf_vec3f))

##### Lifebinder (tsampazk) 05/29/2019 15:56:35
it worked, thank you very much 😃

##### David Mansolino [Cyberbotics] 05/29/2019 15:56:43
You're welcome

##### Mr. Scruff 05/29/2019 21:10:29
Is there an issue with this portion of the tutorial documentation (see attached)?  I'm getting a Python error, complaining that "Motor wheels = []"  has a syntax error.

Instead, this simple code worked:



from controller import Robot

robot = Robot()

speed = -1.5  # [rad/s]

wheelsNames = ['wheel1', 'wheel2']

wheels = []

for i, name in enumerate(wheelsNames):

    wheels.append(robot.getMotor(name))

    wheels[i].setPosition(float('inf'))

    wheels[i].setVelocity(speed)



timestep = int(robot.getBasicTimeStep())

while robot.step(timestep) != -1:

    pass
%figure
![Doc_Issue.png](https://cdn.discordapp.com/attachments/565154703139405824/583401806344880128/Doc_Issue.png)
%end

##### Fabien Rohrer [Moderator] 05/30/2019 08:11:04
There is indeed an issue.


I would simply replace « Motor wheels = [] » by « wheels = [] »

##### Luiz Felipe 05/30/2019 08:58:55
Hello everyone, first of all thanks for the hard work.. The simulator is amazing...


I am having troubles with the version 2019a running in windows... I already uninstalled and reinstalled trying to solve the issue... When I start the simulator, right after the following print screen it suddenly closes... So, i can not open the simulator anymore... Any idea on what can be the problem? Cheers.
%figure
![error.png](https://cdn.discordapp.com/attachments/565154703139405824/583580479480135691/error.png)
%end

##### Mr. Scruff 05/30/2019 20:16:36
Hi `@Fabien Rohrer` .  If I simply replace « Motor wheels = [] » by « wheels = [] »

then the following Python error appears:  "NameError: name 'robot' is not defined"

##### Fabien Rohrer [Moderator] 05/30/2019 20:18:17
<@272974104523702273> did you defined the « robot » variable above?


`@Luiz Felipe` are you sure you are above the minimal requirements to run Webots and that your GPU drivers are up to date?


[https://cyberbotics.com/doc/guide/system-requirements](https://cyberbotics.com/doc/guide/system-requirements)


You could also try the safe mode: [https://cyberbotics.com/doc/guide/starting-webots#safe-mode](https://cyberbotics.com/doc/guide/starting-webots#safe-mode)

##### Mr. Scruff 05/30/2019 20:34:06
`@Fabien Rohrer` , if I define the robot variable:  « robot = Robot()» 

then I get the error: "name 'Robot' is not defined"



But, if I define the robot variable « robot = Robot()» 

and change 

 «from controller import Motor» 

to

 «from controller import Robot» 

Then everything works fine.



If the above is correct, then the tutorial example code needs to change in 2 places, to:

 «from controller import Robot»

and

« robot = Robot()»


Oh, and 

wheels = []

##### Fabien Rohrer [Moderator] 05/30/2019 20:41:48
Thank you, we will fix the tutorial soon.

##### Mr. Scruff 05/30/2019 22:48:45
Glad to be able to help 😃

##### Luiz Felipe 05/31/2019 03:16:18
Yes `@Fabien Rohrer` everything was working fine until yesterday but the safe mode did it! Thanks 😃

##### Olivier Michel [Cyberbotics] 05/31/2019 06:14:45
Hi,


<@272974104523702273>: This is already fixed here: [https://github.com/omichel/webots/pull/496](https://github.com/omichel/webots/pull/496)


Regarding the "from controller import Robot", I believe there is no need to repeat it at this point as it should already be part of your Python controller (e.g., before adding the motor API).

##### Mr. Scruff 05/31/2019 06:26:40
HI `@Olivier Michel` .   How about «from controller import Motor» ?  Should it also be removed?

##### Olivier Michel [Cyberbotics] 05/31/2019 06:28:10
Not sure whether it is strictly necessary if "from controller import Robot" was done before. Let me check...

##### Mr. Scruff 05/31/2019 06:29:43
Thanks.

I'm a bit concerned about omitting the steps that auto filled by the controller wizard.  In my opinion, it is better for the student if all code lines are included.

##### Olivier Michel [Cyberbotics] 05/31/2019 06:29:44
Indeed, it is not necessary, but in theory could be useful to benefit from 100% of the Motor API.

##### Mr. Scruff 05/31/2019 06:31:04
Don't forget to add

« robot = Robot()»

##### Olivier Michel [Cyberbotics] 05/31/2019 06:31:29
What is missing in the Python template? The "robot = Robot()" is there.


(added by the Python controller wizard)

##### Mr. Scruff 05/31/2019 06:32:40
Ah, yes.  That is the confusion about not including the statements provided by the wizard.


A recommendation then, is to add a section at the end that shows how the entire code should look like.  That will make sure there is no confusion.  Just a suggestion.

##### Olivier Michel [Cyberbotics] 05/31/2019 06:36:59
Please feel free to contribute such a suggestion on github:  [https://github.com/omichel/webots/edit/master/docs/guide/tutorial-6-4-wheels-robot.md](https://github.com/omichel/webots/edit/master/docs/guide/tutorial-6-4-wheels-robot.md)


We will quickly review your suggestion and merge it in the Webots documentation.

## June

##### Amey 06/01/2019 10:33:08
Hello,

I am actually working on my university project with Webots I have tried several  ideas to make my robot reach the target or destination but nothing worked, is there anything that I can  use so  that my robot reaches the target or the destination.

Thanks

Amey

##### Fabien Rohrer [Moderator] 06/01/2019 11:35:56
This problem is more complex that it could appear in a first glance. Computing the robot odometry could give based on motor position sensor could give good results on wheeled robots (cf. E-puck go to example in the curriculum). Cheating with a perfect GPS and Compass is another solution (cf. Moose sample). Ultimately when obstacles are present, implementing a SLAM algo may be required. You should think about using a library like gmapping.

##### smruti 06/03/2019 06:47:16
hello,i am facing a problem while opening webots.The moment i am opening a box opens and then after the webots closes.As a result i am not able to work on it.

##### David Mansolino [Cyberbotics] 06/03/2019 06:50:00
Hi `@smruti` , what yo you mean by 'a box'? Can you send us a screenshot ?

##### smruti 06/03/2019 06:54:03
after this it automatically closes
%figure
![Screenshot_2.png](https://cdn.discordapp.com/attachments/565154703139405824/584998214944555036/Screenshot_2.png)
%end

##### David Mansolino [Cyberbotics] 06/03/2019 06:57:07
That's the guided tour, it is normal that this window pops-up the first time you start Webots, what happens when you click on 'Next' ? And 'Close' ?

##### smruti 06/03/2019 07:01:41
There's no respond while clicking next or close button .It just  closes automatically.

##### David Mansolino [Cyberbotics] 06/03/2019 07:05:21
Ok, can you please:

  1. check what is you GPU?

  2. make sure the GPU drivers are up to date.

  3. try launahcing Webots in safe mode: [https://cyberbotics.com/doc/guide/starting-webots#safe-mode](https://cyberbotics.com/doc/guide/starting-webots#safe-mode)

##### Mr. Scruff 06/03/2019 14:16:47
I am using the Web Animations feature, which saves object information to a JSON file.   Is there an API available, so I can save other simulation information to this JSON file, such as GPS and IMU telemetry?

##### Olivier Michel [Cyberbotics] 06/03/2019 14:18:05
Hi, unfortunately, this is not possible out of the box.


But you can implement it from the source code of Webots if needed.


Alternatively, you may save the GPS and IMU telemetry information from your controller program into a different JSON file.


With the same time stamp as the web animation JSON.


So that you can merge the two JSON files afterwards.

##### Mr. Scruff 06/03/2019 14:24:55
Thank you `@Olivier Michel`

##### JustDN 06/03/2019 16:40:27
I am trying to program an OP2 bot and I'm confused on how to call the various motion files. I'm looking at the guide, but I'm not sure how to initiate it in the code. Sorry I'm a beginner at this.

##### David Mansolino [Cyberbotics] 06/04/2019 06:50:54
hi `@JustDN`, you need to create an instance of the motion manager:


RobotisOp2MotionManager *manager = new RobotisOp2MotionManager(robot);


then you can call the 'playPage' function:


manager->playPage(int id);


You can find the complete documentation here: [https://www.cyberbotics.com/doc/guide/robotis-op2#motion-manager](https://www.cyberbotics.com/doc/guide/robotis-op2#motion-manager)

##### Arsalan.b.r 06/04/2019 07:30:31
Hello

##### Fabien Rohrer [Moderator] 06/04/2019 07:30:46
Hi

##### Arsalan.b.r 06/04/2019 07:30:49
is Webots free?

##### Fabien Rohrer [Moderator] 06/04/2019 07:31:38
Yes it is: Since last December, Webots is completely open source (apache 2.0). There is no more licenses.


[https://cyberbotics.com/doc/blog/Webots-2019-a-release](https://cyberbotics.com/doc/blog/Webots-2019-a-release)

##### Arsalan.b.r 06/04/2019 07:32:36
as i remember it was a 1 month trial version

##### Fabien Rohrer [Moderator] 06/04/2019 07:33:54
Yes, that was before last december. Now, we dropped the license system including the trial license. Webots and all its features are open-source and free.


We provide paid user support, training and consulting for users who need to quickly develop high-quality Webots simulations, see: [https://www.cyberbotics.com/buy](https://www.cyberbotics.com/buy)

##### Arsalan.b.r 06/04/2019 07:35:25
oh i got it


its excellent. thanks Fabien

##### Fabien Rohrer [Moderator] 06/04/2019 07:35:58
you're welcome ^^

##### Xiangxiao 06/04/2019 10:28:32
Dear all

I would like to ask a question.

In Webots, it is possible that 1. Embed a panel/display/monitor in the environment. And 2. Play a video on the display in the simulated environment?

Thank you in advance.

##### Fabien Rohrer [Moderator] 06/04/2019 11:44:23
Hi


Yes, this is possible


The Display API allows to load an image as a texture in the world.


[https://www.cyberbotics.com/doc/reference/display#wb\_display\_image\_load](https://www.cyberbotics.com/doc/reference/display#wb_display_image_load)


The movie should be send image by image using this API.


However, I think this approach will be very slow.


Indeed, the Display is not optimized to stream images.


If performance is an issue, I would rather recommend to implement a MovieTexture node, but this is a complex task.

##### Xiangxiao 06/04/2019 13:08:19
Hi Fabien Rohrer , thank you very much for the information. I will check the it.


BYW, `@Fabien Rohrer`  do you know, is there anyone has done this before?

##### Fabien Rohrer [Moderator] 06/04/2019 13:18:36
I tried this once ^^


In apartment.wbt, there is a television on. A movie showing the nao is displayed.


But this is a trick, a big image is loaded at startup, and the Display only change the x-y position of the image at each step.


[https://cyberbotics.com/doc/guide/samples-environments#apartment-wbt](https://cyberbotics.com/doc/guide/samples-environments#apartment-wbt)

##### Xiangxiao 06/04/2019 13:35:44
`@Fabien Rohrer`  thank you again. It is pretty helpful. Btw, I am in EPFL and I am wondering whether you are here too?

##### Fabien Rohrer [Moderator] 06/04/2019 13:36:27
Our office is at EPFL PSE-C 😃

##### JustDN 06/04/2019 15:04:44
For the OP2, is there a way to call each individual motor? I've tried using WbDeviceTag shoulderR = wb\_robot\_get\_device("ShoulderR"); and then setting the position using wb\_motor\_set\_position(shoulderR, 10.0); but it doesn't move at all. Am I incorrectly calling the motor? If so, what is the correct way?

##### David Mansolino [Cyberbotics] 06/04/2019 15:11:39
Yes of course it is possible to control the motor individually, for this you should not use the motion or gait manager.

The code you provide seems correct, but make sur to call the wb\_robot\_step function in your main loop.

Here is a simple example of position feedback and position control with this robot: [https://github.com/omichel/webots/blob/revision/projects/robots/robotis/darwin-op/controllers/symmetry/Symmetry.cpp](https://github.com/omichel/webots/blob/revision/projects/robots/robotis/darwin-op/controllers/symmetry/Symmetry.cpp)

##### Mi 06/04/2019 15:59:29
Hello

Is it possible to control a robot through an external python script?

For example my script runs on my terminal and Webots responds to commands.

##### David Mansolino [Cyberbotics] 06/05/2019 05:35:43
HI `@Mi` , this is currently not possible,  Webots is responsible to launch the Webots script for now.

However, we did implement a new mechanism allowing you to launch yourself the controller in our development version, here is the documentation: 

[https://cyberbotics.com/doc/guide/running-extern-robot-controllers?version=develop](https://cyberbotics.com/doc/guide/running-extern-robot-controllers?version=develop)

We will release a new version of Webots including this functionnality in ~1month.

In the meantime, if you need this feature you can recompile Webots yourself from the 'develop' branch, here are the instructions:

[https://github.com/omichel/webots/wiki#installation-of-the-webots-development-environment](https://github.com/omichel/webots/wiki#installation-of-the-webots-development-environment)

##### JustDN 06/05/2019 14:45:48
I'm trying to set a motor position for my OP2 code to infinity with "shoulderl->setPosition(INFINITY);" but when running the code, I came across the error " 'INFINITY' was not declared in this scope" does that mean that the OP2 is unable to do that?

##### Fabien Rohrer [Moderator] 06/05/2019 14:46:26
Hi


No, it means that the INFINITY macro is not declared here. (INFINITY is a macro defined in the C header "math.h")


in C++, you should write infinity this way:


std::numeric\_limits<double>::infinity


[https://en.cppreference.com/w/cpp/types/numeric\_limits/infinity](https://en.cppreference.com/w/cpp/types/numeric_limits/infinity)


This deserves to include: "#include <limits>" too


There is a C++ note about this, in our documentation: [https://cyberbotics.com/doc/reference/motor#motor-functions](https://cyberbotics.com/doc/reference/motor#motor-functions)

##### JustDN 06/05/2019 14:49:45
Thank you for the help! `@Fabien Rohrer`


HI, I'm having trouble having consecutive actions to occur. I wanted my robot to raise its arm and then go back down. When programming the motors to do the actions, the arm down action overrided the arm up action so only the arm down action occured. I tried adding a sleep delay in between each action but that delay took precedent over the actions, so there is a delay at the start of the program and then the actions occurred.

##### Vinicius Aguiar 06/05/2019 16:26:20
Hello, could you guys help me?



I'm programming a robotic manipulator and wanted to move the base of it by 90 ° positive and then by 90 ° negative when I perform the print of each step of the position occurs as expected, but in the simulation it only performed the negative part, ignoring the first position .


```
####This is my code in python####

from controller import Robot, Motor
import numpy as np
import math

TIME_STEP = 64

robot = Robot()
joint1 = Motor('joint_1')


def position(signal):

   target = joint1.getTargetPosition()

   while signal != target:

       target = joint1.getTargetPosition()
       signal = round(signal,2)
       target = round(target,2)

       if signal > target:
          for i in np.arange(target,signal,0.01):
             joint1.setPosition(i)
             print("position",i)

       elif signal < target:
           for i in np.arange(target,signal,-0.01):
               joint1.setPosition(i)
               print("position",i)

while robot.step(TIME_STEP) != -1:
   # first position
   position(1.57)
   # second position
   position(-1.57)
```

##### Stefania Pedrazzi [Cyberbotics] 06/06/2019 06:01:11
`@JustDN` int the main simulation loop `while(wb_robot_ste()){}` you have to check using a PositionSensor that the first action terminated before starting the next one. You can see an example in the `robots/neuronics/ipr/worlds/ipr_collaboration.wbt` simulation. The `positionReached()` function is defined in the `robots/neuronics/ipr/libraries/ipr/IPR.cpp` source file


`@Vinicius Aguiar` your issue is very similar to the one of `@JustDN`: after setting the first position you have to wait until the position is reached before requesting the second position.

To check if the position is reached you can the PositionSensor device. The  `robots/neuronics/ipr/worlds/ipr_collaboration.wbt` I mentioned in the previous comment shows a simple way to execute consecutive actions with robotic manipulators.

##### Karl 06/06/2019 10:14:29
I'm developing a remote control and keep getting `[void] Error: remote control library entry points badly defined`... does anyone have hints why this could be? I do have an entry\_points.hpp with this:



```
extern "C" {
bool wbr_init();
void wbr_cleanup();
}
```



and an entry\_points.cpp where I implement them...

##### Fabien Rohrer [Moderator] 06/06/2019 10:14:55
Hi


Probably that the argument of wbr\_init() is missing:


bool wbr\_init(WbrInterface *ri);

##### Karl 06/06/2019 10:18:21
Ahh right, thank you!

##### bhsapiens91 06/06/2019 12:50:40
Is there a way model gear reduction on webots

##### David Mansolino [Cyberbotics] 06/06/2019 12:52:46
Hi `@bhsapiens91` unfortunately we do not have any gear reduction node in Webots yet (but it should be feasible to implement it because our physics engine does support it). However if you are using the Car node there is a gear field.

##### bhsapiens91 06/06/2019 12:54:40
I have a servo Robotic arm which has a 380:1 reduction at it's joint. Trying to make a realistic model of the hardware in simulation


Without reduction it behaves like a direct drive


Which is not the case in real world

##### David Mansolino [Cyberbotics] 06/06/2019 12:55:45
The simplest solution would be to emulate this reduction at the controller level.

Or can implement a Gear node (and we can help you of course)

##### bhsapiens91 06/06/2019 12:56:11
Let me try the gear node

##### David Mansolino [Cyberbotics] 06/06/2019 12:57:40
Ok, perfect, if you need help I would suggest to present a design (where to put this node, which fields, etc...) and present it on our Github in an issue so that we can already give you feedback.

##### bhsapiens91 06/06/2019 12:58:29
definitely

##### JustDN 06/06/2019 15:15:09
I was looking at the IPR code and I am confused on now to declare the !positionReached function. Also, would I also have to initialize my position sensors for it to work?


I'm also trying to follow the code using "mTimeStep = getBasicTimeStep();" and  "while (!positionReached(shoulderr, 1))

   step(basicTimeStep());" and I was told that I they weren't declared in the scope, I looked at the simulation code and didn't find any of the declarations, what is the right way to do the declarations in C++?

##### Fabien Rohrer [Moderator] 06/06/2019 15:39:52
Do you speak about this controller? [https://github.com/omichel/webots/blob/master/projects/robots/neuronics/ipr/controllers/ipr1\_collaboration/ipr1\_collaboration.cpp](https://github.com/omichel/webots/blob/master/projects/robots/neuronics/ipr/controllers/ipr1_collaboration/ipr1_collaboration.cpp)


This function is declared in this small library helping to control the IPR: [https://github.com/omichel/webots/blob/master/projects/robots/neuronics/ipr/libraries/ipr/IPR.hpp](https://github.com/omichel/webots/blob/master/projects/robots/neuronics/ipr/libraries/ipr/IPR.hpp)


Position sensors are enabled in this library, in the IPR constructor: [https://github.com/omichel/webots/blob/master/projects/robots/neuronics/ipr/libraries/ipr/IPR.cpp#L81](https://github.com/omichel/webots/blob/master/projects/robots/neuronics/ipr/libraries/ipr/IPR.cpp#L81)


The IPR class inherits of the Robot class.


Here is the simplest example of initialization: [https://github.com/omichel/webots/blob/master/projects/robots/neuronics/ipr/controllers/ipr\_cube/ipr\_cube.cpp](https://github.com/omichel/webots/blob/master/projects/robots/neuronics/ipr/controllers/ipr_cube/ipr_cube.cpp)


I hope this answers your question.

##### Mi 06/06/2019 19:17:58
Is there any way to run Web Webs in headless mode?

##### David Mansolino [Cyberbotics] 06/07/2019 06:34:06
`@Mi` si esta posible, pero solo podemos aydarle en inglés...

In which kind of environment would you like to run Webots? The simplest option is to use the '--batch' option when starting Webots, this will prevent Webots from opening any blocking pop-up window, and the '--minimize' option to launch Webots in minimized mode. However you still need to have a valid OpenGl context, if this is problematic there are some way to go around this, let us know.

##### Mi 06/07/2019 14:15:58
`@David Mansolino`  I want to use linux

Thank you very much for the instructions!

##### Mu.Hesham 06/08/2019 16:12:08
how can i start with webots ..

Is there any online course ?

##### Xron 06/10/2019 00:08:45
what are the formats for Display.imageNew(data,format) in Python?

##### Fabien Rohrer [Moderator] 06/11/2019 06:16:43
`@Mu.Hesham` We recommend to start with our tutorial: [https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials)


`@Xron` Formats are:


class Display (Device):

    RGB, RGBA, ARGB, BGRA


Please check the related doc: [https://cyberbotics.com/doc/reference/display?tab=python#wb\_display\_image\_new](https://cyberbotics.com/doc/reference/display?tab=python#wb_display_image_new)

##### nisuunn 06/11/2019 10:26:24
Hi

##### BlackPearl 06/11/2019 10:26:58
Hi there, Are there any Tutorials For Nao with Java ?

##### David Mansolino [Cyberbotics] 06/11/2019 10:28:47
Hi `@BlackPearl` there is no tutorial specific to the Nao robot in Java, however, our Java tutorial are valid for the Nao robot too: [https://www.cyberbotics.com/doc/guide/tutorial-1-your-first-simulation-in-webots-20-minutes?tab=java](https://www.cyberbotics.com/doc/guide/tutorial-1-your-first-simulation-in-webots-20-minutes?tab=java)

##### nisuunn 06/11/2019 10:30:32
I am using a supervisor to spawn in a copy of a robot. The copy's relative centre of mass differs form the original robot's relative COM, why does that happen and how to fix that?

##### BlackPearl 06/11/2019 10:31:23
`@David Mansolino` ok thank you anyways. Will Document it then

##### David Mansolino [Cyberbotics] 06/11/2019 10:31:58
HI `@nisuunn`, you can use the Supervisor APi to get the location and orientation of the other robot and assign it to the new one:

[https://www.cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_get\_position](https://www.cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_get_position)

[https://www.cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_field\_get\_sf\_vec3f](https://www.cyberbotics.com/doc/reference/supervisor#wb_supervisor_field_get_sf_vec3f)


`@BlackPearl`, you're welcome

##### nisuunn 06/11/2019 10:34:17
Could I also just get the original robot's centre of mass field with supervisor and apply that centre of mass to the new spawned robot?

##### David Mansolino [Cyberbotics] 06/11/2019 11:01:07
Yes, that's also a possibility. you can set the correct value for the 'translation and rotation' fields directly when you import the robot.

##### nisuunn 06/11/2019 11:02:47
how does that affect relative COM, isn't relative COM indpendent of translation and orientation/rotation of the robot?

##### David Mansolino [Cyberbotics] 06/11/2019 11:04:30
The COM depends on the origin of the robot (i.e. it's translation) and the gonfiguration of its joints.

##### nisuunn 06/11/2019 12:07:19
okay, thanks!

##### David Mansolino [Cyberbotics] 06/11/2019 12:07:35
You're welcome

##### nisuunn 06/11/2019 12:19:29
Is there any specific reason as to why the centre of mass changes when a copy of a robot is created? (the COM changes such that rather than the COM being in the ~centre of the robot, the COM shifts to the front of the robot, such that it is highly unstable compared to the original robot. )

##### David Mansolino [Cyberbotics] 06/11/2019 12:59:49
It should not, the COM should be at the same position (respectively to the robot) than the one of the other robot. Did you save the simulation after it has run? Do you have a simple procedure to reproduce the issue?

##### nisuunn 06/11/2019 13:03:20
I haven't saved after running sim.


I don't really have any simple reproduction procedure


The main body of the robot, has the coord axes at one of the 4 corners of the body, rather than the center of the body.


Could this have an influence?


even if this doesn't have an effect on the original robot's COM?

##### David Mansolino [Cyberbotics] 06/11/2019 13:05:21
It should not cause any problem


Can you send a screen shot of the 2 robot with the different COM ?

##### nisuunn 06/11/2019 13:08:58

%figure
![Screenshot_from_2019-06-11_15-08-03.png](https://cdn.discordapp.com/attachments/565154703139405824/587991670729998346/Screenshot_from_2019-06-11_15-08-03.png)
%end



%figure
![Screenshot_from_2019-06-11_15-08-35.png](https://cdn.discordapp.com/attachments/565154703139405824/587991694990114816/Screenshot_from_2019-06-11_15-08-35.png)
%end


The robot on the right has the wrong COM, so it leans forward

##### David Mansolino [Cyberbotics] 06/11/2019 13:11:37
Ok I see, there is indeed an important difference. It is difficult to see where does the difference come from, would you mind sending us your simulation at support@cyberbotics.com so that we can better check what is happenning (we will of course keep it confidential)?

##### nisuunn 06/11/2019 13:12:07
I will send it there, thank you

##### David Mansolino [Cyberbotics] 06/11/2019 13:12:17
Perfect, thank you

##### nisuunn 06/11/2019 13:18:43
Sent

##### David Mansolino [Cyberbotics] 06/11/2019 13:25:16
I confirm we just received it, we will check what is happenning and let you know soon.

##### Deleted User 06/11/2019 14:10:50
how do I run webots in ubuntu?

##### nisuunn 06/11/2019 14:11:10
go into webots file with terminal


then type ./webots

##### Deleted User 06/11/2019 14:11:21
which webots file


or do you mean directory?


if so which directory?

##### nisuunn 06/11/2019 14:11:31
yep

##### Deleted User 06/11/2019 14:11:43
ok so we have several directories


I can just pick one at random or something?


none of them work


tried


😦

##### nisuunn 06/11/2019 14:12:21
at least for me I enter the webots directory and no further than that


the directory in which you can see bin, change\_logs, etc

##### Deleted User 06/11/2019 14:13:15
ok


one moment



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/588008523359584278/unknown.png)
%end


its not working?


I mostly get permission denied

##### nisuunn 06/11/2019 14:18:09
strange, for me the exact same procedure works

##### Fabien Rohrer [Moderator] 06/11/2019 14:19:04
Could you try an "ls -lF" in this directory?

##### Deleted User 06/11/2019 14:22:55

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/588010278382338050/unknown.png)
%end



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/588010790947258368/unknown.png)
%end


maybe webots does not work in linux?


I dunno

##### nisuunn 06/11/2019 14:25:25
works on my linux

##### Deleted User 06/11/2019 14:25:30
not on mine


even doing chmod


and ultimate sudo mode

##### Fabien Rohrer [Moderator] 06/11/2019 14:26:59
the root webots launcher does not have the execution rights


ok


the chmod seems to let you go a step beyond


Please try to export the WEBOTS\_HOME environment variable.

##### Deleted User 06/11/2019 14:28:14
no idea how you do that


I think I will just and run this on windows instead

##### Fabien Rohrer [Moderator] 06/11/2019 14:28:21
export WEBOTS\_HOME=/home/somet/Desktop/webots

##### Deleted User 06/11/2019 14:28:49
even after typiung that


makes no difference

##### Fabien Rohrer [Moderator] 06/11/2019 14:29:06
I think you didn't extracted correctly the webots archive.


ok, try this:

##### Deleted User 06/11/2019 14:29:41
yeah I had issues with that


I will try windows I reckon

##### Fabien Rohrer [Moderator] 06/11/2019 14:29:51
export LD\_LIBRARY\_PATH=/home/somet/Desktop/webots/lib


Reading your last terminal output, it seems that the archive extraction failed.

##### Deleted User 06/11/2019 14:30:51
dw about it


yeah I am using a VM


and it has trouble allocating resources for extracting for some reason

##### Fabien Rohrer [Moderator] 06/11/2019 14:31:51
You should retry to download and extract the archive.


.. and use this command: [https://cyberbotics.com/doc/guide/installation-procedure#from-the-tarball-package](https://cyberbotics.com/doc/guide/installation-procedure#from-the-tarball-package)

##### Deleted User 06/11/2019 14:33:22
is there anyway to do portable in windows


in that VM I had only 3 cores


in windows I can allocate around 16


so should be fine


because its a vm its limited for heavy tasks

##### Fabien Rohrer [Moderator] 06/11/2019 14:34:27
cores are a thing, but support of hardware acceleration is more important to run Webots smoothly.


running Webots natively is highly recommended.


(but I run Webots everyday on VMWare fusion, with 4 cores 😉 )

##### Deleted User 06/11/2019 14:36:57
yeah I am using vmware


I mean I just cba with it lol



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/588013981164109828/unknown.png)
%end


is this safe to run?


I downloaded windows setup exe


from


[https://www.cyberbotics.com/#download](https://www.cyberbotics.com/#download)

##### Fabien Rohrer [Moderator] 06/11/2019 14:38:12
Yes it is safe


To remove this warning, we have to pay Microsoft... This is really annoying.


Same on macOS.


You'll have the same installing other OpenSource software

##### Deleted User 06/11/2019 14:41:56
what like there app store program thing?


I think I paid that...


I have a license on all stores usually that don't require subscription


so only one is Apple as they charge yearly which is annoying

##### Fabien Rohrer [Moderator] 06/11/2019 14:42:43
yes, that's true, thank you

##### Deleted User 06/11/2019 14:48:17
```WARNING: System below the minimal requirements.

Webots has detected that your GPU has less than 2Gb of memory. A minimum of 2Gb of memory is recommended to use high-resolution textures. 

 - Texture quality has been reduced.

You can try to re-activate some OpenGL features from the Webots preferences.
```


I get this


but I have two graphics cards in my system


each at 8GB each

##### el\_samu\_el 06/11/2019 14:54:31
I'm always getting this error when trying to get a node from DEF with a supervisor. I had this issue before, but I can't remember how it got solved. The error message isn't helpful at all. Error: [tcpip\_client]   File "/home/samuel/webots/lib/python36/controller.py", line 2679, in <lambda>

[tcpip\_client]     \_\_getattr\_\_ = lambda self, name: \_swig\_getattr(self, Supervisor, name)

[tcpip\_client]   File "/home/samuel/webots/lib/python36/controller.py", line 74, in \_swig\_getattr

[tcpip\_client]     return \_swig\_getattr\_nondynamic(self, class\_type, name, 0)

[tcpip\_client]   File "/home/samuel/webots/lib/python36/controller.py", line 69, in \_swig\_getattr\_nondynamic

[tcpip\_client]     return object.\_\_getattr\_\_(self, name)

[tcpip\_client] AttributeError: type object 'object' has no attribute '\_\_getattr\_\_'


tcpip\_client is my supervisor controller


The error happens after calling sv = Supervisor()

    robot = sv.getFromDev("robot")


I assigned the name robot to my DEF node and the supervisor node is set to TRUE

##### Fabien Rohrer [Moderator] 06/11/2019 15:02:09
<@302517149359144962> Ok it seems to work better on Windows then ^^ The warning is "regular": it appears when Webots detects low-end GPU (including VM).


`@el_samu_el` getFromDev does not exits.


is it a typo?


(It's "getFromDef")


The error is indeed cryptic

##### el\_samu\_el 06/11/2019 15:04:07
yeah but the error is still there

##### Fabien Rohrer [Moderator] 06/11/2019 15:04:28
It's because we use SWIG to generate the Python library

##### el\_samu\_el 06/11/2019 15:04:29
wait nvm

##### Fabien Rohrer [Moderator] 06/11/2019 15:05:05
but the lines shown in the error should help you

##### el\_samu\_el 06/11/2019 15:05:35
Thanks !

##### JustDN 06/11/2019 15:50:34
Does webots come with OpenCV?

##### TH0 06/11/2019 17:04:12
Hi, is it possible to generate binary pixel wise appearance masks of an object in an camera image with webots?

I already used the webots Recognition-Node to draw bounding boxes around objects. but i'm looking for a pixelwise segmentation to generate ground truth masks (something like in the image below)
%figure
![mask.png](https://cdn.discordapp.com/attachments/565154703139405824/588050868872675368/mask.png)
%end

##### David Mansolino [Cyberbotics] 06/12/2019 06:37:16
@THO, this is not currently possible, however, we have a prototype of such feature, see this video at 1:09:

[https://www.youtube.com/watch?time\_continue=1&v=O7U3sX\_ubGc](https://www.youtube.com/watch?time_continue=1&v=O7U3sX_ubGc)

If you want we can either send you a quote for finalizing and integrating this prototype into Webots for you or you can do it by yourself directly (we can guide you if you open a pull request on our Github repository).


`@JustDN`, yes we have an example of controller using OpenCV, please see 'projects/samples/robotbenchmark/visual\_tracking'.

But more generally speaking, you can link your controller with any third party library installed on your computer, here is an explanation for C/C++: [https://cyberbotics.com/doc/guide/using-webots-makefiles#adding-an-external-library-ccp](https://cyberbotics.com/doc/guide/using-webots-makefiles#adding-an-external-library-ccp)

##### TH0 06/12/2019 10:54:48
thanks, the prototyp looks nice! i've send you a pm

##### nisuunn 06/12/2019 12:43:14
Hi, yesterday I had an issue with the center of mass of a robot, has there been any progress with regards to this issue? thanks

##### David Mansolino [Cyberbotics] 06/12/2019 12:52:07
Hi, we are still investigating, there is indeed an issue, but it is more difficult than expected to find exactly where it does come from.


I will let you know (by email) when we have some news.

##### nisuunn 06/12/2019 12:52:52
Thank you very much.

##### JustDN 06/12/2019 15:36:25
Does webots come with python.exe? I was running the visual tracking sample and the code failed because that was missing.


Are there any sample projects or worlds that has reinforcement learning?

##### David Mansolino [Cyberbotics] 06/12/2019 15:37:55
No, you have to install it independently of Webots: [https://www.cyberbotics.com/doc/guide/using-python](https://www.cyberbotics.com/doc/guide/using-python)

##### nisuunn 06/12/2019 16:30:33
Hi, I'm having trouble getting a handle to a GPS device with the name "gps" in a supervisor (in Python) (the gps is in the children field of a robot)

I try the following: gps1 = GPS("gps") 

and I get a device not found warning


whats the correct way of declaring a gps handle?

##### Stefania Pedrazzi [Cyberbotics] 06/13/2019 06:03:47
Hi `@nisuunn` , you have to use the Robot function `Robot.getGPS("gps")`. Please refer to the documentation [https://www.cyberbotics.com/doc/reference/robot?tab=python#getgps](https://www.cyberbotics.com/doc/reference/robot?tab=python#getgps).

##### nisuunn 06/13/2019 11:05:32
Okay, thanks!

##### nick\_robot\_maker 06/13/2019 11:16:46
hello, do you know the name of lidar sensor of sample's of webot?


i want to know the name of lidar sensor that used webot's sample lidar


if you guys know about it, tell me please

##### Fabien Rohrer [Moderator] 06/13/2019 11:21:45
Hi,


You can know the device names by opening the robot window (double click on the robot).


In your case, it's probably "lidar", the default value of the Lidar.name field.


Do you speak about lidar.wbt?

##### nick\_robot\_maker 06/13/2019 11:23:30
yes


the world


it is named lidar.wbt


and i try to know what is lidar sensor's model name that using this sample world

##### Fabien Rohrer [Moderator] 06/13/2019 11:25:43
Ok, it's simply a generic lidar, no model

##### nick\_robot\_maker 06/13/2019 11:27:04
is it? thank a lot. but i found the officail homepage and find ladar sensor's models.


like SICK LD-MRS, SICK LMS 291, Hokuyo URG-04LX-UG01

##### Fabien Rohrer [Moderator] 06/13/2019 11:27:34

%figure
![Capture_decran_2019-06-13_a_13.26.36.png](https://cdn.discordapp.com/attachments/565154703139405824/588690928705732618/Capture_decran_2019-06-13_a_13.26.36.png)
%end

##### nick\_robot\_maker 06/13/2019 11:27:52
yes. this is the world

##### Fabien Rohrer [Moderator] 06/13/2019 11:28:01
Just look at the robot definition (the red rectangle in my screenshot).


It's directly a Lidar node.


The non-default parameters are shown in green.


[https://cyberbotics.com/doc/reference/lidar](https://cyberbotics.com/doc/reference/lidar)

##### nick\_robot\_maker 06/13/2019 11:30:54
um.. then if i want to make new world and want to use this lidar model, what can i do?


you means this lidar sensor is just default sensor?

##### Fabien Rohrer [Moderator] 06/13/2019 11:52:22
You could simply copy the Lidar node, and paste it in another robot.


Yes, this demo is very basic. It shows how to use the Lidar node.


If you would like to use an existing Lidar model, just add a node to your robot, from the GUI, you can select a Sick, etc.


But every existing lidar model (like Sick) are in fact a Lidar node.

##### nick\_robot\_maker 06/13/2019 12:26:09
very good. thank you so much. could you show me how to  choose the sensor to add robot please? i can't find any sensor.

##### Fabien Rohrer [Moderator] 06/13/2019 12:28:02
Just select your Robot.children field, and click on the "+" button, the sick sensors are available in the "Add node dialog..."



%figure
![Capture_decran_2019-06-13_a_14.28.44.png](https://cdn.discordapp.com/attachments/565154703139405824/588706383541567489/Capture_decran_2019-06-13_a_14.28.44.png)
%end

##### el\_samu\_el 06/13/2019 14:28:56
Hey guys, is there a possibility to pass arguments to the controller when starting webots from the command line?

##### Fabien Rohrer [Moderator] 06/13/2019 14:29:07
Hi


Yes sure, see Robot.controllerArgs field

##### el\_samu\_el 06/13/2019 14:29:38
Ah, is that also possible with python?

##### Fabien Rohrer [Moderator] 06/13/2019 14:29:45
yes


you can retrieve them with sys.argv in the controller

##### el\_samu\_el 06/13/2019 14:30:16
okay, I'll have a look. Thanks again Fabien !


Ah but as I understand this it just get's arguments from the .wbt file

##### Fabien Rohrer [Moderator] 06/13/2019 14:34:21
😃 I was sure of this

##### el\_samu\_el 06/13/2019 14:34:36
or does it also work in some way like "webots --mode=fast "example.wbt" "controllerArg"

##### Fabien Rohrer [Moderator] 06/13/2019 14:34:51
No, you can't pass arguments like this.


We assume you master your environment when launching Webots.


So you can for example create a file before launching Webots, and read it in the controllers.


Or, to create a template.wbt, and forge it before launching Webots on it (and change controllerArgs for example)

##### el\_samu\_el 06/13/2019 14:37:29
That's true, but if I want to startup multiple webots processes with different controllers, but the same world file?


yeah the last option you mention, was what I was also thinking

##### Fabien Rohrer [Moderator] 06/13/2019 14:40:19
With several instances, the two options above are still valid (Webots will read the state when it is launched), but in this case, you probably have to be sure that Webots read the right value by implementing a callback mechanism.


In this case, there are certainly better solutions.


For example, the script launching Webots could also have a TCP server, the Webots controllers could be TCP clients, and get the info from there. It has the advantage to be persistent (results could be sent the same way later). And it's a piece of cake to do that in Python 😃


Does this make sense for your expectations?

##### el\_samu\_el 06/13/2019 14:49:04
I made that TCP server already, but there were some issues with it.

##### Fabien Rohrer [Moderator] 06/13/2019 14:49:27
It's a good starting point for sure

##### el\_samu\_el 06/13/2019 14:49:41
So I was trying to workaround this by simply starting several processes and restarting them as often as it is written in a file


But I need to start these processes sometimes with different configurations, thus the controller argument.

##### Fabien Rohrer [Moderator] 06/13/2019 14:50:42
Ok


then probably the template thing is the best

##### el\_samu\_el 06/13/2019 14:51:10
would you use the selector module for TCP in python?

##### Fabien Rohrer [Moderator] 06/13/2019 14:51:24
you could forge a world with a different name for each instance.


I'm not sure exactly about how to implement the TCP client-server, sorry.

##### el\_samu\_el 06/13/2019 14:54:08
Okay, yeah I am new to networking and related stuff in general.  But gave it anyway a try. It's not that easy I think. (For someone with experience it probably is)


Thanks for your help once again!

##### Fabien Rohrer [Moderator] 06/13/2019 14:55:33
I have an example using a template engine if you would like, I can share it with  you in private.

##### Lefa 06/13/2019 15:20:34
Hey guys , i am stuck on a project using webots to avoid obstacle untill reached the desired goal , can anyone help me ?

##### David Mansolino [Cyberbotics] 06/13/2019 15:20:59
Hi


what is your problem ?

##### Lefa 06/13/2019 15:22:01
i need to make a project of a robot in unknown map have a target to reach with avoiding any obstavcles in its way

##### David Mansolino [Cyberbotics] 06/13/2019 15:23:12
I am sorry but this is not striclty related to Webots, but more to robotic in general, this chat is only for Webots related questions


We have developed a benchmark similar to your task, maybe this can help you: [https://robotbenchmark.net/benchmark/obstacle\_avoidance/](https://robotbenchmark.net/benchmark/obstacle_avoidance/)


You might also want to ask for help here: [https://robotics.stackexchange.com/](https://robotics.stackexchange.com/)

##### nisuunn 06/14/2019 13:31:06
Hi, my robot has springs around its feet or knees. They work as expected, except for when the robot is upside down. When the robot is upside down, for some reason the springs move in the opposite direction that you'd expect, why is that?


The spring is a slider joint


with a spring constant

##### Olivier Michel [Cyberbotics] 06/14/2019 13:32:45
Hi, could that be because of gravity?

##### nisuunn 06/14/2019 13:33:23
Perhaps, but if the robot is not upside down, then the springs manage to support the mass of the robot, hence I would expect the springs to not be pulled down by only gravity when upside down

##### Olivier Michel [Cyberbotics] 06/14/2019 13:33:27
Did you try to play with the WorldInfo.gravity value to see how it affects the behavior?

##### nisuunn 06/14/2019 13:33:36
Ill try it out


lowering the gravity to around 3 still pulls the springs down when upside down


or down to 0.5 as well

##### Olivier Michel [Cyberbotics] 06/14/2019 13:37:05
What about reversing the gravity e.g, +9.81 instead of -9.81?


And setting the gravity to 0?

##### nisuunn 06/14/2019 13:40:01
gravity 0: still the same


at +9.81  springs behave as expected i think


even when upside down

##### Olivier Michel [Cyberbotics] 06/14/2019 13:41:15
Ok, that means it is probably not related with gravity...


Can you share a movie showing the problem you have?

##### nisuunn 06/14/2019 13:49:22

> **Attachment**: [out.ogv](https://cdn.discordapp.com/attachments/565154703139405824/589089001751969811/out.ogv)


Here is

##### Olivier Michel [Cyberbotics] 06/14/2019 13:56:57
Thank you. However, it is not very clear to me what the problem is. Could you create a minimalist simulation (just one joint) showing the problem?

##### nisuunn 06/14/2019 13:58:22
In the video: when the robot is the correct way up, its upper legs are supported by the springs that connect the feet and upper leg


however when upside down, as can be see from the video, the springs compress


and allow the feet to drop down


but i would expect the springs to support the feet when upside down, as they were able to support the entire robot's mass when the correct way up

##### Olivier Michel [Cyberbotics] 06/14/2019 14:01:02
I see, but it would be interesting to see if you can reproduce the same problem with a single hinge joint which has the same spring property.

##### nisuunn 06/14/2019 14:01:27
do you mean with a single slider joint, rather than hinge?

##### Olivier Michel [Cyberbotics] 06/14/2019 14:03:13
Yes.

##### nisuunn 06/14/2019 14:30:54
I'm unable to do it, but it's no issue in my case so it's okay.


In the simpler case I tried it in, the spring behaved as expected I think


even when upside down

##### Olivier Michel [Cyberbotics] 06/14/2019 14:32:22
So, you should probably try to complexify your simple example until you get this supposedly wrong behavior and you will likely find the culprit...

##### nisuunn 06/14/2019 14:32:39
yep, thanks for the help and suggestions.

##### Olivier Michel [Cyberbotics] 06/14/2019 14:32:48
You are welcome.

##### sai 06/14/2019 21:53:30
Hello, I have a question in webots world files. Can we use variables to store and use float values in world files? For example I have multiple objects in the scene and using a variable I want to change the position of all objects by the same distance.

##### MohamedSabry 06/15/2019 14:26:14
Hello, I am new to webots and I have a license problem


I just installed it, followed the tutorial on  [https://cyberbotics.com/doc/guide/tutorial-1-your-first-simulation-in-webots-20-minutes?tab=matlab](https://cyberbotics.com/doc/guide/tutorial-1-your-first-simulation-in-webots-20-minutes?tab=matlab) and whenever I start and run the simulation


error requested licenses (webots edu or pro) not available. no license pack


Does anyone have an idea how to solve it?


I followed the installation steps and there isn't anything stating that I should have a license to verify my software after installation


`@Olivier Michel`

##### TH0 06/16/2019 11:28:42
Hi, i have a problem running webots in the windows subsystem for linux:

After starting webots, the gui is shown for part of a second. webots crashed after that with the error:



nao@DESKTOP-O5VO47I:~/Downloads/webots$ ./webots

Warning: QStandardPaths: XDG\_RUNTIME\_DIR not set, defaulting to '/tmp/runtime-nao'

shared memfd open() failed: Function not implemented

pcilib: Cannot open /proc/bus/pci

pcilib: Cannot find any working access method.



(the memfd error appears after we installed pulseaudio because we thought it was an error with the sound device. but the pcilib error is still there)

##### David Mansolino [Cyberbotics] 06/17/2019 06:27:04
`@MohamedSabry` Which version of Webots are you using? As Webots is now open-source you should not have any problem with licenses if you use the latest one.


`@sai` No this is unfortunately not possible. Howver, you can encapsulate all your objects in a PROTO file (in which you can use our template engine) to do this: [https://www.cyberbotics.com/doc/reference/proto](https://www.cyberbotics.com/doc/reference/proto)

Alternatively you can use a Supervisor to change the position of the objects at first step (or any other step): [https://www.cyberbotics.com/doc/reference/supervisor](https://www.cyberbotics.com/doc/reference/supervisor)


`@TH0` wouldn't it be simpler to use the Windows version of Webots directly if you are on Windows?

##### MohamedSabry 06/17/2019 07:02:33
`@David Mansolino` Yes I realized that, but when I installed the latest version, it always crashes even though I updated my graphics drives and made sure to use my "Nvidia Geforce 840M" as the graphics card responsible for the running of the application, so what should I do?

##### David Mansolino [Cyberbotics] 06/17/2019 07:03:35
`@MohamedSabry` have you tried using the safe mode?

 [https://cyberbotics.com/doc/guide/starting-webots#safe-mode](https://cyberbotics.com/doc/guide/starting-webots#safe-mode)

##### MohamedSabry 06/17/2019 07:07:34
`@David Mansolino` thank you so so much, it is working now

##### David Mansolino [Cyberbotics] 06/17/2019 07:08:01
You're welcome.

##### TH0 06/17/2019 09:28:36
`@David Mansolino`  Our Robot Controller uses Unix Domain Sockets to emulate the Lola-Interface from a Softbank Nao V6, so it does not work on Windows.

I ask the question about WSL because we also tested Gazebo and it works without problems in WSL and we wonder that webots doesnt and hope it's maybe only a small fix needed.

##### David Mansolino [Cyberbotics] 06/17/2019 11:52:32
`@Deleted User` Webots can probably work in WSL too (if I remember correctly other users have been able to let it work) but it might be tricky to setup. Won't it be simpler to run it directly under Linux in that case ?

##### TH0 06/17/2019 13:17:49
i know what you mean and it's already working in native linux but some of our team members do not use linux as the native OS. So the idea was to "simply" run webots under WSL for development independent from the native OS.

you say: "other users have been able to let it work". do you know who or where to find the tricks for the setup? 😃

##### David Mansolino [Cyberbotics] 06/17/2019 13:29:38
Here for example a user reported on our old forum that he successfully managed to let Webots work in WSL (he had problem with ROS but this is another topic): [https://www.cyberbotics.com/forum/forum?message=8265](https://www.cyberbotics.com/forum/forum?message=8265)

##### JustDN 06/17/2019 14:26:05
Are there any simulations that uses reinforcement learning?

##### Olivier Michel [Cyberbotics] 06/17/2019 15:26:52
Hi, not in the examples provided in Webots. It is up to you to implement it, like many Webots users did.

##### Xron 06/17/2019 17:11:43
Hello I am trying to use OpenCV(Python) but the image captured from the camera object is in a different format from OpenCV. How can I convert between formats?

##### Fabien Rohrer [Moderator] 06/18/2019 06:35:29
`@Xron` You could refer to the following example/snippets:


[https://cyberbotics.com/forum?message=8666](https://cyberbotics.com/forum?message=8666) Here is a small snippet to convert the camera image to a numpy array (the input of Python OpenCV)


It's in C++, but this released example does similar things: [https://cyberbotics.com/doc/guide/samples-howto#vision-wbt](https://cyberbotics.com/doc/guide/samples-howto#vision-wbt)

##### DimitrisK 06/18/2019 10:15:59
I am trying to apply reinforcement learning in the "obstacle avoidance" environment (as seen in robotbenchmark). After a number of iterations (about 120 episodes,with 800 robot steps each) the Thymio2 is literaly falling apart. I can see that gradually the wheels of the robot are starting to stick out of the body more and more,until it eventually falls apart and the physics stop making sense. I am reseting the physics in each  step but doesn't seem to change anything. What could be the problem? Could it be that the robot is "breaking" due to the colision happening during the training? And if so,how can i stop that from happening?

##### David Mansolino [Cyberbotics] 06/18/2019 10:49:40
Hi `@DimitrisK`, that is indeed possible that very small physic errors accumulate over the iterations and can cause problems after a high number of iterations.

To work around the issue, you can revert the simulation from your supervisor controller or if you don't want your supervisor controller to restart, you can, after a defined number of iterations (e.g. 50) remove and re-import the Thymio2 robot in the simulation, this way you will have a 'brand new robot' without any accumulated errors.

##### DimitrisK 06/18/2019 10:57:54
I though of that as a possible solution. Can you give me a hint on how tp do that? My robot is the Supervisor so i don't know if removing it could cause problems...

##### David Mansolino [Cyberbotics] 06/18/2019 10:58:48
You mean the Thmio2 robot itself is the supervisor ?

##### DimitrisK 06/18/2019 11:00:40
Yes exactly

##### David Mansolino [Cyberbotics] 06/18/2019 11:01:35
In that case the problem is not removing it, but once removed you won't be able to re-add it because you don't have any supervisor anymore...


I would recommend to split your robot and your supervisor controllers. To do so you can of course have a supervisor without any physics/shape/etc.

##### DimitrisK 06/18/2019 11:05:20
Is there an example of this somewhere? On how to use two controllers i mean

##### David Mansolino [Cyberbotics] 06/18/2019 11:06:08
This is documented here: [https://www.cyberbotics.com/doc/guide/using-numerical-optimization-methods#using-two-distinct-types-of-controllers](https://www.cyberbotics.com/doc/guide/using-numerical-optimization-methods#using-two-distinct-types-of-controllers)

##### DimitrisK 06/18/2019 11:07:06
Ok thank you very much! I 'll  make the changes in order to make this work

##### Karl 06/18/2019 11:14:10
I'm developing a remote control and wbr\_init seems to be correctly called from entry\_points . There I link the interface functions to my own (like `ri->mandatory.wbr_start = Wrapper::start;`). However, these functions never seem to be called... also, when running it in WeBots, the simulation still seems to be used. Do I have to tell WeBots to use the real-life remote control somehow? Other than setting it in the remote\_control field of course


Or is 'running the controller', as described in the remote control docs, different from 'running the simulation'?

##### Fabien Rohrer [Moderator] 06/18/2019 11:37:46
In addition to set correctly the Robot.remoteControl field (as you mention), the remote control library is used when the wb\_robot\_set\_mode(WB\_MODE\_REMOTE\_CONTROL) is explicitly called in he controller:


[https://cyberbotics.com/doc/reference/robot#wb\_robot\_get\_mode](https://cyberbotics.com/doc/reference/robot#wb_robot_get_mode)


This allows to switch to the remote control library after some event, typically a GUI button

##### Karl 06/18/2019 11:40:20
Ahh right!


That was it


Thanks a lot! Perhaps it would be good to add that to the remote control docs

##### Fabien Rohrer [Moderator] 06/18/2019 11:42:27
Do you think the references to wb\_robot\_set\_mode in the following page are not enough? [https://www.cyberbotics.com/doc/guide/controller-plugin](https://www.cyberbotics.com/doc/guide/controller-plugin)


If not, I can improve it right now

##### Karl 06/18/2019 11:54:00
I think it would be helpful to mention the WB\_MODE\_REMOTE\_CONTROL argument


You're right, it should be possible to figure it out as it is, but I think we would've had an easier time with that mention

##### Fabien Rohrer [Moderator] 06/18/2019 11:55:09
I'm trying something, please stay online ^^


Probably better like this? [https://github.com/omichel/webots/pull/585](https://github.com/omichel/webots/pull/585)

##### Karl 06/18/2019 12:06:51
Yes thats perfect!

##### el\_samu\_el 06/18/2019 12:06:56
I'm not really understanding this error code. Isn't the first argument here "self" and thus a Supervisor. My class is inheriting SV. Why the type error?
%figure
![Screenshot_from_2019-06-18_13-00-51.png](https://cdn.discordapp.com/attachments/565154703139405824/590512773780340766/Screenshot_from_2019-06-18_13-00-51.png)
%end


self.getFromDef("motorname")

##### Fabien Rohrer [Moderator] 06/18/2019 12:07:25
`@Karl` ok, it will be released with the new release, coming soon.


`@el_samu_el` The error is more related with the argument passed to getFromDef


A Python String is expected, and something else is given


Could you "print(n)" just before line 26? This should be a string, but it is certainly not.

##### el\_samu\_el 06/18/2019 12:17:39
yeah it is a string


this error does not happen when I create a supervisor instance and then use this one to call the method

##### Fabien Rohrer [Moderator] 06/18/2019 12:18:18
oh sorry, I'm maybe wrong

##### el\_samu\_el 06/18/2019 12:18:20
can't share the picture right now in discord


but I printed it

##### Fabien Rohrer [Moderator] 06/18/2019 12:19:14
you're right, it's an issue with the Supervisor class

##### el\_samu\_el 06/18/2019 12:19:18
I can get around this error by just creating a single supervisor instance, but I wonder why inheritance does not work.


Maybe cause the python api asserts that the class name is correct

##### Fabien Rohrer [Moderator] 06/18/2019 12:20:03
Is the Supervisor constructor correctly called?


The \_\_init\_\_ deserve certaily a call to the super() function:


Add "super().\_\_init\_\_()" on the first line of the "\_\_init\_\_" cfunction content.


This is a Python 3 statement.


In Python 2 it's: "Robot\_Environment.\_\_init\_\_(self)"


[https://stackoverflow.com/questions/576169/understanding-python-super-with-init-methods](https://stackoverflow.com/questions/576169/understanding-python-super-with-init-methods)

##### el\_samu\_el 06/18/2019 12:26:45
[RL\_controller]     super().init()

[RL\_controller] AttributeError: 'super' object has no attribute 'init'

##### Fabien Rohrer [Moderator] 06/18/2019 12:27:16
Sorry, Discord has formated my answer:


init is prefixed and suffixed by 2 `_`

##### el\_samu\_el 06/18/2019 12:28:27
you were right, this solves it. I assumed python calls \_\_init\_\_ automatically.

##### Fabien Rohrer [Moderator] 06/18/2019 12:28:42
Great

##### el\_samu\_el 06/18/2019 12:29:31
thanks again !

##### Akash 06/19/2019 00:05:05
Can get a proper lookup table chart values for 'Compass'. The lookup table I have made it is having three vector values -1, 0 and 1, and three response values 0, 90 and 180. When I rotate the bot on 0, 90 or 180 degree the function compass.getValues() giving proper response/output values of 0, 90 and 180 respectively, but while I rotate it to any other degree output values are not as expected like on 45 degree the function is giving a output value of 23. Is there any proper lookup table chart by which  on 45 degree a output value of 45 can be achieved?

##### Stefania Pedrazzi [Cyberbotics] 06/19/2019 06:29:03
`@Akash` the Compass returned value is the direction of virtual north so you cannot perform the conversion directly using the lookup table becaue the returned value for a 45 degrees rotations is [0.7075, 0, 0.7075]. Instead you should compute the angle in your controller using the `get_bearing_in_degress()` function described in the Reference Manual:  [https://www.cyberbotics.com/doc/reference/compass#wb\_compass\_get\_values](https://www.cyberbotics.com/doc/reference/compass#wb_compass_get_values)

##### Akash 06/19/2019 10:44:01
In the function get\_bearing\_in\_degress() there is a variable M\_PI. This variable is also used in other functions but I can find its value. What is the value of  M\_PI ?

##### Olivier Michel [Cyberbotics] 06/19/2019 10:44:37
3.141592653589793238462643383279502884197169399375105820974944592307816406286


It's a standard C constant defined in math.h

##### Akash 06/19/2019 11:09:59
I wrote the function get\_bearing\_in\_degrees() in  python, set the vaule of M\_IP to 3.14...6, and the lookup table I have made it is having three vector values -1, 0 and 1, and three response values 0, 90 and 180 respectively. The output values are not proper it is varying from 270 to 360 only. Any solution so that I can get proper values in the output, like on 56 degree the output will be 56 or on 219 degree output will be 219?

##### Fabien Rohrer [Moderator] 06/19/2019 11:59:57
It's probably due to your algorithm.


Did you thought to use a InerialUnit instead of a Compass? It can return the roll/pitch/yaw, yaw is basically what you need.

##### DimitrisK 06/19/2019 13:28:59
Hello! Can you give me an example of how to use  simulationQuit(variable)  in python? Specifically,what variable do i use  in the parenthesis?

##### David Mansolino [Cyberbotics] 06/19/2019 13:30:44
Hi `@DimitrisK`,  you can put any integer value, this is the exit code ([https://cyberbotics.com/doc/reference/supervisor?tab=python#wb\_supervisor\_simulation\_quit](https://cyberbotics.com/doc/reference/supervisor?tab=python#wb_supervisor_simulation_quit)), you probably want to exit with 0 in case of success and 1 in case of failure

##### Heller Drake 06/19/2019 13:53:37
Good evening everyone! I'm looking for a way to launch Webots in batch mode BUT without GUI. Unfortunately I've been unable to find any clue throughout the online documentation. So I was wondering if there is any way to genuinely achieve it, even with a console trick.



PS: I hope to be in the right section

##### Fabien Rohrer [Moderator] 06/19/2019 13:54:09
Hi,


Try "./webots --minimize --batch --stdout --stderr"


You can have more information about these options here: [https://cyberbotics.com/doc/guide/starting-webots#command-line-arguments](https://cyberbotics.com/doc/guide/starting-webots#command-line-arguments)

##### Heller Drake 06/19/2019 14:02:44
I already took a look into the cli options but none of them has the wanted effect (that is without having the Webots icon popping up on the application bar), sadly.



For now I'll stick with redirecting stdout and stderr to console as you have suggested!



Thank you very much for your time! 😃

##### Fabien Rohrer [Moderator] 06/19/2019 14:29:15
Unfortunately Webots needs a GUI, we don't have better solutions for now.


You're welcome

##### nisuunn 06/19/2019 14:30:55
Hi, I am trying to use an emitter to send data to a physics plugin. (controller is in python).


Right now im trying out:


structPacked = struct.pack(b"chd")

    emit1.send(structPacked)


in the controller


and


int* emitter;

   int size;

   emitter = (int*) dWebotsReceive(&size);

   dWebotsConsolePrintf("%s\n", emitter[0]);


in the physics plugin's webots\_physics\_step loop


This crashes my webots


What is the correct way of sending a message?

##### Olivier Michel [Cyberbotics] 06/19/2019 14:33:06
You should send plain text from your Python controller and read plain text from the physics plugin.


int *emitter; seems wrong


It should be "char *emitter\_data"


dWebotsConsolePrintf("%s\n", emitter[0]); seems wrong

##### nisuunn 06/19/2019 14:34:02
but if i was sending int instead of char, then I'd still use int?

##### Olivier Michel [Cyberbotics] 06/19/2019 14:34:03
it should be dWebotsConsolePrintf("%s\n", emitter\_data);


Your printf is wrong anyhow, it should be:


dWebotsConsolePrintf("%d\n", emitter[0]);


if passing a int *


(note %d instead of %s)


This is a basic C programming question and is not related with Webots.

##### nisuunn 06/19/2019 14:37:08
Yes, thank you

##### fa 06/19/2019 17:04:13
Is it possible to run Webots only in web browser? I don't wanna launch Webots, even in minimized mode

##### Akash 06/19/2019 18:34:03
Is there any example file available in webots where compass device is been used?

##### adhitthana 06/19/2019 21:30:50
`@Heller Drake`  on linux it is possible to run a dummy x server and  launch webots on it: [https://techoverflow.net/2019/02/23/how-to-run-x-server-using-xserver-xorg-video-dummy-driver-on-ubuntu/](https://techoverflow.net/2019/02/23/how-to-run-x-server-using-xserver-xorg-video-dummy-driver-on-ubuntu/).

##### Stefania Pedrazzi [Cyberbotics] 06/20/2019 06:12:30
`@Akash` for the Compass device example please look at the `samples/devices/compass.wbt` world . You can Open it from the `File > Open Sample Worlds` menu

##### Olivier Michel [Cyberbotics] 06/20/2019 06:14:38
`@fa`: Yes, check out [https://robotbenchmark.net](https://robotbenchmark.net)


`@adhitthana`: Yes, this is possible (we use this when running Webots in the cloud).

##### nisuunn 06/20/2019 15:38:47
Hello, is it possible to set the boundingobject mass to 0?


I would liek to use an IMU


for which I need to define a boundingobject


but this boundingobject adds the mass of the boundingobject to my robot


which is undesirable


is there any way around this?

##### David Mansolino [Cyberbotics] 06/20/2019 15:40:13
If you want to set both the mass and desity to 0, you simply have to remove the 'Physics' node.

##### nisuunn 06/20/2019 15:41:02
The documentation says that no physics node is needed for bumper type of touchsensor


im sorry i meatn touch sensor


trying to use touchsensor and set its boundingobject

##### David Mansolino [Cyberbotics] 06/20/2019 15:41:25
Ah ok, for the touchsensors it is indeed different

##### nisuunn 06/20/2019 15:42:02
when I have touchsesnor of  type bumper without physics defined


then my robot's physics behaves weirdly,


when i add physics node robots physics is normal


but the mass of boundingobject gets added to robots mass

##### David Mansolino [Cyberbotics] 06/20/2019 15:42:40
You obviously need to define the boundingObject of the touchsensor to define the detection area, but you should be able to not add any physics


let me try


I just tried and it is working fine, you can define the BoundingObject of the touchSensor without defining any Physics node. What is the weird behavior you observe? Did you make sur the bounding object of the sensors is not touching anythin elese than the robot itself (the collision with this bounding object will still be taken into account).

##### nisuunn 06/20/2019 15:49:12
the boundingobject touches the robots legs, is that okay?


the sensor is attached to the torso


a child of torso

##### David Mansolino [Cyberbotics] 06/20/2019 15:49:32
did you enable the robot.selfCollision ?

##### nisuunn 06/20/2019 15:50:02
the robots selfCollision field in the node tree is set to true

##### David Mansolino [Cyberbotics] 06/20/2019 15:50:41
In that cas that can indeed be a problem, because the legs might collide with the touchsensors and prevent them from moving

##### nisuunn 06/20/2019 15:50:58
so selfcollision to false would help?

##### David Mansolino [Cyberbotics] 06/20/2019 15:51:09
Probably, I would suggest to try

##### nisuunn 06/20/2019 15:51:54
Yes, that solves it


thank you!

##### David Mansolino [Cyberbotics] 06/20/2019 15:52:10
You're welcome

##### nisuunn 06/21/2019 10:30:13
Hi, I'd like to send messages using an emitter from a controller to a physics plugin. (The controller is in python).

I can send strings no problem

But I would like to send floats.

In the controller I do: 

msg = struct.pack("f", xVal) # where xVal is a float

emit1.send(msg)

In the physics plugin I have:


const float *emitterData = (float*) dWebotsReceive(&size); // receiving floats?

   dWebotsConsolePrintf("%d\n", emitterData[0]);


Running this crashes


How to correctly receive floats?

##### Olivier Michel [Cyberbotics] 06/21/2019 10:35:15
I believe Python don't have IEEE float, but only double. Replace const float emitterData = (float) with const double *emitterData = (double *)and replace %d with %g or %f in your printf...

##### nisuunn 06/21/2019 10:35:32
Thank you, I will try that

##### Olivier Michel [Cyberbotics] 06/21/2019 10:36:16
If you can't achieve this, you can always pass strings and convert them to float using sscanf(s, "%f", &value);

##### nisuunn 06/21/2019 10:37:22
Okay, thanks for your time.


I still crash


the python controller is:


b = repr(msg).encode()

        print("b: " + str(b))

        emit1.send(b)


the c++ physics plugin is


std::string *emitterData;

int size; emitterData = (std::string*) dWebotsReceive(&size);

   const char *cstr = emitterData->c\_str();

   float target;

   sscanf(cstr, "%f", &target); // convert string to float

   //float aNumero=strtof((*emitterData).c\_str(),0); // string to float

   //float val = std::stof(*emitterData); 

   //HERE

   dWebotsConsolePrintf("%f\n", target);


ill send physics plugin again


std::string *emitterData;

std::string *emitterData;

emitterData = (std::string*) dWebotsReceive(&size);

const char *cstr = emitterData->c\_str();

float target;

 sscanf(cstr, "%f", &target); // convert string to float

dWebotsConsolePrintf("%f\n", target);


I tried your earlier suggestion of receiving doubles instead because python doesn't have float.


I couldn't get that to work


so I tried to use sscanf to convert the controllers sent string to float


the second line in the physics plugin "std::string emitterData" can be ignored

##### Olivier Michel [Cyberbotics] 06/21/2019 12:22:34
Are you using Python 2 or 3 ?

##### nisuunn 06/21/2019 12:22:41
3

##### Olivier Michel [Cyberbotics] 06/21/2019 12:24:36
Can you try using char * instead of C++ string?

##### nisuunn 06/21/2019 12:24:49
I will try.

##### Olivier Michel [Cyberbotics] 06/21/2019 12:25:16
And try to print the received string instead of trying to convert it to float (for now).

##### el\_samu\_el 06/21/2019 12:27:59
Hi is there any way to turn off sound for webots? I am getting a lot of error messaging from ALSA at every startup when using a VM.

##### Olivier Michel [Cyberbotics] 06/21/2019 12:28:38
Did you try to disable audio from the GUI?

##### nisuunn 06/21/2019 12:28:56
I now have :

char *emitterData;

int size;

 emitterData = (char*) dWebotsReceive(&size);

 dWebotsConsolePrintf("%f\n", *emitterData);


The resulting print is:  0.000000

##### Olivier Michel [Cyberbotics] 06/21/2019 12:29:28
You need to receive char *, not char.


printf should be %s, not %f

##### nisuunn 06/21/2019 12:30:44
ah yeah the 1st line was char *emitterData; , the copy didn't work properly for some reason.

##### el\_samu\_el 06/21/2019 12:30:48
Yeah the sound is turned off. I think it complains because there isn't any sound card.

##### nisuunn 06/21/2019 12:31:46
I still crash

##### Olivier Michel [Cyberbotics] 06/21/2019 12:31:59
`@el_samu_el`: in that case, you should probably recompile Webots from the sources and disable the sound functions (initialization, etc.).


(or ignore the warnings, or install a virtual sound card on your VM).

##### el\_samu\_el 06/21/2019 12:35:26
how do I ignore the warnings that webots outputs on starting from the terminal?

##### nisuunn 06/21/2019 13:06:02
I think it works okay now


I now have


dWebotsConsolePrintf("%s\n", emitterData);


then it prints out  the correct number

##### Olivier Michel [Cyberbotics] 06/21/2019 13:14:20
`@SamuelKopp`: webots 2> /dev/null ?

##### TH0 06/21/2019 15:54:36
Hi, is there some helper function to calculate the SFRotation values?

for example: i just like to rotate a Nao (in the soccer simulation) by a supervisor program by alpha degrees keeping the feet on the ground. 

I thought just to set the rotation vector to the y axis (perpendicular to the field) and than simply adjust the angle. but that does not work.



In contrast the values for such a SFRotation are strange:

Looking towards first goal from center circle: SFRotation = 1   0  0  -1.5708

Looking to other side towards opposite goal from center circle (turn by 180 degrees): 0   0.7071    0.7071    -3.1415


i guess the coordinate system from the nao proto itself is rotated and that caused the strange values. but maybe you know a simple work around for that

##### David Mansolino [Cyberbotics] 06/21/2019 16:10:01
`@TH0`  , you are completely right, the coordinate system of the nao is not perfect as it is by default rotated, you should therefore combine your rotation with the '1   0  0  -1.5708' rotation, we do not provide any tool to do this, but I am sure you can find some nice libraries that do this.

##### riad.youssef 06/22/2019 09:00:14
Hi


I want to download the application, I want to know windows requirments

##### Fabien Rohrer [Moderator] 06/22/2019 09:05:01
Please refer to this link: [https://cyberbotics.com/doc/guide/system-requirements](https://cyberbotics.com/doc/guide/system-requirements)

##### JustDN 06/24/2019 15:01:09
I'm trying to make my robot to do a motion when the spacebar is pressed, how do you do that in C? I've tried enabling the keyboard and getting the key, then doing a statement for when the key is pressed, but it is not working.

##### Fabien Rohrer [Moderator] 06/24/2019 15:02:45
Hi,


What you tried seems correct.


Let me a minute to find a snippet


Something like this. We use often this pattern.
> **Attachment**: [keyboard\_snippet.c](https://cdn.discordapp.com/attachments/565154703139405824/592732386098413589/keyboard_snippet.c)

##### JustDN 06/24/2019 15:17:52
Thank you! It works.


Is there a way to check if a motor has moved to its intended location before executing the next action in C? I've been using an if statement "if(-1.31<wb\_position\_sensor\_get\_value(arms)&& wb\_position\_sensor\_get\_value(arms)<-1.299)" is there an easier way to than doing this?

##### Stefania Pedrazzi [Cyberbotics] 06/26/2019 06:13:02
`@JustDN` using the PositionSensor as you do is the correct way to check if motor movement is completed.

##### abcde 06/26/2019 12:14:05
hi!

##### Olivier Michel [Cyberbotics] 06/26/2019 12:14:18
Hi,

##### abcde 06/26/2019 12:14:25
can i use blender for the 3d models?

##### Olivier Michel [Cyberbotics] 06/26/2019 12:14:30
Sure.

##### abcde 06/26/2019 12:14:39
how do i do the joints

##### Olivier Michel [Cyberbotics] 06/26/2019 12:14:40
We even have a blender to Webots exporter.


Everything is explained here: [https://github.com/omichel/blender-webots-exporter](https://github.com/omichel/blender-webots-exporter)


Let us know if you have specific questions.

##### abcde 06/26/2019 12:16:59
okok thx


is it possible to simulate a bipedal robot instead of an arm?

##### Fabien Rohrer [Moderator] 06/26/2019 12:35:25
Hi, yes for sure. We have several biped models.


do you mean in the exporter?

##### abcde 06/26/2019 12:35:56
well im downloading the webots software rn


but im thinking of using blender to create the base model

##### Fabien Rohrer [Moderator] 06/26/2019 12:36:44
Webots contains lot of biped robots: here is the list of the released robots: [https://cyberbotics.com/doc/guide/robots](https://cyberbotics.com/doc/guide/robots)


In the exporter, an Arm is used as an example.


But it's perfectly possible to use the exporter to create a humanoid

##### abcde 06/26/2019 12:37:19
oh wow u guys have atlas!

##### Fabien Rohrer [Moderator] 06/26/2019 12:39:03
If you have good knowledge in Blender, you could start by studying the arm example. If the node hierarchy is well done in Blender, it's simple to export the Blender model to Webots.

##### abcde 06/26/2019 12:41:35
to moves the parts do i need to add things like bones and rigid body things.....


cuz i have no experience in those

##### Fabien Rohrer [Moderator] 06/26/2019 12:43:58
No, the exporter is currently only supporting solids. You need to create a node per degree of freedom (aka DOF, aka Solid), and create  a tree hierarchy in blender. The relative center of the nodes will used as joint pivot center. The rotation axis can be set in the JSON file.


If you open these files, you will certainly understand what I mean: [https://github.com/omichel/blender-webots-exporter/tree/master/examples/abb](https://github.com/omichel/blender-webots-exporter/tree/master/examples/abb)

##### abcde 06/26/2019 12:53:21
no i meant the thingy


hold on ill send screenshots


ok i cant find it cuz im not that good at blender (literally started this year) but do i need to rig the model in order for it to move?

##### Fabien Rohrer [Moderator] 06/26/2019 12:55:10
no rig


you need to split the object into peaces, one node per DOF

##### abcde 06/26/2019 12:55:39
then how will i or this godly software know where to move the joints?


ah

##### Fabien Rohrer [Moderator] 06/26/2019 12:55:54
then create a node hierarchy

##### abcde 06/26/2019 12:56:11
yeye


im hoping this file doesnt have a virus or malware (not an insult) cuz windows defender marked it as dangerous

##### Fabien Rohrer [Moderator] 06/26/2019 12:57:09
we guarantee it's clean


please take a look at the node hierachy, and the object center.
%figure
![blender.png](https://cdn.discordapp.com/attachments/565154703139405824/593424871082557441/blender.png)
%end

##### abcde 06/26/2019 12:59:28
oh wow


which programming language does webot use?


cuz im tryna build a robot using arduino and a raspberry pi


and sorta have a little bit of python and C

##### David Mansolino [Cyberbotics] 06/26/2019 13:10:36
Webots is compatible with C, C++, Matlab, Java, Python and ROS.

##### Emil Enchev 06/27/2019 07:20:33
Look my first place simulation [https://robotbenchmark.net/benchmark/humanoid\_sprint/](https://robotbenchmark.net/benchmark/humanoid_sprint/) . I'm new in Webots and I think is this good environment for making


walking simulations at all?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/593702341971279922/unknown.png)
%end

##### abcde 06/27/2019 07:21:36
nice


im new too

##### David Mansolino [Cyberbotics] 06/27/2019 07:22:41
Hi, congrats for your ranking !

Yes, many researcher are using Webots for walk simulation. Do you have any problem or specific need for your simulations?

##### Emil Enchev 06/27/2019 07:23:57
Obviously some developer don't think about side resistance as a factor.  Run first place [https://robotbenchmark.net/benchmark/humanoid\_sprint/](https://robotbenchmark.net/benchmark/humanoid_sprint/) and answer me is this simulation behavior normal for Webots?

##### David Mansolino [Cyberbotics] 06/27/2019 07:23:59
In your case it seems you are exploiting some physical instabilites of the physics engine of Webots....

##### Emil Enchev 06/27/2019 07:25:03
But this is important for me, because I will work on walking-running stabilization programming

##### David Mansolino [Cyberbotics] 06/27/2019 07:25:12
Indeed the friction betwwen the feets and the ground seems badly calibrated in this simulation...


But as described in the doc, it is possible to faithfully calibrate the contact properties: [https://www.cyberbotics.com/doc/reference/contactproperties](https://www.cyberbotics.com/doc/reference/contactproperties)

##### Emil Enchev 06/27/2019 07:28:11
Ok David, obviously I first must read documentation and next to complain from bugs. Thanks

##### David Mansolino [Cyberbotics] 06/27/2019 07:29:12
You're welcome, no problem, it is always good to have feedback, we will improve the calibration of the contact for this benchmark for a future version.

##### Emil Enchev 06/27/2019 09:21:40
Why you don't buy some real robots for online testing by your Webots users?

##### Fabien Rohrer [Moderator] 06/27/2019 09:22:33
We do, we have a collection of robots here 😃

##### Emil Enchev 06/27/2019 09:23:30
I talk about real robots, not simulation of real robots.

##### Fabien Rohrer [Moderator] 06/27/2019 09:24:10
We have serveral real robots in our office, the Nao, Robotis OP, several small wheeled robots, etc.


What do you have in mind?

##### abcde 06/27/2019 09:24:54
are the robots available to the public?

##### Emil Enchev 06/27/2019 09:25:07
Why you don't organize some RENT services. Pay for time test small tax

##### abcde 06/27/2019 09:25:17
i would pay!

##### Emil Enchev 06/27/2019 09:25:36
Euro 8000 for now is not what many users can afford


for NAO i mean

##### abcde 06/27/2019 09:26:21
if ur talking abt me i meant the rent services


i meant that i would be happy to pay a small fee for testing

##### Fabien Rohrer [Moderator] 06/27/2019 09:26:34
This is for sure a good idea. But I fear that the price for a rent of a 8000 euros robot would be prohibitive too.

##### abcde 06/27/2019 09:27:26
what about like online physical testing.... :/


like add a camera etc.

##### Fabien Rohrer [Moderator] 06/27/2019 09:28:03
Sending a robot worldwide is costly, robots are breakable (so a QA service should be created), etc.


Maybe that the robot manufactors would be interested by such concept.


Having an online testing service seems more doable.

##### abcde 06/27/2019 09:28:47
its like paying someone to 3d print something


(sort of....)

##### Emil Enchev 06/27/2019 09:29:40
1 Euro on Hour, 24 on day,  8760 euro for year. Of course, if NAO withstand 1 year 😃

##### abcde 06/27/2019 09:30:03
thats 760 euros profit

##### Fabien Rohrer [Moderator] 06/27/2019 09:30:25
The cost of the service should be evaluated too, there are not only benefits haha

##### abcde 06/27/2019 09:30:25
but i dont think it can withstand 24/7/365

##### Fabien Rohrer [Moderator] 06/27/2019 09:31:07
So would you pay 3000$ for a rent of 3 months?

##### abcde 06/27/2019 09:31:17
well i wont rent for 3 months.....


probs like 3 hours


5


robotics is my hobby.....

##### Emil Enchev 06/27/2019 09:32:00
SoftBank are very deluded.  They have to lower prices or put them on cheap rent online


I can make better software from Boston Dynamic 😉

##### abcde 06/27/2019 09:32:43
u work at boston dynamics?

##### Emil Enchev 06/27/2019 09:33:08
Not, they are amateurs. Years and achieved nothing

##### Fabien Rohrer [Moderator] 06/27/2019 09:33:18
Haha

##### abcde 06/27/2019 09:33:21
meh


they achieved SOME things

##### Emil Enchev 06/27/2019 09:33:33
Nothing

##### abcde 06/27/2019 09:33:37
like have u seen atlas doing the frickin back flip


although there isnt any real life situation that could help society by doing a backflip

##### Emil Enchev 06/27/2019 09:34:18
This carap stabilization system they have, can be build very easy with self adapting software


they do with with many tests

##### abcde 06/27/2019 09:34:37
meh


im tryna build a bipedal robot but i have no idea how to  stabilise things

##### Emil Enchev 06/27/2019 09:35:05
I can make NAO to do Backflip if it have power for it

##### abcde 06/27/2019 09:35:06
apart from controlling each motor 1 by 1


but thats just annoying

##### Fabien Rohrer [Moderator] 06/27/2019 09:35:29
`@Emil Enchev` Are you from industry, academics or hobbyist?

##### abcde 06/27/2019 09:35:47
i want to do it as a job but not old enough YET.... :/

##### Emil Enchev 06/27/2019 09:36:18
Not, I'm not.

##### abcde 06/27/2019 09:36:32
so hobbyist?

##### Emil Enchev 06/27/2019 09:38:36
Do you remember the Robostadium old forum, and the most watched topic there, "to stand up the NAO from any position".

##### Fabien Rohrer [Moderator] 06/27/2019 09:39:12
Oh yes, I remember, nice to see you again 😃

##### abcde 06/27/2019 09:39:29
u guys know each other.....?

##### Fabien Rohrer [Moderator] 06/27/2019 09:40:27
robotstadium was a contest we organized

##### abcde 06/27/2019 09:40:43
ah

##### Emil Enchev 06/27/2019 09:41:40
Ooo, this time it will be more from "nice to see you".  So you think RENT will be not profitable?

##### abcde 06/27/2019 09:42:06
well i mean if they charge 5 euros an hour


they would profit a little right?


40k euros if u run it 24/7/365


that is a lot of money

##### Fabien Rohrer [Moderator] 06/27/2019 09:42:53
this has to be thought. create a buisness model. A priori, I think it would be too expensive to work.

##### Emil Enchev 06/27/2019 09:42:56
If NAO withstand

##### abcde 06/27/2019 09:43:03
ye tru

##### Fabien Rohrer [Moderator] 06/27/2019 09:43:06
Renting for less than a day seems impossible


due to constraints with delivery

##### abcde 06/27/2019 09:43:37
even if its online?


like cam

##### Emil Enchev 06/27/2019 09:43:59
Why. The user can upload software to robot, and see result in real time for hour or so

##### abcde 06/27/2019 09:44:09
ye

##### Fabien Rohrer [Moderator] 06/27/2019 09:44:10
Yes, this seems much more doable online.

##### abcde 06/27/2019 09:44:28
but then again, their software is exactly that right?

##### Emil Enchev 06/27/2019 09:44:30
I talk only for online rent, not real

##### Fabien Rohrer [Moderator] 06/27/2019 09:44:39
I mean to have an online access to a real robot.

##### abcde 06/27/2019 09:44:44
same, mainly cuz im scared ill break it

##### Emil Enchev 06/27/2019 09:44:46
yes

##### Fabien Rohrer [Moderator] 06/27/2019 09:45:12
I also fear that users would break robots in such system

##### abcde 06/27/2019 09:45:21
tru

##### Fabien Rohrer [Moderator] 06/27/2019 09:45:25
the API should be very very robust

##### Emil Enchev 06/27/2019 09:45:38
break, will be include in equation and will have protect software for that purpose

##### Fabien Rohrer [Moderator] 06/27/2019 09:45:45
and this does not prevent to do heavy falls

##### Emil Enchev 06/27/2019 09:46:11
what happen with NAO in heavy falls is someone test it

##### abcde 06/27/2019 09:46:13
im j gna continue using the software cuz its FREEEEEE

##### Fabien Rohrer [Moderator] 06/27/2019 09:46:18
hardware on such robots is unfortunately also very breakable


I had bad experiments on the robotis op

##### Emil Enchev 06/27/2019 09:46:45
how breakable do you test it

##### Fabien Rohrer [Moderator] 06/27/2019 09:46:54
a fall => the camera is broken

##### abcde 06/27/2019 09:46:57
i mean its technically the same thing right? the software and hardware... :/

##### Fabien Rohrer [Moderator] 06/27/2019 09:47:18
a bad command => the motor overheats and is dead

##### Emil Enchev 06/27/2019 09:47:36
is there not protection mechanism?!

##### Fabien Rohrer [Moderator] 06/27/2019 09:47:39
It would be much more realist to do that on wheeled robots

##### abcde 06/27/2019 09:47:40
oh ye did i ask already, does anyone know a good motor for robotics?


for carrying heavy objects


and DC not AC


and cheap......

##### Fabien Rohrer [Moderator] 06/27/2019 09:48:40
I'm not expert in motors, sorry.

##### abcde 06/27/2019 09:48:47
ah


thx tho

##### Fabien Rohrer [Moderator] 06/27/2019 09:49:17
good question to ask on [https://robotics.stackexchange.com/](https://robotics.stackexchange.com/)

##### Emil Enchev 06/27/2019 09:49:18
why motor will overheats when reach its limits - it must be stop to do more load from that it can stands

##### abcde 06/27/2019 09:49:44
well most motors have the copper wire connected to the actual line thing

##### Fabien Rohrer [Moderator] 06/27/2019 09:49:45
this is the theory haha

##### abcde 06/27/2019 09:49:50
the center pole


and that creates friction

##### Emil Enchev 06/27/2019 09:50:01
No, I know how to protect the motors, I don't know why NAO don't have such protection?!

##### abcde 06/27/2019 09:50:04
and sparks!


its expensive.....

##### Emil Enchev 06/27/2019 09:50:20
Are you sure, that this is only theory?!

##### abcde 06/27/2019 09:50:22
they should take good care

##### Fabien Rohrer [Moderator] 06/27/2019 09:51:12
in my experiment, there are lot of ways to break a robotm but I'm maybe pessimist with this topic, because I come from software engineering

##### abcde 06/27/2019 09:51:19
its like a baby, u dont force it to walk or run when its just born


or accept money to make it run or walk

##### Emil Enchev 06/27/2019 09:52:16
in my opinion he will endure and there will be a profit, even if repair is required.

##### abcde 06/27/2019 09:52:45
can't be too safe

##### Emil Enchev 06/27/2019 09:52:48
from time to time


No need to be


Fabien, do you allow some testing on yours real NAO?


I really want to make joke with SoftBank

##### Fabien Rohrer [Moderator] 06/27/2019 09:54:52
this is very old, but I once worked on this setup, which is very close of what we are speaking about: [https://www.youtube.com/watch?v=hH5zRy2Rb5U](https://www.youtube.com/watch?v=hH5zRy2Rb5U)


honestly, I have unfortunately not much time for this 😦

##### abcde 06/27/2019 09:56:25
u gotta take care of robots because when singularity happens everyone who didnt will die

##### Emil Enchev 06/27/2019 09:57:17
These robots are cheap, and renting will not be profitable, because there will be no interest in rent.


NAO will be other thing

##### abcde 06/27/2019 09:58:18
u could get a NAO


and rent it out

##### Fabien Rohrer [Moderator] 06/27/2019 09:58:41
this is a good idea to think about

##### Emil Enchev 06/27/2019 09:59:35
If you make complex environment where everyone can test what he want it will be good add


Can you ask SoftBank if they want to sell several robots on cheap for this purpose 😃


Think the idea and try to sell it to them. After they return answer tell me what is it.

##### abcde 06/27/2019 10:03:27
im gonna be completely honest here, I dont think softbank would allow that even if its a good idea

##### Emil Enchev 06/27/2019 10:03:43
If they refuse, we will think another option. Including making our own robot, better from NAO

##### abcde 06/27/2019 10:03:54
then why dont you do that?

##### Emil Enchev 06/27/2019 10:03:55
Next will see, if SoftBank no interest

##### abcde 06/27/2019 10:03:58
i mean its cheaper right?

##### Emil Enchev 06/27/2019 10:04:14
yes, cheaper and better

##### abcde 06/27/2019 10:04:22
so why not go for that option?

##### Emil Enchev 06/27/2019 10:04:38
because it it time consuming

##### abcde 06/27/2019 10:04:48
then ur thinking only of profit

##### Emil Enchev 06/27/2019 10:04:52
it ease to bait on NAO and SoftBank

##### abcde 06/27/2019 10:04:54
im doing it cuz im passionate

##### Fabien Rohrer [Moderator] 06/27/2019 10:05:01
It's typically a project which would deserve to be proposed as an european project: to sell it before any trial 😃


Could I ask you to summarize this in a github issue? [https://github.com/omichel/webots/issues](https://github.com/omichel/webots/issues)


We could keep a trace of this.

##### Emil Enchev 06/27/2019 10:08:50
Fabien, to whom you talking about?

##### Fabien Rohrer [Moderator] 06/27/2019 10:09:02
maybe you in fact 😃

##### abcde 06/27/2019 10:09:03
im assuming its u

##### Emil Enchev 06/27/2019 10:09:27
to summarize what?

##### abcde 06/27/2019 10:09:57
ur idea abt NAO and softbank

##### Emil Enchev 06/27/2019 10:10:13
Ok


I will

##### Fabien Rohrer [Moderator] 06/27/2019 10:10:35
that the creation of an online but real testing setup would be a good idea.


ok thank you


with an issue, it's simpler to be notified.

##### Emil Enchev 06/27/2019 10:27:05
Ok I write issue. You are too dubious. SoftBank Robotics will agree and will provide these robots for free.  They have no idea what they can do with them. You will see.

##### abcde 06/27/2019 10:28:58
u want pepper?

##### Emil Enchev 06/27/2019 10:29:30
I don't understand question DarkMatter

##### abcde 06/27/2019 10:30:26
ah nothin

##### Emil Enchev 06/27/2019 10:31:08
Ie. whether I want to make a profit out of this endeavor?


No, in the near future I will make enough profits from selling software for them.

##### abcde 06/27/2019 10:31:51
no its just that i doubt softbank will give out their robots for free


nowadays most companies are about profit


except for this BEAUTIFUL COMPANY that made webots


they give out their code for FREEEEEE


anyways


they wont give out robots for free cuz then theyll be losing money


which is a pretty stubborn thing to do

##### Emil Enchev 06/27/2019 10:32:55
They will, or they will die in this robot endeavor like Google

##### abcde 06/27/2019 10:33:51
google didnt fail


they are a pretty big company rn


and most companies will fail whether it be a clothing brand or a tech brand if they give stuff out for free

##### Emil Enchev 06/27/2019 10:41:57
Do you understand, these their robots not good for anything useful, just because they NOT have the appropriate software.  For example, Boston Dynamic does good hardware for robots, but software is the weak link for them. That's why Google is getting rid of them. It was a good business move from Google site. If SoftBank want to make progress they must think outside the box. For example, I will never work for them    but I'd love to make paid software for their robots, which their specialists can not. And here we fall into a dilemma. I can make wonderful software for their robots, but I can not test it. RENTING online is simple solution.

##### abcde 06/27/2019 10:48:11
i dont mean it as an insullt


but u say Boston Dynamics has sh*tty software but its not like u can make something better


i mean they hired several people specialized in specific things and made it spending hours

##### Emil Enchev 06/27/2019 10:51:38
I'm the man who HACK Facebook. As you can see no one arrest me yet. They perfectly know who am I but can not do nothing. Do you know why?

##### abcde 06/27/2019 10:54:17
which software u use?


*searches up online*


jkjk


i highly doubt u hacked facebook

##### Emil Enchev 06/27/2019 10:57:08
Because, 2017 year I accidentally found a problem on their site that allowed automated retrieval of phone numbers to their users with simple Python code.  I was stupid enough to inform them about it on it support. So  they fixed the problem quietly.  They did not pay me anything for that.

##### abcde 06/27/2019 10:58:06
i still doubt u.....


can i see that python code?


its patched so no problem right?

##### Emil Enchev 06/27/2019 11:01:00
I got caught one day and found a second serious break in Facebook platform.  A month before I hacked them I told them: Now you will pay me for the first problem I discovered 2017, then for the new one. They refused because they thought I was bluffing, I hacked them. They lost billions.

##### abcde 06/27/2019 11:01:53
so.... which software did u use?

##### Emil Enchev 06/27/2019 11:02:52
Now if they arrest me, they have to admit theirs shareholders, that they are cheap idiots and lost billions because they were not given a few tens of thousands of dollars to the man who hack them.


You can believe me or not.

##### abcde 06/27/2019 11:04:07
i obviously dont

##### Emil Enchev 06/27/2019 11:04:10
Its your problem.


My point is. Never underestimate lone wolves. SoftBank Robotics will die if they don't open opportunity for outsiders to write software for their robots.

##### abcde 06/27/2019 11:05:28
someone who hacked facebook and threatened them, wouldnt be stubborn enough to share that on discord, where they can have 2 life sentences for hacking and corporate espionage

##### Emil Enchev 06/27/2019 11:06:00
And ask them whey they don't arrest me 😉

##### abcde 06/27/2019 11:06:59
ye no i still dont believe u at all

##### Emil Enchev 06/27/2019 11:07:31
Its not matter, as I said it.


you believe me or not.

##### abcde 06/27/2019 11:07:50
tell me which software u used and show me the python code, the bug is patched, its not illegal to possess that code so send it

##### Emil Enchev 06/27/2019 11:08:28
[http://mh370.radiantphysics.com/2017/04/16/atsb-denies-request-from-mh370-families-for-more-info/](http://mh370.radiantphysics.com/2017/04/16/atsb-denies-request-from-mh370-families-for-more-info/)

##### abcde 06/27/2019 11:09:08
ye ik that its like several years old

##### Emil Enchev 06/27/2019 11:09:10
Look for my name there


I'm the man who make Boeing to ground 737 MAX


I find what is wrong with it too


even this problem they find now, is my suggestion


[https://www.bbc.com/news/business-48752932](https://www.bbc.com/news/business-48752932)

##### abcde 06/27/2019 11:11:15
u literally just posted a lot of comments on a blogspot that some random dude created


and another random dude posted it on a website that literally no one knows abt

##### Emil Enchev 06/27/2019 11:12:09
Because you think that big corporation have the biggest brains 😃

##### Nikunj 06/27/2019 11:12:27
hello, i have just downloaded webot 2019b .. can any body guide me how to build hybrid robot with the help of this software?

##### Emil Enchev 06/27/2019 11:12:31
this is my point. They are one big nothing, usually


they only have money, nothing more

##### abcde 06/27/2019 11:12:43
would you download mcafee or malwarebytes?


mcafee is that news site that no one knows


malwarebytes is the godly corporation that has more evidence and power than mcafee

##### Emil Enchev 06/27/2019 11:13:26
no to stop and not interrupt tech issues advises here

##### abcde 06/27/2019 11:15:07
u from Bulgaria?


did u work at HP cuz thats pretty sick......?

##### Emil Enchev 06/27/2019 11:16:57
Nikunj, start with this first [https://cyberbotics.com/doc/guide/foreword?version=R2019b](https://cyberbotics.com/doc/guide/foreword?version=R2019b) I'm too new and now read them.

##### Nikunj 06/27/2019 11:26:32
@ Emil , thank you for the link, i have gone thro' the link earlier ....but found that already available various robot can be  simulated and analyze. please advise if any can develop own robot with 3 wheels and 4 legs type hybrid mobile robot .....please help ...advise...

##### Emil Enchev 06/27/2019 11:27:08
DarkMatter, yes I'm from Bulgaria, and No I'm not working for HP.  As I said you believe me or not, it doesn't matter at all. We will wait some times and you will see, that SoftBank will give Cyberbotics free NAO robots for online renting. Now really to stop, because we interrupt technical support here.


`@Nikunj`  , You must wait for Cyberbotics members to answer you.

##### Nikunj 06/27/2019 11:28:39
@ Emil, thanks a lot for your support .....thank you...

##### Emil Enchev 06/27/2019 11:46:22
`@Fabien Rohrer` answer `@Nikunj`  the question, because I fill this wall with trush.


😃 sorry for that


And `@Fabi`en , I think that if you arrange one big room with 4-5 NAO robots. One man can maintain them and help renting people if there is some minor problems, like robot fall and stuck or something like that. As `@abcde` propose 5 Euro on Hour will be good enough for renting people and  even money will remain for the salary of the supporting NAO's person. Everything will be fine.  First, Softbank will stretch a little bit, because they will think it will hurt their business with the sale of robots of universities but in reality students do not pay anyway for using them.  Renting Online will only bring new users to them. The will give you NAO for free, be sure, if they have some brains in a heads.

##### Fabien Rohrer [Moderator] 06/27/2019 11:56:59
`@Nikunj` I recommend you to do our tutorials first and come back later with specific questions 😃 [https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials)


It's perfectly doable to model a robot with legs and wheels


If you have budget and you are in a hurry, you can contact sales@cyberbotics.com to have a quote for your model (come with precise specifications)

##### Phœnix 06/27/2019 12:46:16
Hello


I need Help

##### Fabien Rohrer [Moderator] 06/27/2019 12:46:40
Hi, how could we help you?

##### Phœnix 06/27/2019 12:49:25
I wanted to try the amd gpu driver from dell website and now i lost a lot of settings the problem is i can't find my previous amd driver please help me


Am i in the correct place ?

##### Fabien Rohrer [Moderator] 06/27/2019 12:51:46
you're on the Webots chat, do you try to use Webots?

##### Phœnix 06/27/2019 12:53:10
Sry i just found this channel while i was doing reshearshe concerning my amd driver

##### Fabien Rohrer [Moderator] 06/27/2019 12:53:51
no, we don't provide support here with your hardware. Sorry, and good luck 😉

##### abcde 06/27/2019 14:17:08
how do companies like boston dynamics move their robots?


do they give commands 1 by 1


or do they have an algorithm that does all the labor?

##### Emil Enchev 06/27/2019 14:18:21
They move their robots with joystick mainly

##### abcde 06/27/2019 14:19:55
ye but like the movement


like the joystick gives the command but its not llike they have 50 joysticks controlling each joints

##### Emil Enchev 06/27/2019 14:21:00
Like NAO

##### abcde 06/27/2019 14:21:17
wut

##### Emil Enchev 06/27/2019 14:21:33
some sort of predetermine Motions

##### Fabien Rohrer [Moderator] 06/27/2019 14:21:41
either they are good in maths (Inverse Kineamtics, etc.) either they trained models (in Webots!? haha)

##### Emil Enchev 06/27/2019 14:21:41
but


they have stabilization software control between steps

##### abcde 06/27/2019 14:22:04
how do u train models tho?


like is it possible to create a similar robot but in blender


and add the motion from blender to the real robot?

##### Emil Enchev 06/27/2019 14:22:32
I will demonstrate such capability with your simulation of NAO

##### Fabien Rohrer [Moderator] 06/27/2019 14:22:41
This is a big topic.


In Blender, you don't deal with physics

##### Emil Enchev 06/27/2019 14:22:53
when I finish documents and get myself in it

##### abcde 06/27/2019 14:23:07
ye ik but its just a pain in the a*s to control 1 motor 1 by 1

##### Fabien Rohrer [Moderator] 06/27/2019 14:23:12
You can create a good looking motion, but it won't work in reality

##### abcde 06/27/2019 14:23:22
can webots do this?


like create a model


and attach the joints in the software to the one in real life?

##### Emil Enchev 06/27/2019 14:23:49
I'm good exactly in MATH they are not. They so long trained their robots this mean they use test result as data for stabilization

##### Fabien Rohrer [Moderator] 06/27/2019 14:23:49
With model, I mean to train an artificial neural network or so.


Webots does not provide such kind of tools, but you can easily embed a library doing this.

##### abcde 06/27/2019 14:24:35
ye but r there any neural nets out there specialized in these kinds of training?

##### Fabien Rohrer [Moderator] 06/27/2019 14:24:39
It's a research topic

##### abcde 06/27/2019 14:24:53
meh

##### Emil Enchev 06/27/2019 14:24:55
forget NN they are shits 😃

##### abcde 06/27/2019 14:25:00
NN?


ah neural network

##### Emil Enchev 06/27/2019 14:25:10
Neural Networks

##### abcde 06/27/2019 14:25:15
well..... im in love with fckin opencv rn

##### Emil Enchev 06/27/2019 14:25:32
OpenCV is shit too :-))

##### abcde 06/27/2019 14:26:00
no theyre not


what else am i supposed to use?


[https://www.lyrn.ai/2018/12/30/sim2real-using-simulation-to-train-real-life-grasping-robots/](https://www.lyrn.ai/2018/12/30/sim2real-using-simulation-to-train-real-life-grasping-robots/)


found smth

##### Emil Enchev 06/27/2019 14:26:48
😉 They are. Something you do yourself, of course

##### abcde 06/27/2019 14:27:08
AI by urself is pretty difficult

##### Emil Enchev 06/27/2019 14:28:38
Do you sure?!  This is what Facebook steal from me 2017

##### abcde 06/27/2019 14:28:39
if anyone elsle is interested in training a robot via sim

##### Emil Enchev 06/27/2019 14:28:40
[https://www.sciencemag.org/news/2017/11/artificial-intelligence-goes-bilingual-without-dictionary](https://www.sciencemag.org/news/2017/11/artificial-intelligence-goes-bilingual-without-dictionary)

##### abcde 06/27/2019 14:28:42
[https://arxiv.org/pdf/1812.07252.pdf](https://arxiv.org/pdf/1812.07252.pdf)


its by cornell so its a trusted source


`@Emil Enchev` isnt that google?

##### Emil Enchev 06/27/2019 14:29:24
They steal it from my computer hacking it to check if I"m not collecting database on their users 😃

##### abcde 06/27/2019 14:29:25
i think they use AI because it was a pain in the a*s to hire like 100 ppl

##### Emil Enchev 06/27/2019 14:30:40
And what they do? I hack them, I fuck them with $ billions, and they still don't understand what to make with my AI work. 😉

##### abcde 06/27/2019 14:30:48
`@Fabien Rohrer` i think using python in blender is a possible solution


since blender runs on python u can program smth so it links


but then again u still need to code each joints

##### Emil Enchev 06/27/2019 14:32:55
no matter what language you use if you don't think to use ready libraries. And for such work you don't have such libraries at all

##### abcde 06/27/2019 14:33:21
meh

##### Emil Enchev 06/27/2019 14:33:49
As I said, wait some weeks and I will show you behavior of NAO like Bostan Dynamics robot.


it is ease, and even I don't need training data like them. Pure math.

##### Fabien Rohrer [Moderator] 06/27/2019 14:34:44
`@Emil Enchev` We look forward to see results, please share them with us when you have something 😃

##### Emil Enchev 06/27/2019 14:35:15
I will share, something more, you will test them on your real Nao


Do you want backflip too?! Only tell me from what distance NAO can fall on his legs and withstand impact. 😃

##### Fabien Rohrer [Moderator] 06/27/2019 14:38:15
`@abcde` At some point you need to switch to Webots (if you would like). Creating animation in robotics is not the best idea because very rigid. You could for example try to actuate the motors with sinus/cosinus, it's a good way to start. Take a look at our mantis.wbt example, for example.

##### abcde 06/27/2019 14:38:42
..............


i have no idea what sinus and cosinus is


mainly cuz im not in uni


yet


in fact im still in hs

##### Fabien Rohrer [Moderator] 06/27/2019 14:39:36
a good exercise could be to open mantis.wbt in Webots and study the controller, play with variables, etc.

##### abcde 06/27/2019 14:41:18
ill be sure to do so!

##### Emil Enchev 06/27/2019 15:17:50
`@Fabien Rohrer`  tell me something more about Nao fall = > camera break? Why their is not build in, protection software that will detect falling robot and take measures to protect camera?

##### Fabien Rohrer [Moderator] 06/27/2019 15:19:03
I think there are, but for sure, we can also bypass such kind of protection. I also think that it's a complex problem, not sure SoftBank adresses this perfectly.

##### Emil Enchev 06/27/2019 15:19:22
HeadPitch and HeadYaw will be enough for me

##### Fabien Rohrer [Moderator] 06/27/2019 15:19:53
When I was discussing of camera break before, I was speaking on another robot.


I had this issue on the Robotis Op

##### Emil Enchev 06/27/2019 15:20:46
Detect what position fall, and move head in other direction, after impact will compensate with slight moving to the flor to reduce impact forces

##### Fabien Rohrer [Moderator] 06/27/2019 15:20:52
For sure, there are simple ways to prevent breaks


I read a paper once about to bend the knees in case of fall, this prevents lot of impact forces.


I think it was an hot topic at robocup at some point, you may find literature there.


[https://ieeexplore.ieee.org/document/6640493](https://ieeexplore.ieee.org/document/6640493) 😃

##### Emil Enchev 06/27/2019 15:26:52
When Softbank give you NAOs for rent, I will build the best protection system for them, to compensate some your repair costs. Bending knee is passive action of protection. There are active moves during impact which will compensate more.

##### Akash 06/27/2019 15:30:25
Is there any way by which the DistanceSensor's or the Compass's rate of response can be increase (decreasing the response time interval) ?

##### Fabien Rohrer [Moderator] 06/27/2019 15:30:46
Hi


The minimum refresh interval is given by WorldInfo.basicTimeStep


Then each sensor refresh rate can be set indepently using the "wb\_sensor\_enable" function


.. typically by a integer multiple of WorldInfo.basicTimeStep


`@Akash` I hope it answers your question.

##### Emil Enchev 06/27/2019 15:38:23
`@Akash` what is your problem exactly? Why you want to increase response of these sensors?

##### Akash 06/27/2019 16:02:37
`@Emil Enchev`   Because I am using distance sensors and compass to map surrounding just as a Lidar, Because of circular dynamic motion of the bot the response  distance sensors and compass are giving back are not proper (I don't want to use Lidar).

##### Emil Enchev 06/27/2019 16:05:38
Do you mean you check them too often, and this is time consuming  slowing down your remaining code?


Or you mean, that you want most fast to build the map?

##### Akash 06/27/2019 16:12:43
`@Fabien Rohrer`  reducing the time  WorldInfo.basicTimeStep is just giving me a lot more number of values but not proper values.

##### Emil Enchev 06/27/2019 16:15:28
The basicTimeStep field defines the duration of the simulation step executed by Webots. It is a floating point value expressed in milliseconds. The minimum value for this field is 0.001, that is, one microsecond. Setting this field to a high value will accelerate the simulation, but will decrease the accuracy and the stability, especially for physics computations and collision detection. It is usually recommended to tune this value in order to find a suitable speed/accuracy trade-off.


[https://www.cyberbotics.com/doc/reference/worldinfo](https://www.cyberbotics.com/doc/reference/worldinfo)


[https://cyberbotics.com/doc/reference/distancesensor](https://cyberbotics.com/doc/reference/distancesensor)


The wb\_distance\_sensor\_enable function allows the user to enable distance sensor measurements. The sampling\_period argument specifies the sampling period of the sensor and is expressed in milliseconds. Note that the first measurement will be available only after the first sampling period elapsed.

##### Akash 06/27/2019 16:25:12
`@Emil Enchev`  No its not the code, its like when the robot is at 90 degree the  compass is reading it 43.(something) degree,  same thing is happening to the distance sensor. I can stretch the values of compass response array to 90 degree by adding  some increasing variable to each and every value in that array, but nothing can be done to the distancesensor's array values.

##### Emil Enchev 06/27/2019 16:26:28
[https://cyberbotics.com/doc/reference/distancesensor](https://cyberbotics.com/doc/reference/distancesensor)

The wb\_distance\_sensor\_enable function allows the user to enable distance sensor measurements. The sampling\_period argument specifies the sampling period of the sensor and is expressed in milliseconds. Note that the first measurement will be available only after the first sampling period elapsed.


and


The wb\_distance\_sensor\_get\_sampling\_period function returns the period given into the wb\_distance\_sensor\_enable function, or 0 if the device is disabled.

##### Akash 06/27/2019 16:28:42
Ok, thank you.

##### Emil Enchev 06/27/2019 16:29:02
"Setting this field to a high value will accelerate the simulation, but will decrease the accuracy and the stability, especially for physics computations and collision detection. It is usually recommended to tune this value in order to find a suitable speed/accuracy trade-off." What is your value of  basicTimeStep


I'm totally newbie here this is why ask you


the problems of others can become mine too


Do you understand me `@Akash` . Probably your basicTimeStep was increased, and this is why you read wrong information from sensors.


Set it to minimum and see what will be result


0.001


Is this normal behavior of Webots to color the caption of the button controls when they are on focus?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/594057038271873025/unknown.png)
%end

##### David Mansolino [Cyberbotics] 06/28/2019 06:51:57
Yes of course, this is done to help you visualize which item is currently focused

##### Emil Enchev 06/28/2019 06:59:11
For me it look like some sort of graphical defect. It disrupts the perception of the first letter and is irritating. The purple border is sufficient to indicate focus.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/594059204885741568/unknown.png)
%end

##### David Mansolino [Cyberbotics] 06/28/2019 07:01:10
There is indeed an issue with the first letter, we are aware of this, if it is irritating you, you can switch to the 'classic' them, the problem should not be present in this theme.

##### Emil Enchev 06/28/2019 07:09:01
I like dark. If you want simply put little purple dot outside of the caption to indicate focus something like this, but coloration on part of text is worse.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/594061680585998337/unknown.png)
%end


I will keep with dark, thx

##### David Mansolino [Cyberbotics] 06/28/2019 07:11:31
Ok, thank you for the suggestion, if you want you can try to do a prototype and submit it to our github repo, the css of the dark mode is defined here: [https://github.com/omichel/webots/blob/revision/resources/webots\_night.qss](https://github.com/omichel/webots/blob/revision/resources/webots_night.qss) (night) or [https://github.com/omichel/webots/blob/revision/resources/webots\_dusk.qss](https://github.com/omichel/webots/blob/revision/resources/webots_dusk.qss) (dusk)

##### Emil Enchev 06/28/2019 07:19:40
So you don't know from where come the problem with the coloration of the text, David? 😋

##### David Mansolino [Cyberbotics] 06/28/2019 07:21:50
You should start by checking the *:selected items

##### Emil Enchev 06/28/2019 07:28:42
Qt Style Sheet is not my thing - I'm on Visual Studio , but OK


`@David Mansolino`  The problem is not only in .qss files you send me. Manly this line of code in


[https://github.com/omichel/webots/blob/revision/resources/webots\_night.qss](https://github.com/omichel/webots/blob/revision/resources/webots_night.qss)

[https://github.com/omichel/webots/blob/revision/resources/webots\_dusk.qss](https://github.com/omichel/webots/blob/revision/resources/webots_dusk.qss)
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/594128214276833306/unknown.png)
%end


...create the problem. But with padding 1px 1px in one dialog problem resolve in other with default buttons the problem persist.


This mean: go to your Qt Editor, and check Button Controls Options  for all Dialogs they are the same and if they differ, by what options exactly.


I really don't understand nothing from Qt and making GUI with it, nor from Qt The Style Sheet but this is your problem.


Suggestion: First try to make the buttons bigger, `@David Mansolino`


Probably size of these buttons is too small, and when you make padding it eat from first letter small portion of pixels. Its create red color because of some semi-transparency shits  or something of the sort.



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/594147105598078977/unknown.png)
%end

##### DimitrisK 06/28/2019 14:05:53
Hello. I am trying to implement a Deep Q Network in a webots environment. What i need to do,is "change" the robot after an amount of steps,with a seemingly new one. I dont want to reset the controller,since i dont want to lose the progress of the training. I have seen that there is a way to remove a robot via a supervisor command,but how can i instantly re-import it so it's like a new one? I want to do that,since after a number of steps and collisions,the robot is damaged and not functioning properly

##### David Mansolino [Cyberbotics] 06/28/2019 14:07:26
`@DimitrisK`, this is the reference page for using numerical optimization methods in Webots: [https://www.cyberbotics.com/doc/guide/using-numerical-optimization-methods](https://www.cyberbotics.com/doc/guide/using-numerical-optimization-methods)


Basically you can remove any node in the simulation using this function: [https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_remove](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_remove)

And add a new node with these ones: [https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_field\_import\_mf\_node](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_field_import_mf_node)

##### DimitrisK 06/28/2019 14:10:02
Thank you David,i 'll give it a try

##### David Mansolino [Cyberbotics] 06/28/2019 14:10:09
You're welcome

##### roboshiv 06/28/2019 14:58:11
Hi all, I am trying to crosscompile a .c file to run on an epuck robot and I am getting the following error:

heap: Link Error: Could not allocate section .heap, size = 512 bytes, attributes = heap keep 



The .c controller itself compiles fine, it seems to be the step after it. Full console output is:

make -f Makefile.e-puck

xc16-gcc -Wall -mcpu=30f6014A -I"/home/student/webots/projects/robots/gctronic/e-puck/transfer/libepuck/include" -I"/home/student/webots/projects/robots/gctronic/e-puck/transfer/library" -MM -MT "mlpGA\_Jh.o" "mlpGA\_Jh.c" -MF "mlpGA\_Jh-pic30.d"

xc16-gcc -c -Wall -mcpu=30f6014A -I"/home/student/webots/projects/robots/gctronic/e-puck/transfer/libepuck/include" -I"/home/student/webots/projects/robots/gctronic/e-puck/transfer/library" -o "mlpGA\_Jh-pic30.o" "mlpGA\_Jh.c"

xc16-ld --heap=512 --script="/home/student/webots/projects/robots/gctronic/e-puck/transfer/xc16/support/dsPIC30F/gld/p30F6014A.gld" -L"/home/student/webots/projects/robots/gctronic/e-puck/transfer/xc16/lib" -L"/home/student/webots/projects/robots/gctronic/e-puck/transfer/xc16/lib/dsPIC30F" -L"/home/student/webots/projects/robots/gctronic/e-puck/transfer/libepuck" --start-group "mlpGA\_Jh-pic30.o" -lc-elf -ldsp-elf -lm-elf -lp30F6014A-elf -lpic30-elf -lepuck --end-group -o "mlpGA\_Jh.cof"

heap: Link Error: Could not allocate section .heap, size = 512 bytes, attributes = heap keep 

 Link Error: Could not allocate data memory

/home/student/webots/projects/robots/gctronic/e-puck/transfer/libepuck/Makefile.include:83: recipe for target 'mlpGA\_Jh.cof' failed

make: *** [mlpGA\_Jh.cof] Error 255

Nothing to be done for build targets.

##### David Mansolino [Cyberbotics] 06/28/2019 15:03:48
Hi `@roboshiv`, have you tried with the example controller distributed with Webots ?

##### roboshiv 06/28/2019 15:04:23
Yes I have successfully cross-compiled epuck controllers onto the real robot in the past


from this same pc and setup

##### David Mansolino [Cyberbotics] 06/28/2019 15:04:40
`@Emil Enchev` thank you for the investigation, we will try to fix this as soon as we have some time.

##### roboshiv 06/28/2019 15:06:48
when I try a different project it works fine. This project uses some matrices which are 16*16 containing doubles which are weights for a simple neural net, nothing massive. and the compiled file is only 19.2 kB

##### David Mansolino [Cyberbotics] 06/28/2019 15:07:45
The memory on the first generation of e-pucks is really limited, it is highly probable that the matrice is too big for the e-puck memory.


Maybe you can try to switch from double to float to save some memory.

##### roboshiv 06/28/2019 15:09:04
Haha yes thanks we were literally trying that to see what happens


but unfortunately it didn't work


So this error just means we don't have enough space on the epuck? What type of memory are we running out of? RAM?

##### David Mansolino [Cyberbotics] 06/28/2019 15:19:38
For information here is the datasheet of the epuck microcontroller: [https://www.microchip.com/wwwproducts/en/en024766](https://www.microchip.com/wwwproducts/en/en024766)

##### MariusJuston [Moderator] 06/28/2019 20:03:04
Hello I am new  to Webots and I am trying to simulate my robot; however I created it using Autodesk Inventor and the file is a *.iam, is there a way to add it in to the simulation world?

##### Fabien Rohrer [Moderator] 06/28/2019 20:05:06
Not as-is unfortunately: Webots doesn’t have a .iam importer. I think the simplest way is to find a way to import it in Blender (which supports lot of formats) and then use the blender2webots add on:


[https://github.com/omichel/blender-webots-exporter](https://github.com/omichel/blender-webots-exporter)

##### MariusJuston [Moderator] 06/28/2019 20:06:35
This is exactly what I needed thank you

##### Fabien Rohrer [Moderator] 06/28/2019 20:07:06
Great, you’re welcome 😉

##### MohamedSabry 06/30/2019 14:40:47
failed: 

[nao\_matlab] 

[nao\_matlab] Function: void \_\_cdecl `anonymous-namespace'::mwJavaAbort(void), file b:\matlab\src\jmi\jmi\javainit.cpp, line 1461

[nao\_matlab] 

[nao\_matlab] --------------------------------------------------------------------------------


why does this always appear for me whenever I try to run a matlab based controller for my robot?

I installed the MinGW-w64 Compiler thinking that maybe this is the problem but it still does not run and whether I send a report or nor, this shows in my webots console
%figure
![2019-06-30.png](https://cdn.discordapp.com/attachments/565154703139405824/594900331662409754/2019-06-30.png)
%end


webots 2019a and matlab 2018a

## July

##### MariusJuston [Moderator] 07/01/2019 01:12:47
Sorry, I have been trying to set up my robot *.iam file through Blender and I have not managed to figure out how to do it. I have managed to export the file as a *.wrl; however, I have no idea how I would actually set it up in Webots, it is a simple 6 axis robot however how do you set up a custom robot from a custom .wrl? 

Also, how do you set up the joints and the sensors, if you have to do it through Blender how would you do this? 

On another note I am trying to import another model to be an object that the robot would need to interact with, again I have the *.iam and I have managed to convert it to a *.wrl; however, when I import the created *.wrl from the Webots Import VRML97 functionality I get a big list of Shape objects, how do I group them into a single Solid?

Many thanks!

##### David Mansolino [Cyberbotics] 07/01/2019 06:47:35
`@MohamedSabry`, this is probably caused by an incompatible library in your PATH, can you please try to delete the file 'protofile\_matlab\_2018b\_webots\_R2019a.m' in your temporary files and then retry (this file will be regenerated by Webots).

For more information, please have a look at this issue (exact same problem): [https://github.com/omichel/webots/issues/63](https://github.com/omichel/webots/issues/63)

##### Fabien Rohrer [Moderator] 07/01/2019 06:47:52
`@MariusJuston` Do you have any possibility to export your file to something else than a *.iam? Blender supports better other formats like *.x3d, *.collada, etc. To set the joints/sensors, please refer to the README here: [https://github.com/omichel/blender-webots-exporter](https://github.com/omichel/blender-webots-exporter) If you don't want to use the addon, you can indeed import the wrl shapes (it's regular it's a list)  directly into a new Solid node (by copy-pasting the Shapes into the new Solid.children fields).

##### Emil Enchev 07/01/2019 07:50:52
Are SoftBank develop further  Romeo robot, or they cut off it. I don't see any documentation about it.

##### Marcey 07/01/2019 08:17:58
Hello, I have a little question, maybe someone can help me: I want to call a supervisor service via ROS. The controller connects successfully to the ROS master, but when I call a service  in the terminal, the controller always crashes.



This is my imput to the command line:  "rosservice call /myBot/supervisor/node/get\_position "node: 0""



"node: 0" is obviously wrong, but what would be the right argument? Or am I completely misusing the supervisor functions?

##### abcde 07/01/2019 08:18:57
guys if u guys wna control shit using a simulation made by AI


use JS code bullet made a video abt it


genetic algorithm..... just sayin

##### David Mansolino [Cyberbotics] 07/01/2019 09:17:58
`@Marcey`, you should first call the  '/supervisor/get\_from\_def' ([https://www.cyberbotics.com/doc/reference/supervisor?tab=ros#wb\_supervisor\_node\_get\_from\_def](https://www.cyberbotics.com/doc/reference/supervisor?tab=ros#wb_supervisor_node_get_from_def)) service to get the id of the node.


`@abcde` , ok thank you for the advice.

##### MohamedSabry 07/01/2019 12:02:15
`@David Mansolino` where can I find this file? and how can I find it?

##### David Mansolino [Cyberbotics] 07/01/2019 12:04:40
`@MohamedSabry` :  C:\Users\USERNAME\AppData\Local\Temp\

##### MohamedSabry 07/01/2019 15:51:55
`@David Mansolino` I still have the same problem

##### borismera 07/01/2019 16:20:23
buenass tardes


disculpenn


como activo el triall de webots

##### Fabien Rohrer [Moderator] 07/01/2019 16:28:57
Since December 2018 Webots is completely free and open-source.



You can download Webots here: [https://www.cyberbotics.com/download](https://www.cyberbotics.com/download) and check out the source code here: [https://github.com/omichel/webots](https://github.com/omichel/webots)



We provide paid user support, training and consulting for users who need to quickly develop high-quality Webots simulations, see: [https://www.cyberbotics.com/buy](https://www.cyberbotics.com/buy)

##### David Mansolino [Cyberbotics] 07/02/2019 07:28:39
`@MohamedSabry`, I will answer you directly on this thread: [https://github.com/omichel/webots/issues/681](https://github.com/omichel/webots/issues/681)

##### Gabriel\_martinez 07/03/2019 04:27:05
How can i  read a character from the keyboard as an example of the 's'?

##### Stefania Pedrazzi [Cyberbotics] 07/03/2019 06:00:15
`@Gabriel_martinez` to read the keyboard input from a controller you have to use the Keyboard API: [https://www.cyberbotics.com/doc/reference/keyboard#wb\_keyboard\_get\_key](https://www.cyberbotics.com/doc/reference/keyboard#wb_keyboard_get_key).

In the `projects/languages` folder you can find an example of controller written in C++, Java, and Python. For a C example please look at the `samples/devices/worlds/gps.wbt` simulation.

##### el\_samu\_el 07/03/2019 10:14:05
I have a question about parallel execution of several webots instances. It seems like parallel execution does not really give any significant speedup. I have access to a cloud instance with 96 cores and around 360 GB RAM. When I launch 40 webots instances to run 100 timesteps each, then this is not (significantly) faster then running 4000 timesteps on a single instance on my local 8GB machine.  I open the instances via the subprocess module of python. They do in fact all run in parallel, but the more instances I start, the slower each of them gets. Where is the bottleneck? Do all of them use shared ressources?


It is run on Ubuntu 18.04 without a gpu

##### Olivier Michel [Cyberbotics] 07/03/2019 10:18:38
It could be the graphics.

##### el\_samu\_el 07/03/2019 10:18:41
the controllers are basically just getting observations from the sensors, feeding this through a tensorflow graph and then actuating the motors


i fast forward all simulations


so I don't see graphics

##### Olivier Michel [Cyberbotics] 07/03/2019 10:19:06
If your cloud has no graphics hardware acceleration, this could be the bottleneck.


Did you try to run the simulation in fast mode?

##### el\_samu\_el 07/03/2019 10:19:36
yes, they all are run in fast mode

##### Olivier Michel [Cyberbotics] 07/03/2019 10:19:58
Do you use camera, lidar or range finders in your simulations?


All these rely on OpenGL acceleration.

##### el\_samu\_el 07/03/2019 10:20:29
I use accelerometer, imu, gyro, position sensors, force sensors and 3 type of motors


but no self collision

##### Olivier Michel [Cyberbotics] 07/03/2019 10:20:45
OK, so it shouldn't be the problem.


So, I have no clue on what could be the bottleneck, you should investigate it on your cluster.

##### el\_samu\_el 07/03/2019 10:22:41
does in your experience parallelization work fine?


before I jump into that

##### Olivier Michel [Cyberbotics] 07/03/2019 10:23:32
We don't have much experience in running Webots on such computer clusters.

##### el\_samu\_el 07/03/2019 10:23:51
okay I understand, thanks Olivier

##### nisuunn 07/03/2019 11:30:25
Hi! I would like to send data with an emitter from a robot controller script to a supervisor. Where should the supervisor's receiver be and how can I access the receiver with the Supervisor?

##### Fabien Rohrer [Moderator] 07/03/2019 11:34:35
You could simply add your Receiver as a child of the supervisor. (a supervisor is a Robot with the supervisor field enabled; it acts the same as a robot controller, but access to supplementary functions).


so you could refer simply to the receiver doc: [https://www.cyberbotics.com/doc/reference/receiver](https://www.cyberbotics.com/doc/reference/receiver)

##### nisuunn 07/03/2019 11:37:38
In my case I cannot connect the supervisor to my robot, as the supervisor spawns a new robot so there would be an endless loop of spawning new robots.


But, I think maybe I can work out some way around this.


Thanks for the help!

##### MariusJuston [Moderator] 07/03/2019 18:36:31
Hello I seem to be having some issues, I have a custom made part that was created from inventor, exported as an .stl, converted using blender into a .wrl and then imported into Webots. When it is imported into Webots it is depicted as a Shape so I copy pasted it into a Solid object, because the shape was complex I just used a Box as a bounding object; however, the origin point was not in the center of the part so I had to wrap the Box with a Transform object. When I start the simulation the object just sinks into the ground a few centimeters. I have tried changing the basicTimStep, and the ERP but I have had no success.


I seem to have figured out the problem, objects a rest sink in the ground by default because of their weights. I will just make the weight of the objects very little and it should work

##### Fabien Rohrer [Moderator] 07/04/2019 05:38:43
Hi Marius, yes heavy robots can « sink » with the default physics parameters. Rather than changing the masses, you should rather modify the ContactProperties softCFM value.


[https://www.cyberbotics.com/doc/reference/contactproperties](https://www.cyberbotics.com/doc/reference/contactproperties)

##### Deleted User 07/04/2019 16:46:22
Sir,


I am trying to  do  path planning in webot using matlab controller, but as enable the matlab controller and start the programme, it do not run and it shows some problem and crashed.


Sir,

I am trying to  do  path planning in webot using matlab controller, but as enable the matlab controller and start the programme, it do not run and it shows some problem and crashed.


Sir,

Please provide me a demo code for NAO robot for obstacle avoidance.

I will be greatfull to you,

please help


@Emil Enchev#2033 sir, can you help me with NAO programming in webot

##### MIKE 07/05/2019 07:33:53
Hi，I want to add an LED sensor to my robot, but every time I am trying edit the LED shape, webots will crash and quit. What is the reason, is it a bug?

##### Olivier Michel [Cyberbotics] 07/05/2019 07:37:00
Hi Mike,


It might be a bug indeed. Can you file a issue at [https://github.com/omichel/webots/issues/new?template=bug\_report.md](https://github.com/omichel/webots/issues/new?template=bug_report.md) and explain the exact procedure to reproduce this crash? We will investigate it and eventually fix it.

##### David Mansolino [Cyberbotics] 07/05/2019 07:41:28
`@Deleted User`, is it you that did open this issue: [https://github.com/omichel/webots/issues/681](https://github.com/omichel/webots/issues/681)

If not is your problem similar to this one?


> from `@Deleted User`:   Sir, no solution for the crash is given, i still have the same problem

> Only c programming is working, but i do not know the c programming. And i need to make a simple program to make NAO to walk in simple environment, detect the obstacles and avoid the collision.



Do you mean that no other language than C (e.g. C++, Python, Java, etc.) are working for you ?

##### Deleted User 07/05/2019 13:10:05
Sir,

I am trying to  do  path planning in webot using matlab controller, but as enable the matlab controller and start the programme, it do not run and it shows some problem and crashed.


`@David Mansolino`  sir only C and C++ is working

##### David Mansolino [Cyberbotics] 07/05/2019 13:11:38
Have you tried the Python version? (e.g. in the humanoid\_sprint world)


Have you tried the other Matlab example to check if they crashes too ?

(e..g. youbot matlab)

##### Deleted User 07/05/2019 13:12:31
No sir, i have not.


i am new in WEBOT, till now i understood developing the environments, placing obstacles and changing there translation and orientation


I can add robot and have used the demo c code provided in it

##### David Mansolino [Cyberbotics] 07/05/2019 13:17:16
Ok, then in that case it is strongly recomended that you follow our tutorial: [https://www.cyberbotics.com/doc/guide/tutorials](https://www.cyberbotics.com/doc/guide/tutorials)

##### Deleted User 07/05/2019 13:17:30
But after that i am not getting anything, because my MATLAB controller is crashing everytime

##### David Mansolino [Cyberbotics] 07/05/2019 13:17:46
It is available in any language and there is a chapter specific to controllers: [https://www.cyberbotics.com/doc/guide/tutorial-4-more-about-controllers-20-minutes](https://www.cyberbotics.com/doc/guide/tutorial-4-more-about-controllers-20-minutes)

##### Deleted User 07/05/2019 13:18:08
i went through it and i can perform the tasks provided there

##### David Mansolino [Cyberbotics] 07/05/2019 13:18:51
Ok, then you should know understand how the controller works


(at least have an overview)


Please try to open this world and let us know if it is working: [https://cyberbotics.com/doc/guide/youbot#youbot\_matlab-wbt](https://cyberbotics.com/doc/guide/youbot#youbot_matlab-wbt)

##### Deleted User 07/05/2019 13:19:53
yes sir, for the wheeled robot, it is working, but for humanoid, i am feeling helpless.


Sir,

Please provide me a demo code for NAO robot for obstacle avoidance.

I will be greatfull to you,

please help

##### David Mansolino [Cyberbotics] 07/05/2019 13:35:08
I am sorry, we don't have such code available, if you want us to develop such controller for us we can send you an offer for this, please refeer to [https://www.cyberbotics.com/buy](https://www.cyberbotics.com/buy)

##### ThundrHawk 07/05/2019 15:28:25
I'm trying to create a python controller, and I'm trying to create other python files alongside the controller file to reduce clutter. I've been changing my runtime.ini file to :

```ini
[python]
OPTIONS = -m demo_controller
```


and I use imports with the following method:

```python
from .utils import *
```


and it tells me that there is no parent directory set up


does anyone know if there's a way to fix this or if I have to do everything in the one controller file?

##### Olivier Michel [Cyberbotics] 07/05/2019 15:30:08
I am currently writing a python controller split in several files.


I don't have a runtime.ini file.

##### ThundrHawk 07/05/2019 15:30:36
how did you set it up?

##### Olivier Michel [Cyberbotics] 07/05/2019 15:30:42
my main python file is called supervisor.py


I have another python file called constants.py


from supervisor.py, I simply "import constants"


and it works.

##### ThundrHawk 07/05/2019 15:31:32
and you have your robot's controller set to "supervisor"?

##### Olivier Michel [Cyberbotics] 07/05/2019 15:31:49
Yes, but this should make no difference.

##### ThundrHawk 07/05/2019 15:31:58
alright thanks i'll see if that works

##### Olivier Michel [Cyberbotics] 07/05/2019 15:32:20
Sorry, yes it is important (I did get your question).


And my supervisor.py is stored into a folder called "supervisor".


which is a subfolder of the controllers directory.


Well, something standard in Webots.

##### ThundrHawk 07/05/2019 15:33:22
alright


thanks


your constants.py file is in the same folder?

##### Olivier Michel [Cyberbotics] 07/05/2019 15:35:05
Yes.

##### ThundrHawk 07/05/2019 15:36:08
alright it looks like that works. I suppose I was confused because I'm using PyCharm for the coding, and it's telling me it can't find the module


thanks for your help

##### Olivier Michel [Cyberbotics] 07/05/2019 15:36:24
No problem, you are welcome.


Maybe you should configure PyCharm so that it searches for modules in the current folder as well.

##### ThundrHawk 07/05/2019 15:59:21
yeah we stuck it in a try except to make pycharm happy

##### Olivier Michel [Cyberbotics] 07/05/2019 16:03:38
OK...

##### ThundrHawk 07/05/2019 16:04:18
its real hacker man hours 😎


```python
try:
    from demo_controller import kinematics_solver
except ImportError:
    import kinematics_solver
```

##### Olivier Michel [Cyberbotics] 07/05/2019 16:09:16
😂


I don't know PyCharm, but it should be possible to configure it so that it searches the current folder for imports, no?

##### ThundrHawk 07/05/2019 16:38:06
¯\\_(ツ)\_/¯

##### Huey 07/06/2019 14:13:43
Hi... im new to webot


it says " Make sure you choose the most recent 64-bit release of the Standard Edition (SE) of the JDK 8. Then, follow the installation instructions." on  language setup page


found two on the page ([https://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html](https://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)) 

should i install  JDK 8u211 /JDK 8u212


or both?

##### ThundrHawk 07/06/2019 14:18:14
I'm not sure that it will matter a whole lot, but you should probably get 212 if its the latest

##### Huey 07/06/2019 14:19:22
cool.. thanks. will try


turns out, installing the JDK 8u211 worked fine

##### NearMonzter 07/08/2019 00:24:22
Hi there, does anybody know how to do a triangle in webots or know how to import from SolidWorks?

##### Fabien Rohrer [Moderator] 07/08/2019 06:30:20
`@NearMonzter` From SolidWork, you can export a VRML or X3D file, and then you can import it in Webots as a Shape (the graphical info only). Alternatively, you can use Blender inbetween and add more information (collisions, masses, joints, etc.), using this addon: [https://github.com/omichel/blender-webots-exporter](https://github.com/omichel/blender-webots-exporter)

##### NearMonzter 07/08/2019 06:55:38
Thank you, I struggled a lot but I finally did what I expected, I needed the triangle shape to make an omnidirectional robot
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/597682190062452744/unknown.png)
%end

##### Fabien Rohrer [Moderator] 07/08/2019 08:44:26
Ok, I see


Do you need collisions on it?


Why not using the Extrusion proto? [https://cyberbotics.com/doc/guide/object-geometries#extrusion](https://cyberbotics.com/doc/guide/object-geometries#extrusion)

##### OnlyPtuuu 07/08/2019 13:22:55
Hi, I have installed 8.6.2 webots to simulate Pepper for use it with Choregraphe. But the Open Sample World section is empty.


Screenshot
%figure
![webots.png](https://cdn.discordapp.com/attachments/565154703139405824/597780190537580564/webots.png)
%end


[http://doc.aldebaran.com/2-4/software/webots/webots\_index.html](http://doc.aldebaran.com/2-4/software/webots/webots_index.html)

##### Fabien Rohrer [Moderator] 07/08/2019 13:26:27
Hi

##### OnlyPtuuu 07/08/2019 13:26:41
I'm looking this guide

##### Fabien Rohrer [Moderator] 07/08/2019 13:26:45
First question: why Webots 8.6.2 and not Webots R2019b 😄 ?

##### OnlyPtuuu 07/08/2019 13:27:13
Because in aldebaran documentation say to use this version


Do you advice downloading the latest version?

##### Fabien Rohrer [Moderator] 07/08/2019 13:29:22
Ok, I think you can use the latest version, it will solves more issues than others. However, in recent Webots versions, you have to install naoqisim by yourself from this deprecated repo: [https://github.com/omichel/naoqisim](https://github.com/omichel/naoqisim)


Webots is free and open-source since R2019a.

##### OnlyPtuuu 07/08/2019 13:31:28
Ok now I install last version R2019B

##### Fabien Rohrer [Moderator] 07/08/2019 13:32:27
that sounds to be a good idea 👍🏻

##### OnlyPtuuu 07/08/2019 13:33:12
next step install naoqisim

##### Fabien Rohrer [Moderator] 07/08/2019 13:33:40
Yes this may be not obvious, I'm not sure about the current state of this project.


This project is no more supported by Cyberbotics nor SoftBanks, sorry for this.


What about pepper? Do you have a model of this robot in Webots?

##### OnlyPtuuu 07/08/2019 13:35:57
I would like to use webots to simulate pepper and test my projects

##### Fabien Rohrer [Moderator] 07/08/2019 13:36:46
Ok, just to warn you Webots does not contain a Pepper model.

##### OnlyPtuuu 07/08/2019 13:37:54
And how can I do?

##### Fabien Rohrer [Moderator] 07/08/2019 13:39:36
Modeling a humanoid robot model is not a simple task. And it depends on your skills, your input data, etc.


A good strategy would be to find a good 3D model of the Pepper, for example here: [https://www.turbosquid.com/fr/3d-model/softbank-robotics](https://www.turbosquid.com/fr/3d-model/softbank-robotics)


And to use the Webots2blender addon to import it in Webots:  [https://github.com/omichel/blender-webots-exporter](https://github.com/omichel/blender-webots-exporter)

##### OnlyPtuuu 07/08/2019 13:44:11
[http://doc.aldebaran.com/2-4/software/webots/webots\_index.html#webots-launching](http://doc.aldebaran.com/2-4/software/webots/webots_index.html#webots-launching) 

I want only use one robot like this guide


I think that work with pepper

##### Fabien Rohrer [Moderator] 07/08/2019 13:56:44
Unfortunately, there is not pepper robot 3d model in Webots


the guide you mention is done for the Nao only

##### OnlyPtuuu 07/08/2019 14:18:29
Ok I see


Thank you very much for helping

##### Fabien Rohrer [Moderator] 07/08/2019 14:25:20
you're welcome, good luck with your project 😉

##### Huey 07/08/2019 15:33:43
anyone knows anywhere to shop for electronics components/accessories in London? (other than RS in Surrey Quay)

##### David Mansolino [Cyberbotics] 07/08/2019 15:34:35
Hi, I am sorry but this chat is dedicated to Webots, you will for sure find more appropriate answers on dedicated forums, for example: [https://robotics.stackexchange.com/](https://robotics.stackexchange.com/)

##### Huey 07/08/2019 15:37:15
cool.. thanks! sorry again for the random unrelated question

##### David Mansolino [Cyberbotics] 07/08/2019 15:38:01
No problem, you're welcome

##### MariusJuston [Moderator] 07/08/2019 15:49:52
Hello, is there a way to make the program "wait" until the set\_position of a motor is finished? If that is not possible is it possible to get the current position of a RotationMotor, please? Many thanks

##### David Mansolino [Cyberbotics] 07/08/2019 15:54:37
Yes, the idea is to use a position sensors([https://www.cyberbotics.com/doc/reference/positionsensor](https://www.cyberbotics.com/doc/reference/positionsensor)) and to call the robot step function until the desired position is reached.

##### MariusJuston [Moderator] 07/08/2019 15:57:19
Doesn't the Motor object have to know its own position in order for set position to work? Does it not store that information as a variable that could be accessed?

##### David Mansolino [Cyberbotics] 07/08/2019 15:57:57
No, it only stores the target position, you should use the position sensor associated to the same joint that the motor

##### MariusJuston [Moderator] 07/08/2019 15:59:02
Okay, thank you

##### David Mansolino [Cyberbotics] 07/08/2019 15:59:07
You're welcome

##### MariusJuston [Moderator] 07/08/2019 17:23:45
Sorry me again, is there a way to retrieve the bounding box of an node? If so if the bounding box was a box would it be possible to extract the height? I would like to implement this in Python, please. Many thanks

##### Fabien Rohrer [Moderator] 07/08/2019 18:32:10
The Supervisor API can be used for retrieving any node field, typically the position or the size IF these fields are open! You can retrieve programmatically all what you sea in the tree view on the left. There is currently no method to retrieve the AABB.

##### MerySgh 07/08/2019 23:40:56
hello everyone.....how can i add some characters in my webots project ,something like pedestrians in the  road or like  human ...please help me

##### Fabien Rohrer [Moderator] 07/09/2019 06:57:14
Please look at this world file: $(WEBOTS\_HOME)/projects/humans/pedestrian/worlds/pedestrian.wbt


.. and the related movie: [https://www.youtube.com/watch?v=tmz3JRuZT9A](https://www.youtube.com/watch?v=tmz3JRuZT9A)


There is a Pedestrian PROTO file that you can use everywhere, its parameters can be used to define an itinerary in absolute coordinates, speed, etc.

##### aji 07/09/2019 07:57:23
Hello! I have a question regarding WeBots and Choregraph (for NAO bot). Does anybody know why there are no robots on the list of connection in Choregraph?


I tried running a sample world with a NAO bot on it, then my choregraph wont show the NAO bot on the "Make a connection" list

##### Fabien Rohrer [Moderator] 07/09/2019 07:59:45
Hi,  we don't support anymore naoqisim (the controller responsible in doing the connection between Webots and Choregraph) because SoftBank abandoned the support of the library doing this.


That said, did you tried basic things:


- the controller and Webots should be in run mode


- the ports should match (an argument of naoqisim if I remember well)


- The port should not be blocked (by another badly closed connection, or by your firewall)


- Are you sure naoqisim is running smoothly? (what do you have in the Webots console)


I hope this will gives you clues to go ahead.

##### aji 07/09/2019 08:03:01
Ah, that's quite unfortunate.


Will try to do the following things, i just set up both of the programs so i havent quite tinkered with it yet

##### Fabien Rohrer [Moderator] 07/09/2019 08:04:38
Defintively! The decision of SoftBank to drop the support of any simulator is weird, many users/customers are complaining about this. I recommend you to send them an e-mail to complain about this.


(the more we are to complain, the best chance we have to change the situation 😃 )

##### aji 07/09/2019 08:08:27
That's a good suggestion. They clearly didnt involve as much features as webots on their own choregraph simulator (i.e. speech recognition).


Thanks for the help btw 😄

##### guxgux 07/09/2019 20:10:24
hello


some  know have to connecto webot to  real robot


with microcontroler

##### Fabien Rohrer [Moderator] 07/09/2019 20:20:02
Hi, maybe you could create a remote control plugin? [https://www.cyberbotics.com/doc/guide/controller-plugin#remote-control-plugin](https://www.cyberbotics.com/doc/guide/controller-plugin#remote-control-plugin)

##### guxgux 07/09/2019 20:26:33
thanks l will try to do

##### speedy6451 07/10/2019 00:51:28
are mecanum wheels supported?

##### Fabien Rohrer [Moderator] 07/10/2019 06:36:53
Hi, yes they are!


You could either implement the entire mechanism with joints and cylinders for each sub wheel, like in this example: [https://cyberbotics.com/doc/guide/samples-howto#omni\_wheels-wbt](https://cyberbotics.com/doc/guide/samples-howto#omni_wheels-wbt)


Or you could use the asymmetric friction (cf. WorldInfo.contactProperties.coulombFriction ) to simulate sub wheels, like in the Kuka youbot robot: [https://cyberbotics.com/doc/guide/youbot](https://cyberbotics.com/doc/guide/youbot)


When implementing the youBot, I have done lot of tests related to this, and the second solution (the one used by the youBot) is much more accurate, robust and quick to solve of our physics engine.


In this movie (at time = 71s), we can see an older version of the youBot with the subwheels entirely modeled: [https://youtu.be/vFwNwT8dZTU?t=71](https://youtu.be/vFwNwT8dZTU?t=71) but this has been dropped to use the asymmetric friction in newer versions.

##### Huey 07/10/2019 12:00:39
Hi, i am trying to follow this tutorial to build a simple robot, when i get to the end,  trying to run the simulation, i got these messages
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/598483724291670017/unknown.png)
%end


any idea what might have went wrong?

##### David Mansolino [Cyberbotics] 07/10/2019 12:01:45
You should check the name of the wheel 1 motor, does it matches the one used in your controller (e.g. 'wheel1') ?

##### Huey 07/10/2019 12:03:17

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/598484390116327455/unknown.png)
%end


if i understand it correctly i named one of the HingeJoint as wheel1

##### David Mansolino [Cyberbotics] 07/10/2019 12:04:20
The name of the motor device is defined in the HingeJoint->device->RotationalMotor->name

##### Huey 07/10/2019 12:06:39
Thanks mate!!! That s it!

##### David Mansolino [Cyberbotics] 07/10/2019 12:06:48
You're welcome

##### Huey 07/10/2019 12:07:28
although now i have other issues to resolve


some axis might have been set wrongly, it s now crawling like a baby monster



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/598485527833870356/unknown.png)
%end

##### David Mansolino [Cyberbotics] 07/10/2019 12:10:21
You shoul dcheck the joint anchor, they are probably wrong...

##### R\_LCL 07/10/2019 14:00:51
Hello! Can the software simulate wheel slips? Preferably on an inclined surface or on a slippery surface.

##### Fabien Rohrer [Moderator] 07/10/2019 14:01:43
Hi,


Yes, you should play with the WorldInfo.contactProperties, particularily the one dealing with the contact between the surface and the wheel.


[https://www.cyberbotics.com/doc/reference/contactproperties](https://www.cyberbotics.com/doc/reference/contactproperties)


in particular, the coulombFriction and the forceDependentSlip fields

##### R\_LCL 07/10/2019 14:16:37
thank you man.

##### MariusJuston [Moderator] 07/10/2019 22:19:40
Hello is there a simple way to make a robot arm move smoothly? When I use setPosition on the joints it is very choppy because it seems that it is moving the joints sequentially. Is it possible to move the joints in parallel in Python? Also is there a way to do some kind of object bound detection and have the program display an alert that the housing boxes gave collided? Many thanks!

##### ThundrHawk 07/10/2019 22:42:19
If you use set velocity, they'll move at the same time. you would just then have to figure out when to stop then

##### speedy6451 07/10/2019 22:42:53
How do you change the language of a robot controller?

##### ThundrHawk 07/10/2019 22:43:21
you just have to write it in a new language and tell the robot to use that one

##### speedy6451 07/10/2019 23:16:52
I figured it out, thanks!

##### David Mansolino [Cyberbotics] 07/11/2019 06:22:46
`@MariusJuston` about the boxes collision, the simplest solution is probably to add a touchSensors ([https://cyberbotics.com/doc/reference/touchsensor](https://cyberbotics.com/doc/reference/touchsensor)) to your robot arm.

##### MariusJuston [Moderator] 07/11/2019 14:08:00
Thank you. Sorry again but I seem to have some issues with the PositionSensors, I have not moved the robot yet and the values the sensors are returning to me are not 0. I assume that that is not normal. Is there a way to reset/zero the sensor?

##### David Mansolino [Cyberbotics] 07/11/2019 14:18:06
The position sensors will return the value of the jointparameter.position field (if any) is it not the case ?

##### ThundrHawk 07/11/2019 14:29:16
We figured it out. We had set the position to infinity when we grabbed them from the supervisor, but we hadn't set their velocity to 0 at that time. We only did it after they had already moved a bit.

##### David Mansolino [Cyberbotics] 07/11/2019 14:30:16
Ok, thank you for the feedback, that sounds indeed like a very good reason.

##### R\_LCL 07/11/2019 17:55:59
Hello, I need help with the getVelocity() function. I used the getFromDef function and assigned it to my robot. But, as you can see in the  attached picture, the velocity values that I'm getting are all zeroes. May I know what am I doing wrong?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/598935535159083038/unknown.png)
%end

##### MariusJuston [Moderator] 07/11/2019 18:36:12
`@R_LCL` it seems that you are retrieving your velocity at initialization when the robot is not moving. If you want to retrieve the velocities continuously you want to use the getVelocity method inside the while loop


setVelocity is nice and all but if possible I would like to use setPosition to make my life easier. Is there a way to multi-thread setPosition?

##### David Mansolino [Cyberbotics] 07/11/2019 19:04:00
`@MariusJuston` , the idea is actually to use both the setPosition and setVelocity function, the first sets the target position and the second defines the maximum allowed velocity to reach this target position.

##### MariusJuston [Moderator] 07/11/2019 19:14:14
I understand that; however, I am using a 6 DOF robot arm and I want to make the joints in sync and thus make the motion seem smooth, with the setPosition it is very choppy because it is moving the motors sequentially.

##### David Mansolino [Cyberbotics] 07/11/2019 19:17:19
You should be able to call the setPosition for each motor at the same step and they should move all in parallel isn't it the case?

##### MariusJuston [Moderator] 07/11/2019 19:17:46
No it does not seem so

##### David Mansolino [Cyberbotics] 07/11/2019 19:18:14
Here is for example a robot arm controlled in Webots: [https://www.youtube.com/watch?v=Jq0-DkEwwj4](https://www.youtube.com/watch?v=Jq0-DkEwwj4)

##### MariusJuston [Moderator] 07/11/2019 19:18:51
Yes how they do it is that they use setPosition in very small intervals

##### ThundrHawk 07/11/2019 19:21:38

> **Attachment**: [its\_choppy.mp4](https://cdn.discordapp.com/attachments/565154703139405824/598957091700473856/its_choppy.mp4)


we may be using it wrong, but we're running this bit of code:

```python
for motor, position in zip(motors, positions):
    motor.setPosition(position)
    motor.setVelocity(MAX_V)
```


I've tried running those functions with the threading library to the same results. I could be doing something wrong with that, though

##### David Mansolino [Cyberbotics] 07/11/2019 19:25:22
Using thread should not help because in any case the simulation time will increase only when you call the robot step function.

##### ThundrHawk 07/11/2019 19:25:33
makes sense

##### David Mansolino [Cyberbotics] 07/11/2019 19:25:58
But the bit of code you sent seems correct, this should work


You should probably try reproducing the problem in a minimalistic example (e.g. just two DOF) and post an issue with this example on github: [https://github.com/omichel/webots/issues/new/choose](https://github.com/omichel/webots/issues/new/choose)

##### ThundrHawk 07/11/2019 19:29:21
sounds good

##### MariusJuston [Moderator] 07/11/2019 19:58:19
Done: [https://github.com/omichel/webots/issues/711](https://github.com/omichel/webots/issues/711)

##### MerySgh 07/12/2019 01:31:20
hello everyone , i have a problem of adding pedestrians in my project its give me a error ..please how to solve it
%figure
![error.PNG](https://cdn.discordapp.com/attachments/565154703139405824/599050129537892381/error.PNG)
%end

##### ThundrHawk 07/12/2019 01:55:04
the first thing you should try is adding PYTHONPATH as an environment variable: [https://stackoverflow.com/a/4855685](https://stackoverflow.com/a/4855685)

then if you're having more trouble, go into Tools>Preferences and change the python command to the python executable. mine is:

`C:\Users\Russell\AppData\Local\Programs\Python\Python37\python`


ideally, changing the environment variable will work, but it can be tricky


make sure to restart webots after making changes to your environment variables

##### MerySgh 07/12/2019 01:56:31
okay i will try

##### MariusJuston [Moderator] 07/12/2019 01:59:34
`@MerySgh`  Otherwise you can just pass in the Python.exe path in webots ->  Tools -> Preferences -> python. i.e: C:\Users\\...\Anaconda3\envs\\...\python.exe

##### MerySgh 07/12/2019 02:01:43
what next ?
%figure
![python.PNG](https://cdn.discordapp.com/attachments/565154703139405824/599057772369281029/python.PNG)
%end

##### MariusJuston [Moderator] 07/12/2019 02:01:58
Replace the python command with the path of the python exe

##### MerySgh 07/12/2019 02:02:20
where i can find this this path ?

##### MariusJuston [Moderator] 07/12/2019 02:02:43
Wait, have you already added the python path to the environmental variable?

##### MerySgh 07/12/2019 02:03:09
NOT yet

##### MariusJuston [Moderator] 07/12/2019 02:03:49
Okay that is good. Is you python a virtual environment?

##### ThundrHawk 07/12/2019 02:05:13
if you're using python37, the path is most likely going to be C:\Users\<user>\AppData\Local\Programs\Python\Python37\python.exe

##### MerySgh 07/12/2019 02:05:46
`@MariusJuston`  sorry i don't understand you


`@ThundrHawk` i will try it

##### MariusJuston [Moderator] 07/12/2019 02:06:47
```python
import sys
print(sys.executable)
```


This will give you the Python path of you running python.exe


Paste that in the  Python command field

##### ThundrHawk 07/12/2019 02:07:29
you're probably not using a virtual environment if you're not programming in an ide

##### MerySgh 07/12/2019 02:10:15
like this ?
%figure
![python_command.PNG](https://cdn.discordapp.com/attachments/565154703139405824/599059921559879700/python_command.PNG)
%end

##### MariusJuston [Moderator] 07/12/2019 02:11:25
`@MerySgh`  No sorry about that it seems that I was not very clear. That was a Python command. You can open up your python console and run those commands to give you the location. When you install Python it comes with  IDLE search for that and then run the command


`@MerySgh`  If you think that you are only going to be using a single Python interpreter for all your projects/worlds it might just be easier for you to place your python path as an environmental variable.

##### MerySgh 07/12/2019 02:15:29
mmmmm ok I will try, I'm sorry if you don't understand quickly it's because I'm still beginner I trying to  be adapted to these technical words

##### MariusJuston [Moderator] 07/12/2019 02:17:21
No problems. I will be sure to clarify as much as I can.


`@MerySgh` So a Python Interpreter is what contains the python.exe and contains all the libraries and dependencies associated with it (as far as I understand it).

A virtual environment in python is useful if you have multiple projects and these projects differ. Such a vision processing project and a text editor project. As you can imagine these for two projects you would need very different libraries. If you used a single interpreter you would have to install the libraries to a single python interpreter, meaning that each project would contain both python projects libraries because they are both referencing the same interpreter and you are installing the libraries to a single interpreter. If you use virtual environments it copies over to a new folder a "clean" copy of your python base interpreter as a separate environment. In the previous example, you would create 2 virtual environments: one for the vision processing project and one for the text editor project. That would allow you to install the libraries for the vision processing project and which would only be available to that project and same with the text editor libraries and environment. This allows your python interpreters to be more lightweight and usually makes it easier for IDEs, you also delete these virtual environments without needing to reinstall Python, etc... (this is how I understand it)

I personally really like Anaconda for creating virtual environments and PyCharm as my Python IDE. Anaconda and PyCharm make it very easy to install libraries and create virtual environments for your projects. (this is of course just my opinion there are plenty more of other IDE and Virtual environment options)

##### R\_LCL 07/12/2019 13:41:33
Hi, can anyone help me in adding a DifferentialWheels node for the e-puck robot (since I really need to get the speed of the wheels). I tried following the guide from the website but I have not make it work yet.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/599233895132823552/unknown.png)
%end

##### David Mansolino [Cyberbotics] 07/12/2019 13:52:04
Hi, I am not sure to understand your question, the e-puck robot is already a differential wheel, you should simply use its position-sensors to get the wheel position.


(which you can then derive to get the speed)

##### R\_LCL 07/12/2019 13:58:59
I was referring to this one: → [https://www.cyberbotics.com/doc/reference/differentialwheels](https://www.cyberbotics.com/doc/reference/differentialwheels). Since it has some of the functions that I need for my project. (slipNoise, speed, etc)

##### David Mansolino [Cyberbotics] 07/12/2019 14:00:08
I am sorry but this node is deprecated, it is highly recommended to use the regular robot node instead (and the e-puck model has been updated accordingly)


You can add noise on the position-senors too, the speed can be retrieved, etc. all the functionnalities are also available.

##### R\_LCL 07/12/2019 14:14:00
Ohhhhh.  I will try getting the speed from the position-sensors as you have said. Thanks man.

##### David Mansolino [Cyberbotics] 07/12/2019 14:29:03
You're welcome

##### Deleted User 07/14/2019 15:26:01
During motion of NAO, hands are kept stable it is not moving, like we move our hand. When it is moving forward or backward hand are kept stable 

Shoulder pitch  motor is not working in NAO (i think)

I have attached the picture,

it is in this position for all the time.



please help me to move it
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/599984961071874052/unknown.png)
%end

##### NearMonzter 07/14/2019 19:01:40
Hi there, I don't know if here answer kinda programming questions, but I'll make the question anyway


Last week I was working with my three wheels omnidirectional robot that I created,  so far I've programmed it into two mode (manual and autonomous), so far so good, the issue is when in the autonomous mode, when I detect an obstacle through a distance sensor I want my robot to stop and THEN, if the obstacle is more to the left TURN the robot to the right, however I don't know how to make the robot  wait a little bit stopped a little bit and then turn the robot

##### Fabien Rohrer [Moderator] 07/14/2019 19:48:18
`@NearMonzter` which language are you using? Do you know finite state machines and how to program it? [https://en.m.wikipedia.org/wiki/Finite-state\_machine](https://en.m.wikipedia.org/wiki/Finite-state_machine)


`@Deleted User` are you using the motion files embedded in Webots?

##### NearMonzter 07/14/2019 22:45:03
I'm programming in C, I think I need a function that kinda stops the robot a little bit when I set velocities to 0 and then after some time the robot turns right, I have the two functions, the one that stops the robot and the one that turns the robot, however since the loop is so fast I don't know how to simulate that 'STOP'


Please, help

##### Fabien Rohrer [Moderator] 07/15/2019 04:27:02
You could take a copy-paste the « wait\_a\_while » function (and the required « step » function defined above) in your code: [https://github.com/omichel/webots/blob/revision/projects/robots/kinematics/tinkerbots/controllers/crane/crane.c#L33](https://github.com/omichel/webots/blob/revision/projects/robots/kinematics/tinkerbots/controllers/crane/crane.c#L33)


This function calls the wb\_robot\_step() function several times until reaching the expected time (in seconds) It is based on the wb\_robot\_get\_time() function. You should obviously stop the motors before calling « wait\_a\_while ».


We are trying to carry a duty period out during business hours (European time). Out of this period, answers may come with a certain delay 😉

##### Akash 07/15/2019 09:36:16
Is there any example file in webots, where the accelerometer is been used?

##### Fabien Rohrer [Moderator] 07/15/2019 09:36:32
Hi, yes, many.


Let me check..


In WEBOTS\_HOME/projects/robots/gctronic/e-puck/worlds/e-puck\_cross-compilation.wbt the accelerometer is used to analyse the slope inclination, the most bottom led is enabled accordingly.


Is it sufficient?

##### Akash 07/15/2019 09:57:36
What should be the values of lookup table  for accelerometer and how could i convert it to m/s^2 from vector values?

##### Fabien Rohrer [Moderator] 07/15/2019 10:00:05
The lookupTable, is simply a function converting the vector in m/s^2 (typically [0, -9.81, 0]) to something else. It is applied on each channel independently.


You may find here how a lookup table is working: [https://cyberbotics.com/doc/reference/distancesensor#lookup-table](https://cyberbotics.com/doc/reference/distancesensor#lookup-table)


So, to have the values of an accelerometer without alterating it, you could define a lookupTable like this:


lookupTable []


The default lookupTable does not do any interpolation.

##### Deleted User 07/15/2019 10:59:42
During motion of NAO, hands are kept stable it is not moving, like we move our hand. When it is moving forward or backward hand are kept stable 

Shoulder pitch  motor is not working in NAO (i think)

I have attached the picture,

it is in this position for all the time.



please help me to move it
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/600280326216089601/unknown.png)
%end

##### Fabien Rohrer [Moderator] 07/15/2019 11:56:50
`@Deleted User` are you using the motion files embedded in Webots? In which language?

##### Deleted User 07/15/2019 12:43:40
I have used nao\_demo.c

It is in C language


`@Fabien Rohrer` I have used nao\_demo.c

It is in C language

##### Fabien Rohrer [Moderator] 07/15/2019 12:47:30
In the motion file you are playing, probably that the hands are not actuated. When a motion is played, you can still use the wb\_motor\_set\_position() on the arm motors. Could this be a solution for you?

##### Deleted User 07/15/2019 12:49:34
"wb\_motor\_set\_position() on the arm motors"



yes it is helpfull

But can you explain the process to do this

Because i am new for C language.

`@Fabien Rohrer`

##### Fabien Rohrer [Moderator] 07/15/2019 12:50:44
yes, just one minute, I will send you a code snippet here.

##### Deleted User 07/15/2019 12:51:07
Thank you  sir

i am waiting

##### Fabien Rohrer [Moderator] 07/15/2019 12:55:19
In this C file, you can see how to move the right elbow when playing a motion file.
> **Attachment**: [move\_arm\_while\_playing\_a\_motion.c](https://cdn.discordapp.com/attachments/565154703139405824/600309424325787680/move_arm_while_playing_a_motion.c)


(I didn't test this code snippet 😉 )

##### Deleted User 07/15/2019 13:28:57
Ok sir, i will test and will inform you, thank you


nao\_demo.c: In function 'main':

nao\_demo.c:453:35: error: 't' undeclared (first use in this function)

   wb\_motor\_set\_position(hand, sin(t)); // move the elbow according to a sinusoidal function.

                                   ^

nao\_demo.c:453:35: note: each undeclared identifier is reported only once for each function it appears in

nao\_demo.c:452:10: warning: unused variable 'time' [-Wunused-variable]

   double time = wb\_robot\_get\_time(2); // get the simulated time in seconds.

          ^~~~

make: *** [C:/Program Files/Webots/resources/Makefile.include:679: build/release/nao\_demo.o] Error 1

make: *** Waiting for unfinished jobs....













Here is the error, after solving two errors  ( WbDevice to WbDeviceTag and TIME\_STEP to time\_step)


`@Fabien Rohrer`

##### Fabien Rohrer [Moderator] 07/15/2019 14:22:18
please change "t" to "time", sorry for this



> **Attachment**: [move\_arm\_while\_playing\_a\_motion.c](https://cdn.discordapp.com/attachments/565154703139405824/600331379800145921/move_arm_while_playing_a_motion.c)


please change "t" to "time", sorry for this



> **Attachment**: [move\_arm\_while\_playing\_a\_motion.c](https://cdn.discordapp.com/attachments/565154703139405824/600334204315893781/move_arm_while_playing_a_motion.c)

##### Deleted User 07/15/2019 14:36:07
thank you sir

but i have asked for the shoulder motion because it is not moving the hand while moving.

I replaced the elbow command to shoulder pitch


It is working now


But now only shoulder is moving, robot is not moving


And showing this error again and again

[nao\_demo] Error: wbu\_motion\_is\_over() called with NULL argument

`@Fabien Rohrer`


[NAo] Error: wbu\_motion\_new(): could not open '../../motions/HandWave.motion' file.

[NAo] Error: wbu\_motion\_new(): could not open '../../motions/Forwards50.motion' file.

[NAo] Error: wbu\_motion\_new(): could not open '../../motions/Backwards.motion' file.

[NAo] Error: wbu\_motion\_new(): could not open '../../motions/SideStepLeft.motion' file.

[NAo] Error: wbu\_motion\_new(): could not open '../../motions/SideStepRight.motion' file.

[NAo] Error: wbu\_motion\_new(): could not open '../../motions/TurnLeft60.motion' file.

[NAo] Error: wbu\_motion\_new(): could not open '../../motions/TurnRight60.motion' file.

[NAo] Error: wbu\_motion\_set\_loop() called with NULL argument.

[NAo] Error: wbu\_motion\_play() called with NULL 'motion' argument.

[NAo] Error: wbu\_motion\_new(): could not open 'walk.motion' file.

[NAo] Error: wbu\_motion\_play() called with NULL 'motion' argument.

[NAo] Error: wbu\_motion\_is\_over() called with NULL argument.


This is what happening now
> **Attachment**: [NAo.mp4](https://cdn.discordapp.com/attachments/565154703139405824/600336628703166465/NAo.mp4)

##### Fabien Rohrer [Moderator] 07/15/2019 14:52:42
Which motion file are you trying to play?

##### Deleted User 07/15/2019 14:53:44
i have just pasted your programme in NAO\_demo.c after hand wave


i am doing nothing other than this

##### Fabien Rohrer [Moderator] 07/15/2019 14:55:03
ok, let me check..

##### Deleted User 07/15/2019 14:55:12
yes sir

##### Fabien Rohrer [Moderator] 07/15/2019 14:59:52
would it be ok if I sent you a C controller with the Forwards50.motion playing while moving an elbow?

##### Deleted User 07/15/2019 15:00:15
yes sir


moving the shoulder sir


Actually i want to perform task of moving the shoulder while moving forward

Means i was trying to imitate the human walking

##### Fabien Rohrer [Moderator] 07/15/2019 15:06:09
Here is a complete and working controller which does this. to replace nao\_demo.c
> **Attachment**: [nao\_demo.c](https://cdn.discordapp.com/attachments/565154703139405824/600342346579836930/nao_demo.c)


.. and the result
> **Attachment**: [nao\_demo.mp4](https://cdn.discordapp.com/attachments/565154703139405824/600342639577006102/nao_demo.mp4)

##### Deleted User 07/15/2019 15:15:06
Yes sir, its working,

But again shoulder is not moving 



Sorry to disturb you

And thank you for your help



Can't we make NAO move hand as we do while walking

##### Fabien Rohrer [Moderator] 07/15/2019 15:25:21
In the controller I sent you, you can refer to another motor, for example by changing "RElbowYaw" to "RShoulderPitch"



> **Attachment**: [nao\_demo\_1.mp4](https://cdn.discordapp.com/attachments/565154703139405824/600347332051075102/nao_demo_1.mp4)

##### Deleted User 07/15/2019 15:27:15
Yes sir 

I tried the same 



Isnt this little ocward

Means, the movement of hand 



How to change that angle sir

##### Fabien Rohrer [Moderator] 07/15/2019 15:32:11
In the code I sent you, the shoulder is actuated by a sin() function. You could play with this by adding constants, for example:


AMPLITUDE * sin(FREQUENCY * time) + OFFSET


You may find values for these constants in order to match with the gait.

##### Deleted User 07/15/2019 15:37:33
Ok sir i will try and inform


thank you so much

##### Fabien Rohrer [Moderator] 07/15/2019 15:37:55
You’re welcome 😉

##### Deleted User 07/15/2019 17:14:03
wbu\_motion\_play() called with NULL 'motion' argument





Why this error appears ??

##### Fabien Rohrer [Moderator] 07/15/2019 17:50:56
It could be explained if the path to « Forwards50.motion » is wrong. Could you check this file exists? It is defined relatively to your controller directory.

##### Deleted User 07/15/2019 17:55:33
I was working on it for about 2 and half hours 

But unable to get the resulf 😔 



Sir my objective is simple



I want the same control as in Nao\_demo.c



I want to make it walk when i press the button as mentioned in demo code. 

The only thing i am not getting is the hand movement as we do.



I need to control it by pressing the button 

Means it should move when i press the button along with the shoulder pitch


i am pressing the forward button in Nao\_demo.c controller, it is moving forward but hand is not moving.



What i need is, when i press the forwars button it should move along with moving the hand.

##### Fabien Rohrer [Moderator] 07/15/2019 18:08:10
Sorry to read this 😕 I think you should focus first with running the controller I sent you. I tested it and it is working.


If it’s too difficult, you should give a try to our tutorial

##### Deleted User 07/15/2019 18:08:52
Yes sir it is working,

##### Fabien Rohrer [Moderator] 07/15/2019 18:09:09
And to C programming tutorials


Ok


To deal with keyboard input, please take a look at the Keyboard API and snippets:


[https://www.cyberbotics.com/doc/reference/keyboard](https://www.cyberbotics.com/doc/reference/keyboard)

##### Deleted User 07/15/2019 18:11:09
But i needed to do some changes in the nap\_demo.c controller



Where it moves while pressing the button



Can't you do some changes in that demo code , so that it will move its hand while walking, when i press the forward bottom ??

##### Fabien Rohrer [Moderator] 07/15/2019 18:12:31
Yes that’s simple


I’ll do it in 5 minutes

##### Deleted User 07/15/2019 18:14:00
Sir, please help me then,

##### Fabien Rohrer [Moderator] 07/15/2019 18:23:00
Here is a new controller. Please try to improve it by yourself 😉
> **Attachment**: [nao\_demo.c](https://cdn.discordapp.com/attachments/565154703139405824/600391887332966514/nao_demo.c)


I have to leave, have a good evening.


I'll be back tomorrow

##### Deleted User 07/16/2019 04:36:29
This is happening with the following error



[NAo] Error: wbu\_motion\_new(): could not open '../../motions/Forwards50.motion' file.

[NAo] Error: wbu\_motion\_play() called with NULL 'motion' argument.

[NAo] Error: wbu\_motion\_is\_over() called with NULL argument.
> **Attachment**: [NAo\_1.mp4](https://cdn.discordapp.com/attachments/565154703139405824/600546275729080321/NAo_1.mp4)


after modifications


Sir, here is the demo code, please do some modification in it, so that it moves its hand while walking.

please sir, my whole project is stopped due to this simple things (So big for me).



Sir please help

`@Fabien Rohrer`
> **Attachment**: [nao\_demo.c](https://cdn.discordapp.com/attachments/565154703139405824/600547754850254886/nao_demo.c)

##### Fabien Rohrer [Moderator] 07/16/2019 06:49:55
You have this error because of this line:


WbMotionRef forwards = wbu\_motion\_new("../../motions/Forwards50.motion");


The motion file cannot be found, therefore, forwards == NULL.


Please check in your file system that "PATH\_TO\_YOUR\_CONTROLLER/../../motions/Forwards50.motion" exists, if not, copy-paste it.

##### Deleted User 07/16/2019 06:54:15
Good morning sir,

ok sir, i will try it



Can you insert the shoulder pitch option in the demo code i send you in previous message?

please sir, i small request.

##### Fabien Rohrer [Moderator] 07/16/2019 06:56:31
I'm sorry, but I will stop here. I already sent you a demo code doing exactly what you need (a rare thing on this support channel). I don't think I can help you more. Please try to run my sample before going ahead.

##### Deleted User 07/16/2019 08:45:53
Ok sir 

Thank you so much for your help 😊

##### BG 07/16/2019 13:32:48
hello

##### Fabien Rohrer [Moderator] 07/16/2019 13:33:06
Hi BG, how could we help you?

##### ThundrHawk 07/16/2019 13:43:06
He was looking for tutorials, so I sent him to [https://cyberbotics.com/doc/guide/getting-started-with-webots](https://cyberbotics.com/doc/guide/getting-started-with-webots)

##### Fabien Rohrer [Moderator] 07/16/2019 13:43:50
Thank you ^^ this is for sure a good reference


BG also contacted me in private, I redirected him to our official tutorial: [https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials)


`@BG` You should communicate on this public channel, we are here to help you 😃

##### MariusJuston [Moderator] 07/16/2019 13:57:51
Hello, good gentlemen's i am trying to run a python controller from PyCharm without needing to run Webots. The problem is that it says that: 

```python
Traceback (most recent call last):
  File "C:/Users/***/Documents/GitHub/***/***/controllers/state_controller/state_controller.py", line 40, in <module>
    from controller import Supervisor, Motor
  File "C:\Users\***\AppData\Local\Programs\Webots\lib\python37\controller.py", line 17, in <module>
    import _controller
ImportError: DLL load failed: The specified module could not be found.
```

I am trying to load the DLLs; however, I have not managed to find them. I have added `'C:\\Users\\***\\AppData\\Local\\Programs\\Webots\\lib\\python37'` to my `sys.path` variable already.

What can I do to make this work?

Many thanks!

##### Fabien Rohrer [Moderator] 07/16/2019 14:00:16
Hi Marius,


Yes, it seems to be a DLL or path issue.


the path you are including is indeed correct and should contain all the Python3 stuff. I will just check this to confirm it.

##### Olivier Michel [Cyberbotics] 07/16/2019 14:03:23
Hi Marius, you probably need to add another path:


C:\\Users\\***\\AppData\\Local\\Programs\\Webots\\msys64\\mingw64\\bin


So that Python will be able to load the Controller.dll library.

##### MariusJuston [Moderator] 07/16/2019 14:37:13
`@Olivier Michel` it seems that it is still doing the same thing...

##### Olivier Michel [Cyberbotics] 07/16/2019 14:38:05
Then, can you try to add this path to your "Path" environment variable?

##### MariusJuston [Moderator] 07/16/2019 14:38:19
Will do!


`@Olivier Michel`  Still does not work even when I added it to my Path environmental variable

##### Olivier Michel [Cyberbotics] 07/16/2019 14:41:21
Do you get the same error?

##### MariusJuston [Moderator] 07/16/2019 14:41:26
yes


I can see it in `sys.path` though


I do not have the WEBOTS\_HOME environmental variable is that something I should set (I am in Windows)? Am I supposed to do that myself or is the installer supposed to do that?

##### Olivier Michel [Cyberbotics] 07/16/2019 14:49:39
Yes.


No, you should do it yourself.


Did you follow the instructions here: [https://cyberbotics.com/doc/guide/running-extern-robot-controllers?](https://cyberbotics.com/doc/guide/running-extern-robot-controllers?)

##### MariusJuston [Moderator] 07/16/2019 14:52:08
Still no... 
```bash
echo %WEBOTS_HOME%
C:\Users\***\AppData\Local\Programs\Webots\
```

##### Olivier Michel [Cyberbotics] 07/16/2019 14:52:29
There is quite a large number of environment variables to setup.

##### MariusJuston [Moderator] 07/16/2019 14:54:07
Would you be so kind as to inform me of what they are please?

##### Olivier Michel [Cyberbotics] 07/16/2019 14:54:27
They are all documented on the web page I pointed out.

##### MariusJuston [Moderator] 07/16/2019 14:54:44
Sorry I did not see the link

##### Olivier Michel [Cyberbotics] 07/16/2019 14:54:46
[https://cyberbotics.com/doc/guide/running-extern-robot-controllers](https://cyberbotics.com/doc/guide/running-extern-robot-controllers)


(there was an extra question mark making my previous link wrong)

##### MariusJuston [Moderator] 07/16/2019 14:58:30
By the way the documentation for that table is probably outdated becasue for windows it is saying that the WEBOTS\_HOME directory is at C:\Program Files\Webots when it is actually: C:\Users\***\AppData\Local\Programs\Webots\

##### Olivier Michel [Cyberbotics] 07/16/2019 14:59:39
Yes, that's because you installed Webots for a single user (without administrator privileges).

##### MariusJuston [Moderator] 07/16/2019 15:00:00
makes sense

##### Olivier Michel [Cyberbotics] 07/16/2019 15:00:15
So, it is correct to set it to C:\Users***\AppData\Local\Programs\Webots in your case.

##### MariusJuston [Moderator] 07/16/2019 15:04:48
Again I have setup all the environmental variables (except for the Python PYTHONPATH and PYTHONENCODING because I am using an env) for Windows, WEBOTS\_HOME and added mingw64 to the environment. It still does not work... same old error

##### Olivier Michel [Cyberbotics] 07/16/2019 15:05:25
Let me try it.

##### MariusJuston [Moderator] 07/16/2019 15:08:58
here are the returns of `sys.path` for when it is run on Webots and PyCharm respectively:

```python
webots_environmental_variables = [
    'C:\\Users\\***\\Documents\\GitHub\\***\\***\\controllers\\state_controller',
    'C:\\Users\\***\\AppData\\Local\\Programs\\Webots\\lib\\python37',
    'C:\\Users\\***\\Anaconda3\\envs\\***\\python37.zip',
    'C:\\Users\\***\\Anaconda3\\envs\\***\\DLLs', 'C:\\Users\\***\\Anaconda3\\envs\\***\\lib',
    'C:\\Users\\***\\Anaconda3\\envs\\***',
    'C:\\Users\\***\\Anaconda3\\envs\\***\\lib\\site-packages']

pycharm_environmental_variables = [
    'C:\\Users\\***\\Documents\\GitHub\\***\\***\\controllers\\state_controller',
    'C:\\Users\\***\\AppData\\Local\\Programs\\Webots\\msys64\\mingw64\\bin',
    'C:\\Users\\***\\AppData\\Local\\Programs\\Webots\\lib\\python37',
    'C:\\Users\\***\\Documents\\GitHub\\***\\***\\controllers',
    'C:\\Users\\***\\Documents\\GitHub\\***\\***\\controllers\\state_controller',
    'C:\\Users\\***\\Documents\\GitHub\\***\\***\\controllers\\inverse_kinematics',
    'C:\\Users\\***\\Documents\\GitHub\\***\\***\\controllers\\demo_controller',
    'C:\\Users\\***\\Documents\\GitHub\\***\\***\\controllers\\complete_controller',
    'C:\\Users\\***\\Documents\\GitHub\\***\\***\\controllers\\collision_detection',
    'C:\\Users\\***\\AppData\\Local\\JetBrains\\Toolbox\\apps\\PyCharm-P\\ch-0\\191.7479.30\\helpers\\pycharm_display',
    'C:\\Users\\***\\Anaconda3\\envs\\***\\python37.zip',
    'C:\\Users\\***\\Anaconda3\\envs\\***\\DLLs', 'C:\\Users\\***\\Anaconda3\\envs\\***\\lib',
    'C:\\Users\\***\\Anaconda3\\envs\\***',
    'C:\\Users\\***\\Anaconda3\\envs\\***\\lib\\site-packages',
    'C:\\Users\\***\\AppData\\Local\\JetBrains\\Toolbox\\apps\\PyCharm-P\\ch-0\\191.7479.30\\helpers\\pycharm_matplotlib_backend']
```

##### Olivier Michel [Cyberbotics] 07/16/2019 15:11:02
I could follow the instructions and launch my python controller from a MSYS2 console without any problem.


I will now try from a DOS console...


Are you sure you have a 64 bit python?

##### MariusJuston [Moderator] 07/16/2019 15:19:04
Python 3.7.3 (default, Apr 24 2019, 15:29:51) [MSC v.1915 64 bit (AMD64)] on win32

##### Olivier Michel [Cyberbotics] 07/16/2019 15:19:09
OK.


For me it also works from a DOS console.


So, I don't understand why it doesn't work from your PyCharm environment...

##### MariusJuston [Moderator] 07/16/2019 15:33:37
is there anything I could provide you in order to help you troubleshoot?

##### Olivier Michel [Cyberbotics] 07/16/2019 15:34:18
I believe the next step would be for me to install PyCharm and try from there.


But I don't time right now (I am flying to China very soon).

##### MariusJuston [Moderator] 07/16/2019 15:35:23
No problems, thank you very much for the time that you have sent trying to help me! I really appreciate it!

##### Olivier Michel [Cyberbotics] 07/16/2019 15:40:59
Last thing to try: could try to use pythonw.exe (instead of python.exe) to execute your robot controller? It seems that makes a difference in some cases.

##### MariusJuston [Moderator] 07/16/2019 15:48:37
sadly no...

##### Olivier Michel [Cyberbotics] 07/16/2019 15:51:35
Also, can you try to launch your python controller from a DOS CMD by typing "python my\_controller.py" in the controller folder?

##### MariusJuston [Moderator] 07/16/2019 15:54:01
No it does not work for either: `C:\Users\***\Anaconda3\envs\***\python.exe state_controller/state_controller.py`. For `C:\Users\***\Anaconda3\envs\***\pythonw.exe state_controller/state_controller.py` it does not print anything even though there are print statements, so i am assuming that it is not working

##### Olivier Michel [Cyberbotics] 07/16/2019 15:54:28
OK...

##### Fabien Rohrer [Moderator] 07/17/2019 12:56:15
`@Akash` I'm currently adding a better example for the Accelerometer node: [https://github.com/omichel/webots/pull/730](https://github.com/omichel/webots/pull/730) Any feedback would be welcome 😃

##### Huey 07/17/2019 17:05:23
Hi.. im following the tutorial to create my first proto,  any idea on where to look for the format to define the 'children' according to the robot i have created earlier? Thank you.


my proto file
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/601097164256575584/unknown.png)
%end


scene tree, these are the children
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/601097268002816027/unknown.png)
%end

##### David Mansolino [Cyberbotics] 07/17/2019 18:18:54
Hi, the simplest solution is to save your world and open the world file in your text editor, this way you will see the format of all the nodes you have in the 'children' field, you can simply copy paste the content of the 'children' field from your world file to your PROTO file.

##### Huey 07/18/2019 08:22:30
Brilliant! Thanks mate!

##### David Mansolino [Cyberbotics] 07/18/2019 08:22:40
You're welcome

##### Deleted User 07/20/2019 10:03:49
NAO robot, shoulder pitch not working. It is walking using  nao\_demo code but hand is still at one place.

Please solve it.


I need to make it walk like we do (moving hand while walking)

##### MerySgh 07/21/2019 04:52:19
Hello is there someone here ?


I work on webots for a project of the autonomous vehicles and I have to do like simulation a car in the road which strikes a pedestrians according to precise attacks .... so far I have just realized a city with a car above .. now I have to add a few passengers but I don't know how to do it


I tried to add a pedestrian from  files .... but the problem is the city is programmed in C while pedestrian is programmed in python ... so I can't compile the two files



%figure
![pedestrianinpython.PNG](https://cdn.discordapp.com/attachments/565154703139405824/602366080685703188/pedestrianinpython.PNG)
%end



%figure
![C.PNG](https://cdn.discordapp.com/attachments/565154703139405824/602366217373876234/C.PNG)
%end

##### Deleted User 07/21/2019 05:21:50
NAO robot, shoulder pitch not working. It is walking using  nao\_demo code but hand is still at one place.

Please solve it.

I need to make it walk like we do (moving hand while walking)

##### MariusJuston [Moderator] 07/21/2019 14:21:55
`@MerySgh` You should still be able to run the simulation... and edit them, the controllers are compiled separately from each other

##### MerySgh 07/21/2019 14:58:03
So how can i do it ?

##### MariusJuston [Moderator] 07/21/2019 14:58:30
Just press the run button like usual

##### Nora 07/21/2019 16:39:17
hello, excuse me i new in the webots world and Im trying to create omnidirecctional wheels with rollers at 45 degrees, can anybody help me?

##### David Mansolino [Cyberbotics] 07/22/2019 06:14:18
Hi `@Nora`, please use the technical-questions' for futur questions.

About omnidirectional wheels the simplest (and most efficient solution) is to use asymetric friction (this can be defined in the Worldinfo.contactproperties field). Please have a look at these examples where the youbot robot uses this mechanism: [https://www.cyberbotics.com/doc/guide/youbot#samples](https://www.cyberbotics.com/doc/guide/youbot#samples)


`@Deleted User` The shoulder is not moving because the motion file does not contain any value for the shoulders, if you want them to move you have to either use another motion file or modify this motion file to add shoulder value. For more information about motion files please refer to: [https://cyberbotics.com/doc/reference/motion-functions](https://cyberbotics.com/doc/reference/motion-functions)


`@MerySgh`, `@MariusJuston` is completely right, since each one (the car and the robot) is using its own seperated controller you should be able to have one of them in C (which you need to compile) and on of them in PYthon (which of course doesn't need to be compiled, but please make sure you have correctly setup Python: [https://cyberbotics.com/doc/guide/using-python](https://cyberbotics.com/doc/guide/using-python)).

However, if you want one (or more) of the pedestrian to be a 'static' passenger of the car, it is strongly recommended to put it's 'controller' field to `""`, or you can use the 'MotorbikeDriver' ([https://github.com/omichel/webots/blob/revision/projects/vehicles/protos/generic/MotorbikeDriver.proto](https://github.com/omichel/webots/blob/revision/projects/vehicles/protos/generic/MotorbikeDriver.proto)), which is already in the correct position (but in that case it should be 'encapsulated' in a Slot node: [https://cyberbotics.com/doc/reference/slot](https://cyberbotics.com/doc/reference/slot))

##### ShravanTata 07/22/2019 12:30:14
Hi Cyberbotics, Is there a way to launch Webots with a particular python command to run the controller instead of setting it globally in the preferences?

##### David Mansolino [Cyberbotics] 07/22/2019 12:47:03
Hi Sharavan, yes you can add a 'runtime.ini' file in the same directory than your controller. This is documented here: [https://cyberbotics.com/doc/guide/controller-programming#languages-settings](https://cyberbotics.com/doc/guide/controller-programming#languages-settings)

##### MariusJuston [Moderator] 07/22/2019 13:22:49
Hello I do not know if this should be in the technical-question channel, but when I try to look at the website I am unable to see the text, this seems to happen for all the documentation/tutorial pages, not the home page or the forums
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/602853057398571078/unknown.png)
%end

##### Fabien Rohrer [Moderator] 07/22/2019 13:24:49
Hi Marius,


Are you behind some firewall? From which country are you?

##### MariusJuston [Moderator] 07/22/2019 13:25:32
US

##### Fabien Rohrer [Moderator] 07/22/2019 13:25:49
Do you have an error in the Javascript console?

##### MariusJuston [Moderator] 07/22/2019 13:26:20

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/602853942224879631/unknown.png)
%end

##### Fabien Rohrer [Moderator] 07/22/2019 13:29:09
these blocked resources are required to see the doc...


not sure why..

##### MariusJuston [Moderator] 07/22/2019 13:29:39
I can see every other website fine...

##### Fabien Rohrer [Moderator] 07/22/2019 13:30:07
In the meantime, you can access to the doc inside Webots in: "Help / Offline documentation / Guide"


Our SSL certificate seems blocked by your browser for some reason.

##### MariusJuston [Moderator] 07/22/2019 13:31:33
Ok thanks

##### Fabien Rohrer [Moderator] 07/22/2019 13:31:49
Could you access this page? [https://www.cyberbotics.com/doc/guide/index?version=revision](https://www.cyberbotics.com/doc/guide/index?version=revision)

##### MariusJuston [Moderator] 07/22/2019 13:32:02
No also blank

##### Fabien Rohrer [Moderator] 07/22/2019 13:33:49
This is the same problem has mentioned here: [https://stackoverflow.com/questions/56552048/loading-jquery-cdn-on-chrome-gives-failed-to-load-resource-neterr-cert-autho](https://stackoverflow.com/questions/56552048/loading-jquery-cdn-on-chrome-gives-failed-to-load-resource-neterr-cert-autho)


It seems that your system blocks the jsdelivr CDN


Maybe you blacklisted it by some operation.

##### MariusJuston [Moderator] 07/22/2019 13:36:01
To be honest I have no idea how to do that, nor what jsdelivr CDN is

##### Fabien Rohrer [Moderator] 07/22/2019 13:36:40
haha, sorry, I write this loudly. Let me check if I can find a procedure to help you..


The Webots website requires some resources which are on another website: jsdelivr and these resources cannot be loaded on your computer.

##### MariusJuston [Moderator] 07/22/2019 13:37:55
Okay

##### Fabien Rohrer [Moderator] 07/22/2019 13:38:44
what is your browser?

##### MariusJuston [Moderator] 07/22/2019 13:39:23
Chrome version 75.0.3770.142; however, it also does not work on explorer and Edge

##### Fabien Rohrer [Moderator] 07/22/2019 13:39:59
Ok so it probably comes from your firewall.

##### MariusJuston [Moderator] 07/22/2019 13:40:55
Would you know how to make it so that I can edit my firewall to allow this connection/js script to go through? I am in Windows 10

##### Fabien Rohrer [Moderator] 07/22/2019 13:42:44
I'm sorry, but I think I can't help you much further: this is very specific to your system. It may comes from the router also 😕

##### MariusJuston [Moderator] 07/22/2019 13:48:16
No problems, thank you very much


It seems I have fixed it without doing anything to my firewall: [https://www.repairwin.com/fix-net-err\_cert\_authority\_invalid-connection-is-not-private-in-chrome/](https://www.repairwin.com/fix-net-err_cert_authority_invalid-connection-is-not-private-in-chrome/) I followed Method 2. Change DNS settings.

##### juashkapsup 07/22/2019 19:19:41
somebody speak spanish in this group?

##### speedy6451 07/22/2019 21:10:40
can you integrate usb controllers in webots? The F310 to be specific.

##### David Mansolino [Cyberbotics] 07/23/2019 06:17:29
`@speedy6451`, yes we have a joystick API which is compatible with most of the usb controllers (but we never tested it with the F310): [https://www.cyberbotics.com/doc/reference/joystick](https://www.cyberbotics.com/doc/reference/joystick)

##### Chen-moon 07/23/2019 07:49:59
Sorry for bothering, but I want to ask some questions. I use webot for robotic arm enviroment setting now, and I also use the htc vive tracker to control robotic arm in webot.


However, I want the htc vive headset can    show the webot enviroment simulation view. Does anyone know how to solve this problem~?Thanks!

##### David Mansolino [Cyberbotics] 07/23/2019 07:54:50
Hi, `@Chen-moon` , Webots does support indeed the HTC Vive out of the box if you are on Windows and you have SteamVR Installed: [https://www.cyberbotics.com/doc/guide/computer-peripherals#virtual-reality-headset](https://www.cyberbotics.com/doc/guide/computer-peripherals#virtual-reality-headset)

You can enable it from the 'View' menu: [https://www.cyberbotics.com/doc/guide/the-user-interface#view-menu](https://www.cyberbotics.com/doc/guide/the-user-interface#view-menu)

##### Chen-moon 07/23/2019 07:59:53
Hi `@David Mansolino` ,thanks for your response, I just see your second websites and set the enable control.However, after I set the enable control, the view in webot  become all blue(which meens the robotic arm I have set just disappear)


Have you ever meet this problem~?Thanks!

##### David Mansolino [Cyberbotics] 07/23/2019 08:04:29
In the View->Virtual Reality Headset have you checked 'View left eye' (or right) ?

##### Chen-moon 07/23/2019 08:07:47
Hi `@David Mansolino` ,these setting (View left eye(right eye))I have tried, but the view is still all  blue ..

##### Huey 07/23/2019 08:28:51
Hi, sorry if this question is too general, i like to program a robot to first find 'target', once found, push it towards 'goal'



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/603141482886594579/unknown.png)
%end


any idea what d be the appropriate method?

##### David Mansolino [Cyberbotics] 07/23/2019 08:29:43
`@Chen-moon`, ok can you see something in the headset? Have you tried with various samples world distributed within Webots ?

##### Olivier Michel [Cyberbotics] 07/23/2019 08:30:39
`@Huey`, you should probably put a camera onboard your robot and perform image processing to detect the target, navigate towards it and push it to the goal.

##### Huey 07/23/2019 08:34:00
Thanks Oliver, im  quite a newb (as in noob) on this, any idea where i could learn more about programming a camera to detect something?


possible to use get pixels or certain color? like this?



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/603142879623512074/unknown.png)
%end

##### Chen-moon 07/23/2019 08:50:48
Hi `@David Mansolino` , here I just see the same blue screen in htc vive headset

##### David Mansolino [Cyberbotics] 07/23/2019 08:51:49
`@Huey`, yes, that's the idea, please find here an example (in Python) of Webots online simulation using OpenCV to perform object recognition: [https://robotbenchmark.net/benchmark/visual\_tracking/](https://robotbenchmark.net/benchmark/visual_tracking/)

##### Chen-moon 07/23/2019 08:53:44
And I also try the example(universal robot->ure.wbt) in webot. However, when I set the enable setting, the view in webot and headset all become blue..



%figure
![DSC_0101.JPG](https://cdn.discordapp.com/attachments/565154703139405824/603148275289423878/DSC_0101.JPG)
%end

##### Huey 07/23/2019 08:55:56
Thanks loads `@David Mansolino` ! will look into it

##### David Mansolino [Cyberbotics] 07/23/2019 08:56:03
`@Chen-moon` , ok in that case there is probably a problem with the interface with Webots and The Vive, do you have the latest version of SteamVR installed ?


`@Huey`, you're welcome

##### Chen-moon 07/23/2019 08:56:40
The steam vr version I used is 1.5.16


I just install this version a few days ago


`@David Mansolino` , Should I use the beta version? Thanks!

##### David Mansolino [Cyberbotics] 07/23/2019 08:59:12
Have you tried with other application to make sure your steamVr setup is working fine?

##### Chen-moon 07/23/2019 09:10:33
Hi `@David Mansolino` , I just test steamVR in steamVR tutorial, and it work in whole tutorial process!

##### David Mansolino [Cyberbotics] 07/23/2019 09:11:27
Ok, then there is maybe a problem with the interface, I didn't tried it since a few months, can you please open a bug report on our github and we will let you know when we have tested: [https://github.com/omichel/webots/issues/new/choose](https://github.com/omichel/webots/issues/new/choose)

##### Chen-moon 07/23/2019 09:21:47
Hi `@David Mansolino` ,Thanks for your suggestions!

##### David Mansolino [Cyberbotics] 07/23/2019 09:21:56
You're welcome

##### MariusJuston [Moderator] 07/23/2019 19:10:21
Hello, is there a way, from the  supervisor, to enable and disable the physics node?

##### Chen-moon 07/24/2019 05:58:05
Hi `@David Mansolino` , I just send a bug report to the github(The title is HTC Vive is not working in Webots). Thank you!

##### David Mansolino [Cyberbotics] 07/24/2019 06:16:25
`@Chen-moon` , thank you for the bug report, I did indeed saw it. We don't have any headset available right now, but we will try as soon as we have it back and let you know.

In the meantime, I saw that you have a HTC Vive Pro (I did try only with the HTC Vive for now), can you try reducing the resolution to ~50% (this is feasible in the preferences of SteamVR, then you have to restart Webots once changed)?


`@MariusJuston`, unfortunately I don't see any simple solution, we are indeed missing a `wb_supervisor_field_remove_sf` and `wb_supervisor_field_import_sf_node` function. Maybe someone else has an idea for a workaroud ?

##### Deleted User 07/24/2019 06:20:57
How to connect webot with choreograph?


For NAO robot

##### David Mansolino [Cyberbotics] 07/24/2019 06:26:50
`@Deleted User` , please refeer to [https://cyberbotics.com/doc/guide/nao#naoqi-and-choregraphe](https://cyberbotics.com/doc/guide/nao#naoqi-and-choregraphe)

##### Chen-moon 07/24/2019 06:27:51
Hi `@David Mansolino` , thanks for you suggestion.

##### David Mansolino [Cyberbotics] 07/24/2019 06:28:13
`@MariusJuston` , I just thought about one possible workaround (but this requires to have a bit of knowledge of rhe PROTO mechanism: [https://www.cyberbotics.com/doc/reference/proto](https://www.cyberbotics.com/doc/reference/proto)). The idea would be tu use a procedural boolean field to determine of the physics node should be present or not (and switch this field from the supervisor API)

##### Chen-moon 07/24/2019 06:29:01
But I want to ask where can I change the resolutions to ~50%?

##### David Mansolino [Cyberbotics] 07/24/2019 06:32:40
`@Chen-moon`, you should have somethign similar to this in the settings of SteamVR: [https://forum.pimaxvr.com/uploads/default/original/2X/c/c8a523e0449a8005684daea6a47d9b6f5cd9f3bf.png](https://forum.pimaxvr.com/uploads/default/original/2X/c/c8a523e0449a8005684daea6a47d9b6f5cd9f3bf.png)

##### Chen-moon 07/24/2019 06:35:41
Hi `@David Mansolino` , thanks for your suggestion!

##### David Mansolino [Cyberbotics] 07/24/2019 06:36:37
You're welcome

##### Deleted User 07/24/2019 07:11:04
`@David Mansolino`  I went through, i started the simulation in webot and then trying to connect it with chreograph


The simulated nao is displayed in choreograph


it is connected also, but thereafter it is not doing anything while giving command

##### Amrit 07/24/2019 08:23:44
Hi....! I am trying to connect webots with python and it gets connected with a specified port. But when I am trying to send some data from python to webots for simulation, but then its not working. Can anyone please help me out....?

##### Olivier Michel [Cyberbotics] 07/24/2019 08:26:45
`@Amrit`, please read our docs about programming robot controller in Python. You cannot connect through a port.


[https://cyberbotics.com/doc/guide/cpp-java-python#python-example](https://cyberbotics.com/doc/guide/cpp-java-python#python-example)

##### Amrit 07/24/2019 08:33:24
`@Olivier Michel`  Thanks for the information. But I would like to tell you that I have configured server on webots side.

##### Olivier Michel [Cyberbotics] 07/24/2019 08:42:41
Are you using Webots as a cloud-based web service?


Like in [https://robotbenchmark.net](https://robotbenchmark.net) ?

##### Amrit 07/24/2019 11:10:56
`@Olivier Michel`....! No

##### Deleted User 07/24/2019 12:18:27
Can i operate NAO in webot through choreograph?

If yes, then what are the basic requirment and how to handle?

##### Olivier Michel [Cyberbotics] 07/24/2019 12:21:20
`@Amrit`: so I don't understand what you are trying to do...


`@Deleted User`: the choregraph interface is not supported any more (too many bugs, not maintained any more by Softbank), so I would recommend to walk away from it.

##### Deleted User 07/24/2019 12:23:52
Thank you for saving my time sir.


I have a doubt that why NAO robot does not move its hand while walking like human do?


in webot simulation and controller (nao\_demo.c)

##### Olivier Michel [Cyberbotics] 07/24/2019 12:25:20
I don't know.

##### David Mansolino [Cyberbotics] 07/24/2019 12:25:30
`@Deleted User` The shoulder is not moving because the motion file does not contain any value for the shoulders, if you want them to move you have to either use another motion file or modify this motion file to add shoulder value. For more information about motion files please refer to: [https://cyberbotics.com/doc/reference/motion-functions](https://cyberbotics.com/doc/reference/motion-functions)

##### Fabien Rohrer [Moderator] 07/24/2019 12:32:32
`@Deleted User` I allow myself to enter in the discussion. We saw your same question here several times and on several other threads too (youTube comments, private messages, etc.) . We received them all, no need to spam us. Last week I spent time to provide you a complete and working example doing what you expect. You should focus only on letting this example work:



> **Attachment**: [nao\_demo-2.c](https://cdn.discordapp.com/attachments/565154703139405824/603565238083911680/nao_demo-2.c)

##### Deleted User 07/24/2019 12:56:12
Sorry to disturb you, i am messaging again  because of this reason (can be seen in video),

Now hand is moving but the robot is not moving.

Ok sir thank you for your help till now
> **Attachment**: [nao\_demo.mp4](https://cdn.discordapp.com/attachments/565154703139405824/603571133928701962/nao_demo.mp4)

##### Fabien Rohrer [Moderator] 07/24/2019 12:56:43
Is this the controller I sent you?

##### Deleted User 07/24/2019 12:56:53
yes the same one

##### Fabien Rohrer [Moderator] 07/24/2019 12:57:09
Do you have a message in the console?

##### Deleted User 07/24/2019 12:57:12
i clicked the forward button and this is happenig


[my\_controller] Error: wbu\_motion\_new(): could not open '../../motions/Forwards50.motion' file.

INFO: Video recording starts when you run a simulation...

[my\_controller] Error: wbu\_motion\_play() called with NULL 'motion' argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.

[my\_controller] Error: wbu\_motion\_is\_over() called with NULL argument.


this is what happening

##### Fabien Rohrer [Moderator] 07/24/2019 12:58:07
I already tried to help you with this error.

##### Deleted User 07/24/2019 12:58:13
ok ok

##### Fabien Rohrer [Moderator] 07/24/2019 12:58:35
This is simply because you didn't copied the'../../motions/Forwards50.motion' at the correct location.

##### Deleted User 07/24/2019 12:59:15
i just copy and paste the controller you have sent

##### Fabien Rohrer [Moderator] 07/24/2019 12:59:34
You also need to copy-paste Forwards50.motion'


From WEBOTS\_HOME/projects/robots/softbank/nao/motions/Forwards50.motion


To YOUR\_PROJECT/motions/Forwards50.motion


(The "motions" directory should appear at the same directory level as your "worlds" and "controllers" directory)

##### Deleted User 07/24/2019 13:01:08
Ok sir 

i am trying


Thank you so much sir 

Thank you for your help


Now it is working

##### Fabien Rohrer [Moderator] 07/24/2019 13:04:14
Great 👍🏻

##### Deleted User 07/24/2019 13:15:39
Thank you sir.😊 

And sorry if i make you angry. 😑
> **Attachment**: [nao\_demo.mp4](https://cdn.discordapp.com/attachments/565154703139405824/603576029184917504/nao_demo.mp4)

##### Fabien Rohrer [Moderator] 07/24/2019 13:17:45
I'm happy you achieved your goal. Next time, please focus on a single thread, certainly here, it will help us and you 😉

##### Deleted User 07/24/2019 13:18:08
Yes sir

##### KumarSubh 07/24/2019 14:13:35
Hello everyone! I m new to WEBOTS...Can anyone help me to gather the info about drone simulation in webots?

##### Fabien Rohrer [Moderator] 07/24/2019 14:14:27
Hi Kumar

##### KumarSubh 07/24/2019 14:14:49
Hello Fabien


I want to simulate drone on WEBOTS...How should I proceed?

##### Fabien Rohrer [Moderator] 07/24/2019 14:16:09
The most important node to master is the Propeller node:


[https://cyberbotics.com/doc/reference/propeller](https://cyberbotics.com/doc/reference/propeller)


It allows to simulate a thrust.


(a controllable and customizable force and torque applied on some solid)


You may find an example of the Propeller use here: [https://cyberbotics.com/doc/guide/samples-devices#propeller-wbt](https://cyberbotics.com/doc/guide/samples-devices#propeller-wbt)


It shows 3 different designs of helicopter. This is definitively a good starting point.


Funny enough, I'm currently working on a better drone demo.


It's already in our revision branch, but not released on a stable release.


It's a model of the DJI Mavic 2 PRO:

##### KumarSubh 07/24/2019 14:19:54
GREAT!!!

##### Fabien Rohrer [Moderator] 07/24/2019 14:20:09
Here is some doc: [https://cyberbotics.com/doc/guide/mavic-2-pro?version=revision](https://cyberbotics.com/doc/guide/mavic-2-pro?version=revision)


I'm about to release a demo movie on youTube about this.


To go further than Propellers, fluid can be simulated too, in a basic way.


In my Mavic simulation, I only set some global damping.


But Webots supports basic fluids like in this demo:


[https://cyberbotics.com/doc/guide/samples-geometries#floating\_geometries-wbt](https://cyberbotics.com/doc/guide/samples-geometries#floating_geometries-wbt)

##### KumarSubh 07/24/2019 14:22:41
Oooo Nice! I would look forward to make a share of your knowledge in my project... 😀

##### Fabien Rohrer [Moderator] 07/24/2019 14:23:17
Do you have particular needs? Some drone models in mind?

##### KumarSubh 07/24/2019 14:24:40
Actually not right now,


However, in long run YES


Thanks for your initials 😊

##### Fabien Rohrer [Moderator] 07/24/2019 14:28:01
You're welcome.

##### Huey 07/25/2019 08:36:12
Hi, I have some coding problem here. After added a camera under the Robot children, name  it as 'camera'.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/603868092912762899/unknown.png)
%end


I added few lines in the controller script to initialize it.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/603868203113775115/unknown.png)
%end


I must have done something wrong there, these are the error messages I get


javac -Xlint -classpath "E:\E Program\Webots\lib\java\Controller.jar;." TwoWheelsCollisionAvoidance.java

TwoWheelsCollisionAvoidance.java:30: error: no suitable constructor found for Camera(no arguments)

   Camera cm=new Camera();

             ^

    constructor Camera.Camera(long,boolean) is not applicable

      (actual and formal argument lists differ in length)

    constructor Camera.Camera(String) is not applicable

      (actual and formal argument lists differ in length)

TwoWheelsCollisionAvoidance.java:31: error: illegal initializer for String

   String cmName={"camera"}; 

                 ^

TwoWheelsCollisionAvoidance.java:33: error: cannot find symbol

   cm=robot.getCamera(camera);

                      ^

  symbol:   variable camera

  location: class TwoWheelsCollisionAvoidance

3 errors

Nothing to be done for build targets.

##### Stefania Pedrazzi [Cyberbotics] 07/25/2019 08:39:11
`@Huey` : yes you are initializing the devices in the wrong way. You have to use the Robot API to get device instance: [https://cyberbotics.com/doc/reference/robot?tab=java#getaccelerometer](https://cyberbotics.com/doc/reference/robot?tab=java#getaccelerometer)


You can find a java example at projects/languages/java/worlds/example.wbt


another very simple one is also included in the User Guide: [https://cyberbotics.com/doc/guide/cpp-java-python#java-example](https://cyberbotics.com/doc/guide/cpp-java-python#java-example)

##### Huey 07/25/2019 08:54:55
Thanks Stefania, I followed the simple one and tweak the script this way
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/603872801719320584/unknown.png)
%end


and it worked!

##### Stefania Pedrazzi [Cyberbotics] 07/25/2019 08:55:48
Cool!


do not forget to do the same for the motors as well 😉

##### MariusJuston [Moderator] 07/25/2019 15:15:33
Sorry about this but I am trying to have this box use conveyor belts to move from one place to another and there needs to be a gap in between the conveyors as such. I am using the Webots conveyor belt robots; however, the  conveyors are not moving the block. The height of the borders is 0
%figure
![Conveyor_belt_inbetween.jpg](https://cdn.discordapp.com/attachments/565154703139405824/603968588478414859/Conveyor_belt_inbetween.jpg)
%end

##### Fabien Rohrer [Moderator] 07/25/2019 15:16:11
Hi


Is your block a Solid with bounding object and physics?

##### MariusJuston [Moderator] 07/25/2019 15:16:29
yes


the block does move when it is completely on one of the conveyors

##### Fabien Rohrer [Moderator] 07/25/2019 15:18:25
Could you monitor the contact points (View / Optional Rendering / Show Contact Points) and tell if there are always 2 blue rectangles, one on each conveyor belt?

##### MariusJuston [Moderator] 07/25/2019 15:19:44
At the beginning there are the two rectangles but then they disappear


They probably disappeared becasue the contact points did not change

##### Fabien Rohrer [Moderator] 07/25/2019 15:23:40
mmm, normally they should remain even if they do not change


let me check something..

##### MariusJuston [Moderator] 07/25/2019 15:24:09
okay


Here is a video of it happening if it helps
> **Attachment**: [Test\_Conveor\_Belt\_System.mp4](https://cdn.discordapp.com/attachments/565154703139405824/603971154935283745/Test_Conveor_Belt_System.mp4)

##### Fabien Rohrer [Moderator] 07/25/2019 15:25:49
Could you set the 2 Conveyor.borderHeight to -0.05 ?

##### MariusJuston [Moderator] 07/25/2019 15:26:19
It works now

##### Fabien Rohrer [Moderator] 07/25/2019 15:26:24
oh really?

##### MariusJuston [Moderator] 07/25/2019 15:27:10
That is strange becasue when I put -1 to test if it was the borders it was telling me: `WARNING: DEF ConveyorBelt Robot > ConveyorBelt (PROTO) > Group > Transform > Box: Invalid 'size' changed to 1 1 1. The value should be positive.`

##### Fabien Rohrer [Moderator] 07/25/2019 15:27:23
Here is also a working example if you need it..
> **Attachment**: [tinkerbots\_demo.wbt](https://cdn.discordapp.com/attachments/565154703139405824/603971569907269652/tinkerbots_demo.wbt)


Ok, the borderHeight should be smaller than the size.y value. A better warning is missing there.

##### MariusJuston [Moderator] 07/25/2019 15:28:34
Many thanks!

##### Fabien Rohrer [Moderator] 07/25/2019 15:30:00
you're welcome 👍🏻

##### MariusJuston [Moderator] 07/25/2019 15:52:56
Is there, when taking the video of the current animation to made the custom robot window included into the video as well?

##### David Mansolino [Cyberbotics] 07/25/2019 15:54:07
Are you speaking of the robot window ?

##### MariusJuston [Moderator] 07/25/2019 15:54:22
Yes, sorry I meant the robot window

##### David Mansolino [Cyberbotics] 07/25/2019 15:55:41
I am sorry but this is not possible in video. But you can either:

  - Use a display which overlay is visible in the video.

  - Use an external tool to record a movie from your screen

##### MariusJuston [Moderator] 07/25/2019 15:57:40
That is shame, but still thank you

##### David Mansolino [Cyberbotics] 07/25/2019 15:58:50
That would be indeed a nice feature, unfortunately, since  the robot window is in the controller process this would be difficult to implement. You're welcome

##### MariusJuston [Moderator] 07/25/2019 16:03:21
Would it be possible to find the current robot window instances and then when recording be able to select one of the open tab, then during the recording it saves the robot window view as an image and appends it to the right or left of the simulation view frame?

##### Fabien Rohrer [Moderator] 07/25/2019 16:30:15
The generic robot window is defined here: [https://github.com/omichel/webots/tree/revision/resources/projects/plugins/robot\_windows/generic](https://github.com/omichel/webots/tree/revision/resources/projects/plugins/robot_windows/generic)


I never tried this, but it’s certainly possible to copy the HTML content into an image.


[https://stackoverflow.com/questions/18581379/how-to-save-the-contents-of-a-div-as-a-image](https://stackoverflow.com/questions/18581379/how-to-save-the-contents-of-a-div-as-a-image)

##### Berethore 07/26/2019 07:31:25
Hi guys, i'm really new with Webots, and trying to fix a connector to my robotic arms


But i don't find how to just fix an object to another with a fixed joint


I know that's probably obvious, but well..

##### David Mansolino [Cyberbotics] 07/26/2019 07:32:06
Hi Berethore


Did you follow our the tutorial?

[https://www.cyberbotics.com/doc/guide/tutorials](https://www.cyberbotics.com/doc/guide/tutorials)


If not I strongly recommend to do tutorial 1-7, you will learn how to design new object (including some with joints)

##### Berethore 07/26/2019 07:35:14
Yeah thanks for your answer, i didn't look so much on it because i was in hurry


But i think i will have to


Tanks for your answer anyway

##### David Mansolino [Cyberbotics] 07/26/2019 07:35:51
You're welcome

##### Huey 07/26/2019 13:22:57
Hi, I try to change the RectangleArena into a plain color. So, I made a copy of the RectangleArena.proto and 'save as' a different file under my project protos folder. 



Subsequently, I created new proto file, MatteGreyConcrete.proto  in Webots\projects\appearances\protos

then change: field SFNode     floorAppearance to MatteGreyConcrete {} inside the new RectangleArena.proto file



 That s when i get the error message: /protos/RectangleArenaPlain.proto':14:36: error: Skipped unknown 'MatteGreyConcrete' node or PROTO.


May I asked if that's not allowed? Or any other way i could change the floor texture map into a plain color?

##### Fabien Rohrer [Moderator] 07/26/2019 13:24:39
This is possible, you should be close from a solution. Did you put your new file in the "protos" directory?


YOUR\_PROJECT/protos/MatteGreyConcrete.proto?

##### Huey 07/26/2019 13:26:54
Oh i didn't, instead I put the MatteGreyConcrete.proto in the Webots\projects\appearances\protos

##### Fabien Rohrer [Moderator] 07/26/2019 13:27:35
It should work too.


Did you rename the proto name inside the MatteGreyConcrete.proto ?


By the way, your solution seems quite complexe. Instead of creating a PROTO for this, you could simply use the PBRAppearance node:


RectangleArena.floorAppearance PBRAppearance { baseColor 1 0 0}

##### Huey 07/26/2019 13:33:18
LOL... indeed... doi me


I was fidgeting with the floorAppearance and somehow got sucked into the blackhole


your solution is much more straightforward, i wll use that


however, i did the changing filename too, but still getting this error msg: v14:36: error: Skipped unknown 'MatteGreyConcrete' node or PROTO.

##### Fabien Rohrer [Moderator] 07/26/2019 13:36:22
the PROTO name should match with the filename.


Inside the .proto file, you need to change the name too:


PROTO nameEqualsToFileName [ field ... ]

##### Huey 07/26/2019 13:45:43
`@Fabien Rohrer`  thanks mate!

##### Deleted User 07/27/2019 13:07:38
`@Fabien Rohrer` 

sir, i have modified the code that you have given and now i am getting the desired action by robot. 

But i am getting one problem that when i am trying to add turn left or turn right motion in the same file.

it is taking only one command

either it is goiing forward or turning left 

why so sir??


i added the below code, it is not showing any error, the file compiled successfully



but when i  am putting the turn left movement above the forward movement the robot is only taking the turning left command.



and when i am putting the forward command above the turn left command it is only taking the forward command

why sir?



> **Attachment**: [Nao\_turn\_left.txt](https://cdn.discordapp.com/attachments/565154703139405824/604663082391830565/Nao_turn_left.txt)

##### Huey 07/28/2019 11:30:12
Hi, I have a question about boundingObject. To prevent the robot from swaying during motion. 



(video)



I have added a cone shape point (touching the ground) (a polulu ball caster like device would have been ideal, but might be complex to build?)



(images)



Is it possible to set boundingObject to take more than one Shape?
> **Attachment**: [output.mp4](https://cdn.discordapp.com/attachments/565154703139405824/604999043164405762/output.mp4)



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/604999071568232451/unknown.png)
%end



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/604999377819271178/unknown.png)
%end

##### MariusJuston [Moderator] 07/28/2019 17:54:05
You can instead of making the bounding object just a simple shape, you can make it into a group and then add as many shapes as you want

##### siti\_raehan 07/29/2019 00:15:41
Anybody please help me
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/605191683017015362/unknown.png)
%end

##### MariusJuston [Moderator] 07/29/2019 00:17:14
Could you give more description about what the code is supposed to do, what robot there, etc... please just giving the error is not very helpful

##### David Mansolino [Cyberbotics] 07/29/2019 06:41:04
`@Huey`, `@MariusJuston` is completely right, you should use a group, please have a look at this tutorial showing something very similar: [https://cyberbotics.com/doc/guide/tutorial-5-compound-solid-and-physics-attributes-15-minutes](https://cyberbotics.com/doc/guide/tutorial-5-compound-solid-and-physics-attributes-15-minutes)


Note also that to prevent the robot from swaying during motion we ususally use a sphere(which can be partially 'inside' the robot) this is more efficient to simulate, and we set the ContactProperties.coulombFriction to a very low value to make the contact between the sphere and the ground frictionless ([https://www.cyberbotics.com/doc/reference/contactproperties](https://www.cyberbotics.com/doc/reference/contactproperties))

##### Huey 07/29/2019 06:49:46
Thanks `@MariusJuston` , like you said, I added 'group' instead of 'shape' and playing with it a little bit, and slowly understand what you mean! 



and Thanks `@David Mansolino`  for pin pointing out the tutorial and documentation! They are really helpful! 🙇  I shall take a look at them

##### Deleted User 07/29/2019 09:41:21
`@Fabien Rohrer` 

sir, i have modified the code that you have given and now i am getting the desired action by robot. 

But i am getting one problem that when i am trying to add turn left or turn right motion in the same file.

it is taking only one command

either it is goiing forward or turning left 

why so sir??


i added the below code, it is not showing any error, the file compiled successfully



but when i  am putting the turn left movement above the forward movement the robot is only taking the turning left command.



and when i am putting the forward command above the turn left command it is only taking the forward command

why sir?

A
> **Attachment**: [Nao\_turn\_left.txt](https://cdn.discordapp.com/attachments/565154703139405824/605334306839330868/Nao_turn_left.txt)

##### Stefania Pedrazzi [Cyberbotics] 07/29/2019 09:54:21
`@Deleted User` you have to wait until the first motion is over before starting the next one, otherwise the second motion commands will overwrite the first one.

Please look at the `wbu_motion_is_over` documentation: [https://www.cyberbotics.com/doc/reference/motion#wbu\_motion\_is\_over](https://www.cyberbotics.com/doc/reference/motion#wbu_motion_is_over)

here is also a sample controller showing how to use it: [https://github.com/omichel/webots/blob/revision/projects/robots/softbank/nao/controllers/nao\_wave\_hand/nao\_wave\_hand.c](https://github.com/omichel/webots/blob/revision/projects/robots/softbank/nao/controllers/nao_wave_hand/nao_wave_hand.c)

##### Deleted User 07/29/2019 10:13:51
while (true) {

    // Wait until the Up key is pushed

    printf("Select the robot and press the up button go forward\n");

    while (wb\_robot\_step(time\_step) != -1) {

      if (wb\_keyboard\_get\_key() == WB\_KEYBOARD\_UP)

        break; // stop waiting

    }



    // Run the simulator while the motion is not over.

    wbu\_motion\_play(forwards);  // Start the walk motion.



    do {

      double time = wb\_robot\_get\_time(); // get the simulated time in seconds.

      wb\_motor\_set\_position(RShoulderPitch, 1.2 + 0.3 * sin(-5.0 * time)); // move the elbow according to a sinusoidal function.

      wb\_motor\_set\_position(LShoulderPitch, (1.2 + 0.3 * sin(5.0 * time)));

      wb\_robot\_step(time\_step);

    } while (! wbu\_motion\_is\_over(forwards));

  }

  

  return 0;

}


here i have given wbu motion is over command for forward motion



where should i add the turn left command

##### Stefania Pedrazzi [Cyberbotics] 07/29/2019 10:17:57
to turn left after going forward you have to add it just after the `do..while`. 

But I strongly suggest you to try to understand your code and play with it by moving the instructions at different position in the code and see what happens.

##### Deleted User 07/29/2019 10:19:08
so you are saying agin i have to give a while loop after   } while (! wbu\_motion\_is\_over(forwards));

##### Stefania Pedrazzi [Cyberbotics] 07/29/2019 10:21:00
yes. from `wbu_motion_play` to `while (! wbu_motion_is_over(forwards))` you are running the forward motion. To run the turn left you have to execute the same instructions.

##### Deleted User 07/29/2019 10:41:56
it is working sir



but now one problem arises

when I am pressing the forward motion after that only the turn left command is working and it is working for the single time only

and if i need to turn it more i have to first go forward then only turning command is working


`@Stefania Pedrazzi`


`@Fabien Rohrer` sir please guide me.

how can i add more motions in the code you have sent me



actually when i am doing it is taking one command only.



now as stefania sir guided 



it is working but now it is working as i send the above message

##### Stefania Pedrazzi [Cyberbotics] 07/29/2019 11:36:03
`@Deleted User` note that what you have now it is a generic programming issue not depending on Webots. You have all the resources needed to execute the Webots commands and move the robot. You just have to structure your code correctly. To write Webots controller programs it is requried to have some programming knowledge. If you struggle with it you may also consider following some programming tutorials.

##### Huey 07/29/2019 20:42:41
Hi, i created a two wheels robot base on this tutorial. 



[https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot?tab=java](https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot?tab=java)



As i was hoping to test out different function which the robot can test run on. So i modified the structure a little bit, hoping it would work.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/605500470013853772/unknown.png)
%end


The code 'build' successfully, however, i got this error message
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/605500635491598379/unknown.png)
%end


So i wonder what is it that im getting wrong here?

##### MariusJuston [Moderator] 07/29/2019 21:07:13
Hmm... can you show me the robot in the tree view please


The code looks good

##### Huey 07/29/2019 21:07:56
The robot node in scene tree
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/605506822131941376/unknown.png)
%end

##### MariusJuston [Moderator] 07/29/2019 21:08:34
Can you open up the robot node please , and please verify that it is pointing to the correct controller

##### Huey 07/29/2019 21:09:31
oh of course, my apology
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/605507220154613791/unknown.png)
%end

##### MariusJuston [Moderator] 07/29/2019 21:09:53
Did you refresh the jar?

##### Huey 07/29/2019 21:11:07
not sure if i understand refresh correctly, do you mean save the controller file and build(F7)?

##### MariusJuston [Moderator] 07/29/2019 21:11:36
Yes


Oh wait I know the issue


It is a code issue. Please remove the `extends Robot`


Save and build the controller and it should work


Or instead of removing `extends Robot` you can remove the `Robot robot = new Robot();` and then remove all instances off `robot.SOMETHING` and replace it with `this.SOMETHING` or just `SOMETHING`, either of the options would work

##### Huey 07/29/2019 21:25:14
I tried both methods, somehow both came up with this error messages
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/605511175521959976/unknown.png)
%end

##### MariusJuston [Moderator] 07/29/2019 21:26:56
in your while statement it has to be `robot.step(TIME_STEP) != -1`

##### Huey 07/29/2019 21:27:51
Brilliant!!!!! Thank you so much :))))))

##### MariusJuston [Moderator] 07/29/2019 21:28:08
No problems!!

##### Huey 07/30/2019 06:35:45
Morning sunshine! 



I added a touch sensor to the robot, change the type to 'force'. During the simulation, seems like it is at constant 0 (or NaN) , is there something im missing here?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/605649718546268160/unknown.png)
%end

##### David Mansolino [Cyberbotics] 07/30/2019 06:38:58
Hi `@Huey`, did you enable the sensor? Can you share the whole code of the controller (we can't see the part where you are enabling the sensors and getting the value in your screenshot)?

##### Huey 07/30/2019 06:44:37
Sure. Here is the code
> **Attachment**: [TwoWheelsAgent.java](https://cdn.discordapp.com/attachments/565154703139405824/605651952424058912/TwoWheelsAgent.java)

##### David Mansolino [Cyberbotics] 07/30/2019 06:50:05
Thank you for the file. The problem is that you are getting the value of the touch-sensor at the first step only, you should move the line `tForce=ts.getValue();` in the while loop, just before the print:


```java
  public void run2()
  {
    while(this.step(TIME_STEP)!=1)
    {    
      tForce=ts.getValue();
      System.out.println(" "+tForce);      
    
      double leftSpeed=1.0;
      double rightSpeed=1.0;
      
      if (tForce>0)
      {
        leftSpeed = -2.0;
        rightSpeed= -2.0;
      }
      wheels[0].setVelocity(leftSpeed);
      wheels[1].setVelocity(rightSpeed);    
    }
  }
```

##### Berethore 07/30/2019 06:53:51
Hi every body. I have some issues with connector... I'm trying to use your Connector interface. I'm taking the exemple you provide in Webots. But i'm trying to connect a robotic arm to a random object (a box here), nothing happenned. I put the code in my controller, i have the autoLock at True, et the model is the same for both connector... I have no idea what's wrong.. Can i have your help pls ?

##### Fabien Rohrer [Moderator] 07/30/2019 06:54:28
Hi, yes sure.


Basically, you need to add a Connector in the arm end effector, and in your object.


You need to align them correctly.


And set their fields correctly.


The alignment may be a source of issue, it's certainly the first thing to check.


Please look at this figure: [https://cyberbotics.com/doc/reference/connector#connector-axis-system](https://cyberbotics.com/doc/reference/connector#connector-axis-system)

##### Berethore 07/30/2019 06:56:57
I'm trying to align them perfectly while the simulation isn't running to see if it's working, but when i run again the simulation after that, still nothing


Ooh.. Maybe is it a problem about axis

##### Fabien Rohrer [Moderator] 07/30/2019 06:57:55
the positive z-axis of the connectors should face each other


and the y axis should match


Could you check/fix this first?


We could look at the parameters together when we are sure this is correct..

##### Berethore 07/30/2019 07:01:06
Ok my axis match but still nothing.

##### Fabien Rohrer [Moderator] 07/30/2019 07:01:27
Ok, that's a good step.


What do you want to achieve exactly? To automatically snap the objects at step 0?


i.e. at the beginning of the simulation?

##### Huey 07/30/2019 07:02:30
oh.. that s why. Thanks so much `@David Mansolino`, it looks like it s working, however, it seems like im only getting negligible readings in the beginning, so i thought maybe the box too 'light', I increase the size (mass), and im still getting 0.0, is it bcz the robot itself is too small?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/605656450844327950/unknown.png)
%end

##### Berethore 07/30/2019 07:03:21
For a preliminary test, yes. But the final goal is to grasp object with specific interface


But if i can just make it working at step 0, it could be a good point

##### Fabien Rohrer [Moderator] 07/30/2019 07:04:01
yes, indeed, it's a good starting point. let me check something..


`@Berethore` I'm trying to do an example of this.

##### Berethore 07/30/2019 07:09:19
Ok, that's very nice, thank you.

##### David Mansolino [Cyberbotics] 07/30/2019 07:09:51
`@Huey`, that's normal the force sensor meseaure external forces applied to the object, the mass/size of the object itself doesn't matter, try pushing your robot against a wall/obstacle in that case you will see the value increasing.

##### Fabien Rohrer [Moderator] 07/30/2019 07:33:04
`@Berethore` Here I am 😃

##### Berethore 07/30/2019 07:33:44
So do I !

##### Fabien Rohrer [Moderator] 07/30/2019 07:34:06
I tried to do that on an ABB arm:



%figure
![connector.png](https://cdn.discordapp.com/attachments/565154703139405824/605664462027882496/connector.png)
%end


Here is the world:



> **Attachment**: [inverse\_kinematics.wbt](https://cdn.discordapp.com/attachments/565154703139405824/605664556521488394/inverse_kinematics.wbt)


I set the connectors like this:


Arm connector:



Connector {

  locked TRUE

  type "active"

  isLocked TRUE

  autoLock TRUE

}





Solid connector:



Connector {

  type "passive"

}


You should pay attention to:


- it is required to have a Solid parent with physics and bounding object in the each parents of the connectors


- the distanceTolerance may be increased to be sure you are in the correct threshold

##### Huey 07/30/2019 07:37:22
Good to know. Thanks `@David Mansolino` for helping to solve this problem 😄

##### Fabien Rohrer [Moderator] 07/30/2019 07:37:45
- you can display the connector axis in View / Optional Rendering / Show Connector Axes


You have certainly all what you need to solve your issue now (?)

##### Huey 07/30/2019 07:38:33
Another question, may i know why when the robot pushes against something very heavy, the reading falls flat (0.0) again?
> **Attachment**: [output1.mp4](https://cdn.discordapp.com/attachments/565154703139405824/605665524436697099/output1.mp4)

##### Berethore 07/30/2019 07:38:57
Ok thanks a lot ! I think i can solve my issue with all your information now yes ! Thanks again 😃

##### Fabien Rohrer [Moderator] 07/30/2019 07:39:18
Don't hesitate to come back later if it's not the case 😉

##### Berethore 07/30/2019 07:41:02
Ok thanks 😃

##### David Mansolino [Cyberbotics] 07/30/2019 07:45:14
`@Huey`, you can define the response of the sensor in its lookup table ([https://cyberbotics.com/doc/reference/touchsensor](https://cyberbotics.com/doc/reference/touchsensor)) in any case make sure that the robot is still pushing agains the obstacle (you can for example verify that there are still some contact points using the view menu: [https://cyberbotics.com/doc/guide/the-user-interface#view-menu](https://cyberbotics.com/doc/guide/the-user-interface#view-menu) )

##### Berethore 07/30/2019 09:27:44
Hi again `@Fabien Rohrer` , don't want to bother you, but when i'm looking at your example, i have some questions :



 - How can I control the connector ? I made a new controller in your world, got the device, used the enable\_presence fonction and tried to unlock with the fonction in the loop. But nothing happenned.



- Same thing when i just put the isLocked boolean false, the box still is connected to the robot. Is it normal ?



- I would like to know if there are many things to change to get symetric connector, or if it's juste the type to change ?

##### Fabien Rohrer [Moderator] 07/30/2019 09:32:02
Connectors which can be lock/unlock programatically needs slightly different fields.


you could refer to this example:


[https://cyberbotics.com/doc/guide/samples-devices#connector-wbt](https://cyberbotics.com/doc/guide/samples-devices#connector-wbt)


The fields to set are:


Connector {

  model "magnetic"

  autoLock TRUE

}


(other fields are implicitly set to their default values)


Once this is done, you should explicitly call the wb\_connector\_lock() function to lock the Connector.

##### Berethore 07/30/2019 09:38:19
I did that. I added magnetic to the field model for both connector, same for the autoLock at True. I changed isLocked to be False (want to try to lock it with the controller). And then i wrote this in my controller :
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/605695662482259968/unknown.png)
%end


But still , the box just fall


Here is my Connector fields. (Same for the connector on the robot but with type = active) :
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/605696143124463805/unknown.png)
%end

##### Fabien Rohrer [Moderator] 07/30/2019 09:40:54
It seems very correct to me.


the connector.wbt example mentionned above works with these parameters.


maybe except the box is a Robot and the type is passive..


let me check in another example..

##### Berethore 07/30/2019 09:43:10
Ok i got it, the problem is about my way to launch the controller i think

##### Fabien Rohrer [Moderator] 07/30/2019 09:44:17
Great, nice to read this!

##### Berethore 07/30/2019 09:46:44
That was only a librairies problem.. Damn, i'm sure what i did before would have worked too.... He can't load shared libraries libController.so

##### Fabien Rohrer [Moderator] 07/30/2019 09:48:07
haha, no problems, you're welcome

##### Berethore 07/30/2019 09:49:01
Can't we just open the controller from webots ? We have to run it from terminal ?

##### Fabien Rohrer [Moderator] 07/30/2019 09:51:10
No sure to understand... Webots launches its controllers as child processes. Webots can be launched from the terminal.

##### Berethore 07/30/2019 09:56:11
Ok just forget that, was a dumb question, i could answer it myself (what i actually did). Thanks for all your information. Webots is a very great environment and you are doing a great work with it and with the community 😃

##### Marcey 07/30/2019 12:53:03
Hello everybody, I have question: I am using Webots with ROS but I am new to both of them. I want to use the supervisor function to change the translation of my robot using the terminal. I believe it should work with "rosservice call /mybot/supervisor/field/set\_vec3f [service-args]" but I'm struggeling with the service-arguments. Can you help me with that? Or is this a completly wrong way? 😄

##### David Mansolino [Cyberbotics] 07/30/2019 12:54:23
Hi `@Marcey`, you can find a complete list of the service type here: [http://docs.ros.org/melodic/api/webots\_ros/html/index-msg.html](http://docs.ros.org/melodic/api/webots_ros/html/index-msg.html)


And in the supervisor doeumentation: [https://cyberbotics.com/doc/reference/supervisor?tab=ros#wb\_supervisor\_field\_set\_sf\_vec3f](https://cyberbotics.com/doc/reference/supervisor?tab=ros#wb_supervisor_field_set_sf_vec3f)

##### Huey 07/30/2019 12:55:20
thanks again  `@David Mansolino` , i gave a look on the documentation on touch sensor again to investigate the weird reading my robot is getting, and i realize that my coordinate z was not pointing towards the solid object, hence, when the robot is no longer pushing ahead further, the reading become 0. my bad.

##### David Mansolino [Cyberbotics] 07/30/2019 12:56:07
`@Huey` that seems indeed a very good explanation. thank you for the feedback!

##### Marcey 07/30/2019 13:08:33
`@David Mansolino` thanks, so it says:

"uint64 field

int32 index

geometry\_msgs/Vector3 value" 

How do I translate this to a correct command line in the terminal? What are my values for field and index?

##### David Mansolino [Cyberbotics] 07/30/2019 13:11:34
`@Marcey` , index does not really matter if you want to change the translation field (just set it to 0) and 'field' is the id of the field (you can get it with the '/supervisor/node/get\_field' function/service: [https://cyberbotics.com/doc/reference/supervisor?tab=ros#wb\_supervisor\_node\_get\_field](https://cyberbotics.com/doc/reference/supervisor?tab=ros#wb_supervisor_node_get_field))

##### Marcey 07/30/2019 13:34:21
hm, for this service I need a value of the 'node'. How do I get that? "rosservice call /mybot/supervisor/node/get\_field 0 translation" crashes my controller. And does the ID of the field always stay the same or does ist change?

##### David Mansolino [Cyberbotics] 07/30/2019 13:41:01
You first need to get the node id: [https://cyberbotics.com/doc/reference/supervisor?tab=ros#wb\_supervisor\_node\_get\_from\_def](https://cyberbotics.com/doc/reference/supervisor?tab=ros#wb_supervisor_node_get_from_def)


Probably before trying to use Supervisor Services you should get familiar with the Supervisor API, I would strongly recommend to read the Supervisor documentation: [https://cyberbotics.com/doc/reference/supervisor](https://cyberbotics.com/doc/reference/supervisor)

And have a look at this example (including the code): [https://cyberbotics.com/doc/guide/samples-devices#supervisor-wbt](https://cyberbotics.com/doc/guide/samples-devices#supervisor-wbt)

##### Marcey 07/30/2019 14:49:05
Ah ok, I finally was successfull 😃 Thank you! My problem was to transfer the information in the supervisor API into a correct command line. 



Today I also learned that using the terminal to change the position is propably not practical because node handle and field handle change every time webots is restarted. I'll try to find an other way

##### David Mansolino [Cyberbotics] 07/30/2019 15:36:01
Very good news, thank you for the feedback. Indeed you probably need to write a ROS node instead of using the command line.

##### YCL 07/30/2019 17:20:28
Hello is there someone who simulate robots walking in sandy environment? Thank you!

##### MariusJuston [Moderator] 07/30/2019 17:22:05
Like do you mean having sandy properties or just looks like sand?

##### YCL 07/30/2019 17:23:18
Sandy properties. Like robot is  walking in beach.

##### MariusJuston [Moderator] 07/30/2019 17:26:55
That is harder I do not know if Webots can do it, I believe that there are ways to change the surface in the contact surface property field


Maybe look at the bb 8 example


[https://cyberbotics.com/doc/guide/bb8](https://cyberbotics.com/doc/guide/bb8)

##### YCL 07/30/2019 17:33:35
Thank you so much! I will try and learn soon.


Yes, that is what I am considering now. I saw the moon DEMO of Wevots and try to see if it can mimic the real moon contact properties when mimic  robots is moving on moon.    While "texCoord TextureCoordinate {

      point [ "    of    " the geometry IndexedFaceSet {

    coord DEF coord\_Plane Coordinate"     in the "moon.wbt " , there are so many cordinates, I am wondering is there other ways to input the cordinates instead  of  printing it in nodes.

##### MariusJuston [Moderator] 07/30/2019 17:35:07
You can  just copy the index face set and then paste

##### YCL 07/30/2019 17:49:52
Do you mean copying and pasting them like this?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/605819367380353048/unknown.png)
%end

##### MariusJuston [Moderator] 07/30/2019 17:50:59
Click and then press CTRl c on the cord def


And then you can paste is somewhere else

##### YCL 07/30/2019 17:58:18
Hi  MariusJuston, thank you for your kind help. I am wondering do you mean copy one cord and paste it one by one? My question seems so absurd and easy. The reason why I am asking it is if we copy every coor and paste it on nodes like the above picture I showed you, it will be a little time consuming and easy to make mistakes. So I am wondering if I am wrong, maybe there exists other methods that I do not know.

##### MariusJuston [Moderator] 07/31/2019 04:31:20
`@YCL` Sorry for the delay. A simple way to do all this, if you want to just reuse the same GROUND shape is to Export that Shape/Solid. To do this you can left click the Shape/Solid and then select the Export button, this will save the information to a .wbo, to re import this .wbo you can just press the Add button and the press Import (bottom right corner ), find your .wbo and you should see the Shape/Solid appear in the tree directory. If you want to move it around you can `Ctrl + X` (Cut) and then `Ctrl + V` (Paste) it in the location you want it to be; however, please be aware that you cannot place a Shape/Solid everywhere so if `Ctrl + V` fails you can probably assume that that is the reason.  



If you just want to copy the coordinates, you can select, the `point` field in the tree view and then `Ctrl + C` it, if you want to verify that it copied, you can open up notepad and the paste your clipboard there, if there is text then you are good. Once you have copied the points you can go to your new `Coordinate` node, select the point node and the press the delete button on your keyboard, all the points in the `point` should disappear, select `point` and then press `Ctrl + V`. You should see all the points that you copied appear in the `point` field

##### Huey 07/31/2019 11:54:35
Hi `@MariusJuston`  i find your reply to yang chen relevant to a problem i may have too. I am working on a decentralize system, and currently creating a small and abstract experiment on number of homogeneous robots to collaborate on a task. I use 'solid' to represent 'obstacle' needs to be cleared. And when i have too many of those (video), webot would tell me that the 'complicated world' might affect accuracy of calculation.



May i know using the export/import method, does it create a replica/will it reference a link to the source? Thank you
> **Attachment**: [compare\_single\_and\_double\_agent.mp4](https://cdn.discordapp.com/attachments/565154703139405824/606092345280299028/compare_single_and_double_agent.mp4)

##### MariusJuston [Moderator] 07/31/2019 11:59:02
Using the export/import method I believe that it will just create a replica

##### Fabien Rohrer [Moderator] 07/31/2019 11:59:48
Not sure if this helps, but there are several ways to avoid duplicating nodes:


- the DEF-USE mechanism allows to duplicate a node within some context (in a .wbt file, or in a .proto file): [https://www.cyberbotics.com/doc/reference/def-and-use](https://www.cyberbotics.com/doc/reference/def-and-use)


- proto can be contain LUA scripts (containing for example for loops): [https://www.cyberbotics.com/doc/reference/procedural-proto-nodes](https://www.cyberbotics.com/doc/reference/procedural-proto-nodes)

##### MariusJuston [Moderator] 07/31/2019 12:04:21
`@Huey` if you keep the base solid node and then create a bunch of Transforms, then in the each of the transforms use the DEF and USE method to have it so that the solid is not duplicated and just reused and translated to different place. Maybe, I do not remember if transforms can handle solids

##### Fabien Rohrer [Moderator] 07/31/2019 12:05:31
It's better to avoid to put solids in transforms, but a pattern could be:


Solid {

  translation 0 0 0

  name "solid 1"

  children [

    Shape DEF MY\_SHAPE {

      appearance PBRAppearance {}

      geometry Box {}

    }

  ]

  boundingObject USE MY\_SHAPE

  physics DEF MY\_PHYSICS Physics {}

}

Solid {

  translation 0 0 1

  name "solid 2"

  children [

    Shape USE MY\_SHAPE

  ]

  boundingObject USE MY\_SHAPE

  physics USE MY\_PHYSICS

}

Solid {

  translation 0 0 2

  name "solid 3"

  children [

    Shape USE MY\_SHAPE

  ]

  boundingObject USE MY\_SHAPE

  physics USE MY\_PHYSICS

}


You can even be more concise using the procedural templates. Look at the SimpleStair example here: [https://www.cyberbotics.com/doc/reference/procedural-proto-nodes#example](https://www.cyberbotics.com/doc/reference/procedural-proto-nodes#example)

##### Huey 07/31/2019 12:16:33
Oh splendid! Thanks so much for sharing these `@MariusJuston`,  `@Fabien Rohrer`, earlier i was abt confused with the DEF use, as i dont always find the DEF i created, but will sure to take a second look on the documentation and other methods you have mentioned! Double thumbs up to you guys 👍  👍

##### YCL 07/31/2019 14:31:40
Hi, `@MariusJuston` , thank you for kind help! And give me suggestion in detail.

##### MariusJuston [Moderator] 07/31/2019 14:35:55
`@YCL` No problems!

##### YCL 07/31/2019 14:39:07
👏 while I am wondering about this one


in the Animated Skin sample, [https://github.com/omichel/webots/blob/master/projects/samples/rendering/worlds/animated\_skin.wbt](https://github.com/omichel/webots/blob/master/projects/samples/rendering/worlds/animated_skin.wbt), Line 186. there are so many points. How can I create it in my tree or nodes more convinient?
%figure
![C9EH8UGVWVQQX.png](https://cdn.discordapp.com/attachments/565154703139405824/606135971339304960/C9EH8UGVWVQQX.png)
%end

##### Fabien Rohrer [Moderator] 07/31/2019 14:48:57
To be honest Webots is not very good to create meshes.


We used to work with 3D modeling software, especially Blender


I wrote a Blender add-on to export Webots objects: [https://github.com/omichel/blender-webots-exporter](https://github.com/omichel/blender-webots-exporter)


.. but this requires to have skills in Blender and modeling unfortunately.


You could also simply work with basic primitives:


[https://cyberbotics.com/doc/guide/samples-geometries#geometric\_primitives-wbt](https://cyberbotics.com/doc/guide/samples-geometries#geometric_primitives-wbt)

##### YCL 07/31/2019 14:52:42
While it is worthy of it, maybe it is useful and can save time in  long run. In addition, does Solidworks and CAD make sense?


`@Fabien Rohrer` Thank you for your kind help and so many materials provided! 👏

##### Fabien Rohrer [Moderator] 07/31/2019 14:55:03
SolidWorks and other CADs are generally exporting very big meshes, hardly usable directly in Webots. Moreover, physics data (joints, etc.) are lost in this operation. Therefore, using Blender inbetween is for sure the simplest solution.

##### YCL 07/31/2019 15:00:12
As you are familiar with Blender combined with our Webots, could you tell me can Blender build hexapod robots,like those in the picture.



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/606139068383756289/unknown.png)
%end


the thing is I want to build hexapod robots like those in pictures, and then simulate hexapod robots walking in beach which contains sandy properties by Webots.


Is it ok for me to realize it?

##### Fabien Rohrer [Moderator] 07/31/2019 15:02:32
Yes, for sure. We have several legged-robots, and most of the time, we modeled their meshes and materials in Blender before migrating to Webots for the physics and robotics simulation.


We have an hexapod model here: [https://cyberbotics.com/doc/guide/mantis](https://cyberbotics.com/doc/guide/mantis)


.. and your robot is close to this robot look, size and hardware: [https://cyberbotics.com/doc/guide/bioloid](https://cyberbotics.com/doc/guide/bioloid)


I think that you can already go far using Webots only. I recommend you to do our tutorial, you will learn there how to create a robot with Webots basic geometric primitives:


[https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials)


I have also to mention that we can model this for you if you have some budget for this.

##### YCL 07/31/2019 15:28:56
Thank you! The hexiapod robot in your samples is really amazing and it could be control by human being ! I need a small one, about 0.5m l,  0.5m w and 0.3m h for robots' whole size and the shape should be like my physical robot.   Can I revise yours based on my physical robot?

##### Fabien Rohrer [Moderator] 07/31/2019 15:33:45
Feel free to choose your best solution 😃


Downscaling a robot may be tricky.


personnally, I would go for the solution to rewrite it from scratch using the Webots primitives, and to copy the mantis controller.

##### YCL 07/31/2019 15:35:54
Also, how do you ( specialist) in webots  deal with the sandy properties environtment? I saw the contact properties setting. I am wondering can it realize the real scene in our real world that when a robot is walking in beach while the mutual impact bettween sand and robot's feet influence both the state of sand and robot, like the particle dynamics.


Really a good idea! 😀  I learned the tutorials, and built a simple hexapod robot.


And the budget depends on my advisor.


I mean my teacher.

##### Fabien Rohrer [Moderator] 07/31/2019 15:41:50
You could contact sales@cyberbotics.com with precise robot specs to have a quote if you would like to go on. I think such kind of robot could be done in about 1 week of development.


About the sandy environment, you can model this quite precisely I think.


At least, you can set up precisely how the robot feet penetrate the ground.


To do that, first fix the WorldInfo.basicTimeStep (I would say something like 8 ms)


.. and then play with the ContactProperties.softERP and ContactProperties.softCFM (for the contact between the feet and the sand only).


You should be able to define precisely at which depth the robot shrinks.


For sure it's not a perfect deformation simulation, but it gives already a good approximation.

##### YCL 07/31/2019 15:57:21
Get it!  Thank you so much for your kind help and professional instruction!  And Webots are really good!


`@Fabien Rohrer` While the problem is that the I can not  define or set up precisely at which depth the robot shrinks, as this is what I plan to export or obtain from Webots.  And are there any idea about it?

##### Fabien Rohrer [Moderator] 07/31/2019 16:43:02
Did you tried to play with ContactProperties.softERP/CFM? That’s the key to set the shrink depth.


[https://www.cyberbotics.com/doc/reference/contactproperties](https://www.cyberbotics.com/doc/reference/contactproperties)

##### YCL 07/31/2019 17:05:08
Thank you 😀 ! I will try. And if it is needed, I will contact sales@cyberbotics.com.

##### MariusJuston [Moderator] 07/31/2019 18:42:17
`@YCL` sorry being late for the party but on the topic of adding CADed models to Webots what you can is that you can export your CAD as a .obj or an .stl (make sure the scaling is set to meters) then open blender, import the .stl or .obj, then install the adding that `@Fabien Rohrer` made. After importing be sure that the orientation of the object is correct to make sure, to change it select the model and the the plus button the right, that will open up a window where you can select set the orientation. Using the Addin you can the press the export button and then export to a .wbl file not a .wbt

##### YCL 07/31/2019 21:00:45
`@MariusJuston` Hi , welcome party.  it begins. I am studying the Mantis now, while the controller is using this function to control the motor's position.



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/606229921005830214/unknown.png)
%end


Can you explain why should we use this function? Does it relate to Central Pattern Generator?

##### MariusJuston [Moderator] 07/31/2019 21:04:20
To be honest I am not very versed in what the Central Pattern Generator is, but I am assuming that is to make it so that that the robot always has enough points of contact to the ground in order to stay stable and still allows the robot to move forwards.

##### YCL 07/31/2019 21:05:54
Of course, I agree with you. And Central Pattern Generator can realize it. So ...


Then, could you tell me the meaning of "M\_PI" in this fuction

##### MariusJuston [Moderator] 07/31/2019 21:07:09
Oh that is the the PI constant so that is equivalent to 3.14....


[https://stackoverflow.com/questions/15231466/whats-the-difference-between-pi-and-m-pi-in-objc](https://stackoverflow.com/questions/15231466/whats-the-difference-between-pi-and-m-pi-in-objc)


the a[i] is the amplitude of the motion, the 2 PI f is the frequency and p[i] is the phase shift

##### YCL 07/31/2019 21:12:52
Thank you ! this is really clear.  So how do you adjust or choose the amplitude, phase, offset

##### MariusJuston [Moderator] 07/31/2019 21:14:13
So I am assuming that you are the one that define the a, p and d arrays and f constant so you just have to change those values

##### YCL 07/31/2019 21:19:40
Yep. Does this controller  realize the tripod gait on this vedio



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/606234564074930206/unknown.png)
%end

##### MariusJuston [Moderator] 07/31/2019 21:21:23
Oh, I did not know Webots had already implemented this, like me follow along with you. What is the tripod gait?


I do not believe that it does just that but something similar, but do not quote me on that


Actually yes it does do the tripod gait in that video


Wow it really is impressive how it requires so little code to it make look so complex!

##### YCL 07/31/2019 21:30:54
Yep. I see you are moderator. Are you belongs to  cyberbotics?


`@Fabien Rohrer` could you teach us  the  Mantis controller function here.



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/606237895933558786/unknown.png)
%end

##### MariusJuston [Moderator] 07/31/2019 21:32:57
No, I just joined Discord like a month ago because I had a question about Webots and after working with Webots, learning about it for a month and helping people I managed to become a Moderator 😉

##### YCL 07/31/2019 21:35:34
Congratulations!

##### MariusJuston [Moderator] 07/31/2019 21:36:35
Thanks

##### YCL 07/31/2019 21:36:58
What is  your project  with Webots?

##### MariusJuston [Moderator] 07/31/2019 21:38:21
It involved using an industrial robot Fanuc M-710iC


We managed to convert the CAD model of the industrial robot into Webots

##### YCL 07/31/2019 21:39:44
and then?

##### MariusJuston [Moderator] 07/31/2019 21:40:34
We were able to make the robot move to and from positions using inverse kinematics, etc... Sorry I do not know how far into detail I can go with this.

##### YCL 07/31/2019 21:40:59
I would do this too, and thank you again for your kind instruction before.

##### MariusJuston [Moderator] 07/31/2019 21:41:09
No problems

##### YCL 07/31/2019 21:41:34
I mean the  CAD model of the  hixapod robot into Webots

##### MariusJuston [Moderator] 07/31/2019 21:42:00
Did you manage?

##### YCL 07/31/2019 21:42:08
by using Blender


I am planning to do it  in the near future.


Can you give me some suggestion , website etc. ?

##### MariusJuston [Moderator] 07/31/2019 21:44:15
If you want I can create a video of me converting the CAD model of the robot into a Webots model? (I only have Autodesk Inventor though)

##### YCL 07/31/2019 21:46:54
You are so nice! I am very willing to see your video, if it is convenient for you.

##### MariusJuston [Moderator] 07/31/2019 21:47:55
No problems it should not take very long, which version of Blender do you have? Blender just updated to 2.80 and it seems that the Export to .wrl that `@Fabien Rohrer` created no longer appears...

##### YCL 07/31/2019 21:50:41
which version are you using now?

##### MariusJuston [Moderator] 07/31/2019 21:51:23
I was using 2.79, I believe. Do not worry about it I will show you in the video everything

##### YCL 07/31/2019 21:53:46
Thank you so much! the version that is convenient for you  is ok for me.  Then I can setup the same one as you.

##### MariusJuston [Moderator] 07/31/2019 21:54:06
I will show you how to do it in the video, if you want

##### YCL 07/31/2019 21:56:33
Are you an angel? I mean you are so so kind.

##### MariusJuston [Moderator] 07/31/2019 21:57:31
👼


This I you called?

##### YCL 07/31/2019 21:59:44
I think so.


😀

##### MariusJuston [Moderator] 07/31/2019 22:00:34
I will probably have the video in something like 2-3 hours

##### YCL 07/31/2019 22:01:42
You are so efficient! I definitely should learn from you.


Is it your dinner time now?

##### MariusJuston [Moderator] 07/31/2019 22:02:29
No it will probably just take me around 2-3 hours to setup everything up and doing the video

##### YCL 07/31/2019 22:04:18
Ok. I get it.


👏

## August

##### MariusJuston [Moderator] 08/01/2019 00:39:18
`@YCL` Here is the video [https://youtu.be/L0FVsFD2rS4](https://youtu.be/L0FVsFD2rS4)

##### YCL 08/01/2019 02:50:41
`@MariusJuston`  Sorry that reply to now. Thank you so much! I will try later.

##### Fabien Rohrer [Moderator] 08/01/2019 07:39:52
`@MariusJuston` that’s awesome! I’m happy to see the Blender addon is used 😄

##### MariusJuston [Moderator] 08/01/2019 14:10:12
`@Fabien Rohrer` Actually I think that we do not use your Webots addon due to the fact that it does not actually export a *.wrl but a *.wrt


Blender 2.7* has an VRML97 (*.wrl) exporter by default


`@Olivier Michel` I do not know if you remember but previously I was trying to get Webots to run through PyCharm; however, the program stopped at the import statement because it had a `ImportError: DLL load failed error`. You tried to help me; however we did not manage to solve the issue even though I had all the environmental variables setup. You said that you were going to try it on PyCharm; however you were on your trip to China so you had to do it later. I was wondering if you had tried?

##### NERanger 08/02/2019 15:04:01
I am running a simulatoin for a 4WD tractor, how can I get the support force from the ground to the wheel? The return value of function "dBodyGetForce" is only valid until any changes are made, but there is no need to add any force, I only want to check the support force. Can anyone help me out plz 😃

##### YCL 08/03/2019 15:23:09
Hi `@MariusJuston`,  your Youtube Video are very helpful and clear! As I use the Solidework 3D model - your method is also useful!

##### MariusJuston [Moderator] 08/03/2019 15:23:37
No problems glad I was able to help!

##### YCL 08/03/2019 15:24:57
Thank you for kind share!


While I face up with a problem about the scale and coordinates,when imported it in Webots. I am not sure if it is due to the  Solidework 3D model ,  as I see you introduce we need transform function in Blenders in your vedio.


I am wondering do you face up with these kind of situation befor? I plan to try the transform and unit you teach us in your vedio. As I my  Solidework 3D model  is finished by my partner.  Maybe I need to communicate with him about these problem to find out the reason. And can you give me some suggestion?

##### MariusJuston [Moderator] 08/03/2019 18:48:55
`@YCL` sorry about that. So when you imported the file as into blender did it show up as being huge?


The problem is probably the scaling I said inside the video Webots only takes in meters, so if you export your file in inches or mm you will see your model, when imported into Blender being way too large. To fix this either you can export your solid work file directly in the correct units (meters) which is the easiest way of doing it (I have no real idea as to how you would do it in Solidwords because I do not have it). Or you can, when you are exporting the file using blender, when you are asked where to save the file, on the left you can see a scale text bar, make it so that it return current units to meter scale. i.e if it was in inches it would be (1 m /39.3701 inches)  or 0.0254


`@NERanger`  I do not know if you know but  [https://cyberbotics.com/doc/reference/touchsensor](https://cyberbotics.com/doc/reference/touchsensor) has a force mode that can allow it to measure force or touch

##### David Mansolino [Cyberbotics] 08/05/2019 06:12:28
> `@Olivier Michel` I do not know if you remember but previously I was trying to get Webots to run through PyCharm; however, the program stopped at the import statement because it had a ImportError: DLL load failed error. You tried to help me; however we did not manage to solve the issue even though I had all the environmental variables setup. You said that you were going to try it on PyCharm; however you were on your trip to China so you had to do it later. I was wondering if you had tried?



`@MariusJuston`, unfortunately `@omichel` was very very busy last week and he is now on holiday, but he will probably try when he will be back at the end of the week (do not hesitate to re-ask him 😉 )


`@NERanger` , the solution with touch-sensors that `@MariusJuston` suggested is indeed a good solution, you just need to change the wheels of your vehicles to a TouchSensor node instead of a Solid node and set the 'type' field to "force-3d"


Another alternative that doesn't require to change the hierarchy of your vehicle is to use indeed a physic plugin, and in the `webots_physics_collide` callback ([https://cyberbotics.com/doc/reference/callback-functions](https://cyberbotics.com/doc/reference/callback-functions)) create a contact joint and attach a joint feedback to retrieve the force at the contact point ([http://ode.org/wiki/index.php?title=Manual#Joint\_feedback](http://ode.org/wiki/index.php?title=Manual#Joint_feedback)).

##### MariusJuston [Moderator] 08/05/2019 14:43:24
`@David Mansolino` I completely understand, I will thus wait for the end of the week when he comes back.

##### YCL 08/06/2019 02:29:32
`@MariusJuston` Thank you so much for your kind suggestion. Sorry reply you late. Recently I am focusing on and would focus on another task. These like multiple lines run. Definitely I would try your idea later.

##### MariusJuston [Moderator] 08/06/2019 02:30:11
No problems! Good luck!

##### Olivier Michel [Cyberbotics] 08/07/2019 07:11:28
`@MariusJuston`: Sorry I had no time so far to look into the PyCharm issue and I won't be able to do it today as I am still very busy.

##### NERanger 08/07/2019 15:04:56
`@David Mansolino` `@MariusJuston` Thanks for your suggestion, I will try it.

##### David Mansolino [Cyberbotics] 08/07/2019 15:05:17
`@NERanger` you're welcome

##### MariusJuston [Moderator] 08/07/2019 15:07:31
`@Olivier Michel` No problems this is not really urgent so do not really worry about it. Your work should be your priority 😉

##### YCL 08/08/2019 21:28:12
Hello, is there someone dealing with legged-robots?


I am wondering how can I  caculate  the gait funcion's parameter of mantis?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/609139750770704396/unknown.png)
%end


Can you refer me some links ?  Thank you!

##### David Mansolino [Cyberbotics] 08/09/2019 07:01:44
Hi `@YCL`, these coefficient where found empirically. An easy way to thune them is tu use numerical optimization methods: [https://cyberbotics.com/doc/guide/using-numerical-optimization-methods](https://cyberbotics.com/doc/guide/using-numerical-optimization-methods)

##### YCL 08/09/2019 15:56:57
Hi `@David Mansolino` ,Thank you so much for your kind help! I would try this later.


In addition, I want to understand of this sin function. could you recommend me this gait control's  theory. I think I have trouble with the swing and stance phase control.


Thank you!

##### David Mansolino [Cyberbotics] 08/09/2019 16:07:16
`@YCL`, you should probably have a look at 'central pattern generators' (CPG). Here is for example an article about this: [https://www.sciencedirect.com/science/article/pii/S0960982201005814](https://www.sciencedirect.com/science/article/pii/S0960982201005814)

##### YCL 08/09/2019 16:11:23
`@David Mansolino`  👏 Thank you for your kind help! Get it!😀

##### David Mansolino [Cyberbotics] 08/09/2019 18:01:11
You're welcome.

##### SimonDK 08/11/2019 18:35:12
Hi

##### MariusJuston [Moderator] 08/11/2019 18:35:18
Hello

##### SimonDK 08/11/2019 18:36:15
New here, going to check out Webots. Have used Gazebo and vrep for many years in robotics, so a bit excited to see what Webots is about


A small beginner question, is it possible to run Webots on Mac connected to a rosmaster on Ubuntu? Does it require any ros packages compiled on Mac to make this setup work? If not possible, I will keep everything to Ubuntu.

##### MariusJuston [Moderator] 08/11/2019 18:39:46
I believe this might help you: [https://cyberbotics.com/doc/guide/using-ros](https://cyberbotics.com/doc/guide/using-ros)

##### SimonDK 08/11/2019 18:44:46
Thank you, much appreciated

##### MariusJuston [Moderator] 08/11/2019 18:44:58
No problems 😉


`@Fabien Rohrer` for your Blender addon once you have exported your file to a *.wbt how are you supposed to open it in Webots? When I tried to open it using Open World It gave me   `Expected node or PROTO name, found '{'.` and `Failed to load due to syntax error(s).`

##### Olivier Michel [Cyberbotics] 08/12/2019 06:23:14
Hi `@MariusJuston`, `@Fabien Rohrer` is off for vacation, but I can try to answer for him. The Blender addon should produce .wbt files that you can open directly in Webots. If that doesn't work, please open an issue to report the problem, including your .blend file, so that we can try to reproduce it and better understand what's wrong.

##### Liu 08/12/2019 12:02:31
My webots crashed. I try to fix the world program through opening wordpad, but it doesn't work. I also can not open other words. how do I fix it.

##### David Mansolino [Cyberbotics] 08/12/2019 12:03:06
Hi


Was it crashing before? Is it crashing with any world? Are your GPU drivers up to date ?

##### Liu 08/12/2019 12:08:23
it is first time.  i can not open other word or create a new world now. i use the newest webots 2019b.

##### David Mansolino [Cyberbotics] 08/12/2019 12:17:49
Can you please try to stasrt Webots in safe mode: [https://cyberbotics.com/doc/guide/starting-webots#safe-mode](https://cyberbotics.com/doc/guide/starting-webots#safe-mode)

##### Clement 08/12/2019 14:46:50
Hi everyone, I am playing with Webots physics/collision system and I'm facing a weird situation. I have a manipulator arm that I control manually through the Webots interface. When I make the arm push an object against a static object (no physic field), at some point the arm starts passing through the object. I was expecting the arm to be blocked. I tried setting the ERP field to 0 but it doesn't fix it. Is it possible to achieve my goal or is this a limitation from ODE ?

##### Olivier Michel [Cyberbotics] 08/12/2019 14:48:03
Hi Clement,


Does your static object have a boundingObject?


What do you mean control manually from the Webots interface? If you do that from the scene tree, nothing will prevent you from colliding with objects. You should control your robot from your controller so that the simulation runs and the collision detector works.

##### Clement 08/12/2019 14:52:16
Well the static object is a Robot with no boundingObject but it has children which have boundingBox


I am using the robot window that appears when I double click the robot. It allows me to move the joints individually


Here is a video of the simulation: [https://drive.google.com/file/d/1y7L2c4tgUtV1QFr1IhSEaV874-e8H3C9/view?usp=sharing](https://drive.google.com/file/d/1y7L2c4tgUtV1QFr1IhSEaV874-e8H3C9/view?usp=sharing)

##### David Mansolino [Cyberbotics] 08/12/2019 15:04:45
Have you tried decreasing the 'basciTimeStep' ? Or the motor acceleration (bedefault motors have an infinite acceleration)

##### Clement 08/12/2019 15:08:00
I've tried with 16 and 8ms but I haven't seen any difference.

##### Olivier Michel [Cyberbotics] 08/12/2019 15:10:07
It looks like the force of the motor is too strong...


Can you reduce the maxTorque value of the motor to see whether it helps?

##### Clement 08/12/2019 15:14:59
indeed it was set to 10000!!


It works perfectly if I reduce it. Thanks a lot!

##### Olivier Michel [Cyberbotics] 08/12/2019 15:16:18
Great.

##### YCL 08/13/2019 15:02:34
Hello! I see the mantis has tripod gait. Could you please show me  the code or other setup which decides when one pair of three feet fix on ground and when one pair of three feet lift the ground?

##### MariusJuston [Moderator] 08/13/2019 15:10:26
`wb_motor_set_position(motors[i], a[i] * sin(2.0 * M_PI * f * time + p[i]) + d[i]);` It is all part of the sinusoidal function, the amplitude tells the legs if it should from left to right or right to left and by how much it should move by

##### YCL 08/13/2019 15:21:40
Hi,`@MariusJuston` Thank you! Yes. While I need a specific one. As I am confusing how  this fuction realizes which pair of feet fix on ground and only rotate the legs and when feet lift while legs rotate. My robot's feet always slip.


So I am trying to totally understand this function. Then, maybe the robot can go normal.

##### MariusJuston [Moderator] 08/13/2019 15:25:00
That is really all based on timing and fine tuning the function. If you look at the graphs of the robot motors while it is running (double click the robot) you will seen the sinusoidal functions and hopefully that helps you

##### YCL 08/13/2019 15:27:30
Thank you so much for your kind suggestion! Yes, of course I will see it now.


Do you mean this link [https://cyberbotics.com/doc/guide/mantis](https://cyberbotics.com/doc/guide/mantis) ?

##### MariusJuston [Moderator] 08/13/2019 15:30:18
No I meant for in the webots simulation:
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/610857673646407700/unknown.png)
%end

##### YCL 08/13/2019 15:33:28
Thank you! Get it! I will do it right now!


Hi,`@MariusJuston` can you see the webots simulation of mantis for a while again. Could you please zoom in the robot walking scenary and see the motion of the feet? When I do this, I find the feet are slipping.

##### MariusJuston [Moderator] 08/13/2019 15:53:11
I cannot seem to see the legs slipping

##### YCL 08/13/2019 16:29:13
`@MariusJuston` Do you use the same nodes and codes as the github shows?

##### MariusJuston [Moderator] 08/13/2019 16:30:12
I am assuming so? I am using the latest R2019b Webots

##### YCL 08/13/2019 16:31:10
I use the same nodes and codes as the github shows, after the matis robot walks for a while I can find feet slipping. Sorry I focus on this point. As the slipping is what I want to eliminate.


Me too.


While the abnormal thing is that when I run real time simulation for the first time. It alwys shows these.

##### MariusJuston [Moderator] 08/13/2019 16:32:59
Could you show me a video of it happening please

##### YCL 08/13/2019 16:37:18

%figure
![20190813123611.jpg](https://cdn.discordapp.com/attachments/565154703139405824/610874532601004072/20190813123611.jpg)
%end


And then run it again, it succeed.


Ok, please watt a moment. I need to learn how to do it.

##### MariusJuston [Moderator] 08/13/2019 16:38:02
I have never gotten that error...

##### YCL 08/13/2019 16:38:33
I get this error from the first time I use real time simulation.

##### MariusJuston [Moderator] 08/13/2019 16:42:55
You are running the mantis sample world right?

##### YCL 08/13/2019 16:47:07

> **Attachment**: [Mantis.mp4](https://cdn.discordapp.com/attachments/565154703139405824/610877003788320808/Mantis.mp4)


you can see this one.


on 15s the left hind leg is slipping.


also the right hind leg.



%figure
![20190813125102.jpg](https://cdn.discordapp.com/attachments/565154703139405824/610878183075479552/20190813125102.jpg)
%end

##### MariusJuston [Moderator] 08/13/2019 16:53:12
That is probably because it is going up the sloped terain

##### YCL 08/13/2019 16:53:27
mine worlds follow this website [https://github.com/omichel/webots/blob/master/projects/robots/micromagic/mantis/worlds/mantis.wbt](https://github.com/omichel/webots/blob/master/projects/robots/micromagic/mantis/worlds/mantis.wbt)


Mine controller follows this websites[https://github.com/omichel/webots/blob/master/projects/robots/micromagic/mantis/controllers/mantis/mantis.c](https://github.com/omichel/webots/blob/master/projects/robots/micromagic/mantis/controllers/mantis/mantis.c)

##### MariusJuston [Moderator] 08/13/2019 16:56:09
Since the motors are set to absolute positions it is forcing the legs to go to those specific positions no matter. This causes it, if there is a slopped terrain to be lift off from the other legs, to fix this you would need to create some kind of suspention on the legs

##### YCL 08/13/2019 16:56:21
how about yours? Do your mantis' simulation has slipping ?


I want to make sure there is no difference between yours and mine.


to escape I am running in the wrong direction.

##### MariusJuston [Moderator] 08/13/2019 17:21:25
It actually does not seem to be slipping


basicTimeStep 10

FPS 60

ERP 0.62

CFM 1e-05


Try reducing the basic time step maybe?


Oh wait I did notice it slipping a little, when it was going downhill, probably due to its mass


You probably want to fiddle with columbFriction in the ContactProperties in order to increase friction. Set the columbFriction to 100 and try it now


Or if you want you can go in the PROTO of the mantis and then set the mass property in the physics node to be 1

##### YCL 08/13/2019 17:33:02
Ok. Thank  you so much for your valuable time and patience!

##### MariusJuston [Moderator] 08/13/2019 17:34:45
That was already set in the sample world so you do not actually need to change those


But if you were to use the mantis in another world you proabbly want to have very similar world settings

##### YCL 08/13/2019 17:36:09
When you are doing your own robot simulation, how do you deal with these parameters, like basicTimeStep 10

FPS 60

ERP 0.62

CFM 1e-05, mass, coefficiency etc.


Just use the our real world's related parameter?

##### MariusJuston [Moderator] 08/13/2019 17:37:06
Usually you do not need to edit them, but if you see some things behave strangely such as objects sinking in ground you probably want to edit them


[https://www.cyberbotics.com/doc/reference/worldinfo](https://www.cyberbotics.com/doc/reference/worldinfo)


For more info about what each parameter means

##### YCL 08/13/2019 17:44:07
`@MariusJuston` OK. I will try. Thank you !

##### David Mansolino [Cyberbotics] 08/14/2019 09:05:20
`@MariusJuston`, I had a bit of time to test PyCharm with Webots, there is a few tricky settings to set, but it seems to work. Can you please try the following:

  1. Open your simulation in pause mode and set the controller of the robot to `<extern>`

  2. in PyCharm select: `File->Settings->Project->Project Structure-> Add Content Root` and select webots/lib/python37 (or any other version), then OK->Apply->OK.

  3. in PyCharm select: `Run->Edit Configurations->Python', Then in the `Environment variables` value add `LD\_LIBRARY\_PATH` (or `PATH` if you are on windows) and set the value to ${WEBOTS_HOME]/lib



Please let me know if you still have any problems with this procedure.


`@MariusJuston`, this is now documented here: [https://www.cyberbotics.com/doc/guide/using-pycharm-with-webots](https://www.cyberbotics.com/doc/guide/using-pycharm-with-webots)

##### MariusJuston [Moderator] 08/14/2019 14:22:57
`@David Mansolino` I have run through the steps multiple times however it is still not working for me. It is either sending my the same dll not found error still or a new `ImportError: DLL load failed: %1 is not a valid Win32 application.`

##### David Mansolino [Cyberbotics] 08/14/2019 14:24:44
Can you make sure that PyCharm is using a 64bit version of Python ?

##### Olivier Michel [Cyberbotics] 08/14/2019 14:26:11
I just tried on Windows, following David's instructions and it worked fine for me. PyCharm defaulted to Python 2.7. I didn't try with Python 3.7 yet. But I can try it if you want.

##### MariusJuston [Moderator] 08/14/2019 14:26:20
`Python 3.7.3 (default, Apr 24 2019, 15:29:51) [MSC v.1915 64 bit (AMD64)] on win32`


It gave the previous `ImportError: DLL load failed: %1 is not a valid Win32 application.` when I was indeed in a 32 bit  Python 3.7


`@Olivier Michel` I would be really grateful, if you did, thank you! Also `@David Mansolino` many thanks for putting together this web page for Pycharm!


I will try with Python 2. as well

##### David Mansolino [Cyberbotics] 08/14/2019 14:30:25
You're welcome.



> It gave the previous ImportError: DLL load failed: %1 is not a valid Win32 application. when I was indeed in a 32 bit  Python 3.7



Ok, good news, what is the error message you have now?

##### MariusJuston [Moderator] 08/14/2019 14:31:36
It is the same old `ImportError: DLL load failed: The specified module could not be found.`

##### Olivier Michel [Cyberbotics] 08/14/2019 14:32:11
Did you install Webots from the installation package on Windows or are you running Webots in the git development environment?

##### MariusJuston [Moderator] 08/14/2019 14:32:47
I have installed Webots from the installation package on Windows

##### Olivier Michel [Cyberbotics] 08/14/2019 14:35:38
OK, I did the same and it worked for me... Now trying to configure PyCharm to get Python 3.7 running.

##### MariusJuston [Moderator] 08/14/2019 14:35:56
I just tried with Python 2.7 and I still go the error...

##### Olivier Michel [Cyberbotics] 08/14/2019 14:36:10
The error you get is probably because Webots cannot find the Controller.dll.

##### MariusJuston [Moderator] 08/14/2019 14:36:54
Yes but I can see it in C:\Users\mariu\AppData\Local\Programs\Webots\msys64\mingw64\bin

##### David Mansolino [Cyberbotics] 08/14/2019 14:37:06
Maybe you can get more information with 'Debug' insted of 'Run' ?

##### Olivier Michel [Cyberbotics] 08/14/2019 14:37:58
In File / Settings... / Project: driver / Project Structure / Add Content  Root, did you add this folder?

##### MariusJuston [Moderator] 08/14/2019 14:38:09
yes

##### Olivier Michel [Cyberbotics] 08/14/2019 14:40:45
Did you set it also in the Run/Debug configuration for the PATH environment variable?

##### MariusJuston [Moderator] 08/14/2019 14:41:16

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/611207719617298432/unknown.png)
%end


```Traceback (most recent call last):
  File "C:\Users\mariu\AppData\Local\JetBrains\Toolbox\apps\PyCharm-P\ch-0\192.5728.105\helpers\pydev\pydevd.py", line 2060, in <module>
    main()
  File "C:\Users\mariu\AppData\Local\JetBrains\Toolbox\apps\PyCharm-P\ch-0\192.5728.105\helpers\pydev\pydevd.py", line 2054, in main
    globals = debugger.run(setup['file'], None, None, is_module)
  File "C:\Users\mariu\AppData\Local\JetBrains\Toolbox\apps\PyCharm-P\ch-0\192.5728.105\helpers\pydev\pydevd.py", line 1405, in run
    return self._exec(is_module, entry_point_fn, module_name, file, globals, locals)
  File "C:\Users\mariu\AppData\Local\JetBrains\Toolbox\apps\PyCharm-P\ch-0\192.5728.105\helpers\pydev\pydevd.py", line 1412, in _exec
    pydev_imports.execfile(file, globals, locals)  # execute the script
  File "C:\Users\mariu\AppData\Local\JetBrains\Toolbox\apps\PyCharm-P\ch-0\192.5728.105\helpers\pydev\_pydev_imps\_pydev_execfile.py", line 18, in execfile
    exec(compile(contents+"\n", file, 'exec'), glob, loc)
  File "C:/Users/mariu/Documents/GitHub/****/*****/controllers/collision_detection/collision_detection.py", line 1, in <module>
    from controller import Supervisor, Motor, PositionSensor, TouchSensor
  File "C:\Users\mariu\AppData\Local\Programs\Webots\lib\python37\controller.py", line 16, in <module>
    import _controller
ImportError: DLL load failed: The specified module could not be found.
```



This is the output when running it through the debugger

##### David Mansolino [Cyberbotics] 08/14/2019 14:45:37
do you have a '\_controller' library in `C:\Users\mariu\AppData\Local\Programs\Webots\lib\python37\` ?

##### MariusJuston [Moderator] 08/14/2019 14:46:17

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/611208981477589013/unknown.png)
%end


I am just as confused as you guys are. There should be no reason as to why it would not work...

##### Olivier Michel [Cyberbotics] 08/14/2019 14:53:45
Can you check that in File / Settings... / Project: driver / Project Structure  you have under "+ Add Content Root" the following path: C:\Users\mariu\AppData\Local\Programs\Webots\lib\python37?



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/611211057717051392/unknown.png)
%end


Which version of Python 3 do you have ?

##### MariusJuston [Moderator] 08/14/2019 14:55:40
Python 3.7.3 (default, Apr 24 2019, 15:29:51) [MSC v.1915 64 bit (AMD64)] on win32
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/611211346931089438/unknown.png)
%end


I am using a Anaconda virtual environment

##### Olivier Michel [Cyberbotics] 08/14/2019 14:56:26
OK, that seems correct. Are you trying to open the "driver.py" example as in the documenation?

##### MariusJuston [Moderator] 08/14/2019 14:57:09
No I am trying to open my own controller, where is that controller located?

##### Olivier Michel [Cyberbotics] 08/14/2019 14:57:17
I am not using Anaconda... That's a difference...


In WEBOTS\_HOME\projects\languages\python\controllers\driver

##### MariusJuston [Moderator] 08/14/2019 14:57:58
got it

##### Olivier Michel [Cyberbotics] 08/14/2019 14:58:58
Can you try without Anaconda (using the standard Python from Python.org)?

##### MariusJuston [Moderator] 08/14/2019 14:59:16
That was just what I was about to try

##### Olivier Michel [Cyberbotics] 08/14/2019 15:00:01
For Anaconda, you might need to recompile the python libraries as explained here: [https://cyberbotics.com/doc/guide/using-python#use-an-alternative-python-version](https://cyberbotics.com/doc/guide/using-python#use-an-alternative-python-version). However, it can be tricky...


The safest option is probably to use the standard Python (in a first step).

##### MariusJuston [Moderator] 08/14/2019 15:01:07
It should be fine if I used Python 3.7.4 right?

##### Olivier Michel [Cyberbotics] 08/14/2019 15:01:36
Yes, I think so.


(beware to get the 64 bit version)

##### MariusJuston [Moderator] 08/14/2019 15:02:50
Yes I made sure to get the 64 bit version this time

##### Olivier Michel [Cyberbotics] 08/14/2019 15:03:00
I am installing it as well to test it.


I confirm it is working well with Python 3.7.4 from python.org as well.

##### MariusJuston [Moderator] 08/14/2019 15:05:29
Ah, it seem to be working now

##### Olivier Michel [Cyberbotics] 08/14/2019 15:06:27
Is there any reason why you used Anaconda in the first place?

##### MariusJuston [Moderator] 08/14/2019 15:06:40
To use virtual environments

##### Olivier Michel [Cyberbotics] 08/14/2019 15:06:49
OK.


So, if you really need Anaconda, you should probably try to recompile the Webots Python libraries with Anaconda as explained here: [https://cyberbotics.com/doc/guide/using-python#use-an-alternative-python-version](https://cyberbotics.com/doc/guide/using-python#use-an-alternative-python-version)

##### MariusJuston [Moderator] 08/14/2019 15:08:14
Thank you so much for the time your spent on this `@Olivier Michel` `@David Mansolino` !!!

##### David Mansolino [Cyberbotics] 08/14/2019 15:08:54
You're welcome. Thank you for the feedback!

##### juan jose 08/15/2019 02:40:19
Hi, my name is Juan and I am new, I come to ask for help, I have problems linking matlab and webots


could someone help me ?, thanks

##### David Mansolino [Cyberbotics] 08/15/2019 05:58:33
Hi Juan!


Yes of course, what is your problem?


First, did you read this page?

[https://www.cyberbotics.com/doc/guide/using-matlab](https://www.cyberbotics.com/doc/guide/using-matlab)

##### Mr. Scruff 08/15/2019 16:26:14
I'm having a terrain problem where the wheels of my virtual tractor keep sinking in the terrain. 

Here is a small video:   [https://agjunction-my.sharepoint.com/:v:/p/mvillela/Ead4Z4TufSBDimFK2IWp-1QBhme0FEhT6Bj1niilwLqCOA?e=4OIt0O](https://agjunction-my.sharepoint.com/:v:/p/mvillela/Ead4Z4TufSBDimFK2IWp-1QBhme0FEhT6Bj1niilwLqCOA?e=4OIt0O)

##### MariusJuston [Moderator] 08/15/2019 16:27:12
Try changing the basic time step to be smaller

##### Mr. Scruff 08/15/2019 16:30:02
Thanks, Marius.   My basic time step is already at 4msec.


But the problem persists.  This happens with the front wheels and with the back wheels.  It is more easily visible when the wheels are spinning at low speeds, as shown in the video.  The problem also appears to not happen in flat terrain.

##### MariusJuston [Moderator] 08/15/2019 16:54:49
Have you tried changing the softCFM in the ContactsProperties


The object is sinking because it is so heavy

##### Mr. Scruff 08/15/2019 16:55:26
I will try that next.  Thank you.


I am reading about softCFM at [https://www.cyberbotics.com/doc/reference/worldinfo](https://www.cyberbotics.com/doc/reference/worldinfo)

Can you provide more in-depth information?  Perhaps a paper or some other documentation that talks about softCFM in more detail?

##### MariusJuston [Moderator] 08/15/2019 17:08:46
To be honest I am not sure exactly how it works in the backend but I was informed when I had a similar problem of my object sinking in the ground to change the CFM and it seemed to work. From what I can understand from the documentation, imagine that you have a spring (which represents the ground) and the spring constant (k) is represented by the CFM. If you have a high spring constant (lower CFM) it will take more force for it be pressed down, if you have a low spring constant (higher CFM) it will take less force to push the spring down thus causing the object to “sink” in the material. If you do not want the spring be able to sink you would make the spring constant infinity, in this case make the CFM 0.


Hope this helps...

##### Mr. Scruff 08/15/2019 17:14:09
This does help.  I'm curious to see the effects of changing the softCFM.  Thanks again.


Hi `@MariusJuston`.   While the effects of changing the softCFM were clearly visible,  unfortunately it was not able to resolve the issue.  As you can see in the video, the tractor moves fine in certain areas, but very briefly sinks in others.  This effect does not manifest itself in flat terrains.

[https://agjunction-my.sharepoint.com/:v:/p/mvillela/Ead4Z4TufSBDimFK2IWp-1QBQBMu6c6aIAGeGk50m0D2Ow?e=5jG3Nk](https://agjunction-my.sharepoint.com/:v:/p/mvillela/Ead4Z4TufSBDimFK2IWp-1QBQBMu6c6aIAGeGk50m0D2Ow?e=5jG3Nk)

##### MariusJuston [Moderator] 08/15/2019 21:24:09
Maybe it has something to do with the uneven terrain generator?

##### Mr. Scruff 08/15/2019 22:16:36
I've tried 2 independent terrain generators, but got the same results.   Can you send me a known good terrain so I can test?


Not flat.

##### MariusJuston [Moderator] 08/15/2019 22:53:40
Try the terrain that is in the mantis test world

##### aknuclear 08/16/2019 15:52:23
Is there any example code in webots exmaple where the rotational motor is controlled by the setPosition() function?

##### MariusJuston [Moderator] 08/16/2019 16:19:31
`@aknuclear` [https://cyberbotics.com/doc/guide/tutorial-1-your-first-simulation-in-webots-20-minutes](https://cyberbotics.com/doc/guide/tutorial-1-your-first-simulation-in-webots-20-minutes)

##### Mr. Scruff 08/16/2019 17:30:09
Hi `@MariusJuston` .  I simplified my experiment by using the standard Boomer vehicle, and driving it in the sample terrain that comes with the Clearpath Moose terrain.   Under these conditions, we've been able to duplicate the issue where, at slow speeds, the tractor tires suddenly sink into the ground and then suddenly pop out of the ground.

Given that we are using components that come standard with Webots, without modification, can you reproduce the problem?

[https://agjunction-my.sharepoint.com/:v:/p/mvillela/Ead4Z4TufSBDimFK2IWp-1QBQBMu6c6aIAGeGk50m0D2Ow?e=5jG3Nk](https://agjunction-my.sharepoint.com/:v:/p/mvillela/Ead4Z4TufSBDimFK2IWp-1QBQBMu6c6aIAGeGk50m0D2Ow?e=5jG3Nk)

##### MariusJuston [Moderator] 08/16/2019 17:43:21
`@Mr. Scruff` you are talking about this kind of setup right?
%figure
![mr_scruff.png](https://cdn.discordapp.com/attachments/565154703139405824/611978319436447745/mr_scruff.png)
%end

##### Mr. Scruff 08/16/2019 17:44:17
Exactly!   🙂


I'll post a video of that experiment.

##### MariusJuston [Moderator] 08/16/2019 17:45:10
I am also having the tractor go into the ground, let me mess around with the constants to see if it does anything

##### Mr. Scruff 08/16/2019 17:46:16
[https://photos.app.goo.gl/Xdtwq2hbeLgkg2Pe8](https://photos.app.goo.gl/Xdtwq2hbeLgkg2Pe8)


This new 18 seconds video is a good one.  That left tire slowly sinks and then abruptly pops up again.

##### MariusJuston [Moderator] 08/16/2019 17:47:36
It is also happening on my side...

##### Mr. Scruff 08/16/2019 17:48:16
Glad to hear that.   It appears to only happen close to the connection point between 2 adjacent tiles.

##### MariusJuston [Moderator] 08/16/2019 17:58:50
For me it never behaves exactly how yours does, it never has wheels jumping around... all I did was copy the same WorldInfo as the moose\_demo.wbt and it works, yes the tractor still sinks a little in the ground but it is barely noticeable


Here is my world if you want
> **Attachment**: [mr\_scruff.wbt](https://cdn.discordapp.com/attachments/565154703139405824/611982779961311239/mr_scruff.wbt)

##### Mr. Scruff 08/16/2019 19:07:25
Thanks for the .wbt.     I think the reason why it is barely noticeable when you tried it is because you need to run it at slower speeds, like 0.1 kph.   Can you run at these low speeds?   We tried the world you provided and the problem is there, just like in all the other worlds that we've tried so far.

##### r0dvan 08/17/2019 01:26:29
Greetings everyone. I am a developer interested in learning various algorithms to assemble my bots. But since I dont have enough resources to buy all required parts, this project will allow me to simulate that right?

##### Akash 08/17/2019 11:10:27
Is it possible to run some other function while the bot is moving with the setPosition() function  ?

##### Huey 08/18/2019 20:00:24
Hi, may I know if there s a way to store all values throughout the entire simulation session?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/612737586481594393/unknown.png)
%end

##### MariusJuston [Moderator] 08/18/2019 20:01:30
You can store these values in arrays

##### Huey 08/18/2019 20:08:49
seems like that s the only way.


Thanks marius

##### MariusJuston [Moderator] 08/18/2019 20:09:30
No problems, as far as I know there is way to export these values directly from the plots

##### Huey 08/19/2019 00:34:14
i wondering if there would be a way to store separate sets of values each robot collects, e.g using three identical robot in the simulation
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/612806496777928714/unknown.png)
%end

##### MariusJuston [Moderator] 08/19/2019 00:35:21
Well if you store it using arrays, each robot will create a unique instance of the controller so that a controller instance is only associate with the robot

##### Huey 08/19/2019 00:37:49
oh.. you mean like a csv file?

##### MariusJuston [Moderator] 08/19/2019 00:42:12
Sorry I might have been unclear. What I was saying was that each robot when it runs creates its own instance of the Controller. Thus meaning that none of the variables are shared. Each robot, if you were to have a controller that stored the values into an array would have arrays with different values. You could then, if you wanted to, export those arrays to a file such as a *.csv file. Was that what you were asking?

##### Huey 08/19/2019 00:51:01
I'm not sure if I was doing this correctly, I use println function to print values to the console, but then I was confused how to differentiate which value belongs to which robot

##### MariusJuston [Moderator] 08/19/2019 00:51:33
Oh so what you can do is that you can get the name of the robot and then print that along side the value


What language are you using ?

##### Huey 08/19/2019 00:53:40
I'm using java. I created a proto for the robot, they kinda share the same name


Would using DEF work in this situation?

##### MariusJuston [Moderator] 08/19/2019 00:55:10
?


Can you change the “name” field?

##### Huey 08/19/2019 00:57:06
Now that you mentioned it, perhaps I can make the name field public under proto, so I can give each a different name


You think that might work?

##### MariusJuston [Moderator] 08/19/2019 00:57:22
That would be the easiest solution

##### Huey 08/19/2019 00:57:58
Thanks for shedding light on the mate :D I'm gonna try that later :)))

##### MariusJuston [Moderator] 08/19/2019 00:58:05
No problems!


Or if the controllerArgs field is visible you can pass in the name of your robot and then read it using the args in the `public static void main(String[] args) `

##### bhanu 08/19/2019 05:49:16
I am new to robotics. I have installed webots 2019b on Windows 10. Frequently the ide is not responding after create a new project

##### Huey 08/19/2019 06:22:52
`@MariusJuston` I made the name field public, and use the getName function to print it alongside the value, copy to excel to separate data. Works like a charm! THANK YOU😁

##### David Mansolino [Cyberbotics] 08/19/2019 06:54:58
`@r0dvan`, yes exactly, you will be able to use simulation instead of expensive hardware using Webots 🙂


`@Akash`, yes of course, the setPosition() function is not blocking, while the motor is moving you can perform other tasks.


`@bhanu`, what is your computer configuration? Are your GPU dirvers up to date? Do you have writte rights where the new project is created?

##### bhanu 08/19/2019 07:01:55
windows ,intel i5-8th gen ,8gb ram,intel u620 graphics + Nvidia Mx150 with driver version 431.60. I created project in documents folder

##### David Mansolino [Cyberbotics] 08/19/2019 07:03:07
Ok, thank you. Can you maybe try to start Webots in safe mode to check if the problem is still present: [https://cyberbotics.com/doc/guide/starting-webots#safe-mode](https://cyberbotics.com/doc/guide/starting-webots#safe-mode)

##### Mr. Scruff 08/19/2019 18:53:16
Hi `@MariusJuston` .

Thanks for the .wbt.     I think the reason why it is barely noticeable when you tried it is because you need to run it at slower speeds, like 0.1 kph.   Can you run at these low speeds?   We tried the world you provided and the problem is there, just like in all the other worlds that we've tried so far.

##### owen6789 08/20/2019 08:15:01
Hi, I wonder to know how URDF or solidwork file convert to  Webots PROTO file?

##### David Mansolino [Cyberbotics] 08/20/2019 08:15:17
Hi `@owen6789`


We have a script converting an urdf file to a Webots compatible PROTO file, you can find this script here: [https://github.com/omichel/urdf2webots](https://github.com/omichel/urdf2webots)

Let us know if you have any problem with this script


About Solidworks to Webots, you will have to export your Solidworks file to VRML2.0 which can then be imported in Webots (but you will loose some information in the process), here is an example from autodesk (but the process is very similar): [https://www.youtube.com/watch?v=L0FVsFD2rS4&feature=youtu.be](https://www.youtube.com/watch?v=L0FVsFD2rS4&feature=youtu.be)

##### owen6789 08/20/2019 08:17:59
OK, I got it. I will try. Thanks for helping me.

##### David Mansolino [Cyberbotics] 08/20/2019 08:18:12
You're welcome

##### NERanger 08/20/2019 16:14:51
I wonder if there is a difference between the field "coulombFriction" in the node "ContactProperties" and the mu in struct "dSurfaceParameters" in ODE explained as the "coulomb Friction coefficient". I have this question because when I am using ContactProperties, set coulombFriction to 1 is enough to prevent slip, but when I am using collsion handling callback function provided by Webots, I always need to set the "mu" to about 200 to prevent slip (I do not want to set it to infinity since I need to implement "slip ratio"). I suspect that the "mu" explained as "coulomb Friction coefficient" in the document is actually divided by some number like 1000. But I am not sure since the document does not mention it. Can anyone tell me what is the difference between these two things? Thanks for listening to my question 😃

##### David Mansolino [Cyberbotics] 08/21/2019 06:01:06
`@NERanger`, the behavior you mention is strange, the only difference between `coulombFriction` and `mu` is that `mu` is set to `dInfinity` if `coulombFriction` is -1.

This is done here: [https://github.com/omichel/webots/blob/0e8541fc48cda235cec85f6d3131dcf13c93a3c2/src/webots/engine/WbSimulationCluster.cpp#L228](https://github.com/omichel/webots/blob/0e8541fc48cda235cec85f6d3131dcf13c93a3c2/src/webots/engine/WbSimulationCluster.cpp#L228)



And then assigned to the contact surface here: [https://github.com/omichel/webots/blob/revision/src/webots/engine/WbSimulationCluster.cpp#L279](https://github.com/omichel/webots/blob/revision/src/webots/engine/WbSimulationCluster.cpp#L279)

Are you sure you are setting correctly the flags in your physics plugin? In Webots this is done here: [https://github.com/omichel/webots/blob/revision/src/webots/engine/WbSimulationCluster.cpp#L333](https://github.com/omichel/webots/blob/revision/src/webots/engine/WbSimulationCluster.cpp#L333))

Are you using asymetric friction (more than one value in the `coulombFriction` field)?

##### NERanger 08/21/2019 14:19:34
`@David Mansolino` I am not using asymetric friction. I use mu for the friction between tractor wheel and ground, should I enable rolling firction instead?

##### David Mansolino [Cyberbotics] 08/21/2019 14:28:05
Can you try to use in your physics plugins the equivalent than in the Webots contact properties:

```
contact->surface.mu = coulombFriction;
contact->surface.mode = (bounce != 0.0 ? dContactBounce : 0) | dContactApprox1 | dContactSoftCFM | dContactSoftERP;
if (forceDependentSlipSize.size() > 1)
  contact->surface.mode = contact->surface.mode | dContactMu2;
contact->surface.slip1 = forceDependentSlipSize;
contact->surface.slip2 = forceDependentSlipSize;
contact->surface.bounce = bounce;
contact->surface.bounce_vel = bounceVelocity;
contact->surface.soft_cfm = softCFM;
contact->surface.soft_erp = softERP;
```

And compoare if you have the same behavior or not?

##### NERanger 08/21/2019 14:39:49
I found that I did not set the dContactApprox1 flag. Now I got normal behaviour. Appreciate you help ! You really help me out☺ `@David Mansolino`

##### David Mansolino [Cyberbotics] 08/21/2019 14:40:12
Very good news, thank you for the feedback 😄

##### Dannte98 08/21/2019 21:57:32
Good afternoon

##### David Mansolino [Cyberbotics] 08/22/2019 06:12:37
Hi `@Dannte98`

##### Rambo 08/23/2019 16:12:17
I was applying a force which includes a 'for' loop to a vehicle's wheel in the physics plugin,(in physics step, dbodyaddforce).  the force was supposed to be calculated by integration, I simply use a for loop to add things up. If the division of the integeration is over 3(three times in for loop) and the force was applied, the webots crash. However, if the force is only displayed and calculated in webots, but not applied, the division of the integration could go as high as 1000 and still runs smoothly. And the forces calculated by normal means like adds multiflies and such works normally. I tried many methods, but none worked. I wonder if you guys have any clues to this...😫

##### TheWgang 08/24/2019 20:38:37
Hi, the light sensor function returns a value 'E' based on the incidence of light on the sensor. How can  i detect a red light using the light sensor function

##### Olivier Michel [Cyberbotics] 08/26/2019 06:22:55
Hi `@Rambo`: unfortunately, I have no idea about the problem you are experiencing. Did you try to apply a force of 0 on this object? Does it still crash? If so, the pointer to the object may be wrong.


hi `@TheWgang`: light sensors don't make any different between colored lights. To detect a colored light, I would recommend to use a small white box put in front of a low resolution camera. The box will reflect the color of the light and the camera will get it.

##### Marcey 08/26/2019 08:59:30
Hi, I'm using ROS to get to the translation of my robot. In my program I use the "node\_get\_field"-service. This worked until I tried to convert my Robot to a PROTO. Now I can not get the translation field anymore. The supervisor API says :



    Note: The wb\_supervisor\_node\_get\_field function will return a valid field handler if the field corresponding to the field name is an hidden field.



What would be the correct field name to get the handler to a hidden field?

##### Fabien Rohrer [Moderator] 08/26/2019 09:21:12
Hi Marcey, it seems you are getting a field which is hidden in the PROTO definition.


To verify this, simply look at your PROTO header (open it in a text editor), and check if this field has the "hiddenField" keyword, like here:


[https://github.com/omichel/webots/blob/master/projects/robots/saeon/protos/AltinoWheel.proto#L8](https://github.com/omichel/webots/blob/master/projects/robots/saeon/protos/AltinoWheel.proto#L8)


Is it this case?


To be accessible from the Supervisor, the field should be "open", i.e., defined as a regular field in the PROTO definition.


(like the AltinoWheel.name field mentioned above)

##### Marcey 08/26/2019 09:35:30
`@Fabien Rohrer` thank you! That worked 🙂

##### Rambo 08/26/2019 14:00:06
`@Olivier Michel` , Thanks for your advice! I tried apply other force(both 0 and larger than 0), and it would not crash. However, if the applied force is calculated by 'for' loops(over 3) in the physics step, webots would crash. I will continue to try to find out the solution. thank you!

##### tonyzjtong 08/26/2019 17:40:44
Does anyone have any idea on why my controller crashes when I'm trying to process the camera image? I think the camera size is too big, I'm using the multisense camera node with 1024 x 544 resolution.


It crashes every time when I'm trying to pass/output the wb\_camera\_get\_image() result. But with a smaller resolution, it's working just fine.

##### Fabien Rohrer [Moderator] 08/26/2019 18:04:59
Could it be due to a memory management issue? Did you check these snippets? [https://cyberbotics.com/doc/reference/camera#wb\_camera\_get\_image](https://cyberbotics.com/doc/reference/camera#wb_camera_get_image)


You should share your code here 😉

##### Frodo 08/26/2019 21:55:52
Hello everybody! I'm new to Webots and I'm developing a bioinspired algorith based on a bug from the family psocoptera. I already have tried it on Matlab but now i want to implement it using e-puck robots. Nevertheless, I'm looking for a way to control them like sending positions or velocities to the robots like goto(). So I don't know if there's already some function that does that or I'll have to implement it by myself. I'm not really interested in a realistic control method since, for now, I just want to get the robots moving with my algorithm. I've already looked at something called Compass and Supervisor but I couldn't manage to find some examples with the e-puck. I'm using Python. Thank you in advance for your advice!

##### David Mansolino [Cyberbotics] 08/27/2019 06:02:49
HI `@Frodo`, if you don't car about realistic motion/control, a good solution is to set the 'kinematic' field of the e-puck node to TRUE. Then the Supervisor is indeed the way to go, you can get move/rotate the robot at any position you want.


You can find in this example a simple supervisor controller that moves the position of the light: [https://cyberbotics.com/doc/guide/samples-devices#supervisor-wbt](https://cyberbotics.com/doc/guide/samples-devices#supervisor-wbt)


The samle python controller also use the supervisor to move a robot: [https://github.com/omichel/webots/blob/revision/projects/languages/python/controllers/driver/driver.py](https://github.com/omichel/webots/blob/revision/projects/languages/python/controllers/driver/driver.py)


The main steps are the following:


1. get the robot node handle: [https://github.com/omichel/webots/blob/7fb253767f2b4077f396f7e6e37510c00cd441a3/projects/languages/python/controllers/driver/driver.py#L33](https://github.com/omichel/webots/blob/7fb253767f2b4077f396f7e6e37510c00cd441a3/projects/languages/python/controllers/driver/driver.py#L33)


2. get the translation field of the node: [https://github.com/omichel/webots/blob/7fb253767f2b4077f396f7e6e37510c00cd441a3/projects/languages/python/controllers/driver/driver.py#L34](https://github.com/omichel/webots/blob/7fb253767f2b4077f396f7e6e37510c00cd441a3/projects/languages/python/controllers/driver/driver.py#L34)


3. in the main loop, get or set the translation field: 

[https://github.com/omichel/webots/blob/7fb253767f2b4077f396f7e6e37510c00cd441a3/projects/languages/python/controllers/driver/driver.py#L58](https://github.com/omichel/webots/blob/7fb253767f2b4077f396f7e6e37510c00cd441a3/projects/languages/python/controllers/driver/driver.py#L58)

[https://github.com/omichel/webots/blob/7fb253767f2b4077f396f7e6e37510c00cd441a3/projects/languages/python/controllers/driver/driver.py#L62](https://github.com/omichel/webots/blob/7fb253767f2b4077f396f7e6e37510c00cd441a3/projects/languages/python/controllers/driver/driver.py#L62)

##### Ashish 08/27/2019 06:27:35
Hello,


I have started my phd in robotics in India

##### Fabien Rohrer [Moderator] 08/27/2019 06:27:57
Hi Ashish

##### Ashish 08/27/2019 06:28:09
Planning to work on Stability and Gait Pattern Generation of Quadruped


Is webots good for me?

##### Fabien Rohrer [Moderator] 08/27/2019 06:29:42
For sure, Webots is particularly good to simulate robots with legs, and to create controllers linked with specific libraries.

##### Ashish 08/27/2019 06:30:00
ok. Great

##### Fabien Rohrer [Moderator] 08/27/2019 06:30:07
You may be interested by this example: [https://www.cyberbotics.com/doc/guide/salamander](https://www.cyberbotics.com/doc/guide/salamander)

##### Ashish 08/27/2019 06:30:17
Please suggest the best option to learn the software

##### Fabien Rohrer [Moderator] 08/27/2019 06:30:45
The salamander is controlled by a CPG. This example is released in Webots and work out of the box.

##### Ashish 08/27/2019 06:31:03
great

##### Fabien Rohrer [Moderator] 08/27/2019 06:31:19
It depends if you would like to create robots or only controllers, but I would recommend you to do our tutorial first:


[https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials)

##### Ashish 08/27/2019 06:31:39
ok

##### Fabien Rohrer [Moderator] 08/27/2019 06:31:40
It will learn you the basics

##### Ashish 08/27/2019 06:31:47
both robot and controller

##### Fabien Rohrer [Moderator] 08/27/2019 06:31:58
And then, to look at the salamander demo.

##### Ashish 08/27/2019 06:32:08
okk

##### Fabien Rohrer [Moderator] 08/27/2019 06:32:29
Don't hesitate to ask here if you're stuck at any point 😉

##### Ashish 08/27/2019 06:33:04
Thanks a lot.

##### Abinav 08/27/2019 09:22:40
Hi, I just stumbled upon WeBots while looking for a robotics simulation package for Tracked Robots. Is it possible to model Tracked Robots?

##### Fabien Rohrer [Moderator] 08/27/2019 09:23:04
Hi Abinav


Yes this is possible


Please look at the following node:


[https://cyberbotics.com/doc/reference/track](https://cyberbotics.com/doc/reference/track)

##### Abinav 08/27/2019 09:24:07
Thanks.

##### Fabien Rohrer [Moderator] 08/27/2019 09:24:18
This robot for example is using the Track node: [https://cyberbotics.com/doc/guide/telemax-pro](https://cyberbotics.com/doc/guide/telemax-pro)

##### Abinav 08/27/2019 09:26:01
Interesting. Is it possible to model through a GUI or is it code based?

##### Fabien Rohrer [Moderator] 08/27/2019 09:27:13
Yes, Webots has a GUI


You can model a robot using a tree view by adding nodes defining the physics / graphics / actuators / sensors


It looks like this: [https://cyberbotics.com/doc/guide/the-user-interface](https://cyberbotics.com/doc/guide/the-user-interface)


The models (robots / objects / environments) are stored in a human-readable format (a language derived from VRML). So it's also possible to modify anything programatically.

##### Abinav 08/27/2019 09:31:28
Just now installed it. I'm going through the tutorials. Looks like this will ease all the pain. Thanks again.

##### Fabien Rohrer [Moderator] 08/27/2019 09:31:56
Sure, our tutorial is the good starting point 😉

##### sonjay 08/28/2019 02:12:05
Hello, i'm having trouble following Tutorial 6


The matlab code doesn't work

##### David Mansolino [Cyberbotics] 08/28/2019 05:58:15
Hi, `@sonjay`, can you give more details? Which part of the code doesn't work and what is th error message?

##### Chen-moon 08/28/2019 11:21:20
Hi `@David Mansolino` , I  reported a bug to the github a month ago(The title is HTC Vive Pro is not working in Webots).


I just want to ask how about the progress about the bug fixed, thank you!

##### David Mansolino [Cyberbotics] 08/28/2019 11:53:49
Hi `@Chen-moon`, I am sorry but unfortunately I haven't been able to get our HTC Vive back yet to test, neither had the time to create an emulation of the headset.

##### Chen-moon 08/28/2019 12:49:05
Hi `@David Mansolino` , thanks for uour reply!


*your

##### David Mansolino [Cyberbotics] 08/28/2019 12:49:23
You're welcome.

##### Chen-moon 08/28/2019 12:51:49
But, I have another question about how long can your HTC Vive back and test the headset?Thanks!

##### David Mansolino [Cyberbotics] 08/28/2019 12:56:56
I am sorry but I have no idea, we did lend our HTC for a demonstration at the World Virtual Reality Forum ([https://worldxr.org/](https://worldxr.org/))

##### Frodo 08/28/2019 21:53:28
Thank you so much for the info `@David Mansolino`  I'll start working on that!


Do you know how to write text into the Webots Console? I can't find a way to write something into the console

##### Chen-moon 08/29/2019 01:28:00
Hi `@David Mansolino` , thanks for your reply!

##### Olivier Michel [Cyberbotics] 08/29/2019 05:49:14
`@Frodo`: did you try print()? Note you need to call Robot.step() so that your print command is sent to Webots and displayed in the console.

##### SasukeFan 08/29/2019 14:04:36
hello, how can i convert .txt to .motion that webots can understand?I have a .txt file that contains the angle of each joints.Now i want nao to be in that position specified in that .txt file. For that reason i have to convert it into .motion file. So how should i do that?Can anyone help

##### Fabien Rohrer [Moderator] 08/29/2019 14:06:13
The Webots motion files are CSV human readable files


For example:


[https://github.com/omichel/webots/blob/revision/projects/robots/softbank/nao/motions/Backwards.motion](https://github.com/omichel/webots/blob/revision/projects/robots/softbank/nao/motions/Backwards.motion)


You should try to forge a similar file.


Basically, the header list the motor names, and each line contain a pose (time,posName,motorPositions*)

##### SasukeFan 08/29/2019 14:39:34
actually i m getting my data from kinect and using that data i m getting my joints angle (motor positions)using python . Now i have information of angles of left shoulder pitch and roll, left elbow pitch and roll.Now i want it to be in a file that webots can read.so how should i do that?Continuosly i will be getting data from kinect which will be given to webots .So how should i continuosly convert it to motion files?

##### Fabien Rohrer [Moderator] 08/29/2019 14:42:26
Using the motion mechanism is certainly overkilled. Why not simply and directly actuating the simulated robot motors? (cf. Motor.setPosition(double pos))

##### SasukeFan 08/29/2019 14:43:18
sorry i can't get u

##### Fabien Rohrer [Moderator] 08/29/2019 14:45:51
The Webots motion file mechanism aims to play back a stored motion in simulation. You would like to map the Kinect and the Webots simulated robot in real-time, right? In this case, it seems more appropriated to not use the Webots motion file mechanism, but rather to directly move the simulated robot actuators.

##### SasukeFan 08/29/2019 14:48:48
okay thank you

##### ClLebi 08/29/2019 15:21:59
Hi, 

Do you guys have a github where I can find an example of custom ROS controller written in C++ ? I've red in the Guide that it's possible ("It is possible to implement such a ROS node in C++ using the "roscpp" library on Linux and macOS. However, in this case, you need to setup a build configuration to handle both the "catkin\_make" from ROS and the "Makefile" from Webots to have the resulting binary linked both against the Webots "libController" and the "roscpp" library.") and I wonder if there is an example ? Thanks a lot ! I discovered Webots yersterday and I love it !

##### David Mansolino [Cyberbotics] 08/29/2019 15:37:42
Hi `@ClLebi`


We are recently doing a lots of progress on this topic. On our main github repo you can find the source of the default ros controller: [https://github.com/omichel/webots/tree/revision/projects/default/controllers/ros](https://github.com/omichel/webots/tree/revision/projects/default/controllers/ros)

This default controller can then be used as a base to create your own C++ ROS controller, here is an example for a car interface for example: [https://github.com/omichel/webots/tree/revision/projects/vehicles/controllers/ros\_automobile](https://github.com/omichel/webots/tree/revision/projects/vehicles/controllers/ros_automobile)

Another possibility is to use the python API, we have a very simple example of this here: [https://github.com/omichel/webots/tree/revision/projects/languages/ros/controllers/ros\_python](https://github.com/omichel/webots/tree/revision/projects/languages/ros/controllers/ros_python)

##### ClLebi 08/29/2019 15:41:29
Thank you ! I tested with the Python but I'm working with LiDAR and it's too slow for me !


I'm gonna look on your links

##### David Mansolino [Cyberbotics] 08/29/2019 15:42:25
If you are interested in ROS2, we are currently creating an interface with ROS2, this interface will be much simpler to integrate and to use, here is the repository (this is still work in progress but is starting to be functional): [https://github.com/cyberbotics/webots\_ros2](https://github.com/cyberbotics/webots_ros2)


finnally, you might also be interested by this repo that gather a few example of ROS nodes that use the default ROS controller to communicate with Webots: 

[https://github.com/cyberbotics/webots\_ros](https://github.com/cyberbotics/webots_ros)

This package is released with ros and is documented here: [http://wiki.ros.org/webots\_ros](http://wiki.ros.org/webots_ros)


> Thank you ! I tested with the Python but I'm working with LiDAR and it's too slow for me !



In that case C++ is indeed most probably the way to go. But make sure before that your GPU is powerful enough and your GPU drivers are up to date (this can drastically influence the simulation speed when using lidars).

##### ClLebi 08/29/2019 15:44:48
Yes I'm using it but I'm trying to have a ROS C++ controller for my robot to control topics, datatypes (like using PointCloud2 instead of PointCloud for LiDAR)

##### David Mansolino [Cyberbotics] 08/29/2019 15:45:28
Ok then creating your custom ros controller seems indeed the way to go.

##### ClLebi 08/29/2019 15:59:43
Based on the ros\_automobile example I successfully compiled my ros controller ! Thank you David !

##### David Mansolino [Cyberbotics] 08/29/2019 16:03:44
Very good news! You're welcome !

##### ClLebi 08/29/2019 17:06:48
Another question, I added the "--synchronize --clock --use-sim-time" args on  controllerArgs but still no /clock published. Is there any line to add in my controller to take this controllerArgs into account ?


(Of course the use\_sim\_time rosparam is set to true)

##### David Mansolino [Cyberbotics] 08/30/2019 06:30:25
`@ClLebi`, no you should not need to change anything to your controller. The `launchRos` function will parse the arguments and publish the clock if needed: [https://github.com/omichel/webots/blob/revision/projects/default/controllers/ros/Ros.cpp#L132](https://github.com/omichel/webots/blob/revision/projects/default/controllers/ros/Ros.cpp#L132)

Can you please check if you have the same problem with the default ros controller?

##### ClLebi 08/30/2019 09:05:52
I didn't use launchRos in my controller, I'll test it

##### David Mansolino [Cyberbotics] 08/30/2019 10:20:34
`@ClLebi` if you call the ``ROS::run`` function the ``launchRos`` function is called automatically.

## September

##### Frodo 09/02/2019 06:32:51
Do you guys know why I cant change the robot from the example.wbt from the python folder to an e-puck robot. When I do that it just tells me "INFO: slave: Starting controller: python.exe -u "slave.py" 

WARNING: slave: The process crashed some time after starting successfully.

WARNING: 'slave' controller crashed.

##### Fabien Rohrer [Moderator] 09/02/2019 06:33:19
Hi


The controller may have some code relative to the robot, typically the actuator and sensor names.


This may be the reason of the crash.


You should check the actuator and sensor names match with your new robot.

##### Frodo 09/02/2019 06:35:58
Okay Thank you very much, do you know where can I find the names and functions of the e-puck. I dont know why but I'm having a really hard time doing this simulation. I barely can find about the e-puck in webots

##### Fabien Rohrer [Moderator] 09/02/2019 06:37:36
In the user guide, the sensors and actuators of the robots are listed into the 3D component on top of the page:


[https://cyberbotics.com/doc/guide/epuck](https://cyberbotics.com/doc/guide/epuck)

##### Frodo 09/02/2019 06:41:00
Now I get it, distance sensors from the old robot were "ds" and now they are "ps"


One last question... is there a way to know what the self.Mode.something functions are available for the e-puck?

##### Fabien Rohrer [Moderator] 09/02/2019 06:49:20
Do you speak about the wb\_robot\_get\_mode functions? [https://cyberbotics.com/doc/reference/robot#wb\_robot\_get\_mode](https://cyberbotics.com/doc/reference/robot#wb_robot_get_mode)

##### Frodo 09/02/2019 06:54:19
Actually I was wondering about these modes
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/617975577684738049/unknown.png)
%end


Because they doesn't appear to be programed in other part of the .py or the driver. But they move the robot for some reason and the code doesn't crash

##### Fabien Rohrer [Moderator] 09/02/2019 07:06:17
ok I see


this mode is simply a variable defining the robot behavior


It's the state of the finite state automaton:


[https://en.wikipedia.org/wiki/Finite-state\_machine](https://en.wikipedia.org/wiki/Finite-state_machine)

##### jhielson 09/02/2019 14:54:05
Hi, I am a new user on Webots and I am trying to figure out how can I access the distance sensor data through the supervisor node. I got the position and rotation of the robot but I do not know about the sensors.

##### Fabien Rohrer [Moderator] 09/02/2019 14:54:24
Hi


Programmatically speaking, the Supervisor is a subclass of the Robot class, therefore, what is true in a robot controller is also true in a supervisor controller.


Therefore, you can access the Supervisor devices using the wb\_robot\_get\_device() functions as usual.


cf. [https://cyberbotics.com/doc/reference/robot#wb\_robot\_get\_device](https://cyberbotics.com/doc/reference/robot#wb_robot_get_device)

##### jhielson 09/02/2019 14:57:08
Nice, I was trying to use some supervisor function instead.  Thanks.

##### Fabien Rohrer [Moderator] 09/02/2019 14:57:51
Note that you cannot get the devices of another robot. The Robot/Supervisor API are bound to get only the devices of the current robot.

##### jhielson 09/02/2019 15:03:50
Hi Fabien Rohrer, when I use this function I get an error. Warning: "ps0" device not found.

[rnnGAsupervisor\_Jh] Error: wb\_distance\_sensor\_enable(): invalid device tag.


It is not recognizing the device.


I am using only one e-puck

##### Fabien Rohrer [Moderator] 09/02/2019 15:06:10
"ps0" is indeed a distance sensor name of the E-puck.proto model. This seems to be correct.


Are you sure you defined well the controller field?


E-puck.controller => your controller

##### jhielson 09/02/2019 15:06:58
On controller node it works but on supervisor node it does not

##### Fabien Rohrer [Moderator] 09/02/2019 15:07:52
you don't need to create a Supervisor node, but you need to enable the supervisor flag on your e-puck node:


E-puck [

  controller "e-puck\_avoid\_obstacles" # This should point your supervisor controller.

  supervisor TRUE

]

##### jhielson 09/02/2019 15:08:46
I have a supervisor node to run my Genetic Algorithm code.


It is seperated from my robot controller

##### Fabien Rohrer [Moderator] 09/02/2019 15:10:33
Ok I see. In this case this will be annoying because you cannot access directly the devices of a robot controller through an external supervisor.


You can think to:


1. Merge your supervisor to your robot controller.


2. Or communicate between the robot controller and the supervisor.

##### jhielson 09/02/2019 15:12:26
I see. I will try to use multiple channels via emitter and receivers.

##### Fabien Rohrer [Moderator] 09/02/2019 15:12:54
Yes, using the Emitter-Receiver is perfect for this.

##### jhielson 09/02/2019 15:13:14
Thanks Fabien!

##### Fabien Rohrer [Moderator] 09/02/2019 15:13:29
you're welcome, good luck with your project 😉

##### jhielson 09/02/2019 15:13:40
Thanks

##### SamSmurfitt 09/02/2019 15:37:50
Hi all. Quick question. What would be the best way to go about making a robot similar to Yamor? I.e module links with 1 d.o.f rotating connection between them? 

I'm trying to simulate a snake-like robot similar to yamor but with the joint axis being vertical on each module, not horizontal, and having motorised wheels on each module to move it forwards

##### Fabien Rohrer [Moderator] 09/02/2019 15:38:25
Hi Sam


I would recommend you to do our tutorial first to learn how to create simple robots and to be able to move the joints wherever you want:


[https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials)


Then to add the Connector at the right position, using the Yamor as an inspiration.


.. and finally / eventually encapsulate your model in a .proto file in order to be able to instanciate it easily.


But basically, you could also try to "hack" the yamor model to change the joint axis and add wheels. Learning this is the focus of the first tutorials.


I hope this helps 😉

##### SamSmurfitt 09/02/2019 15:46:39
Hi Fabien, I've just finished doing the tutorials this afternoon. Is it possible to get hold of the Yamor files? I've opened up the .wbt file and all it shows is that it is made of multiple modules. I was hoping to find information on how to build one of those modules.


Actually... ignore that I just found the .proto file

##### Fabien Rohrer [Moderator] 09/02/2019 15:48:07
😄


FYI there is also a context menu when right clicking in the scene tree view called "Convert to Base Node(s)". This allows to explode the PROTO into basic nodes
%figure
![Capture_decran_2019-09-02_a_17.49.48.png](https://cdn.discordapp.com/attachments/565154703139405824/618110550945300481/Capture_decran_2019-09-02_a_17.49.48.png)
%end


... this is also certainly a good starting point for your project.

##### SamSmurfitt 09/02/2019 15:52:09
brilliant thank you


for making more complex structures am I better off importing CAD models?

##### Fabien Rohrer [Moderator] 09/02/2019 15:53:12
For sure this is for level 3 users 😉


You should take a look at our Blender addon: [https://github.com/cyberbotics/blender-webots-exporter](https://github.com/cyberbotics/blender-webots-exporter)

##### SamSmurfitt 09/02/2019 15:54:25
is there a solidworks add on, or will I have to convert between the two via blender?

##### Fabien Rohrer [Moderator] 09/02/2019 15:54:33
You can import your CAD model into Blender, modify it a bit, and use this addon to export to Webots.


Unfortunatly we don't have yet any SolidWorks exporter plugin.

##### SamSmurfitt 09/02/2019 15:58:20
ok thanks again.. will probably be a while until I get around to that anyway!

##### Fabien Rohrer [Moderator] 09/02/2019 15:59:38
you're welcome! don't hesitate to ask question here 🙂

##### SamSmurfitt 09/03/2019 15:04:35
Hi, me again.

In order to get a connector to lock do I have to write it in the controller code, or is there a way to lock two connectors just in the tree?

##### Fabien Rohrer [Moderator] 09/03/2019 15:05:06
Both are possible


depending on the Connector fields, you can achieve these both results.


You should start with the second one (which is simpler to setup and to debug).

##### SamSmurfitt 09/03/2019 15:07:04
I have this set up at the moment, with both connectors touching, and isLocked set to true
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/618461971871694848/unknown.png)
%end

##### Fabien Rohrer [Moderator] 09/03/2019 15:08:46
Visually, it seems good.


A very important point with the connectors is that the local z-axis of the 2 connecting Connectors should point each other, as depicted in this image. And y-axis should match at best.


[https://cyberbotics.com/doc/reference/connector#connector-axis-system](https://cyberbotics.com/doc/reference/connector#connector-axis-system)



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/618462674077876235/unknown.png)
%end


Not sure at 100% but it seems your connector is not well aligned (red = x axis towards)

##### SamSmurfitt 09/03/2019 15:12:53
ah that will be it, thanks


I've aligned both the z-axes, and relocked everything but it's still no connecting, is there something else I could be missing?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/618466302000103434/unknown.png)
%end



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/618466325240610816/unknown.png)
%end

##### Fabien Rohrer [Moderator] 09/03/2019 15:39:36
You would like to snap first without controller right?


The alignment seems right now

##### SamSmurfitt 09/03/2019 15:41:59
Yeah ideally without using the controller, although I can do if I have to

##### Fabien Rohrer [Moderator] 09/03/2019 15:45:06
Simple set the type to « passive » and autoLock to TRUE should do the job


But to test I recommend you to exagerate the thresholds:


distanceTolerance 0.05

numberOfRotations 0


Better results?


For sure, the direct parent of the connector should have physics and boundingObject defined.

##### SamSmurfitt 09/03/2019 16:58:00
still doesnt seem to want to work. I will rebuild the model completely tomorrow

##### Fabien Rohrer [Moderator] 09/03/2019 18:02:03
You’re welcome to send it here!

##### Frodo 09/03/2019 18:02:42
Hello! I'm advancing a lot with my algorithm now. But I'm having trouble using the emitter/receiver with python. I've already set different channels for the receivers for every slave. Now I need to send a vector with wheel velocities to every robot [L,R]. I found the documentation but there is no example on how to do this on python.

##### Fabien Rohrer [Moderator] 09/03/2019 18:04:48
Please look at the WEBOTS\_HOME/projects/language/python/worlds/example.wbt


In this example the supervisor sends data to the slave controllers

##### Frodo 09/03/2019 18:07:35
Thank you! I'm actually using that example to understand how it works. In the example they send the same for every robot and they are sending strings as I understand.

##### Fabien Rohrer [Moderator] 09/03/2019 18:08:34
You can send more complex data using the Python struct module as explained here : [https://www.cyberbotics.com/doc/reference/emitter?tab=python](https://www.cyberbotics.com/doc/reference/emitter?tab=python)


import struct

\#...

message = struct.pack("chd","a",45,120.08)

emitter.send(message)


And to extract:


import struct

\#...

message=receiver.getData()

dataList=struct.unpack("chd",message)

##### SamSmurfitt 09/03/2019 18:11:28
Here you are. Thanks for taking the time.
> **Attachment**: [Module\_1.wbt](https://cdn.discordapp.com/attachments/565154703139405824/618508375558324234/Module_1.wbt)

##### Fabien Rohrer [Moderator] 09/03/2019 18:11:28
Using the struct module, you can stringify any data and extract it


`@SamSmurfitt` i’m not in front of my computer, I’ll take a look at this tomorrow morning if you don’t mind

##### SamSmurfitt 09/03/2019 18:13:32
thats comepletely fine!

##### Frodo 09/03/2019 18:15:54
I think the setChannel is working but the data has some problems
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/618509491868270602/unknown.png)
%end


btw you guys are amazing, this discord channel is helping lots of lost people like me 😅

##### aysegulucar 09/03/2019 19:32:22
Hi


Webots remote control is too slow. Why? Do you have an idea?


We  previosuly had connected  to robot by remote control in Webots. previosuly, It was fast after we run firmawere instal/update in robotis-op2..

Later we changed our motors and we updated firmwares by using dynamixel 2.alsıo. Now webots remot contr is too slow


WE are using robotis-op2


and visual traking .wbt.


Is the firmware update  a problem?

##### Fabien Rohrer [Moderator] 09/04/2019 06:20:31
`@Frodo` The idea is to use the struct module to pack the data to string:


>>> import struct



\# Emitter side



\# dummy values to test

phiR = 1.0

phiL = 2.1

data = struct.pack('ff', phiR, phiL)  # pack the 2 floats to a byte array.

self.emitter.send()



\# Receiver side

data = struct.unpack('ff', receiver.getData())

print(data)

\# > (1.0, 2.1)

##### jacqueline 09/04/2019 06:34:36
Hi, I just starting to use webots 2019


I have a issue on floor arena, how do I add wall tile?

##### Fabien Rohrer [Moderator] 09/04/2019 06:36:54
`@jacqueline` Hi, do you mean to change the ground texture, or to add solid walls?


`@aysegulucar` David answered you through support tickets.


`@SamSmurfitt` Here is a working solution with active connectors. I simply copied the yamor values inside your robot.



> **Attachment**: [Module\_1-2.wbt](https://cdn.discordapp.com/attachments/565154703139405824/618699935688294401/Module_1-2.wbt)



> **Attachment**: [connector\_unlock.py](https://cdn.discordapp.com/attachments/565154703139405824/618700002444705802/connector_unlock.py)


The controller breaks the connection after 2 seconds.



> **Attachment**: [worlds.diff](https://cdn.discordapp.com/attachments/565154703139405824/618700902898860043/worlds.diff)

##### jacqueline 09/04/2019 07:21:15
Sorry I went away just now to do some teaching. In rectangle arena, 2018 version has wall tile feature but not 2019. I just wonder if I can get the same. Noted thanks.

##### Fabien Rohrer [Moderator] 09/04/2019 07:23:21
We changed a lot our rendering engine between R2018 and R2019 (we added HDR backgrounds, PBR appearances, and used them intensively) causing few minor modifications in the Assets, like the RectangleArena.


You should take a look at the RectangleArena.floorAppearance field and the RectangleArena.floorTileSize fields.


If you have old simulations, you may fix them.

##### jacqueline 09/04/2019 07:25:10
I see. In my school, we are still using 2018 till recently, I am preparing the update and feedback to the lecturer on the need to edit his teaching materials. May I know if there is more extensive documents to refer to?

##### Fabien Rohrer [Moderator] 09/04/2019 07:26:29
A typical rectangle arena looks like this now:



%figure
![Capture_decran_2019-09-04_a_09.25.43.png](https://cdn.discordapp.com/attachments/565154703139405824/618708515531390987/Capture_decran_2019-09-04_a_09.25.43.png)
%end

##### jacqueline 09/04/2019 07:27:25
I am referring to the wall though

##### Fabien Rohrer [Moderator] 09/04/2019 07:28:25
Ok, then you should fix the RectangleArena.wallApperance.textureTransform


We didn't wrote such kind of document. Normally most of the assets are backward compatible, except few small issues like this one.

##### jacqueline 09/04/2019 07:29:46
our lab assignment for student
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/618709274591363072/unknown.png)
%end

##### Fabien Rohrer [Moderator] 09/04/2019 07:30:01
You're welcome to report here other compatibility issue, we certainly have simple fixes for eveything.

##### jacqueline 09/04/2019 07:30:29
of course that is based on 2018. Ok noted. We will just modify the assignment

##### Fabien Rohrer [Moderator] 09/04/2019 07:31:52
Yes, checking your worlds are working smoothly in r2019 is unfortunately required, but it should be a simple task, and for good, R2019 is now open-source 🙂

##### jacqueline 09/04/2019 07:32:30
because I tried to edit wall appearance, using scale etc, it does not fit well. I will just feedback to remove the wall. 🙂 thank you for your time

##### Fabien Rohrer [Moderator] 09/04/2019 07:34:35
Good to know too, we have now a set of good-looking predefined appearances using the PBR technology: [https://www.cyberbotics.com/doc/guide/appearances](https://www.cyberbotics.com/doc/guide/appearances)


these appearances can be set directly in the RectangleArena.wall/floorAppearance


They are even more impressive if a TexturedBackground is present in the scene.

##### jacqueline 09/04/2019 07:37:03
Thank you for the info.

##### Owen 09/04/2019 09:18:04
Hi,  I  utilize webots2019b to simulate hand grsp task. When the hand is grasping a cylinder, the bounding object looks strange. There are some parts of  hand overlapping with the cylinder.  It's the screenshot I described. The red circle part is overlapping area.
%figure
![KK.JPG](https://cdn.discordapp.com/attachments/565154703139405824/618736532156579841/KK.JPG)
%end

##### Fabien Rohrer [Moderator] 09/04/2019 09:29:20
Hi Owen


The parameter which affects the most this kind of penetration is certainly WorldInfo.basicTimeStep


Ideal value would be 8 (or 4?)

##### Owen 09/04/2019 09:38:01
Awesome, it really work. The overlapping problem is improved a lot. Thanks.

##### Fabien Rohrer [Moderator] 09/04/2019 09:42:22
To go deeper; this value is directly responsible of the physics engine accuracy. the smaller it is, the more physics steps there is per seconds. As our physics engine works with discrete steps, some penetration is required to be able to detect a collision. In any case, a penetration exists in such physics engine. It can only be reduced.

##### ClLebi 09/04/2019 09:45:30
Hi guys, I'm wondering if it's possible to change the world frame to have Z in up direction ? In my point of view it's weird to have Y up, and we make some mistakes using IMU etc ...

##### Fabien Rohrer [Moderator] 09/04/2019 09:46:46
Hi Cleni, this direction is quite arbitrary in fact.


You can setup a world with up along the z axis.


All our assets support to be rotated.


One thing to know is that the the WorldInfo.gravity vector should be changed accordingly.


Several computation (like the main viewpoint movements) are computed thanks to this vector.

##### ClLebi 09/04/2019 09:48:52
Nice ! I didn't know we have only this parameter to change !

##### Fabien Rohrer [Moderator] 09/04/2019 09:49:55
We choose y-axis up because it's in the VRML and X3D specification. So the geometric primitives are working directly (without transformation). Otherwise, we have to rotate all the plans and elevation grids for example.

##### ClLebi 09/04/2019 09:59:10
I got it !

##### SamSmurfitt 09/04/2019 14:00:45
Hi again, If I'm writing a controller in Matlab code is it possible to get the code to call another function?

##### David Mansolino [Cyberbotics] 09/04/2019 14:50:31
Hi, what do you mean exactly by 'the code to call another function' ?

##### SamSmurfitt 09/04/2019 14:54:29
so If I write a function in normal Matlab is it possible to have a robot's controller use it inside Webots?

##### David Mansolino [Cyberbotics] 09/04/2019 15:01:30
If you write your controller in matlab yes it should be possible as the controller will be interpreted by Matlab.

##### Fabien Rohrer [Moderator] 09/04/2019 15:02:10
`@SamSmurfitt` Did you noticed the world I sent you this morning?

##### SamSmurfitt 09/04/2019 15:11:25
So in this example, After 5 seconds the motor is set to a position (Joint\_Angle) but I would like that to be calculated using a function, 'ang\_cal' from a certain input
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/618825453540737054/unknown.png)
%end


I guess I'm really trying to make a subroutine in matlab really


`@Fabien Rohrer`  Yes thanks! managed to get the connectors working


Although I cant get Python working on this computer, and I dont really know it anyway - hence why im trying to use matlab for everything as thats what I have the most experience with

##### David Mansolino [Cyberbotics] 09/04/2019 15:14:44
Not sure exactly where you should put the file, but probably in the same folder than the controller.

You probably want to add the `desktop;` line at the beginning of your controller to use the desktop version of matlab for debugging.

##### SamSmurfitt 09/04/2019 15:30:02
ok thanks, looks like Ive got it working

##### David Mansolino [Cyberbotics] 09/04/2019 15:32:50
Perfect 🙂

##### Clement 09/04/2019 16:02:11
Hi guys, is there a way to constrain a robot to follow a curved rail and command its position along it ?


the cube would be the robot
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/618838480859955231/unknown.png)
%end

##### Fabien Rohrer [Moderator] 09/04/2019 16:05:31
Hi Clement


Do you need physics on the cube?


i.e. Could a kinematic motion do the job?

##### Clement 09/04/2019 16:08:19
No,  the cube would just be a support for a robotic arm


what do you mean be a kinematic motion ?

##### Fabien Rohrer [Moderator] 09/04/2019 16:09:54
In Webots we usually differentiate kinematics vs physics motions, respecively without running the physics engine and  forcing the position vs using the physics engine to compute the positions.


You may achieve results by removing physics from the solid and by moving the box using a Supervisor.

##### Clement 09/04/2019 16:22:11
ok I understand. And using the physics engine I'll have to represent all the mecanism of the moving part ?

##### Fabien Rohrer [Moderator] 09/04/2019 16:24:56
Yes exactly.


Maybe I have a better solution to propose you:


you could create a robot having an physics arm mounted on a vertical kinematics motorized slider mounted on an horizontal kinematics motorized slider.


then, you can move the 2 sliders in order to match your path, this will move the arm accordingly.


Could this match your expectations?

##### Clement 09/04/2019 16:31:24
you're speaking about SliderJoint ?


or maybe a linearmotor, yes it could do it

##### Fabien Rohrer [Moderator] 09/04/2019 17:37:49
Yes, a slider joint actuated by a linear motor

##### maitham 09/04/2019 18:02:27
Hi

##### Fabien Rohrer [Moderator] 09/04/2019 18:18:51
`@maitham` hi! Welcome here!

##### maitham 09/04/2019 19:25:25
thank you Fabien


Hi. I have problem to install Webots on my laptop,


I need help,please.. thank you in advance

##### Fabien Rohrer [Moderator] 09/05/2019 05:39:41
`@maitham` hi! Basically you need to follow these instructions: [https://cyberbotics.com/doc/guide/installation-procedure](https://cyberbotics.com/doc/guide/installation-procedure)


Do you have specific errors/issues (any error message? Screenshots?)

##### maitham 09/05/2019 05:41:25
Good Morning Fabien, Thank you , could I txt you the error message please? I post it here. thanks

##### Fabien Rohrer [Moderator] 09/05/2019 05:41:45
Yes, here is fine!

##### maitham 09/05/2019 08:23:26
Hi Fabien. the message is I dont have Python installed on my laptop and  the message I get now is : The application was unable to start correctly (0xc000007b). click OK to close the application


thank you

##### Fabien Rohrer [Moderator] 09/05/2019 08:25:25
Do you try to install Webots from the installer (link above)? Or do you try to build Webots?


What crashes? Webots or the Python controller?

##### maitham 09/05/2019 08:27:06
i tried to install it from the website

##### Fabien Rohrer [Moderator] 09/05/2019 08:30:19
Do you see the Webots window at some point?

##### maitham 09/05/2019 08:35:02
yes I saw the window last night but could not do anything


thank you very much for your time


now installing from the link you posted here

##### Fabien Rohrer [Moderator] 09/05/2019 08:35:59
Ok. You should try:


1. Check your GPU drivers are up-to-date


2. Run Webots in safe mode following these instructions: [https://cyberbotics.com/doc/guide/starting-webots#safe-mode](https://cyberbotics.com/doc/guide/starting-webots#safe-mode)


Note that Python is not a requirement to run Webots. You can for example run all the guided tour without Python installed.


.. and check your system is above the minimal requirements [https://cyberbotics.com/doc/guide/system-requirements](https://cyberbotics.com/doc/guide/system-requirements) 😉

##### alinux 09/05/2019 16:28:00
Hi! How can I get the absolute position of a pen?

##### David Mansolino [Cyberbotics] 09/06/2019 06:15:55
Hi `@alinux` , do you want to get the position from the GUI or from your controller?

##### fateme 09/06/2019 07:11:10
hello


how can i gather data with keyboard in e-puck environment

##### David Mansolino [Cyberbotics] 09/06/2019 07:24:42
Hi, you can get keyboard input from the controller using the keyboard API: [https://www.cyberbotics.com/doc/reference/keyboard](https://www.cyberbotics.com/doc/reference/keyboard)

##### maitham 09/06/2019 07:25:20
I am here now and please what to do ?
> **Attachment**: [webots.docx](https://cdn.discordapp.com/attachments/565154703139405824/619432936915206154/webots.docx)


sorry very new here

##### David Mansolino [Cyberbotics] 09/06/2019 07:26:28
It depends, what are you trying to do ?

##### fateme 09/06/2019 07:27:10
thank you David

##### David Mansolino [Cyberbotics] 09/06/2019 07:27:30
`@maitham`, if you are new to Webots, you should probably try our tutorial first: [https://www.cyberbotics.com/doc/guide/tutorials](https://www.cyberbotics.com/doc/guide/tutorials)


`@fateme`, you're welcome

##### maitham 09/06/2019 07:28:16
Morning David. thank you very for your answer

##### David Mansolino [Cyberbotics] 09/06/2019 07:28:58
Morning Maitham. You're welcome.

##### maitham 09/06/2019 07:29:18
How I can run a sample world?

##### David Mansolino [Cyberbotics] 09/06/2019 07:34:50
You can run the guided tour from the 'Help => Webots Gudied Tour...' menu

##### maitham 09/06/2019 07:35:21
thank  you David

##### David Mansolino [Cyberbotics] 09/06/2019 07:39:12
You're welcome

##### Derek 09/06/2019 09:41:16
Hello


Does webots support the close chain?

##### Olivier Michel [Cyberbotics] 09/06/2019 09:45:09
Hi Derek,


Yes it does.


I assume you mean mechanical loops?


It is achieved with a SolidReference node: [https://cyberbotics.com/doc/reference/solidreference](https://cyberbotics.com/doc/reference/solidreference)

##### Derek 09/06/2019 10:04:53
Yes,I mean mechanical loops, parallelogram mechanism


Its very convenient to create or compound solids in v-rep. Can I drag some solid using mouse to scence and merge them in webots?

##### Olivier Michel [Cyberbotics] 09/06/2019 10:10:29
Here is an example of mechanical loop: [https://cyberbotics.com/doc/guide/samples-howto#pedal\_racer-wbt](https://cyberbotics.com/doc/guide/samples-howto#pedal_racer-wbt)


Merging solids in Webots can be achieved by copy/paste nodes inside the children list of the parent solid node.

##### alinux 09/06/2019 11:04:14
`@David Mansolino` from the GUI.... I want to calculate the error between the target position and the pen position, but the absolute position given by GUI for the pen isn't the same when I use the getPosition function

##### David Mansolino [Cyberbotics] 09/06/2019 12:05:40
`@alinux` , you can get the position of any node by selecting it and checking it's position  in the 'Position' tab of the field editor ([https://www.cyberbotics.com/doc/guide/the-scene-tree#field-editor](https://www.cyberbotics.com/doc/guide/the-scene-tree#field-editor)).

##### alinux 09/06/2019 12:18:27
`@David Mansolino`  but how I get that position by code?

##### David Mansolino [Cyberbotics] 09/06/2019 12:20:12
You told me you wanted it from the GUI...

From the code you should either use a GPS node attached to the PEN: [https://cyberbotics.com/doc/reference/gps](https://cyberbotics.com/doc/reference/gps)

Either use the Supervisor APi which allows you to get the position of any node in the scene: [https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_get\_position](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_get_position)

##### alinux 09/06/2019 12:30:41
I was trying to get with the supervisor, but the position it gives me is not the same as in the GUI

##### David Mansolino [Cyberbotics] 09/06/2019 12:32:09
In the GUI you can select in which reference (locally, globally, etc) you want the position. With the supervisor it also depends if you get the translation field or the node absolute position, this can explain the difference if the references are not the same.

##### alinux 09/06/2019 12:44:44
And how can I get the absolute position with the supervisor?

##### David Mansolino [Cyberbotics] 09/06/2019 12:48:36
This function will give you the absolute position: [https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_get\_position](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_get_position)

##### alinux 09/06/2019 13:28:14
This p of this equation is given by what? I think that is my error

##### David Mansolino [Cyberbotics] 09/06/2019 13:30:13
> Where p is a point whose coordinates are given with respect to the local coordinate system of a node



this is only if you want to compute the location of a point for which you know the relative position compared to your node

##### alinux 09/06/2019 13:37:52
That is what I'm getting.... I'm sorry, I'm new user of webots so I'm still lost 😳
%figure
![webots.png](https://cdn.discordapp.com/attachments/565154703139405824/619526687457017897/webots.png)
%end

##### David Mansolino [Cyberbotics] 09/06/2019 13:39:21
No problem, are you selecting the same node in the scene tree as the one you get with the supervisor ?

##### alinux 09/06/2019 13:40:51
Yes

##### David Mansolino [Cyberbotics] 09/06/2019 13:43:34
ok, then this is strange, would you agree to send us your simulation so that we can quickly check ?

##### alinux 09/06/2019 13:45:02
I'm using the inversekinematic world, just modifying the text file

##### David Mansolino [Cyberbotics] 09/06/2019 13:46:07
Ok, and which is the node you select ?

##### alinux 09/06/2019 13:50:33
Here is the simulation archives
> **Attachment**: [IK.zip](https://cdn.discordapp.com/attachments/565154703139405824/619529879960158219/IK.zip)

##### David Mansolino [Cyberbotics] 09/06/2019 13:51:34
Ok, let me try


Ok, I checked and I get the same result from the controller and from, the gui, just a few remarks:

  - Make sure to select the 'DEF PEN Pen' node in the three.

  - In the 'Position' tab make sure to select 'Absolute'

  - Your controller doesn't have any loop, so it run one step and then quit (and therfore only print the initial position).


- It can be confusing, but the position returned by the supervisor function is one step late compared to the one of the gui (it make sens because the simulation move one step between your controller step and then when you read the value in the gui).

  - I also see in the world file that you did run the simulation and then saved it. This is not a good practice (which can lead to instabilities), you should try to make sure that when you save your simulation the time is always 0.00.


Let me know if something is not clear or if you think the result still doesn't make sense

##### alinux 09/06/2019 14:16:14
I completely forgot about the loop... now it works


Thank you!

##### David Mansolino [Cyberbotics] 09/06/2019 14:17:28
You're welcome

##### daapa 09/06/2019 17:19:52
Hi guys, I can create a clockwise propeller, but I can't create counter-clockwise propeller. How do I do?

##### aknuclear 09/07/2019 06:08:03
How to resolve this problem "WARNING: ros: The process crashed some time after starting successfully.

WARNING: 'ros' controller crashed.""

##### fateme 09/07/2019 06:33:09
hello. I want to know that  is there any ready sample for collecting data of epuck 's sensors in webots


?

##### ShalomShalom 09/07/2019 23:12:35
Hey guys 🙂 on the documentation here [https://cyberbotics.com/doc/guide/robots?tab=c](https://cyberbotics.com/doc/guide/robots?tab=c) it says user can also add their models. Does it mean you could use some 3d modelling software to build a model and import it into Webots? Thanks 🙂

##### MariusJuston [Moderator] 09/08/2019 00:38:56
`@ShalomShalom` I made a video of how to do it [https://youtu.be/L0FVsFD2rS4](https://youtu.be/L0FVsFD2rS4) or else you can try to use the blender adding created to export to Webots

##### ShalomShalom 09/08/2019 03:57:02
Cool thanks alot!!


so you could make a CAD model which specifies joints in autodesk Inventor? 🙂  Which will be automatically recognized by webot?

##### Edit 09/08/2019 06:31:56
hi y'all—how would i go about interfacing an algorithm that modifies the structure of a robot?

##### aknuclear 09/08/2019 10:47:21
Where could I found the controller directory and runtime.ini file at webots in ubuntu?

##### MariusJuston [Moderator] 09/08/2019 18:35:53
`@ShalomShalom` as far as I know no

##### David Mansolino [Cyberbotics] 09/09/2019 06:04:10
`@aknuclear` , on which OS are you ?


`@fateme` ,  unfortunately not, but it should be quite easy to modify one of the existing controller to save the sensor value in a file.


`@ShalomShalom` , unfortunately `@MariusJuston` is right, from autodesk Inventor, only the shapes will be exported, you will have to re-specify the joints in Webots, joints are only exported from blender when using our exporter: [https://github.com/cyberbotics/blender-webots-exporter](https://github.com/cyberbotics/blender-webots-exporter)


`@daapa`, yes this is possible, to do that you have to inverse the 'shaftAxis' (e.g. put `0 -1 0`), you will then have to inverse the 'thrustConstant' too otherwise the thrust will be in the wrong direction.


`@Edit`, what you are looking for is the Supervisor API: [https://cyberbotics.com/doc/reference/supervisor](https://cyberbotics.com/doc/reference/supervisor)


`@Edit`, or you might also be interested by the connector device: [https://cyberbotics.com/doc/reference/connector](https://cyberbotics.com/doc/reference/connector)


`@aknuclear`, the controller folder is in the project directory close to the world file. You can open the controller in the Webots text editor by right clicking on your robot and selecting 'Edit Controller'. Please find more information about the structure of a Webots project here: [https://www.cyberbotics.com/doc/guide/the-standard-file-hierarchy-of-a-project](https://www.cyberbotics.com/doc/guide/the-standard-file-hierarchy-of-a-project)

##### daapa 09/09/2019 07:05:12
`@David Mansolino` I did it!

Thank you for giving me good advice!

##### David Mansolino [Cyberbotics] 09/09/2019 07:05:33
Good news. Your Welcome !

##### aknuclear 09/09/2019 13:38:04
How to change the python version from 2.7 to 3.7  of the controller in ubuntu?

##### HCC 09/09/2019 13:51:38
Hi, I would like to use Webots in Ubuntu 16.04.  I can see the Guided Tour project like robotis\_op3.wbt...etc. But when l click "Open World" or "Open an exist text file", Webots2019b will be not responding.And terminal will show:



[7892:7892:0909/214506.238222:ERROR:browser\_main\_loop.cc(336)] GLib: g\_once\_init\_leave: assertion 'result != 0' failed

[7892:7892:0909/214506.238248:ERROR:browser\_main\_loop.cc(336)] GLib-GObject: g\_type\_register\_dynamic: assertion 'parent\_type > 0' failed

[7892:7892:0909/214506.238261:ERROR:browser\_main\_loop.cc(336)] GLib-GObject: g\_object\_new: assertion 'G\_TYPE\_IS\_OBJECT (object\_type)' failed



How can I solve this problem?Thank for your advice!

##### Fabien Rohrer [Moderator] 09/09/2019 13:55:37
`@aknuclear` In R2019b, you can simply change the python command in the Webots Preferences.


.. and set "python3.7" instead of "python"


.. in the "General / Python command" preference.


`@HCC` Could you try the safe mode? [https://cyberbotics.com/doc/guide/starting-webots#safe-mode](https://cyberbotics.com/doc/guide/starting-webots#safe-mode)


You should check you are above the minimal requirements to run Webots, and upgrade your GPU drivers. [https://cyberbotics.com/doc/guide/system-requirements](https://cyberbotics.com/doc/guide/system-requirements)

##### SamSmurfitt 09/10/2019 13:44:49
Hi again. I have a question about using Matlab as the controller for multiple robots linked together. For my project, I am making a snake-like robot and currently I am modelling it as multiple robots using connectors to link them together. The problem is I have built a mathematical controller, which calculates the motor speeds and angles for the drive and joint motors, for each module, depending on the speeds and angles of the modules in front of it.



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/620978009263964192/unknown.png)
%end


So, my questions are; is it possible to have global variables, which each Matlab controller can access? So that one module can access the previous module’s calculated speeds.

If not, is my only option to build the system as a single robot with lots of children within children etc?

##### Fabien Rohrer [Moderator] 09/10/2019 13:48:19
Hi. Why not defining your global constants in a single file? The controllers may read it.

##### SamSmurfitt 09/10/2019 13:49:57
Oh so if I have a standard .m file in the same directory as the controller's, the controller's should be able to read and write to it?

##### Fabien Rohrer [Moderator] 09/10/2019 13:50:28
yes.


Personnaly, I would go for a file format which is done especially for storing settings. For example JSON: [https://www.mathworks.com/help/matlab/ref/jsondecode.html](https://www.mathworks.com/help/matlab/ref/jsondecode.html)

##### SamSmurfitt 09/10/2019 13:52:24
Ok thanks, I'll give it a go!

##### Fabien Rohrer [Moderator] 09/10/2019 13:55:03
Good to know too (and a step further), you can use the Robot.controllerArgs to send some specific data to a robot controller. Typically, the path to some JSON config file can be sent this way.

##### fateme 09/11/2019 06:34:34
hello , how can i set e-puck in determined point by code

##### Fabien Rohrer [Moderator] 09/11/2019 06:44:04
`@fateme` Hello. Using the e-puck sensors and actuators only, you should use the robot odometry, as explained in chapter 1 here: [https://en.wikibooks.org/wiki/Cyberbotics%27\_Robot\_Curriculum/Advanced\_Programming\_Exercises#Odometry\_](https://en.wikibooks.org/wiki/Cyberbotics%27_Robot_Curriculum/Advanced_Programming_Exercises#Odometry_)[Advanced]


If you can cheat at some point and get the robot absolute position (by using a GPS or a Supervisor), you can simply move the robot in this direction.


Or maybe, you want to teleport the robot? In this case, you should refer to:


[https://cyberbotics.com/doc/guide/using-numerical-optimization-methods#resetting-the-robot](https://cyberbotics.com/doc/guide/using-numerical-optimization-methods#resetting-the-robot)

##### Clement 09/11/2019 09:35:27
Hi guys, I'm wondering if its possible to deform a mesh during a simulation

##### Fabien Rohrer [Moderator] 09/11/2019 09:36:14
Hi Clement, yes, this is possible. The Supervisor API allows to modify the scene tree in runtime.


this is for example used in this example to draw the trail: [https://cyberbotics.com/doc/guide/samples-howto#supervisor\_draw\_trail-wbt](https://cyberbotics.com/doc/guide/samples-howto#supervisor_draw_trail-wbt)


or here: [https://www.youtube.com/watch?v=OKxdaNqmQco](https://www.youtube.com/watch?v=OKxdaNqmQco)

##### Clement 09/11/2019 09:41:16
Wow cool! Is it possible to do this from the physics plugin ? The idea is to compute the deformation in the physics plugin through an external library and  modify the mesh accordingly.

##### Fabien Rohrer [Moderator] 09/11/2019 09:42:12
the physics plugin has only access to ODE, not the graphical parts.

##### Clement 09/11/2019 09:45:17
Ok I see. I think I've seen somewhere in the doc that the physics plugin can "speak" with a controller through an emiter/receiver. Is that right ?


The physics plugin API provides the dWebotsSend function to send messages to robot controllers and the dWebotsReceive function to receive messages from robot controllers.

##### Fabien Rohrer [Moderator] 09/11/2019 09:47:42
Yes, exactly.


You physics plugin may transmit instructions to a Supervisor.

##### Clement 09/11/2019 09:49:32
Nice, I'll try this. Thanks a lot for your answers !

##### SamSmurfitt 09/11/2019 11:41:13
Hi, Are there any example worlds which use a joystick? Im trying to use a joystick but cant seem to get it to be detected by webots, and wondered if there is an example one I could test it on

##### Fabien Rohrer [Moderator] 09/11/2019 11:41:57
`@SamSmurfitt` Hi, there are examples, let me check..


This one is certainly the reference:


[https://github.com/cyberbotics/webots/blob/revision/projects/vehicles/controllers/racing\_wheel/main.cpp](https://github.com/cyberbotics/webots/blob/revision/projects/vehicles/controllers/racing_wheel/main.cpp)


It interfaces Logitech G27 and G29 with the vehicle library used to drive cars.

##### SamSmurfitt 09/11/2019 11:46:00
ah ok, so could  it be a case that my joystick isnt compatible?

##### Fabien Rohrer [Moderator] 09/11/2019 11:48:06
Webots uses libOIS to manage the joysticks. Normally most of the standard joysticks should be compatible with libOIS.


Is wb\_joystick\_is\_connected() returns true? [https://cyberbotics.com/doc/reference/joystick#wb\_joystick\_is\_connected](https://cyberbotics.com/doc/reference/joystick#wb_joystick_is_connected)


(once enabled)

##### SamSmurfitt 09/11/2019 11:49:58
No it not, I've written a little test code



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/621311533909278732/unknown.png)
%end


and it returns the not connected statement


the joystick does show up in windows device manager as a usb input device

##### Fabien Rohrer [Moderator] 09/11/2019 11:51:48
could you add a wb\_robot\_step() between the enable and the is\_connected calls?


.. and test your joystick here? [https://gamepadviewer.com](https://gamepadviewer.com)

##### SamSmurfitt 09/11/2019 11:56:10
ah there we go


it shows up as connected now


when inside the wb\_robot\_step loop

##### Fabien Rohrer [Moderator] 09/11/2019 11:57:05
great 😄

##### SamSmurfitt 09/11/2019 11:58:47
nice, thank you again!

##### Edit 09/12/2019 04:23:37
hi -- is it possible to remove a joint/motor during simulation?

##### David Mansolino [Cyberbotics] 09/12/2019 06:21:06
Hi `@Edit` , yes you can use the Supervisor API ([https://cyberbotics.com/doc/reference/supervisor](https://cyberbotics.com/doc/reference/supervisor)) to change the structure of your robot during the simulation, however new devices (such motors) requires that you reboot the controller (also feasible with the Supervisor API). Another alternative would be to use connectors: [https://cyberbotics.com/doc/reference/connector](https://cyberbotics.com/doc/reference/connector)

##### Gaarci 09/13/2019 12:51:24
Hey everyone, I'm working with clement and I've been trying to implements the "supervisor approach"  to deform a mesh (like this video) [https://www.youtube.com/watch?v=OKxdaNqmQco](https://www.youtube.com/watch?v=OKxdaNqmQco)


I use "wb\_supervisor\_field\_get\_mf\_vec3f" and "wb\_supervisor\_field\_set\_mf\_vec3f" to modify a shape that is defined  by an IndexFaceSet, with 93 points.


I encounter two mains problem, 1. the simulation is "pretty slow" : Moving 93 points by iteration, the simulation can "only run" at 50% the speed


and secondly, the exact same supervisor coded in C++ runs 10 time slower


How did you get something so smooth ? Also, why do C++ supervisor seems to be so muche slower ?

##### David Mansolino [Cyberbotics] 09/13/2019 13:11:44
Hi Gaarci. Changing the vertices at each step is computationnally expensive on the Webots side and it seems 'normal' that you have a slow simulation speed. But this might also depend on the field you are changing, is it a field of a PROTO? About the C++, it is very strange as the C++ API is just a wrapper for the C API, is it possible to share your simulation/controller so that we can check if there is a way to speed this up ?

##### Gaarci 09/13/2019 13:25:57
Thank you a lot for the response !


No, it's not the field of a proto, just a "basic shape" that I generated thanks to Blender (and converted to a world using the blender's plugin on the cyberbotics website)


Here is the "world"
> **Attachment**: [mesh-deformation.wbt](https://cdn.discordapp.com/attachments/565154703139405824/622060882142167040/mesh-deformation.wbt)


Here is the "C" controller


[https://pastebin.com/wTBZk88e](https://pastebin.com/wTBZk88e)


and the C++ one [https://pastebin.com/52eVZHFY](https://pastebin.com/52eVZHFY)

##### David Mansolino [Cyberbotics] 09/13/2019 13:32:17
Ok thank you, everything seems correct with your world and controllers.

I will try to run them (a bite later in the afternoon) and let you know if a see any possible optimization.

##### Gaarci 09/13/2019 13:33:04
Ok, thank you a lot for the time you took to respond an help me !

##### David Mansolino [Cyberbotics] 09/13/2019 13:34:04
You're welcome


`@Gaarci` , I have made several test and I can explain the difference you get between the C and C++ versions, it is because of the time step, in the C version you define the time step using a #define to be 64. But in the C++ version you use the  world time step (getBasicTimeStep()) which is equal to 8. therefore your C++ controller makes 4 times more steps than the equivalent C version. I have tested to use 64 in both and the simulation speed is very similar.


About optimizing the simulation speed, unfortunately I don't see any simple solution for this, maybe my collegue `@Fabien Rohrer` (who originally did the video you are refeering to) has some idea, but he is out of the office today, I will ask him next week.

##### Gaarci 09/13/2019 15:01:08
Oh, I'm so sorry, it's such a stupid mistake !


Thank you for you're response. I'll be looking forward for next week then 🙂

##### David Mansolino [Cyberbotics] 09/13/2019 15:26:21
> Oh, I'm so sorry, it's such a stupid mistake !



No problem, we all do such kind of mistakes 😉

##### Amrita 09/14/2019 06:21:41
Hello sir, 

I am new here, i am working on humanoid robot NAO,

and i am trying to move the robot like it perform task in choreographe (software to control real nao)



there we can easily give command to move upto 1 m then move left upto 0.5 m then take a turn of 45 degree and reach the target, 

Can we do the same here in C programming?

##### pavlos27t 09/14/2019 13:09:05
Hello everyone , i just downloaded Webots distribution for Windows and i'm having this problem:    When i open a world file(.wbt) from tutorials projects everything is ok , but when i create a new empty project the program terminates of its own after a few seconds. Could you help me to solve this problem?

##### Fabien Rohrer [Moderator] 09/15/2019 11:25:44
@Armita yes it's possible. We developed at some point a bunch of predefined motion files (closed loop) for the Nao which can be played using the Webots motion API:


[https://github.com/cyberbotics/webots/tree/revision/projects/robots/softbank/nao/motions](https://github.com/cyberbotics/webots/tree/revision/projects/robots/softbank/nao/motions)


[https://cyberbotics.com/doc/reference/motion](https://cyberbotics.com/doc/reference/motion)


These motions are certainly not matching exactly 0.5 m and 45° but it's certainly a good starting point to achieve your goal.


`@pavlos27t` This is weird. For sure it's not the expected behavior 😉 We need more information to help you. What is the controller you are trying to build? A sample we release? In which language? C?

##### Amrita 09/15/2019 19:30:43
`@Fabien Rohrer`  yes sir i went through earlier, edited these motion files and obtained various results,

what i am asking is how can i get the robot perform 3 or more tasks one after other.  means first go straight then take turn then go forward like this.



do we have any c program for it?


Or guide me if there is any other way.

Thank you in advance


And how can i download files from above provided link.?

##### Fabien Rohrer [Moderator] 09/16/2019 06:29:57
You could write a C (or other) controller which plays the motion files one after the other. The Motion API allows to do this.


For example, to play three motion consecutively in loop (I didn't test this snippet):


>>> WbMotionRef motions[3] = {

  wbu\_motion\_new("path/to/Forwards.motion"),

  wbu\_motion\_new("path/to/TurnLeft40.motion"),

  wbu\_motion\_new("path/to/HandWave.motion")

};



int counter = 0;

while (true) {

  counter = (counter + 1) % 3;

  wbu\_motion\_play(motions[counter]);

  do {

    wb\_robot\_step(TIME\_STEP);

  }

  while (! wbu\_motion\_is\_over(motion));

}


These motion files are released in Webots, in the WEBOTS\_HOME / projects/robots/softbank/nao/motions/*.motion path.

##### pavlos27t 09/16/2019 08:21:42
i open this file     Webots\projects\samples\robotbenchmark\maze\_runner\worlds\maze\_runner.wbt     and everything works fine,  then i go to Wizards -> New Project Directory  and i create a new project with the default world settings and it terminates of its own after a  few seconds while trying loading the world.

##### Fabien Rohrer [Moderator] 09/16/2019 08:22:46
Ok, it seems Webots crashes for an unknown reason..


Could you try to reduce the OpenGL preferences? (Tools / Preferences / OpenGL)

##### pavlos27t 09/16/2019 08:23:49
I didn't try that

##### Fabien Rohrer [Moderator] 09/16/2019 08:24:45
It would be awesome if you could open a bug report there: [https://github.com/cyberbotics/webots/issues/new?template=bug\_report.md](https://github.com/cyberbotics/webots/issues/new?template=bug_report.md)


.. describing the exact procedure, your hardware, the software/OS versions...

##### pavlos27t 09/16/2019 08:27:17
wow , i  reduced texture quality from high to medium and  it worked


it seems that was the problem


thank you very much

##### Fabien Rohrer [Moderator] 09/16/2019 08:33:25
great.


this may be explained if your GPU is quite short in term of memory.


We are working to reduce the GPU memory usage for Webots R2020a

##### Amrita 09/16/2019 15:14:00
i have used this code but

it is build 

but when i clicked the play button it chrashed

##### Fabien Rohrer [Moderator] 09/16/2019 15:14:23
What does crash? the controller?

##### Amrita 09/16/2019 15:14:23
With following error


[Shoulder\_movement] Error: wbu\_motion\_new(): could not open '../../motions/turn\_left\_60.motion' file.

[Shoulder\_movement] Error: wbu\_motion\_new(): could not open '../../motions/hand\_wave.motion' file.

[Shoulder\_movement] Error: wbu\_motion\_play() called with NULL 'motion' argument.

WARNING: Shoulder\_movement: The process crashed some time after starting successfully.

WARNING: 'Shoulder\_movement' controller crashed.

##### Fabien Rohrer [Moderator] 09/16/2019 15:14:31
ok


The error message is quite clear: the motion path is wrong, the motion cannot be created.


Please check your path: '../../motions/turn\_left\_60.motion'


the motions are searched relatively to the controller path.

##### Amrita 09/16/2019 15:16:13
The privious controller had the same path 

i just copied and pasted it


Shoulder\_movement.c: In function 'main':

Shoulder\_movement.c:69:31: warning: passing argument 1 of 'wbu\_motion\_is\_over' from incompatible pointer type [-Wincompatible-pointer-types]

   69 |   while (! wbu\_motion\_is\_over(motions));

      |                               ^~~~~~~

      |                               |

      |                               struct WbMotionStructPrivate **



And this error was coming during building of program

##### Fabien Rohrer [Moderator] 09/16/2019 15:17:21
the warning refers to something different which should not cause issues.

##### Amrita 09/16/2019 15:17:27
Seperately all motion files are working 



But three at a time is not working

##### Fabien Rohrer [Moderator] 09/16/2019 15:18:28
Please open your controller directory in your OS, and check that the "../../motions" directory exists. I bet it's not the case 😉

##### Amrita 09/16/2019 15:19:40
😂


Can you correct the above code sir?


Sir, do we have sit and stand motion file?

i cannot get it in the directory?

##### Fabien Rohrer [Moderator] 09/16/2019 15:22:04
Unfortunately, the sit and stand motions do not exist.


The code seems very correct, you should only copy the motion directory at the correct location on your computer to let it work.

##### marina\_diamanti 09/16/2019 21:36:21
hello, i am trying to do visual mapping using openCV and webots


and some functions (such as cv::findEssentialMat)  require as input focal length and principal point


what value should i add for focal length and principal point?


🙂

##### Fabien Rohrer [Moderator] 09/17/2019 06:39:10
`@marina_diamanti` Hi. Webots cameras are implemented in OpenGL, and so have the same constraints. They use a basic parallelepipoid frustum. They can be extended with special effects, including a depth of field "shader".


So the principal point matches directly with the Camera.near field.


The focal length is defined optionally by the Focus.focalLength field (to define the depth of field shader).


(parallelepipoid frustum)
%figure
![frustum.jpg](https://cdn.discordapp.com/attachments/565154703139405824/623409227406376960/frustum.jpg)
%end

##### marina\_diamanti 09/17/2019 08:36:17
so if the near field is 0.05 for example principal point is (0.05,0.05) ?


if i have for exaple  e-puck robot how can i define the focal length then? cause there is no focus field in that case


thanks for answering 🙂

##### Fabien Rohrer [Moderator] 09/17/2019 12:15:44
The principal point is at the center of the near plane. In 3D, it's (0, 0, -0.05) (5 centimeters in the direction of the camera axis: -z). This value has not much sense in 2D (width/2, height/2 🙂 ? ).


In the e-puck robot, the Focus node is not defined, so the focal length has not much sense, if I'm not wrong.

##### marina\_diamanti 09/18/2019 13:40:35
thanks its been really helpfull  🙂

##### daapa 09/19/2019 07:16:32
Hi guys.

I wanna make a complex  stream velocity, such as x = sin(t) .

How do I do?

##### Fabien Rohrer [Moderator] 09/19/2019 07:26:10
`@daapa` Hi, are you talking about the Fluid.streamVelocity field? [https://www.cyberbotics.com/doc/reference/fluid](https://www.cyberbotics.com/doc/reference/fluid)

##### daapa 09/19/2019 07:43:33
Yes, it is.

##### Fabien Rohrer [Moderator] 09/19/2019 07:44:26
Ok. You're supposed to use a Supervisor controller. A supervisor can change fields in runtime.


Typically, you could tag your fluid:


DEF MY\_FLUID Fluid {}


and then:


>>> streamVelocityField = supervisor.getFromDef('MY\_FLUID').getField('streamVelocity')

time = supervisor.getTime()

streamVelocityField.setSFVec3f([0.0, 0.0, math.sin(time)])


(In Python 🙂 )

##### daapa 09/19/2019 07:52:04
`@Fabien Rohrer`  Oh, thank you!

I'll try it.

##### ClLebi 09/19/2019 15:03:25
Hi guys ! Has anyone succeed to link new ros msg in controller ? Like nav\_msgs that is not in webots/projects/default/controllers/ros/include


I make it works by copy /opt/ros/kinetic/include/nav\_msgs into webots/projects/default/controllers/ros/include, but if I want to use TF, it's more complicated


What has to be done in the Makefile to link properly ros, tf, etc ?

##### David Mansolino [Cyberbotics] 09/19/2019 15:45:25
Hi, you probably want to have a look at this example controller that is extending the default ros controller of Webots: [https://github.com/cyberbotics/webots/tree/revision/projects/vehicles/controllers/ros\_automobile](https://github.com/cyberbotics/webots/tree/revision/projects/vehicles/controllers/ros_automobile)

##### ClLebi 09/19/2019 15:55:43
Yes I've already seen this, the problem is that it links webots-ros, not real ros


And webots-ros is not complete, for example we can't broadcast tf

##### David Mansolino [Cyberbotics] 09/19/2019 16:02:20
You're right, one possible solution would be to add the includes and libraries  to your  ros installation in the makefile. It is the `INCLUDE` and `LIBRARIES` variable:

[https://github.com/cyberbotics/webots/blob/revision/projects/vehicles/controllers/ros\_automobile/Makefile#L53](https://github.com/cyberbotics/webots/blob/revision/projects/vehicles/controllers/ros_automobile/Makefile#L53)

##### ClLebi 09/19/2019 16:14:58
It's what I did but I have a boost error : In file included from /usr/include/boost/thread/detail/move.hpp:12:0,

                 from /usr/include/boost/thread/lock\_types.hpp:11,

                 from /usr/include/boost/thread/pthread/mutex.hpp:16,

                 from /usr/include/boost/thread/mutex.hpp:16,

                 from /opt/ros/kinetic/include/tf/time\_cache.h:36,

                 from /opt/ros/kinetic/include/tf/tf.h:43,

                 from /opt/ros/kinetic/include/tf/transform\_broadcaster.h:36,

                 from test\_controller.cpp:16:

/usr/include/boost/core/enable\_if.hpp:28:10: error: redefinition of default argument for 'class T'

   struct enable\_if\_c {



May be because ros boost is different than the version used to create webots deb ...


I'm trying to build webots from source to see if I still get this problem


Just finished to build webots and still get this error :/

##### LukasBoy2000 09/20/2019 03:11:55
Hi I Just Got A WebBot


I Need Some Help...

##### David Mansolino [Cyberbotics] 09/20/2019 06:19:29
`@ClLebi` 

> May be because ros boost is different than the version used to create webots deb ...



Yes that's indeed a good explanation.

Recompiling Webots (or at least the ros controller is indeed a good start).


You should try to use only the boost version from your system and not the one of Webots.

You can either remove them from the sources or edit the makefile to remove the '$(WEBOTS\_HOME\_PATH)/projects/default/controllers/ros/include' part


`@LukasBoy2000`,


Welcome! I hope (and I am sure 😉) you will enjoy using Webots.


I suggest to start with the tutorial: [https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials)


And if you have specific question do not hesitate to ask here directly


`@LukasBoy2000`, you might also find some answers in the FAQ: [https://cyberbotics.com/doc/guide/webots-faq](https://cyberbotics.com/doc/guide/webots-faq)

##### alinux 09/23/2019 02:37:03
Hi! I'm using the irb world and the manipulator is able to go through a table for example. What can I do to prevent the arm from passing through the objects?

##### Stefania Pedrazzi [Cyberbotics] 09/23/2019 06:11:36
Hi `@alinux`, usuallly reducing the world time step (defined in `WorldInfo.basicTimeStep`) helps fixing this issue.


If not you could also try to tune the contactProperties (see [https://www.cyberbotics.com/doc/reference/contactproperties](https://www.cyberbotics.com/doc/reference/contactproperties)).


You should also check that the Robot.selfCollision flag is enabled ([https://www.cyberbotics.com/doc/reference/robot](https://www.cyberbotics.com/doc/reference/robot)) otherwise, if the table and the arm are part of the same robot, the collision between the two will be ignored.

##### alinux 09/23/2019 11:37:23
`@Stefania Pedrazzi` Can I get some information from selfCollision? I wanna add a penalty on the controller when the robot is colliding with something

##### Stefania Pedrazzi [Cyberbotics] 09/23/2019 12:03:12
`@alinux` Did you already check the description of the `selfCollision` flag on the robot documentation: [https://www.cyberbotics.com/doc/reference/robot](https://www.cyberbotics.com/doc/reference/robot)


this flag defines if collisions between parts of the robot are detected. But it doesn't have any impact on collisions with other objects that are not part of the robot itself.

##### alinux 09/23/2019 12:06:38
Yes, there's a table that is part from the robot


Apparently when it collides with that table, my controller is ignored

##### Stefania Pedrazzi [Cyberbotics] 09/23/2019 12:07:39
in that case you need to enable the `selfCollision` flag


you can change the value from the Webots scene tree by selecting the irb Robot.selfCollision item


the `selfCollision` flag doesn't give any information about the collided object. Which information do you need?

##### alinux 09/23/2019 12:25:37
I just wanna get on the controller when there are collisions. There is a way to do that?


I have already enabled the selfCollision flag

##### Stefania Pedrazzi [Cyberbotics] 09/23/2019 12:27:33
which function are you using in your controller to get the collisions?


> I just wanna get on the controller when there are collisions. There is a way to do that?

Yes! This is possible


The most generic way to detect collisions is to use a Physics Plugin ([https://cyberbotics.com/doc/reference/physics-plugin](https://cyberbotics.com/doc/reference/physics-plugin)) and implement the `webots_physics_collide` function ([https://cyberbotics.com/doc/reference/callback-functions#int-webots\_physics\_collidedgeomid-dgeomid](https://cyberbotics.com/doc/reference/callback-functions#int-webots_physics_collidedgeomid-dgeomid))


Then you can pass data from the physics plugin to your controller using the `dWebotsSend` ([https://cyberbotics.com/doc/reference/utility-functions#dwebotssend](https://cyberbotics.com/doc/reference/utility-functions#dwebotssend)) function


Alternatively you can add a TouchSensor device to the model and use the TouchSensor API to check for collisions.


But if you want to detect collisions with any part of the robot, then you should add many TouchSensor devices (one per joint Solid)

##### alinux 09/23/2019 12:36:41
I am using a Simulated Annealing controller for find a target and I want to implement a way to avoid the collisions. I will give a look on that Physics collide function and see if it works in my case.

##### Stefania Pedrazzi [Cyberbotics] 09/23/2019 12:37:48
don't hesitate to ask if you need some help with it

##### alinux 09/23/2019 12:38:38
Ok, thanks!

##### yuan gao 09/23/2019 19:39:33
hi, I have a question, how I supposed to adjust the friction coefficient of the floor, I only same default in ContactMaterial

##### David Mansolino [Cyberbotics] 09/24/2019 05:27:37
Hi `@yuan gao`, in that case leave both `material1` and `material2` fields of the ContactProperties ([https://www.cyberbotics.com/doc/reference/contactproperties](https://www.cyberbotics.com/doc/reference/contactproperties)) node to `default` and change the `coulombFrictioǹ` field.

##### Anoop 09/25/2019 01:14:59
Hi, I just installed Webots R2019b on my Lenovo yoga 720, buts its not supportive. While turning on webots, its getting shut off instantly. My laptop is having 3 GB of dedicated Nvidia Gpu and 4 Gb of shared GPU. Any helpp?

##### David Mansolino [Cyberbotics] 09/25/2019 06:18:48
Hi `@Anoop`, please make sure your GPU drivers are up to date. Have you tried starting Webots in safe mode? [https://cyberbotics.com/doc/guide/starting-webots#safe-mode](https://cyberbotics.com/doc/guide/starting-webots#safe-mode)

##### Vidicon 09/25/2019 09:24:16
hello everyone, I have been looking in to simulating a 'heavy' 3 wheeled robot (800Kg) but my robot sinks into the floor and cant drive. following a form post I changed ERP to 0.8 (was 0.4) but this only helps a bit. i also changed the floor to a solid box. if i set the basicTimeStep to 1 it works but then the simulation runs really slow. (0.8x realtime on a laptop with a RTX2060, i7-8750H, ubuntu 18.04) Is there something im overlooking?

##### Fabien Rohrer [Moderator] 09/25/2019 09:25:16
Hi


softCFM is certainly the key 😉 [https://cyberbotics.com/doc/guide/modeling#my-robot-is-heavy-and-sinks-into-the-ground-constantly](https://cyberbotics.com/doc/guide/modeling#my-robot-is-heavy-and-sinks-into-the-ground-constantly)

##### Vidicon 09/25/2019 09:26:41
thank you, i will look in to that.

##### Olivier Michel [Cyberbotics] 09/25/2019 09:27:30
Please, let us know if you believe we should add a comment regarding the ERP value in this FAQ.

##### Rahul0496 09/25/2019 09:35:53
Hey,i have been trying to work on the kuka robot. But the bot only moves in the forward direction and it does not strafe, any idea why?

##### Fabien Rohrer [Moderator] 09/25/2019 09:36:11
Hi,


yes, you have to copy the contact properties from the youBot examples in your world.


These lines: [https://github.com/cyberbotics/webots/blob/revision/projects/robots/kuka/youbot/worlds/youbot.wbt#L10-L34](https://github.com/cyberbotics/webots/blob/revision/projects/robots/kuka/youbot/worlds/youbot.wbt#L10-L34)

##### Rahul0496 09/25/2019 09:37:43
thank you,i will try it out


its working now, thanks

##### Radonism 09/25/2019 15:07:59
Does anybody have some insights about Khepeta IV robots?

##### David Mansolino [Cyberbotics] 09/25/2019 15:08:34
Hi, yes a model is available in webots [https://www.cyberbotics.com/doc/guide/khepera4](https://www.cyberbotics.com/doc/guide/khepera4)

##### Radonism 09/25/2019 15:10:03
Yeah I have checked that but I want to know whether it is compatible with ROS

##### David Mansolino [Cyberbotics] 09/25/2019 15:10:43
From what I know it only have a limited support for ROS for know, you probably have to recompile ROS for the robot yourself.

##### Radonism 09/25/2019 15:11:37
Currently I want to access the on board camera and grab relative position to ARtags


and further I hope to make them communicate


I just couldn't find any documentation about implementing ROS on it

##### David Mansolino [Cyberbotics] 09/25/2019 15:15:34
You should probably contact K-Team who is the company who created the robot: [https://www.k-team.com/](https://www.k-team.com/)

##### Radonism 09/25/2019 15:17:22
yeah probably. Thank you!

##### David Mansolino [Cyberbotics] 09/25/2019 15:22:49
You're welcome

##### SamSmurfitt 09/26/2019 10:52:32
Hi, is there an example Virtual Reality world I can test out?

##### David Mansolino [Cyberbotics] 09/26/2019 12:03:00
Hi  `@SamSmurfitt`, all of our worl files are compaible with virtual reality heasdsets ([https://cyberbotics.com/doc/guide/computer-peripherals#virtual-reality-headset](https://cyberbotics.com/doc/guide/computer-peripherals#virtual-reality-headset)). Is this what you are looking for ?

##### SamSmurfitt 09/26/2019 12:46:41
I've loaded up a sample world, (boomer) but whenever I enable the headset in webots I just get a fully light blue view

##### David Mansolino [Cyberbotics] 09/26/2019 14:04:04
which VR headset are you using ?

##### SamSmurfitt 09/26/2019 14:15:31
it's the Acer Windows mixed reality headset

##### David Mansolino [Cyberbotics] 09/26/2019 14:43:16
Unfortunately we never tested Webots with this headset (we mainly tested it with HTC Vive and Occulus).

Is this headset working fine with steamVR ?


(our interface relies on steamVR)

##### SamSmurfitt 09/26/2019 14:48:45
Yeah I play steam VR games all the time with it

##### David Mansolino [Cyberbotics] 09/26/2019 14:51:27
Can you maybe try with an older version of Webots ?

Webots R2018b is a good candidate as I am sure I tested VR with this one (just to make sure nothing was broken since then): [https://www.cyberbotics.com/archive/index.php](https://www.cyberbotics.com/archive/index.php)

##### SamSmurfitt 09/26/2019 15:12:01
ok thanks, ive got it working in one of the demo worlds.


One question, is it posible to link the viewpoint to a robot? I've built a driveable robot for my project and would like to be able to drive it from a first-person perspective. Is this possible?

##### David Mansolino [Cyberbotics] 09/26/2019 15:14:23
> ok thanks, ive got it working in one of the demo worlds.



good news, with the latest version or with R2018b ?



> One question, is it posible to link the viewpoint to a robot? I've built a driveable robot for my project and would like to be able to drive it from a first-person perspective. Is this possible?



Yes of course, from the view menu you have two option 'Follow Object' and 'Follow Object and Rotate': [https://cyberbotics.com/doc/guide/the-user-interface#view-menu](https://cyberbotics.com/doc/guide/the-user-interface#view-menu)

##### SamSmurfitt 09/26/2019 15:24:17
ah ok. So do I need to create an object where the virtual camera will be?


It was with the 2018b verison

##### David Mansolino [Cyberbotics] 09/26/2019 15:24:50
No, you just need to select you're robot and then enable the Follow behavior.


> It was with the 2018b verison



Ok thank you for the information


Just for info, we did log the issue with the latest version here: [https://github.com/cyberbotics/webots/issues/747](https://github.com/cyberbotics/webots/issues/747)

We will fix this as soon as we can.

##### SamSmurfitt 09/26/2019 15:29:15
ive selected to robot and under view selected 'Follow Object' but it follows from a top down view. Do i need to adjust the viewpoint position and orientation also?

##### David Mansolino [Cyberbotics] 09/26/2019 15:30:14
Yes, you have to put the initial position of the viewpoint where you want it to be (then it is going to keep its position/orientation relatively to the robot constant).


For example if you want to see as if you were the robot you have to put the viewpoint 'inside the robot' at the same position as its 'eyes'

##### SamSmurfitt 09/26/2019 15:33:23
ok thanks

##### David Mansolino [Cyberbotics] 09/26/2019 15:33:30
You're welcome

##### marina\_diamanti 09/26/2019 17:34:57
hello, ive put in an e-puck in turretslot a display node


and i wanna display in the display image some points


but i get these errors


[my\_controll] Error: wb\_display\_set\_color(): invalid device tag.

[my\_controll] Error: wb\_display\_draw\_line(): invalid device tag.


whats wrong?

##### elnaz 09/26/2019 17:47:21
Hi


I get this error when i am running e-puck with matlab language


failed to start: matlab -nosplash -nodesktop -r launcher


what should i do?

##### Fabien Rohrer [Moderator] 09/26/2019 19:05:42
`@marina_diamanti` the Display.name field should match exactly with the argument you pass to the wb\_robot\_get\_device(displayFieldName) function.


`@elnaz` is the matlab binary available from a terminal? Basically the matlab path should be added to the PATH environment variable. But this depends on the OS:


[https://cyberbotics.com/doc/guide/using-matlab](https://cyberbotics.com/doc/guide/using-matlab)

##### Vishal\_kumar 09/27/2019 06:24:29
i am not able to program physics plugin in python

##### David Mansolino [Cyberbotics] 09/27/2019 06:32:10
Unfortunately, physics plugin is one of the rare exception where you can only program in C (because the resulting shared library is directly loaded within Webots).

##### Raggy 09/27/2019 06:32:40
Cython integration?

##### David Mansolino [Cyberbotics] 09/27/2019 06:34:56
If I am not wrong Cython is for calling C function from Python right ?

##### Raggy 09/27/2019 06:35:35
Cython compiles Python to C


So you can generate those compiled files and directly link those

##### David Mansolino [Cyberbotics] 09/27/2019 06:36:29
Yes, that can indeed be a solution in that case, thanks for the pointer

##### Raggy 09/27/2019 06:37:31
Even if there isn't integration with webots, it is probably possible to use Cython to generate C files directly and paste those into webots  `@Vishal_kumar`


(Although that calls CPython's code too, not sure how webots handles external dependencies)

##### David Mansolino [Cyberbotics] 09/27/2019 06:40:37
As long as they are in the path and you take them into account in your Makefile everything should works fine with external dependencies, it works the same way as for controllers ([https://cyberbotics.com/doc/guide/using-webots-makefiles#adding-an-external-library-ccp](https://cyberbotics.com/doc/guide/using-webots-makefiles#adding-an-external-library-ccp)).

##### Vishal\_kumar 09/27/2019 07:31:53
`@Raggy` `@David Mansolino`  Thank you

##### aysegulucar 09/27/2019 07:47:50
Hi, we are preparing robotis-op3 gait manager. Is there a xm430.h library? We gone ahaed to write gait manager but it are not standing at the balance.


Is there a xm430.h library?


Thanks

##### Vidicon 09/27/2019 09:55:00
what is the "ask" argument in the ros service? Cant find it in the documentation.



`Type: webots\_ros/get\_float

Args: ask`

##### aysegulucar 09/27/2019 14:06:55
we cant reach to mgaitmanager nethod. all lines including mgaitmanager dont work. Why?

##### David Mansolino [Cyberbotics] 09/27/2019 14:22:19
<@301078397801725953>, you can ignore this argument (or set it to 1 by default).


`@aysegulucar` we dont have any 'xm430.h' file, but you should probably ask ROBOTIS, or look on internet. a qucik check on internet lead me to: [https://github.com/ROBOTIS-GIT/ROBOTIS-Framework](https://github.com/ROBOTIS-GIT/ROBOTIS-Framework)

You may find the required informations on this repository.

##### Vidicon 09/27/2019 15:07:52
`@David Mansolino` ok, Thanks.

##### David Mansolino [Cyberbotics] 09/27/2019 15:08:40
You're welcome

##### aysegulucar 09/27/2019 18:27:44
Thanks David,  Could you please answer  this question also. we cant reach to mgaitmanager nethod. all lines including mgaitmanager for op3 dont work. Why?  Just give an idea.

##### Daniel Deng 09/28/2019 02:40:54
Hi! I'm trying to use supervisor.simulationReset() in Pycharm, but after resetting the simulation, Pycharm will show "Process finished with exit code 1" when executing the following supervisor.step(timeStep). What went wrong?

##### Tutu 09/29/2019 08:25:56
Can someone send me a link to learn more about webots except their offical tutorials given by them

##### SimonDK 09/29/2019 18:57:25
Evening. Webots is complaining about ikpy not being installed even though I have installed it with pip. I am on macos. Any suggestions why it complains about ikpy not being installed? What Python path does Webots use?


I want to use vscode for my Python developments for webots. Can I add the webots libs to my path in vscode?

##### isayev ND-21 09/30/2019 01:52:41
at some point wasnt there an ERS-210 web bot model back in the day?

##### David Mansolino [Cyberbotics] 09/30/2019 06:35:49
`@aysegulucar`, are speaking of the real robot or the simulated one? If you speak about the real one, that's perfectly normal because the API of the real op3 is completely different from the one of the 1 and 2. about simulation, if you can't find the include, you probably have to adapt your makefile (however, since the shape fo the robot is not the same the gait manager may not work for the op3)

##### Fabien Rohrer [Moderator] 09/30/2019 06:37:42
`@SimonDK` Hi, you should basically include WEBOTS\_HOME/include and WEBOTS\_HOME/lib, and link with libController, as explained in this draft page of the documentation: [https://cyberbotics.com/doc/guide/using-your-ide?version=enhancement-ide-section](https://cyberbotics.com/doc/guide/using-your-ide?version=enhancement-ide-section)

##### Stefania Pedrazzi [Cyberbotics] 09/30/2019 06:43:20
`@isayev ND-21` : yes there was a ERS 210 model back in Webots 5. But after Sony decided to drop it, it was no longer possible to fully support it in Webots and only the ERS-7 model was kept.

##### Fabien Rohrer [Moderator] 09/30/2019 06:53:16
`@Daniel Deng` Hi, simulationReset() resets also the controllers, including the supervisor itself! So the termination of the process is the expected behavior.

##### Olivier Michel [Cyberbotics] 09/30/2019 06:53:42
`@Tutu`, you can listen to a recent podcast about Webots here: [http://www.theconstructsim.com/webots-robot-simulator-ros-olivier-michel/](http://www.theconstructsim.com/webots-robot-simulator-ros-olivier-michel/). Otherwise a simple google search will probably help you find what you need.

##### Daniel Deng 09/30/2019 15:16:32
`@Fabien Rohrer` Thanks for the response! So is there a way to reset only the world from external controller?

##### Fabien Rohrer [Moderator] 09/30/2019 15:34:58
Unfortunately no; Your controller should deal with this. For example by storing its state before reseting it, and restore it.


You could also think about to use another method than supervisor.simulationReset(). For example to reset only the robot position, its physics and its motors.


Another alternative is to use a script which runs Webots as a slave and create one Webots instance per individual training.

##### Daniel Deng 09/30/2019 15:40:15
Ok, thanks for the advice! I will try them and see if anything new pops up.

##### SimonDK 09/30/2019 19:39:21
`@Fabien Rohrer` Hi. I found that page as well, my challenge is that I cannot figure out where to add it in vscode 🤔Any ideas?

## October

##### chamandana 10/01/2019 01:10:59
guys, sorry for interrupting, can you point me to some quick resources to learn Inverse Kinematics. I need to move an end plate vertically upwards, which is connected to the base with two arms.

##### Fabien Rohrer [Moderator] 10/01/2019 06:21:24
`@chamandana` hi, Webots is released with an IK example using an ABB IRB 4600 arm and the ikpy Python module: [https://www.cyberbotics.com/doc/guide/irb4600-40#inverse\_kinematics-wbt](https://www.cyberbotics.com/doc/guide/irb4600-40#inverse_kinematics-wbt)

##### Owen 10/01/2019 11:21:04
Hi, there are few questions about contact points. When I open the "Show Contact points" function. I obseve that there are only few contact point on a plane(two plane are contact with each other). Is there any way to add more contact points on the plane when simulation running.

##### SimonDK 10/01/2019 11:30:50
When I start Webots it runs the last simulation immediately. Can I turn this off?

##### Fabien Rohrer [Moderator] 10/01/2019 12:05:29
`@Owen` Could you post a screenshot showing your issue here?


`@SimonDK` You can specify the world to open thanks to the webots launcher arguments:


[https://cyberbotics.com/doc/guide/starting-webots#command-line-arguments](https://cyberbotics.com/doc/guide/starting-webots#command-line-arguments)


(see worldFile)


Typically:


>>> ./webots path/to/you/world.wbt

##### Owen 10/01/2019 12:11:59
The situation is just like this. It looks like there are more than one point contacting black object, but actually there are only one point.
%figure
![grasp_task_ALL_init_v1.png](https://cdn.discordapp.com/attachments/565154703139405824/628564770135736321/grasp_task_ALL_init_v1.png)
%end

##### Fabien Rohrer [Moderator] 10/01/2019 12:13:17
I see. You can indeed expect to have a contact line there.


Are you sure these objects are defined by 2 Box nodes (cf. Solid.boundingObject) ?

##### Owen 10/01/2019 12:15:15
I'm sure that the two objects are box nodes.

##### Fabien Rohrer [Moderator] 10/01/2019 12:15:29
Ok


Are the black box and the fingers perfectly aligned? As in reality, 2 point collisions are possible in this case if the box is misaligned, and the stifness of the finger joints is really big.


.. and if the contact point has a big friction.

##### Owen 10/01/2019 12:24:26
I think that the black box and finger are misaligned. Is this the main problem to my simulation?

##### Fabien Rohrer [Moderator] 10/01/2019 12:27:44
I think that if you would like to have a better grip, you can act on the 3 parameters mentioned above: 1. box alignment, 2. stifness (due to joint torques or general physics paramaters such as ERP/CFM/basicTimeStep), 3. the contact friction between the finger and the box (ContactFriction). The good action depends on your requirements.


Another clue; I have done some experiments about gripping, and controlling the hand in torque control (rather than in position control) gives better results (you can adjust precisely the used strength)

##### Owen 10/01/2019 12:31:32
Okay, I got it.  I will try. Thanks for your precious and useful advise.

##### Fabien Rohrer [Moderator] 10/01/2019 12:32:41
(this is how the PR2 grabs the boxes in this released simulation: [https://www.cyberbotics.com/doc/guide/pr2](https://www.cyberbotics.com/doc/guide/pr2) cf. the youtube movie)


You're welcome, good luck with your project.

##### SimonDK 10/01/2019 13:12:47
Thanks `@Fabien Rohrer`. Great, I found it the menu "Preferences > Startup mode". Now it is set to Pause. /cheers


Isn't it possible to undo (ctrl+z) if you delete something from the node tree?

##### David Mansolino [Cyberbotics] 10/02/2019 15:22:41
Hi `@SimonDK`, unfortunately undo node deletion is not yet supported.

##### JoHn 10/02/2019 15:57:37
Hi guys, could you give me some advices here? I am new to Webots. I would like to run an autonomous driving simulation using ROS and Webots. Now I am able to launch Webots and a truck by following [https://github.com/cyberbotics/webots\_ros](https://github.com/cyberbotics/webots_ros), but I failed to move the truck using services. I am not sure what I am doing wrong. Thanks a lot!

##### David Mansolino [Cyberbotics] 10/02/2019 15:59:59
Hi `@JoHn`, you can find all the required services here: [https://cyberbotics.com/doc/automobile/driver-library?tab=ros](https://cyberbotics.com/doc/automobile/driver-library?tab=ros)

In particular these services:

  - /automobile/set\_steering\_angle

  - /automobile/set\_cruising\_speed

If you have any specific question/issue do not hesitate to ask us

##### JoHn 10/02/2019 16:04:27
Hi `@David Mansolino`, thank you for your quick response. This is what I am doing in the ROS node:    steeringClient = n.serviceClient<webots\_ros::set\_float>("vehicle/automobile/set\_steering\_angle");

    throttleClient = n.serviceClient<webots\_ros::set\_float>("vehicle/automobile/set\_throttle");

    steeringSrv.request.value = 0.5;

    throttleSrv.request.value = 0.5;

    steeringClient.call(steeringSrv);

    throttleClient.call(throttleSrv);

    ros::spinOnce();


My question is, do I need to have the services files in the catkin workspace?

##### David Mansolino [Cyberbotics] 10/02/2019 16:08:15
If you have installed the webots\_ros package you don't need: [http://wiki.ros.org/webots\_ros](http://wiki.ros.org/webots_ros)

##### JoHn 10/02/2019 16:12:30
Thank you `@David Mansolino`. Yes, I have installed the webots\_ros package.

##### danBOTT 10/02/2019 16:34:40
Hi guys, I'm new to Weebots; can I build a robot completely from scratch without using any built in model? If yes, how can I do that? Thanks.

##### LaCava 10/02/2019 16:36:04
`@danBOTT` Take a look here


[https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot](https://cyberbotics.com/doc/guide/tutorial-6-4-wheels-robot)


The other tutorials go over other scene editing necessities; this here is specifically for the robot part.

##### danBOTT 10/02/2019 16:41:11
Thanks. `@LaCava`

##### JoHn 10/02/2019 16:49:02
Hi guys, when we control robots in ROS, we need to do like this:  timeStepClient = n.serviceClient<webots\_ros::set\_int>(controllerName + "/robot/time\_step");  My question is: when we control vehicles (eg. BMW x5 or truck) in Ros, should we do the same like this:  timeStepClient = n.serviceClient<webots\_ros::set\_int>(controllerName +"/automobile/time\_step"); Thanks  a lot.

##### Saha 10/02/2019 19:06:13
Hey Im new to webots.. Where should I start??

##### JoHn 10/03/2019 01:39:46
Hi guys, is there any tutorials for controlling vehicles using ROS? Now I am able to control robots but not vehicles. I thought controlling robots and vehicles via ROS should be similar, but am not sure why vehicles do not move and steer.


Thank you in advance for any advices.

##### Stefania Pedrazzi [Cyberbotics] 10/03/2019 06:31:39
`@Saha` Welcome! I would suggest you to start from our tutorials: [https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials)


Hi `@JoHn`, for the `time_step` question: also in vehicle simulation you have to use the same service `<modelName>/robot/time_step`. All the required instructions for the vehicle are then executed internally by the ros\_automobile controller.

##### David Mansolino [Cyberbotics] 10/03/2019 06:57:11
`@JoHn`, about your second issue with vehicles and ROS, unfortunately there are no specific tutorial for vehicles with ROS, but as you said there is no real difference (juste a few more services and topics), so this tutorial is valid for vehicles too: [https://cyberbotics.com/doc/guide/tutorial-8-using-ros](https://cyberbotics.com/doc/guide/tutorial-8-using-ros)

The only main difference is that instead of the 'ros' controller you should use the 'ros\_automobile' one.

##### TunfischKorn 10/03/2019 09:42:42
Hey, does somebody knows how to fix that? I have Python 3.7 64Bit installed. But I can't make a connection to SUMO?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/629251977422962708/unknown.png)
%end

##### Fabien Rohrer [Moderator] 10/03/2019 09:44:06
Could you tell us what is your traci version?

##### TunfischKorn 10/03/2019 09:45:58
It is the one witch gets installed with the "webots-R2019b\_setup" so it is "sumo-0.30-windows".

From the \_init\_.py file -> @version $Id: \_\_init\_\_.py 23953 2017-04-16 16:19:38Z behrisch $

##### Fabien Rohrer [Moderator] 10/03/2019 10:13:41
Ok, more info...


traci is released in Webots, so there is no versioning. My previous comment has not much sense.


Your issue is weird, we just tested sumo with python 2.7 and 3.7 and it seems to work well.


Do you use a special version of Python 3.7? You should install it as described here: [https://cyberbotics.com/doc/guide/using-python](https://cyberbotics.com/doc/guide/using-python)


is it a possibility for you to disable the traffic lights? (By passing "--disable-traffic-lights" to the sumo\_supervisor.py controller, thanks to the SumoSupervisor.enableTrafficLights field). This would bypass the problematic code.

##### Saha 10/03/2019 10:23:56
`@Stefania Pedrazzi`  Thanks Ill check that out.

##### Tutu 10/03/2019 11:07:06
`@Olivier Michel` Thanks

##### TunfischKorn 10/03/2019 11:24:20
`@Fabien Rohrer` 

The Python example ->WEBOTS\_HOME/projects/languages/python/worlds/example.wbt works just fine. I can control the 3 robots and there is no crash. So I think Python is working with Webots software.



My version of Python out of cmd.exe -> "python --version" -> Python 3.7.4 (64bit) So the Path is also set correct



If I set the option to --disable-traffic-lights

...See in the Picture 😑
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/629277552837525505/unknown.png)
%end

##### David Mansolino [Cyberbotics] 10/03/2019 11:56:50
Hi `@TunfischKorn`, from the output it seems you have another conflicting installation of traci in 'Program files(x86)\Eclipse\Sumo'. This other version is probably not compatible with the one used by Webots. Please make sure that this path is not included neither in your PYTHONPATH and PATH environment variables

##### TunfischKorn 10/03/2019 12:17:11
`@Fabien Rohrer`

`@David Mansolino` Absolutely right! Thanks. Haven't seen it 😂 🙌

Now it works just fine!

##### David Mansolino [Cyberbotics] 10/03/2019 12:19:21
Perfect, thank you for the feedback.

##### JoHn 10/03/2019 14:37:41
Thank you `@David Mansolino` and `@Stefania Pedrazzi` for your kind answers, I appreciate that. Yes, I misunderstood it and used 'ros' instead of 'ros\_automobile' controller. The simulation has worked since last night and I will add more features to the simulation later on, including GPS, lidar and imported map. Thanks again.

##### David Mansolino [Cyberbotics] 10/03/2019 14:39:20
You're welcome, thank you for the feedback!

##### Himil 10/03/2019 20:30:46
Hey guys, 

We are group of CMU students and planning to use Webots for our capstone project. One of the requirement is a photorealistic images from Webots so that we can use them for SLAM and 3D reconstruction algorithm. While I found a lot of information about Appearance and stuff, I couldn't find much information or samples related to photo realistic images. Can someone guide me to a sample or some tutorial on it ?


Also what sort of maps are importable in Webots ? Can i make a elevationgrid in blender and import it here ?

##### Olivier Michel [Cyberbotics] 10/04/2019 06:05:59
Hi Himil,

Webots indeed provides highly realistic 3D images for its camera sensors compared to other robot simulators, so that's probably the best choice for your application. Creating environments that generates close to photo-realistic images requires some expertise, but you can start from existing samples and play with the parameters (lights, backgrounds, materials, etc.). A good starting point may be the following environment: [https://www.cyberbotics.com/doc/guide/samples-environments#hall-wbt](https://www.cyberbotics.com/doc/guide/samples-environments#hall-wbt)


You can import whatever 3D map in Webots given you convert them to VRML97 format. Importing a Blender-generated elevation grid is a typical use case. The Blender to webots exporter may turn our to be useful for such operations: [https://github.com/cyberbotics/blender-webots-exporter](https://github.com/cyberbotics/blender-webots-exporter)

##### Tahir [Moderator] 10/04/2019 08:48:10
Hey Guys

I am using webots to simulate multiple robots. I am using ROS and currently having synchronization problems.

Can someone tell me:

1. How to use wb\_robot\_step when using ROS, as we have no option to use it in the context of controller.

2. How to sync multiple homogeneous robots i.e. every robot should have its own wb\_robot\_step or just one which executes the next physics step would be enough.

Thanks in advance

##### David Mansolino [Cyberbotics] 10/04/2019 08:52:04
Hi `@Tahir`


1. if you need synchronization, you should call the `/robot/time_step` service from your node with a non-null argument. It is also strongly adviced to add the `--synchronize` argument in the controllerArgs field of each robot (se more info here: [https://cyberbotics.com/doc/guide/using-ros?tab=ros#using-the-standard-ros-controller](https://cyberbotics.com/doc/guide/using-ros?tab=ros#using-the-standard-ros-controller)).

2. Each robot should call it's `/robot/time_step` service.


Let me know if once this is done you still have issues

##### Tahir [Moderator] 10/04/2019 09:11:41
sure just let me try


by the way thanks fo r the reply

##### David Mansolino [Cyberbotics] 10/04/2019 09:24:04
You're welcome

##### Himil 10/04/2019 15:16:06
`@Olivier Michel` Thanks a lot for the help. Just yesterday I was looking into the blender exporter thing and looks like it might work for us. 

We also want to change the direction of the light as the time in the simulation changes (For example simulating the position of the sun throughout the day and thereby changing the shadow of the objects) Do you think that is something possible ?

##### David Mansolino [Cyberbotics] 10/04/2019 15:24:09
Hi `@Himil`, yes of course that is possible, you can change any parameter of the simulation at runtime using a Supervisor controller: [https://cyberbotics.com/doc/reference/supervisor](https://cyberbotics.com/doc/reference/supervisor)


You can find an example of this where the position of a PointLight is moved here: [https://cyberbotics.com/doc/guide/samples-devices#supervisor-wbt](https://cyberbotics.com/doc/guide/samples-devices#supervisor-wbt)

##### Himil 10/04/2019 16:16:17
That's awesome. Exactly what I was looking for. I was quickly able to simulate the trajectory I wanted. Example of it
> **Attachment**: [supervisor\_sunlight.mp4](https://cdn.discordapp.com/attachments/565154703139405824/629713411730833488/supervisor_sunlight.mp4)


Might want to skip the initial 10 seconds

##### javad 10/04/2019 16:18:16
hello every body

I want to make occupancy grid map by lidar in webots

my webots version is 7.1.0, so there is not lidar node in that. it use camera node for lidar using. wb\_camera\_get\_range\_image.


my question is how can I do mapping by lidar in this webots version


any body could help me

##### machinekoder 10/06/2019 09:54:26
hello everyone

Does anybody know why I can't add a PROTO file containing a Connector node to something other than a Robot node? Is this a bug?

##### Dumindu 10/06/2019 14:21:47
Hello everyone.


I'm using python controller to actuate a hinge joint using rotational motor. Do I have to define the motor name in order to use it in python script?

##### machinekoder 10/06/2019 14:27:41
`@Dumindu` yes, best go through the tutorials to learn the basics: [https://cyberbotics.com/doc/guide/tutorial-1-your-first-simulation-in-webots](https://cyberbotics.com/doc/guide/tutorial-1-your-first-simulation-in-webots)

##### Dumindu 10/06/2019 14:37:07
Thanks `@machinekoder`


I found it hard to learn via tutorials. Is there any resource or slides that explain the basics?

##### machinekoder 10/06/2019 15:09:54
`@Dumindu` I don't know if there are any slides. But I found the documentation very helpful too. Most of the docs contain C examples though. [https://www.cyberbotics.com/doc/guide/controller-programming](https://www.cyberbotics.com/doc/guide/controller-programming)

##### Stefania Pedrazzi [Cyberbotics] 10/07/2019 06:36:52
`@machinekoder` it is correct that the Connector, as all the devices in Webots, can only be inserted as a descendant of a Robot node. This is because in general devices can only be controlled/activated by controller program that has to be associated with a Robot node.

##### Dumindu 10/07/2019 06:50:59
ARNING: "python.exe" was not found.

Webots requires Python version 3.7 or 2.7 (64 bit) from python.org in your current PATH.

To fix the problem, you should:

1. Check the Python command set in the Webots preferences.

2. Check the COMMAND set in the [python] section of the runtime.ini file of your controller program if any.

3. Fix your PATH environment variable to use the required Python 64 bit version (if available).

4. Install the required Python 64 bit version and ensure your PATH environment variable points to it.

##### Stefania Pedrazzi [Cyberbotics] 10/07/2019 06:50:59
`@Dumindu` I confirm that there are no official slides about Webots. As already suggested, Webots User Guide and Reference Manual also contain examples. Otherwise it could also be useful to look directly at the code of the sample simulations: for example at `projects/languages/python/worlds/example.wbt`.

It is true that most of the docs contains C examples, but the python API matches almost one to one the C API.  

[https://www.cyberbotics.com/doc/reference/nodes-and-api-functions](https://www.cyberbotics.com/doc/reference/nodes-and-api-functions)

##### Dumindu 10/07/2019 06:51:23
How to resolve this exception:


WARNING: "python.exe" was not found.

Webots requires Python version 3.7 or 2.7 (64 bit) from python.org in your current PATH.

To fix the problem, you should:

1. Check the Python command set in the Webots preferences.

2. Check the COMMAND set in the [python] section of the runtime.ini file of your controller program if any.

3. Fix your PATH environment variable to use the required Python 64 bit version (if available).

4. Install the required Python 64 bit version and ensure your PATH environment variable points to it.

##### Stefania Pedrazzi [Cyberbotics] 10/07/2019 06:52:31
did you try to follow the instructions printed in the error message?


does the PATH environment variable in your system contain the Python directory?


Here is the documention about how to setup Python on Windows. Please let me know if this doesn't work for your or you have problems with it

[https://www.cyberbotics.com/doc/guide/using-python#windows-installation](https://www.cyberbotics.com/doc/guide/using-python#windows-installation)


`@javad` note that Webots 7 is an old version that is no longer supported  since many years. But the algorithm to create an occupancy grid map using a Webots 7 "range-finder" Camera or a Lidar should be the same as soon as you get the distance from the objects that you can extract from the image.

##### Dumindu 10/07/2019 09:22:42
I'm using python 3.7,4 but the programme still shows the same error


WARNING: "python.exe" was not found.

Webots requires Python version 3.7 or 2.7 (64 bit) from python.org in your current PATH.

To fix the problem, you should:

1. Check the Python command set in the Webots preferences.

2. Check the COMMAND set in the [python] section of the runtime.ini file of your controller program if any.

3. Fix your PATH environment variable to use the required Python 64 bit version (if available).

4. Install the required Python 64 bit version and ensure your PATH environment variable points to it.

##### Stefania Pedrazzi [Cyberbotics] 10/07/2019 09:23:40
did you add the path to your Python installation directory in your environmental variable PATH?

##### Dumindu 10/07/2019 09:23:48
yeah


CMD shows it

##### Stefania Pedrazzi [Cyberbotics] 10/07/2019 09:25:21
just to be sure, are you using the 64-bit version of python?

##### Dumindu 10/07/2019 09:25:31
Yes I am

##### Stefania Pedrazzi [Cyberbotics] 10/07/2019 09:27:57
we would need more info about your environment because usually setting up the PATH is enough to make it work correctly in Webots.

Which version of Webots are you using? which command is set in the Webots preferences?

##### Dumindu 10/07/2019 09:29:09
webots R2019b


command is python

##### Stefania Pedrazzi [Cyberbotics] 10/07/2019 09:38:35
did you install Webots for all the users or just for your account?

##### machinekoder 10/07/2019 11:26:08
Lets say I want to simulate a touchscreen inside my simulation environment. Would it be possible to get the mouse clicks on a Display device?


Or in general, can I somehow interact with objects in the environment during simulation by clicking on them? Let's say a physical button on a machine control?

##### Stefania Pedrazzi [Cyberbotics] 10/07/2019 11:45:51
`@machinekoder` yes it is possible to interact with the environment


you can achieve it using different methods:

1. modeling a button using the TouchSensor device ([https://www.cyberbotics.com/doc/reference/touchsensor#bumper-sensors](https://www.cyberbotics.com/doc/reference/touchsensor#bumper-sensors) see `projects/samples/devices/worlds/bumper.wbt` for a sample simulation)

2. using the Mouse API and handling the mouse events on the controller (see for example [https://www.cyberbotics.com/doc/guide/samples-howto#mouse\_events-wbt](https://www.cyberbotics.com/doc/guide/samples-howto#mouse_events-wbt))


In the `bumper.wbt` simulation if you click with the mouse on the black box in front of the robot you will trigger a collision (the same as the robot hits the wall)


`@Dumindu` please note that CMD could find the correct Python version even if the PATH (or Path) environement variable is not correctly set because it uses the Python launcher. Webots doesn't, so you have to really check the Path variable and ensure that the Python installation directory is specified in the PATH.

##### jwanner 10/07/2019 14:58:32
Hello,

##### David Mansolino [Cyberbotics] 10/07/2019 15:00:12
Hi `@jwanner`

##### jwanner 10/07/2019 15:02:52
I am working a modeling a new robot, it is a soft bodied robot with actuators in the body that act like hinges. In webots I want to model it with hinges and solid body part, but for now I only was able to use hinge as so : two body parts linked with a hinge(motor) and when the motor is activated it lifts the linked body part. But I would like that the two body parts to be lift with the same angle around the hinge, is it possible ? ( I am sorry if I am not clear)

##### David Mansolino [Cyberbotics] 10/07/2019 15:05:20
You mean the lighter par is lifted more than the weighter? Is this the problem?

##### jwanner 10/07/2019 15:08:25
I have a solid part, a hinge and a second solid part. When I activate the motor of the hinge I would like that both parts are lifted with the same angle around the hinge



%figure
![Hinge.jpeg](https://cdn.discordapp.com/attachments/565154703139405824/630784882200543251/Hinge.jpeg)
%end


like the second line, not the first line

##### David Mansolino [Cyberbotics] 10/07/2019 15:14:28
Ok, that should indeed be feasible in Webots.


You should make sure that the total mass of both side of the joint are equal (and probably that the center of mass at an equivalent distance of the joint too).

##### Marcey 10/07/2019 15:15:05
Hello guys, I am using Webots with ROS. I want to get the velocity of a wheel/RotationalMotor, to create a graph where I can compare the set velocity and the actual velocity and see how it changes with different accelerations.



My plan was to use the "/<device\_name>/get\_velocity"-service, but it turns out this just mirrors the inputs from "set\_velocity, right?



Is there an easy way to get the actual velocity(a service would be prefered)?

##### David Mansolino [Cyberbotics] 10/07/2019 15:15:51
Hi `@Marcey`, you are completely right about the 'get\_velocity' service.


The workaround is to use the position and to derivate it in order to get the speed


For this you need to use the position-sensors: [https://www.cyberbotics.com/doc/reference/positionsensor?tab=ros#wb\_position\_sensor\_enable](https://www.cyberbotics.com/doc/reference/positionsensor?tab=ros#wb_position_sensor_enable)

##### Marcey 10/07/2019 15:49:56
`@David Mansolino`, thanks! Since I am not the most expirienced programmer, I hoped to avoid this 😃

##### David Mansolino [Cyberbotics] 10/07/2019 15:54:00
You're welcome 🙂

##### Muhsin Kompas 10/07/2019 20:43:13
K



> **Attachment**: [video.mov](https://cdn.discordapp.com/attachments/565154703139405824/630868097083113472/video.mov)


why does this object constantly fall into space?

##### hayRobots 10/07/2019 23:47:05
Hello


Is there a way to use the naoqi library with webots ??

##### Sohil 10/08/2019 03:25:43
Hi everyone! I am new the Webots and it looks really promising for my project!

I want to read the time of the simulation and apply a directional light according to it. I know how to set the directional light using the supervisor, but I am finding it hard to read the time or progression of simulation time. Any leads?

##### Tahir [Moderator] 10/08/2019 04:06:45
`@Sohil`  you can get the simulation time in supervisor node.

##### Chirantha 10/08/2019 05:16:27
Hi everyone! I'm actually new to webobts and I'm trying to figure out how to write my script. I prefer using python as I'm more familiar with it (version 3.7, 64-bit). But where can I find the basic Webots libraries to be added into Python needed for this? And also where should I write my code? In a python file or inside Webots itself?

##### Tahir [Moderator] 10/08/2019 05:57:01
`@Chirantha` Hello,

You can write your script both in webots amd external file. In case of Webots(which is preferable), you just need to specify the controller.

In case of an external file you need to specify 'extern' in controller field of the robot.

And about the library I would recommend you to go through webots tutorials. There you will find the necessary information and you'll get an idea how you can proceed in your project may be.


[https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials)


[https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers](https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers)


In tutorials you can find example codes in Pyrhon, C, CPP, Java etc.

##### Stefania Pedrazzi [Cyberbotics] 10/08/2019 06:22:02
`@Sohil` the Robot API function `wb_robot_get_time` returns the current simulation time in seconds: [https://cyberbotics.com/doc/reference/robot#wb\_robot\_get\_time](https://cyberbotics.com/doc/reference/robot#wb_robot_get_time)


`@Muhsin Kompas` the objects falls because the road doesn't have any boundingObject (physics shape). To fix it you have to add a ground object, for example a Floor PROTO

##### David Mansolino [Cyberbotics] 10/08/2019 06:29:31
`@hayRobots`, yes even if this is not recommended: [https://github.com/cyberbotics/naoqisim](https://github.com/cyberbotics/naoqisim)

##### Marcey 10/08/2019 08:52:04
I have a follow up question to my question from yesterday. You recommended to use the PositionSensor and derivate it to get the speed. My idea was to record a rosbag of the "/<device\_name>/value"-topic (that worked) and use Matlab to derivate. But I cannot open the bag-File in Matlab and when I want to plot the bag in rqt, I get this error:



For type [webots\_ros/Float64Stamped] stored md5sum [e6c99c37e6f9fe98e071d524cc164e65] has invalid message definition."



Do you have any ideas on how to solve this or for another approach?

##### David Mansolino [Cyberbotics] 10/08/2019 09:02:29
It seems the definition of the message (and therefore the md5sum) is different in Matlab 😕 why using matlab? It should be quite simple to compute the position difference and divide it by the time difference directly in your ROS controller.

##### Marcey 10/08/2019 09:09:46
the error is not from matlab, but from rqt ([http://wiki.ros.org/rqt](http://wiki.ros.org/rqt)). Yeah, it probably is quite simple, if you know how to do it 😅.

##### David Mansolino [Cyberbotics] 10/08/2019 09:33:14
You're right it's rqt (which is even more strange), but anyway computing it directly in the ros node is probably the simplest workaround.

From the webots\_ros/Float64Stamped ([http://docs.ros.org/melodic/api/webots\_ros/html/msg/Float64Stamped.html](http://docs.ros.org/melodic/api/webots_ros/html/msg/Float64Stamped.html)) you can get the value (in the data field)  and the associate time (in the header.stamp field) you need then to keep the previous value and time and when you receive a new value you can compute the speed with: speed = (new\_value - old\_value) / (new\_time - old\_time)

##### MIKE 10/08/2019 12:56:52
Hi！Does the supervisor\_rotation function allow a robot to rotate around a specified axis? Or is it only able to find its own center to rotate?

##### David Mansolino [Cyberbotics] 10/08/2019 12:59:39
Hi Mike, If you are speaking about the follow and rotate behavior of the viewpoint, the rotation will be the same as the object itself, the goal is to be able to as if you were the object

##### Sohil 10/08/2019 17:00:09
`@Stefania Pedrazzi` Can I get time using an instance of `Supervisor()`? I am currently using that to change the direction of light? Will I also have to add an instance of `Robot()` to get time?

##### Muhsin Kompas 10/08/2019 19:12:12
`@Stefania Pedrazzi` I have another problem. The image is distorted when I use Floor.



%figure
![Untitled.png](https://cdn.discordapp.com/attachments/565154703139405824/631207438225506304/Untitled.png)
%end

##### Tahir [Moderator] 10/08/2019 20:22:58
`@Sohil` upto my knowledge,

Supervisor node is available only in Robot. So here in this case you can add a dummy Robot() from which you can enable the Supervisor and than can use this to get the simulation time.

##### jwanner 10/08/2019 20:25:55
Hi again ! Thank you for your help the other day, I was able to achieve what I wanted. But now I have another question, is it possible to have an automatic generation of nodes in Webots ? I have a robot with several rigid segments linked together with hinge/rotational motor, and for example I would like to say, my total length is 10cm and I want 5 segments and it would build the 5 rigid segments of 2cm each linked with rotational motor. I have no idea if it is feasible or if we just have to do it by hand each time, I am curious.
%figure
![image0_1.jpeg](https://cdn.discordapp.com/attachments/565154703139405824/631225788867412038/image0_1.jpeg)
%end

##### Stefania Pedrazzi [Cyberbotics] 10/09/2019 06:14:43
`@Sohil` the Supervisor class inherits from Robot class, so you can call the `getTime()` function from the Supervisor instance.


Adding dummy Robot instances as suggested by `@Tahir` should be avoided and it is useless.


`@Muhsin Kompas` the visual issue you get is called z-fighting ([https://en.wikipedia.org/wiki/Z-fighting](https://en.wikipedia.org/wiki/Z-fighting)).

This happens because the road and the floor are coplanar and can be solved by slightly modifying the translation.y value of one of the two objects.


If you look at the `projects/vehicles/wrolds/city.wbt` simulation distributed within Webots, the translation.y value of the road elements is set to 0.02.


Another option is to remove the graphical appearance of the Floor and keep only the physical one.

##### David Mansolino [Cyberbotics] 10/09/2019 06:35:06
`@jwanner`, yes this is feasible, for this you should do a PROTO.

If you are not familiar with PROTO, I strongly recommend to follow this tutorial: [https://www.cyberbotics.com/doc/guide/tutorial-7-your-first-proto](https://www.cyberbotics.com/doc/guide/tutorial-7-your-first-proto)

Then reading the chapter about PROTO: [https://www.cyberbotics.com/doc/reference/proto](https://www.cyberbotics.com/doc/reference/proto)

In particular the sub-chapter about procedural PROTO that allows you to use a scripting language inside your PROTO definition: [https://www.cyberbotics.com/doc/reference/procedural-proto-nodes](https://www.cyberbotics.com/doc/reference/procedural-proto-nodes)

##### Dumindu 10/09/2019 07:33:00
Hi,


Is there is a way to delay a function. Like delaying a funcition by 1000ms


?

##### Olivier Michel [Cyberbotics] 10/09/2019 07:35:36
You can simply check the time in the main loop and don't call this function until the waiting time you need is over.

##### Dumindu 10/09/2019 07:35:54
I'm using python


How to invoke a delaying statement there?

##### Olivier Michel [Cyberbotics] 10/09/2019 07:40:11
Simply make a test in your control loop to check if the delay is over and call the function if this is the case.

##### Dumindu 10/09/2019 10:41:52
How can I read the position of a motor when I'v attached a motor as a actuator?

##### Olivier Michel [Cyberbotics] 10/09/2019 10:46:55
You should use a PositionSensor node.

##### Muhsin Kompas 10/09/2019 11:31:11
`@Stefania Pedrazzi` floor translation y=-0.005, road translations y =0

##### David Mansolino [Cyberbotics] 10/09/2019 12:05:32
`@Muhsin Kompas` what is the value of the Viewpoint.near field ?

##### Muhsin Kompas 10/09/2019 12:10:11
`@David Mansolino` how can i see

##### David Mansolino [Cyberbotics] 10/09/2019 12:10:46
You can see this in the scene-tree ([https://cyberbotics.com/doc/guide/the-scene-tree](https://cyberbotics.com/doc/guide/the-scene-tree))

##### Muhsin Kompas 10/09/2019 12:11:45
floor 50 -0.05 25


road 0 0 0

##### David Mansolino [Cyberbotics] 10/09/2019 12:12:20
And what about th eViewpoint near value ?

##### Muhsin Kompas 10/09/2019 12:12:55
i dont know it

##### David Mansolino [Cyberbotics] 10/09/2019 12:15:06
You can read it here.
%figure
![Capture_du_2019-10-09_14-14-28.png](https://cdn.discordapp.com/attachments/565154703139405824/631464657021960202/Capture_du_2019-10-09_14-14-28.png)
%end

##### Muhsin Kompas 10/09/2019 12:15:21
0.05

##### David Mansolino [Cyberbotics] 10/09/2019 12:16:49
Ok, then try setting it to 0.5 (or even 1-2 if it still doesn't work)


For more information, please refeer here: [https://www.cyberbotics.com/doc/reference/viewpoint](https://www.cyberbotics.com/doc/reference/viewpoint)

##### Muhsin Kompas 10/09/2019 12:23:03
thanks `@David Mansolino`

##### David Mansolino [Cyberbotics] 10/09/2019 12:23:10
You're welcome

##### BlackPearl 10/09/2019 13:11:46
`@David Mansolino`  can’t change the values anymore


For example viewpoint

##### David Mansolino [Cyberbotics] 10/09/2019 13:41:43
What yo you mean by 'you can't change' ?

##### Lars 10/09/2019 13:41:48
Trying to read radians of a PositionSensor using Python and the readout into terminal from the function `robot.getPositionSensor` is something like 

`[<controller.PositionSensor; proxy of <Swig Object of type 'webots::PositionSensor *' at 0x0000029A5954D5A0> >` What's going wrong here?

##### BlackPearl 10/09/2019 13:42:15
The window doesn’t show up to change the value

##### David Mansolino [Cyberbotics] 10/09/2019 13:43:38
`@Lars`, I strongly recommend to follow this tutorial to understand how the Python API works: [https://www.cyberbotics.com/doc/guide/tutorial-4-more-about-controllers?tab=python](https://www.cyberbotics.com/doc/guide/tutorial-4-more-about-controllers?tab=python)

basically you should:

  1. get the position -sensors device.

  2. enable it.

  3. get the value


```Python
positionSensor = robot.getPositionSensor('name')
positionSensor.enable(32)
....
positionSensor.getValue()
```


`@BlackPearl`, try Tools => Restore Layout

##### Lars 10/09/2019 13:46:22
`@David Mansolino` Thanks, I'll give it a try

##### David Mansolino [Cyberbotics] 10/09/2019 13:46:36
You're welcome

##### capucsc 10/10/2019 05:00:00
Quick question: has anyone tried to build the ROS2 package on Mac? I'm having a difficult time with an odd path:



```
[Processing: webots_ros2_desktop]                   
--- stderr: webots_ros2_desktop                     
error: can't copy '../../../../../../../Applications/Webots.app/lib/libzip.dylib': doesn't exist or not a regular file
---
Failed   <<< webots_ros2_desktop    [ Exited with code 1 ]
```


the system variable is set with: `export WEBOTS_HOME=/Applications/Webots.app/`

##### equinox 10/10/2019 05:44:38
Hi everyone, i'm new to webots and i'm trying to develop a quadcopter proto, simply with 1 body (solid with shape of a box), 4 propellers and 2 diagonals and i'm reading both from the propeller world file and mavic2pro proto.  however, although the 4 propellers seem to be working fine (with a controller as simple as setting velocity to the motors),  the quadcopter spins around itself whenever the velocities go high and never takes off the ground. What may be the possible issues with this? Thank you all so much.

##### machinekoder 10/10/2019 07:54:30
Can I set the position of a Motor node from a supervisor controller? I was thinking about something along the lines of 

```
motor = robot.getFromDef(name)
position = motor.getField('position')
def set_position(target):
  position.setSFFloat(target)
```

##### Stefania Pedrazzi [Cyberbotics] 10/10/2019 08:38:54
`@machinekoder` it is only possible to control a device from its Robot controller and using the device API


so if the Motor node is not part of the Supervisor model, you cannot control it from your supervisor controller


nevertheless you should be able to set the position of the joint, the same as you will do from the scene tree, using the snippet you posted

##### Lars 10/10/2019 13:57:39
`@David Mansolino` Since implementing the `getValue` function to obtain the motor position it just keeps reading `[nan]` at the start of the program or `[0]` after moving the motors into a new position. I didn't have these issues when I initially did the tutorials for distance sensor and neither when I replicated it again.

##### Gautier 10/11/2019 07:35:48
Hello, Is there a way to change the geometry of an object using supervisor in a "fast way" ?


I want to simulate deformation on a mesh, I've used supervisor to change the points of an IndexFaceSet, but it's pretty slow.


(Here is the C code [https://pastebin.com/wTBZk88e](https://pastebin.com/wTBZk88e) )


(In another question, do you know how this demonstration/exemple has been done ? [https://youtu.be/OKxdaNqmQco](https://youtu.be/OKxdaNqmQco)  )

##### Olivier Michel [Cyberbotics] 10/11/2019 09:15:52
`@Lars`: you should enable the sensors so that you don't get the `nan` value any more.


[https://www.cyberbotics.com/doc/reference/positionsensor?tab=python#wb\_position\_sensor\_enable](https://www.cyberbotics.com/doc/reference/positionsensor?tab=python#wb_position_sensor_enable)

##### Lars 10/11/2019 09:20:22
`@Olivier Michel` I've done that. I also implemented `while robot.step(timeStep) != -1:` to prevent it from reading sensor values before there are anything in there.

Is there anything preventing me from doing this using robot() rather than supervisor()?

##### Olivier Michel [Cyberbotics] 10/11/2019 09:20:47
`@Gautier`: I believe the demo you are mentioning (waves) was achieved about the same way, but instead of changing the coordinates directly, the supervisor changes some field of a procedural PROTO node which in turn will recompute its internal coordinate values accordingly. This might be faster than changing the coordinates directly, not sure.


`@Lars`: you should be able to read this information from any robot, no need for a supervisor. Are you sure your device tags are non-null and the corresponding devices are properly enabled? Do you get any warning in the Webots console?

##### Lars 10/11/2019 09:27:36
`@Olivier Michel` Yeah, the reason I am asking is because I took a simple example from [https://github.com/cyberbotics/webots/issues/711#issuecomment-510832161](https://github.com/cyberbotics/webots/issues/711#issuecomment-510832161) and implemented the getValue() function to read the motion. But I can't do it for my own robot/code where I go through similar steps.


I can upload my own project if that would make it easier.

##### Olivier Michel [Cyberbotics] 10/11/2019 09:48:23
Are you sure you are calling the step function regularly before checking the sensor value?

##### Lars 10/11/2019 10:19:16
Looks like this

```while robot.step(timeStep) != 1:
    for i in range(len(psName)):
        value = ps[i].getValue()
        psValues.append(value)
    break
print(psValues)
```

Prints all zeros in the list for all of my sensors.

##### Olivier Michel [Cyberbotics] 10/11/2019 10:50:11
That's normal, it prints the sensor values at step 0, so, it could be that all values are 0. You should put the print inside the loop and should not break the loop.


```
while robot.step(timeStep) != 1:
    for i in range(len(psName)):
        value = ps[i].getValue()
        psValues.append(value)
    print(psValues)
```


Would that work better?

##### Gautier 10/11/2019 11:24:07
`@Olivier Michel`  Ok, thank  you !


So basically, If i want to test that out, I have to make a new proto, with the "points" of the coordinate as a field ?


If so, what should be the type of the corresponding field ? "Coord" I guess ?

##### Olivier Michel [Cyberbotics] 10/11/2019 11:28:43
No, that's not what was done in the demo PROTO.


The field of the new PROTO should be used to recompute the coordinates. It could be for example the amplitude and frequency parameters of the wave and the procedural PROTO will recompute the coordinates according to these input fields.


Do you see what I mean?

##### Lars 10/11/2019 11:30:42
`@Olivier Michel` It just continues to print 0 endlessly. Might there be a problem in the construction of the device(s)?

One of the leg joints with a rotational motor and sensor looks like this:

```DEF LEG-1-COXA HingeJoint {
      jointParameters HingeJointParameters {
        axis 0 0 1
        anchor 0.0716 0.12096 0
      }
      device [
        PositionSensor {
          name "c1_pos"
          resolution 4096
        }
        RotationalMotor {
          name "c1_ser"
          maxVelocity 1
          maxTorque 100
        }
      ]
```

I tried changing parameters such as resolution, but it didn't change the outcome.

##### Olivier Michel [Cyberbotics] 10/11/2019 11:31:45
Is the motor actually moving?

##### Lars 10/11/2019 11:32:42
`@Olivier Michel` Yeah all motors are moving as they should, that's why I'm so confused.

##### Olivier Michel [Cyberbotics] 10/11/2019 11:33:56
When you open the robot window, can you see the position sensor values in the graphical user interface?

##### Gautier 10/11/2019 11:34:55
`@Olivier Michel`  I think I see. So, for exemple, I can create a new value "coefficient" in my proto, and multiply the points in the field "coordinate" by that coefficient ?

##### Lars 10/11/2019 11:35:51
`@Olivier Michel` Yes, when I run the program and command the first joint to move to 0.1 it changes as it should from 0 to 0.1, but sensor doesn't see the change.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/632179555171434526/unknown.png)
%end

##### Gautier 10/11/2019 11:35:57
(here is my proto : [https://pastebin.com/WYnj7u2E](https://pastebin.com/WYnj7u2E) , I want to modify some of the "z" coordinate" of some points )

##### Olivier Michel [Cyberbotics] 10/11/2019 12:04:00
`@Lars`: I mean the robot window. Double-click on the robot and the robot window should open, then you should see a tab with position sensors.


`@Gautier`: it should be a procedural proto.


[https://www.cyberbotics.com/doc/reference/procedural-proto-nodes](https://www.cyberbotics.com/doc/reference/procedural-proto-nodes)


That contains some LUA code which recomputes the coordinates.

##### Lars 10/11/2019 12:07:55
`@Olivier Michel` Yeah I see change in the motors, but sensors stay at 0.

##### Olivier Michel [Cyberbotics] 10/11/2019 12:09:56
Ok, that means the problem is in the sensors, not in the controller.


Not sure...


At which frequency did you enabled the sensors?

##### Lars 10/11/2019 12:11:20
Same as the motors. `timeStep = int(4 * robot.getBasicTimeStep())`

##### Olivier Michel [Cyberbotics] 10/11/2019 12:11:38
OK. Is you controller in C or C++?

##### Lars 10/11/2019 12:11:44
Python.

##### Olivier Michel [Cyberbotics] 10/11/2019 12:11:48
OK.


Well, I am running out of ideas...

##### Lars 10/11/2019 12:13:29
Will anything change if I put the robot into a proto file and use it?

##### Olivier Michel [Cyberbotics] 10/11/2019 12:13:52
No, it shouldn't change anything.


I would recommend you to start from the provided example:


webots/projects/samples/devices/worlds/position\_sensor.wbt


write a python controller for it and check whether it works.


Then modify it step-by-step to get as close as possible as your own model.


At some point you may find the problem.

##### Lars 10/11/2019 12:17:12
Yeah that was my plan B if my current controller couldn't be salvaged. I have a simulation working that can read position sensor in Python. But it is not for my own robot. 

Anyway thanks so much for the help. 😀

##### Gautier 10/11/2019 12:28:38
`@Olivier Michel`  Thank you for your response !


Well, your idea kind of works. I'm still stuck at "something" and I don't really know why it doesn't works


I've proceed as you suggested, with lua code, and defined the followings lines :


%{

        print('test')

        local i

        for i = 1,40,1

        do

            print("\n")

            print(fields.points.value[i]["x"])

            fields.points.value[i]["x"] = fields.points.value[i]["x"] + 10

            print("   ")

            print(fields.points.value[i]["x"])

        end

    }%


The problem is that it "display" the change of value in the console, in lua. But it doesn't change the field values in itself, in webots


Where did I made a mistake ?

##### Olivier Michel [Cyberbotics] 10/11/2019 12:42:01
You should not change input fields, but rather change the coordinates values inside the PROTO.


It's the point list inside the PROTO that should be generated.


Your PROTO should contain an IndedexFaceSet node for which you should compute the coordinates and assign them with the `%{=` statement.

##### jhielson 10/11/2019 13:50:38
Hi, I would like to know if there is a way to upload a hex file into epuck using Webots.


I did the cross-compilation from Webots to generate the file but I do not know how to upload on E-puck robot. It is already paired via Bluetooth.

##### Olivier Michel [Cyberbotics] 10/11/2019 13:55:59
You can do this from the Robot Window choosing Upload HEX pop-up menu: [https://cyberbotics.com/doc/guide/epuck#robot-window](https://cyberbotics.com/doc/guide/epuck#robot-window)

##### jhielson 10/11/2019 13:59:33
In my computer, it opens a generic robot windows. There are some information about the readings of the sensors but I can not find this option. Am I doing something wrong?

##### Olivier Michel [Cyberbotics] 10/11/2019 14:00:41
That means the robot is not recognized as an e-puck robot... Did you change it?


Can you open the default e-puck world? In this one, you should be able to open the e-puck robot window and upload your HEX file.

##### jhielson 10/11/2019 14:02:36
I didnt generate the project. I am gonna check the default one. Thanks Oliver Michel!!!


You are right. It works on the default epuck project.

##### ClLebi 10/11/2019 17:47:22
Hi Guys, I'm wondering if it's possible to have a Collision sensor without generating a collision. Like a box sensor that say : There is something in me without using radar or lidar or camera etc. Juste a Collision sensor without generating collision ?

##### JoHn 10/11/2019 19:49:06
Hey guys, I'd like to control multiple robots via ROS in one environment. Should each robot have one ROS node to control it? Thanks a lot.

##### SimonDK 10/11/2019 20:14:02
Can I select multiple objects in the Scene Tree sidebar? I need to select several hundred items

##### Lars 10/12/2019 12:47:19
`@Olivier Michel` Since yesterday I worked on some changes for the position sensor that did not resolve my issue. Then it occurred to me that it was perhaps not the controller or my code that was at fault, but rather something with the device(s). Then I stripped everything away to see what might be the problem. And apparently it was because I had put a value in the resolution field, when I changed it to -1 (default value) I was able to read the position of all my sensors.

But this begs the question as to why I was unable to specify the resolution in the first place? According to the documentation in [https://cyberbotics.com/doc/reference/positionsensor](https://cyberbotics.com/doc/reference/positionsensor) you can set the resolution to anything in the interval of 0 and infinity(-1). So what's the problem here?

##### SimonDK 10/12/2019 18:14:11
Can I mirror a component?

##### Olivier Michel [Cyberbotics] 10/14/2019 06:09:23
`@ClLebi`: yes, this is possible, you will have to program it from a physics plugin, detect the collision, then remove it, so that it won't be taken into account by the physics engine.


`@JoHn`: what you want to do is possible. I see no problem doing that. Did you try to do it? Did you experienced any problem?


`@SimonDK`: it's not possible to select multiple objects in the scene tree. Not sure what you mean by mirroring a component... It is indeed possible to set a negative Transform.scale parameter to somewhat mirror a shape. Is that what you meant?


`@Lars`: it's great that you found the problem. We are going to investigate this resolution problem to see whether you uncovered a bug... I will be back about it.

##### Choomah 10/14/2019 07:44:45
Hi all!

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 07:46:24
Hi  `@Choomah` !

##### Choomah 10/14/2019 07:49:34
I am trying to build an MVP for an idea, which incorporates [https://robotbenchmark.net/](https://robotbenchmark.net/) style of using webots. Looking at [https://cyberbotics.com/doc/guide/web-simulation](https://cyberbotics.com/doc/guide/web-simulation) and following that, I end up not seeing a web interface.


is there another version or should you build your own ?

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 07:52:42
The web interface is available in the webots.min.js package available on the cyberbotics website ([https://cyberbotics.com/files/repository/www/wwi/R2019b/webots.min.js](https://cyberbotics.com/files/repository/www/wwi/R2019b/webots.min.js))


You have to build a web page that instatiates the webots.View main objects defined in `webots.min.js` adn executes it .


[https://cyberbotics.com/doc/guide/web-simulation#how-to-embed-a-web-scene-in-your-website](https://cyberbotics.com/doc/guide/web-simulation#how-to-embed-a-web-scene-in-your-website)

##### Choomah 10/14/2019 07:55:12
got it


THanks Stefania



%figure
![Screen_Shot_2019-10-14_at_09.58.35.png](https://cdn.discordapp.com/attachments/565154703139405824/633212093478338560/Screen_Shot_2019-10-14_at_09.58.35.png)
%end


cat config/session/local.json 

{

  "port": 1999,

  "server": "localhost",

  "simulationServers": [

    "localhost:2000"

  ]

}


Looks like the right port to me


its in a Docker container

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 08:01:40
You can check if the the session and simulation servers are working correctly by checking the monitor pages:

* localhost:1999/monitor

* localhost:2000/monitor

##### Choomah 10/14/2019 08:02:01
500 Error


2019-10-14 08:02:17,755 [ERROR  ]  Uncaught exception GET /monitor (172.20.0.1)

HTTPServerRequest(protocol='http', host='localhost:2000', method='GET', uri='/monitor', version='HTTP/1.1', remote\_ip='172.20.0.1')

Traceback (most recent call last):

  File "/usr/local/lib/python3.5/dist-packages/tornado/web.py", line 1697, in \_execute

    result = method(*self.path\_args, **self.path\_kwargs)

  File "./simulation\_server.py", line 437, in get

    self.write(cpu + cores + "</p>\n")

UnboundLocalError: local variable 'cpu' referenced before assignment

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 08:03:18
Are you running the scripts with python3?

##### Choomah 10/14/2019 08:03:56
python --version

Python 3.5.2

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 08:04:04
Currently they only work with Python 2, but I'm about to fix it

##### Choomah 10/14/2019 08:04:11
aha

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 08:04:16
I just have an open PR about this..

##### Choomah 10/14/2019 08:04:27
Then there is some confusion in Documentation about this

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 08:05:26
yes, you are right.

##### Choomah 10/14/2019 08:05:34
I'll rebuild the container for Python2 then

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 08:06:07
or you can apply the patch to the scripts if it takes too long to rebuild


(I'm looking for the link)


Here is the fixed `simulation_server.py` version working for python 3. The session script doesn't need any change:

[https://github.com/cyberbotics/webots/pull/884/files#diff-8230a21f4eeb4b071e8135e1b8ef2c50](https://github.com/cyberbotics/webots/pull/884/files#diff-8230a21f4eeb4b071e8135e1b8ef2c50)

##### Choomah 10/14/2019 08:16:42
For python 2.7 there als o is an error


root@93d179b72523:/webots/resources/web/server# Traceback (most recent call last):

  File "./session\_server.py", line 26, in <module>

    import tornado.ioloop

  File "/usr/local/lib/python2.7/dist-packages/tornado/ioloop.py", line 67

    def fileno(self) -> int:

                     ^

SyntaxError: invalid syntax

Traceback (most recent call last):

  File "./simulation\_server.py", line 37, in <module>

    import tornado.ioloop

  File "/usr/local/lib/python2.7/dist-packages/tornado/ioloop.py", line 67

    def fileno(self) -> int:

                     ^

SyntaxError: invalid syntax


I am going to try your PR

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 08:20:09
that's strange.. I'm running this script with python2 without any issue..

##### Choomah 10/14/2019 08:20:32
pip install tornado websocket-client nvidia-ml-py psutil requests


This the revision branch

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 08:22:50
yes, this is correct

##### Choomah 10/14/2019 08:27:00
I am trying your patch on Python3

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 08:27:32
Ok! Let me know if it works

##### Choomah 10/14/2019 08:39:03
nice!


/monitor works on both ports



%figure
![Screen_Shot_2019-10-14_at_10.39.28.png](https://cdn.discordapp.com/attachments/565154703139405824/633222386082381834/Screen_Shot_2019-10-14_at_10.39.28.png)
%end


We are getting there slowly. You are a hero Stefania


apparently I am still missing some files. I am looking for them in the repo


Whoops


too early ^\_^


2019-10-14 08:42:56,855 [ERROR  ]  Uncaught exception GET / (172.20.0.1)

HTTPServerRequest(protocol='http', host='localhost:1999', method='GET', uri='/', version='HTTP/1.1', remote\_ip='172.20.0.1')

Traceback (most recent call last):

  File "/usr/local/lib/python3.5/dist-packages/tornado/websocket.py", line 649, in \_run\_callback

    result = callback(*args, **kwargs)

  File "/usr/local/lib/python3.5/dist-packages/tornado/websocket.py", line 427, in on\_message

    raise NotImplementedError

NotImplementedError

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 08:43:20
The code for Webots 2020a (not released yet) is still a work in progress... Are you using Webots from the revision ro develop branch?

##### Choomah 10/14/2019 08:43:32
yeah revisions


but now I am using your branch


hotfix-linux-launcher

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 08:45:38
I would suggest you to simply apply the patch of the simulation\_server.py file to the revision branch

##### Choomah 10/14/2019 08:45:54
ok

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 08:45:57
and then use the files from [https://www.cyberbotics.com/files/repository/www/wwi/R2019b/](https://www.cyberbotics.com/files/repository/www/wwi/R2019b/)


I will create a new PR to apply the fix to the revision branch

##### Willem3141 10/14/2019 11:39:45
I'm currently experimenting with modifying the e-puck robot window, to allow connecting to an e-puck 2 by Bluetooth, without having to switch to an e-puck 1


I was wondering, to make debugging easier, is there a way to open a HTML/JS debugger for the robot window?


I found the documentation at [https://www.cyberbotics.com/doc/guide/controller-plugin#robot-window](https://www.cyberbotics.com/doc/guide/controller-plugin#robot-window), is there some other documentation about this that I'm missing?

##### Choomah 10/14/2019 12:04:43
`@Stefania Pedrazzi` i found the bug. I will push a PR ontop of your PR

##### David Mansolino [Cyberbotics] 10/14/2019 12:19:12
`@Willem3141`, if you recompile Webots from sources, you should be able to activate the robot widnow web inspector by uncommenting these parts:

[https://github.com/cyberbotics/webots/blob/c4ce84200caa0725655d1538076099d2fc036a22/src/webots/gui/WbRobotWindow.cpp#L38](https://github.com/cyberbotics/webots/blob/c4ce84200caa0725655d1538076099d2fc036a22/src/webots/gui/WbRobotWindow.cpp#L38)

[https://github.com/cyberbotics/webots/blob/c4ce84200caa0725655d1538076099d2fc036a22/src/webots/gui/WbRobotWindow.cpp#L91](https://github.com/cyberbotics/webots/blob/c4ce84200caa0725655d1538076099d2fc036a22/src/webots/gui/WbRobotWindow.cpp#L91)

##### Willem3141 10/14/2019 12:20:13
Thanks! I will try that

##### David Mansolino [Cyberbotics] 10/14/2019 12:20:19
You're welcome

##### Choomah 10/14/2019 12:25:03
O\_o
%figure
![Screen_Shot_2019-10-14_at_14.24.42.png](https://cdn.discordapp.com/attachments/565154703139405824/633279102496931841/Screen_Shot_2019-10-14_at_14.24.42.png)
%end


Not a clue what's going on here


2019-10-14 12:26:14,014 [INFO   ]  [localhost:1999] Client disconnected

2019-10-14 12:26:14,619 [INFO   ]  101 GET / (172.20.0.1) 0.88ms

2019-10-14 12:26:14,621 [INFO   ]  [localhost:1999] New client


The console states: WebSocket error: Unknown message received: "x3d;broadcast"

##### John0911 10/14/2019 13:01:36
I want to know whether the signal strength will be affected if more than one emitter send message


`@Olivier Michel` I have known the supervisor can rotation with one axis. However, I want to know whether the supervisor  can rotation with a defined axis.

##### Olivier Michel [Cyberbotics] 10/14/2019 13:09:42
Sure, you can define whatever rotation axis you need.


This page can help you to define the rotation axis in VRML format: [https://cyberbotics.com/rotation\_tool](https://cyberbotics.com/rotation_tool)

##### John0911 10/14/2019 13:31:46
If a cylinder shape robot rotation with a defined axis but not y axis, how can I achieve this?


And this axis is parallel to y axis.

##### Olivier Michel [Cyberbotics] 10/14/2019 13:33:59
... not sure to understand what you want to achieve...

##### John0911 10/14/2019 13:37:42
I want let a cylinder supervisor rotation with a axis parallel to y axis and do not use hingejoint node,  how can I set supervisor rotation parameters?


If the Supervisor robot rotates around a defined axis parallel to the Y-axis and does not apply the differential drive method, how should the rotation parameters of the Supervisor node be set?


`@Olivier Michel`  The axis does not pass through the origin of the robot


Because the hingejoint node need a lot of sources of computer, however, we want simulate with about 49 robots. So we need simplify the robot's model.

##### Choomah 10/14/2019 14:02:36
`@Stefania Pedrazzi` A little bit progress again here


now the webots subprocess is defunct as soon as it starts


Process log


Warning: QXcbConnection: Could not connect to display :0

Critical: Could not connect to any X display.

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 14:18:04
`@Choomah` the error about `The console states: WebSocket error: Unknown message received: "x3d;broadcast"` is probably because the Webots version and the `webots.min.js` version are not matching

##### Choomah 10/14/2019 14:18:25
that seems to be fixed now

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 14:19:25
you can try to comment line `os.environ["DISPLAY"] = ":0"` (about line 702) in simulation\_server.py


this is needed to run on a headless server machine, but for example on my system with dual screen this instruction doesn't work

##### Choomah 10/14/2019 14:20:52
I am running it headless in a docker container


Ah that worked semi ^\_^


I commented the DISPLAY line out


now new errors incoming


AL lib: (WW) alc\_initconfig: Failed to initialize backend "pulse"

ALSA lib confmisc.c:768:(parse\_card) cannot find card '0'

ALSA lib conf.c:4292:(\_snd\_config\_evaluate) function snd\_func\_card\_driver returned error: No such file or directory

ALSA lib confmisc.c:392:(snd\_func\_concat) error evaluating strings


etc...

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 14:24:46
I never got these ones... if you check in the `<webots>/resources/web/server/log/simulation/output.log` file you can get the exact instruction that is run by the simulation server. Did you try to run Webots directly and see if it works?

##### Choomah 10/14/2019 14:31:56
I think something good happened


Docker has become useless, which I guess is due to the 10 webots servers running in the background


I have a very simple machine

##### Willem3141 10/14/2019 14:56:20
`@David Mansolino` Unfortunately I can't get Webots to compile on Linux with the inspector code enabled, because `<QtWebKitWidgets/QWebInspector>` cannot be found (my understanding is that Webots uses `QtWebEngine` instead of `QtWebKit` on Linux). I then found out that you can set `QTWEBENGINE_REMOTE_DEBUGGING` to some port number, and open `http://localhost:<port>` in Chrome to do remote debugging. Unfortunately this doesn't work either: after choosing the page to inspect, it only results in a blank page 😦


Anyone has any experience with `QTWEBENGINE_REMOTE_DEBUGGING`?

##### JoHn 10/14/2019 14:56:55
Hey guys, now I am able to control one vehicle via a ROS node. My question is, is it possible to control multiple vehicles via ROS at the same time? Should each vehicle have one ROS node to control it? Thank you very much for your help.

##### David Mansolino [Cyberbotics] 10/14/2019 14:58:00
`@JoHn`, as you prefeer you can either control on vehicle by node either control several vehicles from the same node (in that case you have to call the corresponding services for each vehicles)


`@Willem3141` oh yes you are completely right `QtWebKitWidgets` is valid on windows only


about `QTWEBENGINE_REMOTE_DEBUGGING` you should actually be able to start Webots with the `--remote-debugging-port=....` port. See this link for more information: [https://doc.qt.io/qt-5/qtwebengine-debugging.html](https://doc.qt.io/qt-5/qtwebengine-debugging.html)

##### JoHn 10/14/2019 15:05:52
Thank you `@David Mansolino` for your answer. I tried to control two vehicles in a single ROS node. However, sometimes only one vehicle moved or both vehicles did not move. I am not sure what mistake I made. I am guessing the problem may come from the 'controllername'. Would you give me some advice?

##### David Mansolino [Cyberbotics] 10/14/2019 15:08:21
You probably want to add the `--name=<robot_unique_name>.` argument to both controller with different unique\_names to have always the exact same services/topics names: [https://cyberbotics.com/doc/guide/using-ros#using-the-standard-ros-controller](https://cyberbotics.com/doc/guide/using-ros#using-the-standard-ros-controller)

##### Choomah 10/14/2019 15:09:23
`@Stefania Pedrazzi` [https://forums.docker.com/t/cant-make-audio-device-to-work-on-aws-solved/71852/3](https://forums.docker.com/t/cant-make-audio-device-to-work-on-aws-solved/71852/3)


in case you are interested



%figure
![Screen_Shot_2019-10-14_at_17.09.39.png](https://cdn.discordapp.com/attachments/565154703139405824/633320585640017940/Screen_Shot_2019-10-14_at_17.09.39.png)
%end


😀 thanks

##### Willem3141 10/14/2019 15:10:25
`--remote-debugging-port=...` behaves the same as setting `QTWEBENGINE_REMOTE_DEBUGGING`, that is: you get a list with inspectable pages (in my case: the robot window and a user guide viewer), but clicking on either of these just results in a blank window. Webots returns an empty HTTP response 😕


`@David Mansolino` Thanks for the help so far. I think I'll give up on this for today, let's see if I can fix things another day 🙂

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 15:12:31
`@Choomah` Cool! thank you for the link! I will check to add this to the documentation

##### David Mansolino [Cyberbotics] 10/14/2019 15:24:55
`@Willem3141`, you're welcome. If you want you can re-ask the question on Wednesday, one of me colleague which is way more aware of the debugging mechanism of the robot-window will be connected.

##### JoHn 10/14/2019 15:30:56
Thank you `@David Mansolino`.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/633325882257440768/unknown.png)
%end


Thank you `@David Mansolino`. Is this a correct way to add the argument?

##### David Mansolino [Cyberbotics] 10/14/2019 15:32:18
yes, but you have to replace `<robot_unique_name>` by something else (a name that you choose and that is different for each vehicle)

##### Choomah 10/14/2019 15:32:39
`@Stefania Pedrazzi` are there certein worlds that contain the Code Editor? The option is now disabled.

##### JoHn 10/14/2019 15:34:22
Thank you `@David Mansolino`. I see. I kind of understand it now. I am giving it a shot.

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 15:36:53
`@Choomah` yes, the "Edit controller" option in the robot context menu of the web interface should be enabled if the "controller" field is visible in the Webots scene tree

##### Choomah 10/14/2019 15:38:03
Euhm....

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 15:38:11
Is the simulation play button enabled?

##### Choomah 10/14/2019 15:38:17
no

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 15:38:42
ok, so you are probably running the simulation in broadcast mode.

##### Choomah 10/14/2019 15:39:49
ah ok

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 15:40:19
You should set `view.broadcast = false;` or simply do not set it (for the webots.View instance).

##### Choomah 10/14/2019 15:43:05
nice!


is there a way for me to start the code editor immidiatly after the scene is loaded in a predefined div ?

##### David Mansolino [Cyberbotics] 10/14/2019 15:43:42
`@JoHn` you're welcome

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 15:44:36
> is there a way for me to start the code editor immidiatly after the scene is loaded in a predefined div ?

no. you have to modify `webots.min.js` to do this

##### Choomah 10/14/2019 15:45:41
that would the webots.js and than run make right ?

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 15:45:41
probably unsetting the broadcast value is not enough

##### Choomah 10/14/2019 15:46:03
It was


it is working now

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 15:46:13
ok, nice

##### Choomah 10/14/2019 15:46:23
controls are available, Editor is availble


Simulation does NOT run yet

##### Stefania Pedrazzi [Cyberbotics] 10/14/2019 15:47:27
> that would the webots.js and than run make right ?

yes, the make  bundles the dependencies and minimize the JS file. Then you have to make sure to link to your own webots.min.js file

##### Choomah 10/14/2019 16:08:57
One more hurdle
%figure
![Screen_Shot_2019-10-14_at_18.08.42.png](https://cdn.discordapp.com/attachments/565154703139405824/633335448969936916/Screen_Shot_2019-10-14_at_18.08.42.png)
%end


`@Stefania Pedrazzi` Thanks for the help man!


If anybody can point me in the right direction


I still encounter two problems: 1. can't load the textures. It times out. 2. simulation doesn't run

##### Mehran Raisi 10/14/2019 20:29:28
Hi everyone, I'm using pr2 robot in my project and I want to derive its Denavit Hartenberg parameters from webots. I know in robots proto, there exists its coordinates but its really hard to work with. If anyone has its parameters, I would appreciate to share these data. thanks.

##### Stefania Pedrazzi [Cyberbotics] 10/15/2019 06:15:42
`@Choomah` 

1. did you try to connect directly to the textures URL? does it work?

2. what happens exactly when you click on the play button? and the step button?

I would suggest you to clear the browser cache, this helps sometimes


`@Mehran Raisi` about the PR2 Webots model: instead of using the data from the PROTO file you could

1. convert it to base nodes: right click on the PR2 item in the scene tree and "Convert to base node(s)"

2. if you select a Solid or device node, then you can get the absolute or relative position of the part in the "Position" tab of the field editor (just below the scene tree)

##### Choomah 10/15/2019 06:32:13
`@Stefania Pedrazzi` 1. I get an Empty response if I visit the root. I get an 404 when I visit the texture file

##### Stefania Pedrazzi [Cyberbotics] 10/15/2019 06:32:48
The texture file should be automatically published by Webots

which version are you using? always the revision one?

##### Choomah 10/15/2019 06:34:27
always the revision one with you hotfix patch applied



> **Attachment**: [webots.mov](https://cdn.discordapp.com/attachments/565154703139405824/633553420891979786/webots.mov)


I thought it might be better if you heard my sexy accent


😋



%figure
![Screen_Shot_2019-10-15_at_08.41.55.png](https://cdn.discordapp.com/attachments/565154703139405824/633555220600848384/Screen_Shot_2019-10-15_at_08.41.55.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 10/15/2019 06:43:58
Did you clear the cache?

to debug this you should inspect what does the Webots instance. First step would be to simply start webots with the `--stream` option and try to connect to the texture (with default port at http://localhost:1234/textures/<texture path>)

##### Choomah 10/15/2019 06:48:37
cache is cleared I will restart the computer just to be sure


/root/webots/webots --batch --mode=pause --minimize --no-sandbox --stdout --stderr --stream=port=2001;monitorActivity;controllerEdit /root/webots/projects/samples/robotbenchmark/robot\_programming/worlds/robot\_programming.wbt


Gives empty response on localhost:2001


brb

##### Stefania Pedrazzi [Cyberbotics] 10/15/2019 06:55:32
on localhost:2001 it is normal that doesn't find anything, but for example this URL should work http://localhost:2001/textures/conveyor\_belt\_rubber.jpg


if it doesn't then you should inspect if Webots prints any error message (in the Webots console or in the terminal)


I just tested on revision and it works just fine on my machine

##### Choomah 10/15/2019 07:13:18
so you ran the same command and you can access the texture files?


hmmm...

##### Stefania Pedrazzi [Cyberbotics] 10/15/2019 07:14:29
yes

##### Choomah 10/15/2019 07:17:54
does the streaming server write any logs ?


no logs in /var/log

##### Stefania Pedrazzi [Cyberbotics] 10/15/2019 07:19:25
the streaming server writes logs by default in the <webots>/resources/web/server/log folder. But here the problem seem to come from Webots itself


Webots writes errors to the stderr/stdout and in the Webots console


one thing you could also try is to test a different port

##### Choomah 10/15/2019 07:24:47
Tried different port. same result


It's a shame we couldn't get it to work


I'll try later again


Hereby the promised PR: [https://github.com/cyberbotics/webots/pull/1006](https://github.com/cyberbotics/webots/pull/1006)


In case you might be curious: [https://github.com/trackq/webots/tree/dev-kaweh](https://github.com/trackq/webots/tree/dev-kaweh)


Thanks for helping a brother out `@Stefania Pedrazzi`

##### Stefania Pedrazzi [Cyberbotics] 10/15/2019 08:09:22
`@Choomah`  you're welcome. I will look at your PR

##### Choomah 10/15/2019 08:12:00
[https://gist.github.com/trackq/32082c69d7dc16ede2e8d5ba3cafef0e](https://gist.github.com/trackq/32082c69d7dc16ede2e8d5ba3cafef0e)


`@Stefania Pedrazzi` I push this container to a repo if you want

##### JoHn 10/15/2019 14:26:10
Hi `@David Mansolino`, I am controlling two trucks by using one ROS node. As you suggested yesterday, I put "--name=leader" and "--name=aaa" in the 'controllerArgs' of each truck. Then in the ROS node, I wrote the code as shown in the picture. But it did not work, the simulation time just freezed. Could you please give me some advice?



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/633672000581402664/unknown.png)
%end

##### David Mansolino [Cyberbotics] 10/15/2019 14:27:16
Do you call the step service for each node?


[https://cyberbotics.com/doc/reference/robot?tab=ros#wb\_robot\_step](https://cyberbotics.com/doc/reference/robot?tab=ros#wb_robot_step)

##### JoHn 10/15/2019 14:29:14
Are you saying this 'timeStepClient.call(timeStepSrv) '?


I need to call timeStepClient for both vehicles, is that right?

##### David Mansolino [Cyberbotics] 10/15/2019 14:33:40
Yes exactly once per vehicle


with a positive integer value for the 'value' argument.

##### JoHn 10/15/2019 14:35:27
Thank you `@David Mansolino` very much. I am trying it. I think it will work.

##### David Mansolino [Cyberbotics] 10/15/2019 14:35:49
You're welcome

##### JoHn 10/15/2019 15:15:59
Hi `@David Mansolino`, I called timeStepClient for both vehicles at the end of the ROS node, and I launched the node, the simulation time still freezed as follows:



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/633684526635155466/unknown.png)
%end


Is the setting for the truck wrong?

##### David Mansolino [Cyberbotics] 10/15/2019 15:25:02
Your setting seems correct. How do you call the timeStepClient? In a loop? with which value?

##### JoHn 10/15/2019 15:29:01
`@David Mansolino`, yes, I call the timeStepClient in a loop with 32.



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/633687807394840606/unknown.png)
%end

##### Mehran Raisi 10/15/2019 16:35:57
`@Stefania Pedrazzi` Best Regards.

##### JoHn 10/15/2019 16:49:03
Hi `@David Mansolino`, I reduces the TimeStep value from 32 to 16, it works now.


Thank you very much.


One quick question,
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/633708086036660225/unknown.png)
%end


is it because there is a steering limit (from -0.4 to 0.4)?

##### David Mansolino [Cyberbotics] 10/15/2019 17:08:36
> Thank you very much.



Very good news.



> is it because there is a steering limit (from -0.4 to 0.4)?



Yes exactly, the steering angle is probably limited to -0.4 to +0.4 in the vehicle model you are using. Which model are you using?

##### JoHn 10/15/2019 17:19:01
Thank you very much for your help `@David Mansolino`. I am using the truck model as shown below:



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/633715484524675091/unknown.png)
%end

##### David Mansolino [Cyberbotics] 10/16/2019 05:51:14
`@JoHn`, I just checked and for the Truck model it is indeed limited to -0.4 to + 0.4, if needed you can change this here: [https://github.com/cyberbotics/webots/blob/revision/projects/vehicles/protos/generic/Truck.proto#L36](https://github.com/cyberbotics/webots/blob/revision/projects/vehicles/protos/generic/Truck.proto#L36)

##### abcde 10/17/2019 10:12:45
guys does anyone know how to do inverse kinematics in general

##### David Mansolino [Cyberbotics] 10/17/2019 10:14:15
Hi `@abcde`, you can use libraries that do this for you, we have for example some experience with ikpy (this simulation is released as open-source within Webots): [https://www.youtube.com/watch?v=Jq0-DkEwwj4](https://www.youtube.com/watch?v=Jq0-DkEwwj4)

##### abcde 10/17/2019 10:16:14
thank u

##### David Mansolino [Cyberbotics] 10/17/2019 10:16:20
You're welcome

##### jhielson 10/18/2019 15:31:48
Hi, I am trying to improve the speed of my simulation in order to reduce the training time of my model.  I saw that I can increase the WorldInfo.basicTimeStep value but can it be greater than my current wb\_robot\_step. For example, my wb\_robot\_step value is 256 and I would like to use 2048 as my basicTimeStep.

##### David Mansolino [Cyberbotics] 10/18/2019 15:33:45
Hi `@jhielson`, unfortunately no, the WorldInfo.basicTimeStep shouldn't bee bigger than your controller tiem step.


Note also that 256 is already a quite big value, usign higher values may make your simulation unstable/unprecise.


But there are probably other solution to make your simulation faster, have you read this page: [https://cyberbotics.com/doc/guide/speed-performance](https://cyberbotics.com/doc/guide/speed-performance) ?

##### jhielson 10/18/2019 15:35:25
Thanks, I just noticed it was not working well. I am gonna check the link.

##### David Mansolino [Cyberbotics] 10/18/2019 15:35:37
You're welcome

##### Marcey 10/19/2019 11:58:46
Short question: if I add a noise to a Lidar, does the appearance (colour, reflections, ...) of other objects have any influence?

##### dulla 10/20/2019 23:47:23
Hello, i have just come across this website as i was looking for website that are about making a robot design


is this what the website is about?


like it shows which parts you need to assemble the robot


?

##### Olivier Michel [Cyberbotics] 10/21/2019 06:59:09
`@Marcey`: The appearance of objects (color, reflections, etc.) has no impact on the Lidar measurements. The Lidar only uses depth of field to compute its return values (based on the OpenGL depth buffer, also called Z buffer), in addition to the optional noise value.


`@dulla`: Yes, Webots is a useful tool to design your own robot in simulation, program it and see how it will interact with its environment.

##### dreamerz 10/21/2019 09:32:51
hi, im trying this software out but it only shows a white screen, if i screenshot i can see but not on the live screen, any remedies

##### Fabien Rohrer [Moderator] 10/21/2019 09:34:01
`@dreamerz` Hi, could you follow these items? [https://www.cyberbotics.com/doc/guide/general-faq#webots-crashes-at-startup-what-can-i-do](https://www.cyberbotics.com/doc/guide/general-faq#webots-crashes-at-startup-what-can-i-do)

##### fateme 10/23/2019 07:34:03
hello


i write two program in c in webots and in matlab but after i synchronize by write in files this 2 program my project run very slow  .  how can i increase the speed


?

##### David Mansolino [Cyberbotics] 10/23/2019 07:37:09
Hi `@fateme`, have you tried to run each program separately to make sure that both controller sparately runs fine?

##### fateme 10/23/2019 07:37:58
hi `@David Mansolino`  yes

##### David Mansolino [Cyberbotics] 10/23/2019 07:38:24
Ok, both program are Webots cotnroller right?

##### fateme 10/23/2019 07:38:46
no one in webots and another in matlab app

##### David Mansolino [Cyberbotics] 10/23/2019 07:39:06
Ok, then why aren't you using directly a Matlab controller?

##### fateme 10/23/2019 07:39:47
because i use toolbox of matlab

##### David Mansolino [Cyberbotics] 10/23/2019 07:40:51
Ok, in that case you may need other way to communicate between Webots and Matlab than files (which are indeed quite slow). Have you tried using TCPIP for example? Or pipes?

##### fateme 10/23/2019 07:41:14
no

##### David Mansolino [Cyberbotics] 10/23/2019 07:42:56
In that case that's probably the way to go for better performances.

##### fateme 10/23/2019 07:44:12
if i want use another way except file what way you suggest?


the amount of time\_step in wb\_robot\_step(time\_step) is effective in run time?

##### David Mansolino [Cyberbotics] 10/23/2019 07:45:59
If you are in real\_time mode then Webots will try to match as much as possible the amount time\_step to the real time.


> if i want use another way except file what way you suggest?



I am not an expert in Matlab, but you should check what is the simplest to implemetn in Matlab between pipes and TCPIP communication.

##### fateme 10/23/2019 07:47:21
ok thank you David

##### David Mansolino [Cyberbotics] 10/23/2019 07:47:27
You're welcome

##### Marcey 10/23/2019 21:48:05
I have another question.  I am building a PROTO-Node right now. The PROTO is of the type Group. I have a field called numberofNodes and in the protoBody I want to use a if-function as followed:



Group{

  children[

     Node1{}

if fields.numberofNodes.value > 1 then

    Node2{}

end

if fields.numberofNodes.value > 2 then

    Node3{}

end



and so on. I hope you can understand, what I want to do. It doesn't work like this, but do you have an idea how to do it? Thanks 🙂



Edit: Solved it. Just add %{}% to if and end

##### Samir Hosny Mohamed 10/23/2019 22:54:34
Hello

i have an weird issue!

the program is closing while finalizing nodes!

how can i fix it ?

##### Gordon 10/24/2019 04:07:08
Hi Experts on this channel


How can I make another LiDAR module? I am trying to make a Lidar modele from SICK. MRS6124 or MRS6000

##### David Mansolino [Cyberbotics] 10/24/2019 06:30:03
`@Samir Hosny Mohamed` Have you tried to use the safe mode? [https://cyberbotics.com/doc/guide/starting-webots#safe-mode](https://cyberbotics.com/doc/guide/starting-webots#safe-mode)


`@Gordon`, if you want to use a Lidar model that is not already available in Webots, you should use the default Lidar node and then set the fields of this node to matches the specification of the sick MRS6****: [https://cyberbotics.com/doc/reference/lidar](https://cyberbotics.com/doc/reference/lidar)

Let us know if you have any specific issue doing so.

And do not hesitate to share your result on github if you want: [https://github.com/cyberbotics/webots](https://github.com/cyberbotics/webots)

##### Samir Hosny Mohamed 10/24/2019 06:32:27
No, i'll try as soon as i can

Thanks in advance

##### David Mansolino [Cyberbotics] 10/24/2019 06:33:07
`@Samir Hosny Mohamed`, you're welcome. Let us know if this still doesn't work. And make also sure that your GPU drivers are up to date.

##### Olivier Michel [Cyberbotics] 10/24/2019 06:36:00
`@Gordon`: you may take inspiration from the existing Lidars already included in Webots: [https://github.com/cyberbotics/webots/tree/revision/projects/devices/sick/protos](https://github.com/cyberbotics/webots/tree/revision/projects/devices/sick/protos) and create your own proto file your the MRS6124/MRS6000 Lidars.

##### Samir Hosny Mohamed 10/24/2019 07:07:08
`@David Mansolino` I tried the safe mode and it worked 👌 thanks.

Then i tried to remove the environment variable and it worked also

##### David Mansolino [Cyberbotics] 10/24/2019 07:07:35
Ok, very good news !

##### Gordon 10/24/2019 07:18:16
`@David Mansolino` Thanks for your help. 

`@Olivier Michel` I am modifying the MRS8 to MRS6124 now. I need more understanding of the parameters of LiDAR


I have some issues with RVIZ for not able to use tf data from webots\_ros on a remote machine. Is there anyone experiencing such problem? I can get most of the topics diaplayed to rviz, not tf. I can see tf comes by topic monitoring tools from rqt.

##### David Mansolino [Cyberbotics] 10/24/2019 07:25:15
`@Gordon`, have you tried the 'Pioneer 3 AT' webots-ros sample that use tf just to check if it works fine with your setup: [http://wiki.ros.org/webots\_ros/Tutorials/Sample%20Simulations#Simulation\_Pionneer\_3\_AT](http://wiki.ros.org/webots_ros/Tutorials/Sample%20Simulations#Simulation_Pionneer_3_AT)  ?

##### Gordon 10/24/2019 23:25:48
`@David Mansolino`  Thanks for your reply, I have used Pioneer 3AT example, and I could see all topics in RVIZ and RQT topic monitor on Host computer, However, remote computer RVIZ did not pick up any tf related topics. Rqt topic monitor on remote shows tf related topics. Therefore, there are something with Rviz on Remote.

##### David Mansolino [Cyberbotics] 10/25/2019 07:25:57
`@Gordon` ok, good to know that the problem is more likely on the Rviz side and not on the Webots-ROS interface side.

##### POTATO\_CHIP 10/25/2019 12:45:08
hi,How can webots generate the .Hex profile?Can someone help?Thanks.

##### David Mansolino [Cyberbotics] 10/25/2019 12:55:41
Hi `@POTATO_CHIP` you can use cross-compilation for the e-puck robot: [https://cyberbotics.com/doc/guide/epuck#cross-compilation](https://cyberbotics.com/doc/guide/epuck#cross-compilation)

##### JustDN 10/25/2019 16:59:59
Is there any Webots documentation on cartesian control for the NAO robot? Or is there any way to program the NAO robot to move parts using [x y z] coordinates?

##### POTATO\_CHIP 10/26/2019 07:46:49
Hi,When I upload the .HEX file to the e-puck,after press the reset botton ,but it always show uploading and nothing happen.`@David Mansolino`


btw,after press the reset botton,the statu light turn green.


and the uploading stripe is grey.



%figure
![pressreset.png](https://cdn.discordapp.com/attachments/565154703139405824/637562841913622538/pressreset.png)
%end



%figure
![grey.png](https://cdn.discordapp.com/attachments/565154703139405824/637563118955790368/grey.png)
%end

##### threeal 10/27/2019 14:43:50
how to manually compile a controller in Webots using make command?


already solved it with create a new WEBOTS\_HOME variable that contain my webots path and setup ld config to Webots library path


but now i have a problem to link visual studio code with webots include files


well seem i got it with creating dynamic link between webots include folder to my default include folder "/usr/local/include"


thank you guys

##### Fabien Rohrer [Moderator] 10/28/2019 07:49:03
`@threeal` you’re welcome 😂

##### David Mansolino [Cyberbotics] 10/28/2019 12:33:03
`@POTATO_CHIP` which version of the e-puck are you using?


`@JustDN` unfortunately no, but you might use invere kinematic to do this.

##### Gautier 10/28/2019 12:56:04
Hello


I have a problem  : I'm trying to understand how a robot with multiple other robots inside its children node (with, for each one, a different control) behaves, but I can't really make sense of the simulation I just did

##### David Mansolino [Cyberbotics] 10/28/2019 12:57:32
Hi `@Gautier` what is the behavior that doesn't make sense for you?

##### Gautier 10/28/2019 12:58:05
I tried to run a puma robotic arm on a pioner, but even when the puma doesn't have any controller, a part of the robot "glitches" and there is some kind of offset between its links
> **Attachment**: [show-2019-10-28\_12.07.26](https://cdn.discordapp.com/attachments/565154703139405824/638360842689183754/show-2019-10-28_12.07.26)


What I would like to do, to put it in a nutshell, is see if I can use a robot for "moving", and another robot for "sensor" or manipulation, with each one having seperate controllers

##### David Mansolino [Cyberbotics] 10/28/2019 13:04:05
The behavior is indeed not the expected one!

Are you shure all your Solid/robot nodes in the hierarchy have both a bounding object and a physics defined?

##### Gautier 10/28/2019 13:16:00
Thank you, you were right, a bonding box was missing


The behavior seems more coherent now

##### David Mansolino [Cyberbotics] 10/28/2019 13:24:34
Perfect! thank you for the feedback 🙂

##### Gautier 10/28/2019 13:26:01
So, I modified as you said. But there's still a thing I can't understand. The robot to behaves likes the "puma arm" was very heavy, when I explicitely defined all the masses as 0.1kg (and the mass of the pioneer of something like 50kg). It's especially wired when it seems that the change of direction of the Puma seems to impact the inertia force of the "whole robot" (which is good and excepted !)
> **Attachment**: [show-2019-10-28\_14.22.21](https://cdn.discordapp.com/attachments/565154703139405824/638367874502885416/show-2019-10-28_14.22.21)


Basically, i have to take the inertia matrix of the pionneer as something along -6m on the y axis for it to work properly


Here is the complete archive if you want to take a look 😅 I'm sorry to annoy you
> **Attachment**: [robotOnRobot.zip](https://cdn.discordapp.com/attachments/565154703139405824/638368600675188756/robotOnRobot.zip)

##### David Mansolino [Cyberbotics] 10/28/2019 13:29:02
What you are changing there, is the center mass of the base segment of the robot, but all the children segment still have their center of mass up.

##### Gautier 10/28/2019 13:30:14
Yes, I agree. But because I set a masse of the pionner 500x time superior than the masses of the Puma, the Puma should have a negligible impact, shouldn't it ?


Or did a make a mistake somewhere else ?

##### David Mansolino [Cyberbotics] 10/28/2019 13:31:04
Let me check quiclky your simulation files


I don't understand qhy you have 3 robots in your hierarchy.


the pioneer should be the rrot robot and the puma a direct children of the pioneer.


Additionnally, the total mass of the pioneer is 42.3kg and of the puma 27.4, therefore the difference is not so big

##### Gautier 10/28/2019 13:41:56
Thank you !


For the problem of the three robots, well, it's because I want to see how it would behaves if two "concurrent robot" with each one having its own controller would behave. It's for a project of a simulator, where mesh would be simulated as robot to simulate very little deformation of their mesh (and IndexFaceSet) for exemple


How can you check the total weight of a robot ? I was pretty sure I changed the mass on all the field of the Puma

##### David Mansolino [Cyberbotics] 10/28/2019 13:46:20
If you select any Robot/Solid node in the scene-tree you can then go to the node editor in the 'mass' field ans select 'including descendants', it will then compute the total mass of the robot/solid with all of its descendants:



%figure
![12.png](https://cdn.discordapp.com/attachments/565154703139405824/638373015230480391/12.png)
%end

##### Gautier 10/28/2019 14:02:14
Ok, thank you a lot !


I corrected it, it seems to be workuing


working fine now* !

##### David Mansolino [Cyberbotics] 10/28/2019 14:02:41
Perfect, you're welcome

##### Amr 10/28/2019 15:25:29
Is there is a world of a city with detailed sidewalks to simulate a wheeled robot performing last mile delivery?


[https://www.youtube.com/watch?v=IfkxpnD9VzM](https://www.youtube.com/watch?v=IfkxpnD9VzM)


check this link as an example for what I am looking for and thanks in advance

##### POTATO\_CHIP 10/28/2019 15:28:36
`@David Mansolino` e-puck2

##### David Mansolino [Cyberbotics] 10/28/2019 15:37:53
`@Amr`, you  might be interested by the 'village\_center' world.
%figure
![22.png](https://cdn.discordapp.com/attachments/565154703139405824/638401055226331146/22.png)
%end



%figure
![21.png](https://cdn.discordapp.com/attachments/565154703139405824/638401059353657354/21.png)
%end


`@POTATO_CHIP`, unfortunately the cross-compilation is supported only for the e-puck version 1 for now.

##### Gautier 10/28/2019 15:52:42
Hey, sorry to bother you again, but I have a question. A solid, that has another solid has a children, would "inherit" the boundingObject of its children (in addition to his own) ?

##### David Mansolino [Cyberbotics] 10/28/2019 15:59:16
Yes, if it's a direct children (no joint between them) the two solids will kind of 'merge' and form on solid only.

##### Gautier 10/28/2019 16:00:19
Ok, thank you !

##### threeal 10/28/2019 17:30:24
when a world has 2 robots, does it treats each robot as separate node when using ros controllers?

##### Tahir [Moderator] 10/28/2019 17:34:35
`@threeal` Yes, different robots are treated sepreately, They will have all the information in different ros services (namespaced by the robots name you specify)

##### threeal 10/28/2019 19:55:27
and how do i know if i subsribe a message from one robot and not the other?


and in the e-puck sample, i tried to change the robot name, it changes the topic name of the robot, but the rosnode that control the robot could still able to send a message to webots simulator even if it has different topic name. how could it happens?

##### Tahir [Moderator] 10/28/2019 20:10:04
`@threeal`  when you change the robot name and add corresponding arguments in the controller args field then every information changes from rosnodes, topics and list.


By changing the robot name the all ros names are appended by robot name



> **Attachment**: [E-Puck\_ros.zip](https://cdn.discordapp.com/attachments/565154703139405824/638469847151542341/E-Puck_ros.zip)


You can check here in the world file protos are the same with different naming arguments so everything changes accordingly


you can check it with rosnode list, rosservice list etc.


and connection of nodes you can check by rqt\_graph

##### Gordon 10/29/2019 06:55:01
`@David Mansolino` I have got errors which is with Rviz crashing the when it try to read Pointcloud topic which was created from Webot\_ros. It seems to be with "ros Assertion `(min.x <= max.x && min.y <= max.y && min.z <= max.z) && "The minimum corner of the box must be less than or equal to maximum corner"' failed."

I suspect this is due to some erroneous point cloud, I can see 11MB/s of point clouds are created at 32 Hz. but some or all may have some errorneous points which crashes Rviz.

Is there any method to validate or fix point cloud from webots side?


Is there any method publishing sensor\_msgs::PointCloud2? instead of PointCloud?

##### David Mansolino [Cyberbotics] 10/29/2019 07:25:23
`@Gordon`, unfortunately, only `sensor_msgs::LaserScan` and `sensor_msgs::PointCloud` are supported yet. However, you should be able to change/extend this quite easily. The publication fo the `sensor_msgs::PointCloud` is done here: [https://github.com/cyberbotics/webots/blob/revision/projects/default/controllers/ros/RosLidar.cpp#L121](https://github.com/cyberbotics/webots/blob/revision/projects/default/controllers/ros/RosLidar.cpp#L121)

In this function you should be able to extend to `sensor_msgs::PointCloud2` and then you have to recompile the `ros` controller (let me know if you have any issue doing so).

##### Gautier 10/29/2019 08:44:09
Concerning the implicit solid merging indicated on that page ([https://cyberbotics.com/doc/reference/physics](https://cyberbotics.com/doc/reference/physics)), does that mean that the upper (most high parent in the node hiearchy) retains all the information of mass and bounding box of all its children solid ?


(Do you have any more reference on how the physics engine works and merge the bounding box / the masse of the differents solid ?)

##### David Mansolino [Cyberbotics] 10/29/2019 08:49:30
> does that mean that the upper (most high parent in the node hiearchy) retains all the information of mass and bounding box of all its children solid ?



No each solid should have it's own mass and boundign box informations, then they masses and bounding boxes are just merged together.

##### Gautier 10/29/2019 08:54:14
But, what I mean is that the "upper parent" parents is going to have its own boundingbox/masses, which will be the sum of the information in its own physics node, and its children physics node ?

##### David Mansolino [Cyberbotics] 10/29/2019 08:55:25
Yes exactly.

##### Gautier 10/29/2019 08:55:43
Thank you !

##### David Mansolino [Cyberbotics] 10/29/2019 08:55:56
You're welcome

##### Oxygenius 10/29/2019 09:25:10
Hi everyone, 

I have a question about the physics node. For example, if we design a robot with multiple hinge joints and hence multiple endsolids with defined physics node(weights and center of mass), does the robot's physics node have to  include all the weights of all its parts?

Thanks in advance

##### Fabien Rohrer [Moderator] 10/29/2019 09:25:50
hi


No, the physics node of the robot should only take into account the mass of its associated bounding object.


The Robot node derives the Solid node.


You can monitor the overall mass (the sum of all the solids) by selecting the robot, and check the mass tab under the scene tree.

##### Oxygenius 10/29/2019 09:29:51
Okay, thank you !

##### Fabien Rohrer [Moderator] 10/29/2019 09:30:12

%figure
![Capture_decran_2019-10-29_a_10.30.02.png](https://cdn.discordapp.com/attachments/565154703139405824/638670917966495744/Capture_decran_2019-10-29_a_10.30.02.png)
%end

##### threeal 10/29/2019 10:41:08
`@Tahir` where can i view the source code of the E-Puck ros node. i want to create my own node to control robot in webots simulator, but don't know where to start

##### David Mansolino [Cyberbotics] 10/29/2019 10:42:07
`@threeal` , you can find the Webots ROS controller here: [https://github.com/cyberbotics/webots/tree/revision/projects/default/controllers/ros](https://github.com/cyberbotics/webots/tree/revision/projects/default/controllers/ros)


If you want to extend it, here is an example: [https://github.com/cyberbotics/webots/tree/revision/projects/vehicles/controllers/ros\_automobile](https://github.com/cyberbotics/webots/tree/revision/projects/vehicles/controllers/ros_automobile)

##### threeal 10/29/2019 10:45:24
i am sorry, but when i want to create a robot program using ros, should i create the behavior in webots controller or in the ros node?


and how is the relation between both of them?

##### David Mansolino [Cyberbotics] 10/29/2019 10:48:23
The ros controller is just the interface between ros and Webots, you should create the behavior in the ros node.


You can find some example here: [http://wiki.ros.org/webots\_ros/Tutorials/Sample%20Simulations](http://wiki.ros.org/webots_ros/Tutorials/Sample%20Simulations)

##### threeal 10/29/2019 10:49:52
so i just need to attach ros controller to my robot? no need to create my own controller in webots?

##### David Mansolino [Cyberbotics] 10/29/2019 10:50:24
Yes, that's all what you need to do on the Webots side.

##### JoHn 10/29/2019 16:09:49
Hi `@David Mansolino`, I'd like to use the xbox 360 joystick to control both steering and speed of a vehicle in Webots. I notice that Webots is able to response the bottons on the right side of the joystick, but I am not sure whether it can also response to the stick marked in the red circle. Would you please give me some advice? Thanks a lot.


/home/northstar/Documents/1111111



> **Attachment**: [1111111](https://cdn.discordapp.com/attachments/565154703139405824/638771671871914025/1111111)


Could you see the picture above?



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/638773078385754132/unknown.png)
%end

##### Fabien Rohrer [Moderator] 10/29/2019 16:17:02
Hi. Normally, this should work. Do you use the Joystick API directly?


[https://cyberbotics.com/doc/reference/joystick#wb\_joystick\_get\_axis\_value](https://cyberbotics.com/doc/reference/joystick#wb_joystick_get_axis_value)


wb\_joystick\_get\_axis\_value should be able to return the "red pad" axis. The point is to find to which axis id it's mapped. (the argument of wb\_joystick\_get\_axis\_value() )

##### JoHn 10/29/2019 16:24:27
Hi `@Fabien Rohrer`, thank you very much for your response. Yes, I am use ROS and I subscribe /joystick/axis<X> topic.



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/638775216587145256/unknown.png)
%end


When I subscribe this topic, I delete <> and replace X with 0 or 1. Is this a correct way?

##### David Mansolino [Cyberbotics] 10/29/2019 16:27:27
Yes exactly, can you print the number of axes detected by webots by calling the '/joystick/get\_number\_of\_axes' service just to check?

`rosservice call .../joystick/get_number_of_axes`

##### JoHn 10/29/2019 16:28:42
Hi `@David Mansolino`, thank you for your response, Yes, I am double-checking it. 😁


Hi `@David Mansolino`, my joystick has 6 axis. Does it mean that each stick on the joystick has 2 axis? And are the axis numbering from 0 to 5? For example, if I only use one stick (marked in the red circle) on the left side of the joystick to control both steering and speed (throttle and brake), I should subscribe the topics of /joystick/axis0 and /joystick/axis1, is this correct? Thank you in advance.

##### David Mansolino [Cyberbotics] 10/29/2019 17:07:36
> Does it mean that each stick on the joystick has 2 axis?



Yes, that's often the case with such kind of joystick.



> And are the axis numbering from 0 to 5?



Yes.



> I should subscribe the topics of /joystick/axis0 and /joystick/axis1, is this correct



I can't say which axis is which one, you probably want to print the value of all the 6 axes and then move them one by one to identify which one is which one.

##### JoHn 10/29/2019 17:09:16
Thank you very much `@David Mansolino`, you have a good night.

##### David Mansolino [Cyberbotics] 10/29/2019 17:09:26
You're welcome, thank you!

##### Gordon 10/30/2019 01:14:35
`@David Mansolino` Thanks so much for your help. I will try to recompile the ROS controller.

##### Sohil 10/30/2019 01:58:00
I am using the rock10cm proto to make a custom environment. I wanted a bigger rock so I decided to scale all the coordinate values by an appropriate amount to get a bigger rock. For e.g. 40 cm , I scaled the `coord Coordinate points` by a value of 4. Now, I have a bigger rock, but it appears to sink into my environment. What am I not considering in this?
> **Attachment**: [rock\_anamoly.mp4](https://cdn.discordapp.com/attachments/565154703139405824/638919504319217704/rock_anamoly.mp4)

##### David Mansolino [Cyberbotics] 10/30/2019 07:18:22
`@Gordon`, you're welcome, let us know if you have any recompilation issue.


`@Sohil`, can you please check (and even send us a screenshot) of the contact points between the rock and the ground. You can enable contact points visualization from the 'View / Optional Rendering / Show Contact Points' menu.

##### Gautier 10/30/2019 08:08:09
Hello, I want to modelise the behavior of Reaction Wheel in Webots. It's basically this comportement. Do you have any suggestion on how to proceed  ?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/639012656359145472/unknown.png)
%end

##### Fabien Rohrer [Moderator] 10/30/2019 08:10:20
Hi Gautier

##### Gautier 10/30/2019 08:10:32
I was thinking about doing some variation of the motor using the source of webots, but I'm not sure if that approach is wise


Hi Fabien !

##### Fabien Rohrer [Moderator] 10/30/2019 08:11:55
I could first confirm that it's certainly possible to model a reaction wheel in Webots, indeed, the physics engine supports momentum reactions.


That said, we don't have examples of such setup.


I expect that controlling a (or several) rotational motor(s) in torque control is a good starting point.


You could have the actual speed of the motor thanks to a PositionSensor (you should just derive the position along the time once to get the speed).


So I confirm Webots allows to set the input and retrieve the output of the algorithm you mention.


but the algorithm should be written by yourself.


Is it a satisfying answer?

##### Gautier 10/30/2019 08:16:02
Yes, thank you a lot !


I've already tried some little experiments with rotationnal motors and position sensor (using hinge joint)  and it seemed to be working, but to be honest, what worries me a little is the  abscence of anti-windup in this configuration (because the torque "saturate" at 5mNm, the PID is gonna integrate the error caused by the saturation)

##### Fabien Rohrer [Moderator] 10/30/2019 08:24:23
Indeed, the PID control we provide does not propose an anti-windup. But in theory, this could be implemented in the controller. I'm wondering why you get this saturation? Did you play with the maxTorque field? Normally this field should allow you to increase the motor torque.

##### Gautier 10/30/2019 08:35:58
It's more because of the caracteristic of the wheel itself. The torque is very low in the "datasheet" of the constructor


It's a reaction wheel used on satellite, it doesn't neccesitate a lot of torque.

##### Fabien Rohrer [Moderator] 10/30/2019 08:36:40
Ok I see, this sounds good then.


Are you interested on modeling a satellite in Webots? This is definitively something I would like to see 🙂

##### Gautier 10/30/2019 08:44:23
Ahaha, more or less. I'm experimenting with webots for the moment 🙂


Actually, we are working on an Europeean Project, H2020 open project Pulsar


[https://www.h2020-pulsar.eu/](https://www.h2020-pulsar.eu/)


The aim of this project is the assembly of large structure in space

##### Fabien Rohrer [Moderator] 10/30/2019 09:19:22
If perhaps, we are always looking for funds 😜

##### Oxygenius 10/30/2019 12:02:21
Hi!

I was trying to make a stair climbing rover but i have a problem with friction as you can see in the world attached. I tried to change contact properties but it did not work.

Thanks in advance.
> **Attachment**: [rover.wbt](https://cdn.discordapp.com/attachments/565154703139405824/639071593183576064/rover.wbt)


And this is the controller
> **Attachment**: [my\_controller.py](https://cdn.discordapp.com/attachments/565154703139405824/639071785932947469/my_controller.py)

##### Fabien Rohrer [Moderator] 10/30/2019 12:23:01
`@Oxygenius` Hi, let me give a look..


Ok, I have your simulation running on my computer.


It seems you defined well the contact properties between "sol" and "caoutchou"


there is a typo in ContactProperties


"caoutchou" => "caoutchouc" 😁


Once fixing this, the friction acts as expected.


By the way, you have also several not-related warnings about density. You can fix them by changing these fields: Physics.density=-1 for each Physics node where you defined the Physics.mass

##### Oxygenius 10/30/2019 12:46:52
when we fix the mass , shouldn't the density be ignored?

##### Fabien Rohrer [Moderator] 10/30/2019 12:47:41
Yes, it's the case. My comment is just about how to remove the annoying warnings appearing in the Webots console (I looked at them first).

##### Oxygenius 10/30/2019 12:49:57
Ah okay, so i fixed the typo but the robot still cannot climb the stairs.


I suppose that there is a problem between "bois" and "caoutchouc", they are not always in contact like it would be in real life.

##### Fabien Rohrer [Moderator] 10/30/2019 12:54:43
the motors are not powerful enough to climb the stairs.


you could either increase the friction (I set the ContactProperties.coulmbFriction to 10 between caoutchouc and sol, and it is working)


or to increase the Motor.maxTorque (I didn't test this)


=> you're right about the bois <=> caoutchouc, it's set to -1. this is not a good idea 🙂


setting both coulombFriction to 10 gives this result
> **Attachment**: [rover.mp4](https://cdn.discordapp.com/attachments/565154703139405824/639085975494066195/rover.mp4)


the hinge between the 2 front wheel and the body seems too fragile to support the impact 🙂

##### Oxygenius 10/30/2019 13:01:09
I changed the (bois <=> caoutchouc) to 10 and I still have the same problem in the video

##### Fabien Rohrer [Moderator] 10/30/2019 13:04:10
I can solve this by setting IiEssieu1.jointParameters.spring/dampingConstant fields to 0.3
> **Attachment**: [rover\_1.mp4](https://cdn.discordapp.com/attachments/565154703139405824/639087149148405771/rover_1.mp4)



> **Attachment**: [rover.wbt](https://cdn.discordapp.com/attachments/565154703139405824/639087241494265899/rover.wbt)

##### Oxygenius 10/30/2019 13:19:39
okay that's fixed the problem of the hingeJoint but I had another problem with stability as shown in this screenshot .
%figure
![rover.png](https://cdn.discordapp.com/attachments/565154703139405824/639091045983977484/rover.png)
%end

##### Fabien Rohrer [Moderator] 10/30/2019 13:22:49
This kind of issues is due the robot layout. I would bet that the same occurs on a real robot 😄


The point is about to find good physics properties to let this work smoothly.


For example, if the spring/damping are too low the robot cannot climp the box, if they are too big, they cannot retract quickly enough (as shown here).


I'm pretty sure a good tradeoff can be found, because this layout is used in real robots.

##### Oxygenius 10/30/2019 13:27:16
Okay I will try to adjust some parameters to see if it works .

Thank you very much !

##### Fabien Rohrer [Moderator] 10/30/2019 13:27:55
you're welcome. Give us some news 🙂

##### RenardBleu 10/31/2019 09:08:51
Hello

##### David Mansolino [Cyberbotics] 10/31/2019 09:09:12
Hi `@RenardBleu`

##### RenardBleu 10/31/2019 09:09:28
Is there some way to clear a "display" via a controller ?


(I'm talking about the display node)


Hi `@David Mansolino`

##### David Mansolino [Cyberbotics] 10/31/2019 09:10:30
Yes of course, you can draw a rectangle with the same size than the e-puck with 'wb\_display\_fill\_rectangle' ([https://www.cyberbotics.com/doc/reference/display#wb\_display\_fill\_rectangle](https://www.cyberbotics.com/doc/reference/display#wb_display_fill_rectangle))


This is done for example in the 'display' sample, see for example: [https://github.com/cyberbotics/webots/blob/revision/projects/samples/devices/controllers/display/display.c#L111](https://github.com/cyberbotics/webots/blob/revision/projects/samples/devices/controllers/display/display.c#L111)

##### RenardBleu 10/31/2019 09:13:03
It's working ! Thank you a lot ! 😄

##### David Mansolino [Cyberbotics] 10/31/2019 09:13:13
You're welcome

##### JoanGerard 10/31/2019 14:22:25
Hello, I am trying to make a web animation and I would like to see an example online. Webots documentation suggests this site-> [https://robotbenchmark.net/](https://robotbenchmark.net/) but it's not working for me. I cannot enter into any simulation at all. For instance, this site [https://robotbenchmark.net/](https://robotbenchmark.net/)benchmark/viewpoint\_control/simulation.php is a blank page. Do you know if there are any other sites similar to that one where I can find an example?

##### David Mansolino [Cyberbotics] 10/31/2019 14:25:23
Hi `@JoanGerard`


Can you see the animation on this page for example?

[https://robotbenchmark.net/benchmark/inverted\_pendulum/](https://robotbenchmark.net/benchmark/inverted_pendulum/)

##### JoanGerard 10/31/2019 14:30:24
oh! wait, I think it should be my internet connection or something because I's working now, my apologies !

##### David Mansolino [Cyberbotics] 10/31/2019 14:30:48
Ok, no problem, thank you for the feedback

##### JoHn 10/31/2019 14:47:48
Hi guys, I am using ROS to control two trucks in Webots and each truck has a GPS. When I did the simulation and published these two GPS signals of the two trucks, the signals were kind of random. I could not figure out what is causing this problem. Would you advise? Thanks a lot.

##### David Mansolino [Cyberbotics] 10/31/2019 14:48:20
What do you mean by random, there is some noise on the position?

##### JoHn 10/31/2019 14:54:29
The GPS signals I got were varying within 10m. I guess the noise would not be that much, right?  And when I turned off one of the GPS, the simulation worked very good.


I am just guessing, would the two GPS be conflict with each other?

##### David Mansolino [Cyberbotics] 10/31/2019 14:55:29
The noise of the GPS can be defined with it's 'accuracy' field ([https://cyberbotics.com/doc/reference/gps](https://cyberbotics.com/doc/reference/gps)), can you please confirm this field is set to 0 ?


> I am just guessing, would the two GPS be conflict with each other?



They should not. Maybe you can use the robot-window to see in real time the value of the GPS (just double click on your truck to open it).

##### JoHn 10/31/2019 14:57:40

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/639478102304751649/unknown.png)
%end


Yes, the 'accuracy' is 0.

##### David Mansolino [Cyberbotics] 10/31/2019 14:58:47
ok, and does the values displayed in the robot window make sense ?

##### JoHn 10/31/2019 14:59:34
Oh, sure, I am checking the robot window. Give a minute 😁


Hi `@David Mansolino`, the values shown in the robot window make sense. Thank you very much for your input. I am trying to locate the problem. Will update you later on.

##### JoanGerard 10/31/2019 16:00:38
How can I re-open the HTML window from Webots once closed?

##### David Mansolino [Cyberbotics] 10/31/2019 16:10:59
`@JoHn` you're welcome.


`@JoanGerard` from Webots you should be able to re-open it the exact same way you open it the first time (either double-clicking on the robot or right click => Show Robot Window)

##### JoanGerard 10/31/2019 16:14:28
thanks!


I am trying to create my first HTML window but I receive this error message while starting the simulation.
%figure
![Screen_Shot_2019-10-31_at_6.53.40_PM.png](https://cdn.discordapp.com/attachments/565154703139405824/639522709767847937/Screen_Shot_2019-10-31_at_6.53.40_PM.png)
%end


I'm using Python3.7

## November

##### buzzlightyear 11/01/2019 00:28:21
Quick question,


how do you print to console


From a robot controller


(I am using C)

##### Sohil 11/01/2019 01:29:16
`@David Mansolino` Here are the clips. Sorry it took some time.

The rock on the left is the standard Rock10cm as defined by  webots. The one on the right is the same with coordinate points scaled by 4x to make Rock40cm. In this environment I have created this happens. The left rock is perfect in dynamics by the right one is sinking, etc.
> **Attachment**: [rock\_anamoly\_2\_clip1.mp4](https://cdn.discordapp.com/attachments/565154703139405824/639637046868312084/rock_anamoly_2_clip1.mp4)



> **Attachment**: [rock\_anamoly\_2\_1.mp4](https://cdn.discordapp.com/attachments/565154703139405824/639637327664250880/rock_anamoly_2_1.mp4)

##### David Mansolino [Cyberbotics] 11/01/2019 07:32:42
`@JoanGerard`, are you sure your robot windows is at the correct location, i.e. `YOUR\_PROJECT/plugins/robot\_windows/YOUR\_ROBOT\_WINDOW\_NAME' and the folder contains a YOUR\_ROBOT\_WINDOW\_NAME.html file ?


`@buzzlightyear` as any regular program you can simply do a printf (i.e. `printf("Hello World\n");`)


`@Sohil` that's indeed strange, would you agree sending us your Rock40cm PROTO file so that we can test ?

##### smg 11/01/2019 08:44:08
Hello. I wanted to ask if anyone has ever tried to run webots in a docker container

##### David Mansolino [Cyberbotics] 11/01/2019 08:45:56
Hi `@smg`, yes of course we have tried, a docker image of the latest release (with the development environment too) is available here: [https://hub.docker.com/r/cyberbotics/webots](https://hub.docker.com/r/cyberbotics/webots)

Here is also the procedure to install Webots in a Docker environment: [https://github.com/cyberbotics/webots/wiki/Docker](https://github.com/cyberbotics/webots/wiki/Docker)

##### smg 11/01/2019 08:47:21
`@David Mansolino` Nice! thanks

##### David Mansolino [Cyberbotics] 11/01/2019 08:47:47
You're welcome.

##### JoHn 11/01/2019 15:49:37
Hi `@David Mansolino`, I have fixed the GPS problem. We did something wrong in the transformation 😂. Thanks a lot for your help. I'd like to ask one more question, because I am doing the control of vehicles via ROS. When the vehicles move, is there a way that moves the screen automatically, so that the vehicles can always be shown in the screen, and it doesn't need us to move (zoom in or out) the screen manually.

##### Olivier Michel [Cyberbotics] 11/01/2019 15:50:31
Hi `@JoHn`,


Yes, select your robot and hit F5.


You can also select your robot and go to the View menu of Webots to select either "Follow Object" or "Follow Object and Rotate". You will quickly understand the difference by trying both options.

##### JoHn 11/01/2019 15:55:56
Hi `@Olivier Michel`, thank you very much for your help. It does track the vehicle movement. Great.


You guys have a nice weekend!

##### Olivier Michel [Cyberbotics] 11/01/2019 15:57:59
Thanks, same for you!

##### JoanGerard 11/01/2019 16:17:19
do you know what is the equivalent of wbu\_default\_robot\_window\_configure function for python?

##### David Mansolino [Cyberbotics] 11/01/2019 16:17:50
Let me check


Unforunately it seems the 'wbu\_default\_robot\_window\_*' functions are not  available in other languages than C, but it should be possible to add them, would you be interested in doing this ?

##### JoanGerard 11/01/2019 16:25:49
This function configures the robot window only right? I think that I may need it in the future but for the moment it's ok

##### David Mansolino [Cyberbotics] 11/01/2019 16:30:43
Yes exactly this is just if you want to 'inherit' from the default robot window, but you can of course create your own custom robot-window without using this function.

##### JoanGerard 11/01/2019 16:33:57
an other question, is there any way of watching to the console.log printed messages when using a robot window?

##### David Mansolino [Cyberbotics] 11/01/2019 16:36:46
If I am not wrong they are supposed to be displayed in the Webots Console


I juste tried and it seems to work fine.


By the way, you should probably have a look at the 'custom\_robot\_window.wbt' world which is a very simple example of custom robot window.

##### JoanGerard 11/01/2019 17:15:19
You're right. Thank you!

##### David Mansolino [Cyberbotics] 11/01/2019 17:15:59
You're welcome

##### Xiangxiao 11/01/2019 17:53:50
Hello, I am doing simulation of fish-like robot swimming in fluid water. I am doing the fluid water based on the sample of Webots (floating\_geometries.wbt). In the sample, the water flow contains two components, including the DEF FLOWING\_WATER fluid (defining the physics of water flow) and DEF WATER\_FLOWING\_ANIMATION robot (defining the visual behavior/animation of water flow). In FLOWING\_WATER fluid, I can define the velocity of water flow. In the controller of WATER\_FLOWING\_ANIMATION robot, I can define the speed of the animation that simulate the water flow. I would like to know that how can I induce [the parameter of velocity of physical water flow in FLOWING\_WATER fluid] into [the controller of WATER\_FLOWING\_ANIMATION robot]? I want to do this since I would like to correlate the physical velocity of water flow to the playing-speed of animation.


Thanks in advance

##### smg 11/01/2019 18:25:37
I tried installing webots through snap on archlinux: [https://snapcraft.io/install/webots/arch](https://snapcraft.io/install/webots/arch)

which was successful but the resulting webots binary fails with a seg fault. 

would be great if someone can take a look: [https://gist.github.com/shivamMg/d8f871719264b03e52c40ac95c217c08](https://gist.github.com/shivamMg/d8f871719264b03e52c40ac95c217c08)

##### HiguchiNakamura 11/02/2019 15:26:57
Hi, How do I open the Motion Editor? When I double-click on the NAO, the window appears, but no Motion Editor entry.



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/640210331020427264/unknown.png)
%end

##### Alexx 11/03/2019 12:24:50
i have the same issue..there is any solution???

##### Stefania Pedrazzi [Cyberbotics] 11/03/2019 12:39:49
hi `@HiguchiNakamura` and `@Alexx` , the motion editor is currently not available with the new HTML window.


In Webots R2019b is still possible to use the old Qt robot window (deprecated) which contains the MotionEditor by changing the Robot.window field from the default value "generic" to "generic\_window"


Note that the "window" field is not exposed for the Nao PROTO, i.e. you have to modify the Nao.proto file.


`@smg` I coudl successfully install and run Webots on archlinux with Gnome desktop environemnt. Which (if any) desktop environment are you using?

##### Alexx 11/03/2019 12:56:57
where can i find the Robot.window field to change the "generic" to "generic\_window"? when i double click on robot it appears a window with name "Generic robot window". In this window there are all the tabs expect motion editor


`@Stefania Pedrazzi`



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/640535440247619594/unknown.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 11/04/2019 07:00:33
`@Alexx` has I wrote in the previous message, the "window" field is not exposed in the Nao PROTO (see PROTO description [https://www.cyberbotics.com/doc/reference/proto](https://www.cyberbotics.com/doc/reference/proto)). So you have to:


1. right-click on the Nao node in the scene tree and choose the "View PROTO source" item in the context menu;

2. add the line `window "generic_window"` at line 67 just after the `Robot {`

3. save the Nao.proto file in the "protos" folder in your project folder (see the description of the structure of the Webots project folder [https://www.cyberbotics.com/doc/guide/the-standard-file-hierarchy-of-a-project](https://www.cyberbotics.com/doc/guide/the-standard-file-hierarchy-of-a-project))


Alternatively instead of modifying the Nao PROTO node, you can also convert it to base nodes (right-click on the Nao node in the scene tree and choose "Convert to Base Node(s)") and then change the Robot.window field.


`@Xiangxiao` in case of the `floating_geometry.wbt` I think that the water animation speed was empirically on and not using a precise formula.

If you want that when you change the fluid properties also the animation speed changes I would suggest you to use a Robot with supervisor capabilities (`Robot.supervisor` field set to TRUE) so that in the controller you can read the `Fluid` properties and use them to compute the animation speed.

##### RossBatten 11/04/2019 07:38:38
Hi, I'm struggling with getting any kind of supervisor controller functionality working in my world.  If I remove the normal supervisor features and treat the controller as a simple robot only declaring it as a supervisor it works fine but the moment I try to use something as simple as setLabel(...) the simulation freezes and I get a spinning wheel and have to restart.  Any idea what's going on?


I'm building the controller in visual studio with no errors


I have the robot set to have supervisor status

##### Olivier Michel [Cyberbotics] 11/04/2019 07:50:28
`@smg` can you try to remove the cache of fontconfig:

```
sudo rm /var/cache/fontconfig/*
rm ~/.cache/fontconfig/*
```

##### RossBatten 11/04/2019 07:50:48
I'm using windows currently so no ubuntu console

##### Olivier Michel [Cyberbotics] 11/04/2019 07:50:54
It seemed to work for some users: [https://forum.snapcraft.io/t/snapped-app-not-loading-fonts-on-fedora-and-arch/12484/23](https://forum.snapcraft.io/t/snapped-app-not-loading-fonts-on-fedora-and-arch/12484/23)

##### RossBatten 11/04/2019 07:51:03
Oh sorry not for me

##### Olivier Michel [Cyberbotics] 11/04/2019 07:52:36
`@RossBatten`: Can you check that your `Robot` node has the `supervisor` field set to `TRUE`?

##### RossBatten 11/04/2019 07:52:44
Yes it does


If it's set to false I get an actual error message.  In this case it's simply freezing and I have to restart webots


`@Olivier Michel`

##### Olivier Michel [Cyberbotics] 11/04/2019 07:53:53
Can you share the source code of your controller?

##### RossBatten 11/04/2019 07:54:05
Sure, what's the best way to do that here?

##### Olivier Michel [Cyberbotics] 11/04/2019 07:54:38
For example on a gist: [https://gist.github.com/](https://gist.github.com/)


(if you have a github account)

##### RossBatten 11/04/2019 07:55:30
I do but I haven't got my source code versioned anywhere yet because I only just started it


Like it's 30 lines max

##### Olivier Michel [Cyberbotics] 11/04/2019 07:55:50
Yes, just copy/paste it on a gitst and that will be fine.


BTW, did you try to compile your code from the Webots built-in editor?

##### RossBatten 11/04/2019 07:58:09
I didn't, no


[https://gist.github.com/RossBatten/43a069c06a28f7faad9992754653330f](https://gist.github.com/RossBatten/43a069c06a28f7faad9992754653330f)


I just copied and pasted the code into a webots editor controller and the same thing happened.  Spinning circle crash

##### Olivier Michel [Cyberbotics] 11/04/2019 08:02:41
Thanks, I will inspect your code and try to reproduce the problem. I have a meeting now, so, I will get back to you after that.

##### RossBatten 11/04/2019 08:03:19
OK no problem.  I probably won't be able to get back to this until tomorrow.

##### Olivier Michel [Cyberbotics] 11/04/2019 10:06:00
`@RossBatten`: I could test your code from the Webots IDE on Windows 10 and it works as expected. I had to comment out the `lds[i] -> enable(timeStep);` line because the sensors were not found in my test world and this line was crashing the controller, but once that was commented out, everything was working as expected and I could see the "Hello" string displayed in the 3D view.


I did the same from Visual Studio 2019 and it worked as well.

##### HE110 11/04/2019 11:30:34
How can I use Motor in controller programming with python?


import controller

robot=controller.Robot()

epuckMotor=controller.Motor('e-puck')

epuckname=robot.createMotor('e-puck')

Force=epuckMotor.setForce(50)


hello?Anyone can help me?

##### David Mansolino [Cyberbotics] 11/04/2019 11:37:47
Hi `@HE110`

##### HE110 11/04/2019 11:38:11
Hi


Can u give me a example?

##### David Mansolino [Cyberbotics] 11/04/2019 11:39:23
The way you get the motor is wrong, you have to get them like this:

```
epuckMotor = robot.getMotor()
```

##### HE110 11/04/2019 11:40:00
thanks,let me try

##### David Mansolino [Cyberbotics] 11/04/2019 11:40:14
You should probably follow this tutorial about controlling the e-puck robot: [https://www.cyberbotics.com/doc/guide/tutorial-4-more-about-controllers](https://www.cyberbotics.com/doc/guide/tutorial-4-more-about-controllers)

It is available in Python too.

##### HE110 11/04/2019 11:41:01
ok,thank u

##### David Mansolino [Cyberbotics] 11/04/2019 11:43:08
You're welcome

##### RossBatten 11/05/2019 01:58:41
`@Olivier Michel`  I don't understand what's going on then.  I've tried running the same controller now on two different PCs, one windows 10, one linux, visual studio and webots internal editor/cmake builder and the same crash occurs every time.  If I omit the label line it works fine but if I include it or any other supervisor controller command it crashes the whole of webots on play.


I was only able to get it working in a C code controller.  At least I can move ahead with it

##### Stefania Pedrazzi [Cyberbotics] 11/05/2019 07:27:15
`@RossBatten` could you add all your project files to the gitst? at least the world file and any another controllers/plugins that you are using?

##### HE110 11/05/2019 08:22:52
How can i sleep 1s in controller by C?

##### David Mansolino [Cyberbotics] 11/05/2019 08:23:35
`@HE110` do you want the simulation to continue or to wait for 1s too?

##### HE110 11/05/2019 08:24:06
Wait

##### David Mansolino [Cyberbotics] 11/05/2019 08:24:27
ok, which programming language are you using?

##### HE110 11/05/2019 08:24:32
C

##### David Mansolino [Cyberbotics] 11/05/2019 08:25:00
In that case you can use simply the 'sleep' function for example: [https://linux.die.net/man/3/sleep](https://linux.die.net/man/3/sleep)

##### HE110 11/05/2019 08:25:48
Thank u,i go to try it

##### David Mansolino [Cyberbotics] 11/05/2019 08:25:54
You're welcome

##### mkjhnb 11/05/2019 10:21:10
the range of this lidar is very big, why the result like this
%figure
![2019-11-056.19.49.png](https://cdn.discordapp.com/attachments/565154703139405824/641220456262664202/2019-11-056.19.49.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 11/05/2019 10:31:38
`@mkjhnb` the displayed range (cyan lines) corresponds to the value of the `Lidar.maxRange` field. Is this not the case for you?

So if you want to reduce the range you should change the `Lidar.maxRange` value

##### mkjhnb 11/05/2019 10:39:31
sorry, I mean that the value of the maxRange is so big, but the laser can't seem to touch the black wall, it seem very short. Why?

##### Stefania Pedrazzi [Cyberbotics] 11/05/2019 10:45:45
can you share your world file?  (for example on a gist: [https://gist.github.com/](https://gist.github.com/))

##### mkjhnb 11/05/2019 10:52:44
ok,


I wanted to use the ready model VelodyneHDL-32, but it seemed have some problems.
%figure
![2019-11-057.02.04.png](https://cdn.discordapp.com/attachments/565154703139405824/641231870444437505/2019-11-057.02.04.png)
%end

##### David Mansolino [Cyberbotics] 11/05/2019 11:12:08
`@mkjhnb` for rotating lidar it is recomended to either set a slow rotation frequency either have a small simulation time step.

##### mkjhnb 11/05/2019 11:12:28
[https://gist.github.com/mkjhnb/079bd3731ce2aac3e4d20566800d95e7](https://gist.github.com/mkjhnb/079bd3731ce2aac3e4d20566800d95e7)

##### David Mansolino [Cyberbotics] 11/05/2019 11:15:01
From your world file, I would recommend to:


- set the Lidar `tiltAngle` to 0 (this is know to cause issues, see: [https://github.com/cyberbotics/webots/issues/37](https://github.com/cyberbotics/webots/issues/37) )

  - set the Lidar `spherical` to TRUE

  - decrease the WorldInfo.timeStep to 8

##### mkjhnb 11/05/2019 11:17:14
thanks,let me try

##### David Mansolino [Cyberbotics] 11/05/2019 11:17:19
You're welcome

##### mkjhnb 11/05/2019 12:05:44
oh my god, it seems that the laser shining through the floor
%figure
![2019-11-058.00.12.png](https://cdn.discordapp.com/attachments/565154703139405824/641246773322711090/2019-11-058.00.12.png)
%end


why？

##### juanluciano 11/05/2019 21:55:05
hello


i need help. 



When I try to do cross compilation or remote control with any controller in the last version of webots, whether with some example of the Darwin robot or some controller that I have programmed, I have many errors. I get the following code when I try to remotely control the robot.



%figure
![error_darwin.PNG](https://cdn.discordapp.com/attachments/565154703139405824/641397089124745226/error_darwin.PNG)
%end



%figure
![error2_Darwin.PNG](https://cdn.discordapp.com/attachments/565154703139405824/641397152152551425/error2_Darwin.PNG)
%end



%figure
![error3_darwin.PNG](https://cdn.discordapp.com/attachments/565154703139405824/641397178681524280/error3_darwin.PNG)
%end



%figure
![error4_darwin.PNG](https://cdn.discordapp.com/attachments/565154703139405824/641397822582816768/error4_darwin.PNG)
%end



%figure
![Inkederror5_darwin_LI.jpg](https://cdn.discordapp.com/attachments/565154703139405824/641397897405005864/Inkederror5_darwin_LI.jpg)
%end

##### Hayden Woodger 11/05/2019 22:18:36
Have you tried turning it on and off again?

##### juanluciano 11/05/2019 22:20:20
yes

##### Hayden Woodger 11/05/2019 22:23:09
Unistall and re-install with all the latest version?


*versions

##### juanluciano 11/05/2019 22:24:28
I've only tried the 2019 version

##### Hayden Woodger 11/05/2019 22:26:15
I think it's saying your first error that the sleep() command can't be found. Have you imported  the correct libraries?


Could you send me the code you're using on your robot? I'll take a look and see if i can help.

##### juanluciano 11/05/2019 22:28:59
I think so, since the errors appear with both the programs and the ones I have made


yes of course



> **Attachment**: [traker.py](https://cdn.discordapp.com/attachments/565154703139405824/641404011173380107/traker.py)


but I don't think the code is the cause of the failure  because I get the same errors with the exemplary controllers.

##### Hayden Woodger 11/05/2019 22:35:10
Yeah the code looks fine to me.


Was about to test it


This might sound dumb. Are you trying to remotely connect to a darwin robot in real life?


Or just in the simulation?

##### juanluciano 11/05/2019 22:37:54
It fails when I try to connect to the robot remotely, everything is fine in the simulation

##### Hayden Woodger 11/05/2019 22:38:31
Okay,.Have you connected to it successfully before?

##### juanluciano 11/05/2019 22:39:21
no, it's the first time I try

##### Hayden Woodger 11/05/2019 22:43:59
Is a there some configuration on the robot you need to do in order to connect wirelessly?

##### juanluciano 11/05/2019 22:48:26
the only thing I do is double click on the robot and press the transfer icon



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/641408586001547296/unknown.png)
%end

##### Hayden Woodger 11/05/2019 22:50:39
Can you upload your code with a wire?


*usb cable

##### juanluciano 11/05/2019 22:52:02
I do not know how to do it

##### Hayden Woodger 11/05/2019 22:52:39
Can you plug the robot into your computer?

##### juanluciano 11/05/2019 22:57:06
At this moment I have no way to connect it but I will try

##### David Mansolino [Cyberbotics] 11/06/2019 07:39:53
`@mkjhnb` did you set the tilt angle to 0?


`@juanluciano` which version of the robot are you using? Have you tried to press the uninstall button (just to the right to the one you pressed) and the re-start remote-control again?

##### juan luciano 11/06/2019 14:22:55
I am using the first version of the robot, and if I already press the uninstall button

##### Hayden Woodger 11/06/2019 15:51:47
Juan, did you get this sorted out?

##### David Mansolino [Cyberbotics] 11/06/2019 15:55:14
`@juan luciano`, do you know which version of the darwin-op framework is installed on your robot? You can see this in the 'ReleaseNote.txt ' file.@

##### juan luciano 11/06/2019 20:17:34
`@Hayden Woodger` not yet


DARwIn-OP v1.4.0

##### Derek 11/07/2019 07:00:53
7

##### David Mansolino [Cyberbotics] 11/07/2019 07:56:49
`@juan luciano` you should probably try to update to the latest one (v1.6.1): [https://sourceforge.net/p/darwinop/code/HEAD/tree/trunk/darwin/](https://sourceforge.net/p/darwinop/code/HEAD/tree/trunk/darwin/)

##### Prasad 11/07/2019 08:56:28
In connection with [https://github.com/cyberbotics/webots/issues/1071](https://github.com/cyberbotics/webots/issues/1071)



When I do

targetY = float(params["yPositionFactor"]) * targetY

pitchPID = PID(float(params["pitch\_Kp"]), float(params["pitch\_Ki"]), float(params["pitch\_Kd"]), setpoint=targetY)



The code works.

If I do

pitchPID = PID(float(params["pitch\_Kp"]), float(params["pitch\_Ki"]), float(params["pitch\_Kd"]), setpoint=float(params["yPositionFactor"]) * targetY)



Can anybody let me know any root cause of this? I would greatly appreciate any help


pitchPID = PID(float(params["pitch\_Kp"]), float(params["pitch\_Ki"]), float(params["pitch\_Kd"]), setpoint=float(params["yPositionFactor"]) * targetY)



With this, position hold locks at wrong coordinates

##### Fabien Rohrer [Moderator] 11/07/2019 09:01:27
`@Prasad` Hi


again 🙂


I would recommend to translate EXACTLY mavic2pro.c into Python, and THEN (when it's working smoothly) bring modifications.

##### Prasad 11/07/2019 10:34:14
Thanks `@Fabien Rohrer` 🙂 I translated mavic2pro.c .However, the quadcopter was flipping and I couldn't work with it. That is when I studied and introduced control systems.

##### Fabien Rohrer [Moderator] 11/07/2019 10:36:40
In theory, the C controller can be translated to Python, there is a one-to-one match. So I guess you missed some point.


By the way..

##### Prasad 11/07/2019 10:37:27
Correct. That is what I guess and [https://github.com/PrasadNR/Webots-Quadcopter-Python-SITL/commit/737eaa870e448c24365b4b6dbb6fa52b0494f576](https://github.com/PrasadNR/Webots-Quadcopter-Python-SITL/commit/737eaa870e448c24365b4b6dbb6fa52b0494f576) was the commit


*guessed

##### Fabien Rohrer [Moderator] 11/07/2019 10:37:44
The mavic sample world has also some physics settings:

##### Prasad 11/07/2019 10:37:58
Oh! I hadn't known that

##### Fabien Rohrer [Moderator] 11/07/2019 10:38:01
[https://github.com/cyberbotics/webots/blob/master/projects/robots/dji/mavic/worlds/mavic\_2\_pro.wbt#L1](https://github.com/cyberbotics/webots/blob/master/projects/robots/dji/mavic/worlds/mavic_2_pro.wbt#L1)


=> You should copy the WorldInfo node from there.


It may explain some mismatch

##### Prasad 11/07/2019 10:39:21
Yes. I guess so. Thanks `@Fabien Rohrer` ! 🙂 If that works or any of the code solutions work, I will definitely reply back

##### Fabien Rohrer [Moderator] 11/07/2019 10:39:45
Gladly, feedback are always welcome 😄

##### Prasad 11/07/2019 10:40:18
Yes. And one final thing. If I get this Python controller working, is there any way to include it officially in Webots?

##### Fabien Rohrer [Moderator] 11/07/2019 10:40:51
Yes, it would be awesome.


Basically, you need to create a pull request from your Webots fork on github targeting our main branch.


We will review it.

##### Prasad 11/07/2019 10:41:40
Sure! Let me give that a shot

##### Fabien Rohrer [Moderator] 11/07/2019 10:41:41
Here is the doc: [https://github.com/cyberbotics/webots/blob/revision/CONTRIBUTING.md](https://github.com/cyberbotics/webots/blob/revision/CONTRIBUTING.md)

##### chamandana 11/07/2019 12:03:14
I'm using the Kuka YouBot. I haven't called base\_backwards() but the robot is going backward. What do you think is happening here?
> **Attachment**: [C\_\_Users\_Chamuth\_Documents\_P\_Bots\_Round2\_Round2\_worlds\_Round2.wbt\_Round2\_-\_Webots\_R2019b\_2019-11-07\_.mp4](https://cdn.discordapp.com/attachments/565154703139405824/641970917882462208/C__Users_Chamuth_Documents_P_Bots_Round2_Round2_worlds_Round2.wbt_Round2_-_Webots_R2019b_2019-11-07_.mp4)

##### Fabien Rohrer [Moderator] 11/07/2019 12:47:25
`@chamandana`  I suspect the WorldInfo.contactProperties field to be badly defined.


Could you check it's equivalent to this? [https://github.com/cyberbotics/webots/blob/master/projects/robots/kuka/youbot/worlds/youbot.wbt#L2](https://github.com/cyberbotics/webots/blob/master/projects/robots/kuka/youbot/worlds/youbot.wbt#L2)

##### KyleM 11/08/2019 03:31:02
Hey folks! I'm new here

##### David Mansolino [Cyberbotics] 11/08/2019 09:03:40
Hi `@KyleM`, welcome!

##### Prasad 11/08/2019 09:51:22
Hey guys, I am almost done with my repo and thinking about contributing with official Python example. I was not able to get Emitter and Receiver working with Python. Could anybody help?



I am doing



emitter = Emitter("emitter")

while (robot.step(timestep) != -1):

    emitter.send('x'.encode())



and at the receiver



receiver = Receiver("receiver")

receiver.enable(TIME\_STEP)

while (robot.step(timestep) != -1):

    print(receiver.getData())


It doesn't matter if I use bytes() in Python or struct with message unpack. I am getting errors all along. Could anyone let me know?

##### Olivier Michel [Cyberbotics] 11/08/2019 09:52:12
Did you try to use strings instead?

##### Prasad 11/08/2019 09:52:27
Yes. That was the first thing I tried. Sorry. I forgot to mention


I tried int and float too. None of them worked


Tried receiver.nextData() also


*nextPacket()

##### Olivier Michel [Cyberbotics] 11/08/2019 09:53:19
Please have a look at Webots/projects/languages/python/controllers/slave and Webots/projects/languages/python/controllers/driver


This is a good example of using emitters and receivers in Python.

##### Prasad 11/08/2019 09:54:48
Oh. That should give me a head start. Thank you!


Thank you `@Olivier Michel` I was able to integrate communication. I was doing GPS based object tracking. However, I am facing one issue with ODE Physics engine. I keep getting the following warning.



WARNING: The current physics step could not be computed correctly. Your world may be too complex. If this problem persists, try simplifying your bounding object(s), reducing the number of joints, or reducing WorldInfo.basicTimeStep.



Repo link: [https://github.com/PrasadNR/Webots-Quadcopter-Python-SITL](https://github.com/PrasadNR/Webots-Quadcopter-Python-SITL)


If I set the TIME\_STEP too large like 64, rover works properly. But, quadcopter flips due to PID update rate. If I set TIME\_STEP to 4 or 8, quadcopter works. But, rover fails to follow the line


Question here is should I set the same time step for both quadcopter and rover? What about communication sync?

##### Olivier Michel [Cyberbotics] 11/08/2019 11:29:44
No, you can use different control step for different robots, this is not a problem.

##### Prasad 11/08/2019 11:29:55
Oh ok. Let me try that!

##### Olivier Michel [Cyberbotics] 11/08/2019 11:30:11
Beware however that the control step should be multiple of the WorldInfo.basicTimeStep.

##### Prasad 11/08/2019 11:30:35
Oh ok! I will check that out

##### Olivier Michel [Cyberbotics] 11/08/2019 11:30:41
Eg., if you WorldInfo.basicTimeStep is 8 ms, your controllers may have a control time step of 8, 16, 24, 32, etc.

##### Prasad 11/08/2019 11:31:09
Ok. I guess the world has 32. That is why I was getting that error. Awesome. Thanks!


For basicTimeStep 8, rover physics is behaving differently
> **Attachment**: [Untitled.mp4](https://cdn.discordapp.com/attachments/565154703139405824/642359055498608640/Untitled.mp4)


Should I raise a bug report on that?

##### Olivier Michel [Cyberbotics] 11/08/2019 13:47:41
It's normal to observe a small difference in the simulation result when you change the basic time step. If that difference is big when changing only moderately the time step, then it is certainly worth a bug report.

##### Prasad 11/08/2019 13:48:20
The rover is getting stuck and I am not able to get the simulation of both quadcopter and rover working. I guess I will raise a bug report for now


In connection with [https://github.com/cyberbotics/webots/issues/1081](https://github.com/cyberbotics/webots/issues/1081) , could anyone let me know any tips as to how I can fix it for now?

##### kairu 11/08/2019 17:16:52
hey everyone is there a way to attach a receiver to a robot like the epuck?

##### David Mansolino [Cyberbotics] 11/08/2019 17:18:41
`@kairu` yes of course, you can add it in the 'turretSlot' field.

##### kairu 11/08/2019 17:31:24
ty!

##### David Mansolino [Cyberbotics] 11/08/2019 17:56:59
You're welcome

##### Yamda 11/09/2019 01:30:46
hello

##### Hayden Woodger 11/09/2019 01:31:11
Hi there

##### KyleM 11/09/2019 03:09:29
Hey folks

##### Hayden Woodger 11/09/2019 03:09:36
hey

##### KyleM 11/09/2019 03:10:04
Hi 🙂


I have a vehicle driving over a mesh I imported


Webots is flickering the mesh red


At times of collision


How do I turn this off?

##### Hayden Woodger 11/09/2019 03:11:41
Could you upload a video of  it happening please.

##### KyleM 11/09/2019 03:12:51
Sure

##### Hayden Woodger 11/09/2019 03:12:58
thank you 🙂

##### KyleM 11/09/2019 03:15:00

> **Attachment**: [Screencast\_2019-11-08\_201306.m4v](https://cdn.discordapp.com/attachments/565154703139405824/642562758671597601/Screencast_2019-11-08_201306.m4v)


The terrain is "highlighted" red


and I don't really want to see wireframe of the terrain either

##### Hayden Woodger 11/09/2019 03:16:52
Sorry your talking about the mesh on the wheels. They are turning red on collision. You'll need to disable this, but i'm not sure exactly where that setting is.

##### KyleM 11/09/2019 03:18:19
It's hard to tell but the terrain's mesh lines are red which indicate collision I think


And...


I think I just fixed it

##### Hayden Woodger 11/09/2019 03:18:43
That's what i wanted to hear 🙂

##### KyleM 11/09/2019 03:19:23
View > Optional Rendering > Show All Bounding Objects


I tried this before but it had no effect. Strange...


But it finally turned off so I'm happy 🙂

##### Hayden Woodger 11/09/2019 03:20:07
Sometimes i think the world needs to be reloaded possibly


But nice one 🙂


Thanks for explaning how you did it

##### KyleM 11/09/2019 03:21:02
Sure thing and thanks for responding automaton

##### Hayden Woodger 11/09/2019 03:21:16
Anytime

##### JFXian 11/09/2019 07:49:47
Hey guys, I want to to build a planar four-bar mechanism simulation in webots, does webots support closed kinematic loops?



%figure
![plotclock.png](https://cdn.discordapp.com/attachments/565154703139405824/642631973356961813/plotclock.png)
%end

##### Hayden Woodger 11/09/2019 07:50:32
yes

##### JFXian 11/09/2019 08:01:57
But if I start with this link to build this model, the end point of the last joint is the link itself😂
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/642634975820120073/unknown.png)
%end

##### Hayden Woodger 11/09/2019 08:04:15
Ah you've got me there. I would use 1 arm instead. Less computation and less work in general.

##### JFXian 11/09/2019 08:13:15
😋

##### chamandana 11/09/2019 16:55:06
hello everyone, Please note that I'm a noob to Webots. I got a problem with KUKA youBot. Here's my code. [https://pastebin.com/j3h9Z4Y3](https://pastebin.com/j3h9Z4Y3). I've only called base\_turn\_left() but the bot seems to be going forward, and also turning to the right as shown in the video :/ Can anyone help me figure out what's wrong?
> **Attachment**: [C\_\_Users\_Chamuth\_Documents\_P\_Bots\_Round2\_Round2\_worlds\_Round2.wbt\_Round2\_-\_Webots\_R2019b\_2019-11-09\_.mp4](https://cdn.discordapp.com/attachments/565154703139405824/642769143602348055/C__Users_Chamuth_Documents_P_Bots_Round2_Round2_worlds_Round2.wbt_Round2_-_Webots_R2019b_2019-11-09_.mp4)

##### threeal 11/09/2019 21:58:39
is there any way to get the value of controlPID in the motor using python?

##### Hayden Woodger 11/09/2019 21:59:49
[https://cyberbotics.com/doc/reference/motor?tab-language=python](https://cyberbotics.com/doc/reference/motor?tab-language=python)


Scroll down to motor functions and change the code to python.

##### threeal 11/09/2019 22:28:41
but there is no function to get control pid value, just set

##### Hayden Woodger 11/09/2019 22:35:05
Sorry you are right, my mistake.


You can get the velocity and position though.

##### threeal 11/10/2019 09:23:16
i have been able to create ros controller in webots using python. when will there be  a support for ros controller using c++? as i think it would be faster using that.

##### Tahir [Moderator] 11/10/2019 10:41:21
ROS controller is written in C++. Its upon you how you handle your program on ROS side. From Webots everything is available in the form of Services, now its upon you how you handle it wither in Python or C++.

##### threeal 11/10/2019 10:58:10
i mean the custom ros controller, not the ros controller that been provided by webots

##### Tahir [Moderator] 11/10/2019 11:18:48
Well, I am not good in roscpp but just going through the ros folder provided by Webots also includes roscpp as well.


So I think you can try to code with that and see what does it says and than fix that


Sorry if I am wrong

##### pedagil 11/10/2019 15:51:49
hey guys, im trying to record a video in webots, and process is stuck at INFO: Creating video..


does anybody have any advice?

##### Prasad 11/10/2019 17:27:16
`@pedagil` You need to wait for a while. I did that recording yesterday. It did take some while (like 5 minutes) to process some 15 seconds video

##### thrilok emmadsietty 11/10/2019 23:46:47
i am new to this software where can i get the tutorials?

##### JFXian 11/11/2019 00:30:37
On the official website, [https://www.cyberbotics.com/doc/guide/getting-started-with-webots](https://www.cyberbotics.com/doc/guide/getting-started-with-webots)

##### Stefania Pedrazzi [Cyberbotics] 11/11/2019 07:06:40
`@JFXian`: about closed kinematics loops, you can model them inserting a SolidReference node ([https://www.cyberbotics.com/doc/reference/solidreference](https://www.cyberbotics.com/doc/reference/solidreference)) in the joint endPoint field. This node is a reference to an existing Solid and allows you to reuse the same Solid node twice in the model.

For an example you can look at the `projects/samples/demows/worlds/stewart_platform.wbt` example ([https://www.cyberbotics.com/doc/guide/samples-demos#stewart\_platform-wbt](https://www.cyberbotics.com/doc/guide/samples-demos#stewart_platform-wbt))


`@threeal`: to get the control PID values you can use the Supevisor API function to read the node field value. There is no Motor API function to retrieve it directly.


`@threeal`: What do you mean exactly by "support for ros controller in c++"? You can already implement a ros controller as webots controller in c++ as we did when implementing the default ROS interface procided by Webots.


`@chamandana` please pay attention that the youBot omni wheels only work if the WorldInfo.contactProperties of the world are set correctly. You should open the default `youbot.wbt` world and copy them in your world.

##### threeal 11/11/2019 08:17:20
`@Stefania Pedrazzi` so how is the relation between supervisor and the robot? and does it still written in robot controller?

##### Stefania Pedrazzi [Cyberbotics] 11/11/2019 08:18:34
You can enable the Supervisor functionality in a robot controller by setting the `Robot.supervisor` field to TRUE

##### threeal 11/11/2019 08:19:34
`@Stefania Pedrazzi` the webots only provides a sample tutorial for creating ros controller in python, and not in c++, and i dont know how to link the webots library and ros library in catkin make


thank you, i will try to use the supervisor features in the webots

##### Stefania Pedrazzi [Cyberbotics] 11/11/2019 08:22:54
Can you post the link to the sample tutorial for ros that you are referring to?

##### JFXian 11/11/2019 08:34:20
`@Stefania Pedrazzi`, thank you, i'll try

##### Prasad 11/11/2019 14:50:07
`@Fabien Rohrer` Regarding [https://github.com/cyberbotics/webots/issues/1081](https://github.com/cyberbotics/webots/issues/1081) , shall we get on a call and close this one out? I am not able to replicate this consistently. However, I will be more than happy to just get it running properly with stability.

##### Fabien Rohrer [Moderator] 11/11/2019 14:53:10
Hi, I just read your messages.

##### Prasad 11/11/2019 14:54:15
That is great

##### Fabien Rohrer [Moderator] 11/11/2019 14:56:36
I'm sorry, but this issue is difficult to follow because it reports several issues at the same time.


Could we discuss of them one by one?


1. Is my answer about timestep and mavic satisfying?


(I expect its normal to not have a perfect match, because the constants are hardcoded. The controller is not robust enough to support this. So setting the constants exactly as in mavic\_2\_pro.wbt should solve your issue.)


respectfully 😉

##### Prasad 11/11/2019 15:04:51
Yes `@Fabien Rohrer`  I agree with you! However, as it is just PID controller, I believe, it will converge to the position without the need of world parameters like dampening etc. (I am deliberately avoiding too many parameters) + if I set `targetX` and `targetY` to -1.0 and -1.0, the quadcopter flies rock solid


And you are also right regarding several issues. I would help you with whatever info I can provide! Let me see if I can break that issue

##### Fabien Rohrer [Moderator] 11/11/2019 15:12:09
Ok, let's focus on this first.


I added damping to render the controller more easy to write. It may indeed lose some physics accuracy doing so. For sure, in reality, their is some damping, but certainly not so big.


So you're right telling that world parameters should not affect so much the simulation.


Is it satisfying? Could we "close" this topic?

##### Prasad 11/11/2019 15:17:25
> So you're right telling that world parameters should not affect so much the simulation.



You are right! I meant that.



> Could we "close" this topic?



Sure!

##### Fabien Rohrer [Moderator] 11/11/2019 15:17:35
Ok.


The second topic is about the pioneer control, right?


Do you still have issues with this?

##### Prasad 11/11/2019 15:17:58
Yes


> Do you still have issues with this?



Yes

##### Fabien Rohrer [Moderator] 11/11/2019 15:18:42
If I understand well: when the basictimestep is modified, it affects the robot speed, right?

##### Prasad 11/11/2019 15:18:54
To some extent yes!


Sometimes, the rover gets stuck also. Either the rover gets stuck or the camera flickers and goes gray in certain red regions

##### Fabien Rohrer [Moderator] 11/11/2019 15:20:24
Are the camera values used to control somehow the motor speeds?

##### Prasad 11/11/2019 15:21:08
Camera values as in? Image processing? Yes. I am using image processing to control motor speeds.

Camera values as in parameters like width and height? No. Those are fixed

##### Fabien Rohrer [Moderator] 11/11/2019 15:22:21
yes, I meant image processing.


So, the first thing to determine is that: can you move the robot smoothly when the image processing is not used?

##### threeal 11/11/2019 15:23:15
`@Stefania Pedrazzi` here [https://github.com/cyberbotics/webots/tree/revision/projects/languages/ros/controllers/ros\_python](https://github.com/cyberbotics/webots/tree/revision/projects/languages/ros/controllers/ros_python)

##### Prasad 11/11/2019 15:23:31
Definitely yes.

##### Fabien Rohrer [Moderator] 11/11/2019 15:23:41
`@Prasad` let's switch to a direct discussion 🙂

##### Prasad 11/11/2019 15:23:45
But, in my tests, rover was teleoperated alone


Sure!

##### Stefania Pedrazzi [Cyberbotics] 11/11/2019 15:33:53
`@threeal`: ok, now I better understand what you mean. For C++ there is the complete standard `ros` controller implementation ([https://github.com/cyberbotics/webots/tree/revision/projects/default/controllers/ros](https://github.com/cyberbotics/webots/tree/revision/projects/default/controllers/ros))

The C++ files corresponding to the `ros_controller.py` file are defined here [https://github.com/cyberbotics/webots/tree/revision/projects/languages/ros/webots\_ros](https://github.com/cyberbotics/webots/tree/revision/projects/languages/ros/webots_ros). It also contains a sample of the CMakeLists.txt file that you could adapt to write your simple ros example in cpp. 

Other instructions for the standard ROS controller in cpp that you could need are described here [https://cyberbotics.com/doc/guide/using-ros#standard-ros-controller](https://cyberbotics.com/doc/guide/using-ros#standard-ros-controller)

##### threeal 11/11/2019 15:49:48
can i use the makefile here ([https://github.com/cyberbotics/webots/tree/revision/projects/default/controllers/ros](https://github.com/cyberbotics/webots/tree/revision/projects/default/controllers/ros)) to build my own controller that support ros in c++?

##### Stefania Pedrazzi [Cyberbotics] 11/11/2019 15:54:38
yes, you can reuse it. You should also copy the `include` folder.

##### threeal 11/11/2019 16:14:12
what about the msg and srv? will it able to builds them? and where will it located? and how can i include it to my main ros workspace?

##### Stefania Pedrazzi [Cyberbotics] 11/11/2019 16:15:27
if you copy the include folder in your controller folder, then they should be built automatically (even if probably you won't need all of them)


To include them into the ros workspace you just have to copy the msg and srv folders into the catkin workspace.


Here are some old instructions to setup manually the webots\_ros package (before it was published in ros): [https://cyberbotics.com/doc/guide/tutorial-7-using-ros?version=cyberbotics:R2019a#webots\_ros-package-installation](https://cyberbotics.com/doc/guide/tutorial-7-using-ros?version=cyberbotics:R2019a#webots_ros-package-installation)

##### threeal 11/11/2019 17:04:24
thank you `@Stefania Pedrazzi`

##### machinekoder 11/12/2019 06:54:44
I would like to simulate a 3D/ToF camera. For this purpose I've set up a sim with with a Lidar device. The ToF cam has a resolution of 224x171 and FOV of 62x45deg, I've entered that in Webots. So far so good.



When I now try to use the sim with a `ros` controller, the controller just crashes. I supect that this is due to the high layer resolutions, since it doesn't crash when I reduce the layers. 



Any ideas how to resolve this?


I've shared the sim here: [https://wolke.roessler.systems/s/q3Map9kMnPxwYA6](https://wolke.roessler.systems/s/q3Map9kMnPxwYA6)

##### Fabien Rohrer [Moderator] 11/12/2019 07:37:26
`@machinekoder` Hi,


Correct me if I'm wrong, but I think that using a Webots RangeFinder would be closer than a Webots Lidar node to simulate a 3D/ToF camera:


[https://cyberbotics.com/doc/guide/samples-devices#range\_finder-wbt](https://cyberbotics.com/doc/guide/samples-devices#range_finder-wbt)


[https://www.cyberbotics.com/doc/reference/rangefinder](https://www.cyberbotics.com/doc/reference/rangefinder)


.. and more efficient

##### Hayden Woodger 11/12/2019 07:38:53
Could somebody please tell what a 3D/Tof is? xD

##### Fabien Rohrer [Moderator] 11/12/2019 07:39:11
That said, there is no reason that the ROS controller crashes when increasing the number of layers.


Unless you reach a limitation with your GPU.


=> what is your GPU? are you above the minimal requirements? [https://cyberbotics.com/doc/guide/system-requirements](https://cyberbotics.com/doc/guide/system-requirements) Are your drivers up-to-date?


> Could somebody please tell what a 3D/Tof is? xD


It's a camera which can retrieve the depth information for each pixel, using time of flight: [https://en.wikipedia.org/wiki/Time-of-flight\_camera](https://en.wikipedia.org/wiki/Time-of-flight_camera)

##### Hayden Woodger 11/12/2019 07:44:52
Thank you for your response Fabien. In your opinion, is this the fastest method in terms of response time?

##### Fabien Rohrer [Moderator] 11/12/2019 07:45:24
Yes, we use to use this way to simulate kinect cameras for example.

##### Hayden Woodger 11/12/2019 07:45:54
Used to? So what method would you use now?

##### Fabien Rohrer [Moderator] 11/12/2019 07:45:59
like here (old movie): [https://www.youtube.com/watch?v=9Fjyu\_wzIgc](https://www.youtube.com/watch?v=9Fjyu_wzIgc)


This method is still in use (sorry, it was not what I meant.)

##### Hayden Woodger 11/12/2019 07:52:37
That's awesome 🙂  I see what you mean now, thank you! I'm curious, did you use ikpy for the kinematics on the arm?

##### Fabien Rohrer [Moderator] 11/12/2019 07:54:45
Not in this example. This is a "home-made" IK algorithm: [https://github.com/cyberbotics/webots/blob/master/projects/robots/kuka/youbot/libraries/youbot\_control/src/arm.c#L184](https://github.com/cyberbotics/webots/blob/master/projects/robots/kuka/youbot/libraries/youbot_control/src/arm.c#L184)


At this time, ikpy was not released 🙂


However, we provide another example using ikpy:

##### Hayden Woodger 11/12/2019 07:55:18
I know i've used the code a lot ;P

##### Fabien Rohrer [Moderator] 11/12/2019 07:55:23
[https://www.cyberbotics.com/doc/guide/irb4600-40](https://www.cyberbotics.com/doc/guide/irb4600-40)


[https://www.youtube.com/watch?v=Jq0-DkEwwj4](https://www.youtube.com/watch?v=Jq0-DkEwwj4)

##### Hayden Woodger 11/12/2019 07:56:16
Yeah yeah i've seen it, it's very impressive xD


I'm amazed how well ikpy works, you can pretty much apply it every robot (thank you again

##### Fabien Rohrer [Moderator] 11/12/2019 07:57:29
Yes, this Python module is amazing


(but a bit slow 😦 )

##### Hayden Woodger 11/12/2019 07:58:19
Do you think so?


If you don't synconize the controller it'll go fast.

##### Fabien Rohrer [Moderator] 11/12/2019 08:00:25
Yes, it was slowing down the ABB simulation, I had to reduce the controller rate to be able to run in real time. It's not very annoying in such demo, but I dreamed to use it to control the legs of a spider robot, and in this case, I think it's too slow 😦


> f you don't synconize the controller it'll go fast.


Good point

##### Hayden Woodger 11/12/2019 08:04:31
I have applied the inverse kinematics to each arm and leg of Atlas. When I try and run the simulation, whilst telling the arms and legs to go to new x,y,z position, it dramatically slows down the simulation.

##### Fabien Rohrer [Moderator] 11/12/2019 08:05:21
Yes, IK is for sure a slow task.


On the other hand, I have not a lot experiment in other libraries than ikpy

##### machinekoder 11/12/2019 08:05:52
`@Fabien Rohrer` I assumed the same, but the RangeFinder doesn't give me PointCloud. So the Lidar device seems to be the better fit.

##### Hayden Woodger 11/12/2019 08:06:16
I haven't tested un-synchronizing the controller on that yet though. I will sometime today and upload the results 🙂

##### Fabien Rohrer [Moderator] 11/12/2019 08:06:41
`@Hayden Woodger` Please share your results 😉

##### Hayden Woodger 11/12/2019 08:06:54
I shall ;P

##### machinekoder 11/12/2019 08:07:05
`@Fabien Rohrer` GPU is NVidia Quadro P2000

##### Fabien Rohrer [Moderator] 11/12/2019 08:07:45
`@machinekoder` Ok, and did you check your GPU drivers?

##### machinekoder 11/12/2019 08:08:26
Yes, most recent nvidia proprietary drivers.

##### Fabien Rohrer [Moderator] 11/12/2019 08:09:45
Do you have a stack to share when the ROS controller crashes?

##### machinekoder 11/12/2019 08:10:51
How can I generate one? the console output only give me a short error message.


exact specs:

```
glxinfo -B
name of display: :0
display: :0  screen: 0
direct rendering: Yes
Memory info (GL_NVX_gpu_memory_info):
    Dedicated video memory: 4096 MB
    Total available memory: 4096 MB
    Currently available dedicated video memory: 2878 MB
OpenGL vendor string: NVIDIA Corporation
OpenGL renderer string: Quadro P2000/PCIe/SSE2
OpenGL core profile version string: 4.6.0 NVIDIA 410.104
OpenGL core profile shading language version string: 4.60 NVIDIA
OpenGL core profile context flags: (none)
OpenGL core profile profile mask: core profile

OpenGL version string: 4.6.0 NVIDIA 410.104
OpenGL shading language version string: 4.60 NVIDIA
OpenGL context flags: (none)
OpenGL profile mask: (none)

OpenGL ES profile version string: OpenGL ES 3.2 NVIDIA 410.104
OpenGL ES profile shading language version string: OpenGL ES GLSL ES 3.20
```

##### Fabien Rohrer [Moderator] 11/12/2019 08:11:56
Your GPU should work well with Webots.


To have a stack on linux, probably that the ROS controller should be build in debug mode... It begins to be too difficult to solve here.


Could I ask you to fill a bug report here:


[https://github.com/cyberbotics/webots/issues](https://github.com/cyberbotics/webots/issues)


With info about your system (Webots version, OS version, etc.)?


It would be much simpler to track this issue which seems to be a bug in our ROS controller..

##### machinekoder 11/12/2019 08:15:50
Okay I will. My guess is that has something to do with the amount of published "layer" laser scan topics, since reducing the layers fixes the issue

##### Fabien Rohrer [Moderator] 11/12/2019 08:17:19
Yes, certainly. But this does not explain the crash. I would be on a bad memory management somewhere. But we should be able reproduce it in order to fix it.


Thank you for the bug report.

##### aysegulucar 11/12/2019 16:55:55
Hi, 



We try to walk op3 at webots. We are changing the codes of op2.



Is it important EEPROM Area for using op3 at simulation. they are different from op2. We did not changed them.


Thanks


[http://emanual.robotis.com/docs/en/platform/op2/getting\_started/](http://emanual.robotis.com/docs/en/platform/op2/getting_started/)

##### Anoop 11/12/2019 18:26:47
HI, I'm using Windows 10 64-bit & have Nvidia gtx 1080 TI with the updated drivers. But I installed the current version of Webots, i.e., R2019b-rev1 it outouts an FATAL error; saying that "Webots use OpelGL 3.3 but only OpenGL 1.1 can be initialized. Check for latest drivers". Can anybody help with this? Thanks in Advance!

##### Stefania Pedrazzi [Cyberbotics] 11/13/2019 07:18:08
`@Anoop` did you check if your GPU is correctly detected by Webots? could you post the content of the Help > OpenGL information message box (or the output of `webots --sysinfo`)?

##### Redsign DEMI 11/13/2019 14:48:31
I am used to designing in Catia V5. How could I import my design to webot

##### Hayden Woodger 11/13/2019 14:49:03
What he/she said  /\


Seems like you can only import VRML97??


Why not something more recent like stl, model, etc..


I'm sure there's a very good reason why

##### Fabien Rohrer [Moderator] 11/13/2019 14:51:46
Yes, Webots relies on VRML97, it's a good way to import something in Webots.

##### Hayden Woodger 11/13/2019 14:52:29
Fair enough, what program are you using to generate VRMLs?

##### Fabien Rohrer [Moderator] 11/13/2019 14:53:09
We usually use Blender in-between. And this Blender plugin: [https://github.com/cyberbotics/blender-webots-exporter](https://github.com/cyberbotics/blender-webots-exporter)


I think it's the best solution, but it requires Blender skills

##### Hayden Woodger 11/13/2019 14:53:48
You're a legend, i was looking into fusion 360 xD


i know right

##### RossBatten 11/15/2019 00:35:11
I'm having trouble getting pycharm to work with webots, getting an error that it can't find \_controller that gets imported in the controller.py script


From what I can see, the \_controller.pyd file is in the same directory as the controller.py file so how is it able to find one but not the other?


For reference, I'm using anaconda python 3.7.5 in windows


Any help I can get would be much appreciated


Both files are in the source root directory: C:\Program Files\Webots\lib\python37


ImportError: DLL load failed: The specified module could not be found.


Does this mean the file was found but the module contained was unable to be read?  Or does it mean the dll couldn't be found


?

##### Hayden Woodger 11/15/2019 00:49:52
I had the same issue, didn't find a solution so


i had to go back to python 2.7

##### RossBatten 11/15/2019 00:56:24
I have been reading that this error comes up when the dll is compiled in a different python version to the one you're using


Could be it's compiled in 2.7


That wouldn't make sense since the folder the \_controller.pyd file resides in is Webots/lib/python37


I ended up getting it to work by swiching to vscode environment.  Still unsure of the cause

##### Fabien Rohrer [Moderator] 11/15/2019 07:49:28
I recently added documentation about IDEs:  [https://cyberbotics.com/doc/guide/using-your-ide?version=revision](https://cyberbotics.com/doc/guide/using-your-ide?version=revision)

##### JoanGerard 11/15/2019 15:43:23
Hello, do you know if it is possible to see a plugin in the web simulation?

##### JoHn 11/15/2019 15:48:36
Hi guys, an issue happens to me frequently when I launch Webots, please see the screenshot below:



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/644926763268571168/unknown.png)
%end


With the same setup and software, Webots launches fine sometimes, but it also crashes like this sometimes.

##### Fabien Rohrer [Moderator] 11/15/2019 15:51:02
`@JoanGerard` Webots allows to stream the content of a simulation to web pages. As explained here: [https://cyberbotics.com/doc/guide/web-streaming](https://cyberbotics.com/doc/guide/web-streaming) What do you mean by plugin?

##### JoHn 11/15/2019 15:51:47
I feel that when I reduce the 'basicTimeStep' in WorldInfo, it could help. But I am not entirely sure about it.

##### JoanGerard 11/15/2019 15:53:27
`@Fabien Rohrer`  I mean a controller plugin like this one: [https://cyberbotics.com/doc/guide/controller-plugin](https://cyberbotics.com/doc/guide/controller-plugin)

##### Fabien Rohrer [Moderator] 11/15/2019 15:54:59
`@JoHn` regarding your log, it seems that the Webots ROS controller crashes. And not Webots. It would be awesome if you could open a bug report [https://github.com/cyberbotics/webots/issues](https://github.com/cyberbotics/webots/issues) and even more awesome if you could have a stack and a simple way to reproduce it 😅

##### JoHn 11/15/2019 15:55:23
Now I set 'basicTimeStep' in WorldInfo to 12, TIME\_STEP of all ServiceClient in the ROS node to 20, and the loop rate to 10.



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/644928458547265547/unknown.png)
%end

##### Fabien Rohrer [Moderator] 11/15/2019 15:58:01
`@JoanGerard` yes, our web server example can stream the HTML robot windows: [https://cyberbotics.com/doc/guide/web-simulation](https://cyberbotics.com/doc/guide/web-simulation)

##### JoanGerard 11/15/2019 16:01:08
I am working on a robot positioning prediction using ML techniques and Probabilistic methods so I developed a plugin that shows me the real, odometry, and predicted position of a simple robot in a simple environment and I would like to put it online... I would like to know if there is someone else who is working in the same or similar subject... please contact me if so.
%figure
![Screen_Shot_2019-11-15_at_4.56.49_PM.png](https://cdn.discordapp.com/attachments/565154703139405824/644929889677606912/Screen_Shot_2019-11-15_at_4.56.49_PM.png)
%end

##### JoHn 11/15/2019 16:01:44
Hi `@Fabien Rohrer`, thank you very much for your response. Yes, I will open the bug report as you instructed.

##### JoanGerard 11/15/2019 17:48:58
is there any tutorial on how to create a Web Simulation using AWS?


Do you know why I can't update Webots?
%figure
![Screen_Shot_2019-11-15_at_8.47.23_PM.png](https://cdn.discordapp.com/attachments/565154703139405824/644987297095483408/Screen_Shot_2019-11-15_at_8.47.23_PM.png)
%end

##### David Mansolino [Cyberbotics] 11/15/2019 20:24:13
`@JoanGerard` maybe you are behind a firewall that blocks cyberbotics.com, can you access [https://www.cyberbotics.com/webots\_current\_version.txt](https://www.cyberbotics.com/webots_current_version.txt) ?


`@JoanGerard`, About AWS, we are currently actively collaborating with Amazon to integrate Webots in AWS RoboMaker we already have some functional prototype working with ROS2, would you be interested in testing this?

##### Dorteel 11/16/2019 13:33:08
Hi!

I have problems getting the sample world example.wbt to work for some reason. When I load it the keyboard commands won't work. The first time I tried had no issues with it, but now it won't work. any suggestions?

##### Tahir [Moderator] 11/16/2019 13:34:24
Hello

##### Dorteel 11/16/2019 13:35:33
Didn't change anything in the controllers, so have no idea what should I try. The example was in Python

##### Tahir [Moderator] 11/16/2019 13:41:36
Can you just restart everything and check

##### Dorteel 11/16/2019 13:44:31
I tried restarting the simulation, reloading the world, and restarting WeBots too, none of them works anymore. Restarting WeBots worked once half an hour ago, but it doesn't work either anymore.


It works again, thanks!

##### JoanGerard 11/16/2019 21:09:43
`@David Mansolino` I do have access to [https://www.cyberbotics.com/webots\_current\_version.txt](https://www.cyberbotics.com/webots_current_version.txt)  but Webots continues giving me that error when I try to update it. Yes, I'm interested in testing that functional prototype.


When my simulation server tries to execute the following code:



/usr/local/webots/webots --batch --mode=pause --minimize --stream="port=81;monitorActivity;controllerEdit" /tmp/webots/instances/140514729

106960/worlds/four\_wheels.wbt



It throws the following error message: 



Info: Could not load the Qt platform plugin "xcb" in "" even though it was found.

Fatal: This application failed to start because no Qt platform plugin could be initialized. Reinstalling the application may fix this problem.

Available platform plugins are: xcb.

/usr/local/webots/webots: line 51:  4815 Aborted                 (core dumped) "$webotsHome/bin/webots-bin" "$@"

##### David Mansolino [Cyberbotics] 11/17/2019 09:03:42
`@JoanGerard` are able to launch Webots on your machine (directly, without using the simulation server) ?

##### JoanGerard 11/17/2019 12:54:57
you mean trying to lunch a project manually on my simulation server?


I tried executing "webots --version" on my simulation server and it throws the same error message


I'm using Ubuntu 18.04


`@David Mansolino` 👆

##### David Mansolino [Cyberbotics] 11/17/2019 14:30:38
`@JoanGerard` are you using the server version of Ubuntu 18.04 ?

##### JoanGerard 11/17/2019 14:32:01
I am using this version from GCloud:

Ubuntu 18.04 LTS

amd64 bionic image built on 2019-11-13

##### David Mansolino [Cyberbotics] 11/17/2019 14:33:45
Have you installed Webots from the debian, tarball and snap package?

##### JoanGerard 11/17/2019 14:34:55
I installed from here: [https://cyberbotics.com/doc/guide/installation-procedure#using-advanced-packaging-tool-apt](https://cyberbotics.com/doc/guide/installation-procedure#using-advanced-packaging-tool-apt)


It gives the same error message, then I tried this:  [https://snapcraft.io/install/webots/ubuntu](https://snapcraft.io/install/webots/ubuntu)


and same


but I should not receive any error message when I execute "webots --version" command right?

##### David Mansolino [Cyberbotics] 11/17/2019 14:36:29
Ok so you tried the debian and snap, they should be fine.


> but I should not receive any error message when I execute "webots --version" command right?



No indeed, it should just display the version


Are you able to run other GUI application on your machine?

##### JoanGerard 11/17/2019 14:38:15
I am kind of new with cloud computing so I only see the command line


so I did not tried to install/open other GUI applications

##### David Mansolino [Cyberbotics] 11/17/2019 14:38:43
Ok, so that might be the problem if you don't have any GUI interfac


In that case you can take inspiration from our Docker procedure to run Webots without GUI: [https://github.com/cyberbotics/webots/wiki/Docker](https://github.com/cyberbotics/webots/wiki/Docker)

##### JoanGerard 11/17/2019 14:42:06
thanks! I will try to use that instead!


so in theory I could use that docker image to create the simulator server right?

##### David Mansolino [Cyberbotics] 11/17/2019 14:45:16
You're welcome


Yes of course it should be feasible.

##### Nocturnal Warfare 11/18/2019 04:38:06
[https://cyberbotics.com/terrain\_generator](https://cyberbotics.com/terrain_generator) does this work for anyone? I click generate and nothing happens

##### Stefania Pedrazzi [Cyberbotics] 11/18/2019 07:13:21
`@Nocturnal Warfare` no, the terrain generator seems to be broken. We will check the issue..

##### Olivier Michel [Cyberbotics] 11/18/2019 10:06:15
Hi, I just fixed the terrain generator. It should work now.

##### JoanGerard 11/18/2019 11:04:55
Hello, I'm still trying to run Webots Simulator without success... 

I receive this information on simulator server even though I'm running a GPU instance with NVIDIA Tesla P100.
%figure
![Screen_Shot_2019-11-18_at_12.01.57_PM.png](https://cdn.discordapp.com/attachments/565154703139405824/645942508873842688/Screen_Shot_2019-11-18_at_12.01.57_PM.png)
%end


The monitor does not show webots running in any time I do not know why...
%figure
![Screen_Shot_2019-11-18_at_12.07.28_PM.png](https://cdn.discordapp.com/attachments/565154703139405824/645943284505509918/Screen_Shot_2019-11-18_at_12.07.28_PM.png)
%end


I receive this message together with a "WebSocket network error: The operation couldn’t be completed. Connection refused" console message even though I have opened the ports from 81 to 3000 tcp on my simulator server to allow websockets communication
%figure
![Screen_Shot_2019-11-18_at_12.08.13_PM.png](https://cdn.discordapp.com/attachments/565154703139405824/645943827311362119/Screen_Shot_2019-11-18_at_12.08.13_PM.png)
%end


can you give me a hand with this?

##### Stefania Pedrazzi [Cyberbotics] 11/18/2019 12:18:22
Hi `@JoanGerard`, the issue with the GPU detection is not related to the problem connecting to the simulation server. You can check which GPU is detected by webots by running `webots --sysinfo` in a terminal on the simulation server. Are your NVIDIA drivers up-to-date?


For the connection issue, a first thing to test is to start webots manually on the simulation server and try to connect to it directly using the `streaming_viewer` page ([https://github.com/cyberbotics/webots/tree/revision/resources/web/streaming\_viewer](https://github.com/cyberbotics/webots/tree/revision/resources/web/streaming_viewer))


You can retrieve the exact command used by the simulation server script to start Webots in the logs (usually `<WEBOTS_INSTALLATION>/resources/web/server/log/simulation/output.log`) and just remove the world path


Please try it and let us know what you get.

##### JoanGerard 11/18/2019 16:00:55
thanks for your answer so I got these errors below:



ERROR: Error when creating the TCP streaming server: Cannot set the server in listen mode: The address is protected

AL lib: (WW) alc\_initconfig: Failed to initialize backend "pulse"

ALSA lib conf.c:3916:(snd\_config\_update\_r) Cannot access file /usr/share/alsa/alsa.conf

ALSA lib pcm.c:2495:(snd\_pcm\_open\_noupdate) Unknown PCM default

AL lib: (EE) ALCplaybackAlsa\_open: Could not open playback device 'default': No such file or directory

ERROR: Cannot initialize the sound engine: Cannot initialize OpenAL default device 'OpenAL Soft'

ALSA lib conf.c:3916:(snd\_config\_update\_r) Cannot access file /usr/share/alsa/alsa.conf

ALSA lib pcm.c:2495:(snd\_pcm\_open\_noupdate) Unknown PCM default

AL lib: (EE) ALCplaybackAlsa\_open: Could not open playback device 'default': No such file or directory

ERROR: Cannot initialize the sound engine: Cannot initialize OpenAL default device 'OpenAL Soft'


while trying to execute: webots --stdout --stderr --batch --no-sandbox --mode=pause --minimize --stream="port=84;monitorActivity;controllerEdit" ~/cyberbotics/streaming\_projects/samples/worlds/binocular.wbt

##### Stefania Pedrazzi [Cyberbotics] 11/18/2019 16:04:50
> ERROR: Error when creating the TCP streaming server: Cannot set the server in listen mode: The address is protected

there is clearly an issue whith the port. It seems that the address you are trying to listen on is available only to privileged applications (i.e. application ran as root).

You should check your network configurations and change them

##### Nocturnal Warfare 11/18/2019 19:11:58
`@Olivier Michel` i just tried using the terrain generator and still nothing seems to be happening. Am I just using it wrong? You just scale the map to what you want to generate then hit generate right?

##### threeal 11/18/2019 20:05:30
does webots support ros 2?

##### Olivier Michel [Cyberbotics] 11/18/2019 20:28:35
`@Nocturnal Warfare`: I tried it now and it works for me, but it takes time. Just below the map, you should see a counter increasing until it displays "Ready" and the Webots fill will appear in the text area below.

##### David Mansolino [Cyberbotics] 11/18/2019 20:36:03
`@threeal`, yes, we have already an interface with ROS2: [http://wiki.ros.org/webots\_ros2](http://wiki.ros.org/webots_ros2)

We are still improving it, you can follow the development and get the latest version here: [https://github.com/cyberbotics/webots\_ros2.git](https://github.com/cyberbotics/webots_ros2.git)

##### threeal 11/18/2019 21:21:42
does the performance better in ros 2 or is it the same?

##### Nocturnal Warfare 11/18/2019 21:53:58
`@Olivier Michel` oh I got that working now thanks, but the staticmap.png doesn't work for me, it just says Google maps platform rejected your request

##### David Mansolino [Cyberbotics] 11/18/2019 22:16:50
`@threeal` yes we do expect a better integration and performance, however since the interface is still in development we can't give precise measurements for now.

##### JoanGerard 11/19/2019 01:43:46
So when I execute webots manually from the simulator server and I use the stream\_viewer example everything works good. 



The server.sh file needs to be executed with sudo command otherwise the simulator\_server.py crashes. The problem arises when the instance of Webots is lunched due to the connexion of a new client cause it is been executed as root. Do you have any idea on how I can solve this?
%figure
![Screen_Shot_2019-11-19_at_2.28.06_AM.png](https://cdn.discordapp.com/attachments/565154703139405824/646163679787614223/Screen_Shot_2019-11-19_at_2.28.06_AM.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 11/19/2019 07:20:27
`@JoanGerard` it is a bad practice to run Webots as root. So please make sure that it works when you start Webots manually without root privileges. Then it seems that you installed Webots from the snap package. We never tested the streaming server with the snap version of Webots and it is very likely that it is doesn't work. I would suggest you to install Webots using the debian or tarball package [https://www.cyberbotics.com/download](https://www.cyberbotics.com/download)

##### HE110 11/19/2019 09:16:18
I programmed by python, but it told me:controller exited successful

##### Fabien Rohrer [Moderator] 11/19/2019 09:16:49
Hi


It seems your controller reaches successfully the end 😉


Could you provide a better description?


Maybe copying your controller here?

##### HE110 11/19/2019 09:17:41
Hold on


from controller import Motor

from controller import Robot

import time

rbt=Robot()



MAX\_SPEED = 6.28

left = Motor("left wheel motor")

right = Motor("right wheel motor")

t=0

right.setVelocity(6)

left.setVelocity(3)

while 1:

    time.sleep(1)

    t+=1

    if t<=5:

        right.setVelocity(6)

        left.setVelocity(3)

        # print(right)

        # print(left)

    elif 5<t<=10:       

        left.setVelocity(6)

        right.setVelocity(3)

        # print(right)

        # print(left)

    elif t>10:

        print("quit")

        break

##### Fabien Rohrer [Moderator] 11/19/2019 09:18:40
ok

##### HE110 11/19/2019 09:19:04
E puck cant move

##### Fabien Rohrer [Moderator] 11/19/2019 09:19:23
first of all, you need to replace time.sleep() by robot.step(time\_step)


with time\_step = int(robot.getBasicTimeStep())


secondly, your controller quits, because "t" reaches 10, and enter in the "quit"  section (the loop is broken)

##### HE110 11/19/2019 09:22:06
Im going to try it,thank u


How can i control it to move two circles?


Two circles likes "8"


TIME\_STEP is always 32


from controller import Motor

from controller import Robot

rbt=Robot()



MAX\_SPEED = 6.28

TIME\_STEP = 32

left = Motor("left wheel motor")

right = Motor("right wheel motor")



right.setVelocity(6)

left.setVelocity(3)

while 1:

    right.setVelocity(6)

    left.setVelocity(3)


It is always still

##### Fabien Rohrer [Moderator] 11/19/2019 09:46:46
It's still because you don't step the simulator: cf my comment above.


your loop is a blocking infinite loop.


to do a 8, you have to do a circle, then switch the velocties.


Let me show you...

##### HE110 11/19/2019 09:49:00
Thanks...


It is  toooooooo hard

##### Fabien Rohrer [Moderator] 11/19/2019 09:53:03
>>> from controller import Robot



robot = Robot()

timestep = int(robot.getBasicTimeStep())

TIME\_FOR\_A\_CIRCLE = 10.0   # 10 seconds to do a perfect circle? => to adjust



left = robot.getMotor("left wheel motor")

right = robot.getMotor("right wheel motor")

left.setPosition(float('inf'))  # Set motor in speed mode

right.setPosition(float('inf'))  # Set motor in speed mode



while robot.step(timestep) != -1:

    time = robot.getTime()

    if time < TIME\_FOR\_A\_CIRCLE:

      right.setVelocity(6)

      left.setVelocity(3)

    elif time < 2.0 * TIME\_FOR\_A\_CIRCLE:

      right.setVelocity(3)

      left.setVelocity(6)

    else:

      print('quit')

      break


I didn't test, but this controller should do a "8". You should only adjust TIME\_FOR\_A\_CIRCLE

##### HE110 11/19/2019 09:54:18
🤔


[PYTHON\_controller] Traceback (most recent call last):

[PYTHON\_controller]   File "PYTHON\_controller.py", line 9, in <module>

[PYTHON\_controller]     left.setPosition(float('inf'))  # Set motor in speed mode

[PYTHON\_controller] AttributeError: 'NoneType' object has no attribute 'setPosition'

##### Fabien Rohrer [Moderator] 11/19/2019 09:56:37
your error means that the motors cannot be found. Are you using the e-puck?


(I just test my controller, and it's workign)

##### HE110 11/19/2019 09:57:33
I creating new world and try again



%figure
![JPEG_20191119_175917.jpg](https://cdn.discordapp.com/attachments/565154703139405824/646288457550921740/JPEG_20191119_175917.jpg)
%end

##### Fabien Rohrer [Moderator] 11/19/2019 10:00:45
Select the controller field, and press on the "edit" button. Make sure PYTHON\_controller is correct.

##### HE110 11/19/2019 10:03:09
Im sure it's correct.

##### Fabien Rohrer [Moderator] 11/19/2019 10:03:19
what's the behavior now?

##### HE110 11/19/2019 10:04:14
[PYTHON\_controller] Traceback (most recent call last):

[PYTHON\_controller]   File "PYTHON\_controller.py", line 9, in <module>

[PYTHON\_controller]     left.setPosition(float('inf'))  # Set motor in speed mode

[PYTHON\_controller] AttributeError: 'NoneType' object has no attribute 'setPosition'


.....

##### Fabien Rohrer [Moderator] 11/19/2019 10:05:10
This is very weird.


It means that "left" is None.


So `left = robot.getMotor("left wheel motor")` failed


the name of the e-puck left motor is for sure "left wheel motor"


Did you changed something? (in the controller, or in the e-puck PROTO)


(I tested, and it's working)

##### HE110 11/19/2019 10:06:41
.....


what is your edition?



%figure
![JPEG_20191119_180735.jpg](https://cdn.discordapp.com/attachments/565154703139405824/646290541536870400/JPEG_20191119_180735.jpg)
%end

##### Fabien Rohrer [Moderator] 11/19/2019 10:08:24
R2019b.rev1. But this mechanism cannot fail in any version.


(otherwise nothing could work)


I think we missed a basic thing 😉

##### HE110 11/19/2019 10:09:19
It is really toooooo hard for me.....


Miss what?

##### Fabien Rohrer [Moderator] 11/19/2019 10:10:08
Let me send you a working project.

##### HE110 11/19/2019 10:11:45
My controller.py ,dont have setPosition in Class Robot


I cant find it in Class Robot


Only find in Class Motor

##### Fabien Rohrer [Moderator] 11/19/2019 10:15:19

> **Attachment**: [8.zip](https://cdn.discordapp.com/attachments/565154703139405824/646292415531057153/8.zip)


Could you try this project?


for sure, setPosition is a member of the Motor class.

##### HE110 11/19/2019 10:18:52

%figure
![JPEG_20191119_181830.jpg](https://cdn.discordapp.com/attachments/565154703139405824/646293305885196288/JPEG_20191119_181830.jpg)
%end

##### Fabien Rohrer [Moderator] 11/19/2019 10:19:18
wow, that's really weird

##### HE110 11/19/2019 10:19:35
Nice ,I want to kill this computer now

##### Fabien Rohrer [Moderator] 11/19/2019 10:20:12
I suspect an issue with Python.


How did you install python?


[https://cyberbotics.com/doc/guide/using-python#windows-installation](https://cyberbotics.com/doc/guide/using-python#windows-installation)

##### HE110 11/19/2019 10:21:38
Im going to reinstall webots and try aaaaaaaagain

##### Fabien Rohrer [Moderator] 11/19/2019 10:21:56
please, reinstall Python too 🙂

##### HE110 11/19/2019 10:22:04
ok


Thank u

##### Fabien Rohrer [Moderator] 11/19/2019 10:22:21
2.7 64bits, from the official website

##### HE110 11/19/2019 10:22:51
2.7?

##### Fabien Rohrer [Moderator] 11/19/2019 10:23:00
or 3.7

##### HE110 11/19/2019 10:23:13
Im using 3.7

##### Fabien Rohrer [Moderator] 11/19/2019 10:24:18
ok, this is fine. but check that it's really the 64 bits version, and that the correct python is launched by Webots (cf. Webots preferences / Python command)

##### HE110 11/19/2019 10:25:31
Ok


Thank u again

##### Fabien Rohrer [Moderator] 11/19/2019 10:27:06
you're welcome, I hope you'll find the issue. Sorry for the bad first impression. What you experiment is not regular.

##### HE110 11/19/2019 10:28:25
I use python regularly


from controller import Robot

from controller import Motor

robot = Robot()

timestep = int(robot.getBasicTimeStep())

TIME\_FOR\_A\_CIRCLE = 10.0   # 10 seconds to do a perfect circle? => to adjust



left = Motor("left wheel motor")

right = Motor("right wheel motor")

left.setPosition(float('inf'))  # Set motor in speed mode

right.setPosition(float('inf'))  # Set motor in speed mode



while robot.step(timestep) != -1:

    

    time = robot.getTime()

    if time < TIME\_FOR\_A\_CIRCLE:

        

        right.setVelocity(6)

        left.setVelocity(3)

    elif time < 2.0 * TIME\_FOR\_A\_CIRCLE:

        

        right.setVelocity(3)

        left.setVelocity(6)

    else:

        

        

        print('quit')

        break


`@Fabien Rohrer`


It's running right this time

##### Fabien Rohrer [Moderator] 11/19/2019 10:43:04
great

##### HE110 11/19/2019 10:43:20
I need import motor

##### Fabien Rohrer [Moderator] 11/19/2019 10:43:38
because you use Motor('')


but this is not how the API should be use:


you should use the Robot.getMotor() function instead


[https://cyberbotics.com/doc/reference/robot?tab-language=python#getmotor](https://cyberbotics.com/doc/reference/robot?tab-language=python#getmotor)

##### HE110 11/19/2019 10:44:27
I still cant use robot.setposition

##### Fabien Rohrer [Moderator] 11/19/2019 10:44:51
robot.setPosition() does not exists.


you have to control the motors.


What do you expect? to teleport the robot?

##### HE110 11/19/2019 10:46:02
But in your code u use robot.setposition

##### Fabien Rohrer [Moderator] 11/19/2019 10:46:21
No, I use motor.setPosition()


left and right variables are Motor instances.

##### HE110 11/19/2019 10:47:26
can your code run right in your computer?


I can't

##### Fabien Rohrer [Moderator] 11/19/2019 10:48:45
yes, I'm quite sure my code is running smoothly

##### HE110 11/19/2019 10:48:56
I just want to practice programming


...................


Thank u in general

##### Fabien Rohrer [Moderator] 11/19/2019 10:53:08
Did you tried our tutorial first? [https://cyberbotics.com/doc/guide/tutorials?tab-language=python](https://cyberbotics.com/doc/guide/tutorials?tab-language=python)

##### HE110 11/19/2019 10:54:47
My English is poor,I copy all code and cant run anyone

##### ClBaze 11/19/2019 11:05:54
Hi guys, I'm making a procedural proto which have to add several solid to the children field of the robot. I'd like to give a unique name to these solid using lua.


Robot {

    children [

      %{ for i = 0, 6 do }%

        ARTag {

          translation 0.25 -0.649755 0.0942168

          rotation -5.338508348004178e-08 -0.7071067811865466 0.7071067811865466 3.14159

          size         0.1 0.1 0.001

          name         %{="top\_tag\_" .. tostring(i)}%

        }

      %{ end }%


but I get an error: Expected string literal, found 'top\_tag\_0'.

##### Fabien Rohrer [Moderator] 11/19/2019 11:06:36
Yes, I see.


The point is to add a \" character at the beginning and at the end of your string, like this:


>>> %{= "\\"toptag" .. tostring(i) .. "\\"" }%


Otherwise the name is not protected correctly, and is interpreted by the wbt parser.

##### ClBaze 11/19/2019 11:09:21
Ok I see


it works thanks !

##### Olivier Michel [Cyberbotics] 11/19/2019 12:39:48
`@Nocturnal Warfare`: yes, it seems there is still a problem with this image... I am going to check it as soon as possible.

##### ClBaze 11/19/2019 12:56:31
Does Webots include any lua module to calculate frame transformations and manipulate quaternions ?


If not, is there any recommendations on how and where to include such library so that it is accessible from lua in proto files ?

##### Fabien Rohrer [Moderator] 11/19/2019 13:25:24
Yes, there is, but a small/limited one 😉


In the procedural PROTO, you have access to these modules: [https://github.com/cyberbotics/webots/tree/revision/resources/lua/modules/webots](https://github.com/cyberbotics/webots/tree/revision/resources/lua/modules/webots)


wbrotation.lua deals with some typical rotations issues you may have with Webots.


Here is a typical usage: [https://github.com/cyberbotics/webots/blob/revision/projects/devices/sick/protos/SickLdMrs.proto#L46](https://github.com/cyberbotics/webots/blob/revision/projects/devices/sick/protos/SickLdMrs.proto#L46)


(2 eulerAndAxis rotations are multiplied)

##### ClBaze 11/19/2019 13:36:42
great ! thanks

##### JoHn 11/19/2019 18:38:16
Hi guys, I'd like to ask a question about the lidar coordinate system. I added a lidar to the vehicle and the default coordinate system is shown in figure below:



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/646419010883420170/unknown.png)
%end


But what I really want is, the lidar has the coordinate system shown below:



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/646419343931867156/unknown.png)
%end


But apparently, the current lidar will not work because the laser beams are shooting to the ground and the sky.


Is there any chance to modify the default coordinate system of the lidar? Thanks a lot!

##### Sohil 11/20/2019 04:25:36
Hi guys! I am working with Webots and ROS. I wanted to know if there is any neat way to publish transforms for suppose between cameras and robot?

##### Stefania Pedrazzi [Cyberbotics] 11/20/2019 07:17:20
`@JoHn` it is not possible to change the coordinate system of the Lidar device itself. The tilt will always be along the X axis of the Lidar device node and rotation along the Y axis (see the documentation [https://www.cyberbotics.com/doc/reference/lidar#field-summary](https://www.cyberbotics.com/doc/reference/lidar#field-summary)). About the graphical shape, you can add a Tranform inbetween to change it and if you need to change the coordinates of the Lidar device values you have to do it manually in the controller program.


`@Sohil` which transform do you want to publish? Currently it is not possible to retrieve directly the transform matrix from a parent node to a descendant node using the Webots API, but you can compute it in your controller program based on the camera and robot global position and orientation ([https://www.cyberbotics.com/doc/reference/supervisor?tab-language=ros#wb\_supervisor\_node\_get\_orientation](https://www.cyberbotics.com/doc/reference/supervisor?tab-language=ros#wb_supervisor_node_get_orientation))

##### ClBaze 11/20/2019 13:14:29
Hi guys! I've made several protos (1 folder per proto) in the main "protos" folder of my webots project. When I try to make another proto using these last ones, Webots console outputs: "error: Skipped unknown node "artag" or Proto". If I move all the protos in the same folder, I can reference them. What is the best way to make a proto that can be used by several protos ?

##### Fabien Rohrer [Moderator] 11/20/2019 13:18:26
Hi clement.


I'm checking, at some point we had a mechanism to add a resource folder.


Ok, we have indeed a mechanism to set supplementary paths in order to search for general protos: it's about to add a directory in the "Webots Preferences / General / Extra Project Path".


You could add there a path to a generic project directory containing your generic PROTOs. They should be defined in a "protos" directory (as usual in fact).

##### ClBaze 11/20/2019 13:59:36
It's working 🙂 . I suppose Webots saves its preferences to a file. Where can I find it ? I don't find it in its installation folder.

##### Fabien Rohrer [Moderator] 11/20/2019 14:01:09
This depends on the OS. On which OS are you working?

##### ClBaze 11/20/2019 14:01:20
ubuntu 16.04

##### Fabien Rohrer [Moderator] 11/20/2019 14:03:00
Then the Webots preferences are stored into INI config files (one per Webots version) into your ~/.config/Cyberbotics directory. Please modify these files with care 😉


This option is stored as a string under the "General/extraProjectsPath" key.

##### ClBaze 11/20/2019 14:05:16
Exact, I can see it. Thank you very much!

##### KamiRonin 11/20/2019 17:31:24
Greetings to all.

Has installed Webots r2019b rev1 on Windows 10 and followed lessons from the documentation.

But in a window 3D I see that textures are not displayed, and to consoles writes "/Webots/projects/default/worlds/textures/gray\_brick\_wall.jpg ' is not a power of two: rescaling it from-1x-1 to 0x0." And "projects/default/worlds/textures/gray\_brick\_wall.jpg ': Unsupported image format.".

But after all it is textures from the delivery and with them it is obvious everything is all right.

After some search has found out that jpg/jpeg a file of textures the program rejects ANY. Accepts normally only png!



To me that to convert all textures installed with the program in png?

##### Stefania Pedrazzi [Cyberbotics] 11/21/2019 07:21:24
`@KamiRonin` no you don't have to convert the textures. JPG textures should work out of the box when installing Webots. If it doesn't then there could be some issues with the Webots installation or with your system PATH. I would first suggest you to download again Webots and reinstall it. If it still doesn't work we should inspect your environment settings.

##### KamiRonin 11/21/2019 07:29:15
`@Stefania Pedrazzi` I reinstalled Webots twice. Once it was installed in folder Users/MyName, at times (having deleted and having cleaned system), I have forced it to be installed in Program files.

After the second installation, there was a first start and textures from jpg files were visible, but right after an output and an input anew in the program - they any more were not shown.

What paths need to be registered in PATH for Webots?

##### David Mansolino [Cyberbotics] 11/21/2019 07:38:57
`@KamiRonin` I think I already saw this issue once, it was du to another libjpeg somewhere in the PATH. Please try cleanning your PATH environment variable before starting Webots.

##### Dorteel 11/21/2019 10:25:08
Hi guys! Is there a way to add nodes with code as a supervisor? I can't seem to find it in the documentation, only wb\_supervisor\_node\_remove.

##### Stefania Pedrazzi [Cyberbotics] 11/21/2019 10:26:41
Hi `@Dorteel` yes there are the import Supervisor API functions:

- [https://www.cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_field\_import\_mf\_node\_from\_string](https://www.cyberbotics.com/doc/reference/supervisor#wb_supervisor_field_import_mf_node_from_string)

##### Dorteel 11/21/2019 10:27:24
Thank you `@Stefania Pedrazzi` !

##### Stefania Pedrazzi [Cyberbotics] 11/21/2019 10:46:30
`@JoanGerard` I tested the streaming server from a docker image and  I could successfully run it using the default Webots image "cyberbotics/webots:R2019b-rev1" (and just adding the "--no-sandbox" options to the Webots command). Maybe this could be an alternative for you as well.

##### Dorteel 11/21/2019 12:08:08
Hi again! I have an issue when trying to set the translation field of a Solid with setSFVec3f, WeBots shuts down. Here is a snippet of the Python code, last line causes the crash:
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/647045582225014784/unknown.png)
%end


There is the same implementation in the python example.wbt with the Robot teleportation, so have no idea what went wrong.


The Solid I used is imported through importMFNode when running the controller, and it is not saved in the world file, could that be the issue?

##### KamiRonin 11/21/2019 12:20:53
`@David Mansolino` I in all variables of environment PATH do not have paths to libraries libjpeg where to look - that it is necessary to register for Webots? Path addition to bin or lib has not helped folders Webots, he all as does not distinguish texture.

##### David Mansolino [Cyberbotics] 11/21/2019 12:50:30
`@Dorteel` , the fact that you did import the Solid before should not cause any issue. Would you agree to share your controller so that we can check what is happenning?


`@KamiRonin`, can you try temporarily just to remove completely all the content of your 'PATH' environment variable and then start Webots

##### KamiRonin 11/21/2019 12:52:19
`@David Mansolino` yes, one moment. I try...


`@David Mansolino` No, changes are not present. The same warnings also are not displayed textures.

##### David Mansolino [Cyberbotics] 11/21/2019 13:00:38
Can you check in the preferences what is the value for the 'Texures Quality'?

[https://cyberbotics.com/doc/guide/preferences#opengl](https://cyberbotics.com/doc/guide/preferences#opengl)

##### KamiRonin 11/21/2019 13:03:47
`@David Mansolino` Tried all customisations and High, and Average, and Low.

png textures of any size are displayed normally and fast, jpg he considers as an unsupported format, neither progressive, nor standard, any size, bitreit and so on


Here so
%figure
![SPOILER_unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/647060391289028609/SPOILER_unknown.png)
%end

##### David Mansolino [Cyberbotics] 11/21/2019 13:11:20
Ok, can you check on your computer if you have any 'libjpeg.dll' (or similar) in your 'Windows' folder (e.g. `C:\Windows\System32\libjpeg.dll`)?

##### KamiRonin 11/21/2019 13:16:48
`@David Mansolino` hm, no, any library has not found. That is it not complete with Webots? ok what to instal? It is enough to throw in Win a folder and all?

##### Dorteel 11/21/2019 13:18:00
Thanks for looking at it `@David Mansolino`
> **Attachment**: [masterv2.py](https://cdn.discordapp.com/attachments/565154703139405824/647063165279928321/masterv2.py)

##### David Mansolino [Cyberbotics] 11/21/2019 13:19:07
I am sorry but I do not fully understand your last message, Webtos does provide everyhing that you need to run it. However, it might happen that other software badly designed already intalled on your computer can 'corrupt' your Windows by adding some libraries (in your case libjpeg) that are not compatible with the ones used by Webots.


`@Dorteel`, thank you I will try it and let you know

##### KamiRonin 11/21/2019 13:23:00
`@David Mansolino` I am sorry, for my English! No, for me any package, framework or the software environment has not installed libjpeg.dll in Windows. Review and processing of these files is carried on through something else. Now there is a question - which library from what framework to take and instal for me that Webots worked through it. GnuWin will approach?

##### David Mansolino [Cyberbotics] 11/21/2019 13:31:18
I am not familiar with GnuWin, but it is probably worth trying it.

##### KamiRonin 11/21/2019 13:34:15
Well, thanks big for the help! I will try different libraries for operation with jpeg, one question, Webots engine important concrete name or what or API functions of such library?

##### David Mansolino [Cyberbotics] 11/21/2019 13:52:15
`@Dorteel`, I had to slightly change your controller to make it run with my test world, but I have no crash. Can you please:

  1. Make sure that you are using the latest version of Webots.

  2. check that when you do both `obj = self.getFromDef('GOAL')` and `dest = obj.getField('translation')` None of the result are equal to `None`.


`@KamiRonin` you can find more info about the Webots API here: [https://cyberbotics.com/doc/guide/programming-fundamentals](https://cyberbotics.com/doc/guide/programming-fundamentals)

##### KamiRonin 11/21/2019 13:56:59
`@David Mansolino` Well, thanks! I have thought that processing of rendering of textures, is function not from API Webots, and internal.

##### David Mansolino [Cyberbotics] 11/21/2019 13:58:39
Ah ok, sorry I misunderstood your question. Webots uses it's own rendering engine called wren: [https://github.com/cyberbotics/webots/tree/revision/include/wren](https://github.com/cyberbotics/webots/tree/revision/include/wren)

And it also uses Qt to load images.

##### Dorteel 11/21/2019 14:17:53
Thanks for taking the time to check it `@David Mansolino` . I have WeBots up-to-date, and none of the objects are None. However I gave the whole world and controller to my friend, and he could also run it perfectly, so have no idea what's wrong

##### David Mansolino [Cyberbotics] 11/21/2019 14:22:38
That's indeed strange, are you both runnin on the same OS ?

##### Dorteel 11/21/2019 14:25:31
yes, Ubuntu 18.04.3


But I checked it again and it works now, I have absolutely no idea why. Thanks for the help though!

##### David Mansolino [Cyberbotics] 11/21/2019 14:41:53
Ok, good news. You're welcome for the help

##### nighthearing 11/21/2019 20:27:17
Hello. I'm about 1 hour into discovering webots. I've been looking for an approach to controlling the Logitech G29 steering wheel. Based on the documentation and input from `@Fabien Rohrer` back in September, it sounds like this is totally possible. I was hoping that I could just write a Python script to do this but my ignorance is getting the best of me. Should I be able to directly import the Joystick API (i.e., `from controller import Joystick API`) and connect to the steering wheel? 

```
from controller import Joystick
stick = Joystick()
stick.enable()
```

Edit: Also, can I bypass using the backend Webots server to use this API?

##### David Mansolino [Cyberbotics] 11/21/2019 22:42:13
HI `@nighthearing`, yes of course this is feasible. Please find an example that use the Joystick API to control a car using a G29 (or other steering wheel model) here (the example is in C++, but the Python API is exactly the same): [https://github.com/cyberbotics/webots/tree/revision/projects/vehicles/controllers/racing\_wheel](https://github.com/cyberbotics/webots/tree/revision/projects/vehicles/controllers/racing_wheel)


I am not sure to understand what you mean by bypassing Webots? If you want to use the Webots API you need Webots to run as it is Webots that is getting the joystick inputs and then transmit them to the controller.


If you want to get directly Joystick inputs from your script without using Webots you might use OIS ([https://github.com/wgois/OIS](https://github.com/wgois/OIS)) directly in your script.

##### lunarpulse 11/22/2019 00:22:20
Is there a safe and convenient way to swap parts of the robot or solid pragmatically? I have 20+ parts to scan with LiDARs and these parts needs to be swapped conditionally, by time elapsed.

##### Dorteel 11/22/2019 14:46:24
Hi! Is there a way to make a robot controller wait for for a supervisor to give 'permission' to take a step in the environment? I'm trying to set up a reinforcement learning environment, so would like the supervisor to make the calculations, and the robot to "stand by" in the meanwhile

##### Fabien Rohrer [Moderator] 11/22/2019 15:50:14
`@Dorteel` wouldn’t be simpler to merge your supervisor with your controller? After all, a supervisor is simply a special controller

##### ZOLTAN OPS 11/22/2019 15:50:42
Is there anyone that could give me a brief explanation of how i would write a basic compass that works on a robot? Using C

##### Fabien Rohrer [Moderator] 11/22/2019 15:52:45
`@lunarpulse` are you aware that you can write PROTO files programnatically? [https://www.cyberbotics.com/doc/reference/procedural-proto-nodes](https://www.cyberbotics.com/doc/reference/procedural-proto-nodes)


This is very convenient to create robots with optional parts, or to reuse subparts.


`@ZOLTAN OPS` you certainly look for this example: [https://github.com/cyberbotics/webots/blob/master/projects/samples/devices/worlds/compass.wbt](https://github.com/cyberbotics/webots/blob/master/projects/samples/devices/worlds/compass.wbt)


[https://github.com/cyberbotics/webots/blob/master/projects/samples/devices/controllers/compass/compass.c](https://github.com/cyberbotics/webots/blob/master/projects/samples/devices/controllers/compass/compass.c)

##### ZOLTAN OPS 11/22/2019 18:23:37
For the compass, is there an object i need to make a child of the robot to put a compass on the robot?

Also the first one you posted, is that all code for one compass or is there other stuff in there with it like motors. `@Fabien Rohrer`

##### Fabien Rohrer [Moderator] 11/22/2019 19:01:13
You can put the Compass directly into the Robot.children.


Yes, there are other stuff. If you open this world in Webots, it will be self explanatory 😉


The robot is a blue 2 wheeled robot equipped with a motorized yellow arrow. The controller makes in order that the yellow arrow point always the north, using the compass feedback.

##### SimonDK 11/22/2019 23:59:03
I have a simulation connected with ROS and have access to the services. Is there a way to reset and pause the simulation from ROS?

##### David Mansolino [Cyberbotics] 11/23/2019 12:23:31
Hi `@SimonDK`, yes, you first have to make sure that the 'supervisor' field of your robot is set to 'TRUE' then you can use the supervisor services: 

[https://cyberbotics.com/doc/reference/supervisor?tab-language=ros#wb\_supervisor\_simulation\_set\_mode](https://cyberbotics.com/doc/reference/supervisor?tab-language=ros#wb_supervisor_simulation_set_mode)

[https://cyberbotics.com/doc/reference/supervisor?tab-language=ros#wb\_supervisor\_world\_load](https://cyberbotics.com/doc/reference/supervisor?tab-language=ros#wb_supervisor_world_load)

##### SimonDK 11/23/2019 15:12:59
Hi `@David Mansolino` , thanks for that. Have been trying it out, but every time I use any of those service calls from ROS, Webots crashes/freezes 🤔Any idea why? I am just running the Pioneer ROS example with Webots running on a remote machine. Everything works fine, can do mapping and control the robot.



Edit: figured it may be related to an issue on github, where the environment may freeze on Mac.

##### Prasad 11/24/2019 05:49:29
I am figuring out a workaround for the bug [https://github.com/cyberbotics/webots/issues/1081](https://github.com/cyberbotics/webots/issues/1081)

Question: Can I get the floor size in the controller?


I am using Python. Floor("floor") does not work. I was thinking about programmatically extracting the attributes of the floor in the controller


I have one more question. What does compass do? I believe it gives the orientation of the robot. But, where is the magnetic north? Is it the north of the webots floor or some north in infinity? Am I better off using gyro or should I use compass to get the orientation of the Pioneer rover?

##### ZOLTAN OPS 11/24/2019 13:06:44
`@Fabien Rohrer`  how would I open a world with compass.c?


im new to webots

##### Moumen 11/24/2019 16:24:20
How does webots work exactly


I'm kind of new to this stuff


I wanna just put some code and see if a robot moves


It probably doesn't work that way

##### SimonDK 11/24/2019 22:35:17
In the Supervisor controller, there are options for reset, pause, set real time, run, and fast. Isn't there an option to execute one simulation step like there is from the menu inside Webots?

##### Prasad 11/25/2019 02:49:54
Also, I am facing one terrible problem which is significantly consuming my development time (which I believe is unnecessary). The Y coordinates in Webots is flipped. There should be one option for the user to select the direction of Y coordinates (with reference to the floor). Should I raise an enhancement issue regarding the same?


Also, sorry to say this. Camera does not work properly. Rover gets stuck repeatedly. Coordinates are flipped. I do not think I can build a production ready SITL with Webots. I am quite frankly disappointed with flipped coordinates. I am thinking about archiving my repository citing the same reason by tomorrow. I would appreciate any insight/input.

##### Stefania Pedrazzi [Cyberbotics] 11/25/2019 06:49:21
`@Prasad` yes it is possible to retrieve the Floor size. You have to use the Supervisor API ([https://cyberbotics.com/doc/reference/supervisor?tab-language=python#wb\_supervisor\_node\_get\_def](https://cyberbotics.com/doc/reference/supervisor?tab-language=python#wb_supervisor_node_get_def)) to get the node and the  `Node.getField` (wb\_supervisor\_node\_get\_field) and `Field.getSFVec2` to retrieve the value of the "size" field. For an example you could for example look at the way the slave robots translation fields is read in the `projects/languages/python/worlds/example.wbt` sample simulation.


As documented in the `Compass` node documentation page ([https://cyberbotics.com/doc/reference/compass?tab-language=python](https://cyberbotics.com/doc/reference/compass?tab-language=python)) the Webots north direction is the one set in the `WorldInfo.northDirection` field this is fully customizable and you are free to change it and adjust it to your needs. It "some north in infinity" and has nothing to do with the floor or any object in the scene. The Compass node works just fine in retrieving the orientation of a robot and that's what does: it returns the orientation of the device with respect to the `WorldInfo.northDirection`.


`@SimonDK` to execute just one step you can set the mode to REAL\_TIME, run the  `wb_robot_step(time)` function where `time` corresponds to the worlds basic time step (to execute one step) and then set the mode to PAUSE.


`@Moumen` to start with Webots I suggest you to read the Tutorials ([https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials)). Here it is also explained how to program a robot. If after checking them it is still not clear how to run some code, please don't hesitate to ask again.


`@Prasad` in Webots it is possible to change the direction of the up vector and the direction of the north using the `WorldInfo.gravity` and `WorldInfo.northDirection` this should be sufficient to customize the global coordinates of the world. Please check the documentation `[https://cyberbotics.com/doc/reference/worldinfo](https://cyberbotics.com/doc/reference/worldinfo)` and try to play with these parameters. If it still you cannot achieve what you want, then you could open an issue and we will evaluate it.


`@Prasad` I'm sorry that you have some issues using Webots and we are continuously trying to help you solving them. I'm also sure that there are workarounds for the coordinates issue and any other issue you will find. Please tell me if I'm wrong, but I have the feeling that you are trying to port one by one your Gazebo code in Webots and you are expecting that Webots works exactly the same way as Gazebo. But this is not the case, they are two different software that have been programmed independently following different standards. I think that to get the best from Webots (also in terms of performance) it would be better to try to understand how Webots works and adapt the code.

##### Dorteel 11/25/2019 08:28:25
Hi! I'm trying to make parts of my WeBots simulation as a Gym environment for RL (python) ([https://stable-baselines.readthedocs.io/en/master/guide/custom\_env.html](https://stable-baselines.readthedocs.io/en/master/guide/custom_env.html) ). However, having troubles with implementing it with the supervisor class. I was trying double inheritance, but didn't succeeed so far, as both Robot and Gym requires a step() function. Might be that there is just an easy programming workaround I don't know about? Any suggestions?

##### Stefania Pedrazzi [Cyberbotics] 11/25/2019 08:32:13
Hi `@Dorteel`, if I understand correctly your issue is simply than both base classes have the same method name , is it correct?


This is a common programming issue that could be solved by specifying the class in the call: [https://stackoverflow.com/questions/3810410/python-multiple-inheritance-from-different-paths-with-same-method-name](https://stackoverflow.com/questions/3810410/python-multiple-inheritance-from-different-paths-with-same-method-name)

##### Dorteel 11/25/2019 08:34:19
Yes, the controller needs to inherit methods with the same name from both parents, and I'm not sure how it works when these methods are used by other packages.

##### Stefania Pedrazzi [Cyberbotics] 11/25/2019 08:37:05
Calling `(super, Robot).step()` vs `(super, CustomEnv).step()` you should be able to control which function is called.

##### Dorteel 11/25/2019 08:37:33
Thank you very much `@Stefania Pedrazzi` I'll try that!

##### Gautier 11/25/2019 10:08:03
Hello, I have a problems with a proto I exported from Blender

##### Fabien Rohrer [Moderator] 11/25/2019 10:08:53
`@Gautier` Hi. Could you add precisions? Do you use the blenderToWebots addon?

##### Gautier 11/25/2019 10:11:04
`@Fabien Rohrer` Hi, thank you for your response. Sorry I was uploading the video on youtube because I wasn't able to put it on the discord


[https://www.youtube.com/watch?v=\_azoSKFOYu0&feature=youtu.be](https://www.youtube.com/watch?v=_azoSKFOYu0&feature=youtu.be)


I used the blender addon for blender 2.79 to export it and transform it in proto


these "bugs" of display only appear when I use textures. Without any textures, webots render the model prefectly fine

##### Fabien Rohrer [Moderator] 11/25/2019 10:12:45
This is because your object is rendered as a transparent object.


You can simply solving this by removing the alpha layer from your textures. This is a condition to switch the objects into the transparent render queue.


To do so, simply open your image in Gimp, and select the "Layer / Transparency / Remove alpha chanel", and override your previous texture.


voila

##### Gautier 11/25/2019 10:15:11
Ok, I'm gonna try that ! Thank you !

##### Fabien Rohrer [Moderator] 11/25/2019 10:15:14
(do this for all your textures)

##### Gautier 11/25/2019 10:20:16
It worked, thank you a lot !


If I need to have a transparent texture on an object though, how would I have to do to not encounter this kind of problem ?

##### Fabien Rohrer [Moderator] 11/25/2019 10:24:09
transparent objects is a global issue in computer graphics, without perfect solutions.


Our solution is about to sort transparent objects according to their center, but not to sort polygons, because it has a big cost in term of performance.


This is good enough to render simple cases (rectangular glasses, etc.) but not to render well complex meshes.


we currently do not provide better solution. But it would be possible to change our rendering engine to optionnaly sort polygons (but we should see a big interest in this topic, which is currently not the case).

##### Gautier 11/25/2019 10:33:38
Ok, I totally understand your point of view


Thank you a lot ! 🙂


I have another question on a different topic, if I might ? 😅

##### Fabien Rohrer [Moderator] 11/25/2019 10:53:13
😄 Please ask!

##### Gautier 11/25/2019 10:53:29
Thank you !


So, I have this proto node that generate some other other solid in its "children" thanks to some Lua. The  behavior of the proto in itself is what we excepted, the only thing is that we want to separate the "physics" of the different solid (exemple : At the end of the video, I apply a force on some of the mirror, and I would like that these mirrors "undocks" and just go the other way of the rest of the structure)
> **Attachment**: [HowTo-2019-11-25\_11.49.28.mp4](https://cdn.discordapp.com/attachments/565154703139405824/648477047772938260/HowTo-2019-11-25_11.49.28.mp4)


I would like my "robot" to not be considered as a "big object" by my physics engine but "multiple object" docked in a central one


Could I modelize the "docking" of these differents object on Webots (and the necessary force to seperate themselves from the central dock) ? If not, how could I do in order that, even if these objects are generated by a lua proto, they're considered separated and with differents physics ?

##### Fabien Rohrer [Moderator] 11/25/2019 11:02:01
(I definitvely have to add a new background of space for such 0 gravity areas 🙂 )

##### Gautier 11/25/2019 11:02:58
It's the fact that it's treated as a "big object", instead of little small objects, in prescence of force that I would like to correct
> **Attachment**: [HowTo2-2019-11-25\_12.00.34.mp4](https://cdn.discordapp.com/attachments/565154703139405824/648478732285116426/HowTo2-2019-11-25_12.00.34.mp4)


That would be pretty nice  !

##### Fabien Rohrer [Moderator] 11/25/2019 11:05:14
You unfortunately reached a limitation of our PROTO system which can handle only one root node.


Because of this, and the fact that we do other assumptions on the top nodes, it seems unfortunately to be not possible to achieve this this way.


(our PROTO system should deal with multiple top nodes to be able to manage this case)


A better approach would be rather to create a PROTO per tile, and to assembly them using another mechanism, typically a Supervisor can import PROTO in a scene.

##### Stefania Pedrazzi [Cyberbotics] 11/25/2019 11:11:01
What about using a Group as a top node? You will be able to add multiple independent Solid children nodes from the procedural PROTO

##### Fabien Rohrer [Moderator] 11/25/2019 11:11:56
You're right, but I'm not sure we support this. Is it possible to add Robot inside a Group?


I'm testing this right now.

##### Stefania Pedrazzi [Cyberbotics] 11/25/2019 11:12:45
probably not,  just the Solid nodes

##### Gautier 11/25/2019 11:13:56
Both are very good idea ! I'm gonna test them out !


Thanks you a lot


Also, wouldn't a "slot" works ?


Field*

##### Dorteel 11/25/2019 11:18:51
Hi again guys, I run into a problem when trying to import some libraries from stable\_baselines, using the same environment I have no issues in PyCharm, so I'm not sure what the problem is
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/648482732967723009/unknown.png)
%end


Do you have any suggestions?

##### Fabien Rohrer [Moderator] 11/25/2019 11:20:47
`@Gautier` `@Stefania Pedrazzi` I confirm that a Group does not solve the issue, because currently a Robot cannot be inserted in it (but a Solid can).


`@Gautier` I would say that a slot is more a matter on how two protos are connected together.


If you go in the way I suggest about to use a Supervisor to assembly the tiles, slots could be use to facilitate their assembly (cf. the tinkerbots examples)


[https://cyberbotics.com/doc/guide/tinkerbots](https://cyberbotics.com/doc/guide/tinkerbots)

##### Gautier 11/25/2019 11:22:57
Ok, I'm gonna try that !


Thank you a lot ! 🙂

##### Fabien Rohrer [Moderator] 11/25/2019 11:24:02
`@Dorteel` Sorry, I don't know about this error. It's likely to be an issue when importing a third party library. Could I kindly redirect you to the support of pycharm/python ?

##### Dorteel 11/25/2019 12:07:39
Hi `@Fabien Rohrer` , the problem is, that it's only in WeBots that it doesn't work, it works in any other IDE (including PyCharm), that's why I referred to you guys. I use the Shebang line to define the environment if that's any help. But I'll try to find another solution, just thought you guys might know something about it.

##### Fabien Rohrer [Moderator] 11/25/2019 12:59:00
`@Dorteel` I can only tell that your python environment miss to load an internal resource: libmpi.so.20. The first thing to try is to locate this resource on your computer, and try to add in it LD\_LIBRARY\_PATH.

##### nighthearing 11/25/2019 13:38:30
`@David Mansolino` Thanks for your response a few days ago! If I have more questions, I'll follow up!

##### David Mansolino [Cyberbotics] 11/25/2019 13:39:06
You're welcome.

##### SimonDK 11/25/2019 21:08:03
`@Stefania Pedrazzi` Thanks, that's exactly it. I also needed the controller arg --synchronize to make it work properly with ROS 🥳

##### Dorteel 11/25/2019 21:38:55
Thank you `@Fabien Rohrer` !

##### JoanGerard 11/26/2019 12:05:39
hello, I am trying to run a simulation with the custom robot of example distance\_sensor.wbt. After X steps I should reset the initial position of my robot to be 0,0,0. I'm using a Supervisor class just like it is shown in the soccer.wbt example but with python. This is my code: 



robot\_sup = robot.getFromDef("e-puck")

    robot\_trans = robot\_sup.getField("translation")

    robot\_trans.setSFVec3f([0, 0, 0])


but after 3 minutes of simulation the robot seems to disarm and the wheels start to walk away the robot body... really weird



%figure
![Screen_Shot_2019-11-26_at_1.07.21_PM.png](https://cdn.discordapp.com/attachments/565154703139405824/648857400941477888/Screen_Shot_2019-11-26_at_1.07.21_PM.png)
%end


it works for the first few translations but it seems that there is a cumulative error somewhere that gets worst in time ...


does anybody know what could be happening here?



> **Attachment**: [ex-low.mov](https://cdn.discordapp.com/attachments/565154703139405824/648861775395160074/ex-low.mov)

##### Fabien Rohrer [Moderator] 11/26/2019 12:27:36
We are aware of this severe issue, and we will do our best to solve it quickly: [https://github.com/cyberbotics/webots/issues/1118](https://github.com/cyberbotics/webots/issues/1118) You're welcome to share your expertise on this issue report. You should follow this issue to be informed on the fix progress.


Meanwhile, I can only advise you to use the heavy wb\_supervisor\_simulation\_reload() function instead.

##### JoanGerard 11/26/2019 12:29:37
ok, thank you and good luck solving the issue!

##### Gautier 11/27/2019 09:25:31
Hello !

##### David Mansolino [Cyberbotics] 11/27/2019 09:26:04
Hi

##### Gautier 11/27/2019 09:26:37
So, I have a little problem I can't really make sense off


I would be very gratefull if you helped me understand the "unexpected behavior"

##### David Mansolino [Cyberbotics] 11/27/2019 09:27:16
Yes of course, what is this behavior?

##### Gautier 11/27/2019 09:28:28
I have a world in Webots that consists in a rail (a solid that can move in one axis compared to another solid in reference) and a connector is fixed to the end of the rail


and I put a robot (sample robot, abb I think) on the "end connector"


The unexpected behavior is that there seems to be "no constraint" on the Z axis : when a force is applied to the robotic arm, it just "kind off" seperate itself from the rest of the solid


But it's still the end point of the slider joint (when I move the slider joint, the "robotic arm" floating stop to float and moves accordingly to the joint)


Here's a video to show tou


you*


[https://www.youtube.com/watch?v=pHSAeZXq5wM&feature=youtu.be](https://www.youtube.com/watch?v=pHSAeZXq5wM&feature=youtu.be)


Here is the associated world, too
> **Attachment**: [bug\_physics.wbt](https://cdn.discordapp.com/attachments/565154703139405824/649180356439703562/bug_physics.wbt)

##### David Mansolino [Cyberbotics] 11/27/2019 09:31:28
Ok thank you, I will check your world and let you know

##### Gautier 11/27/2019 09:32:03
(for the video, sorry I recorded a little too much, skip to 0:50 for unepexcted behavior)


Ok, thank you a lot ! 🙂

##### David Mansolino [Cyberbotics] 11/27/2019 09:32:26
You're welcome


It seems the hierarchy of your nodes is somehow wrong. You have a root node that has Physics without bounding objcet (this doesn't make sense) and then another Solid in the children at the same level than the joint that have bounding object (same size than root Solid) but no Physics


Then in the children of your joint you have some useless transform and the endpoint Solid of the first joint doesn't have bounding objects and physics


You should try to:

  1. remove useless transform (it is pointless to put a transform as parent of a solid)


2. make sure all your Solids nodes (maybe except the root one) have both bounding object and physics set

##### Gautier 11/27/2019 09:54:26
Ok, that was a problem of physic node lacking 😄


Thank you so much !


But, by the way, Is it really necessary for the root node (robot) to have a bouding object ?

##### David Mansolino [Cyberbotics] 11/27/2019 09:55:09
You're welcome

##### Gautier 11/27/2019 09:55:17
It inherit the bouding object from its children, doesn't it ?


inherits*

##### David Mansolino [Cyberbotics] 11/27/2019 09:55:46
Yes it does if you have direct chidrens (not linked with a joint)

##### Gautier 11/27/2019 09:55:57
understood !


Thank you !


Hi again, I'm sorry to annoy you, I have another question 😅


Is it possible to import a proto node instead of a world using a supervisor ?

##### David Mansolino [Cyberbotics] 11/27/2019 15:20:27
Yes


You can import it using these functions: [https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_field\_import\_mf\_node](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_field_import_mf_node)

##### Gautier 11/27/2019 15:23:56
Thank you

##### David Mansolino [Cyberbotics] 11/27/2019 15:24:06
You're welcome

##### Gautier 11/27/2019 15:24:13
I tried again, but I can import a ".wbo" file but not a ".proto"


I don't really know why


(the .wbo file is just the proto exported)

##### David Mansolino [Cyberbotics] 11/27/2019 15:25:00
You can use the 'wb\_supervisor\_field\_import\_mf\_node\_from\_string' function and import a string like this one:

"MyProtoName { translation 0 0.5 0 ...}"

##### Gautier 11/27/2019 15:26:07
Ohhh, I see


Thank you !

##### David Mansolino [Cyberbotics] 11/27/2019 15:26:22
You're welcome

##### Gautier 11/27/2019 16:06:44
I'm sorry to bother you again, but it doesn't seems to work 😅


I'm trying to import a simple proto, like this
> **Attachment**: [Solid.proto](https://cdn.discordapp.com/attachments/565154703139405824/649280061177987111/Solid.proto)

##### Fabien Rohrer [Moderator] 11/27/2019 16:08:07
You should call the file "MyProtoName.proto", and put it in "yourWebotsProject/protos/MyProtoName.proto"


Naming it Solid.proto is a bad idea 😉

##### Gautier 11/27/2019 16:09:39
Ok !


That's working !


Thank you 😄

##### askslk 11/27/2019 22:54:59
Hi everyone.What can I use to code robot back to central point?GPS?

##### David Mansolino [Cyberbotics] 11/28/2019 07:31:29
Hi `@askslk`, yes the GPS is the most simpler way to do this.

##### askslk 11/28/2019 13:00:43
`@David Mansolino` which function I can do about it?


I am trying supervisor(emmitter and receiver)now

##### David Mansolino [Cyberbotics] 11/28/2019 13:03:06
`@askslk`, please find here the GPS documentation: [https://cyberbotics.com/doc/reference/gps](https://cyberbotics.com/doc/reference/gps)

And a simple example here:  [https://cyberbotics.com/doc/guide/samples-devices#gps-wbt](https://cyberbotics.com/doc/guide/samples-devices#gps-wbt)

##### askslk 11/28/2019 14:07:53
I've seen this a few times, but I still don't know how to set the robot return to center point after getting the coordinates.

##### David Mansolino [Cyberbotics] 11/28/2019 14:54:08
This is a general control algorithm problem not related to Webots

##### UncleDucky 11/28/2019 15:18:48
Hey guys - Having some problems making my little robot do a circle. Im just a noob lol.

##### David Mansolino [Cyberbotics] 11/28/2019 15:56:52
`@UncleDucky` assuming you are using a differential wheel, simply setting the left speed lower than the right one (or vice-versa) should do the job 😉

##### Dorteel 11/28/2019 16:17:56
Hi everyone, for some reason I'm having troubles recording a video while running the controller, do you have any suggestions?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/649645162611343360/unknown.png)
%end


The frame also freezes, can't see anything moving while the recording is ongoing

##### Fabien Rohrer [Moderator] 11/28/2019 16:19:13
This warning occurs normally when no simulation steps occured between the beggining and the end of the movie recording.


A regular workflow is about to select "File / Make Movie...", click OK, run the simulation, select "File / Stop Movie", and finally choose the target file.


Some information should be displayed in the Webots console.


Once the target file is chosen, the movie creation takes a while (see the log in the console)


Please wait on the result before doing something else..

##### Dorteel 11/28/2019 16:22:54
Thank you, I'll try to do that!

##### Fabien Rohrer [Moderator] 11/28/2019 16:23:08
Please come back if this does not work 😉

##### Dorteel 11/28/2019 16:43:03
Same issue unfortunately, tried multiple times :/ I managed to create one video though, but can't play it and it and the robot was frozen during that recording as well, but when I clicked (anywhere), the robot jumped to the location where it should be.


But I'll ask my groupmate who also works on the same project, we'll see if he has issues with recording.

##### Fabien Rohrer [Moderator] 11/28/2019 16:50:34
Do you have any warning in the Webots console?


Did you installed `ffmpeg` as described here? [https://cyberbotics.com/doc/guide/installation-procedure#installation-on-linux](https://cyberbotics.com/doc/guide/installation-procedure#installation-on-linux)


> sudo apt install ffmpeg libfdk-aac1 ubuntu-restricted-extras

##### Matt60 11/28/2019 17:41:34
Hello, I'm having trouble with the example get\_bearing\_in\_degrees code from the compass page. After this code I want to print the value of bearing into the console, however this only returns an error "bearing" undeclared. I've been having no luck with this for a few hours now, being a noob with code does not help. Is there something fundamental that I'm missing? Thank you for your help in advance.
%figure
![Code.PNG](https://cdn.discordapp.com/attachments/565154703139405824/649666207699566632/Code.PNG)
%end



%figure
![Error.PNG](https://cdn.discordapp.com/attachments/565154703139405824/649666214859112448/Error.PNG)
%end

##### Fabien Rohrer [Moderator] 11/28/2019 17:44:41
the scope of the "bearing" variable is only in the get\_bearing\_in\_degrees() function. It seems your variable is badly declared.


do you do:


> double bearing = get\_bearing\_in\_degrees();


just before the printf() call?

##### Matt60 11/28/2019 17:45:39
Not at the moment, I will try that


That worked, thank you so much! I didn't know that's what you had to do

##### Fabien Rohrer [Moderator] 11/28/2019 18:02:38
Thank you for the feedback. You should document about variable scoping in C 😉

##### askslk 11/28/2019 21:00:04
hi everyone,how differential robot mapping the world on webots？

##### lunarpulse 11/29/2019 08:00:05
Hi everyone, I have simulation environment running on webots and ROS. I have difficulty in recording simulation environment with simulated time (2 minutes). My machine is slow and many ROS nodes slows down the system and simulation to 0.05x and ROSbag records wall time (1 hour) instead of the simulation time 2 minute. Could you help me to record a ros bag in simulated time instead of wall time?

##### David Mansolino [Cyberbotics] 11/29/2019 08:00:35
Hi lunarpulse


First, you have to make sure that you did put in the controllerArgs field of your robot the following arguments: '--clock' and '--use-sim-time' (see v for more information)


Then you have to set the 'use\_sim\_time' ROS parameter ([http://wiki.ros.org/Parameter%20Server](http://wiki.ros.org/Parameter%20Server)) to true.


Then both your nodes and bag should use simulation time (or at least record it too).


Hi `@askslk`, I am not sure to understand your question, but if you want to perform SLAM with a differential wheel robot in Webots you might be interested by [https://en.wikibooks.org/wiki/Cyberbotics%27\_Robot\_Curriculum/Advanced\_Programming\_Exercises#SLAM\_](https://en.wikibooks.org/wiki/Cyberbotics%27_Robot_Curriculum/Advanced_Programming_Exercises#SLAM_)[Advanced]

##### Kormit 11/29/2019 13:30:22
Hi everyone, is there a possible way to create or use existing software to run a webots simulation without the developer environment webots initially provides, effectively to just run the 3d view? Thanks.

##### David Mansolino [Cyberbotics] 11/29/2019 13:35:58
Hi  `@Kormit`, yes if you download the webots package: [https://www.cyberbotics.com/download](https://www.cyberbotics.com/download)

You can run all the samples provided withing Webots without coding enything.

If you want to control the motion of your robot/object in Webots from another software, you can create a controller that will act as a bridge between Webots and your other software.

Is that what you would like to do?

##### Kormit 11/29/2019 13:41:42
`@David Mansolino` Thanks for the response, perhaps I can clarify what I'm looking for. I'm currently developing a world with supervisors to detect specific robot actions to gain points. Is there a way that the simulation can be run so that people can develop their own controllers for the robots, but not have the ability to view/edit the world (the node tree etc.) so the simulation is effectively running only the 3d viewing portion rendering the world.

##### David Mansolino [Cyberbotics] 11/29/2019 13:47:53
Yes, there are several way to do this:


1. You can set the controller to be 'extern' then the user can write his controller and launch it independently of Webots: [https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers](https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers)


2. you can run Webots as a web server and user can only program the controller, such as what we did here: [https://robotbenchmark.net/](https://robotbenchmark.net/)

##### UncleDucky 11/29/2019 13:55:11
Hey guys, I'm using C to control a differential robot in Webots. I want to be able to control the path of the robot using GPS. How would I begin to do this?

##### David Mansolino [Cyberbotics] 11/29/2019 13:56:35
You can find some path plannign example in Webots here: [https://en.wikibooks.org/wiki/Cyberbotics%27\_Robot\_Curriculum/Advanced\_Programming\_Exercises#Path\_planning\_](https://en.wikibooks.org/wiki/Cyberbotics%27_Robot_Curriculum/Advanced_Programming_Exercises#Path_planning_)[Advanced]

##### UncleDucky 11/29/2019 14:05:31
Thans


Thanks

##### David Mansolino [Cyberbotics] 11/29/2019 14:21:07
You're welcome

##### askslk 11/29/2019 22:10:58
hi everyone,how differential robot go to the target point use controller ？


hi, why after add a  camera to my robot, the camera cant shot, just a plot of black

## December

##### chenpixx 12/01/2019 20:52:45
Hi,  I want to plot a point on the right foot sole of robotis-op2. How can I do it?

##### YCL 12/01/2019 23:09:14
Hi everyone. Recently, I want to compare the speed of a robot in simulation with the same robot in real life. In order to let the robot move forward, in simulation, when I use differnt basic time step  with all other parameters stay the same, the time to finish this same motion is differnt, and the speed by calculating the position of the robot over simulation time  is different too. And I see the user manual about basic time  step set-up. Does that mean I need tune the basic time step according to the real robot's speed, with the same parameters and controller betweem the simulation and real robot?


And when I set a robot arm to move a constant degree(eg. from 10deg to 30deg) by a given simulation time period, what is the kind of motion of the simulation every basic time step? Eg. sometime the simulation can not move the constant degree by a given time. By changing the given time, the simulation could done well. So I am wondering what's the motion style of the simulation, like how many degree the robot move every single basic time step or the robot just move to the goal？And how can I find the exact time need to finish moving a constant degree? All are based on the same robot model with same max torque for the rotation motors, same mass of the robot.


Thank you!

##### lunarpulse 12/01/2019 23:36:16
`@David Mansolino` Thanks very much! I could not find this information online. fortunately, you provided in detail. Now I get all in sim time. Thanks again.

##### Stefania Pedrazzi [Cyberbotics] 12/02/2019 07:24:07
> hi, why after add a camera to my robot, the camera cant shot, just a plot of black

`@askslk` did you enable the camera in your robot controller?


`@askslk` there are multiple techniques and algorithm to move a robot to a target point depending on the constraints and devices mounted on the robot (i.e. for example if the robot knows its current position, etc.). You should add more details about your scenario.

##### David Mansolino [Cyberbotics] 12/02/2019 07:40:44
Hi  `@chenpixx`, you can use the supervisor API to import/move any node in the scene:

  - [https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_field\_import\_mf\_node](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_field_import_mf_node)

  - [https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_field\_set\_sf\_vec3f](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_field_set_sf_vec3f)

This way you can for example simply import a sphere shape and move it to the right foot sole position.

##### Olivier Michel [Cyberbotics] 12/02/2019 07:46:49
`@YC`L#9474: you should run your simulation in real time mode (using the PLAY button). If the speedometer displays a value significantly lower than 1, that means your simulation is unable to run in real time (at the same speed as the real system). So, only in this case, you should adapt your simulation until it runs in real time. Reducing the `WorldInfo.basicTimeStep` is certainly going to speed-up your simulation, but there are other possibilities such as simplifying the simulation models or the robot controllers.

##### Dorteel 12/02/2019 08:20:21
Hi guys! For some reason I can't access the supervisor documentation, were there any changes I'm not aware of?


([https://www.cyberbotics.com/doc/reference/supervisor](https://www.cyberbotics.com/doc/reference/supervisor) )


I actually get the same if I'm just trying to access the website
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/650981682127699979/unknown.png)
%end

##### David Mansolino [Cyberbotics] 12/02/2019 08:59:49
`@Dorteel` we are currenlty running an update, please try again later.

##### Dorteel 12/02/2019 09:03:17
Thanks `@David Mansolino` !

##### David Mansolino [Cyberbotics] 12/02/2019 09:50:49
You're welcome

##### kwy 12/02/2019 11:11:44
Hallo，I just builded a simulation environment for mobil robot with python. Now I want to build a 3D simulation environment. Is it possible to implement this on Webots?

##### David Mansolino [Cyberbotics] 12/02/2019 11:12:10
Yes, Webots is exactly the tool you need 🙂

##### kwy 12/02/2019 11:12:36

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/651017875359924234/unknown.png)
%end

##### David Mansolino [Cyberbotics] 12/02/2019 11:13:57
Yes this kind of setup is feasible, see for example some already available environments: [https://github.com/cyberbotics/webots/blob/develop/docs/guide/samples-environments.md](https://github.com/cyberbotics/webots/blob/develop/docs/guide/samples-environments.md)

##### kwy 12/02/2019 11:16:19
thanks ! where can i find some cool robot model, like a omni robot

##### David Mansolino [Cyberbotics] 12/02/2019 11:16:52
You're welcome.


Here is the list of robots: [https://github.com/cyberbotics/webots/blob/develop/docs/guide/robots.md](https://github.com/cyberbotics/webots/blob/develop/docs/guide/robots.md)


But you can of course build your own one


The youbot is an omni one: [https://github.com/cyberbotics/webots/blob/develop/docs/guide/youbot.md](https://github.com/cyberbotics/webots/blob/develop/docs/guide/youbot.md)


And we are currently working on the creation of a model of the robotino from Festo which is an omni robot too: [https://www.festo-didactic.com/int-en/services/robotino/?fbid=aW50LmVuLjU1Ny4xNy4zNC4xMTY4](https://www.festo-didactic.com/int-en/services/robotino/?fbid=aW50LmVuLjU1Ny4xNy4zNC4xMTY4)

##### kwy 12/02/2019 11:28:50
👌  thanks


still can't access the webots documentation ?

##### Fabien Rohrer [Moderator] 12/02/2019 13:37:05
We have currently issues with our server running cyberbotics.com. We are trying to fix this as soon as possible. Meanwhile, you could run the Webots offline documentation (identical to the one on cyberbotics.com), just click on the "Help / Offline documentation" menu item...

##### JasonChen 12/03/2019 05:04:45
Hello, I need help for "how to log information?" Right now, I have a variable need to bog to the console to watch the value, but the printf() seems doesn't point to the Webots console. So, where can I see the log. Thank you.

##### Stefania Pedrazzi [Cyberbotics] 12/03/2019 07:24:58
`@JasonChen` printf should work fine and write the text in the Webots console. Did you add a new line "\n" character at the end of your printf message to flush the buffer and print the text in console?

##### JasonChen 12/03/2019 07:40:07
`@Stefania Pedrazzi` Yes. Got that. I searched the discussion history, and find that answer. Thank you. 😀

##### Meisam 12/03/2019 09:51:05
Hello, I have just upgraded to Webots R2019b revision1. Unfortunately, it is too slow. I used the previous version Webot8 without any significant problem in performance. I checked the documentation about graphic drivers:

[https://www.cyberbotics.com/doc/guide/verifying-your-graphics-driver-installation](https://www.cyberbotics.com/doc/guide/verifying-your-graphics-driver-installation)

Is there any suggestion to fix the problem?

The specifications of my System are:

OS: Ubuntu 18.04,

Cpu:  Intel® Core™2 Duo Processor T9550,

Ram: @GB,

Graphics card: NVIDIA GeForce 9600M GT

Thanks

##### Fabien Rohrer [Moderator] 12/03/2019 10:27:29
`@Meisam` Hi,

##### ega 12/03/2019 10:27:52
How to use opencv in webots?

##### Fabien Rohrer [Moderator] 12/03/2019 10:32:07
`@Meisam` Between these 2 releases, we increased the GPU load, mainly because of our migration to PBR, the use of big floating point textures for backgrounds, and postprocessing effects (bloom, global ambient occlusion, etc.). The Webots preferences contain an "OpenGL" tab where you could reduce parameters. You should try to reduce the texture quality, and the ambient occlusion quality. The next release of Webots will use less GPU memory, you could already try the latest nightly builds (develop) to give a try.


`@ega` Please take a look at this example: [https://cyberbotics.com/doc/guide/samples-howto#vision-wbt](https://cyberbotics.com/doc/guide/samples-howto#vision-wbt) OpenCV is currently released in Webots, you can use it out-of-the-box

##### Steve222 12/03/2019 11:58:31
Hello, I am trying to use a linear slider joint with a linear motor and I am trying to drive it to a position  with  the following:wb\_motor\_set\_position(sliderR, 0.2);


and this isnt working. any ideas of what could be wrong?

##### Fabien Rohrer [Moderator] 12/03/2019 12:17:30
This seems to be correct. Do you have any error message in the Webots console?

##### Steve222 12/03/2019 12:20:17
No errors in the console, I have used Wb\_get\_device  the same way i have for other moments which are all working. Are there any settings i need to have in the tree that could prevent this from working?


*other motors

##### Fabien Rohrer [Moderator] 12/03/2019 12:20:54
You could maybe compare your setup with this example: WEBOTS\_HOME/projects/samples/devices/worlds/linear\_motor.wbt


A linear motor should be defined this way:


>>>     SliderJoint {

      jointParameters JointParameters {

        position 0.05

        axis 1 0 0

      }

      device [

        LinearMotor {

        }

      ]

      endPoint Solid {

        # ...

      }

    }


Complete code: [https://github.com/cyberbotics/webots/blob/revision/projects/samples/devices/worlds/linear\_motor.wbt](https://github.com/cyberbotics/webots/blob/revision/projects/samples/devices/worlds/linear_motor.wbt)

##### Steve222 12/03/2019 12:23:22
Thank you, Ill have a look through that and hopefully get it to work

##### Meisam 12/03/2019 13:03:57
`@Fabien Rohrer` Thank you. I have reduced ambient occlusion and texture quality to low. Also, I have disabled shadow and anti-aliasing options. However, they do not make tangible speedup. I noticed that when I am interacting with the 3D environment of Webots or running simulation the CPU usage reaches 100% while GPU usage is about 20%.

##### Fabien Rohrer [Moderator] 12/03/2019 13:06:35
It seems the bottleneck is on the CPU then.


This may mean several things...


the bottleneck of your simulation comes from physics and/or your controllers and/or general Webots updates (node update, rendering sort, etc.)


Probably my hypothesis about rendering improvements is wrong then.


Does it has sense in your simulation?

##### Wolfgang Fahl 12/03/2019 16:28:03
There is an issue with cyberbotics domain website and e-mail. What can we do about it?

##### Fabien Rohrer [Moderator] 12/03/2019 16:29:19
The website is currently down because of issues during a migration on our main server. We are currently working hard to solve this as soon as possible.


You may have access to the documentation inside Webots (Help / Offline documentation)


Some services are currently broken, such as the web resource files.


Do you have other issues?

##### Olivier Michel [Cyberbotics] 12/03/2019 16:31:33
You can download Webots from here: [https://github.com/cyberbotics/webots/releases/tag/R2019b-rev1](https://github.com/cyberbotics/webots/releases/tag/R2019b-rev1)

##### jacqueline 12/04/2019 00:44:11
hi since web is still down, can you tell me if husarion robot [https://husarion.com/](https://husarion.com/) is supported in webot like pioneer mobile base?

##### YCL 12/04/2019 04:11:11
`@Olivier Michel` Thank you for your kind help!

##### David Mansolino [Cyberbotics] 12/04/2019 07:31:41
`@jacqueline` husarion robot is not supported out of the box in Webots, but it should be quite straightforward to create a model of this robot. Or to create our URDF importer ([https://github.com/cyberbotics/urdf2webots](https://github.com/cyberbotics/urdf2webots)) to convert the URDF model to Webots: [https://github.com/husarion/rosbot\_description/tree/master/src/rosbot\_description](https://github.com/husarion/rosbot_description/tree/master/src/rosbot_description)

##### chamandana 12/04/2019 11:16:03
hello, can you find me a documentation about  function wb\_camera\_recognition\_get\_objects(). or at least tell me about it. The website is down.

##### David Mansolino [Cyberbotics] 12/04/2019 11:17:37
Hi `@chamandana` we are sorry we are indeed running some long updates on the website, you can find a backup of the simulation here: [https://github.com/cyberbotics/webots/blob/revision/docs/reference/recognition.md](https://github.com/cyberbotics/webots/blob/revision/docs/reference/recognition.md)

##### chamandana 12/04/2019 11:19:14
thanks

##### David Mansolino [Cyberbotics] 12/04/2019 11:19:19
You're welcome

##### chamandana 12/04/2019 11:27:51
another question, How can I use this code,

const WbCameraRecognitionObject *objects = wb\_camera\_recognition\_get\_objects(camera);



to get all the objects visible to the camera? I want to loop between those objects and see their positions, is there a sample code? 🙂

##### David Mansolino [Cyberbotics] 12/04/2019 11:28:49
You can get the number of object with the 'wb\_camera\_recognition\_get\_number\_of\_objects' function as documented here: [https://github.com/cyberbotics/webots/blob/revision/docs/reference/camera.md#wb\_camera\_has\_recognition](https://github.com/cyberbotics/webots/blob/revision/docs/reference/camera.md#wb_camera_has_recognition)


And here is an example: [https://github.com/cyberbotics/webots/blob/revision/projects/samples/devices/controllers/camera\_recognition/camera\_recognition.c#L59](https://github.com/cyberbotics/webots/blob/revision/projects/samples/devices/controllers/camera_recognition/camera_recognition.c#L59)

##### chamandana 12/04/2019 11:30:16
thank you, i'm really new to this 🙂

##### David Mansolino [Cyberbotics] 12/04/2019 11:30:33
You're welcome. Have fun with Webots 😉

##### chamandana 12/04/2019 11:51:51
the camera only detects 2 boxes out of all 30 currently visible in the camera. Some are pretty small. How can I make wb\_camera\_recognition\_get\_objects detect everything in the view? without changing the position or fov of the camera.


Is there a way? 🙂

##### David Mansolino [Cyberbotics] 12/04/2019 11:52:56
You have to make sure that all the objects you want to detect have a recognition color, a model and a boundign object set.


Be also carfull with the 'maxRange' and 'occlusion' fields of the 'Recognition' node.

##### chamandana 12/04/2019 11:54:01
oh okay. I'll take a look at them

##### KamiRonin 12/04/2019 11:58:39
Greetings to all!

My small problem with textures jpeg on Windows and did not manage to be solved. Neither variables PATH, nor installation of different libraries jpeg decoding, has not passed. After investigation has seen that the read error arises in Qt class QImageReader but why he does not see library libjpeg-8.dll delivered in package Webots - I have not understood. Perhaps someone has an access to a code of this class going to release Webots? To clarify what path it searches for library of decoding and why does not find.



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/651755060614791169/unknown.png)
%end

##### kwy 12/04/2019 14:45:51
Hallo, I want to implement a landmark-based SLAM, is there a GUI that can display map?

##### David Mansolino [Cyberbotics] 12/04/2019 14:46:41
`@kwy`, yes you can for example use a Display device to plot the map.

##### kwy 12/04/2019 14:46:41
except rviz


is there a GUI i can directly use on Webots?


or should I build a GUI with QT for that ?


`@David Mansolino`  thanks anyway

##### David Mansolino [Cyberbotics] 12/04/2019 15:08:19
You can use the Display device in Webots directly. But you can of course use QT or a robot-window: [https://www.cyberbotics.com/doc/guide/controller-plugin#robot-window](https://www.cyberbotics.com/doc/guide/controller-plugin#robot-window)

##### kwy 12/04/2019 15:14:44
Thank you for your kind help!

##### David Mansolino [Cyberbotics] 12/04/2019 15:19:13
You're welcome

##### ClBaze 12/04/2019 16:34:45
Hi guys, what is the time handling strategy of Webots ?

##### Fabien Rohrer [Moderator] 12/04/2019 16:37:52
Hi, the simulation refresh rate is given by WorldInfo.basicTimeStep which increases the simulation time and the physics engine by a constant rate. This time is virtual and is not linked with the true time, therefore a simulation can run faster or slower than true time. Robot controller are generally synchronized on this rate (or an integer multiple of it)


This section gives more info about this: [https://cyberbotics.com/doc/guide/controller-programming#the-step-and-wb\_robot\_step-functions](https://cyberbotics.com/doc/guide/controller-programming#the-step-and-wb_robot_step-functions)

##### ClBaze 12/04/2019 16:47:02
Ok then I don't understand this widget



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/651826861994803200/unknown.png)
%end


what does the 0.00x means


the basictimestep is fixed right ?

##### Fabien Rohrer [Moderator] 12/04/2019 16:50:54
Every 0.3 true seconds, the simulation speed is compared with the true time. This second value of the speedometer show the ratio with the true time. If you have 1.0x, you're close to the real time. If you have 2.0x you run twice faster than real time.


Yes, basictimestep is a constant. The same rate for all the simulation.


0.0x means the simulation is stopped.

##### JoHn 12/04/2019 16:57:01
Hi guys, I'd like to ask a question when I add a radar to a truck in Webots. As can be seen from the screenshot, the truck on the right has a radar installed in the front , and another truck is very closed to the radar.



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/651829348998840360/unknown.png)
%end

##### ClBaze 12/04/2019 16:57:32
Ok I understand, thank you Fabien

##### JoHn 12/04/2019 16:58:52
But when I check the radar data on the left. It shows that the distance is 3.4 meters. I am not sure why. Could you give me some help. Thanks a lot.

##### Fabien Rohrer [Moderator] 12/04/2019 17:10:29
`@JoHn` don't you think that another object is detected? what is the location of your radar in the truck? (cf. Radar.translation)

##### JoHn 12/04/2019 17:15:08
Hi `@Fabien Rohrer`, thank you for your response. The locationf of the radar can be seen in the screenshot below



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/651833900817186816/unknown.png)
%end

##### Fabien Rohrer [Moderator] 12/04/2019 17:16:04
this seems very correct.

##### JoHn 12/04/2019 17:20:17

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/651835181229146113/unknown.png)
%end


Now the white truck is about 8 meters ahead of the grey truck.

##### Fabien Rohrer [Moderator] 12/04/2019 17:21:22
the lidar seems well oriented, the azimuth seems to indicate indeed something front. I cannot explain this 4 meters offset from the screenshot.

##### JoHn 12/04/2019 17:21:40
But the radar shows that the distance is 10 meters and only one object is detected.

##### Fabien Rohrer [Moderator] 12/04/2019 17:22:03
Could you try to simplify your world/controller at best and send it to us? Maybe through a GitHub issue ticket?


[https://github.com/cyberbotics/webots/issues/new/choose](https://github.com/cyberbotics/webots/issues/new/choose)


We have to open this in Webots to determine what's going on..

##### JoHn 12/04/2019 17:23:21

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/651835953328947236/unknown.png)
%end


Now both trucks are very closed. The radar detects one object and the distance is 3.56 meters.


Sure, thank you very much for your answers `@Fabien Rohrer`

##### Fabien Rohrer [Moderator] 12/04/2019 17:24:45
Isn't it the center of the other truck?


What I would do is to create a sphere, and put it at the location indicated by the radar.


Quite archaic, but efficient.


In this case, you will be sure of what the radar detects...


To do so, just put the first truck at the world position 0 (or better, its radar), create a "Transform > Sphere", and move it at the location given by the azimuth+distance.


The center of vehicles is at the center of the 2 rear wheels.


Not very intuitive.

##### JoHn 12/04/2019 17:28:10
Sure, I will try it now.

##### Fabien Rohrer [Moderator] 12/04/2019 17:28:41
I have to go, please open a ticket or come back here tomorow. Good evening

##### JoHn 12/04/2019 17:37:09
Thank you `@Fabien Rohrer`. You have a good night.

##### abaskh46 12/05/2019 07:04:36
how can i create wall . i stuck in tutorial 2 ,ADD WALL section,

##### UchihaX2 12/05/2019 07:11:07
hi. i'm intending to build a bi-pedal robot. any advice to whether webots is suitable for simulating this project

##### chamandana 12/05/2019 07:16:44
hello, can anyone suggest me a way to receive the location of the two youbots in this world. I have a depth and color sensor attached on the ceiling with fov large enough to view all the area. Can I use range\_finder to identify objects? Can you tell me how. 🙂
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/652045680646422528/unknown.png)
%end

##### Stefania Pedrazzi [Cyberbotics] 12/05/2019 07:17:02
Hi `@abaskh46`: you have to build your wall as you previously did for the Ball, but this time using a Box geometry instead of a Sphere geometry. The structure of the Wall you are supposed to create is the same as the one of the ball [https://www.cyberbotics.com/doc/guide/tutorial-2-modification-of-the-environment#def-use-mechanism-applied-on-the-shape-node-of-a-solid](https://www.cyberbotics.com/doc/guide/tutorial-2-modification-of-the-environment#def-use-mechanism-applied-on-the-shape-node-of-a-solid)

##### David Mansolino [Cyberbotics] 12/05/2019 07:22:48
Hi `@UchihaX2`, yes Webots is probably the tool you need, as you can see on this list many pi-pedal robots are already available out of the box in Webots: [https://cyberbotics.com/doc/guide/robots](https://cyberbotics.com/doc/guide/robots)

##### UchihaX2 12/05/2019 07:44:23
`@David Mansolino`  thank you.

##### David Mansolino [Cyberbotics] 12/05/2019 07:45:21
You'rw welcome


`@chamandana`, yes you can of course use a range finder or camera, but a simpler solution is simply to use a Supervisor controller to track the robots: [https://cyberbotics.com/doc/reference/supervisor](https://cyberbotics.com/doc/reference/supervisor)


`@UchihaX2` You're welcome

##### chamandana 12/05/2019 07:50:54
`@David Mansolino` thanks 🙂

##### David Mansolino [Cyberbotics] 12/05/2019 07:51:37
`@chamandana` you're welcome

##### chamandana 12/05/2019 08:24:54
Is there a way to share data (values in an array) between two controllers (Robot controller and camera controller)?

##### Stefania Pedrazzi [Cyberbotics] 12/05/2019 08:26:32
Yes, a common way is to use the Emitter/Receiver nodes.


Otherwise any method for interprocess communication works also between Webots controllers (i.e. write/read to/from file, shared memory, sockets, etc.)

##### chamandana 12/05/2019 08:28:22
oh okay 🙂


can I get an example for shared memory or sockets please

##### Stefania Pedrazzi [Cyberbotics] 12/05/2019 08:45:25
this is not specific to Webots, you should ask in a programming forum. But I can point you to an example where Emitter/Reveicer communication method is used.

##### chamandana 12/05/2019 08:46:01
I found many examples, but couldn't find a way to include socket.h or other library


in webots


where should I copy them :/

##### Stefania Pedrazzi [Cyberbotics] 12/05/2019 08:46:44
You should not copy them, but link them from the Makefile:  [https://www.cyberbotics.com/doc/guide/using-webots-makefiles](https://www.cyberbotics.com/doc/guide/using-webots-makefiles)


Note that standard libraries can usually be used directly simply including them in the code. Not sure about socket.h

##### chamandana 12/05/2019 08:48:24
okay. I'll check it out.

##### White 12/05/2019 20:29:18
Hi, I'd like to register a new Webots account but when i click on the link on browser i see the error 404 page. How can i solve the problem?

##### Tahir [Moderator] 12/05/2019 20:59:23
Hi

I am using webots ros controller on a custom robots in a multi robot industrial scenario. When two robots face each other, the controller stops sending the velocity commands, but the robots move with the last sent velocity command. So I have written a ros service which sends zero to the robot when velocity controller stops. I have got a strange behaviour that this service works for one robot and it stops but the other one moves with the last cmd.

Anyone have an idea what's could be the problem or a point where I am doing the mistake

##### askslk 12/05/2019 23:22:25
Hi, I have a problem is that I code a program use if fuction and camera to touch the ball.Once I touch the ball,touch sensor receive data, my next if function will call the car back to central point.But in real, my robot touch the ball but didnt goto next function, camera part still work and still go to touch the ball.

##### chamandana 12/06/2019 03:29:31
hello, can anyone tell me how to use wb\_robot\_get\_custom\_data  function? there seems to be no argument to pass to it (I thought I could pass the WbDeviceTag)


I want to change customData of another objet


another robot*

##### seppl2018 12/06/2019 06:33:02
The highway driving example does not work properly for me:



[sumo\_supervisor] Connect to SUMO... This operation may take a few seconds.

[sumo\_supervisor] Unable to connect to SUMO, please make sure any previous instance of SUMO is closed.

[sumo\_supervisor]  You can try changing SUMO port using the "--port" argument.

[sumo\_supervisor] Could not connect to TraCI server at localhost:1550 [Errno 61] Connection refused

[sumo\_supervisor]  Retrying in 1 seconds

[sumo\_supervisor] Could not connect to TraCI server at localhost:1550 [Errno 61] Connection refused

[sumo\_supervisor]  Retrying in 2 seconds

##### David Mansolino [Cyberbotics] 12/06/2019 08:07:50
`@White` we are currenlty updating our website and not all the features are yet available. However, you shouldn't need and accoutn to use Webots anymore, why do you want to create an account?


`@askslk`, I am really sorry but it is very difficult to understand what is happenning without looking at your simulation.


`@chamandana` there is indeed no argument as the `wb_robot_get_custom_data` function will return the content of the customData field of the robot calling the function, this function is not linked with any device but with the robot directly.


`@seppl2018` Webots is using the port '8873' to communicate with SUMO, it is possible that on your computer this port is not available and therefore Webots can't communicate. You shoudl try to change the port with the correspondign field of the SumoInterface node.


`@Tahir` sorrry no idea where this could come from. Are you sure you are sending these zero to both robots ?

##### Tahir [Moderator] 12/06/2019 08:23:18
Yes I am logging the info with the robot names as well


So I can see both are services are being called

##### David Mansolino [Cyberbotics] 12/06/2019 08:25:20
And then are you calling the step service of both robot ?

##### Tahir [Moderator] 12/06/2019 08:25:32
Yes

##### David Mansolino [Cyberbotics] 12/06/2019 08:26:22
Ok then I don't see any obvious reason for this to fail. Maybe adding some print in the ros controller to check if the service messages are received and processed is a good way to go for debugging

##### Tahir [Moderator] 12/06/2019 08:26:53
OK I'll check that

##### David Mansolino [Cyberbotics] 12/06/2019 08:27:06
For example here: [https://github.com/cyberbotics/webots/blob/revision/projects/default/controllers/ros/RosMotor.cpp#L108](https://github.com/cyberbotics/webots/blob/revision/projects/default/controllers/ros/RosMotor.cpp#L108)

##### Tahir [Moderator] 12/06/2019 08:28:36
I am doing the same thing but except I am taking robot name as an argument to set its velocity to zero

##### lf\_lidar\_support 12/06/2019 10:56:10
Hello, is the forum down?

##### David Mansolino [Cyberbotics] 12/06/2019 10:57:09
Hi, yes we are currently running some migration of the server, please use stackoverflow or this discord server instead.

##### lf\_lidar\_support 12/06/2019 10:57:25
Okay, thank you

##### David Mansolino [Cyberbotics] 12/06/2019 10:57:31
Your'e welcome

##### lf\_lidar\_support 12/06/2019 11:00:05
Im running into some issues with a lidar. 

Im having multiple robots (Elisa3), with attatched lidars on top, but they are having trouble seeing other Elisa3 robots (except of the wheels? How do I make the lidar detect the diffuser?

##### David Mansolino [Cyberbotics] 12/06/2019 11:00:56
Lidar do not detctes transparent objects so this might be the problem, if the body of the Elisa is transparent the lidar might missed it.

##### lf\_lidar\_support 12/06/2019 11:02:11
Objects are only transparent if they have transparentcie = 1? right?

##### David Mansolino [Cyberbotics] 12/06/2019 11:14:18
Non semi-transparent (transparenci != 0.0) object might be affected too.

##### Hayden Woodger 12/06/2019 12:50:55
Hi there, how can I manipulate an object handle in python please?

##### David Mansolino [Cyberbotics] 12/06/2019 13:20:54
Hi `@Hayden Woodger` you can move any object using the Python Supervisor API: [https://www.cyberbotics.com/doc/reference/supervisor](https://www.cyberbotics.com/doc/reference/supervisor)

Here is an example where a light is moved: ps://www.cyberbotics.com/doc/guide/samples-devices#supervisor-wbt

##### Hayden Woodger 12/06/2019 13:22:47
Thanks, that helped 🙂

##### David Mansolino [Cyberbotics] 12/06/2019 13:23:42
You're welcome :-)

##### YCL 12/06/2019 14:08:46
Hello! I am wondering is there someone tell me how to  plot  a figure of processing data by using webots?  Thank you!


And can you open these links?



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/652511899824422952/unknown.png)
%end

##### David Mansolino [Cyberbotics] 12/06/2019 14:11:03
Hi `@YCL` you can use the Display device to plot data, see for example this example how plots the position of a joint: [https://cyberbotics.com/doc/guide/samples-howto#ziegler\_nichols-wbt](https://cyberbotics.com/doc/guide/samples-howto#ziegler_nichols-wbt)


Unfortunately the forum is currently offline, we are running some migration.

##### YCL 12/06/2019 14:14:32
Thanks, that helped

##### David Mansolino [Cyberbotics] 12/06/2019 14:14:48
You're welcome

##### threeal 12/06/2019 14:42:20
why i cannot fetch to [https://www.cyberbotics.com/debian/binary-amd64/InRelease](https://www.cyberbotics.com/debian/binary-amd64/InRelease) ?

##### Olivier Michel [Cyberbotics] 12/06/2019 14:54:08
Hi, we moved the web site on a new server and didn't yet fixed that. We will try to fix it asap.

##### White 12/06/2019 15:23:52
`@David Mansolino` Because for the first login I need email and password. I don't see a skip button or something similar \_:D

##### David Mansolino [Cyberbotics] 12/06/2019 15:28:10
`@White` as documented here if you use an old version of Webots you need indeed an email and password, but you can but any email and set the password to 'webots': [https://www.cyberbotics.com/doc/guide/general-faq#can-i-still-use-a-webots-version-before-the-r2019a-release](https://www.cyberbotics.com/doc/guide/general-faq#can-i-still-use-a-webots-version-before-the-r2019a-release)


But we strongly recommend to use the latest version (R2019b)

##### White 12/06/2019 15:28:50
I installed the version for nao, the 8


Thanks David🙂

##### David Mansolino [Cyberbotics] 12/06/2019 15:31:53
You're welcome

##### YCL 12/06/2019 16:04:00
`@David Mansolino` Hello, Can I export data got from webot to excel?


Hello, is there someone know how to export data got from webot to excel?

##### David Mansolino [Cyberbotics] 12/06/2019 16:19:09
`@YC`L there is no out of the box tool to export to excel, but from you controller you can writte any data you want in a cvs file for example as you would do with any other program. A quick search on the web pointed me on this for Python for example: [https://realpython.com/python-csv/](https://realpython.com/python-csv/)

##### YCL 12/06/2019 16:25:12
`@David Mansolino` Thank you very much!

##### David Mansolino [Cyberbotics] 12/06/2019 16:25:19
You're welcome

##### askslk 12/06/2019 21:49:41
Hi, I have a problem is that I code a program use if fuction and camera to touch the ball.Once I touch the ball,touch sensor receive data, my next if function will call the car back to central point.But in real, my robot touch the ball but didnt goto next function, camera part still work and still go to touch the ball.

##### David Mansolino [Cyberbotics] 12/06/2019 21:54:01
>   `@askslk`, I am really sorry but it is very difficult to understand what is happening without looking at your simulation.

##### JasonChen 12/07/2019 09:00:16
Hi, I am getting the error message of using the function "wb\_motor\_set\_postion" int Matlab, and don't know how to fix it. Can you help me? Thanks.

The Webots version is 2019b-rev1, and Matlab version is 2019b.



Sorry. My fault. The wrong typo.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/652796511524093995/unknown.png)
%end


Hi, I notice there is a view named "support polygon", and I am recently doing simulation on bipedal robot. I wonder through your Webots API, can I get the support polygon information (data)?

##### Fabien Rohrer [Moderator] 12/07/2019 10:59:12
About your issue with matlab, could you show us your controller instead? It seems there is an issue with the arguments. Could you check what is « inf »?


About the support polygon, the supervisor api can retrieve some of these values:

- [https://cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_node\_get\_static\_balance](https://cyberbotics.com/doc/reference/supervisor#wb_supervisor_node_get_static_balance)


And the contact points:

- [https://cyberbotics.com/doc/reference/supervisor?tab-language=matlab#wb\_supervisor\_node\_get\_contact\_point](https://cyberbotics.com/doc/reference/supervisor?tab-language=matlab#wb_supervisor_node_get_contact_point)

##### JasonChen 12/07/2019 11:03:47
`@Fabien Rohrer` Thanks, that would be great helpful.


`@Fabien Rohrer` Hi, here is another question. I am now trying to use the wb\_supervisor\_node\_get\_number\_of\_contact\_points function. But it seems that the function in the Matlab has two input parameters? the noderef and index. While in the document, it says it only has one. And the demo code in C also shows it only needs one input parameters. So, what's the second parameter for?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/652843112535687169/unknown.png)
%end

##### UhName 12/08/2019 06:33:44
Hi all, just downloaded the app, using it for testing a movement program.

##### Dorteel 12/08/2019 09:30:00
Hi! I'm trying to use my python virtual environment from webots, but it doesn't seem to work when I try to define it with the shebang line (from Linux), but get an error message, do you have any suggestions?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/653166381948207104/unknown.png)
%end

##### bsr.nur.bahadir 12/08/2019 10:44:13
It happened to me yesterday I checked the Path it didn't work again but after a while I restarted the app it works now.

##### Otto 12/08/2019 11:40:26
hello

##### JasonChen 12/08/2019 11:40:37
hello


how are you

##### Otto 12/08/2019 11:41:41
Does someone know where to get a tutorial about protos and nodes from?


I'm happy to have Webots 🙂

##### JasonChen 12/08/2019 11:42:03
proto


I think you can use the User Guide


There is a chapter talking about PROTO

##### Otto 12/08/2019 11:44:10
I know, but it didnt help me much. I think I just continue trying to understand it with the User guide


Hi

##### JasonChen 12/08/2019 14:34:42
Has anyone here done bipedal robot simulation before?


I would like to learn such thing from you. If you like to teach me something about that. 😛

##### moustachemartin 12/08/2019 14:37:29
Hi everyone. I need a little help. I need to add a linear actuator but it doesn’t seem to be working. I’m not sure whether I’m not using the right commands in the controller or I haven’t set it up properly in the scene tree.

##### chamandana 12/08/2019 14:41:42
Hello, I attached a receiver to the bodyslot in a Kuka youbot and when I try to read data using wb\_receiver\_get\_data it says invalid tag.

##### Fabien Rohrer [Moderator] 12/09/2019 05:10:37
`@chamandana` I just answer you on the « general » chat.


`@moustachemartin` the reference example is [https://cyberbotics.com/doc/guide/samples-devices#linear\_motor-wbt](https://cyberbotics.com/doc/guide/samples-devices#linear_motor-wbt)


You should give a try, and understand how this works.


If you’re still stuck, please send us your problematic  world file.

##### JasonChen 12/09/2019 05:13:44
`@Fabien Rohrer` Hi, here is another question. I am now trying to use the wb\_supervisor\_node\_get\_number\_of\_contact\_points function. But it seems that the function in the Matlab has two input parameters? the noderef and index. While in the document, it says it only has one. And the demo code in C also shows it only needs one input parameters. So, what's the second parameter for?


[https://media.discordapp.net/attachments/565154703139405824/652843112535687169/unknown.png](https://media.discordapp.net/attachments/565154703139405824/652843112535687169/unknown.png)

##### Fabien Rohrer [Moderator] 12/09/2019 05:15:01
`@Otto` the Proto doc is the best we have: [https://www.cyberbotics.com/doc/reference/proto](https://www.cyberbotics.com/doc/reference/proto)


The PROTO mechanism is quite simple, it only hide some node hierarchy in a file, with custom fields. It could be extended with a scripting language (lua as a template language). This is extremely powerful, but it requires to be expert in the Webots hierarchy nodes.


`@JasonChen` let me check... normally the matlab API is a direct match of the C API...


Ok I see. This is a bit weird to use...


wb\_supervisor\_node\_get\_contact\_point() is supposed to be called several times.


When an objet collides, it can have several contact polygons.


So this function should be used as follows (in C but in Matlab, it’s the same logics):


>>> for (int p=0; p < wb\_supervisor\_node\_get\_number\_of\_contact\_points(my\_solid); p++) { // foreach polygon

    const double *polygon\_points = wb\_supervisor\_node\_get\_contact\_point(my\_solid, p); // get the polygon coordinates

  printf(« polygon %d\n »,  p);

  int c = 0;

  while (true) {

     if (isnan(polygon\_points[3 * c])

      break; // no more points for this polygon 

    printf(« point %f %f %f\n »,

      polygon\_points[3 * c],

      polygon\_points[3 * c + 1],

      polygon\_points[3 * c + 2]

    );

    c++;

  }

}


So the second parameter of wb\_supervisor\_node\_get\_contact\_point() is the polygon index. This function should be called several times until it returns NaN to get the polygon coordinates.

##### JasonChen 12/09/2019 05:39:47
no, no, no


I mean the wb\_supervisor\_node\_get\_number\_of\_contact\_points(my\_solid) has another parameter in matlab


you see, you have only one parameter here.


`@Fabien Rohrer`


can you please have a check

##### Fabien Rohrer [Moderator] 12/09/2019 05:42:28
noderef and index?

##### JasonChen 12/09/2019 05:42:33
yes


which is weird for the function "wb\_supervisor\_node\_get\_number\_of\_contact\_points(my\_solid)"

##### Fabien Rohrer [Moderator] 12/09/2019 05:44:01
Oh, I get the point, now. Sorry it seems indeed there is a mistake


Let me check.

##### JasonChen 12/09/2019 05:44:28
ok, thanks.

##### Fabien Rohrer [Moderator] 12/09/2019 05:48:37
Yes, this is definitely wrong I will open an issue ticket so that you could monitor the issue resolution, and will do my best to fix this on this morning.

##### JasonChen 12/09/2019 05:49:00
Thanks. Best wishes.

##### Fabien Rohrer [Moderator] 12/09/2019 05:54:27
`@JasonChen` here is the issue to track: [https://github.com/cyberbotics/webots/issues/1156](https://github.com/cyberbotics/webots/issues/1156)


Thank you for your contribution, and sorry do not have understand this earlier: these 2 API functions are cryptic, even for me 😅

##### JasonChen 12/09/2019 05:56:04
hahaha, ok, no big deal. 🤣


I think Webots is really great. Wish you can make it better. 👍

##### Fabien Rohrer [Moderator] 12/09/2019 05:58:12
`@Dorteel` you should dig into this question: why your python path (/home/...) is not found in the Webots environment? Did you modified PATH or so?

##### Nocturnal Warfare 12/09/2019 06:46:35
Has anyone been able to program controllers and run from visual studio code? I know that visual studio ide works for c and pycharm for python, but I was wondering if I could use vs code with python and webots

##### Fabien Rohrer [Moderator] 12/09/2019 06:52:56
`@Nocturnal Warfare` Webots is compatible with any IDE, it’s a matter to set it up correctly.


You could refer to this document to have general guidelines to setup your IDE: [https://www.cyberbotics.com/doc/guide/using-your-ide](https://www.cyberbotics.com/doc/guide/using-your-ide)


`@JasonChen` Just to notify you that the bug has been fixed by `@Stefania Pedrazzi` in the develop version of Webots, and will be a part of the next release (before the end of this year), and of the next "develop" nightly build (tomorrow): [https://github.com/cyberbotics/webots/pull/1157](https://github.com/cyberbotics/webots/pull/1157)

##### JasonChen 12/09/2019 10:32:12
Ok, thanks.

##### Hayden Woodger 12/09/2019 11:49:03
Hi there, i can't seem to import any file types other than VRML97. I've read it's a very old format. Is this the best we can do?

##### Stefania Pedrazzi [Cyberbotics] 12/09/2019 12:17:32
Hi `@Hayden Woodger`, yes to import your model or project (if not already developed with Webots) in Webots you have to export to VRML. The reason is that Webots format is an extension of VRML. But if you are using Blender, we provide a tool to export your Blender project directly to Webots format: [https://github.com/cyberbotics/blender-webots-exporter](https://github.com/cyberbotics/blender-webots-exporter)

##### Hayden Woodger 12/09/2019 12:19:49
Thank you! I will use blender in future with this add-on. 

After you've exported to VRML from blender, will the model in Webots be accurate in dimensions?

##### Stefania Pedrazzi [Cyberbotics] 12/09/2019 12:22:56
Yes, you just have to make sure that the model is exported in meters.

##### Hayden Woodger 12/09/2019 12:23:33
Awesome! Ill get cracking haha


thanks man

##### Stefania Pedrazzi [Cyberbotics] 12/09/2019 12:23:53
you're welcome

##### chamandana 12/09/2019 13:20:12
Hey, is there a way to debug measurements like I need to spawn a circle /line gizmo at a given position to see if the robot reached the actual location.

##### David Mansolino [Cyberbotics] 12/09/2019 13:41:01
Hi, yes you can use the supervisor API to diplay shapes in the 3D scene

##### chamandana 12/09/2019 14:38:43
I can't get this device using wb\_robot\_get\_device("distance sensor"); Please help
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/653606461795991562/unknown.png)
%end

##### Olivier Michel [Cyberbotics] 12/09/2019 14:45:24
Did you try to rename it to something different, like "ds" instead of "distance sensor"?

##### chamandana 12/09/2019 14:48:53
i'll try


I'm not getting an error but it only gives out 1 as an ouput



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/653609409372749834/unknown.png)
%end



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/653609445859000341/unknown.png)
%end

##### Olivier Michel [Cyberbotics] 12/09/2019 14:52:18
OK, then it's not a problem with the name, but rather with the behavior of the sensor.

##### Stefania Pedrazzi [Cyberbotics] 12/09/2019 14:52:24
`wb_distance_sensor_get_value` returns a double not an int => you should use `%f` in your printf

##### chamandana 12/09/2019 14:52:38
it returns 0.000 once %f is used



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/653610044855943169/unknown.png)
%end

##### Olivier Michel [Cyberbotics] 12/09/2019 14:53:01
That may be normal. Is there any obstacle in front of the sensor?

##### chamandana 12/09/2019 14:53:07
it says they expected an int


`@Olivier Michel` here it shows fine
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/653610269213458443/unknown.png)
%end


but I can't get it via code

##### Olivier Michel [Cyberbotics] 12/09/2019 14:55:33
The warning you get looks wrong as `wb_distance_sensor_get_value` for sure returns a double value. See [https://cyberbotics.com/doc/reference/distancesensor#distancesensor-functions](https://cyberbotics.com/doc/reference/distancesensor#distancesensor-functions)

##### chamandana 12/09/2019 14:56:01
I get another error warning: implicit declaration of function 'wb\_distance\_sensor\_enable'; did you mean 'wb\_range\_finder\_enable'? [-Wimplicit-function-declaration]

##### Olivier Michel [Cyberbotics] 12/09/2019 14:56:01
So, there may be something wrong in your code. Did you include properly `<webots/distance_sensor.h>`?

##### chamandana 12/09/2019 14:56:39
yeah

##### Olivier Michel [Cyberbotics] 12/09/2019 14:57:04
So, I don't understand why you get these warnings...


You shouldn't.


And that's probably the cause of your problem, not being able to retrieve distance sensor values.

##### chamandana 12/09/2019 14:57:55
what if I try remove #include range\_finder.h

##### Olivier Michel [Cyberbotics] 12/09/2019 14:58:30
Can you try ? I would be surprised however if that helps.

##### chamandana 12/09/2019 14:58:59
yeah. 😄 it didn't help

##### Olivier Michel [Cyberbotics] 12/09/2019 14:59:34
Otherwise, I would recommend you to try to run `webots/projects/samples/devices/worlds/distance_sensor.wbt` and edit the associated robot controller which works retrieving distance sensor values. Then, compare it with yours.

##### chamandana 12/09/2019 15:00:12
okay, thank you for your time 😄 I'll try that

##### lgrobots 12/09/2019 16:16:05
how can I link opencv3 when compiling with webots

##### David Mansolino [Cyberbotics] 12/09/2019 16:16:24
Hi `@lgrobots`

##### lgrobots 12/09/2019 16:16:29
hello

##### David Mansolino [Cyberbotics] 12/09/2019 16:17:36
You can add any external library by modifing your makefile, please see the doc for more information: [https://cyberbotics.com/doc/guide/using-webots-makefiles#adding-an-external-library-ccp](https://cyberbotics.com/doc/guide/using-webots-makefiles#adding-an-external-library-ccp)

##### lgrobots 12/09/2019 16:18:35
I have done that and it seems to be conflicting with opencv2.4 which is included in webots

##### David Mansolino [Cyberbotics] 12/09/2019 16:19:46
That's indeed possible, we are currently improved the library hierarchy to avoid polluting the controller path with any external libraries (such as opencv).

##### lgrobots 12/09/2019 16:19:46
this is my error:

/usr/bin/ld: warning: libopencv\_core.so.3.2, needed by /usr/lib/gcc/x86\_64-linux-gnu/7/../../../x86\_64-linux-gnu/libopencv\_highgui.so, 

may conflict with libopencv\_core.so.2.4

undefined reference to symbol '\_ZN2cv6String10deallocateEv'

##### David Mansolino [Cyberbotics] 12/09/2019 16:20:19
Can't you link directly with libopencv\_highgui.so.3.2 ?

##### lgrobots 12/09/2019 16:22:15
yes I believe that is what I am doing but it is linking libopencv\_core.so.2.4 instead of libopencv\_core.so.3.2


Is there a way to override opencv2.4 used by webots?

##### David Mansolino [Cyberbotics] 12/09/2019 16:23:25
Worse case you can remove it from the 'lib' folder (this will not prevent Webots from working as it is not used directly by Webots)

##### lgrobots 12/09/2019 16:23:42
oh i didnt realize that


I will try that, thank you

##### David Mansolino [Cyberbotics] 12/09/2019 16:24:00
You're welcome

##### Hayden Woodger 12/10/2019 00:33:48
Hi David, i've been trying all day to get the "webots\_exporter" plugin to work. Hopefully you can I help! Here's where i'm at...
%figure
![hmm.png](https://cdn.discordapp.com/attachments/565154703139405824/653756217218433054/hmm.png)
%end

##### David Mansolino [Cyberbotics] 12/10/2019 07:31:47
Hi `@Hayden Woodger` which version of blender are you using ?

##### Fabien Rohrer [Moderator] 12/10/2019 07:57:17
`@Hayden Woodger` I confirm that the plugin is supposed to work with Blender 2.79. In Blender 2.80, the plugin API has changed, our source code should be updated. [https://github.com/cyberbotics/blender-webots-exporter/blob/master/\_\_init\_\_.py#L19](https://github.com/cyberbotics/blender-webots-exporter/blob/master/__init__.py#L19)

##### Gautier 12/10/2019 08:59:43
Hey everyone !


I have a little problem with webots, I have a little problem with Webots. When I instanciate a new proto thanks to the supervisor, and that this proto is "locked" by default to another connector, I run into "unexpected behaviors". It kind of "explode" ?


Here is a video of the instanciation with the lock :


[https://www.youtube.com/watch?v=ZP6e8Upyqhk&feature=youtu.be](https://www.youtube.com/watch?v=ZP6e8Upyqhk&feature=youtu.be)


And here is a video when the connector doesn't try to "lock": [https://youtu.be/iCPuJ1ZjQm0](https://youtu.be/iCPuJ1ZjQm0)

##### Fabien Rohrer [Moderator] 12/10/2019 09:02:47
`@Gautier` Hi Gautier

##### nap 12/10/2019 09:03:15
Hi everyone

I have a project from Webots 7 that I'm migrating into Webots 2019b r1.

I have a number of 'Solid' objects which have a texture applied to them, but the rendered texture appears 'washed out' compared to the original texture.  Any tips on how to fix this please?

##### Fabien Rohrer [Moderator] 12/10/2019 09:04:20
`@Gautier` Could it be that the Connector are badly oriented? Normally, it's z-axis forwards, y-axis upwards.


`@Gautier` I'm contacting you with a private message, we have things to discuss 😉


`@nap` Good choice 😉


`@nap` Between Webots 7 and R2019b, we changed our rendering engine and introduced PBR, HDR backgrounds etc.


You should change the following in your .wbt and .protos files:


1. Remove the Background node and replace it by a TexturedBackground instance.


2. Replace every Appearance node by the PBRAppearance node, and update their fields: [https://cyberbotics.com/doc/reference/pbrappearance](https://cyberbotics.com/doc/reference/pbrappearance)


3. An iteration on the light fields is also often required.


For the textures in particular, you should use this PBRAppearance template:


>>> PBRAppearance {

  roughness 1

  metalness 0

  baseColorMap ImageTexture {

    url "path/to/your/texture.jpg"

  }

}


Let us know if you encounter issues during your migration 🙂

##### nap 12/10/2019 09:15:01
`@Fabien Rohrer` : Thanks!!  I'll do that now and see how I go.


I'm only using a .wbt for this, and in its XML, I have the following the solid I'm going to fix first:

Solid {

  translation 0 0.025 0.82

  children [

    DEF Static\_Obstacle Shape {

      appearance Appearance {

        texture ImageTexture {

          url [

            "textures/do\_not\_enter.png"

          ]

        }

      }

      geometry Box {

        size 0.05 0.05 0.05

      }

    }

  ]

  name "fixed01"

  boundingObject USE Static\_Obstacle

}


So, no TexturedBackground.  But let me go through the rest and see if it works.

##### Fabien Rohrer [Moderator] 12/10/2019 09:19:46
Ok, it seems that the only problematic point is the Appearance node then, if you follow the instructions above, the migration would work smoothly.


About the TexturedBackground node, I recommend you to add one, it would increase A LOT the rendering quality of your simulation.

##### nap 12/10/2019 09:21:30
`@Fabien Rohrer` : Ok, I'll do that.

Another question:  Is there a search function in the Off-line User Manual?

##### Fabien Rohrer [Moderator] 12/10/2019 09:22:09
Unfortunately no.


To be honest, googling our doc is very powerful, we are well referenced: "webots lidar", "webots using python", etc.

##### nap 12/10/2019 09:24:43
True, but then I have the latency accessing the web.

##### Fabien Rohrer [Moderator] 12/10/2019 09:25:31
I understand this, this is an interesting missing feature, I will keep it in mind.

##### Stefania Pedrazzi [Cyberbotics] 12/10/2019 09:27:50
`@nap` if you clone the Webots github repository you should be able to run the documention on your browser offline


I'm not sure that this works with the released Webots package too


yes, it works from the Webots package too, I just checked


you should simply start a python HTTP server:

1. go to the `<WEBOTS>/docs` folder

2. start the python HTTP server: `python -m SimpleHTTPServer`

3. then from your browser open `localhost:8000` and you should get the documentation


for the reference manual the URL is `http://localhost:8000/?book=reference&page=index`

##### Dorteel 12/10/2019 09:36:42
Thank you `@Fabien Rohrer` , unfortunately I don't know much about modifying the PATH, it's a desktop from the university that we got the error on, it works perfectly on my own computer. From what I could read online, I tried exporting the virtual environment's path to PYTHONPATH, but that didn't solve it.

##### Fabien Rohrer [Moderator] 12/10/2019 09:47:00
Could you give me the path to your Python binary?


>>> ls -lF $(which python)


>>> ls -lF $(which python3)

##### Dorteel 12/10/2019 09:54:07
Thanks for the quick answer. I get "lrwxrwxrwx 1 root root 9 Mar 19  2019 /usr/bin/python3 -> python3.5*"

##### Fabien Rohrer [Moderator] 12/10/2019 09:56:04
>>> Repost of your issue
%figure
![dorteel.png](https://cdn.discordapp.com/attachments/565154703139405824/653897717524725760/dorteel.png)
%end


It seems your environement tries to run another Python:


>>> /home/robotlab/r762/bin/ROB762/bin/python3


and Webots fails to run it because of a version mismatch.


Could you try the following command?


>>> 

/home/robotlab/r762/bin/ROB762/bin/python3 --version

##### Dorteel 12/10/2019 10:00:03
It says Python 3.5.2

##### Fabien Rohrer [Moderator] 12/10/2019 10:01:59
Webots fails to run this python binary (WARNING: command not found).

##### Dorteel 12/10/2019 10:02:12
but I thought if I define this python for WeBots on the shebang line, then that's what WeBots will use

##### Fabien Rohrer [Moderator] 12/10/2019 10:04:35
it's the case. Webots uses first the shebang, and then the "Webots Preferences / General / Python command" preference to locate the Python binary.


How did you installed Webots? From snap? Which version? What is your OS?

##### Dorteel 12/10/2019 10:06:27
Distributor ID:    Ubuntu

Description:    Ubuntu 16.04.6 LTS

Release:    16.04

Codename:    xenial


installed through the Ubuntu software store


R2019b revision 1

##### Gautier 12/10/2019 10:10:40
`@Fabien Rohrer`  I think the axis are "correct". I delted the shape of the connector and just have the "connector" in the Tree object to show only its coordinate system
%figure
![test_supervisor_tile_container_2.png](https://cdn.discordapp.com/attachments/565154703139405824/653901392230088714/test_supervisor_tile_container_2.png)
%end


Also, its not really visible but it's a supervisor that instanciate the tiles and, after instanciating them, move them to the correct coordiante and orientation. The "connectors" works when it's placed and locked manually before the simulation begins, but the "problem" is when we use the supervisor to instanciate the proto of the object with the connector.

##### Fabien Rohrer [Moderator] 12/10/2019 10:16:52
`@Dorteel` Ok, I think I see the issue. When is installing from the Ubuntu store (aka snap), it runs with restricted privileges, this may cause issues to detect external Python binaries. You could either uninstall Webots and reinstall it from the tar.bz2 package (cf. [https://github.com/cyberbotics/webots/releases](https://github.com/cyberbotics/webots/releases)) or use external controllers mechanism which should prevent this limitation (cf. [https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers](https://www.cyberbotics.com/doc/guide/running-extern-robot-controllers))

##### Dorteel 12/10/2019 10:19:13
Thank you very much `@Fabien Rohrer` for your help and patience, we'll try that!

##### Stefania Pedrazzi [Cyberbotics] 12/10/2019 10:19:16
here are some specific instructions for the snap package in case: [https://www.cyberbotics.com/doc/guide/installation-procedure?version=develop&tab-os=windows#installing-the-snap-package](https://www.cyberbotics.com/doc/guide/installation-procedure?version=develop&tab-os=windows#installing-the-snap-package)

##### nap 12/10/2019 10:19:33
`@Fabien Rohrer` : Thanks for the help on converting.  My texture issue is resolved to my satisfaction.  (There are more settings in the PBR than I want to mess with right now.)  I also did the same for the floor and walls. (Though I need to work out the exact mapping of the walls because my Uni Logo isn't showing in the right place.)

##### Fabien Rohrer [Moderator] 12/10/2019 10:21:23
`@nap` thank you for the feedback, I also advise you to use our PROTO to manage floors and walls, typically the RectangleArena PROTO: [https://www.cyberbotics.com/doc/guide/object-floors#rectanglearena](https://www.cyberbotics.com/doc/guide/object-floors#rectanglearena) It is simple to set, and more robust on long term.


`@Gautier` Ok, the axis seems indeed to be correct. So the next step is to dig into the Connector fields.


`@Gautier` About the supervisor instantiation, why not directly set the translation/rotation when calling the import function? It seems simpler and more robust. (Do you understand what I'm speaking about?)

##### nap 12/10/2019 10:25:13
`@Fabien Rohrer` : Yes, I'm using a RectangleArena (and its default proto) with FloorAppearance and WallAppearance stanzas that have the PBR code.


`@Stefania Pedrazzi` :  I've cloned the repo, run the simple http server and browsed the url you gave.  However, I'm not getting anything useful.  It appears to me that I need to build the documentation using something like Doxygen, first?

##### Stefania Pedrazzi [Cyberbotics] 12/10/2019 10:29:38
`@nap` if you run the simple http server from a Webots installation folder (installed from any package) it should work out of the box. From the Git repo you first need to run `python local_exporter.py` in the `<WEBOTS>/docs` folder

##### nap 12/10/2019 10:47:11
`@Fabien Rohrer` : For a search function, you may wish to look at how Doxygen does it when it builds a html based site.

##### Fabien Rohrer [Moderator] 12/10/2019 10:50:46
`@nap` thank you for the advise. The Webots documentation is a "hand-made" system, based on GitHub and Markdown. Inspiring a search system on the Doxygen one is definitively a good idea.

##### Gautier 12/10/2019 12:16:44
`@Fabien Rohrer`  Thank you a lot ! It works now ! It was indeed a problem because the object was instanciated through the supervisor at the "orign point", which might have posed problems with ODE

##### Hayden Woodger 12/10/2019 12:45:46
`@Fabien Rohrer` Thanks, i've just seen your reply. Much appreciated.

##### Uncle Ducky 12/10/2019 14:14:48
hey guys, I have a differential robot but the camera is facing the rear. How do I make it face the front? Thanks, peace

##### Fabien Rohrer [Moderator] 12/10/2019 14:15:24
`@Uncle Ducky` Hi.

##### Uncle Ducky 12/10/2019 14:15:40
hi

##### Fabien Rohrer [Moderator] 12/10/2019 14:16:16
Just select the Camera node in the scene tree, a rotational gizmo should appear in the 3D view. You could rotate the camera using this gizmo.


minus z-axis is the direction of the camera, the y-axis should point to the top.

##### Uncle Ducky 12/10/2019 14:17:25
Ok thank you!

##### Fabien Rohrer [Moderator] 12/10/2019 14:25:44
You're welcome 😉

##### chamandana 12/10/2019 15:34:11
Can anyone give me any tips to reduce memory usage.
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/653982805100986388/unknown.png)
%end


I wondered why webots crashing after few seconds of simulation execution. It was actually reaching 100% memory usage


I know it should be my fault, but is there in Webots or the API that can be used to collect garbage

##### Uncle Ducky 12/10/2019 16:21:32
Does anyone know a sample world where a robot will move towards a specified object once it has been recognised?

##### Fabien Rohrer [Moderator] 12/10/2019 16:24:55
`@chamandana` It's more a programming issue than Webots issue. No Webots is not providing such tool, but your programming language does. You should debug it.

##### David Mansolino [Cyberbotics] 12/10/2019 16:25:30
Hi `@Uncle Ducky` Webots doesn't provide such example, however you might either use Radar ([https://cyberbotics.com/doc/reference/radar?tab-language=c](https://cyberbotics.com/doc/reference/radar?tab-language=c)) or smart camera ([https://cyberbotics.com/doc/reference/camera?tab-language=c#wb\_camera\_recognition\_get\_objects](https://cyberbotics.com/doc/reference/camera?tab-language=c#wb_camera_recognition_get_objects)) to detect position of objects relative to the robot and them move toward it.

##### lgrobots 12/10/2019 16:28:53
Hi I need to download webots on a new computer and it seems like your download page is giving a 404 error

##### Fabien Rohrer [Moderator] 12/10/2019 16:30:50
yes, we recently migrate to this page:


[https://github.com/cyberbotics/webots/releases](https://github.com/cyberbotics/webots/releases)

##### lgrobots 12/10/2019 16:31:26
perfect, thank you for your help

##### Uncle Ducky 12/10/2019 16:45:09
Ok thanks David

##### David Mansolino [Cyberbotics] 12/10/2019 16:45:58
You're welcome

##### chamandana 12/10/2019 17:35:16
`@Fabien Rohrer` hey, I'm using I/O (a file) to simulate data transfer between two controllers. Might that be the reason of excessive RAM usage


shouldn't it only be HDD usage?

##### Fabien Rohrer [Moderator] 12/10/2019 18:36:13
Difficult to tell without looking at your code.. why not using the Emitter and Receiver nodes to communicate between your controllers?

##### chamandana 12/10/2019 18:37:10
Ok i'm gonna try to implement emmitter receiver again. thanks


hey, how does the moving of kuka youbot's gripper work? It moves even after I set the arm\_ik(x,y,z)


as I've asked before when using the receiver it always says invalid device tag


😦

##### Fabien Rohrer [Moderator] 12/10/2019 20:08:31
About the arm, you should inspire yourself from the youbot demo controller.


About the invalid device tag, this generally occurs when the device name does not match between the model and the controller.

##### chamandana 12/10/2019 20:10:05

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/654052237215137794/unknown.png)
%end



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/654052251371044894/unknown.png)
%end

##### Fabien Rohrer [Moderator] 12/10/2019 20:10:37
This should work.

##### chamandana 12/10/2019 20:10:38
yet it matches 😥

##### Fabien Rohrer [Moderator] 12/10/2019 20:12:17
Are you sure the invalid device tag comes from this call? Are you sure that the right controller is running?

##### chamandana 12/10/2019 20:12:35

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/654052869737152542/unknown.png)
%end


I'm working on the same controller

##### Fabien Rohrer [Moderator] 12/10/2019 20:13:27
(This is certainly a stupid mistake, This mechanism cannot be buggy in Webots, every controller uses this!)

##### chamandana 12/10/2019 20:13:44
yeah It should be


Can you remind me of any of those stupid things that could go wrong

##### Fabien Rohrer [Moderator] 12/10/2019 20:14:33
The error appears on the wb\_receiver\_get\_data() function. Could you show this part of your code?

##### chamandana 12/10/2019 20:14:52
wait a sec



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/654053701161320448/unknown.png)
%end


this is the emitter code
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/654053765447417868/unknown.png)
%end

##### Fabien Rohrer [Moderator] 12/10/2019 20:16:40
Are you sure you don’t override somehow the receiver variable?

##### chamandana 12/10/2019 20:17:05
yeah, I haven't used receiver for anything else

##### Fabien Rohrer [Moderator] 12/10/2019 20:17:33
Please send your entire controller 😅 you can drag/drop it here.

##### chamandana 12/10/2019 20:18:06

> **Attachment**: [kbotR\_controller.c](https://cdn.discordapp.com/attachments/565154703139405824/654054258324406274/kbotR_controller.c)



> **Attachment**: [kinect\_controller.c](https://cdn.discordapp.com/attachments/565154703139405824/654054303610175500/kinect_controller.c)


kbotR is the receiver


kinect is the emitter


please take a look if you have time. 🙂


this is the code that makes my RAM fry :(, can you also point out which things may do so.

##### Fabien Rohrer [Moderator] 12/10/2019 20:26:13
Are you sure that the receiver device is get before it is used? (Sorry I read this on my phone 😅)

##### chamandana 12/10/2019 20:27:16
yeah. It's above the step loop so It gets initialized first thing


ooh, I just found out that the RAM usage is same as before (with and without using file I/O for Inter-process Communication). So, I could still use the file method. 🙂 but I should optimize it I think. I'm still learning C, don't know much about garbage collection.


thanks for replying anyway. 🙂

##### Fabien Rohrer [Moderator] 12/10/2019 20:32:27
There is no garbage collection in C, you have to explicitly free the memory. 😉

##### chamandana 12/10/2019 20:32:44
yeah yeah.

##### Fabien Rohrer [Moderator] 12/10/2019 20:33:05
I could test your controller, but only tomorrow. Come back here if you’re still stuck!

##### chamandana 12/10/2019 20:33:39
that's nice of you. thanks!

##### nap 12/11/2019 04:44:00
Hi, I have a small problem trying to compile my controller in a terminal using MacOS (Mojave).  I've exported the WEBOTS\_HOME variable, and when I run 'make', I get the following:

'/Applications/Webots/resources/Makefile.os.include:86: *** The Mac OS X SDK is not found. Please check that XCode and its command line tools are correctly installed..  Stop.'



Now, I definitely have Xcode installed and it's command line tools as I use them for other projects I'm doing at Uni.

Anyone know how to resolve this?


Running "xcode-select -p" gives "/Applications/Xcode10-2.app/Contents/Developer"


Since I have two versions of Xcode installed, my Xcodes are called 10-1.app (v10.1) and 10-2.app (v10.2)

looks like I need to fiddle Makefile.os.include in my situation.


hmmm, I'm now getting an odd error: 



make

\# compiling epuckMeanderLinesByGSM.cpp

c++: error: unrecognized command line option '-stdlib=libc++'                              <====

make: *** [build/release/epuckMeanderLinesByGSM.o] Error 1


`@Stefania Pedrazzi` : Hi Stefania, thanks for the local documentation tip yesterday.  However, the command you showed me only generated the Reference Manual.  Is there a command to generate the User Guide?

##### Stefania Pedrazzi [Cyberbotics] 12/11/2019 07:23:22
`@nap` you just have to change the URL

* http://localhost:8000/?book=reference&page=index for the Reference Manual

* http://localhost:8000/?book=guide&page=index for the User Guide


Here is the complete documentation: [https://github.com/cyberbotics/webots/blob/master/docs/README.md#run-the-doc-locally](https://github.com/cyberbotics/webots/blob/master/docs/README.md#run-the-doc-locally)

##### Fabien Rohrer [Moderator] 12/11/2019 07:35:47
`@nap` About your issues with the macOS SDK...


Webots is looking for the SDK with this command:


> xcrun --sdk macosx --show-sdk-path


Could you try to run it in a terminal?


About the command line tools, sometimes after an XCode update, it is required to just launch XCode to update them. Could you try this?


You can also trigger manually the command line tool update by typing the following line in the terminal:


> xcode-select --install

##### nap 12/11/2019 07:52:47
`@Fabien Rohrer` :

xcrun => /Applications/Xcode10-2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk



I use Xcode all the time here, so it is setup and working.

##### Fabien Rohrer [Moderator] 12/11/2019 07:54:12
Yes, I'm sure of this is setup correctly.


What version of Webots do you use?

##### nap 12/11/2019 07:55:42
I have R2019b revision 1

##### Fabien Rohrer [Moderator] 12/11/2019 07:58:18
The failure comes from this line: [https://github.com/cyberbotics/webots/blob/revision/resources/Makefile.os.include#L75](https://github.com/cyberbotics/webots/blob/revision/resources/Makefile.os.include#L75)


It's the exact  same `xcrun`  command I asked you to run.


So it means that Webots runs in an environment where xcrun cannot be executed smoothly.

##### nap 12/11/2019 07:59:32
`@Fabien Rohrer` :  I've already looked there and commented out the next 3 lines which give me the error.


I did do the export /Applications/Webots.app

##### Fabien Rohrer [Moderator] 12/11/2019 08:00:06
You should rather replace them like this:


> MACOSX\_SDK\_PATH = /Applications/Xcode10-2.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk


(replace lines 75 to 78)

##### nap 12/11/2019 08:02:50
`@Fabien Rohrer` : I'm sorry but I have to step away for about an hour.  Will be back though.

##### Fabien Rohrer [Moderator] 12/11/2019 08:03:06
no problems, see you later.


I'll be there.


😄

##### chamandana 12/11/2019 08:53:58
Hey `@Fabien Rohrer`. couldn't figure out a way to reduce the memory usage. It increases 30mbps
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/654244473576095744/unknown.png)
%end


😦

##### Fabien Rohrer [Moderator] 12/11/2019 08:58:21
`@chamandana` Hi


Good news first, I found the issue with your receiver 🙂

##### chamandana 12/11/2019 09:00:14
what is it? 😋

##### Fabien Rohrer [Moderator] 12/11/2019 09:02:12
It is required to call wb\_receiver\_get\_queue\_length() before wb\_receiver\_get\_data(), but the warning in case this is not fulfilled is cryptic: invalid device tag.


So you should modify your controller like this:


> while (wb\_receiver\_get\_queue\_length(receiver) > 0) {

>     const char *buffer = wb\_receiver\_get\_data(receiver);

>     // Do something with the buffer.

>   }


This is a cryptic message from our API, I will solve this in the next release:


[https://github.com/cyberbotics/webots/pull/1177](https://github.com/cyberbotics/webots/pull/1177)

##### chamandana 12/11/2019 09:04:00
oh, Thanks you for looking into it !!! 😘😘

##### Fabien Rohrer [Moderator] 12/11/2019 09:07:26
This may cause the memory leak too. But I would recommend you to switch to Webots R2020a as soon as we release, because we improved the memory usage of Webots (the "develop" nightly builds are already solving this).

##### chamandana 12/11/2019 09:09:13
okay will do.


but why does kinect.exe, kbotr.exe, and kbotb.exe not take that much RAM
%figure
![as.gif](https://cdn.discordapp.com/attachments/565154703139405824/654249935163752449/as.gif)
%end


gif is fast forwarded

##### Fabien Rohrer [Moderator] 12/11/2019 09:16:49
I think you should switch to R2020 😉

##### chamandana 12/11/2019 09:17:08
👍

##### nap 12/11/2019 09:39:25
`@Fabien Rohrer` : lol, it's still giving the same error after I replaced the assignment with  a static path.

There is something wrong in line 76::   ifeq ($(wildcard $(MACOSX\_SDK\_PATH)),)

The  MACOSX\_SDK\_PATH variable has the correct path (including the extension).



I've had a quick check to see what 'wildcard' does, but haven't found anything yet.  So I have no idea what its purpose is,


`@Fabien Rohrer` : BTW, I like the way the 'pinned' messages have been utilised.  Good feedback.

##### Fabien Rohrer [Moderator] 12/11/2019 09:45:54
`@nap` The wildcard is simply a way to check that the file exists in the Makefile syntax.

##### nap 12/11/2019 09:46:46
Oh, but the way the line is coded, it's being interpreted as a bash command.


But I have another question at the moment, because it seems I can build inside Webots itself.  So, whilst I really need to be able to build from the terminal later, I am trying to get my old E-Puck Controller going first.

##### Fabien Rohrer [Moderator] 12/11/2019 09:48:39
yes, the `xcrun` is executed by bash, it returns a path, and it's existence is checked by the wildcard Makefile function. This should work, you're the first user who is annoyed by this line.


Ok, please ask your questions 😉

##### nap 12/11/2019 09:52:06
There is a error in 76.  The "$(" in front of the *wildcard* command, and its trailing parenthesis, need to be removed.

I can post it as an issue on github if you like.

##### Fabien Rohrer [Moderator] 12/11/2019 09:54:23
you're welcome to post an issue, or even better, to create a pull request 😉 => however I think this is correct.

##### nap 12/11/2019 09:55:04
(My Q)

In W7 (using C++), I was able to include:

  #include <webots/Robot.hpp>

  #include <webots/DifferentialWheels.hpp>

in my controller and then be able to use the various sensors.

Now I'm getting an error:

  *epuckMeanderLinesByGSM\_Behaviour.cpp:121:14: error: member access into incomplete type 'webots::DistanceSensor'*

So it seems that I need to change which headers I include.

I'm wondering if this information can be found anywhere?  Something that summarises how things have changed?


`@Fabien Rohrer` :  lol.  With them removed, I've moved on to another error in the Makefile, but this is not something like the above.

##### Fabien Rohrer [Moderator] 12/11/2019 09:56:49
Are you sure that "#include <webots/DifferentialWheels.hpp>" is included in "epuckMeanderLinesByGSM\_Behaviour.cpp"? This is a typical build error occuring when the class definition is not found.


MACOSX\_SDK\_PATH should be defined correctly, otherwise it is for sure not working later...

##### nap 12/11/2019 10:02:41
`@Fabien Rohrer` : Now it's working with the original code, using xcrun to retrieve the path.  That is really strange!!


Yes, both of those includes are in the cpp file.


`@Fabien Rohrer` : Yes, both includes are in the cpp file.


`@Fabien Rohrer` : As a reference, the code is in my repository on github @ [https://github.com/Napoleon-BlownApart/IdempotentPlanningProtocol/tree/master/webots/controllers/epuckMeanderLinesByGSM](https://github.com/Napoleon-BlownApart/IdempotentPlanningProtocol/tree/master/webots/controllers/epuckMeanderLinesByGSM)


`@Fabien Rohrer` : Ok, I added: #include "<webots/DistanceSensor.hpp>", and it's built.  So I'm wondering if there is some sort of migration guide as I have other more complicated projects.

##### Fabien Rohrer [Moderator] 12/11/2019 10:14:49
Oh I undestand what's going on.

##### nap 12/11/2019 10:15:18
`@Fabien Rohrer` : So I ran the controller, but unfortunately I'm now getting another error during the simulation run:



[epuck-meander] Error: ignoring illegal call to wb\_differential\_wheels\_set\_speed() in a 'Robot' controller.

[epuck-meander] Error: this function can only be used in a 'DifferentialWheels' controller.

[epuck-meander] Error: ignoring illegal call to wb\_differential\_wheels\_set\_speed() in a 'Robot' controller.

[epuck-meander] Error: this function can only be used in a 'DifferentialWheels' controller.

##### Fabien Rohrer [Moderator] 12/11/2019 10:15:29
We deprecated DifferentialWheels node, it does not work as before.


Since R2018a

##### nap 12/11/2019 10:16:13
`@Fabien Rohrer` ic, thought something like that might be going on.  Are you across any documents that explain the changes?

##### Fabien Rohrer [Moderator] 12/11/2019 10:17:42
Here is the change log: [https://cyberbotics.com/doc/reference/changelog-r2018](https://cyberbotics.com/doc/reference/changelog-r2018)


>>> Deprecated the DifferentialWheels node. Please use the Robot node instead with two HingeJoint, RotationalMotor and PositionSensor nodes.

##### nap 12/11/2019 10:17:55
`@Fabien Rohrer` : There are a few things that have changed.

##### Fabien Rohrer [Moderator] 12/11/2019 10:18:32
Normally we have a very good backward compatibility, but for sure, time to time, we need to break things.


DifferentialWheel node has been dropped.


You need to use the Robot node instead, and 2 motorized HingeJoints


The e-puck proto model contains already this modification, but you need to modify your controller accordingly.

##### nap 12/11/2019 10:32:57
`@Fabien Rohrer` : Ok, I've found the DifferentialWheels material in the documentation.  Thanks.  I'll fix up my controller, and then I'll come back to the CLI building problem.

##### Fabien Rohrer [Moderator] 12/11/2019 10:33:46
That sounds good, good luck with this 👍🏻

##### nap 12/11/2019 10:37:58
Would you know of any sample C++ code for an e-puck?  All the examples in Webots seem to be either in C or use bsg files.

##### Fabien Rohrer [Moderator] 12/11/2019 10:39:12
Not in my knowledge, the reason is that the e-puck is very related to C (originally at least), because to be able to cross-compile code on it.

##### nap 12/11/2019 10:40:04
Ic.  I did notice a number of items relating to cross-compiling.


But, I might be lucky...  I found some 'botstudio' thing for the e-puck.  It's in C++.  Could you shed some light on what this does?  Its located at:

/Applications/Webots.app/projects/robots/gctronic/e-puck/plugins/robot\_windows/botstudio/e-puck

##### Fabien Rohrer [Moderator] 12/11/2019 10:43:52
This is a complex robot window, moreover it's deprecated. This is not a good example for you 😦


You should rather take a look at other differential wheels robots, typically this example:


[https://github.com/cyberbotics/webots/blob/revision/projects/languages/cpp/worlds/example.wbt](https://github.com/cyberbotics/webots/blob/revision/projects/languages/cpp/worlds/example.wbt)

##### nap 12/11/2019 10:54:34
My world .wbt file includes a reference to an e-puck proto as follows:



E-puck {

  translation -0.165 0 0.83

  controller "epuckwhiteboard"

  groundSensorsSlot [

    E-puckGroundSensors {

    }

  ]

}



Are you saying I should not use the e-puck in this way?


And that instead I should define the PROTO structure using the above example? (Which looks to me to have 3 (or 4 if you robots if you count the supervisor).

##### Fabien Rohrer [Moderator] 12/11/2019 11:02:46
This is the good way to include the e-puck.

##### chamandana 12/11/2019 11:04:38
`@Fabien Rohrer` so, I've downloaded Webots 2020a, but the RAM usage is still going up


😦


`@Fabien Rohrer` I've run my project without any code (controllers) and the RAM usage still increases. Can you take a look at it please
> **Attachment**: [final.zip](https://cdn.discordapp.com/attachments/565154703139405824/654278552476319784/final.zip)

##### Fabien Rohrer [Moderator] 12/11/2019 11:13:10
`@chamandana` I just run some test on projects/samples/devices/worlds/emitter\_receiver.wbt. When running the example in real time, there is indeed a small memory leak of about 100k every 4 seconds. Could you confirm you have similar results?


but it stabilized after a while...


Is it worst in your simulation?

##### nap 12/11/2019 11:14:24
`@Fabien Rohrer` : What about the graphic representation?

##### Fabien Rohrer [Moderator] 12/11/2019 11:15:03
`@nap` Sorry, but I don't understnad. Could you clarify your question?

##### chamandana 12/11/2019 11:15:44
`@Fabien Rohrer` In the above example (original version) i've sent there is no code running. It's just the world. Can you examine it and tell me what might be wrong please. thanks

##### nap 12/11/2019 11:16:22
And what do I select from within Webots when I use the "Add New -> PROTO nodes (Webots Projects)"?


Sorry, I want to be able to see an e-puck robot in my simulation, without needing to create my own skins, etc.

##### Fabien Rohrer [Moderator] 12/11/2019 11:17:58
`@nap` This seems correct, just select the E-puck.proto from the "Add new node" dialog (as you suggest).

##### nap 12/11/2019 11:18:27
`@Fabien Rohrer` : Ok, that's what I've done.


and that's what's generated the above XML that is in my .wbt file.

##### Fabien Rohrer [Moderator] 12/11/2019 11:22:03
`@chamandana`  I can indeed reproduce a huge memory request at the begining of your simulation, in Webots. ~100Mo per second. But it stabilized after a while at 3.0Go (which is big).


I will take a closer look at this.


`@nap` This seems to be correct: what disturbs you?

##### chamandana 12/11/2019 11:37:38
`@Fabien Rohrer` okay 🙂

##### nap 12/11/2019 11:48:40
`@Fabien Rohrer` : I'm having a little difficulty understanding how to transition from DifferentialWheels to the other classes;  two HingeJoint, RotationalMotor and PositionSensor nodes, but whilst still using the GCTronic e-puck.


My code builds, but nothing happens in the simulator, and once I enabled the LED and LightSensors on the robot, the controller crashes. 😦

##### David Mansolino [Cyberbotics] 12/11/2019 11:56:17
`@nap`, you should probably start from one of the controller provided within Webots and then change it step by step to integrate your controller inside.

##### nap 12/11/2019 11:58:55
`@David Mansolino` : unfortunately they are all in C, but I'll take that approach as I recognised the naming convention.  I just need to work out the class hierarchy.

##### David Mansolino [Cyberbotics] 12/11/2019 12:00:45
In that cass, I strongly recommend to check this tutorial: [https://cyberbotics.com/doc/guide/tutorial-4-more-about-controllers](https://cyberbotics.com/doc/guide/tutorial-4-more-about-controllers)

This tutorial shows how to control the e-puck robot in any language.

##### nap 12/11/2019 12:01:06
`@David Mansolino` oh, nice.  Thanks!

##### David Mansolino [Cyberbotics] 12/11/2019 12:01:11
You're welcome

##### nap 12/11/2019 12:01:57
looks perfect, and familiar too (from Webots6 & 7)

##### juanrh 12/11/2019 13:27:47
Hi, I'm trying to launch the python example [https://github.com/cyberbotics/webots/tree/revision/projects/languages/python](https://github.com/cyberbotics/webots/tree/revision/projects/languages/python) but the keyboard is ignored. I added `print("{} - Detected key [{}]".format(datetime.now(), k))` to see which key is detected, and it prints `-1` all the time, even when no key is pressed


I'm on Fedora 30 installed through snap. Any idea what is wrong here?

##### Fabien Rohrer [Moderator] 12/11/2019 13:29:05
Hi, do you select the robot in the 3D window before trying to use the keyboard?

##### juanrh 12/11/2019 13:29:38
no, let me try that


yeah, it was that


thanks a lot Fabien!

##### Fabien Rohrer [Moderator] 12/11/2019 13:31:37
you're welcome 😉


the 3d window should be selected to deal with the keyboard events. In this case the keyboard events are sent to every controllers. If only one robot is selected, then the keyboard events are only sent to the selected robot.

##### nap 12/11/2019 14:04:13
`@Fabien Rohrer` : The old Webots 'run()' method is gone now, and the main() method is used directly instead of a sub-class.  So the controller is a standalone executable that links to webots.

Is this a correct interpretation?

##### Fabien Rohrer [Moderator] 12/11/2019 14:35:56
Mmmm. The controller is indeed a standalone executable linked with a shared library (libController) dialoging with Webots. So the main() method is used directly. But it has been always the case, and Webots never had a `run()` method.

##### nap 12/11/2019 14:37:26
Cheers.  All our controllers have used a run() method, but I just followed what some others had done.

##### Fabien Rohrer [Moderator] 12/11/2019 14:38:03
I cannot explain this, it's not related with Webots.


I don't know how your controllers were working before. But a main() was always required.

##### aysegulucar 12/11/2019 14:59:02
Hi, when my controller transferring  to real robotis-op2 , I received the error "Unable to establish connection with the robot.

Connection error: server identity not verified". However it works at visual taking, that it is connected.


why

##### Fabien Rohrer [Moderator] 12/11/2019 15:02:55
`@chamandana` I found and fix the memory leak issue that I can reproduce from your simulation: [https://github.com/cyberbotics/webots/pull/1181](https://github.com/cyberbotics/webots/pull/1181)


Please download the next release (this month) or the next `develop` nightly build (tomorrow) to test my patch: a feedback would be welcome.

##### Mope 12/11/2019 15:09:54
Hi everyone, I am currently working on a robot. I would like to use steps to command the robot. How do I implement this into my code? I am using C. Thank you

##### Fabien Rohrer [Moderator] 12/11/2019 15:11:14
`@Mope` Hi, I recommend you to start to do our tutorial; this will answer a lot of your questions, and come back here when it's done or if you're stuck 😉 [https://cyberbotics.com/doc/guide/tutorials](https://cyberbotics.com/doc/guide/tutorials)


`@aysegulucar` It looks like you cannot connect to the robot... Difficult to tell more.

##### Mope 12/11/2019 15:14:08
I've done the tutorials. I do not find anything that relates to using steps. For example, I would like it to do a specific task but if something happens, do something else. Any help would be appreciated!

##### Fabien Rohrer [Moderator] 12/11/2019 15:14:29
Ok, great.


Let me check, we have documentation for this...


This page, and in particular this section explains this in details: [https://cyberbotics.com/doc/guide/controller-programming#the-step-and-wb\_robot\_step-functions](https://cyberbotics.com/doc/guide/controller-programming#the-step-and-wb_robot_step-functions)

##### Mope 12/11/2019 15:17:03
Thank you, I am reading through it now.

##### Fabien Rohrer [Moderator] 12/11/2019 15:17:56
But very basically, when you call the wb\_robot\_step(period), the controller is blocked for the "period" time. The commands are sent to Webots. Then one or several physics steps can be executed in the meantime, for a "period" duration. And finally the sensor feedback is sent to the controller just before continuing its execution.


All of this is in "simulated time", it's the virtual time of the simulation (which can be slower or quicker than the real true time).

##### Mope 12/11/2019 15:20:06
Awesome, appreciate the help! I'm gonna try and get it working now!

##### Fabien Rohrer [Moderator] 12/11/2019 15:20:21
You're welcome.

##### Mope 12/11/2019 15:33:41
`@Fabien Rohrer` Do you know where I can find some examples? I am struggling to fit this into my robot.

##### Fabien Rohrer [Moderator] 12/11/2019 15:34:39
We generally provide at least one example per robot and per device, it depends on your goals.


You should certainly browse these pages:


- [https://cyberbotics.com/doc/guide/robots](https://cyberbotics.com/doc/guide/robots)


- [https://cyberbotics.com/doc/guide/sample-webots-applications](https://cyberbotics.com/doc/guide/sample-webots-applications)


.. and pick what you would like from there.

##### webot123 12/11/2019 15:39:04
hi guys, i am trying to get my robot to respond to its GPS position in a world using an if command. what is the right code to us, i have tried          :

   wb\_gps\_get\_values(gps);

    if (gps\_value[0] < 1 && gps\_value[0] < -1){


but it doesnt seem to wokr

##### Fabien Rohrer [Moderator] 12/11/2019 15:45:59
`@webot123` indeed, it seems far to work 😉 your if condition is triggered when the absolute x-axis of the position is below minus one meter. I don't know what you would like to do with this..

##### webot123 12/11/2019 15:46:55
i am getting this warning when i build the code  GPS2.c:385:9: error: 'gps\_value' undeclared (first use in this function); did you mean 'ps0\_value'?

  385 |     if (gps\_value[0] < 1 && gps\_value[2] < 1){

##### Fabien Rohrer [Moderator] 12/11/2019 15:49:45
this means that `gps_value` is not declared. It's a C programming issue.

##### webot123 12/11/2019 15:50:24
ok, thanks... what line of code do you recommend to declare that?

##### Fabien Rohrer [Moderator] 12/11/2019 15:53:39
typically: "const double *gps\_value = wb\_gps\_get\_values(my\_well\_declared\_gps);" just above your line.

##### webot123 12/11/2019 15:56:19
thats great thanks for your help

##### Mope 12/11/2019 19:42:07
Hi, is anyone available to provide me with some basic functions?


(c language)


me with assistance*

##### Tahir [Moderator] 12/11/2019 19:46:36
`@Mope` Hello

Didn't got your query which type of assistance do you want?

##### Mope 12/11/2019 19:59:06
I would like to know how to use the step function correctly


`@Tahir`


Please can anyone help me with steps?

##### Tahir [Moderator] 12/11/2019 20:25:41
Webots uses two different time steps:



The simulation step (specified in the Scene Tree: WorldInfo.basicTimeStep). It actually is the time which calculates the simulation step. i.e. physics if used. means it adavqances the simulation. On the other hand the control step (specified as an argument of the wb\_robot\_step function for each robot) is the time which controls the control loop it take cares of sensors and actuators


[https://cyberbotics.com/doc/guide/controller-programming#the-step-and-wb\_robot\_step-functions](https://cyberbotics.com/doc/guide/controller-programming#the-step-and-wb_robot_step-functions)


[https://www.cyberbotics.com/doc/reference/worldinfo](https://www.cyberbotics.com/doc/reference/worldinfo)

##### Mope 12/11/2019 21:57:01
[https://tenor.com/view/sad-blackish-anthony-anderson-tears-upset-gif-4988274](https://tenor.com/view/sad-blackish-anthony-anderson-tears-upset-gif-4988274)

##### nap 12/12/2019 05:54:14
I'm having a little difficulty getting the E-Puck camera to work when using a C++ based controller.

I have included the library and defined a pointer as "Camera* myCam"

Later, when I initialise it using:

myCam = this->getCamera("Camera");

(Where 'this' is a subclass of Robot)


I get an error "Warning: "Camera" device not found."


If I try to enable the cam using "myCam->enable(2 * TIME\_STEP);

my controller crashes.


The documentation about the E-Puck gives extensive details about how the camera works and what methods are available for use.  However, there is no code that shows how to actually use it.

Can anyone suggest a link to the right place in the documentation?


Please, and thanks.

##### Nocturnal Warfare 12/12/2019 06:02:15
I am trying to go through the tutorials and I'm running into an issue with running my custom controller for the epuck, this is the error the console pops out:

```INFO: e-puck_go_forward: Starting controller: python.exe -u "e-puck_go_forward.py" 
[e-puck_go_forward] Traceback (most recent call last):
[e-puck_go_forward]   File "e-puck_go_forward.py", line 3, in <module>
[e-puck_go_forward]     from controller import Robot, Motor, Camera
[e-puck_go_forward]   File "C:\Program Files\Webots\lib\python37\controller.py", line 15, in <module>
[e-puck_go_forward]     import _controller
[e-puck_go_forward] ValueError: source code string cannot contain null bytes
WARNING: 'e-puck\_go\_forward' controller exited with status: 1.
```

Any ideas as to what is causing "ValueError: source code string cannot contain null bytes"

##### nap 12/12/2019 06:04:41
What are lines 3 and 15?


are you sure there should be an underscore in front of \_controller?

##### Nocturnal Warfare 12/12/2019 06:07:23
line 3 is just the import in the controller:

`from controller import Robot, Motor, Camera`

line 15 is from the Python API files, so line 15 of the controller.py imports \_controller:

```# Import the low-level C/C++ module
if __package__ or "." in __name__:
    from . import _controller
else:
    import \_controller
```


im guessing that the \_controller is the C controller module

##### nap 12/12/2019 06:10:29
Yes, doing C bindings.  I don't program much in Python, but from the 'null bytes' message, maybe the C controller isn't being found?  Have you set the necessary environment variables?

##### Nocturnal Warfare 12/12/2019 06:11:09
no, what environment variables do i have to set?

##### nap 12/12/2019 06:13:59
Usually you should export the path to webots.

[https://cyberbotics.com/doc/guide/compiling-controllers-in-a-terminal](https://cyberbotics.com/doc/guide/compiling-controllers-in-a-terminal)

##### Nocturnal Warfare 12/12/2019 06:20:43
im going to reinstall webots and see if that fixes anything

##### nap 12/12/2019 06:21:01
have fun 😉

##### Nocturnal Warfare 12/12/2019 06:21:22
lol it seems to have fixed it

##### nap 12/12/2019 06:21:32
nice

##### Nocturnal Warfare 12/12/2019 06:21:39
it looks like i was missing \_controller.pyd and \_vehicle.pyd files

##### nap 12/12/2019 06:23:53
Can someone recommend a C++ example that uses an Epuck with its camera please?

##### Nocturnal Warfare 12/12/2019 06:24:56
have u checked out the guided tour for the camera?


Help --> Webots Guided Tour --> Devices Tour --> camera.wbt

##### nap 12/12/2019 06:33:28
Thanks.  I had a look, but it's not an E-Puck (whose PROTO is very different).

The Robot in that example has a Camera Node, but the E-Puck doesn't (but I'm going to check the default proto to see if it is there).

##### Nocturnal Warfare 12/12/2019 06:36:23
so im messing around with the motors now, when you make a robot object, and then try to get robot.getMotor('left motor') or whatever the name of the motor is on the robot, it doesnt seem to return as a motor object, at least visual studio code and pycharm are recognizing it as a motor. on the other hand, both of those editors recognize robot properly and have code filling for its respective functions, do you have any idea why that is? should i make a motor object on its own in the code?

##### nap 12/12/2019 06:36:41
Yes it does have a cam defined in its proto:  DEF EPUCK\_CAMERA Camera


try "left wheel motor"  and "right wheel motor"


and for the encoders, if you're using them, use "left wheel sensor", "right wheel sensor"

##### Nocturnal Warfare 12/12/2019 06:39:01
no i can manipulate the functions associated with them, just the code does not autofill when i put a '.' for instance, where when i do the same with the robot, it does


hey it works, like if i just do 
```leftMotor = Motor('left wheel motor')
rightMotor = Motor('right wheel motor')` instead of `#leftMotor = robot.getMotor('left wheel motor')
\#rightMotor = robot.getMotor('right wheel motor')
``` it works fine


have you had any luck setting up an external controller through something other than the built in editor like Visual Studio Code?

##### nap 12/12/2019 06:52:30
Success here too.  Needed "camera" instead of "Camera".  lol

##### Nocturnal Warfare 12/12/2019 06:52:39
nice

##### nap 12/12/2019 06:53:28
I'm currently using the built in editor because the the build fails from the command line.


I'm migrating my project from Webots7 to the current OS one.


Since there have been some substantial API changes, I decided that I would get my controller working first, then worry about CLI building.


In linux I use CodeBlocks for the controllers and was able to build directly from the C::B IDE.  On MacOS, which I'm on right now, unfortunately there is no working C::B available, so I normally use Xcode (which I hate).


I haven't used Windows for any coding in a while.

##### Nocturnal Warfare 12/12/2019 06:57:22
yeah im currently on windows, trying to mess with the environment variables in order to make VS code see the necessary modules

##### nap 12/12/2019 06:57:48
Yes, that aspect is much easier to manage in Linux/Macos.


brb

##### Nocturnal Warfare 12/12/2019 07:01:53
this is so annoying, it saying it cant find \_controller.py now, even though it literally in the same directory of controller.py, which is what is calling for the import of \_controller



%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/654578665182724096/unknown.png)
%end


anyone have any ideas why it cant find \_controller but has no issues finding controller since both are in the same folder?

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 07:15:24
`@Nocturnal Warfare` are you using Webots R2010b revision 1?

##### Nocturnal Warfare 12/12/2019 07:16:13
2019b? yes i just did a fresh install which fixed my previous issue of that \_controller.pyd and \_vehicle.pyd not even existing which cause the controller to not run at all

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 07:16:34
yes, sorry, R2019b . And on which platform? Windows?

##### Nocturnal Warfare 12/12/2019 07:16:40
yes

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 07:17:18
ok, this is probably due to a bug that we fixed recently: [https://github.com/cyberbotics/webots/pull/1131](https://github.com/cyberbotics/webots/pull/1131)

##### Nocturnal Warfare 12/12/2019 07:18:21
I am simply trying to do external controller creation in VS Code that allows me to start simulation from the editor


when will that fixed be pushed to the live release?

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 07:19:05
yes, but there are some libraries missing in the Windows package. You should try to use the nightly build of R2020a


[https://github.com/cyberbotics/webots/releases/tag/nightly\_11\_12\_2019](https://github.com/cyberbotics/webots/releases/tag/nightly_11_12_2019)


We are about to release it (probably next week)

##### Nocturnal Warfare 12/12/2019 07:19:24
sweet thank you ill give that a try


do i have to uninstall 2019b first?

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 07:20:16
Yes, but the Windows installer should do it automatically.

##### Nocturnal Warfare 12/12/2019 07:21:54
is there a way to dynamically get the webots install directory, for the purposes of environment variables and transferring between computers?

##### nap 12/12/2019 07:22:13
`@Stefania Pedrazzi` hello.

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 07:27:37
`@Nocturnal Warfare` what do you mean exactly?


`@nap` Hi

##### Nocturnal Warfare 12/12/2019 07:30:08
like to get my controller to run externally i have to create PYTHONPATH variables that tell the program to look in a certain directory shown below:

`PYTHONPATH = C:\Program Files\Webots\lib\controller\python37; C:\Program Files\Webots\msys64\mingw64\bin; C:\Program Files\Webots\lib\python37;`

But I was wondering if there is a more generic way to do that if I want to run this code on multiple computers where that path isnt necessarily valid, so I was wondering if there is any way to get the Webots Directory dynamically versus having it hard coded


BTW its still not finding the \_controller file, failing with the same error

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 07:35:14
The controller need the Webots libraries to run so you should link to them on all the machines. You can set other environmental variables on your system to make it more generic (for example using a variable fot the Webots installation directory) but other than this I don't see any other things that could not be hard coded

##### Nocturnal Warfare 12/12/2019 07:35:41
ok i figured as much thank you anyway


`C:\Program Files\Webots\lib\python37` The first has both controller.py, vehicle.py and \_controller.pyd, vehicle.pyd while the latter only has the \_controller.py and vehicle.py

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 07:41:41
Ok, thank you. I will check but this seems an error of the installer

##### Nocturnal Warfare 12/12/2019 07:41:55
should all 6 be in both locations?


or just in the lib\controller\python37?

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 07:42:11
the Webots\lib\python* folder should not exist

##### Nocturnal Warfare 12/12/2019 07:42:17
oh ok

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 07:42:59
could you please try to manually uninstall Webots before installing R2020a?

##### Nocturnal Warfare 12/12/2019 07:43:34
i will try reinstalling again and make sure to delete the folder before install of 2020a


yeah it look like it was a leftover from the previous install, new install only has the controller folder,

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 07:46:50
just for info did you touch any file in the `<webots>\lib` folder?

##### Nocturnal Warfare 12/12/2019 07:47:11
no

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 07:47:17
ok, thank you

##### Nocturnal Warfare 12/12/2019 07:48:19
i mean what do you mean by touch? like modify? because i have been adding that folder to the VS code workspace and idk exactly what that does


weird it still cannot find that \_controller, the lib folder only has controller.py, vehicle.py and \_controller.pyd, vehicle.pyd fyi. I assume the \_controller its looking for is a .py not a .pyd correct?

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 07:50:14
I'm trying to understand why the uninstaller didn't work correctly, and sometimes the directories are not deleted if the files have beed modified

##### Nocturnal Warfare 12/12/2019 07:50:41
I think it notified me that some files were not deleted but i ignored it. sorry 🙂

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 07:53:04
In the `<WEBOTS>/lib/controller` directory, could you check that you have libController.dll or Controller.dll, Controller.lib, libController.a, libCppController.dll, CppController.lib, libCppController.a

##### Nocturnal Warfare 12/12/2019 07:53:55

%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/654591752367374352/unknown.png)
%end


it appears to be all there

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 07:54:44
ok, good. that's seems correct

##### Nocturnal Warfare 12/12/2019 07:55:07
libCppController.dll seems to be missing actually

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 07:55:50
no it's CppController.dll (I confused with the linux library name)

##### Nocturnal Warfare 12/12/2019 07:55:58
oh ok


CppController.lib is also missing

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 07:57:32
another mistake from my side. I checked and you have all the required libraries

##### Nocturnal Warfare 12/12/2019 07:57:42
ok sweet


and am i right in thinking that if VS Code can see controller.py, and import it properly, it should be able to see \_controller and import that as well?

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 07:58:59
> weird it still cannot find that \_controller, the lib folder only has controller.py, vehicle.py and \_controller.pyd, vehicle.pyd fyi. I assume the \_controller its looking for is a .py not a .pyd correct

the <WEBOTS>/lib/controller/python37/ directory should also contain the \_controller.pyd file

##### Nocturnal Warfare 12/12/2019 07:59:46
yes it has that


is that the file that is referenced by the import \_controller?

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 08:02:12
If I'm not wrong the the .pyd takes preference over the .py

##### Nocturnal Warfare 12/12/2019 08:02:33
am i right in thinking that if VS Code can see controller.py, and import it properly, it should be able to see \_controller and import that as well?

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 08:03:42
theoretically this makes sense, but I'm not sure

##### Nocturnal Warfare 12/12/2019 08:03:54
also is that \_controller.pyd supposed to be a text file? because neither VS Code nor notepad++ can read it

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 08:04:52
no, it contains bytes and not text

##### Nocturnal Warfare 12/12/2019 08:05:00
oh ok

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 08:08:19
does the import work correctly if you run python from a terminal?

##### Nocturnal Warfare 12/12/2019 08:09:37
```PS F:\OneDrive\GitHub\Cjkeenan\NGCP_UGV_2019-2020\my_first_simulation> python
Python 3.7.5 (tags/v3.7.5:5c02a39a0b, Oct 15 2019, 00:11:34) [MSC v.1916 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
>>> import controller
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "C:\Program Files\Webots\lib\controller\python37\controller.py", line 15, in <module>
    import _controller
ImportError: DLL load failed: The specified module could not be found.
```


same error

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 08:10:24
did you define the PYTHONPATH before starting the python shell?

##### Nocturnal Warfare 12/12/2019 08:10:51
no how do i do that?

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 08:12:25
are using cmd terminal? this should work `SET PYTHONPATH=C:\Program Files\Webots\lib\controller\python37; C:\Program Files\Webots\msys64\mingw64\bin; `

##### Nocturnal Warfare 12/12/2019 08:13:36
```PS F:\OneDrive\GitHub\Cjkeenan\NGCP_UGV_2019-2020\my_first_simulation> SET PYTHONPATH=C:\Program Files\Webots\lib\controller\python37
PS F:\OneDrive\GitHub\Cjkeenan\NGCP_UGV_2019-2020\my_first_simulation> python
Python 3.7.5 (tags/v3.7.5:5c02a39a0b, Oct 15 2019, 00:11:34) [MSC v.1916 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
>>> import controller
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "C:\Program Files\Webots\lib\controller\python37\controller.py", line 15, in <module>
    import _controller
ImportError: DLL load failed: The specified module could not be found.
```


same issue it seems

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 08:16:04
ok, I will now test on my computer if it works and let you know

##### Nocturnal Warfare 12/12/2019 08:16:50
thank you so much, you have been very helpful

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 08:29:16
I could make it work on my system from CMD by also updating the PATH before running python: 

`set PATH=%PATH%;C:\Program Files\Webots\lib\controller;C:\Program Files\Webots\lib\controller\python37`

##### Nocturnal Warfare 12/12/2019 08:31:17
so you ran that in CMD, then python, then import controller?


and it worked?

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 08:31:48
yes, I set both the PATH and PYTHONPATH and then started python and the import was fixed


does it work for you as well?

##### Nocturnal Warfare 12/12/2019 08:32:08
im trying now


how do you set both python path and path?

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 08:32:58
running the two commands I wrote you

##### Nocturnal Warfare 12/12/2019 08:33:20
`set PATH=%PATH%;C:\Program Files\Webots\lib\controller;C:\Program Files\Webots\lib\controller\python37` this does both?

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 08:34:19
no this sets only the PATH, for the PYTHONPATH type:

```
set PYTHONPATH=C:\Program Files\Webots\lib\controller\python37
```

##### Nocturnal Warfare 12/12/2019 08:36:08
```C:\Users\Username>set PYTHONPATH=C:\Program Files\Webots\lib\controller\python37

C:\Users\Username>set PATH=%PATH%;C:\Program Files\Webots\lib\controller;C:\Program Files\Webots\lib\controller\python37

C:\Users\Username>python
Python 3.7.5 (tags/v3.7.5:5c02a39a0b, Oct 15 2019, 00:11:34) [MSC v.1916 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
>>> import controller
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "C:\Program Files\Webots\lib\controller\python37\controller.py", line 15, in <module>
    import _controller
ImportError: DLL load failed: The specified module could not be found.
```

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 08:37:33
and if you set the path to:

```
set PATH=C:\Program Files\Webots\lib\controller;C:\Program Files\Webots\lib\controller\python37;%PATH%
```


sorry I just edited the previous message

##### Nocturnal Warfare 12/12/2019 08:39:07
nope


is there a way to verify that the path is actually getting set?

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 08:39:50
yes, with `echo %PATH%`

##### Nocturnal Warfare 12/12/2019 08:40:54
yeah both of those new paths are in there

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 08:41:05
or from python 
```import os
print(os.getenv('PATH'))
```

##### Nocturnal Warfare 12/12/2019 08:42:17
both echo and python see both lib\controller and lib\controller\python37


import still failing


how do we make sure the pythonpath is working?


or is that what that python code checks?

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 08:44:05
the error you get is probably due to the PATH and not to PYTHONPATH


it could be that PATH contains other resources that are conflicting with the latest Webots version

##### Nocturnal Warfare 12/12/2019 08:46:06
i can type controller.py from the console, and it opens that file, so im guessing it is finding that path fine

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 08:47:00
but it doesn't find \_controller. on my system I got this error when PATH was not set

##### Nocturnal Warfare 12/12/2019 08:47:22
i wonder if we switch the import in the controller.py to specify the .pyd extension


like import \_controller.pyd

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 08:48:53
I doubt this is the issue, but you can try. I would also try to clean the PATH variable in the terminal and make it link only to the two Webots folders and to the python 37 folder

##### Nocturnal Warfare 12/12/2019 08:49:36
yeah adding the .pyd didnt do anything


how do you set the path variable versus just appending it?

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 08:54:16
if you type ``set PATH=`` you will completely clean it

if you type ``set PATH=C:\Program Files\Webots\lib\controller;C:\Program Files\Webots\lib\controller\python37;`` you will set it only to the Webots lib/controller folders

to append you have type something like this ``set PATH=<new path>;%PATH%``

##### Nocturnal Warfare 12/12/2019 08:54:36
got it thanks


crap, now it doesnt recognize python cuz that path is gone lol

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 08:55:56
yes, you need to add it to the PATH too

##### Nocturnal Warfare 12/12/2019 08:59:31
ok so i have the \lib\controller, \python37, \lib\controller\python37 in my path now


grrr... still failed


```C:\Users\Username>echo %PATH%
C:\Users\Username\AppData\Local\Programs\Python\Python37\; C:\Program Files\Webots\lib\controller; C:\Program Files\Webots\lib\controller\python37

C:\Users\Username>set PYTHONPATH=C:\Program Files\Webots\lib\controller\python37

C:\Users\Username>python
Python 3.7.5 (tags/v3.7.5:5c02a39a0b, Oct 15 2019, 00:11:34) [MSC v.1916 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
>>> import controller
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "C:\Program Files\Webots\lib\controller\python37\controller.py", line 15, in <module>
    import _controller
ImportError: DLL load failed: The specified module could not be found.
```


is it loading on your side fine?

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 09:03:05
yes, but I'm now checking on a different machine

##### Nocturnal Warfare 12/12/2019 09:03:43
when I was looking into integrating with PyCharm it said something about adding the mingw64\bin folder as an environment variable or something, is that something not necessary here?


my python installation couldnt be the issue correct since it runs fine in the Webots application right?

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 09:06:50
yes, you are right and you probably also have tp add C:\Program Files\Webots\msys64\mingw64\bin to the PATH as well.

##### Nocturnal Warfare 12/12/2019 09:07:04
ok


didnt help


`PYTHONPATH: C:\Program Files\Webots\lib\controller\python37`

`PATH: C:\Users\Username\AppData\Local\Programs\Python\Python37\; C:\Program Files\Webots\lib\controller; C:\Program Files\Webots\lib\controller\python37;C:\Program Files\Webots\msys64\mingw64\bin`


thats all correct right? cuz thats currently what they are


wtf it started working

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 09:12:10
yes, can you also try to set the WEBOTS\_HOME variable: ``set WEBOTS\_HOME=C:\Program Files\Webots``?

##### Nocturnal Warfare 12/12/2019 09:12:15
not from console, but from VS Code


........


```PYTHONPATH=C:\Program Files\Webots\lib\controller\python37
PATH=%PATH%;C:\Program Files\Webots\lib\controller;C:\Program Files\Webots\lib\controller\python37;C:\Program Files\Webots\msys64\mingw64\bin
``` is my .env that is loaded using the launch.json


WTF, now it works.... why doesnt the console work tho?

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 09:13:58
the json looks correct

##### Nocturnal Warfare 12/12/2019 09:14:22
```{
  // Use IntelliSense to learn about possible attributes.
  // Hover to view descriptions of existing attributes.
  // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Python: Current File",
      "type": "python",
      "request": "launch",
      "program": "${file}",
      "console": "integratedTerminal",
      "envFile": "${workspaceFolder}/.env"
    }
  ]
}

``` is the json


that other one was the .env file

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 09:14:47
no idea why it doesn't work form the console.

##### Nocturnal Warfare 12/12/2019 09:15:20
is there a way send simulation commands from the code? like pause, reset, etc?

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 09:16:03
Yes, using the Supervisor API: [https://www.cyberbotics.com/doc/reference/supervisor#wb\_supervisor\_simulation\_set\_mode](https://www.cyberbotics.com/doc/reference/supervisor#wb_supervisor_simulation_set_mode)

##### Nocturnal Warfare 12/12/2019 09:19:45
i think thats for another day lol, are there any plans to add version control or code autofill to the built in editor cuz those are the two main reasons i wanted to use VS Code


Thank you by the way, you help has been invaluable

##### JasonChen 12/12/2019 09:21:33
Hello, if I want to add more friction to my floor, how can I do that?

Right now, the robot slips all the time.

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 09:22:01
`@Nocturnal Warfare` you're welcome

no, for the moment we don't plan to improve the editor, even if we know that it is limited


`@JasonChen` the friction is defined in the WorldInfo.contactProperties field:

[https://www.cyberbotics.com/doc/reference/contactproperties](https://www.cyberbotics.com/doc/reference/contactproperties)

##### Nocturnal Warfare 12/12/2019 09:26:08
alright i think that is all for tonight, good night and thank you again, having this discord was a great idea btw

##### JasonChen 12/12/2019 09:44:43
`@Stefania Pedrazzi` Thanks.

##### Stefania Pedrazzi [Cyberbotics] 12/12/2019 09:45:00
you're welcome

##### Gautier 12/12/2019 10:52:51
Hello, I have a question on the execution scheme


How does an execution of a robots's controller/supervisor is inserted in the "global execution scheme" of a simulation ?


And does this execution scheme is gonna change between webots 2019b and 2020a ?


(I'm mainly interested on the wb\_supervisor\_node\_add\_force function that are gonna be available in Webots 2020, and I want to understand how it interacts with ODE and, for exemple, if they (ODE and the controller) are "executed and synchronized" at the same timestamp)

##### David Mansolino [Cyberbotics] 12/12/2019 10:58:00
Hi `@Gautier` yes they are synchronized, let me check for the exact order.

##### Gautier 12/12/2019 11:00:47
Hi `@David Mansolino` , thank you

##### David Mansolino [Cyberbotics] 12/12/2019 11:03:26
Ok it works in this order (to summarize), at each step:

  - the controller request is read (i.e. the force is applied).

  - the physic step is executed.

  - the new state of the sensors is sent back to the controllers.



Of course this is in the case where the controller and simulation steps are the same.

##### Gautier 12/12/2019 11:05:25
Ok, so the controller are executed before the physics (from ODE ?)


(In the case the controllers and simulation step are the same, of course)

##### David Mansolino [Cyberbotics] 12/12/2019 11:05:54
Yes.

##### Gautier 12/12/2019 11:07:36
Therefore, if understand correctly, the execution of the controller at time T is based on the data he retrieved from the timestamp T-1 during its execution at T ?


(Also, final question, is the execution scheme gonna change between webots 2019 and 2020 ?)

##### David Mansolino [Cyberbotics] 12/12/2019 11:09:32
> Also, final question, is the execution scheme gonna change between webots 2019 and 2020 ?



No



> Therefore, if understand correctly, the execution of the controller at time T is based on the data he retrieved from the timestamp T-1 during its execiton at T ?



Yes

##### Gautier 12/12/2019 11:10:27
Ok, thank you a lot for your responses. They are gonna be very usefull. 😄

##### David Mansolino [Cyberbotics] 12/12/2019 11:10:35
You're welcome

##### chamandana 12/12/2019 13:02:43
hey `@Fabien Rohrer`, When will the nightly build release that fixes this? 🙂 [https://github.com/cyberbotics/webots/pull/1181](https://github.com/cyberbotics/webots/pull/1181) .

##### Fabien Rohrer [Moderator] 12/12/2019 13:09:55
Sorry, you will have to wait until tomorrow 😉


We started the release procedure, and the nightly builds may be affected.

##### chamandana 12/12/2019 13:10:57
I mean timezones and all

##### Fabien Rohrer [Moderator] 12/12/2019 13:11:24
Please look at this page in 20 hours: [https://github.com/cyberbotics/webots/releases](https://github.com/cyberbotics/webots/releases)

##### chamandana 12/12/2019 13:13:41
ok

##### nap 12/12/2019 14:00:59
`@Fabien Rohrer` : Hi!  I just got my controller working. 🙂  But I need to tweak some control coefficients as well as the floor rendering to make it sharper.  At the moment, the e-puck can't detect the lines while turning around, and may also be unable to detect intersections.  So, going quite well.


`@David Mansolino` :  Thanks for the link to the E-Puck demo.  It helped me a lot!


(Tutorial)

##### David Mansolino [Cyberbotics] 12/12/2019 14:02:10
`@nap` you're welcome, glad to hear that it helped 🙂

##### nap 12/12/2019 14:04:17
I have a question about PBRAppearance settings.  I would like to get a really sharp rendering of my floor texture (png).  Roughness 1 and metalness 1  (or both on zero) just don't cut it.  Either the line is dim and diffused, or it's just out of focus.


Anyone know what settings would help?

##### David Mansolino [Cyberbotics] 12/12/2019 14:05:36
I am not sure to understand exactly what you want to achieve, do you want the floor to belike a mirror ?

##### Hayden Woodger 12/12/2019 14:07:24
Sounds like he wants the floor to be HD

##### nap 12/12/2019 14:08:07
no, not a mirror.  My texture has a (almost) black floor (it's actually a shade of really dark red), with bright white lines that intersect in a grid.  At each intersection I have a cyan square which lets the robot know when it's in the right position in the intersection so it can make aligned turns.


No aliasing, no merging.  I would like maximum contrast between the floor, lines, and intersection marker.


(With sharp edges between each.)


(And for the floor to look sharp.)


lol, so hard to express what I want, now that I'm trying to find the right words.

##### David Mansolino [Cyberbotics] 12/12/2019 14:11:25
Ok,, in that case you probably want:

  - metalness 0

  - roughness 1

  - baseColorMap.filtering 0



Make also sur to set in the preference OpenGL.Texture Quality to high


Let me know if that helps

##### nap 12/12/2019 14:12:20
Where does 'baseColorMap.filtering 0' go?  In the same PBR stanza?

##### David Mansolino [Cyberbotics] 12/12/2019 14:12:50
you did add a texure using the basColorMap field rights ?

##### nap 12/12/2019 14:13:16
yes I did

##### David Mansolino [Cyberbotics] 12/12/2019 14:13:32
then the node in basColorMap you should see a 'filtering' field

##### nap 12/12/2019 14:13:57
This is what I have atm:



  floorAppearance PBRAppearance {

    baseColorMap ImageTexture {

      url [

        "textures/Floor\_10px\_small\_Inter\_center\_boarder\_infra-red31\_1024.png"

      ]

    }

    roughness 1

    metalness 0

  }


just add it after metalness?

##### David Mansolino [Cyberbotics] 12/12/2019 14:14:36
You probably want this:

```
floorAppearance PBRAppearance {
    baseColorMap ImageTexture {
      url [
        "textures/Floor_10px_small_Inter_center_boarder_infra-red31_1024.png"
      ]
      filtering 0
    }
    roughness 1
    metalness 0
  }
```

##### nap 12/12/2019 14:16:08
This is what it looks like atm.
%figure
![Screen_Shot_2019-12-13_at_12.15.27_am.png](https://cdn.discordapp.com/attachments/565154703139405824/654687939913187338/Screen_Shot_2019-12-13_at_12.15.27_am.png)
%end


This is what I would like it to look like:
%figure
![Screen_Shot_2019-12-13_at_12.16.55_am.png](https://cdn.discordapp.com/attachments/565154703139405824/654688249142444032/Screen_Shot_2019-12-13_at_12.16.55_am.png)
%end

##### David Mansolino [Cyberbotics] 12/12/2019 14:18:11
Ok is the problem the fact that the black 'squarre' are not exactly black ?

##### nap 12/12/2019 14:18:18
(Note that the 2nd image is from a Webots7 simulation video.)


To me, it looks like there is a layer of old plexiglass over the floor that has clouded due to UV exposure.


Or someone poured some acetone onto it.


Its like here is some 'grey' color in the image.

##### David Mansolino [Cyberbotics] 12/12/2019 14:21:09
But can you share the texture 'Floor\_10px\_small\_Inter\_center\_boarder\_infra-red31\_1024.png' just to see how it looks like ?

##### nap 12/12/2019 14:21:14
The colours are specifically chosen to be at maximum extremes from each other to make the GroundSensors work best.


Floor Texture
%figure
![Floor_10px_small_Inter_center_boarder_infra-red31_1024.png](https://cdn.discordapp.com/attachments/565154703139405824/654689370242285581/Floor_10px_small_Inter_center_boarder_infra-red31_1024.png)
%end

##### David Mansolino [Cyberbotics] 12/12/2019 14:22:33
let me try

##### nap 12/12/2019 14:22:51
kk.  Cheers.

##### David Mansolino [Cyberbotics] 12/12/2019 14:26:55
I checked with roughness 1 and metalness 0

It looks like what you want to achieve no ?
%figure
![khepera3_gripper_1.png](https://cdn.discordapp.com/attachments/565154703139405824/654690656152977428/khepera3_gripper_1.png)
%end

##### nap 12/12/2019 14:26:56
Could it have something to do with the lights?  I have two DirectionalLight nodes.  There were also there in the Webots7 project,  I haven't changed any of their settings, but perhaps this version of Webots uses a different lighting model?


To me, there is still a layer of gray.  The red isn't deep enough.

##### David Mansolino [Cyberbotics] 12/12/2019 14:28:08
Yes, the lightning might influence the rendering for sure, and also the background !

##### nap 12/12/2019 14:29:41
Well, I really appreciate you looking into this.  I need to calibrate some of my coefficients to suit the new e-puck, so I may be able to get it working with what I have.

##### David Mansolino [Cyberbotics] 12/12/2019 14:30:39
Ok, let us know if you can't find any working calibration, I am sure we can find some other solution if needed

##### nap 12/12/2019 15:19:51
Is there a way of getting the simulation to run more smoothly?  I reduced TIME\_STEP from 64 to 32.


I disabled "Ambient Occlusion" in the OpenGL settings, and it's much better now!!

##### ega 12/12/2019 20:15:33
why is my robot stopped after calling robot->step(32) ?

##### David Mansolino [Cyberbotics] 12/13/2019 07:29:07
Hi `@ega` what do you mean by is stopped? is it only the robo or the simulation too?

##### nap 12/13/2019 07:32:52
`@David Mansolino` : His message was around 9 hours ago.  So I'm not sure what happened.

##### David Mansolino [Cyberbotics] 12/13/2019 07:33:14
Indeed let's wait and see if he comes back or not.

##### nap 12/13/2019 07:34:12
I got my controller calibrated to suit the new units of measure.  Works great!


The only issue I have at the moment is that whilst I can build my project inside the Webots Editor, I'm not able to do so from the CLI.

##### David Mansolino [Cyberbotics] 12/13/2019 07:45:42
Very good news !


What is your compilation issue? Did you define correctly WEBOTS\_HOME before trying to compile ?

##### Gautier 12/13/2019 09:19:42
Hey, Might I ask you something ?

##### David Mansolino [Cyberbotics] 12/13/2019 09:19:51
Of course

##### Gautier 12/13/2019 09:19:57
Thank you ! `@David Mansolino`


I'm experimenting with the new functions of the supervisor and I don't really understand how they behave


I have made a video to show you: [https://www.youtube.com/watch?v=fF0xkMFGZTQ&feature=youtu.be](https://www.youtube.com/watch?v=fF0xkMFGZTQ&feature=youtu.be)



Basically, I test a motor on a hinge joint in zero gravity and observe how the two solid react.

 - When a torque is applied to the joint through a motor (first mode, "motor mode"), the two solid react correctly according to their own mass and inertial matrix

 - When a torque is applied through a supervisor on the endsolid, we have two kind of different behavior depending on the initial condition:

           - If there was no initial linear velocity, the two solid rotate in the same sense, even though there is a joint between them (this behavior is not shown in the video above, I'm gonna link you another video right after)

           - If there was an initial linear velocity, only the endpoint solid spin


Did I do something wrong ? Do I need to apply a torque of the same intensity but in the other sense to simulate the "normal behavior" of mecanics ?


Here is the behavior when a torque is applied on endsolid through a supervisor at the beginning of simulation (so without any linear velocity at the beginning): [https://youtu.be/nn7nhfxPxrQ](https://youtu.be/nn7nhfxPxrQ)

##### David Mansolino [Cyberbotics] 12/13/2019 09:37:22
Can you reproduce similar behavior simply by adding torque/force manually?

[https://cyberbotics.com/doc/guide/the-3d-window#applying-a-force-to-a-solid-object-with-physics](https://cyberbotics.com/doc/guide/the-3d-window#applying-a-force-to-a-solid-object-with-physics)

##### Gautier 12/13/2019 09:49:41
Yes, of course


[https://www.youtube.com/watch?v=CiH2hsKYLzc&feature=youtu.be](https://www.youtube.com/watch?v=CiH2hsKYLzc&feature=youtu.be)


The two solid are considered "one" even though there is an hinge joint between them

##### David Mansolino [Cyberbotics] 12/13/2019 09:51:53
Ok, this is kind of reassuring at least the behavior is the same in both cases.

##### Gautier 12/13/2019 09:52:21
Here is the scene tree, Maybe I did an error in it ?
%figure
![unknown.png](https://cdn.discordapp.com/attachments/565154703139405824/654983945150136337/unknown.png)
%end


Yeah, I agree

##### David Mansolino [Cyberbotics] 12/13/2019 09:52:35
Just to make sure, you have a motor in the joint lonking the two solid ?

##### Gautier 12/13/2019 09:52:52
Yes


Do you want me to send my world and controller ?

##### David Mansolino [Cyberbotics] 12/13/2019 09:53:40
Ok, then this is probably the cause of the issue, the motor is applying some torque to make sure the joint remains at the 0 position. Can you try the same without the motor ?

##### Gautier 12/13/2019 09:57:46
[https://youtu.be/8SqgdZr4MTQ](https://youtu.be/8SqgdZr4MTQ)


Without motor, it has this kind of behavior


You were right


I assume that, than, to completly simulate the "expected behavior" (by that, I mean, the reaction of torque in the opposite sense on the base), I have to put a torque inverse to the first one on the solid node of the base ?


(I understand now why it had this kind of behavior for the first video, I assume it was because after controlling the motor directly in torque, the "PID" switch off so the motor no longer try to stay in position "0" ?)

##### David Mansolino [Cyberbotics] 12/13/2019 10:03:03
Very good news, it now makes much more sense.



>  I have to put a torque inverse to the first one on the solid node of the base



Yes, that's probabyl the best solution.



> I understand now why it had this kind of behavior for the first video, I assume it was because after controlling the motor directly in torque, the "PID" switch off so the motor no longer try to stay in position "0" ?



If you want to keep the motor, to disable it you can probably call the `wb_motor_set_available_torque` with a zero torque ([https://www.cyberbotics.com/doc/reference/motor#motor-functions](https://www.cyberbotics.com/doc/reference/motor#motor-functions))

##### Gautier 12/13/2019 10:11:20
Ok, thank you a lot !

##### David Mansolino [Cyberbotics] 12/13/2019 10:11:49
You're welcome

##### Gautier 12/13/2019 10:11:54
😄

##### nap 12/13/2019 12:31:00
`@David Mansolino` : I solved the CLI problem...  I needed to prepend the the `make` command with `CXX=clang++` (as I'm on MacOS here).


I've updated my Makefile now.

##### David Mansolino [Cyberbotics] 12/13/2019 12:32:27
It makes perfectly sens indeed, thank you for the feedback.

##### nap 12/13/2019 14:01:36
`@David Mansolino` : I'm experiencing a strange problem here.  In some folders, `make` ends up generating a dylib (shared library) instead of an executable. Does `make` have a default location where it checks for Makefiles?  I thought it was just the local directory or if specified, at a given path.

##### chamandana 12/13/2019 14:09:34
hey, in which nightly build can I find this merged? [https://github.com/cyberbotics/webots/pull/1181](https://github.com/cyberbotics/webots/pull/1181)

##### nap 12/13/2019 14:12:55
Once it's been merged in, any build after that I would think.

##### chamandana 12/13/2019 14:15:24
this was merged 2 days ago. So, a build done 20 hours ago is fine right?


🙂

##### David Mansolino [Cyberbotics] 12/13/2019 14:16:04
Yes, it should be included in this one: [https://github.com/cyberbotics/webots/releases](https://github.com/cyberbotics/webots/releases)


`@nap`, yes if you do not specify any path, make is executing the Makefile in the current folder

##### nap 12/13/2019 14:42:22
`@David Mansolino` : I figured it out.

`Makefile.include` relies on a hierarchy of folders for various things;

1. The name of the controller is assigned from the name of the containing folder, 

2, If there is a folder named 'controllers' in the branch (presumably one level above), then the code is compiled to an executable, otherwise a shared library, and

3. Maybe other things which I haven't worked out yet.

##### David Mansolino [Cyberbotics] 12/13/2019 14:45:08
You are completely right, the makefile.include assumes that you are using the Webots project standard file hierarchy: [https://www.cyberbotics.com/doc/guide/the-standard-file-hierarchy-of-a-project](https://www.cyberbotics.com/doc/guide/the-standard-file-hierarchy-of-a-project)

##### nap 12/13/2019 14:45:51
Who reads the manual?  lol


But thanks for the link

##### David Mansolino [Cyberbotics] 12/13/2019 14:46:27
You're welcome 😉

##### chamandana 12/15/2019 06:58:19
How can I increase dostance sensor length? I mean its always returning 1000. only reduces to lower values when an object reaches like 5 cm

##### David Mansolino [Cyberbotics] 12/15/2019 11:51:14
HI `@chamandana` , you should change its lookupTable field: [https://cyberbotics.com/doc/reference/distancesensor#lookup-table](https://cyberbotics.com/doc/reference/distancesensor#lookup-table)

##### chamandana 12/15/2019 12:18:57
oh okay. thanks

##### David Mansolino [Cyberbotics] 12/15/2019 12:19:58
You're welcome.

##### chamandana 12/15/2019 12:34:28
how can I hide errors, printed into the webots console?

##### David Mansolino [Cyberbotics] 12/15/2019 12:41:27
By fixing them 😉

##### csnametala 12/16/2019 14:19:41
Hi! Some pages related to Webots are giving error 404.php. Does anyone know the reason? please

##### Stefania Pedrazzi [Cyberbotics] 12/16/2019 14:25:56
Hi `@csnametala`, we updated the server and we are still working on restoring some of the pages. But we also decided to drop some of them, like the forum. Which page are you trying to load?

##### csnametala 12/16/2019 14:28:53
`@Stefania Pedrazzi` I am trying to validate a Webots 7.4.3 EDU key, and it is not possible

##### Stefania Pedrazzi [Cyberbotics] 12/16/2019 14:30:51
Since last week you don't need any longer a license to run old Webots versions.

You just need to use the password `webots`

[https://www.cyberbotics.com/doc/guide/general-faq#can-i-still-use-a-webots-version-before-the-r2019a-release](https://www.cyberbotics.com/doc/guide/general-faq#can-i-still-use-a-webots-version-before-the-r2019a-release)

##### csnametala 12/16/2019 14:34:50
OK! thank you very much for your help `@Stefania Pedrazzi` . I will check

##### Stefania Pedrazzi [Cyberbotics] 12/16/2019 14:35:11
you're welcome!

##### chamandana 12/17/2019 09:58:04
hey, I was recording a long simulation and forgot I didn't have much space left in C:/ . So, the recording stopped as expected, but I couldn't remove the temporary files generated from the recording to regain the disk space. Do you have any insights where the temporary recording files are?

##### Stefania Pedrazzi [Cyberbotics] 12/17/2019 10:00:53
Hi `@chamandana`, they are in the temporary folder (that depends on the OS). you are on Windows, is it correct?


on Windows they should be at `C:\Users\user\AppData\Local\Temp\Webots-<pid>`

##### chamandana 12/17/2019 10:02:58
okay thanks 🙂

##### Arthur VAL 12/17/2019 11:14:55
Hi guys I really need some help please, I’m currently in my high school  in France and I’ve to do a project with Webots


We have to make a complete Yamor

##### Fabien Rohrer [Moderator] 12/17/2019 11:17:54
Glad to read that. You should come here with very precise and well formulated questions if you want help. Webots already contain a yamor model.

##### Tahir [Moderator] 12/17/2019 14:27:54
Hello,

I am having a strange thing which I am not able to understand, I have set my robits position and all of them are rounded off to the upper value and it is moving every robot a little


The value in translation field is rounded off but in the down in edit area it shows the right value
%figure
![JPEG_20191217_152841.jpg](https://cdn.discordapp.com/attachments/565154703139405824/656503316632829963/JPEG_20191217_152841.jpg)
%end



%figure
![JPEG_20191217_152827.jpg](https://cdn.discordapp.com/attachments/565154703139405824/656503357997056031/JPEG_20191217_152827.jpg)
%end


I can see in rviz that every robot is moved from their home position


Any insight on that will highly be appreciated



%figure
![JPEG_20191217_153433.jpg](https://cdn.discordapp.com/attachments/565154703139405824/656504558847131648/JPEG_20191217_153433.jpg)
%end


Here blue points are robot's home positions but they are shifted towards right

##### Fabien Rohrer [Moderator] 12/17/2019 14:38:23
`@Tahir` Hi


the value you see on the scene tree is indeed rounded to few decimals.


the value you see in the field editor is however more precise.


the perfect value can be found from the API or is stored in the .wbt file using a double precision.


Does this make more sense for you?

##### Tahir [Moderator] 12/18/2019 13:40:15
Is there some issue with cyberbotics website?


I am trying to access the download page and it hoes to 404 not found

##### Hayden Woodger 12/18/2019 13:41:02
It's been down for some time, i haven't been able access it either

##### Fabien Rohrer [Moderator] 12/18/2019 13:41:56
It seems to work well from my side: [https://cyberbotics.com/](https://cyberbotics.com/)


Could you clear your cache and try again?

##### Tahir [Moderator] 12/18/2019 13:42:15
The downloads page


I tried from 3 pcs


Let me do it again

##### Fabien Rohrer [Moderator] 12/18/2019 13:42:31
the download page is not accessible anymore


We migrate it to the github page:


[https://github.com/cyberbotics/webots/releases](https://github.com/cyberbotics/webots/releases)

##### Tahir [Moderator] 12/18/2019 13:43:05
OK

##### Hayden Woodger 12/18/2019 13:43:55
Can't access the forum, if i click the top link in the search results: [https://www.google.com/search?q=webots+cannot&rlz=1C1CHBF\_en-GBGB879GB879&oq=webots+cannot&aqs=chrome..69i57.9887j0j4&sourceid=chrome&ie=UTF-8](https://www.google.com/search?q=webots+cannot&rlz=1C1CHBF_en-GBGB879GB879&oq=webots+cannot&aqs=chrome..69i57.9887j0j4&sourceid=chrome&ie=UTF-8)

##### Fabien Rohrer [Moderator] 12/18/2019 13:43:57
We recently had big issues with the website, and we are about to migrate to a new system, and have a brand new website ;.)


For the forum, it's another story.

##### Hayden Woodger 12/18/2019 13:44:26
There's a story? lol

##### Fabien Rohrer [Moderator] 12/18/2019 13:44:37
It has been dropped for ever


But this is for good

##### Hayden Woodger 12/18/2019 13:44:56
That's a shame, there's so much useful info there

##### Fabien Rohrer [Moderator] 12/18/2019 13:45:17
We think that it contains a lot of obsolete information.

##### Hayden Woodger 12/18/2019 13:45:27
ah good point

##### Tahir [Moderator] 12/18/2019 13:45:33
Oh thats alright I just was trying to install from sources and it was not able to find .asc file byt now I will take .deb from github

##### Fabien Rohrer [Moderator] 12/18/2019 13:45:34
We fear that the users may be confused by information on it.


It's a good opportunity to restart on a strong basis of knowledge.

##### Hayden Woodger 12/18/2019 13:46:29
I couldn't agree more

##### Fabien Rohrer [Moderator] 12/18/2019 13:47:09
For now, it would be awesome to use stackeoveflow for all the questions of general interest.


`@Tahir` About the installation, installing it from a precompiled package is indeed much simpler if you don't want to modify Webots sources.

##### Tahir [Moderator] 12/18/2019 13:51:32
OK thanks for the prompt replies I'll do that way

##### Hao 12/19/2019 01:20:19
webots for nao

##### Fabien Rohrer [Moderator] 12/22/2019 16:28:18
There no need of account anymore for this old version, you should simply use « webots » as password: [https://www.cyberbotics.com/doc/guide/general-faq#can-i-still-use-a-webots-version-before-the-r2019a-release](https://www.cyberbotics.com/doc/guide/general-faq#can-i-still-use-a-webots-version-before-the-r2019a-release)

##### Alexis Balayre 12/22/2019 16:47:20
thanks !


how can i connect choregraph to webot ?

##### Fabien Rohrer [Moderator] 12/22/2019 18:38:30
Please refer to this documentation: [https://github.com/cyberbotics/naoqisim/blob/master/README.md](https://github.com/cyberbotics/naoqisim/blob/master/README.md)


Unfortunately, naoqisim (« wrapper » between Webots and Choregraph) is not maintained anymore.

##### Alexis Balayre 12/22/2019 18:40:23
so i can't use webot with choregraph ...

##### swebdev 12/22/2019 20:44:34
Hi, I created a question on SO - [https://stackoverflow.com/questions/59447623/webots-open-multiple-windows-side-by-side](https://stackoverflow.com/questions/59447623/webots-open-multiple-windows-side-by-side) but will ask here as well - Is it possible to have multiple webots world windows simultaneously open?

##### swebdev 12/22/2019 21:17:11
Hi, what's the difference between MFNode and SFNode? Is it documented somewhere - I tried looking but couldn't find. Appreciate any pointers or help!

##### Tahir [Moderator] 12/22/2019 21:28:26
I think its Single Field and Multi Field,  but I am not sure


[https://cyberbotics.com/doc/reference/solid](https://cyberbotics.com/doc/reference/solid)


If you check here in this link with every MF field they are arrays

##### swebdev 12/22/2019 21:34:09
`@Tahir` Makes sense, thanks!

##### c.w 12/23/2019 03:45:27
I follow this tutorial, [https://en.wikibooks.org/wiki/Cyberbotics%27\_Robot\_Curriculum/Beginner\_programming\_Exercises](https://en.wikibooks.org/wiki/Cyberbotics%27_Robot_Curriculum/Beginner_programming_Exercises)

but I don't know  how to open the BotStudio, I believe `@Stefania Pedrazzi` said that 

`we won't drop BotStudio until we have a working alternative`

Can anyone help me?

##### BoBo73 12/23/2019 18:45:25
i can't open the Motion Editor. windows = "generic\_window" then reload, 

it does not work!!

##### Redsign DEMI 12/24/2019 07:37:54
I designed a robot in solidworks. How could i upload it to webot

##### sugus105 12/24/2019 09:29:10
i have same question of Redsign

##### Tahir [Moderator] 12/24/2019 20:44:20
I jave changed the webots coordinate system from LHS to RHS, to get everyhing compatible with ROS. Initially I had problems with the light source so I changed to Point light and it worked fine for a smaller scenario. Now I am having a bigger scenario around 300x300 meters and point light does not seems to be a feasible option . As to get a better overview after increasing its intensity I can not monitor it as after sonetime I get headache because of too much brightness. Any alernative or suggestions to change will highly be appreciated

##### swebdev 12/25/2019 07:10:53
BoundingObject of Robot becomes null after world reload - This is likely a bug, but I posted as a question to confirm - [https://stackoverflow.com/questions/59476046/webots-boundingobject-of-robot-becomes-null-after-world-reload-how-to-prevent](https://stackoverflow.com/questions/59476046/webots-boundingobject-of-robot-becomes-null-after-world-reload-how-to-prevent)


Also, how do you affix a solid to the rectangle arena, I would like to make it immovable w.r.t. the ground. Any pointers shall be appreciated.

##### SomeOne 12/26/2019 10:40:28
Hi, is there an app ore something to create protos in Webots easier? Or another kind of user interface (,instead of writing that Lua code,) to create new photos?


Oh, I think my question belong to general. Sorry

##### nap 12/27/2019 09:49:03
hi, I'm trying to build my controller on linux, and whilst the compilation works, the linking fails with an error stating:

ld: warning: libpng12.so.0, needed by libController.so, not found (try using -rpath or -rpath-link)

Now, I do have that library in /usr/local/webots/lib/webots



How do I fix this?

##### David Mansolino [Cyberbotics] 12/27/2019 09:59:31
@nap, is this problem only visible with the latest version of Webots? You might fix it simply by copying libpng12.so.0 into /usr/local/webots/lib/controller

##### nap 12/27/2019 10:26:50
`@David Mansolino` Hi David.  I'm running this on Ubuntu 18.04.3 LTS using the package manager's version.


I can try that.


Same problem even though it's in the same directory.  When I run ldd on libCppController.so, it says : `libpng12.so.0 => not found`


libController.so, not the Cpp version.

##### Shanks 12/27/2019 13:34:11
Hello there Im new to webots, Is there a course or site to follow for development, Like from modelling objects to programming

##### nap 12/27/2019 13:51:36
`@David Mansolino` :Problem solved.  I needed to add a line in my Makefile with the following: `LFLAGS=-Wl,-rpath-link,/usr/local/webots/lib/webots` parameters, and it's worked.


`@David Mansolino` : Well, that almost worked.  It generated a binary, but upon running it, it issued an error stating it could not find  `libCppController.so`.  This is confirmed by using `ldd` on my controller.


`@David Mansolino` :  I made a little progress.  Setting the link command to:

`g++ -Wl,-rpath-link,"/usr/local/webots/lib/webots/",-rpath-link,"/usr/local/webots/lib/controller/" -Wl,-R "/usr/local/webots/lib/controller/" -s -o build/release/ePuckMeander build/release/ePuck.o build/release/ePuckMeanderBhvr.o build/release/ePuckContainer.o -lm -L"/usr/local/webots/lib/controller" -lController -lCppController`



Fixed the `libCppController.so` runtime error, but now it's complaining about libController.so.  

I would have thought that given the two are in the same folder specified by `-rpath-link` this would be fixed.  However, since I'm not actually building `libCppController.so`, it may be the reason.



By setting `LD_LIBRARY_PATH=/usr/local/webots/lib/controller/` fixed the problem, but I would consider this a bandaid solution.


`@David Mansolino` :  I've found that under MacOS, I have the same problem with `libController.dylib`.  The runtime error is:



`dyld: Library not loaded: @rpath/lib/libController.dylib

  Referenced from: /Users/user/src/webots/projects/controllers/ePuckIface/./build.host/ePuckIface

  Reason: image not found`



And as before, setting `LD_LIBRARY_PATH` makes the problem go away.

##### swebdev 12/28/2019 23:40:36
`@Shanks` You could try to get your hands dirty on [http://robotbenchmark.net/](http://robotbenchmark.net/) which is fun. The next step would be to follow the tutorials here: [https://cyberbotics.com/doc/guide/tutorials?tab-language=java](https://cyberbotics.com/doc/guide/tutorials?tab-language=java) . As you start going with it, the further next step will be to stick to the rest of the documentation.

##### HBK 12/30/2019 08:11:51
Is Webots a good simulator for Multi-agent planning ? It will be a great help if I get a brief comparison of gazebo and Webots for ROS integration.

##### Tahir [Moderator] 12/30/2019 08:31:00
Well I am using Webots for multi agents planning. I would say it's great. I haven't worked with Gazebo quite much, but we already had simulation for single robot in Gazebo(which I didn't worked on, my supervisor did, he said that it might not work for multiple robots in our scenario we can just try) so I moved on to Webots. It didn't gave me much hard time as compared to other ones. I imported my custom robot. Wrote interfaces for Webots services and our custom software. Than just created multiple instances of robots in Webots, run multiple software instances and eveything worked.


Webots ROS handling is also good everything is in form of services which might make life easier at some points

##### HBK 12/30/2019 18:28:39
`@Tahir` Great !!!  Thank you very much.  I am planning to work with 10 robots wont creating multiple instances make process slow ?

##### Tahir [Moderator] 12/30/2019 18:39:39
Well I have 25 I will suggest use asynchronous mode that will not create much load and will update every robot individually, initially I was using synchronous so robots sometimes got out of the planned path. Also in my case visuals are not important I have everything in rviz. So previously when I run without graphics it didn't worked at all because ros and webots frequencies were different so it didn't made sense, now in asynchronous if I run without graphics robots stays on path.

Also I connected two pcs directly with lan, one running just webots and another just ros, so in this way its working well for me.

##### HBK 12/30/2019 18:48:46
I am well verse with ROS and gazebo. What will be best way to start shifting my project in webots `@Tahir`


Brief of my project details:


I have multi-drone system with 4 drones working in an environment. Where I am applying Travelling salesmen Problem (K-TSP) to solve the issue.

So In gazebo sometimes drones fell down

In Gazebo



%figure
![step_1.png](https://cdn.discordapp.com/attachments/565154703139405824/661279920210837509/step_1.png)
%end

##### Tahir [Moderator] 12/31/2019 11:24:45
1. you can start import your robot model in Webots. You can use urdf2webots or some other plugins available on Webots gihub. The model produced will be a little crude. So you can start organizing the stuff e.g. naming sensors and actuators, setting up the fields with resepctive bounding objects etc and atlast you can make a proto if you want. There are alot of example protos you can just change them to base node and see the hieracrhy there is also DJI drone in webots you can check that as well. 



2. After that I will say check the robots maneuverabilty that it moves in the correct direction and orientation for example teleopate on a line and a circle. 



3. Next is just write a small script which at the start enables webots devices. 



4. Webots is left handed system whereas ROS is right handed. This was another problem which I faced so there was two options one to write a transformation which actually does the correct mappings or another to change webots simulation to right handed, so I have gone for the second I changedevery nodes perspective which is now perfectly fine this will have some graphics issues but thats dependant on your need what do you want.


5. I will again highly suggest using asynchronous mode with this you can also run the headless mode and the robot will for sure wait for the commads and hopefully there will be no problem but I am not expert may be there might somone else shed light on this


I will suggest spending a little more time on step one and two and making sure that everything is 101% working as expected becase if there is a problem at this point this might give you hard time and you might see random behaviour

##### HBK 12/31/2019 20:55:15
As you are emphasizing more on asynchronous mode can I know the trade offs of using this mode.(Mostly cons over synchronous mode)

##### Tahir [Moderator] 12/31/2019 20:56:54
[https://cyberbotics.com/doc/reference/robot#synchronous-versus-asynchronous-controllers](https://cyberbotics.com/doc/reference/robot#synchronous-versus-asynchronous-controllers)


like here on this link you can see asynchronous waits for webots\_robot\_step to execute


sorry I forgot to mention in my case the local planner takes some time so I to synchronize or in other words to wait for ros to plan everything I did that


so I would say it depends upon your requirements so just try it

##### HBK 12/31/2019 21:02:32
`@Tahir` Thank you very much these were really useful insights.

##### Tahir [Moderator] 12/31/2019 21:03:40
you are welcomed

