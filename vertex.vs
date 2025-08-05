#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 3) in float aTexID;

out vec2 TexCoord;
out vec3 FragmentPos;
out float texID;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const float texSize = 1.f;

void main()
{
    FragmentPos = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * model * vec4(aPos, 1.0);
    texID = aTexID;
    TexCoord = aTexCoord;

}