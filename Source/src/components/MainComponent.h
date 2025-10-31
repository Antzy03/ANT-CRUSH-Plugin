#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"
#include "VisualiserComponent.h"
#include "KnobComponent.h"
#include "../styles/ColorScheme.h"

class MainComponent : public juce::Component
{
public:
    MainComponent(juce::AudioProcessor& processor, juce::AudioProcessorValueTreeState& vts);
    ~MainComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // Update the visualizer with new audio data
    void updateVisualizer(const float* buffer, int size);
    

private:
    juce::AudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;
    
    CustomLookAndFeel customLookAndFeel;
    
    std::unique_ptr<VisualiserComponent> visualiser;

    // Flat knob grid
    juce::OwnedArray<KnobComponent> knobs;
    
    juce::Label titleLabel;
    juce::Label versionLabel;
    std::unique_ptr<juce::Component> titleComponent; // Custom component for title rendering
    
    // No section state in flat layout
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
}; 