#version 450 core
layout(binding = 0, std430) readonly buffer gridPts{
	vec4 pts[];
};

layout(binding = 5, std430) readonly buffer normalsSSBO{
	vec3 normals[];
};

out vec3 vPos;
out vec3 vNormal;

uniform mat4 pvMatrix;

void main(){
	gl_Position = pvMatrix * vec4(pts[gl_VertexID].xzy, 1.0f); 
	vPos = pts[gl_VertexID].xzy;
	vNormal = normals[gl_VertexID];
}
