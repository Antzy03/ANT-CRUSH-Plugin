#include "MainComponent.h"
#include "../../PluginProcessor.h"

// Simple subtle title component
class TitleComponent : public juce::Component
{
public:
    void paint(juce::Graphics& g) override
    {
        auto titleBounds = getLocalBounds();
        
        // Subtle semi-transparent background
        g.setColour(ColorScheme::backgroundDark.withAlpha(0.7f));
        g.fillRect(titleBounds);
        
        // Simple centered text - no borders or outlines
        const juce::String titleText = "<ANT//CRUSH>";
        g.setFont(juce::Font("Consolas", 18.0f, juce::Font::plain)); // Smaller, plain font
        g.setColour(ColorScheme::neonCyan.withAlpha(0.8f)); // Subtle color
        g.drawText(titleText, titleBounds, juce::Justification::centred);
    }
};

MainComponent::MainComponent(juce::AudioProcessor& processor, juce::AudioProcessorValueTreeState& vts)
    : audioProcessor(processor), valueTreeState(vts)
{
    // Set look and feel
    setLookAndFeel(&customLookAndFeel);
    
    // Create visualizer as animated background
    visualiser.reset(new VisualiserComponent());
    visualiser->setInterceptsMouseClicks(false, false);
    addAndMakeVisible(*visualiser);
    visualiser->toBack();
    
    // Flat knob grid: create knobs for available parameters (order defined here)
    auto addKnob = [&](const juce::String& id, const juce::String& label)
    {
        if (auto* p = dynamic_cast<juce::RangedAudioParameter*>(valueTreeState.getParameter(id)))
        {
            auto* k = new KnobComponent(id, label, p, valueTreeState);
            knobs.add(k);
            addAndMakeVisible(k);
        }
    };

    addKnob("drive",        "DRIVE");
    addKnob("saturation",   "SAT");
    addKnob("midside",      "MID/SIDE");
    addKnob("mix",          "MIX");
    addKnob("bitcrush",     "BIT");
    addKnob("bitmodulation","MOD");
    addKnob("downsample",   "DOWN");
    addKnob("jitter",       "JITTER");
    addKnob("threshold",    "THRESH");
    addKnob("attack",       "ATTACK");
    addKnob("release",      "RELEASE");
    addKnob("spectralshift","SPECTR");
    
    // Create custom title component that draws on top
    titleComponent = std::make_unique<TitleComponent>();
    titleComponent->setInterceptsMouseClicks(false, false);
    titleComponent->setOpaque(true); // Opaque so it covers what's behind
    addAndMakeVisible(*titleComponent);
    titleComponent->toFront(false); // Bring title to front
    
    // Version label removed - only title shown
}

MainComponent::~MainComponent()
{
    setLookAndFeel(nullptr);
}

void MainComponent::paint(juce::Graphics& g)
{
    // No border or divider lines - clean minimal look
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();
    
    // No border padding - use full width
    
    // Position header - smaller, simpler
    auto headerBounds = bounds.removeFromTop(35); // Smaller header
    titleLabel.setBounds(headerBounds);
    
    // Position title component - minimal
    if (titleComponent != nullptr)
    {
        titleComponent->setBounds(headerBounds);
        titleComponent->toFront(false);
    }
    
    // Visualizer: full width and height as background
    visualiser->setBounds(getLocalBounds());
    visualiser->toBack();
    
    // Knob area - minimal padding, maximize space
    bounds.removeFromTop(2);
    auto knobArea = bounds.reduced(4); // Reduced padding for less side space

        // Clean organized layout: group by function
        if (knobs.size() > 0)
        {
            juce::Array<KnobComponent*> driveKnobs;
            juce::Array<KnobComponent*> distortionKnobs;
            juce::Array<KnobComponent*> bitcrushKnobs;
            juce::Array<KnobComponent*> dynamicsKnobs;

        for (auto* k : knobs)
        {
            auto id = k->getParameterID();
            if (id == "drive" || id == "mix")
                driveKnobs.add(k);
            else if (id == "saturation" || id == "midside")
                distortionKnobs.add(k);
            else if (id == "bitcrush" || id == "bitmodulation" || id == "downsample" || id == "jitter")
                bitcrushKnobs.add(k);
            else
                dynamicsKnobs.add(k);
        }

        const int gap = 8;
        const int knobHeight = 75;
        const int knobWidth = 70;
        int yPos = knobArea.getY();

        // Row 1: Drive & Mix (primary controls)
        if (driveKnobs.size() > 0)
        {
            int totalWidth = (driveKnobs.size() * knobWidth) + ((driveKnobs.size() - 1) * gap);
            int xStart = knobArea.getX() + (knobArea.getWidth() - totalWidth) / 2;
            int x = xStart;
            for (auto* k : driveKnobs)
            {
                k->setBounds(x, yPos, knobWidth, knobHeight);
                x += knobWidth + gap;
            }
            yPos += knobHeight + gap;
        }

        // Row 2: Distortion (saturation, midside)
        if (distortionKnobs.size() > 0)
        {
            int totalWidth = (distortionKnobs.size() * knobWidth) + ((distortionKnobs.size() - 1) * gap);
            int xStart = knobArea.getX() + (knobArea.getWidth() - totalWidth) / 2;
            int x = xStart;
            for (auto* k : distortionKnobs)
            {
                k->setBounds(x, yPos, knobWidth, knobHeight);
                x += knobWidth + gap;
            }
            yPos += knobHeight + gap;
        }

        // Row 3: Bit Crusher (4 knobs)
        if (bitcrushKnobs.size() > 0)
        {
            int totalWidth = (bitcrushKnobs.size() * knobWidth) + ((bitcrushKnobs.size() - 1) * gap);
            int xStart = knobArea.getX() + (knobArea.getWidth() - totalWidth) / 2;
            int x = xStart;
            for (auto* k : bitcrushKnobs)
            {
                k->setBounds(x, yPos, knobWidth, knobHeight);
                x += knobWidth + gap;
            }
            yPos += knobHeight + gap;
        }

        // Row 4: Dynamics (remaining)
        if (dynamicsKnobs.size() > 0)
        {
            int totalWidth = (dynamicsKnobs.size() * knobWidth) + ((dynamicsKnobs.size() - 1) * gap);
            int xStart = knobArea.getX() + (knobArea.getWidth() - totalWidth) / 2;
            int x = xStart;
            for (auto* k : dynamicsKnobs)
            {
                k->setBounds(x, yPos, knobWidth, knobHeight);
                x += knobWidth + gap;
            }
        }
        
    }
}


void MainComponent::updateVisualizer(const float* buffer, int size)
{
    if (visualiser != nullptr && buffer != nullptr && size > 0)
    {
        visualiser->setAudioBuffer(buffer, size);
    }
}