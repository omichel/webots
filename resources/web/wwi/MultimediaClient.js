'use strict';
import SystemInfo from './system_info.js';
import {webots} from './webots.js';

export default class MultimediaClient {
  constructor(view, parentObject) {
    this._view = view;
    this._domElement = document.createElement('img');
    this._domElement.style.background = 'white';
    this._domElement.id = 'remoteScene';
    this._domElement.setAttribute('draggable', false);
    parentObject.appendChild(this._domElement);

    this._viewMode = false;
    this._worldInfo = {title: null};

    this._mouseDown = 0;
    this.onmousemove = (e) => { this._onMouseMove(e); };
    this._lastMousePosition = null;
    this._touchEvent = { move: false };
    this.ontouchmove = (e) => { this._onTouchMove(e); };
    this.ontouchend = (e) => { this._onTouchEnd(e); };
  }

  disconnect() {
    this._domElement.src = '';
  }

  finalize(onready) {
    this._domElement.addEventListener('mousedown', (e) => { this._onMouseDown(e); }, false);
    this._domElement.addEventListener('mouseup', (e) => { this._onMouseUp(e); }, false);
    this._domElement.addEventListener('wheel', (e) => { this._onWheel(e); }, false);
    this._domElement.addEventListener('touchstart', (event) => { this._onTouchStart(event); }, true);
    this._domElement.addEventListener('contextmenu', (event) => { event.preventDefault(); }, false);
    this._view.toolBar.enableToolBarButtons(!this._viewMode);
    if (typeof onready === 'function')
      onready();
  }

  setWorldInfo(title) {
    this._worldInfo = {title: title};
  }

  setFollowed(solidId, mode) {
    const socket = this._view.stream.socket;
    if (!socket || socket.readyState !== 1)
      return;
    socket.send('follow: ' + mode + ',' + solidId);
  }

  requestNewSize() {
    if (this._lastWidth === this._domElement.width && this._lastHeight === this._domElement.height)
      return;
    this._view.stream.socket.send('resize: ' + this._domElement.width + 'x' + this._domElement.height);
    this._lastWidth = this._domElement.width;
    this._lastHeight = this._domElement.height;
  }

  resize(width, height) {
    this.requestNewSize();
  }

  processServerMessage(data) {
    if (data.startsWith('multimedia: ')) {
      const list = data.split(' ');
      const httpUrl = 'http' + this._view.stream.wsServer.slice(2); // replace 'ws' with 'http'
      const url = httpUrl + list[1];
      this._view.toolBar.setMode(list[2]);
      this._domElement.src = url;
      this._viewMode = list.length > 4; // client in view mode
      if (this._viewMode) {
        this._domElement.style.width = list[3] + 'px';
        this._domElement.style.height = list[4] + 'px';
      }
      this._view.toolBar.enableToolBarButtons(!this._viewMode);
      console.log('Multimedia streamed on ' + url);
    } else if (data.startsWith('resize: ')) {
      if (this._viewMode) {
        let list = data.split(' ');
        this._domElement.style.width = list[1] + 'px';
        this._domElement.style.height = list[2] + 'px';
      } // else ignore resize triggered from this instance
    } else if (data.startsWith('world info: ')) {
      let dataString = data.substring(data.indexOf(':') + 1).trim();
      let dataObject = JSON.parse(dataString);
      this.setWorldInfo(dataObject.title, dataObject.window);
    } else
      return false;
    return true;
  }

  _onMouseDown(event) {
    if (this._viewMode)
      return false;

    this._mouseDown = 0;
    switch (event.button) {
      case 0: // MOUSE.LEFT
        this._mouseDown |= 1;
        break;
      case 1: // MOUSE.MIDDLE
        this._mouseDown |= 4;
        break;
      case 2: // MOUSE.RIGHT
        this._mouseDown |= 2;
        break;
    }
    if (SystemInfo.isMacOS() && 'ctrlKey' in event && event['ctrlKey'] && this._mouseDown === 1)
      // On macOS, "Ctrl + left click" should be dealt as a right click.
      this._mouseDown = 2;

    event.target.addEventListener('mousemove', this.onmousemove, false);
    this._sendMouseEvent(-1, this._computeRemoteMouseEvent(event), 0);
    event.preventDefault();
    return false;
  }

  _onMouseMove(event) {
    if (this._mouseDown === 0) {
      event.target.removeEventListener('mousemove', this.onmousemove, false);
      return false;
    } else if (event.buttons === 0) {
      // mouse button released outside the 3D scene
      // send a mouse up event to complete the drag
      let mouseUpEvent = this._computeRemoteMouseEvent(event);
      mouseUpEvent.offsetX = this._lastMousePosition.x;
      mouseUpEvent.offsetY = this._lastMousePosition.y;
      event.target.removeEventListener('mousemove', this.onmousemove, false);
      this._sendMouseEvent(1, mouseUpEvent, 0);
      event.preventDefault();
      return false;
    }
    this._sendMouseEvent(0, this._computeRemoteMouseEvent(event), 0);
    return false;
  }

  _onMouseUp(event) {
    if (this._viewMode)
      return;

    event.target.removeEventListener('mousemove', this.onmousemove, false);
    this._sendMouseEvent(1, this._computeRemoteMouseEvent(event), 0);
    event.preventDefault();
    return false;
  }

  _onWheel(event) {
    if (this._viewMode)
      return false;

    this._sendMouseEvent(2, this._computeRemoteMouseEvent(event), Math.sign(event.deltaY));
    return false;
  }

  _onTouchStart(event) {
    if (this._viewMode)
      return;

    this._mouseDown = 0;
    let touch0 = event.targetTouches['0'];
    this._touchEvent.x = Math.round(touch0.clientX); // discard decimal values returned on android
    this._touchEvent.y = Math.round(touch0.clientY);
    if (event.targetTouches.length === 2) {
      const touch1 = event.targetTouches['1'];
      this._touchEvent.x1 = touch1.clientX;
      this._touchEvent.y1 = touch1.clientY;
      const distanceX = this._touchEvent.x - this._touchEvent.x1;
      const distanceY = this._touchEvent.y - this._touchEvent.y1;
      this._touchEvent.distance = distanceX * distanceX + distanceY * distanceY;
      this._touchEvent.orientation = Math.atan2(this._touchEvent.y1 - this._touchEvent.y, this._touchEvent.x1 - this._touchEvent.x);
      this._mouseDown = 3; // two fingers: rotation, tilt, zoom
    } else
      this._mouseDown = 2; // 1 finger: translation or single click

    event.target.addEventListener('touchend', this.ontouchend, true);
    event.target.addEventListener('touchmove', this.ontouchmove, true);

    if (typeof webots.currentView.ontouchstart === 'function')
      webots.currentView.ontouchstart(event);

    this._touchEvent.initialTimeStamp = Date.now();
    this._touchEvent.moved = false;
    this._touchEvent.initialX = null;
    this._touchEvent.initialY = null;
    this._touchEvent.mode = undefined;
    this._lastTouchEvent = this._computeTouchEvent(event);
    this._sendMouseEvent(-1, this._lastTouchEvent, 2);
    event.preventDefault();
    return false;
  }

  _onTouchMove(event) {
    if (this._viewMode || event.targetTouches.length === 0 || event.targetTouches.length > 2)
      return false;
    if (typeof this._touchEvent.initialTimeStamp === 'undefined')
      // Prevent applying mouse move action before drag initialization in mousedrag event.
      return;
    if ((typeof this._touchEvent.mode !== 'undefined') && (this._mouseDown !== 3))
      // Gesture single/multi touch changed after initialization.
      return false;

    let touch0 = event.targetTouches['0'];
    let x = Math.round(touch0.clientX); // discard decimal values returned on android
    let y = Math.round(touch0.clientY);
    let dx = x - this._touchEvent.x;
    let dy = y - this._touchEvent.y;

    if (this._mouseDown === 2) { // translation
      // On small phone screens (Android) this is needed to correctly detect clicks and longClicks.
      if (this._touchEvent.initialX == null && this._touchEvent.initialY == null) {
        this._touchEvent.initialX = Math.round(this._touchEvent.x);
        this._touchEvent.initialY = Math.round(this._touchEvent.y);
      }
      if (Math.abs(dx) < 2 && Math.abs(dy) < 2 &&
        Math.abs(this._touchEvent.initialX - x) < 5 && Math.abs(this._touchEvent.initialY - y) < 5)
        this._touchEvent.moved = false;

      else
        this._touchEvent.moved = true;

      this._lastTouchEvent = this._computeTouchEvent(event);
      if (this._touchEvent.moved)
        this._sendMouseEvent(0, this._lastTouchEvent, 2);
    } else {
      let touch1 = event.targetTouches['1'];
      let x1 = Math.round(touch1.clientX);
      let y1 = Math.round(touch1.clientY);
      let distanceX = x - x1;
      let distanceY = y - y1;
      let newTouchDistance = distanceX * distanceX + distanceY * distanceY;
      let pinchSize = this._touchEvent.distance - newTouchDistance;

      let moveX1 = x - this._touchEvent.x;
      let moveX2 = x1 - this._touchEvent.x1;
      let moveY1 = y - this._touchEvent.y;
      let moveY2 = y1 - this._touchEvent.y1;
      let ratio = window.devicePixelRatio || 1;

      // Initialize multi-touch gesture.
      if (typeof this._touchEvent.mode === 'undefined') {
        if (Date.now() - this._touchEvent.initialTimeStamp < 100)
          // Wait some ms to be able to detect the gesture.
          return;
        if (Math.abs(pinchSize) > 500 * ratio) { // zoom and tilt
          this._mouseDown = 3;
          this._touchEvent.mode = 2;
        } else if (Math.abs(moveY2 - moveY1) < 3 * ratio && Math.abs(moveX2 - moveX1) < 3 * ratio) { // rotation (pitch and yaw)
          this._mouseDown = 3;
          this._touchEvent.mode = 1;
        } else
          return;

        if (typeof this._touchEvent.mode !== 'undefined')
          // send rotation/zoom picked position
          this._sendMessage('touch -1 ' + this._touchEvent.mode + ' ' + x + ' ' + y);
      } else if (this._touchEvent.mode === 2) {
        let d;
        if (Math.abs(moveX2) < Math.abs(moveX1))
          d = moveX1;
        else
          d = moveX2;
        let tiltAngle = 0.0004 * d;
        let zoomScale = 0.015 * pinchSize;
        this._sendMessage('touch 0 ' + this._touchEvent.mode + ' ' + tiltAngle + ' ' + zoomScale);
      } else { // rotation
        dx = moveX1 * 0.8;
        dy = moveY1 * 0.5;
        this._sendMessage('touch 0 ' + this._touchEvent.mode + ' ' + dx + ' ' + dy);
      }

      this._touchEvent.moved = true;
      this._touchEvent.distance = newTouchDistance;
      this._touchEvent.x1 = x1;
      this._touchEvent.y1 = y1;
    }

    this._touchEvent.x = x;
    this._touchEvent.y = y;

    if (typeof webots.currentView.ontouchmove === 'function')
      webots.currentView.ontouchmove(event);
    event.preventDefault();
    return false;
  }

  _onTouchEnd(event) {
    if (this._viewMode)
      return false;

    this._domElement.removeEventListener('touchend', this.ontouchend, true);
    this._domElement.removeEventListener('touchmove', this.ontouchmove, true);

    if (typeof webots.currentView.ontouchend === 'function')
      webots.currentView.ontouchend(event);

    if (typeof this._lastTouchEvent === 'undefined') {
      this._touchEvent.initialTimeStamp = undefined;
      event.preventDefault();
      return false;
    }

    if (typeof this._touchEvent.mode === 'undefined') {
      let longClick = Date.now() - this._touchEvent.initialTimeStamp >= 100;
      if (this._touchEvent.move === false && !longClick) {
        // A single short click corresponds to left click to select items.
        this._lastTouchEvent.button = 0;
        this._lastTouchEvent.buttons = 1;
        this._sendMouseEvent(-1, this._lastTouchEvent, longClick);
      }
      this._sendMouseEvent(1, this._lastTouchEvent, longClick);
    }
    this._lastTouchEvent = undefined;
    this._touchEvent.initialTimeStamp = undefined;
    this._touchEvent.move = false;
    this._touchEvent.mode = undefined;
    event.preventDefault();
    return false;
  }

  _computeRemoteMouseEvent(event) {
    let remoteEvent = {};
    remoteEvent.button = event.button;
    remoteEvent.buttons = this._mouseDown;
    remoteEvent.modifier = (event.shiftKey ? 1 : 0) + (event.ctrlKey ? 2 : 0) + (event.altKey ? 4 : 0);
    remoteEvent.offsetX = event.offsetX;
    remoteEvent.offsetY = event.offsetY;
    return remoteEvent;
  }

  _computeTouchEvent(event, move) {
    let remoteEvent = {};
    remoteEvent.buttons = this._mouseDown;
    remoteEvent.modifier = 0; // disabled
    const touch = event.targetTouches['0'];
    remoteEvent.offsetX = Math.round(touch.clientX); // discard decimal values returned on android
    remoteEvent.offsetY = Math.round(touch.clientY);

    if (this._touchEvent.mode === 3) // zoom/tilt
      remoteEvent.button = 3;
    else if (this._touchEvent.mode === 1) // rotation
      remoteEvent.button = 1;
    else
      remoteEvent.button = 2; // 1 finger: translation or single click

    // Adjust touch point coordinates so that they are local to the remote scene.
    remoteEvent.offsetX -= this._domElement.x;
    remoteEvent.offsetY -= this._domElement.y;
    return remoteEvent;
  }

  _sendMessage(message) {
    let socket = this._view.stream.socket;
    if (!socket || socket.readyState !== 1)
      return;
    socket.send(message);
  }

  _sendMouseEvent(type, event, wheel) {
    this._sendMessage('mouse ' + type + ' ' + event.button + ' ' + event.buttons + ' ' +
                event.offsetX + ' ' + event.offsetY + ' ' + event.modifier + ' ' + wheel);
    this._lastMousePosition = {x: event.offsetX, y: event.offsetY};
  }
}
