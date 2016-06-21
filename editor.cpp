#pragma once
#include "editor.h"
#include "strings.h"

Editor::Editor()
{
    // Determine the username
    QString username = qgetenv("USER");
    if (username.isEmpty())
        username = qgetenv("USERNAME");

    // The location of the save file is different on Windows and Mac
    this->playerDataLocation = Strings::playerDataPrefix + username.toStdString() + Strings::playerDataSuffix;

    // Open playerDataStream
    this->playerDataStream.open(this->playerDataLocation);
    if (playerDataStream.fail()) std::abort(); // TODO: Implement more graceful exception handling.
}

Editor::~Editor()
{
    this->playerDataStream.close();
}

void Editor::restart()
{
    // Return playerDataStream to the beginning of PlayerData.txt
    this->playerDataStream.clear();
    this->playerDataStream.seekg(0, std::ios::beg);
}

template<typename T> T Editor::loadValue(std::string specifier)
{
    T value;
    std::string word;

    // Search the file up to the specifier
    while (this->playerDataStream >> word)
        if (word == specifier) break;

    // Find the value
    while(this->playerDataStream >> word && word != Strings::terminator)
    {
        if (word != Strings::separator && word != Strings::intSpecifier && word != Strings::stringSpecifier)
            value = word;
    }

    this->restart(); // Return the stream to the beginning of the file

    return value;
}

void Editor::setValue(std::string specifier) {}

QString* Editor::loadCharacterNames()
{
    /* This function searches playerData.txt for all character names and adds seperators to
     * the "Load Player" section in the menu bar of w. Type can either be System.String or
     * System.Int32. */
    std::string characterName;
    std::string specifier;
    QString* characterNames = new QString[this->MAX_CHARACTERS];
    int i;

    /* In PlayerData.txt, character names are specified by their number, ranging from
     * 0 to MAX_CHARACTERS, followed by "name" (e.g. "0name : Smurfalicious"). */
    for (i = 0; i < this->MAX_CHARACTERS; i++)
    {
        // Load the name and store it in characterNames
        specifier = std::to_string(i) + Strings::nameSpecifier;
        characterName = loadValue<std::string>(specifier);
        characterNames[i] = QString::fromStdString(characterName);
    }

    this->numCharacters = i; // Update numCharacters to store the number of characters loaded

    return characterNames;
}
