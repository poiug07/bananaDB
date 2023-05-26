add_library(server server.cpp)

target_link_libraries(server bananadb)

target_include_directories(server PUBLIC "${PROJECT_SOURCE_DIR}/src/bananadb")