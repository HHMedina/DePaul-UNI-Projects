// #version 150 - This is already in Header.vert

in vec4 surfacePosition;
in vec3 surfaceNormal;

uniform mat4 modelViewTransform;
uniform mat4 normalTransform;
uniform mat4 modelTransform;
uniform mat4 modelViewProjection;

out vec3 viewN;
out vec4 viewP;

void main(void)
{	
	// To Do
	//
	// Compute the normal N and position P in view coordinates and pass them to the 
	// fragment shader

	viewP = modelViewTransform * surfacePosition;
	viewN = normalize(mat3(normalTransform)  * surfaceNormal);
	gl_Position = modelViewProjection * surfacePosition;
}
