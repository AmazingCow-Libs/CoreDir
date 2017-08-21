//Header
#include "../include/CoreDir.h"
//C
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
//CoreFS
#include "CoreFS.h"


#if __linux__

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
std::vector<std::string> GetDirectories(
    const std::string &path,
    const std::string &pattern   = "*",
    bool               recursive = true);


//Returns the names of files (including their paths) that match the
//specified search pattern in the specified directory, using a value
//to determine whether to search subdirectories.
std::vector<std::string> GetFiles(
    const std::string &path,
    const std::string &pattern   = "*",
    bool               recursive = true);

//Returns an array of all the file names and directory names that match
//a search pattern in a specified path, and optionally searches subdirectories.
std::vector<std::string> GetFileSystemEntries(
    const std::string &path,
    const std::string &pattern   = "*",
    bool               recursive = true);


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
