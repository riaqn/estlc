using namespace std;

int print_html(const TokenStream &tokenStream, const SyntaxAnalyzer &syntaxAnalyzer){
	try{
		ofstream os("driver/qsort.tks");
		os << "[";
		for (unsigned i = 0, sz = tokenStream.size(); i <sz; i++){
			Token* tp = &tokenStream[i];
			// {"type":"comment", "name" : "Hello", "line" : 2}, 
			os << "{\"type\":\"" << TokenName[tp->type] << "\", \"name\" : \"" << tp->name << "\", \"line\" : " << tp->line << "}";
			if (i != sz - 1)
				os << ", ";
		}
		os << "]";
		os.close();
		os.clear();

		os.open("driver/qsort.ast");
		syntaxAnalyzer.printTree(os);
		os.close();
		os.clear();
	}
	catch (const exception& e){
		cerr << e.what() << "\n";
		return 1;
	}
	return 0;
}