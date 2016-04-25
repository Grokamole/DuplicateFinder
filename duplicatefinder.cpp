/*
    Copyright Joseph Miller (c) 2014
*/

#include "duplicatefinder.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iostream>

unsigned int DuplicateFinder::GetNumberOfDuplicates() const
{
    return numberOfDuplicates;
}

std::list<std::wstring>::const_iterator DuplicateFinder::Begin() const
{
    return duplicates.cbegin();
}

std::list<std::wstring>::const_iterator DuplicateFinder::End() const
{
    return duplicates.cend();
}

/*
	FindDuplicates() searches inside a path and optional recursive subdirectories
	for files that are byte-level duplicates.  It returns how many duplicates it has
	found and copies the filenames into its list of wstrings: "duplicates"
*/
unsigned int DuplicateFinder::FindDuplicates( const std::wstring & searchPath,
                                              const bool & includeSubDirectories )
{
	numberOfDuplicates = 0;
	duplicates.clear();

	std::multimap<unsigned int, std::wstring> files; //a map for filenames

	//first, get a list of all files that we need to search through, including their file sizes
	if( includeSubDirectories )
	{
		//search through paths recursively
		boost::filesystem::recursive_directory_iterator rdi( searchPath );
		while( rdi != boost::filesystem::recursive_directory_iterator() )
		{
			//if it's a file, add it to the filename list
			if( boost::filesystem::is_regular_file( *rdi ) )
			{
				files.insert( std::pair<unsigned int, std::wstring>(
                    ( unsigned int ) boost::filesystem::file_size( *rdi ),
                    ( *rdi ).path().wstring() ) );
			}
			++rdi;
		}
	}
	else
	{
		//search only through the current path
		boost::filesystem::directory_iterator di( searchPath );
		while( di != boost::filesystem::directory_iterator() )
		{
			//if it's a file, add it to the filename list
			if( boost::filesystem::is_regular_file( *di ) )
			{
				files.insert( std::pair<unsigned int, std::wstring>(
                    ( unsigned int ) boost::filesystem::file_size( *di ),
                    ( *di ).path().wstring() ) );
			}
			++di;
		}
	}

	if( !files.empty() )
	{
		//check to see if a file's size has other matching files' sizes
		auto mit = files.begin(); //mit is an iterator through the file list
		while( mit != files.end() ) //while searching through the list
		{
			//checking the file immediately after the currently iterated file
			if( ( std::next( mit ) != files.end() ) )
			{
				if( ( std::next( mit ) )->first == mit->first ) // if filesize is same as next's:
				{
				    //open the iterated entry
					boost::filesystem::ifstream original( mit->second, std::ios::binary );
					if( original.is_open() ) //if we opened it...
					{
					    //open the next file to compare
						boost::filesystem::ifstream checking( ( std::next( mit ) )->second,
                                                                std::ios::binary );
						if( checking.is_open() ) //if successfully opened,
						{
							char originalByte; //for original's same byte
							char nextByte; //for next's same byte
							//start by reading check's first byte
							checking.read( &nextByte, sizeof( char ) );
							//start by reading original's first byte
							original.read( &originalByte, sizeof( char ) );

							//while we're not at the end of the files
							while( !checking.eof() || !original.eof() )
							{
								//check to make sure the bytes are not different
								if( nextByte != originalByte )
								{
									break; //move on to the next file
								}

								//read in the next bytes
								checking.read( &nextByte, sizeof( char ) );
								original.read( &originalByte, sizeof( char ) );
							}
							//if we were at the end of both files, we report a duplicate into the
							//duplicate list and add an extra entry to "numberOfDuplicates"
							if( checking.eof() && original.eof() )
							{
								duplicates.push_back( next( mit )->second +
                                                      std::wstring( L" is a duplicate of ") +
                                                      mit->second);
								++numberOfDuplicates;
							}
						}
					}
				}
			}

			++mit;
		}
	}
	return numberOfDuplicates;
}
