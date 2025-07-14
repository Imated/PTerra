#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D mainTexture;
uniform int tileID;

void main()
{
    vec2 sampleUV = (vec2(int(tileID % 21), int(tileID / 21)) + TexCoords) / vec2(21, 7);

    FragColor = texture(mainTexture, sampleUV);
    //FragColor = vec4(TexCoords, 0.0, 1.0);
}
