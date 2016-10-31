/* 
 * File:   Usb_Extractor.h
 * Author: rodrigo
 *
 * Created on October 26, 2016, 9:53 PM
 */
#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#ifndef USB_EXTRACTOR_H
#define	USB_EXTRACTOR_H

class Usb_Extractor {
    
private:
    
    int set_interface_attribs(int fd, int speed);
    void set_mincount(int fd, int mcount);
    
public:
    
    Usb_Extractor();
    int takeFromUSB();
    
};


#endif	/* USB_EXTRACTOR_H */

