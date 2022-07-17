cmake_minimum_required(VERSION 3.21)
project (maman14 C)

set(CMAKE_C_STANDARD 90)

add_executable(maman14 main.c utils/files.c utils/files.h utils/strings.c utils/strings.h utils/conversion.c utils/conversion.h)
