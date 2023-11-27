#version 410 core

in vec3 v_vertexColors;
in vec3 v_vertexNormals;
//in vec3 v_FragPos;

uniform vec3 u_LightPos;
//uniform float ambientIntensity;
//uniform vec3 u_LightColor;

//out vec4 color;
out vec4 out_color;

// Entry point of program
void main()
{
	// Normalize every attribute incoming
    // from vertex shader because they are
    // interpolated values
    vec3 normals  = normalize(v_vertexNormals); 
    //vec3 lightDirection = normalize(u_LightPos - v_FragPos);

    //float diff = max(dot(normals, lightDirection), 0.0);
    //vec3 diffuse = diff * u_LightColor;

    //vec3 Lighting = diffuseLight + ambient;

	//color = vec4(v_vertexColors.r,v_vertexColors.g, v_vertexColors.b, 1.0f);
	out_color = vec4(normals.x, normals.y, normals.z, 1.0f);
}
