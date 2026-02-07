#version 330 core
out vec4 FragColor;

struct Material{
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    float shininess;
};

uniform vec3 viewPosition;
uniform vec3 lightPosition;
uniform vec4 lightColor;
uniform bool useTexture;
uniform sampler2D d_texture;
uniform Material material;
uniform float lightStrength;

in vec3 fragPos;
in vec2 textCoord;
in vec3 normals;

void main() {
    vec3 unitNormal = normalize(normals);

    vec3 fragToLight = lightPosition - fragPos;
    vec3 fragToView = viewPosition - fragPos;

    float lightDistance = length(fragToLight);
    float viewDistance = length(fragToView);

    float lightDistanceAttenuation = lightStrength / (1.0 + 0.1*lightDistance +0.01 *lightDistance * lightDistance);

    vec3 ambient = lightColor.xyz * material.ambient;

    float diff = lightDistanceAttenuation * max(dot(unitNormal, normalize(fragToLight)), 0.0);
    vec3 diffuse = lightColor.xyz * (diff *material.diffuse);

    vec3 midwayVector = normalize(normalize(fragToLight) + normalize(fragToView));
    float spec = lightDistanceAttenuation * pow(max(dot(unitNormal,midwayVector),0.0f),material.shininess);
    vec3 specular = lightColor.xyz*(spec * material.specular);
        
    vec3 result = ambient + diffuse + specular;

    vec3 pointColor;
    if(!useTexture){
        FragColor =vec4(result,1.0f); 
    }
    else
    {
        pointColor =result * texture(d_texture,textCoord).rgb;
        FragColor = vec4(pointColor.rgb,1.0f);
    }
    
}