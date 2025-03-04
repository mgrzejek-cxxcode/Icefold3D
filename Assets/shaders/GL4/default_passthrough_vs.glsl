
#version 430 core

layout( location = 0, semantic = POSITION ) in vec3 vPosition;
layout( location = 1, semantic = NORMAL ) in vec3 vNormal;
layout( location = 4, semantic = COLOR ) in vec4 vColor;
layout( location = 5, semantic = TEXCOORD0 ) in vec2 vTexCoord0;

layout( binding = 0, std140 ) uniform CB0
{
	layout(row_major) mat4 cb0ModelMatrix;
	layout(row_major) mat4 cb0ViewMatrix;
	layout(row_major) mat4 cb0ProjectionMatrix;
};

out gl_PerVertex
{
	vec4  gl_Position;
	float gl_PointSize;
	float gl_ClipDistance[];
};

smooth out vec4 psColor;
smooth out vec2 psTexCoord0;

void main()
{
	psColor = vColor;
	psTexCoord0 = vTexCoord0;
	gl_Position = cb0ProjectionMatrix * cb0ViewMatrix * cb0ModelMatrix * vec4( vPosition, 1.0 );
}
