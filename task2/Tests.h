#pragma once

#include "Objects.h"

void simple_test() {
    std::cout << "start simple test" << std::endl;
    TRY (
            Simple_Object obj1;
            CommitObject(obj1);

            Simple_Object obj2;
            CommitObject(obj2);

            THROW(OUT_OF_MEMORY);
    ) CATCH(BAD_FILE,
            std::cout << "Caught exception: BAD_FILE" << std::endl;
    ) CATCH(OUT_OF_MEMORY,
            std::cout << "Caught exception: OUT_OF_MEMORY" << std::endl;
    )
    std::cout << "finish simple test" << std::endl << std::endl;
}

void functions_test() {
    std::cout << "start functions test" << std::endl;
    TRY (
            Simple_Object obj1;
            CommitObject(obj1);
            external_function();
    ) CATCH(BAD_FILE,
            std::cout << "Caught exception: BAD_FILE" << std::endl;
    ) CATCH(OUT_OF_MEMORY,
            std::cout << "Caught exception: OUT_OF_MEMORY" << std::endl;
    )
    std::cout << "finish functions test" << std::endl << std::endl;
}

void double_throw_test() {
    std::cout << "start double throw test" << std::endl;
    TRY (
            Simple_Object obj1;
            CommitObject(obj1);

            Throw_Object obj2;
            CommitObject(obj2);

            THROW(OUT_OF_MEMORY);
    ) CATCH(BAD_FILE,
            std::cout << "Caught exception: BAD_FILE" << std::endl;
    ) CATCH(OUT_OF_MEMORY,
            std::cout << "Caught exception: OUT_OF_MEMORY" << std::endl;
    )
    std::cout << "finish double throw test" << std::endl << std::endl;
}