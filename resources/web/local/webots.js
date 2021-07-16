// On the local version of the webots.js API, only one instance of a
// webots.Robot is supported (corresponding to the robot window displayed in
// Webots)

/* global _webots */
/* global qt */
/* global QWebChannel */
/* eslint no-extend-native: ["error", { "exceptions": ["String"] }] */

var webots = {};

window.addEventListener('DOMContentLoaded', function() {
  if (typeof _webots === 'undefined')
    webots.setupQtChannelTransport(null);
}, true);

webots.Window = function(name) {
  this.name = name;
};

webots.Window.prototype.send = function(message, robot) {
  message = (typeof message === 'undefined') ? '' : message; // Avoid undefined variables.
  if (typeof _webots === 'undefined')
    webots.setupQtChannelTransport(function() { _webots.receiveFromJavascript(message); });
  else
    _webots.receiveFromJavascript(message);
};

webots.Window.prototype.receive = function(message, robot) { // Should be overriden.
  console.log("Robot.window '" + this.name + "' of Robot.name '" + robot + "' received message: " + message);
};

webots.Window.prototype.init = function(func) {
  func();
};

webots.Window.prototype.setTitle = function(title, tabbedTitle, robot) {
  title = (typeof title === 'undefined') ? '' : title; // Avoid undefined variables.
  tabbedTitle = (typeof tabbedTitle === 'undefined') ? '' : tabbedTitle; // Avoid undefined variables.
  if (typeof _webots === 'undefined')
    webots.setupQtChannelTransport(function() { _webots.setTitle(title, tabbedTitle); });
  else
    _webots.setTitle(title, tabbedTitle);
};

webots.Window.receive = function(message, robot) { // static method
  webots.Window.instance.receive(message, robot);
  if (typeof _webots === 'undefined')
    webots.setupQtChannelTransport(function() { _webots.ackReceived = true; });
  else
    _webots.ackReceived = true;
};

webots.window = function(name) {
  if (typeof webots.Window.instance === 'undefined')
    webots.Window.instance = new webots.Window(name);
  else if (name !== webots.Window.instance.name) {
    console.log("Robot window name mismatch: '" + name + "' != '" + webots.Window.instance.name + "'");
    return null;
  }
  return webots.Window.instance;
};

webots.setupQtChannelTransport = function(callbackFunction) {
  if (typeof QWebChannel === 'function')
    new QWebChannel(qt.webChannelTransport, function(channel) { // eslint-disable-line no-new
      window._webots = channel.objects._webots;
      if (callbackFunction)
        callbackFunction();
    });
};

webots.userCredentials = function() {
  return '';
};

webots.alert = function(title, message) {
  console.log(title + ': ' + message);
};

// add startsWith() and endsWith() functions to the String prototype
if (typeof String.prototype.startsWith !== 'function') {
  String.prototype.startsWith = function(prefix) {
    return this.slice(0, prefix.length) === prefix;
  };
}

if (typeof String.prototype.endsWith !== 'function') {
  String.prototype.endsWith = function(suffix) {
    return this.indexOf(suffix, this.length - suffix.length) !== -1;
  };
}
