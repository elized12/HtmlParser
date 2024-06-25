#pragma once
#include "tokenizator.h"
#include <string>

enum tokenType;

class Token
{
	private:
		std::wstring content;
		tokenType type;
	public:
		Token(std::wstring& _content, tokenType _type) :content(_content), type(_type) {}
	public:
		std::wstring& getContent();
		tokenType getType();
};