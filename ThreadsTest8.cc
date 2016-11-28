#include <utility/ostream.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <machine.h>
#include "machine/cortex/adc.h"
#include "periodic_thread.h"
#include "mutex.h"
#include "usb.h"
#include "thread.h"
#include "alarm.h"

using namespace EPOS;

int a = 0,b = 0,c = 0;
OStream cout;
Mutex mutex;
ADC adc;
USB usb;

int act1(){
	while(1){
		mutex.lock();
			a++;
			cout<<"Thread1: "<< a << ", "<< b << ", "<< c << ", " <<endl;
		mutex.unlock();
		Alarm::delay(250);
	}
}

int act2(){
	while(1){
		mutex.lock();
			b++;
			cout<<"Thread2: "<< a << ", "<< b << ", "<< c << ", " <<endl;

		mutex.unlock();
		Alarm::delay(500);
	}
}

int act3(){
	while(1){
		mutex.lock();
			c++;
			cout<<"Thread3: "<< a << ", "<< b << ", "<< c << ", " <<endl;

		mutex.unlock();
		Alarm::delay(500);
	}
}

int main()
{
	Thread thread1(&act1);
	Thread thread2(&act2);
	Thread thread3(&act3);
	thread1.join();
	thread2.join();
	thread3.join();
    
    return 0;
}
