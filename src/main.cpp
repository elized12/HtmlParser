#include <iostream>
#include <io.h>
#include <fcntl.h>
#include "tokenizator/tokenizator.h"
#include <algorithm>
#include <fstream>
#include <locale>

using namespace std;

int main()
{

	_setmode(_fileno(stdout), _O_U8TEXT);

	setlocale(LC_ALL, "RUSSIAN");


	wifstream file("C:\\Users\\User\\Desktop\\frontend\\тренировка\\index.html");

	file.imbue(std::locale("rus_rus.utf8"));

	wstring text;
	wstring buffer;

	while (getline(file, buffer))
	{
		text += buffer;
	}

	wcout << text << "\n";

	vector<Token> tokens = Tokenizator::tokenization(text);

	for (int i = 0; i < tokens.size(); i++)
		wcout << i << ". " << tokens[i].getContent() << " " << tokens[i].getType() << " \n";


	return 0;
}