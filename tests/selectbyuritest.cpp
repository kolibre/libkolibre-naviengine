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

// vector to contain uris for childs
std::vector<std::string> uris;

class RegularChild: public MenuNode
{
public:
    RegularChild(std::string name)
        : MenuNode(name)
    {
        MenuNode* rc1 = new MenuNode("regular child 1");
        addNode(rc1);
        MenuNode* rc2 = new MenuNode("regular child 2");
        addNode(rc2);
        MenuNode* rc3 = new MenuNode("regular child 3");
        addNode(rc3);
    }

    bool onRender()
    {
        uris.clear();
        if (firstChild() == NULL)
        {
            return 0;
        }

        const AnyNode* tmp = firstChild();
        do
        {
            uris.push_back(tmp->uri_);
            tmp = tmp->next_;
        } while (tmp != firstChild() && tmp != NULL);

        return true;
    }

};

class VirtualChild: public VirtualMenuNode
{
public:
    VirtualChild(std::string name)
        : VirtualMenuNode(name)
    {
        VirtualNode vc1("virtual child 1");
        children.push_back(vc1);
        VirtualNode vc2("virtual child 2");
        children.push_back(vc2);
        VirtualNode vc3("virtual child 3");
        children.push_back(vc3);
    }

    bool onRender()
    {
        uris.clear();
        for (int i=0; i<children.size(); i++)
        {
            uris.push_back(children[i].uri_);
        }
        return true;
    }

    bool selectByUri(NaviEngine& navi, std::string uri)
    {
        for (int i=0; i<children.size(); i++)
        {
            if (children[i].uri_ == uri)
                return true;
        }
        return false;
    }
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

    // create 1 child with 3 childs for root
    RegularChild* rc = new RegularChild("regular child");
    root->addNode(rc);

    // create 1 virtual child with 3 virtual childs for root
    VirtualChild* vc = new VirtualChild("virtual child");
    root->addNode(vc);

    return root;
}

int main()
{
    Navi navi;

    // open simple navigation model with 2 childs (one regular and one virtual)
    naviengine::MenuNode* model = menu_model_builder();
    assert(navi.openMenu(model));

    assert(navi.getCurrentNode()->name_ == "root");
    assert(navi.numberOfChildren(navi.getCurrentNode()) == 2);
    assert(navi.getCurrentChoice()->name_ == "regular child");
    assert(navi.next());
    assert(navi.getCurrentChoice()->name_ == "virtual child");
    assert(navi.next());
    assert(navi.getCurrentChoice()->name_ == "regular child");
    assert(navi.next());
    assert(navi.getCurrentChoice()->name_ == "virtual child");
    assert(navi.prev());
    assert(navi.getCurrentChoice()->name_ == "regular child");

    // open regular child
    assert(navi.select());
    assert(navi.getCurrentNode()->name_ == "regular child");
    assert(navi.numberOfChildren(navi.getCurrentNode()) == 3);
    assert(navi.getCurrentChoice()->name_ == "regular child 1");
    navi.renderNode(navi.getCurrentNode());

    // select each child by uri
    for (int i=0; i<uris.size(); i++)
    {
        assert(navi.selectNodeByUri(uris[i]));
        switch (i)
        {
            case 0:
                assert(navi.getCurrentNode()->name_ == "regular child 1");
                break;
            case 1:
                assert(navi.getCurrentNode()->name_ == "regular child 2");
                break;
            case 2:
                assert(navi.getCurrentNode()->name_ == "regular child 3");
                break;
        }
        assert(navi.up());
    }
    // selecting a non-existing child should fail
    assert(not navi.selectNodeByUri("invalid uri"));

    assert(navi.top());
    assert(navi.next());
    assert(navi.getCurrentChoice()->name_ == "virtual child");

    // open virtual child
    assert(navi.select());
    assert(navi.getCurrentNode()->name_ == "virtual child");
    assert(navi.numberOfChildren(navi.getCurrentNode()) == 0);
    navi.renderNode(navi.getCurrentNode());

    // select each child by uri
    for (int i=0; i<uris.size(); i++)
    {
        assert(navi.selectNodeByUri(uris[i]));
    }
    // selecting a non-existing child should fail
    assert(not navi.selectNodeByUri("invalid uri"));

    return 0;
}
