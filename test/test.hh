#ifndef __TEST_HH__
#define __TEST_HH__

#include <iostream>
#include "common.hh"
#include "colors.hh"

#define Check(cnd) _check(__FILE__, __LINE__, __func__, #cnd, cnd)
#define CheckNot(cnd) Check(!(cnd))
#define CheckMsg(cnd, msg) _msg_check(__FILE__, __LINE__, __func__, msg, cnd)
#define CheckEq(expected, actual) _eql_check(__FILE__, __LINE__, __func__, #actual, expected, actual);


namespace calc {

using std::cout;
using std::endl;



class Test {
public:
	enum verbosity { silent, quiet, normal, verbose };

	virtual ~Test() {}

	static void summary();

	static int 
	failures() {
		return _failures;
	}

	static int
	tests() {
		return _tests;
	}

	void run();

	virtual void run_tests() = 0;

	static void
	set_verbosity(enum verbosity v) {
		_verbosity = v;
	}

private:
	static int _tests, _failures;


protected:
	
	static enum verbosity _verbosity;

	static void 
	_check(char const *file,
	       int line,
	       char const *func,
	       char const *expr,
	       bool cnd) {
		++_tests;
		if (!cnd) {
			++_failures;
			if (_verbosity > quiet) {
				cout << R_BOLD("Failed")" @ (";
				cout << file << " : " << func << " : " << line << "): "; 
				cout << expr << " was false." << endl;
			}
		} else if (_verbosity == verbose) {
			cout << G_BOLD("Passed") ": " << expr << endl;
		}
	}

	static void
	_msg_check(char const *file,
	           int line,
	           char const *func,
	           char const *msg,
	           bool cnd) {
		++_tests;
		if (!cnd) {
			++_failures;
			if (_verbosity > quiet) {
				cout << R_BOLD("Failed")" @ (";
				cout << file << " : " << func << " : " << line << "): ";
				cout << msg << endl;
			}
		} else if (_verbosity == verbose) {
			cout << G_BOLD("Passed") ": " << msg << endl;
		}
	}

	template <typename L, typename R>
	static void
	_eql_check(char const *file,
	           int line,
	           char const *func,
	           char const *expr, 
	           L const &val_lhs,
	           R const &val_rhs) {
		++_tests;
		if (!(val_lhs == val_rhs)) {
			++_failures;
			if (_verbosity > quiet) {
				cout << R_BOLD("Failed")" @ (";
				cout << file << " : " << func << " : " << line << "): ";
				cout << "Expected that" << expr << " == " << val_lhs;
				cout << ", got " << val_rhs << "." << endl;
			}
		} else if (_verbosity == verbose) {
			cout << G_BOLD("Passed") ": " << expr << " == " << val_lhs << endl;
		}
	}



};











}













#endif