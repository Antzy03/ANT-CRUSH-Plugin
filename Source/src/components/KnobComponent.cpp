#include "KnobComponent.h"

KnobComponent::KnobComponent(const juce::String& paramName, 
                           const juce::String& labelText,
                           juce::RangedAudioParameter* parameter,
                           juce::AudioProcessorValueTreeState& vts)
    : parameterID(paramName), valueTreeState(vts)
{
    // Set up the knob - MUST use normalized 0-1 range for AudioProcessorValueTreeState
    knob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    knob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    knob.setRange(0.0, 1.0, 0.0); // Normalized range - attachment handles conversion
    knob.addListener(this);
    knob.setName(paramName);
    addAndMakeVisible(knob);
    
    // Set the color based on parameter name (before using it)
    color = ColorScheme::getColorForParameter(paramName);
    
    // Set up the name label (monospace for ASCII aesthetic)
    nameLabel.setText(labelText, juce::dontSendNotification);
    nameLabel.setJustificationType(juce::Justification::centred);
    nameLabel.setFont(juce::Font("Consolas", 11.0f, juce::Font::bold));
    nameLabel.setColour(juce::Label::textColourId, color);
    addAndMakeVisible(nameLabel);
    
    // Set up the value label (monospace)
    valueLabel.setJustificationType(juce::Justification::centred);
    valueLabel.setFont(juce::Font("Consolas", 9.0f, juce::Font::plain));
    valueLabel.setColour(juce::Label::textColourId, color.withAlpha(0.8f));
    addAndMakeVisible(valueLabel);
    
    // Create the attachment FIRST, it will set the initial value
    if (parameter != nullptr)
    {
        attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, paramName, knob));
        updateValueLabel();
    }
    
    // No animations in ASCII mode
}

KnobComponent::~KnobComponent()
{
    knob.removeListener(this);
}

void KnobComponent::paint(juce::Graphics& g)
{
    // ASCII mode: no hover/active effects
    juce::ignoreUnused(g);
}

void KnobComponent::resized()
{
    auto bounds = getLocalBounds();
    
    // Position the knob
    knob.setBounds(bounds.removeFromTop(bounds.getHeight() - 30));
    
    // Position the labels
    nameLabel.setBounds(bounds.removeFromTop(15));
    valueLabel.setBounds(bounds);
}

float KnobComponent::getValue() const
{
    return (float)knob.getValue();
}

void KnobComponent::setValue(float newValue)
{
    knob.setValue(newValue);
}

void KnobComponent::setValueFormat(const juce::String& format)
{
    valueFormat = format;
    updateValueLabel();
}

void KnobComponent::showValueLabel(bool shouldShow)
{
    showValue = shouldShow;
    valueLabel.setVisible(showValue);
}

void KnobComponent::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &knob)
    {
        updateValueLabel();
    }
}

void KnobComponent::updateValueLabel()
{
    if (attachment != nullptr)
    {
        // Get the parameter directly from the valueTreeState
        auto* param = dynamic_cast<juce::RangedAudioParameter*>(
            valueTreeState.getParameter(parameterID));
        
        if (param != nullptr)
        {
            valueLabel.setText(param->getCurrentValueAsText(), juce::dontSendNotification);
        }
    }
    else
    {
        valueLabel.setText(juce::String::formatted(valueFormat.toRawUTF8(), knob.getValue()), juce::dontSendNotification);
    }
}

// No hover/press/timer handlers in ASCII mode