#version 450 core
in vec3 vPos;
in vec3 vNormal;
out vec4 FragColor;




uniform vec3 lightPos;
uniform vec3 lightColour;

const vec3 nullTexColour = vec3(0.5f,0.5f,0.5f);
void main(){
	vec3 lightDir = lightPos - vPos;
	float diffuse = max(dot(normalize(lightDir),vNormal), 0.0);

	float strength = 1500000/pow(length(lightDir),2);
	//FragColor = vec4(((diffuse/10)+strength)*lightColour*nullTexColour, 1.0f); 
	FragColor = vec4((vPos.y/20) * nullTexColour,1.0f);
}
