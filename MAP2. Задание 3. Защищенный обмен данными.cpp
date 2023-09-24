#include <thread>
#include <iostream>
#include <mutex>


class Data
{
public:
	int data_ = 0;
	std::mutex m;
	Data() {};
	Data(int data) { data_ = data;}

	void print()
	{
		std::cout << data_ << " ";
	}
};

void swapLock(Data& d1, Data& d2)
{
	if (&d1 == &d2)
		return;
	std::lock(d1.m, d2.m);
	std::lock_guard<std::mutex> lockD1(d1.m, std::adopt_lock);
	std::lock_guard<std::mutex> lockD2(d2.m, std::adopt_lock);
	std::swap(d1.data_, d2.data_);
	d1.print();
	d2.print();
	std::cout << std::endl;
}

void swapScopedLock(Data& d1, Data& d2)
{
	if (&d1 == &d2)
		return;
	std::scoped_lock lock (d1.m, d2.m);
	std::swap(d1.data_, d2.data_);
	d1.print();
	d2.print();
	std::cout << std::endl;
}

void swapUniqueLock(Data& d1, Data& d2)
{
	if (&d1 == &d2)
		return;
	std::unique_lock<std::mutex> lockD1(d1.m, std::defer_lock);
	std::unique_lock<std::mutex> lockD2(d2.m, std::defer_lock);
	std::lock(lockD1, lockD2);
	std::swap(d1.data_, d2.data_);
	d1.print();
	d2.print();
	std::cout << std::endl;
}


int main()
{
	setlocale(LC_ALL, "Rus");

	Data d1(2), d2(4);

	auto print = [&d1, &d2]()
		{
			d1.print();
			d2.print();
			std::cout << std::endl;
		};
	print();

	std::thread t1(swapLock, std::ref(d1), std::ref(d2));
	std::thread t2(swapScopedLock, std::ref(d1), std::ref(d2));
	std::thread t3(swapUniqueLock, std::ref(d1), std::ref(d2));

	t1.join();
	t2.join();
	t3.join();

}




