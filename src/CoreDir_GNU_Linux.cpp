//----------------------------------------------------------------------------//
//               █      █                                                     //
//               ████████                                                     //
//             ██        ██                                                   //
//            ███  █  █  ███        CoreDir_GNU_Linux.cpp                     //
//            █ █        █ █        CoreDir                                   //
//             ████████████                                                   //
//           █              █       Copyright (c) 2017                        //
//          █     █    █     █      AmazingCow - www.AmazingCow.com           //
//          █     █    █     █                                                //
//           █              █       N2OMatt - n2omatt@amazingcow.com          //
//             ████████████         www.amazingcow.com/n2omatt                //
//                                                                            //
//                  This software is licensed as GPLv3                        //
//                 CHECK THE COPYING FILE TO MORE DETAILS                     //
//                                                                            //
//    Permission is granted to anyone to use this software for any purpose,   //
//   including commercial applications, and to alter it and redistribute it   //
//               freely, subject to the following restrictions:               //
//                                                                            //
//     0. You **CANNOT** change the type of the license.                      //
//     1. The origin of this software must not be misrepresented;             //
//        you must not claim that you wrote the original software.            //
//     2. If you use this software in a product, an acknowledgment in the     //
//        product IS HIGHLY APPRECIATED, both in source and binary forms.     //
//        (See opensource.AmazingCow.com/acknowledgment.html for details).    //
//        If you will not acknowledge, just send us a email. We'll be         //
//        *VERY* happy to see our work being used by other people. :)         //
//        The email is: acknowledgment_opensource@AmazingCow.com              //
//     3. Altered source versions must be plainly marked as such,             //
//        and must not be misrepresented as being the original software.      //
//     4. This notice may not be removed or altered from any source           //
//        distribution.                                                       //
//     5. Most important, you must have fun. ;)                               //
//                                                                            //
//      Visit opensource.amazingcow.com for more open-source projects.        //
//                                                                            //
//                                  Enjoy :)                                  //
//----------------------------------------------------------------------------//

#if __linux__
//Header
#include "../include/CoreDir.h"
//C
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
//std
#include <regex>
//CoreFS
#include "CoreFS.h"


////////////////////////////////////////////////////////////////////////////////
// Helper Functions                                                           //
////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> get_filesystem_entries_helper(
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

        //Test if name matches the pattern.
        auto match = std::regex_match(name, sm, re);
        if(!match)
            continue;

        auto fullname = CoreFS::Join(path, {name});
        bool is_dir   = CoreFS::IsDir(fullname);

        //This entry is a directory we need:
        //  1 - Check if we want get the directory entries.
        //  2 - Check if we want recursive search.
        if(is_dir)
        {
            //1 - We want get the directory entries?
            if(getDirs)
                entries.push_back(fullname);

            //2 - We want recursive search?
            if(recursive)
            {
                auto recursive_entries = CoreDir::GetDirectories(
                    fullname,
                    pattern,
                    recursive
                );

                std::copy(
                    recursive_entries.begin(),
                    recursive_entries.end  (),
                    std::back_inserter(entries)
                );
            }
        }
        //This is a regular file, we need check
        //if we want get the file entries
        else if(getFiles)
        {
            entries.push_back(fullname);
        }
    }

    return entries;
}

////////////////////////////////////////////////////////////////////////////////
// Function Definitions                                                       //
////////////////////////////////////////////////////////////////////////////////
//Creates all directories and subdirectories in the
//specified path unless they already exist.
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
    for(const auto &comp : components)
    {
        curr_path        = CoreFS::Join(curr_path, {comp});
        auto need_create = !CoreFS::Exists(curr_path);

        if(need_create)
        {
            //Some error occurred...
            //  Let's try to delete all created directories.
            if(mkdir(curr_path.c_str(), mode) != 0)
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
            CoreDir::Delete(*rit);
        }
    }

    return !clear_dirs_created;
}

//Deletes the specified directory and, if indicated, any
//subdirectories and files in the directory.
bool CoreDir::Delete(const std::string &path, bool recursive /* = false */)
{
    //COWHACK(n2omatt): We can make this function based
    //  upon GetDirectories and GetFiles.
    //  1 - Get all Directories recursive.
    //  2 - Get all Files recursive.
    //  3 - Delete Files.
    //  4 - Delete Dirs.

    //Simple case:
    //  Just delete the directory itself, it'll fail
    //  unless the dir is already empty...
    if(!recursive)
        return (rmdir(path.c_str()) == 0);

    //Recursive case:
    //  1 - For each directory entry found call CoreDir::Delete
    //      with the entry path.
    //  2 - Delete all files found in current path.
    //  3 - Delete the directory entry.
    DIR *p_DIR = opendir(path.c_str());
    if(!p_DIR)
        return false;

    struct dirent *p_dirent = nullptr;
    while((p_dirent = readdir(p_DIR)) != nullptr)
    {
        auto name = std::string(p_dirent->d_name);
        if(name == ".." || name == ".")
            continue;

        auto fullname = CoreFS::Join(path, {name});

        // 1 - For each directory entry found call CoreDir::Delete
        //     with the entry path.
        bool is_dir = CoreFS::IsDir(fullname);
        if(is_dir)
            CoreDir::Delete(fullname, recursive);

        //We can call remove(3) both for files and directories.
        // 2 - Delete all files found in current path.
        // 3 - Delete the directory entry.
        //COWTODO(n2omatt): Should we check the errors here????
        remove(fullname.c_str());
    }

    closedir(p_DIR);
    p_DIR = nullptr;

    return false;
}



//Returns the names of the subdirectories (including their paths)
//that match the specified search pattern in the specified directory,
//and optionally searches subdirectories.
std::vector<std::string> CoreDir::GetDirectories(
    const std::string &path,
    const std::string &pattern   /* = ".*"  */,
    bool               recursive /* = true */ )
{
    return get_filesystem_entries_helper(path, pattern, recursive, false, true);
}


//Returns the names of files (including their paths) that match the
//specified search pattern in the specified directory, using a value
//to determine whether to search subdirectories.
std::vector<std::string> CoreDir::GetFiles(
    const std::string &path,
    const std::string &pattern   /* = ".*"  */,
    bool               recursive /* = true */)
{
    return get_filesystem_entries_helper(path, pattern, recursive, true, false);
}

//Returns an array of all the file names and directory names that match
//a search pattern in a specified path, and optionally searches subdirectories.
std::vector<std::string> CoreDir::GetFileSystemEntries(
    const std::string &path,
    const std::string &pattern   /* = ".*"  */,
    bool               recursive /* = true */)
{
    return get_filesystem_entries_helper(path, pattern, recursive, true, true);
}


//Determines whether the given path refers to
//an existing directory on disk.
bool CoreDir::Exists(const std::string &path)
{
    return CoreFS::IsDir(path);
}

//Moves a file or a directory and its contents to a new location.
bool CoreDir::Move(const std::string &src, const std::string &dst)
{
    auto abs_src = CoreFS::AbsPath(src);
    auto abs_dst = CoreFS::AbsPath(dst);

    if(abs_src == abs_dst)
        return false;

    return (rename(abs_src.c_str(), abs_dst.c_str()) == 0);
}

#endif //#if __linux__
