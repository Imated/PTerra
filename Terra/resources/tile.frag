#version 330 core
out vec4 FragColor;
in vec2 uv;

uniform sampler2D mainTexture;

uniform uint tileFrames[256];

void main()
{
    vec2 scaledUV = uv * 16.0;
    vec2 tilePos = floor(scaledUV);

    uint tileID = tileFrames[int(tilePos.x + tilePos.y * 16)];

    vec2 tileOffset = vec2(float(tileID % 21u), float(tileID / 21u));
    vec2 atlasSize = vec2(21.0, 7.0);
    vec2 atlasUV = (tileOffset + fract(scaledUV)) / atlasSize;

    FragColor = texture(mainTexture, atlasUV);
}
