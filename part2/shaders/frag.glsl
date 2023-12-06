// ====================================================
#version 330 core

// ======================= uniform ====================


// Our light sources
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float ambientIntensity;
// Used for our specular highlights
uniform mat4 view;
// If we have texture coordinates, they are stored in this sampler.
uniform sampler2D u_DiffuseMap; 
// set a uniform up in main, based on keypresses increase the light or decrease
uniform float cellShading;

// ======================= IN =========================
in vec3 myNormal; // Import our normal data
in vec2 v_texCoord; // Import our texture coordinates from vertex shader
in vec3 FragPos; // Import the fragment position

// ======================= out ========================
// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;

// ======================= Globals ====================
// Constants for adjusting the differentiation levels based on the cell shading
float specularStrength = 0.5;
const float diff_levels = 5.0;
const float spec_levels = 1000.0;
//multiply the uniform by these
//also consider increasing the levels

//create a mosaic effect
//select the pixel, rgb of the nearest neighbours and take an average 
//and then set the rbg of all the pixels in this set of pixel to the average colour rbg
//as a post processing for the colour

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    // Store our final texture color
    vec3 diffuseColor;
    diffuseColor = texture(u_DiffuseMap, v_texCoord).rgb;

    // Calculate mosaic effect
    vec2 texSize = vec2(textureSize(u_DiffuseMap, 0));
    vec2 blockSize = vec2(30.0); // Adjust this value for mosaic block size
    //float overlap = 2.0;

    // Calculate the block index for each pixel
    vec2 blockIndex = floor(v_texCoord * texSize / blockSize);

    // Define the size of the center square within each block
    float centerSize = 0.2; // Adjust this value for the size of the center area

    // Calculate the center area in the block
    vec2 centerStart = blockIndex * blockSize + blockSize * centerSize;
    vec2 centerEnd = blockIndex * blockSize + blockSize * (1.0 - centerSize);

    vec2 pixelatedTexCoord = floor(v_texCoord * texSize / blockSize) * blockSize / texSize;

    vec3 mosaicColor = texture(u_DiffuseMap, pixelatedTexCoord).rgb;

    // Introduce random variations to the pixelated color
    float randomOffset = 0.1; // Adjust this value for the range of randomness

    //Create random offsets for each channel using the rand() function
    float randOffsetR = rand(pixelatedTexCoord) * randomOffset * 2.0 - randomOffset;
    float randOffsetG = rand(pixelatedTexCoord + vec2(42.0, 17.0)) * randomOffset * 2.0 - randomOffset;
    float randOffsetB = rand(pixelatedTexCoord - vec2(71.0, 33.0)) * randomOffset * 2.0 - randomOffset;

    mosaicColor += vec3(randOffsetR, randOffsetG, randOffsetB);

    // Define colors for the inner and outer areas
    vec3 innerColor = mosaicColor;
    vec3 outerColor = vec3(0.0, 0.0, 0.0);

     // Determine if the pixel is within the center or outer area
     vec3 finalColor = outerColor;
    if (v_texCoord.x > centerStart.x && v_texCoord.x < centerEnd.x &&
        v_texCoord.y > centerStart.y && v_texCoord.y < centerEnd.y) {
        finalColor = innerColor;
    }

    
    // (1) Compute ambient light
    vec3 ambient = ambientIntensity * lightColor;

    // (2) Compute diffuse light

    // Compute the normal direction
    vec3 norm = normalize(myNormal);
    // From our lights position and the fragment, we can get
    // a vector indicating direction
    // Note it is always good to 'normalize' values.
    vec3 lightDir = normalize(lightPos - FragPos);
    // Now we can compute the diffuse light impact
    float dotDif = dot(norm, lightDir);


    // Adjust differentiation levels based on cellShading
    // First checking if to apply the shading or not if shading is greater than zero
    //if(cellShading > 0){
     // float diff_levels = diff_levels * cellShading;
     // float spec_levels = spec_levels * cellShading;
    //}

    float adjusted_diff_levels = diff_levels * cellShading;
    float adjusted_spec_levels = spec_levels * cellShading;
    

    float upper_bound = 1.0;
    for(int i=1; i <= adjusted_diff_levels; i++) {
		float lower_bound = 1.0 * ((adjusted_diff_levels - i) / adjusted_diff_levels);

		if(dotDif <= upper_bound && dotDif > lower_bound) {
			dotDif = lower_bound;
			break;
		}
		else {
			upper_bound = lower_bound;
		}
	}

    float diffImpact = max(dotDif, 0.0);
    vec3 diffuseLight = diffImpact * lightColor;
    

    // (3) Compute Specular lighting
    vec3 viewPos = vec3(0.0,0.0,0.0);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    dotDif = dot(viewDir, reflectDir);
    upper_bound = 1.0;
    for(int i=1; i <= adjusted_spec_levels; i++) {
		float lower_bound = 1.0 * ((adjusted_spec_levels - i) / adjusted_spec_levels);

		if(dotDif <= upper_bound && dotDif > lower_bound) {
			dotDif = upper_bound;
			break;
		}
		else {
			upper_bound = lower_bound;
		}
	}

    float spec = pow(max(dotDif, 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Our final color is now based on the texture.
    // That is set by the diffuseColor
    vec3 Lighting = diffuseLight + ambient + specular;

    FragColor = vec4(diffuseColor * Lighting,1.0); //for regular cell shading output

    //FragColor = vec4(mosaicColor * Lighting, 1.0); //for pixelated effect
    //FragColor = vec4(finalColor * Lighting, 1.0); //for pixelated effect

    
}
// ==================================================================