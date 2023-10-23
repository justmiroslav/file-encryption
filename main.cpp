#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;

typedef char* (*encrypt_ptr_t)(const char*, int);
typedef char* (*decrypt_ptr_t)(const char*, int);

class IReader {
public:
    virtual string read(const string& filename, int start, int end) = 0;
    virtual ~IReader() {}
};

class FileReader : public IReader {
public:
    string read(const string& filename, int start, int end) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file for reading: " << filename << endl;
            throw runtime_error("Error opening file for reading");
        }
        string text;
        char character;
        int charCount = -1;
        while (file.get(character)) {
            charCount++;
            if (charCount >= start && charCount <= end) {
                text += character;
            }
        }
        if (start < 0 || start >= end || end < 0 || end > charCount) {
            throw runtime_error("Invalid boundaries");
        }
        file.close();
        cout << "Text has been loaded successfully" << endl;
        return text;
    }
};

class IWriter {
public:
    virtual void write(const string& filename, const string& text) = 0;
    virtual ~IWriter() {}
};

class FileWriter : public IWriter {
public:
    void write(const string& filename, const string& text) {
        fstream file(filename, fstream::app);
        if (file.is_open()) {
            file << text;
            file.close();
            cout << "Text has been saved successfully" << endl;
        } else {
            throw runtime_error("Error opening file for saving");
        }
    }
};

int main() {
    HMODULE handle = LoadLibrary(TEXT("caesar.dll"));

    if (handle == nullptr || handle == INVALID_HANDLE_VALUE) {
        cout << "Lib not found" << endl;
        return 1;
    }
    encrypt_ptr_t encrypt_ptr = (encrypt_ptr_t)GetProcAddress(handle, "encrypt");
    decrypt_ptr_t decrypt_ptr = (decrypt_ptr_t)GetProcAddress(handle, "decrypt");

    if (encrypt_ptr == nullptr || decrypt_ptr == nullptr) {
        cout << "Functions not found in DLL" << endl;
        return 1;
    }

    int key, command, start, end;
    IReader* reader = new FileReader();
    IWriter* writer = new FileWriter();
    string loadFile, writeFile;
    while (true) {
        cout << "Enter 1 to encrypt the text/2 to decrypt the text/3 to stop the program: " << endl;
        cin >> command;
        cin.ignore();
        if (command == 1 || command == 2) {
            cout << "Enter the filename for loading: " << endl;
            getline(cin, loadFile);
            cout << "Enter the start and end boundaries: " << endl;
            cin >> start >> end;
            cin.ignore();
            try {
                string fileText = reader->read(loadFile, start, end);
                cout << "Enter the filename for saving: " << endl;
                getline(cin, writeFile);
                try {
                    cout << "Enter " << (command == 1 ? "encryption" : "decryption") << " key: " << endl;
                    cin >> key;
                    cin.ignore();
                    char* processedText = (command == 1) ? encrypt_ptr(fileText.c_str(), key) : decrypt_ptr(fileText.c_str(), key);
                    writer->write(writeFile, processedText);
                    delete[] processedText;
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << endl;
                }
            } catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        } else if (command == 3) {
            cout << "Program stopped" << endl;
            delete reader;
            delete writer;
            break;
        } else {
            cout << "Enter a valid command" << endl;
        }
    }
    return 0;
}
