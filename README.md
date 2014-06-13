# Snow-Common

Snow-Common, or libsnow-common, is essentially a collection of commonly-used
classes, functions, and other code for the Snow engine. It covers maths, some
data handling (the Sparse C++ implementation), and other common utilities and
structures that have utility across multiple projects (including sbfgen, Snow,
and a few other projects).

## Building

To build libsnow-common, you'll need [premake] and a version of OpenSSL,
preferably >= 1.0.0. This means the default OS X OpenSSL is sort of off-limits
due to the entire thing being marked as deprecated. You'll also need a C++11
compatible compiler and standard library (Clang and libc++, for example).

When all that's make, just do the following:

[premake]: http://industriousone.com/premake

    $ premake4 gmake
    $ make

That should build the static library. The default configuration is
`release-static`, though there is also `release-shared`, `debug-static`, and
`debug-shared`. These're for what they sound like they're for.

If you want to install the library, you can do the following:

    $ premake4 install

Optionally, you can also specify the prefix (it defaults to `/usr/local`) using
the `--prefix=/foo/bar/baz` argument. This must come before `install` on the
command line. This method of installation is not currently suited to anything
non-UNIX-esque. So, basically, just the Windows users are left out. Seems like
a fair deal.

There is currently no `uninstall` action, mostly because I don't like the idea
of letting a script delete things, potentially with super-user access. So, if
you feel the need to remove the library, it'll just be libsnow-common.* under
your prefix/lib directory and the entire prefix/include/snow directory as well
as snow-common.hh from prefix/include. It tries to keep to itself, so you
probably won't blow a foot off.

## Dependencies

libsnow-common currently depends only on OpenSSL for its sha256 functions.

## Documentation

Documentation can be found over on [The Codex], my personal TiddlyWiki. It's a
work in progress, but likely to contain more thorough documentation when needed.
In other cases, there may be no documentation.

[The Codex]: http://spifftastic.net/codex/

## License

Except where noted otherwise, all source code in Snow-Common is public
domain. You are free to do with it as you like on the condition that you are not
a dick. I cannot enforce this condition, so it's meaningless and still public
domain. Be excellent.

See COPYING for further details.
