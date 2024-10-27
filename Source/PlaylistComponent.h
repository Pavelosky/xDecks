class PlaylistComponent : public juce::Component,
                          public juce::TableListBoxModel,
                          public juce::Button::Listener
{
public:
    PlaylistComponent(); // Constructor for the PlaylistComponent class

    ~PlaylistComponent() override; // Destructor for the PlaylistComponent class

    void paint(juce::Graphics &) override; // Function to paint the component
    void resized() override;               // Function to handle resizing of the component
    int getNumRows() override;             // Function to get the number of rows in the table

    // Function to paint the background of a row in the table
    void paintRowBackground(juce::Graphics &g,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected) override;

    // Function to paint the content of a cell in the table
    void paintCell(juce::Graphics &g,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected) override;

    // Function to refresh the component for a specific cell in the table
    Component *refreshComponentForCell(int rowNumber,
                                       int columnId,
                                       bool isRowSelected,
                                       Component *existingComponentToUpdate) override;

    // Function to iterate through the music folder and populate the table with track titles
    void iterateMusicFolder(const std::string folderPath);

    // Function to handle button click events
    void buttonClicked(Button *button) override;

private:
    TableListBox tableComponent;                  // Table component to display the playlist
    std::vector<std::vector<String>> trackTitles; // Vector to store track titles

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
    // Macro to declare the class as non-copyable and enable leak detection
};