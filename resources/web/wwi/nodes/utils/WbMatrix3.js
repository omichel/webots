import WbVector3 from './WbVector3.js';

export default class WbMatrix3 {
  constructor(m0 = 1.0, m1 = 0, m2 = 0, m3 = 0, m4 = 1.0, m5 = 0, m6 = 0, m7 = 0, m8 = 1.0) {
    this.m = [9];
    this.m[0] = m0;
    this.m[1] = m1;
    this.m[2] = m2;
    this.m[3] = m3;
    this.m[4] = m4;
    this.m[5] = m5;
    this.m[6] = m6;
    this.m[7] = m7;
    this.m[8] = m8;
  }

  mulByVec3(vector) {
    return new WbVector3(this.m[0] * vector.x + this.m[1] * vector.y + this.m[2] * vector.z,
      this.m[3] * vector.x + this.m[4] * vector.y + this.m[5] * vector.z,
      this.m[6] * vector.x + this.m[7] * vector.y + this.m[8] * vector.z);
  }
}
