#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "util\Error_Handler.h"

using namespace std;

int main(int argc, char *argv[]){
    // Create an ErrorHandler to handle errors.
    Error_Handler error_handler;

    // Check to make sure there is the correct number of arguments.
    if(argc != 2){
        if(argc < 2) error_handler.fileMissingError(1);
        else error_handler.fileExtraError(argc);
    }

    // Gets the file passed as an argument.
    fstream programFile;
    programFile.open(argv[1], ios::in);

    // To Do: Check the file has correct extension (error out if not)

    // Moves the file contents into the program.
    char programText[1000000]; //Limits program to 1,000,000 characters (bad in practice) - but this hardcode will work for now
    int pos = 0; //Position (substite for i)
    int programSize = 0; // Count the size of the program
    if(programFile.is_open()){
        while (!programFile.eof()){
            programFile >> programText[pos];
            pos++;
            programSize++;
        }
    }
    programFile.close();

    //Test to make sure the programText holds program
    /*
    cout << "Input Program:" << endl;
    for(int i = 0; i <= programSize; i++)
        cout << programText[i];
    cout << endl;
    */

    // Simulate the tape.
    int tapeSize = 1000; // Limits the tape to 1,000 integers (bad in practice) - but this hardcode will work for now
    int tape[1000]; // Compiler doesn't like setting tapeSize to const so this is the workaround for now
    int tapePointer = 0;
    int pointerMemory = 0;
    int maxTapeUsage = 0;

    bool cDebug = false; //Set this to try to debug the compiler.

    // Do the computation.
    int i = 0;
    if(cDebug) cout << "Program Size: " << programSize << endl;
    while(i <= programSize){
        if(cDebug) cout << "Index " << i << ": ";
        char currChar = programText[i]; // Shorthand for simplicity
        if(currChar == '<'){
            if(cDebug) cout << '<' << endl;
            if(tapePointer - 1 < 0) error_handler.outOfBoundsError(); // This prevents the pointer from using a negative index (crashing the program).
            tapePointer--;
        } else if (currChar == '>'){
            if(cDebug) cout << '>' << endl;
            if(tapePointer + 1 > tapeSize) error_handler.outOfBoundsError(); // This prevents the pointer from using an index after the array (crashing the program).
            if(maxTapeUsage < tapePointer) maxTapeUsage = tapePointer; // This is set so that the print functions don't print the tapeSize worth of characters
            tapePointer++;
        } else if (currChar == '+'){
            if(cDebug) cout << '+' << endl;
            tape[tapePointer] += 1;
        } else if (currChar == '-'){
            if(cDebug) cout << '-' << endl;
            tape[tapePointer] -= 1;
        } else if (currChar == '.'){
            if(cDebug) cout << '.' << endl;
            pointerMemory = tape[tapePointer];
        } else if (currChar == '*'){
            if(cDebug) cout << '*' << endl;
            tape[tapePointer] = tape[tapePointer] + pointerMemory;
        } else if (currChar == '/'){
            if(cDebug) cout << '/' << endl;
            tape[tapePointer] = tape[tapePointer] - pointerMemory;
        } else if (currChar == '#'){
            if(cDebug) cout << '#' << endl;
            cout << tape[tapePointer];
        } else if (currChar == '@'){
            if(cDebug) cout << '@' << endl;
            cout << (char) tape[tapePointer];
        } // Ideally loop cases belong here
        else if (currChar == '^'){
            if(cDebug) cout << '^' << endl;
            cout << endl;
        } else if (currChar == '\''){
            if(cDebug) cout << '\'' << endl;
            i++; // This hijacks the overall loops iteration so that this gets printed as is
            cout << programText[i];
            i++;
        } else if (currChar == '\"'){
            if(cDebug) cout << '\"' << endl;
            i++; // This hijacks the overall loops iteration so that this gets printed as is
            while(programText[i] != '\"'){
                cout << programText[i];
                i++;
            }
        } else if(currChar == '&'){
            if(cDebug) cout << '&' << endl;
            for(int j=0; j < maxTapeUsage; j++)
                cout << tape[j];
        } else if(currChar == '$'){
            if(cDebug) cout << '$' << endl;
            for(int j=0; j < maxTapeUsage; j++)
                cout << tape[j] << " ";
        } else if(currChar == '\n' || currChar == ' '){
            if(cDebug) cout << "Whitespace" << endl;
        } else if(currChar == ';'){
            if(cDebug) cout << ';' << endl;
            return 0;
        } else {
            if(cDebug) cout << "Error Handling" << endl;
            error_handler.unexpectedCharacterError(programText[i], i);
        }
        i++; //Increment the loop
    }

    return 0;
}