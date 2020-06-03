#include <iostream>
#include "SFcode.h"

using namespace std;

int main()
{
	string input = "0123456789";

	SFCode encryptor;
	string code = encryptor.encode(input);
	encryptor.print();
	encryptor.decode(code);
}