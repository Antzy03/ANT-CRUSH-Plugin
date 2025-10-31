#pragma once

#include <JuceHeader.h>

class TextureManager
{
public:
    // Singleton pattern
    static TextureManager& getInstance();
    
    // Get textures
    juce::Image getBackgroundTexture() const { return backgroundTexture; }
    juce::Image getNoiseTexture() const { return noiseTexture; }
    juce::Image getCircuitTexture() const { return circuitTexture; }
    juce::Image getDistortionKnobTexture() const { return distortionKnobTexture; }
    juce::Image getBitCrusherKnobTexture() const { return bitCrusherKnobTexture; }
    juce::Image getDynamicsKnobTexture() const { return dynamicsKnobTexture; }
    
private:
    // Private constructor for singleton
    TextureManager();
    ~TextureManager();
    
    // Prevent copying
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    
    // Load textures
    void loadTextures();
    
    // Texture storage
    juce::Image backgroundTexture;
    juce::Image noiseTexture;
    juce::Image circuitTexture;
    juce::Image distortionKnobTexture;
    juce::Image bitCrusherKnobTexture;
    juce::Image dynamicsKnobTexture;
}; 