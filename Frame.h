//Frame.h
#ifndef NEOGUI_FRAME_H
#define NEOGUI_FRAME_H

#include "Element.h"
namespace sdlGUI {
    class Frame: public Element {
        //Members
        public:
            Frame *Next, *Previous;
        protected:
            bool AllowDebug, Embedded;
        //Access Methods
        public:
            Frame();
            ~Frame();
            bool AllowsDebug();
            bool IsEmbedded();
        //Process Methods
        public:
            void SetAllowDebug(bool State);
            void SetEmbedded(bool State);
    };
}

#endif
