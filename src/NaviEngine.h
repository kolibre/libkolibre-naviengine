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

#ifndef NAVIENGINE
#define NAVIENGINE

#include "Nodes/AnyNode.h"
#include "Nodes/MenuNode.h"

#include <stack>
#include <string>

namespace naviengine
{

/**
 * NaviEngine relays commands to the current node and keeps track of open menus.
 * The menu can be e.g. a service, a book, or a context menu.
 * The menus form a model with a MenuNode as it's root.
 */
class NaviEngine
{
public:
    NaviEngine();
    virtual ~NaviEngine();

    bool top();
    bool up();
    bool select();
    bool selectNodeByUri(std::string uri);
    bool next();
    bool prev();

    bool openContextMenu();
    bool openMenu(AnyNode* node, bool narrable = true);
    bool closeMenu();

    void narrateNode();
    void narrateNode(AnyNode* node);
    bool renderNode(AnyNode* node);

    bool good() const;

    bool process(int command, void* data = 0);

    int numberOfChildren(AnyNode* node);

    AnyNode* getCurrentNode();
    void setCurrentNode(AnyNode* node);
    AnyNode* getCurrentChoice();
    void setCurrentChoice(AnyNode* node);

    /**
     * Build a context menu.
     *
     * Use this function to build a menu which can be open with openMenu.
     */
    virtual MenuNode* buildContextMenu() = 0;

    /**
     * A data type to hold a selection
     */
    struct selection_type
    {
        AnyNode* currentNode;
        AnyNode* currentChoice;
    };

    /**
     * A data type to hold a state
     */
    struct MenuState
    {
        AnyNode* menuModel;
        selection_type state;
        MenuState() :
                menuModel(NULL)
        {
            state.currentNode = NULL;
            state.currentChoice = NULL;
        }
    };

private:
    /**
     * NaviEngine call this functions when state changes
     */
    virtual void narrateChange(const MenuState& before, const MenuState& after) = 0;
    /**
     * NaviEngine call this function when state changes
     */
    virtual void narrate(const std::string text) = 0;
    /**
     * NaviEngine call this function when state changes
     */
    virtual void narrate(const int value) = 0;
    /**
     * NaviEngine call this function when state changes
     */
    virtual void narrateStop() = 0;
    /**
     * NaviEngine call this function when state changes
     */
    virtual void narrateShortPause() = 0;
    /**
     * NaviEngine call this function when state changes
     */
    virtual void narrateLongPause() = 0;

    bool stateHasChanged(const MenuState& before);
    bool openOnChange(const MenuState& before);
    std::stack<MenuState> menuStack;
    bool good_;
};
}
#endif
