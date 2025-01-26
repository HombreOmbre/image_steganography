#pragma once

#include <iostream>

auto showInfoAboutFile(std::string const& path) -> void;
auto encryptFile(std::string const& path, std::string & msg) -> void;
auto decryptFile(std::string const& path) -> void;
auto checkIfMessageCanBeInsertedIntoFile(std::string const& path, std::string & msg) -> void;
auto showHelpInfoAboutApp() -> void;