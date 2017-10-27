//Message.cpp
#include "Message.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <AtGfx/Renderer.h>

namespace AtGLui {
    void Message::Add(char C) {
        if (!CharacterLimit || (CharacterLimit && (signed)Value.length() < CharacterLimit)) {
            std::string String;
            String.push_back(C);
            Value.insert(Value.length()+CursorPosition, String.c_str());
        }
    }

    void Message::Erase(int Modifier) {
        if (Value != "") {
            if (abs(CursorPosition+Modifier) < (signed)Value.length() && CursorPosition+Modifier < 1) {
                Value.erase(Value.begin()+Value.length()-1+CursorPosition+Modifier);
                CursorPosition += Modifier;
            }
        }
    }

    void Message::EraseFast(int Modifier) {
        if (Value != "") {
            while (Value[Value.length()-1+CursorPosition+Modifier] != ' ' && abs(CursorPosition+Modifier) < (signed)Value.length() && CursorPosition+Modifier < 1) {
                Erase(Modifier);
            }
            Erase(Modifier);
        }
    }

    std::string Message::GetFont() {
        return Font;
    }

    int Message::GetSize() {
        float Scale = Scaled?this->Scale.Y():1;
        return Scale*Size;
    }

    int Message::GetStyles() {
        return Styles;
    }

    std::string Message::GetText() {
        return Value;
    }

    void Message::HoldKey(int Key) {
        Repeat = Key;
        RepeatTimer = 0;
    }

    bool Message::IsEditable() {
        return Editable;
    }

    bool Message::IsFocused() {
        return Focused;
    }

    Message::Message() {
        Next = Previous = NULL;
        Container = false; Interactable.EnableInteraction(false);
        Editable = Focused = false;
        Cursor = NULL;
        CharacterLimit = CursorPosition = CursorTimer = Repeat = RepeatTimer = Size = Styles = 0; ShadowColor[0] = ShadowColor[1] = ShadowColor[2] = -1;
    }

    void Message::MoveCursor(int Step) {
        if (Editable) {
            if ((CursorPosition+Step <= 0) && (abs(CursorPosition+Step) <= (signed)Value.length())) CursorPosition += Step;
        }
    }

    void Message::MoveCursorFast(int Step) {
        if (Editable && Value != "") {
            if (Value[Value.length()-1+Step+CursorPosition] == ' ') MoveCursor(Step);
            while (Value[Value.length()-1+Step+CursorPosition] != ' ' && abs(CursorPosition+Step) <= (signed)Value.length() && CursorPosition+Step < 1) {
                MoveCursor(Step);
            }
            MoveCursor(Step);
        }
    }

    int Message::OnInput(SDL_Event &Event) {
        int Input = 0;

        if (Focused&&Editable) {
            if (Event.type == SDL_TEXTINPUT) {
                Add(*Event.text.text);
                EventQueue.push_back(AtPhys::Events::ValueChange);
                Input = 501;
            } else if (Event.type == SDL_KEYDOWN) {
                HoldKey(Event.key.keysym.scancode);

                if ((Event.key.keysym.scancode >= SDL_SCANCODE_A && Event.key.keysym.scancode <= SDL_SCANCODE_0) || (Event.key.keysym.scancode >= SDL_SCANCODE_SPACE && Event.key.keysym.scancode <= SDL_SCANCODE_SLASH)) {
                    //Catch text input keydowns
                    Input = 501;
                } else if (Event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
                    const Uint8 *Keyboard = SDL_GetKeyboardState(NULL);
                    if (Keyboard[SDL_SCANCODE_LCTRL] || Keyboard[SDL_SCANCODE_RCTRL]) {
                        EraseFast();
                    } else {
                        Erase();
                    }

                    EventQueue.push_back(AtPhys::Events::ValueChange);

                    Input = 502;
                } else if (Event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
                    const Uint8 *Keyboard = SDL_GetKeyboardState(NULL);
                    if (Keyboard[SDL_SCANCODE_LCTRL] || Keyboard[SDL_SCANCODE_RCTRL]) {
                        MoveCursorFast(-1);
                    } else {
                        MoveCursor(-1);
                    }

                    Input = 503;
                } else if (Event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
                    const Uint8 *Keyboard = SDL_GetKeyboardState(NULL);
                    if (Keyboard[SDL_SCANCODE_LCTRL] || Keyboard[SDL_SCANCODE_RCTRL]) {
                        MoveCursorFast(1);
                    } else {
                        MoveCursor(1);
                    }

                    Input = 504;
                } else if (Event.key.keysym.scancode == SDL_SCANCODE_DELETE) {
                    const Uint8 *Keyboard = SDL_GetKeyboardState(NULL);
                    if (Keyboard[SDL_SCANCODE_LCTRL] || Keyboard[SDL_SCANCODE_RCTRL]) {
                        EraseFast(1);
                    } else {
                        Erase(1);
                    }

                    EventQueue.push_back(AtPhys::Events::ValueChange);

                    Input = 505;
                }
            } else if (Event.type == SDL_KEYUP) {
                ReleaseKey(Event.key.keysym.scancode);
            } else if (Event.type == SDL_MOUSEBUTTONDOWN) {
                if (!Hovered) {
                    SetFocused(false);
                }
            }
        }

        return Input;
    }

    void Message::OnLeftButtonDown() {
        SetFocused(true);
    }

    void Message::OnProcess() {
        //TEMP ? PERFORMANCE CONCERN
        if (Cursor) {
            if (Cursor->IsShown()) {
                std::string String = Value.substr(Value.length()+CursorPosition, abs(CursorPosition));
                int Width = 0, Height = 0;
                AtGfx::Renderer::GetStringSize(GetFont(), GetSize(), String, Width, Height);
                Cursor->Offset(Axis::X, -Width);
            }

            if (Focused) {
                CursorTimer++;
                if (CursorTimer > 15) {
                    Cursor->Toggle();
                    CursorTimer = 0;
                }
            }
        }

        if (Focused && Repeat) {
            if (RepeatTimer < 15) {
                RepeatTimer++;
            } else {
                if (Repeat == SDLK_BACKSPACE) {
                    Erase();
                } else if (Repeat == SDLK_DELETE) {
                    Erase(1);
                } else if (Repeat == SDLK_LEFT) {
                    MoveCursor(-1);
                } else if (Repeat == SDLK_RIGHT) {
                    MoveCursor(1);
                } else {
                    char C = Value[Value.length()-1];
                    Add(C);
                }
            }
        }
    }

    void Message::OnRightButtonDown() {
        SetFocused(true);
    }

    void Message::ReleaseKey(int Key) {
        if (Repeat == Key) {
            Repeat = 0;
            RepeatTimer = 0;
        }
    }

    void Message::Render(float Interpolation) {
        if (Value != "" && (IsShown(true) or IsMoving(Axis::XY, true))) {
            UpdateClipping();

            Scaled = true;

            float TargetX = X(Interpolation, Reference::Origin);
            float TargetY = Y(Interpolation, Reference::Origin);

            float TargetWidth = Width();
            float TargetHeight = Height();

            AtGfx::Texture *Texture = Renderable.GetTexture();

            //TEMP ? PERFORMANCE CONCERN
            if (ShadowColor[0] > -1) {
                AtGfx::Renderer::SetColor(ShadowColor[0], ShadowColor[1], ShadowColor[2], 255);
                AtGfx::Renderer::RenderTexture(Texture, TargetX+ShadowOffset.X(), TargetY+ShadowOffset.Y(), TargetWidth, TargetHeight, 0, ClipSize.X(), ClipSize.Y(), ClipPosition.X(), ClipPosition.Y()); //Tile?
            }

            Renderable.RenderAsTexture(TargetX, TargetY, TargetWidth, TargetHeight, ClipSize.X(), ClipSize.Y(), ClipPosition.X(), ClipPosition.Y());

            Scaled = false;
        }
    }

    void Message::SetCharacterLimit(int CharacterLimit) {
        this->CharacterLimit = CharacterLimit;
    }

    void Message::SetCursor(Frame *Cursor) {
        if (Editable) {
            this->Cursor = Cursor;
            if (Cursor) Cursor->Hide();
        } else std::cout << "(Interface/Message.cpp) SetCursor(): Attempted to assign CursorFrame to a non-editable message: " << Name << std::endl;
    }

    void Message::SetEditable(bool Editable) {
        this->Editable = Editable;
    }

    void Message::SetFocused(bool Focused) {
        if (Editable) {
            this->Focused = Focused;

            CursorPosition = CursorTimer = Repeat = 0;

            if (Cursor) {
                if (Focused) {
                    Cursor->Show();
                } else {
                    Cursor->Hide();
                }
            }
        }
    }

    void Message::SetFont(std::string Font) {
        this->Font = Font;
    }

    void Message::SetShadowColor(int ShadowColor[3]) {
        for (int i=0; i<3; i++) {
            if (ShadowColor[i] > -1) this->ShadowColor[i] = ShadowColor[i];
        }
    }

    void Message::SetShadowOffset(float X, float Y) {
        ShadowOffset = AtPhys::Vector2(X, Y);

        if (ShadowColor[0] == -1) {
            ShadowColor[0] = ShadowColor[1] = ShadowColor[2] = 0;
        }
    }

    void Message::SetSize(int Size) {
        this->Size = Size;
    }

    void Message::SetStyle(int Styles) {
        this->Styles = Styles;
    }

    void Message::SetText(std::string Text) {
        if (Text != Value) {
            EventQueue.push_back(AtPhys::Events::ValueChange);
        }

        Value = Text;

        Scaled = true;

        int Width = 0, Height = 0;
        AtGfx::Renderer::GetStringSize(Font, GetSize(), Text, Width, Height);
        ResizeTo((float)Width/Scale.X(), (float)Height/Scale.Y());

        Scaled = false;
    }

    Message::~Message() {
        Next = Previous = NULL;
        Editable = Focused = false;
        Cursor = NULL;
        CharacterLimit = CursorPosition = CursorTimer = Repeat = RepeatTimer = Size = Styles = 0; ShadowColor[0] = ShadowColor[1] = ShadowColor[2] = -1;
    }
}
