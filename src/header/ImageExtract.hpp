
/*
- Class yang digunakan untuk mengekstrak gambar dari input file
- Disini dibuat struct RGB untuk menyimpan data warna RGB dari pixel yang didapat
- Class ImageExtract memiliki atribut pixel (vector dari vector RGB), lebar, tinggi, dan jumlah pixel
- Fungsi stbi_load dan stbi_write didapat dari library stb_image.h dan stb_image_write.h

*/


#ifndef IMAGE_EXTRACT_HPP
#define IMAGE_EXTRACT_HPP
#include <iostream>
#include <vector>
#include <fstream>

#include "../stb_image/stb_image.hpp"
#include "../stb_image/stb_image_write.hpp" 

using namespace std;

struct RGB {
    double r, g, b;
    RGB() : r(0), g(0), b(0) {}
    RGB(double r, double g, double b) : r(r), g(g), b(b) {}
};

class ImageExtract {
private:
    vector<vector<RGB>> pixels;
    int width;
    int height;
    int N;
public:

    ImageExtract() : width(0), height(0), N(0) {}
    
    ImageExtract(int height, int width, int N, vector<vector<RGB>> pixels) {
        this->height = height;
        this->width = width;
        this->N = N;
        this->pixels = pixels;
    }

    ~ImageExtract() {}

    bool loadImage(const string& filename) {
        int channels;
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 3);
        N = width * height;
        if (!data) {
            return false;
        }
        
        pixels.resize(height);
        for (int i = 0; i < height; i++) {
            pixels[i].resize(width);
            for (int j = 0; j < width; j++) {
                int pos = (i * width + j) * 3;
                pixels[i][j] = RGB(data[pos], data[pos + 1], data[pos + 2]);
            }
        }
        stbi_image_free(data);
        return true;
    }



    void saveImage(const vector<vector<RGB>>& imgData, const string& filename) {
        int height = imgData.size();
        int width = imgData[0].size();
        vector<unsigned char> data(width * height * 3);
    
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int idx = (i * width + j) * 3;
                data[idx] = imgData[i][j].r;
                data[idx + 1] = imgData[i][j].g;
                data[idx + 2] = imgData[i][j].b;
            }
        }
        stbi_write_jpg(filename.c_str(), width, height, 3, data.data(), 70);
    }


    vector<vector<RGB>> getPixels() const { return pixels; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int pixelCount() const { return N; }
    long getFileSize(const string& filename) {
        ifstream file(filename, ios::binary | ios::ate);
        return file.tellg();
    }
};

#endif
