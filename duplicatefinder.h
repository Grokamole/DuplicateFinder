#ifndef DUPLICATEFINDER_H_INCLUDED
#define DUPLICATEFINDER_H_INCLUDED

#include <list>
#include <map>
#include <fstream>
#include <string>

class DuplicateFinder
{
	private:
		unsigned int numberOfDuplicates{0};
		std::list<std::wstring> duplicates;
	public:
		unsigned int GetNumberOfDuplicates()
		{
			return numberOfDuplicates;
		}
		std::list<std::wstring>::const_iterator Begin()
		{
			return duplicates.cbegin();
		}
		std::list<std::wstring>::const_iterator End()
		{
			return duplicates.cend();
		}
		unsigned int FindDuplicates(std::wstring searchPath, bool includeSubDirectories=false);
};

#endif // DUPLICATEFINDER_H_INCLUDED
