// ====================================================
#version 330 core

// Import our texture coordinates from vertex shader
in vec2 v_texCoord;

// ======================= out ========================
// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;

void main()
{
    // Width of the outline (adjust as needed)
    float edgeWidth = 0.002; // Experiment with this value

    // Calculate the distance from the center of the teapot 
    vec2 center = vec2(0.5, 0.5);
    float dist = length(v_texCoord - center);

    // Create a stepped effect at the edges
    float edgeEffect = smoothstep(0.5 - edgeWidth, 0.5, dist);
    
    

    // Final color + "how dark or light to make fragment"
    FragColor = vec4(0.0, 0.0, 0.0 ,edgeEffect);
}
// ==================================================================
