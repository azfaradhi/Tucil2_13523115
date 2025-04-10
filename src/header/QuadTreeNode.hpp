
/*
- Class QuadTreeNode digunakan sebagai pondasi utama dari algoritama DNC
- Memiliki atribut x,y sebagai posisi kiri atas dari node terhadapt pixel,
- Memiliki atribut height dan width sebagai ukuran dari node yang sudah dibagi
- Memiliki atribut averageColor untuk menyimpan warna rata-rata dari node tersebut, diisi ketika node yang bersangkutan adakah daun
- Memiliki atribut isLeaf sebagai penanda jika node adalah sebuah daun
- shared_ptr<QuadTreeNode> topLeft, topRight, bottomLeft, bottomRight untuk menyimpan anak dari node tersebut
*/

#ifndef _QUADTREE_NODE_HPP_
#define _QUADTREE_NODE_HPP_

#include <iostream>
#include <memory>
#include "ImageExtract.hpp"


class QuadTreeNode {
private:
    int x,y,height,width;
    RGB averageColor;
    bool isLeaf;
    shared_ptr<QuadTreeNode> topLeft;
    shared_ptr<QuadTreeNode> topRight;
    shared_ptr<QuadTreeNode> bottomLeft;
    shared_ptr<QuadTreeNode> bottomRight;
public:
    QuadTreeNode(int x, int y, int height, int width, RGB color, bool isLeaf) {
        this->x = x;
        this->y = y;
        this->height = height;
        this->width = width;
        this->averageColor = color;
        this->isLeaf = isLeaf;
        this->topLeft = nullptr;
        this->topRight = nullptr;
        this->bottomLeft = nullptr;
        this->bottomRight = nullptr;
    }

    ~QuadTreeNode(){};

    void setChildren(
        std::shared_ptr<QuadTreeNode> tl, 
        std::shared_ptr<QuadTreeNode> tr, 
        std::shared_ptr<QuadTreeNode> bl, 
        std::shared_ptr<QuadTreeNode> br
    ) 
    {
        topLeft = tl;
        topRight = tr;
        bottomLeft = bl;
        bottomRight = br;
        isLeaf = false;
    }


    // Metode getter
    int getX() const { return x; }
    int getY() const { return y; }
    int getHeight() const { return height; }
    int getWidth() const { return width; }
    RGB getAverageColor() const { return averageColor; }
    bool getIsLeaf() const { return isLeaf; }
    void setAverageColor(RGB color) { averageColor = color; }
    
    // Mengambil node dari anak
    shared_ptr<QuadTreeNode> getTopLeft() const { return topLeft; }
    shared_ptr<QuadTreeNode> getTopRight() const { return topRight; }
    shared_ptr<QuadTreeNode> getBottomLeft() const { return bottomLeft; }
    shared_ptr<QuadTreeNode> getBottomRight() const { return bottomRight; }
};

#endif