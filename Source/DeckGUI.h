/*
  ==============================================================================

  DeckGUI.h
  Created: 12 Aug 2024 10:40:10am
  Author:  pavelosky

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
 * DeckGUI class
 * This class represents the graphical user interface for a DJ deck.
 * It handles user interactions with buttons, sliders, and file drag-and-drop operations.
 */

class DeckGUI : public juce::Component,
        public juce::Button::Listener,
        public juce::Slider::Listener,
        public juce::FileDragAndDropTarget,
        public juce::Timer
{
public:
  // Creates a DeckGUI object.
  // Parameters:
  //   - player: Pointer to the DJAudioPlayer object.
  //   - formatManagerToUse: Reference to the AudioFormatManager object.
  //   - cacheToUse: Reference to the AudioThumbnailCache object.
  DeckGUI(DJAudioPlayer *player,
      AudioFormatManager &formatManagerToUse,
      AudioThumbnailCache &cacheToUse);
  
  // Destroys the DeckGUI object.
  ~DeckGUI() override;

  // Paint method to draw the GUI components
  // Parameters:
  //   - g: Reference to the Graphics object
  void paint(juce::Graphics &g) override;
  
  // Resized method to set the bounds of the GUI components
  void resized() override;

  // Button click handler
  // Parameters:
  //   - button: Pointer to the button that was clicked.
  void buttonClicked(Button *button) override;
  
  // Slider value change handler
  // Parameters:
  //   - slider: Pointer to the slider
  void sliderValueChanged(Slider *slider) override;

  // File drag-and-drop handlers

  // Check if the component is interested in the dragged files
  // Parameters:
  //   - files: Array of sthe paths of the files.
  // Returns:
  //   - true if the component is interested in the dragged files, false otherwise.
  bool isInterestedInFileDrag(const StringArray &files) override;

  // Handle the dropped files
  void filesDropped(const StringArray &files, int x, int y) override;

  // Timer callback to update the GUI
  void timerCallback() override;
  
  // Cue point for the audio player
  float cue;
  
  // Rotation angle for visual elements (e.g., spinning record)
  float rotationAngle;
  
  // Loop start and end points
  float in;
  float out;

  // Loop mode flag
  bool loopMode;

private:
  // Play button
  TextButton playButton{"PLAY"};
  
  // Cue button
  TextButton cueButton{"CUE"};
  
  // Load button
  TextButton loadButton{"LOAD"};
  
  // Loop button
  TextButton loopButton{"LOOP OFF"};
  
  // In-loop button
  TextButton inLoopButton{"IN"};
  
  // Out-loop button
  TextButton outLoopButton{"OUT"};

  // Volume slider
  Slider volSlider;
  
  // Speed slider
  Slider speedSlider;
  
  // Position slider
  Slider positionSlider;

  // High-pass filter knob
  Slider highKnob;
  
  // Low-pass filter knob
  Slider lowKnob;

  // Rectangle to store circle bounds for visual elements
  Rectangle<int> circleBounds;

  // Pointer to the DJ audio player
  DJAudioPlayer *player;

  // File chooser for loading audio files
  juce::FileChooser fChooser{"Select an audio file to play"};

  // Waveform display for visualizing the audio waveform
  WaveformDisplay waveformDisplay;

  // Macro to prevent copying and ensure leak detection
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};