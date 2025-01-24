#SHADER VERTEX
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 oTexCoords;

void main() {
	gl_Position = proj * view * model * vec4(aPos, 1.0f);
	oTexCoords = texCoords;
};

#SHADER FRAGMENT
#version 330 core

in vec2 oTexCoords;

uniform sampler2D sunTex;

out vec4 FragColor;

void main() {
	// FragColor = vec4(1.0f);
	FragColor = texture(sunTex, oTexCoords);
};