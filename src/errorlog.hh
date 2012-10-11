#ifndef __ERRORLOG_HH__
#define __ERRORLOG_HH__
#include "common.hh"
#include "token.hh"
namespace numbers {
	class ErrorLog {
		bool _repl;
		int _nerrors;
		bool _need_lines;
	public:
		ErrorLog(bool repl=false) : _repl(repl), _nerrors(0), _need_lines(false) {}
		void error(const Position &pos, const char *fmt, ...);
		void want_lines();
		int nerrors() const { return _nerrors; }
		bool need_lines() const { return _need_lines; }
	};
}











#endif