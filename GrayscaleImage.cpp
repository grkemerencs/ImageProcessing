#include "GrayscaleImage.h"
#include <iostream>
#include <cstring>  // For memcpy
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdexcept>


// Constructor: load from a file
GrayscaleImage::GrayscaleImage(const char* filename) {

    // Image loading code using stbi
    int channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_grey);

    if (image == nullptr) {
        std::cerr << "Error: Could not load image " << filename << std::endl;
        exit(1);
    }

    // TODO: Your code goes here.
    // Dynamically allocate memory for a 2D matrix based on the given dimensions.
    data = new int*[height];
    for(int i = 0; i < height; i++) {
        data[i] = new int[width];
    }
    // Fill the matrix with pixel values from the image
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            data[i][j] = image[i * width + j];
        }
    }
    // Free the dynamically allocated memory of stbi image
    stbi_image_free(image);
}

// Constructor: initialize from a pre-existing data matrix
GrayscaleImage::GrayscaleImage(int** inputData, int h, int w) {
    // TODO: Your code goes here.
    // Initialize the image with a pre-existing data matrix by copying the values.
    data = new int*[h];
    for(int i = 0; i < h; i++) {
        data[i] = new int[w];
        memcpy(data[i], inputData[i], w * sizeof(int));
    }
    // Don't forget to dynamically allocate memory for the matrix.
}

// Constructor to create a blank image of given width and height
GrayscaleImage::GrayscaleImage(int w, int h) : width(w), height(h) {
    // TODO: Your code goes here.
    // Just dynamically allocate the memory for the new matrix.
    data = new int*[height];
    for(int i = 0; i < height; i++) {
        data[i] = new int[width];
    }
}


// Copy constructor
GrayscaleImage::GrayscaleImage(const GrayscaleImage& other) :width(other.width), height(other.height) {
    // TODO: Your code goes here.
    // Copy constructor: dynamically allocate memory and
    // copy pixel values from another image.
    data = new int*[height];
    for(int i = 0; i < height; i++) {
        data[i] = new int[width];
    }
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            data[i][j] = other.data[i][j];
        }
    }
}

// Destructor
GrayscaleImage::~GrayscaleImage() {
    // TODO: Your code goes here.
    // Destructor: deallocate memory for the matrix.
    for(int i = 0; i < height; i++) {
        delete[] data[i];
    }
    delete[] data;
 }

// Equality operator
bool GrayscaleImage::operator==(const GrayscaleImage& other) const {
    // TODO: Your code goes here.
    // Check if two images have the same dimensions and pixel values.
    if (height == other.height && width == other.width) {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (data[i][j] != other.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
    // If they do, return true.
}

// Addition operator
GrayscaleImage GrayscaleImage::operator+(const GrayscaleImage& other) const {
    // Create a new image for the result
    GrayscaleImage result(width, height);
    
    // TODO: Your code goes here.
    if (height != other.height || width != other.width) {
        throw std::invalid_argument("Images must have same dimensions.");
    }
    for(int i = 0; i < height ; i++) {
        for(int j = 0; j < width; j++) {
            int grayTone = data[i][j] + other.data[i][j];
            if (grayTone > 255) {
                grayTone = 255;
            }
            result.data[i][j] = grayTone;
        }
    }
    // Add two images' pixel values and return a new image, clamping the results.
    return result;
}

// Subtraction operator
GrayscaleImage GrayscaleImage::operator-(const GrayscaleImage& other) const {
    // Create a new image for the result
    GrayscaleImage result(width, height);
    
    // TODO: Your code goes here.
    // Subtract pixel values of two images and return a new image, clamping the results.
    if (height != other.height || width != other.width) {
        throw std::invalid_argument("Images must have same dimensions.");
    }
    for(int i = 0; i < height ; i++) {
        for(int j = 0; j < width; j++) {
            int grayTone = data[i][j] - other.data[i][j];
            if (grayTone < 0) {
                grayTone = 0;
            }
            result.data[i][j] = grayTone;
        }
    }
    return result;
}

// Get a specific pixel value
int GrayscaleImage::get_pixel(int row, int col) const {
    return data[row][col];
}

// Set a specific pixel value
void GrayscaleImage::set_pixel(int row, int col, int value) {
    data[row][col] = value;
}

// Function to save the image to a PNG file
void GrayscaleImage::save_to_file(const char* filename) const {
    // Create a buffer to hold the image data in the format stb_image_write expects
    unsigned char* imageBuffer = new unsigned char[width * height];

    // Fill the buffer with pixel data (convert int to unsigned char)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            imageBuffer[i * width + j] = static_cast<unsigned char>(data[i][j]);
        }
    }

    // Write the buffer to a PNG file
    if (!stbi_write_png(filename, width, height, 1, imageBuffer, width)) {
        std::cerr << "Error: Could not save image to file " << filename << std::endl;
    }

    // Clean up the allocated buffer
    delete[] imageBuffer;
}

