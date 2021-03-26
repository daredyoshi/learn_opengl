#version 330 core
struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}


out vec4 FragColor;
in vec2 Uv;
in vec3 N;
in vec3 FragPos;
in vec3 LightPos;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform bool flip;
uniform float scale;
uniform float opacity;

uniform vec3 lightColor;

/* uniform vec3 viewPos; */


void main()
{
    vec2 newUv = Uv;
    if(flip){
        newUv.x = 1 - newUv.x;
    }

    vec3 baseColor = vec3(mix(texture(texture1, Uv), texture(texture2, newUv), opacity));
    vec3 ambient = material.ambient * lightColor;
    
    vec3 normalizedN = normalize(N);
    vec3 lightDir = normalize(LightPos - FragPos);

    // diffuse
    float diffuseStength = max(dot(normalizedN, lightDir), 0.0);
    vec3 diffuse = material.diffuse * lightColor;

    //specular
    vec3 viewDir = normalize(-FragPos);
    // negate the light dir to match the reflect function.
    vec3 reflectDir = reflect(-lightDir, normalizedN);
    // calculate the dot produce between view direction and reflect direction
    // raise it to the pow of 32 (higher is sharper spec)
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materia.shininess);
    vec3 specular = lightcolor * (spec * material.specular);

    
    vec3 result = ( ambient + diffuse + spec) * baseColor;
    FragColor = vec4(result, 1.0);
}

