#VertexShader
#line 2

layout(location=0)in vec3 position;
#if DIFFUSE_TEXTURED || BUMP_MAP
    layout(location=1)in vec2 texCoord;
#endif
    layout(location=2)in vec3 normal;
#if BUMP_MAP
    layout(location=3)in vec3 tangent;
    out vec3 interpTangent;
#endif

uniform mat4 view;
uniform mat4 projection;
uniform mat4 shadowmapProjection;

out vec3 interpNormals;
out vec3 interpPosition;
out vec3 interpShadowmapCoords;

#if DIFFUSE_TEXTURED ||BUMP_MAP
    out vec2 v_texCoords;
#endif

void main()
{    
#if BUMP_MAP
    interpTangent=normalize(tangent);
#endif
#if DIFFUSE_TEXTURED || BUMP_MAP
    v_texCoords=texCoord;
#endif
    vec4 shadowmapSpacePosition = (shadowmapProjection*vec4(position, 1.0f));
    interpShadowmapCoords = shadowmapSpacePosition.xyz/shadowmapSpacePosition.w;
    interpNormals=normal;
    interpPosition=position;
    gl_Position = projection*view*vec4(position,1.0);
}
#End
#FragmentShader
#line 42

vec3 phongBRDF(vec3 lightDir, vec3 viewDir, vec3 normal, vec3 phongDiffuseCol, vec3 phongSpecularCol, float phongShininess) {
  vec3 color = phongDiffuseCol;
  vec3 reflectDir = reflect(-lightDir, normal);
  float specDot = max(dot(reflectDir, viewDir), 0.0f);
  color += pow(specDot, phongShininess) * phongSpecularCol;
  //color += pow(specDot, 4.0f) * vec3(1.0f,1.0f,1.0f);
  return color;
}

#if DIFFUSE_TEXTURED || BUMP_MAP
    in vec2 v_texCoords;
#endif

#if DIFFUSE_TEXTURED
    uniform sampler2D colorSampler;
#endif

#if DIFFUSE_SOLID
    uniform vec3 diffuseColor;
#endif

#if BUMP_MAP
    uniform sampler2D bumpSampler;
    in vec3 interpTangent;
#endif

in vec3 interpNormals;
in vec3 interpPosition;
in vec3 interpShadowmapCoords;

uniform vec3 directionalLight;
uniform vec3 cameraPosition;
uniform vec3 specularity;
uniform float shininess;
uniform sampler2D shadowmap;
uniform mat4 shadowmapProjection;

out vec3 outputColor;

const int sampleSizeConstant = 4;
const int sampleOffsetStart = -sampleSizeConstant;
const int sampleOffsetEnd = sampleSizeConstant;
const int totalSampleNumber = (sampleSizeConstant-sampleOffsetStart+1)*(sampleSizeConstant-sampleOffsetStart+1);


void main()
{
    
    
    vec3 viewDirection = -normalize(cameraPosition - interpPosition);
    vec3 radiance;
    vec3 outputNormal = interpNormals;
    vec3 ambientColor;
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
    float filteredShadowMap = float(sampledShadowAmount)/float(totalSampleNumber);
#if BUMP_MAP
    const ivec2 up = ivec2(0,1);
    float heightUp = textureOffset(bumpSampler,v_texCoords,up).r;
    const ivec2 down = ivec2(0,-1);
    float heightDown = textureOffset(bumpSampler,v_texCoords,down).r;
    const ivec2 right = ivec2(1,0);
    float heightRight = textureOffset(bumpSampler,v_texCoords,right).r;
    const ivec2 left = ivec2(-1,0);
    float heightLeft = textureOffset(bumpSampler,v_texCoords,left).r;
    const ivec2 center = ivec2(0,0);
    float heightCenter = textureOffset(bumpSampler,v_texCoords,center).r;
    
    float dispVertical = (heightUp - heightCenter) + (heightCenter - heightDown);
    float dispHorizontal = (heightLeft - heightCenter) + (heightCenter - heightRight);

    const float bumpFactor = 10.0f;
    vec3 bitangent = normalize(cross(interpNormals,interpTangent));
    outputNormal = normalize(interpNormals+bumpFactor*(dispHorizontal*interpTangent)+bumpFactor*(dispVertical*bitangent));

#endif
#if DIFFUSE_TEXTURED
    vec3 diffuseColorTexture = texture(colorSampler,v_texCoords).rgb;
    ambientColor = diffuseColorTexture;
    float irradiance = max(dot(outputNormal,directionalLight),0.0f) * clamp(dot(interpNormals,directionalLight),0.0f,1.0f);
    vec3 brdf = phongBRDF(directionalLight,viewDirection, outputNormal, diffuseColorTexture, specularity, shininess);
    
    radiance = brdf * irradiance * 0.7f * filteredShadowMap + ambientColor * 0.01f;
    
#elif DIFFUSE_SOLID
    ambientColor = diffuseColor;
    float irradiance = max(dot(interpNormals,directionalLight),0.0);
    vec3 brdf = phongBRDF(directionalLight,viewDirection, outputNormal, diffuseColor, specularity, shininess);
    
    radiance = brdf*irradiance * 0.7f * filteredShadowMap + ambientColor * 0.01f;
#else
    radiance=vec3(1.0f,0.0f,1.0f);
#endif

    outputColor= radiance;
}
#End