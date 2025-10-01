#include "terminalManagement.h"
#include <fstream>
#include <gmp.h>
#include <gmpxx.h>

using namespace std;

// Remove these, please for the love of everything holy find a better way.
mpz_class string_to_mpz(const std::string &message) {
  mpz_class result = 0;
  for (size_t i = 0; i < message.size(); ++i) {
    result = (result << 8) + (unsigned char)message[i];
  }

  return result;
}

string mpz_to_string(const mpz_class &num) {
  if (num == 0)
    return string(1, '\0');

  mpz_class temp = num;
  string result;

  while (temp > 0) {
    mpz_class quotient, remainder;
    mpz_fdiv_qr_ui(quotient.get_mpz_t(), remainder.get_mpz_t(),
                   temp.get_mpz_t(), 256);
    result = (char)remainder.get_ui() + result;
    temp = quotient;
  }

  return result;
}

// PS: might place each large section into its own function. dunno
void generateKeys(const unsigned &bits, const string &filename) {
  mpz_class p, q, n, phi, e, k;

  // init random
  printStep(1, "Initializing random number generator.");
  gmp_randstate_t state;
  gmp_randinit_mt(state);
  gmp_randseed_ui(state, time(NULL));

  // generate large primes
  printStep(2, "Generating random primes.");
  mpz_urandomb(p.get_mpz_t(), state, bits);
  mpz_nextprime(p.get_mpz_t(), p.get_mpz_t());

  mpz_urandomb(q.get_mpz_t(), state, bits);
  mpz_nextprime(q.get_mpz_t(), q.get_mpz_t());

  // modulus and phi of modulus
  printStep(3, "Calculating modulus and phi.");
  n = p * q;
  phi = (p - 1) * (q - 1);

  // get public key
  // generate e a random GCD(phi, e) = 1
  printStep(4, "Using exponent 65537.");
  e = 65537;
  if (gcd(phi, e) != 1) {
    printVeryRareEvent();
    do {
      mpz_urandomb(e.get_mpz_t(), state, 17);
      if (e % 2 == 0) ++e;
      showAttempt();
    } while (gcd(phi, e) != 1);
    printNewLine();
  }

  // get private key
  // generate invert of e
  printStep(5, "Generating private key.");
  mpz_invert(k.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());

  // clean up
  gmp_randclear(state);

  // store into files
  // store public key
  printStep(6, "Saving keys to files.");
  ofstream file(filename + ".pub");
  // TODO: file checking
  file << n.get_str(16) << '\n';
  file << e.get_str(16);
  file.close();

  // store private key
  file.open(filename);
  // TODO: file checking
  file << n.get_str(16) << '\n';
  file << k.get_str(16);
  file.close();

  printFinishedKeygen(filename);
  return;
}

void encrypt(const std::string &filename, const std::string &message){
  // Consider making a separate functon to load keys
  ifstream file(filename);
  if (!file.is_open()) {
    printFileError(filename);
    return;
  }

  mpz_class n, e, m, r;
  {string tmp;
    getline(file, tmp);
    n.set_str(tmp, 16);
    getline(file, tmp);
    e.set_str(tmp, 16);}
  file.close();

  m = string_to_mpz(message);
  mpz_powm(r.get_mpz_t(), m.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
  printCipher(r.get_str(16));

  return;
}

void decrypt(const std::string &filename, const std::string &cipher);

