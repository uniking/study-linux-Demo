link_directories(../../lib)
add_library(myadd SHARED myadd.cpp)
target_link_libraries(myadd mysub)
