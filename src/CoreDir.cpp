//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : CoreDir.cpp                                                   //
//  Project   : CoreDir                                                       //
//  Date      : Oct 17, 2017                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2017                                             //
//                                                                            //
//  Description :                                                             //
//    Implementation for the CoreDir functions.                               //
//    Notice that this file implements all functions in a platform agnostic   //
//    way, so all things that are specific to each platform is forwarded      //
//    to a external function.                                                 //
//                                                                            //
//    This functions are defined in a "private" header, called os_functions.h //
//    and implemented separately for each supported platform.                 //
//                                                                            //
//    So to add a new platform is just create the platform directory          //
//    and implement the os_functions.cpp for it.                              //
//---------------------------------------------------------------------------~//

// Header
#include "../include/CoreDir.h"
// C
#include <stdio.h>
// CoreFS
#include "CoreFS/CoreFS.h"
// Wrappers
#include "os_functions.h"


//----------------------------------------------------------------------------//
// Function Definitions                                                       //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
bool CoreDir::CreateDirectory(
    const std::string &path,
    unsigned           mode /* = 0777 */)
{
    if(CoreFS::Exists(path))
        return false;

    auto curr_path          = CoreFS::AbsPath(path);
    auto components         = CoreFS::SplitAll(curr_path);
    auto dirs_created       = std::vector<std::string>();
    auto clear_dirs_created = false; //For when an error occurrs
                                     //we can delete the created dirs.

    curr_path.clear();
    curr_path = components.front();

    for(auto it = ++components.begin(); it != components.end(); ++it)
    {
        curr_path        = CoreFS::Join(curr_path, {*it});
        auto need_create = !CoreFS::Exists(curr_path);

        if(need_create)
        {
            //Some error occurred...
            //  Let's try to delete all created directories.
            if(!os_mkdir(curr_path.c_str(), mode))
            {
                clear_dirs_created = true;
                break;
            }

            dirs_created.push_back(curr_path);
        }
    }

    //An error occurred that prevented the fullpath to be created
    //  So we gonna try to delete all directories that we created
    //  before, this way we try to maintain the filesystem consistency
    //  as we found before the functions.
    if(clear_dirs_created)
    {
        for(auto rit = dirs_created.rbegin();
            rit != dirs_created.rend();
            ++rit)
        {
            os_remove_dir(*rit);
        }
    }

    return !clear_dirs_created;
}

//------------------------------------------------------------------------------
bool CoreDir::Delete(const std::string &path, bool recursive /* = false */)
{
    auto all_files = GetFiles(path, ".*", true);
    //We can't delete non empty directories.
    if(all_files.size() > 0 && !recursive)
        return false;

    auto all_dirs = GetDirectories(path, ".*", true);

    //Remove all files
    //  This will let all underneath directories empty...
    for(const auto &filename : all_files)
    {
        auto success = os_remove_file(filename);
        if(!success)
            return false;
    }

    //Remove all directories.
    for(const auto &dirname : all_dirs)
    {
        auto success = os_remove_dir(dirname);
        if(!success)
            return false;
    }

    os_remove_dir(path);

    return true;
}

//------------------------------------------------------------------------------
std::vector<std::string> CoreDir::GetDirectories(
    const std::string &path,
    const std::string &pattern   /* = ".*"  */,
    bool               recursive /* = true */ )
{
    return os_get_filesystem_entries_helper(
        CoreFS::AbsPath(path),
        pattern,
        recursive,
        false,
        true
    );
}

//------------------------------------------------------------------------------
std::vector<std::string> CoreDir::GetFiles(
    const std::string &path,
    const std::string &pattern   /* = ".*"  */,
    bool               recursive /* = true */)
{
    return os_get_filesystem_entries_helper(
        CoreFS::AbsPath(path),
        pattern,
        recursive,
        true,
        false
    );
}

//------------------------------------------------------------------------------
std::vector<std::string> CoreDir::GetFileSystemEntries(
    const std::string &path,
    const std::string &pattern   /* = ".*"  */,
    bool               recursive /* = true */)
{
    return os_get_filesystem_entries_helper(
        CoreFS::AbsPath(path),
        pattern,
        recursive,
        true,
        true
    );
}

//------------------------------------------------------------------------------
bool CoreDir::Exists(const std::string &path)
{
    return CoreFS::IsDir(path);
}

//------------------------------------------------------------------------------
bool CoreDir::Move(const std::string &src, const std::string &dst)
{
    auto abs_src = CoreFS::AbsPath(src);
    auto abs_dst = CoreFS::AbsPath(dst);

    if(abs_src == abs_dst)
        return false;

    if(!CoreDir::Exists(abs_src))
        return false;

    if(CoreDir::Exists(abs_dst))
        return false;

    return os_rename(abs_src.c_str(), abs_dst.c_str());
}
