#include "Crypto.h"
#include "GrayscaleImage.h"


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    // 1. Reconstruct the SecretImage to a GrayscaleImage.
    GrayscaleImage image = secret_image.reconstruct();
    // 2. Calculate the image dimensions.
    int height = image.get_height();
    int width = image.get_width();
    // 3. Determine the total bits required based on message length.
    int totalBits = message_length*7;
    // 4. Ensure the image has enough pixels; if not, throw an error.
    if(height*width/7 < totalBits/7) {
        throw std::invalid_argument("Image doesn't have enough pixels.");
    }
    // 5. Calculate the starting pixel from the message_length knowing that  
    //    the last LSB to extract is in the last pixel of the image.
    int rowIndex = height-(totalBits/width+1);
    int columnIndex = width-(totalBits%width);
    // 6. Extract LSBs from the image pixels and return the result.
    bool start = false;
    for(int i = rowIndex; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if (!start) {
                j = columnIndex-1;
                start = true;
                continue;
            }
            std::bitset<8> pixelBits(image.get_pixel(i,j));
            LSB_array.push_back(pixelBits[0]);
        } 
    }
    return LSB_array;
}


// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;
    // TODO: Your code goes here.

    // 1. Verify that the LSB array size is a multiple of 7, else throw an error.
    if(LSB_array.size()%7 != 0) {
        throw std::invalid_argument("LSB arraySize is not a multiple of 7.");
    }
    // 2. Convert each group of 7 bits into an ASCII character.
    for(int i = 0; i < LSB_array.size(); i+=7) {
        std::bitset<7> bits;
        for(int j = 0; j < 7; j++) {
            bits[bits.size()-1-j] = LSB_array[i+j];
        }
        message.append(1,char(bits.to_ulong()));
    }

    // 3. Collect the characters to form the decrypted message.
    // 4. Return the resulting message.
    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    // 1. Convert each character of the message into a 7-bit binary representation.
    //    You can use std::bitset.
    // 2. Collect the bits into the LSB array.
    // 3. Return the array of bits.
    for(char c : message) {
        std::bitset<7> bits(c);
        for(int i = 0; i < 7; i++) {
            LSB_array.push_back(bits[6-i]);
        }
    }
    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {
    // TODO: Your code goes here.
    // 1. Ensure the image has enough pixels to store the LSB array, else throw an error.
    if(image.get_height()*image.get_width()/7 < LSB_array.size()/7) {
        throw std::invalid_argument("Image doesn't have enough pixels.");
    }
    // 2. Find the starting pixel based on the message length knowing that  
    //    the last LSB to embed should end up in the last pixel of the image.
    int rowIndex = image.get_height()-(LSB_array.size()/image.get_width()+1);
    int columnIndex = image.get_width()-(LSB_array.size()%image.get_width());
    int LSB_index = 0;
    // 3. Iterate over the image pixels, embedding LSBs from the array.
    while(rowIndex != image.get_height()) {
        std::bitset<8> pixelBits(image.get_pixel(rowIndex, columnIndex)); // changes last bit to LSB_array[LSB_index];
        pixelBits.set(0,LSB_array[LSB_index]);
        image.set_pixel(rowIndex, columnIndex, int(pixelBits.to_ulong()));
        LSB_index++;
        if(columnIndex == image.get_width()-1) {
            rowIndex++;
            columnIndex = 0;
        } else {
            columnIndex++;
        }
    }
    SecretImage secret_image(image);

    // 4. Return a SecretImage object constructed from the given GrayscaleImage 
    //    with the embedded message.
    return secret_image;
}
