#include "html_parser.h"
#include <algorithm>
#include <iostream>
#include <algorithm>

std::map<std::wstring, bool> HtmlParser::singleTags =
{
	{L"abbr",true},
	{L"area",true},
	{L"base",true},
	{L"br",true},
	{L"col",true},
	{L"embed",true},
	{L"hr",true},
	{L"img",true},
	{L"input",true},
	{L"keygen",true},
	{L"link",true},
	{L"menuitem",true},
	{L"meta",true},
	{L"param",true},
	{L"source",true},
	{L"track",true},
	{L"wbr",true},
	{L"frame",true},
	{L"isindex",true}

};

std::wstring HtmlParser::getNameTag(Token& token)
{
	std::wstring& tokenText = token.getContent();
	std::wstring name;

	int i = 0;

	while (!iswalpha(tokenText[i]) && tokenText[i] != '/')
		i++;

	while (iswalpha(tokenText[i]) || tokenText[i] == '/' || iswdigit(tokenText[i]))
	{
		name += tokenText[i++];
	}

	return name;


}

std::map<std::wstring, std::wstring> HtmlParser::getProperties(Token& token)
{
	std::map<std::wstring, std::wstring> properies;

	std::wstring& textToken = token.getContent();

	int size = textToken.length();

	int i = 0;

	while (!iswalpha(textToken[i++]));
	while (iswalpha(textToken[i++]));

	std::wstring nameProperties;
	std::wstring valueProperties;



	while (i < size)
	{
		bool singleTag = false;

		//find properties
		while (i < size && !iswalpha(textToken[i]))
			i++;

		//copy nameProperties
		while (i < size && iswalpha(textToken[i]))
		{
			nameProperties += textToken[i];
			i++;
		}


		while (i <= size && textToken[i] != L'=')
		{
			if (iswalpha(textToken[i]) || i >= size)
			{
				singleTag = true;
				break;
			}
			i++;

		}

		if (i >= size)
			singleTag = true;

		if (!nameProperties.empty() && singleTag)
		{
			properies[nameProperties] = L"true";
			nameProperties.clear();
			continue;
		}

		while (i < size && textToken[i++] != L'"');

		while (i < size && textToken[i] != L'"')
		{
			valueProperties += textToken[i];
			i++;
		}

		if (!nameProperties.empty() && !valueProperties.empty())
		{
			properies[nameProperties] = valueProperties;
			nameProperties.clear();
			valueProperties.clear();

		}


	}


	return properies;


}

bool HtmlParser::isOpenTag(std::wstring& htmlTagName)
{
	for (int i = 0; !iswalpha(htmlTagName[i]); i++)
		if (htmlTagName[i] == '/')
			return false;

	return true;
}

bool HtmlParser::isSingleTag(std::wstring& tag)
{

	std::transform(tag.begin(), tag.end(), tag.begin(),
		towlower);

	return singleTags[tag];
}

std::wstring HtmlParser::trim_and_compress(const std::wstring& str) {

	std::wstring result;
	bool in_space = false;

	for (char ch : str) {
		if (std::isspace(ch)) {
			if (!in_space) {
				result += ' ';
				in_space = true;
			}
		}
		else {
			result += ch;
			in_space = false;
		}
	}

	if (!result.empty() && result.front() == ' ') {
		result.erase(result.begin());
	}

	if (!result.empty() && result.back() == ' ') {
		result.pop_back();
	}

	return result;
}

std::wstring HtmlParser::parseTextTag(std::wstring& query, int& pos)
{
	std::wstring text;

	for (; pos < query.length() && query[pos] != L',' && query[pos] != L'.' && query[pos] != L'#' && !iswspace(query[pos]); pos++)
	{
		text += query[pos];
	}

	if (query[pos] == L',' || query[pos] == L'.' || query[pos] == L'#')
		pos--;

	return text;
}

std::vector<Query> HtmlParser::parsingQuery(std::wstring& query)
{
	using sp = Query::statusParse;

	std::vector<Query> queries;

	sp statusParse = sp::None;

	Query _qr;

	for (int i = 0; i < query.size(); i++)
	{

		if (query[i] == L'#')
		{
			statusParse = sp::Id;
		}
		else if (query[i] == L'.')
		{
			statusParse = sp::Class;
		}
		else if (statusParse == sp::None && iswalpha(query[i]))
		{
			statusParse = sp::Tag;
		}
		else if (query[i] == L',' || i == query.length() - 1)
		{
			statusParse = sp::End;
		}

		switch (statusParse)
		{
		case sp::Id:
			if (!_qr.idElement.empty())
				throw SyntacisError("double Id in one query");
			i++;
			_qr.idElement = parseTextTag(query, i);
			break;

		case sp::Class:
			i++;
			_qr.classesElement.push_back(parseTextTag(query, i));
			break;

		case sp::Tag:
			if (!_qr.nameElement.empty())
				throw SyntacisError("double Tag in one query");

			_qr.nameElement = parseTextTag(query, i);
			break;

		case sp::End:
			queries.push_back(_qr);
			_qr = Query();
			break;

		default:
			break;
		}

		statusParse = sp::None;
	}

	if (!_qr.classesElement.empty() || !_qr.idElement.empty() || !_qr.nameElement.empty())
	{
		queries.push_back(_qr);
	}


	return queries;

}

HtmlParser::HtmlParser(std::wstring htmlText)
{
	std::vector<Token> tokens = Tokenizator::tokenization(htmlText);

	std::stack<std::wstring> postionNestring;

	DomHtmlNode* currentPosition = nullptr;
	DomHtmlNode* tag;

	for (int i = 0; i < tokens.size(); i++)
	{
		if (tokens[i].getType() == tokenType::TAG)
		{
			std::wstring tagName = getNameTag(tokens[i]);

			if (isOpenTag(tagName))
			{

				if (!isSingleTag(tagName))
					postionNestring.push(tagName);

				std::map<std::wstring, std::wstring> propeties = getProperties(tokens[i]);


				propeties[L"name"] = tagName;

				tag = new DomHtmlNode(propeties, currentPosition);

				dom.add(currentPosition, tag);

				if (!isSingleTag(tagName))
					currentPosition = tag;
			}
			else
			{
				tagName.erase(tagName.begin());

				if (postionNestring.top() == tagName)
				{
					postionNestring.pop();
					currentPosition = currentPosition->parent;
				}
			}
		}
		else
		{
			DomHtmlNode* text = new DomHtmlNode({ {L"name",L"text"},{L"content",tokens[i].getContent()} }, currentPosition);
			dom.add(currentPosition, text);
		}
	}


}

void HtmlParser::print()
{
	dom.print();
}

HtmlParser::Iterator HtmlParser::begin()
{
	Iterator p = dom.head;
	return p;
}

HtmlParser::Iterator HtmlParser::end()
{
	return Iterator(dom.end());
}


std::vector<std::wstring> HtmlParser::getArrayClasses(std::wstring stringClass)
{
	std::vector < std::wstring> classes;

	std::wstring buffer;

	for (int i = 0; i < stringClass.size(); i++)
	{
		buffer += stringClass[i];

		if (iswspace(stringClass[i]))
		{
			if (!buffer.empty())
			{
				classes.push_back(std::move(buffer));
			}
			buffer.clear();
		}
	}

	if (!buffer.empty())
	{
		classes.push_back(std::move(buffer));
	}
	return classes;
}

void HtmlParser::search(DomHtmlNode*start, std::vector<Query>& query, std::vector<DomHtmlNode*>& resultSearch, int posQuery)
{
	if (posQuery == query.size())
	{
		resultSearch.push_back(start);
		return;
	}

	std::vector<DomHtmlNode*> childrens = start->getAllChildren();

	if (posQuery == 0)
	{
		childrens.push_back(start);
	}

	for (int i = 0; i < childrens.size(); i++)
	{
		if (checkTagOnRequest(childrens[i], query[posQuery]))
			search(childrens[i], query, resultSearch, posQuery + 1);
	}



}

std::vector<DomHtmlNode*> HtmlParser::searchAll(std::vector<Query>& query)
{
	std::vector<DomHtmlNode*> resultSearch;

	search(dom.head, query, resultSearch, 0);

	return resultSearch;
}

bool HtmlParser::checkTagOnRequest(DomHtmlNode* it, Query& query)
{
	bool isMatchesTag = true;

	if (!query.nameElement.empty())
	{
		if (((*it)[L"name"] != query.nameElement))
			return false;
	}

	if (!query.classesElement.empty())
	{
		if ((*it)[L"class"].empty())
		{
			return false;
		}

		std::vector<std::wstring> classes = getArrayClasses((*it)[L"class"]);

		bool classFinded = false;

		for (int i = 0; i < classes.size(); i++)
			for (int j = 0; j < query.classesElement.size(); j++)
				if (classes[i] == query.classesElement[j])
				{
					classFinded = true;
					break;
				}

		if (!classFinded)
			return false;
	}

	if (!query.idElement.empty())
	{
		if ((*it)[L"id"] != query.idElement)
			return false;
	}

	return true;

}

std::vector<DomHtmlNode*> HtmlParser::getAllElement(std::wstring query)
{
	std::vector<Query> queries = parsingQuery(query);

	return searchAll(queries);
}

HtmlParser::Iterator::Iterator(DomHtmlNode* node) : iterator(node)
{
	infoNodeTravel infoNode;
	infoNode.index = 0;
	infoNode.node = node;
	position.push(infoNode);
}

HtmlParser::Iterator& HtmlParser::Iterator::operator=(const Iterator& it)
{
	iterator = it.iterator;
	position = it.position;

	return *this;
}


HtmlParser::Iterator& HtmlParser::Iterator::operator++() throw(NotRange)
{
	int index = position.top().index;

	if (index >= position.top().node->childrens.size())
	{
		position.pop();

		index = (position.top().index + 1);

		while (!position.empty() && index >= position.top().node->childrens.size())
		{
			position.pop();

			if (!position.empty())
				index = (position.top().index + 1);
			else
				throw NotRange();
		}

		if (iterator == nullptr)
			throw NotRange();


		DomHtmlNode* temp = position.top().node;
		int index_temp = position.top().index + 1;

		position.pop();

		infoNodeTravel infoNode;
		infoNode.index = index_temp;
		infoNode.node = temp;

		position.push(infoNode);

		iterator = position.top().node->childrens[index];
		infoNode.node = iterator;
		infoNode.index = 0;

		position.push(infoNode);

	}
	else
	{
		iterator = position.top().node->childrens[index];

		if (iterator == nullptr)
			throw NotRange();

		infoNodeTravel temp;

		temp.node = iterator;
		temp.index = 0;

		position.push(temp);

	}

	return *this;

}

DomHtmlNode& HtmlParser::Iterator::operator*() const
{
	if (iterator == nullptr)
		throw NotInitialization();

	return *iterator;
}

bool HtmlParser::Iterator::operator==(const Iterator& it)
{
	return it.iterator == iterator;
}

bool HtmlParser::Iterator::operator!=(const Iterator& it)
{
	return !(this->operator==(it));
}

