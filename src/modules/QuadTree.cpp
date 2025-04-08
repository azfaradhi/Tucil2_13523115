#include "../header/QuadTree.hpp"
#include "../header/gif.cpp"

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

    if (error <= threshold){
        return makeLeaf(x,y,height,width);
    }

    int halfW = width/2;
    int resW = width % 2;
    int halfH = height/2;
    int resH = height % 2;

    shared_ptr<QuadTreeNode> tl = buildQuadTree(x, y, halfW, halfH, threshold, minSize, errorMethod);
    shared_ptr<QuadTreeNode> tr = buildQuadTree(x + halfW, y, halfW + resW, halfH, threshold, minSize, errorMethod);
    shared_ptr<QuadTreeNode> bl = buildQuadTree(x, y + halfH, halfW, halfH + resH, threshold, minSize, errorMethod);
    shared_ptr<QuadTreeNode> br = buildQuadTree(x + halfW, y + halfH, halfW + resW, halfH + resH, threshold, minSize, errorMethod);
    
    shared_ptr<QuadTreeNode> node = make_shared<QuadTreeNode>(x, y, height, width, calculateAverage(x,y,height,width), false);
    node->setChildren(tl, tr, bl, br);
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


void QuadTree::drawQuadTreeBoundaries(vector<vector<RGB>>& image, shared_ptr<QuadTreeNode> node, const RGB& boundaryColor) {
    if (!node) return;
    
    int x = node->getX();
    int y = node->getY();
    int w = node->getWidth();
    int h = node->getHeight();
    
    for (int i = x; i < min(x + w, (int)image[0].size()); i++) {
        if (y >= 0 && y < (int)image.size()) {
            image[y][i] = boundaryColor;
        }
        if (y + h - 1 >= 0 && y + h - 1 < (int)image.size()) {
            image[y + h - 1][i] = boundaryColor;
        }
    }
    
    for (int j = y; j < min(y + h, (int)image.size()); j++) {
        if (x >= 0 && x < (int)image[0].size()) {
            image[j][x] = boundaryColor;
        }
        if (x + w - 1 >= 0 && x + w - 1 < (int)image[0].size()) {
            image[j][x + w - 1] = boundaryColor;
        }
    }
    
    if (!node->getIsLeaf()) {
        drawQuadTreeBoundaries(image, node->getTopLeft(), boundaryColor);
        drawQuadTreeBoundaries(image, node->getTopRight(), boundaryColor);
        drawQuadTreeBoundaries(image, node->getBottomLeft(), boundaryColor);
        drawQuadTreeBoundaries(image, node->getBottomRight(), boundaryColor);
    }
}

vector<vector<RGB>> QuadTree::getQuadTreeVisualization(int width, int height) {
    vector<vector<RGB>> visualization = reconstructImage(width, height);
    
    RGB black(0, 0, 0);
    drawQuadTreeBoundaries(visualization, root, black);
    
    return visualization;
}

void QuadTree::createCompressionGIF(const std::string& outputPath, int width, int height) {

    int imgWidth = width;
    int imgHeight = height;
    
    GifWriter g;
    GifBegin(&g, outputPath.c_str(), imgWidth, imgHeight, 50);
    
    std::queue<std::shared_ptr<QuadTreeNode>> nodeQueue;
    std::vector<std::shared_ptr<QuadTreeNode>> allNodes;
    
    if (root) {
        nodeQueue.push(root);
    }
    
    std::vector<uint8_t> originalFrame(imgWidth * imgHeight * 4);
    for (int y = 0; y < imgHeight; y++) {
        for (int x = 0; x < imgWidth; x++) {
            int idx = (y * imgWidth + x) * 4;
            originalFrame[idx] = pixels[y][x].r;
            originalFrame[idx + 1] = pixels[y][x].g;
            originalFrame[idx + 2] = pixels[y][x].b;
        }
    }
    GifWriteFrame(&g, originalFrame.data(), imgWidth, imgHeight, 50);
    
    std::vector<std::vector<std::shared_ptr<QuadTreeNode>>> levelNodes;
    
    while (!nodeQueue.empty()) {
        int levelSize = nodeQueue.size();
        std::vector<std::shared_ptr<QuadTreeNode>> currentLevel;
        
        for (int i = 0; i < levelSize; i++) {
            auto node = nodeQueue.front();
            nodeQueue.pop();
            currentLevel.push_back(node);
            allNodes.push_back(node);
            
            if (!node->getIsLeaf()) {
                if (node->getTopLeft()) nodeQueue.push(node->getTopLeft());
                if (node->getTopRight()) nodeQueue.push(node->getTopRight());
                if (node->getBottomLeft()) nodeQueue.push(node->getBottomLeft());
                if (node->getBottomRight()) nodeQueue.push(node->getBottomRight());
            }
        }
        
        if (!currentLevel.empty()) {
            levelNodes.push_back(currentLevel);
        }
    }
    
    for (size_t level = 0; level < levelNodes.size(); level++) {
        std::vector<std::vector<RGB>> reconstruction = pixels;        
        for (size_t l = 0; l <= level; l++) {
            for (auto& node : levelNodes[l]) {
                if (node->getIsLeaf()) {
                    for (int y = node->getY(); y < std::min(node->getY() + node->getHeight(), imgHeight); y++) {
                        for (int x = node->getX(); x < std::min(node->getX() + node->getWidth(), imgWidth); x++) {
                            reconstruction[y][x] = node->getAverageColor();
                        }
                    }
                }
            }
        }

        std::vector<uint8_t> frame(imgWidth * imgHeight * 4);
        for (int y = 0; y < imgHeight; y++) {
            for (int x = 0; x < imgWidth; x++) {
                int idx = (y * imgWidth + x) * 4;
                frame[idx] = reconstruction[y][x].r;
                frame[idx + 1] = reconstruction[y][x].g;
                frame[idx + 2] = reconstruction[y][x].b;
                frame[idx + 3] = 255;
            }
        }
        
        GifWriteFrame(&g, frame.data(), imgWidth, imgHeight, 50);
    }
    
    std::vector<std::vector<RGB>> finalReconstruction = reconstructImage(imgWidth, imgHeight);
    std::vector<uint8_t> finalFrame(imgWidth * imgHeight * 4);
    for (int y = 0; y < imgHeight; y++) {
        for (int x = 0; x < imgWidth; x++) {
            int idx = (y * imgWidth + x) * 4;
            finalFrame[idx] = finalReconstruction[y][x].r;
            finalFrame[idx + 1] = finalReconstruction[y][x].g;
            finalFrame[idx + 2] = finalReconstruction[y][x].b;
            finalFrame[idx + 3] = 255;
        }
    }
    
    GifWriteFrame(&g, finalFrame.data(), imgWidth, imgHeight, 50);
    
    GifEnd(&g);
}