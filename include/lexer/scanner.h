#ifndef SCANNER_H
#define SCANNER_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <lexer/token.h>

namespace Lox {
    typedef std::vector<std::shared_ptr<ITokenBase>> TokenContainer;
    typedef std::map<std::string, TokenType> KeywordContainer;

    class CScanner {
        const std::string m_Source;
        TokenContainer m_Tokens;
        KeywordContainer m_Keywords;

        int m_Start = 0;
        int m_Current = 0;
        int m_Line = 1;

        // position helpers
        char Advance();
        char Peek() const;
        char PeekNext() const;
        bool Match(char Ch);
        bool AtEnd() const;

        // advanced parsing helpers
        void String();
        void Number();
        void Identifier();

        template<typename T = std::monostate>
        void AddToken(TokenType Type, T Literal = T{}) {
            std::string Text = m_Source.substr(m_Start, m_Current - m_Start);
            m_Tokens.emplace_back(std::make_shared<CToken<T> >(m_Line, Type, Text, Literal));
        }
        void AddEOFToken() {
            m_Tokens.emplace_back(std::make_shared<CToken<> >(m_Line, EOFi));
        }

        void ScanToken();

    public:
        explicit CScanner(const std::string &Source) : m_Source(Source) {
            m_Keywords.emplace("and", AND);
            m_Keywords.emplace("class", CLASS);
            m_Keywords.emplace("else", ELSE);
            m_Keywords.emplace("false", FALSE);
            m_Keywords.emplace("for", FOR);
            m_Keywords.emplace("fun", FUN);
            m_Keywords.emplace("if", IF);
            m_Keywords.emplace("nil", NIL);
            m_Keywords.emplace("or", OR);
            m_Keywords.emplace("print", PRINT);
            m_Keywords.emplace("return", RETURN);
            m_Keywords.emplace("super", SUPER);
            m_Keywords.emplace("this", THIS);
            m_Keywords.emplace("true", TRUE);
            m_Keywords.emplace("var", VAR);
            m_Keywords.emplace("while", WHILE);
        }

        TokenContainer ScanTokens();
    };
}

#endif // SCANNER_H
