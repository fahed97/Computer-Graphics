#version 330 core

struct Material
{
	sampler2D textureMap;
	sampler2D normalMap;
	sampler2D diffuse;
	sampler2D specular;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	int alpha;
	// You can add more fields here...
	// Such as:
	//		1. diffuse/specular relections constants
	//		2. specular alpha constant
	//		3. Anything else you find relevant
};


// We set this field's properties from the C++ code
uniform Material material;
uniform bool islight;
uniform	vec3 eye;
uniform vec3 Ambient[5];
uniform vec3 Diffuse[5];
uniform vec3 Specular[5];
uniform vec3 lightsPosition[5];
uniform vec3 lightsDirection[5];
uniform float lightsType[5];
uniform int lightCount;
uniform bool isFogExists;
uniform bool isLinearFog;
uniform float FogStart;
uniform float FogEnding;
uniform float FogDensity;
uniform bool isOrtho;
uniform bool istextures;
uniform bool isplanar;
uniform bool isspherical;
uniform bool normalmapping;

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;
in vec3 lightpos;
in mat3 TBN;
// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
	// Sample the texture-map at the UV coordinates given by 'fragTexCoords'
	vec3 textureColor = vec3(texture(material.diffuse, fragTexCoords));
	vec3 I_a,I_d,I_s,LightDirection,temp,Normal;
	if(normalmapping)
	{
		Normal = vec3(texture(material.normalMap, fragTexCoords).rgb* 2.0 - 1.0);
		//vec3 tempNormal = vec3(Normal * 2.0 - 1.0);   
		Normal = normalize((TBN) * Normal);
		
	}
	else
		Normal=fragNormal;
	if(lightCount>0)
	{
		for(int i=0;i<lightCount;i++)
		{
			if(lightsType[i]==0)//Point Light
			LightDirection=normalize(fragPos-lightsPosition[i]);
			else
			LightDirection=lightsDirection[i];
			
			I_a+=vec3(material.Ambient.x*Ambient[i].x,material.Ambient.y*Ambient[i].y,material.Ambient.z*Ambient[i].z);
			//if(istextures||isplanar||isspherical||normalmapping)
			//{
			//temp=vec3(Diffuse[i].x * textureColor.x, Diffuse[i].y *textureColor.y, Diffuse[i].z * textureColor.z);
			//}
			//else
				temp=vec3(material.Diffuse.x * Diffuse[i].x, material.Diffuse.y * Diffuse[i].y, material.Diffuse.z * Diffuse[i].z);
			float i_n = max(0.f,dot(-(Normal), LightDirection));
			I_d+=i_n*temp;
	
			vec3 temp_Normal;
			//if(isOrtho==false)
			//	temp_Normal = fragNormal;
			//else
			temp_Normal = -Normal;
			//if(istextures||isplanar||isspherical||normalmapping)
			//{
			//temp=vec3(Specular[i].x * textureColor.x, Specular[i].x *textureColor.y, Specular[i].z * textureColor.z);
			//}
			//else
				temp = vec3(material.Specular.x * Specular[i].x, material.Specular.y * Specular[i].y, material.Specular.z * Specular[i].z);
			vec3 r = vec3(2.f * dot(temp_Normal, LightDirection) * Normal - LightDirection);
			float Power = (pow(max(0.0f, dot((r), normalize(eye))), material.alpha));
			I_s+=vec3(temp * Power);
	
	
				
		
		}
		
	
	frag_color = vec4(I_a+I_d+I_s,1.f);
	
	}

	//if(lightCount>0)
	//{
	//	for(int i=0;i<lightCount;i++)
	//	{
	//		if(lightsType[i]==0)//Point Light
	//		LightDirection=normalize(fragPos-lightsPosition[i]);
	//		else
	//		LightDirection=lightsDirection[i];
	//		
	//		I_a+=Ambient[i]  * vec3(texture(material.diffuse, fragTexCoords));
	//		temp=Diffuse[i] * vec3(texture(material.diffuse, fragTexCoords));
	//		float i_n = max(0.f,dot(-(Normal), LightDirection));
	//		I_d+=i_n*temp;
	//
	//		vec3 temp_Normal;
	//		temp_Normal = -Normal;
	//		temp = Specular[i] * vec3(texture(material.specular, fragTexCoords));
	//		vec3 r = vec3(2.f * dot(temp_Normal, LightDirection) * Normal - LightDirection);
	//		float Power = (pow(max(0.0f, dot((r), normalize(eye))), material.alpha));
	//		I_s+=vec3(temp * Power);
	//
	//
	//			
	//	
	//	}
	//	
	//
	//frag_color = vec4(I_a+I_d+I_s,1.f);
	//
	//}
	//else
	//frag_color = vec4(textureColor,1.f);
	

	if(islight)
	{
		frag_color=vec4(1.f,1.f,1.f,1.f);
	}
	
}
