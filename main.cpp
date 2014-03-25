#include <iostream>
#include <string>
#include <cctype>
#include <limits>
#include "duplicatefinder.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

const std::wstring strGetVersionShort = L"-v";
const std::wstring strGetVersionLong = L"-version";
const std::wstring strVersion = L"DuplicateFinder Version 1.0.2";

const std::wstring strCreator = L"Joseph P. Miller";

const std::wstring strSearchRecursively = L"-r";
bool searchRecursively = false;

const std::wstring strChangeOutputFilename = L"-o";
std::wstring outputFilename = L"results.txt";

const std::wstring strGetHelpShort = L"-h";
const std::wstring strGetHelpQuestion = L"-?";
const std::wstring strGetHelpLong = L"-help";
const std::wstring strHelpText = L"--------------------\n"
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

const std::wstring strOutputToScreen = L"-s";
bool outputToScreen = false;

int main(int argc, char **argv)
{
	//if no arguments are specified
	if(argc <= 1)
	{
		std::wcout << L"Options not specified.  Searching this directory only.  Output will go to results.txt.";
	}
	
	//set options
	for(int args=1;args < argc;++args)
	{
		std::string sArg = argv[args];
		std::wstring arg = std::wstring(sArg.begin(), sArg.end());

		//check for recursiveness
		if(!arg.compare(strSearchRecursively))
		{
			std::wcout << L"-Searching recursively" << std::endl;
			searchRecursively = true;
		}
		//check for output filename modification
		else if(!arg.compare(strChangeOutputFilename))
		{
			//if there is a next argument(should be a filename)
			if((args+1) < argc)
			{
				//check to see if the next argument is a filename
				std::string sArgNext = argv[args+1];
				outputFilename = std::wstring(sArgNext.begin(), sArgNext.end());
				//check that the filename is valid
				if(!boost::filesystem::portable_name(sArgNext))
				{
					std::wcout << L"Error: " << outputFilename << L" is not a valid filename." << std::endl;
					return -2;
				}
				//check to see if the filename exists
				if(boost::filesystem::exists(outputFilename))
				{
					//ask the user whether they want to overwrite or cancel
					bool userChose=false;
					while(!userChose)
					{
						std::wcout << L"Filename: " << outputFilename << L" exists." << std::endl;
						std::wcout << L"(O)verwrite or (C)ancel?: ";
						wchar_t userInput=0;
						
						std::wcin.get(userInput);
						switch(tolower(userInput))
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
				return 0;
			}
		}
		//show the help screen
		else if(!arg.compare(strGetHelpLong) || !arg.compare(strGetHelpQuestion) || !arg.compare(strGetHelpShort))
		{
			std::wcout << strHelpText << std::endl;
			return 0;
		}
		//output to the screen instead of a file
		else if(!arg.compare(strOutputToScreen))
		{
			std::wcout << L"-Outputting to screen" << std::endl;
			outputToScreen = true;
		}
		//show the version
		else if(!arg.compare(strGetVersionLong) || !arg.compare(strGetVersionShort))
		{
			std::wcout << strVersion << std::endl << L"Created by: " << strCreator << std::endl;
			std::wcout << L"Using the MIT License" << std::endl;
			return 0;
		}
	}
	
	//save the read buffer
	std::wstreambuf *buf = std::wcout.rdbuf();
	//open the file for writing if -s was not specified
	boost::filesystem::wofstream ofs;
	if(!outputToScreen)
	{
		ofs.open(outputFilename);
		if(ofs.is_open())
		{
			std::wcout.rdbuf(ofs.rdbuf());
		}
		else
		{
			std::wcout << L"Error opening filename: " << outputFilename << std::endl;
			return -1;
		}
	}
	
	DuplicateFinder df;

	//check for duplicates
	if(df.FindDuplicates(L".", searchRecursively))
	{
		std::wcout << df.GetNumberOfDuplicates() << L" duplicates found: " << std::endl;
		//output duplicates to the stream
		for(auto lit=df.Begin();lit != df.End();++lit)
		{
			std::wcout << *lit << std::endl;
		}
	}
	else
	{
		std::wcout << L"No duplicates found!" << std::endl;
	}

	//close the file
	if(ofs.is_open())
	{
		ofs.close();
	}
	
	//change back to standard input
	if(!outputToScreen)
	{
		std::wcout.rdbuf(buf);
	}
	
	return 0;
}
