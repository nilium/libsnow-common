#ifndef __SNOW_COMMON__COMMON__COMPARE_HH__
#define __SNOW_COMMON__COMMON__COMPARE_HH__

#include <snow/config.hh>

BEGIN_SNOW_NS

/*==============================================================================
  score_strings

    Does a scored comparison of two strings. Perfect matches return SIZE_MAX.
    Compares the left-hand string to the right-hand string. Their positions are
    swapped if the left string is longer than the right string.
    Note that this should only really be used for short-ish strings. Though it
    will likely never have an overflow on the score, it is possible with a long
    enough string, but it also won't attempt to do complex scoring. This is very
    far from fuzzy string matching.
==============================================================================*/
size_t score_strings(const string &source, const string &other);

/*==============================================================================
  pattern_match

    Tries to match a string against a pattern using * and ? for wildcards, like
    the shell. Unlike the shell, * can consume as much as it needs in order to
    successfully match the string against the pattern. So, trying to match
    "foo/*bar" against "foo/bar/baz/quux/bar" will result in a match, as it will
    go back to the last * wilcard when it fails to match the string and consume
    the first instance of "bar".

    Keep in mind that a ? can never follow a * and a * following a * is an
    automatic failure (these patterns are invalid).

    Both * and ? must consume at least one character. They are essentially
    equivalent to .+? and . in a regex.
==============================================================================*/
bool pattern_match(const string &pattern, const string &other);

END_SNOW_NS

#endif /* end __SNOW_COMMON__COMMON__COMPARE_HH__ include guard */
