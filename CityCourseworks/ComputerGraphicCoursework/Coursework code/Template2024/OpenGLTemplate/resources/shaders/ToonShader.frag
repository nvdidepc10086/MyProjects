#version 400 core

in vec3 vColour;           // Interpolated colour using colour calculated in the vertex shader
in vec2 vTexCoord;         // Interpolated texture coordinate using texture coordinate from the vertex shader

out vec4 fragColor;        // The output colour

void main()
{
    // Define the thresholds for toon shading
    float edgeThreshold = 0.2;
    float silhouetteThreshold = 0.1;
    
    // Calculate the luminance of the vertex colour
    float luminance = dot(vColour.rgb, vec3(0.2126, 0.7152, 0.0722));
    
    // Apply toon shading based on luminance
    if (luminance < silhouetteThreshold)
        fragColor = vec4(0.0, 0.0, 0.0, 1.0); // Silhouette
    else if (luminance < edgeThreshold)
        fragColor = vec4(0.5, 0.5, 0.5, 1.0); // Edge
    else
        fragColor = vec4(1.0, 1.0, 1.0, 1.0); // Highlight
}
