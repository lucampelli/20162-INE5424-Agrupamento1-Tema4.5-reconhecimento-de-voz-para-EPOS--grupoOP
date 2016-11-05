/* 
 * File:   main.cpp
 * Author: rodrigo
 *
 * Created on October 26, 2016, 9:50 PM
 */

#include <cstdlib>

#include "Usb_Extractor.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    Usb_Extractor* extractor = new Usb_Extractor();
    extractor->takeFromUSB();
    return 0;
}

