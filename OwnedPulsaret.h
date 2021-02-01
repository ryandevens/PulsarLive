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
    
    
    void setLengthInSamples(float numSamples, float pulsarPeriod);
    void isSingleCycle(bool is);
    void setContinuous(bool test);
    void setAsHit();
    void setAsMiss();
    
    void setWaveType(float wave)
    {
        auto normalized = wave / 100.f;
        pulsaretTable.setTable(normalized);
    }
    
    juce::AudioBuffer<float>& getEnv();
private:
    float freq;
    bool trigger = true;
    bool singleCycle = true;
    bool continuous = false;
    
    double mSampleRate = 0;
    Envelope env;
    int tableSize;
    
    int pulsarPeriodInSamples = 100;
    float currentIndex = 0.0f;
    float tableDelta = 0.0f;
    float cycleSamples = 1;
    float cycleSamplesLeft = 1;
    int cycles = 1;
    int numCycles = 1;
    
    
    
    // a reference to the array that holds this OwnedPulsaret
    juce::OwnedArray<OwnedPulsaret>& pulsaretArray;
};
