
attribute vec4 position;
attribute vec4 normal;

uniform float shiness;
varying float Diffuse;

uniform vec4 materialColour;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec4 lightPosition;
uniform vec3 vertexOffset;
uniform float vertexScaleIn;
uniform float vertexScaleOut;
uniform vec3 vertexXYZAmount;

varying float lightIntensity;

uniform vec3 positiveStretch;
uniform vec3 negativeStretch;


uniform float stretch_amount;


void main()
{
    vec4 light = viewMatrix * lightPosition;
    lightIntensity = dot (light, normal);
    vec3 vertex = position.xyz + noise3 (vertexOffset + position.xyz * vertexScaleIn * vertexXYZAmount) *
    vertexScaleOut * 1.0;

    vec3 transformed = vertex;
    vec3 p_s = (positiveStretch - 1.0) * stretch_amount;
    vec3 n_s = (negativeStretch - 1.0) * stretch_amount;

    if(transformed.x >= 0.0)
    {
        transformed.x = vertex.x * (1.0 + p_s.x);
    }else if(transformed.x < 0.0)
    {
        transformed.x = vertex.x * (1.0 + n_s.x);
    }

    if(transformed.y >= 0.0)
    {
        transformed.y = vertex.y * (1.0 + p_s.y);
    }else if(transformed.y < 0.0)
    {
        transformed.y = vertex.y * (1.0 + n_s.y);
    }

    if(transformed.z >= 0.0)
    {
        transformed.z = vertex.z * (1.0 + n_s.z);
    }else if(transformed.z < 0.0)
    {
        transformed.z = vertex.z * (1.0 + p_s.z);
    }



    gl_Position = projectionMatrix * viewMatrix * vec4(transformed, 1.0);

}
