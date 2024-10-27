/*
  ==============================================================================

    WaveformDisplay.h
    Created: 12 Aug 2024 7:58:22pm
    Author:  pavelosky

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
 */
class WaveformDisplay : public juce::Component, public juce::ChangeListener
{
public:
  /**
   * @brief Constructs a WaveformDisplay object.
   *
   * @param formatManagerToUse The AudioFormatManager object to use for loading audio files.
   * @param cacheToUse The AudioThumbnailCache object to use for caching audio thumbnails.
   */
  WaveformDisplay(AudioFormatManager &formatManagerToUse,
                  AudioThumbnailCache &cacheToUse);

  /**
   * @brief Destructs the WaveformDisplay object.
   */
  ~WaveformDisplay() override;

  /**
   * @brief Paints the waveform display.
   *
   * @param g The Graphics object used for painting.
   */
  void paint(juce::Graphics &g) override;

  /**
   * @brief Called when the component is resized.
   */
  void resized() override;

  /**
   * @brief Callback function called when the audio source changes.
   *
   * @param source The ChangeBroadcaster object that triggered the callback.
   */
  void changeListenerCallback(ChangeBroadcaster *source) override;

  /**
   * @brief Loads an audio file from the given URL.
   *
   * @param audioURL The URL of the audio file to load.
   */
  void loadURL(URL audioURL);

  /**
   * @brief Sets the relative position of the playhead.
   *
   * @param pos The relative position of the playhead, ranging from 0.0 to 1.0.
   */
  void setPositionRelative(double pos);

private:
  AudioThumbnail audioThumbnail; // The audio thumbnail used for displaying the waveform
  bool fileLoaded;               // Flag indicating whether an audio file is loaded
  double position;               // The relative position of the playhead

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
