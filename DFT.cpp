#include "stdio.h"
#include <iostream>

#define SIZE 128

using namespace std;

float * a = new float[SIZE/2 + 1];
float * b = new float[SIZE/2 + 1];


template <typename T>
void print(T array){
	for(int j = 0; j < SIZE; j++){
		cout<<array[j]<<endl;
	}
	cout<<"TICK"<<endl;
}


int fat (int n){	//funciona
	int toReturn;
	if(n == 0){
		return 1;
	} else {
		toReturn = n * fat(n-1);
	}
	return toReturn;
}
 
double mysin(double value){		//não tem pq nao funcionar
	double x = value;

	if (x < -3.14159265){
	    x += 6.28318531;
	}else
	if (x >  3.14159265){
	    x -= 6.28318531;
	}

	double sin = 0;

	if (x < 0){
    	sin = 1.27323954 * x + .405284735 * x * x;
	}
	else{
    	sin = 1.27323954 * x - 0.405284735 * x * x;
	}

	//cout<<(int)(sin*1000)<<endl;
	return sin;
}

double mycos(double value){			//por algum motivo nao funciona
	return mysin(value + 3.14159265/2);
}

void FDFT(int s[SIZE]){
	// aqui s é a entrada e a e b as saidas;
	//main
	int n = SIZE;
	for(int k = 0; k <= n/2; k++){
		a[k] = b[k] = 0;
		for(int x = 0; x < n; x++){
			a[k] += s[x] * mycos(2 * 3.14159265 / n * k * x);
			b[k] += s[x] * mysin(2 * 3.14159265 / n * k * x);
		}
		//normalize
		a[k] *= (k==0 || k == n/2) ? 1. /n : 2./n;
		b[k] *= 2./n;
	}
}

void RDFT(int s[SIZE]){
	//aqui a e b são as entradas e s vira a saida
	int n = SIZE;	
	for(int x = 0; x < n; x++){
		s[x] = a[0];
		for(int k = 1; k <=n; k++){
			s[x] = a[k] * mycos(2 * 3.14159265 / n * k * x) + b[k] * mysin(2 * 3.14159265 / n * k * x);
		}
	}
}

int main(){
	int samples[SIZE] = {};
	int d = 7;
	bool grow = true;

	for(int i = 0; i < SIZE; i++){
		samples[i] += d;
		if(grow){
			d += 10;
		} else {
			d -= 10;
		}
		if(d > 120){
			grow = false;
		}
		if(d < -120){
			grow = true;
		}
	}

	cout<<"FDFT"<<endl;
	FDFT(samples);
	cout<<"Samples"<<endl;
	print(samples);
	cout<<"A"<<endl;
	print(a);
	cout<<"B"<<endl;
	print(b);

}
