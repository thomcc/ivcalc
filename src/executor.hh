#ifndef __EXECUTOR_HH__
#define __EXECUTOR_HH__
#include "visitors/compiler.hh"
#include "visitors/eval.hh"
#include "visitors/printer.hh"
#include "visitors/derivator.hh"
#include "visitors/simplifier.hh"
#include "interval.hh"

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"


namespace calc {

class Executor {
private:

	Simplifier _simplifier;
	Printer _printer;
	Compiler _compiler;
	Evaluator _evaluator;
	Derivator _derivator;

	llvm::FunctionPassManager _fpm;
	llvm::ExecutionEngine *_engine;

	DISALLOW_COPY_AND_ASSIGN(Executor);

public:
	Executor();




};






}


#endif