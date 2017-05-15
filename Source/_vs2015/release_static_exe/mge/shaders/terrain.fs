//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D heightMap;
uniform sampler2D splatMap;

uniform sampler2D diffuse1;
uniform sampler2D diffuse2;
uniform sampler2D diffuse3;
uniform sampler2D diffuse4;

uniform float time;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform int shininess;

uniform float pointLightCount;
uniform vec3 pointLightColor[5];
uniform vec3 pointLightPos[5];
uniform vec3 pointAttenuation[5];

uniform float directionalLightCount;
uniform vec3 directionalLightColor[5];
uniform vec3 directionalLightDir[5];

uniform float spotLightCount;
uniform vec3  spotLightColor[5];
uniform vec3  spotLightPos[5];
uniform vec3  spotLightDir[5];
uniform vec3  spotLightAttenuation[5];
uniform float  spotLightAngle[5];
uniform float  spotFallOffRate[5];

uniform vec4 cameraPosition;

uniform bool debugMode;

in vec2 texCoord;
in vec4 fragWorldPosition;
in vec4 fragWorldNormal;

out vec4 fragment_color;

void main( void ) 
{
	//animate water texture
	vec2 uvDistanceFromCenter = texCoord - vec2(0.5, 0.5);
	float distanceFromCenter = length(uvDistanceFromCenter);
	float newDistanceFromCenter = distanceFromCenter + time;
	newDistanceFromCenter = mod(newDistanceFromCenter,1) +  0.35f;   //this line fixes water texture squishing over time...
	uvDistanceFromCenter = normalize(uvDistanceFromCenter) * newDistanceFromCenter;
	vec2 newUv = uvDistanceFromCenter+vec2(0.5, 0.5);
	
	//get fragment color of all difuse textures
	vec4 diffuse1FragmentColor = texture(diffuse1,texCoord*3);
	vec4 diffuse2FragmentColor = texture(diffuse2,newUv*3);
	vec4 diffuse3FragmentColor = texture(diffuse3,texCoord*3);
	vec4 diffuse4FragmentColor = texture(diffuse4,texCoord*3);
	
	//get fragment color without light
	vec4 splatMapFragmentColor = texture(splatMap,texCoord);
	vec4 fragmentColorNoLight = splatMapFragmentColor.x * diffuse1FragmentColor+
	splatMapFragmentColor.y * diffuse2FragmentColor+
	splatMapFragmentColor.z * diffuse3FragmentColor+
	splatMapFragmentColor.w * diffuse4FragmentColor;
	
	
	//directional lights
	vec4 specularTerm;
	vec4 diffuseTerm;
	for (int i = 0; i < directionalLightCount; i ++)
	{
		vec4 lightDirectionNorm = normalize(vec4(directionalLightDir[i],0));
		
		vec4 reflectedRay = lightDirectionNorm - 2*dot(lightDirectionNorm, fragWorldNormal)*fragWorldNormal;
		vec4 fragToCameraNorm = cameraPosition - fragWorldPosition;
		fragToCameraNorm.w = 0;
		fragToCameraNorm = normalize(fragToCameraNorm);
		
		diffuseTerm += max(dot(-lightDirectionNorm, fragWorldNormal),0) * fragmentColorNoLight * vec4(directionalLightColor[i],1)* vec4(diffuseColor,1); 
		specularTerm += pow(max(dot(reflectedRay, fragToCameraNorm),0), shininess) * vec4(directionalLightColor[i],1) * vec4(specularColor,1); 
	}
	
	//point lights
	for (int i = 0; i < pointLightCount; i ++)
	{
		vec4 lightDirectionNorm = normalize(fragWorldPosition - vec4(pointLightPos[i],1));
		
		float lightDistance = length (vec4(pointLightPos[i],1) - fragWorldPosition);
		float attenuationParameter = 1/(pointAttenuation[i].x + pointAttenuation[i].y * lightDistance + pointAttenuation[i].z * lightDistance * lightDistance );
		
		vec4 reflectedRay = lightDirectionNorm - 2*dot(lightDirectionNorm, fragWorldNormal)*fragWorldNormal;
		
		vec4 fragToCameraNorm = cameraPosition - fragWorldPosition;
		fragToCameraNorm.w = 0;
		fragToCameraNorm = normalize(fragToCameraNorm);
		
		diffuseTerm += max(dot(-lightDirectionNorm, fragWorldNormal),0) * fragmentColorNoLight * vec4(pointLightColor[i],1) * vec4(diffuseColor,1) * attenuationParameter;
		specularTerm += pow(max(dot(reflectedRay, fragToCameraNorm), 0), shininess) * vec4(pointLightColor[i],1) * vec4(specularColor,1) * attenuationParameter;
	}
	//spot lights
	
	for (int i = 0; i < spotLightCount; i ++)
	{
		
		vec4 lightDirectionNorm = normalize(fragWorldPosition - vec4(spotLightPos[i],1));
		float lightAngleRelativeToLightForward = acos(dot(lightDirectionNorm, vec4(spotLightDir[i],0)));
		
		if( lightAngleRelativeToLightForward < radians(spotLightAngle[i]))
		{
		
			float fallOffParameter = (min(radians(spotFallOffRate[i]), radians(spotLightAngle[i]) - lightAngleRelativeToLightForward))/radians(spotFallOffRate[i]);

			float lightDistance = length (vec4(spotLightPos[i],1) - fragWorldPosition);
			float attenuationParameter = 1/(spotLightAttenuation[i].x + spotLightAttenuation[i].y * lightDistance + spotLightAttenuation[i].z * lightDistance * lightDistance );
			
			
			vec4 reflectedRay = lightDirectionNorm - 2*dot(lightDirectionNorm, fragWorldNormal)*fragWorldNormal;
			vec4 fragToCameraNorm = cameraPosition - fragWorldPosition;
			fragToCameraNorm.w = 0;
			fragToCameraNorm = normalize(fragToCameraNorm);
		
			diffuseTerm += max(dot(-lightDirectionNorm, fragWorldNormal),0) *  vec4(spotLightColor[i],1)[i] *fragmentColorNoLight* vec4(diffuseColor,1) * attenuationParameter*fallOffParameter;
			specularTerm += pow(max(dot(reflectedRay, fragToCameraNorm), 0), shininess) * vec4(spotLightColor[i],1)[i] * vec4(specularColor,1) * attenuationParameter*fallOffParameter;
		}
	}
	
	vec4 finalColor = specularTerm+diffuseTerm;
	if(debugMode)
	{
		finalColor = vec4(fragWorldPosition.y, fragWorldPosition.y,fragWorldPosition.y,1); 
	}
	fragment_color = vec4(finalColor.x,finalColor.y,finalColor.z,fragmentColorNoLight.w);
}
