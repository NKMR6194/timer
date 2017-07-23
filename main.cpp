#include <iostream>
#include <chrono>
#include <thread>
#include <boost/progress.hpp>

using namespace std;
using namespace chrono;

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
	const unsigned long expected_count = min * 600;
	boost::progress_display show_progress(expected_count);

	auto begin = steady_clock::now();
	auto end = begin;
	auto chrono_min = minutes(min);
	while (duration_cast<milliseconds>(end - begin) < chrono_min) {
		this_thread::sleep_for(milliseconds(100));
		end = steady_clock::now();
		++show_progress;
	}

	show_progress += expected_count - show_progress.count();

	cout << endl;
	cout << "Stop: Ctrl+C Ctrl+C" << endl;
	while (true) {
		system("afplay /System/Library/Components/CoreAudio.component/Contents/SharedSupport/SystemSounds/system/burn\\ complete.aif");
		this_thread::sleep_for(milliseconds(500));
	}
	return 0;
}
