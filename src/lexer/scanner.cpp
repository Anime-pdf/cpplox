#include <lexer/scanner.h>

#include "interpreter.h"

char Lox::CScanner::Advance() {
    return m_Source[m_Current++];
}

char Lox::CScanner::Peek() const {
    if (AtEnd()) return '\0';
    return m_Source[m_Current];
}

char Lox::CScanner::PeekNext() const {
    if (m_Current + 1 >= m_Source.length()) return '\0';
    return m_Source[m_Current + 1];
}

bool Lox::CScanner::Match(const char Ch) {
    if (AtEnd()) return false;
    if (m_Source[m_Current] != Ch) return false;

    m_Current++;
    return true;
}

void Lox::CScanner::String() {
    while (Peek() != '"' && !AtEnd()) {
        if (Peek() == '\n') m_Line++;
        Advance();
    }

    if (AtEnd()) {
        CInterpreter::Error(m_Line, "Unterminated string.");
        return;
    }

    // The closing ".
    Advance();

    // Trim the surrounding quotes.
    AddToken(STRING, m_Source.substr(m_Start + 1, m_Current - m_Start - 2));
}

void Lox::CScanner::Number() {
    while (isdigit(Peek())) Advance();

    // Look for a fractional part.
    if (Peek() == '.' && isdigit(PeekNext())) {
        // Consume the "."
        Advance();

        while (isdigit(Peek())) Advance();
    }

    AddToken(NUMBER, std::stod(m_Source.substr(m_Start, m_Current - m_Start)));
}

void Lox::CScanner::Identifier() {
    while (isalnum(Peek()) || Peek() == '_') Advance();

    const std::string Text = m_Source.substr(m_Start, m_Current - m_Start);
    TokenType Type;
    if (const auto TypeIt = m_Keywords.find(Text); TypeIt != m_Keywords.end()) Type = TypeIt->second;
    else Type = IDENTIFIER;
    AddToken(Type);
}

bool Lox::CScanner::AtEnd() const {
    return m_Current >= m_Source.length();
}

void Lox::CScanner::ScanToken() {
    // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
    switch (char c = Advance()) {
        case '(': AddToken(LEFT_PAREN); break;
        case ')': AddToken(RIGHT_PAREN); break;
        case '{': AddToken(LEFT_BRACE); break;
        case '}': AddToken(RIGHT_BRACE); break;
        case ',': AddToken(COMMA); break;
        case '.': AddToken(DOT); break;
        case '-': AddToken(MINUS); break;
        case '+': AddToken(PLUS); break;
        case ';': AddToken(SEMICOLON); break;
        case '*': AddToken(STAR); break;

        case '!': AddToken(Match('=') ? BANG_EQUAL : BANG); break;
        case '=': AddToken(Match('=') ? EQUAL_EQUAL : EQUAL); break;
        case '<': AddToken(Match('=') ? LESS_EQUAL : LESS); break;
        case '>': AddToken(Match('=') ? GREATER_EQUAL : GREATER); break;

        case '/':
            if (Match('/'))
                while (Peek() != '\n' && !AtEnd()) Advance();
            else
                AddToken(SLASH);
        break;

        case ' ': case '\r': case '\t': break;
        case '\n': m_Line++; break;

        case '"': String(); break;

        default:
            if (isdigit(c)) Number();
            else if (isalpha(c) || c == '_') Identifier();
            else
                CInterpreter::Error(m_Line, "Unexpected character."); break;
    }
}

Lox::TokenContainer Lox::CScanner::ScanTokens() {
    while (!AtEnd()) {
        m_Start = m_Current;
        ScanToken();
    }

    AddEOFToken();
    return m_Tokens;
}
