/*
  ==============================================================================

    OwnedPulsaret.cpp
    Created: 21 Nov 2020 8:18:28pm
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#include "OwnedPulsaret.h"

OwnedPulsaret::OwnedPulsaret(juce::OwnedArray<OwnedPulsaret>& p, PulsaretTable& t) : pulsaretTable(t), pulsaretArray(p)
{
    tableSize = pulsaretTable.getTable().getNumSamples() - 1;
    env.setTableSize(tableSize);
}

OwnedPulsaret::~OwnedPulsaret()
{
    
}

/*=========================================================*/
void OwnedPulsaret::prepare(double sampleRate)
{
    mSampleRate = sampleRate;
}




/*=========================================================*/
void OwnedPulsaret::setFrequency (float frequency)
{
    tableSize = pulsaretTable.getTable().getNumSamples() - 1;
    auto tableSizeOverSampleRate = (float) tableSize / mSampleRate;
    tableDelta = frequency * tableSizeOverSampleRate;
    
}

void OwnedPulsaret::moveToBack(OwnedPulsaret& p)
{
    pulsaretArray.move(pulsaretArray.indexOf(&p), pulsaretArray.size() - 1);  // put a freed pulsaret to back of array
}

float OwnedPulsaret::getNextSample() noexcept
{
    auto index0 = (unsigned int) currentIndex;
    auto index1 = index0 + 1;
    
    auto frac = currentIndex - (float) index0;
    
    auto* table1 = pulsaretTable.getTable().getReadPointer (0);
    auto value0 = table1[index0];
    auto value1 = table1[index1];
    
    auto* table2 = pulsaretTable.getNextTable().getReadPointer (0);
    auto value2 = table2[index0];
    auto value3 = table2[index1];
    
    auto waveSample1 = value0 + (frac * (value1 - value0));
    waveSample1 *= 1.f - pulsaretTable.getWaveFraction(); // pulsaretTable.getWaveFraction() should equal 0.f if no interp should occur
    
    auto waveSample2 = value2 + (frac * (value3 - value2));
    waveSample2 *= pulsaretTable.getWaveFraction();
    
    auto currentSample = waveSample1 + waveSample2;
    
    /*
        reset currentIndex to 0 (restart wavetable).  Don't free the pulsaret until no cycles left
    */
    if ((currentIndex += tableDelta) > (float) tableSize)
    {
        currentIndex = 0;
        cyclesLeft--;
        
        if(cyclesLeft <= 0)
        {
            currentIndex = 0;
            isFree = true;
            cyclesLeft = cycles;
            //DBG(cycles);
        }

        //moveToBack(*this);
    }
    
    return currentSample * env.getNextSample();
}

void OwnedPulsaret::setRunning()
{
    isFree = false;
}

void OwnedPulsaret::resetPhase()
{
    currentIndex = 0;
}
juce::AudioBuffer<float>& OwnedPulsaret::getEnv()
{
    return env.getTable();
}


