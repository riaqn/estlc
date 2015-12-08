#pragma once
#include <stdexcept>

using namespace std;

class lexical_error : public runtime_error  {
public:
	explicit lexical_error(const unsigned& line, const char& peek, const string& what = "")
		:runtime_error(what), line(line), peek(peek)
	{
	}
	unsigned line;
	char peek;
};

class syntax_error : public runtime_error {
public:
	explicit syntax_error(const string& token, const unsigned& line, const string& what = "")
		:runtime_error(what), token(token), line(line)
	{
	}
  ~syntax_error() throw (){}
	string token;
	unsigned line;
};
