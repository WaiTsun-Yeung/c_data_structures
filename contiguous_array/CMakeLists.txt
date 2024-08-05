add_library(array STATIC private/array.c)
target_include_directories(array PUBLIC public)
set(DEBUG_BUILDS Debug RelWithDebInfo)
if(MSVC AND CMAKE_BUILD_TYPE IN_LIST DEBUG_BUILDS)
    target_compile_options(array PUBLIC 
        /fsanitize=address 
        /fsanitize=fuzzer
        /fsanitize-address-use-after-return
    )
endif()

add_subdirectory(tests)

add_executable(copy_create_destroy_int_array 
    tests/copy_create_destroy_int_array.c
)
target_link_libraries(copy_create_destroy_int_array PRIVATE array array_tests)
add_test(
    NAME copy_create_destroy_int_array
    COMMAND copy_create_destroy_int_array.exe
)
add_executable(copy_int_array tests/copy_int_array.c)
target_link_libraries(copy_int_array PRIVATE array array_tests)
add_test(NAME copy_int_array COMMAND copy_int_array.exe)
add_executable(create_destroy_int_array tests/create_destroy_int_array.c)
target_link_libraries(create_destroy_int_array PRIVATE array array_tests)
add_test(NAME create_destroy_int_array COMMAND create_destroy_int_array.exe)
add_executable(create_destroy_byte_array tests/create_destroy_byte_array.c)
target_link_libraries(create_destroy_byte_array PRIVATE array array_tests)
add_test(NAME create_destroy_byte_array COMMAND create_destroy_byte_array.exe)
add_executable(resize_int_array tests/resize_int_array.c)
target_link_libraries(resize_int_array PRIVATE array array_tests)
add_test(NAME resize_int_array COMMAND resize_int_array.exe)