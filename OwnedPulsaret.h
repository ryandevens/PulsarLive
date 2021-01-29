/*
  ==============================================================================

    OwnedPulsaret.h
    Created: 21 Nov 2020 8:18:28pm
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#pragma once

#include "Pulsaret.h"
#include "Envelope.h"



/*
    A sub-class of Pulsaret for the pulsarets in the array owned by the pulsar train.
*/
class OwnedPulsaret : public Pulsaret
{
public:
    OwnedPulsaret(juce::OwnedArray<OwnedPulsaret>& p, PulsaretTable& t);
    ~OwnedPulsaret();
    
    void prepare(double mSampleRate);
    void setTable(int selection);
    void setFrequency (float frequency);

    void moveToBack(OwnedPulsaret& p);
    void setRunning() override;
    bool isFree = true;
    float getNextSample() noexcept;
    
    juce::AudioBuffer<float>& getBuffer();
    
    void resetPhase() override;
    
    PulsaretTable& pulsaretTable;
    
    
    void setCycles(int cyc)
    {
        cycles = jmax(cyc, 1);
        cyclesLeft = cycles;
    }
    
    juce::AudioBuffer<float>& getEnv();
private:
    double mSampleRate = 0;
    Envelope env;
    int tableSize;
    float currentIndex = 0.0f;
    float tableDelta = 0.0f;
    int cycles = 1;
    int cyclesLeft = 1;
    
    // a reference to the array that holds this OwnedPulsaret
    juce::OwnedArray<OwnedPulsaret>& pulsaretArray;
};
