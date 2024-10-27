/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 12 Aug 2024 7:58:22pm
    Author:  pavelosky

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"


//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager& formatManagerToUse, 
                                 AudioThumbnailCache& cacheToUse) 
                                : audioThumbnail(512, formatManagerToUse, cacheToUse), 
                                  fileLoaded(false),
                                  position(0.0)
{
	audioThumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

// Paint method to draw the waveform display
void WaveformDisplay::paint (juce::Graphics& g)
{
    // Clear the background
    g.fillAll (Colour::fromRGB(13, 27, 42));   

    // Draw an outline around the component
    g.setColour (Colour::fromRGB(119, 141, 169));
    g.drawRect (getLocalBounds(), 1);   

    // Set the colour to light grey
    g.setColour (Colour::fromRGB(224, 225, 221));

    // Check if a file is loaded
    if (fileLoaded) 
    {
        // Set the fill colour to grey with 40% opacity
        g.setColour(Colour::fromRGB(224, 225, 221).withAlpha(0.4f)); 
        // Tint the left side of the waveform display grey
        g.fillRect(0, 0, position * getWidth(), getHeight()); 
        // Set the colour to light blue
        g.setColour(Colour::fromRGB(119, 141, 169));
        // Draw the audio waveform
        audioThumbnail.drawChannels(g, getLocalBounds(), 0.0, audioThumbnail.getTotalLength(), 1.0f);
        // Set the colour to white
        g.setColour(Colours::white);
        // Draw a vertical line at the current position
        g.drawRect(position * getWidth(), 0, 2, getHeight(), 2);
    }
    else
    {
        // Set the font size to 20
        g.setFont (juce::FontOptions (20.0f));
        // Draw placeholder text indicating that no file is loaded
        g.drawText ("File not loaded...", 
                    getLocalBounds(),
                    juce::Justification::centred, true);   
    }
}

void WaveformDisplay::resized()
{

}

// Load an audio file from a URL
void WaveformDisplay::loadURL(URL audioURL)
{
    // Clear the current audio thumbnail
    audioThumbnail.clear();
    // Load the audio file into the thumbnail
    bool fileLoaded = audioThumbnail.setSource(new URLInputSource(audioURL));
}

// Set the playback position relative to the total length
void WaveformDisplay::setPositionRelative(double pos)
{
    // Update the position if it has changed
    position = pos;
    if (pos != position)
    {
        position = pos;
    }
    // Repaint the component to reflect the new position
    repaint();
}

// Callback method for changes in the audio thumbnail
void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
    // Check if the change source is the audio thumbnail
    if (source == &audioThumbnail)
    {
        // Set fileLoaded to true and repaint the component
        fileLoaded = true;
        repaint();
    }
}
