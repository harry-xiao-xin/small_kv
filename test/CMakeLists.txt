find_package(GTest REQUIRED)
find_package(glog REQUIRED)

include_directories(../include)
# source code
aux_source_directory(../src/db LIB_TEST_DB)
aux_source_directory(../src/memory LIB_TEST_MEMORY)
aux_source_directory(../src/file LIB_TEST_FILE)

# test code
aux_source_directory(../test DIR_TEST)
aux_source_directory(../test/test_cache DIR_TEST_CACHE)
aux_source_directory(../test/test_memory DIR_TEST_MEMORY)
aux_source_directory(../test/test_db DIR_TEST_DB)
aux_source_directory(../test/test_file DIR_TEST_FILE)


set(link_lib glog gtest pthread)

add_executable(test_small_kv ${LIB_TEST_DB} ${LIB_TEST_MEMORY} ${LIB_TEST_FILE}
        ${DIR_TEST} ${DIR_TEST_CACHE} ${DIR_TEST_MEMORY} ${DIR_TEST_DB} ${DIR_TEST_FILE})
link_directories(/usr/local/lib/)
target_link_libraries(test_small_kv ${link_lib})