#ifndef __UNOTU_UTILITY_CLASSES_CARD_H_
#define __UNOTU_UTILITY_CLASSES_CARD_H_

#include <string>

struct card
{
        card() = default;
        card( const std::string&, const std::string& = "" );
        std::string Title;
        std::string Content;
};

struct indexed_card : public card
{
        indexed_card() = default;
        indexed_card( const std::string&, const std::string& = "" );
        indexed_card( int, const std::string& = "", const std::string& = "" );
        int ID = -1;
};

#endif
