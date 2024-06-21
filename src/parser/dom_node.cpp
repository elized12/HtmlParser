#include "dom_node.h"


std::wstring parseTextTag(std::wstring& query, int& pos)
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

std::vector<Query> DomHtmlNode::parsingQuery(std::wstring& query)
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

std::wstring& DomHtmlNode::operator[](std::wstring property)
{
	return properties[property];
}

std::map<std::wstring, std::wstring>::iterator DomHtmlNode::beginProperties()
{
	return properties.begin();
}

std::map<std::wstring, std::wstring>::iterator DomHtmlNode::endProperties()
{
	return properties.end();
}

bool DomHtmlNode::propertyExist(std::wstring property)
{
	if (properties.count(property))
		return true;

	return false;
}


/* TODO:доделать */
DomHtmlNode* DomHtmlNode::getFirstElement(std::wstring query)
{
	std::vector<Query> result = parsingQuery(query);

	

	for (int i = 0; i < result.size(); i++)
	{
		std::wcout << (i + 1) << ":\n";
		std::wcout << "NameTag:" << result[i].nameElement << "\n";
		std::wcout << "IDTag:" << result[i].idElement << "\n";
		std::wcout << "ClassesTag:";

		for (int j = 0; j < result[i].classesElement.size(); j++)
			std::wcout << result[i].classesElement[j] << " ";
		
		std::wcout << "\n";

	}

	return nullptr;

}
/* TODO:доделать */
std::vector<DomHtmlNode*> DomHtmlNode::getAllElement(std::wstring query)
{
	return std::vector<DomHtmlNode*>();
}

void Query::clear()
{
	nameElement.clear();
	classesElement.clear();
	idElement.clear();
}


