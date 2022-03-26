#include <iostream>

#include "containers/Queue.h"


class A {
	friend std::ostream& operator<<(std::ostream& out, const A& value);
public:
	A() {
		static int b = 0;
		id_ = b++;
		std::cout << "Default constructor A " << id_ << "\n";
	}
	A(const A& o) :id_(o.id_) {
		std::cout << "Copy constructor A " << id_ << "\n";
	}
	A& operator=(const A& o) {
		id_ = o.id_;
		std::cout << "operator= A " << id_ << "\n";
		return *this;
	}
	~A() {
		std::cout << "Destructor A " << id_ << "\n";
	}

private:
	int id_;
};

std::ostream& operator<<(std::ostream& out, const A& value) {
	out << value.id_;
	return out;
}

int main() {

	Queue<A> a;
	std::cout << "Push:\n";
	a.emplace();
	a.push(A());
	a.push(A());
	a.push(A());
	a.push(A());
	A& d = a.back();
	A& f = a.front();
	std::cout << "Swap:\n";
	Queue<A> n;
	a.swap(n);
	std::cout << n.front() << " " << n.back() << "\n";
	std::cout << f << " " << d << "\n";
	a.swap(n);
	std::cout << a.front() << " " << a.back() << "\n";
	std::cout << "Pop:\n";
	a.pop();
	std::cout << a.front() << " " << a.back() << "\n";
	a.push(A());
	std::cout << f << " " << d << "\n";
	a.front();
	std::cout << a.front() << "\n";
	std::cout << "Assign:\n";
	Queue<A> b;
	b = a;
	std::cout << a.front() << " " << b.front() << "\n";

	std::cout << "End:\n";

	return 0;
}