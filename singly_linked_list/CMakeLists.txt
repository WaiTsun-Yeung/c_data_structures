add_library(singly_linked_list STATIC 
    private/singly_linked_list.c
    private/linked_list.c
)
target_include_directories(singly_linked_list PUBLIC public)
set(DEBUG_BUILDS Debug RelWithDebInfo)
if(MSVC)
    if(CMAKE_BUILD_TYPE IN_LIST DEBUG_BUILDS)
        target_compile_options(singly_linked_list PUBLIC 
            /fsanitize=address 
            /fsanitize=fuzzer
        )
    endif()
endif()
target_link_libraries(singly_linked_list PUBLIC utilities)

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
add_executable(create_destroy_singly_linked_list_with_mutex_type 
    tests/create_destroy_singly_linked_list_with_mutex_type.c
)
target_link_libraries(create_destroy_singly_linked_list_with_mutex_type PRIVATE 
    singly_linked_list
)
add_test(
    NAME create_destroy_singly_linked_list_with_mutex_type
    COMMAND create_destroy_singly_linked_list_with_mutex_type.exe
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
add_executable(swap_singly_linked_list_nodes_pp
    tests/swap_singly_linked_list_nodes_pp.c
)
target_link_libraries(swap_singly_linked_list_nodes_pp PRIVATE 
    singly_linked_list
)
add_test(
    NAME swap_singly_linked_list_nodes_pp
    COMMAND swap_singly_linked_list_nodes_pp.exe
)
add_executable(swap_singly_linked_list_nodes_pf
    tests/swap_singly_linked_list_nodes_pf.c
)
target_link_libraries(swap_singly_linked_list_nodes_pf PRIVATE 
    singly_linked_list
)
add_test(
    NAME swap_singly_linked_list_nodes_pf
    COMMAND swap_singly_linked_list_nodes_pf.exe
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
add_executable(singly_linked_list_remove_if
    tests/singly_linked_list_remove_if.c
)
target_link_libraries(singly_linked_list_remove_if PRIVATE singly_linked_list)
add_test(
    NAME singly_linked_list_remove_if
    COMMAND singly_linked_list_remove_if.exe
)