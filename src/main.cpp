#define _CRTDBG_MAP_ALLOC

#include <iostream>
#include <io.h>
#include <fcntl.h>
#include "parser/html_parser.h"
#include <crtdbg.h>
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <string>
#include <fcntl.h>  

using namespace std;

int main()
{

	// Устанавливаем режим вывода в UTF-8
	_setmode(_fileno(stdout), _O_U8TEXT);

	system("chcp 1251");

	wstring htmlText;

	std::locale utf8_to_utf16(std::locale(), new std::codecvt_utf8<wchar_t>);

	wifstream file(L"C:\\Users\\User\\Downloads\\Текстовый документ.txt");



	file.imbue(locale::locale("ru_RU.utf8"));

	while (true)
	{
		if (file.eof())
			break;
		wchar_t ch;
		file.get(ch);
		htmlText += ch;
	}

  
	wcout << htmlText << "\n";

	HtmlParser p(htmlText);

	p.print();


	vector<DomHtmlNode*> arr = p.getAllElement(L"section .services, img");
	

	for (int i = 0; i < arr.size(); i++)
	{
		wcout << (i+1) << ":\n";
		wcout << "name:" << (*arr[i])[L"name"] << "\n";
		
		auto p = (*arr[i]).beginProperties();
		
		while (p != (*arr[i]).endProperties())
		{
			wcout << p->first << ": " << p->second << "\n";
			p++;
		}

	}

	_CrtDumpMemoryLeaks();
	

	return 0;
}