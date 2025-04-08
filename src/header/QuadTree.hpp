
/*
- Class QuadTree digunakan untuk menyimpan quadtree yang telah dibuat
- Memiliki atribut root, sebagai alamat awal dari quadtree yang dibuat
- Memiliki atribut threshold, minSize, errorMethod untuk menyimpan parameter yang digunakan dalam algoritma DNC
- Atribut pixels digunakan untuk menyimpan data pixel yang didapat dari gambar asli
*/
#ifndef _QUADTREE_HPP_
#define _QUADTREE_HPP_

#include <iostream>
#include <memory>
#include <unordered_map>
#include <math.h>
#include <queue>


#include "ImageExtract.hpp"
#include "QuadTreeNode.hpp"

using namespace std;

class QuadTree{

private:

    shared_ptr<QuadTreeNode> root;
    double threshold;
    int minSize;
    int errorMethod;
    vector<vector<RGB>> pixels;

public:

    QuadTree(double threshold, int minSize, int errorMethod);
    // Ini adalah konstruktor untuk membangun quadtree

    RGB calculateAverage(int x, int y, int height, int width);
    // Metode yang digunakan untuk menghitung rata-rata warna dari tiap elemen RGB pada bagian pixel yang bersesuaian

    double errorVariance(int x, int y, int height, int width);
    double errorMAD(int x, int y, int height, int width);
    double errorMaxPixelDifference(int x, int y, int height, int width);
    double errorEntropy(int x, int y, int height, int width);
    double errorSSIM(int x, int y, int height, int width);
    // Lima metode error yang berbeda berdasarkan parameter threshold dan minSize yang diberikan

    shared_ptr<QuadTreeNode> makeLeaf(int x, int y, int width, int height);
    // Dipanggil ketika suatu node sudah tidak dapat dibagi berdasarkan aturan yang berlaku

    shared_ptr<QuadTreeNode> buildQuadTree(int x, int y, int width, int height, double threshold, int minSize, int errorMethod); 
    // Jika node masih dapat dibagi, maka akan membagi node tersebut menjadi 4 bagian, yaitu topLeft, topRight, bottomLeft, dan bottomRight

    void startDNC(const vector<vector<RGB>>& originalImage);
    // Fungsi yang dipanggil pada main dan sebagai konstruktor pixels dari class

    vector<vector<RGB>> reconstructImage(int width, int height);
    // Fungsi yang dipanggi pada main untuk merekontruksi ulang gambar hasil kompresi

    void constructImageRec(shared_ptr<QuadTreeNode> node, vector<vector<RGB>>& result); 
    // Fungsi rekursif yang digunakan untuk mengisi gambar hasil kompresi

    int countNodes(shared_ptr<QuadTreeNode> node) const;
    int totalNodes() const;
    // Menghitung jumlah nodes dalam quadtree

    int countLeafNodes(shared_ptr<QuadTreeNode> node) const;
    int leafNodes() const;
    // Menghitung jumlah node daun dalam quadtree

    int countDepth(shared_ptr<QuadTreeNode> node) const;
    int depth() const;
    // Menghitung kedalaman quadtree
    
    shared_ptr<QuadTreeNode> getRoot() const { return root; }
    // Mengambil alamat root

    void createCompressionGIF(const std::string& outputPath, int width, int height);
};

#endif
