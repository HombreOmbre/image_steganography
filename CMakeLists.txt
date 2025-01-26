cmake_minimum_required(VERSION 3.29)
project(image_steganography)

set(CMAKE_CXX_STANDARD 20)

add_executable(image_steganography
	main.cpp
	ppmFile/ppmFile.cpp
	pixel/Pixel.cpp
	bmpFile/bmpFile.cpp
	bmpFile/Header.cpp
	bmpFile/HeaderInfo.cpp
	flag_functions/flag_funcs.cpp
	validation_functions/validation_funcs.cpp
)
