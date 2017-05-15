#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 projectionMatrix;
uniform	mat4 viewMatrix;
uniform	mat4 modelMatrix;
uniform	float time;
uniform	float maxOffset;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec2 texCoord; //make sure the texture coord is interpolated

void main( void )
{
	
	float offset = (sin (time* (vertex.x+ vertex.y+ vertex.z))); //value from -1 to 1; 
	float currentOffset = maxOffset * offset;//value from -maxVertexOffset to maxVertexOffset;
	
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4 (vertex + (vertex * currentOffset), 1.0f);
	
	texCoord = uv;
}
