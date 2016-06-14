#version 330 core
struct Material {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    
    float shininess;
};

struct DirLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct SpotLight {
    vec3 position;
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};
#define NR_POINT_LIGHTS 1

in vec3 FragPos;
in vec3 Normal;
out vec4 color;

Material in_material;
Material green;
Material white;
Material silver;

uniform int is_tree;
uniform float max_height;
uniform float min_height;
uniform float x_offset;
uniform float y_offset;
uniform float z_offset;
uniform int light_mode;
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform int prev_light_mode;
// Function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
void main()
{
    float mid_point = -350.0f;
    
    green.ambient = vec3(0.0f, 0.05f, 0.0f);
    green.diffuse = vec3(0.4f, 0.5f, 0.4f);
    green.specular = vec3(0.04f, 0.7f, 0.04f);
    green.shininess = 0.78125f;
    
    silver.ambient = vec3(0.19225f, 0.19225f, 0.19225f);
    silver.diffuse = vec3(0.50754f, 0.50754f, 0.50754f);
    silver.specular = vec3(0.508273f, 0.508273f, 0.508273f);
    silver.shininess = 0.4f;
    
    white.ambient = vec3(.2f, .2f, .2f);
    white.diffuse = vec3(.5f, .5f, .5f);
    white.specular = vec3(1.0f, 1.0f, 1.0f);
    white.shininess = 1.0f;
    
    in_material = material;
    
    if(is_tree == 0)
    {
        float k;
        if(FragPos.y <= max_height && FragPos.y > mid_point){
            k = (FragPos.y-mid_point)/(max_height-mid_point);
            in_material.ambient = k*white.ambient+(1-k)*silver.ambient;
            in_material.diffuse = k*white.diffuse+(1-k)*silver.diffuse;
            in_material.specular = k*white.specular+(1-k)*silver.specular;
            in_material.shininess = k*white.shininess+(1-k)*silver.shininess;
        }
        else if(FragPos.y <= mid_point){
            k = (FragPos.y-min_height)/(mid_point-min_height);
            in_material.ambient = k*silver.ambient+(1-k)*green.ambient;
            in_material.diffuse = k*silver.diffuse+(1-k)*green.diffuse;
            in_material.specular = k*silver.specular+(1-k)*green.specular;
            in_material.shininess = k*silver.shininess+(1-k)*green.shininess;
        }
    }
    
    
    vec3 norm = normalize(Normal);
    //    norm.x += x_offset;
    //    norm.y += y_offset;
    //    norm.z += z_offset;
    //    norm = normalize(norm);
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0f);
    
    
    
    /*
     if(FragPos.y <= -200 && FragPos.y >= -256){
     in_material.ambient = vec3(0.0f, 0.05f, 0.0f);
     in_material.diffuse = vec3(0.4f, 0.5f, 0.4f);
     in_material.specular = vec3(0.04f, 0.7f, 0.04f);
     in_material.shininess = 0.78125f;
     }
     
     if(FragPos.y <= -150 && FragPos.y > -200){
     in_material.ambient = vec3(0.19225f, 0.19225f, 0.19225f);
     in_material.diffuse = vec3(0.50754f, 0.50754f, 0.50754f);
     in_material.specular = vec3(0.508273f, 0.508273f, 0.508273f);
     in_material.shininess = 0.4f;
     }
     */
    
    
    
    
    if(light_mode == 0){
        if(prev_light_mode == 1)
            result = CalcDirLight(dirLight, norm, viewDir);
        else if(prev_light_mode == 2)
            for(int i = 0; i < NR_POINT_LIGHTS; i++)
                result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
        else if(prev_light_mode == 3)
            result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    }
    if(light_mode == 1){
        result = CalcDirLight(dirLight, norm, viewDir);
    }
    
    
    if(light_mode == 2){
        for(int i = 0; i < NR_POINT_LIGHTS; i++)
            result = CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }
    
    
    if(light_mode == 3){
        result = CalcSpotLight(spotLight, norm, FragPos, viewDir);
    }
    
    //color = vec4(trash[0], trash[1], 0.0, 1.0);
    color = vec4(result, 1.0);
    //color = vec4(1.0,0.0,0.0,1.0);
    /*if(FragPos.y > 0)
     color = vec4(1.0,0.0,0.0,1.0);
     else
     color = vec4(0.0,1.0,0.0,1.0);
     */
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), in_material.shininess);
    // Combine results
    vec3 ambient = light.ambient * in_material.ambient;
    vec3 diffuse = light.diffuse * diff * in_material.diffuse;
    vec3 specular = light.specular * spec * in_material.specular;
    //    float spec = pow(max(dot(viewDir, reflectDir), 0.0) , 1.0f);
    //    vec3 ambient = light.ambient;
    //    vec3 diffuse = light.diffuse * diff;
    //    vec3 specular = light.specular * spec;
    return (ambient + diffuse + specular);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // Combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // Combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}