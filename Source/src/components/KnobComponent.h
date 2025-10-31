#pragma once

#include <JuceHeader.h>
#include "../styles/ColorScheme.h"

class KnobComponent : public juce::Component,
                     private juce::Slider::Listener
{
public:
    KnobComponent(const juce::String& paramName, 
                 const juce::String& labelText,
                 juce::RangedAudioParameter* parameter,
                 juce::AudioProcessorValueTreeState& vts);
    ~KnobComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // Get the current value
    float getValue() const;
    
    // Set the value (normalized 0-1)
    void setValue(float newValue);
    
    // Set the display value format (e.g., "%.1f dB", "%.2f ms")
    void setValueFormat(const juce::String& format);
    
    // Set whether to show value label
    void showValueLabel(bool shouldShow);
    
    // Get the underlying slider
    juce::Slider* getSlider() { return &knob; }
    const juce::String& getParameterID() const { return parameterID; }
    
    // No mouse hover/press overrides in ASCII mode

private:
    void sliderValueChanged(juce::Slider* slider) override;
    void updateValueLabel();
    
    juce::Slider knob;
    juce::Label nameLabel;
    juce::Label valueLabel;
    
    juce::String valueFormat = "%.2f";
    bool showValue = true;
    
    juce::Colour color;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
    
    // Store parameter ID and valueTreeState reference
    juce::String parameterID;
    juce::AudioProcessorValueTreeState& valueTreeState;
    
    // No animations in ASCII mode
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KnobComponent)
}; 