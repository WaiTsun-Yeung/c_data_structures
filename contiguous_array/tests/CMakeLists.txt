add_library(array_tests STATIC
    private/rand.c
)
target_include_directories(array_tests INTERFACE public)