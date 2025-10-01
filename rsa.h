#ifndef RSA_H
#define RSA_H
#include <gmpxx.h>

mpz_class string_to_mpz(const std::string &message);

std::string mpz_to_string(const mpz_class &num);

void generateKeys(const unsigned &bits, const std::string &filename);

bool encrypt(const std::string &filename, std::string message);

bool decrypt(const std::string &filename, std::string cipher);

#endif
