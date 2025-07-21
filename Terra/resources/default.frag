#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D mainTexture;

uniform uvec2 tilePositions[256];
uniform uint tileFrames[256];

// O(n) dict implementation
uint getTileIdAtWorldPos(vec2 pos) {
    uvec2 upos = uvec2(uint(int(pos.x)), uint(int(pos.y)));
    for (uint i = 0u; i < 256u; i++) {
        if (tilePositions[i] == upos) {
            return tileFrames[i];
        }
    }
    return 0u;
}

void main()
{
    vec2 scaledUV = TexCoords * 16.0;
    vec2 tilePos = floor(scaledUV);

    uint tileID = getTileIdAtWorldPos(tilePos);

    vec2 tileOffset = vec2(float(tileID % 21u), float(tileID / 21u));
    vec2 atlasSize = vec2(21.0, 7.0);
    vec2 atlasUV = (tileOffset + fract(scaledUV)) / atlasSize;

    FragColor = texture(mainTexture, atlasUV);
}
