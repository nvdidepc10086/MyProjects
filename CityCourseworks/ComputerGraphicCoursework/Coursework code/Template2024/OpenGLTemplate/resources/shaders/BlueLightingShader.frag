#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform float t; // Time variable for blinking effect

void main()
{
    float alpha = abs(sin(t)); // Blinking effect based on sine wave
    vec4 texColor = texture(texture1, TexCoord);
    
    // Apply the blinking effect to the alpha channel of the texture's color
    // FragColor = vec4(texColor.rgb, texColor.a * alpha);
    
    // Output a red color with the same alpha value
    FragColor = vec4(0.0, 0.0, 1.0, texColor.a * alpha);
}
