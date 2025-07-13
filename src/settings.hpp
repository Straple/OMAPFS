#pragma once

#include <cstdint>

// Compile-time settings now controlled by CMake options:
// -DENABLE_ROTATE_MODEL=ON/OFF
// -DENABLE_ASSERT=ON/OFF  
// -DENABLE_SCHEDULER_CHANGE_TASK=ON/OFF
// -DEPIBT_DEPTH=<value>
// -DTHREADS_NUM_DEFAULT=<value>

// These are now defined by CMake, but we provide fallback defaults:

#ifndef ENABLE_ROTATE_MODEL
//#define ENABLE_ROTATE_MODEL  // Controlled by CMake option
#endif

#ifndef ENABLE_ASSERT
#define ENABLE_ASSERT  // Default: enabled
#endif

#ifndef ENABLE_SCHEDULER_CHANGE_TASK
#define ENABLE_SCHEDULER_CHANGE_TASK  // Default: enabled
#endif

#ifndef THREADS_NUM_DEFAULT
#define THREADS_NUM_DEFAULT 8  // Default fallback
#endif

// Use the CMake-defined value, or fallback
constexpr uint32_t THREADS_NUM = THREADS_NUM_DEFAULT;

/* TODO
 * 1) добавить GraphGuidanceBuilder, который строит GG для разных карт
 * 2) улучшить проверки в Answer
 */

/*
EPIBT, CONST, RANDOM, 1000 STEPS
400: 6431 51ms
700: 5063 174ms
800: 4401 366ms
 */
