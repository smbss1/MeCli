/*
** EPITECH PROJECT, 2019
** CodeGenerator
** File description:
** Created by besseausamuel
*/


#ifndef PARSER_HELPER_H
#define PARSER_HELPER_H

#include <string>
#include "Lexer.h"
#include "Token.h"

class ParserHelper
{
private:

    Token oCurrentToken;

    enum token_advance_mode
    {
        e_hold    = 0,
        e_advance = 1
    };

protected:
    Lexer oLexer;
public:

    bool Init(const std::string& str);
    void Define(const std::string& strId, const std::string& strValue, bool bAddInTrash = false);
    void DefineArea(const std::string& strId, char c1, char c2);

    Lexer& GetLexer();
    const Lexer& GetLexer() const;

    void NextToken();

    const Token& CurrentToken() const;

    void AdvanceToken(token_advance_mode mode);

    bool IsEnd();

    bool IsToken(const std::string& strValue, bool bAdvance = true);
    bool IsToken(const std::string& strType, const std::string& strValue, bool bAdvance = true);
    bool IsToken(const std::string& strType, const char *strValue, bool bAdvance = true);
    bool IsTokenType(const std::string &strType, bool bAdvance = true);

    bool IsTokenThenAssign(const std::string& strType, Token& oToken, bool bAdvance = true);
    bool IsTokenThenAssign(const std::string& strType, const char *strValue, Token& oToken, bool bAdvance);
    template <typename Allocator, template <typename, typename> class Container>
    bool IsTokenThenAssign(const std::string& strType, Container<Token&, Allocator>& oTokenList, bool bAdvance = true);
    template <typename Allocator, template <typename, typename> class Container>
    bool IsTokenThenAssign(const std::string& strType, const char *strValue, Container<Token&, Allocator>& oTokenList, bool bAdvance);

    bool PeekTokenIsType(const std::string& strType);
    bool PeekTokenIs(const std::string& strValue);
};


#endif //PARSER_HELPER_H