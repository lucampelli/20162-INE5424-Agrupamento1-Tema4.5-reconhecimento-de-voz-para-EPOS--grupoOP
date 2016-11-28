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

using namespace EPOS;

int a = 0,b = 0,c = 0;
OStream cout;
Mutex mutex;
ADC adc;
USB usb;

int act1(){
	while(1){
		mutex.lock();
		if(usb.get() == 'a'){
			a++;
			cout<<"Thread1: "<< a << ", "<< b << ", "<< c << ", " <<endl;
		} else {
			cout<<"Thread1 don't accept that"<<endl;
		}
		mutex.unlock();
		Periodic_Thread::wait_next();
	}
}

int act2(){
	while(1){
		mutex.lock();
		if(usb.get() == 'b'){
			b++;
			cout<<"Thread2: "<< a << ", "<< b << ", "<< c << ", " <<endl;
		} else {
			cout<<"Thread2 don't accept that"<<endl;
		}
		mutex.unlock();
		Periodic_Thread::wait_next();
	}
}

int act3(){
	while(1){
		mutex.lock();
		if(usb.get() == 'c'){
			c++;
			cout<<"Thread3: "<< a << ", "<< b << ", "<< c << ", " <<endl;
		} else {
			cout<<"Thread3 don't accept that"<<endl;
		}
		mutex.unlock();
		Periodic_Thread::wait_next();
	}
}

int main()
{
	Periodic_Thread thread1((Periodic_Thread::Microsecond)500, &act1);
	Periodic_Thread thread2((Periodic_Thread::Microsecond)500, &act2);
	Periodic_Thread thread3((Periodic_Thread::Microsecond)500, &act3);
	thread1.join();
	thread2.join();
	thread3.join();
    
    return 0;
}

