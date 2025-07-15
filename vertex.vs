#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec4 offset;

out vec2 TexCoord;
out vec3 FragmentPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const float texSize = 1.f;

void main()
{
    FragmentPos = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * model * vec4(aPos + offset.xyz, 1.0);
	int blockID = int(offset.w);

	TexCoord = vec2(aTexCoord.x + (blockID % 16) * texSize, aTexCoord.y + (blockID % 16) * texSize);
}
