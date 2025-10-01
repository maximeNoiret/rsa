#include <cctype>
#include <gmp.h>
#include <gmpxx.h>

#include "rsa.h"
#include "terminalManagement.h"

using namespace std;

int main(const int argc, const char **argv) {
  if (argc == 1) {
    incorrectUsage(argv[0]);
    return 1;
  }
  if (argv[1] == string("help")) {
    helpMenu(argc, argv);
    return 0;
  }
  if (argv[1] == string("keygen")) {
    unsigned bits = 512;
    string filename = "key";
    if (argc >= 3 && isdigit(argv[2][0])) {
      bits = stoul(argv[2]);
      if (argc == 4) {
        filename = argv[3];
      }
    }
    generateKeys(bits, filename);
  }
  if (argv[1] == string("encrypt")) {
    if (argc < 4) {
      incorrectUsage(argv[0]);
      return 1;
    }
    return encrypt(argv[2], argv[3]);
  }
  if (argv[1] == string("decrypt")) {
    if (argc < 4) {
      incorrectUsage(argv[0]);
      return 1;
    }
    return decrypt(argv[2], argv[3]);
  }
  incorrectUsage(argv[0]);
  return 1;
}
