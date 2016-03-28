// #version 150 - This is already in Header.frag

in vec4 outputColor;

out vec4 fragmentColor;

void main (void)
{		
	// We simply write the interpolated output color to the framgent 
	// ... i.e. Gouraud Shading!
	fragmentColor = outputColor;
}
