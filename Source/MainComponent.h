/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public AudioAppComponent
/**
 * @class MainComponent
 * @brief The main component of the application.
 *
 * This class represents the main component of the application. It is responsible for managing the audio playback, GUI components, and playlist.
 */
{
public:
  /**
   * @brief Constructs a MainComponent object.
   */
  MainComponent();

  /**
   * @brief Destructs the MainComponent object.
   */
  ~MainComponent();

  /**
   * @brief Prepares the audio playback.
   *
   * This function is called before the audio playback starts. It initializes the audio format manager and thumbnail cache.
   *
   * @param samplesPerBlockExpected The number of samples per audio block expected.
   * @param sampleRate The sample rate of the audio.
   */
  void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

  /**
   * @brief Retrieves the next audio block.
   *
   * This function is called to retrieve the next audio block to be played. It fills the bufferToFill with audio data.
   *
   * @param bufferToFill The buffer to be filled with audio data.
   */
  void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;

  /**
   * @brief Releases the audio resources.
   *
   * This function is called to release the audio resources when the audio playback stops.
   */
  void releaseResources() override;

  /**
   * @brief Paints the GUI component.
   *
   * @param g The Graphics object used for painting.
   */
  void paint(Graphics &g) override;

  /**
   * @brief Resizes the GUI component.
   */
  void resized() override;

private:
  // Your private member variables go here...

  AudioFormatManager formatManager;        /**< The audio format manager. */
  AudioThumbnailCache thumbnailCache{100}; /**< The audio thumbnail cache. */

  DJAudioPlayer player1{formatManager};                      /**< The audio player for deck 1. */
  DeckGUI deckGUI1{&player1, formatManager, thumbnailCache}; /**< The GUI component for deck 1. */

  DJAudioPlayer player2{formatManager};                      /**< The audio player for deck 2. */
  DeckGUI deckGUI2{&player2, formatManager, thumbnailCache}; /**< The GUI component for deck 2. */

  MixerAudioSource mixerSource;        /**< The mixer audio source. */
  PlaylistComponent playlistComponent; /**< The playlist component. */

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent) /**< Macro to declare the class as non-copyable with leak detector. */
};