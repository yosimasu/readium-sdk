//
//  element.h
//  ePub3
//
//  Created by Jim Dovey on 2012-11-27.
//  Copyright (c) 2012-2013 The Readium Foundation and contributors.
//  
//  The Readium SDK is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef __ePub3_xml_element__
#define __ePub3_xml_element__

#include "base.h"
#include "node.h"
#include "ns.h"

EPUB3_XML_BEGIN_NAMESPACE

class Element : public Node
{
public:
    explicit Element(const xmlNodePtr xml) : Node(xml) {}
    Element(const string & name, class Document * doc = nullptr, const string & nsUri=string(), const string & nsPrefix=string()) : Node(name, NodeType::Element, "") {
        if ( doc != nullptr && !nsUri.empty() ) {
            class Namespace ns(doc, nsPrefix, nsUri);
            SetNamespace(&ns);
        }
    }
    Element(const string & name, const class Namespace & ns = xml::Namespace()) : Node(name, NodeType::Element, "", ns) {}
    Element(Element &&o) : Node(dynamic_cast<Node&&>(o)) {}
    virtual ~Element() {}
};

EPUB3_XML_END_NAMESPACE

#endif /* defined(__ePub3__element__) */
