#include "Cryptographer.hpp"
#include "DefaultPassword.hpp"
#include "Validator.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>

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

template <typename Operation>
bool rejectsInvalidArgument(const Operation& operation) {
    try {
        operation();
    } catch (const invalid_argument&) {
        return true;
    }

    return false;
}

string readFile(const filesystem::path& path) {
    ifstream input(path);
    return string(istreambuf_iterator<char>(input),
                  istreambuf_iterator<char>());
}

filesystem::path uniqueTempPath(const string& suffix) {
    const auto timestamp = chrono::steady_clock::now()
                               .time_since_epoch()
                               .count();
    return filesystem::temp_directory_path() /
           ("cen4078_" + to_string(timestamp) + suffix);
}

} // namespace

int main() {
    const Cryptographer cryptographer;
    const Validator validator;
    const DefaultPassword defaultPassword;
    int failureCount = 0;

    // Alphabetic Vigenere: expected output, case preservation, and invalid input.
    const string alphaCipher = cryptographer.encryptVigenere("ABC", "AbcZ");
    check(alphaCipher == "AceZ", "TC-CR-01 mixed-case alphabetic encryption", failureCount);
    check(cryptographer.decryptVigenere("ABC", alphaCipher) == "AbcZ", "TC-CR-02 alphabetic decryption round trip", failureCount);
    check(cryptographer.encryptVigenere("ARGOSROCK", "AbcZ") == "AsiN", "TC-CR-03 required alphabetic key encryption", failureCount);
    check(cryptographer.decryptVigenere("ARGOSROCK", "AsiN") == "AbcZ", "TC-CR-04 required alphabetic key decryption", failureCount);
    check(rejectsInvalidArgument([&] {
              cryptographer.encryptVigenere("", "AbcZ");
          }),
          "TC-CR-05 empty alphabetic key rejected",
          failureCount);
    check(rejectsInvalidArgument([&] {
              cryptographer.encryptVigenere("ABC", "Abc!");
          }),
          "TC-CR-06 disallowed cryptography input rejected",
          failureCount);

    // Numeric Vigenere: verify the supplied table and invalid boundary values.
    const string numberCipher = cryptographer.encryptNumber(numberKey, "0123456789");
    check(numberCipher == "9260360470", "TC-CR-07 numeric encryption follows the supplied table", failureCount);
    check(cryptographer.decryptNumber(numberKey, numberCipher) == "0123456789", "TC-CR-08 numeric decryption round trip", failureCount);
    check(rejectsInvalidArgument([&] {
              cryptographer.encryptNumber(-1, "1234");
          }),
          "TC-CR-09 negative numeric key rejected",
          failureCount);
    check(rejectsInvalidArgument([&] {
              cryptographer.decryptNumber(numberKey, "");
          }),
          "TC-CR-10 empty cipher input rejected",
          failureCount);

    // Required credential comparisons, using an in-memory encrypted record.
    const string inputUsername = "scientist";
    const string inputPassword = "LabSecure1";
    const string storedUsername = encryptCredential(cryptographer, inputUsername);
    const string storedPassword = encryptCredential(cryptographer, inputPassword);

    check(encryptCredential(cryptographer, inputUsername) == storedUsername &&
              encryptCredential(cryptographer, inputPassword) == storedPassword,
          "TC-CRED-01 encrypted input matches stored ciphertext",
          failureCount);
    check(decryptCredential(cryptographer, storedUsername) == inputUsername &&
              decryptCredential(cryptographer, storedPassword) == inputPassword,
          "TC-CRED-02 decrypted stored credentials match cleartext input",
          failureCount);

    // Allowlist and password policy: positive, negative, and length boundaries.
    check(validator.isSafeFromSqlInjection("User123"), "TC-VAL-01 allowed username characters accepted", failureCount);
    check(!validator.isSafeFromSqlInjection("User123!"), "T1 PE2-VAL-001 User123! contains a disallowed character", failureCount);
    check(!validator.meetsPasswordPolicy("password"), "T2 PE2-VAL-002 lowercase-only password is rejected", failureCount);
    check(validator.meetsPasswordPolicy("Password1"), "T3 PE2-VAL-002 policy-compliant password is accepted", failureCount);
    check(!validator.meetsPasswordPolicy("invalidpassword"), "T4 PE2-VAL-002 overlength password is rejected", failureCount);
    check(validator.meetsPasswordPolicy("ValidPass1"), "TC-PW-01 another valid password is accepted", failureCount);
    check(validator.meetsPasswordPolicy("Abcdef1X"), "TC-PW-08 eight-character lower boundary accepted", failureCount);
    check(validator.meetsPasswordPolicy("Abcdefgh1XYZ"), "TC-PW-12 twelve-character upper boundary accepted", failureCount);
    check(!validator.meetsPasswordPolicy("Abcde1X"), "TC-PW-07 seven-character password rejected", failureCount);
    check(!validator.meetsPasswordPolicy("Abcdefgh1XYZ0"), "TC-PW-13 thirteen-character password rejected", failureCount);
    check(!validator.meetsPasswordPolicy("abcdefg1"), "TC-PW-UP missing uppercase rejected", failureCount);
    check(!validator.meetsPasswordPolicy("ABCDEFG1"), "TC-PW-LOW missing lowercase rejected", failureCount);
    check(!validator.meetsPasswordPolicy("Abcdefgh"), "TC-PW-DIG missing digit rejected", failureCount);
    check(!validator.meetsPasswordPolicy("Abcdef!1"), "TC-PW-CHAR disallowed password character rejected", failureCount);
    check(validator.getPasswordPolicy().find("8-12 characters") != string::npos,
          "TC-POL-01 policy method describes the length rule",
          failureCount);
    check(validator.getPasswordPolicy().find("uppercase letter") != string::npos &&
              validator.getPasswordPolicy().find("lowercase letter") != string::npos &&
              validator.getPasswordPolicy().find("number") != string::npos,
          "TC-POL-02 policy method describes character classes",
          failureCount);
    check(validator.meetsPasswordPolicy("Password1"),
          "TC-POL-03 centralized validator accepts a policy-compliant value",
          failureCount);
    check(!validator.meetsPasswordPolicy("Abcdefgh"),
          "TC-POL-04 centralized validator rejects a value missing a digit",
          failureCount);

    // MFA integer boundaries: verify overflow and malformed input are rejected.
    int token = 0;
    check(validator.isValidMfaToken("1234567890", token) && token == 1234567890,
          "TC-MFA-01 valid ten-digit MFA input accepted",
          failureCount);
    check(validator.isValidMfaToken("2147483647", token) && token == 2147483647,
          "TC-MFA-02 signed-int maximum MFA token accepted",
          failureCount);
    check(!validator.isValidMfaToken("2147483648", token),
          "TC-MFA-03 first overflowing MFA value rejected",
          failureCount);
    check(!validator.isValidMfaToken("9999999999", token),
          "TC-MFA-04 required 9999999999 overflow case rejected",
          failureCount);
    check(!validator.isValidMfaToken("0234567890", token),
          "TC-MFA-05 MFA value with leading zero rejected",
          failureCount);

    // Default-password generation should produce policy-compliant 12-char values.
    bool generatedPasswordsValid = true;
    for (int test = 0; test < 100; ++test) {
        const string generated = defaultPassword.createRandomPassword(validator);
        generatedPasswordsValid = generatedPasswordsValid &&
                                  generated.length() == 12 &&
                                  validator.meetsPasswordPolicy(generated);
    }
    check(generatedPasswordsValid,
          "TC-DEF-01 generated default passwords meet policy and length",
          failureCount);

    // Delivery success and failure paths use only temporary test files.
    const string generatedPassword = defaultPassword.createRandomPassword(validator);
    const filesystem::path deliveryPath = uniqueTempPath("_email.txt");
    const bool deliverySucceeded =
        defaultPassword.sendSecureEmailSimulation(generatedPassword, deliveryPath);
    check(deliverySucceeded,
          "TC-DEF-02 delivery simulation succeeds for a writable file",
          failureCount);

    const string deliveryContents = readFile(deliveryPath);
    // This security assertion is expected to fail on the current implementation:
    // the simulation writes the generated password in cleartext to the file.
    check(deliveryContents.find(generatedPassword) == string::npos,
          "TC-DEF-03 generated password is not written in cleartext",
          failureCount);

    ostringstream notificationOutput;
    streambuf* originalOutput = cout.rdbuf(notificationOutput.rdbuf());
    defaultPassword.notifyUser(deliverySucceeded);
    cout.rdbuf(originalOutput);
    check(notificationOutput.str().find("default password") != string::npos &&
              notificationOutput.str().find(generatedPassword) == string::npos,
          "TC-DEF-04 notification reports default setup without exposing password",
          failureCount);

    const filesystem::path missingParent = uniqueTempPath("_missing_parent");
    const filesystem::path failedDeliveryPath = missingParent / "email.txt";
    check(!defaultPassword.sendSecureEmailSimulation(generatedPassword,
                                                     failedDeliveryPath),
          "TC-DEF-05 delivery failure is reported for an unavailable path",
          failureCount);

    error_code cleanupError;
    filesystem::remove(deliveryPath, cleanupError);
    cleanupError.clear();
    filesystem::remove_all(missingParent, cleanupError);

    if (failureCount != 0) {
        cerr << failureCount << " test(s) failed.\n";
        return 1;
    }

    cout << "All tests passed.\n";
    return 0;
}
