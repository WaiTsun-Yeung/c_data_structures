add_library(singly_linked_list STATIC 
    private/singly_linked_list.c
    private/linked_list.c
)
target_include_directories(singly_linked_list PUBLIC public)
set(DEBUG_BUILDS Debug RelWithDebInfo)
if(MSVC)
    target_compile_options(singly_linked_list PUBLIC /openmp:llvm)
    if(CMAKE_BUILD_TYPE IN_LIST DEBUG_BUILDS)
        target_compile_options(singly_linked_list PUBLIC 
            /fsanitize=address 
            /fsanitize=fuzzer
        )
    endif()
endif()
target_link_libraries(singly_linked_list PUBLIC utilities)

find_package(OpenMP REQUIRED)
target_link_libraries(singly_linked_list PUBLIC OpenMP::OpenMP_C)

add_executable(copy_create_destroy_singly_linked_list 
    tests/copy_create_destroy_singly_linked_list.c
)
target_link_libraries(copy_create_destroy_singly_linked_list PRIVATE 
    singly_linked_list
)
add_test(
    NAME copy_create_destroy_singly_linked_list
    COMMAND copy_create_destroy_singly_linked_list.exe
)
add_executable(create_destroy_singly_linked_list_node 
    tests/create_destroy_singly_linked_list_node.c
)
target_link_libraries(create_destroy_singly_linked_list_node PRIVATE 
    singly_linked_list
)
add_test(
    NAME create_destroy_singly_linked_list_node
    COMMAND create_destroy_singly_linked_list_node.exe
)
add_executable(copy_create_destroy_singly_linked_list_node 
    tests/copy_create_destroy_singly_linked_list_node.c
)
target_link_libraries(copy_create_destroy_singly_linked_list_node PRIVATE 
    singly_linked_list
)
add_test(
    NAME copy_create_destroy_singly_linked_list_node
    COMMAND copy_create_destroy_singly_linked_list_node.exe
)
add_executable(create_destroy_singly_linked_list_with_hint 
    tests/create_destroy_singly_linked_list_with_hint.c
)
target_link_libraries(create_destroy_singly_linked_list_with_hint PRIVATE 
    singly_linked_list
)
add_test(
    NAME create_destroy_singly_linked_list_with_hint
    COMMAND create_destroy_singly_linked_list_with_hint.exe
)
add_executable(create_destroy_singly_linked_list 
    tests/create_destroy_singly_linked_list.c
)
target_link_libraries(create_destroy_singly_linked_list PRIVATE 
    singly_linked_list
)
add_test(
    NAME create_destroy_singly_linked_list
    COMMAND create_destroy_singly_linked_list.exe
)
add_executable(change_singly_linked_list_node_data_type 
    tests/change_singly_linked_list_node_data_type.c
)
target_link_libraries(change_singly_linked_list_node_data_type PRIVATE 
    singly_linked_list
)
add_test(
    NAME change_singly_linked_list_node_data_type
    COMMAND change_singly_linked_list_node_data_type.exe
)
add_executable(copy_singly_linked_list_node 
    tests/copy_singly_linked_list_node.c
)
target_link_libraries(copy_singly_linked_list_node PRIVATE 
    singly_linked_list
)
add_test(
    NAME copy_singly_linked_list_node
    COMMAND copy_singly_linked_list_node.exe
)
add_executable(destroy_singly_linked_list_node
    tests/destroy_singly_linked_list_node.c
)
target_link_libraries(destroy_singly_linked_list_node PRIVATE 
    singly_linked_list
)
add_test(
    NAME destroy_singly_linked_list_node
    COMMAND destroy_singly_linked_list_node.exe
)
add_executable(erase_following_singly_linked_list_nodes
    tests/erase_following_singly_linked_list_nodes.c
)
target_link_libraries(erase_following_singly_linked_list_nodes PRIVATE 
    singly_linked_list
)
add_test(
    NAME erase_following_singly_linked_list_nodes
    COMMAND erase_following_singly_linked_list_nodes.exe
)
add_executable(erase_preceding_singly_linked_list_nodes
    tests/erase_preceding_singly_linked_list_nodes.c
)
target_link_libraries(erase_preceding_singly_linked_list_nodes PRIVATE 
    singly_linked_list
)
add_test(
    NAME erase_preceding_singly_linked_list_nodes
    COMMAND erase_preceding_singly_linked_list_nodes.exe
)
add_executable(push_next_singly_linked_list_node
    tests/push_next_singly_linked_list_node.c
)
target_link_libraries(push_next_singly_linked_list_node PRIVATE 
    singly_linked_list
)
add_test(
    NAME push_next_singly_linked_list_node
    COMMAND push_next_singly_linked_list_node.exe
)
add_executable(reverse_singly_linked_list
    tests/reverse_singly_linked_list.c
)
target_link_libraries(reverse_singly_linked_list PRIVATE singly_linked_list)
add_test(
    NAME reverse_singly_linked_list
    COMMAND reverse_singly_linked_list.exe
)
add_executable(swap_singly_linked_list_nodes
    tests/swap_singly_linked_list_nodes.c
)
target_link_libraries(swap_singly_linked_list_nodes PRIVATE singly_linked_list)
add_test(
    NAME swap_singly_linked_list_nodes
    COMMAND swap_singly_linked_list_nodes.exe
)
add_executable(pop_next_singly_linked_list_node 
    tests/pop_next_singly_linked_list_node.c
)
target_link_libraries(pop_next_singly_linked_list_node PRIVATE 
    singly_linked_list
)
add_test(
    NAME pop_next_singly_linked_list_node
    COMMAND pop_next_singly_linked_list_node.exe
)
add_executable(empty_singly_linked_list
    tests/empty_singly_linked_list.c
)
target_link_libraries(empty_singly_linked_list PRIVATE singly_linked_list)
add_test(
    NAME empty_singly_linked_list
    COMMAND empty_singly_linked_list.exe
)
add_executable(invert_singly_linked_list
    tests/invert_singly_linked_list.c
)
target_link_libraries(invert_singly_linked_list PRIVATE singly_linked_list)
add_test(
    NAME invert_singly_linked_list
    COMMAND invert_singly_linked_list.exe
)