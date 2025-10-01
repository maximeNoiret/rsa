#ifndef RSA_H
#define RSA_H
#include <gmpxx.h>

mpz_class string_to_mpz(const std::string &message);

std::string mpz_to_string(const mpz_class &num);

void generateKeys(const unsigned &bits, const std::string &filename);

void encrypt(const std::string &filename, const std::string &message);

void decrypt(const std::string &filename, const std::string &cipher);

#endif
