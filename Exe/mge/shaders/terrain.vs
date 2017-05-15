//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 projectionMatrix;
uniform	mat4 viewMatrix;
uniform	mat4 modelMatrix;

uniform sampler2D heightMap;
uniform sampler2D splatMap;

uniform float waveRadiusMaxOffset;
uniform float waveRadiusMaxOffsetFrequency;
uniform float waveWidth;
uniform float waveMaxHeight;
uniform float waveMaxHeightOffset;
uniform float waveHeightOffsetFrequency;
uniform float waveCount;

uniform float intensity;
uniform float time;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec2 texCoord; //make sure the texture coord is interpolated

out vec4 fragWorldPosition;
out vec4 fragWorldNormal;

void main( void )
{
	//get new local position of vertex
	vec3 offset = vec3(0,texture(heightMap,uv).x*intensity,0);
	vec4 newLocalPosition = vec4(vertex+offset, 1.f);
	
	//get actual vertex normal
	float size = 32;
	vec3 nextVertexRight = vertex + vec3(1./size,0,0) + vec3(0,texture(heightMap,uv+vec2(1./size,0)).x*intensity,0);
	vec3 nextVertexLeft = vertex + vec3(-1./size,0,0)+ vec3(0,texture(heightMap,uv+vec2(-1./size,0)).x*intensity,0);
	vec3 nextVertexUp = vertex + vec3(0,0,1./size)+ vec3(0,texture(heightMap,uv+vec2(0,-1./size)).x*intensity,0);
	vec3 nextVertexDown = vertex + vec3(0,0,-1./size)+ vec3(0,texture(heightMap,uv+vec2(0,1./size)).x*intensity,0);
	
	vec3 slope1 = nextVertexRight-nextVertexLeft;
	vec3 slope2 = nextVertexDown - nextVertexUp;
	vec3 actualNormal = cross(slope1,slope2);
	
	//if its water texture
	if(texture(splatMap,uv).y != 0)
	{
		for(int i = 0; i < waveCount; i ++) //make 5 waves
		{
			//get current radius of the wave
			float currentRadius = 1-mod(time, 1)+ i*(1/waveCount);
			currentRadius = mod(currentRadius, 1);
			currentRadius += sin(uv.x*waveRadiusMaxOffsetFrequency+uv.y*waveRadiusMaxOffsetFrequency)*waveRadiusMaxOffset;//sine offset for radius
				
			float uvToWaveDistance = length(vec2(0.5,0.5) - uv) - currentRadius;//distance of current uv to wave
			
			if (uvToWaveDistance < waveWidth/2 && uvToWaveDistance > -waveWidth/2) //uvToWaveDistance from -0.025 to 0.025 means this uv is on the wave
			{
				float waveHeight = uvToWaveDistance/(waveWidth/2); //value from -1 to 1
				if(waveHeight < 0){waveHeight *= -1;} //value fronm 0 to 1
				waveHeight = 1 - waveHeight;// invert value(changes wave moving direction)
			
				vec4 currentWaveHeight = vec4(0,sin(waveHeight* (3.14/2) )*waveMaxHeight,0,0);
				vec4 heightSineOffset = vec4(0,(sin(uv.x*waveHeightOffsetFrequency +uv.y*waveHeightOffsetFrequency ))*waveMaxHeightOffset,0,0);//sine offset for wave height
					
				float finalHeight = (heightSineOffset + currentWaveHeight).y;
					
				if(newLocalPosition.y < finalHeight)
				{
					newLocalPosition.y = finalHeight; 
				}
			}
		}
	}
	
	texCoord = uv;

	fragWorldPosition = modelMatrix * newLocalPosition;
	fragWorldNormal = normalize(modelMatrix * vec4(actualNormal,0));
	
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * newLocalPosition;
}
