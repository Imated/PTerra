#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform float time;
uniform sampler2D mainTexture;

void main()
{
    FragColor = texture(mainTexture, TexCoords);
}
