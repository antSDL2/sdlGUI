//Element.cpp
#include "Element.h"
#include <iostream>
#include <AtGfx/Renderer.h>
#include <math.h>

namespace AtGLui {
    void Element::AddPadding(float X, float Y) {
        Padding = AtPhys::Vector2(X, Y);
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

    AtGfx::Renderable *Element::GetRenderable() {
        return &Renderable;
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
            AtGfx::Texture *Texture = Renderable.GetTexture();
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
            AtGfx::Texture *Texture = Renderable.GetTexture();
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
            ChildrenOffset = AtPhys::Vector2(Position-Padding.X(), ChildrenOffset.Y());
        } else if (Axis == Axis::Y) {
            ChildrenOffset = AtPhys::Vector2(ChildrenOffset.X(), Position-Padding.Y());
        }
    }

    void Element::OnAnimationChange() {
    }

    void Element::OnMouseIn() {
        if (!Disabled) {
            if (Renderable.GetTile() == 1) Renderable.SetTile(2);

            if (IsHovered()) Renderable.SetColor(AtGfx::Color::Hovered);
        }
    }

    void Element::OnMouseOut() {
        if (!Disabled) {
            if (Renderable.GetTile() != 1) Renderable.SetTile(1);

            if (!IsHovered()) Renderable.SetColor(AtGfx::Color::Idle);
        }
    }

    void Element::OnProcess() {
        //EventHandler
        int Event = Renderable.HandleEvents();
        switch(Event) {
            case AtGfx::Events::AnimationChange:
                OnAnimationChange();
                break;
            case AtGfx::Events::TextureChange:
                OnTextureChange();
                break;
            default:
                break;
        }
    }

    void Element::OnTextureChange() {
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

    void Element::RenderShape(float Interpolation, int DebugState) {
        //if (Name == "FramesPerSecondMessage") std::cout << Focus << std::endl;

        //if (IsShown(true)) {
            float R, G, B, A;

            if (DebugState == 1) {
                R = 1.0f;
                G = 0.2f;
                B = 0.3f;
                A = 1.0f;
            } else if (DebugState == 2) {
                R = 0.2f;
                G = 1.0f;
                B = 0.3f;
                A = 1.0f;
            } else if (DebugState == 3) {
                R = 1.0f;
                G = 1.0f;
                B = 0.3f;
                A = 1.0f;
            } else {
                R = 0.2f;
                G = 0.3f;
                B = 1.0f;
                A = 1.0f;
            }

            if (IsHovered() || DebugState) {
                if (IsClicked(SDL_BUTTON_LEFT) || IsClicked(SDL_BUTTON_RIGHT)) {
                    AtGfx::Renderer::SetColor(R, G, B, A);
                } else {
                    AtGfx::Renderer::SetColor(R*0.75f, G*0.75f, B*0.75f, A);
                }
            }

            if (IsHovered() || DebugState) {
                Scaled = true;
                int Shape = Interactable.Shape;
                if (Shape == AtPhys::Shapes::Rectangle) {
                    float TargetX = X(Interpolation, Reference::Origin);
                    float TargetY = Y(Interpolation, Reference::Origin);
                    AtGfx::Renderer::RenderRectangle(TargetX, TargetY, Width(), Height(), false);

                    AtGfx::Renderer::RenderRectangle(TargetX, TargetY, 4, 4, true);
                    AtGfx::Renderer::RenderRectangle(TargetX+Width()-4, TargetY, 4, 4, true);
                    AtGfx::Renderer::RenderRectangle(TargetX+Width()-4, TargetY+Height()-4, 4, 4, true);
                    AtGfx::Renderer::RenderRectangle(TargetX, TargetY+Height()-4, 4, 4, true);
                } else if (Shape == AtPhys::Shapes::Disk) {
                    float TargetCenterX = X(Reference::Origin, Position::Center, 1)+InterpolateX(Interpolation);
                    float TargetCenterY = Y(Reference::Origin, Position::Center, 1)+InterpolateY(Interpolation);
                    AtGfx::Renderer::RenderDisk(TargetCenterX, TargetCenterY, Width()/2.f, Height()/2.f, false);
                }
                Scaled = false;
            }
        //}
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
            Renderable.SetColor(AtGfx::Color::Disabled);
        } else if (!Disabled && this->Disabled) {
            Renderable.SetTile(1);
            Renderable.SetColor(AtGfx::Color::Idle);
        }

        this->Disabled = Disabled;
        State = !Disabled;
    }

    void Element::SetMargin(int X, int Y) {
        Margin = AtPhys::Vector2(X, Y);
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
                if (Scale == AtPhys::Vector2(1.f, 1.f)) Parent->EnableScaling(true);

                float LocalX = X(Reference::Parent)+Parent->ChildrenOffsetX(), LocalY = Y(Reference::Parent)+Parent->ChildrenOffsetY();
                float Width = this->Width(), Height = this->Height();
                float ParentWidth = Parent->Width(), ParentHeight = Parent->Height();

                float ClipWidth = 1.f-(LocalX+Width-ParentWidth+2.f*Parent->PaddingX())/Width;
                float ClipHeight = 1.f-(LocalY+Height-ParentHeight+2.f*Parent->PaddingY())/Height;

                float ClipX = -LocalX/Width;
                float ClipY = -LocalY/Height;

                ClipSize = AtPhys::Vector2(ClipWidth, ClipHeight);
                ClipPosition = AtPhys::Vector2(ClipX, ClipY);

                if (Scale == AtPhys::Vector2(1.f, 1.f)) Parent->EnableScaling(false);
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

                ClipPosition = AtPhys::Vector2(ClipX, ClipY);
                ClipSize = AtPhys::Vector2(ClipWidth, ClipHeight);
            } else {ClipPosition = AtPhys::Vector2(0.f, 0.f); ClipSize = AtPhys::Vector2(1.f, 1.f);  }
        }
    }

    Element::~Element() {
        Parent = NULL;
        CopyInput = Container = ClipContents = Disabled = ShareInput = false;
    }
}
