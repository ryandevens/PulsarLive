/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PulsarTrain.h"
#include "OwnedPulsaret.h"
//#include "PulsarSynthesiser.h"

//==============================================================================
/**
*/
class PulsarAudioProcessor  : public juce::AudioProcessor,
public ValueTree::Listener
{
public:
    //==============================================================================
    PulsarAudioProcessor();
    ~PulsarAudioProcessor() override;

    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    void setTrainRunning();
    void setTrainStopped();
    void resetEnvelopes();
    
    AudioProcessorValueTreeState e;
    AudioProcessorValueTreeState::ParameterLayout createParams();
    void update();
    void updateFund();
    void updateForm();
    void updateForm2();
    void updateWave();
    void updateAmp();
    void updatePan();

    
    void setFlash(bool state)
    {
        triggerFlash = state;
    }
    float getFlashState()
    {
        return pulsarTrain.getFlashState();
    }


    
    //void update();
    void updateFormant(float center, float low, float high);
    


    // prototypical pulsaret which I will make an array of to be considered an "ownedPulsaret"
    
    PulsaretTable& getPulsaretTable();
    PulsarTrain& getPulsarTrain()
    {
        return pulsarTrain;
    }
    
    juce::AudioBuffer<float>& getEnvBuffer();
    
    void setRanges();
    
    void handleMidi(juce::MidiBuffer midiBuffer);
    
private:
//    PulsarSynthesiser pulsarSynth;
//    MidiMessageCollector messageCollector;
    
    bool trainRunning = false;
    
    
    PulsaretTable pulsaretTable;
    PulsarTrain pulsarTrain;
    //PulsarTrainVisualizer trainVisualizer;
    
    // remember this is just one type/trajectory of pulsarets, meant to accomodate overlap
    juce::OwnedArray<OwnedPulsaret> pulsaretArray;
    juce::OwnedArray<OwnedPulsaret> pulsaretArray2;

    Atomic<bool>  trainIsRunning    {false};
    Atomic<bool> triggerFlash {false};
    
    Atomic<float> waveType           { 0.f };
    Atomic<float> waveSpread         { 0.f };
    Atomic<float> waveRand           { 0.f };
    Atomic<bool> waveIsSpread        { false };
    juce::Range<int> waveRange;
    
    bool mustUpdateProcessing { false };
    bool mustUpdateFund { false };
    bool mustUpdateFormant { false };
    bool mustUpdateFormant2 { false };
    bool mustUpdateWave { false };
    bool mustUpdateAmp { false };
    bool mustUpdatePan { false };
    
    void valueTreePropertyChanged(ValueTree& tree, const Identifier& property) override;

    
    
        

    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PulsarAudioProcessor)
};
