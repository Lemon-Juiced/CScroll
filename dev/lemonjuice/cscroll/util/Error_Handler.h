#pragma once 

class Error_Handler{
private:
public:
    Error_Handler();
    void changeErrorStreamColor();
    void exitProgram(int);
    void fileExtraError(int);
    void fileMissingError(int);
    void loopCommaSyntaxError(int);
    void nanError(char);
    void outOfBoundsError();
    void revertErrorStreamColor();
    void unexpectedCharacterError(char, int);
};