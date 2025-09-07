#shader vertex
#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_transform;

out vec3 Normal;
out vec3 FragPos;

void main() {
    vec4 worldPos = u_model * u_transform * vec4(aPos, 1.0);
    FragPos = vec3(worldPos);
    Normal = mat3(transpose(inverse(u_model))) * aNormal;
    gl_Position = u_projection * u_view * worldPos;
}


#shader fragment
#version 430 core

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 u_lightDir;
uniform vec4 u_lightColor;
uniform vec4 u_color;

// === SDF UBO ===
struct SDFPrimitive {
    int type;        // 0=Plane,1=Triangle,2=Cube
    vec3 position;
    vec3 scale;
    vec3 rotation;   // Euler
};

layout(std140, binding = 0) uniform SDFPrimitivesBlock {
    int u_primitiveCount;
    SDFPrimitive u_primitives[128];
};

// === Funções auxiliares ===
vec3 applyInverseRotation(vec3 p, vec3 euler) {
    float cx = cos(-euler.x); float sx = sin(-euler.x);
    float cy = cos(-euler.y); float sy = sin(-euler.y);
    float cz = cos(-euler.z); float sz = sin(-euler.z);

    mat3 rotX = mat3(1,0,0,0,cx,-sx,0,sx,cx);
    mat3 rotY = mat3(cy,0,sy,0,1,0,-sy,0,cy);
    mat3 rotZ = mat3(cz,-sz,0,sz,cz,0,0,0,1);

    return rotZ * rotY * rotX * p;
}

float sdfPlane(vec3 p) { return p.y; }
float sdfBox(vec3 p, vec3 b) {
    vec3 q = abs(p) - b;
    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}
float sdfTriangle(vec3 p) { return length(p.xy)-1.0; }

float sceneSDF(vec3 p) {
    float d = 1e9;
    for(int i=0;i<u_primitiveCount;i++) {
        SDFPrimitive prim = u_primitives[i];
        vec3 localP = (p - prim.position)/prim.scale;
        localP = applyInverseRotation(localP, prim.rotation);
        float dist = 1e9;
        if(prim.type==0) dist = sdfPlane(localP);
        else if(prim.type==1) dist = sdfTriangle(localP);
        else if(prim.type==2) dist = sdfBox(localP, vec3(0.5));
        d = min(d, dist);
    }
    return d;
}

// Raymarch de sombra
float shadowRay(vec3 origin, vec3 lightDir, float maxDist) {
    float t = 0.01;
    float res = 1.0;
    for(int i=0;i<64;i++){
        float h = sceneSDF(origin + lightDir*t);
        if(h<0.001) return 0.0;
        res = min(res, 10.0*h/t);
        t += h;
        if(t>maxDist) break;
    }
    return clamp(res,0.0,1.0);
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-u_lightDir);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_lightColor.rgb;

    float diff = max(dot(lightDir,norm),0.0);
    
    // shadow por fragmento
    float shadow = shadowRay(FragPos + norm*0.01, lightDir, 50.0);

    vec3 result = (ambient + diff*shadow*u_lightColor.rgb) * u_color.rgb;
    FragColor = vec4(result,u_color.a);
}