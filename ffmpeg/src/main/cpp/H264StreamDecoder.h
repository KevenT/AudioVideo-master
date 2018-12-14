//
// Created by aiya on 2017/4/5.
//

#ifndef AUDIOVIDEO_H264STREAMDECODER_H
#define AUDIOVIDEO_H264STREAMDECODER_H

#include "Codec.h"

class H264StreamDecoder: public Codec {
private:
    int width;
    int height;
    size_t yFrameSize;
    size_t uvFrameSize;
public:
    int start1();
    int start();
    int input(uint8_t * data,int len);
    int output(uint8_t * data);
    void set(int key,int value);
    int get(int key);
    int stop();
};


#endif //AUDIOVIDEO_H264STREAMDECODER_H
