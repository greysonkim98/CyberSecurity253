//Program name: "SwapEncryption"                                                                                           
//Copyright (C) 2024 Minjae Kim                 
//                                                                             
//This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License  
//version 3 as published by the Free Software Foundation.                                                                    
//This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied         
//warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.     
//<https://www.gnu.org/licenses/>.
//
//Author information
//  Author name: Minjae Kim
//  Author email: minjaek@csu.fullerton.edu
//  Author CWID: 885206615
//  Author class: CPSC253
//
//Program information
//  Program name: SwapEncryption
//  Programming languages: C++
//  Date of last update: 2024-09-30
//  Date of reorganization of comments: 2024-09-30
//  Files in this program: SwapEncryption.cpp
//  Developed OS: Ubuntu 24.04.1 LTS
//  Tested OS Name: Microsoft Windows 11 Home
//  Tested OS Version: 10.0.22631 N/A Build 22631
//  Tested Compiler: g++.exe (Rev1, Built by MSYS2 project) 14.2.0
//
//Program description
//  This encrypt and decrypt text file and generate cipher text by using key text file.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <algorithm>

using namespace std;

// Function to read characters from a file into an integer vector
vector<int> readFile(const string& filename) {
    vector<int> data;
    ifstream file(filename);

    if (file.is_open()) {
        char c;
        while (file.get(c)) {
            data.push_back(static_cast<int>(c));
        }
        file.close();
    } else {
        cerr << "Error opening file: " << filename << endl;
    }

    return data;
}

// Function to write an integer vector to a file as characters
void writeFile(const string& filename, const vector<int>& data) {
    ofstream file(filename);

    if (file.is_open()) {
        for (int c : data) {
            file.put(static_cast<char>(c));
        }
        file.close();
    } else {
        cerr << "Error creating file: " << filename << endl;
    }
}

// Function to convert vector key to integer key
int readKey(const vector<int>vec_key) {
    int key{0};
    for(int element: vec_key) {
        key += element;
    }
    return key;
}

// Function to perform encryption
vector<int> encrypt(vector<int> message, vector<int> vec_key) {
    // Add message with the key 
    vector<int> pre_encrypt1;
    int key = readKey(vec_key);
    for (int element: message){
        pre_encrypt1.push_back(element + key);
    }
   
    // Putting pre_encrypt1ed message into 3 capacity container 
    const int arraySize{3};
    array<int,arraySize> column = {0,0,0};
    int row = pre_encrypt1.size() / arraySize + (pre_encrypt1.size() % arraySize != 0 ? 1 : 0);
    vector<array<int, arraySize>> pre_encrypt2(row, column); 
 
    int messageIndex = 0;
    for (int i = 0; i < pre_encrypt2.size(); i++ ) {
        for (int j = 0; j < arraySize && messageIndex < pre_encrypt1.size(); j++) {
            pre_encrypt2[i][j] = pre_encrypt1[messageIndex++];
        }
    }
    // Swapping the boxes
    unsigned int count = 0;
    while(count < pre_encrypt2.size() - 1) {
        swap(pre_encrypt2[count], pre_encrypt2[count+1]);
        count += 2;
    }

    // Backward rotation 1
    if (pre_encrypt1.size() % arraySize != 0) {
        rotate(pre_encrypt2.rbegin(), pre_encrypt2.rbegin() + 1, pre_encrypt2.rend());
    }

    // Letting zero coming first
    if(pre_encrypt1.size() % 3 == 1) {
        rotate(pre_encrypt2[0].rbegin(), pre_encrypt2[0].rbegin() + 1, pre_encrypt2[0].rend());
    } else if (pre_encrypt1.size() % 3 == 2) {
        rotate(pre_encrypt2[0].rbegin(), pre_encrypt2[0].rbegin() + 2, pre_encrypt2[0].rend());
    }
    
    // Store rotated 2Dvector into 1D integer vector
    vector<int> encrypted;
    for (const auto& row : pre_encrypt2) {
        encrypted.insert(encrypted.end(), row.begin(), row.end());
    }

    return encrypted;
}

// Function to perform decryption
vector<int> decrypt(const vector<int>& encrypted, const vector<int>& vec_key) {
    const int arraySize{3};
    int row = encrypted.size() / arraySize;
    vector<array<int, arraySize>> pre_decrypt(row, array<int, arraySize>{0, 0, 0});
    
    // Reconstruct the 2D vector from the encrypted 1D vector
    int encryptedIndex = 0;
    for (int i = 0; i < pre_decrypt.size(); i++) {
        for (int j = 0; j < arraySize; j++) {
            pre_decrypt[i][j] = encrypted[encryptedIndex++];
        }
    }
    
    // Undo the zero rotation
    if (encrypted.size() % 3 == 1) {
        rotate(pre_decrypt[0].begin(), pre_decrypt[0].begin() + 1, pre_decrypt[0].end());
    } else if (encrypted.size() % 3 == 2) {
        rotate(pre_decrypt[0].begin(), pre_decrypt[0].begin() + 2, pre_decrypt[0].end());
    }

    // Undo the backward rotation
    if (encrypted.size() % arraySize != 0) {
        rotate(pre_decrypt.begin(), pre_decrypt.begin() + 1, pre_decrypt.end());
    }

    // Undo the box swapping
    unsigned int count = 0;
    while (count < pre_decrypt.size() - 1) {
        swap(pre_decrypt[count], pre_decrypt[count + 1]);
        count += 2;
    }

    // Flatten the 2D vector back to 1D
    vector<int> decrypted;
    for (const auto& row : pre_decrypt) {
        for (int val : row) {
            if (val != 0) {  // Skip padding zeros
                decrypted.push_back(val);
            }
        }
    }

    // Undo the key addition
    int key = readKey(vec_key);
    for (int& element : decrypted) {
        element -= key;
    }

    return decrypted;
}



int main() {
    int choice{0}; 
    
    do {
        cout << "Choose an option:\n";
        cout << "1. Encryption\n";
        cout << "2. Decryption\n";
        cout << "3. Exit\n";
        cout << "Selection: ";
        cin >> choice;

        if (choice == 1) {
            string inputFile, keyFile, outputFile;
            cout << "Enter message file name: ";
            cin >> inputFile;
            cout << "Enter key file name: ";
            cin >> keyFile;
            cout << "Enter cipher file name: ";
            cin >> outputFile;

            vector<int> plainText = readFile(inputFile);
            vector<int> key = readFile(keyFile);
            vector<int> pre_encrypt2 = encrypt(plainText, key);
            writeFile(outputFile, pre_encrypt2);
            cout << "Encryption successful.\n";
        } else if (choice == 2) {
            string encryptedFile, keyFile, outputFile;
            cout << "Enter encrypted file name: ";
            cin >> encryptedFile;
            cout << "Enter key file name: ";
            cin >> keyFile;
            cout << "Enter output file name: ";
            cin >> outputFile;

            vector<int> pre_encrypt2 = readFile(encryptedFile);
            vector<int> key = readFile(keyFile);
            vector<int> decrypted = decrypt(pre_encrypt2, key);
            writeFile(outputFile, decrypted);
            cout << "Decryption successful.\n";
        } else if (choice == 3) {
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    } while(choice != 3);

    return 0;
}