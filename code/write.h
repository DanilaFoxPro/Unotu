#ifndef __WRITE_H_
#define __WRITE_H_

#include "item.h"

#include <vector>

namespace file{
	
	bool write(header_index index, std::vector<item> items);
	
}//namespace file.

#endif
