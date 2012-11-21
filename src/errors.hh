#ifndef __ERRORS_HH__
#define __ERRORS_HH__

#include "common.hh"
#include <llvm/Support/raw_ostream.h>

namespace calc {

enum ELevel {
	E_Error = llvm::raw_ostream::RED,
	E_Warn  = llvm::raw_ostream::YELLOW,
	E_Info  = llvm::raw_ostream::BLUE,
	E_Bug   = llvm::raw_ostream::MAGENTA,
};

class ErrorHandler {
public:
	virtual void error(std::string const &msg, ELevel lvl=E_Error) = 0;
	virtual unsigned errors() const = 0;
	virtual bool need_lines() const = 0;
	virtual void set_need_lines() = 0;

	static std::unique_ptr<ErrorHandler> make_cerr(bool repl=false);
	static std::unique_ptr<ErrorHandler> make_cout(bool repl=false);
	static std::unique_ptr<ErrorHandler> make_silent(bool repl=false);
};

class BasicErrorHandler : public ErrorHandler {
	int _errors;
	llvm::raw_ostream &_out;
public:
	BasicErrorHandler(llvm::raw_ostream &out=llvm::errs()) : _errors(0), _out(out) {}
	virtual void error(std::string const &msg, ELevel lvl=E_Error) {
		if (need_lines()) return;
		++_errors;
		_out << "[";
		_out.changeColor((llvm::raw_ostream::Colors)lvl, lvl==E_Error || lvl==E_Bug);
		switch (lvl) {
		case E_Error: _out << "Error"; break;
		case E_Warn: _out << "Warning"; break;
		case E_Info: _out << "Info"; break;
		case E_Bug: default: _out << "Bug"; break;
		}
		_out.resetColor();
		_out << "]: " << msg << "\n";
		_out.flush();
	}
	virtual unsigned errors() const { return _errors; }
	virtual void set_need_lines() {}
	virtual bool need_lines() const { return false; }
};

class ReplErrorHandler : public BasicErrorHandler {
	bool _need_lines;
public:
	ReplErrorHandler(llvm::raw_ostream &out=llvm::errs()) : BasicErrorHandler(out), _need_lines(false) {}
	virtual void set_need_lines() { _need_lines = true; }
	virtual bool need_lines() const { return _need_lines; }
};


inline std::unique_ptr<ErrorHandler> ErrorHandler::make_cerr(bool repl) {
	if (repl) return std::unique_ptr<ErrorHandler>{new ReplErrorHandler};
	return std::unique_ptr<ErrorHandler>{new BasicErrorHandler};
}
inline std::unique_ptr<ErrorHandler> ErrorHandler::make_cout(bool repl) {
	if (repl) return std::unique_ptr<ErrorHandler>{new ReplErrorHandler(llvm::outs())};
	return std::unique_ptr<ErrorHandler>{new BasicErrorHandler(llvm::outs())};
}


inline std::unique_ptr<ErrorHandler> ErrorHandler::make_silent(bool repl) {
	if (repl) return std::unique_ptr<ErrorHandler>{new ReplErrorHandler(llvm::nulls())};
	return std::unique_ptr<ErrorHandler>{new BasicErrorHandler(llvm::nulls())};
}





}

#endif