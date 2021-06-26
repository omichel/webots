export default class WbVector3 {
  constructor(x = 0.0, y = 0.0, z = 0.0) {
    this.x = x;
    this.y = y;
    this.z = z;
  }

  add(vector) {
    return new WbVector3(this.x + vector.x, this.y + vector.y, this.z + vector.z);
  }

  almostEquals(vector, tolerance) {
    return Math.abs(this.x - vector.x) < tolerance && Math.abs(this.y - vector.y) < tolerance && Math.abs(this.z - vector.z) < tolerance;
  }

  // angle between two vectors (in radians)
  angle(vector) {
    const s = this.dot(vector) / Math.sqrt(this.length2() * vector.length2());
    return (s >= 1.0) ? 0 : (s <= -1.0) ? Math.PI : Math.acos(s);
  }

  cross(vector) {
    return new WbVector3(this.y * vector.z - this.z * vector.y, this.z * vector.x - this.x * vector.z, this.x * vector.y - this.y * vector.x);
  }

  div(number) {
    return new WbVector3(this.x / number, this.y / number, this.z / number);
  }

  dot(vector) {
    return this.x * vector.x + this.y * vector.y + this.z * vector.z;
  }

  equal(vector) {
    return this.x === vector.x && this.y === vector.y && this.z === vector.z;
  }

  get(index) {
    if (index === 0)
      return this.x;
    else if (index === 1)
      return this.y;
    else if (index === 2)
      return this.z;
  }

  isNull() {
    return this.x === 0.0 && this.y === 0.0 && this.z === 0.0;
  }

  length() {
    return Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z);
  }

  length2() {
    return this.x * this.x + this.y * this.y + this.z * this.z;
  }

  mul(number) {
    return new WbVector3(this.x * number, this.y * number, this.z * number);
  }

  normalize() {
    const result = this.div(this.length());
    this.x = result.x;
    this.y = result.y;
    this.z = result.z;
  }

  normalized() {
    return this.div(this.length());
  }

  setXyz(x, y, z) {
    this.x = x;
    this.y = y;
    this.z = z;
  }

  sub(vector) {
    return new WbVector3(this.x - vector.x, this.y - vector.y, this.z - vector.z);
  }

  clone() {
    return new WbVector3(this.x, this.y, this.z);
  }
}
