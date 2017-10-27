//Button.h
#ifndef NEOGUI_BUTTON_H
#define NEOGUI_BUTTON_H

#include "List.h"
#include "Message.h"
namespace AtGLui {
    namespace Buttons {
        static const int Standard = 0;
        static const int Checkbox = 1;
        static const int DropDown = 2;
    }

    class Button: public Element {
        //Members
        public:
            Button *Next, *Previous;
        protected:
            bool Checked;
            int Type;
            List *DisplayList;
            Message *DisplayMessage;
        //Access Methods
        public:
            Button();
            ~Button();
            List *GetDisplayList();
            Message *GetDisplayMessage();
            int GetType();
            bool IsChecked();
        //Process Methods
        public:
            void SetChecked(bool State);
            void SetDisplayList(List *DisplayList);
            void SetDisplayMessage(Message *DisplayMessage);
            void SetType(int Type);
            void ToggleCheck();
        protected:
            void OnLeftButtonDown();
            void OnLeftButtonUp();
            void OnMouseIn();
            void OnMouseOut();
    };
}
#endif
