#include "Cryptographer.hpp"
#include "DefaultPassword.hpp"
#include "Validator.hpp"

#include <array>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <conio.h>
#include <io.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;

namespace {

struct User {
    string encryptedUsername;
    string encryptedPassword;
    int mfaToken;
};
constexpr char alphaKey[] = "ARGOSROCK";
constexpr int numberKey = 1963;

string encryptCredential(const Cryptographer& cryptographer,
                              const string& clearText) {
    const string letterCipher =
        cryptographer.encryptVigenere(alphaKey, clearText);
    return cryptographer.encryptNumber(numberKey, letterCipher);
}

string decryptCredential(const Cryptographer& cryptographer,
                              const string& cipherText) {
    const string numberClearText =
        cryptographer.decryptNumber(numberKey, cipherText);
    return cryptographer.decryptVigenere(alphaKey, numberClearText);
}

array<User, 3> createEncryptedUserDatabase(
    const Cryptographer& cryptographer) {
    return {{{encryptCredential(cryptographer, "scientist"),
              encryptCredential(cryptographer, "LabSecure1"),
              1234567890},
             {encryptCredential(cryptographer, "engineer"),
              encryptCredential(cryptographer, "BuildSafe2"),
              1987654321},
             {encryptCredential(cryptographer, "security"),
              encryptCredential(cryptographer, "GuardMode3"),
              2040608010}}};
}

bool saveCredentialDatabase(const array<User, 3>& users,
                            const filesystem::path& filePath) {
    ofstream output(filePath, ios::trunc);
    if (!output) {
        return false;
    }

    for (const User& user : users) {
        output << user.encryptedUsername << ' '
               << user.encryptedPassword << '\n';
    }

    output.close();
    if (!output) {
        return false;
    }

    // Restrict the demonstration credential file to its owner where supported.
    error_code error;
    filesystem::permissions(
        filePath,
        filesystem::perms::owner_read |
            filesystem::perms::owner_write,
        filesystem::perm_options::replace,
        error);

    return !error;
}

bool loadCredentialDatabase(const filesystem::path& filePath, array<User, 3>& users, const Cryptographer& cryptographer) {

    ifstream input(filePath);
    if (!input) {
        return false;
    }

    array<User, 3> loadedUsers = users;
    for (size_t index = 0; index < loadedUsers.size(); ++index) {
        string encryptedUsername;
        string encryptedPassword;
        if (!(input >> encryptedUsername >> encryptedPassword)) {
            return false;
        }

        try {
            const string expectedUsername = decryptCredential(cryptographer, users[index].encryptedUsername);
            const string loadedUsername = decryptCredential(cryptographer, encryptedUsername);

            if (loadedUsername != expectedUsername) {
                return false;
            }
        } catch (const exception&) {
            return false;
        }

        loadedUsers[index].encryptedUsername = encryptedUsername;
        loadedUsers[index].encryptedPassword = encryptedPassword;
    }

    string unexpectedData;
    if (input >> unexpectedData) {
        return false;
    }

    users = loadedUsers;
    return true;
}

string readHiddenPassword(const string& prompt) {
    cout << prompt << flush;

#ifdef _WIN32
    if (_isatty(_fileno(stdin)) == 0) {
        string password;
        getline(cin, password);
        return password;
    }

    string password;
    int character = 0;
    while ((character = _getch()) != '\r' && character != '\n') {
        if ((character == '\b' || character == 127) && !password.empty()) {
            password.pop_back();
        } else if (character >= 32 && character <= 126) {
            password.push_back(static_cast<char>(character));
        }
    }
    cout << '\n';
    return password;
#else
    if (isatty(STDIN_FILENO) == 0) {
        string password;
        getline(cin, password);
        return password;
    }

    termios oldSettings{};
    if (tcgetattr(STDIN_FILENO, &oldSettings) != 0) {
        string password;
        getline(cin, password);
        return password;
    }

    termios hiddenSettings = oldSettings;
    hiddenSettings.c_lflag &= static_cast<tcflag_t>(~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &hiddenSettings);

    string password;
    getline(cin, password);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
    cout << '\n';
    return password;
#endif
}

User* findMatchingUser(array<User, 3>& users, const Cryptographer& cryptographer, const string& username, const string& password, int mfaToken) {

    const string encryptedInputUsername = encryptCredential(cryptographer, username);
    const string encryptedInputPassword = encryptCredential(cryptographer, password);

    for (User& user : users) {
        const bool encryptedValuesMatch = user.encryptedUsername == encryptedInputUsername && user.encryptedPassword == encryptedInputPassword;

        const bool decryptedValuesMatch = decryptCredential(cryptographer, user.encryptedUsername) == username 
        && decryptCredential(cryptographer, user.encryptedPassword) == password;

        if (encryptedValuesMatch && decryptedValuesMatch && user.mfaToken == mfaToken) {
            return &user;
        }
    }

    return nullptr;
}

bool offerPasswordUpdate(User& user, array<User, 3>& users, const Validator& validator, const Cryptographer& cryptographer, const filesystem::path& databaseFile) {
   
    cout << "Create a new password for this account? (y/n): ";
    string answer;
    if (!getline(cin, answer)) {
        return false;
    }

    if (answer != "y" && answer != "Y") {
        return true;
    }

    cout << "Password policy: " << validator.getPasswordPolicy() << '\n';
    constexpr int passwordAttempts = 2;

    for (int attempt = 1; attempt <= passwordAttempts; ++attempt) {
        const string newPassword =
            readHiddenPassword("New password: ");

        if (validator.isSafeFromSqlInjection(newPassword) &&
            validator.meetsPasswordPolicy(newPassword)) {
            user.encryptedPassword =
                encryptCredential(cryptographer, newPassword);

            if (!saveCredentialDatabase(users, databaseFile)) {
                return false;
            }

            cout << "Password updated successfully.\n";
            return true;
        }

        cout << "Password creation failed. Please use the stated policy.\n";
    }

    const DefaultPassword defaultPassword;
    const string generatedPassword = defaultPassword.createRandomPassword(validator);
    user.encryptedPassword = encryptCredential(cryptographer, generatedPassword);

    if (!saveCredentialDatabase(users, databaseFile)) {
        return false;
    }

    const bool deliverySucceeded = defaultPassword.sendSecureEmailSimulation(generatedPassword, "secure_email.txt");
    defaultPassword.notifyUser(deliverySucceeded);
    return deliverySucceeded;
}

}

int main() {
    const filesystem::path databaseFile = "credentials.txt";
    const Validator validator;
    const Cryptographer cryptographer;

    array<User, 3> users =
        createEncryptedUserDatabase(cryptographer);
    if (!loadCredentialDatabase(databaseFile, users, cryptographer) &&
        !saveCredentialDatabase(users, databaseFile)) {
        cerr << "The application could not initialize securely.\n";
        return 1;
    }

    constexpr int maximumAttempts = 3;

    cout << "Secure Login\n";
    cout << "------------\n";

    for (int attempt = 1; attempt <= maximumAttempts; ++attempt) {
        string username;
        cout << "Username: ";
        if (!getline(cin, username)) {
            cerr << "Input could not be read.\n";
            return 1;
        }

        const string password = readHiddenPassword("Password: ");

        string mfaInput;
        cout << "10-digit MFA token: ";
        if (!getline(cin, mfaInput)) {
            cerr << "Input could not be read.\n";
            return 1;
        }

        int mfaToken = 0;
        const bool validInput =
            validator.isSafeFromSqlInjection(username) &&
            validator.isSafeFromSqlInjection(password) &&
            validator.meetsPasswordPolicy(password) &&
            validator.isValidMfaToken(mfaInput, mfaToken);

        User* authenticatedUser = nullptr;
        if (validInput) {
            authenticatedUser = findMatchingUser(
                users, cryptographer, username, password, mfaToken);
        }

        if (authenticatedUser != nullptr) {
            cout << "Welcome, " << username << "!\n";

            if (!offerPasswordUpdate(*authenticatedUser, users, validator, cryptographer, databaseFile)) {
                cerr << "The requested operation could not be completed "
                             "securely.\n";
                return 1;
            }

            return 0;
        }

        // Keep the message generic so attackers cannot learn which field failed.
        cout << "Login failed. Please check your credentials.\n";
    }

    cout << "Too many failed attempts. Access denied.\n";
    return 1;
}
