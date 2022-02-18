#shader vertex
#version 410 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

struct VertexOutput
{
	vec2 texCoord;
};

layout (location = 0) out VertexOutput v_Output;

void main()
{
	v_Output.texCoord = aTexCoord;
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}

#shader fragment
#version 410 core

struct VertexOutput
{
	vec2 texCoord;
};

layout (location = 0) in VertexOutput v_Input;

uniform sampler2D uTexture;

out vec4 FragColor;
void main()
{
	FragColor = texture(uTexture, v_Input.texCoord);
}