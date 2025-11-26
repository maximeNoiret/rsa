#ifndef TERMINALMANAGEMENT_H
#define TERMINALMANAGEMENT_H

#include <string>

void incorrectUsage(const char *programName);

void helpMenu(const int &argc, const char **argv);

void printStep(const unsigned &step, const std::string &message);

void printFound(const char name);

void printSaved(const std::string path);

void printVeryRareEvent();

void showAttempt();

void printNewLine();

void printFinishedKeygen(const std::string &filename);

void printFileError(const std::string &filename);

void printMessage(const std::string &message);

void getFromSTDIN(std::string &input);
#endif
