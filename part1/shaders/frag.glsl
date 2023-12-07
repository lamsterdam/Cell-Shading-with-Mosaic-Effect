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

// Function on indiegame.net and I used it to generate random numbers for adding digital noise in GLSL.
// I then used it in the mosaic effect for the blocks for getting a pattern of colours slightly darker and ligheter
float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    // Store our final texture color
    vec3 diffuseColor;
    diffuseColor = texture(u_DiffuseMap, v_texCoord).rgb;

    // Calculate mosaic effect by getting the texture size from the DiffuseMap and I set the size of the mosaic blocks
    // This is an arbitrary size but it seemed to look best to me for this teapot
    vec2 texSize = vec2(textureSize(u_DiffuseMap, 0));
    vec2 blockSize = vec2(30.0); // Adjust this value for mosaic block size
    
    // I was originally using this to try and move the mosaic colour inwards and have a border around it to make the
    // blocks more mosaic and pronounced but leave this here in hopes of coming back to this project in the future
    // Calculate the block index for each pixel
    vec2 blockIndex = floor(v_texCoord * texSize / blockSize);

    // Define the size of the center square within each block
    float centerSize = 0.2; // Adjust this value for the size of the center area

    // Calculate the center area in the block
    vec2 centerStart = blockIndex * blockSize + blockSize * centerSize;
    vec2 centerEnd = blockIndex * blockSize + blockSize * (1.0 - centerSize);

    // To create the effect I divided the texture cordinate by the size of the texture and the size of the block, then rounded it 
    //down to find the pixel the block belongs to. Now that I know the pixel the block belongs to I scale back using block size and 
    // texture size which creates the blocky appearance because the texture is constrained to the edge of the block, so each block
    // has a distinct edge or border
    vec2 pixelatedTexCoord = floor(v_texCoord * texSize / blockSize) * blockSize / texSize;

    // now the mosaicColour is based on this 
    vec3 mosaicColor = texture(u_DiffuseMap, pixelatedTexCoord).rgb;

    // This is the offset random variations to the pixel coord to increase or decrease the randomness in the colour
    float randomOffset = 0.1; 

    // I got the calculations for the offset based off of YACA (Yet Another Chromatic Abberation) on the reshade.me forum page

    //Create random offsets for each colour channel using the rand() function for red, green and blue
    float randOffsetR = rand(pixelatedTexCoord) * randomOffset * 2.0 - randomOffset;
    float randOffsetG = rand(pixelatedTexCoord + vec2(42.0, 17.0)) * randomOffset * 2.0 - randomOffset;
    float randOffsetB = rand(pixelatedTexCoord - vec2(71.0, 33.0)) * randomOffset * 2.0 - randomOffset;

    // then using the offsets I add this to mosaicColor to make the colour more randomized
    mosaicColor += vec3(randOffsetR, randOffsetG, randOffsetB);

    // This section is again related to the idea I had to move the mosaic colour in by some factor within each block
    // but I will leave here for the future when I come back
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
    // At first I was going to check if the shading was zero so as to not apply the shading but decided to apply it and just toggle it
    //if(cellShading > 0){
     // float diff_levels = diff_levels * cellShading;
     // float spec_levels = spec_levels * cellShading;
    //}

    //I am creating new diff levels and spec levels that change with the cell shading effect, so when cell shading increases so do 
    // these values 
    float adjusted_diff_levels = diff_levels * cellShading;
    float adjusted_spec_levels = spec_levels * cellShading;
    

    // Discretizing the impact of the diffuse lighting based on the dot difference by using upper and lower bounds for the shading.
    // The dot difference is changed to manipulate the impact of diffuse light
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

    // Discretizing the impact of the specural lighting based on the dot difference and levels
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

    // Our final color is now based on the texture. Now that we have all the lighting calculate the final lighting
    // That is set by the diffuseColor
    vec3 Lighting = diffuseLight + ambient + specular;

    // This is the output colour to only see the regular cell shading effect
    //FragColor = vec4(diffuseColor * Lighting,1.0); //for regular cell shading output

    // This is the output colour to see a mosaic effect which can be in combination with cell shading 
    FragColor = vec4(mosaicColor * Lighting, 1.0); //for pixelated effect

    // This was from the mosaic colour scaling inwards idea
    //FragColor = vec4(finalColor * Lighting, 1.0); //for pixelated effect

    
}
// ==================================================================