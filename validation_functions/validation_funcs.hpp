#pragma once

#include <iostream>

auto validateImageExtension(std::string const& path) -> bool;
auto validateNumOfArgs(int const& numOfArgs, int const& expectedNum) -> bool;
auto checkIfFileExists(std::string const& path) -> bool;
auto checkFilePermissions(std::string const& path) -> bool;
auto validateArguments(int const& numOfArgs, int const& expectedNum, std::string const& path) -> bool;