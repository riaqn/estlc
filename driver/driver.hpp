#ifndef _DRIVER_H_
#define _DRIVER_H_

#include <string>
#include <fstream>
#include "..\common\include\ast.hpp"
#include "..\backend\include\codegen.hpp"

namespace Driver{
	enum termType{
		reference = 0,
		abstraction,
		appliction,
		desum,
		deproduct,
		fixpoint
	};
	enum typeType{
		primitive,
		function,
		sumtype,
		product
	};
	std::string json;
	std::map<const ast::Term *, Codegen::Term> *type_of_term;

	int myTermType(const ast::Term* term){
		// Reference  0
		if (typeid(*term) == typeid(ast::Reference))
			return reference;
		// Abstraction 1
		if (typeid(*term) == typeid(ast::Abstraction))
			return abstraction;
		// Application 2
		if (typeid(*term) == typeid(ast::Application))
			return appliction;
		// Desum 3
		if (typeid(*term) == typeid(ast::Desum))
			return desum;
		// Deproduct  4
		if (typeid(*term) == typeid(ast::Deproduct))
			return deproduct;
		// Fixpoint  5
		if (typeid(*term) == typeid(ast::Fixpoint))
			return fixpoint;
		return -1;
	}
	int mytypeType(const ast::Type* type){
		if (typeid(*type) == typeid(ast::PrimitiveType))
			return primitive;
		if (typeid(*type) == typeid(ast::FunctionType))
			return function;
		if (typeid(*type) == typeid(ast::SumType))
			return sumtype;
		if (typeid(*type) == typeid(ast::ProductType))
			return product;
		return -1;
	}

	void printTerm(const ast::Term* term);

	void printFuncType(const ast::Type* type){
		if (mytypeType(type) == primitive){
			json += ((ast::PrimitiveType *)type)->name;
			return;
		}
		switch (mytypeType(type)){
		case function:
			printFuncType(((ast::FunctionType *)type)->left);
			json += "->";
			printFuncType(((ast::FunctionType *)type)->right);
			break;
		case product:
			json += "Product Type";
		case sumtype:
			json += "Sum Type";
		}

	}
	void printType(const ast::Type* type){
		switch (mytypeType(type)){
		case primitive:
			json += "\"" + ((ast::PrimitiveType *)type)->name + "\"";
			break;
		case function:
			json += "{\"id\":\"FuncType\", ";
			json += "\"value\":\"Function Type<br>" + type->to_string() + "\", ";
			json += "\"left\":\"";
			printFuncType(((ast::FunctionType *)type)->left);
			json += "\", \"right\":\"";
			printFuncType(((ast::FunctionType *)type)->right);
			json += "\"";
			json += "}";
			break;
		case sumtype:
			json += "\"Sum Type\"";
			break;
		case product:
			json += "\"Product Type\"";
			break;
		}

	}

	void printReference(const ast::Reference* ref){
		json += "{ \"id\":\"Reference\", ";
		json += "\"value\" : \"" + ref->name + "<br>" + (*type_of_term)[ref].type->to_string() + "\" }";
	}
	void printAbstraction(const ast::Abstraction* abs){
		json += "{ \"id\":\"Abstraction\", ";
		json += "\"value\":\"Abstraction\", ";
		//json += "\"value\":\"Abstraction<br>" + (*type_of_term)[abs].type->to_string() + "\", ";
		json += "\"arg\":\"" + abs->arg + "\", ";

		json += "\"type\":";
		printType(abs->type);
		json += ", ";

		json += "\"term\":";
		printTerm(abs->term);
		json += " }";
	}
	void printApplication(const ast::Application* app){
		json += "{ \"id\":\"Application\", ";
		json += "\"value\":\"Application<br>" + (*type_of_term)[app].type->to_string() + "\", ";

		json += "\"func\":";
		printTerm(app->func);
		json += ", ";

		json += "\"arg\":";
		printTerm(app->arg);
		json += " }";
	}
	void printDesum(const ast::Desum* sum){
		json += "{ \"id\":\"Desum\", ";
		json += "\"value\":\"Desum\", ";

		json += "\"sum\":";
		printTerm(sum->sum);
		json += ", ";

		json += "\"cases\":[";

		int sz = sum->cases.size();
		for (int i = 0; i < sz; i++){
			json += "{";

			json += "\"id\":\"Case\", ";
			json += "\"value\":\"Case<br>" + (*type_of_term)[sum->cases[i].second].type->to_string() + "\", ";
			json += "\"name\":\"" + sum->cases[i].first + "\", ";
			json += "\"term\":";
			printTerm(sum->cases[i].second);

			json += "}";
			if (i != sz - 1)
				json += ", ";
		}

		json += "]}";
	}
	void printDeproduct(const ast::Deproduct* dep){
		json += "{ \"id\":\"Deproduct\", ";
		json += "\"value\":\"Deproduct<br>" + (*type_of_term)[dep].type->to_string() + "\", ";

		json += "\"product\":";
		printTerm(dep->product);
		json += ", ";

		json += "\"names\":\"";
		for (int i = 0; i < dep->names.size(); i++){
			json += dep->names[i] + ' ';
		}
		json += "\", ";

		json += "\"term\":";
		printTerm(dep->term);

		json += " }";
	}
	void printFixpoint(ast::Fixpoint* fix){
		json += "{ \"id\":\"Fixpoint\", ";
		json += "\"value\":\"Fixpoint<br>" + (*type_of_term)[fix].type->to_string() + "\", ";

		json += "\"term\":";
		printTerm(fix->term);

		json += " }";
	}

	void printTerm(const ast::Term* term){
		switch (myTermType(term)){
		case reference:
			printReference((ast::Reference *)term);
			break;
		case abstraction:
			printAbstraction((ast::Abstraction *)term);
			break;
		case appliction:
			printApplication((ast::Application *)term);
			break;
		case desum:
			printDesum((ast::Desum *)term);
			break;
		case deproduct:
			printDeproduct((ast::Deproduct *)term);
			break;
		case fixpoint:
			printFixpoint((ast::Fixpoint *)term);
			break;
		}
	}

	void serialize(ast::Program* prog, std::map<const ast::Term *, Codegen::Term> *tmp){
		type_of_term = tmp;

		json = "";
		printTerm(prog->term);

		//found = str.find("needles are small", found + 1, 6);
		std::size_t found;
		while ((found = json.find("'")) != std::string::npos){
			json.replace(found, 1, "[prime]");
		}

		std::ifstream in("driver/template/static/reserve.js");
		std::ofstream out("driver/template/static/template.js");
		out << "var _MAGIC_AST_ =  '" << json << "';\n";
		
		char buffer[10000];
		while (in.getline(buffer, 10000)){
			out << buffer << std::endl;
		}
		out.close();

	}
}


#endif