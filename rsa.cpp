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

  printStep(1, "Initializing random number generator.");
  gmp_randstate_t state;
  gmp_randinit_mt(state);
  gmp_randseed_ui(state, time(NULL));

  printStep(2, "Generating random primes.");
  mpz_urandomb(p.get_mpz_t(), state, bits);
  mpz_nextprime(p.get_mpz_t(), p.get_mpz_t());

  mpz_urandomb(q.get_mpz_t(), state, bits);
  mpz_nextprime(q.get_mpz_t(), q.get_mpz_t());

  printStep(3, "Calculating modulus and phi.");
  n = p * q;
  phi = (p - 1) * (q - 1);

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

  printStep(5, "Generating private key.");
  mpz_invert(k.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());

  gmp_randclear(state);

  // store into files (make functions later)
  printStep(6, "Saving keys to files.");
  ofstream file(filename + ".pub");
  // TODO: file checking
  file << n.get_str(16) << '\n';
  file << e.get_str(16);
  file.close();

  file.open(filename);
  // TODO: file checking
  file << n.get_str(16) << '\n';
  file << k.get_str(16);
  file.close();

  printFinishedKeygen(filename);
  return;
}

bool loadKeys(mpz_class &n, mpz_class &x, const string &filename) {
  ifstream file(filename);
  if (!file.is_open()) {
    printFileError(filename);
    return false;
  }

  {string tmp;
    getline(file, tmp);
    n.set_str(tmp, 16);
    getline(file, tmp);
    x.set_str(tmp, 16);}
  file.close();
  return true;
}

int encrypt(const string &filename, string message){
  mpz_class n, e, m, r;
  if (!loadKeys(n, e, filename))
    return 1;

  if (message == "-") {
    getFromSTDIN(message);
  }

  m = string_to_mpz(message);
  mpz_powm(r.get_mpz_t(), m.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
  printMessage(r.get_str(16));

  return 0;
}

int decrypt(const string &filename, string cipher) {
  mpz_class n, k, c, r;
  if(!loadKeys(n, k, filename))
    return 1;

  if (cipher == "-") {
    getFromSTDIN(cipher);
  }

  c.set_str(cipher, 16);
  mpz_powm(r.get_mpz_t(), c.get_mpz_t(), k.get_mpz_t(), n.get_mpz_t());
  printMessage(mpz_to_string(r));

  return 0;
}

