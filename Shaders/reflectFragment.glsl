#version 330 core

uniform sampler2D diffuseTex ;
uniform samplerCube cubeTex ;
uniform sampler2D shadowTex; 


uniform vec3 cameraPos ;

in Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
	vec4 shadowProj; // New !
 }IN ;

out vec4 fragColour;

void main ( void ) {
	vec4 diffuse = texture ( diffuseTex , IN.texCoord );
	vec3 viewDir = normalize ( cameraPos - IN.worldPos );

	vec3 reflectDir = reflect ( -viewDir , normalize ( IN.normal ));
	vec4 reflectTex = texture ( cubeTex , reflectDir );

	fragColour = reflectTex + ( diffuse * 0.25f );

	float shadow = 1.0f; // New !
	vec3 shadowNDC = IN.shadowProj.xyz / IN.shadowProj.w ;
	if( abs ( shadowNDC.x ) < 1.0f && abs ( shadowNDC.y ) < 1.0f && abs ( shadowNDC.z ) < 1.0f ) 
	{
		vec3 biasCoord = shadowNDC *0.5f + 0.5f ;
		float shadowZ = texture ( shadowTex , biasCoord.xy ).x ;
		if( shadowZ < biasCoord.z ) 
		{
			fragColour.a = 0.3f;
			return;
		}
	}

}
