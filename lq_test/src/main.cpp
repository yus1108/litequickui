#include <iostream>
using namespace std;

#include <lq_core.h>

int main()
{
	lq_byte_t* htmlBuffer = (lq_byte_t*)"<!DOCTYPE html><html><head><title>Test</title></head><body><h1>Hello CMake.</h1></body></html>";
	cout << htmlBuffer << endl;
	return 0;
}
