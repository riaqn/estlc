#pragma once
#include <vector>
#include <string>

namespace ast{
	struct Type {
	};

	struct PrimitiveType : public Type {
		std::string name;
		PrimitiveType(const std::string& name);
	};

	struct AlgebraicType : public Type {
		struct Constructor {
			std::string name;
			std::vector<Type *> args;
		};
		std::vector<Constructor> constructors;
	};

	struct FunctionType : public Type {
		Type *left, *right;
		FunctionType(Type *left, Type *right);
		~FunctionType();
	};

	struct Term {
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
}

