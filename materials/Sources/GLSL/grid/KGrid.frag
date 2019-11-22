
uniform vec2 resolution;
uniform vec2 gridInterval;
uniform float sw;

void main()
{
    vec2 st = gl_FragCoord.xy / resolution.xy;
    vec2 st2 = st;
    vec3 color = vec3(0.0);

    
    st *= vec2(resolution.x / gridInterval.x , resolution.y / gridInterval.x);
    st = fract(st);
    
    vec2 bl = step(vec2(0.05), st);
    vec2 tr = step(vec2(0.05), 1.0-st);
    
    
    st2 *= vec2(resolution.x / gridInterval.y , resolution.y / gridInterval.y);
    st2 = fract(st2);
    
    vec2 bl2 = step(vec2(0.02), st2);
    vec2 tr2 = step(vec2(0.02), 1.0-st2);
    
  // The multiplication of left*bottom will be similar to the logical AND.
    if(sw > 0.0){
        color = vec3((bl.x * bl.y * tr.x * tr.y) * (bl2.x * bl2.y * tr2.x * tr2.y) + 0.9);
        //color *= vec3();

    }else color = vec3(1.0,1.0,1.0);
        
        
  gl_FragColor = vec4(color,1.0);
    
}
