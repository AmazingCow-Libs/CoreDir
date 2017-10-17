//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : CoreDir.h                                                     //
//  Project   : CoreDir                                                       //
//  Date      : Aug 11, 2017                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2017                                             //
//                                                                            //
//  Description :                                                             //
//                                                                            //
//---------------------------------------------------------------------------~//

#pragma once

//std
#include <string>
#include <vector>
//CoreDir
#include "CoreDir_Utils.h"

//
// Notice:
//   This lib is a "rip-off" of the C# Directory class.
//
//   We find very useful have those "functions" on C#,
//   but we want to use them on C++ as well.
//   We made a lot of effort to make it correct and respect the
//   semantics of both original API and C++ usage.
//   But there some places that it wasn't possible or would make
//   the API hard to use. On these places we choose to the possibility
//   of the easier usage.
//
//   There some functions that aren't implemented and we don't have
//   plans to implement in a near future, for those, you're free
//   to implement ;D
//
//   Lastly, our target here is make it very usable both on Windows and GNU/Linux,
//   there are no efforts currently to make it usable on OSX or other Unices neither
//   for mobile platforms. But this might change on future.
//
//   Have fun...
//   n2omatt - <n2omatt@amazingcow.com>
//
//   C# API Reference:
//      https://msdn.microsoft.com/en-us/library/system.io.directory(v=vs.110).aspx
//

NS_COREDIR_BEGIN

//Creates all directories and subdirectories in the
//specified path unless they already exist.
bool CreateDirectory(const std::string &path, unsigned mode = 0777);

//Deletes the specified directory and, if indicated, any
//subdirectories and files in the directory.
bool Delete(const std::string &path, bool recursive = false);


//Returns the names of the subdirectories (including their paths)
//that match the specified search pattern in the specified directory,
//and optionally searches subdirectories.
std::vector<std::string> GetDirectories(
        const std::string &path,
        const std::string &pattern   = ".*",
        bool               recursive = true);

//Returns the names of files (including their paths) that match the
//specified search pattern in the specified directory, using a value
//to determine whether to search subdirectories.
std::vector<std::string> GetFiles(
    const std::string &path,
    const std::string &pattern   = ".*",
    bool               recursive = true);

//Returns an array of all the file names and directory names that match
//a search pattern in a specified path, and optionally searches subdirectories.
std::vector<std::string> GetFileSystemEntries(
    const std::string &path,
    const std::string &pattern   = ".*",
    bool               recursive = true);


//Determines whether the given path refers to
//an existing directory on disk.
bool Exists(const std::string &path);

//Moves a file or a directory and its contents to a new location.
bool Move(const std::string &src, const std::string &dst);

NS_COREDIR_END
