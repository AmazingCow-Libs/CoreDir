//COWTODO: Add license header.
#pragma once

//std
#include <string>
#include <vector>
//CoreDir
#include "CoreDir_Utils.h"


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
