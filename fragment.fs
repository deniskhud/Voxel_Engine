#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragmentPos;
in float BlockID;
// texture samplers
uniform sampler2DArray texture1;
//uniform sampler2D textureBot;
//uniform sampler2D textureTop;

void main()
{

	FragColor = texture(texture1, vec3(TexCoord, BlockID));
}
