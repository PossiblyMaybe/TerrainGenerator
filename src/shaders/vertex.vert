#version 450 core
layout(binding = 0, std430) readonly buffer gridPts{
	vec4 pts[];
};

layout(location = 0) uniform mat4 view;
layout(location = 1) uniform mat4 projection;

void main(){
	gl_Position = projection * view * vec4(pts[gl_VertexID].xwz, 1.0f); 
}
