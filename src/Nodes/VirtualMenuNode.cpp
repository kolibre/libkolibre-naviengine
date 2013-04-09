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

#include "VirtualMenuNode.h"
#include "NaviEngine.h"

using namespace naviengine;

/**
 * Constructor.
 *
 * Generates a unique uri for this node.
 *
 * @param name The name of this node.
 */
VirtualMenuNode::VirtualMenuNode(const std::string& name)
{
    name_ = name;
    play_before_onOpen_ = "opening";

    std::ostringstream uri_from_anything;
    uri_from_anything << reinterpret_cast<long>(this);
    uri_ = uri_from_anything.str();
    currentChild = 0;
}

/**
 * Destructor.
 */
VirtualMenuNode::~VirtualMenuNode()
{
}

AnyNode* VirtualMenuNode::firstChild() const
{
    return NULL;
}

bool VirtualMenuNode::select(NaviEngine& navi)
{
    return false;
}

bool VirtualMenuNode::selectByUri(NaviEngine& navi, std::string uri)
{
    return false;
}

bool VirtualMenuNode::up(NaviEngine& navi)
{
    navi.setCurrentNode(this->parent_);
    navi.setCurrentChoice(this);

    if (parent_ == 0)
    {
        return false;
    }

    return true;
}

bool VirtualMenuNode::next(NaviEngine& navi)
{
    currentChild++;
    currentChild %= children.size();
    return true;
}

bool VirtualMenuNode::prev(NaviEngine& navi)
{
    if (currentChild == 0)
        currentChild = children.size() - 1;
    else
        currentChild--;
    return true;
}

bool VirtualMenuNode::menu(NaviEngine& navi)
{
    return false;
}

bool VirtualMenuNode::onOpen(NaviEngine& navi)
{
    return true;
}

bool VirtualMenuNode::onNarrate()
{
    return false;
}

bool VirtualMenuNode::onRender()
{
    return true;
}

bool VirtualMenuNode::isVirtual()
{
    return true;
}

bool VirtualMenuNode::process(NaviEngine& navi, int command, void* data)
{
    return false;
}

/**
 * Get number of children in this node.
 *
 * @return Number of children.
 */
int VirtualMenuNode::numberOfChildren()
{
    return children.size();
}
