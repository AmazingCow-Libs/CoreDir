#pragma once

//All classes of this core is placed inside this namespace.
//We use MACROS so is easier to change if needed.
//Is (in our opinion) more explicit.
//And finally the editors will not reformat the code.
#define NS_COREDIR_BEGIN namespace CoreDir {
#define NS_COREDIR_END   }
#define USING_NS_COREDIR using namespace CoreDir

//The core version number.
#define COW_COREDIR_VERSION_MAJOR    "0"
#define COW_COREDIR_VERSION_MINOR    "0"
#define COW_COREDIR_VERSION_REVISION "0"

#define COW_COREDIR_VERSION       \
    COW_COREDIR_VERSION_MAJOR "." \
    COW_COREDIR_VERSION_MINOR "." \
    COW_COREDIR_VERSION_REVISION
