#VertexShader
#line 2
uniform mat4 projection;
uniform mat4 view;
uniform mat4 lightMatrix;
layout(location=0) in vec3 position;
void main()
{
	gl_Position = projection*view*lightMatrix*vec4(position,1.0);
}
#End
#FragmentShader
#line 13
out vec3 outColor;
uniform vec3 color;
void main()
{
	outColor=color;
}
#End