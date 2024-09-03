#include "Tile.h"

/////////////////////// TILE ///////////////////////

Cell::Cell(Types type)
	: type(type), hasMine(type == BOMB)
{

}

bool Cell::hasFlag() const
{
	return flag != nullptr;
}

void Cell::open()
{
	if (!isOpened && !hasFlag() && !hasMine)
		isOpened = true;
}

void Cell::toggleFlag()
{
	if (hasFlag()) 
	{
		flag->isRemoving() ? flag->set() : flag->unset();
	}
	else if(!isOpened)
	{
		//Create an instance of a flag which will automatically set it
		flag = std::make_unique<Flag>();
	}
}

void Cell::draw(glm::vec3 const& pos, glm::vec3 const& scale, Shader* const& shad) const
{
	glm::mat4 model{ 1.f };
	model = glm::translate(model, pos);
	model = glm::scale(model, scale);

	shad->setUniform("model", model);
	shad->setUniform("row", 2);
	shad->setUniform("column", isOpened ? type : BLOCK);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	if (hasFlag())
		flag->draw(shad);
}

void Cell::update()
{
	if (hasFlag())
	{
		flag->update();
		if (flag->isRemoved())
			flag = nullptr;
	}
}

/////////////////////// FLAG ///////////////////////

void Cell::Flag::changeAnimation(int frames, bool reverse, bool loop, int offset)
{
	animation = std::make_unique<Animation>(frames, reverse, loop, offset);
}

void Cell::Flag::changeState(States st)
{
	using enum States;

	//Set offset if flag was either raising and then started to lower or vice versa
	int offset = (animation && (state == PLACING && st == REMOVING) || (state == REMOVING && st == PLACING))
		? animation->getNumberOfFrames() - 1 - animation->getFrameNumber() : 0; 
	int frames = st == STANDING ? 3 : 9;	//Removing and adding animations have the same number of frames
	bool isLooped = st == STANDING;			//Loop animation only if flag is standing
	bool isReversed = st == REMOVING;		//Reverse animation only if flag is removing

	this->state = st;
	changeAnimation(frames, isReversed, isLooped, offset);
}

Cell::Flag::Flag()
{
	this->set();
}

bool Cell::Flag::isRemoved() const
{
	return state == States::REMOVED;
}

bool Cell::Flag::isRemoving() const
{
	return state == States::REMOVING;
}

int Cell::Flag::getAnimationIndex() const
{
	return state == States::STANDING ? 0 : 1;
}

void Cell::Flag::set()
{
	changeState(States::PLACING);
}

void Cell::Flag::unset()
{
	changeState(States::REMOVING);
}

void Cell::Flag::draw(Shader* const& shad) const
{
	shad->setUniform("row", getAnimationIndex());
	shad->setUniform("column", animation->getFrameIndex());
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Cell::Flag::update()
{
	//Update animation in any case
	animation->update();
		
	//If flag is removing check if the animation finished and then if so modify the flag state
	if (isRemoving())
	{
		if (animation->getIsFinished())
			state = States::REMOVED;
		return;
	}

	//If first animation has finished which is the only one that can be finished if flag is not removing
	if (!animation->getIsFinished())
		return;
	 
	changeState(States::STANDING);
}