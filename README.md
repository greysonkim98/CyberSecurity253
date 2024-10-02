# SwapEncryption Program Explanation

The **SwapEncryption** program is written in C++ and developed on Microsoft Windows 11 version 10.0.22631. It was compiled and tested using **g++.exe 14.2.0 (Rev1, Built by the MSYS2 project)**. The program encrypts and decrypts text files using a specified key file, generating a cipher text file through a three-step process. It requires two text files: a plain text file and a key file, both provided by the user.

---

## Encryption Process

The encryption consists of three main steps:

### 1. Key File Addition
- **Input**: The plain text file is read, and each character is converted into its ASCII integer equivalent, stored in a vector called `plainText`.
- **Operation**: Each element in `plainText` is incremented by the sum of the key file's ASCII values modulo 1000. This result is stored in a vector called `pre_encrypt1`.

### 2. Odd-Even Element Swapping
- **Input**: The vector `pre_encrypt1` is divided into 3-element groups and stored in a 2D vector `pre_encrypt2`. If the total number of elements isn’t divisible by 3, the last group is padded with zeros.
- **Operation**: Odd-positioned groups are swapped with even-positioned ones. The last group remains untouched.

### 3. Shifting
- **Input**: The modified 2D vector `pre_encrypt2` is further processed.
- **Operation**: All groups are cyclically shifted to the right by one position. If the first group contains padding zeros, it is rotated so that the zero comes first. For example, if the first group was `[700]`, it becomes `[007]` after rotation.

The encryption process uses the key file to add an additional layer of security. Without the correct key or knowledge of the swapping and shifting algorithm, any attacker would face a scrambled text. The final shift also ensures that any padding elements are included at the start, enhancing the security of the encryption.

---

## Decryption Process

The **decryption** process follows the reverse of the encryption steps. The user inputs the cipher text and key files, and the program reconstructs the original plain text by reversing the shifting, swapping, and key addition operations.

---

## Error Handling

The program includes error handling for the following cases:
- **Missing input files**.
- **Input files not found in the directory**.
- **Attempting to create a file that already exists in the directory**.
