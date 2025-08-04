#version 330 core
out vec4 FragColor;
in vec2 uv;

uniform sampler2D mainTexture;

uniform uint groundFrames[256];
uniform uint topFrames[256];

void main() {
    vec2 scaledUV = uv * 16.0;
    vec2 tilePos = floor(scaledUV);

    uint groundTileID = groundFrames[int(tilePos.x + tilePos.y * 16)];
    uint topTileID = topFrames[int(tilePos.x + tilePos.y * 16)];

    vec2 groundTileOffset = vec2(float(groundTileID % 21u), float(groundTileID / 21u));
    vec2 topTileOffset = vec2(float(topTileID % 21u), float(topTileID / 21u));
    vec2 atlasSize = vec2(21.0, 7.0);
    vec2 groundAtlasUV = (groundTileOffset + fract(scaledUV)) / atlasSize;
    vec2 topAtlasUV = (topTileOffset + fract(scaledUV)) / atlasSize;

    vec4 groundColor = texture(mainTexture, groundAtlasUV);
    vec4 topColor = texture(mainTexture, topAtlasUV);

    FragColor = topColor;
    if (FragColor.rgb == vec3(0.0)) {
        FragColor = vec4(1.0, 0.0, 1.0, 1.0);
    }
}
