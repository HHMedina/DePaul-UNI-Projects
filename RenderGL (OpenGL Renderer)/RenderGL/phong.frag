// #version 150 - This is already in Header.frag

in vec3 viewN;
in vec4 viewP;

uniform int nLights;
uniform vec4 globalAmbient;
uniform vec4 lightPosition[8];
uniform vec4 lightColor[8];
uniform vec4 lightSpecular[8];

uniform mat4 viewTransform;

uniform vec4 surfaceDiffuse;
uniform vec4 surfaceSpecular;
uniform float surfaceShininess;

out vec4 fragmentColor;

void main(void)
{		
	vec4 ambient = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);

	vec3 toLight;
	vec3 Rvec;
	float NdotL;

	// To Do
	//
	// Just like you did in the gouraud shader, compute the diffuse and specular reflectance.
	// One note is that you should do the forward-facing normal computation here in the fragment
	// shader, and OpenGL has a nice built-in variable glFrontFacing to help you do that (but
	// it is only available in the fragment shader.  
	//
	// Use the varying inputs provided by the vertex shader, but note that you may need to re-normalize
	// them here.
		
	vec3 toViewer = normalize( vec3(-viewP)); // in view space, camera is at 0,0,0 so -viewP = ViewTrans*CamPos - viewP 

	vec3 viewNNorm = normalize(faceforward(viewN, -toViewer, viewN));

	for (int i = 0; i < nLights; i++)
	{
		toLight = normalize(lightPosition[i].xyz - viewP.xyz);//Are lightPositions elements in view spacE??? only works if not applying view transform.
		Rvec = normalize(reflect(-toLight,viewNNorm));
		NdotL = dot(viewNNorm,toLight);
		
		diffuse += max( NdotL ,1e-6) * surfaceDiffuse * lightColor[i];
		
		if(NdotL > 1e-6)
		{
			//RenderGL uses a different light color for specular. use lightSpecular[i] instead of lightColor[i]
			specular += lightSpecular[i]  *  surfaceSpecular * pow( max( dot(toViewer,Rvec) ,1e-6) , surfaceShininess );
		}
	}


	ambient = globalAmbient * surfaceDiffuse;
		

	fragmentColor = ambient + diffuse + specular;
}

