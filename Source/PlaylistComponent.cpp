/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 13 Aug 2024 7:33:49pm
    Author:  pavelosky

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <Windows.h>
#include <filesystem>

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    // Iterate through the music folder and populate the track list
    iterateMusicFolder("C:/Users/pawel/Music");

    // Add columns to the table header
    tableComponent.getHeader().addColumn("Title", 1, 300);
    tableComponent.getHeader().addColumn("Size", 2, 100);
    // tableComponent.getHeader().addColumn("Artist", 3, 200);
    // tableComponent.getHeader().addColumn("Play", 4, 50);

    // Set the table model to this component
    tableComponent.setModel(this);

    // Make the table component visible
    addAndMakeVisible(tableComponent);
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint(Graphics &g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    // Clear the background
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

    // Draw an outline around the component
    g.setColour(Colour::fromRGB(119, 141, 169));
    g.drawRect(getLocalBounds(), 1);

    // Draw some placeholder text
    g.setColour(Colours::white);
    g.setFont(FontOptions(14.0f));
    g.drawText("PlaylistComponent", getLocalBounds(),
               Justification::centred, true);
}

void PlaylistComponent::resized()
{
    // Set the bounds of the table component to fill the entire area
    tableComponent.setBounds(0, 0, getWidth(), getHeight());
}

int PlaylistComponent::getNumRows()
{
    // Return the number of rows in the table, which is the size of the track list
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(Graphics &g, int rowNumber, int width, int height, bool rowIsSelected)
{
    // Set the background color based on whether the row is selected
    if (rowIsSelected)
    {
        g.fillAll(Colour::fromRGB(119, 141, 169));
    }
    else
    {
        g.fillAll(Colour::fromRGB(224, 225, 221));
    }
}

void PlaylistComponent::paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    // Set the text color and font
    g.setColour(juce::Colours::black);
    g.setFont(14.0f);

    // Check if the row number is within the bounds of the track list
    if (rowNumber < trackTitles.size())
    {
        // Draw the appropriate text based on the column ID
        switch (columnId)
        {
        case 1:
            g.drawText(trackTitles[rowNumber][0], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
            break;
        case 2:
            g.drawText(trackTitles[rowNumber][1], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
            break;
        case 3:
            g.drawText(trackTitles[rowNumber][2], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
            break;
        default:
            break;
        }
    }
}

Component *PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                      int columnId,
                                                      bool isRowSelected,
                                                      Component *existingComponentToUpdate)
{
    // Check if the column ID is for the "Play" button
    if (columnId == 4)
    {
        // If there is no existing component, create a new play button
        if (existingComponentToUpdate == nullptr)
        {
            TextButton *playButton = new TextButton("Play");
            String id{std::to_string(rowNumber)};
            playButton->setComponentID(id);

            existingComponentToUpdate = playButton;
            playButton->addListener(this);
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button *button)
{
    // Get the row number from the button's component ID
    int id = std::stoi(button->getComponentID().toStdString());
    std::cout << "Button clicked" << std::endl;
}

/**
 * Iterates through the specified folder and adds each file to the track list.
 * !!! This function was AI generated. !!!
 *
 * @param folderPath The path of the music folder to iterate through.
 */
void PlaylistComponent::iterateMusicFolder(const std::string folderPath)
{
    try
    {
        // Check if the specified path exists and is a directory
        if (std::filesystem::exists(folderPath) && std::filesystem::is_directory(folderPath))
        {
            // Iterate through the directory and add each file to the track list
            for (const auto &entry : std::filesystem::directory_iterator(folderPath))
            {
                const auto &filePath = entry.path();

                // Add the file name, size, and a placeholder artist to the track list
                trackTitles.push_back({entry.path().filename().string(), std::to_string(std::filesystem::file_size(filePath) / 10000), "Artist 1"});
            }
        }
        else
        {
            std::cerr << "The specified path is not a valid directory." << std::endl;
        }
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
}