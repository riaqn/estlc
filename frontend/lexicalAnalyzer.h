#pragma once
#include "tokenStream.h"
#include <iostream>
class LexicalAnalyzer
{
public:
	LexicalAnalyzer();
	~LexicalAnalyzer();

	TokenStream parse(istream& is);

private:

};

