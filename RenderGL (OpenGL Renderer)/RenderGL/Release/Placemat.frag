#version 150

in vec3 viewN;
in vec4 viewP;
in vec2 uv;

uniform int nLights;
uniform vec4 lightPosition[8];   // By default we support up to 8 lights
uniform vec4 lightColor[8];
uniform vec4 globalAmbient;

uniform vec4 surfaceDiffuse;
uniform vec4 surfaceSpecular;
uniform float surfaceShininess;

uniform sampler2D laceColor;
uniform sampler2D laceOpacity;

out vec4 fragColor;

void main (void)
{		
	vec4 ambient = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
	vec3 N, L, R, V;
	float NdotL;

	// To Do
	//
	// Look up the surface color as usual for a diffuse map, and then do a separate lookup 
	// for the opacity from the "laceOpacity" map.  
	vec4 surfaceColor = texture(laceColor,uv);
	vec4 surfaceOpacity = texture(laceOpacity,uv);
	
	vec4 specularColor = surfaceSpecular;

	V = normalize(-viewP).xyz; 
	N = normalize(faceforward(viewN, -V, viewN));
	for (int i = 0; i < nLights; i++)
	{
		L = normalize(lightPosition[i].xyz - viewP.xyz);
		R = normalize(reflect(-L, N));
		NdotL = dot(N, L);

		diffuse += lightColor[i] * surfaceColor * max(NdotL, 1e-6);
		if (NdotL > 1e-6)
			specular += lightColor[i] * specularColor * 
				   pow(max(dot(V, R), 1e-6), surfaceShininess);
	}
	ambient = globalAmbient * surfaceColor;	
	
	// Then use the surface opacity's red channel to set the  alpha value of the frag color
	// Note that you can add the ambient, diffuse and specular by selecting the r, g and b 
	// components by swizzling.
	fragColor = vec4(ambient.xyz + diffuse.xyz + specular.xyz,surfaceOpacity.x);
}
