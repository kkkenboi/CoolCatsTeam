#shader vertex
#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 aCol;

layout(location = 7) out vec3 vCol;

out vec2 v_TexCoord;

void main()
{
	gl_Position = vec4(position, 0.0, 1.0);
	vCol = aCol;
	v_TexCoord = texCoord;
};


#shader fragment
#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 7) in vec3 v_Color;
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

void main()
{
	color = vec4(v_Color,1.0);
};