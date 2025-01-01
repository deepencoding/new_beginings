#SHADER VERTEX
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 atCoords;

uniform mat4 transform;

out vec2 texCoord;

void main() {
	gl_Position = transform * vec4(aPos, 1.0f);
	texCoord = atCoords;
};

#SHADER FRAGMENT
#version 330 core

in vec2 texCoord;

uniform vec3 triColor;
uniform sampler2D oTexture1;
uniform sampler2D oTexture2;

out vec4 FragColor;

void main() {
	// FragColor = vec4(triColor, 1.0f);
	FragColor = mix(texture(oTexture2, texCoord), texture(oTexture1, texCoord) * vec4(triColor, 1.0f), 0.2f);
};