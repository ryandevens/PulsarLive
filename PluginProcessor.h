/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PulsarTrain.h"
#include "PulsaretTable.h"

//==============================================================================
/**
*/
class PulsarAudioProcessor  : public juce::AudioProcessor, public ValueTree::Listener
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
    
    void update();
    
    void handleMidi(juce::MidiBuffer midiBuffer);
    
    AudioProcessorValueTreeState e;
    AudioProcessorValueTreeState::ParameterLayout createParams();
    void valueTreePropertyChanged(ValueTree& tree, const Identifier& property) override;
    
    void triggerPulsarTrain();
    void releasePulsarTrain();
    
    PulsaretTable& getPulsaretTable();
    
    /*
        These three are used to communicate between the train and the editor
    */
    bool isTrainRunning();
    bool isFlashing(); // checks if train should flash for a "hit" or not flash for a "miss" using trigger patterns / stochasticism
    float getFlashCoef(); // scale flash by amp 
    
//    void fillImpulseBuffer()
//    {
//        impulseBuffer.setSize (1, 128);
//        impulseBuffer.clear();
//
//        auto* buffWrite = impulseBuffer.getWritePointer (0);
//
//        auto angleDelta = juce::MathConstants<double>::twoPi / (double) (128);
//        auto pi = juce::MathConstants<double>::pi;
//        double currentAngle = -pi;
//
//        for (unsigned int i = 0; i < 128; ++i)
//        {
//            float sample;
//
//            if (currentAngle == 0)
//            {
//                sample = 1;
//            }
//            else
//            {
//                sample = std::sin (pi * currentAngle) / (pi * currentAngle);
//                buffWrite[i] += sample;
//
//            }
//
//            currentAngle += angleDelta;
//        }
//
//        buffWrite[128] = buffWrite[0];
//    }
    void fillImpulseBuffer()
    {
        impulseBuffer.setSize (1, 128);
        impulseBuffer.clear();
        auto* buffWrite = impulseBuffer.getWritePointer (0);
        int harmonics = 32;
        for (auto i = 0; i < harmonics; i++)
        {
            float harmonic = i + 1;
            auto angleDelta = juce::MathConstants<double>::twoPi / (double) (128) * harmonic;
            auto currentAngle = 0.0;
            float harmonicAmp = 0.9f / (harmonic * harmonic);
            for (unsigned int i = 0; i < 128; ++i)
            {
                auto sample = std::sin (currentAngle);
                buffWrite[i] += (float) sample * harmonicAmp; //harmonicWeights[harmonic];
                currentAngle += angleDelta;
            }
        }
        buffWrite[128] = buffWrite[0];
    }
 
private:
    PulsarTrain pulsarTrain;
    bool trainRunning = false;
    bool mustUpdateProcessing { false };
    
    juce::dsp::Convolution              convolver;
    juce::AudioBuffer<float>            impulseBuffer; // triangle for now 4/6
//    juce::dsp::Convolution::Latency     latency;
    

    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PulsarAudioProcessor)
};
