//
// Created by Matthew McCall on 2/10/22.
//

#ifndef YORK_XML_DOCUMENT_HPP
#define YORK_XML_DOCUMENT_HPP

#include <optional>
#include <functional>

#include <xercesc/parsers/XercesDOMParser.hpp>

#include "Tag.hpp"

namespace york::xml {

class Document {
public:
    Document(std::string filepath);
    std::optional<std::reference_wrapper<Tag>> getRootTag();
    ~Document();

private:
    xercesc::XercesDOMParser* m_parser;
    xercesc::DOMDocument* m_document;

    std::optional<Tag> m_rootTag;
};

}


#endif // YORK_XML_DOCUMENT_HPP
