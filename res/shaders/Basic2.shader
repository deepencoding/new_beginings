#SHADER VERTEX
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 oNormal;
out vec3 oFragPos;
out vec2 oTexCoords;

void main() {
	gl_Position = proj * view * model * vec4(aPos, 1.0);
	oFragPos = vec3(model * vec4(aPos, 1.0));
	// oNormal = mat3(transpose(inverse(model))) * aNormal;
	oNormal = aNormal;
	oTexCoords = TexCoords;
};

#SHADER FRAGMENT
#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 oNormal;
in vec3 oFragPos;
in vec2 oTexCoords;

// uniform vec3 objectColor;
// uniform vec3 lightPos;
// uniform vec3 lightColor;
uniform vec3 viewPos;
uniform sampler2D earthTex;
uniform Material material;
uniform Light light;

out vec4 FragColor;

void main() {
	// float ambientStr = 0.1;
	// vec3 ambient = material.ambient * lightColor;
	vec3 ambient = light.ambient * material.ambient;

	vec3 normal = normalize(oNormal);
	vec3 lightDir = normalize(light.position - oFragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// float specularStr = 0.5;
	vec3 viewDir = normalize(viewPos - oFragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	vec3 objectColor = texture(earthTex, oTexCoords).xyz;
	vec3 result = (ambient + diffuse + specular) * objectColor;
	// vec3 result = (ambient + diffuse) * objectColor + specular;
	// vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
};