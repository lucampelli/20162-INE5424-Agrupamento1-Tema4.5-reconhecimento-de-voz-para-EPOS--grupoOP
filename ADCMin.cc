#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "adc.h"
#include "periodic_thread.h"
#include <utility/ostream.h>

#define SIZE 40

using namespace EPOS;
OStream cout;
ADC adc;

//prints---------------------------------------------
template <typename T>
void print(T array){
	for(int j = 0; j < SIZE; j++){
		cout<<array[j]<<endl;
	}
	cout<<"TICK"<<endl;
}

//---------------------------------------------------

int sample(){
	int samples[SIZE];
	int i = 0;

	while(1){
		samples[i] = adc.read();
		i++;
		samples[i] = adc.read();
		i++;
		samples[i] = adc.read();
		i++;
		samples[i] = adc.read();
		i++;	

		if(i > SIZE){
			i = 0;
			print(samples);
		}
		Periodic_Thread::wait_next();
	}
}

int main(){
	Periodic_Thread thread(500, &sample);
	thread.join();
	return 0;
}
#endif
