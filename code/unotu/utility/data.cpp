#include <unotu/utility/data.h>

#include <unotu/utility/deps/sqlite_includes.h>

#include <unotu/entities/ent_application.h>
#include <unotui/utility/text.h>

namespace unotu {

/** @brief Split search query into parts. Currently does so on spaces.
 */
std::vector<std::string> SplitSearchQuery( const std::string& Query )
{
        std::size_t LastSpace = -1;
        std::vector<std::string> Output;
        
        for( std::size_t i = 0; i <= Query.size(); i++ ) {
                const char Cur = Query[i];
                if( Cur == ' ' || i == Query.size() ) {
                        const std::size_t StrLen = i-(LastSpace+1);
                        if( StrLen > 0 ) {
                                Output.push_back( Query.substr( LastSpace+1, StrLen ) );
                        }
                        LastSpace = i;
                }
        }
        
        
        return Output;
}

/** @brief Check if provided card matches all supplied queries.
 *         Checks title and content.
 */
bool MatchesQueries( const card& Card, const std::vector<std::string>& Queries ) {
        
        using namespace unotui;
        
        for( std::string Part : Queries ) {
                
                const std::string LowPart = ToLower( Part );
                const std::string LowTitle = ToLower( Card.Title );
                const std::string LowContent = ToLower( Card.Content );
                
                if( LowTitle.find( LowPart ) != std::string::npos
                        || LowContent.find( LowPart ) != std::string::npos ) {
                        continue;
                } else {
                        return false;
                }
        }
        return true;
}

/** @brief Find all cards matching the query.
 *  @return Vector of indexed cards.
 */
std::vector<indexed_card> SearchCards( const std::string& Query )
{
        SQLite::Database& Database = *TheApplication.Database.get();
        
        std::vector<std::string> SplitQuery = SplitSearchQuery( Query );
        std::vector<indexed_card> Results;
        
        SQLite::Statement DQuery( Database, "SELECT ID, Title, Content FROM cards ORDER BY Title" );
        if( SplitQuery.size() == 0 ) {
                while( DQuery.executeStep() ) {
                        const indexed_card Current = indexed_card(
                                DQuery.getColumn(0).getInt(),
                                DQuery.getColumn(1).getText(),
                                DQuery.getColumn(2).getText()
                        );
                        Results.push_back( Current );
                }
        } else {
                while( DQuery.executeStep() ) {
                        const indexed_card Current = indexed_card(
                                DQuery.getColumn(0).getInt(),
                                DQuery.getColumn(1).getText(),
                                DQuery.getColumn(2).getText()
                        );
                        if( MatchesQueries( Current, SplitQuery ) ) {
                                Results.push_back( Current );
                        }
                }
        }
        
        return Results;
        
}

/** @brief Get card by index.
 */
indexed_card GetCard( const int Index )
{
        SQLite::Database& Database = *TheApplication.Database.get();

        SQLite::Statement Query( Database, "SELECT Title, Content FROM cards WHERE ( ID = :ID )" );
        
        Query.bind( ":ID", Index );
        
        Query.executeStep();
        
        return indexed_card(
                Index,
                Query.getColumn(0).getText(),
                Query.getColumn(1).getText()
        );
}

void AddCard( const card& Card )
{
        
        try {
                SQLite::Database& Database = *TheApplication.Database.get();
        
                SQLite::Statement Query( Database, "INSERT INTO cards ( Title, Content ) VALUES ( :Title, :Content );" );
                
                Query.bind( ":Title", Card.Title );
                Query.bind( ":Content", Card.Content );
                
                Query.exec();
        } catch( std::exception& Except ) {
                printf( "New card insertion failed, reason: %s\n", Except.what() );
        }
        
}

bool ReplaceCard( const indexed_card& Card )
{
        try {
                SQLite::Database& Database = *TheApplication.Database.get();
        
                SQLite::Statement Query( Database, "UPDATE cards SET Title = :Title, Content = :Content WHERE ( ID = :ID )" );
                
                Query.bind( ":ID", Card.ID );
                Query.bind( ":Title", Card.Title );
                Query.bind( ":Content", Card.Content );
                
                Query.exec();
        } catch( std::exception& Except ) {
                printf( "Replacement of card ID %i failed: %s\n", Card.ID, Except.what() );
                return false;
        }
        return true;
}

bool RemoveCard( const int Index )
{
        try {
                SQLite::Database& Database = *TheApplication.Database.get();
        
                SQLite::Statement Query( Database, "DELETE FROM cards WHERE (ID = :ID)" );
                
                Query.bind( ":ID", Index );
                
                return Query.exec()>0;
        } catch( std::exception& Except ) {
                printf( "Removal of card ID %i failed: %s\n", Index, Except.what() );
                return false;
        }
}

} // namespace unotu
