#include "DefaultPassword.hpp"

#include "Validator.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <system_error>

using namespace std;

string DefaultPassword::createRandomPassword(
    const Validator& validator) const {
    static constexpr char uppercase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static constexpr char lowercase[] = "abcdefghijklmnopqrstuvwxyz";
    static constexpr char numbers[] = "0123456789";
    static constexpr char allCharacters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    static constexpr size_t passwordLength = 12;

    random_device randomDevice;
    mt19937 generator(randomDevice());

    const auto randomCharacter = [&generator](const char* characters, size_t count) {
        uniform_int_distribution<size_t> distribution(0, count - 1);
        return characters[distribution(generator)];
    };

    string password;
    password.reserve(passwordLength);
    password.push_back(randomCharacter(uppercase, sizeof(uppercase) - 1));
    password.push_back(randomCharacter(lowercase, sizeof(lowercase) - 1));
    password.push_back(randomCharacter(numbers, sizeof(numbers) - 1));

    while (password.length() < passwordLength) {
        password.push_back(randomCharacter(allCharacters, sizeof(allCharacters) - 1));
    }

    shuffle(password.begin(), password.end(), generator);

    // This defensive check covers any future change to the generation logic.
    if (!validator.meetsPasswordPolicy(password)) {
        return createRandomPassword(validator);
    }

    return password;
}

bool DefaultPassword::sendSecureEmailSimulation(
    const string& password,
    const filesystem::path& outputFile) const {
    ofstream output(outputFile, ios::trunc);
    if (!output) {
        return false;
    }

    output << "Secure email simulation\n"
           << "Your temporary password is: " << password << '\n'
           << "Change this password after your next login.\n";
    output.close();

    if (!output) {
        return false;
    }

    error_code error;
    filesystem::permissions(
        outputFile,
        filesystem::perms::owner_read |
            filesystem::perms::owner_write,
        filesystem::perm_options::replace,
        error);

    return !error;
}

void DefaultPassword::notifyUser(bool deliverySucceeded) const {
    if (deliverySucceeded) {
        cout << "A default password was created. A secure email "
                     "simulation containing it was delivered.\n";
    } else {
        cout << "A default password was created, but its secure "
                     "delivery could not be completed.\n";
    }
}
