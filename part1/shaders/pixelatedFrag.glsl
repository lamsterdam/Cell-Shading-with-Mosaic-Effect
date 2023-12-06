// ====================================================
#version 330 core

// ======================= uniform ====================
// If we have texture coordinates, they are stored in this sampler.
uniform sampler2D u_DiffuseMap; 
uniform float mosaicSize; // Size of each mosaic block


// ======================= IN =========================
in vec2 v_texCoord; // Import our texture coordinates from vertex shader

// ======================= out ========================
// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;

void main()
{
    // Store our final texture color
    vec3 diffuseColor;
    diffuseColor = texture(u_DiffuseMap, v_texCoord).rgb;    
    //FragColor = vec4(diffuseColor,1.0);

    // Calculate mosaic effect
    vec2 texSize = vec2(textureSize(u_DiffuseMap, 0));
    vec2 blockSize = vec2(10.0); // Adjust this value for mosaic block size

    vec2 pixelatedTexCoord = floor(v_texCoord * texSize / blockSize + 0.5) * blockSize / texSize;
    vec3 mosaicColor = texture(u_DiffuseMap, pixelatedTexCoord).rgb;
    //FragColor = vec4(mosaicColor,1.0);


    //vec2 uv = floor(v_texCoord * mosaicSize) / mosaicSize;
    //vec3 averageColor = texture(u_DiffuseMap, uv).rgb;
    //FragColor = vec4(averageColor, 1.0);
}
// ==================================================================