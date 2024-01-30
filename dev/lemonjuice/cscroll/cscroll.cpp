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
 * The compilation happens here, despite being called loop this doesn't only run for loops. 
 * The initial compilation is treated as one loop, the function calls itself recursively if a loop is entered.
 */
int loop(char programText[], int programSize, Error_Handler error_handler){
    cout << "Input Program:" << endl;
    for(int i = 0; i <= programSize; i++)
        cout << programText[i];
    cout << endl;

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
        } else if (currChar == '('){
            // This handles loops alone.
            // We need to find how many times to iterate and what .cscroll file to iterate over.
            char loopText[1000]; //This is hardcoded to assume that the name of the loop won't contain over 1,000 characters (bad in practice) - but this hardcode will work for now
            i++; // This hijacks the overall loops iteration so that this gets printed as is
            int j = 0; // Also ultimately stores loopText's own size
            int iCommaPos = i;
            int jCommaPos = 0;
            while(programText[i] != ')'){
                if(programText[i] == ' ' || programText[i] == '\n') i++; //Skipping whitespace
                if(programText[i] == ',') jCommaPos = j; 
                loopText[j] = programText[i];
                i++; //Increment i to increment the loop
                j++;
            }
            // loopText holds an amount of times to iterate and the file to iterate over seperated by a comma. If there is no comma, throw error.
            // We should see jCommaPos be something other than 0, if it is more than 0 no error.
            if(jCommaPos == 0) error_handler.loopCommaSyntaxError(iCommaPos);

            // Now determine which loop syntax is in use '%' or {number_of_iterations}
            bool isLoopSyntax1 = false;
            int numberOfIterations = 0;
            if(loopText[0] == '%') {
                isLoopSyntax1 = true;
                numberOfIterations = pointerMemory;
            } else {
                for(int k = 0; k < jCommaPos; k++){
                    // If the text here isn't a digit, its a char throw an error
                    // Else it is a digit, add it to the number of iterations, multiplied by 10, because this number is added to the 1s place.
                    if(!isdigit(loopText[k])) error_handler.nanError(loopText[k]);
                    else numberOfIterations = (int) (numberOfIterations * 10) + loopText[k];
                }
            }

            // Find the file to loop over.
            string subProgramName = "";
            for(int k = jCommaPos; k < j; k++) subProgramName += loopText[k];

            // Now set the subProgram up to be run by this method.

            // Now just run the loop
            for(int l = 0; l < numberOfIterations; l++){
                //return(loop);
            }

        } else if (currChar == '^'){
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

    return pointerMemory; //This will return the current memory of the pointer, this is so the program (or loop) given to the compiler doesn't just return 0.
}

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

    // Hands the file to loop, loop takes over from here.
    return loop(programText, programSize, error_handler);

    // Below isn't being reached right now, but that's fine, this is just for back up in case the conception for loops fails.

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

    return pointerMemory; //This will return the current memory of the pointer, this is so the program (or loop) given to the compiler doesn't just return 0.
}