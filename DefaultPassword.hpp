#ifndef DEFAULT_PASSWORD_HPP
#define DEFAULT_PASSWORD_HPP

#include <filesystem>
#include <string>

class Validator;

using namespace std;

// C++ identifiers cannot contain a hyphen, so the required "default-password"
// class is represented by the valid C++ class name DefaultPassword.
class DefaultPassword {
public:
    string createRandomPassword(const Validator& validator) const;

    bool sendSecureEmailSimulation(
        const string& password,
        const filesystem::path& outputFile) const;

    void notifyUser(bool deliverySucceeded) const;
};

#endif
