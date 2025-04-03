#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <functional>
#include <string>
#include <ostream>

namespace Lox {
    typedef std::function<void(int, std::string, std::string)> ReportFunc;

    class CInterpreter final {
        static bool ms_HadError;

        static std::ostream* ms_OutStream;
        static std::ostream* ms_ErrStream;

        static ReportFunc ms_fnReport;
        static void DefaultReport(int, const std::string&, const std::string&);

    public:
        CInterpreter() = default;

        static std::ostream &Out() { return *ms_OutStream; }
        static std::ostream &Err() { return *ms_ErrStream; }
        static void Error(int Line, const std::string& Message);
        static void Report(int Line, const std::string& From, const std::string& Message);

        static void AttachReportFunc(const ReportFunc& Func);
        static void AttachDefaultReportFunc();
        static void AttachOutStream(std::ostream *pStream);
        static void AttachErrStream(std::ostream *pStream);
        static void AttachDefaultOutStream();
        static void AttachDefaultErrStream();

        static void Run(const std::string &Source);
        static void RunFile(const char *pPath);
        static void RunPrompt();
    };
}

#endif // INTERPRETER_H
