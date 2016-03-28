// #version 150 - This is already in Header.vert

// Input Variables for Vertex ... look in "DrawTriangles" in C++ for where
// these get set!
//
// Notice that "attribute" variables from earlier GLSL versions become 
// input "in" variables in GLSL 1.5 
in vec4 surfacePosition;
in vec3 surfaceNormal;

// For all the uniforms below, look in the scene's RegisterUniformParameters
// for where these get set!

// Uniforms for transforms 
uniform mat4 modelViewTransform;
uniform mat4 normalTransform;
uniform mat4 modelViewProjection;

// Uniforms for lights.  Note that we have a limit of 8 lights here.  This could 
// easily be changed to accommodate a larger lighting rig but performance may suffer 
// on some hardware
uniform int nLights;
uniform vec4 globalAmbient;
uniform vec4 lightPosition[8];
uniform vec4 lightColor[8];
uniform vec4 lightSpecular[8];

// Uniforms for the surface material
uniform vec4 surfaceDiffuse;
uniform vec4 surfaceSpecular;
uniform float surfaceShininess;

// Outputs to send to the fragment shader
//
// Notice that "varying" variables from earlier GLSL versions become 
// output "out" variables for the vertex shader in version 1.5
out vec4 outputColor;

void main (void)
{	
	vec4 ambient = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);

	// To Do
	// 
	// Calculate the view-space position, normal and view vector
	//
	// Then make sure that the normal is facing toward the viewer, if it isn't, then reverse it

	// Now loop through the lights.  Most GPU's will allow a uniform to be used as the limit on 
	// a loop, but if you have a problem compiling, change "nLights" here to the constant 8.
	for (int i = 0; i < nLights; i++)
	{
		// To Do 
		// 
		// Compute the light vector L and the reflection of the light vector R, and then use them 
		// to compute the diffuse reflectance.

		// Finally compute the specular highlight. Note that if we are looking at the back side with
		// respect to the light (i.e. dot(N, L) < 0) then there should be no specular highlight
	}

	// Finally calculate the ambient term
	ambient = globalAmbient * surfaceDiffuse;	

	// Now set the output color variable
	outputColor = ambient + diffuse + specular;

	// Finally calculate the projected position of the vertex onscreen
	// Note that gl_Position is one of the LAST gl_ variable left in GLSL 1.5
	gl_Position = modelViewProjection * surfacePosition;
}
