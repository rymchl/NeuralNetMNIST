#pragma once
#ifndef UTIL
#define UTIL

#include<vector>
#include<math.h>
#include<iostream>

#define LOG(x) std::cout << x << std::endl


static float rand_float(){ 
    return (float)(rand()%1001)/500.0f - 1.0f;
}

static float dot_prod(std::vector<float> a, std::vector<float> b){
    float out = 0;
    for(int i = 0; i < a.size(); i++){ out += a[i]*b[i]; }
    return out;
}


static std::vector<float> add_vec(std::vector<float> a, std::vector<float> b){
    for(int i = 0; i < a.size(); i++){ a[i] += b[i]; }
    return a;
}

static std::vector<float> mat_vec_mul(std::vector< std::vector < float > > mat, std::vector< float > vec){
    std::vector<float> out;
    for(std::vector<float> row : mat){
        out.push_back(dot_prod(row,vec));
    }
    return out;
}

static std::vector<float> apply_sigmoid(std::vector<float> vec){
    for(float &x : vec) x = 1/(1 + exp(-x));
    return vec;
}

static float der_sigmoid(float x){
    return exp(-x) / pow(1 + exp(-x), 2);
}

#endif