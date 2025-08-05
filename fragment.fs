#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragmentPos;
in float texID;
// texture samplers
uniform sampler2DArray textureArray;

void main()
{

    FragColor = texture(textureArray, vec3(TexCoord, texID));

}