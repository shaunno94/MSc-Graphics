#version 450 core

uniform sampler2D depthTex;
uniform sampler2D normTex;
uniform sampler2D irradianceTex;
uniform sampler2DShadow shadowTex;
uniform mat4 shadowMatrix;

uniform vec2 pixelSize;
uniform vec3 cameraPos;

uniform float lightRadius;
uniform vec3 lightPos;
uniform vec4 lightColour;
uniform float intensity;
uniform int dir;

in mat4 invProjView;

layout(location = 0) out vec4 diffuseColour;
layout(location = 1) out vec4 specularColour;

void main(void)	
{
	//NDC - X,Y (0 to 1.0)
	vec3 pos = vec3((gl_FragCoord.x * pixelSize.x), (gl_FragCoord.y * pixelSize.y), 0.0);
	//Sample depth buffer texture, store value in Z.
	pos.z = texture(depthTex, pos.xy).r;
	if(pos.z == 1)
	{
		discard;
	}
	float sFac = texture(irradianceTex, pos.xy).g;
	float alpha = texture(irradianceTex, pos.xy).a;

	//Sample normal texture, scale and bias (-1 to 1.0)
	vec3 normal = normalize(texture(normTex, pos.xy).xyz * 2.0 - 1.0);

	//Calculate world space co-ordinates using inverse view projection matrix. (scale and bias (-1 to 1.0))
	vec4 clip = invProjView * vec4((pos * 2.0) - 1.0, 1.0);
	pos = clip.xyz / clip.w;

	float atten = 1.0;
	if(dir == 0)
	{
		//Distance from light to fragment
		float dist = length(lightPos - pos);
		atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);	
		//If the light is too far, discard this fragment.
		if(atten == 0.0)
		{
			discard;
		}
	}
	//Light ray direction
	vec3 incident = normalize(lightPos - pos);
	//View Direction
	vec3 viewDir = normalize(cameraPos - pos);
	//Half Angle
	vec3 halfDir = normalize(incident + viewDir);
	
	//Diffuse factor //HDR no clamp
	float lambert = clamp(dot(incident, normal), 0.0, 1.0);
	//Specular Factor.
	float specular = clamp(dot(normal, halfDir), 0.0, 1.0);
	specular = pow(specular, sFac);

	if(dir == 1)
	{
		float offset = clamp(length(pos - incident), 1.5, 35.0);
		vec4 shadowProj = (shadowMatrix * vec4(pos + (normal * offset), 1.0));
		float shadow = 1.0;
		if(shadowProj.w > 0.0)
		{
			shadow = textureProj(shadowTex, shadowProj);
		}
		lambert *= shadow * alpha;
	}

	diffuseColour = vec4(lightColour.xyz * lambert * atten, 1.0);
	specularColour = vec4(lightColour.xyz * specular * atten * 0.35, 1.0);
}