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

#include "york/XML/Tag.hpp"

namespace york::xml {

Tag::Tag(std::string& name, std::string& value) : m_tagName(name), m_value(value)
{
}

std::string Tag::getValue()
{
    return m_value;
}

std::string Tag::getName()
{
    return m_tagName;
}

std::vector<Tag> Tag::getChildTags()
{
    updateChildren();

    return m_childTags;
}

std::vector<Tag>::iterator Tag::begin()
{
    updateChildren();

    return m_childTags.begin();
}

std::vector<Tag>::iterator Tag::end()
{
    updateChildren();

    return m_childTags.end();
}

Tag::Tag(xercesc::DOMNode* node)
{
    if (node->getNodeType() != xercesc::DOMNode::NodeType::ELEMENT_NODE) return;

    m_element = dynamic_cast<xercesc::DOMElement*>(node);
    m_tagName = xercesc::XMLString::transcode(m_element->getTagName());

    xercesc::DOMNodeList* childNodes;
    xercesc::DOMNode* iNode;

    childNodes = m_element->getChildNodes();

    for (unsigned i = 0; i < childNodes->getLength(); i++) {
        iNode = childNodes->item(i);

        if (iNode->getNodeType() == xercesc::DOMNode::TEXT_NODE) {
            m_value += xercesc::XMLString::transcode(iNode->getNodeValue());
        }
    }
}

void Tag::updateChildren()
{
    if (m_childTags.empty() && m_element->hasChildNodes()) {

        xercesc::DOMNodeList* childNodes;
        xercesc::DOMNode* iNode;

        childNodes = m_element->getChildNodes();

        for (unsigned i = 0; i < childNodes->getLength(); i++) {
            iNode = childNodes->item(i);

            if (iNode->getNodeType() == xercesc::DOMNode::ELEMENT_NODE) {
                m_childTags.emplace_back(iNode);
            }
        }
    }
}

}