#include "flag_tests.hh"

namespace calc {

using namespace flag;


void FlagTest::test_parse() {
	char *args[] = {
		(char*)"program_name",
		(char*)"-bool",
		(char*)"-bool2=true",
		(char*)"--int", (char*)"22",
		(char*)"-int2=30",
		(char*)"-string", (char*)"asdffdsa",
		(char*)"-double", (char*)"1.23e-1",
		(char*)"extra_arg"
	};

	FlagSet flagset(11, args, 0, FlagSet::ContinueOnError);
	bool boolflag = false;
	bool bool2flag = false;
	int intflag = 0;
	int int2flag = 0;
	std::string stringflag = "0";
	double dblflag = 0.0;
	flagset.Bool(boolflag, "bool", "bool value");
	flagset.Bool(bool2flag, "bool2", "bool2 value");
	flagset.Int(intflag, "int", "int value");
	flagset.Int(int2flag, "int2", "int2 value");
	flagset.String(stringflag, "string", "string value");
	flagset.Double(dblflag, "double", "double value");
	Error e = flagset.Parse();
	Check(e.good());
	Check(flagset.Parsed());
	Check(boolflag);
	Check(bool2flag);
	CheckEq(22, intflag);
	CheckEq(30, int2flag);
	CheckEq(std::string("asdffdsa"), stringflag);
	CheckEq(1.23e-1, dblflag);
	CheckEq(1, flagset.NArgs());
	auto argres = flagset.Args();
	CheckEq(1, argres.size());
	CheckEq(std::string("extra_arg"), argres.at(0));
}




}

