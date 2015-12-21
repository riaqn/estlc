#include "tokenStream.h"
#include "lexicalAnalyzer.h"
#include "syntaxAnalyzer.h"
#include "myException.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <set>

using namespace std;
using namespace ast;

inline void printSpace(ostream& os, int depth){
	for (int i = 0; i < depth; i++){
		os << ' ';
	}
}

void printType(const Type* type, ostream& os, int depth){
	static set<const Type*> types;
	if (types.find(type) != types.end()){
		printSpace(os, depth);
		os << "_type_\n";
		return;
	}
	else{
		types.emplace(type);
	}
	printSpace(os, depth);
	if (typeid(*type) == typeid(PrimitiveType)){
		os << "|Prim(" << ((PrimitiveType*)type)->name << ")\n";
	}
	else if (typeid(*type) == typeid(SumType)){
		os << "|Sum\n";
		for (unsigned i = 0; i < ((SumType*)type)->types.size(); i++){
			printSpace(os, depth);
			os << "(" << ((SumType*)type)->types[i].second << "\n";
			printType(((SumType*)type)->types[i].first, os, depth + 1);
			printSpace(os, depth);
			os << ")\n";
		}
	}
	else if (typeid(*type) == typeid(ProductType)){
		os << "|Product " << ((ProductType*)type)->cons << "\n";
		for (unsigned i = 0; i < ((ProductType*)type)->types.size(); i++){
			printType(((ProductType*)type)->types[i], os, depth + 1);
		}
	}
	else if (typeid(*type) == typeid(FunctionType)){
		os << "|Func" << "\n";
		printType(((FunctionType*)type)->left, os, depth + 1);
		printType(((FunctionType*)type)->right, os, depth + 1);
	}
}

void printTerm(const Term* term, ostream& os, int depth){
	printSpace(os, depth);
	if (typeid(*term) == typeid(Reference)){
		os << "|Ref(" << ((Reference*)term)->name << ")\n";
	}
	else if (typeid(*term) == typeid(Abstraction)){
		os << "|Abs " << ((Abstraction*)term)->arg << "\n";
		printType(((Abstraction*)term)->type, os, depth + 1);
		printTerm(((Abstraction*)term)->term, os, depth + 1);
	}
	else if (typeid(*term) == typeid(Application)){
		os << "|App\n";
		printTerm(((Application*)term)->func, os, depth + 1);
		printTerm(((Application*)term)->arg, os, depth + 1);
	}
	else if (typeid(*term) == typeid(Desum)){
		os << "|Desum\n";
		printTerm(((Desum*)term)->sum, os, depth + 1);
		for (unsigned i = 0; i < ((Desum*)term)->cases.size(); i++){
			printSpace(os, depth);
			os << "(" << ((Desum*)term)->cases[i].first << "\n";
			printTerm(((Desum*)term)->cases[i].second, os, depth + 1);
		}
	}
	else if (typeid(*term) == typeid(Deproduct)){
		os << "|Deproduct\n";
		printTerm(((Deproduct*)term)->product, os, depth + 1);
		printSpace(os, depth);
		os << "names:";
		for (unsigned i = 0; i < ((Deproduct*)term)->names.size(); i++){
			os << ((Deproduct*)term)->names[i] << ' ';
		}
		os << "\n";
		printTerm(((Deproduct*)term)->term, os, depth + 1);
	}
	else if (typeid(*term) == typeid(Fixpoint)){
		os << "|Fixpoint\n";
		printTerm(((Fixpoint*)term)->term, os, depth + 1);
	}
}

int main(){
	LexicalAnalyzer lexicalAnalyzer;
	//string srcfile = "it.estlc";
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

		printTerm(syntaxAnalyzer.getProgram()->term, os, 0);
		
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