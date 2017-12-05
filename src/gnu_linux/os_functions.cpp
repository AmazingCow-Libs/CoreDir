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
//    Implementation for GNU/Linux.                                           //
//---------------------------------------------------------------------------~//

#if __linux__

// Header
#include "../os_functions.h"
// C
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
// std
#include <regex>


//------------------------------------------------------------------------------
bool CoreDir::os_rename(const std::string &src, const std::string &dst)
{
    return rename(src.c_str(), dst.c_str());
}

//------------------------------------------------------------------------------
bool CoreDir::os_remove_file(const std::string &path)
{
    return remove(path.c_str());
}

//------------------------------------------------------------------------------
bool CoreDir::os_remove_dir(const std::string &path)
{
    return rmdir(path.c_str());
}

//------------------------------------------------------------------------------
bool CoreDir::os_mkdir(const std::string &path, int perms)
{
    return mkdir(path.c_str(), perms);
}

//------------------------------------------------------------------------------
std::vector<std::string> os_get_filesystem_entries_helper(
    const std::string &path,
    const std::string &pattern,
    bool               recursive,
    bool               getFiles,
    bool               getDirs)
{
    std::vector<std::string> entries;

    DIR *p_DIR = opendir(path.c_str());
    if(!p_DIR)
        return entries;

    auto re = std::regex(pattern, std::regex::egrep);
    auto sm = std::smatch();

    struct dirent *p_dirent = nullptr;
    while((p_dirent = readdir(p_DIR)) != nullptr)
    {
        auto name = std::string(p_dirent->d_name);
        if(name == ".." || name == ".")
            continue;

        // Test if name matches the pattern.
        auto match = std::regex_match(name, sm, re);
        if(!match)
            continue;

        auto fullname = CoreFS::Join(path, {name});
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
    }

    return entries;
}

#endif //__linux__
