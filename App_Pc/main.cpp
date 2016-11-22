/* 
 * File:   main.cpp
 * Author: rodrigo
 *
 * Created on October 26, 2016, 9:50 PM
 */

#include <cstdlib>
#include "Usb_Extractor.h"
#include <pthread.h>
#include <stdio.h>
#define NUM_THREADS     1

int main(int argc, char *argv[]) {
    int status;

    if (fork() == 0) { //Filho
        USB_Ext();
        status = system("my_app");
        exit(0);
    } else { //Father
        if (fork() == 0) { //Filho
            USB_Send();
            status = system("my_app");
            exit(0);
        } else { //Father


        }

    }
}

void USB_Ext() {
    Usb_Extractor* extractor = new Usb_Extractor();
    extractor->takeFromUSB();
}

void USB_Send() {
    Usb_Extractor* extractor = new Usb_Extractor();
    extractor->USB_Sender();
}