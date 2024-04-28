#pragma once
#include <vector>
#include "token.h"
#include <string>
#include <map>

class Token;

enum statusTags
{
	PAIRED = 0,
	SINGLE = 1
};

enum tokenType
{
	TAG = 10,
	TEXT = 11
};

class Tokenizator
{
	private:
		static std::map<std::wstring, bool> tableInfoTags;
	private:
		Tokenizator() = default;
		Tokenizator(const Tokenizator&) = default;
	private:
		static void clearToken(std::wstring& token);
	private:
		static bool checkIsTag(std::wstring& str);
	public:
		static std::vector<Token> tokenization(std::wstring hmtlText);
		static std::vector<Token> tokenization(const wchar_t* hmtlText);
};

