#include<iostream>
#include<fstream>
#include"Image.h"

int reverseInt (int i) 
{
    unsigned char c1, c2, c3, c4;

    c1 = i & 255;
    c2 = (i >> 8) & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;

    return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}




std::vector<unsigned int> read_mnist_labels(std::string path)
{
    std::vector<unsigned int> out;
    std::ifstream file(path);
    if (file.is_open())
    {
        unsigned int magic_number=0;
        unsigned int number_of_images=0;

        file.read((char*)&magic_number,sizeof(magic_number)); 
        magic_number= reverseInt(magic_number);
        file.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images= reverseInt(number_of_images);

        std::cout << path << std::endl;
        std::cout << "Num Images : " << number_of_images << std::endl;
        std::cout << "Magic Number : " << magic_number << std::endl << std::endl;
                
        for(int i = 0; i < number_of_images; i++){
            unsigned char temp=0;
            file.read((char*)&temp,sizeof(temp));
            out.push_back((unsigned int)temp); 
        }  

        file.close();
    }
    else{
        printf("ERROR OPENING FILE AT %s\n", path.c_str());
    }
    return out;
}

std::vector<Image> read_mnist_images(std::string path, std::vector<unsigned int> labels)
{
    std::vector<Image> out;

    std::ifstream file(path);
    if (file.is_open())
    {
        unsigned int magic_number=0;
        unsigned int number_of_images=0;
        unsigned int n_rows=0;
        unsigned int n_cols=0;

        file.read((char*)&magic_number,sizeof(magic_number)); 
        magic_number= reverseInt(magic_number);
        file.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images= reverseInt(number_of_images);
        file.read((char*)&n_rows,sizeof(n_rows));
        n_rows= reverseInt(n_rows);
        file.read((char*)&n_cols,sizeof(n_cols));
        n_cols= reverseInt(n_cols);

        std::cout << path << std::endl;
        std::cout << "Num Rows : " << n_rows << std::endl;
        std::cout << "Num Cols : " << n_cols << std::endl;
        std::cout << "Num Images : " << number_of_images << std::endl;
        std::cout << "Magic Number : " << magic_number << std::endl << std::endl;

        for(unsigned int i=0;i<number_of_images;++i)
        {
            Image image(n_cols,n_rows, labels[i]);
            for(unsigned int r=0;r<n_rows;++r)
            {
                for(unsigned int c=0;c<n_cols;++c)
                {
                    unsigned char temp=0;
                    file.read((char*)&temp,sizeof(temp));
                    image.put(c,r,(unsigned int)temp);
                }
            }
            out.push_back(image);
        }
        file.close();
    }
    else{
        printf("ERROR OPENING FILE AT %s\n", path.c_str());
    }
    return out;
}