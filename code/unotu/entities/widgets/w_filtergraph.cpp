#include <unotu\entities\widgets\w_filtergraph.h>

#include <unotu\entities\widgets\w_popup_dialog.h>

namespace unotu
{
        
        w_filtergraph::w_filtergraph()
        {
                
                this->bKeyboardFocusable    = true;
                this->bAcceptExternalScroll = true;
                
                // Testing!
                this->bValidateOnRefresh = false;
                
                std::vector< std::shared_ptr<w_filtergraph::node_type> > Nodez = {
                        std::make_shared<graph_node<filter_node*, void*>>( new fn_out() ),
                        std::make_shared<graph_node<filter_node*, void*>>( new filter_node({0.1, 0.5}) ),
                        std::make_shared<graph_node<filter_node*, void*>>( new filter_node({0.8, 0.6}) ),
                };
                
                for( auto Node : Nodez ) {
                        Graph.NodeAdd( Node );
                        Node->DataGet()->ParentGraphNode = Node;
                }
                
                Nodez[1]->ConnectTo( Nodez[0] );
                Nodez[2]->ConnectTo( Nodez[0] );
                
                Graph.NodeAdd(
                        std::make_shared<graph_node<filter_node*, void*>>( new filter_node(
                                {0.0, 0.0},
                                "I'm the origin!"
                        ) )
                );
                
        }
        
        void w_filtergraph::OnTick()
        {
                if( this->bDragging ) {
                        this->AdjustViewToDrag();
                }
        }
        
        void w_filtergraph::OnRefresh( unotui::ValidityState_t )
        {
                auto Nodes = Graph.NodeGetAll();
                
                gText.Clear();
                gColor.Clear();
                
                gTextOverlay.Clear();
                gColorOverlay.Clear();
                
                const int TextSize = int( w_filtergraph::GraphNodeTextSize / this->Viewzone );
                
                filter_node* const Colliding = CollidingNode( MousePositionInGraphCoordinates() );
                
                for( auto Node : Nodes ) {
                        const filter_node Data = *Node->DataGet();
                        
                        const point RealPosition = this->ToRealPosition( Data.Position );
                        
                        const std::string NodeName = Data.NameGet();
                        const auto NodeNameLines   = Data.NameGetLines();
                        
                        gText.AddText(
                                Data.Name,
                                NodeNameLines,
                                TextSize,
                                RealPosition,
                                Node->DataGet() == Colliding ? unotui::color::green : unotui::color::black
                        );
                        
                        if( Data.ErrorHas() ) {
                                gTextOverlay.AddText(
                                        Data.ErrorGet(),
                                        TextSize,
                                        RealPosition,
                                        unotui::color::red
                                );
                        }
                        
                        const ipoint NodeSizeCharacters = Data.SizeGet();
                        
                        const int&   NodeNameWidth  = NodeSizeCharacters.x;
                        const int&   NodeNameHeight = NodeSizeCharacters.y;
                        
                        const unotui::rectangle Bounds = unotui::rectangle(
                                point(
                                        RealPosition.x,
                                        RealPosition.y
                                ),
                                point(
                                        RealPosition.x + pixel(TextSize*NodeNameWidth/2),
                                        RealPosition.y - pixel(TextSize*NodeNameHeight)
                                )
                        );
                        
                        gColor.AddRectangle( unotui::colored_rectangle (
                                Bounds,
                                unotui::color::light_sky_blue
                        ) );
                        
                        gColor.AddOutline(
                                Bounds,
                                int( -5/this->Viewzone ),
                                unotui::TheTheme.PrimaryBack,
                                unotui::TheTheme.PrimaryLit,
                                unotui::TheTheme.PrimaryLit,
                                unotui::TheTheme.PrimaryBack
                        );
                        
                        gColor.AddOutline(
                                unotui::colored_rectangle(
                                        Bounds,
                                        unotui::TheTheme.Primary
                                ),
                                int( 1/this->Viewzone )
                        );
                        
                        const fpoint NodeCenter = Bounds.Center();
                        
                        // Draw arrows between this node and all connected nodes.
                        const auto ConnectedNodes = Node->ConnectedOutgoingGet();
                        for( auto ConnectedNode : ConnectedNodes ) {
                                const filter_node ConnectedData = *ConnectedNode->DataGet();
                                const point ConnectedRealPosition = this->ToRealPosition( ConnectedData.Position );
                                
                                gColor.AddArrow(
                                        NodeCenter,
                                        ConnectedRealPosition,
                                        6 /this->Viewzone,
                                        16 /this->Viewzone,
                                        32/this->Viewzone,
                                        unotui::color::black
                                );
                                
                        }
                        
                        
                }
                
                // Coordinate debug.
                gText.AddText(
                        std::to_string( ViewOrigin.x ) + ", " + std::to_string( ViewOrigin.y ),
                        32,
                        point(0, 32),
                        unotui::color::green*0.3f
                );
                
                // Draw arrow from graph origin to mouse position.
                gColor.AddArrow( ToRealPosition( dpoint(0) ), unotui::MousePosition(), 10, unotui::color::black );
                
                gText.Update();
                gColor.Update();
                
                gTextOverlay.Update();
                gColorOverlay.Update();
                
        }
        
        void w_filtergraph::OnDraw()
        {
                gColor.Draw();
                gText.Draw();
                
                gColorOverlay.Draw();
                gTextOverlay.Draw();
        }
        
        void w_filtergraph::OnEvent( std::shared_ptr<unotui::widget_event> Event )
        {
                auto ScrollLines = dynamic_cast<unotui::we_scrolllines*>(Event.get());
                
                if( ScrollLines ) {
                        this->ZoomBy( 0.2 * ScrollLines->Lines );
                        Event->Handle();
                }
                
                auto ScrollPages = dynamic_cast<unotui::we_scrollpages*>(Event.get());
                if( ScrollPages ) {
                        this->ZoomBy( 0.5 * ScrollPages->Pages );
                        Event->Handle();
                }
                
        }
        
        void w_filtergraph::OnMousePressed( const int Button )
        {
                
                const bool bShiftPressed = unotui::IsKeyPressed( GLFW_KEY_LEFT_SHIFT ) || unotui::IsKeyPressed( GLFW_KEY_RIGHT_SHIFT );
                
                switch( Button ) {
                        case GLFW_MOUSE_BUTTON_LEFT: {
                                if( bShiftPressed ) {
                                        this->StartDragging();
                                }
                                break;
                        }
                        case GLFW_MOUSE_BUTTON_MIDDLE: {
                                this->StartDragging();
                                break;
                        }
                }
        }
        
        void w_filtergraph::OnMouseReleased( const int Button, const bool )
        {
                switch( Button ) {
                        case GLFW_MOUSE_BUTTON_LEFT: {
                                this->StopDragging();
                                break;
                        }
                        case GLFW_MOUSE_BUTTON_MIDDLE: {
                                this->StopDragging();
                                break;
                        }
                }
        }
        
        void w_filtergraph::OnKeyInput( const int& Action, const int& Key, const int& /*Modifiers*/ )
        {
                
                switch( Key ) {
                        case GLFW_KEY_HOME: {
                                
                                if( Action != GLFW_PRESS )
                                        break;
                                
                                this->ViewOrigin = {0.0, 0.0};
                                this->Invalidate( unotui::ValidityState::ParametersUpdated );
                                
                                break;
                        }
                        case GLFW_KEY_SPACE: {
                                if( Action == GLFW_PRESS ) {
                                        this->StartDragging();
                                } else if( Action == GLFW_RELEASE ) {
                                        this->StopDragging();
                                }
                                break;
                        }
                        case GLFW_KEY_1: {
                                if( Action == GLFW_PRESS ) {
                                        auto Node = std::make_shared<graph_node<filter_node*, void*>>( new fn_boolean_literal() );
                                        Graph.NodeAdd( Node );
                                        Node->DataReferenceGet()->Position = MousePositionInGraphCoordinates();
                                        printf( "Added node at: %s.\n", Node->DataReferenceGet()->Position.String().c_str() );
                                }
                                break;
                        }
                        case GLFW_KEY_ENTER: {
                                if( Action == GLFW_PRESS ) {
                                        this->ExecuteGraph();
                                }
                                break;
                        }
                }
        }
        
        void w_filtergraph::ExecuteGraph()
        {
                printf( "Executing graph...\n" );
                
                fn_out* OutNode = nullptr;
                auto Nodes = Graph.NodeGetAll();
                
                for( auto& Item : Nodes ) {
                        if( Item.get() ) {
                                OutNode = dynamic_cast<fn_out*>( Item.get()->DataGet() );
                                if( OutNode ) {
                                        break;
                                }
                        }
                }
                assert( OutNode, "Graph must contain an 'OUT' node." );
                
                // Clear all errors.
                for( auto& Item : Nodes ) {
                        if( Item.get() ) {
                                const auto Data = Item.get()->DataGet();
                                if( Data ) {
                                        Data->ErrorClear();
                                }
                        }
                }
                
                filter_node_parameter* Result = nullptr;
                try {
                        Result = OutNode->OutputGet();
                } catch( unotu::user_error_exception& Exception ) {
                        // TODO: Remove obsolete error handling code.
                        /*
                        Graph.NodeAdd(
                                std::make_shared<graph_node<filter_node*, void*>>( new filter_node(
                                        Exception.Source->Position + dpoint{0.2, 0.0},
                                        Exception.what()
                                ) )
                        );
                                */
                        Exception.Source->ErrorSet( Exception.what() );
                        return;
                }
                
                fnp_boolean* Boolean = dynamic_cast<fnp_boolean*>(Result);
                assert( Boolean, "'OUT' should always output a boolean." );
                
                printf( "Graph execution result: %s.\n",
                        Boolean->Boolean ?
                        "true" : "false"
                );
        }
        
        point w_filtergraph::ToRealPosition( const dpoint NodePosition )
        {
                // Shift position to be relative to view origin.
                const dpoint ShiftedPosition = NodePosition - ViewOrigin;
                const dpoint Ratio = GraphCoordinatesConvertionRatio();
                
                const unotui::ent_window TheWindow = unotui::TheWindowManager.Cur();
                const dpoint WindowPosition        = point( TheWindow.PositionX, TheWindow.PositionY );
                
                // Divide by ratio and viewzone to get real coordinates.
                // Subtracting window position to tie graph to screen coordinates.
                const point RealPosition = point( ShiftedPosition / Ratio / this->Viewzone - WindowPosition );
                
                return RealPosition;
        }
        
        dpoint w_filtergraph::ToImaginaryPosition( const point Position )
        {
                const dpoint Ratio = GraphCoordinatesConvertionRatio();
                
                const unotui::ent_window TheWindow = unotui::TheWindowManager.Cur();
                const dpoint WindowPosition        = point( TheWindow.PositionX, TheWindow.PositionY );
                
                // Multiply by ratio and viewzone to get imaginary position.
                // Adding window position to tie graph to screen coordinates.
                const dpoint ScaledPosition = ((dpoint)Position+WindowPosition) * Ratio * this->Viewzone;
                
                return ScaledPosition + ViewOrigin;
        }
        
        dpoint w_filtergraph::MousePositionInGraphCoordinates()
        {
                return ToImaginaryPosition( unotui::MousePosition() );
        }
        
        /** @brief Returns first node that collides with a point specified, if any. */
        filter_node* w_filtergraph::CollidingNode( const dpoint GraphPosition )
        {
                
                const auto Nodes = Graph.NodeGetAll();
                
                for( const auto Node : Nodes ) {
                        auto Data = Node->DataGet();
                        
                        const ipoint NodeSize = Data->SizeGet();
                        
                        // TODO: A non-hacky way to convert node size in characters to size in
                        //       graph coordinates.
                        
                        const double TextSize = (double)w_filtergraph::GraphNodeTextSize;
                        
                        // This code employs inversion, 'NodeSize.x * 1.0/TextSize' ->
                        // 'NodeSize.x / TextSize', 'NodeSize.y * 2.0/TextSize' ->
                        // 'NodeSize.y / (TextSize*0.5)'.
                        //
                        const dpoint Position1 = Data->Position;
                        const dpoint Position2 = dpoint(
                                Position1.x + NodeSize.x / TextSize,
                                Position1.y - NodeSize.y / (TextSize*0.5)
                        );
                        
                        if(
                                   GraphPosition.x > Position1.x
                                && GraphPosition.y < Position1.y
                                && GraphPosition.x < Position2.x
                                && GraphPosition.y > Position2.y
                        ) {
                                return Data;
                        }
                        
                }
                
                return nullptr;
        }
        
        void w_filtergraph::StartDragging()
        {
                this->bDragging = true;
                this->DragOrigin = MousePositionInGraphCoordinates();
                this->OldOrigin = this->ViewOrigin;
        }
        
        void w_filtergraph::StopDragging()
        {
                if( this->bDragging ) {
                        this->bDragging  = false;
                        this->AdjustViewToDrag();
                }
        }
        
        void w_filtergraph::AdjustViewToDrag()
        {
                this->ViewOrigin = this->OldOrigin - this->DragOffset();
                this->Invalidate( unotui::ValidityState::ParametersUpdated );
        }
        
        dpoint w_filtergraph::DragOffset()
        {
                const dpoint MousePosition = MousePositionInGraphCoordinates();
                /* Eeeeeeh.. Well. Some bullshit vector maths here. OriginDifference
                 * accounts for the difference between origin before the drag, and
                 * origin after the drag. This is needed because when you adjust the
                 * graph canvas according to mouse drag -- you move the mouse cursor
                 * relative to ViewOrigin. This in turn means that if you don't
                 * add the OriginDifference -- the cursor would 'move' further and
                 * futher as the time progresses thus creating velocity. I don't
                 * want any velocity here, so I subtract OriginDifference, thus
                 * stabilizing this mess.
                 */
                return MousePosition - this->DragOrigin - this->OriginDifference();
        }
        
        dpoint w_filtergraph::OriginDifference()
        {
                return this->ViewOrigin - this->OldOrigin;
        }
        
        void w_filtergraph::ZoomBy( double Amount )
        {
                const double NewViewzone = unotui::clamp( this->Viewzone - Amount, 0.2, 20.0 );
                
                // Somehow the dragging code also compensates for zoom offset.
                if( !bDragging ) {
                        this->StartDragging();
                        this->Viewzone = NewViewzone;
                        this->StopDragging();
                } else {
                        this->Viewzone = NewViewzone;
                }
                
                this->Invalidate( unotui::ValidityState::ParametersUpdated );
        }
        
        //:: Internal.
        
        /** @brief Returns how many graph units the window would fit, on each axis.
         *  @note  Multiply with window coordinates to get graph coordinates.
         *         Divide graph coordinates by this to get window coordinates.
         */
        inline dpoint w_filtergraph::GraphCoordinatesConvertionRatio()
        {
                const point  WindowSize       = point(1.0f);
                const dpoint WindowSizePixels = dpoint(
                        WindowSize.xpixels(),
                        WindowSize.ypixels()
                );
                return WindowSizePixels/w_filtergraph::PixelsPerGraphUnit;
        }
        
} // namespace unotu
