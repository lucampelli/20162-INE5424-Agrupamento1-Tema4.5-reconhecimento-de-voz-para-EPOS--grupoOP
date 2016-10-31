/* 
 * File:   Wav.h
 * Author: rodrigo
 *
 * Created on October 26, 2016, 9:51 PM
 */
#include <cmath>
#include <fstream>
#include <iostream>

#ifndef WAV_H
#define	WAV_H
/*
Cria um arqivo no construtor com cabeçálio pré definido e adiciona samples neste arquivo até receber um término. 
 */
using namespace std;

class Wav {
private:
    ofstream f;
    size_t data_chunk_pos;

public:

    Wav(string name);

    void addSample(int sample);

    void endWav();

};
#endif	/* WAV_H */