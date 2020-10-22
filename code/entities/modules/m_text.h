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
        
        virtual void TextSet( const std::string& ) {}
        virtual void TextClear(void) {};
        virtual void TextUpdated() {};
        virtual std::string  TextGet(void) { return ""; }
        virtual std::string* TextGetRef(void) { printf( "Called master!\n" ); return nullptr; }
        virtual std::string  OriginalTextGet(void) { return this->TextGet(); }
        virtual std::string* OriginalTextGetRef(void) { return this->TextGetRef(); }
        virtual std::size_t  TextGetSize() { return this->TextGet().size(); }
        virtual std::size_t  OriginalTextGetSize() { return this->OriginalTextGet().size(); }
        virtual std::size_t  TextGetLineCount() { return CountCharacter( this->TextGet(), '\n' )+1; }
};

struct m_textlines : public m_text
{
        virtual std::string LineGet() const { return ""; }
        virtual std::vector<std::string> LinesGet() const { return {}; }
        virtual std::vector<split_line> LineMapGet() const { return {}; }
};

#endif
