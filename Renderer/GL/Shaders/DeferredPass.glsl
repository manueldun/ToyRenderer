#VertexShader
#line 2


const vec2 triangleScreen[3] = vec2[3](
    vec2(-1.0f,-1.0f),
    vec2(4.0f,-1.0f),
    vec2(-1.0f,4.0f)
);
void main()
{    

    gl_Position = vec4(triangleScreen[gl_VertexID],0.0f,1.0f);
}
#End
#FragmentShader
#line 17

uniform sampler2D gPosition;
uniform sampler2D gAlbedo;
uniform sampler2D gNormal;
uniform sampler2D gMetallicPoughness;

uniform sampler2D albedoRSM;
uniform sampler2D normalRSM;
uniform sampler2D positionRSM;

uniform sampler2D shadowmap;
uniform mat4 lightMatrix;
uniform vec3 directionalLight;
uniform vec3 cameraPosition;

const int sampleSizeConstant = 1;
const int sampleOffsetStart = -sampleSizeConstant;
const int sampleOffsetEnd = sampleSizeConstant;
const int totalSampleNumber = (sampleSizeConstant-sampleOffsetStart+1)*(sampleSizeConstant-sampleOffsetStart+1);

out vec4 outputColor;


const float PI = 3.14159265359;
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   

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
    vec2 screenCoords = (gl_FragCoord.rg)/vec2(1280,720);
    
    vec3 normal = texture(gNormal,screenCoords).rgb;
    vec3 worldPosition = texture(gPosition,screenCoords).rgb;
    vec2 rsmTextureCoord = (lightMatrix*vec4(worldPosition, 1.0f)).xy/(lightMatrix*vec4(worldPosition, 1.0f)).w;
    
    vec3 indirectFlux=vec3(0.0f);
    const float sampleSpan = 0.2f;
    for(int i=0;i<50;i++)
    {

    
        vec2 randomSamplePoint = (vec3(pcg3d(uvec3(i,0,0)))/vec3(pow(2,32))).xy;
        vec2 samplePoint = (vec2(
            rsmTextureCoord.x + sampleSpan*randomSamplePoint.x*sin(2.0f*PI*randomSamplePoint.y),
            rsmTextureCoord.y + sampleSpan*randomSamplePoint.x*cos(2.0f*PI*randomSamplePoint.y))+1.0f)/2.0f;
            

        vec3 albedoIndirect = texture(albedoRSM,samplePoint).rgb;
        vec3 normalIndirect = texture(normalRSM,samplePoint).rgb; 
        vec3 positionIndirect = texture(positionRSM,samplePoint).rgb;

        float numerator = 
        max(0.0f,dot(normal,positionIndirect-worldPosition))*
        max(0.0f,dot(normalIndirect,worldPosition-positionIndirect));

        float positionDistance = distance(worldPosition,positionIndirect);
        float denominator = positionDistance*positionDistance*positionDistance*positionDistance;

        indirectFlux += randomSamplePoint.x*randomSamplePoint.x*albedoIndirect*12000.0f*numerator/denominator;
    }

    vec4 positionShadowMapSpace = lightMatrix*vec4(worldPosition,1.0f);
    vec3 positionShadowMapSpaceNormalized = positionShadowMapSpace.xyz/positionShadowMapSpace.w;
    vec3 shadowMapCoordsTextureSpace = (positionShadowMapSpaceNormalized.xyz / 2.0f) + 0.5f;
    float meshDistance = shadowMapCoordsTextureSpace.z;

    ivec2 shadowMapSize = textureSize(shadowmap,0);
    float sampledShadowAmount = 0.0;
    for(int i=sampleOffsetStart;i<=sampleOffsetEnd;i++)
    {
        for(int j=sampleOffsetStart;j<=sampleOffsetEnd;j++)
        {
            float shadowMapDistance = texture(shadowmap,shadowMapCoordsTextureSpace.xy + vec2(i,j) / shadowMapSize).x;
            if(!(shadowMapDistance< meshDistance-0.01f))
            {
                sampledShadowAmount += 1.0;
            }

        }
    }
    sampledShadowAmount/=totalSampleNumber;

    vec3 albedo = texture(gAlbedo,screenCoords).rgb;
    float metallic = texture(gMetallicPoughness,screenCoords).r;
    float roughness = texture(gMetallicPoughness,screenCoords).g;

    
    vec3 radiance = vec3(8.0);
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 N = normalize(normal);
    vec3 V = normalize(-cameraPosition - worldPosition);
    

    vec3 light = transpose(mat3(lightMatrix))*vec3(0.0,0.0,-1.0);
    vec3 L = normalize(light);
    vec3 H = normalize(V + L);
    vec3 Lo = vec3(0.0);
    float NDF = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
   
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	  
   
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;  
      
    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);                
    Lo = ((kD * albedo / PI + specular) * radiance * NdotL) * sampledShadowAmount; 
    Lo+=indirectFlux*albedo;

    outputColor = vec4(Lo,1.0f);
    
    if(length(normal)<0.5f)
    {
        outputColor=vec4(1.0f,1.0f,1.0f,1.0f);
    }
}
#End