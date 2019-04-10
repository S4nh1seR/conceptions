#pragma once

#include "Macroses.h"

template <class T>
void CommitObject(T& obj) {
    num_objects++;
    destructors.push([&obj]() { obj.~T(); });
}

class Simple_Object {
public:
    Simple_Object() {
        std::cout << "Object constructor was called!" << std::endl;
    }
    ~Simple_Object() {
        std::cout << "Object destructor was called!" << std::endl;
    }
};

class Throw_Object {
public:
    Throw_Object() {
        std::cout << "Object constructor was called!" << std::endl;
    }
    ~Throw_Object() {
        std::cout << "ObjectWithThrow destructor was called!" << std::endl;
        THROW(BAD_FILE);
    }
};


void internal_function() {
    Simple_Object obj1;
    CommitObject(obj1);
    THROW(BAD_FILE);
}

void external_function() {
    Simple_Object obj1;
    CommitObject(obj1);
    internal_function();
}