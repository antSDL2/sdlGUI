//Slider.cpp
#include "Slider.h"
#include <iostream>

namespace AtGLui {
    int Slider::GetOrientation() {
        return Orientation;
    }

    float Slider::GetSliderValue() {
        return ((1.f-Status)*MinimumValue+Status*MaximumValue);
    }

    int Slider::GetType() {
        return Type;
    }

    int Slider::OnInput(SDL_Event &Event) {
        int Input = 0;

        if (IsShown(true) && Type == Sliders::Drag) {
            if (Event.type == SDL_MOUSEWHEEL) {
                const Uint8 *Keyboard = SDL_GetKeyboardState(NULL);
                bool ShiftState = Keyboard[SDL_SCANCODE_LSHIFT]||Keyboard[SDL_SCANCODE_RSHIFT];

                if ((ShiftState && Orientation == Orientation::Horizontal) || (!ShiftState && Orientation == Orientation::Vertical)) {
                    Status -= Event.button.x*0.2f;
                    Input = 241;
                }

                if (Status < 0) Status = 0; else if (Status > 1) Status = 1;
            }
        }

        return Input;
    }

    void Slider::OnProcess() {
        if (Cursor) {
            if (IsClicked(SDL_BUTTON_LEFT)) {
                Scaled = true;
                Cursor->EnableScaling(true);

                if (Orientation == Orientation::Horizontal) {
                    //std::cout << Name <<" " <<  DragReference.X()-Cursor->X(Reference::Parent) << std::endl;
                    SetStatus((DragReference.X()-Cursor->Width()/2.f)/(Width()-Cursor->Width()));

                    if (Status < 0) {
                        Status = 0;
                        DragReference = AtObjects::Vector2(Cursor->Width()/2.f, DragReference.Y());
                    } else if (Status > 1) {
                        Status = 1;
                        DragReference = AtObjects::Vector2(Width()-Cursor->Width()/2.f, DragReference.Y());
                    }

                    Cursor->DropAt((DragReference.X()-Cursor->Width()/2.f)/Scale.X(), 0);
                } else if (Orientation == Orientation::Vertical) {
                    SetStatus((DragReference.Y()-Cursor->Height()/2.f)/(Height()-Cursor->Height()));

                    if (Status < 0) {
                        Status = 0;
                        DragReference = AtObjects::Vector2(DragReference.X(), Cursor->Height()/2.f);
                    } else if (Status > 1) {
                        Status = 1;
                        DragReference = AtObjects::Vector2(DragReference.X(), Height()-Cursor->Height()/2.f);
                    }

                    Cursor->DropAt(0, (DragReference.Y()-Cursor->Height()/2.f)/Scale.Y());
                }

                //std::cout << Status << std::endl;

                Cursor->EnableScaling(false);
                Scaled = false;
            } else {
                if (Orientation == Orientation::Horizontal) {
                    Cursor->DropAt(Status*(Width()-Cursor->Width()), 0.f);
                } else if (Orientation == Orientation::Vertical) {
                    Cursor->DropAt(0.f, Status*(Height()-Cursor->Height()));
                }
            }
        }

        if (Type == Sliders::Drag) {
            if (Parent) {
                if (Orientation == Orientation::Horizontal) {
                    if (Scale == AtObjects::Vector2(1.f, 1.f)) Parent->EnableScaling(true);

                    float FrameSize = MaximumValue-Width();
                    Parent->OffsetChildren(Axis::X, -Status*FrameSize);
                    //DropAt(Status*FrameSize+Parent->PaddingX(), 0.f);

                    if (Cursor) {
                        float ParentWidth = Parent->Width();
                        Resize(Axis::X, ParentWidth-2.f*Parent->PaddingX());
                        Cursor->Resize(Axis::X, ParentWidth/MaximumValue*ParentWidth);

                        if (Cursor->Width() >= ParentWidth-2.f*Parent->PaddingX()) {
                            Shown = false;
                            Parent->OffsetChildren(Axis::X, 0);
                        } else Shown = true;
                    }

                    if (Scale == AtObjects::Vector2(1.f, 1.f)) Parent->EnableScaling(false);
                } else if (Orientation == Orientation::Vertical) {
                    if (Scale == AtObjects::Vector2(1.f, 1.f)) Parent->EnableScaling(true);

                    float FrameSize = MaximumValue-Height();
                    Parent->OffsetChildren(Axis::Y, -Status*FrameSize);
                    //DropAt(0.f, Status*FrameSize+Parent->PaddingY());

                    if (Cursor) {
                        float ParentHeight = Parent->Height();
                        Resize(Axis::Y, ParentHeight-2.f*Parent->PaddingY());
                        Cursor->Resize(Axis::Y, ParentHeight/MaximumValue*ParentHeight);

                        //if (Name == "DebugDialogSlider2") std::cout << GetID() << " " << Height() << " " << ParentHeight << " " << InheritedHeight() << " " << Parent->ChildrenOffsetY() << " " << Parent->GetName() << std::endl;

                        if (Cursor->Height() >= ParentHeight-2.f*Parent->PaddingY()) {
                            Shown = false;
                            Parent->OffsetChildren(Axis::Y, 0);
                        } else Shown = true;
                    }

                    if (Scale == AtObjects::Vector2(1.f, 1.f)) Parent->EnableScaling(false);
                }
            }
        }
    }

    void Slider::SetCursor(Frame *Cursor) {
        this->Cursor = Cursor;
    }

    void Slider::SetMaximumValue(float MaximumValue) {
        this->MaximumValue = MaximumValue;
    }

    void Slider::SetMinimumValue(float MinimumValue) {
        this->MinimumValue = MinimumValue;
    }

    void Slider::SetOrientation(int Orientation) {
        this->Orientation = Orientation;
    }

    void Slider::SetStatus(float Status) {
        this->Status = Status;
        EventQueue.push_back(AtObjects::Events::ValueChange);
    }

    void Slider::SetSliderValue(float Value) {
        if (MaximumValue != MinimumValue) {
            Status = (Value-MinimumValue)/(MaximumValue-MinimumValue);
            if (Status > 1) Status = 1; else if (Status < 0) Status = 0;
            EventQueue.push_back(AtObjects::Events::ValueChange);
        }
    }

    void Slider::SetType(int Type) {
        this->Type = Type;
    }

    Slider::Slider() {
        Next = Previous = NULL;
        Sticky = true;
        MaximumValue = 1; MinimumValue = Status = 0;
        Orientation = Orientation::Horizontal; Type = Sliders::Standard;
        Cursor = NULL;
    }

    Slider::~Slider() {
        Next = Previous = NULL;
        MaximumValue = 1; MinimumValue = Status = 0;
        Orientation = Type = 0;
        Cursor = NULL;
    }
}
