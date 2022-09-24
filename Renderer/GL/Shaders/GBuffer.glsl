#VertexShader
#line 2

layout(location=0)in vec3 position;
layout(location=1)in vec2 texCoord;
layout(location=2)in vec3 normal;
layout(location=3)in vec4 tangent;

uniform mat4 projection;
uniform mat4 view;

out vec3 interpNormal;
out vec3 interpTangent;
out vec3 interpPosition;
out vec2 interpTexCoords;

void main()
{    

    interpTangent=normalize(vec3(tangent));

    interpTexCoords=texCoord;
    interpNormal=normal;
    interpPosition=position;
    gl_Position = projection*view*vec4(position,1.0);
}
#End
#FragmentShader
#line 33


uniform sampler2D albedoSampler;
uniform sampler2D normalSampler;
uniform sampler2D roughnessMetalnessSampler;

uniform float metalness;
uniform float roughness;
uniform vec3 albedo;

in vec3 interpTangent;
in vec3 interpNormal;
in vec3 interpPosition;
in vec2 interpTexCoords;




layout(location=0)out vec3 outputPosition;
layout(location=1)out vec3 outputAlbedo;
layout(location=2)out vec3 outputNormal;
layout(location=3)out vec3 outputMetallicPoughness;


void main()
{
    outputPosition = interpPosition;
    vec4 texAlbedo = texture(albedoSampler, interpTexCoords).rgba;
    
    if(texAlbedo.a<0.5)
    {
        discard;
    }
    outputAlbedo = texAlbedo.rgb*albedo;
    
    vec3 interpBitangent = cross(interpTangent,interpNormal);
    mat3 TBN = mat3(interpTangent,interpBitangent,interpNormal);

    vec3 normalTexture = texture(normalSampler,interpTexCoords).rgb;
    normalTexture = (normalTexture.rgb-vec3(0.5f))*2.0f;
    outputNormal = normalize(TBN*normalTexture);
    
    float metallic = texture(roughnessMetalnessSampler,interpTexCoords).b*metalness;
    float roughness = texture(roughnessMetalnessSampler,interpTexCoords).g*roughness;
    outputMetallicPoughness = vec3(metallic,roughness,0.0);

}
#End