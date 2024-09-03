#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <numeric>
#include <random>
#include <queue>

#include "stb_image.h"
#include "Tile.h"

class MineSweeper
{
public:
	//Resolutions
	const char* TITLE = "MineSweeper";
	const glm::vec2 SCALE{ 1.5f };
	const int GRID_NUMBER = 16;
	const glm::vec2 TILE_SIDES{SCALE * 30.f};
	const glm::ivec2 RESOLUTION{ TILE_SIDES * float(GRID_NUMBER) };
	const int NUMBER_OF_MINES = 40;

private:
	//Convert function
	glm::vec3 convert(glm::vec2 const& vals) const
	{
		return {
			((2 * vals.x) / RESOLUTION.x) - 1.f,
			1.f - ((2 * vals.y) / RESOLUTION.y),
			0.f
		};
	}

	//Fields
	std::unique_ptr<Shader> shad;
	std::vector<std::vector<std::unique_ptr<Cell>>> field;
	bool isGame = true;

	//Initializations
	void initBuffers() const;
	void initTextures() const;
	void initShader();
	void initField();

	//Updates
	void updateField() const;
	void openCells(glm::ivec2 const& firstCell);
	void bombClicked();
	void isVictory();
	void gameOver();

	//Helpers
	bool outOfBounds(int i, int j) const;

public:
	//Loaders
	GLFWimage loadIcon() const;
	void init();
	
	//Updatings
	void processInput(int key, int action);
	void processMouse(double xpos, double ypos, int button, int action);
	void render() const;
	void update();
};

