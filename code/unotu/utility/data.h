#ifndef __UNOTU_UTILITY_DATA_H_
#define __UNOTU_UTILITY_DATA_H_

#include <vector>

#include <unotu\utility\classes\card.h>

namespace unotu {

std::vector<indexed_card> SearchCards( const std::string& Query );

indexed_card GetCard( const int );
void AddCard( const card& );
bool ReplaceCard( const indexed_card& );
bool RemoveCard( const int );

} // namespace unotu

#endif
