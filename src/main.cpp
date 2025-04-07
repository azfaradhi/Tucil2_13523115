#include "modules/ImageDetection.cpp"
#include "header/ImageExtract.hpp"
#include "header/QuadTree.hpp"
#include "header/QuadTreeNode.hpp"
#include <chrono>

using namespace std::chrono;

int main() {
cout << " ___                                                        \n"
     << "|_ _|_ __ ___   __ _  __ _  ___                             \n"
     << " | || '_ ` _ \\ / _` |/ _` |/ _ \\                            \n"
     << " | || | | | | | (_| | (_| |  __/                            \n"
     << "|___|_| |_| |_|\\__,_|\\__, |\\___|              _             \n"
     << " / ___|___  _ __ ___ |___/  _ __ ___  ___ ___(_) ___  _ __  \n"
     << "| |   / _ \\| '_ ` _ \\| '_ \\| '__/ _ \\/ __/ __| |/ _ \\| '_ \\ \n"
     << "| |__| (_) | | | | | | |_) | | |  __/\\__ \\__ \\ | (_) | | | |\n"
     << " \\____\\___/|_| |_| |_| .__/|_|  \\___||___/___/_|\\___/|_| |_|\n"
     << "                     |_|                                    \n" << endl;

    printf("\x1B[32mMade by Azfa Radhiyya Hakim\033[0m\t\t");
    cout << endl << endl;
    

    ImageExtract img;
    cout << "Masukkan alamat relatif gambar!" << endl;
    cout << "Contoh: test/1.jpg" << endl;
    cout << ">> ";
    string filename;
    cin >> filename;
    string isImage = isImageFile(filename);
    if (isImage == "#") {
        cout << "Silahkan coba lagi!" << endl;
        return 1;
    }
    if (!img.loadImage(filename)) {
        cout << "Gagal load image!" << endl;
        return 1;
    } 

    long orginalSize = img.getFileSize(filename);
    int height = img.getHeight();
    int width = img.getWidth();
    int N = img.pixelCount();
    vector<vector<RGB>> pixels = img.getPixels();


    int method;
    while (true){
        cout << "Masukkan metode yang diinginkan!" << endl;
        cout << "Catatan: 1 = Variance, 2 = MAD, 3 = Max Pixel Difference, 4 = Entropi" << endl;
        cout << ">> ";
        if (!(cin >> method)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input harus berupa angka! Silahkan coba lagi!" << endl;
            continue;
        }
        if (method < 1 || method > 5) {
            cout << "Metode tidak valid! Silahkan coba lagi!" << endl;
            continue;
        } else {
            break;
        }
    }
    double threshold;

    while (true){
        cout << "Masukkan threshold yang diinginkan!" << endl;
        cout << "Catatan:" << endl;
        cout << "1 = Variance (0 - 16256.25)" << endl;
        cout << "2 = MAD (0 - 127.5)" << endl;
        cout << "3 = Max Pixel Difference (0 - 255)" << endl;
        cout << "4 = Entropi (0 - 8)" << endl;
        cout << ">> ";
        if (!(cin >> threshold)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input harus berupa double! Silahkan coba lagi!" << endl;
            continue;
        }
        if (method == 1){
            if (threshold < 0 || threshold > 16256.25) {
                cout << "Range dari threshold tidak valid! Silahkan coba lagi!" << endl;
                continue;
            } else {
                break;
            }
        }
        else if (method == 2) {
            if (threshold < 0 || threshold > 127.5) {
                cout << "Range dari threshold tidak valid! Silahkan coba lagi!" << endl;
                continue;
            } else {
                break;
            }
        }
        else if (method == 3) {
            if (threshold < 0 || threshold > 255) {
                cout << "Range dari threshold tidak valid! Silahkan coba lagi!" << endl;
                continue;
            } else {
                break;
            }
        }
        else if (method == 4) {
            if (threshold < 0 || threshold > 8) {
                cout << "Range dari threshold tidak valid! Silahkan coba lagi!" << endl;
                continue;
            } else {
                break;
            }
        }
    }
    int minSize;
    while (true){
        cout << "Masukkan minimum size yang diinginkan!" << endl;
        cout << "Catatan: anu" << endl;
        cout << ">> ";
        if (!(cin >> minSize)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input harus berupa angka! Silahkan coba lagi!" << endl;
            continue;
        }
        if (minSize > height * width) {
            cout << "Minimum size tidak valid! Silahkan coba lagi!" << endl;
            continue;
        } else {
            break;
        }
    }


    cout << "Memulai kompresi ..." << endl;

    auto startCompression = high_resolution_clock::now();
    QuadTree quadtree(threshold, minSize, method);
    quadtree.startDNC(pixels);
    auto endCompression = high_resolution_clock::now();
    auto compressionTime = duration_cast<milliseconds>(endCompression - startCompression).count();


    
    cout << "Rekonstruksi gambar..." << endl;
    
    vector<vector<RGB>> reconstructedImage = quadtree.reconstructImage(width, height);
    

    filename = filename.substr(0, filename.find_last_of('.')) + "_output.jpg";
    img.saveImage(reconstructedImage, filename);

    long newSize = img.getFileSize(filename);\

    cout << "Original file size: " << orginalSize << " bytes" << endl;
    cout << "New file size: " << newSize << " bytes" << endl;
    cout << "Compression time: " << compressionTime << " ms" << endl;
    cout << "Compression ratio: " << (double) (1 - (double)newSize/orginalSize)* 100 << "%" << endl;
    cout << "Total nodes: " << quadtree.totalNodes() << endl;
    cout << "Leaf nodes: " << quadtree.leafNodes() << endl;
    cout << "Depth: " << quadtree.depth() << endl;
    

    cout << "Membuat GIf kompresi..." << endl;
    filename = filename.substr(0, filename.find_last_of('.')) + "_gif.gif";
    quadtree.createCompressionGIF(filename , width, height);
    cout << "Gambar dan GIF telah berhasil disimpan!" << endl;

    return 0;
}