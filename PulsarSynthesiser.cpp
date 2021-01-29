/*
  ==============================================================================

    PulsarSynthesiser.cpp
    Created: 29 Jan 2021 9:43:29am
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#include "PulsarSynthesiser.h"

PulsarSynthesiser::PulsarSynthesiser()
{
    for (auto i = 0; i < 5; ++i)
    {
        addVoice (new PulsarSynthVoice);
    }

    setVoiceStealingEnabled (true);
}

PulsarSynthesiser::~PulsarSynthesiser()
{
    
}

void PulsarSynthesiser::prepare(double sampleRate, int blockSize)
{
    mSampleRate = sampleRate;
    mBlockSize = blockSize;
    
    setCurrentPlaybackSampleRate(sampleRate);
}

void PulsarSynthesiser::renderNextSubBlock(AudioBuffer<float> &outputAudio, int startSample, int numSamples)
{
    for (auto* v : voices)
    {
        
    }
    
    MPESynthesiser::renderNextSubBlock (outputAudio, startSample, numSamples);
}

