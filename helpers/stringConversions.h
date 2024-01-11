#include <string>

// Return true if the input string can safely be turned into an integer, false if not
bool canstoi(std::string input) {
    try {
        int temp = stoi(input);
    } catch (...) {
        return false;
    }

    return true;
}
// Return true if the input string can safely be turned into a double, false if not
bool canstod(std::string input) {
    try {
        double temp = stod(input);
    } catch (...) {
        return false;
    }

    return true;
}

// Output an integer as a string (identically to std::to_string())
std::string tostring(int input) {
    return std::to_string(input);
}
// Output a double as a string, all extra zeros and decimal points are removed (unlike when using std::to_string())
std::string tostring(double input) {
    std::string output = std::to_string(input);

    // Remove any extra zeros and decimal points if need be
    for (unsigned int i = output.length() - 1; i >= 0; i--) {
        if (output[i] == '0') {
            output.erase(i, 1);
        }
        else if (output[i] == '.') {
            output.erase(i, 1);
            break;
        }
        else {
            break;
        }
    }

    return output;
}
// Output a character as a string (as opposed to a number when using std::to_string())
std::string tostring(char input) {
    std::string output = "-";
    output[0] = input;
    return output;
}
// Output a boolean as a string (true/false)
std::string tostring(bool input) {
    return (input) ? "true" : "false";
}

// Change an entire string to only contain lowercase characters
std::string tlower(std::string input) {
    std::string output;

    for (unsigned int i = 0; i < input.length(); i++) {
        output += tolower(input[i]);
    }

    return output;
}
// Change an entire string to only contain uppercase characters
std::string tupper(std::string input) {
    std::string output;

    for (unsigned  i = 0; i < input.length(); i++) {
        output += toupper(input[i]);
    }

    return output;
}
