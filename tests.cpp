#include "Cryptographer.hpp"
#include "DefaultPassword.hpp"
#include "Validator.hpp"

#include <iostream>
#include <string>

using namespace std;

namespace {

constexpr char alphaKey[] = "ARGOSROCK";
constexpr int numberKey = 1963;

string encryptCredential(const Cryptographer& cryptographer,
                              const string& clearText) {
    return cryptographer.encryptNumber(
        numberKey,
        cryptographer.encryptVigenere(alphaKey, clearText));
}

string decryptCredential(const Cryptographer& cryptographer,
                              const string& cipherText) {
    return cryptographer.decryptVigenere(
        alphaKey,
        cryptographer.decryptNumber(numberKey, cipherText));
}

void check(bool condition,
           const string& description,
           int& failureCount) {
    if (!condition) {
        cerr << "FAILED: " << description << '\n';
        ++failureCount;
    }
}

} // namespace

int main() {
    const Cryptographer cryptographer;
    const Validator validator;
    const DefaultPassword defaultPassword;
    int failureCount = 0;

    const string alphaCipher = cryptographer.encryptVigenere("ABC", "AbcZ");
    check(alphaCipher == "AceZ", "alphabetic Vigenere encryption preserves case", failureCount);
    check(cryptographer.decryptVigenere("ABC", alphaCipher) == "AbcZ", "alphabetic Vigenere decryption restores cleartext", failureCount);

    const string numberCipher = cryptographer.encryptNumber(numberKey, "0123456789");
    check(numberCipher == "9260360470", "numeric encryption follows the supplied number table", failureCount);
    check(cryptographer.decryptNumber(numberKey, numberCipher) == "0123456789", "numeric decryption restores cleartext", failureCount);

    const string inputUsername = "scientist";
    const string inputPassword = "LabSecure1";
    const string storedUsername = encryptCredential(cryptographer, inputUsername);
    const string storedPassword = encryptCredential(cryptographer, inputPassword);

    // Minimum test 1: encrypt cleartext input and compare encrypted values.
    check(encryptCredential(cryptographer, inputUsername) == storedUsername &&
        encryptCredential(cryptographer, inputPassword) == storedPassword, "encrypted input matches encrypted stored credentials", failureCount);

    // Minimum test 2: decrypt stored values and compare cleartext values.
    check(decryptCredential(cryptographer, storedUsername) == inputUsername 
        && decryptCredential(cryptographer, storedPassword) == inputPassword, "decrypted stored credentials match cleartext input", failureCount);

    check(validator.meetsPasswordPolicy("ValidPass1"), "valid alphanumeric password is accepted", failureCount);
    check(!validator.meetsPasswordPolicy("Invalid!1A"), "non-alphanumeric password is rejected", failureCount);

    int token = 0;
    check(validator.isValidMfaToken("1234567890", token), "valid signed-int MFA token is accepted", failureCount);
    check(!validator.isValidMfaToken("9999999999", token), "overflowing MFA token is rejected", failureCount);

    for (int test = 0; test < 100; ++test) {
        check(validator.meetsPasswordPolicy(
                  defaultPassword.createRandomPassword(validator)),
              "generated default password meets the policy",
              failureCount);
    }

    if (failureCount != 0) {
        cerr << failureCount << " test(s) failed.\n";
        return 1;
    }

    cout << "All tests passed.\n";
    return 0;
}
