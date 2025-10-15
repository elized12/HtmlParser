#pragma once
#include <string>

#include "tokenizator.h"

enum tokenType {
    TAG = 10,
    TEXT = 11
};

class Token {
   private:
    std::wstring content;
    tokenType type;

   public:
    Token(std::wstring& _content, tokenType _type) : content(_content), type(_type) {}

   public:
    std::wstring& getContent();
    tokenType getType();
};