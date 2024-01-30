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