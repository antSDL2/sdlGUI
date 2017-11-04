//StatusBar.cpp
#include "StatusBar.h"

namespace AtGLui {
    float StatusBar::GetStatus() {
        return Status;
    }

    void StatusBar::Render(float Interpolation) {
        if (IsShown(true) or IsMoving(Axis::XY, true)) {
            Scaled = true;

            float TargetX = X(Interpolation, Reference::Origin);
            float TargetY = Y(Interpolation, Reference::Origin);
            float TargetWidth = Width();
            float TargetHeight = Height();
            float ClipWidth = 1, ClipHeight = 1;
            float ClipX = 0, ClipY = 0;

            if (Orientation == Orientation::Horizontal) {
                ClipWidth = Status;
                if (Direction == -1) TargetX += Width()-Status*Width();
            } else if (Orientation == Orientation::Vertical) {
                ClipY = 1-Status;
                if (Direction == 1) {
                    //TargetY += Height();
                    //TargetHeight *= -1;
                }
            }

            Renderable.RenderAsTexture(TargetX, TargetY, TargetWidth, TargetHeight, ClipWidth, ClipHeight, ClipX, ClipY);

            Scaled = false;
        }
    }

    void StatusBar::SetDirection(int Direction) {
        this->Direction = Direction;
    }

    void StatusBar::SetOrientation(int Orientation) {
        this->Orientation = Orientation;
    }

    void StatusBar::SetStatus(float Status) {
        if (Status < 0) {
            this->Status = 0;
        } else if (Status > 1) {
            this->Status = 1;
        } else {
            this->Status = Status;
        }
    }

    StatusBar::StatusBar() {
        Next = Previous = NULL;
        Status = 0;
        Direction = 1; Orientation = Orientation::Horizontal;
    }

    StatusBar::~StatusBar() {
        Next = Previous = NULL;
        Status = 0;
        Direction = Orientation = 0;
    }
}
