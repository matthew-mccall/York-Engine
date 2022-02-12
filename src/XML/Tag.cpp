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