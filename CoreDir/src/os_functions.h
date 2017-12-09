//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : os_functions.h                                                //
//  Project   : CoreDir                                                       //
//  Date      : Oct 17, 2017                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2017                                             //
//                                                                            //
//  Description :                                                             //
//    Function declarations for the platform specific pieces.                 //
//    To enable CoreDir.cpp to be implemented only once all functions         //
//    that are depended of platform specific pieces are forwarded to          //
//    functions declared here.                                                //
//                                                                            //
//    Each platform implement those functions separately.                     //
//---------------------------------------------------------------------------~//

#pragma once
//std
#include <vector>
#include <string>
//CoreDir
#include "../include/CoreDir_Utils.h"

NS_COREDIR_BEGIN

//------------------------------------------------------------------------------
bool os_rename     (const std::string &src, const std::string &dst);
bool os_remove_file(const std::string &path);
bool os_remove_dir (const std::string &path);
bool os_mkdir      (const std::string &path, int perms);

void os_chdir(const std::string &path);

//------------------------------------------------------------------------------
std::vector<std::string> os_get_filesystem_entries_helper(
    const std::string &path,
    const std::string &pattern,
    bool               recursive,
    bool               getFiles,
    bool               getDirs);

NS_COREDIR_END
