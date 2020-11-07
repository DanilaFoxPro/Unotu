#include <unotu\utility\classes\card.h>

namespace unotu {

card::card( const std::string& Title, const std::string& Content )
{
        this->Title = Title;
        this->Content = Content;
}

indexed_card::indexed_card( const std::string& Title, const std::string& Content )
{
        this->Title = Title;
        this->Content = Content;
}

indexed_card::indexed_card( int ID, const std::string& Title, const std::string& Content )
{
        this->ID = ID;
        this->Title = Title;
        this->Content = Content;
}

} // namespace unotu
