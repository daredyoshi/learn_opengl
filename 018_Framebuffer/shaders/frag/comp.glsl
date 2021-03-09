#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0/300.0;

void main()
{ 
    // regular output
    /* FragColor = texture(screenTexture, TexCoords); */

    // inverted output
    /* FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0); */
    
    // grayscale output
    /* FragColor = texture(screenTexture, TexCoords); */
    /* float average = (FragColor.r + FragColor.g, + FragColor.b) / 3.0; */
    /* FragColor = vec4(average, average, average, 1.0); */

    // kernels
    // -----------------
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    // sharpen output
    /* float kernel[9] = float[]( */
    /*     -1, -1, -1, */
    /*     -1,  9, -1, */
    /*     -1, -1, -1 */
    /* ); */
    // blur output
    /* float kernel[9] = float[]( */
    /*         1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0, */
    /*         2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0, */
    /*         1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0 */
    /*         ); */
    // edge detection
    float kernel[9] = float[](
            1, 1, 1, 
            1, -8, 1, 
            1, 1, 1
            );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    FragColor = vec4(col, 1.0);

}

