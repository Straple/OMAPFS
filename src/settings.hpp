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
#define ENABLE_ASSERT// Default: enabled
#endif

#ifndef ENABLE_SCHEDULER_CHANGE_TASK
#define ENABLE_SCHEDULER_CHANGE_TASK// Default: enabled
#endif

#ifndef THREADS_NUM_DEFAULT
#define THREADS_NUM_DEFAULT 8// Default fallback
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

EPIBT(2), CONST, RANDOM, 1000 STEPS
100: 3885 25ms
200: 6423 26ms
300: 7232 27ms
400: 7113 29ms
500: 6545 35ms
600: 6204 39ms
700: 6029 43ms
800: 5328 72ms

 */

// TODO: выключить наследование операций EPIBT
// оно показывает хуже результаты

/*
Алгоритм:
1. k раз запускаем пункт (2). Чем больше k, тем лучше решение, но небольшого количества ~3 вполне достаточно
2. Перебираем available_operation_depth = 1, ..., EPIBT_DEPTH и для каждого значения выполняем пункт (3). Эта переменная ограничивает операции, которые может использовать EPIBT. Например, available_operation_depth = 1, это операции N, S, W, E, w.
3. Сортируем агентов по возрастанию их метрики с текущими выбранными операциями. То есть сначала мы рассмотрим агентов, которые уже с такой операцией очень близки к цели.
4. Перебираем агентов в этом порядке, если мы еще не изменяли этого агента в пункте, то удалим ему путь и запустим рекурсивное построение (EPIBT).
5. Внутри EPIBT, когда мы успешно построили путь, то если прошлая метрика лучше текущей, то откатим изменения, иначе примем их.
6. Внутри рекурсии EPIBT также ограничение на количество рекурсивных вызовов. Я поставил 1000.

Если использовать EPIBT(3), то там результаты чуть лучше, чем у EPIBT(2), но время кратно больше
*/
