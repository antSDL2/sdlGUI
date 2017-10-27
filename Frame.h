//Frame.h
#ifndef NEOGUI_FRAME_H
#define NEOGUI_FRAME_H

#include "Element.h"
namespace AtGLui {
    class Frame: public Element {
        //Members
        public:
            Frame *Next, *Previous;
        protected:
            bool AllowDebug;
        //Access Methods
        public:
            Frame();
            ~Frame();
            bool AllowsDebug();
        //Process Methods
        public:
            void SetAllowDebug(bool State);
    };
}

#endif
