//Bind.h
#ifndef NEOGUI_BIND_H
#define NEOGUI_BIND_H

#include "Element.h"
namespace AtGLui {
    class Bind: public Element {
        //Members
        public:
            Bind *Next, *Previous;
        protected:
            bool Repeat;
            SDL_Scancode Key, Modifier;
        //Access Methods
        public:
            Bind();
            ~Bind();
            SDL_Scancode GetKey();
            SDL_Scancode GetModifier();
        //Process Methods
        public:
            void SetKey(SDL_Scancode Key);
            void SetModifier(SDL_Scancode Modifier);
            void SetRepeat(bool Repeat);
        protected:
            int OnInput(SDL_Event &Event);
    };
}

#endif
