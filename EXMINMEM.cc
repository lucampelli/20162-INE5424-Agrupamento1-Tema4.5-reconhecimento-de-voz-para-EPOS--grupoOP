#include "utility/ostream.h"

using namespace EPOS;

OStream cout;
char TEST[8000];
//char TEST[] = new char[8000];

int main(){
	for(int i = 0; i < 8000; i++){
		TEST[i] = i % 128;
	}
	for(int i = 0; i < 8000; i++){
		cout<<TEST[i]<<endl;
	}
	return 0;
}

