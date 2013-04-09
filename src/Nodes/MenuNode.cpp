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

#include "MenuNode.h"
#include "NaviEngine.h"

#include <sstream>

using namespace naviengine;

/**
 * Constructor.
 *
 * Generates a unique uri for this node.
 *
 * @param name The name of this node.
 */
MenuNode::MenuNode(const std::string& name)
{
    name_ = name;
    play_before_onOpen_ = "opening";

    std::ostringstream uri_from_anything;
    uri_from_anything << reinterpret_cast<long>(this);
    uri_ = uri_from_anything.str();
}


/**
 * Destructor.
 *
 * Deletes all its children.
 */
MenuNode::~MenuNode()
{
    for (size_t i = 0; i < children.size(); ++i)
    {
        delete children[i];
    }
}

/**
 * Get the first child in this node.
 *
 * @return A pointer to the first child.
 */
AnyNode* MenuNode::firstChild() const
{
    if (children.empty())
        return NULL;
    return children[0];
}

/**
 * Get the last child in this node.
 *
 * @return A pointer to the last child.
 */
AnyNode* MenuNode::lastChild()
{
    if (children.empty())
        return NULL;
    return children[children.size() - 1];
}

/**
 * Add a child to this node.
 *
 * @param node A pointer to the child.
 */
void MenuNode::addNode(AnyNode* node)
{
    if (node == 0)
    {
        return;
    }

    node->parent_ = this;
    if (not children.empty())
    {
        AnyNode* prevNode = children[children.size() - 1];
        prevNode->next_ = node;
        node->prev_ = prevNode;

        node->next_ = children[0];
        children[0]->prev_ = node;
    }
    else
    {
        node->prev_ = node;
        node->next_ = node;
    }
    children.push_back(node);
}

/**
 * Delete all children in this node.
 */
void MenuNode::clearNodes()
{
    for (size_t i = 0; i < children.size(); ++i)
    {
        delete children[i];
    }
    children.clear();
}

bool MenuNode::up(NaviEngine& navi)
{
    navi.setCurrentNode(this->parent_);
    navi.setCurrentChoice(this);

    if (parent_ == 0)
    {
        return false;
    }

    return true;
}

bool MenuNode::prev(NaviEngine& navi)
{
    if (navi.getCurrentChoice() != NULL)
        if (navi.getCurrentChoice()->prev_ != NULL)
        {
            navi.setCurrentChoice(navi.getCurrentChoice()->prev_);
            return true;
        }
    return false;
}

bool MenuNode::next(NaviEngine& navi)
{
    if (navi.getCurrentChoice() != NULL)
        if (navi.getCurrentChoice()->next_ != NULL)
        {
            navi.setCurrentChoice(navi.getCurrentChoice()->next_);
            return true;
        }
    return false;
}

bool MenuNode::select(NaviEngine& navi)
{
    if (navi.getCurrentChoice() != NULL)
    {
        navi.setCurrentNode(navi.getCurrentChoice());
        navi.setCurrentChoice(navi.getCurrentChoice()->firstChild());
        return true;
    }
    return false;
}

bool MenuNode::selectByUri(NaviEngine& navi, std::string uri)
{
    return false;
}

bool MenuNode::menu(NaviEngine& navi)
{
    return false;
}

bool MenuNode::onOpen(NaviEngine& navi)
{
    return true;
}

bool MenuNode::onNarrate()
{
    return false;
}

bool MenuNode::onRender()
{
    return false;
}

bool MenuNode::isVirtual()
{
    return false;
}

bool MenuNode::process(NaviEngine& navi, int command, void* data)
{
    return false;
}

/**
 * Get number of children in this node.
 *
 * @return Number of children.
 */
int MenuNode::numberOfChildren()
{
    return children.size();
}
