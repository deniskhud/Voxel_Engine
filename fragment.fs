#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragmentPos;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D textureBot;
uniform sampler2D textureTop;

void main()
{
    if(abs(FragmentPos.y - 0.5) < 0.01)
        FragColor = texture(textureBot, TexCoord);
    else if(abs(FragmentPos.y + 0.5) < 0.01)
        FragColor = texture(textureTop, TexCoord);
    else
	    FragColor = texture(texture1, TexCoord);
}
