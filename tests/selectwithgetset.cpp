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
#include "Nodes/MenuNode.h"
#include "Nodes/VirtualMenuNode.h"

#include <assert.h>
#include <string>

using namespace naviengine;

class Navi: public NaviEngine
{
public:
    MenuNode* buildContextMenu()
    {
        return 0;
    }
private:
    void narrateChange(const MenuState& before, const MenuState& after)
    {
    }
    void narrate(const std::string text)
    {
    }
    void narrate(const int text)
    {
    }
    void narrateStop()
    {
    }
    void narrateShortPause()
    {
    }
    void narrateLongPause()
    {
    }
};

int main()
{
    // create root menu (level 0)
    MenuNode* root = new MenuNode("root");

    // create 2 child for root node (level 1)
    MenuNode* l1c1 = new MenuNode("level 1, child 1");
    root->addNode(l1c1);
    MenuNode* l1c2 = new MenuNode("level 1, child 2");
    root->addNode(l1c2);

    // create 2 childs for each node at level 1 (level 2)
    MenuNode* l2c1 = new MenuNode("level 2, child 1");
    l1c1->addNode(l2c1);
    MenuNode* l2c2 = new MenuNode("level 2, child 2");
    l1c1->addNode(l2c2);
    MenuNode* l2c3 = new MenuNode("level 2, child 3");
    l1c2->addNode(l2c3);
    MenuNode* l2c4 = new MenuNode("level 2, child 4");
    l1c2->addNode(l2c4);

    Navi navi;

    // open menu model above
    assert(navi.openMenu(root));
    assert(navi.getCurrentNode()->name_ == "root");
    assert(navi.numberOfChildren(navi.getCurrentNode()) == 2);
    assert(navi.getCurrentChoice()->name_ == "level 1, child 1");

    // change current choice
    navi.setCurrentChoice(l1c2);
    assert(navi.getCurrentNode()->name_ == "root");
    assert(navi.numberOfChildren(navi.getCurrentNode()) == 2);

    assert(navi.select());
    assert(navi.getCurrentNode()->name_ == "level 1, child 2");
    assert(navi.numberOfChildren(navi.getCurrentNode()) == 2);
    assert(navi.getCurrentChoice()->name_ == "level 2, child 3");

    assert(navi.up());
    assert(navi.getCurrentNode()->name_ == "root");
    assert(navi.numberOfChildren(navi.getCurrentNode()) == 2);
    assert(navi.getCurrentChoice()->name_ == "level 1, child 2");

    // change current node and choice
    navi.setCurrentNode(l1c1);
    navi.setCurrentChoice(l2c2);
    assert(navi.getCurrentNode()->name_ == "level 1, child 1");
    assert(navi.numberOfChildren(navi.getCurrentNode()) == 2);
    assert(navi.getCurrentChoice()->name_ == "level 2, child 2");

    assert(navi.up());
    assert(navi.getCurrentNode()->name_ == "root");
    assert(navi.numberOfChildren(navi.getCurrentNode()) == 2);
    assert(navi.getCurrentChoice()->name_ == "level 1, child 1");

    return 0;
}
