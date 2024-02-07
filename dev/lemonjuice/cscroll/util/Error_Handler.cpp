#include "Error_Handler.h"

#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

Error_Handler::Error_Handler(){}

/**
 * Changes the color of the error stream to red for errors.
*/
void Error_Handler::changeErrorStreamColor(){
    HANDLE hConsole = GetStdHandle(STD_ERROR_HANDLE);
    int k = 12;
    SetConsoleTextAttribute(hConsole, k);
}

/**
 * Handles errors when conditional logic uses the pointer as a reference and tries to reach before or after it, thus reaching out of bounds.
 */
void Error_Handler::conditionalLogicOutOfBoundsError(){
    changeErrorStreamColor();
    cerr << "Error: Conditional logic's pointer reference left the bounds of the program." << endl;
    exitProgram(1);
}

/**
 * Handles errors when conditional logic is syntactically incorrect.
 * 
 * @param conditionalLogic The implied logic statement used.
 */
void Error_Handler::conditionalLogicSyntaxError(std::string conditionalLogic){
    changeErrorStreamColor();
    cerr << "Error: The conditional logic \"" << conditionalLogic << "\" is not valid conditional logic." << endl;
    cerr << "Valid conditional logic are: _<<, _>>, _<=, _>=, _==, _!=." << endl;
    exitProgram(1);
}

/**
 * Handles exiting a program when an error is encounted.
 * 
 * @param exitCode The exit code to use.
 */
void Error_Handler::exitProgram(int exitCode){
    revertErrorStreamColor();
    exit(exitCode);
}

/**
 * Handles errors when one or more extra files are specified at compilation.
 * 
 * @param maxArgumentNumber The index of the final argument added
 */
void Error_Handler::fileExtraError(int maxArgumentNumber){
    changeErrorStreamColor();
    for(int i = 3; i < maxArgumentNumber; i++)
        cerr << "Error: Extra file argument at index: " << i << endl;
    cerr << "This normally happens when additional files are specified after the file to be compiled." << endl;
    exitProgram(1);
}

/**
 * Handles errors when there aren't enough errors specified at compilation. 
 * This should only be called if there is 1 in this case.
 * 
 * @param argumentNumber The index of the argument missing
 */
void Error_Handler::fileMissingError(int argumentNumber){
    changeErrorStreamColor();
    cerr << "Error: File argument missing at index: " << argumentNumber << endl;
    cerr << "This normally happens when the file to compile isn't specified." << endl;
    exitProgram(1);
}

/**
 * Handles errors when the file extension is not valid.
 * In this case, the file is not a ".cscroll" file.
 * 
 * @param fileName The name of the file passed to the compiler.
 */
void Error_Handler::invalidFileExtensionError(std::string fileName){
    changeErrorStreamColor();
    cerr << "Error: The file \"" << fileName << "\" is not a valid CScroll file." << endl;
    exitProgram(1);
}

/**
 * Handles errors when a loop's delimiter is in the wrong position
 * 
 * @param index The index of the loop that threw the error
 */
void Error_Handler::loopDelimiterError(int index){
    changeErrorStreamColor();
    cerr << "Error: There is an issue with a delimiter in a loop." << endl;
    cerr << "Loop Index: " << index <<  endl;
    exitProgram(1);
}

/**
 * Handles errors when a loop's integer is not defined between the delimiters
 * 
 * @param d1 The first delimiter
 * @param d2 The second delimiter
 */
void Error_Handler::loopIntNotDefined(int d1, int d2){
    changeErrorStreamColor();
    cerr << "Error: Loop misisng integer between " << d1 << " and " << d2 << "." << endl;
    exitProgram(1);
}

/**
 * Handles errors when a loop is given an invalid type
 * 
 * @param type The type of loop specified, this should not be 'f' or 'w'
 */
void Error_Handler::loopInvalidTypeError(char type) {
    changeErrorStreamColor();
    cerr << "Error: The specified loop type \'" << type << "\' is invalid." << endl;
    cerr << "This can only be \'f\' or \'w\'" <<  endl;
    exitProgram(1);
}

/**
 * Handles errors when there is a nested loop, which is not supported.
 * 
 * @param nestingCharIndex The index at which the nesting character is at.
 */
void Error_Handler::loopNestingError(int nestingCharIndex){
    changeErrorStreamColor();
    cerr << "Error: Loop nesting attempted at index " << nestingCharIndex << endl;
    cerr << "This normally happens when the user tries to nest loops, which is not supported." << endl;
    exitProgram(1);
}

/**
 * Handles errors when a number is expected, but a char is given.
 * 
 * @param nanChar The char that is NaN (not a number).
 */
void Error_Handler::nanError(char nanChar){
    changeErrorStreamColor();
    cerr << "Error: " << nanChar << " is NaN." << endl;
    cerr << "This is normally the result of a typo." << endl;
    exitProgram(1);
}

/**
 * Handles errors when the pointer goes out of bounds.
 */
void Error_Handler::outOfBoundsError(){
    changeErrorStreamColor();
    cerr << "Error: Pointer left the bounds of the program." << endl;
    cerr << "This normally happens when the pointer reaches index \'-1\' on the tape." << endl;
    exitProgram(1);
}

/**
 * Reverts the color of the error stream back to default.
*/
void Error_Handler::revertErrorStreamColor(){
    HANDLE hConsole = GetStdHandle(STD_ERROR_HANDLE);
    int k = 0;
    SetConsoleTextAttribute(hConsole, k);
}


/**
 * Handles errors when an unexpected character is encountered.
 * 
 * @param unexpectedCharacter The unexpected character
 * @param location The index of the error
 */
void Error_Handler::unexpectedCharacterError(char unexpectedCharacter, int location){
    changeErrorStreamColor();
    cerr << "Error: Unexpected character \'" << unexpectedCharacter << "\' found at index: \'" << location << "\'" << endl;
    exitProgram(1);
}