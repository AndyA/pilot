// Tests for Transform
module("Transform");
test("log", function() {
  var a = new Transform();
  var b = a.invert();
  deepEqual(b, a, 'invert unity matrix == unity matrix');
});
