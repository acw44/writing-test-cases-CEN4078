#include "Validator.hpp"

#include <algorithm>
#include <charconv>
#include <cctype>
#include <system_error>

using namespace std;

bool Validator::isSafeFromSqlInjection(const string& input) const {
   
    if (input.empty()) {
        return false;
    }

    return all_of(input.begin(), input.end(), [](unsigned char character) {
        return isalnum(character) != 0;
    });
}

bool Validator::meetsPasswordPolicy(const string& password) const {
    if (password.length() < 8 || password.length() > 12) {
        return false;
    }

    const bool isAlphanumeric = all_of(password.begin(), password.end(), [](unsigned char character) { return isalnum(character) != 0; });
    const bool hasUppercase = any_of(password.begin(), password.end(), [](unsigned char character) { return isupper(character) != 0; });
    const bool hasLowercase = any_of(password.begin(), password.end(), [](unsigned char character) { return islower(character) != 0; });
    const bool hasNumber = any_of(password.begin(), password.end(), [](unsigned char character) { return isdigit(character) != 0; });

    return isAlphanumeric && hasUppercase && hasLowercase && hasNumber;
}

string Validator::getPasswordPolicy() const {
   
    return "8-12 characters using only A-Z, a-z, and 0-9, with at least "
           "one uppercase letter, one lowercase letter, and one number.";
}

bool Validator::isValidMfaToken(const string& input, int& token) const {
   
    if (input.length() != 10 || input.front() == '0') {
        return false;
    }

    if (!all_of(input.begin(), input.end(), [](unsigned char character) {
            return isdigit(character) != 0;
        })) {
        return false;
    }

    int parsedToken = 0;
    const char* first = input.data();
    const char* last = input.data() + input.size();
    const auto result = from_chars(first, last, parsedToken);

    if (result.ec != errc{} || result.ptr != last) {
        return false;
    }

    // A ten-digit value that fits in a signed int must be at least 1 billion.
    if (parsedToken < 1'000'000'000) {
        return false;
    }

    token = parsedToken;
    return true;
}
