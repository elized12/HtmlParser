#pragma once
#include "dom_node.h"
#include <iostream>

class HtmlParser;
class DomHtmlNode;

class DomHtml
{
public:
	friend HtmlParser;
private:
	DomHtmlNode* head;
	size_t size;
	DomHtmlNode* tail;
	bool isChange;
private:
	void printAll(DomHtmlNode* start,int pos = 0);
	void eraseEnd();
public:
	DomHtml() : size(0), head(nullptr), isChange(true),tail(nullptr) {}
	//DomHtml(const DomHtml& copy);
public:
	void add(DomHtmlNode* parent, DomHtmlNode* value);
	void erase(DomHtmlNode* node);
	DomHtmlNode* end();
public:
	void print();
	//~DomHtml();
};