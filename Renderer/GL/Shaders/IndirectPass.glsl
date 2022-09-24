#VertexShader
#line 2
layout(location=0)in vec3 position;
layout(location=1)in vec2 texCoord;
layout(location=2)in vec3 normal;
layout(location=3)in vec4 tangent;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightProjection;
out vec2 rsmTextureCoord;

out vec3 pixelPosition;
out vec2 pixelTexCoord;
out vec3 pixelNormal;
out vec3 pixelTangent;

void main()
{
    vec4 rsmSpacePosition = lightProjection*vec4(position, 1.0f);
    rsmTextureCoord = rsmSpacePosition.xy;


    pixelPosition = position;
    pixelTexCoord = texCoord;
    pixelNormal = normal;
    pixelTangent = tangent.xyz;
    gl_Position = projection*view*vec4(position,1.0f);
}

#End
#FragmentShader
#line 33
const unsigned int sampleCount = 400;


uniform vec3 albedoConstant;
uniform sampler2D albedoTexture;
uniform sampler2D normalTexture;

uniform sampler2D albedoRSM;
uniform sampler2D normalRSM;
uniform sampler2D positionRSM;

uniform sampler1D samplePoints;
out vec3 indirectFlux;


uniform float sampleSpan;

in vec2 rsmTextureCoord;
in vec3 pixelPosition;
in vec2 pixelTexCoord;
in vec3 pixelNormal;
in vec3 pixelTangent;


const float PI = 3.14159265359;

uvec3 pcg3d(uvec3 v)
{
    v = v * 1664525u + 1013904223u;
    v.x += v.y*v.z;
    v.y += v.z*v.x;
    v.z += v.x*v.y;
    v.x = v.x ^ (v.x >> 16u);
    v.y = v.y ^ (v.y >> 16u);
    v.z = v.z ^ (v.z >> 16u);
    v.x += v.y*v.z;
    v.y += v.z*v.x;
    v.z += v.x*v.y;
    return v;
}


void main()
{

    vec3 albedoP = albedoConstant*texture(albedoTexture,pixelTexCoord).rgb;
    vec3 normalMap = ((texture(normalTexture,pixelTexCoord).rgb)-vec3(0.5f))*vec3(2.0f);

    vec3 bitangent = cross(pixelTangent,pixelNormal);
    mat3 TBN = mat3(pixelTangent,bitangent,pixelNormal);
    vec3 normalP = normalize(TBN*normalMap);
	indirectFlux=vec3(0.0f);
    for(unsigned int i=0;i<100;i++)
    {

    
        vec2 randomSamplePoint = (vec3(pcg3d(uvec3(i,0,0)))/vec3(pow(2,32))).xy;
        //vec2 randomSamplePoint = vec2(0.0f);
        //vec2 randomSamplePoint = texture(samplePoints,((float(i))/100.0f)).rg;
        vec2 samplePoint = (vec2(
            rsmTextureCoord.x + sampleSpan*randomSamplePoint.x*sin(2.0f*PI*randomSamplePoint.y),
            rsmTextureCoord.y + sampleSpan*randomSamplePoint.x*cos(2.0f*PI*randomSamplePoint.y))+1.0f)/2.0f;
            

        vec3 albedoIndirect = texture(albedoRSM,samplePoint).rgb;
        vec3 normalIndirect = texture(normalRSM,samplePoint).rgb; 
        vec3 positionIndirect = texture(positionRSM,samplePoint).rgb;

        float numerator = 
        max(0.0f,dot(normalP,positionIndirect-pixelPosition))*
        max(0.0f,dot(normalIndirect,pixelPosition-positionIndirect));

        float positionDistance = distance(pixelPosition,positionIndirect);
        float denominator = positionDistance*positionDistance*positionDistance*positionDistance;

        indirectFlux += randomSamplePoint.x*randomSamplePoint.x*albedoIndirect*10000.0f*numerator/denominator;
    }

}
#End