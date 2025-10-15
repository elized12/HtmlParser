#pragma once
#include <cwctype>
#include <map>
#include <string>
#include <vector>

#include "token.h"

class Token;

enum statusTags {
    PAIRED = 0,
    SINGLE = 1
};

class Tokenizator {
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
