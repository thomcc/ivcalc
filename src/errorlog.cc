#include "errorlog.hh"
#include "common.hh"

namespace numbers {

	void ErrorLog::error(const Position &pos, const char *fmt, ...) {
		if (_need_lines) return;
		char message[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(message, sizeof(message), fmt, args);
		va_end(args);
		std::cerr << "[Error] " << pos.file() << " line "
			<< pos.start_line() << " col " << pos.start_col()
			<< ". '" << message << "'" << std::endl;
	}

	void ErrorLog::want_lines() {
		if (_repl) _need_lines = true;
	}

}

