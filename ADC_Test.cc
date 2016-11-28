#include <utility/ostream.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <machine.h>
#include "machine/cortex/adc.h"
#include "alarm.h"
#include "thread.h"

using namespace EPOS;

OStream cout;
ADC adc;

int sample(){
	cout <<  "Thread start" << endl;
	while(true){
		cout<<adc.read()<<endl;
		Alarm::delay(500);
	}
	return 0;
}

int main()
{
	for(int i = 0; i < 10000; i++){
		cout << "Start" << endl;
	}
	Thread thread(&sample);
	thread.join();
    
    return 0;
}

//Resultado Qualquer valor abaixo de 500us nã imprime nada, portanto não há como saber se está funcionando ou não...