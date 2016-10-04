#include <utility/ostream.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <machine.h>
#include "machine/cortex_m/adc.h"
#include "periodic_thread.h"
#include "thread.h"

using namespace EPOS;

OStream cout;
ADC adc;
short int readings[400] = {};  //entendi o porque dos pacotes serem tao pequenos

void print(){
	int i = 0;
	while(i < 400){
		cout<<readings[i]<<endl;
	}
}

int sample(){
	cout <<  "Thread start" << endl;
	int i = 0;
	while(i < 400){
		readings[i] = adc.read();
		Periodic_Thread::wait_next();
		i++;
	}
	print();
	return 0;
}



int main()
{
	for(int i = 0; i < 10000; i++){
		cout << "Start" << endl;
	}
	Periodic_Thread thread((Periodic_Thread::Microsecond)1000, &sample);
	thread.join();
    
    return 0;
}
