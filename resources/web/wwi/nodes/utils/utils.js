import WbVector3 from './WbVector3.js';
import WbVector4 from './WbVector4.js';
import WbBillboard from '../WbBillboard.js';
import WbTransform from '../WbTransform.js';
import WbWorld from '../WbWorld.js';

let undefinedID = -1; // Negative IDs are assigned to nodes provided by Webots without IDs.

function array3Pointer(x, y, z) {
  const data = new Float32Array([x, y, z]);
  const nDataBytes = data.length * data.BYTES_PER_ELEMENT;
  const dataPtr = Module._malloc(nDataBytes);
  const dataHeap = new Uint8Array(Module.HEAPU8.buffer, dataPtr, nDataBytes);
  dataHeap.set(new Uint8Array(data.buffer));

  return dataHeap.byteOffset;
}

function arrayXPointer(array) {
  const data = new Uint8ClampedArray(array);
  const nDataBytes = data.length * data.BYTES_PER_ELEMENT;
  const dataPtr = Module._malloc(nDataBytes);
  const dataHeap = new Uint8Array(Module.HEAPU8.buffer, dataPtr, nDataBytes);
  dataHeap.set(new Uint8Array(data.buffer));

  return dataHeap.byteOffset;
}

function arrayXPointerInt(array) {
  const data = new Int32Array(array);
  const nDataBytes = data.length * data.BYTES_PER_ELEMENT;
  const dataPtr = Module._malloc(nDataBytes);
  const dataHeap = new Uint8Array(Module.HEAPU8.buffer, dataPtr, nDataBytes);
  dataHeap.set(new Uint8Array(data.buffer));

  return dataHeap.byteOffset;
}

function arrayXPointerFloat(array) {
  const data = new Float32Array(array);
  const nDataBytes = data.length * data.BYTES_PER_ELEMENT;
  const dataPtr = Module._malloc(nDataBytes);
  const dataHeap = new Uint8Array(Module.HEAPU8.buffer, dataPtr, nDataBytes);
  dataHeap.set(new Uint8Array(data.buffer));

  return dataHeap.byteOffset;
}

function direction(vec4) {
  const c = Math.cos(vec4.w);
  const s = Math.sin(vec4.w);
  const t = 1 - c;
  const tTimesZ = t * vec4.z;
  return new WbVector3(tTimesZ * vec4.x + s * vec4.y, tTimesZ * vec4.y - s * vec4.x, tTimesZ * vec4.z + c);
}

function findUpperTransform(node) {
  if (typeof node === 'undefined')
    return undefined;

  let n = WbWorld.instance.nodes.get(node.parent);
  while (typeof n !== 'undefined') {
    if (n instanceof WbTransform)
      return n;
    else
      n = n.parent;
  }
  return undefined;
}

function fromAxisAngle(x, y, z, angle) {
  const result = new WbVector4();
  let l = x * x + y * y + z * z;
  if (l > 0.0) {
    angle *= 0.5;
    result.w = Math.cos(angle);
    l = Math.sin(angle) / Math.sqrt(l);
    result.x = x * l;
    result.y = y * l;
    result.z = z * l;
  } else {
    result.w = 1.0;
    result.x = 0.0;
    result.y = 0.0;
    result.z = 0.0;
  }
  return result;
}

function getAncestor(node) {
  if (typeof node !== 'undefined' && typeof node.parent !== 'undefined') {
    let parent = WbWorld.instance.nodes.get(node.parent);

    if (typeof parent !== 'undefined')
      return getAncestor(parent);
  }

  return node;
}

function getAnId() {
  return 'n' + undefinedID--;
}

function length(vec3) {
  return Math.sqrt(vec3.x * vec3.x + vec3.y * vec3.y + vec3.z * vec3.z);
}

function nodeIsInBoundingObject(node) {
  if (typeof node === 'undefined' || typeof node.parent === 'undefined')
    return false;

  const parent = WbWorld.instance.nodes.get(node.parent);
  if (typeof parent !== 'undefined') {
    if (parent instanceof WbTransform && typeof parent.boundingObject !== 'undefined')
      return parent.boundingObject === node;
    else if (typeof parent.parent !== 'undefined')
      return nodeIsInBoundingObject(parent);
  }

  return false;
}

function isDescendantOfBillboard(node) {
  while (typeof node !== 'undefined') {
    if (node instanceof WbBillboard)
      return true;

    node = WbWorld.instance.nodes.get(node.parent);
  }

  return false;
}

function pointerOnFloat(float) {
  const data = new Float32Array(1);
  data[0] = float;
  const nDataBytes = data.length * data.BYTES_PER_ELEMENT;
  const dataPtr = Module._malloc(nDataBytes);
  const dataHeap = new Uint8Array(Module.HEAPU8.buffer, dataPtr, nDataBytes);
  dataHeap.set(new Uint8Array(data.buffer));

  return dataHeap.byteOffset;
}

function quaternionToVec4(quat) {
  let angle;
  if (quat.w >= 1.0)
    angle = 0.0;
  else if (quat.w <= -1.0)
    angle = 2.0 * Math.PI;
  else
    angle = 2.0 * Math.acos(quat.w);

  // normalise axes
  let div = Math.sqrt(quat.x * quat.x + quat.y * quat.y + quat.z * quat.z);
  if (div === 0)
    div = 1;
  const inv = 1.0 / div;
  const x = quat.x * inv;
  const y = quat.y * inv;
  const z = quat.z * inv;

  return new WbVector4(x, y, z, angle);
}

function right(vec4) {
  const c = Math.cos(vec4.w);
  const s = Math.sin(vec4.w);
  const t = 1 - c;
  const tTimesX = t * vec4.x;
  return new WbVector3(tTimesX * vec4.x + c, tTimesX * vec4.y + s * vec4.z, tTimesX * vec4.z - s * vec4.y);
}

function up(vec4) {
  const c = Math.cos(vec4.w);
  const s = Math.sin(vec4.w);
  const t = 1 - c;
  const tTimesY = t * vec4.y;
  return new WbVector3(tTimesY * vec4.x - s * vec4.z, tTimesY * vec4.y + c, tTimesY * vec4.z + s * vec4.x);
}

function vec4ToQuaternion(vec4) {
  const halfAngle = 0.5 * vec4.w;
  const sinusHalfAngle = Math.sin(halfAngle);
  const cosinusHalfAngle = Math.cos(halfAngle);
  return glm.quat(cosinusHalfAngle, vec4.x * sinusHalfAngle, vec4.y * sinusHalfAngle, vec4.z * sinusHalfAngle);
}

export {array3Pointer, arrayXPointer, arrayXPointerInt, arrayXPointerFloat, pointerOnFloat, direction, up, right, length, vec4ToQuaternion, quaternionToVec4, fromAxisAngle, findUpperTransform, nodeIsInBoundingObject, isDescendantOfBillboard, getAncestor, getAnId};
