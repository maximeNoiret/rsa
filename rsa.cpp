#include <fstream>
#include <gmp.h>
#include <gmpxx.h>
#include <thread>

#include "rsa.h"
#include "terminalManagement.h"

using namespace std;

const string primeText = "prime_";

const char kPathSeparator =
#ifdef _WIN32
    '\\';
#else
    '/';
#endif

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

void generatePrime(mpz_class &x, const unsigned &bits, gmp_randstate_t &state,
                   const char name, const string path = "") {
  mpz_urandomb(x.get_mpz_t(), state, bits);
  mpz_setbit(x.get_mpz_t(), bits - 1); // ensure high bit set
  mpz_setbit(x.get_mpz_t(), 0);        // ensure odd
  mpz_nextprime(x.get_mpz_t(), x.get_mpz_t());
  printFound(name);
  // store prime in case of interruption
  if (!path.empty()) {
    ofstream prime(path + kPathSeparator + primeText +
                   name); // this is... stupid af
    prime << x.get_str(16);
    prime.close();
    printSaved(path + kPathSeparator + primeText + name);
  }
}

// PS: might place each large section into its own function. dunno
void generateKeys(const unsigned &bits, const string &filename,
                  const string path = "") {
  mpz_class p, q, n, phi, e, k;

  printStep(1, "Initializing random number generator.");
  gmp_randstate_t statep, stateq;
  gmp_randinit_mt(statep);
  gmp_randinit_mt(stateq);
  gmp_randseed_ui(statep, time(NULL));
  srand(time(NULL));
  gmp_randseed_ui(stateq, time(NULL) + rand());

  printStep(2, "Generating random primes.");
  // TODO: get primes from file if files exist

  // if file not exist, generate the prime
  thread tp(generatePrime, ref(p), ref(bits), ref(statep), 'p', path);
  thread tq(generatePrime, ref(q), ref(bits), ref(stateq), 'q', path);

  tp.join();
  tq.join();

  gmp_randclear(stateq);

  printStep(3, "Calculating modulus and phi.");
  n = p * q;
  phi = (p - 1) * (q - 1);

  printStep(4, "Using exponent 65537.");
  e = 65537;
  if (gcd(phi, e) != 1) {
    printVeryRareEvent();
    do {
      mpz_urandomb(e.get_mpz_t(), statep, 17);
      if (e % 2 == 0)
        ++e;
      showAttempt();
    } while (gcd(phi, e) != 1);
    printNewLine();
  }

  gmp_randclear(statep);

  printStep(5, "Generating private key.");
  mpz_invert(k.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());

  // store into files (make functions later)
  printStep(6, "Saving keys to files.");
  ofstream file(filename + ".pub");
  if (!file) {
    printFileError(filename + ".pub");
    return;
  }
  file << n.get_str(16) << '\n';
  file << e.get_str(16);
  file.close();

  file.open(filename);
  if (!file) {
    printFileError(filename);
    return;
  }
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

  {
    string tmp;
    getline(file, tmp);
    n.set_str(tmp, 16);
    getline(file, tmp);
    x.set_str(tmp, 16);
  }
  file.close();
  return true;
}

size_t byteLength(const mpz_class &n) {
  return (mpz_sizeinbase(n.get_mpz_t(), 2) + 7) / 8;
}

int encrypt(const string &filename, string message) {
  mpz_class n, e;
  if (!loadKeys(n, e, filename))
    return 1;

  if (message == "-") {
    getFromSTDIN(message);
  }

  const size_t byteLen = byteLength(n);
  const size_t maxClen = byteLen - PAD_LEN; // max chunk length
  string cipher;

  for (size_t i = 0; i < message.size(); i += maxClen) {
    size_t cLen = min(maxClen, message.size() - i); // current chunk length
    string chunk = message.substr(i, cLen);         // extract chunk

    // uh... push_back even though we reserve? need to check on that
    // build EB (00 01 PS 00 DATA)
    string eb;
    eb.reserve(byteLen);
    eb.push_back(0x00);
    eb.push_back(0x01);
    eb.append(byteLen - 3 - cLen, 0xFF); // PS
    eb.push_back(0x00);
    eb.append(chunk);

    mpz_class m = string_to_mpz(eb), r;
    mpz_powm(r.get_mpz_t(), m.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());

    string hexStr = r.get_str(16);
    size_t expectedLen = byteLen * 2; // 2 hex chars per byte
    if (hexStr.size() < expectedLen)
      hexStr.insert(0, expectedLen - hexStr.size(), '0');
    cipher += hexStr + SEPARATOR;
  }
  cipher.pop_back(); // remove trailing separator
  printMessage(cipher);
  return 0;
}

int decrypt(const string &filename, string cipher) {
  mpz_class n, k, c, r;
  if (!loadKeys(n, k, filename))
    return 1;

  // piping
  if (cipher == "-") {
    getFromSTDIN(cipher);
  }

  const size_t byteLen = byteLength(n);
  string message;

  for (size_t pos = 0; pos < cipher.size();) {
    size_t next = cipher.find(SEPARATOR, pos); // find next separator
    if (next == string::npos)
      next = cipher.size();

    string chunk = cipher.substr(pos, next - pos);
    pos = next + 1;

    mpz_class c, r;
    c.set_str(chunk, 16);
    mpz_powm(r.get_mpz_t(), c.get_mpz_t(), k.get_mpz_t(), n.get_mpz_t());

    string hexStr = r.get_str(16);
    size_t expectedLen = byteLen * 2;
    if (hexStr.size() < expectedLen) {
      hexStr.insert(0, expectedLen - hexStr.size(), '0');
    }

    if (hexStr.size() < expectedLen) {
      hexStr.insert(0, expectedLen - hexStr.size(), '0');
    }

    string eb;
    for (size_t i = 0; i < hexStr.size(); i += 2) {
      eb.push_back((char)stoi(hexStr.substr(i, 2), nullptr, 16));
    }

    // padding check
    if (eb.size() != byteLen)
      throw runtime_error("bad block: eb size not byteLen");
    if ((unsigned char)eb[0] != 0x00)
      throw runtime_error("bad block: first element not 0x00");
    if ((unsigned char)eb[1] != 0x01)
      throw runtime_error("bad block: second element isn't 0x01");

    size_t idx = 2;
    // look for 00 after PS
    for (; idx < eb.size() && (unsigned char)eb[idx] != 0x00; ++idx)
      ;
    if (idx >= eb.size() || (unsigned char)eb[idx] != 0x00) {
      throw runtime_error("bad padding: no separator found after PS");
    }

    if (idx < 10) { // PS must be at least 8 bytes (so idx >= 10)
      throw runtime_error("bad padding: PS too short");
    }

    message += eb.substr(idx + 1); // get everyting after padding
  }
  printMessage(message);

  return 0;
}
