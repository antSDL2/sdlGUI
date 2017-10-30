//Frame.cpp
#include "Frame.h"
//#include <iostream>

namespace AtGLui {
    bool Frame::AllowsDebug() {
        return AllowDebug;
    }

    Frame::Frame() {
        Next = Previous = NULL;
        AllowDebug = true; Embedded = false; Interactable.EnableInteraction(false);
        AutoSize = AtObjects::Vector2(true, true);
    }

    bool Frame::IsEmbedded() {
        return Embedded;
    }

    void Frame::SetAllowDebug(bool State) {
        this->AllowDebug = State;
    }

    void Frame::SetEmbedded(bool State) {
        this->Embedded = State;
    }

    Frame::~Frame() {
        Next = Previous = NULL;
    }
}
