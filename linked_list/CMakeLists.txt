add_library(list STATIC private/list.c)
target_include_directories(list PUBLIC public)
set(DEBUG_BUILDS Debug RelWithDebInfo)
if(MSVC)
    target_compile_options(list PUBLIC /openmp:llvm)
    if(CMAKE_BUILD_TYPE IN_LIST DEBUG_BUILDS)
        target_compile_options(list PUBLIC 
            /fsanitize=address 
            /fsanitize=fuzzer
        )
    endif()
endif()
target_link_libraries(list PRIVATE utilities)

find_package(OpenMP REQUIRED)
target_link_libraries(list PUBLIC OpenMP::OpenMP_C)

add_executable(copy_create_destroy_list 
    tests/copy_create_destroy_list.c
)
target_link_libraries(copy_create_destroy_list PRIVATE list)
add_test(
    NAME copy_create_destroy_list
    COMMAND copy_create_destroy_list.exe
)
add_executable(create_destroy_list_node 
    tests/create_destroy_list_node.c
)
target_link_libraries(create_destroy_list_node PRIVATE list)
add_test(
    NAME create_destroy_list_node
    COMMAND create_destroy_list_node.exe
)
add_executable(create_destroy_list_with_hint 
    tests/create_destroy_list_with_hint.c
)
target_link_libraries(create_destroy_list_with_hint PRIVATE list)
add_test(
    NAME create_destroy_list_with_hint
    COMMAND create_destroy_list_with_hint.exe
)
add_executable(create_destroy_list 
    tests/create_destroy_list.c
)
target_link_libraries(create_destroy_list PRIVATE list)
add_test(
    NAME create_destroy_list
    COMMAND create_destroy_list.exe
)
add_executable(destroy_list_node
    tests/destroy_list_node.c
)
target_link_libraries(destroy_list_node PRIVATE list)
add_test(
    NAME destroy_list_node
    COMMAND destroy_list_node.exe
)
add_executable(erase_following_list_nodes
    tests/erase_following_list_nodes.c
)
target_link_libraries(erase_following_list_nodes PRIVATE list)
add_test(
    NAME erase_following_list_nodes
    COMMAND erase_following_list_nodes.exe
)
add_executable(push_next_node
    tests/push_next_node.c
)
target_link_libraries(push_next_node PRIVATE list)
add_test(
    NAME push_next_node
    COMMAND push_next_node.exe
)
add_executable(reverse_list
    tests/reverse_list.c
)
target_link_libraries(reverse_list PRIVATE list)
add_test(
    NAME reverse_list
    COMMAND reverse_list.exe
)
add_executable(swap_list_nodes
    tests/swap_list_nodes.c
)
target_link_libraries(swap_list_nodes PRIVATE list)
add_test(
    NAME swap_list_nodes
    COMMAND swap_list_nodes.exe
)
add_executable(pop_next_node 
    tests/pop_next_node.c
)
target_link_libraries(pop_next_node PRIVATE list)
add_test(
    NAME pop_next_node
    COMMAND pop_next_node.exe
)