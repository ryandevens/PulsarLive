/*
  ==============================================================================

    PulsaretVisualizer.h
    Created: 6 Nov 2020 4:34:21pm
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PulsaretTable.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class PulsaretVisualizer : public juce::Component //, juce::Timer
{
public:
    PulsaretVisualizer(PulsarAudioProcessor& p);
    
    ~PulsaretVisualizer() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
    void setValue();
    
//    void timerCallback() override
//    {
//
//        repaint();
//    }
    
    void setNewWaveColour(juce::Colour newColour)
    {
        waveColour = newColour;
    }
    void setNewBackgroundColour(juce::Colour newColour)
    {
        bgColour = newColour;
    }
    void setWaveSat(float sat)
    {
        waveSat = sat;
    }
    void setBgSat(float sat)
    {
        bgSat = sat;
    }
    
    void setWaveHue(float h)
    {
        waveHue = h;
    }
    void setBgHue(float h)
    {
        bgHue = h;
    }
    
    
    void setWaveVal(float v)
    {
        waveVal = v;
    }
    void setBgVal(float v)
    {
        bgVal = v;
    }
    
    void setAmp(float a)
    {
        amp = a;
        amp = 1;
    }
private:
    PulsaretTable& pulsaretTable;
    bool isDrawing = false;
    
    std::unique_ptr<Label> valueLabel;
    juce::String value = "value";
    
    juce::Point<float> mousePosition;
    juce::Point<float> lastPosition;

    juce::Path wavePath;
    
    float waveValue = 0.f;
    
    juce::Colour    waveColour;
    juce::Colour    bgColour;
    float waveSat;
    float waveHue;
    float waveVal;
    float bgSat;
    float bgHue;
    float bgVal;
    float amp = 1.f;
  
    PulsarAudioProcessor& audioProcessor;
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PulsaretVisualizer)
};
