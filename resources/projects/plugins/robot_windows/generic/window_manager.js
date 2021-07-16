/* global DeviceWidget, TimeplotWidget, PlotWidget, RadarWidget, Canvas */
/* exported menuTabCallback */
/* exported openMenu */
/* exported closeMenu */
/* exported configureDevices */
/* exported addSettingsTab */
/* exported parseJSONMessage */
/* exported enableAllDevicesCallback */
/* exported setupWindow */
/* exported refreshSelectedTab */
/* eslint no-unused-vars: ["error", { "varsIgnorePattern": "Callback", "argsIgnorePattern": "^_"}] */

const REFRESH_LABELS_RATE = 3; // Hz
const REFRESH_CONTENT_RATE = 10; // Hz

var windowIsHidden;
var selectedDeviceType = null;
var selectedTabModified = false;

function menuTabCallback(deviceType) {
  if (document.getElementById(deviceType + '-section').style.display === 'block')
    // tab already selected
    return;

  let i, x, tablinks;
  x = document.getElementsByClassName('devices-container');
  for (i = 0; i < x.length; ++i)
    x[i].style.display = 'none';
  tablinks = document.getElementsByClassName('menu-button');
  for (i = 0; i < tablinks.length; ++i)
    tablinks[i].className = tablinks[i].className.replace(' menu-button-selected', '');
  document.getElementById(deviceType + '-section').style.display = 'block';
  document.getElementById(deviceType + '-menu-button').className += ' menu-button-selected';
  selectedDeviceType = deviceType;

  // Clear canvas
  // Plots will be refreshed when the animation is over
  const canvas = new Canvas();
  canvas.clearCanvas();
  
  window.scrollTo(0, 0);
}

function refreshSelectedTab() {
  selectedTabModified = true;
};

function refreshLabels() {
  if (!selectedDeviceType || !selectedTabModified)
    return;
  const tabWidgets = window.widgets[selectedDeviceType];
  if (tabWidgets) {
    Object.keys(tabWidgets).forEach(function(deviceName) {
      if (typeof tabWidgets[deviceName].refreshLabels === 'function')
        tabWidgets[deviceName].refreshLabels();
    });
  }
  selectedTabModified = false;
}

function refreshContent() {
  if (!selectedDeviceType || !selectedTabModified)
    return;
  const tabWidgets = window.widgets[selectedDeviceType];
  if (!tabWidgets)
    return;
  Object.keys(tabWidgets).forEach(function(deviceName) {
    tabWidgets[deviceName].refresh();
  });
};

function updateTabCallback() {
  const canvas = new Canvas();
  canvas.resizeCanvas();
  if (selectedDeviceType) {
    const tabWidgets = window.widgets[selectedDeviceType];
    if (!tabWidgets)
      return;
    Object.keys(tabWidgets).forEach(function(deviceName) {
      const widget = tabWidgets[deviceName];
      if (widget) {
        widget.resize();
        widget.refresh(true);
      }
    });
  }
}

function openMenu() {
  document.getElementById('menu').style.display = 'flex';
  document.getElementById('menu-open-button').style.display = 'none';
  document.getElementById('content').style.marginLeft = document.getElementById('menu').offsetWidth + 'px';
  updateTabCallback();
}

function closeMenu() {
  document.getElementById('menu-open-button').style.display = 'inline';
  document.getElementById('menu').style.display = 'none';
  document.getElementById('content').style.marginLeft = '0px';
  updateTabCallback();
}

function appendNewElement(id, newElement) {
  const tmp = document.createElement('tmp');
  tmp.innerHTML = newElement;
  const container = document.getElementById(id);
  container.appendChild(tmp.firstChild);
  return container.childNodes[container.childNodes.length - 1];
}

function addTab(type, isDevice, deviceSwitch) {
  if (document.getElementById(type + '-section'))
    return; // check if already exists

  let buttonsDiv = '';
  if (deviceSwitch && type !== 'RotationalMotor' && type !== 'LinearMotor') {
    buttonsDiv += '<div class="device-mode-switch">' +
      '<div>Override controller' +
      '<label id="' + type + '-switch" class="switch">' +
        '<input id="' + type + '-mode-checkbox" type="checkbox" onclick="setDeviceModeCallback(this, \'' + type + '\')">' +
        '<span class="device-mode-slider"></span>' +
      '</label></div>' +
      '<div>If enabled, the devices will be enabled/disabled from their checkbox</div>' +
      '</div>';
  }
  if (isDevice) {
    buttonsDiv += '<div id="' + type + '-buttons" class="device-buttons">' +
      '<input type="button" value="Disable all" id="' + type + '-disable-button" class="device-button" onclick="enableAllDevicesCallback(\'' + type + '\', false)"/>' +
      '<input type="button" value="Enable all" id="' + type + '-enable-button" class="device-button" onclick="enableAllDevicesCallback(\'' + type + '\', true)"/>' +
    '</div>';
  } else
    buttonsDiv = '';

  const section = appendNewElement('content',
    '<div id="' + type + '-section" class="devices-container animate-left">' +
      '<h1>' + type + '</h1>' + buttonsDiv +
      '<section id="' + type + '-layout" class="devices-layout"/>' +
    '</div>'
  );
  appendNewElement('menu',
    '<button id="' + type + '-menu-button" class="menu-button tablink" onclick="menuTabCallback(\'' + type + '\')">' + type + '</button>'
  );

  section.addEventListener('webkitAnimationEnd', updateTabCallback, false);
  section.addEventListener('animationend', updateTabCallback, false);
  section.addEventListener('oanimationend', updateTabCallback, false);
}

function setDeviceModeCallback(_checkbox, _deviceType) {
  // to be overriden
}

function enableAllDevicesCallback(deviceType, enabled) {
  if (deviceType) {
    Object.keys(window.widgets[deviceType]).forEach(function(deviceName) {
      window.widgets[deviceType][deviceName].enable(enabled);
    });
    return;
  }
  Object.keys(window.widgets).forEach(function(deviceType) {
    Object.keys(window.widgets[deviceType]).forEach(function(deviceName) {
      window.widgets[deviceType][deviceName].enable(enabled);
    });
  });
};

function configureDevices(data, controlDevices) {
  if (data.devices == null) {
    document.getElementById('no-controller-label').innerHTML = 'No devices.';
    closeMenu();
    return;
  }

  // Parse the devices once to prepare de device type list.
  let deviceTypes = [];
  data.devices.forEach(function(device) {
    if (DeviceWidget.supportedDeviceTypes.indexOf(device.type) >= 0 && deviceTypes.indexOf(device.type) < 0)
      deviceTypes.push(device.type);
  });

  // Sort the deviceTypes alphabetically.
  deviceTypes.sort(alphabetical);

  // Create a device type container per device types.
  deviceTypes.forEach(function(deviceType) {
    addTab(deviceType, true, controlDevices);
  });

  // Create a device container per device.
  data.devices.forEach(function(device) {
    if (DeviceWidget.supportedDeviceTypes.indexOf(device.type) >= 0) {
      device.htmlName = device.name;
      device.name = device.name.replace(/&quot;/g, '"');
      device.model = device.model.replace(/&quot;/g, '"');
      DeviceWidget.createWidget(data.basicTimeStep, device);
    }
  });

  window.widgets = Object.assign({}, DeviceWidget.widgets);
  return deviceTypes;
}

function addSettingsTab() {
  addTab('Settings', false);
  let div = '<div id="settings" class="settings">';
  div += '<h2>Generic robot window settings</h2>';
  div += '<div class="settings-content">Refresh rate: <input id="refresh-rate-number" value="32"> ms</div>';
  div += '<div class="settings-content"><input type="checkbox" title="Record data when plot is hidden." id="background-data-checkbox" onclick="backgroundDataCheckboxCallback(this)"/>Record data when plot is hidden</div>';
  div += '<div class="settings-content"><input type="button" value="Disable all devices" id="disable-all-button" class="device-button" onclick="enableAllDevicesCallback(null, false)"/></div>';
  div += '</div>';
  appendNewElement('Settings-layout', div);
  document.getElementById('refresh-rate-number').addEventListener('input', function(e) {
    let value = e.target.value;
    if (!value || value.length === 0 || isNaN(value)) {
      console.log('Robot window refresh rate is not a valid number');
      return;
    }
    window.robotWindow.send('refresh-rate ' + value);
  });
}

function backgroundDataCheckboxCallback(checkbox) {
  TimeplotWidget.recordDataInBackground = checkbox.checked;
  PlotWidget.recordDataInBackground = checkbox.checked;
  RadarWidget.recordDataInBackground = checkbox.checked;
}

function setupWindow() {
  // get .device-content { with: XXXpx; height: XXXpx; }
  const tmp = document.createElement('div');
  tmp.classList.add('device-content');
  document.body.appendChild(tmp);
  const deviceContentWidth = window.getComputedStyle(document.body.lastChild).getPropertyValue('width').replace(/px$/g, '');
  const deviceContentHeight = window.getComputedStyle(document.body.lastChild).getPropertyValue('height').replace(/px$/g, '');
  document.body.removeChild(document.body.lastChild);

  windowIsHidden = document.visibilityState === 'hidden';
  window.robotWindow.send('configure { "imageMaxWidth": ' + deviceContentWidth + ', "imageMaxHeight": ' + deviceContentHeight + ', "hidden": ' + (windowIsHidden ? 1 : 0) + ' }');
  document.addEventListener('visibilitychange', function() {
    windowIsHidden = document.visibilityState === 'hidden';
    window.robotWindow.send('window ' + (windowIsHidden ? 'hidden' : 'visible'));
  });

  window.addEventListener('resize', updateTabCallback);
  window.addEventListener('scroll', updateTabCallback);
  setInterval(function() { refreshLabels(); }, 1000 / REFRESH_LABELS_RATE);
  setInterval(function() { refreshContent(); }, 1000 / REFRESH_CONTENT_RATE);
}

function alphabetical(a, b) {
  const A = a.toLowerCase();
  const B = b.toLowerCase();
  if (A < B)
    return -1;
  else if (A > B)
    return 1;
  else
    return 0;
}

function parseJSONMessage(message) {
  let data;
  try {
    data = JSON.parse(message);
  } catch (e) {
    console.log('C to JS protocol error:');
    console.log(e);
    console.log(message);
  }
  return data;
}
