#pragma once
#include <vector>
#include <string>

namespace ast{
	struct Type {
		virtual std::string toString() const = 0;
	};

	struct PrimitiveType : public Type {
		std::string name;
		PrimitiveType(const std::string& name) :name(name){}
		std::string toString() const{ return name; }
	};

	struct AlgebraicType : public Type {
		struct Constructor {
			std::string name;
			std::vector<Type *> args;
		};
		std::vector<Constructor> constructors;
		std::string toString() const{
			std::string ret = "AlgeType:";
			for (unsigned i = 0; i < constructors.size(); i++){
				ret = ret + constructors[i].name + " : ";
				if (constructors[i].args.size() > 0){
					ret += constructors[i].args[0]->toString();
					for (unsigned j = 1; j < constructors[i].args.size(); j++){
						ret = ret + "->" + constructors[i].args[j]->toString();
					}
				}
			}
			return ret;
		}
	};

	struct FunctionType : public Type {
		Type *left, *right;
		FunctionType(Type *left, Type *right) :left(left), right(right){}
		~FunctionType(){
			if (left != NULL && ((PrimitiveType*)left) == NULL){
				delete left;
			}
			if (right != NULL && ((PrimitiveType*)right) == NULL){
				delete right;
			}	
		}
		std::string toString()const{ return left->toString() + "->" + right->toString(); }
	};

	struct Term {
		virtual std::string toString()const = 0;
	};

	struct Reference : public Term {
		std::string name;
		Reference(const std::string name) :name(name){}
		std::string toString() const{ return "\"" + name + "\""; }
	};

	struct Abstraction : public Term {
		std::string arg;
		Type* type; //type of arg
		Term* term;
		Abstraction(const std::string& arg, Type* type, Term* term) :arg(arg), type(type), term(term){}
		~Abstraction(){
			if (type != NULL && ((PrimitiveType*)type) == NULL){
				delete type;
			}
			if (term != NULL){
				delete term;
			}
		}
		std::string toString() const{ return "{\"symbol\":\"Abstraction\", \"arg\":\"" + arg + "\", \"type\":\"" + type->toString() + "\", \"term\":" + term->toString() + "}"; }
	};

	struct Application : public Term {
		Term* func;
		Term* arg;
		Application(Term* func, Term* arg) :func(func), arg(arg){}
		~Application(){
			if (func != NULL){
				delete func;
			}
			if (arg != NULL){
				delete arg;
			}
		}
		std::string toString()const { return "{\"symbol\":\"Application\", \"func\":" + \
											 func->toString() + ", \"arg\":" + arg->toString() + "}"; }
	};
}

