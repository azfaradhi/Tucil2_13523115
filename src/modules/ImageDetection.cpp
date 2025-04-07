

// ImageDetection.cpp adalah file yang berisi fungsi-fungsi untuk medeteksi jenis file gambar.
// Metode yang digunakan adalah dengan membaca magic number dari file yang diinput.

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


string isImageFile(const string& filePath) {
    ifstream file(filePath, ios::binary);
    if (!file) {
        cerr << "Gagal membuka file: " << filePath << endl;
        return "#";
    }

    // Pengecekan magic number yaitu dengan membaca 8 byte pertama dari file

    vector<unsigned char> buffer(8);
    file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

    if (buffer.size() < 3) return "#";
    
    if (buffer[0] == 0xFF && buffer[1] == 0xD8 && buffer[2] == 0xFF) { 
        return "JPG";
    }

    if (buffer.size() >= 8 && buffer[0] == 0x89 && buffer[1] == 0x50 && 
        buffer[2] == 0x4E && buffer[3] == 0x47 && buffer[4] == 0x0D && 
        buffer[5] == 0x0A && buffer[6] == 0x1A && buffer[7] == 0x0A) { 
        return "PNG";
    }

    if ((buffer[0] == 0x47 && buffer[1] == 0x49 && buffer[2] == 0x46 && 
        buffer[3] == 0x38 && (buffer[4] == 0x37 || buffer[4] == 0x39) && 
        buffer[5] == 0x61)) { 
        return "GIF"; 
    }
    cout << "File yang diberikan bukan gambar!" << endl;
    return "#"; 
}