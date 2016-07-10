/*
Author: Shaun Heald
This fragment shader is used to combine all previously rendered textures.
*/

#version 420 core

uniform sampler2D diffuseTex;
uniform sampler2D emissiveTex;
uniform sampler2D specularTex;
uniform sampler2D irradianceTex;
uniform sampler2D reflTex;
uniform sampler2D normTex;
uniform sampler2D blurTex;
uniform int finalRender;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 FragColour;

void main(void)	
{
	if(finalRender == 0) //Pre, post-process
	{
		//Albedo
		vec3 diffuse = texture(diffuseTex, IN.texCoord).xyz;
		diffuse += texture(reflTex, IN.texCoord).xyz;
		//Diffuse light (emissive)
		vec3 light = texture(emissiveTex, IN.texCoord).xyz;
		//Specular light
		vec3 specular = texture(specularTex, IN.texCoord).xyz;
		vec4 irradiance = texture(irradianceTex, IN.texCoord);
		
		float ambient = irradiance.r;
		float alpha = irradiance.a;

		FragColour.xyz = diffuse * ambient; //ambient light
		FragColour.xyz += diffuse * light; //diffuse
		FragColour.xyz += specular; //specular
		FragColour.a = alpha;
	}
	else //Final image
	{
		FragColour = texture(blurTex, IN.texCoord);
	}
}