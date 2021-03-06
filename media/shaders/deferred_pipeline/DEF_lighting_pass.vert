varying vec2 screen_pos;
varying vec3 eyelightpos;
uniform mat4 modelview;
#if defined LIGHT_SPOT || defined LIGHT_SUN
	varying vec3 eyespotdir;
#endif

void main()
{
	screen_pos = vec2(0.5,0.5) + gl_Vertex.st/2.0;
	eyelightpos = (modelview * gl_LightSource[0].position).xyz;

	#if defined LIGHT_SPOT || defined LIGHT_SUN
		eyespotdir = normalize((modelview * vec4(gl_LightSource[0].spotDirection,0)).xyz);
	#endif

	gl_Position = ftransform();
}

