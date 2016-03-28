//#version 150

out vec3 viewN;
out vec4 viewP;
out vec2 uv;

in vec4 surfacePosition;
in vec3 surfaceNormal;
in vec2 surfaceUV;

uniform mat4 modelViewTransform;
uniform mat4 normalTransform;
uniform mat4 modelViewProjection;
uniform mat4 uvTransform;

void main (void)
{	
	viewN = normalize(mat3(normalTransform) * surfaceNormal);
	viewP = modelViewTransform * surfacePosition;

	uv = (uvTransform * vec4(surfaceUV, 0, 1)).st;	
	gl_Position = modelViewProjection * surfacePosition;
}
