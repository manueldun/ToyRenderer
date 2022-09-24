#VertexShader
#line 2
layout(location=0)in vec3 position;
layout(location=1)in vec2 texCoord;
layout(location=2)in vec3 normal;
layout(location=3)in vec4 tangent;

uniform mat4 projection;
out vec3 worldPosition;
out vec3 interpNormal;
out vec3 interpTangent;
out vec2 interpTexCoord;
void main()
{
	vec4 normalizedPosition = projection*vec4(position,1.0f);
	worldPosition = position;
	interpNormal=normal;
	interpTexCoord=texCoord;
	interpTangent = tangent.xyz;
    gl_Position = normalizedPosition;
}

#End
#FragmentShader
#line 23

in vec3 worldPosition;
in vec3 interpNormal;
in vec3 interpTangent;
in vec2 interpTexCoord;
uniform vec3 albedoConstant;
uniform sampler2D albedoSampler;
uniform sampler2D normalMapSampler;
layout(location=0)out vec3 outputPosition;
layout(location=1)out vec3 outputNormal;
layout(location=2)out vec3 outputAlbedo;

uniform sampler2D normalSampler;
void main()
{

	outputPosition = worldPosition;

    vec3 bitangent = cross(interpTangent.xyz,interpNormal);
    mat3 TBN = mat3(interpTangent.xyz,bitangent,interpNormal);
	

	vec3 normalMap = texture(normalMapSampler,interpTexCoord).rgb;
	normalMap=(normalMap.rgb-vec3(0.5f))*2.0f;
	outputNormal = TBN*normalMap;

	
	outputAlbedo= texture(albedoSampler,interpTexCoord).rgb * albedoConstant;


}
#End