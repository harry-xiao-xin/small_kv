find_package(GTest REQUIRED)
find_package(glog REQUIRED)

include_directories(../include)
aux_source_directory(../test DIR_TEST)
aux_source_directory(../test/test_cache DIR_TEST)
set(link_lib glog gtest pthread)

add_executable(test_small_kv ${DIR_TEST} ${DIR_TEST_CACHE})
link_directories(/usr/local/lib/)
target_link_libraries(test_small_kv ${link_lib})