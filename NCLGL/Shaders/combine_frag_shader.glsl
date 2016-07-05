#version 450 core

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
	if(finalRender == 0)
	{
		//float exposure = 0.8;
		vec3 diffuse = texture(diffuseTex, IN.texCoord).xyz;
		diffuse += texture(reflTex, IN.texCoord).xyz;
		vec3 light = texture(emissiveTex, IN.texCoord).xyz;
		vec3 specular = texture(specularTex, IN.texCoord).xyz;
		vec4 irradiance = texture(irradianceTex, IN.texCoord);
		float ambient = irradiance.r;
		float alpha = irradiance.a;

		//vec3 mapped = vec3(1.0) - exp(-(diffuse * ambient) * exposure);

		FragColour.xyz = diffuse * ambient; //ambient light
		FragColour.xyz += diffuse * light; //lambert
		FragColour.xyz += specular; //specular
		FragColour.a = alpha;
	}
	else
	{
		FragColour = texture(blurTex, IN.texCoord);
	}
}