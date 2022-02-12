//
// Created by Matthew McCall on 2/10/22.
//

#include "york/Log.hpp"
#include "york/XML/Document.hpp"

namespace york::xml {

Document::Document(std::string filepath)
{
    m_parser = new xercesc::XercesDOMParser();
    m_parser->setValidationScheme(xercesc::XercesDOMParser::Val_Always);
    m_parser->setDoNamespaces(true);

    try {
        m_parser->parse(filepath.c_str());
        m_document = m_parser->getDocument();
    } catch (const xercesc::XMLException& e) {
        char* message = xercesc::XMLString::transcode(e.getMessage());
        YORK_CORE_CRITICAL(message);
        xercesc::XMLString::release(&message);
        return;
    }
    catch (const xercesc::DOMException& e) {
        char* message = xercesc::XMLString::transcode(e.getMessage());
        YORK_CORE_CRITICAL(message);
        xercesc::XMLString::release(&message);
        return;
    }

    m_rootTag = Tag(m_document->getDocumentElement());
}

std::optional<std::reference_wrapper<Tag>> Document::getRootTag()
{
    if (m_rootTag.has_value()) {
        return *m_rootTag;
    }

    return std::nullopt;
}

Document::~Document()
{
    delete m_parser;
}

}