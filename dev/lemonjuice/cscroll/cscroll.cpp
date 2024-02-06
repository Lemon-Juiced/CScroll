#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "util\Error_Handler.h"

using namespace std;

// Globally simulate the tape, this way each loop isn't creating their own tape.
int tapeSize = 1000; // Limits the tape to 1,000 integers (bad in practice) - but this hardcode will work for now
int tape[1000]; // Compiler doesn't like setting tapeSize to const so this is the workaround for now
int tapePointer = 0;
int pointerMemory = 0;
int maxTapeUsage = 0;

/**
 * This is the main entryway into the compiler but most of the compilation happens in the loop function.
 */
int main(int argc, char *argv[]){
    // Create an ErrorHandler to handle errors.
    Error_Handler error_handler;

    // Check to make sure there is the correct number of arguments.
    if(argc != 2){
        if(argc < 2) error_handler.fileMissingError(1);
        else error_handler.fileExtraError(argc);
    }

    // Everything from this point forward will have to be handled once more in loop
    // Gets the file passed as an argument.
    fstream programFile;
    // Sets the file as a read-only file.
    programFile.open(argv[1], ios::in);

    // Check if the file has correct extension, error if not
    string fileName = argv[1]; // Get the fileName
    // Find the file extension
    size_t lastDotPos = fileName.find_last_of('.');
    // Check if a dot is found in the file name
    if (lastDotPos != string::npos) string fileExtension = fileName.substr(lastDotPos + 1);
    else error_handler.invalidFileExtensionError(fileName);

    // Moves the file contents into the program.
    char programText[1000000]; //Limits program to 1,000,000 characters (bad in practice) - but this hardcode will work for now
    int pos = 0; //Position (substite for i)
    int programSize = 0; // Count the size of the program
    if(programFile.is_open()){
        while (!programFile.eof()){
            programFile >> std::noskipws >> programText[pos];
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

    // Do the computation.
    int i = 0;
    while(i <= programSize){
        char currChar = programText[i]; // Shorthand for simplicity
        switch(currChar){
            // Pointer Move
            case '<':
                if(tapePointer - 1 < 0) error_handler.outOfBoundsError(); // This prevents the pointer from using a negative index (crashing the program).
                tapePointer--;
                break;
            case '>':
                if(tapePointer + 1 > tapeSize) error_handler.outOfBoundsError(); // This prevents the pointer from using an index after the array (crashing the program).
                if(maxTapeUsage < tapePointer) maxTapeUsage = tapePointer; // This is set so that the print functions don't print the tapeSize worth of characters
                tapePointer++;
                break;
            // Pointer Interaction
            case '+':
                tape[tapePointer] += 1;
                break;
            case '-':
                tape[tapePointer] -= 1;
                break;
            case '.':
                pointerMemory = tape[tapePointer];
                break;
            case ',':
                pointerMemory = i;
                break;
            case '*':
                tape[tapePointer] = tape[tapePointer] + pointerMemory;
                break;
            case '/':
                tape[tapePointer] = tape[tapePointer] - pointerMemory;
                break;
            // Pointer Print
            case '#':
                cout << tape[tapePointer];
                break;
            case '@':
                cout << tape[tapePointer];
                break;
            // Conditional Logic
            case '_':
                // First make sure that the program doesn't crash by the pointer reaching out of bounds.
                // This prevents the pointer from using an index after the array (crashing the program) OR prevents the pointer from using an index after the array (crashing the program).
                if(tapePointer - 1 < 0 || tapePointer + 1 > tapeSize) error_handler.conditionalLogicOutOfBoundsError(); 
                i++; // This hijacks the overall loop's iteration so we can figure out what comparison is being run
                if(programText[i] == '<'){ 
                    // Less Than/Less Than Or Equal To
                    i++; // This hijacks the overall loop's iteration so we can figure out what comparison is being run
                    if(programText[i] == '<'){
                        // Less Than
                        if(tape[tapePointer - 1] < tape[tapePointer + 1]) tape[tapePointer] = 1;
                        else tape[tapePointer] = 0;
                    } else if (programText[i] == '='){
                        // Less Than Or Equal To
                        if(tape[tapePointer - 1] <= tape[tapePointer + 1]) tape[tapePointer] = 1;
                        else tape[tapePointer] = 0;
                    } else {
                        // Error
                        string conditionalLogic;
                        conditionalLogic += programText[i-1];
                        conditionalLogic += programText[i];
                        conditionalLogic += programText[i+1];
                        error_handler.conditionalLogicSyntaxError(conditionalLogic);
                    }
                } else if(programText[i] == '>'){
                    // Greater Than/Greater Than Or Equal To
                    i++; // This hijacks the overall loop's iteration so we can figure out what comparison is being run
                    if(programText[i] == '>'){
                        // Greater Than
                        if(tape[tapePointer - 1] > tape[tapePointer + 1]) tape[tapePointer] = 1;
                        else tape[tapePointer] = 0;
                    } else if (programText[i] == '='){
                        // Greater Than Or Equal To
                        if(tape[tapePointer - 1] >= tape[tapePointer + 1]) tape[tapePointer] = 1;
                        else tape[tapePointer] = 0;
                    } else {
                        // Error
                        string conditionalLogic;
                        conditionalLogic += programText[i-1];
                        conditionalLogic += programText[i];
                        conditionalLogic += programText[i+1];
                        error_handler.conditionalLogicSyntaxError(conditionalLogic);
                    }
                } else if(programText[i] == '='){
                    // Equal To
                    i++; // This hijacks the overall loop's iteration so we can figure out what comparison is being run
                    if(programText[i] == '='){
                        // Equal To
                        if(tape[tapePointer - 1] == tape[tapePointer + 1]) tape[tapePointer] = 1;
                        else tape[tapePointer] = 0;
                    } else {
                        // Error
                        string conditionalLogic;
                        conditionalLogic += programText[i-1];
                        conditionalLogic += programText[i];
                        conditionalLogic += programText[i+1];
                        error_handler.conditionalLogicSyntaxError(conditionalLogic);
                    }
                } else if(programText[i] == '!'){
                    // Not Equal To
                    i++; // This hijacks the overall loop's iteration so we can figure out what comparison is being run
                    if(programText[i] == '='){
                        if(tape[tapePointer - 1] != tape[tapePointer + 1]) tape[tapePointer] = 1;
                        else tape[tapePointer] = 0;
                    } else {
                        // Error
                        string conditionalLogic;
                        conditionalLogic += programText[i-1];
                        conditionalLogic += programText[i];
                        conditionalLogic += programText[i+1];
                        error_handler.conditionalLogicSyntaxError(conditionalLogic);
                    }
                } else {
                    // Error
                    string conditionalLogic;
                    conditionalLogic += programText[i-1];
                    conditionalLogic += programText[i];
                    error_handler.conditionalLogicSyntaxError(conditionalLogic);
                }
            // Ideally loop cases will go here
            // Misc.
            case '^':
                cout << endl;
                break;
            case '\'':
                i++; // This hijacks the overall loop's iteration so that this gets printed as is
                cout << programText[i];
                i++;
                break;
            case '\"':
                i++; // This hijacks the overall loop's iteration so that this gets printed as is
                while(programText[i] != '\"'){
                    cout << programText[i];
                    i++;
                }
                break;
            case '&':
                for(int j=0; j < maxTapeUsage; j++)
                    cout << tape[j];
                break;
            case '$':
                for(int j=0; j < maxTapeUsage; j++)
                    cout << tape[j] << " ";
                break;
            // Whitespace Cases
            case '\n':
            case '\t':
            case ' ':
                break;
            case ';':
                return pointerMemory;
            default:
                error_handler.unexpectedCharacterError(programText[i], i);
        }
        i++; //Increment the loop
    }

    return pointerMemory; //This will return the current memory of the pointer, this is so the program (or loop) given to the compiler doesn't just return 0.
}