#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragmentPos;

// texture samplers
uniform sampler2D texture1;
//uniform sampler2D textureBot;
//uniform sampler2D textureTop;

void main()
{

	FragColor = texture(texture1, TexCoord);
}
