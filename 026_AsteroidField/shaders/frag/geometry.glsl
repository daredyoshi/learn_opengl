#version 330 core

out vec4 FragColor;
in vec2 Uv;
in vec3 N;
in vec3 FragPos;



struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    sampler2D diffuseTex1;
    sampler2D diffuseTex2;
    sampler2D diffuseTex3;
    sampler2D specularTex;
};


uniform Material material;

struct DirectionLight{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 CalcDirLight(DirectionLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 baseColor = vec3(texture(material.diffuseTex1, Uv));
    vec3 ambient  = (material.ambient * light.ambient  * baseColor);
    vec3 diffuse  = (material.diffuse * light.diffuse  * diff * baseColor);
    vec3 specular = (material.specular * light.specular * spec);
    return (ambient + diffuse + specular);
}


struct SpotLight{
    vec3 position;   
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float linear;
    float quadratic;
    float cutoff;
    float outerCutoff;
};

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir){

    vec3 lightDir = normalize(light.position - FragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);


    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);



    float lightDistance = length(light.position - FragPos); 
    float lightPow = 0.0;
    float lightSurfaceDiffAngle= dot(lightDir, normalize(-light.direction));

    /* if(lightSurfaceDiffAngle > light.outerCutoff){ */
    lightPow = 1.0 / (1.0 + light.linear * lightDistance + light.quadratic
            * (lightDistance * lightDistance));

    float cutoffRange = light.cutoff - light.outerCutoff;
    lightPow *= clamp((lightSurfaceDiffAngle- light.outerCutoff) / cutoffRange, 0.0, 1.0);


    vec3 baseColor = vec3(texture(material.diffuseTex1, Uv));
    vec3 ambient  = (material.ambient * lightPow * light.ambient  * baseColor);
    vec3 diffuse  = (material.diffuse * lightPow * light.diffuse  * diff * baseColor);
    vec3 specular = (material.specular * lightPow * light.specular * spec );

    return (ambient + diffuse + specular);
}

// the lights
// we only support a single direction light, but there could be multiple
uniform DirectionLight directionLight;
uniform SpotLight spotLight;

uniform vec3 viewPos; 


void main()
{

    vec3 normalizedN = normalize(N);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result  = vec3(0);

    // directional lighting
    result = CalcDirLight(directionLight, normalizedN, viewDir);
    // phase 3: Spot lights
    result += CalcSpotLight(spotLight, normalizedN, FragPos, viewDir);
    /* result = vec3(texture(material.diffuseTex1, Uv)); */
    FragColor = vec4(result, 1.0);
}

