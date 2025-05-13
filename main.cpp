#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

// Improved hash function to reduce collisions (using base = 3)
int HashString(const string& value) {
    int hashCode = 0;
    int base = 3; // Changed base from 31 to 3 for the hash calculation
    for (char c : value) {
        hashCode = hashCode * base + c; // Multiply current total by base and add character
    }
    return (int)(hashCode & 0x7fffffff); // Ensure positive value
}

int main() {
    unordered_map<int, vector<string>> passwordTable; // Handles collisions using a vector
    ifstream passwordFile("10-million-password-list-top-10000.txt"); // Replace with the actual password file

    if (!passwordFile) {
        cerr << "Error: Password file not found!" << endl;
        return 1;
    }

    int collisionCount = 0; // Count how many times a hash already exists

    string password;
    cout << "Reading passwords and hashing...\n";
    while (getline(passwordFile, password)) {
        int hashValue = HashString(password);

        // Check if hashValue already has at least one password stored
        if (!passwordTable[hashValue].empty()) {
            collisionCount++; // This is a collision
        }

        passwordTable[hashValue].push_back(password); // Store in vector to handle collisions
    }
    passwordFile.close();

    cout << "Hashing complete. Total collisions: " << collisionCount << endl;
    cout << "Enter a hashed password to find the original: ";

    int userHash;
    while (true) {
        cout << "\nEnter hashed value (or -1 to exit): ";
        cin >> userHash;

        if (userHash == -1) break;

        if (passwordTable.find(userHash) != passwordTable.end()) {
            cout << "Possible original passwords:\n";
            for (const string& pass : passwordTable[userHash]) {
                cout << " - " << pass << endl;
            }
        } else {
            cout << "Password not found in database." << endl;
        }
    }

    cout << "Goodbye!" << endl;
    return 0;
}