#pragma once
#include <vector>
#include <string>
#include <utility>

namespace ast{
	struct Type {
		virtual ~Type() = 0;
	};

	struct PrimitiveType : public Type {
		std::string name;
		PrimitiveType(const std::string& name);
	};

	struct SumType : public Type {
		/*
		pair is the type and the converter from subtypes to supertype
		*/
		std::vector<std::pair<Type *, std::string> > types;
	};

	struct ProductType : public Type {
		Type *x, *y;
		std::string cons;
		ProductType(Type *x, Type *y, std::string cons);
		~ProductType();
	};

	struct FunctionType : public Type {
		Type *left, *right;
		FunctionType(Type *left, Type *right);
		~FunctionType();
	};

	struct Term {
		virtual ~Term() = 0;
	};

	struct Reference : public Term {
		std::string name;
		Reference(const std::string name);
	};

	struct Abstraction : public Term {
		std::string arg;
		Type* type; //type of arg
		Term* term;
		Abstraction(const std::string& arg, Type* type, Term* term);
		~Abstraction();
	};

	struct Application : public Term {
		Term* func;
		Term* arg;
		Application(Term* func, Term* arg);
		~Application();
	};

	struct Desum : public Term {
		/*
		the terms corresponding to subtypes
		*/
		std::vector<Term *> cases;
	};

	struct Deproduct : public Term {
		std::string x, y;
		Term *term;
	};
}

