import WbVector3 from './WbVector3.js';
import WbVector4 from './WbVector4.js';

export default class WbWrenMeshBuffers {
  constructor(verticesCount, indicesCount, texCoordSetsCount) {
    this.vertexIndex = 0;
    this.index = 0;

    this.resetAll(verticesCount, indicesCount, texCoordSetsCount);
  }

  clear() {
    this.vertexBuffer = undefined;
    this.normalBuffer = undefined;
    this.texCoordBuffer = undefined;
    this.unwrappedTexCoordsBuffer = undefined;
    this.indexBuffer = undefined;

    this.vertexIndex = 0;
    this.index = 0;
  }

  resetAll(verticesCount, indicesCount, texCoordSetsCount) {
    this.clear();

    this.verticesCount = verticesCount;
    this.indicesCount = indicesCount;
    this.texCoordSetsCount = texCoordSetsCount;
    this.vertexBuffer = [];
    this.normalBuffer = [];
    if (texCoordSetsCount > 0) {
      this.texCoordBuffer = [];
      this.unwrappedTexCoordsBuffer = [];
    }
    this.indexBuffer = [];
  }

  // Static functions

  static writeCoordinates(x, y, z, m, buffer, index) {
    const result = m.mulByVec4(new WbVector4(x, y, z, 1.0));
    buffer[index] = result.x;
    buffer[index + 1] = result.y;
    buffer[index + 2] = result.z;
  }

  static writeNormal(x, y, z, m, buffer, index) {
    const result = m.mulByVec3(new WbVector3(x, y, z));
    buffer[index] = result.x;
    buffer[index + 1] = result.y;
    buffer[index + 2] = result.z;
  }
}
