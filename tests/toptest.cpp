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

#include <assert.h>
#include <string>
#include <iostream>

using namespace naviengine;

class SimpleMenuNode: public MenuNode
{
    public:
    SimpleMenuNode(std::string name)
        : MenuNode(name)
    {
        onOpenVisits_ = 0;
    }

    bool onOpen(NaviEngine& navi)
    {
        std::cout << __PRETTY_FUNCTION__ << " name: " << name_ << ", visits: " << onOpenVisits_ << std::endl;
        // if we visit this method two or more times the test has failed
        assert(onOpenVisits_ == 0);
        onOpenVisits_++;
        return true;
    }
    private:
    int onOpenVisits_;
};

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
    void narrate(const int value)
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

MenuNode* menu_model_builder()
{
    // create root menu (level 0)
    MenuNode* root = new MenuNode("root");

    // create 2 children for each child at level 0 (level 1)
    SimpleMenuNode* l1c1 = new SimpleMenuNode("level 1, child 1");
    root->addNode(l1c1);
    SimpleMenuNode* l1c2 = new SimpleMenuNode("level 1, child 2");
    root->addNode(l1c2);

    // create 2 children for each child at level 1 (level 2)
    SimpleMenuNode* l2c1 = new SimpleMenuNode("level 2, child 1");
    l1c1->addNode(l2c1);
    SimpleMenuNode* l2c2 = new SimpleMenuNode("level 2, child 2");
    l1c1->addNode(l2c2);
    SimpleMenuNode* l2c3 = new SimpleMenuNode("level 2, child 3");
    l1c2->addNode(l2c3);
    SimpleMenuNode* l2c4 = new SimpleMenuNode("level 2, child 4");
    l1c2->addNode(l2c4);

    // create 1 child for each child at level 2 (level 3)
    SimpleMenuNode* l3c1 = new SimpleMenuNode("level 3, child 1");
    l2c1->addNode(l3c1);
    SimpleMenuNode* l3c2 = new SimpleMenuNode("level 3, child 2");
    l2c2->addNode(l3c2);
    SimpleMenuNode* l3c3 = new SimpleMenuNode("level 3, child 3");
    l2c3->addNode(l3c3);
    SimpleMenuNode* l3c4 = new SimpleMenuNode("level 3, child 4");
    l2c4->addNode(l3c4);
    return root;
}

int main()
{
    Navi navi;

    // Opening NULL should return false
    assert(not navi.openMenu( 0 ));

    // Adding NULL as child should not crash
    MenuNode testAdd0Child("dummy");
    testAdd0Child.addNode(0);

    // open simple navigation model with 3 levels
    naviengine::MenuNode* model = menu_model_builder();
    assert(navi.openMenu(model));
    assert(navi.getCurrentNode()->name_ == "root");
    assert(navi.numberOfChildren(navi.getCurrentNode()) == 2);
    assert(navi.getCurrentChoice()->name_ == "level 1, child 1");

    // open a leaf node
    assert(navi.select());
    assert(navi.getCurrentNode()->name_ == "level 1, child 1");
    assert(navi.select());
    assert(navi.getCurrentNode()->name_ == "level 2, child 1");
    assert(navi.select());
    assert(navi.getCurrentNode()->name_ == "level 3, child 1");

    // top should take as to the root without opening nodes on the way
    assert(navi.top());
    assert(navi.getCurrentNode()->name_ == "root");

    return 0;
}
