import Stream from './Stream.js';
import {webots} from './webots.js';

export default class Server {
  constructor(url, view, onready) {
    this._url = url;
    this._view = view;
    this._onready = onready;
    // url has one of the following form:
    // "ws(s)://cyberbotics1.epfl.ch:80/simple/worlds/simple.wbt", or
    // "wss://cyberbotics1.epfl.ch/1999/session
    //  ?url=webots://github.com/cyberbotics/webots/branch/master/projects/languages/python/worlds/example.wbt"
    const n = this._url.indexOf('/session?url=', 6);
    // 6 is for skipping the "ws(s)://domain" part of the URL which smallest form is 6 characters long: "ws://a"
    if (n === -1) {
      const m = url.lastIndexOf('/');
      this._project = url.substring(this._url.indexOf('/', 6) + 1, m - 7); // e.g., "simple"
      this._worldFile = url.substring(m + 1); // e.g., "simple.wbt"
    } else
      this._repository = this._url.substring(n + 13);
  }

  connect() {
    const n = this._url.indexOf('/session?url=', 6);
    const url = 'http' + (n > 0 ? this._url.substring(2, n + 8) : this._url.substring(2, this._url.indexOf('/', 6)) + '/session');
    document.getElementById('webotsProgressMessage').innerHTML = 'Connecting to session server...';
    let self = this;
    fetch(url)
      .then(response => response.text())
      .then(function(data) {
        if (data.startsWith('Error:')) {
          document.getElementById('webotsProgress').style.display = 'none';
          let errorMessage = data.substring(6).trim();
          errorMessage = errorMessage.charAt(0).toUpperCase() + errorMessage.substring(1);
          alert('Session server error: ' + errorMessage);
          return;
        }
        self.socket = new WebSocket(data + '/client');
        self.socket.onopen = (event) => {
          self.onOpen(event);
        };
        self.socket.onmessage = (event) => {
          self.onMessage(event);
        };
        self.socket.onclose = (event) => {
          console.log('Disconnected from the Webots server.');
        };
        self.socket.onerror = (event) => {
          console.error('Cannot connect to the simulation server');
        };
      });
  }

  onOpen(event) {
    if (this._repository) {
      let message = `{"start":{"url":"` + this._repository + `"`;
      if (this._view.mode === 'mjpeg')
        message += ',"mode":"mjpeg"';
      message += '}}';
      this.socket.send(message);
    } else { // legacy format
      const host = location.protocol + '//' + location.host.replace(/^www./, ''); // remove 'www' prefix
      this.socket.send('{ "init" : [ "' + host + '", "' + this._project + '", "' + this._worldFile + '" ] }');
    }
    document.getElementById('webotsProgressMessage').innerHTML = 'Starting simulation...';
  }

  onMessage(event) {
    const message = event.data;
    if (message.indexOf('webots:ws://') === 0 || message.indexOf('webots:wss://') === 0) {
      console.log('received ' + message);
      const url = message.substring(7);
      this._httpServerUrl = url.replace(/ws/, 'http');
      this._view.x3dScene.prefix = this._httpServerUrl + '/';
      this._view.stream = new Stream(url, this._view, this._onready);
      this._view.stream.connect();
    } else if (message.indexOf('controller:') === 0 || message.indexOf('reset controller:') === 0) {
      // Need to keep it to avoid having an error message.
    } else if (message.indexOf('queue:') === 0)
      console.log('The server is saturated. Queue to wait: ' + message.substring(6) + ' client(s).');
    else if (message === '.') { // received every 5 seconds when Webots is running
      // nothing to do
    } else
      console.log('Received an unknown message from the Webots server socket: "' + message + '"');
  }
}
