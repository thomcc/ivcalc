#include "queue_test.hh"
#include "parser/queue.hh"
namespace calc {

void
QueueTest::enqueue_dequeue() {
	Queue<int, 3> q;
	
	CheckEq(0, q.count());
	CheckEq(3, q.capacity());
	
	q.enqueue(2);
	
	CheckEq(1, q.count());
	CheckEq(2, q[0]);

	int r = q.dequeue();

	CheckEq(0, q.count());
	CheckEq(2, r);
}

void
QueueTest::enqueue_seq() {
	Queue<int,3> q;
	for (size_t i = 0; i < 6; ++i) {
		CheckEq(0, q.count());
		q.enqueue(i);
		int j = q.dequeue();
		CheckEq(i, j);
	}
}

void
QueueTest::enqueue_several() {
	Queue<int, 3> q;
	q.enqueue(5);
	q.enqueue(6);
	CheckEq(2, q.count());
	CheckEq(5, q.dequeue());
	CheckEq(1, q.count());
	CheckEq(6, q.dequeue());
	CheckEq(0, q.count());
	q.enqueue(7);
	q.enqueue(8);
	CheckEq(2, q.count());
	CheckEq(7, q.dequeue());
	CheckEq(1, q.count());
	CheckEq(8, q.dequeue());
	CheckEq(0, q.count());
}

void
QueueTest::count() {
	Queue<int, 3> q;
	CheckEq(0, q.count());
	q.enqueue(1);
	CheckEq(1, q.count());
	q.enqueue(1);
	CheckEq(2, q.count());
	q.enqueue(1);
	CheckEq(3, q.count());
	q.dequeue();
    CheckEq(2, q.count());
    q.enqueue(1);
    CheckEq(3, q.count());
    q.dequeue();
    CheckEq(2, q.count());
    q.dequeue();
    CheckEq(1, q.count());
    q.dequeue();
    CheckEq(0, q.count());
}

void
QueueTest::access() {
	Queue<int, 3> q;
	q.enqueue(5);
	q.enqueue(6);
	CheckEq(5, q[0]);
	CheckEq(6, q[1]);
	q.dequeue();
	CheckEq(6, q[0]);
	q.enqueue(7);
	q.enqueue(8);
	CheckEq(6, q[0]);
	CheckEq(7, q[1]);
	CheckEq(8, q[2]);
}










}

