
uniform vec2 resolution;

void main()
{
 vec2 st = gl_FragCoord.xy / resolution.xy;
 
 vec4 color = vec4(1.0,0.0,0.0,1.0);
 gl_FragColor = color;
}
