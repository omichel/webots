'use strict';

import {webots} from './webots.js';

export default class Stream {
  constructor(wsServer, view, onready) {
    this.wsServer = wsServer + '/';
    this._view = view;
    this._onready = onready;
    this.socket = null;
  }

  connect() {
    this.socket = new WebSocket(this.wsServer);
    if (document.getElementById('webotsProgressMessage'))
      document.getElementById('webotsProgressMessage').innerHTML = 'Connecting to Webots instance...';
    this.socket.onopen = (event) => { this._onSocketOpen(event); };
    this.socket.onmessage = (event) => { this._onSocketMessage(event); };
    this.socket.onclose = (event) => { this._onSocketClose(event); };
    this.socket.onerror = (event) => {
      this._view.destroyWorld();
      this._view.onerror('WebSocket error: ' + event.data);
    };
  }

  close() {
    if (this.socket)
      this.socket.close();
  }

  _onSocketOpen(event) {
    let mode = this._view.mode;
    if (mode === 'mjpeg')
      mode += ': ' + this._view.view3D.offsetWidth + 'x' + (this._view.view3D.offsetHeight - 48); // subtract toolbar height

    else if (this._view.broadcast)
      mode += ';broadcast';
    this.socket.send(mode);
  }

  _onSocketClose(event) {
    this._view.onerror('Disconnected from ' + this.wsServer + ' (' + event.code + ')');
    if ((event.code > 1001 && event.code < 1016) || (event.code === 1001 && this._view.quitting === false)) { // https://tools.ietf.org/html/rfc6455#section-7.4.1
      if (window.confirm(`Streaming server error
      Connection closed abnormally. (Error code:` + event.code + `)
      The simulation is going to be reset`))
        window.open(window.location.href, '_self');
    }
    this._view.destroyWorld();
    if (typeof this._view.onclose === 'function')
      this._view.onclose();
  }

  _onSocketMessage(event) {
    let data = event.data;
    if (data.startsWith('robot:') ||
        data.startsWith('stdout:') ||
        data.startsWith('stderr:') ||
        data.startsWith('robot window:'))
      return 0; // We need to keep this condition, otherwise the robot window messages will be printed as errors.
    else if (data.startsWith('world:')) {
      data = data.substring(data.indexOf(':') + 1).trim();
      let currentWorld = data.substring(0, data.indexOf(':')).trim();
      data = data.substring(data.indexOf(':') + 1).trim();
      this._view.updateWorldList(currentWorld, data.split(';'));
    } else if (data.startsWith('pause:') || data === 'paused by client') {
      if (this._view.toolBar !== null)
        this._view.toolBar.setMode('pause');
      // Update timeout.
      if (data.startsWith('pause:')) {
        this._view.isAutomaticallyPaused = undefined;
        this._view.time = parseFloat(data.substring(data.indexOf(':') + 1).trim());
      }
      if (this._view.timeout > 0 && !this._view.isAutomaticallyPaused) {
        this._view.deadline = this._view.timeout;
        if (typeof this._view.time !== 'undefined')
          this._view.deadline += this._view.time;

        if (document.getElementById('webotsTimeout'))
          document.getElementById('webotsTimeout').innerHTML = webots.parseMillisecondsIntoReadableTime(this._view.deadline);
      }
    } else if (data === 'real-time' || data === 'run' || data === 'fast') {
      if (this._view.toolBar) {
        this._view.toolBar.setMode(data);
        this._view.runOnLoad = data;
      } else
      if (this._view.timeout >= 0)
        this.socket.send('timeout:' + this._view.timeout);
    } else if (data.startsWith('loading:')) {
      if (document.getElementById('webotsProgress'))
        document.getElementById('webotsProgress').style.display = 'block';
      data = data.substring(data.indexOf(':') + 1).trim();
      let loadingStatus = data.substring(0, data.indexOf(':')).trim();
      data = data.substring(data.indexOf(':') + 1).trim();
      if (document.getElementById('webotsProgressMessage'))
        document.getElementById('webotsProgressMessage').innerHTML = 'Webots: ' + loadingStatus;
      if (document.getElementById('webotsProgressPercent'))
        document.getElementById('webotsProgressPercent').innerHTML = '<progress value="' + data + '" max="100"></progress>';
    } else if (data === 'scene load completed') {
      this._view.time = 0;
      if (document.getElementById('webotsClock'))
        document.getElementById('webotsClock').innerHTML = webots.parseMillisecondsIntoReadableTime(0);
      if (this._view.mode === 'mjpeg') {
        if (document.getElementById('webotsProgress'))
          document.getElementById('webotsProgress').style.display = 'none';
        this._view.multimediaClient.requestNewSize(); // To force the server to render once
      }

      if (typeof this._onready === 'function')
        this._onready();
    } else if (data === 'reset finished') {
      this._view.resetSimulation();
      if (typeof this._view.x3dScene !== 'undefined' && typeof this._view.multimediaClient === 'undefined')
        this._view.x3dScene.resetViewpoint();
      if (webots.currentView.toolBar)
        webots.currentView.toolBar.enableToolBarButtons(true);
      if (typeof this._onready === 'function')
        this._onready();
    } else if (data.startsWith('time: ')) {
      this._view.time = parseFloat(data.substring(data.indexOf(':') + 1).trim());
      if (document.getElementById('webotsClock'))
        document.getElementById('webotsClock').innerHTML = webots.parseMillisecondsIntoReadableTime(this._view.time);
    } else if (data === 'delete world') {
      this._view.destroyWorld();
      webots.currentView.toolBar.enableToolBarButtons(false);
    } else {
      let messagedProcessed = false;
      if (typeof this._view.multimediaClient !== 'undefined')
        messagedProcessed = this._view.multimediaClient.processServerMessage(data);
      else if (typeof this._view.x3dScene !== 'undefined')
        messagedProcessed = this._view.x3dScene.processServerMessage(data, this._view);
      if (!messagedProcessed)
        console.log('WebSocket error: Unknown message received: "' + data + '"');
    }
  }
}
