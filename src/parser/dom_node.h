#pragma once
#include <vector>
#include <map>
#include <string>
#include "dom_html.h"
#include "exception.h"
#include "query.h"
#include "tag.h"

class DomHtml;
class HtmlParser;

class SyntacisError : public Exception 
{
public:
	SyntacisError(std::string information) : Exception("SyntacsisError", information) {}
};

class DomHtmlNode
{
private:

public:
	friend DomHtml;
	friend HtmlParser;
private:
	DomHtmlNode* parent;
	std::vector<DomHtmlNode*> childrens;
	std::map<std::wstring, std::wstring> properties;
private:
	std::vector<Query> parsingQuery(std::wstring& query);
	void treeTraversal(DomHtmlNode* start, std::vector<DomHtmlNode*>& children);
public:
	DomHtmlNode(std::map<std::wstring, std::wstring> _properties, DomHtmlNode* _parent) : properties(_properties), parent(_parent) {}
	DomHtmlNode() = default;
public:
	std::wstring& operator[](std::wstring propeties);
	std::map<std::wstring, std::wstring>::iterator beginProperties();
	std::map<std::wstring, std::wstring>::iterator endProperties();
	bool propertyExist(std::wstring propeties);
	std::vector<DomHtmlNode*> getAllChildren();

};