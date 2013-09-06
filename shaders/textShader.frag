//textShader.frag
#version 330

in vec2 UV;

out vec4 color;

uniform sampler2D myTextureSampler;

void main(){
	vec2 glUV;
	glUV.x = UV.x;
	glUV.y = 1.0 - UV.y;
	color.rgb = texture2D(myTextureSampler, UV).rgb;
	
	if(color.rgb == vec3(1, 0, 1)){
		color.a = 0;
	} else {
		color.a = 1;
	}
}