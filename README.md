# RSA Encryption

RSA encryption implementation to practice it idk

## Requirements
- Linux/macOS
- GMP library
  - Arch/Alpine: ``gmp``
  - Debian/Ubuntu: ``libgmp-dev``
- C++17 compiler (``g++``)

## Installation
### From Source
```bash
git clone https://github.com/maximeNoiret/rsa
cd rsa
make
```

## Usage
```bash
# Generate keys
./rsa keygen [bits] [filename]

# Encrypt
./rsa encrypt <keyfile.pub> <message>

# Decrypt
./rsa decrypt <keyfile> <ciphertext>
```

## Windows Support
lol no, use WSL or, better yet, install Linux.

## Features
- Supports up to 16384-bit keys (maybe more)
- Parallel prime generation
- PKCS#1 v1.5 padding
- File chunking for large messages
- Proper key generation with 65537 as default public exponent

## Note
This is a learning project. Don't use this for anything important unless you're an idiot. I'm not responsible if you get bommed.
