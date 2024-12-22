# Notify that this file was loaded.
message("Linux compiling for Linux.")

# Allow cmake to find the Qt library (this could be automated and version independent)
list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/Qt/6.4.2/gcc_64/lib/cmake/")
list(APPEND Qt6_DIR "$ENV{HOME}/Qt/6.4.2/gcc_64/")

# # Use mingw 64-bit compilers.
# set(CMAKE_C_COMPILER gcc)
# set(CMAKE_CXX_COMPILER g++)