#include <utility/ostream.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <machine.h>
#include "thread.h"
#include "Recorder.h"
#include <gpio.h>

using namespace EPOS;

//---------------------------------------------------

GPIO led('C',3,GPIO::OUT);

void on(){
	led.set(true);
}

void off(){
	led.set(false);
}

int start(){
	REC * rec = new REC();
	rec->addFunc(&on,0);
	rec->addFunc(&off,1);
	rec->start();
}

//MAIN-----------------------------------------------
int main(){
	{
		Thread thread(&start);
	    thread.join();
	    return 0;
	}
}
//Escolhido como vencedor o teste 1 com periodic threads. A 500 us, uma amostra de 20ms terá 40 amostras separadas, e portanto será isso que será passado a DFT.
