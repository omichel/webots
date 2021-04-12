import {WbTriangleMeshGeometry} from './wbTriangleMeshGeometry.js';

class WbIndexedFaceSet extends WbTriangleMeshGeometry {
  constructor(id, coordIndex, normalIndex, texCoordIndex, coord, texCoord, normal, ccw) {
    super(id);

    this.coordIndex = coordIndex;
    this.normalIndex = normalIndex;
    this.texCoordIndex = texCoordIndex;

    this.coord = coord;
    this.texCoord = texCoord;
    this.normal = normal;

    this.ccw = ccw;
  }

  updateTriangleMesh() {
    this.triangleMesh.init(this.coord, this.coordIndex, this.normal, this.normalIndex, this.texCoord, this.texCoordIndex, this.ccw);
  }

  clone(customID) {
    this.useList.push(customID);
    return new WbIndexedFaceSet(customID, this.coordIndex, this.normalIndex, this.texCoordIndex, this.coord, this.texCoord, this.normal, this.ccw, this.normalPerVertex);
  }
}

export {WbIndexedFaceSet};
