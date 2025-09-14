#shader vertex
#version 450 core

layout (location = 0) in vec2 aPos;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
};

#shader fragment
#version 450 core

out vec4 FragColor;

uniform vec3 u_Resolution;
uniform vec3 u_RotationAngle;
uniform float u_Time;

struct Hit {
    float dist;
    vec4 color;
};

Hit max(Hit a, Hit b)
{
    Hit h;
    h.dist = max(-b.dist, a.dist);
    h.color = mix(b.color, a.color, 1);
    return h;
}

Hit smoothMin(Hit a, Hit b, float blendValue) {
    Hit r;
    float h = max (blendValue - abs(a.dist - b.dist), 0.0) / blendValue;
    r.dist = min (a.dist, b.dist) - h * h * h * blendValue * (1.0 / 6.0);

    r.color = mix(b.color, a.color, h);
    return r;
}

Hit planeSDF(vec3 p) {
    Hit h;
    h.dist = p.y + 1.0;
    h.color = vec4(0, 0, 1, 1);
    return h;
}

Hit sphereSDF(vec3 p)
{
    Hit h;
    h.dist = length(p) - 1;
    h.color = vec4(1, 0, 0, 1);
    return h;
}

Hit boxSDF(vec3 p, vec3 b)
{
    Hit h;
    vec3 q = abs(p) - b;
    h.dist = length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
    h.color = vec4(0, 1, 0, 1);
    return h;
}

Hit sceneSDF(vec3 point) {
    vec3 spherePos = vec3(sin(u_Time) * 3, 0, 0);
    
    Hit sphere = sphereSDF(point - spherePos);
    Hit box = boxSDF(point, vec3(1));
    Hit groundPlane = planeSDF(point);

    Hit blended = smoothMin(sphere, box, 1);

    Hit res = groundPlane;
    if (blended.dist < res.dist) res = blended;

    return res;
}

mat3 rotY(float a) {
    float s = sin(a);
    float c = cos(a);
    return mat3(
        c, 0, -s,
        0, 1,  0,
        s, 0,  c
    );
}

mat3 rotX(float a) {
    float s = sin(a);
    float c = cos(a);
    return mat3(
        1, 0,  0,
        0, c, -s,
        0, s,  c
    );
}

void main()
{
    vec2 uv = (gl_FragCoord.xy * 2 - u_Resolution.xy) / u_Resolution.y;

    vec3 ro = vec3(0, 0, -5);
    vec3 rd = normalize(vec3(uv, 1));

    rd = rotY(u_RotationAngle.y) * rotX (u_RotationAngle.x) * rd;
    ro = rotY(u_RotationAngle.y) * rotX (u_RotationAngle.x) * ro;

    vec4 col = vec4(0);

    float t = 0.0f;
    Hit d;

    for(int i = 0; i < 80; i++) {
        vec3 p = ro + rd * t;
        
        d = sceneSDF(p);
        
        t += d.dist;
        
        if (d.dist < 0.001 || t > 100) break;
    }

    col = (d.dist < 0.001) ? d.color : vec4(0.0);
    FragColor = col;
};
