//Paragraph.cpp
#include "Paragraph.h"

using namespace AtUtility;

namespace AtGLui {
    std::string Paragraph::GetFont() {
        return Font;
    }

    int *Paragraph::GetShadowColor() {
        return ShadowColor;
    }

    Vector2 Paragraph::GetShadowOffset() {
        return ShadowOffset;
    }

    int Paragraph::GetSize() {
        float Scale = Scaled?this->Scale.Y():1;
        return Scale*Size;
    }

    int Paragraph::GetStyles() {
        return Styles;
    }

    bool Paragraph::IsEditable() {
        return Editable;
    }

    Paragraph::Paragraph() {
        Next = Previous = NULL;
        Orientation = "HORIZONTAL";
        Editable = false;
        Size = Styles = 0; ShadowColor[0] = ShadowColor[1] = ShadowColor[2] = -1;
    }

    void Paragraph::SetEditable(bool Editable) {
        this->Editable = Editable;
    }

    void Paragraph::SetFont(std::string Font) {
        this->Font = Font;
    }

    void Paragraph::SetShadowColor(int ShadowColor[3]) {
        for (int i=0; i<3; i++) {
            if (ShadowColor[i] > -1) this->ShadowColor[i] = ShadowColor[i];
        }
    }

    void Paragraph::SetShadowOffset(float X, float Y) {
        ShadowOffset = Vector2(X, Y);

        if (ShadowColor[0] == -1) {
            ShadowColor[0] = ShadowColor[1] = ShadowColor[2] = 0;
        }
    }

    void Paragraph::SetSize(int Size) {
        this->Size = Size;
    }

    void Paragraph::SetStyle(int Styles) {
        this->Styles = Styles;
    }

    Paragraph::~Paragraph() {
        Next = Previous = NULL;
        Editable = false;
        Size = Styles = 0; ShadowColor[0] = ShadowColor[1] = ShadowColor[2] = -1;
    }
}
