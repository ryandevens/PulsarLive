/*
  ==============================================================================

    PulsarSynthVoice.h
    Created: 29 Jan 2021 9:35:35am
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PulsarTrain.h"

class PulsarSynthVoice : public MPESynthesiserVoice
{
public:
    PulsarSynthVoice();
    ~PulsarSynthVoice();

    void noteStarted() override;
    void notePitchbendChanged() override;
    void noteStopped (bool) override;

    void notePressureChanged() override {}
    void noteTimbreChanged()   override {}
    void noteKeyStateChanged() override {}
    
    void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    
private:

};
