#include <entities\widgets\w_genericscrollbox.h>

#include <entities\widgets\w_textbox.h>
#include <utility\widget.h>
#include <utility\shortcuts.h>

w_genericscrollbox::w_genericscrollbox( point Position, point Size, unit ItemHeight, int ItemPadding )
{
        this->Position = Position;
        this->Size     = Size;
        
        this->ItemHeight   = ItemHeight;
        this->ItemPadding  = ItemPadding;
}

void w_genericscrollbox::PostConstruct()
{
        this->Buffer = this->AddChild( new w_buffer() );
        this->Scrollbar = this->AddChild( new w_scrollbar() );
        this->bAcceptExternalScroll = true;
}

void w_genericscrollbox::OnRefresh( ValidityState_t Reason )
{
        
        const point Position2 = SecondPosition( this->Position, this->Size );
        if( Reason & ValidityState::Resized ) {
                this->Buffer->Position = this->Position;
                this->Buffer->Size     = this->Size;
                this->Buffer->Invalidate( ValidityState::Resized );
                
                this->Scrollbar->Position = point(
                        Position2.x-pixel( w_scrollbar::IdealWidth ),
                        Position.y
                );
                this->Scrollbar->Size = point( w_scrollbar::IdealWidth, this->Size.y );
        }
        
        if( Reason & ValidityState::ParametersUpdated ) {
                this->Scrollbar->Invalidate( ValidityState::ParametersUpdated );
        }
        
        // Update scrollbar.
        
        this->Scrollbar->ScrollLengthSet( this->ScrollLength() );
        this->Scrollbar->ScrollViewzoneSet( this->Size.y.yratio() );
        
        // Prepare.
        
        InvalidateWidgets( this->Items, Reason );
        
        const double VisibleItems = this->VisibleItemsCount();
        const double Offset = this->Scrollbar->ScrollOffsetGet();
        
        const std::pair< std::size_t, std::size_t > Visible = std::make_pair( this->NextItem( Offset-this->ItemHeight.yratio() ), ((std::size_t)VisibleItems)+2 );
        
        // Refill buffer.
        
        this->Buffer->ClearChildren();
        
        for( std::size_t i = Visible.first; i < (Visible.first+Visible.second) && i < this->Items.size(); i++ ) {
                std::shared_ptr<widget> Current = this->Items[i];
                
                this->Buffer->AddChild( Current );
                
                Current->Position = point( this->Position.x + pixel( this->XPadding ), (float)(this->Position.y.yratio()-(this->ItemOffset( i )-Offset)) );
                Current->SetSecondPosition( point(
                        Position2.x-this->XPadding-w_scrollbar::IdealPreviewWidth,
                        Current->Position.y-this->ItemHeight
                ) );
                
                Current->Invalidate( ValidityState::Resized );
        }
        
}

void w_genericscrollbox::OnEvent( std::shared_ptr<widget_event> Event )
{
        we_scrollsetratio* Scroll = dynamic_cast<we_scrollsetratio*>(Event.get());
        if( Scroll ) {
                this->Invalidate( ValidityState::ParametersUpdated );
                Event->Handle();
                return;
        }
        
        we_scrolllines* ScrollLines = dynamic_cast<we_scrolllines*>(Event.get());
        if( ScrollLines ) {
                this->Scrollbar->Offset(
                        -(this->ItemHeight.yratio()+pixel(this->ItemPadding).yratio()) * ScrollLines->Lines
                );
                this->Invalidate( ValidityState::ParametersUpdated );
                Event->Handle();
        }
        
        auto ScrollPages = dynamic_cast<we_scrollpages*>( Event.get() );
        if( ScrollPages ) {
                this->Scrollbar->OffsetByViewzone( -ScrollPages->Pages );
                this->Invalidate( ValidityState::ParametersUpdated );
        }
        
}


std::shared_ptr<widget>& w_genericscrollbox::operator[]( std::size_t Position )
{
        return this->Items[Position];
}

void w_genericscrollbox::AddItem( std::shared_ptr<widget> Item )
{
        if( Item ) {
                this->Items.push_back( Item );
                this->Invalidate( ValidityState::ParametersUpdated );
        }
}

void w_genericscrollbox::ClearItems()
{
        this->Items.clear();
        this->Invalidate( ValidityState::ParametersUpdated );
}

/** @brief Item height as a fraction of total scroll. */
float w_genericscrollbox::ItemHeightRatio()
{
        const float fItemHeight = this->ItemHeight.yratio();
        const float ItemRatio = fItemHeight/this->ScrollLength();
        return ItemRatio;
}

/** @brief Y padding height as a ratio of total scroll. */
float w_genericscrollbox::ItemYPaddingRatio()
{
        const float fItemPadding = pixel(this->ItemPadding).yratio();
        const float PaddingRatio = fItemPadding/this->ScrollLength();
        return PaddingRatio;
}


/** @brief Calculate how many items can fit in the viewzone. */
float w_genericscrollbox::VisibleItemsCount()
{
        
        const fpoint fSize = this->Size;
        const float fItemHeight = this->ItemHeight.yratio();
        const float fItemPadding = pixel(this->ItemPadding).yratio();
        
        float RemainingSize = fSize.y - fItemPadding;
        float ItemCount = 0.0f;
        
        while( RemainingSize > 0.0f ) {
                RemainingSize -= fItemHeight;
                if( RemainingSize >= 0.0f ) {
                        ItemCount += 1.0f;
                } else {
                        ItemCount += (fItemHeight+RemainingSize)/fItemHeight;
                }
                RemainingSize -= fItemPadding;
        }
        
        return ItemCount;
        
}
/** @brief Total length of the scroll on which the items lay. */
float w_genericscrollbox::ScrollLength()
{
        
        const float fItemHeight = this->ItemHeight.yratio();
        const float fItemPadding = pixel(this->ItemPadding).yratio();
        
        return fItemPadding + ( (float)this->Items.size() * ( fItemHeight + fItemPadding ) );
}

/** @brief First item at, or following, the offset.
 *  @note  Returns the last item in case the offset is higher than possible. */
std::size_t w_genericscrollbox::NextItem( float Offset )
{
        
        const float fItemHeight = this->ItemHeight.yratio();
        const float fItemPadding = pixel(this->ItemPadding).yratio();
        
        float WalkOffset = fItemPadding;
        std::size_t Item = 0;
        
        while( WalkOffset < Offset && Item < this->Items.size() ) {
                Item++;
                WalkOffset += fItemHeight + fItemPadding;
        }
        
        return Item;
        
}

/** @brief Y offset of the item relative to the beginning of the scroll on which the items lay. */
float w_genericscrollbox::ItemOffset( std::size_t Item )
{
        
        const float fItemHeight = this->ItemHeight.yratio();
        const float fItemPadding = pixel(this->ItemPadding).yratio();
        
        return fItemPadding + ( (float)Item * ( fItemHeight + fItemPadding ) );
}

/** @brief Set scrollbox background color. */
void w_genericscrollbox::SetBackgroundColor( const rgba& Color )
{
        this->Buffer->BackgroundColor = Color;
        this->Buffer->Invalidate( ValidityState::ParametersUpdated );
}

