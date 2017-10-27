//Dialog.cpp
#include "Dialog.h"
#include <AtGfx/Renderer.h>
#include <iostream>
#include <math.h>

namespace AtGLui {
    Dialog::Dialog() {
        Next = Previous = NULL;
        CopyInput = true; Focused = false;
        AutoSize = AtPhys::Vector2(true, true);
    }

    int Dialog::OnInput(SDL_Event &Event) {
        int Input = 0;

        if (Event.type == SDL_KEYDOWN) {
            if (Event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                if (Focused && IsShown(true) && Interactable.IsInteractive()) {
                    Hide();
                    Input = 212;
                    Focused = false;
                }
            }
        }

        return Input;
    }

    void Dialog::Render(float Interpolation) {
        if (IsShown(true) or IsMoving(Axis::XY, true)) { //TEMP Is this going to be a problem?
            Scaled = true;

            float TargetX = X(Interpolation, Reference::Origin);
            float TargetY = Y(Interpolation, Reference::Origin);
            //if (ID() == "Menu.Dialog") std::cout << fabs(X(Reference::Origin, 0, 2)-TargetX) << " " << fabs(Interpolation*Velocity.X()) << std::endl;

            Renderable.RenderAsDialog(TargetX, TargetY, Width(), Height());
            Scaled = false;
        }
    }

    void Dialog::SetFocused(bool Focused) {
        this->Focused = Focused;
    }

    Dialog::~Dialog() {
        Next = Previous = NULL;
        Focused = false;
    }
}
