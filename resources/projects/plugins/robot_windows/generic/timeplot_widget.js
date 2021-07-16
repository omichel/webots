/* global Canvas: false */
/* global TimeplotLine: false */

// @param container: DOM element in which to create the plot.
// @param basicTimeStep: world basicTimeStep.
// @param autoRange: cf. AutoRangeType.
// @param yRange: initial range. format `{'min': -1.0, 'max': 1.0}`.
// @param labels: format `{'x': "x-axis", 'y': "y-axis"}`.
// @param device: attached device reference.
function TimeplotWidget(container, basicTimeStep, autoRange, yRange, labels, device, decimals = 3) {
  this.container = container;
  this.basicTimeStep = basicTimeStep;
  this.autoRange = autoRange;
  this.yRange = yRange;
  this.initialYRange = yRange;
  this.labels = labels;
  this.device = device;
  this.decimals = decimals;

  this.initialized = false;
  this.shown = true;
  this.values = []; // store values before the plot is initialized

  // Refresh parameters
  this.lastX = 0;
  this.lastY = [];
  this.lastLabelRefresh = 0; // [simulated seconds]
  this.blockSliderUpdateFlag = false;
  this.pendingRangeLabelsUpdate = false;
  this.pendingSliderPositionUpdate = false;

  this.canvas = null;
  this.plotDiv = null;
  this.plotWidth = undefined;
  this.plotHeight = undefined;
  this.plotLeft = undefined;
  this.plotBottom = undefined;
  this.plotOffscreen = false;

  // Plot
  this.lines = []; // array of TimeplotLine
  this.horizontalGrid = null; // dictionary representing the horizontal grid lines
  this.xOffset = 0;
  this.xRangeSize = 0;

  this.slider = null;
  this.label = null;

  // Compute vertical grid steps.
  this.updateGridConstants();
}

TimeplotWidget.recordDataInBackground = false;

TimeplotWidget.AutoRangeType = {
  NONE: 1, // Fixed range whatever the input data.
  STRETCH: 2, // If the input data is out of the range, then strech the range to see everything.
  JUMP: 3, // If the input data is out of the range, then the range will jump to the closest range having the same size as the initial range.
  ADAPT: 4 // If the input data is out of the range, then stretch the range to see the last values. When possible, shrink back the range to the initial size.
};

// @param value: format `{'x': 0.1, 'y': 0.2}` or `{'x': 0.1, 'y': [0.2, 0.3, 0.5]}`.
TimeplotWidget.prototype.addValue = function(value) {
  if (!TimeplotWidget.recordDataInBackground && this.container.offsetParent === null)
    return;

  const newOffset = value.x - this.xRangeSize;
  if (newOffset > this.xOffset)
    this.xOffset = newOffset;

  const isArray = Array.isArray(value.y);
  const y = isArray ? value.y : [value.y];
  this.lastY = y;
  if (this.initialized) {
    const size = isArray ? value.y.length : 1;
    if (this.lines.length < size)
      this.createLines(size);

    for (let i = 0; i < size; ++i)
      this.lines[i].addPoint(value.x, y[i]);
  } else {
    this.values.push(value);
    while (this.values.length > 0 && this.values[0].x < newOffset)
      this.values.shift();
  }

  if (this.autoRange === TimeplotWidget.AutoRangeType.STRETCH)
    this.stretchRange(value.y);
  else if (this.autoRange === TimeplotWidget.AutoRangeType.JUMP)
    this.jumpToRange(value.y);
  else if (this.autoRange === TimeplotWidget.AutoRangeType.ADAPT) {
    if (this.initialize) {
      this.values.push(value);
      while (this.values.length > 0 && this.values[0].x < newOffset)
        this.values.shift();
    }
    this.adaptRange(value.y, this.values);
  }
};

TimeplotWidget.prototype.setSlider = function(slider) {
  this.slider = slider;
};

TimeplotWidget.prototype.setLabel = function(label) {
  this.label = label;
};

TimeplotWidget.prototype.blockSliderUpdate = function(block) {
  this.blockSliderUpdateFlag = block;
};

TimeplotWidget.prototype.initialize = function() {
  if (!window.WebGLRenderingContext) {
    console.error('This page requires a browser that supports WebGL.');
    return null;
  }

  if (this.initialized)
    return;

  this.initialized = true;
  this.canvas = new Canvas();
  const id = this.container.getAttribute('id');
  this.plotDiv = this.appendChildToContainer('<div id="' + id + '-canvas" class="plot-canvas" />');
  this.computePlotRect();

  this.xLabel = this.appendChildToContainer('<p class="plot-axis-label plot-axis-label-x">' + this.labels['x'] + '</p>');
  this.xMinLabel = this.appendChildToContainer('<p class="plot-axis-label plot-axis-label-x-min">0.0</p>');
  this.xMaxLabel = this.appendChildToContainer('<p class="plot-axis-label plot-axis-label-x-max">0.0</p>');
  this.xMinLabel.textContent = roundLabel(0.0, this.decimals);
  this.xMaxLabel.textContent = roundLabel(this.xRangeSize, this.decimals);

  this.yLabel = this.appendChildToContainer('<p class="plot-axis-label plot-axis-label-y">' + this.labels['y'] + '</p>');
  this.yMinLabel = this.appendChildToContainer('<p class="plot-axis-label plot-axis-label-y-min">' + roundLabel(this.yRange['min'], this.decimals) + '</p>');
  this.yMaxLabel = this.appendChildToContainer('<p class="plot-axis-label plot-axis-label-y-max">' + roundLabel(this.yRange['max'], this.decimals) + '</p>');

  if (this.slider) {
    this.slider.setAttribute('min', this.yRange['min']);
    this.slider.setAttribute('max', this.yRange['max']);
  }

  if (this.values && this.values.length > 0) {
    const linesCount = Array.isArray(this.values[0].y) ? this.values[0].y.length : 1;
    if (this.lines.length < linesCount)
      this.createLines(linesCount);
    if (this.autoRange !== TimeplotWidget.AutoRangeType.ADAPT)
      this.values = [];
  }

  this.show(this.shown);
};

TimeplotWidget.prototype.refresh = function() {
  // Do nothing if the div is hidden.
  if (!this.shown || this.container.offsetParent === null || this.plotOffscreen)
    return;

  // Initialize if required.
  if (!this.initialized)
    this.initialize();

  const gl = this.canvas.getWebglContext();
  gl.enable(gl.SCISSOR_TEST);
  gl.viewport(this.plotLeft, this.plotBottom, this.plotWidth, this.plotHeight);
  gl.scissor(this.plotLeft, this.plotBottom, this.plotWidth, this.plotHeight);
  gl.canvas.style.transform = `translate(${window.scrollX}px, ${window.scrollY}px)`;
  gl.clearColor(1.0, 1.0, 1.0, 1.0);
  gl.clear(gl.COLOR_BUFFER_BIT);

  gl.useProgram(this.canvas.shaderProgram);
  gl.uniformMatrix2fv(this.canvas.scaleUniformLocation, false, [ 2.0 / this.xRangeSize, 0.0, 0.0, 2.0 / (this.yRange['max'] - this.yRange['min']) ]);

  this.displayHorizontalGrid();

  if (this.lines.length > 0 && this.lines[0].size > 0) {
    gl.uniform1f(this.canvas.pointSizeUniformLocation, 0.8);
    gl.uniform2fv(this.canvas.offsetUniformLocation, [this.xOffset, this.yRange['min']]);
    this.lastX = this.lines[0].lastX();
    for (let i = 0; i < this.lines.length; ++i)
      this.drawLine(this.lines[i], this.lines[i].color, 2.0, gl.POINTS);
  }
  gl.disable(gl.SCISSOR_TEST);

  if (this.pendingSliderPositionUpdate)
    this.updateSliderPosition();
};

TimeplotWidget.prototype.show = function(show) {
  this.shown = show;
  if (!this.initialized)
    return;
  const visibility = this.shown ? 'visible' : 'hidden';
  this.plotDiv.style.visibility = visibility;
  this.xLabel.style.visibility = visibility;
  this.xMinLabel.style.visibility = visibility;
  this.xMaxLabel.style.visibility = visibility;
  this.yLabel.style.visibility = visibility;
  this.yMinLabel.style.visibility = visibility;
  this.yMaxLabel.style.visibility = visibility;
  if (this.slider)
    this.slider.style.visibility = visibility;

  if (!this.shown) {
    // clear plot and make it transparent
    const gl = this.canvas.getWebglContext();
    gl.enable(gl.SCISSOR_TEST);
    gl.viewport(this.plotLeft, this.plotBottom, this.plotWidth, this.plotHeight);
    gl.scissor(this.plotLeft, this.plotBottom, this.plotWidth, this.plotHeight);
    gl.canvas.style.transform = `translate(${window.scrollX}px, ${window.scrollY}px)`;
    gl.clearColor(0.0, 0.0, 0.0, 0.0);
    gl.clear(gl.COLOR_BUFFER_BIT);
    gl.disable(gl.SCISSOR_TEST);
  }
};

TimeplotWidget.prototype.resize = function() {
  this.computePlotRect();

  if (!this.slider)
    return;
  // Move slider immediately only if the container is visible.
  if (!this.container.offsetParent)
    this.pendingSliderPositionUpdate = true;
  else
    this.updateSliderPosition();
};

// Private methods
TimeplotWidget.prototype.appendChildToContainer = function(child) {
  let tmp = document.createElement('tmp');
  tmp.innerHTML = child;
  this.container.appendChild(tmp.firstChild);
  return this.container.childNodes[this.container.childNodes.length - 1];
};

TimeplotWidget.prototype.refreshLabels = function() {
  // Refresh the labels only if the container is visible.
  if (!this.initialized || this.container.offsetParent == null || this.container.plotOffscreen)
    return;

  if (this.pendingRangeLabelsUpdate) {
    this.yMinLabel.textContent = roundLabel(this.yRange['min'], this.decimals);
    this.yMaxLabel.textContent = roundLabel(this.yRange['max'], this.decimals);
    this.pendingRangeLabelsUpdate = false;
  }
  if (this.lastLabelRefresh !== this.lastX) {
    // Update the x labels.
    this.xMinLabel.textContent = roundLabel(this.xOffset, this.decimals);
    this.xMaxLabel.textContent = roundLabel(this.xOffset + this.xRangeSize, this.decimals);
  }
  if (this.slider && !this.blockSliderUpdateFlag)
    this.slider.value = this.lastY;
  if (this.label) {
    const v = this.lastY;
    if (v.length > 1) {
      const legend = this.labels['legend'];
      let text = ':' + (legend ? ' ' : '<br>&emsp;&emsp;') + '[ ';
      for (let i = 0; i < v.length; i++) {
        if (i > 0)
          text += ', ';
        if (legend)
          text += '<br>&emsp;&emsp;';
        text += '<span style="color:' + this.labelColorByIndex(i) + '">';
        if (legend && legend.length > i)
          text += ' ' + legend[i] + ': ';
        text += roundLabel(v[i], this.decimals) + '</span>';
      }
      this.label.innerHTML = text + (legend ? '<br>&emsp;&emsp;]' : ' ]');
    } else if (v.length > 0)
      this.label.textContent = ': ' + roundLabel(v[0], this.decimals);
  }
  this.lastLabelRefresh = this.lastX;
};

TimeplotWidget.prototype.createLines = function(size) {
  const gl = this.canvas.getWebglContext();
  for (let i = this.lines.length; i < size; ++i) {
    this.lines.push(new TimeplotLine(this.decimalColorByIndex(size === 1 ? -1 : i), this.plotWidth));
    this.lines[i]._vbo = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, this.lines[i]._vbo);
    gl.bufferData(gl.ARRAY_BUFFER, this.lines[i].xy, gl.STREAM_DRAW);
    gl.bindBuffer(gl.ARRAY_BUFFER, this.lines[i]._vbo);
    this.lines[i]._coord = gl.getAttribLocation(this.canvas.shaderProgram, 'coordinates');
    gl.vertexAttribPointer(this.lines[i]._coord, 2, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(this.lines[i]._coord);

    for (let j = 0; j < this.values.length; ++j) {
      const isArray = Array.isArray(this.values[j]);
      if (isArray)
        this.lines[i].addPoint(this.values[j].x, this.values[j].y[i]);
      else if (i === 0)
        this.lines[i].addPoint(this.values[j].x, this.values[j].y);
    }
  }
};

TimeplotWidget.prototype.drawLine = function(line, color, pointSize, mode) {
  if (line.size <= 0)
    return;

  const gl = this.canvas.getWebglContext();
  gl.uniform1f(this.canvas.pointSizeUniformLocation, pointSize);
  gl.uniform3fv(this.canvas.colorUniformLocation, color);
  gl.bufferData(gl.ARRAY_BUFFER, line.xy, gl.STREAM_DRAW);
  gl.drawArrays(mode, 0, line.xy.length / 2);
};

TimeplotWidget.prototype.displayHorizontalGrid = function() {
  const gl = this.canvas.getWebglContext();
  if (!this.horizontalGrid) {
    this.horizontalGrid = {
      'vbo': gl.createBuffer(),
      'coord': null,
      'xy': new Float32Array(this.plotHeight * 4),
      'maxSize': this.plotHeight,
      'size': 0,
      'changed': true
    };
    gl.bindBuffer(gl.ARRAY_BUFFER, this.horizontalGrid.vbo);
    gl.bufferData(gl.ARRAY_BUFFER, this.horizontalGrid.xy, gl.STREAM_DRAW);
    this.horizontalGrid.coord = gl.getAttribLocation(this.canvas.shaderProgram, 'coordinates');
    gl.vertexAttribPointer(this.horizontalGrid.coord, 2, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(this.horizontalGrid.coord);
  }
  if (this.horizontalGrid.changed) {
    let max = Math.ceil(this.yRange['max'] / this.verticalGridSteps) + 1;
    if (max > this.horizontalGrid.maxSize)
      max = this.horizontalGrid.maxSize;
    const maxX = this.xRangeSize;
    let i = 0;
    for (let y = Math.ceil(this.yRange['min'] / this.verticalGridSteps); y < max; ++y, i += 4) {
      this.horizontalGrid.xy[i] = 0.0; // x1
      this.horizontalGrid.xy[i + 1] = y * this.verticalGridSteps; // y1
      this.horizontalGrid.xy[i + 2] = maxX; // x2
      this.horizontalGrid.xy[i + 3] = this.horizontalGrid.xy[i + 1]; // y2
    }
    this.horizontalGrid.size = i / 2;
    this.horizontalGrid.changed = false;
  }
  this.offsetUniform = [0, this.yRange['min']];
  gl.uniform2fv(this.canvas.offsetUniformLocation, [0, this.yRange['min']]);
  this.drawLine(this.horizontalGrid, [0.867, 0.867, 0.867], 1.0, gl.LINES);
};

TimeplotWidget.prototype.computePlotRect = function() {
  if (!this.initialized)
    return;

  const plotRect = this.plotDiv.getBoundingClientRect();
  const canvas = document.getElementById('canvas');
  this.plotWidth = plotRect.right - plotRect.left - 2; // border 2px
  this.plotHeight = plotRect.bottom - plotRect.top - 2; // border 2px
  this.plotLeft = plotRect.left;
  this.plotBottom = canvas.clientHeight - plotRect.bottom + 1;
  this.plotOffscreen = plotRect.right < 0 || plotRect.left > canvas.clientWidth ||
                       plotRect.bottom < 0 || plotRect.top > canvas.clientHeight;
  this.xRangeSize = this.plotWidth * this.basicTimeStep;

  // Update lines and grid
  for (let i = 0; i < this.lines.length; ++i)
    this.lines[i].resize(this.plotWidth);

  if (this.horizontalGrid && this.horizontalGrid.maxSize < this.plotHeight) {
    const diff = this.horizontalGrid.size - this.plotHeight;
    let j = 0;
    if (diff > 0) {
      j = diff * 4;
      this.horizontalGrid.size = this.plotHeight;
    }
    const pointSize = this.horizontalGrid.size * 4;
    const previousArray = this.horizontalGrid.xy;
    this.horizontalGrid.xy = new Float32Array(this.plotHeight * 4);
    for (let i = 0; i < pointSize; ++i, ++j)
      this.horizontalGrid.xy[i] = previousArray[j];
  }
};

// Jump to a new y range based on the initial range and the new y value.
TimeplotWidget.prototype.jumpToRange = function(y) {
  let jumpUp;
  let jumpDown;
  let yMin, yMax;
  if (Array.isArray(y)) {
    for (let i = 0; i < y.length; ++i) {
      if (!jumpUp) {
        jumpUp = y[i] > this.yRange['max'];
        yMax = y[i];
      }
      if (!jumpDown) {
        jumpDown = y[i] < this.yRange['min'];
        yMin = y[i];
      }
    }
  } else {
    jumpUp = y > this.yRange['max'];
    jumpDown = y < this.yRange['max'];
    yMin = y;
    yMax = y;
  }
  if (jumpUp || jumpDown) {
    const delta = Math.abs(this.initialYRange['max'] - this.initialYRange['min']);
    if (jumpUp) {
      this.yRange['max'] = yMax + 0.05 * delta;
      this.yRange['min'] = this.yRange['max'] - delta;
    } else {
      this.yRange['min'] = yMin - 0.05 * delta;
      this.yRange['max'] = this.yRange['min'] + delta;
    }
    this.updateRange();
  }
};

// Adapt range to current values.
TimeplotWidget.prototype.adaptRange = function(y, values) {
  let min = null;
  let max = null;
  for (let i = 0; i < values.length; ++i) {
    const y = Array.isArray(values[i].y) ? values[i].y : [values[i].y];
    for (let j = 0; j < y.length; j++) {
      if (min == null || y[j] < min)
        min = y[j];
      if (max == null || y[j] > max)
        max = y[j];
    }
  }
  const delta = Math.abs(this.initialYRange['max'] - this.initialYRange['min']);
  let halfOffset = 0;
  if (Math.abs(max - min) < delta)
    halfOffset = (delta - max + min) * 0.5; // Minimum size of range is defined by the initial values.
  else if (y > this.yRange['max'])
    max += 0.05 * delta; // Add small offset to make the value visible in the graph and try to slightly reduce plot redraws.
  else if (y < this.yRange['min'])
    min -= 0.05 * delta; // Add small offset to make the value visible in the graph and try to slightly reduce plot redraws.
  this.yRange['max'] = max + halfOffset;
  this.yRange['min'] = min - halfOffset;
  this.updateRange();
};

// Stretch the y range based on a new y list.
TimeplotWidget.prototype.stretchRange = function(y) {
  var increaseFactor = 1.5; // Increase 50% more than targeted to reduce plot redraws.
  var changed = false;
  if (Array.isArray(y)) {
    for (let i = 0; i < y.length; ++i) {
      const v = y[i];
      if (v < this.yRange['min']) {
        this.yRange['min'] = increaseFactor * v;
        changed = true;
      } else if (v > this.yRange['max']) {
        this.yRange['max'] = increaseFactor * v;
        changed = true;
      }
    }
  } else {
    console.assert(isNumber(y));
    if (y < this.yRange['min']) {
      this.yRange['min'] = increaseFactor * y;
      changed = true;
    } else if (y > this.yRange['max']) {
      this.yRange['max'] = increaseFactor * y;
      changed = true;
    }
  }
  if (changed)
    this.updateRange();
};

// Propagate a modification applied on this.yRange.
TimeplotWidget.prototype.updateRange = function() {
  if (!this.initialized)
    return;

  this.updateGridConstants();

  if (this.yMinLabel && this.yMaxLabel)
    this.pendingRangeLabelsUpdate = true;

  if (this.slider) {
    this.slider.setAttribute('min', this.yRange['min']);
    this.slider.setAttribute('max', this.yRange['max']);
  }
};

TimeplotWidget.prototype.updateGridConstants = function() {
  let delta = Math.abs(this.yRange['max'] - this.yRange['min']);
  delta *= 0.999; // in order to decrease the order of magnitude if delta is a perfect divider of the increment
  const minStep = delta / 10; // initial step size
  const orderOfMagnitude = Math.floor(Math.log(minStep) / Math.LN10);
  const magnitude = Math.pow(10, orderOfMagnitude);
  // Calculate most significant digit of the new step size
  let residual = Math.round(minStep / magnitude + 0.5);
  if (residual > 5.0)
    residual = 10.0;
  else if (residual > 2.0)
    residual = 5.0;
  else if (residual > 1.0)
    residual = 2.0;

  this.verticalGridSteps = magnitude * residual;
  if (this.horizontalGrid)
    this.horizontalGrid.changed = true;
};

TimeplotWidget.prototype.decimalColorByIndex = function(i) {
  if (i < 0) // default for single line plot
    return [0.0, 0.3333, 0.6];
  if (i === 0)
    return [0.6667, 0.2667, 0.2667];
  if (i === 1)
    return [0.2667, 0.6667, 0.2667];
  if (i === 2)
    return [0.2667, 0.2667, 0.6667];
  return [0.2667, 0.2667, 0.2667];
};

TimeplotWidget.prototype.labelColorByIndex = function(i) {
  if (i === 0)
    return 'red';
  if (i === 1)
    return 'green';
  if (i === 2)
    return 'blue';
  return 'black';
};

TimeplotWidget.prototype.updateSliderPosition = function() {
  let parentSection = this.container;
  while (parentSection.className !== 'devices-layout') {
    parentSection = parentSection.parentElement;
    if (!parentSection)
      return;
  }
  if (parentSection.offsetWidth < (this.plotWidth + 30))
    this.slider.classList.add('motor-slider-right');
  else
    this.slider.classList.remove('motor-slider-right');
  this.pendingSliderPositionUpdate = false;
};

function roundLabel(value, decimals = 3) {
  console.assert(isNumber(value));
  if (isNumber(value))
    return parseFloat(value).toFixed(decimals); // select number of decimals
  return value;
}

function isNumber(n) {
  if (Number.isNaN(n) || !Number.isFinite(n) || n.isInn === 'Inf' || n === '-Inf' || n === 'NaN')
    return true;
  return parseFloat(n) === n;
}
