#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec4 offset;

out vec2 TexCoord;
out vec3 FragmentPos;
out float BlockID;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const float texSize = 1.f;

void main()
{
    FragmentPos = vec3(model * vec4(aPos, 1.0));
    BlockID = offset.w;
	gl_Position = projection * view * model * vec4(aPos + offset.xyz, 1.0);

    //TexCoord = vec2(0.5); // фиксированная текстура (или 0.0)
    TexCoord = aTexCoord;
	//TexCoord = vec2(aTexCoord.x + (blockID % 16) * texSize, aTexCoord.y + (blockID % 16) * texSize);

    vec3 off = offset.xyz;

}