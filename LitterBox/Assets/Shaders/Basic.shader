#shader vertex
#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 aCol;
layout(location = 4) in float texIndex;

layout(location = 7) out vec3 vCol;
layout(location = 1) out vec2 v_TexCoord;

out float tIO;

uniform float z_val;

void main()
{
	gl_Position = vec4(position, z_val, 1.0);
	vCol = aCol;
	v_TexCoord = texCoord;
	tIO = texIndex;
};


#shader fragment
#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 7) in vec3 v_Color;
layout(location = 1) in vec2 v_TexCoord;
layout(location = 0) out vec4 color;

in float tIO;

uniform sampler2D u_SamplerID[8];

void main()
{
	if(tIO >= 0.0) {
		//TODO set 32 cases to accomodate for texture amounts
		switch(int(tIO)) {
			case 0:
			color = texture(u_SamplerID[0], v_TexCoord);
			break;
			case 1:
			color = texture(u_SamplerID[1], v_TexCoord);
			break;
			case 2:
			color = texture(u_SamplerID[2], v_TexCoord);
			break;
			case 3:
			color = texture(u_SamplerID[3], v_TexCoord);
			break;
			case 4:
			color = texture(u_SamplerID[4], v_TexCoord);
			break;
		} 
		//color = texture(u_SamplerID[int(tIO)], v_TexCoord);
	}
	else {
		color = vec4(v_Color,1.0);
	}
};