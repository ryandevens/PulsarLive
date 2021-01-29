/*
  ==============================================================================

    WaveTableVisualizer.h
    Created: 26 Jan 2021 5:42:23pm
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/*
    This can draw any buffer you give it in a 0 - 1.0 manner, scaled to fit its size
*/
class WaveTableVisualizer : public juce::Component //, juce::Timer
{
public:
    WaveTableVisualizer(juce::AudioBuffer<float>& buffer);
    
    ~WaveTableVisualizer() override;
    
    void paint (juce::Graphics& g) override;
    void resized() override;
    void setValue();
    
    void setBuffer(juce::AudioBuffer<float>& buffer)
    {
        bufferToDraw.makeCopyOf(buffer);
    }
private:
    juce::AudioBuffer<float>& bufferToDraw;
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

    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveTableVisualizer)
};
