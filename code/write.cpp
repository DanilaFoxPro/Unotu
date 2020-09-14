#include "write.h"

#include <fstream>

/*namespace file{
	
	bool write(header_index index, std::vector<item> items){
		
		std::fstream file(name, std::fstream::out | std::fstream::binary);
		
		if(!file.is_open())
			return false;
		
		
		file.write((char*)&index, sizeof(index));//
		
		for(item item_ : items)
			file.write((char*)&item_, sizeof(item));//
		
		return true;
		
	}
	
}//namespace file.*/
