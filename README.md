DuplicateFinder
===============

This program iterates through one or more directories and returns a list of files that are byte-to-byte equal.

Note: Requires the Boost library to function.(created with Boost 1.55.0 in mind)

Usage: DuplicateFinder -h -? -help -v -version -r -o filename.ext -s
"-h | -? | -help" brings up this help text
"-v | -version" states the version number of this application
"-r" recursively searches subdirectories for duplicates
"-o filename.ext" outputs the results to a specified filename (default is results.txt)
"-s" outputs the results to the screen instead of a file

Known Issues as of Version 1.0.2
--------------------------------
*Note: Tested only Windows 7 using mingw
If you don't specify the -o or -s option, this program WILL OVERWRITE "results.txt" in the running directory.
