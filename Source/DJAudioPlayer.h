/*
	==============================================================================

	DJAudioPlayer.h
	Created: 9 Aug 2024 7:31:05pm
	Author:  pavelosky

	==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public AudioSource
{
public:
	/**
		Constructor.
		@param formatManager The AudioFormatManager object used for loading audio files.
	*/
	DJAudioPlayer(AudioFormatManager &formatManager);

	/**
		Destructor.
	*/
	~DJAudioPlayer();

	/**
		Prepares the audio player for playback.
		@param samplesPerBlockExpected The number of samples per block expected.
		@param sampleRate The sample rate of the audio.
	*/
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

	/**
		Fills the audio buffer with the next block of audio data.
		@param bufferToFill The buffer to be filled with audio data.
	*/
	void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;

	/**
		Releases any resources used by the audio player.
	*/
	void releaseResources() override;

	/**
		Loads an audio file from the specified URL.
		@param audioURL The URL of the audio file to be loaded.
	*/
	void loadURL(URL audioURL);

	/**
		Starts playback of the audio.
	*/
	void start();

	/**
		Stops playback of the audio.
	*/
	void stop();

	/**
		Sets the gain (volume) of the audio.
		@param gain The gain value, ranging from 0.0 to 1.0.
	*/
	void setGain(float gain);

	/**
		Sets the speed (ratio) of the audio playback.
		@param ratio The speed ratio, where 1.0 is normal speed.
	*/
	void setSpeed(float ratio);

	/**
		Sets the position of the playhead in seconds.
		@param posInSecs The position in seconds.
	*/
	void setPosition(float posInSecs);

	/**
		Sets the position of the playhead relative to the current position.
		@param pos The relative position, where 0.0 is the current position.
	*/
	void setPositionRelative(double pos);

	/**
		Sets the cutoff frequency of the low-pass filter.
		@param frequency The cutoff frequency in Hz.
	*/
	void setLowPass(double frequency);

	/**
		Sets the cutoff frequency of the high-pass filter.
		@param frequency The cutoff frequency in Hz.
	*/
	void setHighPass(double frequency);

	/**
		Returns the relative position of the playhead.
		@return The relative position, where 0.0 is the beginning and 1.0 is the end.
	*/
	double getPositionRelative() const;

private:
	AudioFormatManager &formatManager;					   // Reference to the AudioFormatManager object
	std::unique_ptr<AudioFormatReaderSource> readerSource; // Unique pointer to the AudioFormatReaderSource object
	AudioTransportSource transportSource;				   // AudioTransportSource object for playback control

	IIRFilterAudioSource basefilterSource{&transportSource, false}; // IIRFilterAudioSource object for base filtering
	IIRFilterAudioSource lowpassSource{&basefilterSource, false};	// IIRFilterAudioSource object for low-pass filtering

	ResamplingAudioSource resamplingSource{&lowpassSource, false, 2}; // ResamplingAudioSource object for resampling
};