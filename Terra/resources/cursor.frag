#version 330 core
out vec4 FragColor;
in vec2 uv;

uniform int frame;
uniform sampler2D cursorAtlas;

void main() {
    vec2 atlasSize = vec2(2, 1);
    vec2 translatedUV = uv + vec2(float(frame), 0);
    vec2 sampleUV = translatedUV / atlasSize;
    FragColor = texture(cursorAtlas, sampleUV);
    //FragColor = vec4(uv, 0.0, 1.0);
}