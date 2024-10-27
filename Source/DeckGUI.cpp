/*
  ==============================================================================

    DeckGUI.cpp
    Created: 12 Aug 2024 10:40:10am
    Author:  pavelosky

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================

DeckGUI::DeckGUI(DJAudioPlayer *_player,
                 AudioFormatManager &formatManagerToUse,
                 AudioThumbnailCache &cacheToUse)
    : player(_player),
      waveformDisplay(formatManagerToUse, cacheToUse),
      cue(0.0),
      rotationAngle(0.0),
      loopMode(false),
      in(0.0),
      out(0.99)
{
    // Control buttons
    addAndMakeVisible(playButton);
    addAndMakeVisible(cueButton);
    addAndMakeVisible(loadButton);

    // Loop buttons
    addAndMakeVisible(loopButton);
    addAndMakeVisible(inLoopButton);
    addAndMakeVisible(outLoopButton);

    // Sliders
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(positionSlider);

    // High and Low pass filter knobs
    addAndMakeVisible(highKnob);
    addAndMakeVisible(lowKnob);

    // Waveform display
    addAndMakeVisible(waveformDisplay);

    volSlider.setRange(0.0, 1.0);
    volSlider.setValue(1.0);
    speedSlider.setRange(0.5, 2.0);
    speedSlider.setValue(1.0);
    positionSlider.setRange(0.0, 1.0);

    highKnob.setRange(5.0, 5000.0);
    // highKnob.setValue(5.0);
    lowKnob.setRange(500.0, 10000.0);
    // lowKnob.setValue(1000.0);

    // Set the slider styles for the high and low pass filter knobs
    highKnob.setSliderStyle(Slider::Rotary);
    lowKnob.setSliderStyle(Slider::Rotary);

    // Set the text box styles for the high and low pass filter knobs
    highKnob.setTextBoxStyle(Slider::TextBoxAbove, false, 50, 20);
    lowKnob.setTextBoxStyle(Slider::TextBoxAbove, false, 50, 20);

    // Set the button color for the play button to red
    playButton.setColour(TextButton::buttonColourId, juce::Colour::fromRGB(90, 183, 92));
    inLoopButton.setColour(TextButton::buttonColourId, juce::Colour::fromRGB(13, 27, 42));
    outLoopButton.setColour(TextButton::buttonColourId, juce::Colour::fromRGB(13, 27, 42));
    loopButton.setColour(TextButton::buttonColourId, juce::Colour::fromRGB(13, 27, 42));
    loadButton.setColour(TextButton::buttonColourId, juce::Colour::fromRGB(13, 27, 42));

    // Set the slider styles for the volume and speed sliders
    volSlider.setSliderStyle(Slider::LinearVertical);
    speedSlider.setSliderStyle(Slider::LinearVertical);

    // Set the text box styles for the volume and speed sliders
    volSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    speedSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);

    // Set the text box style for the position slider
    positionSlider.setTextBoxStyle(Slider::TextBoxAbove, false, 50, 20);

    // Add listeners to the buttons and sliders
    playButton.addListener(this);
    cueButton.addListener(this);
    loadButton.addListener(this);

    loopButton.addListener(this);
    inLoopButton.addListener(this);
    outLoopButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    positionSlider.addListener(this);

    highKnob.addListener(this);
    lowKnob.addListener(this);

    // Start the timer with a callback interval of 50ms
    startTimer(50);
}

DeckGUI::~DeckGUI()
{
    // Stop the timer and remove listeners
    stopTimer();
    playButton.removeListener(this);
    cueButton.removeListener(this);
    loadButton.removeListener(this);
    volSlider.removeListener(this);
    speedSlider.removeListener(this);
    positionSlider.removeListener(this);
    loopButton.removeListener(this);
    inLoopButton.removeListener(this);
    outLoopButton.removeListener(this);
    highKnob.removeListener(this);
    lowKnob.removeListener(this);
}

void DeckGUI::paint(juce::Graphics &g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll(juce::Colour::fromRGB(65, 90, 119)); // clear the background

    // g.fillAll(Colour::fromRGB(27, 38, 59));
    g.setColour(juce::Colour::fromRGB(27, 38, 59));
    g.drawRect(getLocalBounds(), 1); // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(14.0f));
    g.drawText("", getLocalBounds(),
               juce::Justification::centred, true); // draw some placeholder text

    // Draw the spinning record in the specified square
    g.setColour(juce::Colour::fromRGB(13, 27, 42));
    float circleRadius = std::min(circleBounds.getWidth(), circleBounds.getHeight()) / 2.0f;
    g.fillEllipse(circleBounds.toFloat());

    // Calculate the line's endpoint after rotation
    Point<float> center = circleBounds.getCentre().toFloat();
    float lineLength = circleRadius; // Length of the line (equal to the circle radius)
    float x = center.x + lineLength * std::cos(rotationAngle);
    float y = center.y + lineLength * std::sin(rotationAngle);

    // Draw the white line rotating around the center of the circle
    g.setColour(juce::Colours::white);
    g.drawLine(center.x, center.y, x, y, 2.0f); // Draw the line with a thickness of 2.0f
}

// I've decided to change the layout of the app and make it imitate the layout of the legendary Technics SL-1200MK2 with some modern additions
// like the waveform display and the high, and low pass filters and a cue button that is used in a modern CDJ players.
// To achieve this I divided the layout into a 4x4 grid like below, where each sqare will contain the component of the app or add visual

//       C0     C1     C2     C3     C4     C5
//     +---- + ---- + ---- + ---- + ---- + ---- +
//  R0 |                waveform                |
//     +---- + ---- + ---- + ---- + ---- + ---- +
//  R1 |               PosSlider                |
//     +---- + ---- + ---- + ---- + ---- + ---- +
//  R2 |                   |  Hp  |   V  |   S  |
//     +                   + ---- +   |  +   |  +
//  R3 |    Spinning       |      |   |  |   |  |
//     +     Record        + ---- +   |  +   |  +
//  R4 |                   |  Lp  |   |  |   |  |
//     +---- + ---- + ---- + ---- + ---- + ---- +
//  R5 |   PC    |      Loop dash     |   L     |
//     +---- + ---- + ---- + ---- + ---- + ---- +

void DeckGUI::resized()
{
    // The app is divided into 6x6 grid
    double col = getWidth() / 6.0;  // 6 rows
    double row = getHeight() / 6.0; // 6 columns

    // Bottom button controls
    playButton.setBounds(0, row * 5, col * 1.5, row / 2);           // C0 R5 play button
    cueButton.setBounds(0, row * 5.5, col * 1.5, row / 2);          // C0 R5 for the cue button
    loadButton.setBounds(col * 4.5, row * 5, col * 1.5, row);       // C3 R5 for the load button
    inLoopButton.setBounds(col * 1.5, row * 5, col * 1.5, row / 2); // C0 R5 for the in loop button
    outLoopButton.setBounds(col * 3, row * 5, col * 1.5, row / 2);  // C2 R5 for the out loop button
    loopButton.setBounds(col * 1.5, row * 5.5, col * 3, row / 2);   // C0 R5 for the loop button

    volSlider.setBounds(col * 4, row * 2, col, row * 3);   // C4 R2 for the volume slider
    speedSlider.setBounds(col * 5, row * 2, col, row * 3); // C5 R2 for the speed slider
    positionSlider.setBounds(0, row, col * 6, row);        // C0 R1 for the position slider
    waveformDisplay.setBounds(0, 0, col * 6, row);         // C0 R0 for the waveform display

    highKnob.setBounds(col * 3, row * 2, col, row); // C4 R0 for the high pass filter knob
    lowKnob.setBounds(col * 3, row * 4, col, row);  // C5 R0 for the low pass filter knob

    // Define the bounds for the circle in column 1, row 2
    circleBounds = juce::Rectangle<int>((int)col * 0.5, (int)row * 2.5, (int)col * 2, (int)row * 2);
}

// This method is called when a button is clicked
void DeckGUI::buttonClicked(Button *button)
{
    if (button == &playButton)
    {
        // Log play button click
        std::cout << "Play button clicked" << std::endl;

        // Check if the play button text is "PLAY"
        if (playButton.getButtonText() == "PLAY")
        {
            // Change button text to "PAUSE" and start the player
            playButton.setButtonText("PAUSE");
            player->start();
            // Change button color to red
            playButton.setColour(TextButton::buttonColourId, juce::Colour::fromRGB(218, 79, 74));
        }
        else
        {
            // Change button text to "PLAY" and stop the player
            playButton.setButtonText("PLAY");
            player->stop();
            // Change button color to green
            playButton.setColour(TextButton::buttonColourId, juce::Colour::fromRGB(90, 183, 92));
        }
    }
    else if (button == &cueButton)
    {
        // Check if the audio is paused
        if (playButton.getButtonText() == "PLAY")
        {
            // Set cue point to the current player position
            cue = player->getPositionRelative();
            cueButton.setColour(TextButton::buttonColourId, juce::Colour::fromRGB(250, 166, 50));
        }
        else
        {
            // Set player position to the cue point and start the player
            player->setPositionRelative(cue);
            player->start();
        }
    }
    else if (button == &loopButton)
    {
        // Check if the loop is off
        if (loopButton.getButtonText() == "LOOP OFF")
        {
            // Change button text to "LOOP ON" and enable loop mode
            loopButton.setButtonText("LOOP ON");
            loopButton.setColour(TextButton::buttonColourId, juce::Colour::fromRGB(1, 110, 205));
            loopMode = true;

            // Start the player from the loop start position if valid
            if (in < out)
            {
                player->start();
                // Check if the audio is paused
                if (playButton.getButtonText() == "PLAY")
                {
                    // Change button text to "PAUSE" and start the player
                    playButton.setButtonText("PAUSE");
                    // Change button color to red
                    playButton.setColour(TextButton::buttonColourId, juce::Colour::fromRGB(218, 79, 74));
                }
            }
            else
            {
                // Reset loop points if invalid
                in = 0.0;
                out = 0.99;
            }
        }
        else
        {
            // Change button text to "LOOP OFF" and disable loop mode
            loopButton.setButtonText("LOOP OFF");
            loopMode = false;
            loopButton.setColour(TextButton::buttonColourId, juce::Colour::fromRGB(13, 27, 42));
        }
    }
    else if (button == &inLoopButton)
    {
        // Set loop start point to the current player position
        in = player->getPositionRelative();
        inLoopButton.setColour(TextButton::buttonColourId, juce::Colour::fromRGB(119, 141, 169));

        // Validate loop points to prevent in being greater than out
        if (in > out)
        {
            out = 0.99;
        }
    }
    else if (button == &outLoopButton)
    {
        // Set loop end point to the current player position
        out = player->getPositionRelative();
        outLoopButton.setColour(TextButton::buttonColourId, juce::Colour::fromRGB(119, 141, 169));

        // Validate loop points to prevent out being less than in
        if (out < in)
        {
            in = 0.0;
        }
    }
    else if (button == &loadButton)
    {
        // Launch file chooser to select a file
        auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser &chooser)
                             {
            // Load the selected file into the player and waveform display
            player->loadURL(URL{ chooser.getResult() });
            waveformDisplay.loadURL(URL{ chooser.getResult() }); });
    }
    else
    {
        // Log button click
        std::cout << "Button clicked" << std::endl;
    }
}

void DeckGUI::sliderValueChanged(Slider *slider)
{

    if (slider == &volSlider)
    {
        // Set the player's gain to the slider's value
        player->setGain(slider->getValue());
    }

    else if (slider == &speedSlider)
    {
        // Set the player's speed to the slider's value
        player->setSpeed(slider->getValue());
    }

    else if (slider == &positionSlider)
    {
        // Set the player's position to the slider's value
        player->setPositionRelative(slider->getValue());
    }

    else if (slider == &highKnob)
    {
        // Set the player's high pass filter to the slider's value
        player->setHighPass(slider->getValue());
    }

    else if (slider == &lowKnob)
    {
        // Set the player's low pass filter to the slider's value
        player->setLowPass(slider->getValue());
    }
    // Handle any other sliders
    else
    {
        // Log that a slider value has changed
        std::cout << "Slider value changed" << std::endl;
    }
}

/**
 * Determines if the DeckGUI is interested in a file drag operation.
 *
 * @param files The list of files being dragged.
 * @return True if the DeckGUI is interested in the file drag, false otherwise.
 */
bool DeckGUI::isInterestedInFileDrag(const StringArray &files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}

// This method is called when the user drops a file onto the DeckGUI
void DeckGUI::filesDropped(const StringArray &files, int x, int y)
{
    player->loadURL(URL{File{files[0]}});
    waveformDisplay.loadURL(URL{File{files[0]}});
}

// This method is called every 50ms

/**
 * @brief Timer callback function that updates the GUI and player state.
 *
 * This function is called periodically to perform the following tasks:
 * - Update the rotation angle of a visual element (e.g., a spinning record) if the music is playing.
 * - Handle loop mode by resetting the player's position to the loop start point when it reaches the loop end point.
 * - Update the position slider and waveform display to reflect the current playback position.
 * - Repaint the GUI to reflect these updates.
 */
void DeckGUI::timerCallback()
{
    // Update the rotation angle if the music is playing
    if (playButton.getButtonText() == "PAUSE")
    {
        rotationAngle += 0.05f * speedSlider.getValue(); // Adjust the speed of rotation here
    }

    // Handle loop mode
    if (loopMode)
    {
        if (player->getPositionRelative() >= out)
        {
            player->setPositionRelative(in);
        }
    }

    // Update position slider and waveform display
    positionSlider.setValue(player->getPositionRelative());
    waveformDisplay.setPositionRelative(player->getPositionRelative());

    // Repaint the component
    repaint();
}
