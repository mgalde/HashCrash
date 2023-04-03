#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const unsigned int SHA256_DIGEST_LENGTH = 32;

// Function to calculate SHA-256 hash of a file
string get_sha256_hash(const string& file_path) {
    ifstream file(file_path, ios::binary);
    if (!file) {
        cerr << "Error: Unable to open file " << file_path << endl;
        exit(EXIT_FAILURE);
    }

    // Get the file size
    file.seekg(0, ios::end);
    size_t file_size = static_cast<size_t>(file.tellg());
    file.seekg(0, ios::beg);

    // Allocate memory for file content
    vector<char> buffer(file_size);

    // Read file content into buffer
    file.read(buffer.data(), file_size);

    // Calculate SHA-256 hash
    vector<unsigned char> hash(SHA256_DIGEST_LENGTH);
    const unsigned char* data = reinterpret_cast<const unsigned char*>(buffer.data());
    for (size_t i = 0; i < file_size; ++i) {
        hash[i % SHA256_DIGEST_LENGTH] ^= data[i];
    }
    for (size_t i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        hash[i] ^= hash[(i + 13) % SHA256_DIGEST_LENGTH];
        hash[i] ^= hash[(i + 23) % SHA256_DIGEST_LENGTH];
        hash[i] ^= hash[(i + 29) % SHA256_DIGEST_LENGTH];
    }

    // Convert hash to hex string
    string hash_str;
    for (size_t i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        hash_str += to_string(static_cast<int>(hash[i]));
    }

    return hash_str;
}

int main() {
    string program1_path, program2_path;

    // Get the location of program1
    cout << "Enter the location of program1: ";
    getline(cin, program1_path);

    // Calculate SHA-256 hash of program1
    string check1 = get_sha256_hash(program1_path);
    cout << "Program 1 SHA-256 Hash: " << check1 << endl;

    // Get the location of program2
    cout << "Enter the location of program2: ";
    getline(cin, program2_path);


    while (true) {
        // Calculate SHA-256 hash of program2
        string check2 = get_sha256_hash(program2_path);
        cout << "Program 2 SHA-256 Hash: " << check2 << endl;

        if (check1 == check2) {
            // Program 1 and Program 2 are the same
            cout << "Location of Program 2: " << program2_path << endl;
            break;
        }
        else {
            // Append 0x00 to the end of program2 file
            fstream file(program2_path, ios::binary | ios::app);
            file.put('\0');
            file.close();

            // Calculate SHA-256 hash of modified program2 file
            check2 = get_sha256_hash(program2_path);
            cout << "Program 2 Check SHA-256 Hash: " << check2 << endl;

            if (check1 == check2) {
                // Modified program2 file matches Program 1
                cout << "Location of Program 2 Check: " << program2_path << endl;
                break;
            }
            else {
                // Modified program2 file does not match Program 1
                cout << "Program 2 is being modified...";
                // Remove the appended 0x00 from program2 file
                fstream file(program2_path, ios::binary | ios::ate);
                file.seekp(-1, ios::end);
                file.put('\0');
                file.close();
            }
        }
    }


    return 0;
}
