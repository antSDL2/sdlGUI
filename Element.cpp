//Element.cpp
#include "Element.h"
#include <iostream>
#include <math.h>

namespace AtGLui {
    void Element::AddPadding(float X, float Y) {
        Padding = AtObjects::Vector2(X, Y);
    }

    float Element::ChildrenOffsetX() {
        float Scale = Scaled?this->Scale.X():1;
        return Scale*(Padding.X()+ChildrenOffset.X());
    }

    float Element::ChildrenOffsetY() {
        float Scale = Scaled?this->Scale.Y():1;
        return Scale*(Padding.Y()+ChildrenOffset.Y());
    }

    bool Element::ClipsContents() {
        return ClipContents;
    }

    bool Element::CopiesInput() {
        return CopyInput;
    }

    Element::Element() {
        Parent = NULL;
        CopyInput = ShareInput = false; Container = true; ClipContents = false; Disabled = false;
        Renderable.SetTile(1);
    }

    Element *Element::GetParent() {
        return Parent;
    }

    float Element::GetMarginX() {
        return Margin.X();
    }

    float Element::GetMarginY() {
        return Margin.Y();
    }

    float Element::InheritedHeight(int State) {
        float InheritedHeight = 0.f;

        float Scale = Scaled?this->Scale.Y():1.f;
        float OffsetY = Scale*Interactable.Y();

        if (AutoSize.Y() && Parent) {
            if (Scaled) Parent->EnableScaling(true);
            InheritedHeight = Parent->Height();
            if (InheritedHeight) InheritedHeight -= OffsetY+2.f*Parent->PaddingY();
            if (Scaled) Parent->EnableScaling(false);
        } else {
            AtObjects::Texture *Texture = Renderable.GetTexture();
            if (Texture) {
                InheritedHeight = Scale*Texture->GetHeight();
            }
        }

        return InheritedHeight;
    }

    float Element::InheritedWidth(int State) {
        float InheritedWidth = 0.f;

        float Scale = Scaled?this->Scale.X():1.f;
        float OffsetX = Scale*Interactable.X();

        if (AutoSize.X() && Parent) {
            if (Scaled) Parent->EnableScaling(true);
            InheritedWidth = Parent->Width(State);
            if (InheritedWidth) InheritedWidth -= OffsetX+2.f*Parent->PaddingX();
            if (Scaled) Parent->EnableScaling(false);
        } else {
            AtObjects::Texture *Texture = Renderable.GetTexture();
            if (Texture) {
                InheritedWidth = Scale*Texture->GetWidth();
            }
        }

        return InheritedWidth;
    }

    bool Element::IsContainer() {
        return Container;
    }

    bool Element::IsDisabled() {
        return Disabled;
    }

    bool Element::SharesInput() {
        return ShareInput;
    }

    void Element::OffsetChildren(int Axis, float Position) {
        if (Axis == Axis::X) {
            ChildrenOffset = AtObjects::Vector2(Position-Padding.X(), ChildrenOffset.Y());
        } else if (Axis == Axis::Y) {
            ChildrenOffset = AtObjects::Vector2(ChildrenOffset.X(), Position-Padding.Y());
        }
    }

    void Element::OnMouseIn() {
        if (!Disabled) {
            if (Renderable.GetTile() == 1) Renderable.SetTile(2);

            if (IsHovered()) Renderable.SetColor(AtObjects::Color::Hovered);
        }
    }

    void Element::OnMouseOut() {
        if (!Disabled) {
            if (Renderable.GetTile() != 1) Renderable.SetTile(1);

            if (!IsHovered()) Renderable.SetColor(AtObjects::Color::Idle);
        }
    }

    float Element::PaddingX() {
        float Scale = Scaled?this->Scale.X():1;
        return Scale*Padding.X();
    }

    float Element::PaddingY() {
        float Scale = Scaled?this->Scale.Y():1;
        return Scale*Padding.Y();
    }

    void Element::Render(float Interpolation) {
        if (IsShown(true) or IsMoving(Axis::XY, true)) {
            UpdateClipping();

            Scaled = true;

            float TargetX = X(Interpolation, Reference::Origin);
            float TargetY = Y(Interpolation, Reference::Origin);

            Renderable.RenderAsTexture(TargetX, TargetY, Width(), Height(), ClipSize.X(), ClipSize.Y(), ClipPosition.X(), ClipPosition.Y());
            Scaled = false;
        }
    }

    void Element::SetContainer(bool Container) {
        this->Container = Container;
    }

    void Element::SetClipContents(bool ClipContents) {
        this->ClipContents = ClipContents;
    }

    void Element::SetCopyInput(bool CopyInput) {
        this->CopyInput = CopyInput;
    }

    void Element::SetDisabled(bool Disabled) {
        if (Disabled && !this->Disabled) {
            ResetInput();
            Renderable.SetTile(4);
            Renderable.SetColor(AtObjects::Color::Disabled);
        } else if (!Disabled && this->Disabled) {
            Renderable.SetTile(1);
            Renderable.SetColor(AtObjects::Color::Idle);
        }

        this->Disabled = Disabled;
        State = !Disabled;
    }

    void Element::SetMargin(int X, int Y) {
        Margin = AtObjects::Vector2(X, Y);
    }

    void Element::SetParent(Element *Parent) {
        this->Parent = Parent;
        this->BaseParent = Parent;
    }

    void Element::SetShareInput(bool ShareInput) {
        this->ShareInput = ShareInput;
    }

    void Element::UpdateClipping() {
        //TEMP Performance concern?
        if (Parent) {
            if (Parent->ClipsContents()) {
                if (Scale == AtObjects::Vector2(1.f, 1.f)) Parent->EnableScaling(true);

                float LocalX = X(Reference::Parent)+Parent->ChildrenOffsetX(), LocalY = Y(Reference::Parent)+Parent->ChildrenOffsetY();
                float Width = this->Width(), Height = this->Height();
                float ParentWidth = Parent->Width(), ParentHeight = Parent->Height();

                float ClipWidth = 1.f-(LocalX+Width-ParentWidth+2.f*Parent->PaddingX())/Width;
                float ClipHeight = 1.f-(LocalY+Height-ParentHeight+2.f*Parent->PaddingY())/Height;

                float ClipX = -LocalX/Width;
                float ClipY = -LocalY/Height;

                ClipSize = AtObjects::Vector2(ClipWidth, ClipHeight);
                ClipPosition = AtObjects::Vector2(ClipX, ClipY);

                if (Scale == AtObjects::Vector2(1.f, 1.f)) Parent->EnableScaling(false);
                //if (Name == "Testy") std::cout << ClipY << " " << LocalY << " " << Height << " " << AutoSize.Y() << std::endl;
            } else if (Parent->IsClipped()) {
                float LocalX = X(Reference::Parent), LocalY = Y(Reference::Parent);
                float Width = this->Width(), Height = this->Height();
                float ParentWidth = Parent->Width(), ParentHeight = Parent->Height();

                float ParentClipX = Parent->ClipX(), ParentClipY = Parent->ClipY();
                float ParentClipHeight = Parent->ClipHeight(), ParentClipWidth = Parent->ClipWidth();

                float ClipX = (ParentClipX*ParentWidth-LocalX)/Width;
                float ClipY = (ParentClipY*ParentHeight-LocalY)/Height;

                float ClipWidth = (ParentClipWidth*ParentWidth-LocalX)/Width;
                float ClipHeight = (ParentClipHeight*ParentHeight-LocalY)/Height;

                ClipPosition = AtObjects::Vector2(ClipX, ClipY);
                ClipSize = AtObjects::Vector2(ClipWidth, ClipHeight);
            } else {ClipPosition = AtObjects::Vector2(0.f, 0.f); ClipSize = AtObjects::Vector2(1.f, 1.f);  }
        }
    }

    Element::~Element() {
        Parent = NULL;
        CopyInput = Container = ClipContents = Disabled = ShareInput = false;
    }
}
