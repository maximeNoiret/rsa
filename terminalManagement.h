#ifndef TERMINALMANAGEMENT_H
#define TERMINALMANAGEMENT_H

#include <string>

void incorrectUsage(const char *programName);
void helpMenu(const int &argc, const char **argv);

void printStep(const unsigned &step, const std::string &message);

void showAttempt();

void printNewLine();

void printFinishedKeygen(const std::string &filename);
#endif
