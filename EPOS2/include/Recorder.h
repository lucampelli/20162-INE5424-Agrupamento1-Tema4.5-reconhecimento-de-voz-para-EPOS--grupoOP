/* Apenas para propósito de teste.
Já que graças a dificuldades com sincronização de Threads bloqueantes, e com o Rádio, algumas alterações foram efetuadas para que se possa testar cada módulo individualmente. Ao inicializar o componente, deverá ser escolhido qual função será simulada, das duas possíveis, escolhendo com um T a primeira, e F a segunda. Caso esteja escolhido o módulo Sender, a primeira seria o módulo StandBy/Amostragem, e o segundo o módulo Receber/Agir. Caso esteja escolhido o módulo Receiver, a primeira simboliza o roteamento EPOS->PC, e a segunda, PC->EPOS. Nota: Em todas as circunstancias onde deveria se receber um sinal de rádio (nic.receive()), foram substituídas para um recebimento via USB, por motivos de teste e demonstração. */
/*No módulo Sender para sair do Standby, deve-se enviar uma letra o. e para Agir deve-se enviar uma Letra A, ou B*/

#ifndef RECORDER_H
#define RECORDER_H

#include "adc.h"
#include "nic.h"
#include "mutex.h"
#include "thread.h"
#include "usb.h"
#include "alarm.h"
#include <utility/ostream.h>

#define NOISE 20
#define SIZE 160
#define F_PERIOD 500
#define S_PERIOD 1000


using namespace EPOS;
__BEGIN_SYS

OStream cout;
ADC adc;
NIC nic;
Mutex mutex;
Mutex Smutex;
Mutex Rmutex;
bool action = false;

typedef void(Function)();

Function * _function[2];


//prints---------------------------------------------
template <typename T>
void print(T array){
	for(int j = 0; j < SIZE; j++){
		cout<<array[j]<<endl;
	}
	//cout<<"TICK"<<endl;
}

//---------------------------------------------------

template <typename T>
int absMean(T array[], int size, int zero){
	int sum = 0;
	for(int i = 0; i < size; i++){
		int temp = array[i];
		temp -= zero;
		if(temp < 0){
			temp = -temp;
		}
		temp += zero;
		sum += temp;
	}
	return sum/size;
}

//---------------------------------------------------

//NIC------------------------------------------------
template<typename T>
int send(T array[]){
	NIC::Address dst = NIC::Address::Broadcast::BROADCAST;
		
	Smutex.lock();
	if(nic.send(dst,0,&array[0],sizeof(array))){
		//cout<<"Sent"<<endl;
	}
	Smutex.unlock();
	return 0;
}

int send(unsigned char cmd){
	NIC::Address dst = NIC::Address::Broadcast::BROADCAST;
		
	Smutex.lock();
	if(nic.send(dst,0,&cmd,sizeof(unsigned char))){
		//cout<<"Sent"<<endl;
	}
	Smutex.unlock();
	return 0;
}

//--------------------------------------------------

//Classes-------------------------------------------

class Sender{

	public:
		Sender(){
			if(action){
				Thread thread1(&sample);
				Thread thread(&StandBy);
				thread.join();
				thread1.join();			
			} else {
				Thread thread2(&act);
				thread2.join();
			}
			
		}

		static int sample(){ //Done but nic needs testing
			int i = 0;
			unsigned char samples[SIZE];

			while(true){
				mutex.lock();
				int times = 0;
				while(times < 3000){
					samples[i] = adc.read() + 64;
					i++;
					samples[i] = adc.read() + 64;
					i++;
					samples[i] = adc.read() + 64;
					i++;
					samples[i] = adc.read() + 64;
					i++;

					if(i > SIZE){
						i= 0;					
						send(samples);
						//print(samples);
					}
					times++;
					Alarm::delay(F_PERIOD);
				}
				cout<<"abcdefghijklmnopqrstuvwxyz1234567890"<<endl;
				cout<<"3sec"<<times<<endl;
				mutex.unlock();
				Alarm::delay(F_PERIOD);
			}
			
		}


		static int StandBy(){
			USB * usb = new USB();

			int i = 0;
			unsigned char samples[SIZE/4];
			int media = 0;
			int mediaTotal = 0;
			bool locked = false;
			while(true){
				if(!locked){
					mutex.lock();
					locked = true;
				}
				samples[i] = adc.read() + 64;
				media = absMean(samples, SIZE/4, 64);
				cout<<"media: " << media << endl;
				if(usb->get() == 'o'){//media > NOISE + 64){
					cout<<"ok"<<endl;					
					mutex.unlock();
					locked = false;
					Alarm::delay(F_PERIOD);
				}
				i++;
				if(i > SIZE/4){
					i = 0;
				}
				Alarm::delay(F_PERIOD);
			}


		}

		//receives a command from PC and acts

		static int act(){			//done nic needs testing
			NIC::Address src;
			NIC::Protocol prot;
			unsigned char msg[SIZE];
			USB * usb = new USB();
			while(true){			
				
				//insert NIC here
	
				msg[0] = 'C';
				msg[1] = 'T';
				msg[2] = 'F';
				msg[3] = usb->get();
				bool started = false;
	
				for(int j = 0; j < SIZE; j++){
					if(!started){
						if(msg[j] == 'C' && msg[j+1] == 'T' && msg[j+2] == 'F'){
							started = true;
							j += 2;
						}
					} else {
						if(msg[j] == 'A'){
							cout<<"Lights on"<<endl;
							if(_function[0]){
								_function[0]();
							} else {
								cout<<"Function not set"<<endl;
							}
						}
						if(msg[j] == 'B'){
							cout<<"Lights off"<<endl;
							if(_function[1]){
								_function[1]();
							} else {
								cout<<"Function not set"<<endl;
							}
						}
						break;
					}
				}
			Alarm::delay(F_PERIOD);
			}
		}

};

class Receiver{

	public:
		Receiver(){
			if(action){
				Thread thread(&toPC);
				thread.join();
			} else {
				Thread thread1(&toEPOS);
				thread1.join();
			}
		}
	
		static int toPC(){			//for some reason nic was blocking usb... weird...
			NIC::Address src;
			NIC::Protocol prot;
			unsigned char msg[SIZE] = {};
			USB* usb = new USB();
			while(true){
				/*
				if(nic.receive(&src,&prot,&msg[0],sizeof(msg))){
					cout<<"Received"<<endl;
				}*/
				int i = 0;
				while(msg[i - 1] != '*'){
					msg[i] = usb->get();
					i++;
				}
				for(int j = 0; j < SIZE; j++){
					cout<<msg[j];
				}
				msg = {};
			}
		}

		static int toEPOS(){		//pc to Epos works, Epos to Epos needs more testing...
			char msg[SIZE];
			USB* usb = new USB();
			int i = 0;
			while(true){
				while(msg[i - 1] != '@'){
					msg[i] = usb->get();
					i++;
				}
				msg[i-1] = ' ';
				cout<<"got: ";
				cout<<msg<<endl;
				send(msg);
				i =0;
				msg ={};
			}
		}
};

class REC{

	public:
	REC(){
	}

	void addFunc(Function * f, int place){
		cout<<"changed"<<endl;
		_function[place] = f;
	}

	void start(){
		USB* usb = new USB();
		if(usb->get() == 'T'){
			action = true;
		} else {
			action = false;
		}
		delete usb;
		IF_CLASS<Traits<REC>::send,Sender,Receiver>::Result op;
	}
};

__END_SYS
#endif
