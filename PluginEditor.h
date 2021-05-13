/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MixFeel.h"
#include "NameFeel.h"
#include "PulsaretVisualizer.h"


//==============================================================================
/**
*/
class PulsarAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Button::Listener, juce::Slider::Listener, juce::Timer
{
public:
    PulsarAudioProcessorEditor (PulsarAudioProcessor&);
    ~PulsarAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void buttonClicked(Button* b) override;
    void buttonStateChanged(Button* b) override;
    void sliderValueChanged(Slider* s) override;
    void repaintPulsaret();
    
    void timerCallback() override;
   

private:
    PulsaretVisualizer  pulsaretVisualizer;

    bool flashState = false;
    float alphaSkew = 0.f;
    float low = 0.f;
    float high = 0.f;
    float center = 0.f;
    
    juce::Rectangle<int> frame;
    
    MixFeel mixFeel;
    NameFeel nameFeel;
    std::unique_ptr<juce::TextButton> triggerButton;

    
    std::unique_ptr<juce::Slider>   ampSlider;
    std::unique_ptr<Label>          ampLabel;
    std::unique_ptr<juce::Slider>   ampSpreadSlider;
    std::unique_ptr<juce::Slider>   ampRandSlider;
    std::unique_ptr<juce::Slider>   ampMultiSlider;
    
    std::unique_ptr<juce::Slider>   fundSlider;
    std::unique_ptr<Label>          fundLabel;
    std::unique_ptr<juce::Slider>   fundSpreadSlider;
    std::unique_ptr<juce::Slider>   fundRandSlider;
    std::unique_ptr<juce::Slider>   fundMultiSlider;
    
    std::unique_ptr<juce::Slider>   formFreqSlider;
    std::unique_ptr<Label>          formFreqLabel;
    std::unique_ptr<juce::Slider>   formSpreadSlider;
    std::unique_ptr<juce::Slider>   formRandSlider;
    std::unique_ptr<juce::Slider>   formMultiSlider;
    
    std::unique_ptr<juce::Slider>   formFreqSlider2;
    std::unique_ptr<Label>          formFreqLabel2;
    std::unique_ptr<juce::Slider>   formSpreadSlider2;
    std::unique_ptr<juce::Slider>   formRandSlider2;
    std::unique_ptr<juce::Slider>   formMultiSlider2;
    
    std::unique_ptr<juce::Slider>   waveSlider;
    std::unique_ptr<Label>          waveLabel;
    std::unique_ptr<juce::Slider>   waveSpreadSlider;
    std::unique_ptr<juce::Slider>   waveRandSlider;
    std::unique_ptr<juce::Slider>   waveMultiSlider;
    
    std::unique_ptr<Slider>         panSlider;
    std::unique_ptr<Label>          panLabel;
    std::unique_ptr<juce::Slider>   panSpreadSlider;
    std::unique_ptr<juce::Slider>   panRandSlider;
    std::unique_ptr<juce::Slider>   panMultiSlider;
    
    std::unique_ptr<Slider>         interSlider;
    std::unique_ptr<Label>          interLabel;
    std::unique_ptr<Slider>         triggerOnSlider;
    std::unique_ptr<Slider>         triggerOffSlider;
    std::unique_ptr<Label>          onLabel;
    std::unique_ptr<Label>          offLabel;
    
    std::unique_ptr<Slider>         widthSlider;
    std::unique_ptr<Label>          widthLabel;
    
    std::unique_ptr<Slider>         attackSlider;
    std::unique_ptr<Slider>         decaySlider;
    std::unique_ptr<Slider>         sustainSlider;
    std::unique_ptr<Slider>         releaseSlider;
    
    std::unique_ptr<Label>          attackLabel;
    std::unique_ptr<Label>          decayLabel;
    std::unique_ptr<Label>          sustainLabel;
    std::unique_ptr<Label>          releaseLabel;
    
    
    std::unique_ptr<Slider>         glideSlider;
    std::unique_ptr<Label>          glideLabel;
    
    std::unique_ptr<Label>          nameLabel;
    
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    std::unique_ptr<Attachment>     fundAttachment;
    std::unique_ptr<Attachment>     fundSpreadAttachment;
    std::unique_ptr<Attachment>     fundRandAttachment;
    
    std::unique_ptr<Attachment>     formFreqAttachment;
    std::unique_ptr<Attachment>     formSpreadAttachment;
    std::unique_ptr<Attachment>     formRandAttachment;
    
    std::unique_ptr<Attachment>     formFreqAttachment2;
    std::unique_ptr<Attachment>     formSpreadAttachment2;
    std::unique_ptr<Attachment>     formRandAttachment2;
    
    std::unique_ptr<Attachment>     waveAttachment;
    std::unique_ptr<Attachment>     waveSpreadAttachment;
    std::unique_ptr<Attachment>     waveRandAttachment;
    
    std::unique_ptr<Attachment>     panAttachment;
    std::unique_ptr<Attachment>     panSpreadAttachment;
    std::unique_ptr<Attachment>     panRandAttachment;
    
    std::unique_ptr<Attachment>     ampAttachment;
    std::unique_ptr<Attachment>     ampSpreadAttachment;
    std::unique_ptr<Attachment>     ampRandAttachment;
    
    std::unique_ptr<Attachment>     interAttachment;
    std::unique_ptr<Attachment>     triggerOnAttachment;
    std::unique_ptr<Attachment>     triggerOffAttachment;
    
    std::unique_ptr<Attachment>     widthAttachment;
    std::unique_ptr<Attachment>     attackAttachment;
    std::unique_ptr<Attachment>     decayAttachment;
    std::unique_ptr<Attachment>     sustainAttachment;
    std::unique_ptr<Attachment>     releaseAttachment;
    
    std::unique_ptr<Attachment>     glideAttachment;
    
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment>     triggerAttachment;
   
    PulsarAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PulsarAudioProcessorEditor)
};
