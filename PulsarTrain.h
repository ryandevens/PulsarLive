/*
  ==============================================================================

    PulsarTrain.h
    Created: 5 Nov 2020 11:24:59am
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "OwnedPulsaret.h"




/*
    This will be the train of pulsars.  It will take arguments for fundamental frequency.
    This will read from the
*/
class PulsarTrain
{
public:
    PulsarTrain();

    ~PulsarTrain();
    
    void prepare(double sampleRate);
    void startTrain();
  
    void triggerPulsaret();
    void generateNextBlock(juce::AudioBuffer<float>& buffer);
    
    
    void setFrequencies(float fundFreq, float formFreq, float formFreq2);

    
    int  getPeriod();
    void checkStatus();
    bool needsCalculation = false;
    
    void updateParams(float f, float fSpread, float fRand,
                      float fo, float foSpread, float foRand,
                      float fo2, float foSpread2, float foRand2,
                      float p, float pSpread, float pRand,
                      float a, float aSpread, float aRand,  
                      float intermittance, float atk, float dec, float sus, float rel, float wid,
                      float glide, int trigOn, int trigOff, bool trig, float w, float wSpread, float wRand);
    
    void updateFundamental();

    
    void calculateRanges();
    
    void setTrigger(int on, int off);
    void setPulsaretParamsAndTrigger();
    void setGlideTime(float glideTime);

    float getFormFreq1();
    float getFormFreq2();
    void setPulsaretWidth(float width);
    void triggerEnv();
    void triggerRelease();
    void setEnv();
    
    void triggerPulsaretWithNoAmp();

    PulsaretTable& getPulsaretTable();

    bool trainIsRunning()
    {
        return isRunning;
    }

    bool checkIfFlashing()
    {
        return isFlashing.get();
    }

    float getFlashCoef()
    {
        return flashCoef;
    }

private:
    int tableSize = 1024;
    OwnedPulsaret pulsaret1;
    OwnedPulsaret pulsaret2;
    
    juce::ADSR env;
    juce::ADSR::Parameters envParam;

    bool isContinuous = false;
    bool isSingleCycle = true;
    int  flag = 0;
    float attack;
    float decay;
    float sustain;
    float release;
    
    float pulsaretSamples1 = 1.f;
    float pulsaretSamples2 = 1.f;
    
    float pulsaretWidth = 0.1f;
    
    float flashCoef = 0.f;
    bool isRunning = false;
    
    
    int triggerOn = 1;

    int onCount = 1;
    int triggerOff = 0;

    int offCount = 0;
    bool isTriggerPattern = true;
    bool trigger;
    
    juce::Random rand;

    Atomic<bool> isFlashing { false };
    
    double mSampleRate;

    int pulsarPeriod = 1024;
    int pulsarPhase;
    
    
    int samplesRemainingInPeriod = 0;
    int samplesRemainingInEnvelope = 0;
    
    Atomic<float> mAmp               { 0.0f };
    Atomic<float> mAmpSpread         { 0.0f };
    Atomic<float> mAmpRand           { 0.05f };
    Atomic<bool> ampIsSpread         { false };
    Atomic<float> intermittance      { 1.0f };
    juce::Range<int> ampRange;
    
    Atomic<float> mFundFreq          { 40.0f };
    Atomic<float> mFundSpread        { 0.0f };
    Atomic<float> mFundRand          { 0.05 };
    Atomic<bool> fundIsSpread        { false };
    juce::Range<int> fundRange;
    
    Atomic<float> mFormFreq          { 200.0f };
    Atomic<float> mFormSpread        { 0.0f };
    Atomic<float> mFormRand          { 0.05 };
    Atomic<bool> formIsSpread        { false };
    juce::Range<int>   formRange;
    
    
    Atomic<float> mFormFreq2         { 200.0f };
    Atomic<float> mFormSpread2       { 0.f };
    Atomic<float> mFormRand2         { 0.05 };
    Atomic<bool> formIsSpread2       { false };
    juce::Range<int>   formRange2;
    
    Atomic<float> panL               { 1.f };
    Atomic<float> panR               { 0.f };
    Atomic<float> panSpread          { 0.f };
    Atomic<float> panRand            { 0.f };
    Atomic<bool> panIsSpread         { false };
    juce::Range<int> panRange;
    
    Atomic<float> waveType           { 0.f };
    Atomic<float> waveSpread         { 0.f };
    Atomic<float> waveRand           { 0.f };
    Atomic<bool> waveIsSpread        { false };
    juce::Range<int> waveRange;
    
    Atomic<float> width {0.1f};
    
    Atomic<float> glideTime { 100.f };

    
    
    juce::SmoothedValue<float, ValueSmoothingTypes::Multiplicative> smoothFund;
    juce::SmoothedValue<float, ValueSmoothingTypes::Multiplicative> smoothForm;
    juce::SmoothedValue<float, ValueSmoothingTypes::Multiplicative> smoothForm2;
    juce::SmoothedValue<float, ValueSmoothingTypes::Multiplicative> smoothAmp;
    
};
