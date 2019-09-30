#version 150

#define NUMBERWAVES 4

const float PI = 3.141592654;
const float G = 9.81;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat3 inverseViewNormalMatrix;

uniform float passedTime;

uniform float waterPlaneLength;

uniform vec4 waveParameters[NUMBERWAVES];
uniform vec2 waveDirections[NUMBERWAVES];

in vec4 vertex;

out vec3 vIncident;
out vec3 vBitangent;
out vec3 vNormal;
out vec3 vTangent;
out vec2 vTexCoord;

void main(void)
{
	vec4 finalVertex;

	finalVertex.x = a_vertex.x;
	finalVertex.y = a_vertex.y;
	finalVertex.z = a_vertex.z;
	finalVertex.w = 1.0;

	vec3 finalBitangent;
	vec3 finalNormal;
	vec3 finalTangent;

	finalBitangent.x = 0.0;
	finalBitangent.y = 0.0;
	finalBitangent.z = 0.0;
	
	finalNormal.x = 0.0;
	finalNormal.y = 0.0;
	finalNormal.z = 0.0;

	finalTangent.x = 0.0;
	finalTangent.y = 0.0;
	finalTangent.z = 0.0;

	// see GPU Gems: Chapter 1. Effective Water Simulation from Physical Models
	for (int i = 0; i < NUMBERWAVES; i++)
	{
		vec2 direction = normalize(u_waveDirections[i]);
		float speed = waveParameters[i].x;
		float amplitude = waveParameters[i].y;
		float wavelength = waveParameters[i].z;
		float steepness = waveParameters[i].w;

		float frequency = sqrt(G*2.0*PI/wavelength);
		float phase = speed*frequency;
		float alpha = frequency*dot(direction, vertex.xz)+phase*passedTime;
		
		finalVertex.x += steepness*amplitude*direction.x*cos(alpha);
		finalVertex.y += amplitude*sin(alpha);
		finalVertex.z += steepness*amplitude*direction.y*cos(alpha);
	}

	for (int i = 0; i < NUMBERWAVES; i++)
	{
		vec2 direction = normalize(u_waveDirections[i]);
		float speed = waveParameters[i].x;
		float amplitude = waveParameters[i].y;
		float wavelength = waveParameters[i].z;
		float steepness = waveParameters[i].w;

		float frequency = sqrt(G*2.0*PI/wavelength);
		float phase = speed*frequency;
		float alpha = frequency * dot(direction, finalVertex.xz) + phase*passedTime;
				
		// x direction
		finalBitangent.x += steepness * direction.x*direction.x * wavelength * amplitude * sin(alpha);
		finalBitangent.y += direction.x * wavelength * amplitude * cos(alpha);	
		finalBitangent.z += steepness * direction.x*direction.y * wavelength * amplitude * sin(alpha);	
	
		// y direction
		finalNormal.x += direction.x * wavelength * amplitude * cos(alpha);
		finalNormal.y += steepness * wavelength * amplitude * sin(alpha);
		finalNormal.z += direction.y * wavelength * amplitude * cos(alpha);

		// z direction
		finalTangent.x += steepness * direction.x*direction.y * wavelength * amplitude * sin(alpha);
		finalTangent.y += direction.y * wavelength * amplitude * cos(alpha);	
		finalTangent.z += steepness * direction.y*direction.y * wavelength * amplitude * sin(alpha);	
	}

	finalTangent.x = -finalTangent.x;
	finalTangent.z = 1.0 - finalTangent.z;
	finalTangent = normalize(finalTangent);

	finalBitangent.x = 1.0 - finalBitangent.x;
	finalBitangent.z = -finalBitangent.z;
	finalBitangent = normalize(finalBitangent);

	finalNormal.x = -finalNormal.x;
	finalNormal.y = 1.0 - finalNormal.y;
	finalNormal.z = -finalNormal.z;
	finalNormal = normalize(finalNormal);
	
	vBitangent = finalBitangent;
	vNormal = finalNormal;
	vTangent = finalTangent;
	
	vTexCoord = vec2(clamp((finalVertex.x+waterPlaneLength*0.5-0.5)/waterPlaneLength, 0.0, 1.0), clamp((-finalVertex.z+waterPlaneLength*0.5+0.5)/waterPlaneLength, 0.0, 1.0));

	vec4 tVertex = viewMatrix*finalVertex;
	
	// We caculate in world space.
	vIncident = inverseViewNormalMatrix * vec3(tVertex);	
				
	gl_Position = projectionMatrix*tVertex;
}
