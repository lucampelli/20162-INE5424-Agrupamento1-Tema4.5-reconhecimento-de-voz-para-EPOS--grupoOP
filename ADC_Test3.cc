#include <utility/ostream.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <machine.h>
#include "timer.h"
#include "thread.h"
#include "adc.h"
#include "ic.h"

using namespace EPOS;

OStream cout;
ADC adc;

void print(const IC::Interrupt_Id & t){
	cout<<adc.read()<<endl;
	cout<<"IRQ "<< t<< endl;
}

int sample(){
	User_Timer timer(0, 1000,&print,true);
	while(1){
	}
	return 0;
}

int main()
{
	Thread thread(&sample);
	thread.join();
    
    return 0;
}

//Resultado: qualquer valor abaixo de 1000us causa kernel panic de 1 a 3 segundos depois do inicio da execução.
