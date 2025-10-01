#include "terminalManagement.h"
#include <iomanip>
#include <iostream>

using namespace std;

void incorrectUsage(const char *programName) {
  cerr << "Incorrect usage. Please include a command." << '\n'
       << "Correct usages:" << '\n'
       << '\t' << programName << " help [command]" << '\n'
       << '\t' << programName << " keygen [bits] [output filename]" << '\n'
       << '\t' << programName << " encrypt (keyfile) (message)" << '\n'
       << '\t' << programName << " decrypt (keyfile) (message)" << '\n';
}

void helpMenu(const int &argc, const char **argv) {
  if (argc < 3) {
    cout << "Help Menu" << '\n'
         << '\n'
         << "- help [command]" << '\n'
         << '\t' << "Displays help about a command." << '\n'
         << "- keygen [bits] [output filename]" << '\n'
         << '\t'
         << "Generates a keypair of [bits] bits, 512 by default, and stores "
            "them into key.pub and key by default."
         << '\n'
         << "- encrypt (keyfile) (message)" << '\n'
         << '\t' << "Prints out (message) encrypted with public key (keyfile)"
         << '\n'
         << "- decrypt (keyfile) (cipher)" << '\n'
         << '\t' << "Prints out (cipher) decrypted with private key (keyfile)"
         << '\n';
    return;
  }
  if (argv[2] == string("help")) {
    cout << "Help - help" << '\n'
         << '\n'
         << "This command displays help about the different commands" << '\n'
         << "available or, if a command is passed in argument, displays "
            "help about that command specifically."
         << '\n'
         << "Usage:" << '\n'
         << '\t' << "help [command]" << '\n';
  } else if (argv[2] == string("keygen")) {
    cout << "Help - keygen" << '\n'
         << '\n'
         << "This command generates a pair of public and private keys." << '\n'
         << "It generates them in 2 files, key.pub and key by default." << '\n'
         << "It has 2 arguments:" << '\n'
         << '\t' << setw(15) << left << "- [bits]"
         << "Specifies the bitwidth of the keys. Default is 512" << '\n'
         << '\t' << setw(15) << left << "- [filename]"
         << "Specifies the output file name. Default is 'key'" << '\n';
  } else if (argv[2] == string("encrypt")) {
    cout << "Help - encrypt" << '\n'
         << '\n'
         << "This command takes in a message and prints out the encrypted "
            "version using a public key."
         << '\n'
         << "It has 2 arguments:" << '\n'
         << '\t' << setw(15) << left << "- (keyfile)"
         << "The path to the public key file." << '\n'
         << '\t' << setw(15) << left << "- (message)"
         << "The message to encrypt." << '\n';
  } else if (argv[2] == string("decrypt")) {
    cout << "Help - decrypt" << '\n'
         << '\n'
         << "This command takes in an encrypted message and prints out the "
            "decrypted version using a private key."
         << '\n'
         << "It has 2 arguments:" << '\n'
         << '\t' << setw(15) << left << "- (keyfile)"
         << "The path to the private key file." << '\n'
         << '\t' << setw(15) << left << "- (cipher)"
         << "The encrypted message to decrypt." << '\n';
  }
}

void printStep(const unsigned &step, const string &message) {
  cout << setw(3) << right << step << setw(3) << left << '.' << message << '\n';
}

void printNewLine() { cout << '\n'; }

void showAttempt() { cout << '|'; }

void printFinishedKeygen(const string &filename) {
  cout << "Finished generating key pair." << '\n'
       << "Saved into " << filename << ".pub and " << filename << '\n';
}
