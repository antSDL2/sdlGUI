//State.cpp
#include "State.h"
#include <AtObjects/Renderer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <sys/stat.h>

namespace AtGLui {
    void State::BrowseTextboxes() {
        if (Focus && Messages[Focus]) {
            Message *Message = Messages[Focus];
            if (Message->IsFocused()) {
                Message->SetFocused(false);

                Message = Message->Next;
                while (Message && !Message->IsEditable()) {
                    Message = Message->Next;
                    if (!Message) Message = Messages.GetFirst();
                }

                Message->SetFocused(true);
                Focus = Message;
            }
        }
    }

    void State::ClearState() {
        Location = "";

        //OnExit
        Frame *Frame = Frames.GetFirst();
        while (Frame) {
            if (Frame->GetName() == Frame->GetIndex()) {
                AtTools::Lua::ExecuteScript(Lua, Frame->GetIndex() + ":OnExit()");
                AtTools::Lua::ExecuteScript(Lua, Frame->GetIndex() + " = nil");
            }

            Frame = Frame->Next;
        }

        Binds.Clear();
        Buttons.Clear();
        Dialogs.Clear();
        Frames.Clear();
        Lists.Clear();
        Messages.Clear();
        Paragraphs.Clear();
        Sliders.Clear();
        StatusBars.Clear();

        Objects.clear();
        Scripts.clear();

        //Width = NativeWidth; Height = NativeHeight;

        //FontIndex.clear();
        //TextureIndex.clear();

        Focus = DebugFocus = NULL;

        //std::cout << "Cleared Interface state." << std::endl;
    }

    void State::DisableKeyBindings() {
        Binds.SetState(false);
    }

    bool State::ElementAllowsDebug(Element *Element) {
        bool ElementAllowsDebug = false;

        std::string Index = Element->GetIndex();
        Frame *Frame = Frames[Index + "." + Index];

        if (Frame) ElementAllowsDebug = Frame->AllowsDebug();

        return ElementAllowsDebug;
    }

    void State::EnableKeyBindings() {
        Binds.SetState(true);
    }

    AtObjects::Vector2 &State::GetCursorPosition() {
        return CursorPosition;
    }

    std::string State::GetElementClass(std::string ID) {
        std::string Class;

        if (Binds[ID]) {
            Class = "Bind";
        } else if (Buttons[ID]) {
            Class = "Button";
        } else if (Dialogs[ID]) {
            Class = "Dialog";
        } else if (Frames[ID]) {
            Class = "Frame";
        } else if (Lists[ID]) {
            Class = "List";
        } else if (Messages[ID]) {
            Class = "Message";
        } else if (Paragraphs[ID]) {
            Class = "Paragraph";
        } else if (Sliders[ID]) {
            Class = "Slider";
        } else if (StatusBars[ID]) {
            Class = "StatusBar";
        }

        return Class;
    }

    Element *State::GetFocus(bool State) {
        if (State) return DebugFocus; else return Focus;
    }

    void State::GetSharedAttributes(AtXml::Tag &Tag, Element *Element) {
        if (Element) {
            int Trigger = Tag.GetTrigger();

            if (Trigger) {
                while (Tag.HasAttributes()) {
                    AtXml::Attribute Attribute = Tag.GetAttribute();
                    const std::string &AttributeValue = Attribute.GetValue();

                    if (Attribute == "AllowOtherInput") {
                        if (AttributeValue == "true") Element->EnableOtherInput(true); else Element->EnableOtherInput(false);
                    } else if (Attribute == "Anchor") {
                        if (AttributeValue == "CENTER") {
                            Element->AnchorAt(0.5, 0);
                        } else if (AttributeValue == "TOPCENTER") {
                            Element->AnchorAt(0.5, 0.01);
                        } else if (AttributeValue == "MIDDLECENTER") {
                            Element->AnchorAt(0.5, 0.5);
                        } else if (AttributeValue == "BOTTOMCENTER") {
                            Element->AnchorAt(0.5, 1);
                        } else if (AttributeValue == "RIGHT") {
                            Element->AnchorAt(1, 0);
                        } else if (AttributeValue == "TOPRIGHT") {
                            Element->AnchorAt(1, 0.01);
                        } else if (AttributeValue == "MIDDLERIGHT") {
                            Element->AnchorAt(1, 0.5);
                        } else if (AttributeValue == "BOTTOMRIGHT") {
                            Element->AnchorAt(1, 1);
                        } else if (AttributeValue == "LEFT") {
                            Element->AnchorAt(0.01, 0);
                        } else if (AttributeValue == "TOPLEFT") {
                            Element->AnchorAt(0.01, 0.01);
                        } else if (AttributeValue == "MIDDLELEFT") {
                            Element->AnchorAt(0.01, 0.5);
                        } else if (AttributeValue == "BOTTOMLEFT") {
                            Element->AnchorAt(0.01, 1);
                        } else if (AttributeValue == "MIDDLE") {
                            Element->AnchorAt(0, 0.5);
                        } else if (AttributeValue == "BOTTOM") {
                            Element->AnchorAt(0, 1);
                        } else {
                            float HorizontalAnchor = AtTools::Strings::StringTo<float>(AttributeValue, ' ');
                            float VerticalAnchor = AtTools::Strings::StringTo<float>(AttributeValue, ' ', 1);
                            Element->AnchorAt(HorizontalAnchor, VerticalAnchor);
                        }
                    } else if (Attribute == "AutoSize") {
                        if (AttributeValue == "true") Element->EnableAutoSize(true); else Element->EnableAutoSize(false);
                    } else if (Attribute == "ClipContents") {
                        if (AttributeValue == "true") Element->SetClipContents(true); else Element->SetClipContents(false);
                    } else if (Attribute == "Color") {
                        int Color[4]; Color[0] = Color[1] = Color[2] = Color[3] = -1;
                        for (int i = 0; i<3; i++) Color[i] = AtTools::Strings::StringTo<int>(AttributeValue, ' ', i);
                        AtObjects::Renderable *Renderable = Element->GetRenderable();
                        Renderable->SetIdleColor(Color);
                    } else if (Attribute == "Container") {
                        if (AttributeValue == "true") Element->SetContainer(true); else Element->SetContainer(false);
                    } else if (Attribute == "CopyInput") {
                        if (AttributeValue == "true") Element->SetCopyInput(true); else Element->SetCopyInput(false);
                    } else if (Attribute == "Disabled") {
                        if (AttributeValue == "true") Element->SetDisabled(true); else Element->SetDisabled(false);
                    } else if (Attribute == "Font") {
                        if (Tag == "Message" || Tag == "Paragraph") {
                            SetElementFont(Element->GetID(), AttributeValue);
                        }
                    } else if (Attribute == "Height") {
                        int Height = AtTools::Strings::StringTo<int>(AttributeValue);
                        Element->Resize(Axis::Y, Height);
                    } else if (Attribute == "Interactive") {
                        bool State = false;
                        if (AttributeValue == "true") State = true; else State = false;
                        Element->GetInteractable()->EnableInteraction(State);
                    } else if (Attribute == "Movable") {
                        bool State = false;
                        if (AttributeValue == "true") State = true; else State = false;
                        Element->GetInteractable()->EnableMoving(State);
                    } else if (Attribute == "Offset") {
                        float X = AtTools::Strings::StringTo<float>(AttributeValue, ' ');
                        float Y = AtTools::Strings::StringTo<float>(AttributeValue, ' ', 1);
                        Element->DropAt(X, Y);
                    } else if (Attribute == "OffsetX") {
                        float X = AtTools::Strings::StringTo<float>(AttributeValue);
                        Element->Offset(Axis::X, X);
                    } else if (Attribute == "OffsetY") {
                        float Y = AtTools::Strings::StringTo<float>(AttributeValue);
                        Element->Offset(Axis::Y, Y);
                    } else if (Attribute == "Margin") {
                        float X = AtTools::Strings::StringTo<float>(AttributeValue, ' ');
                        float Y = AtTools::Strings::StringTo<float>(AttributeValue, ' ', 1);
                        Element->SetMargin(X, Y);
                    } else if (Attribute == "MaxVelocity") {
                        float MaxVelocity = AtTools::Strings::StringTo<float>(AttributeValue);
                        Element->LimitVelocity(MaxVelocity);
                    } else if (Attribute == "Padding") {
                        float X = AtTools::Strings::StringTo<float>(AttributeValue, ' ');
                        float Y = AtTools::Strings::StringTo<float>(AttributeValue, ' ', 1);
                        Element->AddPadding(X, Y);
                    } else if (Attribute == "Resistance") {
                        int Resistance = AtTools::Strings::StringTo<int>(AttributeValue);
                        Element->ResistDrag(Resistance);
                    } else if (Attribute == "Resizable") {
                        bool State = false;
                        if (AttributeValue == "true") State = true; else State = false;
                        Element->GetInteractable()->EnableResizing(State);
                    } else if (Attribute == "Shape") {
                        if (AttributeValue == "Disk") {
                            Element->GetInteractable()->Shape = AtObjects::Shapes::Disk;
                        }
                    } else if (Attribute == "ShareInput") {
                        if (AttributeValue == "true") Element->SetShareInput(true); else Element->SetShareInput(false);
                    } else if (Attribute == "Shown") {
                        if (AttributeValue == "true") Element->Show(); else Element->Hide();
                    } else if (Attribute == "Texture") {
                        TextureManager.SetObjectTexture(Element, AttributeValue);
                    } else if (Attribute == "Value") {
                        Element->SetValue(AttributeValue);
                    } else if (Attribute == "Width") {
                        int Width = AtTools::Strings::StringTo<int>(AttributeValue);
                        Element->Resize(Axis::X, Width);
                    }
                }
            }
        }
   }

    void State::GetSpecificAttributes(AtXml::Tag &Tag, std::vector<std::string> &Data) {
        int Trigger = Tag.GetTrigger();
        std::string Text = Tag.GetText();

        if (Trigger) {
            if (Tag == "Bind") {
                Data.resize(3);
                Data[0] = Data[1] = Data[2] = "0";
            } else if (Tag == "Button" || Tag == "Checkbox" || Tag == "DropDownButton") {
                Data.resize(2);
                Data[0] = Data[1] = "";
            } else if (Tag == "Frame" || Tag == "Interface") {
                Data.resize(2);
                Data[0] = Data[1] = "";
            } else if (Tag == "List") {
                Data.resize(6);
                Data[0] = Data[1] = Data[2] = Data[3] = Data[4] = Data[5] = "";
            } else if (Tag == "Slider") {
                Data.resize(5);
                Data[0] = Data[1] = Data[2] = Data[3] = Data[4] = "";
            } else if (Tag == "StatusBar") {
                Data.resize(3);
                Data[0] = Data[1] = Data[2] = "";
            } else if (Tag == "Message" || Tag == "Paragraph") {
                Data.resize(12);
                Data[0] = Data[1] = Data[2] = Data[3] = Data[4] = Data[5] = Data[6] = Data[7] = Data[8] = Data[9] = Data[10] = Data[11] = "";
            } else {
                if (Tag == "Font") {
                    Data.resize(6);
                    Data[0] = Text;
                    Data[1] = Data[2] = Data[3] = Data[4] = Data[5] = "";
                } else if (Tag == "Texture") {
                    Data.resize(6);
                    Data[0] = Text; Data[1] = Data[2] = Data[3] = Data[4] = Data[5] = "";
                }
            }

            while (Tag.HasAttributes()) {
                AtXml::Attribute Attribute = Tag.GetAttribute();
                const std::string &AttributeValue = Attribute.GetValue();

                int i = -1;
                if (Tag == "Bind") {
                    if (Attribute == "Key") {
                        i = 0;
                    } else if (Attribute == "Modifier") {
                        i = 1;
                    } else if (Attribute == "Repeat") {
                        i = 2;
                    }
                } else if (Tag == "Button") {
                    if (Attribute == "Type") {
                        i = 0;
                    }
                } else if (Tag == "Checkbox") {
                    Data[0] = "Checkbox";

                    if (Attribute == "Checked") {
                        i = 1;
                    }
                } else if (Tag == "DropDownButton") {
                    Data[0] = "DropDown";
                } else if (Tag == "List") {
                    if (Attribute == "Orientation") {
                        i = 0;
                    } else if (Attribute == "Type") {
                        i = 1;
                    } else if (Attribute == "Value") {
                        i = 2;
                    } else if (Attribute == "ItemSize") {
                        i = 3;
                    } else if (Attribute == "ItemLimit") {
                        i = 4;
                    } else if (Attribute == "HideOnInput") {
                        i = 5;
                    }
                } else if (Tag == "Frame" || Tag == "Interface") {
                    if (Attribute == "AllowDebug") {
                        i = 0;
                    } else if (Tag == "Interface" && Attribute == "Ambience"){
                        i = 1;
                    }
                } else if (Tag == "Slider") {
                    if (Attribute == "Status") {
                        i = 0;
                    } else if (Attribute == "MaximumValue") {
                        i = 1;
                    } else if (Attribute == "MinimumValue") {
                        i = 2;
                    } else if (Attribute == "Orientation") {
                        i = 3;
                    } else if (Attribute == "Type") {
                        i = 4;
                    }
                } else if (Tag == "StatusBar") {
                    if (Attribute == "Status") {
                        i = 0;
                    } else if (Attribute == "Orientation") {
                        i = 1;
                    } else if (Attribute == "Direction") {
                        i = 2;
                    }
                } else if (Tag == "Message" || Tag == "Paragraph") {
                    if (Attribute == "Size") {
                        i = 1;
                    } else if (Attribute == "Color") {
                        i = 2;
                    } else if (Attribute == "Transparency") {
                        i = 3;
                    } else if (Attribute == "HoverColor") {
                        i = 4;
                    } else if (Attribute == "Style") {
                        i = 5;
                    } else if (Attribute == "Text") {
                        i = 6;
                    } else if (Attribute == "ShadowOffset") {
                        i = 7;
                    } else if (Attribute == "ShadowColor") {
                        i = 8;
                    } else if (Attribute == "Editable") {
                        i = 9;
                    } else if (Attribute == "CharacterLimit") {
                        i = 10;
                    } else if (Attribute == "Focused") {
                        i = 11;
                    }
                } else {
                    if (Tag == "Font") {
                        if (Attribute == "Size") {
                            i = 1;
                        } else if (Attribute == "Color") {
                            i = 2;
                        } else if (Attribute == "Transparency") {
                            i = 3;
                        } else if (Attribute == "HoverColor") {
                            i = 4;
                        } else if (Attribute == "Style") {
                            i = 5;
                        } else if (Attribute == "Text") {
                            i = 6;
                        }
                    } else if (Tag == "Texture") {
                        if (Attribute == "Tile") {
                            i = 1;
                        } else if (Attribute == "Color") {
                            i = 2;
                        } else if (Attribute == "Transparency") {
                            i = 3;
                        } else if (Attribute == "HoverColor") {
                            i = 4;
                        } else if (Attribute == "LinearFiltering") {
                            i = 5;
                        }
                    }
                }

                if (i > -1) Data[i] = AttributeValue;
            }
        }
    }

    AtObjects::TextureManager *State::GetTextureManager() {
        return &TextureManager;
    }

    void State::HandleEvents() {
        //Handle manager events
        for (unsigned int i=0; i<EventQueue.size(); i++) {
            int Event = EventQueue[i];

            if (Event == Events::DebugFocusChanged) {
                std::string Arguments;
                if (DebugFocus) {
                    Arguments = "\""+ DebugFocus->GetID() + "\"";
                } else Arguments = "\"\"";

                Scripts.push_back("Interface.OnGlobalEvent(\"DebugFocusChanged\", " + Arguments + ");");
            }
        }
        EventQueue.clear();

        //TEMP PERFORMANCE CONCERN
        int Count = 0;
        do {
            HandleListEvents(Frames);
            HandleListEvents(Binds);
            HandleListEvents(Buttons);
            HandleListEvents(Dialogs);
            HandleListEvents(Lists);
            HandleListEvents(Messages);
            HandleListEvents(Paragraphs);
            HandleListEvents(Sliders);
            HandleListEvents(StatusBars);

            Count = Scripts.size();

            //Execute scripts
            for (unsigned int i=0; i<Scripts.size(); i++) {
                if (Debug[1]) std::cout << "[" << SDL_GetTicks() << "] " << Scripts[i] << std::endl;
                AtTools::Lua::ExecuteScript(Lua, Scripts[i]);
            }
            Scripts.clear();
        } while (Count);
    }

    template <class IDType, class Type, class InheritedType>
    void State::HandleListEvents(AtObjects::List<IDType, Type, InheritedType> &List) {
        Type *Object = List.GetFirst();

        while (Object) {
            int Event = 0;
            Event = Object->EventHandler();

            if (Event) {
                if (Event == AtObjects::Events::Load) {
                    PushElementCallback(Object, "OnLoad");

                    Slider *Slider = Sliders[Object];
                    if (Slider && Slider->GetType() == Sliders::Drag) {
                        InheritedType *Parent = Slider->GetParent();

                        if (Parent) Parent->SetClipContents(true);
                    }
                } else {
                    //Call Lua functions
                    if (Event == AtObjects::Events::Hide) {
                        PushElementCallback(Object, "OnHide");
                    } else if (Event == AtObjects::Events::LeftButtonDown) {
                        PushElementCallback(Object, "OnInteract");

                        //Textbox Focus assignment
                        if (Buttons[Object] || ((Messages[Object]) && Messages[Object]->IsEditable())) {
                            SetFocus(Object);
                        }
                    } else if (Event == AtObjects::Events::LeftButtonUp) {
                        PushElementCallback(Object, "OnClick");

                        //List Items
                        if (Lists[Object->GetParent()] && !Sliders[Object]) {
                            class List *ParentList = Lists[Object->GetParent()];

                            //Element *SelectedItem = ParentList->GetSelectedItem();

                            //Set SelectedItem
                            if (!Object->IsDisabled()) {
                                if (ParentList->GetType() == Lists::Selection) {
                                    ParentList->SetSelectedItem(Object);
                                    if (ParentList->HidesOnInput()) ParentList->Hide();
                                }
                            }
                        }

                        bool AllowDebug = ElementAllowsDebug(Object);
                        if (Debug[0]&&AllowDebug) {
                            EventQueue.push_back(Events::DebugFocusChanged);
                        }
                    } else if (Event == AtObjects::Events::MouseIn) {
                        PushElementCallback(Object, "OnMouseIn");
                    } else if (Event == AtObjects::Events::MouseOut) {
                        //Reorder Mouse Out
                        std::string Back;
                        if (Scripts.size() > 0) {
                            Back = Scripts.back();
                            Scripts.pop_back();
                        }

                        PushElementCallback(Object, "OnMouseOut");

                        if (Back != "") {
                            Scripts.push_back(Back);
                        }

                        //List Items - Show Selection
                        if (Lists[Object->GetParent()]) {
                            class List *ParentList = Lists[Object->GetParent()];

                            if (ParentList->GetSelectedItem() == Object) {
                                AtObjects::Renderable *Renderable = Object->GetRenderable();
                                if (Renderable) {
                                    Renderable->SetColor(AtObjects::Color::Hovered);
                                }
                            }
                        }
                    } else if (Event == AtObjects::Events::Move) {
                        PushElementCallback(Object, "OnMove");
                    } else if (Event == AtObjects::Events::RightButtonDown) {
                        PushElementCallback(Object, "OnInteract");

                        //Textbox Focus assignment
                        if (Buttons[Object] || ((Messages[Object]) && Messages[Object]->IsEditable())) {
                            SetFocus(Object);
                        }
                    } else if (Event == AtObjects::Events::RightButtonUp) {
                        PushElementCallback(Object, "OnRightClick");
                    } else if (Event == AtObjects::Events::Show) {
                        PushElementCallback(Object, "OnShow");

                        ResetRelatedTo(Object);

                        //TEMP Frame float
                        std::string ObjectID = Object->GetID();
                        Frame *Frame = Frames[ObjectID];
                        if (Frame && Frame->IsContainer()) {
                            Frames.MoveToLast(Frame);

                            //TEMP NECESSARY?
                            //ResetInput();
                        }

                        //TEMP Focus Dialog
                        Dialog *Dialog = Dialogs[ObjectID];
                        if (Dialog) {
                            Dialog->SetFocused(true);
                        }
                    } else if (Event == AtObjects::Events::Stop) {
                        PushElementCallback(Object, "OnStop");
                    } else if (Event == AtObjects::Events::ValueChange) {
                        PushElementCallback(Object, "OnValueChange");

                        if (Messages[Object]) {
                            Element *Parent = Object->GetParent();

                            if (Lists[Parent]) {
                                class List *ParentList = Lists[Parent];
                                ParentList->UpdateItemPositions();
                                ParentList->UpdateSize();
                            }
                        }
                    } else if (Event == AtObjects::Events::Return) {
                        PushElementCallback(Object, "OnReturn");
                    } else {
                        if (Binds[Object]) {
                            if (Event == Events::Press) {
                                PushElementCallback(Object, "OnPress");
                            } else if (Event == Events::Release) {
                                PushElementCallback(Object, "OnRelease");
                            }
                        }
                    }
                }
            }
            Object = Object->Next;
        }
    }

    void State::Hide() {
        Interface.Hide();
    }

    void State::Initialize(std::string AssetsDirectory, std::string GraphicsDirectory, std::string UserDirectory) {
        if (this->AssetsDirectory == "") this->AssetsDirectory = AssetsDirectory;
        if (this->UserDirectory == "") this->UserDirectory = UserDirectory;
        TextureManager.SetGraphicsDirectory(GraphicsDirectory);

        std::cout << "Loading Interface assets: " << AssetsDirectory << std::endl;
        LoadFontIndex(AssetsDirectory+"Fonts/");
        LoadTextureIndex(AssetsDirectory+"Textures/");
        LoadStateIndex(AssetsDirectory+"States/");
    }

    int State::Input(SDL_Event &Event) {
        int Input = 0;

        //Editor Input
        if (Debug[0]) {
            if (Event.type == SDL_KEYDOWN) {
                switch (Event.key.keysym.scancode) {
                    case SDL_SCANCODE_F10:
                        Who();
                        Input = 101;
                        break;
                    case SDL_SCANCODE_F11:
                        TextureManager.Who();
                        std::cout << std::endl;
                        break;
                    case SDL_SCANCODE_DELETE: {
                        /*
                        if (DebugFocus) {
                            Remove(DebugFocus);
                            DebugFocus = NULL;
                            Input = 103;
                        }
                        */
                    }
                        break;
                    default:
                        break;
                }
            }
        }

        if (!Input) {
            Input = InputList(Frames, Event);
            if (!Input) {
                Input = InputFocus(Event);
            }
        }

        if (!Input) {
            if (Event.type == SDL_MOUSEBUTTONDOWN) {
                if (Event.button.button == SDL_BUTTON_LEFT) {
                    SetFocus(NULL);
                    SetFocus(NULL, true);
                }
            }
        }

        return Input;
    }

    int State::InputFocus(SDL_Event &Event) {
        int Input = 0;

        if (Focus) {
            std::string Index = Focus->GetIndex();
            if (Event.type == SDL_KEYDOWN) {
                if (Focus->IsShown(true) && (Event.key.keysym.scancode == SDL_SCANCODE_RETURN || Event.key.keysym.scancode == SDL_SCANCODE_KP_ENTER)) {
                    //Call Name:OnReturn();
                    Focus->PushEvent(AtObjects::Events::Return);
                    Focus->PushEvent(AtObjects::Events::LeftButtonUp, true);

                    Input = 111;

                    //Textbox focus reset
                    if (Messages[Focus]) {
                        Message *Message = Messages[Focus];
                        Message->SetFocused(false);
                    }
                }
            }
        }

        return Input;
    }

    template <class IDType, class Type, class InheritedType>
    int State::InputList(AtObjects::List<IDType, Type, InheritedType> &List, SDL_Event &Event, InheritedType *Parent) {
        int Input = 0;

        if (!Parent) Parent = &Interface;

        Type *Object = List.GetLast();

        int i = 0;

        while (Object && List.IsEnabled()) {
            if (!Input) {
                if (Object->GetParent() == Parent) {
                    //if (Frames[Object]) std::cout << i << " " << Object->GetIndex() << " " << Object->ID() << std::endl;
                    if (Object->IsContainer()) {
                        Input = InputRelatedTo(Object, Event);

                        //CopyInput from Children
                        //if (Input && Object->ID() == "EquipmentFrame") std::cout << Input << std::endl;
                        if (Input) {
                            if (Object->CopiesInput()) {
                                if (Input == AtObjects::Events::LeftButtonUp) {
                                    Object->PushEvent(AtObjects::Events::LeftButtonUp);
                                } else if (Input == AtObjects::Events::LeftButtonDown) {
                                    if (List.GetInputResponse() > 0) List.MoveToLast(Object); else if (List.GetInputResponse() < 0) List.SwapItems(List.GetFirst(), Object);
                                    Object->PushEvent(AtObjects::Events::LeftButtonDown);
                                } else if (Input == AtObjects::Events::RightButtonDown) {
                                    Object->PushEvent(AtObjects::Events::RightButtonDown);
                                } else if (Input == AtObjects::Events::RightButtonUp) {
                                    Object->PushEvent(AtObjects::Events::RightButtonUp);
                                } else if (Input == AtObjects::Events::MouseIn) {
                                    Object->PushEvent(AtObjects::Events::MouseIn);
                                }
                            } else Object->ResetInput();
                        }
                    }

                    if (!Input) {
                        bool AllowDebug = ElementAllowsDebug(Object);

                        Input = Object->Input(Event, (Debug[0]&&AllowDebug));

                        if (Input == AtObjects::Events::LeftButtonDown || Input == AtObjects::Events::RightButtonDown) {
                            //DebugFocus assignment
                            SetFocus(Object, true);

                            //Rearrange list
                            if (List.GetInputResponse() > 0) List.MoveToLast(Object); else if (List.GetInputResponse() < 0) List.SwapItems(List.GetFirst(), Object);
                        } else if (!Input && !(Debug[0]&&AllowDebug)) {
                            //ShareInput from parent
                            if (Parent && Parent->SharesInput() && !Object->GetInteractable()->IsInteractive()) {
                                if (!Parent->IsDisabled()) {
                                    //MouseIn/MouseOut
                                    if (Parent->IsHovered()) {
                                        Object->PushEvent(AtObjects::Events::MouseIn);
                                    } else {
                                        Object->PushEvent(AtObjects::Events::MouseOut);
                                    }
                                }

                                if (Parent->IsClicked(1)) {
                                    Object->PushEvent(AtObjects::Events::LeftButtonDown);
                                } else if (Parent->IsClicked(3)) {
                                    Object->PushEvent(AtObjects::Events::RightButtonDown);
                                } else {
                                    if (Object->IsClicked(1)) Object->PushEvent(AtObjects::Events::LeftButtonUp);
                                    if (Object->IsClicked(3)) Object->PushEvent(AtObjects::Events::RightButtonUp);
                                }
                            }
                        }

                        if (Input && Object->AllowsOtherInput() && !(Debug[0]&&AllowDebug)) Input = 0;

                        if (!Object->IsContainer()) {
                            if (!Input) {
                                Input = InputRelatedTo(Object, Event);
                            } else ResetRelatedTo(Object);
                        }
                    }
                }
            } else {
                if (Object->GetParent() == Parent && !Object->AllowsOtherInput()) {
                    Object->ResetInput();
                    ResetRelatedTo(Object);
                }
            }

            Object = Object->Previous;
            i++;
        }

        return Input;
    }

    int State::InputRelatedTo(Element *Parent, SDL_Event &Event) {
        int Input = 0;

        //TEMP Review ordering necessity
        Input = InputList(Binds, Event, Parent);
        if (!Input) {
            Input = InputList(Frames, Event, Parent);
            if (!Input) {
                Input = InputList(Dialogs, Event, Parent);
                if (!Input) {
                    Input = InputList(Sliders, Event, Parent);
                    if (!Input) {
                        Input = InputList(Buttons, Event, Parent);
                        if (!Input) {
                            Input = InputList(Paragraphs, Event, Parent);
                                if (!Input) {
                                    Input = InputList(Lists, Event, Parent);
                                    if (!Input) {
                                        Input = InputList(StatusBars, Event, Parent);
                                        if (!Input) {
                                            Input = InputList(Messages, Event, Parent);
                                            if (!Input) {
                                            } else ResetAllExcept(Lists::Messages, Parent);
                                        } else ResetAllExcept(Lists::StatusBars, Parent);
                                    } else ResetAllExcept(Lists::Lists, Parent);
                                } else ResetAllExcept(Lists::Paragraphs, Parent);
                        } else ResetAllExcept(Lists::Buttons, Parent);
                    } else ResetAllExcept(Lists::Sliders, Parent);
                } else ResetAllExcept(Lists::Dialogs, Parent);
            } else ResetAllExcept(Lists::Frames, Parent);
        }

        return Input;
    }

    bool State::IsShown() {
        return Interface.IsShown();
    }

    void State::Load(std::string StateName, Element *Parent, bool Embedded) {
        std::string Location = StateIndex[StateName];

        AddOnRequirements[StateName] = true;

        if (Embedded) {
            if (Frames[StateName+"."+StateName]) {
                Remove(Frames[StateName+"."+StateName]);
                std::cout << "\tRemoved: " << StateName << std::endl;
            }
        }

        if (!Frames[StateName+"."+StateName] || Embedded) {
            AtXml::File File;
            if (File.Parse(Location)) {
                std::cout << "\tLoading: " << StateName << std::endl;
                std::string Scripts;
                std::string AddOn = StateName;

                if (!Parent) Parent = &Interface;

                bool BindingsTagOpen = false, FontsTagOpen = false, ScriptsTagOpen = false, TexturesTagOpen = false;
                while (File.HasTags()) {
                    AtXml::Tag Tag = File.GetTag();
                    int Trigger = Tag.GetTrigger();
                    std::string Text = Tag.GetText();

                    std::vector<std::string> Data;
                    std::string Name;

                    if (Trigger) {
                        while (Tag.HasAttributes()) {
                            AtXml::Attribute Attribute = Tag.GetAttribute();
                            const std::string &AttributeValue = Attribute.GetValue();

                            if (Attribute == "Name") {
                                Name = AttributeValue;
                            } else {
                                if (Tag == "Interface") {
                                    if (Attribute == "Scripts" && !Embedded) {
                                        Scripts = AttributeValue;
                                    }
                                }
                            }
                        }
                    }

                    if (FontsTagOpen) {
                        if (Tag == "Font") {
                            if (Trigger) {
                                GetSpecificAttributes(Tag, Data);
                                FontIndex[Name] = Data;
                            }
                        } else if (Tag == "Fonts" && Trigger == AtXml::Trigger::Close) {
                            FontsTagOpen = false;
                        }
                    } else if (ScriptsTagOpen) {
                        if (Tag == "Scripts" && Trigger == AtXml::Trigger::Close) {
                            ScriptsTagOpen = false;
                        } else {
                            if (Parent) {
                                if (Text != "") {
                                    std::string Script = "function " + Parent->GetID() + ":" + Tag.GetName();

                                    if (Tag == "OnMouseButton") {
                                        Script += "(State, Button)\n" + Text + "\n end";
                                    } else {
                                        Script += "()\n" + Text + "\n end";
                                    }

                                    AtTools::Lua::ExecuteScript(Lua, Script);
                                }
                            }
                        }
                    } else if (TexturesTagOpen) {
                        if (Tag == "Texture") {
                            if (Trigger) {
                                GetSpecificAttributes(Tag, Data);
                                TextureManager.SetTextureIndex(Name, Data);
                            }
                        } else if (Tag == "Textures" && Trigger == AtXml::Trigger::Close) {
                            TexturesTagOpen = false;
                        }
                    } else {
                        if (Tag == "Bindings") {
                            if (Trigger == AtXml::Trigger::Open) {
                                if (Text != "") {
                                    this->Load(Text, Parent);
                                } else {
                                    BindingsTagOpen = true;
                                }
                            } else if (Trigger == AtXml::Trigger::Close) BindingsTagOpen = false;
                        } else if (Tag == "Embed") {
                            if (Trigger == AtXml::Trigger::Open) {
                                if (Text != "") {
                                    this->Load(Text, Parent, true);
                                }
                            }
                        } else if (Tag == "Fonts") {
                            if (Trigger == AtXml::Trigger::Open) {
                                if (Text != "") {
                                    this->Load(Text, Parent);
                                } else {
                                    FontsTagOpen = true;
                                }
                            }
                        } else if (Tag == "Load") {
                            if (Trigger == AtXml::Trigger::Open) {
                                if (Text != "") {
                                    this->Load(Text);
                                }
                            }
                        } else if (Tag == "Scripts") {
                            if (Parent && Trigger == AtXml::Trigger::Open) {
                                ScriptsTagOpen = true;
                            }
                        } else if (Tag == "Textures") {
                            if (Trigger == AtXml::Trigger::Open) {
                                if (Text != "") {
                                    this->Load(Text, Parent);
                                } else {
                                    TexturesTagOpen = true;
                                }
                            }
                        } else {
                            if (Trigger) {
                                //Autoname the Interface frame
                                if (Name == "") {
                                    if (Tag == "Interface" && !Frames[AddOn+"."+StateName]) {
                                        Name = AddOn;
                                    } else {
                                        if (Parent) {
                                            if (Lists[Parent]) {
                                                List *ListParent = Lists[Parent];
                                                std::stringstream NameStream;
                                                NameStream << Parent->GetName() << "Item" << ListParent->GetItems()+1;
                                                Name = NameStream.str();
                                                //std::cout << "   Autonamed: " << Name << std::endl;
                                            } else {
                                                std::stringstream NameStream;
                                                int i = 0;
                                                do {
                                                    NameStream.str(std::string());
                                                    NameStream << Tag.GetName() << SDL_GetTicks()+i;
                                                    i++;
                                                } while (Objects[Parent->GetIndex()+"."+NameStream.str()]);

                                                Name = NameStream.str();
                                                //std::cout << "   Autonamed: " << Name << std::endl;
                                            }
                                        }
                                    }
                                }
                            }

                            if (BindingsTagOpen) {
                                if (Tag == "Bind") {
                                    if (Trigger) {
                                        MakeElement(Lists::Binds, AddOn, Name, Parent);
                                        Bind *Bind = Binds[AddOn+"."+Name];
                                        if (Bind) {
                                            LoadElement(Bind, Tag);
                                        }
                                    }
                                }
                            } else {
                                if (Tag == "Button" || Tag == "Checkbox" || Tag == "DropDownButton") {
                                    if (Trigger) {
                                        MakeElement(Lists::Buttons, AddOn, Name, Parent);
                                        Button *Button = Buttons[AddOn+"."+Name];
                                        if (Button) {
                                            LoadElement(Button, Tag);
                                            if (Trigger == AtXml::Trigger::Open) Parent = Button;
                                        }
                                    } else if (Buttons[Parent]) Parent = Parent->GetParent();
                                } else if (Tag == "Dialog") {
                                    if (Trigger) {
                                        MakeElement(Lists::Dialogs, AddOn, Name, Parent);
                                        Dialog *Dialog = Dialogs[AddOn+"."+Name];
                                        if (Dialog) {
                                            LoadElement(Dialog, Tag);
                                            if (Trigger == AtXml::Trigger::Open) Parent = Dialog;
                                        }
                                    } else if (Dialogs[Parent]) Parent = Parent->GetParent();
                                } else if (Tag == "Frame" || Tag == "Image" || Tag == "iFrame") {
                                    if (Trigger) {
                                        if (Tag == "Frame" || Tag == "Image" || !Parent || Tag == "iFrame") {
                                            MakeElement(Lists::Frames, AddOn, Name, Parent);
                                            Frame *Frame = Frames[AddOn+"."+Name];
                                            if (Frame) {
                                                if (Tag == "Image") Frame->SetContainer(false); else if (Tag == "iFrame") Frame->GetInteractable()->EnableInteraction(true);
                                                if (!Parent) Frame->ResizeTo(Resolution.X(), Resolution.Y());
                                                LoadElement(Frame, Tag);
                                                if (Trigger == AtXml::Trigger::Open) Parent = Frame;
                                            }
                                        }
                                    } else if (Frames[Parent]) {
                                        Parent = Parent->GetParent();
                                    }
                                } else if (Tag == "Interface") {
                                    if (Trigger) {
                                        Frame *Frame = NULL;
                                        Frame = MakeObject(Frames, AddOn, Name, Parent);
                                        if (Frame) {
                                            AtTools::Lua::ExecuteScript(Lua, Name + " = Frame.New('" + AddOn + "." + Name + "')");
                                            LoadElement(Frame, Tag);
                                            if (Trigger == AtXml::Trigger::Open) Parent = Frame;
                                        }
                                    } else if (Frames[Parent]) {
                                        if (Scripts != "") {
                                            AtTools::Lua::ExecuteFile(Lua, Scripts);
                                        }
                                    }
                                } else if (Tag == "List") {
                                    if (Trigger) {
                                        MakeElement(Lists::Lists, AddOn, Name, Parent);
                                        List *List = Lists[AddOn+"."+Name];
                                        if (List) {
                                            LoadElement(List, Tag);
                                            if (Trigger == AtXml::Trigger::Open) Parent = List;
                                        }
                                    } else if (Lists[Parent]) Parent = Parent->GetParent();
                                } else if (Tag == "Message") {
                                    if (Trigger) {
                                        MakeElement(Lists::Messages, AddOn, Name, Parent);
                                        Message *Message = Messages[AddOn+"."+Name];
                                        if (Message) {
                                            LoadElement(Message, Tag);
                                            if (Trigger == AtXml::Trigger::Open) Parent = Message;
                                        }
                                    } else if (Messages[Parent]) Parent = Parent->GetParent();
                                } else if (Tag == "Paragraph") {
                                    if (Trigger) {
                                        MakeElement(Lists::Paragraphs, AddOn, Name, Parent);
                                        Paragraph *Paragraph = Paragraphs[AddOn+"."+Name];
                                        if (Paragraph) {
                                            LoadElement(Paragraph, Tag);
                                            if (Trigger == AtXml::Trigger::Open) Parent = Paragraph;
                                        }
                                    } else if (Paragraphs[Parent]) Parent = Parent->GetParent();
                                } else if (Tag == "Slider") {
                                    if (Trigger) {
                                        MakeElement(Lists::Sliders, AddOn, Name, Parent);
                                        Slider *Slider = Sliders[AddOn+"."+Name];
                                        if (Slider) {
                                            LoadElement(Slider, Tag);
                                            if (Trigger == AtXml::Trigger::Open) Parent = Slider;
                                        }
                                    } else if (Sliders[Parent]) Parent = Parent->GetParent();
                                } else if (Tag == "StatusBar") {
                                    if (Trigger) {
                                        MakeElement(Lists::StatusBars, AddOn, Name, Parent);
                                        StatusBar *StatusBar = StatusBars[AddOn+"."+Name];
                                        if (StatusBar) {
                                            LoadElement(StatusBar, Tag);
                                            if (Trigger == AtXml::Trigger::Open) Parent = StatusBar;
                                        }
                                    } else if (StatusBars[Parent]) Parent = Parent->GetParent();
                                } else std::cerr << "(Interface/State.cpp) Load(): Uknown tag '" << Tag.GetName() << "' in " << Location << "." << std::endl;
                            }
                        }
                    }
                }
            } else std::cerr << "(Interface/State.cpp) Load(): Unable to parse file '" << Location << "'." << std::endl;
        }
    }

    void State::LoadAddOnData(std::string AddOn) {
        Frame *Frame = Frames[AddOn+"."+AddOn];
        if (Frame) {
            std::string Index = Frame->GetIndex();
            std::string Name = Frame->GetName();

            if (Index == Name) {
                std::string Location = UserDirectory + "AddOnData/" + Index + ".lua";

                struct stat buffer;
                if (stat (Location.c_str(), &buffer) == 0) {
                    AtTools::Lua::ExecuteFile(Lua, Location);
                }
            }
        }
    }

    void State::LoadCursor(std::string Name) {
        if (Cursor) SDL_FreeCursor(Cursor);

        AtObjects::Texture *Texture = TextureManager.LoadTexture(Name);
        if (Texture) {
            GLubyte Data[4*Texture->GetWidth()*Texture->GetHeight()];
            Texture->GetData(Data);

            Cursor = AtObjects::Renderer::CreateCursorFromData(Data, Texture->GetWidth(), Texture->GetHeight());
            if (Cursor) {
                SDL_SetCursor(Cursor);
            }
        }
    }

    void State::LoadElement(Element *Element, AtXml::Tag &Tag) {
        std::vector<std::string> Data;

        if (Element) {
            std::string Index = Element->GetIndex();

            if(Frames[Element]) {
                if (Tag == "Image") {
                    Element->EnableAutoSize(false);
                }
            }

            if (Tag.HasAttributes()) {
                GetSharedAttributes(Tag, Element);
                GetSpecificAttributes(Tag, Data);
            }

            if (Binds[Element]) {
                if (Data.size() < 3) {
                    if (Data.size() > 0) std::cerr << "(Interface/State.cpp) LoadElement(): Missing Bind data." << std::endl;
                } else {
                    Bind *Bind = Binds[Element];

                    //Key
                    if (Data[0] != "") {
                        SDL_Scancode Key = (SDL_Scancode)AtTools::Strings::StringTo<int>(Data[0]);
                        Bind->SetKey(Key);
                    }

                    //Modifier
                    if (Data[1] != "") {
                        SDL_Scancode Modifier = SDL_SCANCODE_UNKNOWN;
                        if (Data[1] == "LSHIFT") {
                            Modifier = SDL_SCANCODE_LSHIFT;
                        } else if (Data[1] == "RSHIFT") {
                            Modifier = SDL_SCANCODE_RSHIFT;
                        } else if (Data[1] == "LCTRL") {
                            Modifier = SDL_SCANCODE_LCTRL;
                        } else if (Data[1] == "RCTRL") {
                            Modifier = SDL_SCANCODE_RCTRL;
                        } else if (Data[1] == "LALT") {
                            Modifier = SDL_SCANCODE_LALT;
                        } else if (Data[1] == "RALT") {
                            Modifier = SDL_SCANCODE_RALT;
                        }

                        Bind->SetModifier(Modifier);
                    }

                    //Repeat
                    if (Data[2] != "") {
                        bool Repeat = false;
                        if (Data[2] == "true") Repeat = true;

                        Bind->SetRepeat(Repeat);
                    }
                }
            } else if (Buttons[Element]) {
                if (Data.size() < 1) {
                    if (Data.size() > 0) std::cerr << "(Interface/State.cpp) LoadElement(): Missing Button data." << std::endl;
                } else {
                    Button *Button = Buttons[Element];

                    //Type
                    if (Data[0] != "") {
                        int Type = Buttons::Standard;

                        if (Data[0] == "DropDown") {
                            Type = Buttons::DropDown;
                        } else if (Data[0] == "Checkbox") {
                            Type = Buttons::Checkbox;
                        }

                        Button->SetType(Type);
                    }

                    //Checked
                    if (Data[1] != "") {
                        if (Data[1] == "true") Button->SetChecked(true);
                    }
                }
            } else if (Dialogs[Element]) {
            } else if (Frames[Element]) {
                if (Data.size() < 2) {
                    if (Data.size() > 0) std::cerr << "(Interface/State.cpp) LoadElement(): Missing Frame data." << std::endl;
                } else {
                    Frame *Frame = Frames[Element];

                    //AllowDebug
                    if (Data[0] != "") {
                        if (Data[0] == "false") Frame->SetAllowDebug(false);
                    }

                    //Ambience
                    if (Tag == "Interface" && Data[1] != "") {
                        int Ambience[3];
                        Ambience[0] = AtTools::Strings::StringTo<int>(Data[1], ' ');
                        Ambience[1] = AtTools::Strings::StringTo<int>(Data[1], ' ', 1);
                        Ambience[2] = AtTools::Strings::StringTo<int>(Data[1], ' ', 2);

                        AtObjects::Renderer::SetClearColor(Ambience[0], Ambience[1], Ambience[2]);
                    }
                }
            } else if (Lists[Element]) {
                if (Data.size() < 5) {
                    if (Data.size() > 0) std::cerr << "(Interface/State.cpp) LoadElement(): Missing List data." << std::endl;
                } else {
                    List *List = Lists[Element];

                    //Orientation
                    if (Data[0] != "") {
                        List->SetOrientation(Data[0]);
                    }

                    //List Type
                    if (Data[1] != "") {
                        int Type = Lists::Display;

                        if (Data[1] == "Display") {
                            Type = Lists::Display;
                        } else if (Data[1] == "Selection") {
                            Type = Lists::Selection;
                        }

                        List->SetType(Type);
                    }

                    //Value
                    if (Data[2] != "") {
                        List->SetValue(Data[2]);
                    }

                    //ItemSize
                    if (Data[3] != "") {
                        int Width = AtTools::Strings::StringTo<int>(Data[3], 'x');
                        int Height = AtTools::Strings::StringTo<int>(Data[3], 'x', 1);
                        List->SetItemSize(Width, Height);
                    }

                    //ItemLimit
                    if (Data[4] != "") {
                        int ItemLimit = AtTools::Strings::StringTo<float>(Data[4]);
                        List->SetItemLimit(ItemLimit);
                    }

                    //HideOnInput
                    if (Data[5] != "") {
                        bool HideOnInput = false;
                        if (Data[5] == "true") HideOnInput = true;

                        List->SetHideOnInput(HideOnInput);
                    }
                }
            } else if (Messages[Element]) {
                Message *Message = Messages[Element];

                SetMessageFont(Message, Data);

                if (Data.size() < 12) {
                    if (Data.size() > 0) std::cerr << "(Interface/State.cpp) LoadElement(): Missing Message data." << std::endl;
                } else {
                    //Text
                    if (Data[6] != "") {
                        Message->SetText(Data[6]);
                    }

                    //ShadowOffset
                    if (Data[7] != "") {
                        float X = AtTools::Strings::StringTo<float>(Data[7], ' ');
                        float Y = AtTools::Strings::StringTo<float>(Data[7], ' ', 1);
                        Message->SetShadowOffset(X, Y);
                    }

                    //ShadowColor
                    if (Data[8] != "") {
                        int Color[3]; Color[0] = Color[1] = Color[2] = -1;
                        for (int i=0; i<3; i++) Color[i] = AtTools::Strings::StringTo<int>(Data[8], ' ', i);
                        Message->SetShadowColor(Color);
                    }

                    //Editable
                    if (Data[9] != "") {
                        if (Data[9] == "true") Message->SetEditable(true);
                    }

                    //CharacterLimit
                    if (Data[10] != "") {
                        int CharacterLimit = AtTools::Strings::StringTo<int>(Data[10]);
                        Message->SetCharacterLimit(CharacterLimit);
                    }

                    //Focused
                    if (Data[11] != "") {
                        if (Data[11] == "true") {
                            Message->SetFocused(true);
                            Focus = Message;
                        }
                    }
                }
            } else if (Paragraphs[Element]) {
                if (Data.size() < 12) {
                    if (Data.size() > 0) std::cerr << "(Interface/State.cpp) LoadElement(): Missing Paragraph data." << std::endl;
                } else {
                    Paragraph *Paragraph = Paragraphs[Element];

                    //Size
                    if (Data[1] != "") {
                        int Size = AtTools::Strings::StringTo<int>(Data[1]);
                        Paragraph->SetSize(Size);
                    }

                    //Color
                    if (Data[2] != "") {
                        int Color[3]; Color[0] = Color[1] = Color[2] = -1;
                        for (int i=0; i<3; i++) Color[i] = AtTools::Strings::StringTo<int>(Data[2], ' ', i);
                        Paragraph->SetShadowColor(Color);
                    }

                    //ShadowOffset
                    if (Data[7] != "") {
                        float X = AtTools::Strings::StringTo<float>(Data[7], ' ');
                        float Y = AtTools::Strings::StringTo<float>(Data[7], ' ', 1);
                        Paragraph->SetShadowOffset(X, Y);
                    }

                    //ShadowColor
                    if (Data[8] != "") {
                        int Color[3]; Color[0] = Color[1] = Color[2] = -1;
                        for (int i=0; i<3; i++) Color[i] = AtTools::Strings::StringTo<int>(Data[8], ' ', i);
                        Paragraph->SetShadowColor(Color);
                    }

                    //Editable
                    if (Data[9] != "") {
                        if (Data[9] == "true") Paragraph->SetEditable(true);
                    }

                    //Text
                    if (Tag.GetText() != "") LoadParagraphText(Paragraph->GetID(), Tag.GetText());
                }
            } else if (Sliders[Element]) {
                if (Data.size() < 3) {
                    if (Data.size() > 0) std::cerr << "(Interface/State.cpp) LoadElement(): Missing Slider data." << std::endl;
                } else {
                    Slider *Slider = Sliders[Element];

                    //Status
                    if (Data[0] != "") {
                        float Status = AtTools::Strings::StringTo<float>(Data[0]);
                        Slider->SetStatus(Status);
                    }

                    //MaximumValue
                    if (Data[1] != "") {
                        float MaximumValue = AtTools::Strings::StringTo<float>(Data[1]);
                        Slider->SetMaximumValue(MaximumValue);
                    }

                    //MinimumValue
                    if (Data[2] != "") {
                        float MinimumValue = AtTools::Strings::StringTo<float>(Data[2]);
                        Slider->SetMinimumValue(MinimumValue);
                    }

                    //Orientation
                    if (Data[3] != "") {
                        int Orientation = Orientation::Horizontal;

                        if (Data[3] == "HORIZONTAL") {
                            Orientation = Orientation::Horizontal;
                        } else if (Data[3] == "VERTICAL") {
                            Orientation = Orientation::Vertical;
                        }

                        Slider->SetOrientation(Orientation);
                    }

                    //Type
                    if (Data[4] != "") {
                        int Type = Sliders::Standard;

                        if (Data[4] == "Drag") {
                            Type = Sliders::Drag;
                        }

                        Slider->SetType(Type);
                    }
                }
            } else if (StatusBars[Element]) {
                if (Data.size() < 3) {
                    if (Data.size() > 0) std::cerr << "(Interface/State.cpp) LoadElement(): Missing StatusBar data." << std::endl;
                } else {
                    StatusBar *StatusBar = StatusBars[Element];

                    //Status
                    if (Data[0] != "") {
                        float Status = AtTools::Strings::StringTo<float>(Data[0]);
                        StatusBar->SetStatus(Status);
                    }

                    //Orientation
                    if (Data[1] != "") {
                        int Orientation = Orientation::Horizontal;

                        if (Data[1] == "HORIZONTAL") {
                            Orientation = Orientation::Horizontal;
                        } else if (Data[1] == "VERTICAL") {
                            Orientation = Orientation::Vertical;
                        }

                        StatusBar->SetOrientation(Orientation);
                    }

                    //Direction
                    if (Data[2] != "") {
                        int Direction = AtTools::Strings::StringTo<int>(Data[2]);
                        StatusBar->SetDirection(Direction);
                    }
                }
            }

            //Update Parent List
            List *List = Lists[Element->GetParent()];
            if (List) {
                if (!Sliders[Element]) {
                    //Inherit Margin
                    if (!Lists[List->GetParent()]) {
                        Element->SetMargin(List->GetMarginX(), List->GetMarginY());
                    }

                    //Point to a list
                    List->AddItem(Element);
                } else List->EnableAutoSize(false);
            }
        }
    }

    void State::LoadFontIndex(std::string Path) {
        std::string FileType = ".xml";
        std::vector<std::string> DirectoryList = AtTools::Files::GetDirectoryList(Path, FileType);

        std::vector<std::string>::const_iterator it = DirectoryList.begin();
        while (it!=DirectoryList.end()) {
            std::string Name = it->substr(0, it->size()-FileType.size());

            AtXml::File File;

            if (File.Parse(Path+*it, "Font")) {
                std::vector<std::string> Data;

                while (File.HasTags()) {
                    AtXml::Tag Tag = File.GetTag();
                    int Trigger = Tag.GetTrigger();
                    const std::string Text = Tag.GetText();

                    if (Tag == "Font" && Trigger == AtXml::Trigger::Open) {
                        GetSpecificAttributes(Tag, Data);
                        FontIndex[Name] = Data;
                        if (Debug[1]) std::cout << "   Indexed: " << Path+*it << std::endl;
                    }
                }
            } else std::cerr << "(Interface/State.cpp) LoadFontIndex(): Unable to parse file '" << Path << "'." << std::endl;

            it++;
        }
    }

    void State::LoadParagraphText(std::string ID, std::string Text) {
        Paragraph *Paragraph = Paragraphs[ID];
        if (Paragraph) {
            Paragraph->GetInteractable()->EnableInteraction(false);
            std::string AddOn = Paragraph->GetIndex();

            //Empty
            int Items = Paragraph->GetItems();

            Paragraph->ClearItems();

            for (int i=0; i<Items; i++) RemoveRelatedTo(Paragraph);

            int Spaces = AtTools::Strings::FindTotalOf(' ', Text);
            for (int i=0; i<=Spaces; i++) {
                std::string Word = AtTools::Strings::Seperate(Text, ' ', i)+" ";

                std::ostringstream Stream;
                Stream << Paragraph->GetName() << "Item" << (Paragraph->GetItems()) << "Message";
                std::string Name = Stream.str();

                MakeElement(Lists::Messages, AddOn, Name, Paragraph);
                Message *Message = Messages[AddOn+"."+Name];
                if (Message) {
                    Message->GetInteractable()->EnableInteraction(false);

                    //Value
                    Message->SetValue(Word);

                    Message->SetFont(Paragraph->GetFont());
                    Message->SetSize(Paragraph->GetSize());
                    Message->SetText(Word);

                    AtObjects::Renderable *ParagraphRenderable = Paragraph->GetRenderable();
                    AtObjects::Renderable *Renderable = Message->GetRenderable();
                    Renderable->SetIdleColor(ParagraphRenderable->GetColor());

                    Message->EnableScaling(true);
                    int Width = 0, Height = 0;
                    AtObjects::Renderer::GetStringSize(Message->GetFont(), Message->GetSize(), Word, Width, Height);
                    Message->ResizeTo(Width/Scale.X(), Height/Scale.Y());
                    Message->EnableScaling(false);

                    //std::cout << "(" << Word << ") " << Width << " " << Height << std::endl;

                    Paragraph->AddItem(Message);
                }
            }
        }
    }

    void State::LoadState(std::string Name) {
        if (Name != "") {
            Focus = NULL;
            Scripts.clear();
            AddOnRequirements.clear();

            if (Location != Name) {
                std::cout << "Loading Interface state: " << Name << std::endl;
                Location = Name;
                Load(Name);

                //Remove unnecessary AddOns
                Frame *Frame = Frames.GetFirst();
                while (Frame) {
                    std::string Name = Frame->GetName();
                    class Frame *NextFrame = Frame->Next;

                    //Skip
                    while (NextFrame && NextFrame->GetName() != NextFrame->GetIndex()) NextFrame = NextFrame->Next;

                    if (Frame->GetIndex() == Name) {
                        if (!AddOnRequirements[Name]) {
                            if (Frames[Name+"."+Name]) {
                                Remove(Frames[Name+"."+Name]);
                                std::cout << "\tRemoved: " << Name << std::endl;
                            }
                        }
                    }
                    Frame = NextFrame;
                }

                //TEMP Hack to trigger OnLoad() function
                Process();

                //Set Scale
                SetScale(Scale.X(), Scale.Y());
            }
        }
    }

    void State::LoadStateIndex(std::string Path) {
        std::string FileType = ".xml";
        std::vector<std::string> DirectoryList = AtTools::Files::GetDirectoryList(Path, FileType);

        std::vector<std::string>::const_iterator it = DirectoryList.begin();
        while (it!=DirectoryList.end()) {
            std::string Name = it->substr(0, it->size()-FileType.size());
            StateIndex[Name] = Path+*it;
            if (Debug[1]) std::cout << "   Indexed: " << Path+*it << std::endl;
            it++;
        }
    }

    void State::LoadTextureIndex(std::string Path) {
        std::string FileType = ".xml";
        std::vector<std::string> DirectoryList = AtTools::Files::GetDirectoryList(Path, FileType);

        std::vector<std::string>::const_iterator it = DirectoryList.begin();
        while (it!=DirectoryList.end()) {
            std::string Name = it->substr(0, it->size()-FileType.size());

            AtXml::File File;

            if (File.Parse(Path+*it, "Texture")) {
                std::vector<std::string> Data;

                while (File.HasTags()) {
                    AtXml::Tag Tag = File.GetTag();
                    int Trigger = Tag.GetTrigger();
                    const std::string Text = Tag.GetText();

                    if (Tag == "Texture" && Trigger == AtXml::Trigger::Open) {
                        GetSpecificAttributes(Tag, Data);
                        TextureManager.SetTextureIndex(Name, Data);
                        if (Debug[1]) std::cout << "   Indexed: " << Path+*it << std::endl;
                    }
                }
            } else std::cerr << "(Interface/State.cpp) LoadTextureIndex(): Unable to parse file '" << Path << "'." << std::endl;

            it++;
        }
    }

    void State::MakeElement(int ListID, std::string AddOn, std::string Name, std::string ParentName) {
        Element *Parent = Objects[AddOn+"."+ParentName];
        if (!Objects[AddOn+"."+Name]) {
            MakeElement(ListID, AddOn, Name, Parent);

            AtXml::Tag Tag;
            Element *Element = Objects[AddOn+"."+Name];
            LoadElement(Element, Tag);
        }
    }

    void State::MakeElement(int ListID, std::string AddOn, std::string Name, Element *Parent) {
        Element *Element = NULL;
        switch (ListID) {
            case Lists::Binds:
                Element = MakeObject(Binds, AddOn, Name, Parent);
                if (Element) AtTools::Lua::ExecuteScript(Lua, AddOn + "." + Name + " = Bind.New(\"" + AddOn + "." + Name + "\")");
                break;
            case Lists::Buttons:
                Element = MakeObject(Buttons, AddOn, Name, Parent);
                if (Element) AtTools::Lua::ExecuteScript(Lua, AddOn + "." + Name + " = Button.New(\"" + AddOn + "." + Name + "\")");
                break;
            case Lists::Dialogs:
                Element = MakeObject(Dialogs, AddOn, Name, Parent);
                if (Element) AtTools::Lua::ExecuteScript(Lua, AddOn + "." + Name + " = Dialog.New(\"" + AddOn + "." + Name + "\")");
                break;
            case Lists::Frames:
                {
                    Frame *Frame = MakeObject(Frames, AddOn, Name, Parent);
                    if (Frame) {
                        Element = Frame;
                        AtTools::Lua::ExecuteScript(Lua, AddOn + "." + Name + " = Frame.New(\"" + AddOn + "." + Name + "\")");

                        //Point to a Message
                        if (Parent && Messages[Parent]) {
                            Message *Message = Messages[Parent];
                            Message->SetCursor(Frame);
                        //Point to a slider
                        } else if (Parent && Sliders[Parent]) {
                            Slider *Slider = Sliders[Parent];
                            Slider->SetCursor(Frame);
                        }
                    }
                }
                break;
            case Lists::Lists:
                {
                    List *List = MakeObject(Lists, AddOn, Name, Parent);
                    if (List) {
                        Element = List;
                        AtTools::Lua::ExecuteScript(Lua, AddOn + "." + Name + " = List.New(\"" + AddOn + "." + Name + "\")");

                        //Point to a Button
                        if (Parent && Buttons[Parent]) {
                            Button *Button = Buttons[Parent];
                            if (Button && Button->GetType() == Buttons::DropDown) {
                                Button->SetDisplayList(List);

                                //Position List below Button
                                if (!List->Y(Reference::Parent)) List->Offset(Axis::Y, Button->Height());

                                //Point Message to List
                                Message *DisplayMessage = Button->GetDisplayMessage();
                                List->SetDisplayMessage(DisplayMessage);
                            }
                        }
                    }
                }
                break;
            case Lists::Messages:
                {
                    Message *Message = MakeObject(Messages, AddOn, Name, Parent);
                    if (Message) {
                        Element = Message;
                        AtTools::Lua::ExecuteScript(Lua, AddOn + "." + Name + " = Message.New(\"" + AddOn + "." + Name + "\")");

                        //Point to a DropDownButton
                        if (Parent && Buttons[Parent]) {
                            Button *Button = Buttons[Parent];
                            if (Button && Button->GetType() == Buttons::DropDown) {
                                Button->SetDisplayMessage(Message);
                            }
                        }
                    }
                }
                break;
            case Lists::Paragraphs:
                Element = MakeObject(Paragraphs, AddOn, Name, Parent);
                if (Element) AtTools::Lua::ExecuteScript(Lua, AddOn + "." + Name + " = Paragraph.New(\"" + AddOn + "." + Name + "\")");
                break;
            case Lists::Sliders:
                Element = MakeObject(Sliders, AddOn, Name, Parent);
                AtTools::Lua::ExecuteScript(Lua, AddOn + "." + Name + " = Slider.New(\"" + AddOn + "." + Name + "\")");
                break;
            case Lists::StatusBars:
                Element = MakeObject(StatusBars, AddOn, Name, Parent);
                if (Element) AtTools::Lua::ExecuteScript(Lua, AddOn + "." + Name + " = StatusBar.New(\"" + AddOn + "." + Name + "\")");
                break;
            default:
                std::cerr << "(Interface/State.cpp) MakeElement(): Supplied invalid List ID during element creation: " << ListID << std::endl;
                break;
        }
    }

    void State::Process() {
        HandleEvents();

        //Process Objects
        ProcessList(Frames);

        HandleEvents();

        //Textbox Focus clearence
        if (Focus && Messages[Focus]) {
            Message *Message = Messages[Focus];
            if (!Message->IsFocused()) {
                Focus = NULL;
            }
        }

        //Update Textures
        TextureManager.ReloadListTextures(Buttons);
        TextureManager.ReloadListTextures(Dialogs);
        TextureManager.ReloadListTextures(Frames);
        TextureManager.ReloadListTextures(Lists);
        ReloadMessageTextures();
        TextureManager.ReloadListTextures(Paragraphs);
        TextureManager.ReloadListTextures(Sliders);
        TextureManager.ReloadListTextures(StatusBars);
    }

    template <class IDType, class Type, class InheritedType>
    void State::ProcessList(AtObjects::List<IDType, Type, InheritedType> &List, InheritedType *Parent) {
        Type *Object = List.GetFirst();

        if (!Parent) Parent = &Interface;

        while (Object) {
            if (Object->GetParent() == Parent) {
                //Call ID:OnProcess()
                PushElementCallback(Object, "OnProcess");

                if (Object->IsContainer()) {
                    ProcessRelatedTo(Object);
                    Object->Process();
                } else {
                    Object->Process();
                    ProcessRelatedTo(Object);
                }

                if (Parent->SharesInput() && !Object->GetInteractable()->IsInteractive()) {
                    if (Parent->IsDisabled()) {
                        if (!Object->IsDisabled()) {
                            Object->SetDisabled(true);
                        }
                    } else {
                        if (Object->IsDisabled()) {
                            Object->SetDisabled(false);
                        }
                    }
                }
            }
            Object = Object->Next;
        }
    }

    void State::ProcessRelatedTo(Element *Parent) {
        ProcessList(Frames, Parent);
        ProcessList(Dialogs, Parent);
        ProcessList(Buttons, Parent);
        ProcessList(Paragraphs, Parent);
        ProcessList(Lists, Parent);
        ProcessList(StatusBars, Parent);
        ProcessList(Sliders, Parent);
        ProcessList(Messages, Parent);
        ProcessList(Binds, Parent);
    }

    void State::PushElementCallback(Element *Element, std::string Function) {
        if (Element) {
            std::string ID = Element->GetID();
            std::string Index = Element->GetIndex();
            std::string Name = Element->GetName();

            if (Function == "OnProcess") {
                if (Index == Name) {
                    AtTools::Lua::ExecuteScript(Lua, Index + ":" + Function + "()");
                } else {
                    AtTools::Lua::ExecuteScript(Lua, ID + ":" + Function + "()");
                }
            } else {
                if (Index == Name) {
                    Scripts.push_back(Index + ":" + Function + "()");
                } else {
                    Scripts.push_back(ID + ":" + Function + "()");
                }
            }
        }
    }

    void State::ReloadMessageTextures() {
        Message *Message = Messages.GetFirst();
        while (Message) {
            Message->EnableScaling(true);

            AtObjects::Renderable *Renderable = Message->GetRenderable();
            AtObjects::Texture *Texture = Renderable->GetTexture();
            AtObjects::Texture *NewTexture = TextureManager.LoadTextureFromText(Message->GetFont(), Message->GetSize(), Message->GetText(), Message->GetStyles());

            if (Texture != NewTexture) {
                Renderable->SetTexture(NewTexture);

                if (NewTexture) {
                    AtObjects::Vector2 CorrectionFactor = AtObjects::Vector2(Scale.X(), Scale.Y());

                    if (!ScaleMode) CorrectionFactor = AtObjects::Vector2(1.f, 1.f);

                    Message->ResizeTo(NewTexture->GetWidth()/CorrectionFactor.X(), NewTexture->GetHeight()/CorrectionFactor.Y());
                } else {
                    Message->ResizeTo(0.f, 0.f);
                }
            }

            Message->EnableScaling(false);

            Message = Message->Next;
        }
    }

    void State::ReloadState() {
        if (Location != "") {
            std::string Location = this->Location;
            ClearState();
            LoadState(Location);
        }
    }

    bool State::Remove(Element *Target) {
        bool Removed = false;

        if (Target) {
            std::string Script;
            if (Target->GetName() == Target->GetIndex()) {
                Script = Target->GetName() + " = nil; ";
                Script += "Application.RemoveEventHandlers('" + Target->GetID() +"');";
            } else Script = Target->GetID() + " = nil";
            Element *Parent = Target->GetParent();

            Removed = RemoveFromList(Buttons, Buttons[Target]);
            if (!Removed) {
                Removed = RemoveFromList(Dialogs, Dialogs[Target]);
                if (!Removed) {
                    Removed = RemoveFromList(Frames, Frames[Target]);
                    if (!Removed) {
                        Removed = RemoveFromList(Lists, Lists[Target]);
                        if (!Removed) {
                            Removed = RemoveFromList(Messages, Messages[Target]);
                            if (!Removed) {
                                Removed = RemoveFromList(Paragraphs, Paragraphs[Target]);
                                if (!Removed) {
                                    Removed = RemoveFromList(Sliders, Sliders[Target]);
                                    if (!Removed) {
                                        Removed = RemoveFromList(StatusBars, StatusBars[Target]);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if (Removed) {
                //Erase from the Lua table
                AtTools::Lua::ExecuteScript(Lua, Script);

                //Update ParentList
                if (Lists[Parent]) {
                    List *ParentList = Lists[Parent];
                    ParentList->RemoveItem(Target);
                    ParentList->UpdateSize();
                    ParentList->UpdateItemPositions();
                }
            }
        }

        return Removed;
    }

    void State::RemoveElement(int ListID, std::string ID) {
        bool Removed = false;

        Element *Target = NULL, *Parent = NULL;
        Get(Target, ID);
        std::string Name, Index;
        bool IsDebugFocus = false;

        if (Target) {
            Parent = Target->GetParent();
            Name = Target->GetName();
            Index = Target->GetIndex();
            if (Target == DebugFocus) IsDebugFocus = true;
        }

        switch (ListID) {
            case Lists::Binds:
                Removed = RemoveFromList(Binds, Binds[ID]);
                break;
            case Lists::Buttons:
                Removed = RemoveFromList(Buttons, Buttons[ID]);
                break;
            case Lists::Dialogs:
                Removed = RemoveFromList(Dialogs, Dialogs[ID]);
                break;
            case Lists::Frames:
                Removed = RemoveFromList(Frames, Frames[ID]);
                break;
            case Lists::Lists:
                Removed = RemoveFromList(Lists, Lists[ID]);
                break;
            case Lists::Messages:
                Removed = RemoveFromList(Messages, Messages[ID]);
                break;
            case Lists::Paragraphs:
                Removed = RemoveFromList(Paragraphs, Paragraphs[ID]);
                break;
            case Lists::Sliders:
                Removed = RemoveFromList(Sliders, Sliders[ID]);
                break;
            case Lists::StatusBars:
                Removed = RemoveFromList(StatusBars, StatusBars[ID]);
                break;
            default:
                std::cerr << "(Interface/State.cpp) RemoveElement(): Supplied invalid List ID during element creation: " << ListID << std::endl;
                break;
        }

        if (Removed) {
            //Erase from Lua table
            if (Name == Index) AtTools::Lua::ExecuteScript(Lua, Name + " = nil;"); else AtTools::Lua::ExecuteScript(Lua, ID + " = nil;");

            //Update ParentList
            if (Lists[Parent]) {
                List *ParentList = Lists[Parent];
                ParentList->RemoveItem(Target);
                ParentList->UpdateSize();
                ParentList->UpdateItemPositions();
            }

            //Update DebugFocus
            if (IsDebugFocus) {
                SetFocus(NULL, true);
            }
        }
    }

    void State::RemoveRelatedTo(Element *Parent) {
        if (Parent) {
            //if (Parent && Parent->ID() == "CharacterSheet.Dialog") Frames.Who();
            RemoveRelatedToFromList(Binds, Parent);
            RemoveRelatedToFromList(Buttons, Parent);
            RemoveRelatedToFromList(Dialogs, Parent);
            RemoveRelatedToFromList(Frames, Parent);
            RemoveRelatedToFromList(Lists, Parent);
            RemoveRelatedToFromList(Messages, Parent);
            RemoveRelatedToFromList(Paragraphs, Parent);
            RemoveRelatedToFromList(Sliders, Parent);
            RemoveRelatedToFromList(StatusBars, Parent);
            //if (Parent && Parent->ID() == "CharacterSheet.Dialog") Frames.Who();
        }
    }

    void State::Render(float Interpolation) {
        RenderList(Interpolation, Frames);
    }

    template <class IDType, class Type, class InheritedType>
    void State::RenderList(float Interpolation, AtObjects::List<IDType, Type, InheritedType> &List, InheritedType *Parent) {
        Type *Object = List.GetFirst();

        if (!Parent) Parent = &Interface;

        while (Object) {
            if (Object->GetParent() == Parent) {
                bool AllowDebug = ElementAllowsDebug(Object);

                int DebugState = 0;
                if (Object == Focus && Object == DebugFocus) DebugState = 3; else {
                    if (Object == Focus) DebugState = 1;
                    if (Object == DebugFocus) DebugState = 2;
                }

                if (Object->IsContainer()) {
                    Object->Render(Interpolation);
                    if (Debug[2]&&AllowDebug) Object->RenderShape(Interpolation, DebugState);

                    RenderRelatedTo(Object, Interpolation);
                } else {
                    RenderRelatedTo(Object, Interpolation);

                    Object->Render(Interpolation);
                    if (Debug[2]&&AllowDebug) Object->RenderShape(Interpolation, DebugState);
                }
            }
            Object = Object->Next;
        }
    }

    void State::RenderRelatedTo(Element *Parent, float Interpolation) {
        RenderList(Interpolation, Messages, Parent);
        RenderList(Interpolation, StatusBars, Parent);
        RenderList(Interpolation, Lists, Parent);
        RenderList(Interpolation, Paragraphs, Parent);
        RenderList(Interpolation, Buttons, Parent);
        RenderList(Interpolation, Dialogs, Parent);
        RenderList(Interpolation, Frames, Parent);
        RenderList(Interpolation, Sliders, Parent);
    }

    void State::ResetAllExcept(int List, Element *Parent) {
        if (Parent) {
            if (List != Lists::Buttons) ResetList(Buttons, Parent);
            if (List != Lists::Dialogs) ResetList(Dialogs, Parent);
            if (List != Lists::Frames) ResetList(Frames, Parent);
            if (List != Lists::Lists) ResetList(Lists, Parent);
            if (List != Lists::Messages) ResetList(Messages, Parent);
            if (List != Lists::Paragraphs) ResetList(Paragraphs, Parent);
            if (List != Lists::Sliders) ResetList(Sliders, Parent);
            if (List != Lists::StatusBars) ResetList(StatusBars, Parent);
        }
    }

    void State::ResetInput() {
        ResetList(Frames);
    }

    void State::ResetRelatedTo(Element *Parent) {
        ResetList(Buttons, Parent);
        ResetList(Dialogs, Parent);
        ResetList(Frames, Parent);
        ResetList(Lists, Parent);
        ResetList(Messages, Parent);
        ResetList(Paragraphs, Parent);
        ResetList(Sliders, Parent);
        ResetList(StatusBars, Parent);
    }

    void State::SaveAddOnData(std::string AddOn, std::string SaveData) {
        Frame *Frame = Frames[AddOn+"."+AddOn];
        if (Frame) {
            std::string Index = Frame->GetIndex();
            std::string Name = Frame->GetName();

            if (Index == Name) {
                std::string Location = UserDirectory + "AddOnData/" + Index + ".lua";

                std::ofstream Output;
                Output.open(Location);
                Output << SaveData;
                Output.close();
            }
        }
    }

    void State::SetElementFont(std::string ID, std::string FontID) {
        if (Messages[ID]) {
            Message *Message = Messages[ID];
            SetMessageFont(Message, FontID);
        } else if (Paragraphs[ID]) {
            Paragraph *Paragraph = Paragraphs[ID];
            SetParagraphFont(Paragraph, FontID);
        } else std::cerr << "(Interface/State.cpp) SetElementFont(): Attempted to apply font to a non-message or non-existant element: " << ID << std::endl;
    }

    void State::SetElementTexture(std::string ID, std::string TextureID) {
        Element *Element = Objects[ID];
        if (Element) {
            TextureManager.SetObjectTexture(Element, TextureID);
        } else std::cout << "(Interface/State.cpp): SetElementTexture(): Attempted to set texture of invalid element: " << ID << std::endl;
    }

    void State::Toggle() {
        Interface.Toggle();
    }

    void State::SetFocus(std::string ID, bool State) {
        Element *Object = Objects[ID];

        SetFocus(Object, State);
    }

    void State::SetFocus(Element *Object, bool State) {
        if (State) {
            bool AllowDebug = true;
            std::string Index;

            if (Object) {
                AllowDebug = ElementAllowsDebug(Object);
            }

            if (Debug[0]&&AllowDebug) {
                DebugFocus = Object;
                EventQueue.push_back(Events::DebugFocusChanged);
            }
        } else Focus = Object;
    }

    void State::SetMessageFont(Message *Message, std::string FontID) {
        if (Message) {
            std::vector<std::string> Data = FontIndex[FontID];
            SetMessageFont(Message, Data);
        }
    }

    void State::SetMessageFont(Message *Message, std::vector<std::string> &Data) {
        if (Message) {
            if (Data.size() < 6) {
                if (Data.size() > 0) std::cerr << "(Interface/State.cpp) SetMessageFont(): Missing Font data." << std::endl;
            } else {
                //Font
                if (Data[0] != "") {
                    std::string Font;
                    Font = Data[0];
                    Message->SetFont(Font);
                }

                //Size
                if (Data[1] != "") {
                    int Size = 0;
                    Size = AtTools::Strings::StringTo<int>(Data[1]);
                    Message->SetSize(Size);
                }

                //Color
                if (Data[2] != "") {
                    int Color[4]; Color[0] = Color[1] = Color[2] = Color[3] = 255;
                    for (int i = 0; i<3; i++) Color[i] = AtTools::Strings::StringTo<int>(Data[2], ' ', i);

                    AtObjects::Renderable *Renderable = Message->GetRenderable();
                    Renderable->SetIdleColor(Color);
                }

                //Transparency
                if (Data[3] != "") {
                    AtObjects::Renderable *Renderable = Message->GetRenderable();
                    Renderable->SetTransparency(AtTools::Strings::StringTo<float>(Data[3]));
                }

                //Hover Color
                if (Data[4] != "") {
                    int HoverColor[4]; HoverColor[0] = HoverColor[1] = HoverColor[2] = 255; HoverColor[3] = 255;
                    for (int i = 0; i<3; i++) HoverColor[i] = AtTools::Strings::StringTo<int>(Data[4], ' ', i);

                    AtObjects::Renderable *Renderable = Message->GetRenderable();
                    Renderable->SetHoverColor(HoverColor);
                }

                //Styles
                if (Data[5] != "") {
                    int Styles = 0;
                    char Style = '\0';
                    while (Data[5].length() > 0) {
                        if (Data[5].length() == 1) {
                            Style = AtTools::Strings::StringTo<char>(Data[5]);
                        } else {
                            Style = AtTools::Strings::StringTo<char>(Data[5], ' ');
                        }

                        switch (Style) {
                            case 'i':
                                Styles |= TTF_STYLE_ITALIC;
                                break;
                            case 'u':
                                Styles |= TTF_STYLE_UNDERLINE;
                            break;
                                case 'b':
                                Styles |= TTF_STYLE_BOLD;
                                break;
                            case 's':
                                Styles |= TTF_STYLE_STRIKETHROUGH;
                                break;
                            default:
                                break;
                        }

                        if (Style) Data[5].erase(0, 2);
                    }
                    Message->SetStyle(Styles);
                }
            }
        }
    }

    void State::SetParagraphFont(Paragraph *Paragraph, std::string FontID) {
        if (Paragraph) {
            std::vector<std::string> Data = FontIndex[FontID];

            if (Data.size() < 6) {
                if (Data.size() > 0) std::cerr << "(Interface/State.cpp) SetParagraphFont(): Missing Font data." << std::endl;
            } else {
                //Font
                if (Data[0] != "") {
                    std::string Font;
                    Font = Data[0];
                    Paragraph->SetFont(Font);
                }

                //Size
                if (Data[1] != "") {
                    int Size = 0;
                    Size = AtTools::Strings::StringTo<int>(Data[1]);
                    Paragraph->SetSize(Size);
                }

                //Color
                if (Data[2] != "") {
                    int Color[4]; Color[0] = Color[1] = Color[2] = Color[3] = 255;
                    for (int i = 0; i<3; i++) Color[i] = AtTools::Strings::StringTo<int>(Data[2], ' ', i);

                    //Transparency
                    if (Data[3] != "") {
                        Color[3] = AtTools::Strings::StringTo<float>(Data[3])*255.0f/100.0f;
                    }

                    AtObjects::Renderable *Renderable = Paragraph->GetRenderable();
                    Renderable->SetIdleColor(Color);
                }

                //Hover Color
                if (Data[4] != "") {
                    int HoverColor[4]; HoverColor[0] = HoverColor[1] = HoverColor[2] = 255; HoverColor[3] = 255;
                    for (int i = 0; i<3; i++) HoverColor[i] = AtTools::Strings::StringTo<int>(Data[4], ' ', i);

                    //Transparency
                    //TEMP TODO Fix transparency for HoverColor

                    AtObjects::Renderable *Renderable = Paragraph->GetRenderable();
                    Renderable->SetHoverColor(HoverColor);
                }

                //Styles
                if (Data[5] != "") {
                    int Styles = 0;
                    char Style = '\0';
                    while (Data[5].length() > 0) {
                        if (Data[5].length() == 1) {
                            Style = AtTools::Strings::StringTo<char>(Data[5]);
                        } else {
                            Style = AtTools::Strings::StringTo<char>(Data[5], ' ');
                        }

                        switch (Style) {
                            case 'i':
                                Styles |= TTF_STYLE_ITALIC;
                                break;
                            case 'u':
                                Styles |= TTF_STYLE_UNDERLINE;
                            break;
                                case 'b':
                                Styles |= TTF_STYLE_BOLD;
                                break;
                            case 's':
                                Styles |= TTF_STYLE_STRIKETHROUGH;
                                break;
                            default:
                                break;
                        }

                        if (Style) Data[5].erase(0, 2);
                    }
                    Paragraph->SetStyle(Styles);
                }
            }
        }
    }

    void State::SetResolution(int Width, int Height) {
        if (Width > 0 && Height > 0) {
            int NativeWidth = NativeResolution.X();
            int NativeHeight = NativeResolution.Y();

            //Adjust W:H ratio
            if ((float)Width/Height != (float)NativeWidth/NativeHeight) NativeWidth = (float)NativeHeight*Width/Height;
            //if ((float)Width/Height != (float)NativeWidth/NativeHeight) NativeHeight = (float)NativeWidth*Height/Width;

            Interface.ResizeTo(NativeWidth, NativeHeight);

            Resolution = AtObjects::Vector2(Width, Height);

            float ScaleX = (float)Width/NativeWidth;
            float ScaleY = (float)Height/NativeHeight;

            Interface.ScaleTo(ScaleX, ScaleY);
            SetScale(ScaleX, ScaleY);
        }
    }

    void State::Show() {
        Interface.Show();
    }

    void State::SetScale(float ScaleX, float ScaleY) {
        //ScaleX = floor(ScaleX*100.f+0.5f)/100.f;
        //ScaleY = floor(ScaleY*100.f+0.5f)/100.f;

        Scale = AtObjects::Vector2(ScaleX, ScaleY);

        if (ScaleMode) {
            ScaleList(Buttons, Scale);
            ScaleList(Dialogs, Scale);
            ScaleList(Frames, Scale);
            ScaleList(Lists, Scale);
            ScaleList(Messages, Scale);
            ScaleList(Paragraphs, Scale);
            ScaleList(Sliders, Scale);
            ScaleList(StatusBars, Scale);
        } else {
            AtObjects::Vector2 VectorOne = AtObjects::Vector2(1, 1);

            ScaleList(Buttons, VectorOne);
            ScaleList(Dialogs, AtObjects::Vector2(1, 1));

            /*
            Frame *Frame = Frames.GetFirst();
            while (Frame) {
                if (Frame->GetIndex() == Frame->GetName()) {
                    Frame->ScaleTo(Scale.X(), Scale.Y());
                } else Frame->ScaleTo(1.f, 1.f);
                Frame = Frame->Next;
            }
            */

            ScaleList(Frames, VectorOne);

            ScaleList(Lists, VectorOne);
            ScaleList(Messages, VectorOne);
            ScaleList(Paragraphs, VectorOne);
            ScaleList(Sliders, VectorOne);
            ScaleList(StatusBars, VectorOne);
        }
    }

    void State::SetScaleMode(bool ScaleMode) {
        this->ScaleMode = ScaleMode;
    }

    State::State(lua_State *Lua) {
        this->Lua = Lua;
        Focus = NULL;
        Cursor = NULL;

        ScaleMode = true;

        Interface.SetIndex("Interface");
        Interface.SetName("Interface");

        Binds.SetName("Binds");
        Buttons.SetName("Buttons");
        Dialogs.SetName("Dialogs");
        Frames.SetName("Frames");

        Lists.SetName("Lists");
        Messages.SetName("Messages");
        Paragraphs.SetName("Paragraphs");
        Sliders.SetName("Sliders");
        StatusBars.SetName("StatusBars");

        Sliders.SetInputResponse(-1);
    }

    void State::Who() {
        std::cout << std::endl;
        Binds.Who();
        std::cout << std::endl;
        Buttons.Who();
        std::cout << std::endl;
        Dialogs.Who();
        std::cout << std::endl;
        Frames.Who();
        std::cout << std::endl;
        Lists.Who();
        std::cout << std::endl;
        Messages.Who();
        std::cout << std::endl;
        Paragraphs.Who();
        std::cout << std::endl;
        Sliders.Who();
        std::cout << std::endl;
        StatusBars.Who();
        std::cout << std::endl;
    }

    State::~State() {
        Lua = NULL;
        Focus = NULL;
        Cursor = NULL;
    }
}
