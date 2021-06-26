export default class WbWorld {
  constructor() {
    this.hasFog = false;
    this.coordinateSystem = 'ENU';
    this.upVector = glm.vec3(0, 0, 1);
    // Only the top level nodes are represented here
    this.sceneTree = [];
    // All the nodes are included here so it is easier to retrieve them for updates
    // map from id to node
    this.nodes = new Map();

    this.billboards = [];
    this.readyForUpdates = false;
  }

  static init() {
    WbWorld.instance = new WbWorld();
  }

  static computeUpVector() {
    WbWorld.instance.upVector = glm.vec3(WbWorld.instance.coordinateSystem[0] === 'U' ? 1 : 0, WbWorld.instance.coordinateSystem[1] === 'U' ? 1 : 0, WbWorld.instance.coordinateSystem[2] === 'U' ? 1 : 0);
  }
}
