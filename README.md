# Snow-Common

Snow-Common, or libsnow-common, is essentially a collection of commonly-used
classes, functions, and other code for the Snow engine. It covers maths, some
data handling (the Sparse C++ implementation), and other common utilities and
structures that have utility across multiple projects (including sbfgen, Snow,
and a few other projects).

## Building

To build libsnow-common, you'll need [premake] along with a compiler and C++
standard library that supports at least C++11 (Clang and libc++, for example).
Currently, this should include Clang and libc++ and an up to date GCC and most
of libstdc++. Visual Studio support is untested, though its standard library
implementation should work even if the compiler isn't quite there.

When all that's done, just do the following:

[premake]: http://industriousone.com/premake

    $ premake4 gmake
    $ make

That should build the static library. The default configuration is
`release-static`, though there is also `release-shared`, `debug-static`, and
`debug-shared`. These're for what they sound like they're for.

If you want to install the library, you can do the following:

    $ premake4 install

Optionally, you can also specify the prefix (it defaults to `/usr/local`) using
the `--prefix=` argument. This must come before `install` on the command line.
This method of installation is not currently suited to anything non-UNIX-esque.
So, basically, just the Windows users are left out. Seems like a mostly fair
deal at the moment since Visual Studio's not on the target list.

There is currently no `uninstall` action, mostly because I don't like the idea
of letting a script delete things, potentially with super-user access. So, if
you feel the need to remove the library, it'll just be libsnow-common.* under
your prefix/lib directory and the entire prefix/include/snow directory as well
as snow-common.hh from prefix/include. It tries to keep to itself, so you
probably won't blow a foot off.

## Documentation

Documentation can be found over on [The Codex], my personal TiddlyWiki. It's a
work in progress, but likely to contain more thorough documentation when needed.
In other cases, there may be no documentation.

[The Codex]: http://spifftastic.net/codex/

## License

libsnow-common is distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
<http://www.boost.org/LICENSE_1_0.txt>.
