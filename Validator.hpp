#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include <string>

using namespace std;

class Validator {
public:
    // Returns false when input contains characters commonly used to alter SQL.
    bool isSafeFromSqlInjection(const string& input) const;

    // Enforces the assignment's 8-12 character password policy.
    bool meetsPasswordPolicy(const string& password) const;

    string getPasswordPolicy() const;

    // Accepts exactly ten digits that fit in a C++ signed int.
    bool isValidMfaToken(const string& input, int& token) const;
};

#endif
