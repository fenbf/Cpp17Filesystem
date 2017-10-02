# Welcome!

Although C++ is an old programming language, its Standard Library misses a few basic things. Features that Java or .NET had for years were/are not available in STL. With C++17 there’s a nice improvement: for example, we now have the standard filesystem!

Traversing a path, even recursively is so simple now!

Maybe I was a bit harsh in the first paragraph. Although the Standard Library lacks some important features, you could always use Boost with its thousands of sub-libraries and do the work. The C++ Committee and the Community decided that the Boost libraries are so important that some of the systems were merged into the Standard. For example smart pointers (although improved with the move semantics in C++11), regular expressions, and much more.

The similar story happened with the filesystem. Let’s try to understand what’s inside.

This playground is adapted from my blog: Bartek's coding blog: [C++17 in details: Filesystem](http://www.bfilipek.com/2017/08/cpp17-details-filesystem.html)

Visit the blog if you're looking for more good stuff about C++ :)

# Documents & Links

First of all, if you want to dig into the standard on your own, you can read the latest draft here: 

[N4659, 2017-03-21, **Draft, Standard for Programming Language C++**](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4659.pdf) - from [isocpp.org](https://isocpp.org/).

Also, you can grab **my list of concise descriptions of all of the C++17** - It's a one-page reference card, pdf language features: 

[Grab it here: **C++17 Reference Card**](http://eepurl.com/cyycFz)

Links:

* Compiler support: [C++ compiler support](http://en.cppreference.com/w/cpp/compiler_support)
* The official paper with changes: [P0636r0: Changes between C++14 and C++17 DIS](https://isocpp.org/files/papers/p0636r0.html)
* There's also a talk from Bryce Lelbach: [C++Now 2017:  C++17 Features](https://www.youtube.com/watch?v=LvwXJjRQfHk)
* My master C++17 features post: [C++17 Features](http://www.bfilipek.com/2017/01/cpp17features.html)
* Jason Turner: [C++ Weekly channel](https://www.youtube.com/playlist?list=PLs3KjaCtOwSZ2tbuV1hx8Xz-rFZTan2J1), where he covered most (or even all!) of C++17 features.

And the books:

* [**C++17 STL Cookbook** by Jacek Galowicz](http://amzn.to/2f8wnLp)
* [**Modern C++ Programming Cookbook** by Marius Bancila](http://amzn.to/2eRaiUu)

# Filesystem Overview

I think the Committee made a right choice with this feature. The filesystem library is nothing new, as it's modeled directly over Boost filesystem, which is available since 2003 (with the version 1.30). There are only a little differences, plus some wording changes. Not to mention, all of this is also based on POSIX.

Thanks to this approach it's easy to port the code. Moreover, there's a good chance a lot of developers are already familiar with the library. (Hmmm... so why I am not that dev? :))

The library is located in the `<filesystem>` header. It uses namespace `std::filesystem`.

The final paper is [P0218R0: Adopt the File System TS for C++17](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0218r0.html) but there are also others like [P0317R1: Directory Entry Caching](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0317r1.html), [PDF: P0430R2–File system library on non-POSIX-like operating systems](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0430r2.pdf), [P0492R2](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0492r2.html)... All in all, you can find the final spec in the C++17 draft: the "filesystem" section, 30.10.

We have three/four core parts:

* The `path` object
* `directory_entry`
* Directory iterators
* Plus many supportive functions
 * getting information about the path
 * files manipulation: copy, move, create, symlinks
 * last write time
 * permissions
 * space/filesize
 * ...

## Compiler/Library support

Depending on the version of your compiler you might need to use `std::experimental::filesystem` namespace.

* GCC: You have to specify `-lstdc++fs` when you want filesystem. Implemented in `<experimental/filesystem>`.
* Clang should be ready with Clang 5.0
 * https://libcxx.llvm.org/cxx1z_status.html  
* Visual Studio: In VS 2017 (2017.2) you still have to use `std::experimental` namespace, it uses TS implementation.
 * See the [reference link](https://docs.microsoft.com/pl-pl/cpp/standard-library/file-system-navigation) and also [C++17 Features In Visual Studio 2017 Version 15.3 Preview](https://blogs.msdn.microsoft.com/vcblog/2017/05/10/c17-features-in-vs-2017-3/).
 * Hopefully by the end of the year VS 2017 will fully implement C++17 (and STL)

# Examples

All the examples can be found on my Github: [**github.com/fenbf/articles/cpp17**](https://github.com/fenbf/articles/tree/master/cpp17).

I've used Visual Studio 2017 Update 2.

## Working with the Path object

The core part of the library is the `path` object. Just pass it a string of the path, and then you have access to lots of useful functions.

For example, let's examine a path:

```cpp
namespace fs = std::experimental::filesystem;

fs::path pathToShow(/* ... */);
cout << "exists() = " << fs::exists(pathToShow) << "\n"
     << "root_name() = " << pathToShow.root_name() << "\n"
     << "root_path() = " << pathToShow.root_path() << "\n"
     << "relative_path() = " << pathToShow.relative_path() << "\n"
     << "parent_path() = " << pathToShow.parent_path() << "\n"
     << "filename() = " << pathToShow.filename() << "\n"
     << "stem() = " << pathToShow.stem() << "\n"
     << "extension() = " << pathToShow.extension() << "\n";
```

Here's an output for a file path like `"C:\Windows\system.ini"`:

    exists() = 1
    root_name() = C:
    root_path() = C:\
    relative_path() = Windows\system.ini
    parent_path() = C:\Windows
    filename() = system.ini
    stem() = system
    extension() = .ini

What's great about the above code?

It's so simple to use! But there's more cool stuff:

For example, if you want to iterate over all the elements of the path just write:

```cpp
int i = 0;    
for (const auto& part : pathToShow)
    cout << "path part: " << i++ << " = " << part << "\n";
```

The output:

    path part: 0 = C:
    path part: 1 = \
    path part: 2 = Windows
    path part: 3 = system.ini

We have several things here:

* the path object is implicitly convertible to `std::wstring` or `std::string`. So you can just pass a path object into any of the file stream functions.
* you can initialize it from a string, const char*, etc. Also, there's support for `string_view`, so if you have that object around there's no need to convert it to `string` before passing to `path`. [PDF: WG21 P0392](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0392r0.pdf)
* `path` has `begin()` and `end()` (so it's a kind of a collection!) that allows iterating over every part.

What about composing a path?

We have two options: using append or operator `/=`, or operator `+=`.

* [append](http://en.cppreference.com/w/cpp/experimental/fs/path/append) - adds a path with a directory separator.
* [concat](http://en.cppreference.com/w/cpp/experimental/fs/path/concat) - only adds the 'string' without any separator.

For example:

```
fs::path p1("C:\\temp");
p1 /= "user";
p1 /= "data";
cout << p1 << "\n";

fs::path p2("C:\\temp\\");
p2 += "user";
p2 += "data";
cout << p2 << "\n";
```

output:

    C:\temp\user\data
    C:\temp\userdata

Play with the code:

@[Playing with path object]({"stubs": ["cpp_fs_start.cpp"],"command": "sh ./run.sh cpp_fs_start.cpp"})
	
What can we do more?

Let's find a file size (using [`file_size`](http://en.cppreference.com/w/cpp/filesystem/file_size)):

```
uintmax_t ComputeFileSize(const fs::path& pathToCheck)
{
    if (fs::exists(pathToCheck) &&
        fs::is_regular_file(pathToCheck))
    {
        auto err = std::error_code{};
        auto filesize = fs::file_size(pathToCheck, err);
        if (filesize != static_cast<uintmax_t>(-1))
            return filesize;
    }

    return static_cast<uintmax_t>(-1);
}
```

Or, how to find the last modified time for a file:

```
auto timeEntry = fs::last_write_time(entry);
time_t cftime = chrono::system_clock::to_time_t(timeEntry);
cout << std::asctime(std::localtime(&cftime));
```

Isn't that nice? :)

As an additional information, most of the functions that work on a `path` have two versions:

* One that throws: [`filesystem_error`](http://en.cppreference.com/w/cpp/experimental/fs/filesystem_error)
* Another with `error_code` (system specific)

Let's now take a bit more advanced example: how to traverse the directory tree and show its contents?

## Traversing a path

We can traverse a path using two available iterators:

* `directory_iterator`
* `recursive_directory_iterator` - iterates recursively, but the order of the visited files/dirs is unspecified, each directory entry is visited only once.

In both iterators the directories `.` and `..` are skipped.

Ok... show me the code:

```
void DisplayDirTree(const fs::path& pathToShow, int level)
{
    if (fs::exists(pathToShow) && fs::is_directory(pathToShow))
    {
        auto lead = std::string(level * 3, ' ');
        for (const auto& entry : fs::directory_iterator(pathToShow))
        {
            auto filename = entry.path().filename();
            if (fs::is_directory(entry.status()))
            {
                cout << lead << "[+] " << filename << "\n";
                DisplayDirTree(entry, level + 1);
                cout << "\n";
            }
            else if (fs::is_regular_file(entry.status()))
                DisplayFileInfo(entry, lead, filename);
            else
                cout << lead << " [?]" << filename << "\n";
        }
    }
}
```

The above example uses not a recursive iterator but does the recursion on its own. This is because I'd like to present the files in a nice, tree style order.

We can also start with the root call:

```
void DisplayDirectoryTree(const fs::path& pathToShow)
{
    DisplayDirectoryTree(pathToShow, 0);
}
```

The core part is:

```
for (auto const & entry : fs::directory_iterator(pathToShow))
```

The code iterates over `entries`, each entry contains a path object plus some additional data used during the iteration.

Not bad, right?

You can play with the sample here:

@[Iterating through files]({"stubs": ["cpp_fs_more.cpp"],"command": "sh ./run.sh cpp_fs_more.cpp"})

Of course there's more stuff you can do with the library:

* Create files, move, copy, etc.
* Work on symbolic links, hard links
* Check and set file flags
* Count disk space usage, stats

Today I wanted to give you a general glimpse over the library. As you can see there are more potential topics for the future.

# More resources

You might want to read:

* Chapter 7, "Working with Files and Streams" - of [**Modern C++ Programming Cookbook**](https://www.amazon.com/Modern-Programming-Cookbook-Marius-Bancila/dp/1786465183/ref=as_li_ss_il?_encoding=UTF8&me=&linkCode=li3&tag=bfilipek-20&linkId=cee83ce1630b93e141af8fe77f974c97).
 * examples like: Working with filesystem paths, Creating, copying, and deleting files and directories, Removing content from a file, Checking the properties of an existing file or directory, searching.
* The whole Chapter 10  ""Filesystem" from "[**C++17 STL Cookbook**](http://amzn.to/2utFjzB)"
 * examples: path normalizer, Implementing a grep-like text search tool, Implementing an automatic file renamer, Implementing a disk usage counter, statistics about file types,Implementing a tool that reduces folder size by substituting duplicates with symlinks
* [C++17- std::byte and std::filesystem - ModernesCpp.com](http://www.modernescpp.com/index.php/c-17-more-details-about-the-library)
* [How similar are Boost filesystem and the standard C++ filesystem libraries? - Stack Overflow](https://stackoverflow.com/questions/40899267/how-similar-are-boost-filesystem-and-the-standard-c-filesystem-libraries)

# Summary

I think the filesystem library is an excellent part of the C++ Standard Library. A lot of time I had to use various API to do the same tasks on different platforms. Now, I'll be able to just use one API that will work for probably 99.9% cases.

The feature is based on Boost, so not only a lot of developers are familiar with the code/concepts, but also it's proven to work in many existing projects.

And look at my samples: isn't traversing a directory and working with paths so simple now? I am happy to see that everting can be achieved using `std::` prefix and not some strange API :)

## Call To action

If you want to read more about C++17 just [visit my blog](http://www.bfilipek.com/) and start reading :)