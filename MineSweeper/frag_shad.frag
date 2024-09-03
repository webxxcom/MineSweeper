#version 330
out vec4 FragColor;

in vec2 xTextureCoords;

const vec2 tile_sides = vec2(30.f / 360.f, 30.f / 90.f);

uniform sampler2D texture1;
uniform int row;
uniform int column;

void main()
{
	vec2 textureCoord = vec2(
		xTextureCoords.x + tile_sides.x * column,
		xTextureCoords.y + tile_sides.y * row
	);

	FragColor = texture(texture1, textureCoord);
}