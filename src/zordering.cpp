#include "zordering.h"

int Z::binToDec(std::string bin){

    int x = 0;

    std::reverse(bin.begin(), bin.end());

    int i = 0;
    for(char b: bin){
        x += ((b == '1') ? 1 : 0) * (int)pow(2.0, i);
        i++;
    }

    return x;

}

std::string Z::decToBin(int x){

    std::string bin = "";
    while(x > 0){
        bin += ((x % 2) == 1) ? "1" : "0";
        x /= 2;
    }

    bin += std::string((2 * N) - bin.length(), '0');

    std::reverse(bin.begin(), bin.end());
    return bin;

}

int Z::X(int i){

    // Mask Odd bits
    std::string bin = Z::decToBin(i);
    std::string oddBits = "";
    for(int i = 0; i < bin.length(); i+=2){
        oddBits += bin[i];
    }

    return Z::binToDec(oddBits);

}

int Z::Y(int i){

    // Mask Even bits
    std::string bin = Z::decToBin(i);
    std::string evenBits = "";
    for(int i = 1; i < bin.length(); i+=2){
        evenBits += bin[i];
    }

    return Z::binToDec(evenBits);

}

Point Z::toVec(int i){

    int x = X(i);
    int y = Y(i);
    return Point(x, y);

}

int Z::toIndex(Point v){

    std::string x = Z::decToBin(v.x);
    std::string y = Z::decToBin(v.y);

    // int x_len = x.length();
    // int y_len = y.length();
    // int total = x_len;

    // if(x_len > y_len){
    //     y = std::string(x_len - y_len, '0') + y;
    // }else if(x_len < y_len){
    //     total = y_len;
    //     x = std::string(-1 * (x_len - y_len), '0') + x;
    // }

    std::string index = "";
    for(int i = 0; i < 2 * N; i++){
        index += x[i];
        index += y[i];
    }

    return Z::binToDec(index);

}

int Z::N = 64;
