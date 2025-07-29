#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragmentPos;

in float BlockID;
// texture samplers
uniform sampler2DArray textureArray;

void main()
{
    vec3 texCoord = vec3(TexCoord, BlockID);

    FragColor = texture(textureArray, texCoord);


    //FragColor = vec4(BlockID / 10.0, 0.5, 1.0, 1.0); // Просто цвет по слою

}
