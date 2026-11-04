#ifndef CRYPTOGRAPHER_HPP
#define CRYPTOGRAPHER_HPP

#include <string>

using namespace std;

class cryptographer {
public:
    string encryptVigenere(const string& alphaKey, const string& clearText) const;
    string decryptVigenere(const string& alphaKey, const string& cipherText) const;
    string encryptNumber(int numberKey, const string& clearText) const;
    string decryptNumber(int numberKey, const string& cipherText) const;

private:
    static void validateAlphaKey(const string& alphaKey);
    static string numberKeyToString(int numberKey);
    static void validateText(const string& text);
    static char shiftLetter(char character, int amount);
};

// Conventional alias used by the rest of the project while preserving the
// assignment's exact lowercase class name: cryptographer.
using Cryptographer = cryptographer;

#endif
