/*
    Copyright Joseph Miller (c) 2014
*/

#ifndef DUPLICATEFINDER_H_INCLUDED
#define DUPLICATEFINDER_H_INCLUDED

#include <list>
#include <map>
#include <fstream>
#include <string>

class DuplicateFinder
{
	public:
		unsigned int GetNumberOfDuplicates() const;
		std::list<std::wstring>::const_iterator Begin() const;
		std::list<std::wstring>::const_iterator End() const;
		unsigned int FindDuplicates( const std::wstring & searchPath,
                                     const bool & includeSubDirectories = false );

	private:
		unsigned int numberOfDuplicates{0};
		std::list<std::wstring> duplicates;

};

#endif // DUPLICATEFINDER_H_INCLUDED
