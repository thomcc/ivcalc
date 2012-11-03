#ifndef __QUEUE_HH__
#define __QUEUE_HH__
#include <memory>
#include "common.hh"

namespace calc {

template <typename T, int Size>
class Queue {
public:
	typedef T ElementType;

	Queue() : _head(0), _count(0) {}

	int
	count() const {
		return _count;
	}

	bool
	is_empty() const {
		return _count == 0;
	}

	int
	capacity() const {
		return Size;
	}

	void
	clear() {
		while (!is_empty()) dequeue();
	}

	void
	enqueue(ElementType const &item) {
		ASSERT(_count < Size);

		_items[_head] = item;

		_head = _wrap(_head + 1);

		++_count;
	}

	ElementType
	dequeue() {
		ASSERT(_count > 0);
		int tail = _wrap(_head - _count);
		ElementType item = _items[tail];
		_items[tail] = ElementType();
		--_count;
		return item;
	}

	ElementType const &
	operator[](int idx) const {
		RANGE_CHECK(idx, _count);
		return _items[_wrap(_head - _count + idx)];
	}

	friend std::ostream&
	operator<<(std::ostream &o, Queue const &q) {
		if (q.is_empty()) return o << "{|<empty queue>|}";
		o << "{";
		for (int i = 0; i < q.count(); ++i)
			o << (i ? " | " : "| ") << q[i];
		return o << "|}";
	}

private:

	int _head, _count;

	ElementType _items[Size];

	inline int
	_wrap(int idx) const {
		return (idx + Size) % Size;
	}

	DISALLOW_COPY_AND_SWAP(Queue);

};
}












#endif