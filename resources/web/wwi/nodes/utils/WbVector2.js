export default class WbVector2 {
  constructor(x = 0.0, y = 0.0) {
    this.x = x;
    this.y = y;
  }

  add(vector) {
    return new WbVector2(this.x + vector.x, this.y + vector.y);
  }

  // angle between two vectors (in radians)
  angle(vector) {
    const s = this.dot(vector) / Math.sqrt(this.length2() * vector.length2());
    return (s >= 1.0) ? 0 : (s <= -1.0) ? Math.PI : Math.acos(s);
  }

  div(number) {
    return new WbVector2(this.x / number, this.y / number);
  }

  dot(v) {
    return this.x * v.x + this.y * v.y;
  }

  equal(vector) {
    return this.x === vector.x && this.y === vector.y;
  }

  get(index) {
    if (index === 0)
      return this.x;
    else if (index === 1)
      return this.y;
  }

  isNull() {
    return this.x === 0.0 && this.y === 0.0;
  }

  length() {
    return Math.sqrt(this.x * this.x + this.y * this.y);
  }

  length2() {
    return this.x * this.x + this.y * this.y;
  }

  mul(number) {
    return new WbVector2(this.x * number, this.y * number);
  }

  normalize() {
    const result = this.div(this.length());
    this.x = result.x;
    this.y = result.y;
  }

  normalized() {
    return this.div(this.length());
  }

  sub(vector) {
    return new WbVector2(this.x - vector.x, this.y - vector.y);
  }
}
