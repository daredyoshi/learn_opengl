#version 330 core
struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    sampler2D diffuseTex1;
    sampler2D diffuseTex2;
    sampler2D specularTex;
};

struct PointLight{
    vec3 position;   
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float linear;
    float quadratic;
};

out vec4 FragColor;
in vec2 Uv;
in vec3 N;
in vec3 FragPos;
in vec3 LightPos;

uniform bool flip;
uniform float scale;
uniform float opacity;

uniform Material material;
uniform PointLight light;

uniform vec3 viewPos; 


void main()
{
    vec2 newUv = Uv;
    if(flip){
        newUv.x = 1 - newUv.x;
    }

    vec3 baseColor = vec3(mix(texture(material.diffuseTex1, Uv), texture(material.diffuseTex2, newUv), opacity));
    vec3 ambient = material.ambient * light.ambient * baseColor;
    


    vec3 normalizedN = normalize(N);
    vec3 lightDir = normalize(light.position - FragPos);
    float lightDistance = length(light.position - FragPos);
    // diffuse
    float diffuseStength = max(dot(normalizedN, lightDir), 0.0);
    vec3 diffuse = baseColor * vec3(diffuseStength) * material.diffuse * light.diffuse;

    //specular
    vec3 viewDir = normalize(viewPos-FragPos);
    // negate the light dir to match the reflect function.
    vec3 reflectDir = reflect(-lightDir, normalizedN);
    // calculate the dot produce between view direction and reflect direction
    // raise it to the pow of 32 (higher is sharper spec)
    float baseSpec = float(texture(material.specularTex, Uv));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular =light.specular * (spec * material.specular) * baseSpec;


    float attenuation = 1.0 / (1.0 + light.linear * lightDistance + light.quadratic
            * (lightDistance * lightDistance));
    ambient *=attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    

    // emission color
    // vec3 emissionColor = vec3(texture(material.diffuseTex2, newUv) * 0.1);
    vec3 result = ( ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}

