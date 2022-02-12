//
// Created by Matthew McCall on 2/10/22.
//

#ifndef YORK_XML_TAG_HPP
#define YORK_XML_TAG_HPP

#include <string>
#include <vector>

#include <xercesc/dom/DOM.hpp>

namespace york::xml {

class Tag {

public:
    explicit Tag(xercesc::DOMNode* node);
    Tag(std::string& name, std::string& value);

    std::string getValue();
    std::string getName();
    std::vector<Tag> getChildTags();

    std::vector<Tag>::iterator begin();
    std::vector<Tag>::iterator end();


private:

    void updateChildren();

    std::string m_tagName;
    std::string m_value;
    std::vector<Tag> m_childTags;
    xercesc::DOMElement* m_element;
};

}


#endif // YORK_XML_TAG_HPP
