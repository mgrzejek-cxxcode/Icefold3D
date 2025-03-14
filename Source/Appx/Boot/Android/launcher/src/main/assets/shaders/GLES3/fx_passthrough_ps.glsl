#version 310 es

precision highp float;

in vec2 psTexCoord0;

layout( binding = 0 ) uniform sampler2D uSampler0;

layout( location = 0 ) out vec4 outPixelColor;

void main()
{
	vec4 sampledColor = texture( uSampler0, psTexCoord0 );
	outPixelColor = vec4( sampledColor.xyz, 1.0f );
}