#version 330 core

// Fragment shader:
// ================
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;   // extra in variable, since we need the light position in view space we calculate this in the vertex shader

uniform vec4 lightColor;
in vec4 objectColor;

void main()
{
    // ambient
    float ambientStrength = 0.9;
    vec3 ambient = ambientStrength * lightColor.xyz;
    
     // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor.xyz;
    
    // specular
    float specularStrength = 0.4;
    vec3 viewDir = normalize(-FragPos); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor.xyz; 
    
    vec3 result = (ambient + diffuse + specular) * objectColor.xyz;
    FragColor = vec4(result, 1.0);
}

/*
#version 330 core
out vec4 FragColor;  
uniform vec4 wireColour;
uniform vec4 lightColour;

  
void main()
{
    float ambientStrength = 0.21;
    vec4 ambient = ambientStrength * lightColour;

    vec4 result = ambient * wireColour;
    FragColor = vec4(result.xyz, 0.5);
}
*/
