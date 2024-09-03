#pragma once

#include "Timer.hpp"
#include <iostream>

class Animation
{
private:
	int frames;
	int curr_frame;
	int frame_index;
	bool isLooped;
	bool isReversed;
	float delay;
	bool isFinished = false;
	Timer timer{};

public:
	explicit Animation(int number_of_frames, bool isReversed = false, bool isLooped = false, int offset = 0, float delay = 0.085f);

	//Updates
	void advanceFrame();
	void restart();
	void update();

	//Getters
	bool reachedLastFrame() const;
	bool getIsFinished() const;
	int getFrameIndex() const;
	int getFrameNumber() const;
	int getNumberOfFrames() const;
};
