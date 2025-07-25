#pragma once
#ifdef ENABLE_ROTATE_MODEL

#include <chrono>
#include <ctime>
#include <iostream>
#include <planner/wppl/util/Dev.h>
#include <string>
#include <unordered_map>

using std::cout;
using std::endl;
using std::string;
using std::chrono::duration;
using std::chrono::steady_clock;

// deadTimer uses second as the default unit.
// p means time point
// d means time duration
// TODO(rivers): it may be modified to be thread-safe.
class deadTimer {
public:
    // record
    steady_clock::time_point record_p(const string &pkey);
    double record_d(const string &old_pkey, const string &new_pkey);
    double record_d(const string &old_pkey, const string &new_pkey, const string &dkey);

    // get
    steady_clock::time_point get_p(const string &pkey) const;
    // mode: 0-sum, 1-mean
    double get_d(const string &dkey, int mode = 0) const;
    // mode: 0-sum, 1-mean
    std::unordered_map<string, double> get_all_d(int mode = 0) const;

    // print
    void print_d(const string &dkey) const;
    void print_all_d() const;

    // remove
    void remove_p(const string &pkey);
    void remove_d(const string &dkey);

    // clear
    void clear_d();
    void clear_p();
    void clear();

private:
    std::unordered_map<string, steady_clock::time_point> time_points;
    std::unordered_map<string, double> time_durations;
    std::unordered_map<string, size_t> time_duration_counters;
    std::unordered_map<string, double> time_durations_last;
};

extern deadTimer g_timer;
#endif
