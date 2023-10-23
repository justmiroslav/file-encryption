#include <iostream>
#include <cstring>
using namespace std;

#define CAESAR_EXPORTS

extern "C" {
    __declspec(dllexport) char* encrypt(const char* rawText, int key) {
        int textLen = strlen(rawText);
        char* encryptedText = new char[textLen + 1];
        const char* lowerAlphabet = "abcdefghijklmnopqrstuvwxyz";
        const char* upperAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        int alpLen = strlen(upperAlphabet);

        for (int i = 0; i < textLen; i++) {
            if (isalpha(rawText[i])) {
                if (islower(rawText[i])) {
                    encryptedText[i] = lowerAlphabet[(rawText[i] - 'a' + key + alpLen) % alpLen];
                } else {
                    encryptedText[i] = upperAlphabet[(rawText[i] - 'A' + key + alpLen) % alpLen];
                }
            } else {
                encryptedText[i] = rawText[i];
            }
        }
        encryptedText[textLen] = '\0';
        return encryptedText;
    }
    __declspec(dllexport) char* decrypt(const char* encryptedText, int key) {
        int textLen = strlen(encryptedText);
        char* decryptedText = new char[textLen + 1];
        const char* lowerAlphabet = "abcdefghijklmnopqrstuvwxyz";
        const char* upperAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        int alpLen = strlen(upperAlphabet);

        for (int i = 0; i < textLen; i++) {
            if (isalpha(encryptedText[i])) {
                if (islower(encryptedText[i])) {
                    int shiftedIndex = (encryptedText[i] - 'a' - key + alpLen) % alpLen;
                    if (shiftedIndex < 0) {
                        shiftedIndex += alpLen;
                    }
                    decryptedText[i] = lowerAlphabet[shiftedIndex];
                } else {
                    int shiftedIndex = (encryptedText[i] - 'A' - key + alpLen) % alpLen;
                    if (shiftedIndex < 0) {
                        shiftedIndex += alpLen;
                    }
                    decryptedText[i] = upperAlphabet[shiftedIndex];
                }
            } else {
                decryptedText[i] = encryptedText[i];
            }
        }
        decryptedText[textLen] = '\0';
        return decryptedText;
    }
};