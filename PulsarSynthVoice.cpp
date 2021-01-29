/*
  ==============================================================================

    PulsarSynthVoice.cpp
    Created: 29 Jan 2021 9:35:35am
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#include "PulsarSynthVoice.h"

PulsarSynthVoice::PulsarSynthVoice()
{
    
}

PulsarSynthVoice::~PulsarSynthVoice()
{
    
}

void PulsarSynthVoice::noteStarted()
{
    auto velocity = getCurrentlyPlayingNote().noteOnVelocity.asUnsignedFloat();
    auto freqHz = (float) getCurrentlyPlayingNote().getFrequencyInHertz();
    
    DBG(freqHz);
}

void PulsarSynthVoice::notePitchbendChanged()
{
    
}

void PulsarSynthVoice::noteStopped(bool allowTailOff)
{
    clearCurrentNote();
}

void PulsarSynthVoice::renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    
}
