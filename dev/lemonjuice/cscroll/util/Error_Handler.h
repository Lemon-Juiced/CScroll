#pragma once 
#include <string>

class Error_Handler{
private:
public:
    Error_Handler();
    void changeErrorStreamColor();
    void conditionalLogicOutOfBoundsError();
    void conditionalLogicSyntaxError(std::string);
    void exitProgram(int);
    void fileExtraError(int);
    void fileMissingError(int);
    void invalidFileExtensionError(std::string);
    void loopDelimiterError(int);
    void loopIntNotDefined(int, int);
    void loopInvalidTypeError(char);
    void loopNestingError(int);
    void nanError(char);
    void outOfBoundsError();
    void revertErrorStreamColor();
    void unexpectedCharacterError(char, int);
};