#shader vertex
#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 vtx;

layout(location = 1) out vec2 texCoords;

uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(vtx.x, vtx.y, 0.0, 1.0);
	texCoords = vtx.zw;
}


#shader fragment
#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 1) in vec2 texCoords;

layout(location = 1) out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texCoords).r);
	color = vec4(textColor, 1.0) * sampled;
}