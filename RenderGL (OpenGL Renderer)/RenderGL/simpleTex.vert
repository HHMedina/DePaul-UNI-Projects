// #version 150 - This is already in Header.vert

in vec4 surfacePosition;
in vec3 surfaceNormal;
in vec2 surfaceUV;

uniform mat4 modelViewTransform;
uniform mat4 normalTransform;
uniform mat4 modelViewProjection;
uniform mat4 viewInverseTransform;
uniform mat4 uvTransform;

out vec3 viewN;
out vec4 viewP;
out vec2 uv;

void main (void)
{	
	// To Do
	// 
	// Compute the view space position and normal as in Phong, but then also pass the 
	// surfaceUV to the fragment shader.  Later we will multuply the uv by a texture
	// transform for more control, but 

	viewP = modelViewTransform * surfacePosition;
	viewN = normalize(mat3(normalTransform)  * surfaceNormal);
	uv = (uvTransform * vec4(surfaceUV,0.0,1.0)).xy;
	gl_Position = modelViewProjection * surfacePosition;
}

