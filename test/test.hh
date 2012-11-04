#ifndef __TEST_HH__
#define __TEST_HH__

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "common.hh"
#include "colors.hh"

#define Check(cnd) _check(__FILE__, __LINE__, __func__, #cnd, cnd)
#define CheckNot(cnd) Check(!(cnd))
#define CheckMsg(cnd, msg) _msg_check(__FILE__, __LINE__, __func__, msg, cnd)
#define CheckEq(expected, actual) _eql_check(__FILE__, __LINE__, __func__, #expected, #actual, expected, actual);
#define CheckNull(val) _null_check(__FILE__, __LINE__, __func__, #val, val)


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

	static void
	reset() {
		_tests = 0;
		_failures = 0;
	}

private:
	static int _tests, _failures;


protected:

	static enum verbosity _verbosity;

	static bool
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
		return cnd;
	}

	static bool
	_msg_check(char const *file,
	           int line,
	           char const *func,
	           char const *msg,
	           bool cnd) {
		++_tests;
		if (!cnd) {
			++_failures;
			if (_verbosity > quiet) {
				cout << R_BOLD("Failed")" @ ";
				cout << file << ":" << line << "(testing " << func << ")"<< ": ";
				cout << msg << endl;
			}
		} else if (_verbosity == verbose) {
			cout << G_BOLD("Passed") ": " << msg << endl;
		}
		return cnd;
	}

	template <typename L, typename R>
	static bool
	_eql_check(char const *file,
	           int line,
	           char const *func,
	           char const *expect,
	           char const *actual,
	           L const &val_lhs,
	           R const &val_rhs) {
		++_tests;
		bool okay = val_lhs == val_rhs;
		if (!okay) {
			++_failures;
			if (_verbosity > quiet) {
				cout << R_BOLD("Failed")" @ ";
				cout << file << ":" << line << "(testing " << func << ")"<< ": ";
				cout << "Expected " << expect << " == " << actual;
				cout << ", got " << val_lhs << " != " << val_rhs << "." << endl;
			}
		} else if (_verbosity == verbose) {
			cout << G_BOLD("Passed") ": " << expect << " == " << actual << endl;
		}
		return okay;
	}

	template <typename T>
	static T*
	_null_check(char const *file,
	            int line,
	            char const *func,
	            char const *expr,
	            T *val) {
		++_tests;
		if (val == nullptr) {
			++_failures;
			if (_verbosity > quiet) {
				cout << R_BOLD("Failed")" @ ";
				cout << file << ":" << line << "(testing " << func << ")"<< ": ";
				cout << "Expected " << expr << " not null." << endl;
			}
		} else if (_verbosity == verbose) {
			cout << G_BOLD("Passed") ": " << expr << " is not null." << endl;
		}
		return val;
	}


};











}













#endif