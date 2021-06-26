## Web Simulation

### Description

This section describes how to setup a simulation web service similar to [robotbenchmark.net](https://robotbenchmark.net) to run Webots in the cloud.
Such a system may be distributed on several machines.
One machine runs a session server that communicates with several simulation servers.
Each machine runs one instance of simulation server that receives requests from the session server and instantiates for each connected client a new Webots instance that communicates directly with the client.
Webots instances are executed in a secure environment using [Firejail Security Sandbox](https://firejail.wordpress.com/).

The Web Simulation system is still work in progress and could change in the next releases of Webots.

### Streaming Server

#### Prerequisites

The prerequisites for the server machine(s) are the following:

- Ubuntu 18.04 or 20.04 LTS (Windows and macOS X are also supported, but should not be used for production purposes)
- Webots latest version
- Python 3.8
- Web service dependencies ([Windows instructions](https://github.com/omichel/webots/wiki/Windows-Optional-Dependencies#webots-web-service), [Linux instructions](https://github.com/omichel/webots/wiki/Linux-Optional-Dependencies#webots-web-service)):

Note that the simulation server machines have to met the [Webots system requirements](system-requirements.md).
They may however be virtual machine, such as AWS instances.
GPU instances are strongly recommended for performance reasons, especially if the simulation involves sensors relying on OpenGL rendering (cameras, lidars, etc.).

#### Overview

In order to run Webots in the cloud, you need to run at least one session server and one or more simulation servers.
The simulation servers should run on different machines while the session server may run on a machine where a simulation server is running.
Both servers are Python scripts: `simulation_server.py` and `session_server.py` located in "[WEBOTS\_HOME/resources/web/server/](https://github.com/omichel/webots/tree/released/resources/web/server/)".

Note that Webots have to be installed on all the machines where a simulation server is running.

%figure "Web simulation server network infrastructure"

![context_menu.png](images/web_simulation_network_infrastructure.thumbnail.png)

%end


#### Quick Start
This section gives a simple step-by-step guide on how to start a streaming server with one session and one simulation server.
We assume you use Ubuntu 18.04 or newer.

First, you need to install dependencies:
```bash
sudo apt install subversion firejail python3-tornado python3-pynvml
```

Then, start a session and simulation servers:
```bash
cd $WEBOTS_HOME/resources/web/server
./server.sh start default
```
The session server keeps a track of the available simulation servers and assigns a connection to the most suitable simulation server (similar to a load balancer).
A task of the simulation server is to start a Webots instance with the correct world.

To show the user interface simply open the `$WEBOTS_HOME/resources/web/streaming_viewer/index.html` file in your browser.
In the user interface, find a `Connect to` field, and type for example:
```
ws://localhost:1999/session?url=webots://github.com/cyberbotics/webots/branch/develop/projects/languages/python/worlds/example.wbt
```
Click the `Connect` button to initiate the streaming.
Webots will clone the `example.wbt` simulation from GitHub and start it.

If you want to stop the session and simulation servers run:
```
cd $WEBOTS_HOME/resources/web/server
./server.sh stop
```

Further in the document, you will find more details on how to start multiple simulation servers, how to monitor servers, how to rewrite the ports, and more.

#### Protocol

When a web client needs to know whether it may start a simulation, it will open a WebSocket connection to the session server to monitor the availability of simulation servers.
The session server will answer '1' if some simulation server is available and '0' if none is available.
Whenever this situation changes, the session server will notify the web clients.
Note that the session server will never send twice the same value, it sends only changes in the availability of simulation servers.

When a web client wants to start a simulation, it will send an AJAX request to the session server.
The session server will then send the WebSocket URL of an available simulation server or an error if none are available. The web client will then contact directly the simulation server to startup the simulation.
The simulation server will start Webots with the simulation requested by the client.
The projects files requested by the client should be stored on the website host and requested by the simulation server through an AJAX request at `<host>/ajax/download_project.php`.
Then, the simulation server will send the WebSocket URL of Webots, so that the client can communicate directly with Webots.
If the WebSocket connection to the simulation server is closed or broken, the simulation server will close Webots.
If Webots quits, the simulation server will notify the web client and close the connection with it.

The session server regularly queries the simulation servers to monitor their load.
If a simulation server becomes overloaded, then, the session server will consider it is not available any more and will not offer it to web clients.

%figure "Web simulation protocol sequence diagram"
%chart
sequenceDiagram
  participant U as User
  participant C as Web Client
  participant W as Web Server
  participant SE as Session Server
  participant SS1 as Simulation Server 1
  participant SW1 as Webots
  participant SS2 as Simulation Server 2

  SE->>SS1: Check server availability
  activate SS1
    SS1-->>SE: Return machine load
  deactivate SS1
  SE->>SS2: Check server availability
  activate SS2
    SS2-->>SE: Return machine load
  deactivate SS2

  activate C
    C->>W: Load web page
    activate W
      W->>SE: Check server availability
      activate SE
        SE-->>W: 0 or 1
      deactivate SE
      W-->>C: Return web page content
    deactivate W
    U->>C: Start simulation
    Note left of C: webots-streaming web component
    C->>SE: Open simulation session
    activate SE
      SE-->>C: Return simulation server web socket URL
    deactivate SE
    C->>SS1: Start simulation
    activate SS1
      SS1->>W: Download simulation project
      activate W
        Note right of W: download_project.php
        W-->>SS1: Return simulation project archive
      deactivate W
      SS1->>SW1: Start Webots
      activate SW1
        SS1-->>C: Return Webots web socket URL
      deactivate SS1
      C->>SW1: Register client
      SW1->>C: Send simulation worlds status
      U->>C: Run simulation
      C->>SW1: Change simulation running mode
      loop Each simulation step
        SW1->>C: Send simulation world status
      end
      U->>C: Reset simulation
      C->>SW1: Reset simulation
      loop Each simulation step
        SW1->>C: Send simulation worlds status
      end
      C-->>SS1: {Client disconnected}
    deactivate C
    activate SS1
      SS1->>SW1: Kill Webots
    deactivate SW1
  deactivate SS1
%end
%end

#### Session Server

The session server is the entry point for requesting the start of a new simulation.
It manages the load of the simulation server machines and sends the URL of the available simulation server to the client.

These are the configuration parameters for the session server:
- `port`: local port on which the server is listening.
- `portRewrite`: true if local ports are computed from 443 https/wss URLs (apache rewrite rule).
- `server`: host where this session script is running.
- `administrator`: email address of administrator that will receive notifications about the status of the simulation server machines.
- `mailServer`: SMTP mail server host from which the notifications are sent.
- `mailServerPort`: SMTP mail server port.
- `mailSender`: email address used to send the notifications.
- `mailSenderUser`: user name to authenticate on the SMTP server.
- `mailSenderPassword`: password to authenticate on the SMTP server with the mailSenderUser.
- `simulationServers`: lists all the available simulation servers.
- `sslCertificate`: path to the certificate file for a SSL enabled server.
- `sslKey`: path to the private key file for a SSL enabled server.
- `logDir`: directory where the log file is written. Default value is "WEBOTS\_HOME/resources/web/server/log/session".
- `debug`: if true, the output will be written to `stdout` instead of the log file.

HTTP request handlers:
* The `/` request queries the availability of the simulation servers and returns 1 if some are available or 0 if no simulation server is available.

* The `/monitor` request opens a page showing an overview of the available simulation servers and their load.

WebSocket request handler:
* The `/session` request checks for available simulation servers and returns the URL of the minimally loaded server.

#### Simulation Server

The simulation server creates and starts a Webots instance with the desired simulation for each client request and sends the WebSocket URL of Webots to the client so that it can communicate directly with Webots.

These are the configuration parameters for the simulation server:
- `port`: local port on which the server is listening (launching Webots instances).
- `portRewrite`: true if local ports are computed from 443 https/wss URLs (apache rewrite rule).
- `sslKey`: path to the private key file for a SSL enabled server.
- `sslCertificate`: path to the certificate file for a SSL enabled server.
- `projectsDir`: directory in which Webots projects are located.
- `keyDir`: directory where the website host keys needed for validation are stored. This folder should include a file named as the host (for example "robotbenchmark.net") containing a key identifying it.
- `logDir`: directory where the log files are written. Default value is "WEBOTS\_HOME/resources/web/server/log/simulation".
- `debug`: if true, the output will be written to `stdout` instead of the log file.
- `monitorLogEnabled`: specify if the monitor data have to be stored in a file.
- `maxConnections`: maximum number of simultaneous Webots instances. Default value is 100.


HTTP request handlers:
* The `/load` request returns the current load of the machine computed as the maximum value between all the monitored data (main CPU and GPU load and memory usage, and network usage).

* The `/monitor` request opens a page showing some information about the current status of the simulation server machines, i.e the number of Webots instances running, the load of the CPU and GPU, the network usage.

WebSocket request handlers:
* The `/client` request on the simulation server URL will setup a new Webots instance and return the Webots WebSocket URL. The payload have to contain a `init` value or a `start` value:
- `init` should contain the following data: `[host, projectPart, worldFilename, user1Id, user1Name, user1Authentication, user2Id, user2Name, customData]`. This data is then used to retrieve the simulation data and setup the Webots project.
- `start` should contain the following data: `[url]`.

#### Network Settings

In order to make the web simulation work properly from the outside world, you have two possibilities:
1. Configure your web server, `session_server.py` and `simulation_server.py` to perform port rewrite.
Requests arriving to the web server will be redirected to the local network, e.g., `wss://webserver.com/2000/client` &rarr; `ws://webserver.com:2000/client` and `https://webserver.com/2000/monitor` &rarr; `http://webserver.com:2000/monitor`.
You just need to have port 443 open in your local firewall for incoming connections.
Such a setup is compatible with any client firewall allowing outgoing connections on port 443 to your server (which is very standard).
Moreover, it has the advantage of allowing you to run the `session_server.py`, `simulation_server.py` and Webots without SSL encryption while still proving a SSL interface to the clients via your web server.
2. Open all the ports used by the `session_server.py`, `simulation_server.py` and Webots.
On simple networks, this can be done by modifying the NAT settings of the router.
The disadvantage of this method is that local firewalls may block the connections on non-standard ports, and thus some clients may not be able to use the web simulation if they cannot change their firewall rules.
Another disadvantage is that you must run the `session_server.py`, `simulation_server.py` and Webots in SSL mode if you want to provide a SSL connection to the simulation server, so that it can be executed from a `https` web page.

#### Port Rewrite

Port rewrite is useful for providing an SSL interface using the standard 443 port for both WebSockets `wss://` and HTTP `https://` requests.
It can be tested on a local host by setting up [XAMPP to use SSL](https://gist.github.com/nguyenanhtu/33aa7ffb6c36fdc110ea8624eeb51e69).
Port rewrite should be configured in the `session_server.py` and `simulation_server.py` by simply setting the `portRewrite` option to `true`.
In the `session_server.py` configuration, the `simulationServers` should be listed using the outside URL: `hostname/2000` instead of `hostname:2000`.
Your web server should be configured to redirect `http` traffic to `https` and to rewrite ports in URLs for both `https` and `wss`.
With the Apache web server, this can be achieved by adding the following rules in your `httpd.conf` file:

%tab-component "host"

%tab "webserver.com"
```
LoadModule proxy_module modules/mod_proxy.so
LoadModule proxy_http_module modules/mod_proxy_http.so
LoadModule proxy_wstunnel_module modules/mod_proxy_wstunnel.so

<VirtualHost *:80>
  ServerName webserver.com
  ServerAlias www.webserver.com

  [ ... ]

  RewriteEngine on

  # this rule redirect HTTP requests to HTTPS, removing the 'www.' prefix if any
  RewriteCond %{SERVER_NAME} =%{SERVER_NAME} [OR]
  RewriteCond %{SERVER_NAME} =www.%{SERVER_NAME}
  RewriteRule ^ https://%{SERVER_NAME}%{REQUEST_URI} [END,NE,R=permanent]
</VirtualHost>

<VirtualHost *:443>
  ServerName webserver.com
  ServerAlias www.webserver.com

  [ ... ]

  RewriteEngine on

  # this rule removes the 'www.' prefix from the hostname in the URL if any
  RewriteCond %{SERVER_NAME} =www.webserver.com
  RewriteRule ^ https://webserver.com%{REQUEST_URI} [END,NE,R=permanent]

  # port redirection rules (for session_server.py, simulation_server.py and webots)
  # websockets (should come first)
  RewriteCond %{HTTP:Upgrade} websocket [NC]
  RewriteCond %{HTTP:Connection} upgrade [NC]
  RewriteRule ^/(\d*)/(.*)$ "ws://%{SERVER_NAME}:$1/$2" [P,L]
  # http traffic (should come after websocket)
  RewriteRule ^/(\d*)/(.*)$ "http://%{SERVER_NAME}:$1/$2" [P,L]

</VirtualHost>
```
%tab-end

%tab "localhost"
```
LoadModule alias_module modules/mod_alias.so
LoadModule proxy_module modules/mod_proxy.so
LoadModule proxy_http_module modules/mod_proxy_http.so
LoadModule proxy_wstunnel_module modules/mod_proxy_wstunnel.so

<VirtualHost *:80>
  ServerName localhost

  [ ... ]

  # this rule redirect HTTP requests to HTTPS
  Redirect "/" "https://localhost/"
</VirtualHost>

<VirtualHost *:443>
  ServerName localhost

  [ ... ]

  RewriteEngine on

  # port redirection rules (for session_server.py, simulation_server.py and webots)
  # websockets (should come first)
  RewriteCond %{HTTP:Upgrade} websocket [NC]
  RewriteCond %{HTTP:Connection} upgrade [NC]
  RewriteRule ^/(\d*)/(.*)$ "ws://%{SERVER_NAME}:$1/$2" [P,L]
  # http traffic (should come after websocket)
  RewriteRule ^/(\d*)/(.*)$ "http://%{SERVER_NAME}:$1/$2" [P,L]

</VirtualHost>
```
%tab-end

%end

#### SSL Encryption

Webots, the session and simulation servers can work with or without SSL encryption.
If you use the port rewrite method, applying SSL encryption to Webots, the session server or the simulation server is useless.
Otherwise, it is strongly recommended.
SSL encryption requires the `fullchain.pem` and `privkey.pem` files.
Their path have to be specified in the `sslKey` and `sslCertificate` values of session and simulation configuration file.
Note that Webots will look for the file "WEBOTS\_HOME/resources/web/server/ssl/cert.pem", so you may have to rename `fullchain.pem` and copy it in the `ssl` folder or create a soft link.

#### Server Display

On Linux, a working Linux display (":0") should be available to run Webots remotely.
This can be achieved generally simply by executing 'xhost +' on the server computer.

**Note :** On some computers, in addition to the previous comment, the display entity is linked with the fact that a monitor is plugged.
In this case, you can open automatically a user session when the computer is switched on, run `session_server.py` and `simulation_server.py` automatically when the session starts up, and let the monitor switched on at Ubuntu startup.
If you have a headless system, i.e., a system without any physical monitors attached, then with the NVIDIA graphics card you could fake a monitor in the X session.
This solution basically consists in adding a screen configuration to the X server configuration file by copying the Extended Display Identification Data (EDID) of a temporary attached monitor.

#### Startup Procedure

The startup procedure is the following:

1. Start all the simulation servers (`simulation_server.py`).
2. Start the session server (`session_server.py`).

This procedure should be automated in a startup script, so that the servers are restarted after a reboot of the machine.

This folder also contains a `server.sh` utility script to automatically start and stop `session_server.py` and `simulation_server.py` with a given configuration file.

Please make sure that the `WEBOTS_HOME` variable is set before running the simulation and session server scripts.

#### Using Docker

The simulation server can also be run in a Docker container.
In this section we provide a sample Docker setup that could be used to run the simulation on localhost.

You can use the following `Dockerfile` to build your Docker image.
```
FROM cyberbotics/webots:latest

RUN apt update
RUN apt install -y firejail python3-pip
RUN pip3 install tornado pynvml psutil requests distro
ENV DISPLAY=:99
COPY server/config /usr/local/webots/resources/web/server/config
COPY server/key /usr/local/webots/resources/web/server/key
COPY server.sh /usr/local/server.sh
RUN chmod 654 /usr/local/server.sh
CMD ["/usr/local/server.sh", ""]
```

To correctly setup and automatically run the simulation server, you should provide the following files:
* `server/config`: a folder containing the simulation and session configuration files.
    The current example `server.sh` the local configuration by default, i.e. you should provide the `server/config/session/local.json` and `server/config/simulation/local.json` files or specify the configuration files to be used.
    For example, `server/config/session/local.json`
    ```
    {
      "port": 1999,
      "server": "localhost",
      "simulationServers": [
        "localhost:2000"
      ]
    }
    ```
    and `server/config/simulation/local.json`
    ```
    {
      "port": 2000
    }
    ```
* `server/key`: a folder containing your website host keys needed for validation (see [Session server](#session-server) section).
* `server.sh`: a script that configures the virtual screen and starts the simulation and session servers.
    ```bash
    #!/bin/sh
    Xvfb :99 -screen 0 1024x768x16 &
    cd $WEBOTS_HOME/resources/web/server
    python3 simulation_server.py config/simulation/local.json >/dev/null &
    python3 session_server.py config/session/local.json >/dev/null
    ```

Then, you can open in a terminal the directory containing the Dockerfile to build and run the Docker container:
```bash
docker build -t webots-simulation-server .
sudo docker run -p 1999-2100:1999-2100 -it webots-simulation-server
```

This example runs the simulation server on localhost.
If you want to publish the simulation server on the web, then you may need to setup a web server, such as the Apache web server.

### Website Host

Depending on the `/client` request message, `init` or `start` (experimental), the simulation server will download the simulation data:

#### `init` Simulation Data Download

The host where the client website is running should have a `ajax` named folder at the root level containing these scripts:
* `download-project.php`: a script that returns a zipped archive containing the Webots simulation files to be run. It receives these parameters as POST data:
  * `user1Id`: id identifying the owner of the project, if an accounted application is used.
  * `user1Authentication`: password hash or authentication data for the user.
  * `key`: key identifying the host to authenticate the sender.
  * `project`: name of the Webots project to be downloaded.
* `upload-file.php`: a script that uploads the new controller version when the user modifies and saves it from the editor in the web interface. The POST parameters are:
  * `dirname`: name of the directory where the file has to be uploaded, mainly consisting on the project name.
  * `filename`: name of the file to be uploaded.
  * `content`: content of the file to be stored.

Sample PHP files are located in "[WEBOTS\_HOME/resources/web/server/](https://github.com/omichel/webots/tree/released/resources/web/templates/)".

#### `start` Simulation Data Download (Experimental)

Webots will checkout the simulation data from the provided `url` using a custom URI scheme as exemplified below:
```
webots://github.com/john/simulation/tag/v1.0/project/worlds/sample.wbt
└─┬──┘   └───┬────┘└──────┬───────┘└───┬───┘└───────────┬────────────┘
scheme   authority   repository     version            path
```
Currently this protocol only supports public GitHub repositories.
The `version` part support both `/tag/<tag_name>` and `/branch/<branch_name>` formats to refer either to a Git tag or a Git branch.
In the above sample URL, the simulation server will checkout the content of the `/project` directory and start Webots with the specified `sample.wbt` world file.

This protocol is experimental and doesn't support modifying the source code of robot controllers from the web interface.
Also the robot windows are not yet supported.

#### How to Embed a Web Scene in Your Website

Similarly to [this section](web-streaming.md#how-to-embed-a-web-scene-in-your-website), to embed the simulation it is enough to instantiate a `webots-streaming` web component from the [WebotsStreaming.js] package.

This is the API of the `webots-streaming` web component:
* `connect(url, mode, broadcast, mobileDevice, callback, disconnectCallback) `: function instantiating the simulation web interface and taking as argument:
  * `url`: Three different `url` formats are supported:
      * URL to a WBT file (i.e. "ws://localhost:80/simple/worlds/simple.wbt"): this is the format required to start a web simulation. The `url` value specifies both the session server host and the desired simulation name.
      * WebSocket URL (i.e. "ws://localhost:80"): this format is used for web broadcast streaming.
      * URL to a X3D file (i.e. "file.x3d"): this format is used for showing a [web scene](web-scene.md) or a [web animation](web-animation.md).
  * `mode`: `x3d` or `mjpeg`.
  * `broadcast`: boolean variable enabling or not the broadcast.
  * `mobileDevice`: boolean variable specifying if the application is running on a mobile device.
  * `callback`: function to be executed once the simulation is ready.
  * `disconnectCallback`: function to be executed once the web scene is closed.
* `disconnect()`: close the simulation web scene.
* `hideToolbar()`: hide the toolbar. Must be called after connect.
* `showToolbar()`: show the toolbar. Must be called after connect. The toolbar is displayed by default.
* `showQuit(enable)`: specify is the quit button must be displayed on the toolbar. Must be called before connect. The quit button is displayed by default.
* `showRevert(enable)`: specify is the revert button must be displayed on the toolbar. Must be called before connect. The quit button is hidden by default.
* `sendMessage(message)`: send a message to the streaming server through the web socket. Examples of messages could be:
    *`real-time:-1`: to play the simulation.
    *`pause`: to pause the simulation.
    *`robot:{"name":"supervisor","message":"reset"}`: to send a message to the controller of a robot named "supervisor".

An example of a file using this API is available [here](https://cyberbotics1.epfl.ch/open-roberta/setup_viewer.js) and is used to run [this sample](https://cyberbotics1.epfl.ch/open-roberta/).

### Scene Refresh Rate

The scene refresh rate is defined by the `WorldInfo.FPS` field.
The same fields as in the [web animation](web-animation.md#limitations) are updated.

### Limitations

The streaming server has the same limitations as the [Web streaming](web-streaming.md#limitations).

### Technologies and Limitations

The data is sent to the clients using [WebSockets](https://www.websocket.org/).
In case of related issues, make sure that `WebSockets` are enabled in your Web browser settings.

Please refer to the limitations described in [this section](web-animation.md#remarks-on-the-used-technologies-and-their-limitations).
