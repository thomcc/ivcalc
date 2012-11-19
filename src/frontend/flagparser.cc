#include "flagparser.hh"
#include <cstdint>
#include <sstream>
//#include <cfloat>
#include <cmath>
#include <algorithm>


namespace flag {

using namespace std;


Flag::Flag() : name(""), usage(""), def_value(""), value(nullptr) {}

Flag::Flag(string name, string usage, shared_ptr<Value> v)
	: name(name), usage(usage), def_value(v->str()), value(v) {}

Error DoubleValue::set(string const &v) {
	ErrorOr<double> eb = parse(v);
	if (eb.good()) {
		value = eb.value;
		return Error();
	} else return Error(eb);
}

Error IntValue::set(string const &v) {
	ErrorOr<int> eb = parse(v);
	if (eb.good()) {
		value = eb.value;
		return Error();
	} else return Error(eb);
}

Error BoolValue::set(string const &v) {
	ErrorOr<bool> eb = parse(v);
	if (eb.good()) {
		value = eb.value;
		return Error();
	} else return Error(eb);
}

ErrorOr<bool> BoolValue::parse(string const &s) {
	const string trues[]  = {
		"1", "t", "T", "true", "TRUE", "True", "yes", "YES", "Yes", "y", "Y", "on", "ON", "On"
	};
	const string falses[] = {
		"0", "f", "F", "false", "FALSE", "False", "no", "NO", "No", "n", "N", "off", "OFF", "Off"
	};
	for (auto const &t : trues) if (s == t) return ErrorOr<bool>(true);
	for (auto const &f : falses) if (s == f) return ErrorOr<bool>(false);
	return ErrorOr<bool>("SyntaxError: parse_bool");
}

string BoolValue::format(bool b) {
	return b ? "true" : "false";
}

class to_s {
private:
	ostringstream _ss;
public:
	template <typename T> to_s&
	operator<<(T const &v) { _ss << v; return *this; }
	operator string() const { return _ss.str(); }
};


// can't use >> or atoi, etc. because of the lack of indication of failure.
static ErrorOr<int64_t>
parse_int(string s) {
	char *end;
	errno = 0;
	long l = strtol(s.c_str(), &end, 0);
	if ((errno == ERANGE && l == LONG_MAX) || l > INT_MAX)
		return ErrorOr<int64_t>(to_s() << "Bad integer (overflow): " << s);
	if ((errno == ERANGE && l == LONG_MIN) || l < INT_MIN)
		return ErrorOr<int64_t>(to_s() << "Bad integer (underflow): " << s);
	if (s[0] == '\0' || *end != '\0')
		return ErrorOr<int64_t>(to_s() << "Bad integer (inconvertible): " << s);
	return ErrorOr<int64_t>(l);
}

string IntValue::format(int v) {
	return to_s() << v;
}

ErrorOr<int> IntValue::parse(string const &s) {
	ErrorOr<int64_t> ei = parse_int(s);
	if (ei.bad()) return ErrorOr<int>(ei.msg);
	else return ErrorOr<int>(ei.value);
}

ErrorOr<double> parse_f64(string const &s) {
	char *end;
	errno = 0;
	double d = strtod(s.c_str(), &end);
	if (errno == ERANGE && d == HUGE_VAL)
		return ErrorOr<double>(to_s() << "Bad double (overflow): " << s);
	if (errno == ERANGE && d == -HUGE_VAL)
		return ErrorOr<double>(to_s() << "Bad double (underflow): " << s);
	if (s[0] == '\0' || *end != '\0')
		return ErrorOr<double>(to_s() << "Bad double (inconvertible): " << s);
	return ErrorOr<double>(d);
}

ErrorOr<double> DoubleValue::parse(string const &s) {
	return parse_f64(s);
}

string DoubleValue::format(double v) {
	return to_s() << v;
}

vector<Flag> FlagSet::sort_flags(map<string, Flag> const &flgs) const {
	vector<string> list;
	for (auto const &fp : flgs)
		list.push_back(fp.first);
	sort(list.begin(), list.end());
	vector<Flag> result;
	for (auto const &name : list)
		result.push_back(flgs.at(name));
	return result;
}

Error FlagSet::fail(string const &err) {
	fprintf(Output(), "%s\n", err.c_str());
	Usage();
	return Error(err);
}

string FlagSet::Arg(int n) const {
	if (n < 0 || n > args.size()) return "";
	return args.at(n);
}

deque<string> FlagSet::Args() const {
	return args;
}

int FlagSet::NArgs() const {
	return args.size();
}

void FlagSet::Complain(std::string const &complaint) {
	fprintf(Output(), "%s: error: ", name.c_str());
	fputs(complaint.c_str(), Output());
	Usage();
}


ErrorOr<bool> FlagSet::parse_one() {
	if (!args.size()) return ErrorOr<bool>(false);
	string s = args[0];
	if (s.size() == 0 || s[0] != '-' || s.size() == 1)
		return ErrorOr<bool>(false);
	int ndash = 1;
	if (s[1] == '-') {
		++ndash;
		if (s.size() == 2) {
			args.pop_front();
			return ErrorOr<bool>(false);
		}
	}
	string name = s.substr(ndash);
	if (name.size() == 0 || name[0] == '-' || name[0] == '=')
		return fail("Bad Flag syntax: "+s);

	args.pop_front();

	bool has_value = false;
	string value = "";

	for (int i = 1; i < name.size(); ++i) { // '=' cant be first
		if (name[i] == '=') {
			value = name.substr(i+1);
			has_value = true;
			name = name.substr(0, i);
			break;
		}
	}
	auto const &it = formal.find(name); // bug
	if (it == formal.end()) {
		if (name == "help" || name == "h") { // special case: help message
			return fail("help requested");
		}
		return fail("Flag provided but not defined: -"+name);
	}
	Flag f = it->second;
	if (BoolValue *fv = f.value->as_bool()) { // special case: doesnt need arg
		if (has_value) {
			Error e = fv->set(value);
			if (e.bad())
				fail("invalid boolean value "+fv->str()+" for -"+name+": "+e.msg);
		} else fv->set("true");
	} else {
		if (!has_value && args.size() > 0) {
			// value is the next arg.
			has_value = true;
			value = args.front();
			args.pop_front();
		}
		if (!has_value)
			return fail("Flag needs an argument: -"+name);
		Error e = f.value->set(value);
		if (e.bad()) return fail("invalid value '"+value+"' for flag -"+name+": "+e.msg);
	}
	actual[name] = f;
	return ErrorOr<bool>(true);
}

Error FlagSet::Parse() {
	parsed = true;
	for (;;) {
		ErrorOr<bool> seen = parse_one();
		if (seen.good() && seen.value) continue;
		if (seen.good() && !seen.value) break;

		switch (error_handling) {
		case ContinueOnError: return seen;
		case ExitOnError: exit(2);
		case ThrowOnError: throw flag_error(seen);
		}
	}
	return Error();
}

void FlagSet::SetArgs(int argc, char **argv) {
	assert(argc != 0);
	assert(argv != nullptr);
	original_argc = argc;
	original_argv = argv;
	name = string(argv[0]);
	args.clear();
	actual.clear();
	for (int i = 1; i < argc; ++i)
		args.push_back(string(argv[i]));
	parsed = false;
}

void FlagSet::SetUsage(function<void(FlagSet&)> fn) {
	usage = fn;
}

void FlagSet::Usage() {
	if (usage == nullptr) default_usage();
	else usage(*this);
}


void FlagSet::VisitAll(function<void(Flag&)> fn) {
	for (auto &f : sort_flags(formal)) fn(f);
}

void FlagSet::default_usage() {
	fprintf(Output(), "Usage of %s: \n", name.c_str());
	PrintDefaults();
}

void FlagSet::PrintDefaults() {
	int longest = 0;
	VisitAll([this, &longest](Flag &flag) {
		const char *fmt = "  -%s=%s";
		if (flag.value->as_string()) fmt = "  -%s='%s'";
		size_t len = snprintf(NULL, 0, fmt, flag.name.c_str(), flag.def_value.c_str());
		if (len > longest) longest = len;
	});
	VisitAll([this, longest](Flag &flag) {
		const char *fmt = "  -%s=%s:";
		if (flag.value->as_string()) fmt = "  -%s='%s':";
		FILE *fp = Output();
		size_t len = fprintf(fp, fmt, flag.name.c_str(), flag.def_value.c_str());
		fflush(fp);
		while (len++ < 4+longest) fputc(' ', fp);
		fflush(fp);
		fprintf(fp, "%s\n", flag.usage.c_str());
		fflush(fp);
//		fprintf(Output(), "%s", flag.name.c_str(), flag.def_value.c_str(), flag.usage.c_str());
	});
}

void FlagSet::SetOutput(FILE *fp) { out = fp; }

void FlagSet::SetErrorHandling(ErrorHandling eh) {
	error_handling = eh;
	if (eh == ExitOnError) exit_on_error = true;
}

FlagSet::FlagSet(int argc, char **argv, FILE *fp, ErrorHandling eh)
	: usage(nullptr), error_handling(eh), exit_on_error(eh==ExitOnError), out(fp)
{ SetArgs(argc, argv); }

void FlagSet::Var(shared_ptr<Value> v, string name, string usage) {
	Flag f(name, usage, v);
	auto alreadythere = formal.find(name);
	if (alreadythere != formal.end()) {
		fprintf(Output(), "%s flag redefined %s\n", this->name.c_str(), name.c_str());
		fflush(Output());
		throw flag_error("Flag Redefined");
	}
	formal[name] = f;
}

void FlagSet::Bool(bool &b, string name, string usage) {
	shared_ptr<Value> v = static_pointer_cast<Value>(make_shared<BoolValue>(b));
	Var(v, name, usage);
}
void FlagSet::Int(int &i, string name, string usage) {
	shared_ptr<Value> v = static_pointer_cast<Value>(make_shared<IntValue>(i));
	Var(v, name, usage);
}
void FlagSet::String(string &s, string name, string usage) {
	shared_ptr<Value> v = static_pointer_cast<Value>(make_shared<StringValue>(s));
	Var(v, name, usage);
}
void FlagSet::Double(double &d, string name, string usage) {
	shared_ptr<Value> v = static_pointer_cast<Value>(make_shared<DoubleValue>(d));
	Var(v, name, usage);
}

FILE *FlagSet::Output() const { return out ? out : stderr; }
bool  FlagSet::Parsed() const { return parsed; }


}