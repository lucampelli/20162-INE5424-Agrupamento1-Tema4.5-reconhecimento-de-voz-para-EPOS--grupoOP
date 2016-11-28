#include <utility/ostream.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <machine.h>
#include "machine/cortex/adc.h"
#include "clock.h"
#include "thread.h"

using namespace EPOS;

OStream cout;
ADC adc;
int i = 0;


int sample(){
	cout <<  "Thread start" << endl;
	while(true){
		cout<<adc.read()<<endl;
		cout<<i<<endl;
		i++;
	}
	
	return 0;
}



int main()
{
	Thread thread(&sample);
	thread.join();
    
    return 0;
}

//Resultado: amostras coletadas em uma frequencia desconhecida e variavel portanto inutilizável
