#version 330 core

uniform sampler2D diffuseTex;
uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;
 uniform sampler2D bumpTex ;

struct PointLight
{
	 vec4 lightColour;
	 vec3 lightPos;
	 float lightRadius;
};

uniform PointLight pointLights[2];


in Vertex {
	vec3 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent ; // New !
	vec3 binormal ; // New !
	vec3 worldPos;
 } IN;

out vec4 fragColour;

void main(void)
{
	vec3 incident = normalize(pointLights[0].lightPos - IN.worldPos );
	vec3 viewDir = normalize(cameraPos - IN.worldPos );
	vec3 halfDir = normalize(incident + viewDir );

	 mat3 TBN = mat3 ( normalize(IN.tangent) ,normalize(IN.binormal) , normalize( IN.normal ));


	vec4 diffuse = texture(diffuseTex , IN.texCoord );
	vec3 bumpNormal = texture(bumpTex,IN.texCoord).rgb ;
	bumpNormal = normalize ( TBN * normalize ( bumpNormal * 2.0 - 1.0));



	float lambert = max(dot(incident , bumpNormal), 0.0f);

	float distance = length(pointLights[0].lightPos - IN.worldPos );
	float attenuation = 1.0 - clamp(distance / pointLights[0].lightRadius , 0.0, 1.0);

	float specFactor = clamp(dot(halfDir,bumpNormal) ,0.0 ,1.0);
	specFactor = pow(specFactor , 60.0 );

	vec3 surface = (diffuse.rgb * pointLights[0].lightColour.rgb);


	fragColour.rgb = surface * lambert * attenuation;
	fragColour.rgb += (pointLights[0].lightColour.rgb * specFactor )* attenuation *0.33;
	fragColour.rgb += surface * 0.1f; // ambient!
	fragColour.a = diffuse.a;
}