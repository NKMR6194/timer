#include <chrono>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;

void print_time(const system_clock::time_point &alarm,
                const system_clock::time_point &now) {
  static int progress = 0;
  static const string bar = "|/-\\";
  auto left = (alarm - now) + minutes(1);
  auto min = duration_cast<minutes>(left) % 60;
  auto hour = duration_cast<hours>(left);
  printf("\r%02ld:%02ld left %c", hour.count(), min.count(), bar[progress]);
  fflush(stdout);
  progress = (progress + 1) % 4;
}

void print_date(const system_clock::time_point &date) {
  time_t t = system_clock::to_time_t(date);
  const tm *lt = localtime(&t);
  cout << put_time(lt, "%c") << endl;
}

tm *tm_from_clock(const system_clock::time_point &date) {
  time_t t = system_clock::to_time_t(date);
  return localtime(&t);
}

system_clock::time_point mk_alarm(const system_clock::time_point &begin,
                                  int hour, int min) {
  auto alarm_at = tm_from_clock(begin);

  if (alarm_at->tm_hour > hour) {
    alarm_at->tm_mday++;
  }
  alarm_at->tm_hour = hour;
  alarm_at->tm_min = min;
  alarm_at->tm_sec = 0;

  const auto a = mktime(alarm_at);
  const auto b = system_clock::from_time_t(a);
  return b;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "引数が足りません" << endl;
    return -1;
  }

  // input: "01:23";
  int hour = atoi(argv[1]);
  int min = atoi(argv[1] + 3);

  if (hour < 0 || min < 0) {
    cerr << "引数が不正です" << endl;
    return -1;
  }

  system_clock::time_point begin = system_clock::now();
  print_date(begin);

  system_clock::time_point alarm = mk_alarm(begin, hour, min);
  cout << "Set alarm for ";
  print_date(alarm);

  auto now = system_clock::now();
  while (alarm > now) {
    print_time(alarm, now);
    this_thread::sleep_for(seconds(1));
    now = system_clock::now();
  }

  cout << endl;
  cout << "Stop: Ctrl+C Ctrl+C" << endl;
  while (true) {
    system("afplay "
           "/System/Library/Components/CoreAudio.component/Contents/"
           "SharedSupport/SystemSounds/system/burn\\ complete.aif");
    this_thread::sleep_for(milliseconds(500));
  }
  return 0;
}
