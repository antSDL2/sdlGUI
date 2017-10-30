//List.h
#ifndef NEOGUI_LIST_H
#define NEOGUI_LIST_H

#include "Element.h"
#include "Message.h"
#include <list>
namespace AtGLui {
    namespace Lists {
        static const int Display        = 0;
        static const int Selection      = 1;
    }

    class List: public Element {
        //Members
        public:
            List *Next, *Previous;
        protected:
            bool HideOnInput;
            Element *SelectedItem;
            Message *DisplayMessage;
            int ItemLimit, Items, Type;
            AtObjects::Vector2 Item;
            std::list<Element*> ListItems;
            std::string Orientation;
        //Access Methods
        public:
            List();
            ~List();
            Message *GetDisplayMessage();
            AtObjects::Vector2 GetItemSize();
            int GetType();
            int GetItemLimit();
            int GetItems();
            std::string GetOrientation();
            Element *GetSelectedItem();
            bool HidesOnInput();
        //Process Methods
        public:
            void AddItem(Element *ListItem);
            void ClearItems();
            void PositionItem(int i, std::list<Element*>::iterator Iterator);
            void RemoveItem(Element *ListItem);
            void Render(float Interpolation);
            void SetDisplayMessage(Message *DisplayMessage);
            void SetHideOnInput(bool HideOnInput);
            void SetItemLimit(int ItemLimit);
            void SetItemSize(int Width, int Height);
            void SetOrientation(std::string Orientation);
            void SetSelectedItem(Element *ListItem);
            void SetType(int Type);
            void UpdateItemPositions();
            void UpdateSize();
        protected:
            void OnHide();
            int OnInput(SDL_Event &Event);
            void OnValueChange();
    };
}

#endif
