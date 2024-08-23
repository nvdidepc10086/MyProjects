#version 400 core

in vec3 vColour;           // Interpolated colour using colour calculated in the vertex shader
in vec2 vTexCoord;         // Interpolated texture coordinate using texture coordinate from the vertex shader

out vec4 fragColor;        // The output colour for fog

uniform sampler2D sampler0;    // The texture sampler
uniform samplerCube CubeMapTex;
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied
uniform bool renderSkybox;

uniform vec3 fogColor;      // Fog color
uniform float fogDensity;   // Fog density

in vec3 worldPosition;      // Used for skybox

void main()
{
    if (renderSkybox) {
        vec4 skyColor = texture(CubeMapTex, worldPosition);

       
        
        // Calculate fog factor based on the distance from the camera
        float depth = length(worldPosition);
        float fogFactor = 1.0 - exp(-depth * fogDensity);
        
        // Blend sky color with fog color based on fog factor
        fragColor = mix(skyColor, vec4(fogColor, 1.0), fogFactor);
    } else {
        // Get the texel colour from the texture sampler
        vec4 vTexColour = texture(sampler0, vTexCoord); 

        // Output the texel colour without applying fog
        fragColor = vTexColour;

        if (bUseTexture)
			fragColor = vTexColour*vec4(vColour, 1.0f);	// Combine object colour and texture 
		else
			fragColor = vec4(vColour, 1.0f);	// Just use the colour instead
	
    }
}
