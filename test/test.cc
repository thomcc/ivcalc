#include "test.hh"


namespace calc {

int Test::_tests = 0;
int Test::_failures = 0;
Test::verbosity Test::_verbosity = normal;


void Test::summary() {
	if (_verbosity > silent) {
		if (_failures == 0) {
			cout << G_BOLD("PASS") ": all " << _tests << " tests passed." << endl;
		} else {
			cout << R_BOLD("FAIL") ": " << (_tests - _failures);
			cout << " tests passed out of " << _tests << "." << endl;
		}
	}
}

void Test::run() {
	rmath::set_rnear();
	try {
		run_tests();
	} catch (std::exception const &e) {
		CheckMsg(false, (std::string("Failure: uncaught exception: ")+e.what()).c_str());
	} catch (std::string const &s) {
		CheckMsg(false, (std::string("Failure: uncaught exception: ")+s).c_str());
	} catch (...) {
		CheckMsg(false, "Failure: uncaught exception");
	}
}


}