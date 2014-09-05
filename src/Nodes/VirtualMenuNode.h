/*
 * Copyright (C) 2012 Kolibre
 *
 * This file is part of kolibre-naviengine.
 *
 * Kolibre-naviengine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * Kolibre-naviengine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with kolibre-naviengine. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NAVIENGINE_VIRTUALMENUNODE
#define NAVIENGINE_VIRTUALMENUNODE

#include "AnyNode.h"

#include <vector>
#include <string>
#include <sstream>

namespace naviengine
{

class NaviEngine;

/**
 * A data type to hold a virtual child.
 */
struct VirtualNode
{
    std::string name_;
    std::string info_;
    std::string uri_;

    VirtualNode(std::string name, std::string info) : name_(name), info_(info)
    {
        std::ostringstream uri_from_anything;
        uri_from_anything << reinterpret_cast<long>(this);
        uri_ = uri_from_anything.str() + "_" + name_;
    }

    VirtualNode(std::string name) : name_(name), info_("")
    {
        std::ostringstream uri_from_anything;
        uri_from_anything << reinterpret_cast<long>(this);
        uri_ = uri_from_anything.str() + "_" + name_;
    }
};

/**
 * A virtual menu node, can only have virtual children.
 */
class VirtualMenuNode: public AnyNode
{
public:
    VirtualMenuNode(const std::string& name = "");
    ~VirtualMenuNode();
    AnyNode* firstChild() const;

    bool select(NaviEngine& navi);
    bool selectByUri(NaviEngine& navi, std::string uri);
    bool up(NaviEngine& navi);
    bool next(NaviEngine& navi);
    bool prev(NaviEngine& navi);
    bool menu(NaviEngine& navi);
    bool onOpen(NaviEngine& navi);
    void beforeOnOpen();
    bool narrateName();
    bool narrateInfo();
    bool onNarrate();
    bool onRender();
    bool isVirtual();
    bool process(NaviEngine&, int command, void* data = 0);
    bool abort();

    int numberOfChildren();

public:
    /** Vector holding the virtual children */
    std::vector<VirtualNode> children;
    /** Variable holding the current child */
    int currentChild;
};
}
#endif
