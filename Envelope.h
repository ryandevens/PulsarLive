/*
  ==============================================================================

    Envelope.h
    Created: 7 Nov 2020 7:31:31pm
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/*
 This is more than a visualizer.  The visualizer both draws the envelope, and puts its values into the buffer.  (maybe I will make it so it only draws it).
 The Envelope class allows the user to get values from that buffer
 
 The slider and the envelope work in tandem, here are some facts:
 
 The height of the frame is equivalent to the range of the slider
 Envelope and Sliders will return normalized values (0-1)
 The values are converted to the desired range in other classes, as close to the finish line as possible
 
 Fund and Form are converted in the PulsarTrain::generateNextBlock()
 Wavetype is converted in PulsarTrain::generateNextBlock() as well.
 Pan and Amp do not need to be normalized.
 
 Mode 1: Envelope Bypass
    - Values are taken only from the slider value and the envelope is ignored
        - if() statement for this compared with a button in the getNextSample()
 
 Mode 2: Envelope Mod
    - Envelope points will by default be set to the slider level
    - Gray line in background represents slider value
    - Envelope points can be moved to offset slider value
    - The envelope overrides the value of the slider, HOWEVER, the envPoints maintain their position relative to the slider value
      when the slider is changed.
 In this mode the envelope returns the values and the slider simply adjusts the envelope points around it.
 
 */
class Envelope
{
public:
    Envelope();
    Envelope(bool isWavetableEnv);
    ~Envelope();
    
    void createEnvelopeTable();
    juce::AudioBuffer<float>& getTable();
    void setTableSize(int size);
    int getTableSize();
    void setValueAtCurrentIndex(float value);
    void incrementIndex();
    void debugBuffer();
    void trigger();
    
    void prepare(double sampleRate);
    //void prepare(double sampleRate, int samplesPerBlock);
    float getNextSample();
    float getNextBuffer();
    void resetEnvelope();
    void resetIndex();
    
    /*
        Sets total length of envelope in milliseconds
    */
    void setTotalMilliseconds(float ms);
    float getTotalMilliseconds();
    int getTotalLengthInSamples();
    
    float tableIncrement;
    bool isComplete = false;
    void copyEnvelope(Envelope& env);
    
    float getCurrentValue();
    
    void setSliderVal(double val);
    void setEnvVal(float val);
    void setEnvScale(float scale);
    
    bool isEnvelopeForWavetable()
    {
        return isWaveEnv;
    }
    
    void createGaussian();
    void setFrequency(float freq);
    
    void closeEnv(int sampLeft);
    
private:
    bool isWaveEnv = false;
    int currentIndex = 0;
    juce::AudioBuffer<float> envelopeTable;
    float envLengthInSamples = 0;
    
    float currentPhase = 0.f;
    float tableDelta = 0.0f;
    float currentValue = 0.f;
    double mSampleRate;
    const unsigned int tableOrder = 1 << 7;
    int tableSize;
    
    Atomic<double> sliderVal = {0.5f};
    Atomic<float> envVal = {0.5f};
    //Atomic<float> envScale = {0.5f};
    
    float totalMilliseconds;
    

   
    
    
    
    
};
