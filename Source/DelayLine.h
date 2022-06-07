/*
  ==============================================================================

    DelayLine.h
    Created: 1 Jun 2022 1:43:15pm
    Author:  Enrico Damiani

  ==============================================================================
*/

#pragma once

#include <math.h>
#include <iostream>
#include "JuceHeader.h"

class DelayLine
{
public:
	void initDelay(int _delayInSamples, double _sampleRate)
	{
		sampleRate = _sampleRate;
		setDelayLengthInSamples(_delayInSamples);

		for (int i = 0; i < maxDelayTime; i++)
			delay[i] = 0.0f;
	}

	void setDelayTime(float _delayInMS)
	{

		if (_delayInMS < 0) {
			delayInMS = 0;
		}
		delayInMS = _delayInMS;
		delayInSamples = ceil(sampleRate * 0.001 * delayInMS);

		if (delayInSamples > maxDelayTime)
			delayInSamples = maxDelayTime;
	}

	void setDelayLengthInSamples(int lengthInSamples)
	{
		delayInSamples = lengthInSamples;

		if (delayInSamples > maxDelayTime)
			delayInSamples = maxDelayTime;
	}

	void next(float input)
	{
		delay[pos] = input;
		pos = (pos + 1) % (delayInSamples);
		//DBG("Input: " << juce::String(input));
		//DBG("Current position: " << juce::String(pos));
	}

	float getOutput()
	{
		int readPos = pos - delayInSamples;

		if (readPos < 0)
			readPos += delayInSamples;

		//DBG("Delayed position: " << juce::String(readPos));
		//DBG("Output: " << delay[readPos]);
		return delay[readPos];
	}

private:
	static const int maxDelayTime = 96000;
	float delayInMS;
	unsigned int delayInSamples = 0;
	unsigned int pos = 0;

	double sampleRate = 48000;
	float delay[maxDelayTime] = {};
};