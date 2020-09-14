#ifndef __ITEM_H_

#include <string>
#include <vector>

struct header_index{
	//TODO: Remove this later.
};

typedef int tag; //TODO: Replace with propper definition;

struct item{
	
	std::string name;
	std::string description;
	std::vector<tag> tags;
	
};

#define __ITEM_H_
#endif
