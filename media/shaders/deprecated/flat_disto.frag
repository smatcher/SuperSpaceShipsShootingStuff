uniform sampler2D color_map;
uniform sampler2D disto_map;
varying vec3 normal, lightDir, eyeVec;

void main (void)
{
	vec4 disto = texture2D(disto_map, gl_TexCoord[0].st);
	vec4 color = texture2D(color_map, gl_TexCoord[0].st + 0.03*disto.rg);
/*
	vec4 final_color = color * (
	(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + 
	(gl_LightSource[0].ambient * gl_FrontMaterial.ambient));
							
	vec3 N = normalize(normal);
	vec3 L = normalize(lightDir);
	
	float lambertTerm = dot(N,L);
	
	if(lambertTerm > 0.0)
	{
		final_color += color * (
							gl_LightSource[0].diffuse * 
		               gl_FrontMaterial.diffuse * 
					   lambertTerm);	
		
		vec3 E = normalize(eyeVec);
		vec3 R = reflect(-L, N);
		float specular = pow( max(dot(R, E), 0.0), 
		                 gl_FrontMaterial.shininess );
		final_color += gl_LightSource[0].specular * 
		               gl_FrontMaterial.specular * 
					   specular;	
	}

	gl_FragColor = final_color;			
*/
	gl_FragColor = color;
}

