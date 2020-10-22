#ifndef __UNOTU_ENTITIES_MODULES_M_CARET_H_
#define __UNOTU_ENTITIES_MODULES_M_CARET_H_

#include <entities\modules\module.h>
#include <entities\modules\m_text.h>
#include <types.h>

#include <string> // 'std::string::npos', 'std::size_t'.
#include <memory>

using caret_coord = line_coord;

/**
 * @brief Tries to aid in adding caret to things as much as possible.
 */
struct m_caret : public module
{
//:: Options.
        bool bAllowNewlines = false;
        std::size_t TextMaxLength = std::string::npos;

//:: Data.

        bool bTextCaretVisible = true;
        /**
         * @brief Used to force caret to be visible after critical actions.
         * Like focusing on the text field or moving the caret.
         */
        double LastCaretBump = 0.0;
        /** @brief Target which text the caret will modify. */
        std::weak_ptr<m_text> Target;
private:
        static const caret_coord InvalidSelectionPosition = caret_coord(-1);
        
        caret_coord TextCaretPosition = 0;
        caret_coord TextSelectPosition = InvalidSelectionPosition;
public:

//:: Functions.

        void SetCaretTarget( std::weak_ptr<m_text> );

        void BumpCaret();

        void CaretPositionSet( const std::size_t&, const bool& = true );
        void CaretPositionSet( text_coord Position );
        
        caret_coord CaretPositionGet() const;
        std::size_t CaretOffsetGet() const;
        std::size_t CaretCurrentLineIndexGet() const;
        split_line  CaretCurrentLineGet();
        std::size_t CaretCurrentLineGetBeginning() const;
        std::size_t CaretCurrentLineGetEnd() const;
        
        void CaretToLineBeginning();
        void CaretToLineEnd();
        void CaretToBeginning();
        void CaretToEnd();
        void CaretFixupPosition();
        
        caret_coord Normalize( caret_coord Coord );
        
        bool HasCaretSelection() const;
        void VoidCaretSelection();
        void StartSelection(const bool = true);
        caret_coord& FirstCaretSelection();
        caret_coord& SecondCaretSelection();
        
        std::string GetSelectedSubstring();
        void DeleteCaretSelection();
        
        size_t GetCaretStrPos();
        std::vector<split_line> GetCaretLineMap();
        
        bool XMoveCaret( const std::ptrdiff_t, const bool bAllowOverflow = false );
        bool YMoveCaret( const std::ptrdiff_t );

        bool ProcessKeyInput( const int&, const int& );
        bool ProcessCharacterInput( const std::string& );

};

#endif

