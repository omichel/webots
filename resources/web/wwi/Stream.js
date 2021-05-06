'use strict';

import {webots} from './webots.js';

export default class Stream {
  constructor(wsServer, view, onready) {
    this.wsServer = wsServer + '/';
    this.view = view;
    this.onready = onready;
    this.socket = null;
  }

  connect() {
    this.socket = new WebSocket(this.wsServer);
    $('#webotsProgressMessage').html('Connecting to Webots instance...');
    this.socket.onopen = (event) => { this.onSocketOpen(event); };
    this.socket.onmessage = (event) => { this.onSocketMessage(event); };
    this.socket.onclose = (event) => { this.onSocketClose(event); };
    this.socket.onerror = (event) => {
      this.view.destroyWorld();
      this.view.onerror('WebSocket error: ' + event.data);
    };
  }

  close() {
    if (this.socket)
      this.socket.close();
  }

  onSocketOpen(event) {
    let mode = this.view.mode;
    if (mode === 'mjpeg')
      mode += ': ' + this.view.view3D.offsetWidth + 'x' + (this.view.view3D.offsetHeight - 48); // subtract toolbar height

    else if (this.view.broadcast)
      mode += ';broadcast';
    this.socket.send(mode);
  }

  onSocketClose(event) {
    this.view.onerror('Disconnected from ' + this.wsServer + ' (' + event.code + ')');
    if ((event.code > 1001 && event.code < 1016) || (event.code === 1001 && this.view.quitting === false)) { // https://tools.ietf.org/html/rfc6455#section-7.4.1
      webots.alert('Streaming server error',
        'Connection closed abnormally.<br>(Error code: ' + event.code + ')<br><br>' +
        'Please reset the simulation by clicking ' +
        '<a href="' + window.location.href + '">here</a>.');
    }
    this.view.destroyWorld();
    if (typeof this.view.onclose === 'function')
      this.view.onclose();
  }

  onSocketMessage(event) {
    let data = event.data;
    if (data.startsWith('robot:') ||
        data.startsWith('stdout:') ||
        data.startsWith('stderr:'))
      return 0; // We need to keep this condition, otherwise the robot window messages will be printed as errors.
    else if (data.startsWith('world:')) {
      data = data.substring(data.indexOf(':') + 1).trim();
      let currentWorld = data.substring(0, data.indexOf(':')).trim();
      data = data.substring(data.indexOf(':') + 1).trim();
      this.view.updateWorldList(currentWorld, data.split(';'));
    } else if (data.startsWith('pause:') || data === 'paused by client') {
      if (this.view.toolBar !== null)
        this.view.toolBar.setMode('pause');
      // Update timeout.
      if (data.startsWith('pause:')) {
        this.view.isAutomaticallyPaused = undefined;
        this.view.time = parseFloat(data.substring(data.indexOf(':') + 1).trim());
      }
      if (this.view.timeout > 0 && !this.view.isAutomaticallyPaused) {
        this.view.deadline = this.view.timeout;
        if (typeof this.view.time !== 'undefined')
          this.view.deadline += this.view.time;
        $('#webotsTimeout').html(webots.parseMillisecondsIntoReadableTime(this.view.deadline));
      }
    } else if (data === 'real-time' || data === 'run' || data === 'fast') {
      if (this.view.toolBar)
        this.view.toolBar.setMode(data);
      if (this.view.timeout >= 0)
        this.socket.send('timeout:' + this.view.timeout);
    } else if (data.startsWith('loading:')) {
      $('#webotsProgress').show();
      data = data.substring(data.indexOf(':') + 1).trim();
      let loadingStatus = data.substring(0, data.indexOf(':')).trim();
      data = data.substring(data.indexOf(':') + 1).trim();
      $('#webotsProgressMessage').html('Webots: ' + loadingStatus);
      $('#webotsProgressPercent').html('<progress value="' + data + '" max="100"></progress>');
    } else if (data === 'scene load completed') {
      this.view.time = 0;
      $('#webotsClock').html(webots.parseMillisecondsIntoReadableTime(0));
      if (this.view.mode === 'mjpeg') {
        $('#webotsProgress').hide();
        this.view.multimediaClient.requestNewSize(); // To force the server to render once
      }

      if (typeof this.onready === 'function')
        this.onready();
    } else if (data === 'reset finished') {
      this.view.resetSimulation();
      if (typeof this.view.x3dScene !== 'undefined')
        this.view.x3dScene.resetViewpoint();
      if (webots.currentView.toolBar)
        webots.currentView.toolBar.enableToolBarButtons(true);
      if (typeof this.onready === 'function')
        this.onready();
    } else if (data.startsWith('time: ')) {
      this.view.time = parseFloat(data.substring(data.indexOf(':') + 1).trim());
      $('#webotsClock').html(webots.parseMillisecondsIntoReadableTime(this.view.time));
    } else if (data === 'delete world')
      this.view.destroyWorld();
    else {
      let messagedProcessed = false;
      if (typeof this.view.multimediaClient !== 'undefined')
        messagedProcessed = this.view.multimediaClient.processServerMessage(data);
      else if (typeof this.view.x3dScene !== 'undefined')
        messagedProcessed = this.view.x3dScene.processServerMessage(data, this.view);
      if (!messagedProcessed)
        console.log('WebSocket error: Unknown message received: "' + data + '"');
    }
  }
}
