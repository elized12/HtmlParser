#include "token.h"

std::wstring& Token::getContent()
{
    return content;
}

tokenType Token::getType()
{
    return type;
}
