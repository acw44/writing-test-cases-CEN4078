#include "Cryptographer.hpp"

#include <algorithm>
#include <cctype>
#include <stdexcept>

using namespace std; 

void cryptographer::validateAlphaKey(const string& alphaKey) {
    if (alphaKey.empty() || !all_of(alphaKey.begin(), alphaKey.end(), [](unsigned char character) { return isalpha(character) != 0; })){
            throw invalid_argument("The alphabetic key is invalid.");
    }
}

string cryptographer::numberKeyToString(int numberKey) {
    if (numberKey < 0) {
        throw invalid_argument("The numeric key is invalid.");
    }

    return to_string(numberKey);
}

void cryptographer::validateText(const string& text) {
    if (text.empty() ||
        !all_of(text.begin(), text.end(), [](unsigned char character) { return isalnum(character) != 0; })) {
        throw invalid_argument("The cryptography input is invalid.");
    }
}

char cryptographer::shiftLetter(char character, int amount) {
    const char base = isupper(static_cast<unsigned char>(character)) != 0 ? 'A' : 'a';
    const int position = character - base;
    const int shiftedPosition = (position + amount + 26) % 26;
    return static_cast<char>(base + shiftedPosition);
}

string cryptographer::encryptVigenere(const string& alphaKey, const string& clearText) const {
    validateAlphaKey(alphaKey);
    validateText(clearText);

    string cipherText = clearText;
    size_t keyIndex = 0;

    for (size_t index = 0; index < clearText.length(); ++index) {
        const unsigned char character = static_cast<unsigned char>(clearText[index]);

        if (isalpha(character) != 0) {
            const unsigned char keyCharacter = static_cast<unsigned char>(alphaKey[keyIndex % alphaKey.length()]);
            const int shift = toupper(keyCharacter) - 'A';
            cipherText[index] = shiftLetter(clearText[index], shift);
            ++keyIndex;
        }
    }

    return cipherText;
}

string cryptographer::decryptVigenere(const string& alphaKey, const string& cipherText) const {
    validateAlphaKey(alphaKey);
    validateText(cipherText);

    string clearText = cipherText;
    size_t keyIndex = 0;

    for (size_t index = 0; index < cipherText.length(); ++index) {
        const unsigned char character =
            static_cast<unsigned char>(cipherText[index]);

        if (isalpha(character) != 0) {
            const unsigned char keyCharacter = static_cast<unsigned char>(alphaKey[keyIndex % alphaKey.length()]);
            const int shift = toupper(keyCharacter) - 'A';
            clearText[index] = shiftLetter(cipherText[index], -shift);
            ++keyIndex;
        }
    }

    return clearText;
}

string cryptographer::encryptNumber(int numberKey,const string& clearText) const {
    const string key = numberKeyToString(numberKey);
    validateText(clearText);

    string cipherText = clearText;
    size_t keyIndex = 0;

    for (size_t index = 0; index < clearText.length(); ++index) {
        const unsigned char character =
            static_cast<unsigned char>(clearText[index]);

        if (isdigit(character) != 0) {
            const int clearDigit = clearText[index] - '0';
            const int keyDigit = key[keyIndex % key.length()] - '0';

            // The assignment's table shifts the clear digit left by the key.
            const int cipherDigit = (clearDigit - keyDigit + 10) % 10;
            cipherText[index] = static_cast<char>('0' + cipherDigit);
            ++keyIndex;
        }
    }

    return cipherText;
}

string cryptographer::decryptNumber(int numberKey, const string& cipherText) const {
    const string key = numberKeyToString(numberKey);
    validateText(cipherText);

    string clearText = cipherText;
    size_t keyIndex = 0;

    for (size_t index = 0; index < cipherText.length(); ++index) {
        const unsigned char character =
            static_cast<unsigned char>(cipherText[index]);

        if (isdigit(character) != 0) {
            const int cipherDigit = cipherText[index] - '0';
            const int keyDigit = key[keyIndex % key.length()] - '0';
            const int clearDigit = (cipherDigit + keyDigit) % 10;
            clearText[index] = static_cast<char>('0' + clearDigit);
            ++keyIndex;
        }
    }

    return clearText;
}
