#include <entities\chat\ent_chatconnection.h>

#include <utility\shortcuts.h>

const std::size_t OutputLogLineLimit = 1024;

/**
 * @brief Get output log as string.
 * 
 */
std::string ent_chatconnection::GetLog()
{
	std::string Output;
	for( std::size_t i = this->OutputLog.size()-1; i != std::string::npos ; i-- )
	{
		const std::string* Line = this->OutputLog[i].get();
		if( Line == nullptr )
		{
			printf( "%i: Null line.\n", (int)i );
		}
		else
		{
			Output += *(Line);
		}
		const bool bLast = i == 0;
		if( !bLast )
		{
			Output += "\n";
		}
	}
	return Output;
}

/**
 * @brief Shift log lines by offset specified.
 * @param Offset Offset by which to shift the log upwards.
 * @param ClearLines Clear lines that wrap around.
 */
void ent_chatconnection::ShiftLog( std::size_t Offset, bool ClearLines )
{
	
	// Resize and initialize additional lines.
	this->OutputLog.resize( clamp( this->OutputLog.size() + Offset, std::size_t(0), OutputLogLineLimit ) );
	for( std::size_t i = this->OutputLog.size()-1; i >= this->OutputLog.size()-Offset && i != std::string::npos; i-- )
	{
		this->OutputLog[i] = std::unique_ptr<std::string>( new std::string() );
	}
	
	// 'Remove' lines that will be going past log limit.
	if( this->OutputLog.size()+Offset > OutputLogLineLimit )
	{
		if( ClearLines )
		{
			for( std::size_t i = OutputLogLineLimit-1; i > this->OutputLog.size()-1-Offset && i != std::string::npos; i-- )
			{
				if( i > this->OutputLog.size()-1 )
				{
					continue;
				}
				*(this->OutputLog[i].get()) = std::string();
			}
		}
	}
	
	Offset = Offset % (std::size_t)this->OutputLog.size();
	
	if( Offset == 0 )
	{
		return;
	}
	
	// TODO: Consider std::rotate.
	
	// Rotate.
	std::unique_ptr<std::string> TEMP;
	const std::size_t GCD = GreatestCommonDivisor( this->OutputLog.size(), Offset );
	
	for( std::size_t i = 0; i < GCD; i++ )
	{
		
		const std::size_t Initial = this->OutputLog.size()-1-i;
		TEMP.swap( this->OutputLog[Initial] );
		
		std::size_t Current = Initial;
		
		while( true )
		{
			
			std::size_t Next = Current - Offset;
			if( Offset > Current )
			{
				Next += this->OutputLog.size();
			}
			
			// Gone full circle. This iteration is finished.
			if( Next == Initial )
			{
				this->OutputLog[Current].swap( TEMP );
				break;
			}
			
			this->OutputLog[Current].swap( this->OutputLog[Next] );
			Current = Next;
		}
	}
	return;
}

/**
 * @brief Log input, starting on a new line.
 * @param Line Line to log.
 */
void ent_chatconnection::LogLine( const std::string& Line )
{
	this->ShiftLog( 1, false );
	*(this->OutputLog[0].get()) = Line;
    this->NotifyListeners();
}

/**
 * @brief Append input to the last logged line.
 * @param Line What to append.
 */
void ent_chatconnection::LogAppend( const std::string& Line )
{
	if( this->OutputLog.size() > 0 )
	{
		*(this->OutputLog[0].get()) += Line;
	}
	this->NotifyListeners();
}

/**
 * @brief Add widget listener to this chat connection.
 *        Will recieve an event when the log is updated.
 */
void ent_chatconnection::AddListener( const std::weak_ptr<widget>& Listener )
{
    if( !Listener.expired() )
    {
        this->EventListeners.push_back( Listener );
        const std::shared_ptr< widget > Shared = Listener.lock();
        if( Shared )
        {
            Shared->HandleEvent( std::make_shared<we_logupdated>( this ) );
        }
    }
}

/**
 * @brief Notify listeners about log change.
 */
void ent_chatconnection::NotifyListeners()
{
    for( std::size_t i = this->EventListeners.size()-1; i != std::string::npos; i-- )
    {
        const std::shared_ptr< widget > Shared = this->EventListeners[i].lock();
        if( Shared )
        {
            Shared->HandleEvent( std::make_shared<we_logupdated>( this ) );
        }
        else
        {
            this->EventListeners.erase( this->EventListeners.begin()+i );
        }
    }
}
