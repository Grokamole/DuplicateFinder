/*
    Copyright Joseph Miller (c) 2014
*/
#include <iostream>
#include <string>
#include <cctype>
#include <limits>
#include <cstdlib>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "duplicatefinder.h"

static const std::wstring DF_GET_VERSION_SHORT = L"-v";
static const std::wstring DF_GET_VERSION_LONG = L"-version";
static const std::wstring DF_VERSION = L"DuplicateFinder Version 1.0.2";
static const std::wstring DF_CREATOR = L"Joseph P. Miller";
static const std::wstring DF_SEARCH_RECURSIVELY = L"-r";
static const std::wstring DF_CHANGE_OUTPUT_FILENAME = L"-o";
static const std::wstring DF_GET_HELP_SHORT = L"-h";
static const std::wstring DF_GET_HELP_QUESTIONMARK = L"-?";
static const std::wstring DF_GET_HELP_LONG = L"-help";
static const std::wstring DF_OUTPUT_TO_SCREEN = L"-s";
static const std::wstring DF_HELP_TEXT = L"--------------------\n"
								  "DuplicateFinder Help\n"
								  "--------------------\n"
								  "Description: This program iterates through one or more directories and"
								  "returns a list of files that are byte-to-byte equal.\n\n"
								  "Usage: DuplicateFinder -h -? -help -v -version -r -o filename.ext -s\n"
								  "\"-h | -? | -help\" brings up this help text\n"
								  "\"-v | -version\" states the version number of this application\n"
								  "\"-r\" recursively searches subdirectories for duplicates\n"
								  "\"-o filename.ext\" outputs the results to a specified filename (default is results.txt)\n"
								  "\"-s\" outputs the results to the screen instead of a file\n";
static const int MINIMUM_NUM_ARGUMENTS = 1;
static const int NEXT_ARGUMENT = 1;

int main( int argc, char ** argv )
{
    bool searchRecursively = false;
    std::wstring outputFilename = L"results.txt";
    bool outputToScreen = false;

	//if no arguments are specified
	if( argc <= MINIMUM_NUM_ARGUMENTS )
	{
		std::wcout << L"Options not specified.  Searching this directory only.  Output will go to"
                      " results.txt.";
	}

	//set options
	for( int args = MINIMUM_NUM_ARGUMENTS; args < argc; ++args )
	{
		std::string sArg = argv[args];
		std::wstring arg = std::wstring( sArg.begin(), sArg.end() );

		//check for recursiveness
		if( !arg.compare( DF_SEARCH_RECURSIVELY ) )
		{
			std::wcout << L"-Searching recursively" << std::endl;
			searchRecursively = true;
		}
		//check for output filename modification
		else if( !arg.compare( DF_CHANGE_OUTPUT_FILENAME ) )
		{
			//if there is a next argument(should be a filename)
			if( ( args + NEXT_ARGUMENT ) < argc )
			{
				//check to see if the next argument is a filename
				std::string sArgNext = argv[args + NEXT_ARGUMENT];
				outputFilename = std::wstring( sArgNext.begin(), sArgNext.end() );
				//check that the filename is valid
				if( !boost::filesystem::portable_name( sArgNext ) )
				{
					std::wcout << L"Error: " << outputFilename << L" is not a valid filename."
                               << std::endl;
					return EXIT_SUCCESS;
				}
				//check to see if the filename exists
				if( boost::filesystem::exists( outputFilename ) )
				{
					//ask the user whether they want to overwrite or cancel
					bool userChose=false;
					while( !userChose )
					{
						std::wcout << L"Filename: " << outputFilename << L" exists." << std::endl;
						std::wcout << L"(O)verwrite or (C)ancel?: ";
						wchar_t userInput = 0;

						std::wcin.get( userInput );
						switch( tolower( userInput ) )
						{
							case 'o':
							{
								userChose = true;
							} break;
							case 'c':
							{
								std::wcout << std::endl << L"Canceling execution." << std::endl;
								return 0;
							} break;
							default:
							{
								std::wcout << std::endl << L"Error: Incorrect Option." << std::endl;
							} break;
						}
					}
				}
				std::wcout << L"-outputting data to " << outputFilename << std::endl;
				//skip the next argument since we have already used it
				++args;
			}
			else
			{
				std::wcout << L"Error: filename not specified" << std::endl;
				return EXIT_SUCCESS;
			}
		}
		//show the help screen
		else if( !arg.compare( DF_GET_HELP_LONG ) || !arg.compare( DF_GET_HELP_QUESTIONMARK ) ||
                 !arg.compare( DF_GET_HELP_SHORT ) )
		{
			std::wcout << DF_HELP_TEXT << std::endl;
			return EXIT_SUCCESS;
		}
		//output to the screen instead of a file
		else if( !arg.compare( DF_OUTPUT_TO_SCREEN ) )
		{
			std::wcout << L"-Outputting to screen" << std::endl;
			outputToScreen = true;
		}
		//show the version
		else if( !arg.compare( DF_GET_VERSION_LONG ) || !arg.compare( DF_GET_VERSION_SHORT ) )
		{
			std::wcout << DF_VERSION << std::endl << L"Created by: " << DF_CREATOR << std::endl;
			std::wcout << L"Using the MIT License" << std::endl;
			return EXIT_SUCCESS;
		}
	}

	//save the read buffer
	std::wstreambuf *buf = std::wcout.rdbuf();
	//open the file for writing if -s was not specified
	boost::filesystem::wofstream ofs;
	if( !outputToScreen )
	{
		ofs.open( outputFilename );
		if( ofs.is_open() )
		{
			std::wcout.rdbuf( ofs.rdbuf() );
		}
		else
		{
			std::wcout << L"Error opening filename: " << outputFilename << std::endl;
			return EXIT_SUCCESS;
		}
	}

	DuplicateFinder df;

	//check for duplicates
	if( df.FindDuplicates( L".", searchRecursively ) )
	{
		std::wcout << df.GetNumberOfDuplicates() << L" duplicates found: " << std::endl;
		//output duplicates to the stream
		for( auto lit = df.Begin(); lit != df.End(); ++lit )
		{
			std::wcout << *lit << std::endl;
		}
	}
	else
	{
		std::wcout << L"No duplicates found!" << std::endl;
	}

	//close the file
	if( ofs.is_open() )
	{
		ofs.close();
	}

	//change back to standard input
	if( !outputToScreen )
	{
		std::wcout.rdbuf( buf );
	}

	return EXIT_SUCCESS;
}
