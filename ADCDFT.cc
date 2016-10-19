#include <utility/ostream.h>

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#include <machine.h>
#include "machine/cortex/adc.h"
#include "periodic_thread.h"
#include "thread.h"
#include "nic.h"
#include "mutex.h"

#define SIZE 40
#define PI 3.141592

using namespace EPOS;

OStream cout;
ADC adc;
NIC nic;
Mutex mutex;

int i = 0;
int samples[SIZE];

//prints---------------------------------------------

void print(){
	for(int j = 0; j < SIZE; j++){
		cout<<samples[j]<<endl;
	}
	cout<<"TICK"<<endl;
}

//---------------------------------------------------

//SIN&COS&FAT----------------------------------------

int fat (int n){	//funciona
	int toReturn;
	if(n == 0){
		return 1;
	} else {
		toReturn = n * fat(n-1);
	}
	return toReturn;
}
 
float mysin(float value){		//não tem pq nao funcionar
	float x = value;

	if (x < -3.14159265){
	    x += 6.28318531;
	}else
	if (x >  3.14159265){
	    x -= 6.28318531;
	}

	float sin = 0;

	if (x < 0){
    	sin = 1.27323954 * x + .405284735 * x * x;
	}
	else{
    	sin = 1.27323954 * x - 0.405284735 * x * x;
	}

	//cout<<(int)(sin*1000)<<endl;
	return sin;
}

float mycos(float value){			//por algum motivo nao funciona porra
	return mysin(value + PI/2);
}

//---------------------------------------------------

//nic------------------------------------------------
int send(){
	NIC::Address dst = NIC::Address(255);
	while(1){
		mutex.lock();
		if(nic.send(dst,0,&samples,sizeof(samples))){
			cout<<"Sent"<<endl;
		}
		mutex.unlock();
		Periodic_Thread::wait_next;
	}
}

//---------------------------------------------------

//DFT's----------------------------------------------
/*
void FDFT(int s[SIZE],float a[SIZE],float b[SIZE]){
	// aqui s é a entrada e a e b as saidas;
	//main
	int n = SIZE;
	for(int k = 0; k < n; k++){
		a[k] = b[k] = 0;
		for(int x = 0; x < n; x++){
			a[k] += s[x] * cos(2 * PI / n *	k * x);
			b[k] += s[x] * sin(2 * PI / n *	k * x);
		}
		//normalize
		a[k] *= (k==0 || k == n/2) ? 1. /n : 2./n;
		b[k] *= 2./n;
	}
}

void RDFT(int s[SIZE],float a[SIZE],float b[SIZE]){
	//aqui a e b são as entradas e s vira a saida
	int n = SIZE;	
	for(int x = 0; x < n; x++){
		s[x] = a[0];
		for(int k = 1; k <=n/2; k++){
			s[x] = a[k] * cos(2 * PI / n * k * x) + b[k] * sin(2 * PI / n * k * x);
		}
	}
}*/
//---------------------------------------------------

//MAIN-----------------------------------------------

int sample(){
	cout <<  "Thread start" << endl;
	
	while(true){
		
		mutex.lock();
		samples[i] = adc.read();
		mutex.unlock();		
		i++;
		if(i >= SIZE){
			i = 0;
			print();
		}

		Periodic_Thread::wait_next();
	}
	return 0;
}

int main()
{
	Periodic_Thread thread((Periodic_Thread::Microsecond)500, &sample);
	//Periodic_Thread thread2((Periodic_Thread::Microsecond)20000, &send);
	thread.join();
    
    return 0;
}

//Escolhido como vencedor o teste 1 com periodic threads. A 500 us, uma amostra de 20ms terá 40 amostras separadas, e portanto será isso que será passado a DFT.

//Testes com nic.send inconclusivos. Falha de envio (possivelmente por escrita concorrente com leitura... ou operação não atomica...)(Nope... usando mutex pra sincronizar tudo nao funciona...) hora de testar a DFT
