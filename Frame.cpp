//Frame.cpp
#include "Frame.h"

using namespace sdlUtility;

namespace sdlGUI {
    bool Frame::AllowsDebug() {
        return AllowDebug;
    }

    Frame::Frame() {
        Next = Previous = NULL;
        AllowDebug = true; Embedded = false; Interactable.EnableInteraction(false);
        AutoSize = Vector2(true, true);
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
