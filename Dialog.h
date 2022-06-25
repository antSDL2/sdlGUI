//Dialog.h
#ifndef NEOGUI_DIALOG_H
#define NEOGUI_DIALOG_H

#include "Element.h"
namespace sdlGUI {
    class Dialog: public Element {
        //Members
        public:
            Dialog *Previous, *Next;
        protected:
            bool Focused;
        //Access Methods
        public:
            Dialog();
            ~Dialog();
        //Process Methods
        public:
            void Render(float Interpolation);
            void SetFocused(bool Focused);
        protected:
            int OnInput(SDL_Event &Event);
    };
}

#endif
