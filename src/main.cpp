#include <iostream>

#include <interpreter.h>

int main(int argc, char *argv[]) {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    Lox::CInterpreter::AttachDefaultOutStream();
    Lox::CInterpreter::AttachDefaultErrStream();
    Lox::CInterpreter::AttachDefaultReportFunc();

    if (argc == 1) {
        Lox::CInterpreter::RunPrompt();
    }
    else if (argc == 2) {
        Lox::CInterpreter::RunFile(argv[1]);
    }
    else {
        std::cerr << "Usage: " << argv[0] << " [file]\n";
        exit(1);
    }
    exit(0);
}
