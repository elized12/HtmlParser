#pragma once
#include <string>
#include <vector>

class Query
{
public:
	enum statusParse
	{
		None = 0,
		Tag = 1,
		Class = 2,
		Id = 3,
		End = 4
	};
public:
	std::wstring nameElement;
	std::vector<std::wstring> classesElement;
	std::wstring idElement;
public:
	void clear();
};