#ifndef __QUEUE_HH__
#define __QUEUE_HH__
#include <memory>
#include "common.hh"

namespace numbers {

// queue of std::unique_ptrs.

template <typename T, size_t Size>
class Queue {
public:
	typedef T ElementType;
	
	Queue() : _head(0), _count(0) {}

	size_t
	count() const {
		return _count;
	}

	bool
	is_empty() const {
		return _count == 0;
	}

	size_t
	capacity() const {
		return Size;
	}

	void
	clear() {
		while (!is_empty()) dequeue();
	}

	void
	enqueue(ElementType &item) {
		ASSERT(_count < Size);
		_items[_head] = item;
		_head = _wrap(_head + 1);
		++_count;
	}

	ElementType
	dequeue() {
		ASSERT(_count != 0);
		size_t tail = _wrap(_head - _count);
		--_count;
		return _items[tail];
	}

	ElementType const &
	operator[](size_t idx) {
		return _items[_wrap(_head - _count + idx)];
	}

private:

	size_t _head, _count;
	
	ElementType _items[Size];

	inline int
	_wrap(size_t idx) const {
		return (idx + Size) % Size;
	}

	DISALLOW_COPY_AND_SWAP(Queue);

};
}












#endif