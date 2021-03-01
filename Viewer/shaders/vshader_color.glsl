#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in vec3 tangents;
layout(location = 4) in vec3 bitangents;

const float PI = 3.1415926535897932384626433832795;
// The model/view/projection matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 light;
uniform bool isOrtho;
uniform bool islight;
uniform bool istextures;
uniform bool isplanar;
uniform bool isspherical;
uniform vec3 lightsPosition[5];
uniform vec3 lightsDirection[5];
uniform bool normalmapping;
uniform int lightCount;




// These outputs will be available in the fragment shader as inputs
out vec3 orig_fragPos;
out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;
out vec3 lightpos;
out mat3 TBN;

void main()
{
	// Apply the model transformation to the 'position' and 'normal' properties of the vertex,
	// so the interpolated values of these properties will be available for usi n the fragment shader
	orig_fragPos = vec3(vec4(pos, 1.0f));	
	fragPos = vec3(model * vec4(pos, 1.0f));
	fragNormal = mat3(model) * normal;

	vec3 T = normalize(vec3( vec4(tangents,1.0f)));
	vec3 B = normalize(vec3( vec4(bitangents,1.0f)));
	vec3 N = normalize(vec3( vec4(normal,1.0f)));
	if(!isOrtho)
		{T = normalize(T - N * dot(N, T));
		B=cross(N,T);}
	TBN = mat3(T, B, N);

	
	
	gl_PointSize=15;
	

	// Pass the vertex texture coordinates property as it is. Its interpolated value
	// will be avilable for us in the fragment shader
	//fragTexCoords = texCoords;
	
	if(istextures)
		fragTexCoords = texCoords;
	else if(isplanar)
		fragTexCoords = vec2(orig_fragPos.x,orig_fragPos.y);
	else if(isspherical)
	{
		//fragTexCoords.x=0.5f + atan(orig_fragPos.x, orig_fragPos.z) / (2 * PI);
		//fragTexCoords.y= 0.5f - asin(orig_fragPos.y ) / PI;
		float r=sqrt(orig_fragPos.x*orig_fragPos.x + orig_fragPos.y*orig_fragPos.y + orig_fragPos.z*orig_fragPos.z);
		fragTexCoords.x=acos(orig_fragPos.z/r);
		fragTexCoords.y=atan(orig_fragPos.y/orig_fragPos.x);
	}

	// This is an internal OpenGL variable, we must set a value to this variable
	gl_Position = projection * view *  model * vec4(pos, 1.0f);

	if(islight)
		gl_Position = projection * view *  light * vec4(pos, 1.0f);


	

	
}