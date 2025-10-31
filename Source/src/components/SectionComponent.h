#pragma once

#include <JuceHeader.h>
#include "KnobComponent.h"
#include "../styles/ColorScheme.h"

class SectionComponent : public juce::Component
{
public:
    SectionComponent(const juce::String& sectionTitle, juce::Colour sectionColor);
    ~SectionComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // Add a knob to this section
    KnobComponent* addKnob(const juce::String& paramName, 
                          const juce::String& labelText,
                          juce::RangedAudioParameter* parameter,
                          juce::AudioProcessorValueTreeState& vts);
    
    // Get the section color
    juce::Colour getColor() const { return color; }
    
    // Set the section title
    void setTitle(const juce::String& newTitle);
    
    // Set whether this section is active
    void setActive(bool isActive);

private:
    juce::String title;
    juce::Colour color;
    juce::Label titleLabel;
    bool active = false;
    
    juce::OwnedArray<KnobComponent> knobs;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SectionComponent)
}; 