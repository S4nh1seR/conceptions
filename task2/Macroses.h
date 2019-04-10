#pragma once

#include <csetjmp>
#include <functional>
#include <iostream>
#include <stack>

const static int BAD_FILE = 1;
const static int OUT_OF_MEMORY = 2;

jmp_buf env;

bool already_thrown = false;

int num_objects = 0;
std::stack<std::function<void()>> destructors;

void stack_purification() {
    for (; num_objects > 0; --num_objects) {
        auto curr_destructor = destructors.top();
        curr_destructor();
        destructors.pop();
    }
}

#define TRY(block) \
        int val = setjmp(env); \
        if (val == 0) { \
            block \
            stack_purification(); \
        }

#define CATCH(exception, block) \
        else if (val == exception) { \
                already_thrown = false; \
                block \
             }

#define THROW(exception)  \
        if (already_thrown) { \
            std::cout << "Error while stack purifying!" << std::endl; \
            std::abort(); \
        } \
        already_thrown = true; \
        stack_purification(); \
        longjmp(env, exception);

