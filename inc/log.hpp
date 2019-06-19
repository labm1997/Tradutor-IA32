#ifndef HEADER_LOG
#define HEADER_LOG

#include "log.hpp"
#include <iostream>
#include <string>

#define UI_BOLD_RED_BACK_RED "\x1B[1;37;41m"
#define UI_BOLD_RED "\x1B[1;31m"
#define UI_BOLD "\x1B[1m"
#define UI_NORMAL  "\x1B[0m"

void logPrintLines(int ppLine, int defLine, int oLine);

void logSemanticError(std::string message, int ppLine, int defLine, int oLine);

void logSemanticError(std::string message);

void logSyntaxError(std::string message, int ppLine, int defLine, int oLine);

void logInternalError(int ppLine, int defLine, int oLine);

void logppSyntaxError(std::string message, int lineNumber);

void logppSemanticError(std::string message, int lineNumber);
void logppSemanticError(std::string message);

void logppLexicalError(std::string message, int lineNumber);


void logparserSyntaxError(std::string message, int lineNumber);
#endif
