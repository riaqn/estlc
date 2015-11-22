#pragma once

#include "../frontend/token.h"
#include "../frontend/lexicalAnalyzer.h"
#include "../frontend/syntaxAnalyzer.h"
#include "../frontend/myException.h"

#include <iostream>
#include <fstream>
#include <regex>

int print_html(const TokenStream &, const SyntaxAnalyzer &);