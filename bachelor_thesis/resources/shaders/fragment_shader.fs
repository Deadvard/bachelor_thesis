#version 430

in vec3 worldPosition;
in vec3 faceNormal;
out vec4 fragColor;

uniform vec3 camPos;

void main()
{
	vec3 diffuseColor = vec3(0.4,0.4,0.0);
	vec3 specularColor = vec3(0.4,0.4,0.0);
    vec3 ambient = vec3(0.7,0.7, 0.7);
	    // diffuse 
    vec3 norm = normalize(faceNormal);
    // vec3 lightDir = normalize(light.position - FragPos);
    vec3 lightDir = normalize(vec3(-0.5,0.5,1));  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseColor * diff;  
    
    // specular
    vec3 viewDir = normalize(camPos - worldPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = 0.5 * spec * specularColor;
        
    vec3 result = ambient + diffuse + specular;


	fragColor = vec4(result, 1.0);
}