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
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float linear;
    float quadratic;
    float cutoff;
    float outerCutoff;
};

out vec4 FragColor;
in vec2 Uv;
in vec3 N;
in vec3 FragPos;

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


    float lightPow = 0.0;
    float lightSurfaceDiffAngle= dot(lightDir, normalize(-light.direction));
    // remember that this is now with an already calculated cosine
    // angle of 0 is consine value of 1.0, angle of 90 is cosine of 0.0
    // so we wan
    if(lightSurfaceDiffAngle> light.outerCutoff){
        lightPow = 1.0 / (1.0 + light.linear * lightDistance + light.quadratic
                * (lightDistance * lightDistance));

        float cutoffRange = light.cutoff - light.outerCutoff;
        lightPow *= clamp((lightSurfaceDiffAngle- light.outerCutoff) / cutoffRange, 0.0, 1.0);
    } 
    
    // keep ambient to the global
    diffuse *= lightPow;
    specular *= lightPow;

    // emission color
    // vec3 emissionColor = vec3(texture(material.diffuseTex2, newUv) * 0.1);
    vec3 result = ( ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}

