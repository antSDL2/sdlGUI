//Frame.cpp
#include "Frame.h"
//#include <iostream>

namespace AtGLui {
    bool Frame::AllowsDebug() {
        return AllowDebug;
    }

    Frame::Frame() {
        Next = Previous = NULL;
        AllowDebug = true; Interactable.EnableInteraction(false);
        AutoSize = AtObjects::Vector2(true, true);
    }

    void Frame::SetAllowDebug(bool State) {
        this->AllowDebug = State;
    }

    Frame::~Frame() {
        Next = Previous = NULL;
    }
}
