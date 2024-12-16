add_library(linked_list STATIC 
    private/linked_list.c
    private/singly_linked_list.c
    private/doubly_linked_list.c
)
target_include_directories(linked_list PUBLIC public)
set(DEBUG_BUILDS Debug RelWithDebInfo)
if(MSVC)
    if(CMAKE_BUILD_TYPE IN_LIST DEBUG_BUILDS)
        target_compile_options(linked_list PUBLIC 
            /fsanitize=address 
            /fsanitize=fuzzer
        )
    endif()
endif()
target_link_libraries(linked_list PUBLIC utilities)

add_executable(copy_create_destroy_singly_linked_list 
    tests/copy_create_destroy_singly_linked_list.c
)
target_link_libraries(copy_create_destroy_singly_linked_list PRIVATE 
    linked_list
)
add_test(
    NAME copy_create_destroy_singly_linked_list
    COMMAND copy_create_destroy_singly_linked_list.exe
)
add_executable(create_destroy_singly_linked_list_node 
    tests/create_destroy_singly_linked_list_node.c
)
target_link_libraries(create_destroy_singly_linked_list_node PRIVATE 
    linked_list
)
add_test(
    NAME create_destroy_singly_linked_list_node
    COMMAND create_destroy_singly_linked_list_node.exe
)
add_executable(copy_create_destroy_singly_linked_list_node 
    tests/copy_create_destroy_singly_linked_list_node.c
)
target_link_libraries(copy_create_destroy_singly_linked_list_node PRIVATE 
    linked_list
)
add_test(
    NAME copy_create_destroy_singly_linked_list_node
    COMMAND copy_create_destroy_singly_linked_list_node.exe
)
add_executable(create_destroy_singly_linked_list_with_mutex_type 
    tests/create_destroy_singly_linked_list_with_mutex_type.c
)
target_link_libraries(create_destroy_singly_linked_list_with_mutex_type PRIVATE 
    linked_list
)
add_test(
    NAME create_destroy_singly_linked_list_with_mutex_type
    COMMAND create_destroy_singly_linked_list_with_mutex_type.exe
)
add_executable(create_destroy_singly_linked_list 
    tests/create_destroy_singly_linked_list.c
)
target_link_libraries(create_destroy_singly_linked_list PRIVATE 
    linked_list
)
add_test(
    NAME create_destroy_singly_linked_list
    COMMAND create_destroy_singly_linked_list.exe
)
add_executable(change_singly_linked_list_node_data_type 
    tests/change_singly_linked_list_node_data_type.c
)
target_link_libraries(change_singly_linked_list_node_data_type PRIVATE 
    linked_list
)
add_test(
    NAME change_singly_linked_list_node_data_type
    COMMAND change_singly_linked_list_node_data_type.exe
)
add_executable(copy_singly_linked_list_node 
    tests/copy_singly_linked_list_node.c
)
target_link_libraries(copy_singly_linked_list_node PRIVATE 
    linked_list
)
add_test(
    NAME copy_singly_linked_list_node
    COMMAND copy_singly_linked_list_node.exe
)
add_executable(destroy_singly_linked_list_node
    tests/destroy_singly_linked_list_node.c
)
target_link_libraries(destroy_singly_linked_list_node PRIVATE 
    linked_list
)
add_test(
    NAME destroy_singly_linked_list_node
    COMMAND destroy_singly_linked_list_node.exe
)
add_executable(erase_following_singly_linked_list_nodes
    tests/erase_following_singly_linked_list_nodes.c
)
target_link_libraries(erase_following_singly_linked_list_nodes PRIVATE 
    linked_list
)
add_test(
    NAME erase_following_singly_linked_list_nodes
    COMMAND erase_following_singly_linked_list_nodes.exe
)
add_executable(erase_preceding_singly_linked_list_nodes
    tests/erase_preceding_singly_linked_list_nodes.c
)
target_link_libraries(erase_preceding_singly_linked_list_nodes PRIVATE 
    linked_list
)
add_test(
    NAME erase_preceding_singly_linked_list_nodes
    COMMAND erase_preceding_singly_linked_list_nodes.exe
)
add_executable(push_next_singly_linked_list_node
    tests/push_next_singly_linked_list_node.c
)
target_link_libraries(push_next_singly_linked_list_node PRIVATE 
    linked_list
)
add_test(
    NAME push_next_singly_linked_list_node
    COMMAND push_next_singly_linked_list_node.exe
)
add_executable(reverse_singly_linked_list
    tests/reverse_singly_linked_list.c
)
target_link_libraries(reverse_singly_linked_list PRIVATE linked_list)
add_test(
    NAME reverse_singly_linked_list
    COMMAND reverse_singly_linked_list.exe
)
add_executable(swap_next_singly_linked_list_nodes
    tests/swap_next_singly_linked_list_nodes.c
)
target_link_libraries(swap_next_singly_linked_list_nodes PRIVATE 
    linked_list
)
add_test(
    NAME swap_next_singly_linked_list_nodes
    COMMAND swap_next_singly_linked_list_nodes.exe
)
add_executable(swap_free_and_next_singly_linked_list_nodes
    tests/swap_free_and_next_singly_linked_list_nodes.c
)
target_link_libraries(swap_free_and_next_singly_linked_list_nodes PRIVATE 
    linked_list
)
add_test(
    NAME swap_free_and_next_singly_linked_list_nodes
    COMMAND swap_free_and_next_singly_linked_list_nodes.exe
)
add_executable(pop_next_singly_linked_list_node 
    tests/pop_next_singly_linked_list_node.c
)
target_link_libraries(pop_next_singly_linked_list_node PRIVATE 
    linked_list
)
add_test(
    NAME pop_next_singly_linked_list_node
    COMMAND pop_next_singly_linked_list_node.exe
)
add_executable(empty_singly_linked_list
    tests/empty_singly_linked_list.c
)
target_link_libraries(empty_singly_linked_list PRIVATE linked_list)
add_test(
    NAME empty_singly_linked_list
    COMMAND empty_singly_linked_list.exe
)
add_executable(invert_singly_linked_list
    tests/invert_singly_linked_list.c
)
target_link_libraries(invert_singly_linked_list PRIVATE linked_list)
add_test(
    NAME invert_singly_linked_list
    COMMAND invert_singly_linked_list.exe
)
add_executable(swap_singly_linked_list
    tests/swap_singly_linked_list.c
)
target_link_libraries(swap_singly_linked_list PRIVATE linked_list)
add_test(
    NAME swap_singly_linked_list
    COMMAND swap_singly_linked_list.exe
)
add_executable(splice_singly_linked_list
    tests/splice_singly_linked_list.c
)
target_link_libraries(splice_singly_linked_list PRIVATE linked_list)
add_test(
    NAME splice_singly_linked_list
    COMMAND splice_singly_linked_list.exe
)
add_executable(prepend_singly_linked_list_with_list
    tests/prepend_singly_linked_list_with_list.c
)
target_link_libraries(prepend_singly_linked_list_with_list PRIVATE linked_list)
add_test(
    NAME prepend_singly_linked_list_with_list
    COMMAND prepend_singly_linked_list_with_list.exe
)
add_executable(splice_singly_linked_list_range
    tests/splice_singly_linked_list_range.c
)
target_link_libraries(splice_singly_linked_list_range PRIVATE linked_list)
add_test(
    NAME splice_singly_linked_list_range
    COMMAND splice_singly_linked_list_range.exe
)
add_executable(prepend_singly_linked_list_with_range
    tests/prepend_singly_linked_list_with_range.c
)
target_link_libraries(prepend_singly_linked_list_with_range PRIVATE linked_list)
add_test(
    NAME prepend_singly_linked_list_with_range
    COMMAND prepend_singly_linked_list_with_range.exe
)
add_executable(splice_singly_linked_list_beginning
    tests/splice_singly_linked_list_beginning.c
)
target_link_libraries(splice_singly_linked_list_beginning PRIVATE linked_list)
add_test(
    NAME splice_singly_linked_list_beginning
    COMMAND splice_singly_linked_list_beginning.exe
)
add_executable(prepend_singly_linked_list_with_beginning
    tests/prepend_singly_linked_list_with_beginning.c
)
target_link_libraries(prepend_singly_linked_list_with_beginning PRIVATE 
    linked_list
)
add_test(
    NAME prepend_singly_linked_list_with_beginning
    COMMAND prepend_singly_linked_list_with_beginning.exe
)
add_executable(pop_singly_linked_list_beginning
    tests/pop_singly_linked_list_beginning.c
)
target_link_libraries(pop_singly_linked_list_beginning PRIVATE linked_list)
add_test(
    NAME pop_singly_linked_list_beginning
    COMMAND pop_singly_linked_list_beginning.exe
)
add_executable(pop_singly_linked_list_range
    tests/pop_singly_linked_list_range.c
)
target_link_libraries(pop_singly_linked_list_range PRIVATE linked_list)
add_test(
    NAME pop_singly_linked_list_range
    COMMAND pop_singly_linked_list_range.exe
)
add_executable(copy_create_destroy_doubly_linked_list 
    tests/copy_create_destroy_doubly_linked_list.c
)
target_link_libraries(copy_create_destroy_doubly_linked_list PRIVATE 
    linked_list
)
add_test(
    NAME copy_create_destroy_doubly_linked_list
    COMMAND copy_create_destroy_doubly_linked_list.exe
)
add_executable(create_destroy_doubly_linked_list_node 
    tests/create_destroy_doubly_linked_list_node.c
)
target_link_libraries(create_destroy_doubly_linked_list_node PRIVATE 
    linked_list
)
add_test(
    NAME create_destroy_doubly_linked_list_node
    COMMAND create_destroy_doubly_linked_list_node.exe
)
add_executable(copy_create_destroy_doubly_linked_list_node 
    tests/copy_create_destroy_doubly_linked_list_node.c
)
target_link_libraries(copy_create_destroy_doubly_linked_list_node PRIVATE 
    linked_list
)
add_test(
    NAME copy_create_destroy_doubly_linked_list_node
    COMMAND copy_create_destroy_doubly_linked_list_node.exe
)
add_executable(create_destroy_doubly_linked_list_with_mutex_type 
    tests/create_destroy_doubly_linked_list_with_mutex_type.c
)
target_link_libraries(create_destroy_doubly_linked_list_with_mutex_type PRIVATE 
    linked_list
)
add_test(
    NAME create_destroy_doubly_linked_list_with_mutex_type
    COMMAND create_destroy_doubly_linked_list_with_mutex_type.exe
)
add_executable(create_destroy_doubly_linked_list 
    tests/create_destroy_doubly_linked_list.c
)
target_link_libraries(create_destroy_doubly_linked_list PRIVATE 
    linked_list
)
add_test(
    NAME create_destroy_doubly_linked_list
    COMMAND create_destroy_doubly_linked_list.exe
)
add_executable(doubly_linked_list_push_back
    tests/doubly_linked_list_push_back.c
)
target_link_libraries(doubly_linked_list_push_back PRIVATE 
    linked_list
)
add_test(
    NAME doubly_linked_list_push_back
    COMMAND doubly_linked_list_push_back.exe
)
add_executable(reverse_iterate_doubly_linked_list 
    tests/reverse_iterate_doubly_linked_list.c
)
target_link_libraries(reverse_iterate_doubly_linked_list PRIVATE 
    linked_list
)
add_test(
    NAME reverse_iterate_doubly_linked_list
    COMMAND reverse_iterate_doubly_linked_list.exe
)
add_executable(change_doubly_linked_list_node_data_type 
    tests/change_doubly_linked_list_node_data_type.c
)
target_link_libraries(change_doubly_linked_list_node_data_type PRIVATE 
    linked_list
)
add_test(
    NAME change_doubly_linked_list_node_data_type
    COMMAND change_doubly_linked_list_node_data_type.exe
)
add_executable(copy_doubly_linked_list_node 
    tests/copy_doubly_linked_list_node.c
)
target_link_libraries(copy_doubly_linked_list_node PRIVATE 
    linked_list
)
add_test(
    NAME copy_doubly_linked_list_node
    COMMAND copy_doubly_linked_list_node.exe
)
add_executable(destroy_doubly_linked_list_node
    tests/destroy_doubly_linked_list_node.c
)
target_link_libraries(destroy_doubly_linked_list_node PRIVATE 
    linked_list
)
add_test(
    NAME destroy_doubly_linked_list_node
    COMMAND destroy_doubly_linked_list_node.exe
)
add_executable(erase_following_doubly_linked_list_nodes
    tests/erase_following_doubly_linked_list_nodes.c
)
target_link_libraries(erase_following_doubly_linked_list_nodes PRIVATE 
    linked_list
)
add_test(
    NAME erase_following_doubly_linked_list_nodes
    COMMAND erase_following_doubly_linked_list_nodes.exe
)
add_executable(erase_preceding_doubly_linked_list_nodes
    tests/erase_preceding_doubly_linked_list_nodes.c
)
target_link_libraries(erase_preceding_doubly_linked_list_nodes PRIVATE 
    linked_list
)
add_test(
    NAME erase_preceding_doubly_linked_list_nodes
    COMMAND erase_preceding_doubly_linked_list_nodes.exe
)
add_executable(push_next_doubly_linked_list_node
    tests/push_next_doubly_linked_list_node.c
)
target_link_libraries(push_next_doubly_linked_list_node PRIVATE 
    linked_list
)
add_test(
    NAME push_next_doubly_linked_list_node
    COMMAND push_next_doubly_linked_list_node.exe
)
add_executable(push_doubly_linked_list_node
    tests/push_doubly_linked_list_node.c
)
target_link_libraries(push_doubly_linked_list_node PRIVATE 
    linked_list
)
add_test(
    NAME push_doubly_linked_list_node
    COMMAND push_doubly_linked_list_node.exe
)
add_executable(reverse_doubly_linked_list
    tests/reverse_doubly_linked_list.c
)
target_link_libraries(reverse_doubly_linked_list PRIVATE 
    linked_list
)
add_test(
    NAME reverse_doubly_linked_list
    COMMAND reverse_doubly_linked_list.exe
)
add_executable(swap_doubly_linked_list_nodes
    tests/swap_doubly_linked_list_nodes.c
)
target_link_libraries(swap_doubly_linked_list_nodes PRIVATE 
    linked_list
)
add_test(
    NAME swap_doubly_linked_list_nodes
    COMMAND swap_doubly_linked_list_nodes.exe
)
add_executable(pop_doubly_linked_list_node
    tests/pop_doubly_linked_list_node.c
)
target_link_libraries(pop_doubly_linked_list_node PRIVATE 
    linked_list
)
add_test(
    NAME pop_doubly_linked_list_node
    COMMAND pop_doubly_linked_list_node.exe
)
add_executable(doubly_linked_list_pop_back
    tests/doubly_linked_list_pop_back.c
)
target_link_libraries(doubly_linked_list_pop_back PRIVATE 
    linked_list
)
add_test(
    NAME doubly_linked_list_pop_back
    COMMAND doubly_linked_list_pop_back.exe
)
add_executable(doubly_linked_list_destroy_back
    tests/doubly_linked_list_destroy_back.c
)
target_link_libraries(doubly_linked_list_destroy_back PRIVATE 
    linked_list
)
add_test(
    NAME doubly_linked_list_destroy_back
    COMMAND doubly_linked_list_destroy_back.exe
)
add_executable(empty_doubly_linked_list
    tests/empty_doubly_linked_list.c
)
target_link_libraries(empty_doubly_linked_list PRIVATE 
    linked_list
)
add_test(
    NAME empty_doubly_linked_list
    COMMAND empty_doubly_linked_list.exe
)
add_executable(invert_doubly_linked_list
    tests/invert_doubly_linked_list.c
)
target_link_libraries(invert_doubly_linked_list PRIVATE 
    linked_list
)
add_test(
    NAME invert_doubly_linked_list
    COMMAND invert_doubly_linked_list.exe
)
add_executable(swap_doubly_linked_list
    tests/swap_doubly_linked_list.c
)
target_link_libraries(swap_doubly_linked_list PRIVATE 
    linked_list
)
add_test(
    NAME swap_doubly_linked_list
    COMMAND swap_doubly_linked_list.exe
)
add_executable(splice_doubly_linked_list
    tests/splice_doubly_linked_list.c
)
target_link_libraries(splice_doubly_linked_list PRIVATE 
    linked_list
)
add_test(
    NAME splice_doubly_linked_list
    COMMAND splice_doubly_linked_list.exe
)
add_executable(splice_doubly_linked_list_range
    tests/splice_doubly_linked_list_range.c
)
target_link_libraries(splice_doubly_linked_list_range PRIVATE 
    linked_list
)
add_test(
    NAME splice_doubly_linked_list_range
    COMMAND splice_doubly_linked_list_range.exe
)
add_executable(append_doubly_linked_list_with_range
    tests/append_doubly_linked_list_with_range.c
)
target_link_libraries(append_doubly_linked_list_with_range PRIVATE 
    linked_list
)
add_test(
    NAME append_doubly_linked_list_with_range
    COMMAND append_doubly_linked_list_with_range.exe
)
add_executable(pop_doubly_linked_list_range
    tests/pop_doubly_linked_list_range.c
)
target_link_libraries(pop_doubly_linked_list_range PRIVATE 
    linked_list
)
add_test(
    NAME pop_doubly_linked_list_range
    COMMAND pop_doubly_linked_list_range.exe
)