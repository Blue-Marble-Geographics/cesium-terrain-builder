//
// Created on 2023/12/02
//

#ifndef CTB_TESTS_H
#define CTB_TESTS_H

#include <gtest/gtest.h>

#if _DEBUG
#   define CONSOLE(x) do { std::cout << __func__ << ":" << x << '\n';  } while(0)
#else
#   define CONSOLE(x)
#endif

#define CONSOLE_EVAL(x) CONSOLE(#x << " : " << (x))
#define CONSOLE_FEVAL(x) CONSOLE(#x << " : " << std::setprecision(20) << (x))

#endif // CTB_TESTS_H
