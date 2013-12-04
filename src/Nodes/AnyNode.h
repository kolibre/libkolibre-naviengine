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

#ifndef NAVIENGINE_ANYNODE
#define NAVIENGINE_ANYNODE

#include <utility>
#include <string>

namespace naviengine
{

class NaviEngine;
class AnyNode;

/**
 * The abstract base node.
 */
class AnyNode
{
public:
    /**
     * Constructor
     */
    AnyNode() :
            parent_(0), prev_(0), next_(0)
    {
    }

    /*
     * Destructor
     */
    virtual ~AnyNode()
    {
    }

    /**
     * Get the first child in this node.
     */
    virtual AnyNode* firstChild() const
    {
        return NULL;
    }

    /**
     * Open child in this node.
     *
     * @return true on success.
     */
    virtual bool select(NaviEngine&) = 0;

    /**
     * Open child by uri in this node.
     *
     * @return true on success.
     */
    virtual bool selectByUri(NaviEngine&, std::string) = 0;

    /**
     * Go to parent of this node.
     *
     * @return true on success.
     */
    virtual bool up(NaviEngine&) = 0;

    /**
     * Go to next child in this node.
     *
     * @return true on success.
     */
    virtual bool next(NaviEngine&) = 0;

    /**
     * Go the prev child in this node.
     *
     * @return true on success.
     */
    virtual bool prev(NaviEngine&) = 0;

    /**
     * Open menu for this node.
     *
     * @return true on success.
     */
    virtual bool menu(NaviEngine&) = 0;

    /**
     * After select is called on the parent, onOpen is called on the selected child.
     *
     * @return true on successful open. Can be used to dynamically alter the menu model.
     */
    virtual bool onOpen(NaviEngine&) = 0;

    /**
     * Give the node a chance to narrate itself or to determine whether it needs narration.
     *
     * @return true if narrated, false if NaviEngine should try to narrate.
     */
    virtual bool onNarrate() = 0;

    /**
     * Give the node a chance to render itself or to determine whether it needs rendering.
     *
     * @return true if self rendered, false if NaviEngine should render it.
     */
    virtual bool onRender() = 0;

    /**
     * Determine if the node is virtual and contains only virtual child nodes.
     *
     * @return true if the node only has virtual child nodes, otherwise false.
     */
    virtual bool isVirtual() = 0;

    /**
     * Let the node proccess an enumerated command.
     *
     * @return true if the node could process the command.
     */
    virtual bool process(NaviEngine&, int command, void* data = 0) = 0;

    /**
     * Implement this method if the node is doing async work that needs
     * to be aborted
     *
     * @return false if the operation failed
     */
    virtual bool abort() = 0;


public:
    /** Pointer to the parent node */
    AnyNode* parent_;
    /** Pointer to the next child */
    AnyNode *next_;
    /** Pointer to the previous child */
    AnyNode *prev_;
    /** Variable holding the name of this node */
    std::string name_;
    /** Variable holding the info of this node */
    std::string info_;
    /** Variable holding the uri of this node */
    std::string uri_;

    /** Special prompt to be narrated before onOpen */
    std::string play_before_onOpen_;
};
}

#endif
