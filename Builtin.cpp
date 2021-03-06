//
// Created by ilias on 9/8/21.
//

#include "Builtin.hpp"
#include "CustomException.hpp"
#include "Shell.hpp"
#include <cstring>
#include <filesystem>
#include <iostream>
#include <unistd.h>

Builtin::Builtin(Shell* shell)
    : shell_(shell) {}

Status Builtin::changeDirectory(const std::vector<std::string>& arguments) const
{
    auto path_index{ static_cast<size_t>(1) };
    std::string path{ arguments.size() == path_index ? shell_->getHomeDirectory() : arguments.at(path_index) };

    if (path.starts_with("~/")) {
        path = shell_->getHomeDirectory() + "/" + path.substr(static_cast<std::size_t>(2), path.length());
    }

    if (chdir(path.c_str()) != 0) {
        const int error_code{ errno };
        const std::string error_message{ strerror(error_code) };
        throw CustomException(error_message, Status::Continue);
    }
    return Status::Continue;
}

Status Builtin::printWorkingDirectory([[maybe_unused]] const std::vector<std::string>& arguments) const
{
    std::cout << std::filesystem::current_path() << std::endl;
    return Status::Continue;
}

Status Builtin::exit([[maybe_unused]] const std::vector<std::string>& arguments) const
{
    return Status::Terminate;
}

const std::unordered_map<std::string, builtinFunctionPointer_t>& Builtin::getBuiltinCommands() const
{
    return builtin_commands_;
}
