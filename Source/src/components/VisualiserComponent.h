#pragma once

#include <JuceHeader.h>
#include "../styles/ColorScheme.h"

class VisualiserComponent : public juce::Component,
                           private juce::Timer
{
public:
    VisualiserComponent();
    ~VisualiserComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // Set the audio buffer to visualize
    void setAudioBuffer(const float* buffer, int size);
    
    // Set the active section color
    void setActiveColor(juce::Colour newColor);
    
    // Set the intensity of the visualization (0.0 to 1.0)
    void setIntensity(float newIntensity);

private:
    void timerCallback() override;
    void updateVisualisation();
    
    juce::AudioBuffer<float> audioBuffer;
    juce::Colour activeColor = juce::Colours::white; // Monochrome
    float intensity = 0.85f; // Higher intensity for more pop
    
    // Visualization data
    std::vector<float> visualData;
    std::vector<float> targetData;
    std::vector<float> waveformPoints;
    
    // Animation parameters (faster response for vibration effect)
    float smoothingFactor = 0.5f; // Increased for more reactivity
    juce::Random random;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VisualiserComponent)
}; 