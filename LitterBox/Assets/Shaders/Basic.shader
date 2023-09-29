#shader vertex
#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 aCol;
layout(location = 3) in float texIndex;
layout(location = 4) in vec3 width_height_type;
layout(location = 5) in vec3 mat_data;

layout(location = 7) out vec3 vCol;
layout(location = 1) out vec2 v_TexCoord;

out float tIO;

uniform float z_val;
uniform mat4 cam;

void main()
{
	vec4 mdl_points[4];
	mdl_points[0] = vec4(-0.5, -0.5, 0.0, 1.0);
	mdl_points[1] = vec4(0.5f, -0.5f, 0.f, 1.f);
	mdl_points[2] = vec4(0.5f, 0.5f, 0.f, 1.f);
	mdl_points[3] = vec4(-0.5f, 0.5f, 0.f, 1.f);
	
	mat4 translate;
	translate[0] = vec4(1.0, 0.0, 0.0, 0.0);
	translate[1] = vec4(0.0, 1.0, 0.0, 0.0);
	translate[2] = vec4(0.0, 0.0, 1.0, 0.0);
	translate[3] = vec4(position.x, position.y, 0.0, 1.0);
	
	mat4 rot;
	rot[0] = vec4(cos(mat_data.z), sin(mat_data.z), 0.0, 0.0);
	rot[1] = vec4(-sin(mat_data.z), cos(mat_data.z),0.0, 0.0);
	rot[2] = vec4(0.0, 0.0, 1.0, 0.0);
	rot[3] = vec4(0.0, 0.0, 0.0, 1.0);
	
	mat4 scale;
	scale[0] = vec4(width_height_type.x * mat_data.x, 0.0, 0.0, 0.0);
	scale[1] = vec4(0.0, width_height_type.y * mat_data.y, 0.0, 0.0);
	scale[2] = vec4(0.0, 0.0, 1.0, 0.0);
	scale[3] = vec4(0.0, 0.0, 0.0, 1.0);

	vec4 pos;
	switch(int(width_height_type.z)) {
		case 0:
		pos = mdl_points[0];
		break;
		case 1:
		pos = mdl_points[1];
		break;
		case 2:
		pos = mdl_points[2];
		break;
		case 3:
		pos = mdl_points[3];
		break;
	}
	pos = cam * translate * rot * scale * pos;
	gl_Position = vec4(pos.x, pos.y, z_val, 1.0);//vec4(position, z_val, 1.0);

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

uniform sampler2D u_SamplerID[13];

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
			case 5:
			color = texture(u_SamplerID[5], v_TexCoord);
			break;
			case 6:
			color = texture(u_SamplerID[6], v_TexCoord);
			break;
			case 7:
			color = texture(u_SamplerID[7], v_TexCoord);
			break;
			case 8:
			color = texture(u_SamplerID[8], v_TexCoord);
			break;
			case 9:
			color = texture(u_SamplerID[9], v_TexCoord);
			break;
			case 10:
			color = texture(u_SamplerID[10], v_TexCoord);
			break;
			case 11:
			color = texture(u_SamplerID[11], v_TexCoord);
			break;
			case 12:
			color = texture(u_SamplerID[12], v_TexCoord);
			break;
		} 
		//color = texture(u_SamplerID[int(tIO)], v_TexCoord);
	}
	else {
		color = vec4(v_Color,1.0);
	}
};