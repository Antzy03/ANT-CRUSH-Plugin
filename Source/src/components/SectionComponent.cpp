#include "SectionComponent.h"
#include "TextureManager.h"
#include "../styles/ColorScheme.h"

SectionComponent::SectionComponent(const juce::String& sectionTitle, juce::Colour sectionColor)
    : title(sectionTitle), color(sectionColor)
{
    // Set up the title label
    titleLabel.setText(title, juce::dontSendNotification);
    titleLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::left);
    titleLabel.setColour(juce::Label::textColourId, ColorScheme::textColor);
    addAndMakeVisible(titleLabel);
}

SectionComponent::~SectionComponent()
{
    knobs.clear();
}

void SectionComponent::paint(juce::Graphics& g)
{
    // Draw background (monochrome)
    g.setColour(ColorScheme::backgroundLight);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 6.0f);
    
    // Draw border with section color
    g.setColour(juce::Colours::white.withAlpha(active ? 0.7f : 0.25f));
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(0.5f), 6.0f, 1.0f);
    
    // Draw color indicator next to title
    juce::Rectangle<float> indicator(10.0f, 10.0f);
    indicator.setCentre(15.0f, titleLabel.getBounds().getCentreY());
    g.setColour(color);
    g.fillEllipse(indicator);
}

void SectionComponent::resized()
{
    auto bounds = getLocalBounds().reduced(8);
    
    // Position the title label
    titleLabel.setBounds(bounds.removeFromTop(20).withTrimmedLeft(20));
    
    // Position the knobs
    if (knobs.size() > 0)
    {
        const int availableWidth = bounds.getWidth();
        const int numKnobs = knobs.size();
        const int minKnobWidth = 70;
        const int maxKnobWidth = 130;
        int knobWidth = juce::jlimit(minKnobWidth, maxKnobWidth, availableWidth / numKnobs);
        const int gap = numKnobs > 1 ? 6 : 0;
        const int knobHeight = bounds.getHeight();

        int x = bounds.getX();
        for (int i = 0; i < numKnobs; ++i)
        {
            knobs[i]->setBounds(x, bounds.getY(), knobWidth, knobHeight);
            x += knobWidth + gap;
        }
    }
}

KnobComponent* SectionComponent::addKnob(const juce::String& paramName, 
                                       const juce::String& labelText,
                                       juce::RangedAudioParameter* parameter,
                                       juce::AudioProcessorValueTreeState& vts)
{
    if (parameter == nullptr)
        return nullptr;

    auto* knob = new KnobComponent(paramName, labelText, parameter, vts);
    knobs.add(knob);
    addAndMakeVisible(knob);
    resized();
    return knob;
}

void SectionComponent::setTitle(const juce::String& newTitle)
{
    title = newTitle;
    titleLabel.setText(title, juce::dontSendNotification);
}

void SectionComponent::setActive(bool isActive)
{
    active = isActive;
    repaint();
} 