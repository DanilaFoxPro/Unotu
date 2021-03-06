#include <unotu\systems\systems.h>

//::STD.
#include <stdio.h>
#include <system_error> // For throwing exceptions around.

//::SQLite.
#include <unotu\utility\deps\sqlite_includes.h>

//::Mine.
#include <unotu\entities\ent_application.h>

#include <unotu\utility\packed_data.h>
#include <unotu\utility\tab_presets.h>

namespace unotu {

namespace systems
{
        
        void setup_unotui();
        
        void postsetup_database();
        
        void entry( uint8_t Executable )
        {
                TheApplication.Executable = Executable;
                setup_unotui();
        }
        
        void setup_unotui()
        {
                unotui::RegisterInterface( std::unique_ptr<unotui::ent_interface>( new unotui_bridge() ) );
                
                //:: Load window icon.
                
                unotui::TheWindowManager.IconLayers.reserve(3);
                unotui::TheWindowManager.IconLayers.push_back( unotui::LoadGLFWImage( DATA_UNOTU64_PNG, DATA_UNOTU64_PNG_SIZE ) );
                unotui::TheWindowManager.IconLayers.push_back( unotui::LoadGLFWImage( DATA_UNOTU32_PNG, DATA_UNOTU32_PNG_SIZE ) );
                unotui::TheWindowManager.IconLayers.push_back( unotui::LoadGLFWImage( DATA_UNOTU16_PNG, DATA_UNOTU16_PNG_SIZE ) );
                
                //:: Launch UnotUI main loop.
                
                unotui::RunUnotUI();
        }
        
        void postsetup_database()
        {
                
                printf("Opening the database\n{\n");
                
                try {
                        
                        TheApplication.Database = std::unique_ptr<SQLite::Database>(
                                new SQLite::Database( "unotu.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE )
                        );
                        
                        SQLite::Database& Database = *TheApplication.Database.get();
                        
                        if( !Database.tableExists( "cards" ) ) {
                                printf( "  'cards' table doesn't exist!\n" );
                                Database.exec( "CREATE TABLE cards (ID INTEGER PRIMARY KEY, Title TEXT, Content TEXT)" );
                                
                                std::unique_ptr<SQLite::Transaction> Transaction;
                                
                                SQLite::Statement Query( Database, "INSERT INTO cards ( Title, Content ) VALUES ( :Title, :Content ) " );
                                
                                const std::size_t TotalCards = 10;
                                const std::size_t ShowEvery = TotalCards/100+1;
                                
                                const std::size_t CommitEvery = 1000;
                                
                                for( std::size_t i = 0; i < TotalCards; i++ ) {
                                        
                                        if( i%ShowEvery == 0 ) {
                                                printf( "\r  Creating test cards: %i/%i..", (int)i+1, (int)TotalCards );
                                        }
                                        
                                        const std::string Name = "Test Card #" + std::to_string( i );
                                        std::string Content = Name + "\n";
                                        
                                        if( i%3 == 0 ) {
                                                Content += "Fizz";
                                        }
                                        
                                        if( i%5 == 0 ) {
                                                Content += "Buzz";
                                        }
                                        
                                        Query.bind( ":Title", Name );
                                        Query.bind( ":Content", Content );
                                        
                                        Query.exec();
                                        Query.reset();
                                        
                                        if( i%CommitEvery == 0 ) {
                                                if( Transaction ) {
                                                        Transaction->commit();
                                                }
                                                Transaction = std::unique_ptr<SQLite::Transaction>( new SQLite::Transaction(Database) );
                                        }
                                }
                                Transaction->commit();
                                printf( ". Done.\n" );
                        }
                        
                        SQLite::Statement Query( Database, "SELECT COUNT(ID) FROM cards" );
                        if( Query.executeStep() ) {
                                printf( "  Total card count: %i\n", Query.getColumn(0).getInt() );
                        }
                        
                } catch( std::exception& e ) {
                        printf( "  Exception: %s\n", e.what() );
                        printf( "}\n" ); // :)
                        exit(0);
                }
                
                printf( "}\n" );
                
        }
}// namespace system

void unotui_bridge::PostSetup()
{
        systems::postsetup_database();
}

void unotui_bridge::OnNewWindow(unotui::ent_window& Window)
{
        switch( TheApplication.Executable ) {
                case Executable::Main: {
                        Window.SwitchTab( new TabPresets::card_search() );
                        break;
                }
                case Executable::Test: {
                        Window.SwitchTab( new TabPresets::test() );
                        break;
                }
                default: {
                        printf( "Current executable doesn't have a default tab, falling back to CardSearch.\n" );
                        Window.SwitchTab( new TabPresets::card_search() );
                }
        }
}

} // namespace unotu
