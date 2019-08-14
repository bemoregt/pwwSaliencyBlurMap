#include "ofApp.h"

using namespace cv;
using namespace ofxCv;
using namespace std;

typedef vector<double> Array;
typedef vector<Array> Matrix;
typedef vector<Matrix> Image;

// proto type --------------------------------------------------
Matrix getGaussian(int height, int width, double sigma);
Image loadImage(const char *filename);
void saveImage(Image &image, const char *filename);
// pww
//Image applyFilter(Image &image);
Image applyFilter(Image &image, Image &mask);
Image applyFilter(Image &image, Matrix &filter, int times);

//--------------------------------------------------------------
void ofApp::setup(){

    //Matrix filter = getGaussian(5, 5, 10.0);
    
    cout << "Loading image..." << endl;
    Image image = loadImage("/Users/mun/Desktop/smile.png");
    Image mask = loadImage("/Users/mun/Desktop/sal.png");
    cout << "Applying filter..." << endl;
    
    Image newImage = applyFilter(image, mask);
    cout << "Saving image..." << endl;
    
    saveImage(newImage, "/Users/mun/Desktop/result.png");
    cout << "Done!" << endl;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
//-----------------------------------------------------------
Matrix getGaussian(int height, int width, double sigma)
{
    Matrix kernel(height, Array(width));
    double sum=0.0;
    int i,j;
    
    for (i=0 ; i<height ; i++) {
        for (j=0 ; j<width ; j++) {
            kernel[i][j] = exp(-(i*i+j*j)/(2*sigma*sigma))/(2*M_PI*sigma*sigma);
            sum += kernel[i][j];
        }
    }
    
    for (i=0 ; i<height ; i++) {
        for (j=0 ; j<width ; j++) {
            kernel[i][j] /= sum;
        }
    }
    
    return kernel;
}

// ------------------------------------------------------------
Image loadImage(const char *filename)
{
    png::image<png::rgb_pixel> image(filename);
    Image imageMatrix(3, Matrix(image.get_height(), Array(image.get_width())));
    
    int h,w;
    for (h=0 ; h<image.get_height() ; h++) {
        for (w=0 ; w<image.get_width() ; w++) {
            imageMatrix[0][h][w] = image[h][w].red;
            imageMatrix[1][h][w] = image[h][w].green;
            imageMatrix[2][h][w] = image[h][w].blue;
        }
    }
    
    return imageMatrix;
}

//-------------------------------------------------------
void saveImage(Image &image, const char *filename)
{
    assert(image.size()==3);
    
    int height = image[0].size();
    int width = image[0][0].size();
    int x,y;
    
    png::image<png::rgb_pixel> imageFile(width, height);
    
    for (y=0 ; y<height ; y++) {
        for (x=0 ; x<width ; x++) {
            imageFile[y][x].red = image[0][y][x];
            imageFile[y][x].green = image[1][y][x];
            imageFile[y][x].blue = image[2][y][x];
        }
    }
    imageFile.write(filename);
}

// pww -----------------------------------------------------
Image applyFilter(Image &image, Image &mask)
{
    Matrix filter = getGaussian(9, 9, 10.0);
    Matrix filter2 = getGaussian(1, 1, 10.0);
    assert(image.size()==3 && filter.size()!=0);
    
    int height = image[0].size();
    int width = image[0][0].size();
    
    int filterHeight = filter.size();
    int filterWidth = filter[0].size();
    int filterHeight2 = filter2.size();
    int filterWidth2 = filter2[0].size();
    
    int newImageHeight = height-filterHeight+1;
    int newImageWidth = width-filterWidth+1;
    int d,i,j,h,w;
    
    Image newImage(3, Matrix(newImageHeight, Array(newImageWidth)));
    int kk=1;
    for (d=0 ; d<3 ; d++) {
        for (i=19 ; i<newImageHeight-19 ; i++) {
            for (j=19 ; j<newImageWidth-19 ; j++) {
                kk=(int)(mask[d][i][j]/255.0*19);
                kk= 19-kk;
                //ofLog() << ofToString(kk);
                if(kk % 2 ==0) kk+=1;
                
                if(kk <=1) kk=1;
                filter= getGaussian(kk, kk, 10.0);
                for (h=i ; h<i+kk ; h++) {
                    for (w=j ; w<j+kk ; w++) {
                        newImage[d][i][j] += filter[h-i][w-j]*image[d][h][w];
                    }//for w
                }// for h
            }//for j
        }//for i
    }//for d
    
    return newImage;
}
//
Image applyFilter(Image &image, Matrix &filter, int times)
{
    Image newImage = image;
    for(int i=0 ; i<times ; i++) {
        newImage = applyFilter(newImage, newImage);
    }
    return newImage;
}
//

