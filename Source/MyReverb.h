/*
  ==============================================================================

    Reverb.h
    Created: 1 Jun 2022 12:01:40pm
    Author:  Enrico Damiani

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MyReverb
{
public:
    MyReverb();
    ~MyReverb();

    void setSampleRate(float newSampleRate);

    void process(const float* leftChannelIn,
                 const float* rightChannelIn,
                 float* leftChannelOut,
                 float* rightChannelOut,
                 int numSamples);

private:
};