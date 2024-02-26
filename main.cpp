#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include <limits>

using namespace std;

// Function to print messages with borders
void PrintMessage(string message, bool printTop = true, bool printBottom = true)
{
    if (printTop)
    {
        cout << "+---------------------------------+" << endl;
        cout << "|";
    }
    else
    {
        cout << "|";
    }
    bool front = true;
    for (int i = message.length(); i < 33; i++)
    {
        if (front)
        {
            message = " " + message;
        }
        else
        {
            message = message + " ";
        }
        front = !front;
    }
    cout << message.c_str();

    if (printBottom)
    {
        cout << "|" << endl;
        cout << "+---------------------------------+" << endl;
    }
    else
    {
        cout << "|" << endl;
    }
}

// Function to draw the hangman based on the number of guesses
void DrawHangman(int guessCount = 0)
{
    if (guessCount >= 1)
        PrintMessage("|", false, false);
    else
        PrintMessage("", false, false);

    if (guessCount >= 2)
        PrintMessage("|", false, false);
    else
        PrintMessage("", false, false);
    if (guessCount >= 3)
        PrintMessage("O", false, false);
    else
        PrintMessage("", false, false);

    if (guessCount == 4)
        PrintMessage("/  ", false, false);

    if (guessCount == 5)
        PrintMessage("/| ", false, false);

    if (guessCount >= 6)
        PrintMessage("/|\\", false, false);
    else
        PrintMessage("", false, false);

    if (guessCount >= 7)
        PrintMessage("|", false, false);
    else
        PrintMessage("", false, false);

    if (guessCount == 8)
        PrintMessage("/", false, false);

    if (guessCount >= 9)
        PrintMessage("/ \\", false, false);
    else
        PrintMessage(" ", false, false);
}

// Function to print the word and check if the user has won
bool PrintWordAndCheckWin(string word, string guessed)
{
    bool won = true;//. This variable will be used to track whether the player has won the game or not.
    string s;
    for (int i = 0; i < word.length(); i++)
    {

        if (guessed.find(word[i]) == string::npos)//it checks if the character is present in the guessed string using the find
                                                    // If the character is not found (i.e., find returns string::npos)
        {
            won = false;//sets won to false if character is not present
            s += "_ ";
        }
        else
        {
            s += word[i];
            s += " ";//if character is found in guessed string it appends followed by space to string
        }
    }
    PrintMessage(s, false);
    return won;//finally returns the value of won
}


// Function to load a random word from a file
string LoadRandomWord(string path)
{
    int lineCount = 0;
    string word;
    vector<string> v;
    ifstream reader(path);
    if (reader.is_open())
    {
        while (std::getline(reader, word))
            v.push_back(word);

        int randomLine = rand() % v.size();

        word = v.at(randomLine);
        reader.close();
    }
    cout << "Word to be guessed generated!!!" << endl;
    return word;
}

// Function to count the number of tries left based on guessed letters
int TriesLeft(string word, string guessed)
{
    int error = 0;//this variable is used to count ythe number of incorrect guesses
    for (int i = 0; i < guessed.length(); i++)
    {
        if (word.find(guessed[i]) == string::npos)
            error++;//if the character is  not present error will be increased
    }
    return error;
}

// Function to use reward points to reveal a letter in the word
bool UseRewardPoint(string &word, string &guessed)
{
    const int costPerLetter = 20;// represents the cost in reward points for revealing a single letter.
    ifstream rewardFile("rewardpoints.txt");
    if (!rewardFile.is_open())
    {
        cout << "Reward points file not found!" << endl;//if the file can't be opened prints error msg
        return false;
    }

    int rewardPoints;//current reward point is stored
    rewardFile >> rewardPoints;
    rewardFile.close();

    if (rewardPoints < costPerLetter)
    {
        cout << "Insufficient rewards." << endl;//if insufficient rewards 
        return false;
    }

    cout << "Enter the position (starting from 1) to reveal a letter: ";
    int position;//asks which position of word should to reveal
    cin >> position;

    if (position < 1 || position > word.length())
    {
        cout << "Invalid position." << endl;
        return false;//if podition is invalid prints invalid position
    }

    if (guessed.find(word[position - 1]) != string::npos)
    {
        cout << "Letter already revealed." << endl;
        return false;
    }//if the entered position by the user has already guessed prints message

    guessed += word[position - 1];
    rewardPoints -= costPerLetter;//rewards points are reduced
    ofstream rewardFileOut("rewardpoints.txt");
    rewardFileOut << rewardPoints;
    rewardFileOut.close();

    cout << "Letter revealed. Remaining reward points: " << rewardPoints << endl;

    return true;
}

// Function to play the hangman game
int PlayHangman(int &rewardpoints)
{
    cout << "EASY\tMEDIUM\tHARD" << endl;
    cout << "Enter 'e' to choose EASY \n"
         << "Enter 'm' to choose MEDIUM \n"
         << "Enter 'h' to choose HARD " << endl;
    cout << "Choose Level" << endl;
    char x;
    cin >> x;

    srand(time(0));
    string guesses;
    string wordToGuess;
    char letter;
    if (x == 'e' || x == 'E')
    {
        wordToGuess = LoadRandomWord("neweasylevel.text");
    }
    else if (x == 'm' || x == 'M')
    {
        wordToGuess = LoadRandomWord("newmediumlevel.text");
    }
    else if (x == 'h' || x == 'H')
    {
        wordToGuess = LoadRandomWord("newhardlevel.text");
    }
    else
    {
        cout << "Enter the valid character (e, m, h)" << endl;
        return 0; // Return 0 if an invalid level is selected
    }

    vector<char> guessedLetters;
    int tries = 0;
    bool win = false;
    int guessedLetterCount = 0;
    const int maxGuessedLetters = 26;
    do
    {
        system("cls");
        PrintMessage("HANGMAN");
        DrawHangman(tries);
        PrintMessage("Guess the word");
        win = PrintWordAndCheckWin(wordToGuess, guesses);

        if (win)
            break;
        PrintMessage("Guessed Letters");
        char guessedLetters[maxGuessedLetters];
        for (int i = 0; i < guessedLetterCount; i++)
        {
            cout << guessedLetters[i] << " ";
        }
        cout << endl;

        char y;
        cout << ">";
        do
        {
            cout << "Enter a single character: ";
            cin >> y;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

        } while (cin.gcount() != 1);

        guessedLetters[guessedLetterCount] = y;
        guessedLetterCount++;
        if (guesses.find(y) == string::npos)
            guesses += y;

        if (tries >= 5)
        {
            char useReward;
            cout << "Do you want to use reward points to reveal a letter? (y/n): ";
            cin >> useReward;
            if (useReward == 'y' || useReward == 'Y')
            {
                if (!UseRewardPoint(wordToGuess, guesses))
                {
                    continue;
                }
            }
        }

        tries = TriesLeft(wordToGuess, guesses);

    } while (tries <= 9);

    if (win)
    {
        PrintMessage("YOU WON!");
        if (x == 'e' || x == 'E')
        {
            rewardpoints += 10;
        }
        else if (x == 'm' || x == 'M')
        {
            rewardpoints += 30;
        }
        else if (x == 'h' || x == 'H')
        {
            rewardpoints += 50;
        }
        cout << "RewardsPoint:" << rewardpoints << endl;
    }
    else
    {
        PrintMessage("GAME OVER");
        cout << "The secret word is " << wordToGuess << endl;
    }
    return rewardpoints;
}

int main()
{
    char playAgain;
    int totalRewardPoints = 0;               // Initialize total reward points
    ifstream rewardFile("rewardpoints.txt"); // Open file to read existing reward points
    if (rewardFile.is_open())
    {
        rewardFile >> totalRewardPoints; // Read total reward points from file
        rewardFile.close();
    }
    cout << "Total Reward Points: " << totalRewardPoints << endl;

    do
    {
        totalRewardPoints = PlayHangman(totalRewardPoints); // Pass total reward points by reference
        ofstream rewardFileOut("rewardpoints.txt");         // Open file to write updated reward points
        rewardFileOut << totalRewardPoints;                 // Write total reward points to file
        rewardFileOut.close();
        cout << "Do you want to play again? (y/n): ";
        cin >> playAgain;
    } while (playAgain == 'y' || playAgain == 'Y');

    cout << "Total Reward Points: " << totalRewardPoints << endl; // Display total reward points earned

    return 0;
}
