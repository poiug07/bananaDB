set (Boost_USE_STATIC_LIBS_OFF)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)
include_directories (${Boost_INCLUDE_DIRS})

aux_source_directory(../src DIR_SRCS)
add_executable(Boost_Tests_run test.cpp ../src/bananadb/store.cpp)
target_link_libraries (Boost_Tests_run ${Boost_LIBRARIES})