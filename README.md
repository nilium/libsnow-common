# Snow-Common

Snow-Common, or libsnow-common, is essentially a collection of commonly-used
classes, functions, and other code for the Snow engine. It covers maths, some
data handling (the Sparse C++ implementation), and other common utilities and
structures that have utility across multiple projects (including sbfgen, Snow,
and a few other projects).

## Building

To build libsnow-common, you'll need [tup] and a version of OpenSSL >= 1.0.0.
This means the default OS X OpenSSL is off-limits. You'll also need libc++,
clang, and so on since this uses C++11 a bit. Once you have those, just run

[tup]: http://gittup.org/tup/

    $ tup upd

That should build the static library and dylib.

Though this only occurs in the tests I don't ship in libsnow-common (because
they're not unit tests, just tests for me to see if they produce the expected
output since full-on unit tests are a pain in the neck), on OS X, building with
tup and specifying the dylib as an input _will_ result in a race condition in
tup or fusefs or something. This is unavoidable, just run `tup upd` again if you
end up adding little bits of test code to the project for whatever reason.

## Documentation

Documentation can be found over on [The Codex], my personal TiddlyWiki. It's a
work in progress, but likely to contain more thorough documentation when needed.
In other cases, there may be no documentation.

[The Codex]: http://spifftastic.net/codex/

## License

Except where noted otherwise, all source code in Snow-Common is public
domain. You are free to do with it as you like on the condition that you are not
a dick. I cannot enforce this condition, so it's meaningless and still public
domain.

See COPYING for further details.

## Exceptions to The Public Domain License

Current exceptions to public domain-ness in Snow-Common are:

* `include/snow/lbind.hh` —
    Released under the Creative Commons 2.5 Attribution-ShareAlike license. Some
    code in this header is based on work by Johannes Schaub, originally included
    in his Stack Overflow answer at <http://stackoverflow.com/a/7858971/457812>.

    For license information, please refer to
    <http://creativecommons.org/licenses/by-sa/2.5/>.
