#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "Usb_Extractor.h"
#include "Wav.h"

Usb_Extractor::Usb_Extractor() {

}

int Usb_Extractor::set_interface_attribs(int fd, int speed) {
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t) speed);
    cfsetispeed(&tty, (speed_t) speed);

    tty.c_cflag |= (CLOCAL | CREAD); /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8; /* 8-bit characters */
    tty.c_cflag &= ~PARENB; /* no parity bit */
    tty.c_cflag &= ~CSTOPB; /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS; /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

void Usb_Extractor::set_mincount(int fd, int mcount) {
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error tcgetattr: %s\n", strerror(errno));
        return;
    }

    tty.c_cc[VMIN] = mcount ? 1 : 0;
    tty.c_cc[VTIME] = 5; /* half second timer */

    if (tcsetattr(fd, TCSANOW, &tty) < 0)
        printf("Error tcsetattr: %s\n", strerror(errno));
}

int Usb_Extractor::takeFromUSB() {
    char *portname = "/dev/ttyACM0"; //Port for taking information
    int fd;
    int wlen;
    printf("Trying to Open Archive\n");
 
    while(access("/dev/ttyACM0", F_OK) == -1) sleep(1);//Until file do not exists
    
    fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if(fd < 0){
       cout<< "Error while opening archive"<< errno << endl;
       return -1;
    }
        
    printf("Opened\n");
    /*baudrate 115200, 8 bits, no parity, 1 stop bit */
    set_interface_attribs(fd, B115200);

    /*--TESTING OUTPUT--*/
    //wlen = write(fd, "Hello!\n", 7);
    //if (wlen != 7) {
    //    printf("Error from write: %d, %d\n", wlen, errno);
    //}
    //tcdrain(fd); /* delay for output */
    
    
    /* simple noncanonical input */
    unsigned char buf;
    int rdlen;
    
    //while(buf != '/i');//Caracter início
    printf("Criando Wav:\n");
    Wav* wave = new Wav("teste");
    
    while (buf != 'j') {//Caracter Fim
        rdlen = read(fd, &buf, 1);
        if (rdlen > 0) {

            //printf("Sample: %hho", buf);
            wave->addSample(buf * 100);

        } else if (rdlen < 0) {
            printf("Error from read: %d: %s\n", rdlen, strerror(errno));
        }
    }
    wave->endWav();
    printf("Wav Criado\n");

}
