export default class WbVector4 {
  constructor(x = 0.0, y = 0.0, z = 0.0, w = 0.0) {
    this.x = x;
    this.y = y;
    this.z = z;
    this.w = w;
  }

  div(number) {
    return new WbVector4(this.x / number, this.y / number, this.z / number, this.w / number);
  }

  clone() {
    return new WbVector4(this.x, this.y, this.z, this.w);
  }
}
