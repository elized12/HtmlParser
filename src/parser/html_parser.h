#pragma once
#include "dom_html.h"
#include "../tokenizator/tokenizator.h"
#include <stack>
#include <string>
#include "exception.h"

class DomHtml;

class NotRange : public Exception
{
public:
	NotRange() :Exception("NotRange", "RANGE NOT") {}
};

class NotInitialization : public Exception
{
public:
	NotInitialization() :Exception("NotInitialization", "iterator == nullptr") {}
};

class HtmlParser
{
public:

	class Iterator
	{
		friend DomHtml;
		friend HtmlParser;
	private:
		struct infoNodeTravel
		{
			int index;
			DomHtmlNode* node;
		};
	private:
		DomHtmlNode* iterator;
		std::stack<infoNodeTravel> position;
		Iterator(DomHtmlNode* node);
	public:
		Iterator(const Iterator& it) = default;
	public:
		Iterator& operator=(const Iterator& it);
		Iterator& operator=(DomHtmlNode* node);
		Iterator& operator++() throw (NotRange);
		DomHtmlNode& operator*() const;
		bool operator==(const Iterator& it);
		bool operator!=(const Iterator& it);
		void printPath();
		void printInfoNode();

	};

private:
	DomHtml dom;
private:
	static std::map<std::wstring, bool > singleTags;
private:
	std::wstring getNameTag(Token& token);
	std::map<std::wstring, std::wstring> getProperties(Token& token);
	bool isOpenTag(std::wstring& htmlTagName);
	bool isSingleTag(std::wstring& tag);
	std::wstring parseTextTag(std::wstring& query, int& pos);
	std::vector<Query> parsingQuery(std::wstring& query);
	std::wstring trim_and_compress(const std::wstring& str);
public:
	HtmlParser(std::wstring htmlText);
public:
	void print();
	Iterator begin();
	Iterator end();
	std::vector<Iterator> getAllElement(std::wstring query);
};