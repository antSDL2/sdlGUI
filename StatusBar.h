//StatusBar.h
#ifndef NEOGUI_STATUSBAR_H
#define NEOGUI_STATUSBAR_H

#include "Element.h"
namespace sdlGUI {
    class StatusBar: public Element {
        //Members
        public:
            StatusBar *Next, *Previous;
        protected:
            float Status;
            int Direction, Orientation;
        //Access Methods
        public:
            StatusBar();
            ~StatusBar();
            float GetStatus();
        //Process Methods
        public:
            void Render(float Interpolation);
            void SetDirection(int Direction);
            void SetOrientation(int Orientation);
            void SetStatus(float Status);
    };
}

#endif
