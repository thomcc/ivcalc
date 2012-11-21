#ifndef __IPARSER_HH__
#define __IPARSER_HH__
#include "common.hh"
namespace calc {

class IParser {
public:
	virtual ExprPtr parse_expression() = 0;
	virtual int errors() const = 0;
	virtual bool need_lines() const = 0;
};

}
#endif