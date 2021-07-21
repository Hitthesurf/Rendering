precision highp float;
varying vec2 fragTexCoord;
attribute vec3 vertPosition;
attribute vec2 texturePos;
uniform mat4 TTO;
uniform mat4 RTA;
uniform mat4 Proj;
uniform mat4 TTR; uniform mat4 RS; uniform mat4 TTRB;
highp mat4 T(in highp mat4 inMatrix) {highp vec4 i0 = inMatrix[0];highp vec4 i1 = inMatrix[1];highp vec4 i2 = inMatrix[2];highp vec4 i3 = inMatrix[3];
highp mat4 outMatrix = mat4(vec4(i0.x, i1.x, i2.x, i3.x),vec4(i0.y, i1.y, i2.y, i3.y),vec4(i0.z, i1.z, i2.z, i3.z),vec4(i0.w, i1.w, i2.w, i3.w) );

return outMatrix;}
void main()
{
vec4 Pos = T(Proj)*T(RTA)*T(TTO)*T(TTRB)*RTA*T(RS)*T(TTR)*vec4(vertPosition, 1.0);
gl_Position = Pos.xyzw;
fragTexCoord = vec2(texturePos.x, 1.0-texturePos.y);
}