#VertexShader
#line 2

layout(location=0)in vec3 position;
layout(location=1)in vec2 texCoord;
layout(location=2)in vec3 normal;
layout(location=3)in vec4 tangent;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 shadowmapProjection;

out vec3 interpNormal;
out vec3 interpTangent;
out vec3 interpPosition;
out vec3 interpShadowmapCoords;
out vec2 interpTexCoords;

void main()
{    

    interpTangent=normalize(vec3(tangent));

    interpTexCoords=texCoord;
    vec4 shadowmapSpacePosition = (shadowmapProjection*vec4(position, 1.0f));
    interpShadowmapCoords = shadowmapSpacePosition.xyz/shadowmapSpacePosition.w;
    interpNormal=normal;
    interpPosition=position;
    gl_Position = projection*view*vec4(position,1.0);
}
#End
#FragmentShader
#line 37


uniform sampler2D albedoSampler;
uniform sampler2D roughnessMetalnessSampler;
uniform sampler2D normalSampler;

uniform float metalness;
uniform float roughness;
uniform vec3 albedo;

in vec3 interpTangent;
in vec3 interpNormal;
in vec3 interpPosition;
in vec3 interpShadowmapCoords;
in vec2 interpTexCoords;

uniform vec3 directionalLight;
uniform vec3 cameraPosition;
uniform sampler2D shadowmap;
uniform mat4 shadowmapProjection;
uniform sampler2D indirectLight;

out vec4 outputColor;

const int sampleSizeConstant = 2;
const int sampleOffsetStart = -sampleSizeConstant;
const int sampleOffsetEnd = sampleSizeConstant;
const int totalSampleNumber = (sampleSizeConstant-sampleOffsetStart+1)*(sampleSizeConstant-sampleOffsetStart+1);

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

void main()
{
   vec4 albedoTexture = texture(albedoSampler, interpTexCoords);

   if(albedoTexture.a<0.9)
   {
      discard;
   }
    vec3 albedo = pow(albedoTexture.rgb,vec3(2.2))*albedo;
    float metallic = texture(roughnessMetalnessSampler,interpTexCoords).b*metalness;
    float roughness = texture(roughnessMetalnessSampler,interpTexCoords).g*roughness;

    vec3 interpBitangent = cross(interpTangent,interpNormal);
    mat3 TBN = mat3(interpTangent,interpBitangent,interpNormal);

    vec3 normalTexture = (texture(normalSampler,interpTexCoords).rgb);
    normalTexture=(normalTexture.rgb-vec3(0.5f))*2.0f;
    vec3 normal = normalize(TBN*normalTexture);

    vec3 viewDirection = -normalize(cameraPosition - interpPosition);
    vec2 shadowMapCoordsTextureSpace = (interpShadowmapCoords.xy / 2.0f) + vec2(0.5f);
    float meshDistance = (interpShadowmapCoords.z+1.0f)/2.0f;

    ivec2 shadowMapSize = textureSize(shadowmap,0);
    float sampledShadowAmount = 0.0;
    for(int i=sampleOffsetStart;i<=sampleOffsetEnd;i++)
    {
        for(int j=sampleOffsetStart;j<=sampleOffsetEnd;j++)
        {
            float shadowMapDistance = texture(shadowmap,shadowMapCoordsTextureSpace + vec2(i,j) / shadowMapSize).x;
            if(!(shadowMapDistance< meshDistance-0.01f))
            {
                sampledShadowAmount += 1.0;
            }

        }
    }
    sampledShadowAmount/=totalSampleNumber;

    float irradiance = max(dot(normal,directionalLight),0.0f) * clamp(dot(interpNormal,directionalLight),0.0f,1.0f);
    
    vec3 radiance = vec3(8.0);
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 N = normalize(normal);
    vec3 V = normalize(-cameraPosition - interpPosition);
    

    vec3 light=transpose(mat3(shadowmapProjection))*vec3(0.0,0.0,-1.0);
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
    Lo = ((kD * albedo / PI + specular) * radiance * NdotL)*sampledShadowAmount; 
    vec2 screenCoord = gl_FragCoord.xy/vec2(1280,720);
    Lo+=texture(indirectLight,screenCoord).rgb*albedo;
    
    outputColor = vec4(Lo,1.0);
    //outputColor = vec4(texture(indirectLight,screenCoord).rgb,1.0);
}
#End