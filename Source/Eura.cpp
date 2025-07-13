#if eura_major >= 0 and eura_middle >= 0 and eura_minor >= 1
#include "Core/CLI.hpp"

int main(int argc, char** argv)
{
    Eura::CLI::Parse(argc, argv);
}

#endif