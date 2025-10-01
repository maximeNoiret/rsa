#ifndef RSA_H
#define RSA_H
#include <gmpxx.h>

const char SEPARATOR = ':';
const size_t PAD_LEN = 11;

mpz_class string_to_mpz(const std::string &message);

std::string mpz_to_string(const mpz_class &num);

void generateKeys(const unsigned &bits, const std::string &filename);

int encrypt(const std::string &filename, std::string message);

int decrypt(const std::string &filename, std::string cipher);

#endif
