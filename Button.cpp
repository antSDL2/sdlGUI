//Button.cpp
#include "Button.h"
#include <iostream>

using namespace sdlUtility;

namespace sdlGUI {
    Button::Button() {
        Next = Previous = NULL;
        Type = Buttons::Standard;
        Checked = false; ShareInput = true;
        DisplayList = NULL;
        DisplayMessage = NULL;
    }

    List *Button::GetDisplayList() {
        if (Type == Buttons::DropDown) return DisplayList; else return NULL;
    }

    Message *Button::GetDisplayMessage() {
        if (Type == Buttons::DropDown) return DisplayMessage; else return NULL;
    }

    int Button::GetType() {
        return Type;
    }

    bool Button::IsChecked() {
        if (Type == Buttons::Checkbox) return Checked; else return false;
    }

    void Button::OnLeftButtonDown() {
        if (!Disabled) {
            if (Type == Buttons::Standard) {
                Renderable.SetTile(3);
                ChildrenOffset = Vector2(1, 1);
            } else if (Type == Buttons::DropDown) {
                Renderable.SetTile(3);
            }
        }
    }

    void Button::OnLeftButtonUp() {
        if (!Disabled) {
            if (Type == Buttons::Standard) {
                if (Hovered) Renderable.SetTile(2); else Renderable.SetTile(1);
                ChildrenOffset = Vector2(0, 0);
            } else if (Type == Buttons::DropDown) {
                if (Hovered) Renderable.SetTile(2); else Renderable.SetTile(1);
                if (DisplayList) DisplayList->Toggle();
            } else if (Type == Buttons::Checkbox) {
                ToggleCheck();
            }
        }
    }

    void Button::OnMouseIn() {
        if (!Disabled) {
            if (Type == Buttons::Standard) {
                Renderable.SetTile(2);
            } else if (Type == Buttons::DropDown) {
                Renderable.SetTile(2);
            } else if (Type == Buttons::Checkbox) {
                if (Checked) Renderable.SetTile(4); else Renderable.SetTile(2);
            }

            if (IsHovered()) Renderable.SetColor(sdlObjects::Color::Hovered);
        }
    }

    void Button::OnMouseOut() {
        if (!Disabled) {
            if (Type == Buttons::Standard) {
                Renderable.SetTile(1);
                ChildrenOffset = Vector2(0, 0);
            } else if (Type == Buttons::DropDown) {
                Renderable.SetTile(1);
            } else if (Type == Buttons::Checkbox) {
                if (Checked) Renderable.SetTile(3); else Renderable.SetTile(1);
            }

            if (!IsHovered()) Renderable.SetColor(sdlObjects::Color::Idle);
        }
    }

    void Button::SetChecked(bool State) {
        if (Type == Buttons::Checkbox) {
            if (State) {
                if (!Checked) Renderable.SetTile(Renderable.GetTile()+2);
            } else {
                if (Checked) Renderable.SetTile(Renderable.GetTile()-2);
            }

            Checked = State;
        }
    }

    void Button::SetDisplayList(List *DisplayList) {
        if (Type == Buttons::DropDown) this->DisplayList = DisplayList;
    }

    void Button::SetDisplayMessage(Message *DisplayMessage) {
        if (Type == Buttons::DropDown) this->DisplayMessage = DisplayMessage;
    }

    void Button::SetType(int Type) {
        this->Type = Type;

        if (Type == Buttons::Checkbox) {
            CopyInput = true;
        } else CopyInput = false;
    }

    void Button::ToggleCheck() {
        if (Type == Buttons::Checkbox) {
            if (Checked) SetChecked(false); else SetChecked(true);
        }
    }

    Button::~Button() {
        Next = Previous = NULL;
        Type = Buttons::Standard;
        Checked = ShareInput = false;
        DisplayList = NULL;
        DisplayMessage = NULL;
    }
}
