//guiShader.vert
#version 330

uniform mat4 modelMatrix, projectionMatrix;

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec2 vertexUV;

out vec2 UV;

void main(){
	UV = vertexUV;//TODO flip texture to OGL coords
	
	vec4 vertexPosWindowSpace = projectionMatrix * modelMatrix * vec4(vec3(vertexPosition, 0), 1);
	gl_Position = vertexPosWindowSpace;
}