#include <iostream>
#include <chrono>
#include <thread>
#include <cstdio>

using namespace std;
using namespace chrono;

void print_time(const steady_clock::duration& timer, const steady_clock::time_point& begin, const steady_clock::time_point& now) {
	auto left = timer - (now - begin);
	auto sec = duration_cast<seconds>(left) % 60;
	auto min = duration_cast<minutes>(left);
	printf("\r%02ld:%02lld left", min.count(), sec.count());
	fflush(stdout);
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cerr << "引数が足りません" << endl;
		return -1;
	}

	int min = atoi(argv[1]);

	if (min <= 0) {
		cerr << "引数が不正です" << endl;
		return -1;
	}

	cout << "set timer " << min << " min" << endl;
	cout << endl;

	auto begin = steady_clock::now();
	auto end = begin;
	auto chrono_min = minutes(min);
	while (duration_cast<milliseconds>(end - begin) < chrono_min) {
		print_time(chrono_min, begin, end);
		this_thread::sleep_for(milliseconds(200));
		end = steady_clock::now();
	}

	cout << endl;
	cout << "Stop: Ctrl+C Ctrl+C" << endl;
	while (true) {
		system("afplay /System/Library/Components/CoreAudio.component/Contents/SharedSupport/SystemSounds/system/burn\\ complete.aif");
		this_thread::sleep_for(milliseconds(500));
	}
	return 0;
}
