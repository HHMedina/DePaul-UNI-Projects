// #version 150 - This is already in Header.frag

// The constant shader simply passes the surfaceDiffuse color from the object to
// the output color
uniform vec4 surfaceDiffuse;

// We now have to explicitly declare the output variable for a fragment shader
out vec4 fragmentColor;

void main (void)
{		
	// We simply write the surfaceDiffuse color to the framgent 
	fragmentColor = surfaceDiffuse;
}
