#include <interpreter.h>
#include <lexer/scanner.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <utils.h>

bool Lox::CInterpreter::ms_HadError = false;
Lox::ReportFunc Lox::CInterpreter::ms_fnReport = DefaultReport;
std::ostream* Lox::CInterpreter::ms_OutStream = &std::cout;
std::ostream* Lox::CInterpreter::ms_ErrStream = &std::cerr;

void Lox::CInterpreter::Error(const int Line, const std::string &Message) {
    Report(Line, "", Message);
}

void Lox::CInterpreter::Report(const int Line, const std::string &From, const std::string &Message) {
    ms_fnReport(Line, From, Message);
    ms_HadError = true;
}

void Lox::CInterpreter::AttachReportFunc(const ReportFunc& Func) {
    ms_fnReport = Func ? Func : DefaultReport;
}

void Lox::CInterpreter::AttachDefaultReportFunc() {
    AttachReportFunc(DefaultReport);
}

void Lox::CInterpreter::AttachOutStream(std::ostream *pStream) {
    ms_OutStream = pStream ? pStream : &std::cout;
}

void Lox::CInterpreter::AttachErrStream(std::ostream *pStream) {
    ms_ErrStream = pStream ? pStream : &std::cerr;
}

void Lox::CInterpreter::AttachDefaultErrStream() {
    ms_ErrStream = &std::cerr;
}

void Lox::CInterpreter::AttachDefaultOutStream() {
    ms_OutStream = &std::cout;
}

void Lox::CInterpreter::DefaultReport(const int Line, const std::string& From, const std::string& Message) {
    Err() << "[line " + to_string(Line) + "] Error" + From + ": " + Message << "\n";
}

void Lox::CInterpreter::Run(const std::string &Source) {
    CScanner Scanner(Source);

    bool First = true;
    for (const auto Tokens = Scanner.ScanTokens(); const auto &Token: Tokens) {
        if (!First) Out() << "\n"; First = false;
        Out() << Token->str();
    }
}

void Lox::CInterpreter::RunFile(const char *pPath) {
    std::ifstream Fstrm;
    Fstrm.open(pPath);

    if (!Fstrm.is_open()) {
        Err() << "Failed to open " << pPath << "\n";
        exit(2);
    }

    std::stringstream Buffer;
    Buffer << Fstrm.rdbuf();

    Run(Buffer.str());

    if (ms_HadError) {
        exit(65);
    }
}

void Lox::CInterpreter::RunPrompt() {
    Out() << "You entered Interactive Interpreter mode\n";
    while (true) {
        Out() << "> ";
        std::string Input;
        std::getline(std::cin, Input);
        Input = trim(Input.c_str());
        if (Input.empty())
            break;
        Run(Input);
        Out() << "\n";
        ms_HadError = false;
    }
    Out() << "Goodbye!\n";
}
