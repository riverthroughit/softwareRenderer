#include"FuncCodes.h"

unsigned short FuncCodes::code = 2;
queue<unsigned short> FuncCodes::codes;
mutex FuncCodes::codesMutex;