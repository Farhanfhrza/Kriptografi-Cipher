#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

// Utility function to validate key
bool validateKey(const string& key) {
    return key.length() >= 12;
}

// Vigenère Cipher Functions
string vigenereEncrypt(const string& plaintext, const string& key) {
    string cipherText;
    int keyIndex = 0;

    for (size_t i = 0; i < plaintext.size(); ++i) {
        char ch = plaintext[i];

        // Hanya mengenkripsi huruf (A-Z atau a-z)
        if (isalpha(ch)) {
            // Ubah karakter menjadi huruf besar
            ch = toupper(ch);
            char keyChar = toupper(key[keyIndex % key.size()]);

            // Proses enkripsi
            char encryptedChar = (ch + keyChar - 2 * 'A') % 26 + 'A';
            cipherText.push_back(encryptedChar);

            // Hanya geser indeks kunci jika karakter adalah huruf
            keyIndex++;
        } else {
            // Jika bukan huruf, langsung tambahkan tanpa enkripsi
            cipherText.push_back(ch);
        }
    }
    return cipherText;
}

string vigenereDecrypt(const string& cipherText, const string& key) {
    string plainText;
    int keyIndex = 0;

    for (size_t i = 0; i < cipherText.size(); ++i) {
        char ch = cipherText[i];

        // Hanya mendekripsi huruf (A-Z atau a-z)
        if (isalpha(ch)) {
            // Ubah karakter menjadi huruf besar
            ch = toupper(ch);
            char keyChar = toupper(key[keyIndex % key.size()]);

            // Proses dekripsi
            char decryptedChar = (ch - keyChar + 26) % 26 + 'A';
            plainText.push_back(decryptedChar);

            // Hanya geser indeks kunci jika karakter adalah huruf
            keyIndex++;
        } else {
            // Jika bukan huruf, langsung tambahkan tanpa dekripsi
            plainText.push_back(ch);
        }
    }
    return plainText;
}

// Playfair Cipher Functions
pair<int, int> findPosition(char matrix[5][5], char ch) {
    if (ch == 'J') ch = 'I'; // Gabungkan I dan J
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (matrix[i][j] == ch) {
                return {i, j};
            }
        }
    }
    return {-1, -1}; // should not happen
}

void prepareMatrix(const string& key, char matrix[5][5]) {
    string keyProcessed;
    vector<bool> present(26, false); // Untuk menandai huruf yang sudah ada dalam matriks

    // Hilangkan duplikat dan gabungkan 'I' dan 'J'
    for (char ch : key) {
        if (ch == 'J') ch = 'I';  // Gabungkan I dan J
        ch = toupper(ch);
        if (!present[ch - 'A']) {
            keyProcessed.push_back(ch);
            present[ch - 'A'] = true;
        }
    }

    // Tambahkan huruf-huruf alfabet yang tersisa (tanpa J)
    for (char ch = 'A'; ch <= 'Z'; ++ch) {
        if (ch == 'J') continue; // Lewati 'J' karena sudah digabung dengan 'I'
        if (!present[ch - 'A']) {
            keyProcessed.push_back(ch);
            present[ch - 'A'] = true;
        }
    }

    // Mengisi matriks 5x5 dengan kunci yang diproses
    int index = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            matrix[i][j] = keyProcessed[index++];
        }
    }
}

string playfairEncrypt(const string& plaintext, const string& key) {
    char matrix[5][5];
    prepareMatrix(key, matrix);

    // Memproses plaintext menjadi pasangan huruf (digram)
    string preparedText;
    for (size_t i = 0; i < plaintext.length(); ++i) {
        char ch = toupper(plaintext[i]);
        if (isalpha(ch)) {
            if (!preparedText.empty() && preparedText.back() == ch) {
                preparedText.push_back('X'); // Sisipkan X jika dua huruf berturut-turut sama
            }
            preparedText.push_back(ch);
        }
    }

    // Tambahkan padding jika jumlah huruf ganjil
    if (preparedText.size() % 2 != 0) {
        preparedText.push_back('X');
    }

    // Proses enkripsi
    string cipherText;
    for (size_t i = 0; i < preparedText.size(); i += 2) {
        char a = preparedText[i];
        char b = preparedText[i + 1];

        auto [aRow, aCol] = findPosition(matrix, a);
        auto [bRow, bCol] = findPosition(matrix, b);

        if (aRow == bRow) {
            // Jika kedua huruf ada di baris yang sama
            cipherText.push_back(matrix[aRow][(aCol + 1) % 5]);
            cipherText.push_back(matrix[bRow][(bCol + 1) % 5]);
        } else if (aCol == bCol) {
            // Jika kedua huruf ada di kolom yang sama
            cipherText.push_back(matrix[(aRow + 1) % 5][aCol]);
            cipherText.push_back(matrix[(bRow + 1) % 5][bCol]);
        } else {
            // Jika huruf ada di posisi persegi panjang
            cipherText.push_back(matrix[aRow][bCol]);
            cipherText.push_back(matrix[bRow][aCol]);
        }
    }

    return cipherText;
}

string playfairDecrypt(const string& cipherText, const string& key) {
    char matrix[5][5];
    prepareMatrix(key, matrix);

    string plainText;
    for (size_t i = 0; i < cipherText.size(); i += 2) {
        char a = cipherText[i];
        char b = cipherText[i + 1];

        auto [aRow, aCol] = findPosition(matrix, a);
        auto [bRow, bCol] = findPosition(matrix, b);

        if (aRow == bRow) {
            // Jika kedua huruf ada di baris yang sama
            plainText.push_back(matrix[aRow][(aCol + 4) % 5]);
            plainText.push_back(matrix[bRow][(bCol + 4) % 5]);
        } else if (aCol == bCol) {
            // Jika kedua huruf ada di kolom yang sama
            plainText.push_back(matrix[(aRow + 4) % 5][aCol]);
            plainText.push_back(matrix[(bRow + 4) % 5][bCol]);
        } else {
            // Jika huruf ada di posisi persegi panjang
            plainText.push_back(matrix[aRow][bCol]);
            plainText.push_back(matrix[bRow][aCol]);
        }
    }

    // Hilangkan 'X' yang ditambahkan di antara huruf berulang atau sebagai padding
    for (size_t i = 0; i < plainText.size() - 1; ++i) {
        if (plainText[i] == 'X' && plainText[i - 1] == plainText[i + 1]) {
            plainText.erase(i, 1);  // Hilangkan padding 'X' yang tidak perlu
        }
    }

    // Hapus 'X' jika ada di akhir pesan dan merupakan padding
    if (plainText.back() == 'X') {
        plainText.pop_back();
    }

    return plainText;
}

// Hill Cipher Functions
// Utility function for mod 26
int mod26(int x) {
    return (x % 26 + 26) % 26;
}

// Function to find the determinant of a 2x2 matrix
int determinant(const vector<vector<int>>& matrix) {
    return mod26(matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]);
}

// Function to find the multiplicative inverse of a number under modulo 26
int modInverse(int a) {
    a = mod26(a);
    for (int x = 1; x < 26; ++x) {
        if (mod26(a * x) == 1)
            return x;
    }
    return -1;  // No modular inverse exists
}

// Function to calculate the inverse of a 2x2 matrix
vector<vector<int>> inverseMatrix(const vector<vector<int>>& matrix) {
    int det = determinant(matrix);
    int invDet = modInverse(det);

    if (invDet == -1) {
        throw runtime_error("Matrix is not invertible.");
    }

    vector<vector<int>> inverse(2, vector<int>(2));
    inverse[0][0] = mod26(matrix[1][1] * invDet);
    inverse[0][1] = mod26(-matrix[0][1] * invDet);
    inverse[1][0] = mod26(-matrix[1][0] * invDet);
    inverse[1][1] = mod26(matrix[0][0] * invDet);

    return inverse;
}

// Function to encrypt a message using Hill Cipher (2x2 key matrix)
string hillEncrypt(const string& plaintext, const vector<vector<int>>& keyMatrix) {
    string cipherText;
    string sanitizedText;

    // Sanitize input to remove non-alphabetic characters and convert to uppercase
    for (char ch : plaintext) {
        if (isalpha(ch)) {
            sanitizedText += toupper(ch);
        }
    }

    // Ensure the length is even
    if (sanitizedText.size() % 2 != 0) {
        sanitizedText += 'X';  // or any other padding character
    }

    for (size_t i = 0; i < sanitizedText.size(); i += 2) {
        vector<int> block(2);
        block[0] = sanitizedText[i] - 'A';
        block[1] = (i + 1 < sanitizedText.size()) ? sanitizedText[i + 1] - 'A' : 'X' - 'A';

        vector<int> result(2);
        result[0] = mod26(keyMatrix[0][0] * block[0] + keyMatrix[0][1] * block[1]);
        result[1] = mod26(keyMatrix[1][0] * block[0] + keyMatrix[1][1] * block[1]);

        cipherText += (result[0] + 'A');
        cipherText += (result[1] + 'A');
    }
    return cipherText;
}

// Function to decrypt a message using Hill Cipher (2x2 key matrix)
string hillDecrypt(const string& cipherText, const vector<vector<int>>& keyMatrix) {
    vector<vector<int>> invMatrix = inverseMatrix(keyMatrix);
    return hillEncrypt(cipherText, invMatrix);
}

// Helper function to check key matrix is 2x2
vector<vector<int>> getKeyMatrix() {
    vector<vector<int>> keyMatrix(2, vector<int>(2));
    cout << "Masukkan matriks kunci 2x2 (hanya huruf) untuk Hill cipher:\n";
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            char ch;
            cin >> ch;
            keyMatrix[i][j] = toupper(ch) - 'A';
        }
    }
    return keyMatrix;
}

// Main Program
int main() {
    int choice;
    string message, key, filename;

    cout << "===== Cipher Program =====\n";
    cout << "Pilih Cipher:\n";
    cout << "1. Vigenere Cipher\n";
    cout << "2. Playfair Cipher\n";
    cout << "3. Hill Cipher\n";
    cout << "Pilihan: ";
    cin >> choice;

    cout << "Inputkan pesan (1. Dari file, 2. Dari keyboard): ";
    int inputChoice;
    cin >> inputChoice;

    if (inputChoice == 1) {
        cout << "Masukkan nama file (format .txt): ";
        cin >> filename;
        ifstream file(filename);
        if (file.is_open()) { // Memeriksa apakah file berhasil dibuka
        string line;
        while (getline(file, line)) { // Membaca file per baris
            message += line + "\n"; // Menambahkan setiap baris ke variabel content
        }
        file.close(); // Menutup file
    } else {
        cerr << "Tidak dapat membuka file!" << endl; // Pesan error jika file tidak dapat dibuka
        return 0;
    }

    cout << "Isi file:\n" << message;
    } else {
        cout << "Inputkan pesan: ";
        cin.ignore();
        getline(cin, message);
    }

    cout << "Inputkan kunci (minimal 12 karakter): ";
    cin >> key;
    if (!validateKey(key)) {
        cout << "Kunci harus minimal 12 karakter!\n";
        return 1;
    }

    int action;
    cout << "Pilih tindakan (1. Enkripsi, 2. Dekripsi): ";
    cin >> action;
    vector<vector<int>> keyMatrix = getKeyMatrix();
    string result;
    switch (choice) {
        case 1:
            if (action == 1) {
                result = vigenereEncrypt(message, key);
            } else {
                result = vigenereDecrypt(message, key);
            }
            break;
        case 2:
            if (action == 1) {
                result = playfairEncrypt(message, key);
            } else {
                result = playfairDecrypt(message, key);
            }
            break;
        case 3:
            if (action == 1) {
                result = hillEncrypt(message, keyMatrix);
            } else {
                result = hillDecrypt(message, keyMatrix);
            }
            break;
        default:
            cout << "Pilihan tidak valid.\n";
            return 1;
    }

    cout << "Hasil: " << result << endl;

    return 0;
}
