/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 9 Aug 2024 7:31:05pm
    Author:  pavelosky

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager &_formatManager) : formatManager(_formatManager)
{
    // Constructor for DJAudioPlayer class.
}

DJAudioPlayer::~DJAudioPlayer()
{
    // Destructor for DJAudioPlayer class.
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // Prepare the audio player for playback.
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    basefilterSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    lowpassSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resamplingSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    // Get the next audio block to be played.
    resamplingSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    // Release the resources used by the audio player.
    // transportSource.releaseResources();
    basefilterSource.releaseResources();
    lowpassSource.releaseResources();
    resamplingSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL)
{
    // Load audio from a given URL.
    std::cout << "Loading URL: " << audioURL.toString(true) << std::endl;

    auto *reader = formatManager.createReaderFor(audioURL.createInputStream(false));

    if (reader != nullptr)
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}

void DJAudioPlayer::start()
{
    // Start playback of the audio.
    transportSource.start();
}

void DJAudioPlayer::stop()
{
    // Stop playback of the audio.
    transportSource.stop();
}

void DJAudioPlayer::setGain(float gain)
{
    // Set the gain (volume) of the audio.
    if (gain < 0 || gain > 1)
    {
        std::cout << "DJAudioPlayer::Invalid gain value: " << gain << "Gain should be between 0 and 1" << std::endl;
    }
    else
    {
        transportSource.setGain(gain);
    }
}

void DJAudioPlayer::setSpeed(float ratio)
{
    // Set the speed (resampling ratio) of the audio.
    if (ratio < 0 || ratio > 100.0)
    {
        std::cout << "DJAudioPlayer::Invalid speed value: " << ratio << "Speed should be between 0 and 100.0" << std::endl;
    }
    else
    {
        resamplingSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setPosition(float posInSecs)
{
    // Set the position (in seconds) of the audio playback.
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    // Set the position (relative) of the audio playback.
    if (pos < 0 && pos > 1.0)
    {
        std::cout << "DJAudioPlayer::Invalid position value: " << pos << "Position should be between 0 and 1.0" << std::endl;
    }
    else
    {
        double posInSecs = pos * transportSource.getLengthInSeconds();
        setPosition(posInSecs);
    }
}

double DJAudioPlayer::getPositionRelative() const
{
    // Get the current position (relative) of the audio playback.
    if (transportSource.getLengthInSeconds() == 0.0)
    {
        return 0.0;
    }
    else
    {
        return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
    }
}

// found on the forum: forum.juce.com/t/bass-treble-mid-equaliser/52245/7

// This function sets the low-pass filter for the audio player.
// A low-pass filter allows frequencies below a certain cutoff frequency to pass through,
// while attenuating frequencies above the cutoff frequency.
void DJAudioPlayer::setLowPass(double frequency)
{
    auto *reader = readerSource->getAudioFormatReader();
    IIRCoefficients coeffs = IIRCoefficients::makeLowPass(reader->sampleRate, frequency);
    lowpassSource.setCoefficients(coeffs);
}

// Similar to the low-pass filter, this function sets the high-pass filter for the audio player.
void DJAudioPlayer::setHighPass(double frequency)
{
    auto *reader = readerSource->getAudioFormatReader();
    IIRCoefficients coeffs = IIRCoefficients::makeHighPass(reader->sampleRate, frequency);
    basefilterSource.setCoefficients(coeffs);
}