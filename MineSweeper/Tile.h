#pragma once
#include <memory>

#include "Shader.hpp"
#include "Animation.h"

//Represent tile with animations
class Cell
{
public:
	//Struct that represents flag at the field
	class Flag
	{
	private:
		//Type of the current animation
		enum class States
		{
			PLACING,
			STANDING,
			REMOVING,
			REMOVED
		};

		//Helpers
		void changeAnimation(int frames, bool reverse, bool loop, int offset);
		
		void changeState(States state);
	public:
		//Fields
		std::unique_ptr<Animation> animation; 
		States state;

		//Constructor
		Flag();

		//Getters
		bool isRemoved() const;
		bool isRemoving() const;
		int getAnimationIndex() const;

		//Modifiers
		void set();
		void unset();

		//Drawing
		void draw(Shader* const&) const;

		//Updates
		void update();
	};

	//Enums
	enum Types
	{
		BLOCK,
		EMPTY,
		TILE_1,
		TILE_2,
		TILE_3,
		TILE_4,
		TILE_5,
		TILE_6,
		TILE_7,
		TILE_8,
		BOMB,
		NO_BOMB
	};

	//Fields
	Types type;
	std::unique_ptr<Flag> flag = nullptr;
	bool hasMine;
	bool isOpened = false;

	//Contructor
	Cell(Types type);

	//Getters
	bool hasFlag() const;

	//Modifiers
	void open();
	void toggleFlag();

	//Drawing
	void draw(glm::vec3 const& pos, glm::vec3 const& scale, Shader* const& shad) const;

	//Updates
	void update();
};

