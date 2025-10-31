#include "TextureManager.h"

// Singleton instance getter
TextureManager& TextureManager::getInstance()
{
    static TextureManager instance;
    return instance;
}

TextureManager::TextureManager()
{
    loadTextures();
}

TextureManager::~TextureManager()
{
    // Cleanup if needed
}

void TextureManager::loadTextures()
{
    // Create background texture
    backgroundTexture = juce::Image(juce::Image::ARGB, 1, 1, true);
    
    // Create noise overlay texture
    noiseTexture = juce::Image(juce::Image::ARGB, 1, 1, true);
    
    // Create circuit texture
    circuitTexture = juce::Image(juce::Image::ARGB, 1, 1, true);
    
    // Create knob textures for different sections
    distortionKnobTexture = juce::Image(juce::Image::ARGB, 1, 1, true);
    bitCrusherKnobTexture = juce::Image(juce::Image::ARGB, 1, 1, true);
    dynamicsKnobTexture = juce::Image(juce::Image::ARGB, 1, 1, true);
}

// Rest of the implementation... 