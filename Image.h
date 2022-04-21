#include <vector>

class Image{
public:
    unsigned int WIDTH, HEIGHT, LABEL;
    std::vector<std::vector<unsigned int> > pixel_values;
    Image(unsigned int WIDTH,unsigned int HEIGHT, unsigned int LABEL);
    void put(int x,int y,int value);
    void print();
};