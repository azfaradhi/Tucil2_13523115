<h1 align="center">Tugas Kecil 2 IF2211 Strategi Algoritma</h1>
<h2 align="center">Semester II tahun 2024/2025</h2>
<h3 align="center">Image Compression Using Quadtree</h3>

<p align="center">
  <img src="test/gif/spidermanVariance.gif" alt="Main" width="400">
</p>

## Table of Contents
- [Description](#description)
- [Implemented Divide and Conquere Algorithms](#implemented-divide-and-conquer-algorithms)
- [Program Structure](#program-structure)
- [Requirements & Installation](#requirements--installation)
- [Author](#author)
- [References](#references)

## Description
This project implements image compression using the **Quadtree** method based on the **Divide and Conquer** paradigm. The algorithm works by recursively dividing an image into four quadrants until each region is uniform or meets a predefined similarity threshold. This structure is efficient for compressing images with large homogeneous areas.


## Implemented Divide and Conquer Algorithms
- **Quadtree-based image decomposition**: The image is divided into 4 equal quadrants recursively.
- **Homogeneity check**: At each region, a color similarity function is used to decide whether the area can be considered "uniform".
- **Recursive tree construction**: If the region is not uniform, the algorithm continues splitting until base conditions are met.

## Program Structure
```
.
├── bin
│   └── main
├── display.gif
├── doc
├── makefile
├── README.md
├── src
│   ├── header
│   │   ├── gif.cpp
│   │   ├── ImageExtract.hpp
│   │   ├── QuadTree.hpp
│   │   └── QuadTreeNode.hpp
│   ├── main.cpp
│   ├── modules
│   │   ├── ImageDetection.cpp
│   │   ├── QuadTree.cpp
│   │   └── stb_image_impl.cpp
│   └── stb_image
│       ├── stb_image.hpp
│       └── stb_image_write.hpp
└── test
    ├── gif
    ├── image
    └── original
```
- **src** : contains implementation of the algorithm
- **doc** : contains the assignment report and program documentation.

## Requirements & Installation
Before running the program, ensure that the following dependencies are installed:

### Requirements
- Minimum C++ 17 compiler
- `make`
- `stb_image` and `stb_image_write` libraries (already included)
- GIF encoding support (provided via `gif.cpp`)
- Linux/WSL or Unix-based system recommended

### Installation
Clone the repository and navigate to the project root directory:
```bash
git clone https://github.com/azfaradhi/Tucil2_13523115
```
Change the directory
```bash
cd Tucil2_13523115
```
Run makefile
Change the directory
```bash
make run
```
Change directory into bin
Change the directory
```bash
cd bin
```
Run the program
Change the directory
```bash
./main
```

## Author
Azfa Radhiyya Hakim, 13523115

## References
- [Slide Kuliah IF2211 2024/2025 Algoritma Divide and Conquer (Bagian 1)](https://informatika.stei.itb.ac.id/~rinaldi.munir/Stmik/2024-2025/07-Algoritma-Divide-and-Conquer-(2025)-Bagian1.pdf)
- [Slide Kuliah IF2211 2024/2025 Algoritma Divide and Conquer (Bagian 2)](https://informatika.stei.itb.ac.id/~rinaldi.munir/Stmik/2024-2025/08-Algoritma-Divide-and-Conquer-(2025)-Bagian2.pdf)
- [Slide Kuliah IF2211 2024/2025 Algoritma Divide and Conquer (Bagian 3)](https://informatika.stei.itb.ac.id/~rinaldi.munir/Stmik/2024-2025/09-Algoritma-Divide-and-Conquer-(2025)-Bagian3.pdf)
- [Slide Kuliah IF2211 2024/2025 Algoritma Divide and Conquer (Bagian 4)](https://informatika.stei.itb.ac.id/~rinaldi.munir/Stmik/2024-2025/10-Algoritma-Divide-and-Conquer-(2025)-Bagian4.pdf)
- [Image quality assessment: from error visibility to structural similarity](https://ieeexplore.ieee.org/document/1284395)
- [Studio encoding parameters of digital television for standard 4:3 and wide-screen 16:9 aspect ratios](https://www.itu.int/dms_pubrec/itu-r/rec/bt/r-rec-bt.601-7-201103-i!!pdf-e.pdf)

## Credits

All rights to the image and character belong to their respective owners.  
The image used in this project used for educational and non-commercial purposes only.
