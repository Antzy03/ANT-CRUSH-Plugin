#pragma once

#include <JuceHeader.h>
#include "../styles/ColorScheme.h"

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel();
    
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, 
                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle, 
                          juce::Slider& slider) override;
                          
    juce::Font getLabelFont(juce::Label& label) override;
    
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                         float sliderPos, float minSliderPos, float maxSliderPos,
                         const juce::Slider::SliderStyle style, juce::Slider& slider) override;
    
    // Add method for drawing backgrounds
    void drawBackground(juce::Graphics& g, juce::Rectangle<int> bounds);
    
    // ASCII border drawing (box-drawing characters)
    void drawASCIIBorder(juce::Graphics& g, juce::Rectangle<int> bounds, juce::Colour borderColor);
    
    // CRT glow effect
    void drawCRTGlow(juce::Graphics& g, juce::Rectangle<int> bounds);
}; 