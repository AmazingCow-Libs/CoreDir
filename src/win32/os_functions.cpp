//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : os_functions.cpp                                              //
//  Project   : CoreDir                                                       //
//  Date      : Oct 17, 2017                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2017                                             //
//                                                                            //
//  Description :                                                             //
//    Implementation for Windows.                                             //
//---------------------------------------------------------------------------~//

#if _WIN32

//Header
#include "../os_functions.h"
//std
#include <regex>
//Windows
#include <Windows.h>
//CoreFS
#include "CoreFS.h"


bool CoreDir::os_rename(const std::string &src, const std::string &dst)
{
    return MoveFileA(src.c_str(), dst.c_str());
}

bool CoreDir::os_remove_file(const std::string &path)
{
    return DeleteFileA(path.c_str());
}

bool CoreDir::os_remove_dir(const std::string &path)
{
    return RemoveDirectoryA(path.c_str());
}

bool CoreDir::os_mkdir(
    const std::string &path,
    int /* perms - On Windows we ignore the flags... */)
{
    return CreateDirectoryA(path.c_str(), nullptr);
}

std::vector<std::string> CoreDir::os_get_filesystem_entries_helper(
    const std::string &path,
    const std::string &pattern,
    bool               recursive,
    bool               getFiles,
    bool               getDirs)
{
    //References:
    //  https://msdn.microsoft.com/en-us/library/windows/desktop/aa365200(v=vs.85).aspx
    //  FindFirstFile: https://msdn.microsoft.com/en-us/library/windows/desktop/aa364418(v=vs.85).aspx
    //  FindNextFile : https://msdn.microsoft.com/en-us/library/windows/desktop/aa364428(v=vs.85).aspx
    //  FindClose    : https://msdn.microsoft.com/en-us/library/windows/desktop/aa364413(v=vs.85).aspx
    std::vector<std::string> entries;

    WIN32_FIND_DATAA ffd         = { 0 };
    HANDLE           hFind       = INVALID_HANDLE_VALUE;
    auto             search_path = path + "\\*";

    hFind = FindFirstFileA(search_path.c_str(), &ffd);
    if(hFind == INVALID_HANDLE_VALUE)
        return entries;

    auto re = std::regex(pattern, std::regex::egrep);
    auto sm = std::smatch();

    do {
        auto name = std::string(ffd.cFileName);
        if(name == ".." || name == ".")
            continue;

        //Test if name matches the pattern.
        auto match = std::regex_match(name, sm, re);
        if(!match)
            continue;

        auto fullname = CoreFS::Join(path, { name });
        bool is_dir   = CoreFS::IsDir(fullname);

        if(is_dir && getDirs)
            entries.push_back(fullname);
        else if(!is_dir && getFiles)
            entries.push_back(fullname);

        if(recursive)
        {
            auto recursive_entries = os_get_filesystem_entries_helper(
                fullname,
                pattern,
                recursive,
                getFiles,
                getDirs
            );

            std::copy(
                recursive_entries.begin(),
                recursive_entries.end(),
                std::back_inserter(entries)
            );
        }

    } while (FindNextFileA(hFind, &ffd) != 0);


    auto dwError = GetLastError();
    FindClose(hFind);

    if(dwError != ERROR_NO_MORE_FILES)
        entries.clear();

    return entries;
}

#endif //_WIN32
