// #version 150 - This is already in Header.vert

in vec4 surfacePosition;

uniform mat4 modelViewProjection;

void main (void)
{	
	gl_Position = modelViewProjection * surfacePosition;
}
