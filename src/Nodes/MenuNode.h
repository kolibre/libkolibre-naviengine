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

#ifndef NAVIENGINE_MENUNODE
#define NAVIENGINE_MENUNODE

#include "AnyNode.h"

#include <vector>
#include <string>

namespace naviengine
{
class NaviEngine;

/**
 * A menu node, can have children of AnyNode type
 */
class MenuNode: public AnyNode
{
public:
    MenuNode(const std::string& name = "");
    ~MenuNode();
    AnyNode* firstChild() const;
    AnyNode* lastChild();

    void clearNodes();
    void addNode(AnyNode* node);
    bool up(NaviEngine& navi);
    bool prev(NaviEngine& navi);
    bool next(NaviEngine& navi);
    bool select(NaviEngine& navi);
    bool selectByUri(NaviEngine& navi, std::string uri);
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

private:
    std::vector<AnyNode*> children;
};
}
#endif
