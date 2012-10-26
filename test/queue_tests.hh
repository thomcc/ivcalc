#ifndef __QUEUE_TESTS_HH__
#define __QUEUE_TESTS_HH__

#include "test.hh"

namespace calc {
class QueueTest : public Test {
public:
	virtual void
	run_tests() {
		enqueue_dequeue();
		enqueue_seq();
		enqueue_several();
		count();
		access();
	}
private:
	void enqueue_dequeue();
	void enqueue_seq();
	void enqueue_several();
	void count();
	void access();
};

}



#endif