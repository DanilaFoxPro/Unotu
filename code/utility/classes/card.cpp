#include <utility\classes\card.h>

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

