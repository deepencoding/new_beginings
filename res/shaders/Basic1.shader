#SHADER VERTEX
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 atCoords;
layout (location = 2) in vec2 nCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 texCoord;
out vec2 gifCoord;

void main() {
	gl_Position = proj * view * model * vec4(aPos, 1.0f);
	texCoord = atCoords;
	gifCoord = nCoords;
};

#SHADER FRAGMENT
#version 330 core

in vec2 texCoord;
in vec2 gifCoord;

uniform vec3 triColor;
uniform int x_offset;
uniform int y_offset;
uniform sampler2D oTexture1;
uniform sampler2D oTexture2;
uniform sampler2D oTexture3;

out vec4 FragColor;

void main() {
	// FragColor = vec4(triColor, 1.0f);
	// FragColor = texture(oTexture3, gifCoord);
	// FragColor = mix(texture(oTexture2, texCoord), texture(oTexture3, gifCoord), 0.5f);
	FragColor = texture(oTexture3, vec2(gifCoord.x + (0.2f * x_offset), gifCoord.y - (0.167f * y_offset)));
};