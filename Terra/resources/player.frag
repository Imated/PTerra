#version 330 core

out vec4 FragColor;

in vec2 uv;

uniform int frame;
uniform int anim;
uniform sampler2D playerAtlas;

void main() {
    vec2 atlasSize = vec2(8, 6);
    vec2 translatedUV = uv + vec2(float(frame), atlasSize.y - 1 - float(anim));
    vec2 sampleUV = translatedUV / atlasSize;
    FragColor = texture(playerAtlas, sampleUV);
    //FragColor = vec4(uv, 0.0, 1.0);
}