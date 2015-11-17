#include "token.h"
#include "lexicalAnalyzer.h"
#include "syntaxAnalyzer.h"
#include <iostream>
#include <fstream>
#include <regex>
#include "myException.h"

using namespace std;
int main(){
	LexicalAnalyzer lexicalAnalyzer;
	string srcfile = "qsort.mfc";
	try{
		ifstream is(srcfile);
		if (!is){
			throw runtime_error("Non-exist source file \"" + srcfile + "\"");
		}
		TokenStream tokenStream = lexicalAnalyzer.parse(is);

		ofstream os("qsort.tks");
		for (unsigned i = 0; i < tokenStream.size(); i++){
			Token* tp = &tokenStream[i];
			os << "<" << TokenName[tp->type] << ">\t{" << tp->name << "}\tline " << tp->line << "\n";
		}
		os.close();
		os.clear();

		SyntaxAnalyzer syntaxAnalyzer(tokenStream);
		os.open("qsort.ast");
		syntaxAnalyzer.printTree(os);

	}
	catch (const lexical_error& e){
		cerr << srcfile << "(" << e.line << "):lexical error: \'" << e.peek << "\' is not a valid character\n";
		return 1;
	}
	catch (const syntax_error& e){
		cerr << srcfile << "(" << e.line << "):syntax error: " << e.what() << " with " << e.token << "\n";
		return 1;
	}
	catch (const exception& e){
		cerr << e.what() << "\n";
		return 1;
	}
	return 0;
}