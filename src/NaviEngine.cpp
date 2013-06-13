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

#include "NaviEngine.h"

using namespace naviengine;

int currentSibling(const AnyNode* node)
{
    if (node->parent_->firstChild() == NULL)
    {
        return 0;
    }

    const AnyNode* tmp = node;
    int num = 1;
    while (tmp != node->parent_->firstChild() && tmp != NULL)
    {
        tmp = tmp->prev_;
        num++;
    }

    return num;
}

/**
 * Constructor
 */
NaviEngine::NaviEngine() :
        good_(false), openOnChange_(true)
{
}

/**
 * Destructor
 *
 * Deletes nodes in the menu stack
 */
NaviEngine::~NaviEngine()
{
    while (not menuStack.empty())
    {
        delete menuStack.top().menuModel;
        menuStack.pop();
    }
}

/**
 * Check if the last function call was successful
 *
 * @return A boolean indicating if the last function call was successful or not
 */
bool NaviEngine::good() const
{
    return good_;
}

/**
 * Open a menu
 *
 * @param node The menu node to open
 * @param narrable If true, narrate functions are called
 * @return true on success, otherwise false
 */
bool NaviEngine::openMenu(AnyNode* node, bool narrable)
{
    if (node == 0)
        return false;

    MenuState before;

    MenuState menu;
    menu.menuModel = node;
    menu.state.currentNode = node;
    menu.state.currentChoice = node->firstChild();
    menuStack.push(menu);

    if (narrable)
    {
        node->beforeOnOpen();
        narrateShortPause();
        good_ = node->onOpen(*this);
        narrateChange(before, menuStack.top());
    }

    return good_;
}

/**
 * Close a menu
 *
 * @return true on success, otherwsie false
 */
bool NaviEngine::closeMenu()
{
    if (menuStack.size() > 1)
    {
        MenuState menu = menuStack.top();
        delete menu.menuModel;
        menu.menuModel = NULL;
        menuStack.pop();

        return true; // This means the menu was closed
    }

    MenuState& menu = menuStack.top();
    menu.state.currentNode = menu.menuModel; // Fix the root menu state if someone borked it.
    menu.state.currentChoice = menu.state.currentNode->firstChild();

    return false; // No menu closed
}

/**
 * Invokes onNarrate for the current node
 *
 * If onNarrate returns false NaviEngine will render the node
 * with the help of it's virtual narrate functions and the nodes narrate methods
 */
void NaviEngine::narrateNode()
{
    MenuState& menu = menuStack.top();
    if (not menu.state.currentNode->onNarrate())
    {
        if (not menu.state.currentNode->narrateName())
            narrate(menu.state.currentNode->name_.c_str());
        narrateShortPause();
        narrateNode(menu.state.currentChoice);
    }
}

/**
 * Invoke onNarrate for a node
 *
 * If onNarrate returns false NaviEngine will render the node
 * with the help of it's virtual narrate functions and the nodes narrate methods
 *
 * @param choice A pointer the to choice to narrate
 */
void NaviEngine::narrateNode(AnyNode* node)
{
    if (node == 0)
        return;

    if (not node->onNarrate())
    {
        if (not node->narrateName())
        {
            narrate(currentSibling(node));
            narrate(node->name_.c_str());
            narrateLongPause();
        }
    }
}

/**
 * Invoke onRender for a node
 *
 * @param node A pointer the the node to render
 * @return The result from the onRender invoke
 */
bool NaviEngine::renderNode(AnyNode* node)
{
    if (node == NULL)
        return false;

    bool selfRendered = node->onRender();
    return not selfRendered;
}

/**
 * Go to the root (top) node
 *
 * @return true on success, otherwise false
 */
bool NaviEngine::top()
{
    MenuState menu = menuStack.top();

    // If already on top level, open the menu
    if (menu.state.currentNode == menu.menuModel)
    {
        menuStack.top().state.currentNode->beforeOnOpen();
        narrateShortPause();
        menuStack.top().state.currentNode->onOpen(*this);
    }

    openOnChange_ = false;
    // If not on top level, go to top level
    while (menuStack.size() > 1 || menu.state.currentNode != menu.menuModel)
    {
        narrateStop();
        up();
        menu = menuStack.top();
    }
    openOnChange_ = true;

    // We are now on top level, open the menu
    menuStack.top().state.currentNode->beforeOnOpen();
    narrateShortPause();
    menuStack.top().state.currentNode->onOpen(*this);

    return true;
}

/**
 * Check if state has changed
 *
 * @param before The MenuState before
 * @return true is state has changed, otherwise false
 */
bool NaviEngine::stateHasChanged(const MenuState& before)
{
    MenuState& now = menuStack.top();

    if (now.state.currentNode != before.state.currentNode)
    {
        return true;
    }
    return false;
}

/**
 * Open the current node by invoking onOpen on a state change
 *
 * @param before The MenuState before
 * @return the result from onOpen if the state has changed, otherwise false
 */
bool NaviEngine::openOnChange(const MenuState& before)
{
    MenuState& now = menuStack.top();
    if (now.state.currentNode == NULL)
        closeMenu();

    // if openOnChange_ is false we do not want to open nodes when state changes
    if (not openOnChange_)
        return true;

    if (stateHasChanged(before))
    {
        now = menuStack.top();
        now.state.currentNode->beforeOnOpen();
        narrateShortPause();
        good_ = now.state.currentNode->onOpen(*this);
        narrateChange(before, now);
        return good_;
    }
    return false;
}

/**
 * Go to the parent node
 *
 * @return true on success, otherwise false
 */
bool NaviEngine::up()
{
    bool success = false;
    MenuState before = menuStack.top();
    success = menuStack.top().state.currentNode->up(*this);

    return openOnChange(before);
}

/**
 * Open the currently selected child
 *
 * @return true on success, otherwise false
 */
bool NaviEngine::select()
{
    bool success = false;
    MenuState before = menuStack.top();
    success = menuStack.top().state.currentNode->select(*this);

    return openOnChange(before);
}

/**
 * Open the child referenced by uri
 *
 * @param uri The uri of the child
 * @return true on success, otherwise false
 */
bool NaviEngine::selectNodeByUri(std::string uri)
{
    bool success = false;
    MenuState before = menuStack.top();
    AnyNode* currentNode = menuStack.top().state.currentNode;

    if (currentNode->isVirtual())
    {
        success = currentNode->selectByUri(*this, uri);
    }
    else
    {
        AnyNode* currentChild = currentNode->firstChild();
        if (currentChild != NULL)
        {
            do
            {
                if (uri == currentChild->uri_)
                {
                    menuStack.top().state.currentChoice = currentChild;
                    success = currentNode->select(*this);
                    break;
                }
                currentChild = currentChild->next_;
            } while (currentChild != currentNode->firstChild() && currentChild != NULL);
        }
    }

    if (stateHasChanged(before))
        return openOnChange(before);
    return success;
}

/**
 * Go to the next child and set it as the currently selected child
 *
 * @return true on success, otherwise false
 */
bool NaviEngine::next()
{
    MenuState& menu = menuStack.top();
    MenuState before = menu;
    if (menu.state.currentNode->next(*this))
    {
        narrateChange(before, menu);
        return true;
    }
    return false;
}

/**
 * Go to the previous child and set it as the currently selected child
 *
 * @return true on success, otherwise false
 */
bool NaviEngine::prev()
{
    MenuState& menu = menuStack.top();
    MenuState before = menu;
    if (menu.state.currentNode->prev(*this))
    {
        narrateChange(before, menu);
        return true;
    }
    return false;
}

/**
 * Open the context menu for the current node
 *
 * @return true on success, otherwise false
 */
bool NaviEngine::openContextMenu()
{
    MenuState& menu = menuStack.top();
    return menu.state.currentNode->menu(*this);
}

/**
 * Let the current node process the command
 *
 * @param command The enumerated command the node shall process
 * @data data A pointer to a optional data object
 * @return The result of the process operation
 */
bool NaviEngine::process(int command, void* data)
{
    MenuState& menu = menuStack.top();
    MenuState before = menu;

    bool processedCommand = true;

    if (not menu.state.currentNode->process(*this, command, data))
    { // This is a short of time hack.
        processedCommand = false;
    }

    { // Check if the node has changed during process
        MenuState& menu = menuStack.top();

        if (menu.state.currentNode != before.state.currentNode)
        {
            if (menu.state.currentNode == NULL)
            {
                closeMenu();
            }

            MenuState& menu = menuStack.top();
            menu.state.currentNode->beforeOnOpen();
            narrateShortPause();
            good_ = menu.state.currentNode->onOpen(*this);
            // Consider a successful node change to mean that the command was processed.
            if (good_)
                processedCommand = true;
        }
    }

    MenuState& after = menuStack.top();
    narrateChange(before, after);

    return processedCommand;
}

/**
 * Get the number of children for a node
 *
 * @param node A pointer to the node for which to get the number of children
 * @return A positive value if the node has children, otherwise 0
 */
int NaviEngine::numberOfChildren(AnyNode* node)
{
    if (node == NULL || node->firstChild() == NULL)
    {
        return 0;
    }

    const AnyNode* tmp = node->firstChild();
    int num = 0;
    do
    {
        tmp = tmp->next_;
        num++;
    } while (tmp != node->firstChild() && tmp != NULL);

    return num;
}

/**
 * Get the current node
 *
 * @return A pointer to the current node
 */
AnyNode* NaviEngine::getCurrentNode()
{
    return menuStack.top().state.currentNode;
}

/**
 * Set the current node
 *
 * @param node A pointer to the node which shell become the current node
 */
void NaviEngine::setCurrentNode(AnyNode* node)
{
    menuStack.top().state.currentNode = node;
}

/**
 * Get the current choice
 *
 * @return A pointer to the current choice
 */
AnyNode* NaviEngine::getCurrentChoice()
{
    return menuStack.top().state.currentChoice;
}

/**
 * Set the current choice
 *
 * @param node A pointer to the node which shall become the current choice
 */
void NaviEngine::setCurrentChoice(AnyNode* node)
{
    menuStack.top().state.currentChoice = node;
}
