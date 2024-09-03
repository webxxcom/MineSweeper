#include "Animation.h"

Animation::Animation(int number_of_frames, bool isReversed, bool isLooped, int offset, float delay)
	: frames(number_of_frames), curr_frame(offset),
	frame_index(isReversed ? frames - 1 - offset : offset), isLooped(isLooped),
	isReversed(isReversed), delay(delay)
{

}

void Animation::advanceFrame()
{
	++curr_frame;
	frame_index += isReversed ? -1 : 1;
}

void Animation::restart()
{
	curr_frame = 0;
	frame_index = isReversed ? frames - 1 : 0;
}

void Animation::update()
{
	timer.stop();

	//Do nothing if delay was not reached
	if (timer.getElapsedTime() < delay)
		return;

	//Reset the frame counter if the last frame was reached
	if (reachedLastFrame())
	{
		if (isLooped) restart();
		else isFinished = true;
	}
	else advanceFrame();

	timer.start();
}

bool Animation::reachedLastFrame() const
{
	return getFrameNumber() == frames - 1;
}

bool Animation::getIsFinished() const
{
	return isFinished;
}

int Animation::getFrameIndex() const
{
	return frame_index;
}

int Animation::getFrameNumber() const
{
	return curr_frame;
}

int Animation::getNumberOfFrames() const
{
	return frames;
}
