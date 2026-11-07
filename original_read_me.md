
# Secure Login Project V2.0

This updated C++ program expands upon the login system by implementing Vigenere encryption/decryption.
This will include functions that encrypt/decrypt according to a set key. This implementation is important
to keeping passwords safe from potential hackers. This project was created by Jerome Ball on October 4th, 2026.


## Requirements covered

- The alphabetic key is `ARGOSROCK` and the numeric key is `1963`.
- Alphabetic encryption preserves uppercase and lowercase letters.
- Numeric encryption follows the supplied numerical Vigenere table.
- Credentials contain only `A-Z`, `a-z`, and `0-9`.
- Passwords are 8-12 characters and contain at least one uppercase letter, one
  lowercase letter, and one number.
- The user array and `credentials.txt` store encrypted usernames and passwords.
- Login performs both required comparisons:
  1. Encrypt entered cleartext and compare it with stored ciphertext.
  2. Decrypt stored ciphertext and compare it with entered cleartext.
- After a successful login, an authenticated user may create a new password.
- The user receives two attempts to meet the returned password policy.
- After two invalid attempts, `DefaultPassword` generates a compliant random
  password and simulates secure delivery without displaying it in the console.
- Login failures remain generic and do not reveal which field was incorrect.

The requirement calls the fallback class `default-password`. Because a hyphen
is not permitted in a C++ identifier, the source uses `DefaultPassword` and
documents that mapping in the class header and configuration-control file.

## Test accounts

| Username | Password | MFA token |
| --- | --- | ---: |
| scientist | LabSecure1 | 1234567890 |
| engineer | BuildSafe2 | 1987654321 |
| security | GuardMode3 | 2040608010 |

Each password is 8-12 characters and contains an uppercase letter, a lowercase
letter, and a number. The SQL-injection method rejects `/`, `-`, `;`, `"`, and
the single quote. The ten-digit tokens fit within the C++ signed `int` maximum
of 2,147,483,647.

## Build and run on Linux with Make

Open a terminal in the extracted `ProgrammingExercise2` folder and run:

```bash
make
./secure_login
```

Run the automated tests:

```bash
make test
```

To remove the compiled program later:

```bash
make clean
```

## Build and run on Linux with CMake

```bash
cmake -S . -B build
cmake --build build
./build/bin/secure_login
```
