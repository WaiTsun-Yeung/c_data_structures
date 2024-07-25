add_library(array STATIC private/array.c)
target_include_directories(array PUBLIC public)

add_subdirectory(tests)

add_executable(copy_byte_array tests/copy_byte_array.c)
target_link_libraries(copy_byte_array PRIVATE array array_tests)
add_test(NAME copy_byte_array COMMAND copy_byte_array.exe)
add_executable(copy_create_destroy_byte_array 
    tests/copy_create_destroy_byte_array.c
)
target_link_libraries(copy_create_destroy_byte_array PRIVATE array array_tests)
add_test(
    NAME copy_create_destroy_byte_array 
    COMMAND copy_create_destroy_byte_array.exe
)
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
add_executable(resize_byte_array_s tests/resize_byte_array_s.c)
target_link_libraries(resize_byte_array_s PRIVATE array array_tests)
add_test(NAME resize_byte_array_s COMMAND resize_byte_array_s.exe)
add_executable(resize_byte_array tests/resize_byte_array.c)
target_link_libraries(resize_byte_array PRIVATE array array_tests)
add_test(NAME resize_byte_array COMMAND resize_byte_array.exe)
add_executable(resize_int_array_s tests/resize_int_array_s.c)
target_link_libraries(resize_int_array_s PRIVATE array array_tests)
add_test(NAME resize_int_array_s COMMAND resize_int_array_s.exe)
add_executable(resize_int_array tests/resize_int_array.c)
target_link_libraries(resize_int_array PRIVATE array array_tests)
add_test(NAME resize_int_array COMMAND resize_int_array.exe)