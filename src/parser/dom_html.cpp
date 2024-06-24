#include "dom_html.h"

void DomHtml::printAll(DomHtmlNode* start, int pos)
{
	if (start == nullptr)
		return;

	for (int i = 0; i < pos; i++)
		std::wcout << L" ";

	std::wcout << start->properties[L"name"] << "\n";

	for (int i = 0; i < start->childrens.size(); i++)
		printAll(start->childrens[i], pos + 2);

}

void DomHtml::eraseEnd()
{
	DomHtmlNode* node = tail->parent;

	auto iterator = std::find(node->childrens.begin(), node->childrens.end(), tail);

	if (iterator != node->childrens.end())
	{
		node->childrens.erase(iterator);
	}

	delete tail;

}

void DomHtml::eraseDomTree(DomHtmlNode* start)
{
	if (start->childrens.size() == 0)
	{
		delete start;
		return;
	}

	for (int i = 0; i < start->childrens.size(); i++)
	{
		eraseDomTree(start->childrens[i]);
	}

	delete start;
}

void DomHtml::add(DomHtmlNode* parent, DomHtmlNode* value)
{
	if (tail != nullptr)
		eraseEnd();

	if (head == nullptr)
	{
		value->parent = nullptr;
		head = value;
		size = 1;
		isChange = true;
		return;
	}

	if (parent != nullptr)
	{
		parent->childrens.push_back(value);
		value->parent = parent;
		size++;
		isChange = true;
		return;
	}
	else
		throw "error";


}

void DomHtml::erase(DomHtmlNode* node)
{
	if (node->childrens.size() == 0)
	{
		DomHtmlNode* parentNode = node->parent;

		int i = 0;

		for (i = 0; i < parentNode->childrens.size(); i++)
			if (parentNode->childrens[i] == node)
				break;

		parentNode->childrens.erase(parentNode->childrens.begin() + i);
			
		
		delete node;
		return;
	}
	else if (node->childrens.size() > 0)
	{
		DomHtmlNode* parentNode = node->parent;

		for (int i = 0; i < node->childrens.size(); i++)
		{
			parentNode->childrens.push_back(node->childrens[i]);
			node->childrens[i]->parent = parentNode;
		}
		delete node;
	}
	
}

DomHtmlNode* DomHtml::end()
{
	if (isChange)
	{
		isChange = false;
		
		DomHtmlNode* current = head;

		while (current->childrens.size() != 0)
		{
			current = current->childrens[current->childrens.size() - 1];
		}

		DomHtmlNode* endNode = new DomHtmlNode({ {L"name",L"end"} }, current);

		current->childrens.push_back(endNode);

		this->tail = endNode;
	}

	return tail;

}

void DomHtml::print()
{
	printAll(head);
}

DomHtml::~DomHtml()
{
	eraseDomTree(head);
}
