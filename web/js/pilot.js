$(function() {

  var WIDTH = 1280;
  var HEIGHT = 720;
  var VANISH = 1000;
  var PERSPECTIVE = 1000;

  function project(p) {
    var sc = (p.p[2] + VANISH) / PERSPECTIVE;
    return ([p.p[0] * sc, p.p[1] * sc]);
  }

  function line(ctx, p1, p2) {
    var r1 = project(p1);
    var r2 = project(p2);

    console.log('[' + r1[0] + ', ' + r1[1] + ']-[' + r2[0] + ', ' + r2[1] + ']');

    ctx.beginPath();
    ctx.moveTo(r1[0] + WIDTH / 2, HEIGHT / 2 - r1[1]);
    ctx.lineTo(r2[0] + WIDTH / 2, HEIGHT / 2 - r2[1]);
    ctx.closePath();
    ctx.stroke();
  }

  function _render(ctx, v, m, seen) {
    if (seen.hasOwnProperty(v.id)) return;
    seen[v.id] = true;
    console.log("from " + v.id);
    var p1 = v.p.transform(m);
    for (var i = 0; i < v.l.length; i++) {
      var v2 = v.l[i];
//      if (seen.hasOwnProperty(v2.id)) continue;
      console.log("  to " + v2.id);
      var p2 = v2.p.transform(m);
      line(ctx, p1, p2);
      _render(ctx, v2, m, seen);
    }
  }

  function render(ctx, v, m) {
    _render(ctx, v, m, {});
  }

  var $display = $('#display');
  var ctx = $display[0].getContext('2d');

  var v = [
  new Vertex(new Point(-1, -1, -1)), //
  new Vertex(new Point(-1, -1, 1)), //
  new Vertex(new Point(-1, 1, -1)), //
  new Vertex(new Point(-1, 1, 1)), //
  new Vertex(new Point(1, -1, -1)), //
  new Vertex(new Point(1, -1, 1)), //
  new Vertex(new Point(1, 1, -1)), //
  new Vertex(new Point(1, 1, 1))];

  v[0].link(v[1]);
  v[0].link(v[2]);
  v[0].link(v[4]);
  v[1].link(v[3]);
  v[1].link(v[5]);
  v[2].link(v[3]);
  v[2].link(v[6]);
  v[3].link(v[7]);
  v[4].link(v[5]);
  v[4].link(v[6]);
  v[5].link(v[7]);
  v[6].link(v[7]);

  var view = new Transform(200);
  render(ctx, v[0], view);
});
