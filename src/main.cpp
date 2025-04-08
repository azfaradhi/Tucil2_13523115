#include "modules/ImageDetection.cpp"
#include "header/ImageExtract.hpp"
#include "header/QuadTree.hpp"
#include "header/QuadTreeNode.hpp"
#include <chrono>
#include <filesystem>

using namespace std::chrono;

int main() {

    bool isTarget = false;
    double thersholdNow = 0;
    double thersholdMax = 0;
    double thersholdMin = 0;

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
    cout << "Masukkan alamat relatif gambar! (contoh: test/tree.jpg)" << endl;
    printf("\x1B[32m>> \033[0m");
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
        cout << endl;
        cout << "Masukkan metode yang diinginkan!" << endl;
        cout << "Catatan: 1 = Variance, 2 = MAD, 3 = Max Pixel Difference, 4 = Entropi, 5 = SSIM" << endl;
        printf("\x1B[32m>> \033[0m");
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
        cout << endl;
        cout << "Masukkan threshold yang diinginkan!" << endl;
        cout << "Catatan:" << endl;
        cout << "Variance (0 - 16256.25)" << endl;
        cout << "MAD (0 - 127.5)" << endl;
        cout << "Max Pixel Difference (0 - 255)" << endl;
        cout << "Entropi (0 - 8)" << endl;
        cout << "SSIM (0 - 1)" << endl;
        printf("\x1B[32m>> \033[0m");
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
        else if (method == 5) {
            if (threshold < 0 || threshold > 1) {
                cout << "Range dari threshold tidak valid! Silahkan coba lagi!" << endl;
                continue;
            } else {
                break;
            }
        }
    }
    int minSize;
    while (true){
        cout << endl;
        cout << "Masukkan minimum size yang diinginkan!" << endl;
        cout << "Catatan: size tidak boleh lebih besar dari ukuran foto asli!" << endl;
        printf("\x1B[32m>> \033[0m");
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
    double target;
    while (true){
        cout << endl;
        cout << "Masukkan target persentase kompresi!" << endl;
        cout << "Catatan: threshold akan disesuaikan jika mengisi ini, jika tetap ingin menggunakan threshold ketik 0." << endl;
        printf("\x1B[32m>> \033[0m");
        if (!(cin >> target)){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input harus berupa angka! Silahkan coba lagi!" << endl;
            continue;
        }
        if (target == 0){
            isTarget = false;
        }
        else{
            isTarget = true;
        }
        break;
    }
    string outputFileName;
    string outputGifName;
    cout << endl;
    cout << "Masukkan alamat relatif untuk menyimpan gambar! (Contoh: test/pohon_output.jpg)" << endl;
    printf("\x1B[32m>> \033[0m");
    cin >> outputFileName;
    cout << endl;
    cout << "Masukkan alamt relatif untuk menyimpan GIF! (Contoh: test/pohon_output.gif)" << endl;
    printf("\x1B[32m>> \033[0m");
    cin >> outputGifName;

    size_t lastSlash = outputGifName.find_last_of("/\\");
    if (lastSlash != string::npos) {
        string directory = outputGifName.substr(0, lastSlash);
        std::filesystem::create_directories(directory);
    }
    lastSlash = outputFileName.find_last_of("/\\");
    if (lastSlash != string::npos) {
        string directory = outputFileName.substr(0, lastSlash);
        std::filesystem::create_directories(directory);
    }

    if (target == 0){
        thersholdNow = threshold;
    }
    else{
        if (method == 1){
            thersholdMax = 16256.25;
            thersholdMin = 0;
        }
        else if (method == 2){
            thersholdMax = 127.5;
            thersholdMin = 0;
        }
        else if (method == 3){
            thersholdMax = 255;
            thersholdMin = 0;
        }
        else if (method == 4){
            thersholdMax = 8;
            thersholdMin = 0;
        }
        else if (method == 5){
            thersholdMax = 1;
            thersholdMin = 0;
        }
        thersholdNow = (thersholdMax + thersholdMin) / 2;
    }


    cout << "Memulai kompresi ..." << endl;
    int totalnodes = 0;
    int leafnodes = 0;
    int depth = 0;
    double ratio = 0;
    long newSize = 0;
    auto startCompression = high_resolution_clock::now();
    while (true){
        QuadTree quadtree(thersholdNow, minSize, method);
        quadtree.startDNC(pixels);
        vector<vector<RGB>> reconstructedImage = quadtree.reconstructImage(width, height);
        img.saveImage(reconstructedImage, outputFileName);
        newSize = img.getFileSize(outputFileName);

        if (isTarget){
            ratio = (double) (1 - (double)newSize/orginalSize)* 100;
            if (abs(ratio - target) < 0.01){
                cout << "Berhasil mencapai target dengan selisih 0.01!" <<  endl;
                totalnodes = quadtree.totalNodes();
                leafnodes = quadtree.leafNodes();
                depth = quadtree.depth();
                cout << "Membuat GIF kompresi..." << endl;
                quadtree.createCompressionGIF(outputGifName , width, height);
                break;
            }
            else if (abs(thersholdMax - thersholdMin) < 0.01){
                cout << "Threshold tidak dapat diturunkan lagi!!" << endl;
                cout << "Berhasil mencapai target!" <<  endl;
                totalnodes = quadtree.totalNodes();
                leafnodes = quadtree.leafNodes();
                depth = quadtree.depth();
                cout << "Membuat GIF kompresi..." << endl;
                quadtree.createCompressionGIF(outputGifName , width, height);
                break;
            }

            if (ratio < target){
                if (method == 5){
                    thersholdMax = thersholdNow;
                    thersholdNow = (thersholdMax + thersholdMin) / 2;
                }
                else{
                    thersholdMin = thersholdNow;
                    thersholdNow = (thersholdMax + thersholdMin) / 2;
                }
            }
            else if (ratio > target){
                if (method == 5){
                    thersholdMin = thersholdNow;
                    thersholdNow = (thersholdMax + thersholdMin) / 2;
                }
                else{
                    thersholdMax = thersholdNow;
                    thersholdNow = (thersholdMax + thersholdMin) / 2;
                }
            }
            cout << "Persentase kompresi saat ini: " << ratio << "%" << endl;
            remove(outputFileName.c_str());

        }
        else{
            cout << "Berhasil mengkompresi gambar!" <<  endl;
            ratio = (double) (1 - (double)newSize/orginalSize)* 100;
            totalnodes = quadtree.totalNodes();
            leafnodes = quadtree.leafNodes();
            depth = quadtree.depth();
            cout << "Membuat GIF kompresi..." << endl;
            quadtree.createCompressionGIF(outputGifName, width, height);
            break;
        }
    }
    auto endCompression = high_resolution_clock::now();
    auto compressionTime = duration_cast<milliseconds>(endCompression - startCompression).count();


    
    cout << "Ukuran file asli: " << orginalSize << " bytes" << endl;
    cout << "Ukuran file baru: " << newSize << " bytes" << endl;
    cout << "Waktu kompresi: " << compressionTime << " ms" << endl;
    cout << "Rasio kompresi: " << ratio << "%" << endl;
    cout << "Total node: " << totalnodes << endl;
    cout << "Node daun: " << leafnodes << endl;
    cout << "Kedalaman: " << depth << endl;
    

    cout << "Gambar dan GIF telah berhasil disimpan!" << endl;
    cout << "(Catatan: kadang file hasil kompresi tidak langsung muncul pada vscode, silahkan cek pada explorer)" << endl;

    return 0;
}