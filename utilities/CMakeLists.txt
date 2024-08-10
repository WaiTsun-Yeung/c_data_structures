add_library(utilities STATIC
    private/utilities.c
)
target_include_directories(utilities INTERFACE public)
set(DEBUG_BUILDS Debug RelWithDebInfo)
if(MSVC AND CMAKE_BUILD_TYPE IN_LIST DEBUG_BUILDS)
    target_compile_options(utilities PUBLIC 
        /fsanitize=address 
        /fsanitize=fuzzer
        /fsanitize-address-use-after-return
    )
endif()