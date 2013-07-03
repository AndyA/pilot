function Point(x, y, z) {
  this.p = [x, y, z];
}

Point.prototype = {
  transform: function(m) {
    return m.apply(this);
  }
};

function Vector(p1, p2) {
  this.v = [p1, p2];
}

Vector.prototype = {
  transform: function(m) {
    return new Vector(this.v[0].transform(m), this.v[1].transform(m));
  }
}

Vertex.next_id = 1;

function Vertex(p) {
  this.p = p;
  this.l = [];
  this.id = Vertex.next_id++;
}

Vertex.prototype = {
  link: function(v) {
    // free this sucker IE...
    this.l.push(v);
    v.l.push(this);
  },
  transform: function(m) {
    var out = new Vertex(p.transform(m));
    out.l = this.l; // reference
    return out;
  }
}

function Thing() {
  this.v = [];
}

Thing.prototype = {


}

function Transform(scale) {
  if (scale === undefined) scale = 1.0;
  this.m = [
    [scale, 0, 0, 0],
    [0, scale, 0, 0],
    [0, 0, scale, 0],
    [0, 0, 0, scale]];
}

Transform.prototype = {
  append: function(m) {
    var out = new Transform();

    for (var y = 0; y < 4; y++) {
      for (var x = 0; x < 4; x++) {
        out.m[x][y] = 0;
        for (var r = 0; r < 4; r++) out.m[x][y] += this.m[r][y] * m.m[x][r];
      }
    }

    return out;
  },
  invert: function() {
    var s0 = this.m[0][0] * this.m[1][1] - this.m[1][0] * this.m[0][1];
    var s1 = this.m[0][0] * this.m[1][2] - this.m[1][0] * this.m[0][2];
    var s2 = this.m[0][0] * this.m[1][3] - this.m[1][0] * this.m[0][3];
    var s3 = this.m[0][1] * this.m[1][2] - this.m[1][1] * this.m[0][2];
    var s4 = this.m[0][1] * this.m[1][3] - this.m[1][1] * this.m[0][3];
    var s5 = this.m[0][2] * this.m[1][3] - this.m[1][2] * this.m[0][3];

    var c5 = this.m[2][2] * this.m[3][3] - this.m[3][2] * this.m[2][3];
    var c4 = this.m[2][1] * this.m[3][3] - this.m[3][1] * this.m[2][3];
    var c3 = this.m[2][1] * this.m[3][2] - this.m[3][1] * this.m[2][2];
    var c2 = this.m[2][0] * this.m[3][3] - this.m[3][0] * this.m[2][3];
    var c1 = this.m[2][0] * this.m[3][2] - this.m[3][0] * this.m[2][2];
    var c0 = this.m[2][0] * this.m[3][1] - this.m[3][0] * this.m[2][1];

    var det = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
    if (det == 0) throw "Zero determinant";
    var invdet = 1 / det;

    var out = new Transform();

    out.m[0][0] = (this.m[1][1] * c5 - this.m[1][2] * c4 + this.m[1][3] * c3) * invdet;
    out.m[0][1] = (-this.m[0][1] * c5 + this.m[0][2] * c4 - this.m[0][3] * c3) * invdet;
    out.m[0][2] = (this.m[3][1] * s5 - this.m[3][2] * s4 + this.m[3][3] * s3) * invdet;
    out.m[0][3] = (-this.m[2][1] * s5 + this.m[2][2] * s4 - this.m[2][3] * s3) * invdet;

    out.m[1][0] = (-this.m[1][0] * c5 + this.m[1][2] * c2 - this.m[1][3] * c1) * invdet;
    out.m[1][1] = (this.m[0][0] * c5 - this.m[0][2] * c2 + this.m[0][3] * c1) * invdet;
    out.m[1][2] = (-this.m[3][0] * s5 + this.m[3][2] * s2 - this.m[3][3] * s1) * invdet;
    out.m[1][3] = (this.m[2][0] * s5 - this.m[2][2] * s2 + this.m[2][3] * s1) * invdet;

    out.m[2][0] = (this.m[1][0] * c4 - this.m[1][1] * c2 + this.m[1][3] * c0) * invdet;
    out.m[2][1] = (-this.m[0][0] * c4 + this.m[0][1] * c2 - this.m[0][3] * c0) * invdet;
    out.m[2][2] = (this.m[3][0] * s4 - this.m[3][1] * s2 + this.m[3][3] * s0) * invdet;
    out.m[2][3] = (-this.m[2][0] * s4 + this.m[2][1] * s2 - this.m[2][3] * s0) * invdet;

    out.m[3][0] = (-this.m[1][0] * c3 + this.m[1][1] * c1 - this.m[1][2] * c0) * invdet;
    out.m[3][1] = (this.m[0][0] * c3 - this.m[0][1] * c1 + this.m[0][2] * c0) * invdet;
    out.m[3][2] = (-this.m[3][0] * s3 + this.m[3][1] * s1 - this.m[3][2] * s0) * invdet;
    out.m[3][3] = (this.m[2][0] * s3 - this.m[2][1] * s1 + this.m[2][2] * s0) * invdet;

    return out;
  },
  apply: function(p) {
    var out = new Point(0, 0, 0);
    for (var y = 0; y < 3; y++) {
      out.p[y] = this.m[3][y];
      for (var x = 0; x < 3; x++) out.p[y] += p.p[x] * this.m[x][y];
    }
    return out;
  }
}
