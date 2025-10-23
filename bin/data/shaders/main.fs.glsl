#version 400


#define RENDER_MODE_NO_TEXTURES 0
#define RENDER_MODE_WITH_TEXTURES 1

uniform sampler2D tex0;
uniform int renderMode;

in vec2 v_TexCoords;
out vec4 out_Color;


void main() {

    if(renderMode == RENDER_MODE_NO_TEXTURES) {
        out_Color = vec4(1.0);
    } else if(renderMode == RENDER_MODE_WITH_TEXTURES) {
        out_Color = texture(tex0, v_TexCoords);
        //out_Color = vec4(v_TexCoords, 0.0, 1.0);
    } else {
        out_Color = vec4(1.0); // Defaults to RENDER_MODE_NO_TEXTURES
    }
}