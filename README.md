# RSA Encryption

RSA encryption implementation to practice it idk



## Installation
### From Source
#### Requirements
- GMP library
  - Arch/Alpine: ``gmp``
  - Debian/Ubuntu: ``libgmp-dev``
- C++11 compiler (``g++``)

```bash
git clone https://github.com/maximeNoiret/rsa
cd rsa
make
```

### Binaries
Compiled binaries are available in [here](https://github.com/maximeNoiret/rsa/releases/latest).
> [!NOTE]
> Binaries may not be up to date, not that it matters much. \
> Using a compiled binary is the recommended method for Windows. idc if the executable is 3MB. \
> Also linux executables prolly won't be updated much since you can literally just build it.

## Usage
```bash
# Generate keys
./rsa keygen [bits] [filename]

# Encrypt
./rsa encrypt <keyfile.pub> <message>

# Decrypt
./rsa decrypt <keyfile> <ciphertext>
```

## Features
- Supports up to 65536-bit keys (maybe more)
  - Generating a 32768-bit key pair took ~5 hours on a Raspberry Pi 4.
  - Generating a 65536-bit key pair took ~4 days on a Raspberry Pi 4.
- Parallel prime generation
- PKCS#1 v1.5 padding
- File chunking for large messages
- Proper key generation with 65537 as default public exponent
- Encryption and Decryption supports piping using '-' in place of message

## Note
This is a learning project. Don't use this for anything important unless you're an idiot. I'm not responsible if you get bommed.
