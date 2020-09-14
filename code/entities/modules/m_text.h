#ifndef __UNOTU_ENTITIES_MODULES_M_TEXT_H_
#define __UNOTU_ENTITIES_MODULES_M_TEXT_H_

#include <entities\modules\module.h>

#include <string>
#include <vector>

#include <utility\text.h>

struct m_text
{
        m_text() = default;
        virtual ~m_text() {};
        
        virtual void SetText( const std::string& ) {}
        virtual void ClearText(void) {};
        virtual void TextUpdated() {};
        virtual std::string GetText(void) { return ""; }
        virtual std::string* GetTextRef(void) { printf( "Called master!\n" ); return nullptr; }
        virtual std::string GetOriginalText(void) { return this->GetText(); }
        virtual std::string* GetOriginalTextRef(void) { return this->GetTextRef(); }
        
};

struct m_textlines : public m_text
{
        virtual std::string GetLine() { return ""; }
        virtual std::vector<std::string> GetLines() { return {}; }
        virtual std::vector<split_line> GetLineMap() { return {}; }
};

#endif
