/*
  ==============================================================================

    WaveTableVisualizer.cpp
    Created: 26 Jan 2021 5:42:24pm
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#include "WaveTableVisualizer.h"

//==============================================================================
WaveTableVisualizer::WaveTableVisualizer(juce::AudioBuffer<float>& buffer) : bufferToDraw(buffer)
{
    setSize(400, 150);

    
}

WaveTableVisualizer::~WaveTableVisualizer()
{
}

void WaveTableVisualizer::paint (juce::Graphics& g)
{
    
    auto w = getLocalBounds().getWidth();
    auto h = getLocalBounds().getHeight();
    auto frameHalf = h / 2;
    float waveIncrement = (float) w / (float) bufferToDraw.getNumSamples();
    
    juce::Rectangle<int> waveFrame = {0,0, w, h};

    juce::Colour wCol = waveColour.withSaturation(waveSat).withHue(waveHue);
    
    g.setColour(wCol.darker(0.8f).withHue(bgHue).withSaturation(bgSat));
    g.fillRoundedRectangle(waveFrame.toFloat(), 5.f);

    g.setColour(juce::Colours::white);

    juce::Path wavePath;
    wavePath.startNewSubPath(0, frameHalf);
    
    auto* buffRead = bufferToDraw.getReadPointer(0);

    
    for(int i = 0; i <= bufferToDraw.getNumSamples(); ++i)
    {
        auto x = i * waveIncrement;
        auto y = frameHalf - (buffRead[i] * frameHalf * 0.9f); // 0.9 meant to keep the wave from ever touching edge of frame
        
        
        wavePath.lineTo(x, y);
    }
    wavePath.lineTo(w, frameHalf);
    
    PathStrokeType stroke(3.75f, juce::PathStrokeType::curved);
    g.strokePath(wavePath, stroke);
    
    if (isDrawing)
    {
        wavePath.quadraticTo(lastPosition, mousePosition);
        lastPosition.setXY(mousePosition.getX(), mousePosition.getY());
    }
    juce::PathStrokeType stroke2(1.75f, juce::PathStrokeType::curved);
    g.strokePath(wavePath, stroke2);
    
    //setValue();
}

void WaveTableVisualizer::resized()
{
    
}

void WaveTableVisualizer::setValue()
{
    //valueLabel->setText(String(pulsaretTable.getWaveIndex()), dontSendNotification);
}

//void WaveTableVisualizer::timerCallback()
//{
//    repaint();
//}

