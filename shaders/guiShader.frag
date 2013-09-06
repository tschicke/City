//guiShader.frag
#version 330

in vec2 UV;

out vec3 color;

uniform sampler2D myTextureSampler;

void main(){
	vec2 glUV;
	glUV.x = UV.x;
	glUV.y = 1.0 - UV.y;
	color = texture2D(myTextureSampler, UV).rgb;
}