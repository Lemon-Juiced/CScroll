#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include "util\Error_Handler.h"

using namespace std;

// Proto-Types
int loop(char loopType, int intParameter, string loopText, int iOffset);
int forLoop(int iterations, string loopText, int iOffset);
int whileLoop(int index, string loopText);
int run(char programText[], int programSize);
int run(char programText[], int programSize, bool isNested, int iOffset);

// Globally simulate the tape, this way each loop isn't creating their own tape.
int tapeSize = 10000; // Limits the tape to 1,000 integers (bad in practice) - but this hardcode will work for now
int tape[10000]; // Compiler doesn't like setting tapeSize to const so this is the workaround for now
int tapePointer = 0;
int pointerMemory = 0;
int pointerIndex = 0;
int maxTapeUsage = 0;

// Create an ErrorHandler to handle errors.
Error_Handler error_handler;

/**
 * This is the main entryway into the interpreter, but most of the interpretation happens in the run function.
 */
int main(int argc, char *argv[]){

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

    // Sends the program to the run section.
    return run(programText, programSize);
}

/**
 * Handles loops when handed to a program, divides into while and for loop from here.
 * 
 * @param loopType The character describing the loop type 'f' for "for" or 'w' for "while".
 * @param intParameter The integer parameter of the loop.
 * @param loopText The text of the loop.
 * @param iOffset The offset for 'i' in the run section
 */
int loop(char loopType, int intParameter, string loopText, int iOffset){
    if(loopType == 'f') return forLoop(intParameter, loopText, iOffset);
    return whileLoop(intParameter, loopText);
}

/**
 * Handles for loops.
 * 
 * @param iterations The number of iterations this loop runs for
 * @param loopText The text of the loop.
 * @param iOffset The offset for 'i' in the run section
 */
int forLoop(int iterations, string loopText, int iOffset){
    cout << "You've entered the for loop" << endl;

    //Convert the loopText string to a char[] programText
    char programText[1000000]; //Limits program to 1,000,000 characters (bad in practice) - but this hardcode will work for now
    int pos = 0; //Position (substite for i)
    int programSize = loopText.length(); // Count the size of the program

    for(int i = 0; i < programSize; i++){
        programText[i] = loopText[i];
        cout << "Index " << i << ": " << programText[i] << endl; //Troubleshooting line
    }

    for(int i = 0; i < iterations; i++){
        run(programText, programSize - 1, true, iOffset);
        iOffset++;
    }

    return pointerMemory; //This will return the current memory of the pointer, this is so the loop given to the compiler doesn't just return 0.
}

/**
 * Handles while loops.
 * 
 * @param iterations The index that this while loop depends on.
 * @param loopText The text of the loop.
 */
int whileLoop(int index, string loopText){

    cout << "You've entered the while loop" << endl;

    return pointerMemory; //This will return the current memory of the pointer, this is so the loop given to the compiler doesn't just return 0.
}

/**
 * The run section of a program.
 * This calls the 3 argument version of the function, assuming by default that it is not nested and iOffset is 0.
 * 
 * @param programText Contains the text of the program
 * @param programSize The size of the program itself.
*/
int run(char programText[], int programSize){
    return run(programText, programSize, false, 0);
}

/**
 * The run section of a program.
 * 
 * @param programText Contains the text of the program
 * @param programSize The size of the program itself.
 * @param isNested True if this is nested in another CScroll program, false otherwise
 * @param iOffset The offset added to 'i' when initialized.
*/
int run(char programText[], int programSize, bool isNested, int iOffset){
    // This is just stored here in case a loop is used, the switch doesn't like when its initialized inside of it for some reason.
    string loopText;
    int firstDelimiter = 0;
    int secondDelimiter = 0;
    char loopType = ' ';
    int loopIntParameter = 0;

    if(isNested){
        cout << "Debug Info" << endl;
        cout << "tapePointer: " << tapePointer << endl;
        cout << "pointerMemory: " << pointerMemory << endl;
        for(int k = 0; k <= programSize; k++){
            cout << programText[k] ;
        }
        cout << endl;
    }

    // Do the computation.
    int i = 0;
    while(i <= programSize){
        char currChar = programText[i]; // Shorthand for simplicity
        switch(currChar){
            // Pointer Move
            case '<':
                if(tapePointer - 1 < 0) error_handler.outOfBoundsError(); // This prevents the pointer from using a negative index (crashing the program).
                tapePointer--;
                pointerIndex--;
                break;
            case '>':
                if(tapePointer + 1 > tapeSize) error_handler.outOfBoundsError(); // This prevents the pointer from using an index after the array (crashing the program).
                if(maxTapeUsage < tapePointer) maxTapeUsage = tapePointer; // This is set so that the print functions don't print the tapeSize worth of characters
                tapePointer++;
                pointerIndex++;
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
                if(isNested) cout << "pointerMemory set to " << pointerMemory << " from '.'" << endl;
                break;
            case ',':
                pointerMemory = pointerIndex;
                if(isNested) cout << "pointerMemory set to " << pointerMemory << " from ','" << endl;
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
                switch(programText[i]){
                    case '<':
                        // << / <=
                        i++;
                        switch(programText[i]){
                            case '<':
                                if(tape[tapePointer - 1] < tape[tapePointer + 1]) tape[tapePointer] = 1;
                                else tape[tapePointer] = 0;
                                break;
                            case '=':
                                if(tape[tapePointer - 1] <= tape[tapePointer + 1]) tape[tapePointer] = 1;
                                else tape[tapePointer] = 0;
                                break;
                            default:
                                string conditionalLogic;
                                conditionalLogic += programText[i-1];
                                conditionalLogic += programText[i];
                                conditionalLogic += programText[i+1];
                                error_handler.conditionalLogicSyntaxError(conditionalLogic);
                        }
                        break;
                    case '>':
                        // >> / >=
                        i++;
                        switch(programText[i]){
                            case '>':
                                if(tape[tapePointer - 1] > tape[tapePointer + 1]) tape[tapePointer] = 1;
                                else tape[tapePointer] = 0;
                                break;
                            case '=':
                                if(tape[tapePointer - 1] >= tape[tapePointer + 1]) tape[tapePointer] = 1;
                                else tape[tapePointer] = 0;
                                break;
                            default:
                                string conditionalLogic;
                                conditionalLogic += programText[i-1];
                                conditionalLogic += programText[i];
                                conditionalLogic += programText[i+1];
                                error_handler.conditionalLogicSyntaxError(conditionalLogic);
                        }
                        break;
                    case '=':
                        // ==
                        i++; 
                        if(programText[i] == '='){
                            if(tape[tapePointer - 1] == tape[tapePointer + 1]) tape[tapePointer] = 1;
                            else tape[tapePointer] = 0;
                        } else {
                            string conditionalLogic;
                            conditionalLogic += programText[i-1];
                            conditionalLogic += programText[i];
                            conditionalLogic += programText[i+1];
                            error_handler.conditionalLogicSyntaxError(conditionalLogic);
                        }
                        break;
                    case '!':
                        // !=
                        i++; 
                        if(programText[i] == '='){
                            if(tape[tapePointer - 1] != tape[tapePointer + 1]) tape[tapePointer] = 1;
                            else tape[tapePointer] = 0;
                        } else {
                            string conditionalLogic;
                            conditionalLogic += programText[i-1];
                            conditionalLogic += programText[i];
                            conditionalLogic += programText[i+1];
                            error_handler.conditionalLogicSyntaxError(conditionalLogic);
                        }
                        break;
                    default:
                        string conditionalLogic;
                        conditionalLogic += programText[i-1];
                        conditionalLogic += programText[i];
                        conditionalLogic += programText[i+1];
                        error_handler.conditionalLogicSyntaxError(conditionalLogic);
                }
                break;
            // Loop Cases, Ideally This Should Only Be '(', because ')' will be covered before returning
            case '(':
                i++; // This hijacks the overall loop's iteration so that we don't get the '(' symbol
                iOffset = i;

                // A check to make sure loop-nesting was attempted
                if(isNested) error_handler.loopNestingError(i);

                // We need to find the next ')' and every character in between, we can do this because no nested loops.
                // Additionally, we should find each ':' here so we don't have to find them on a second search.
                while(programText[i] != ')'){
                    // Check to make sure loop-nesting wasn't attempted
                    if(programText[i] == '(') error_handler.loopNestingError(i);

                    if(programText[i] == ':'){
                        if(firstDelimiter == 0) firstDelimiter = i;
                        else secondDelimiter = i;
                    }

                    loopText += programText[i];
                    i++;
                }

                if(firstDelimiter == 0 || secondDelimiter == 0) error_handler.loopDelimiterError(i);

                cout << "Loop Text: \"" << loopText << "\"" << endl;
                cout << "Delimiter Positions: " << firstDelimiter << ", " << secondDelimiter << endl;

                // Get the loopType
                for(int j = firstDelimiter - 1; j >= 0; j--){
                    if(isalpha(programText[j])) {
                        loopType = programText[j];
                        break;
                    }
                    if(programText[j] == '(') error_handler.loopInvalidTypeError('(');
                }
                cout << "Loop Type: " << loopType << endl;
                if(loopType != 'f' && loopType != 'w') error_handler.loopInvalidTypeError(loopType);

                // Get the loopIntParameter
                for(int j = firstDelimiter; j < secondDelimiter; j++){
                    if(isdigit(programText[j])) {
                        loopIntParameter *= 10;
                        loopIntParameter += (programText[j] - '0');
                    }
                    if(j == secondDelimiter - 1){
                        if(loopIntParameter != 0) break;
                        else error_handler.loopIntNotDefined(firstDelimiter, secondDelimiter);
                    }
                }
                cout << "Loop Int Param: " << loopIntParameter << endl;

                // Get the rest of the text after the 2nd delimiter
                loopText = loopText.substr(i - secondDelimiter + 1, loopText.length());
                cout << "Loop Text: " << loopText << endl;

                // This can now be sent to the loop function
                pointerMemory = loop(loopType, loopIntParameter, loopText, iOffset);

                // Clear loop variables in case there is another loop
                loopText = "";
                firstDelimiter = 0;
                secondDelimiter = 0;
                loopType = ' '; //This has to be atleast one character
                loopIntParameter = 0;
                break;
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
                for(int j=0; j <= maxTapeUsage; j++)
                    cout << tape[j];
                break;
            case '$':
                for(int j=0; j <= maxTapeUsage; j++)
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

    return pointerMemory; //This will return the current memory of the pointer, this is so the program given to the compiler doesn't just return 0.
}