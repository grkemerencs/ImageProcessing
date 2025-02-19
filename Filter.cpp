#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>
#include <iostream>
#include <thread>

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    // TODO: Your code goes here.
    // 1. Copy the original image for reference.
    GrayscaleImage reference(image);
    int height = reference.get_height();
    int width = reference.get_width();
    int distanceFromTheCenterToCalculate = kernelSize/2;
    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    for(int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int total = 0;
            for(int k = i-distanceFromTheCenterToCalculate; k <= i+distanceFromTheCenterToCalculate; k++) {
                for(int m = j-distanceFromTheCenterToCalculate; m <= j+distanceFromTheCenterToCalculate; m++) {
                    if(0 <= k && k < height && 0 <= m && m < width) {
                        total+=reference.get_pixel(k,m);
                    }
                }
            }
            image.set_pixel(i,j,total/(kernelSize*kernelSize));
        } 
    }
    // 3. Update each pixel with the computed mean.
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    // TODO: Your code goes here.
    // 1. Create a Gaussian kernel based on the given sigma value.
    GrayscaleImage reference(image);
    double** kernel = new double*[kernelSize];
    for(int i = 0; i < kernelSize; i++) {
        kernel[i] = new double[kernelSize];   
    }
    double normalizer = 0;
    for(int i = 0; i < kernelSize; i++) {
        double distanceXsquared = pow(abs((kernelSize-1)/2-i),2);
        for (int j = 0; j < kernelSize; j++) {
            double distanceYsquared = pow(abs((kernelSize-1)/2-j),2);
            double gaussianResult = (1/(2*M_PI*sigma*sigma))*(exp(-((distanceXsquared+distanceYsquared)/(2*sigma*sigma))));
            kernel[i][j] = gaussianResult;
            normalizer += gaussianResult;
        }
    }
    // 2. Normalize the kernel to ensure it sums to 1.
    int height = reference.get_height();
    int width = reference.get_width();
    int distanceFromTheCenterToCalculate = kernelSize/2;
    for(int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double total = 0;
            int kernelRow = -1; // for tracking the kernels index location.
            int kernelColumn = -1;
            for(int k = i-distanceFromTheCenterToCalculate; k <= i+distanceFromTheCenterToCalculate; k++) { // k = row index.
                kernelRow++; 
                for(int m = j-distanceFromTheCenterToCalculate; m <= j+distanceFromTheCenterToCalculate; m++) { // m = column index.
                    if(kernelColumn == kernelSize-1) {
                        kernelColumn = 0;
                    }else {
                        kernelColumn++;
                    }
                    if(0 <= k && k < height && 0 <= m && m < width) {
                        total+=reference.get_pixel(k,m)*kernel[kernelRow][kernelColumn];
                    }
                }
            }
            image.set_pixel(i,j,int(total/normalizer)); // normalize the brightness.
        }
    }
    for(int i = 0; i < kernelSize; i++) {
        delete[] kernel[i];
    }
    delete[] kernel;
    // 3. For each pixel, compute the weighted sum using the kernel.
    // 4. Update the pixel values with the smoothed results.
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    // TODO: Your code goes here.
    // 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.
    // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).
    // 3. Clip values to ensure they are within a valid range [0-255].

    
    GrayscaleImage blurred(image);
    apply_gaussian_smoothing(blurred,kernelSize);
    for(int i = 0; i < image.get_height(); i++) {
        for (int j = 0; j < image.get_width(); j++) {
            double res = image.get_pixel(i,j)+amount*(image.get_pixel(i,j)-blurred.get_pixel(i,j));
            if(res > 255) {
                image.set_pixel(i,j,255);
            } else if (res < 0) {
                image.set_pixel(i,j,0);
            } else {
                image.set_pixel(i,j,int(res));
            }
        }
    }
}
