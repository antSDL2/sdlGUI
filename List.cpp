//List.cpp
#include "List.h"
#include <iostream>
#include <AtObjects/Renderer.h>

namespace AtGLui {
    void List::AddItem(Element *ListItem) {
        Items += 1;
        ListItems.push_back(ListItem);

        UpdateSize();

        std::list<Element*>::iterator Iterator = ListItems.end();
        Iterator--;
        PositionItem(Items-1, Iterator);
    }

    void List::ClearItems() {
        Items = 0;
        ListItems.clear();
        ResizeTo(0.f, 0.f);
    }

    Message *List::GetDisplayMessage() {
        return DisplayMessage;
    }

    int List::GetType() {
        return Type;
    }

    int List::GetItemLimit() {
        return ItemLimit;
    }

    int List::GetItems() {
        return Items;
    }

    AtObjects::Vector2 List::GetItemSize() {
        return Item;
    }

    std::string List::GetOrientation() {
        return Orientation;
    }

    Element *List::GetSelectedItem() {
        return SelectedItem;
    }

    bool List::HidesOnInput() {
        return HideOnInput;
    }

    List::List() {
        AutoSize = AtObjects::Vector2(true, true);
        HideOnInput = false;
        Next = Previous = NULL;
        CopyInput = true;
        ItemLimit = Items = 0; Type = Lists::Display;
        Orientation = "VERTICAL";
        DisplayMessage = NULL;
        SelectedItem = NULL;
    }

    void List::OnHide() {
        OnValueChange();
    }

    int List::OnInput(SDL_Event &Event) {
        int Input = 0;

        if (IsShown(true) && HideOnInput) {
            if (Event.type == SDL_KEYDOWN) {
                if (Event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    Hide();
                    Input = 231;
                }
            } else if (Event.type == SDL_MOUSEBUTTONDOWN) {
                if (Event.button.button == SDL_BUTTON_LEFT || Event.button.button == SDL_BUTTON_RIGHT) {
                    if (!Hovered) {
                        if (Parent && !Parent->IsHovered()) {
                            Hide();
                        }
                    }
                }
            }
        }

        return Input;
    }

    void List::OnValueChange() {
        if (DisplayMessage) DisplayMessage->SetText(Value);
    }

    void List::PositionItem(int i, std::list<Element*>::iterator Iterator) {
        Element *ListItem = *Iterator;
        Element *PreviousItem = NULL;

        if (i > 0) {
            Iterator--;
            PreviousItem = *Iterator;
        }

        if (ListItem) {
            //Resize item
            if (Item.X()) ListItem->Resize(Axis::X, Item.X());
            if (Item.Y()) ListItem->Resize(Axis::Y, Item.Y());

            //Position item
            if (Orientation == "VERTICAL") {
                if (i > 0) {
                    if (PreviousItem) {
                        if (ItemLimit) {
                            int Modulus = Items%ItemLimit;
                            if (Modulus == 1 && Items != 1) {
                                ListItem->DropAt(PreviousItem->X(Reference::Local, Position::Edge), 0.f);
                            } else {
                                ListItem->DropAt(PreviousItem->X(Reference::Local), PreviousItem->Y(Reference::Parent, Position::Edge));
                            }
                        } else {
                            if (AutoSize.Y() && Height() && (PreviousItem->Y(Reference::Parent, Position::Edge)+ListItem->Height() > Height()+i*Margin.Y())) {
                                ListItem->DropAt(PreviousItem->X(Reference::Local, Position::Edge), PreviousItem->GetMarginY());
                            } else {
                                ListItem->DropAt(PreviousItem->X(Reference::Local), PreviousItem->Y(Reference::Parent, Position::Edge)+PreviousItem->GetMarginY());
                                //std::cout << ListItem->ID() << " " << ListItem->X(Reference::Parent) << " " << ListItem->Y(Reference::Parent) << " " << PreviousItem->ID() << std::endl;
                            }
                        }
                    }
                } else {
                    ListItem->DropAt(GetMarginX(), GetMarginY());
                    //std::cout << ListItem->ID() << " " << ListItem->X(Reference::Parent) << " " << ListItem->Y(Reference::Parent) << " NULL" << std::endl;
                }
            } else if (Orientation == "HORIZONTAL") {
                if (i > 0) {
                    //Scaled = true;
                    //PreviousItem->EnableScaling(true);
                    //ListItem->EnableScaling(true);

                    if (PreviousItem) {
                        if (ItemLimit) {
                            int Modulus = Items%ItemLimit;
                            if (Modulus == 1 && Items != 1) {
                                ListItem->DropAt(0, PreviousItem->Y(Reference::Parent, Position::Edge));
                            } else {
                                ListItem->DropAt(PreviousItem->X(Reference::Parent, Position::Edge), PreviousItem->Y(Reference::Parent));
                            }
                        } else {
                            if (AutoSize.X() && Width() && (PreviousItem->X(Reference::Parent, Position::Edge)+ListItem->Width() > Width()+i*Margin.X())) {
                                ListItem->DropAt(PreviousItem->GetMarginX(), PreviousItem->Y(Reference::Local, Position::Edge));
                                //if (Name == "TestList") std::cout << "1 " << ListItem->GetID() << " " << Width()+i*Margin.X() << " " << PreviousItem->X(Reference::Parent, Position::Edge)+ListItem->Width() << std::endl;
                            } else {
                                ListItem->DropAt(PreviousItem->X(Reference::Local, Position::Edge)+PreviousItem->GetMarginX(), PreviousItem->Y(Reference::Local));
                                //if (Name == "TestList") std::cout << "2 " << ListItem->GetID() << std::endl;
                                //if (Name == "TestList") std::cout << ListItem->GetID() << " " << ListItem->X(Reference::Parent) << " " << ListItem->Y(Reference::Parent) << " " << PreviousItem->GetID() << std::endl;
                            }
                        }

                        //if (Name == "TestList") std::cout << PreviousItem->GetID() << " " << ListItem->GetID() << std::endl;
                        //if (Name == "TestList") std::cout << PreviousItem->X(Reference::Parent, Position::Edge)+ListItem->Width() << " " << Width() << std::endl;
                    }

                    //PreviousItem->EnableScaling(false);
                    //ListItem->EnableScaling(false);
                    //Scaled = false;
                } else {
                    //if (Name == "TestList") std::cout << "3" << ListItem->GetID() << std::endl;
                    ListItem->DropAt(GetMarginX(), GetMarginY());
                    //std::cout << ListItem->ID() << " " << ListItem->X(Reference::Parent) << " " << ListItem->Y(Reference::Parent) << " NULL" << std::endl;
                }
            }

            //if (Name == "TestList") std::cout << ListItem->GetID() << " " << ListItem->Width() << " " << ListItem->Height() << " " << ListItem->X(Reference::Parent) << " " << ListItem->Y(Reference::Parent) << std::endl;

            if (AutoSize.Y()) Resize(Axis::Y, ListItem->Y(Reference::Parent, Position::Edge)+2*Padding.Y()+Margin.Y());
        }
    }

    void List::RemoveItem(Element *ListItem) {
        Items -= 1;
        ListItems.remove(ListItem);
    }

    void List::Render(float Interpolation) {
        if (IsShown(true) or IsMoving(Axis::XY, true)) {
            UpdateClipping();

            Scaled = true;

            float TargetX = X()+InterpolateX(Interpolation);
            float TargetY = Y()+InterpolateY(Interpolation);

            Renderable.RenderAsDialog(TargetX, TargetY, Width(), Height());
            Scaled = false;
        }
    }

    void List::SetDisplayMessage(Message *DisplayMessage) {
        this->DisplayMessage = DisplayMessage;
    }

    void List::SetItemLimit(int ItemLimit) {
        this->ItemLimit = ItemLimit;
    }

    void List::SetHideOnInput(bool HideOnInput) {
        this->HideOnInput = HideOnInput;
    }

    void List::SetItemSize(int Width, int Height) {
        Item = AtObjects::Vector2(Width, Height);
    }

    void List::SetOrientation(std::string Orientation) {
        this->Orientation = Orientation;
    }

    void List::SetSelectedItem(Element *ListItem) {
        if (SelectedItem != ListItem) {
            if (SelectedItem) {
                SelectedItem->PushEvent(AtObjects::Events::MouseOut);
                AtObjects::Renderable *Renderable = SelectedItem->GetRenderable();
                Renderable->SetColor(AtObjects::Color::Idle);
            }

            SelectedItem = ListItem;

            EventQueue.push_back(AtObjects::Events::ValueChange);
            OnValueChange();

            if (ListItem) {
                Value = SelectedItem->GetValue();

                AtObjects::Renderable *Renderable = ListItem->GetRenderable();
                if (Renderable) {
                    Renderable->SetColor(AtObjects::Color::Hovered);
                }
            } else Value = "";
        }
    }

    void List::SetType(int Type) {
        this->Type = Type;
    }

    void List::UpdateItemPositions() {
        int i = 0;

        if (Orientation == "HORIZONTAL") {
            Resize(Axis::X, 0);
        } else if (Orientation == "VERTICAL") {
            Resize(Axis::Y, 0);
        }

        std::list<Element*>::iterator Iterator;
        for (Iterator=ListItems.begin(); Iterator!=ListItems.end(); Iterator++) {
            PositionItem(i, Iterator);
            i++;
        }
    }

    void List::UpdateSize() {
        if (AllowsAutoSize()) {
            if (Item.X() || Item.Y()) {
                if (Orientation == "VERTICAL") {
                    int Columns = 1, Rows = Items;
                    if (ItemLimit) Columns = Items/ItemLimit;
                    if (Columns > 1 && ItemLimit) Rows = ItemLimit;
                    ResizeTo(Columns*Item.X()+2*Margin.X()+2*Padding.X(), Item.Y()*Rows+(Items+1)*Margin.Y()+2*Padding.Y());
                } else if (Orientation == "HORIZONTAL") {
                    int Rows = 1, Columns = Items;
                    if (ItemLimit < Items && ItemLimit) Rows = Items/ItemLimit;
                    if (Rows > 1 && ItemLimit) Columns = ItemLimit;
                    ResizeTo(Item.X()*Columns+(Items+1)*Margin.X(), Rows*Item.Y()+2*Margin.Y());
                }
            } else {
                float ParentWidth = 0, ParentHeight = 0;
                float ItemWidth = 0, ItemHeight = 0;

                if (Parent) {
                    ParentWidth = Parent->Width()-2*Parent->ChildrenOffsetX()-X(Reference::Parent);
                    ParentHeight = Parent->Height()-2*Parent->ChildrenOffsetY()-Y(Reference::Parent);
                }

                std::list<Element*>::iterator Iterator = ListItems.end();
                Iterator--;
                Element *ListItem = *Iterator;
                if (ListItem) {
                    ItemWidth = ListItem->X(Reference::Parent, Position::Edge);
                    ItemHeight = ListItem->Y(Reference::Parent, Position::Edge);

                    if (Orientation == "VERTICAL") {
                        if (Items != 1) ItemHeight += Height();
                    } else if (Orientation == "HORIZONTAL") {
                        if (Items != 1) ItemWidth += Width();
                    }
                }

                if (ItemWidth > ParentWidth) Resize(Axis::X, ParentWidth); else Resize(Axis::X, ItemWidth);
                if (ItemHeight > ParentHeight) Resize(Axis::Y, ParentHeight); else Resize(Axis::Y, ItemHeight);
            }
        }
    }

    List::~List() {
        Next = Previous = NULL;
        HideOnInput = false;
        ItemLimit = Items = 0; Type = Lists::Display;
        DisplayMessage = NULL;
        SelectedItem = NULL;
    }
}
