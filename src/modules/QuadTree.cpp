#include "../header/QuadTree.hpp"
#include "../header/gif.cpp"
#include <math.h>

QuadTree::QuadTree(double threshold, int minSize, int errorMethod){
    this->root = nullptr;
    this->threshold = threshold;
    this->minSize = minSize;
    this->errorMethod = errorMethod;
}

RGB QuadTree::calculateAverage(int x, int y, int height, int width){
    double avgR = 0;
    double avgB = 0;
    double avgG = 0;

    for (int i = y; i < y + height; i++) {
        for (int j = x; j < x + width; j++) {
            avgR += pixels[i][j].r;
            avgG += pixels[i][j].g;
            avgB += pixels[i][j].b;
        }
    }
    return RGB(avgR/(height*width), avgG/(height*width), avgB/(height*width));
}

double QuadTree::errorVariance(int x, int y, int height, int width) {

    RGB avg = calculateAverage(x, y, height, width);

    double variance = 0;

    for (int i = y; i < y + height; i++) {
        for (int j = x; j < x + width; j++) {
            variance += ((pixels[i][j].r - avg.r) * (pixels[i][j].r - avg.r)) + ((pixels[i][j].g - avg.g) * (pixels[i][j].g - avg.g)) + ((pixels[i][j].b - avg.b) * (pixels[i][j].b - avg.b));
        }
    }
    return variance / (height * width * 3);
}

double QuadTree::errorMAD(int x, int y, int height, int width) {

    RGB avg = calculateAverage(x, y, height, width);

    double mad = 0;

    for (int i = y; i < y + height; i++) {
        for (int j = x; j < x + width; j++) {
            mad += abs(pixels[i][j].r - avg.r) + abs(pixels[i][j].g - avg.g) + abs(pixels[i][j].b - avg.b);
        }
    }
    return mad / (height * width * 3);
}

double QuadTree::errorMaxPixelDifference(int x, int y, int height, int width) {
    unsigned char minR = 255, minG = 255, minB = 255;
    unsigned char maxR = 0, maxG = 0, maxB = 0;

    for (int i = y; i < y + height; ++i) {
        for (int j = x; j < x + width; ++j) {
            if (pixels[i][j].r < minR) minR = pixels[i][j].r;
            if (pixels[i][j].g < minG) minG = pixels[i][j].g;
            if (pixels[i][j].b < minB) minB = pixels[i][j].b;
            if (pixels[i][j].r > maxR) maxR = pixels[i][j].r;
            if (pixels[i][j].g > maxG) maxG = pixels[i][j].g;
            if (pixels[i][j].b > maxB) maxB = pixels[i][j].b;
        }
    }
    return ((maxR - minR) + (maxG - minG) + (maxB - minB)) / 3;
}

double QuadTree::errorEntropy(int x, int y, int height, int width) {
    unordered_map<int,int> freqR, freqG, freqB;
    const int total = width * height;

    for (int i = y; i < y + height; i++) {
        for (int j = x; j < x + width; j++) {
            freqR[pixels[i][j].r]++;
            freqG[pixels[i][j].g]++;
            freqB[pixels[i][j].b]++;
        }
    }

    double entropyR = 0.0;
    double entropyG = 0.0;
    double entropyB = 0.0;

    for (const auto& pair : freqR) {
        double p = (double)pair.second / total;
        entropyR -= p * log2(p);
    }
    for (const auto& pair : freqG) {
        double p = (double)pair.second / total;
        entropyG -= p * log2(p);
    }
    for (const auto& pair : freqB) {
        double p = (double)pair.second / total;
        entropyB -= p * log2(p);
    }

    return (entropyR + entropyG + entropyB) / 3;
}

double QuadTree::errorSSIM(int x, int y, int height, int width) {

    RGB avg = calculateAverage(x, y, height, width);
    double uRx = avg.r;
    double uGx = avg.g;
    double uBx = avg.b;
    double uRy = avg.r;
    double uGy = avg.g;
    double uBy = avg.b;


    double sigmaRy = 0;
    double sigmaGy = 0;
    double sigmaBy = 0;
    double c1 = 6.5025;
    double c2 = 58.5225;

    double sigmaRx = 0;
    double sigmaGx = 0;
    double sigmaBx = 0;

    for (int i = y; i < y + height; i++){
        for (int j = x; j < x+width; j++){
            sigmaRx += (pixels[i][j].r - uRx) * (pixels[i][j].r - uRx);
            sigmaGx += (pixels[i][j].g - uGx) * (pixels[i][j].g - uGx);
            sigmaBx += (pixels[i][j].b - uBx) * (pixels[i][j].b - uBx);
        }
    }
    sigmaRx /= (height * width);
    sigmaGx /= (height * width);
    sigmaBx /= (height * width);

    double ssimr = ((2 * uRx * uRy + c1) * c2) / (((uRx * uRx) + (uRy * uRy) + c1) * (sigmaRx + c2));
    double ssimg = ((2 * uGx * uGy + c1) * c2) / (((uGx * uGx) + (uGy * uGy) + c1) * (sigmaGx + c2));
    double ssimb = ((2 * uBx * uBy + c1) * c2) / (((uBx * uBx) + (uBy * uBy) + c1) * (sigmaBx + c2));

    return (ssimr + ssimg + ssimb) / 3; 
}

shared_ptr<QuadTreeNode> QuadTree::makeLeaf(int x, int y, int height, int width) {
    return make_shared<QuadTreeNode>(x, y, height, width, calculateAverage(x,y,height,width), true);
}

shared_ptr<QuadTreeNode> QuadTree::buildQuadTree(int x, int y, int width, int height, double threshold, int minSize, int errorMethod) {
    
    if (width * height <= minSize){
        return makeLeaf(x,y,height,width);
    }
    
    double error = 0;
    if (errorMethod == 1){
        error = errorVariance(x,y,height,width);
    }
    else if (errorMethod == 2){
        error = errorMAD(x,y,height,width);
    }
    else if (errorMethod == 3){
        error = errorMaxPixelDifference(x,y,height,width);
    }
    else if (errorMethod == 4){
        error = errorEntropy(x,y,height,width);
    }
    else if (errorMethod == 5){
        error = errorSSIM(x,y,height,width);
    }
    else{
        cout << "Error: Metode error tidak valid!" << endl;
        return nullptr;
    }

    if (errorMethod == 5){
        if (error >= threshold){
            return makeLeaf(x,y,height,width);
        }
    }
    else {
        if (error <= threshold){
            return makeLeaf(x,y,height,width);
        }
    }

    int halfW = width/2;
    int resW = width % 2;
    int halfH = height/2;
    int resH = height % 2;

    if ((halfW * halfH) < minSize || (halfW + resW) * (halfH + resH) < minSize || (halfW + resW) * halfH < minSize || halfW * (halfH + resH) < minSize){
        return makeLeaf(x,y,height,width);
    }
    shared_ptr<QuadTreeNode> tl = buildQuadTree(x, y, halfW, halfH, threshold, minSize, errorMethod);
    shared_ptr<QuadTreeNode> tr = buildQuadTree(x + halfW, y, halfW + resW, halfH, threshold, minSize, errorMethod);
    shared_ptr<QuadTreeNode> bl = buildQuadTree(x, y + halfH, halfW, halfH + resH, threshold, minSize, errorMethod);
    shared_ptr<QuadTreeNode> br = buildQuadTree(x + halfW, y + halfH, halfW + resW, halfH + resH, threshold, minSize, errorMethod);
    
    shared_ptr<QuadTreeNode> node = make_shared<QuadTreeNode>(x, y, height, width, calculateAverage(x,y,height,width), false);
    node->setChildren(tl, tr, bl, br);
    node->setAverageColor(calculateAverage(x,y,height,width));
    return node;
}

void QuadTree::startDNC(const vector<vector<RGB>>& img) {

    this->pixels = img;
    int height = img.size();
    int width = img[0].size();
    
    this->root = buildQuadTree(0, 0, width, height, threshold, minSize, errorMethod);
}

vector<vector<RGB>> QuadTree::reconstructImage(int width, int height) {
    vector<vector<RGB>> result(height, vector<RGB>(width));
    if (root) {
        constructImageRec(root, result);
    }
    return result;
}

void QuadTree::constructImageRec(shared_ptr<QuadTreeNode> node, vector<vector<RGB>>& result) {
    if (!node) {
        return;
    }
    
    if (node->getIsLeaf()) {
        for (int j = node->getY(); j < min(node->getY() + node->getHeight(), (int)result.size()); j++) {
            for (int i = node->getX(); i < min(node->getX() + node->getWidth(), (int)result[0].size()); i++) {
                result[j][i] = node->getAverageColor();
            }
        }
    } else {
        constructImageRec(node->getTopLeft(), result);
        constructImageRec(node->getTopRight(), result);
        constructImageRec(node->getBottomLeft(), result);
        constructImageRec(node->getBottomRight(), result);
    }
}

int QuadTree::countNodes(shared_ptr<QuadTreeNode> node) const {
    if (!node) return 0;
    
    if (node->getIsLeaf()) {
        return 1;
    }
    
    return 1 + countNodes(node->getTopLeft()) + countNodes(node->getTopRight()) + countNodes(node->getBottomLeft()) + countNodes(node->getBottomRight());
}

int QuadTree::totalNodes() const {
    return countNodes(root);
}

int QuadTree::countLeafNodes(shared_ptr<QuadTreeNode> node) const {
    if (!node) return 0;
    
    if (node->getIsLeaf()) {
        return 1;
    }
    
    return countLeafNodes(node->getTopLeft()) + countLeafNodes(node->getTopRight()) + countLeafNodes(node->getBottomLeft()) + countLeafNodes(node->getBottomRight());
}

int QuadTree::leafNodes() const {
    return countLeafNodes(root);
}

int QuadTree::countDepth(shared_ptr<QuadTreeNode> node) const {
    if (!node){
        return 0;
    }
    if (node->getIsLeaf()){
        return 1;
    }

    return 1 + max(countDepth(node->getTopLeft()), max(countDepth(node->getTopRight()), max(countDepth(node->getBottomLeft()), countDepth(node->getBottomRight()))));
}

int QuadTree::depth() const {
    return countDepth(root);
}

void QuadTree::createCompressionGIF(const string& outputPath, int width, int height) {
    int imgWidth = width;
    int imgHeight = height;
    
    GifWriter g;
    GifBegin(&g, outputPath.c_str(), imgWidth, imgHeight, 75);
    
    vector<vector<shared_ptr<QuadTreeNode>>> levelNodes;
    queue<shared_ptr<QuadTreeNode>> q;
    queue<int> levels;
    
    if (root) {
        q.push(root);
        levels.push(0);
    }
    
    int maxl = 0;
    while (!q.empty()) {
        auto node = q.front();
        int level = levels.front();
        q.pop();
        levels.pop();
        
        while (levelNodes.size() <= level) {
            levelNodes.push_back(vector<shared_ptr<QuadTreeNode>>());
        }
        
        levelNodes[level].push_back(node);
        maxl = max(maxl, level);
        
        if (!node->getIsLeaf()) {
            if (node->getTopLeft()) {
                q.push(node->getTopLeft());
                levels.push(level + 1);
            }
            if (node->getTopRight()) {
                q.push(node->getTopRight());
                levels.push(level + 1);
            }
            if (node->getBottomLeft()) {
                q.push(node->getBottomLeft());
                levels.push(level + 1);
            }
            if (node->getBottomRight()) {
                q.push(node->getBottomRight());
                levels.push(level + 1);
            }
        }
    }
    
    vector<uint8_t> firstFrame(imgWidth * imgHeight * 4);
    RGB rootColor = root->getAverageColor();
    for (int y = 0; y < imgHeight; y++) {
        for (int x = 0; x < imgWidth; x++) {
            int idx = (y * imgWidth + x) * 4;
            firstFrame[idx] = rootColor.r;
            firstFrame[idx + 1] = rootColor.g;
            firstFrame[idx + 2] = rootColor.b;
            firstFrame[idx + 3] = 255;
        }
    }
    GifWriteFrame(&g, firstFrame.data(), imgWidth, imgHeight, 75);
    
    vector<vector<RGB>> curentFrame(imgHeight, vector<RGB>(imgWidth, rootColor));
    
    for (int level = 1; level <= maxl; level++) {
        for (auto& node : levelNodes[level]) {
            int nodeX = node->getX();
            int nodeY = node->getY();
            int nodeWidth = node->getWidth();
            int nodeHeight = node->getHeight();
            RGB nodeColor = node->getAverageColor();
            
            for (int y = nodeY; y < min(nodeY + nodeHeight, imgHeight); y++) {
                for (int x = nodeX; x < min(nodeX + nodeWidth, imgWidth); x++) {
                    curentFrame[y][x] = nodeColor;
                }
            }
        }
        
        vector<uint8_t> frame(imgWidth * imgHeight * 4);
        for (int y = 0; y < imgHeight; y++) {
            for (int x = 0; x < imgWidth; x++) {
                int idx = (y * imgWidth + x) * 4;
                frame[idx] = curentFrame[y][x].r;
                frame[idx + 1] = curentFrame[y][x].g;
                frame[idx + 2] = curentFrame[y][x].b;
                frame[idx + 3] = 255;
            }
        }
        
        GifWriteFrame(&g, frame.data(), imgWidth, imgHeight, 75);
    }
    
    GifEnd(&g);
}