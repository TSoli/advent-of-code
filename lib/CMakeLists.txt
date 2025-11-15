add_library(lib src/io.cpp)
target_include_directories(lib PUBLIC include)
target_compile_features(lib PUBLIC cxx_std_20)
