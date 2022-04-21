#include"Image.h"
#include<iostream>
#include<math.h>

static const char* ascii_lum = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
static const int ascii_lum_length = 73;

 Image::Image(unsigned int WIDTH, unsigned int HEIGHT, unsigned int LABEL) : 
    WIDTH(WIDTH), HEIGHT(HEIGHT), LABEL(LABEL) {

    for(int y = 0; y < HEIGHT; y++){
        pixel_values.push_back(std::vector<unsigned int>());
        for(int x = 0; x < WIDTH; x++){
            pixel_values[y].push_back(0);
        }
    }

}

void Image::put(int x, int y, int val){
    pixel_values[y][x] = val;
}

void Image::print(){
    std::cout << "           <" << LABEL << ">:" << std::endl;
    for(int y = 0; y < HEIGHT; y++){
        std::string line = "";
        for(int x = 0; x < WIDTH; x++){
            int val = pixel_values[y][x];
            float brightness = fmax(fmin((float(val)/255.0f),1.0f),0.0f);            
            
            int max = ascii_lum_length-1;
            int index = int(brightness*max);
            
            char b_char = ascii_lum[index];
            line += b_char;
        }
        std::cout << line << std::endl;
    }
}