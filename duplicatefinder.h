/*
    Copyright Joseph Miller (c) 2014-2016
*/
/**
    \file duplicatefinder.h
    \brief This file holds the declaration for the DuplicateFinder class.
    \author Joseph Miller
**/

#ifndef DUPLICATEFINDER_H_INCLUDED
#define DUPLICATEFINDER_H_INCLUDED ///< Defined for multi-inclusion protection.

#include <list>
#include <string>
#include <map>

/**
    \class DuplicateFinder duplicatefinder.h "duplicatefinder.h"
    \brief This class is used to check a directory (and possibly subdirectories) for
        duplicate files by doing a byte by byte comparison.
**/
class DuplicateFinder
{
	public:
	    // Retrieves the number of duplicates found.
		unsigned int GetNumberOfDuplicates() const;
		// Retrieves the beginning found element.
		std::list<std::wstring>::const_iterator Begin() const;
		// Retrieves the end found element.
		std::list<std::wstring>::const_iterator End() const;
		// Find the duplicates in a search path, optionally searching recursively as well.
		unsigned int FindDuplicates( const std::wstring & searchPath,
                                     const bool & includeSubDirectories = false );

	private:
		unsigned int numberOfDuplicates{0}; ///< The number of duplicates found.
		std::list<std::wstring> duplicates; ///< The list of duplicates.
		/// The duplicate comparison function
		unsigned int DuplicateComparison( const std::multimap<unsigned int, std::wstring> & files );
};

#endif // DUPLICATEFINDER_H_INCLUDED
