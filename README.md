# CoreDir

**Made with <3 by [Amazing Cow](http://www.amazingcow.com).**

<!-- ----------------------------------------------------------------------- -->
<!-- Websites                                                                -->
<!-- ----------------------------------------------------------------------- -->

## Websites:

General:
* [Amazing Cow's page](http://www.amazingcow.com).
* [Amazing Cow's Github](https://github.com/AmazingCow).

Libs:
* [Amazing Cow's Libs page](https://github.com/AmazingCow-Libs).
* [Amazing Cow's Libs Github](http://www.amazingcow.com/libs).


<!-- ----------------------------------------------------------------------- -->
<!-- Description                                                             -->
<!-- ----------------------------------------------------------------------- -->

## Description:

__CoreDir__  A C++, cross-platform implementation of the 
[C# Directory](https://msdn.microsoft.com/en-us/library/system.io.directory.aspx) class.

### Motivation:

While C/C++ (and the underlying platforms) provides everything that we need
to access and modify directories, the current way is far from optimal. 
There's no way to make the directory manipulation calls multiplatform by 
default and the resulting code  ends to be a mess of ```#ifdef``` 
and/or a custom wrapper everytime.

```CoreDir``` takes this opportunity to make a straightforward API based both 
on C# Directory class. This way we can use a nice API to manipulate directories
no matter which OS we're currently in.


<!-- ----------------------------------------------------------------------- -->
<!-- Examples                                                                -->
<!-- ----------------------------------------------------------------------- -->

## Examples:



<!-- ----------------------------------------------------------------------- -->
<!-- Implementation                                                          -->
<!-- ----------------------------------------------------------------------- -->
## Implementation:

| Function              | GNU/Linux     | W32   |
| --------------------- |:-------------:| -----:|
| CreateDirectory       | Done          | Done  |
| Delete                | Done          | Done  |
| GetDirectories        | Done          | Done  |
| GetFiles              | Done          | Done  |
| GetFileSystemEntries  | Done          | Done  |
| Exists                | Done          | Done  |
| Move                  | Done          | Done  | 


<!-- ----------------------------------------------------------------------- -->
<!-- Building                                                                -->
<!-- ----------------------------------------------------------------------- -->

## Building:

```CoreDir``` is intended to be include as source in your project.  
So to use it, just grab the whole CoreDir directory, put it in your 
project and setup the include paths in the compiler.

As example, let's imagine that you are copying it to a directory called ```libs```.
In your build script you need to do just the following:

``` bash
g++ -I /path/to/libs/CoreDir \
    ... Other stuff ...     \
    /path/to/libs/CoreDir/CoreDir/src/CoreDir.cpp           \ 
    /path/to/libs/CoreDir/CoreDir/gnu_linux/os_functions.cpp \
    /path/to/libs/CoreDir/CoreDir/win32/os_functions.cpp 
```

Please notice that there's no harm to compile both win32 and gnu_linux
```os_functions.cpp```, but you might want only build the platform specific one.


<!-- ----------------------------------------------------------------------- -->
<!-- Documentation                                                           -->
<!-- ----------------------------------------------------------------------- -->

## Documentation:

We strive to make our source code fully documented.   
While there are a myriad of comments, one might find useful take a look at:

* [Doxygen Docs](http://www.amazingcow.com/docs/CoreDir/).

Anyway if you didn't understand something let us know sending a mail to  
[help_floss@amazingcow.com]() with the subject filled with the name of this repo.



<!-- ----------------------------------------------------------------------- -->
<!-- Dependencies                                                            -->
<!-- ----------------------------------------------------------------------- -->

## Dependencies:

```CoreDir``` depends on [CoreFS](https://github.com/AmazingCow-Libs/CoreFS).
The project is included as a ```git submodule``` on the ```lib``` directory.

So to grab the ```CoreFS``` project is just a matter of: 

```bash
git submodule update --init --recursive
```


<!-- ----------------------------------------------------------------------- -->
<!-- License                                                                 -->
<!-- ----------------------------------------------------------------------- -->

## License:

This software is **proudly** released under [GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html).


<!-- ----------------------------------------------------------------------- -->
<!-- BUGS                                                                    -->
<!-- ----------------------------------------------------------------------- -->

## BUGS:

We strive to make all our code the most bug-free as possible - But we know 
that few of them can pass without we notice ;).

Please if you find any bug report to [bugs_floss@amazingcow.com]() 
with the name of this project and/or create an issue here in Github.


<!-- ----------------------------------------------------------------------- -->
<!-- TODO                                                                    -->
<!-- ----------------------------------------------------------------------- -->

## TODO:

Check the TODO file for general things.

This projects uses the COWTODO tags.   
So install [cowtodo](http://www.github.com/AmazingCow-Tools/COWTODO) and run:

``` bash
$ cd path/to/the/project
$ cowtodo 
```

That's gonna give you all things to do :D.


<!-- ----------------------------------------------------------------------- -->
<!-- Others                                                                  -->
<!-- ----------------------------------------------------------------------- -->

## Others:

Check our other Free Software repos and take a look at our 
[free software site](http://www.amazingcow.com/floss).

