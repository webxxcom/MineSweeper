#include "MineSweeper.h"

void MineSweeper::initBuffers() const
{
	constexpr auto NDC_TILE = glm::highp_vec2{ 30.f / 360.f, 30.f / 90.f };
	constexpr GLfloat vertices[] =
	{	
		//Triangles		//Textures
		-1.f, 1.f,		0.f, NDC_TILE.y,
		-1.f, 0.f,		0.f, 0.f,
		0.f, 1.f,		NDC_TILE.x, NDC_TILE.y,

		-1.f, 0.f,		0.f, 0.f,
		0.f, 1.f,		NDC_TILE.x, NDC_TILE.y,
		0.f, 0.f,		NDC_TILE.x, 0.f
	};


	GLuint VBO;
	GLuint VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}

void MineSweeper::initTextures() const
{
	std::vector<const char*> textures_paths{
		"resources/tiles.png"
	};

	std::vector<GLuint> textures(textures_paths.size());
	glGenTextures((GLsizei)textures_paths.size(), textures.data());

	GLint width;
	GLint height;
	GLint nrChannels;
	stbi_set_flip_vertically_on_load(true);
	for (GLenum i = 0; i < textures_paths.size(); ++i)
	{
		//Enable transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		auto data = stbi_load(textures_paths[i], &width, &height, &nrChannels, 0);
		if (data)
		{
			GLenum format = GL_RGBA;
			if (nrChannels == 3)
				format = GL_RGB;

			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D); 
			stbi_image_free(data);
		}
		else std::cerr << "Failed to load texture" << std::endl;
	}
}

void MineSweeper::initShader()
{
	this->shad = std::make_unique<Shader>("vert_shad.vert", "frag_shad.frag");
}

void MineSweeper::initField()
{
	this->field.resize(GRID_NUMBER);

	std::vector<bool> mines(GRID_NUMBER * GRID_NUMBER, false);
	std::ranges::fill_n(mines.begin(), NUMBER_OF_MINES, true);
	std::ranges::shuffle(mines, std::mt19937(std::random_device()()));

	//Count bombs around the cell not including the cell
	auto countBombsAround = [&](int i, int j) {
		int bombs_number = 0;
		for (int k = i - 1; k <= i + 1; ++k)
			for (int l = j - 1; l <= j + 1; ++l)
				if (!outOfBounds(l, k))
					bombs_number += (int)mines[k * GRID_NUMBER + l];

		return bombs_number;
		};

	//Now traverse the field and set the cell type according to a number of bombs around it
	for (int i = 0; i < GRID_NUMBER; ++i)
		for (int j = 0; j < GRID_NUMBER; ++j)
		{
			Cell::Types type = mines[i * GRID_NUMBER + j] ? Cell::BOMB 
				: Cell::Types(Cell::EMPTY + countBombsAround(i, j));

			field[i].push_back(std::make_unique<Cell>(type));
		}
}

void MineSweeper::updateField() const
{
	for (auto const& row : field)
		for (auto const& tile : row)
			tile->update();
}

void MineSweeper::openCells(glm::ivec2 const& cellPos)
{
	//If bomb is clicked
	if (auto const& cell = field[cellPos.y][cellPos.x]; cell->type == Cell::BOMB)
	{
		bombClicked();
		return;
	}

	//If number is clicked
	else if (cell->type != Cell::EMPTY)
	{
		cell->open();
		return;
	}

	//If user clicked at the empty cell then try to open all the possible adjacent empty cell using breadth first search
	std::queue<glm::ivec2> cells;
	cells.push(cellPos);

	while(!cells.empty())
	{
		auto curr_cell_pos = cells.front();
		field[curr_cell_pos.y][curr_cell_pos.x]->open();
		cells.pop();

		if (field[curr_cell_pos.y][curr_cell_pos.x]->type != Cell::EMPTY)
			continue;

		std::vector<glm::ivec2> directions{ {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
		std::ranges::for_each(directions, [&](auto const& dir) {
			auto next_pos = dir + curr_cell_pos;

			if (!outOfBounds(next_pos.x, next_pos.y)
				&& !field[next_pos.y][next_pos.x]->isOpened
				&& field[next_pos.y][next_pos.x]->type != Cell::BOMB)

				cells.push(next_pos);
			}
		);
	}
}

void MineSweeper::bombClicked()
{
	std::cout << "\n\n\nGAME OVER\n\n\n";
	gameOver();
}

void MineSweeper::isVictory()
{
	bool flag = false;
	for (auto const& row : field)
		for (auto const& col : row)
			if (!col->isOpened && col->type != Cell::BOMB)
			{
				flag = true;
				break;
			}

	if(!flag)
	{
		std::cout << "\n\n\nWIIIIIN!!!!\n\n\n";
		gameOver();
	}
}

void MineSweeper::gameOver()
{
	this->isGame = false;
}

bool MineSweeper::outOfBounds(int i, int j) const
{
	return i < 0 || i >= GRID_NUMBER || j < 0 || j >= GRID_NUMBER;
}

GLFWimage MineSweeper::loadIcon() const
{
	GLFWimage icon{};
	stbi_set_flip_vertically_on_load(false);
	icon.pixels = stbi_load("resources/icon.png", &icon.width, &icon.height, nullptr, 4);
	if (!icon.pixels)
	{
		std::cerr << "\n\nICON IMAGE WAS NOT LOADED\n\n";
		return GLFWimage{};
	}

	return icon;
}

void MineSweeper::init()
{
	this->initBuffers();
	this->initTextures();
	this->initShader();
	this->initField();
}

void MineSweeper::processInput(int key, int action)
{
	if (!isGame)
		return;
	
	//Set game process to pause
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
		isGame = !isGame;
}

void MineSweeper::processMouse(double xpos, double ypos, int button, int action)
{
	if (!isGame)
		return;

	const glm::ivec2 pos = { xpos / TILE_SIDES.x, ypos / TILE_SIDES.y };
	if (pos.y >= GRID_NUMBER || pos.x >= GRID_NUMBER)
		return;
	auto const& tile = field[pos.y][pos.x];

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		openCells(pos);

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		tile->toggleFlag();
}

//Displays all entities
void MineSweeper::render() const
{
	shad->use();
	for (uint16_t i = 0; i < GRID_NUMBER; ++i)
	{
		for (uint16_t j = 0; j < GRID_NUMBER; ++j)
		{
			auto& cell = field[i][j];
			cell->draw(
				convert(TILE_SIDES * (glm::vec2(j, i) + 1.f)),
				glm::vec3((TILE_SIDES * 2.f) / glm::vec2(RESOLUTION), 0.f),
				shad.get()
			);
		}
	}
}

//Updates all objects
void MineSweeper::update()
{
	if (isGame)
	{
		this->updateField();
		this->isVictory();
	}
}
