/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2022 Matthew McCall
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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