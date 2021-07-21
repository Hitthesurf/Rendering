precision highp float;
varying vec2 fragTexCoord;
uniform sampler2D sampler0;
void main()
{
gl_FragColor = texture2D(sampler0, fragTexCoord);
}