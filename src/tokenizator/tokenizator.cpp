#include "tokenizator.h"
#include <algorithm>

std::map<std::wstring, bool> Tokenizator::tableInfoTags =
{
    {L"!doctype", statusTags::SINGLE },
    {L"area",statusTags::SINGLE},
    {L"base", statusTags::SINGLE },
    {L"br",statusTags::SINGLE},
    {L"col", statusTags::SINGLE },
    {L"embed",statusTags::SINGLE},
    {L"hr", statusTags::SINGLE },
    {L"img",statusTags::SINGLE},
    {L"input", statusTags::SINGLE },
    {L"keygen",statusTags::SINGLE},
    {L"link", statusTags::SINGLE },
    {L"meta",statusTags::SINGLE},
    {L"param", statusTags::SINGLE },
    {L"source",statusTags::SINGLE},
    {L"track", statusTags::SINGLE },
    {L"wbr",statusTags::SINGLE},
};



void Tokenizator::clearToken(std::wstring& token)
{
    if (token.empty()) return;

    token.erase(std::unique(token.begin(), token.end(),
        [](char a, char b)
        {
            return std::isspace(a) && std::isspace(b);

        }), token.end());

    token.erase(token.begin(), std::find_if(token.begin(), token.end(),
        [](int ch)
        {
            return !std::isspace(ch);

        }));

    token.erase(std::find_if(token.rbegin(), token.rend(),
        [](int ch)
        {
            return !std::isspace(ch);

        }).base(), token.end());

    token.erase(std::remove_if(token.begin(), token.end(),
        [](wchar_t ch)
        {
            return ch == L'\n';
        }), token.end());

}

std::vector<Token> Tokenizator::tokenization(std::wstring htmlText)
{
    std::vector<Token> tokens;

    std::wstring bufferTag;

    std::wstring bufferText;
    
    bool isTag = false;
    bool isText = false;

    for (int i = 0; i < htmlText.length(); i++)
    {
        wchar_t symbol = htmlText[i];

        if (symbol == L'<' )
        {
            isTag = true;
            isText = false;
            bufferTag += symbol;
        }
        else if (symbol == L'>')
        {
            isTag = false;
            isText = true;
            bufferTag += symbol;

            if (!(iswalpha(bufferTag[1]) || bufferTag[1] == '!'))
            {
                bufferText += bufferTag;
                bufferTag.clear();
            }
            else
            {
                clearToken(bufferText);

                if (!bufferText.empty())
                {
                    tokens.push_back(Token(bufferText,tokenType::TEXT));
                    bufferText.clear();
                }
                clearToken(bufferTag);
                tokens.push_back(Token(bufferTag,tokenType::TAG));
                bufferTag.clear();
            }

        }
        else if (isTag)
        {
            bufferTag += symbol;
        }
        else if (isText)
        {
            bufferText += symbol;
        }

    }

    return tokens;
}

std::vector<Token> Tokenizator::tokenization(const wchar_t* htmlText)
{
    int size = wcslen(htmlText);


    std::vector<Token> tokens;

    std::wstring bufferTag;

    std::wstring bufferText;

    bool isTag = false;
    bool isText = false;

    for (int i = 0; i < size; i++)
    {
        wchar_t symbol = htmlText[i];

        if (symbol == L'<')
        {
            isTag = true;
            isText = false;
            bufferTag += symbol;
        }
        else if (symbol == L'>')
        {
            isTag = false;
            isText = true;
            bufferTag += symbol;

            if (bufferTag == L"<>")
            {
                bufferText += bufferTag;
                bufferTag.clear();
            }
            else
            {
                clearToken(bufferText);

                if (!bufferText.empty())
                {
                    tokens.push_back(Token(bufferText,tokenType::TEXT));
                    bufferText.clear();
                }

                clearToken(bufferTag);
                tokens.push_back(Token(bufferTag, tokenType::TAG));
                bufferTag.clear();
            }

        }
        else if (isTag)
        {
            bufferTag += symbol;
        }
        else if (isText)
        {
            bufferText += symbol;
        }

    }

    return tokens;



}
