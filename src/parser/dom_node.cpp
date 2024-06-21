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

void DomHtmlNode::treeTraversal(DomHtmlNode* start, std::vector<DomHtmlNode*>& children)
{
	children.push_back(start);
	
	if (start->childrens.size() == 0)
		return;


	for (int i = 0; i < start->childrens.size(); i++)
		treeTraversal(start->childrens[i], children);
}

std::vector<DomHtmlNode*> DomHtmlNode::getAllChildren()
{
	std::vector<DomHtmlNode*> childrens;

	treeTraversal(this, childrens);

	childrens.erase(childrens.begin());

	return childrens;
}


