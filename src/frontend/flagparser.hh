#ifndef __FLAG_PARSER_HH__
#define __FLAG_PARSER_HH__
#include <cassert>
#include <functional>
#include <string>
#include <map>
#include <vector>
#include <deque>
#include <memory>
#include <iostream>
// taken from another project of mine, which was compiled with
// -fno-exceptions, hence the c-style error handling
// and repeated code present elsewhere

namespace flag {


template <typename T> struct ErrorOr;

template <> struct ErrorOr<void> {
	ErrorOr() : did_error(false), msg("") {}
	ErrorOr(std::string const &s) : did_error(true), msg(s) {}
	template <typename S> ErrorOr(ErrorOr<S> const &es)
		: did_error(es.did_error), msg(es.msg) {}
	operator bool() const { return !did_error; }
	bool good() const { return !did_error; }
	bool bad() const { return did_error; }
	bool did_error;
	std::string msg;
};

typedef ErrorOr<void> Error;

template <typename T> struct ErrorOr : public Error {
	ErrorOr(T t) : Error(), value(t) {}
	ErrorOr(std::string const &s) : Error(s) {}
	ErrorOr(Error const &v) : Error(v), value() {}
	T value;
};

// thrown if a flag is redefined or if Flagset::error_handling is FlagSet::ThrowOnError.
class flag_error : public std::runtime_error {
public:
	flag_error() : std::runtime_error("flag_error") {}
	template<typename T>
	flag_error(ErrorOr<T> err) : std::runtime_error(err.msg) {}
	flag_error(char const *msg) : std::runtime_error(msg) {}
	flag_error(std::string const &msg) : std::runtime_error(msg) {}
};

struct BoolValue;
struct IntValue;
struct StringValue;
struct DoubleValue;

struct Value {
	enum Type { BoolType, IntType, StringType, DoubleType };
	virtual ~Value() {}
	virtual Type type() const = 0;
	virtual std::string str() const = 0;
	virtual Error set(std::string const &v) = 0;

	virtual BoolValue *as_bool() { return nullptr; }
	virtual DoubleValue *as_double() { return nullptr; }
	virtual IntValue *as_int() { return nullptr; }
	virtual StringValue *as_string() { return nullptr; }
};


struct BoolValue : public Value {
	bool &value;
	BoolValue(bool &b) : value(b) {}

	std::string str() const { return format(value); }
	Error set(std::string const &v);
	Type type() const { return Value::BoolType; }
	BoolValue *as_bool() { return this; }
	static ErrorOr<bool> parse(std::string const &s);
	static std::string format(bool b);
};

struct IntValue : public Value {
	int &value;
	IntValue(int &i) : value(i) {}
	std::string str() const { return format(value); }
	Type type() const { return Value::IntType; }
	IntValue *as_int() { return this; }
	Error set(std::string const &v);
	static ErrorOr<int> parse(std::string const &s);
	static std::string format(int v);
};

struct StringValue : public Value {
	std::string &value;
	StringValue(std::string &s) : value(s) {}
	Type type() const { return Value::StringType; }
	StringValue *as_string() { return this; }
	std::string str() const { return value; }
	Error set(std::string const &v) { value = std::string(v); return Error(); }
};

struct DoubleValue : public Value {
	double &value;
	DoubleValue(double &d) : value(d) {}
	std::string str() const { return format(value); }
	Type type() const { return Value::DoubleType; }
	DoubleValue *as_double() { return this; }
	Error set(std::string const &v);
	static ErrorOr<double> parse(std::string const &s);
	static std::string format(double v);
};

struct Flag {
	Flag(std::string, std::string, std::shared_ptr<Value> v);
	Flag();
	Flag(Flag const &o) = default;
	Flag &operator=(Flag const &o) = default;
	std::string name;
	std::string usage;
	std::string def_value;
	// todo: un shared_ptrify now that theres no sharing.
	std::shared_ptr<Value> value;
};




class FlagSet {
public:
	enum ErrorHandling {
		ContinueOnError, ExitOnError, ThrowOnError
	};
private:
	std::function<void(FlagSet&)> usage;
	std::string                   name;
	bool                          parsed;
	std::map<std::string, Flag>   actual;
	std::map<std::string, Flag>   formal;
	std::deque<std::string>       args;
	ErrorHandling                 error_handling;
	bool                          exit_on_error;
	int                           original_argc;
	char                        **original_argv;
	FILE                         *out;
public:
	FlagSet(int argc, char **argv, FILE *fp=nullptr, ErrorHandling eh=ExitOnError);
	void  SetArgs(int argc, char **argv);
	void  SetOutput(FILE *fp);
	void  SetErrorHandling(ErrorHandling eh);
	FILE *Output() const;
	bool  Parsed() const;
	Error Parse();
	void  SetUsage(std::function<void(FlagSet&)> fn);
	void  Usage();
	void  VisitAll(std::function<void(Flag&)> fn);
	void  PrintDefaults();

	int NArgs() const; // number of args after parsing
	std::string Arg(int n) const;
	std::deque<std::string> Args() const;

//	bool &Bool(std::string name, bool value, std::string usage);
	void Bool(bool &b, std::string name, std::string usage);

//	int &Int(std::string name, int value, std::string usage);
	void Int(int &i, std::string name, std::string usage);

//	std::string &String(std::string name, std::string value, std::string usage);
	void String(std::string &s, std::string name, std::string usage);

//	double &Double(std::string name, double value, std::string usage);
	void Double(double &d, std::string name, std::string usage);

	void Var(std::shared_ptr<Value> value, std::string name, std::string usage);
private:
	void default_usage();
	std::vector<Flag> sort_flags(std::map<std::string, Flag> const &flg) const;
	Error fail(std::string const &str);
	ErrorOr<bool> parse_one();

};


} // flag

#endif