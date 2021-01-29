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
#include <pedal/CurvedEnvelope.hpp>


/*
    This will be the train of pulsars.  It will take arguments for fundamental frequency.
    This will read from the
*/
class PulsarTrain
{
public:
    PulsarTrain(juce::OwnedArray<OwnedPulsaret>& array, juce::OwnedArray<OwnedPulsaret>& array2,
                Atomic<float>& fund, juce::Range<int>& fuRange, Atomic<float>& fundRand, Atomic<bool>& fuIsSpread,
                Atomic<float>& form, juce::Range<int>& fRange, Atomic<float>& formRand, Atomic<bool>& fIsSpread,
                Atomic<float>& form2, juce::Range<int>& fRange2, Atomic<float>& formRand2, Atomic<bool>& fIsSpread2,
                Atomic<float>& pL, Atomic<float>& pR, juce::Range<int>& pRange, Atomic<float>& panRand, Atomic<bool>& pIsSpread,
                Atomic<float>& amp, juce::Range<int>& aRange, Atomic<float>& ampRand, Atomic<bool>& ampIsSpread,
                Atomic<float>& inter, Atomic<float>& wType, juce::Range<int>& wRange, Atomic<float>& wRand, Atomic<bool>& wIsSpread);
    
//    PulsarTrain(Envelope& ampEnv, Envelope& fEnv, Envelope& foEnv, Envelope& pEnv, Envelope& wEnv, juce::OwnedArray<OwnedPulsaret>& array);
    ~PulsarTrain();
    
    void prepare(double sampleRate);
    void startTrain();
    void scheduleNextPulsar(); // wait time in between pulsars == period of pulsar == currentTime + pulsarPeriodinSamples
    void triggerPulsaret();
    void generateNextBlock(juce::AudioBuffer<float>& buffer);
    
    void readPulsaret();
    
    void setFrequencies(float fundFreq, float formFreq, float formFreq2);
    void setFundamental(float fundFreq);
    void setFormant(float formFreq);
    
    int  getPeriod();
    void checkStatus();
    bool needsCalculation = false;
    
    void updateParams(Atomic<float>& fund, Atomic<float>& fundSpread, Atomic<float>& fundRand,
                      Atomic<float>& form, Atomic<float>& formSpread, Atomic<float>& formRand,
                      Atomic<float>& form2, Atomic<float>& formSpread2, Atomic<float>& formRand2,
                      Atomic<float>& pan, Atomic<float>& panSpread, Atomic<float>& panRand,
                      Atomic<float>& amp, Atomic<float>& ampSpread, Atomic<float>& ampRand,
                      Atomic<float>& intermittance);
    
    void updateFundamental();
    void updateFormant();
    
    
    void updateRanges();
    
    float getFlashState();
    void flipFlashState();
    void setTrigger(int on, int off);
    void setPulsaretParamsAndTrigger();
    void setGlideTime(float glideTime);
    void setHarmonicFormants(float fund, float form1, float form2);
    bool getHarmonicModeStatus();
    float getFormFreq1();
    float getFormFreq2();
    void setPulsaretWidth(float width);
    void triggerEnv();
    void triggerRelease();
    void setEnv(float atk, float dec, float sus, float rel);

    
private:
    pedal::CurvedEnvelope   env;
    
    
    int pulsaretCycles1 = 1;
    int pulsaretCycles2 = 1;
    
    float pulsaretWidth = 0.1f;
    
    bool inHarmonicMode = false;
    
    int triggerOn = 1;
    int onCount = 1;
    int triggerOff = 0;
    int offCount = 0;
    bool isTriggerPattern = false;
    
    juce::Random rand;
    Atomic<float> flashState { 0.f };
    Atomic<bool> isFlashing { false };
    
    juce::OwnedArray<OwnedPulsaret>& pulsaretArray;
    juce::OwnedArray<OwnedPulsaret>& pulsaretArray2;
    
    double mSampleRate;

    int pulsarPeriod;
    int pulsarPhase;
    
    
    int samplesRemainingInPeriod = 0;
    int samplesRemainingInEnvelope = 0;
    
    Atomic<float>& mAmp;
    //Atomic<float>& mAmpSpread;
    Atomic<float>& mAmpRand;
    Atomic<bool>& ampIsSpread;
    Atomic<float>& intermittance;
    juce::Range<int>& ampRange;
    
    Atomic<float>& mFundFreq;
    //Atomic<float>& mFundSpread;
    Atomic<float>& mFundRand;
    Atomic<bool>& fundIsSpread;
    juce::Range<int>& fundRange;
    
    Atomic<float>& mFormFreq;
    //Atomic<float>& mFormSpread;
    Atomic<float>& mFormRand;
    Atomic<bool>& formIsSpread;
    juce::Range<int>&   formRange;
    
    
    Atomic<float>& mFormFreq2;
    //Atomic<float>& mFormSpread2;
    Atomic<float>& mFormRand2;
    Atomic<bool>& formIsSpread2;
    juce::Range<int>&   formRange2;
    
    Atomic<float>& panL;
    Atomic<float>& panR;
    //Atomic<float>& panSpread;
    Atomic<float>& panRand;
    Atomic<bool>& panIsSpread;
    juce::Range<int>& panRange;
    
    
    Atomic<float>& waveType;
    //Atomic<float>& waveSpread;
    Atomic<float>& waveRand;
    Atomic<bool>& waveIsSpread;
    juce::Range<int>& waveRange;
    
    
    juce::SmoothedValue<float, ValueSmoothingTypes::Multiplicative> smoothFund;
    juce::SmoothedValue<float, ValueSmoothingTypes::Multiplicative> smoothForm;
    juce::SmoothedValue<float, ValueSmoothingTypes::Multiplicative> smoothForm2;
    juce::SmoothedValue<float, ValueSmoothingTypes::Multiplicative> smoothAmp;
    
};
