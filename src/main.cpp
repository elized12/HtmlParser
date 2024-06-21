#include <iostream>
#include <io.h>
#include <fcntl.h>
#include "tokenizator/tokenizator.h"
#include <algorithm>
#include <fstream>
#include <locale>
#include "parser/html_parser.h"
#include <list>

using namespace std;

int main()
{

	_setmode(_fileno(stdout), _O_U8TEXT);

	setlocale(LC_ALL, "RUSSIAN");

	wstring textHtml = LR"(
<body>
<header>
<nav id = "menu" class="menu">
<div class="menu__logo">Кирилл</div>
<ul class="menu__navigation">
<li class="menu__link active" id="home">Главная</li>
<li class = "menu__link">Каталог</li>
<li class = "menu__link" >Корзина</li>
<li class = "menu__link" >Избранное</li>
<li class = "menu__link">Аккаунт</li>
<input type = "submit" reqired>
</ul>
</nav>
</header>
<main>

</main>
</body>

)";


	HtmlParser p(textHtml);
	p.print();

	vector<HtmlParser::Iterator> vec = p.getAllElement(L".menu__link");

	for (int i = 0; i < vec.size(); i++)
	{
		wcout << (*vec[i])[L"name"] << "\n";
	}


	return 0;
}