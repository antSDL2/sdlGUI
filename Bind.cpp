//Bind.cpp
#include "Bind.h"
#include <iostream>

static const int SDL_CUSTOM_CTRL       = 1001;
static const int SDL_CUSTOM_SHIFT      = 1002;
static const int SDL_CUSTOM_ALT        = 1003;
static const int SDL_CUSTOM_LMB        = 1004;
static const int SDL_CUSTOM_RMB        = 1005;
static const int SDL_CUSTOM_MMB        = 1006;
static const int SDL_CUSTOM_X1MB       = 1007;
static const int SDL_CUSTOM_X2MB       = 1008;
static const int SDL_CUSTOM_WHEELUP    = 1009;
static const int SDL_CUSTOM_WHEELDOWN  = 1010;

namespace sdlGUI {
    Bind::Bind() {
        Next = Previous = NULL;
        Key = Modifier = SDL_SCANCODE_UNKNOWN;
        Repeat = false;
    }

    SDL_Scancode Bind::GetKey() {
        return Key;
    }

    SDL_Scancode Bind::GetModifier() {
        return Modifier;
    }

    int Bind::OnInput(SDL_Event &Event) {
        int Input = 0;

        if (State) {
            if (Event.type == SDL_KEYDOWN || Event.type == SDL_KEYUP) {
                if (Event.key.keysym.scancode == Key && (Event.key.repeat == 0 || Repeat)) {
                    const Uint8 *KeyboardState = SDL_GetKeyboardState(NULL);

                    bool ModifierCheck = true;
                    if (Modifier == SDL_CUSTOM_CTRL) {
                        ModifierCheck = (KeyboardState[SDL_SCANCODE_LCTRL] || KeyboardState[SDL_SCANCODE_RCTRL]);
                    } else if (Modifier == SDL_CUSTOM_SHIFT) {
                        ModifierCheck = (KeyboardState[SDL_SCANCODE_LSHIFT] || KeyboardState[SDL_SCANCODE_RSHIFT]);
                    } else if (Modifier == SDL_CUSTOM_ALT) {
                        ModifierCheck = (KeyboardState[SDL_SCANCODE_LALT] || KeyboardState[SDL_SCANCODE_RALT]);
                    } else if (Modifier) {
                        ModifierCheck = KeyboardState[Modifier];
                    }

                    if (ModifierCheck || !Modifier) {
                        if (Event.type == SDL_KEYDOWN) {
                            EventQueue.push_back(Events::Press);

                            //Input = Events::Press;
                        } else {
                            EventQueue.push_back(Events::Release);
                            //Input = Events::Release;
                        }
                    }
                }
            } else if (Event.type == SDL_MOUSEBUTTONDOWN || Event.type == SDL_MOUSEBUTTONUP) {
                int Key = this->Key;

                if (Key == SDL_CUSTOM_MMB) {
                    Key = SDL_BUTTON_MIDDLE;
                } else if (Key == SDL_CUSTOM_X1MB) {
                    Key = SDL_BUTTON_X1;
                } else if (Key == SDL_CUSTOM_X2MB) {
                    Key = SDL_BUTTON_X2;
                }

                if (Event.button.button == Key) {
                    const Uint8 *KeyboardState = SDL_GetKeyboardState(NULL);

                    bool ModifierCheck = false;
                    if (Modifier == SDL_CUSTOM_CTRL) {
                        ModifierCheck = (KeyboardState[SDL_SCANCODE_LCTRL] || KeyboardState[SDL_SCANCODE_RCTRL]);
                    } else if (Modifier == SDL_CUSTOM_SHIFT) {
                        ModifierCheck = (KeyboardState[SDL_SCANCODE_LSHIFT] || KeyboardState[SDL_SCANCODE_RSHIFT]);
                    } else if (Modifier == SDL_CUSTOM_ALT) {
                        ModifierCheck = (KeyboardState[SDL_SCANCODE_LALT] || KeyboardState[SDL_SCANCODE_RALT]);
                    } else {
                        ModifierCheck = KeyboardState[Modifier];
                    }

                    if (ModifierCheck || !Modifier) {
                        if (Event.type == SDL_MOUSEBUTTONDOWN) {
                            EventQueue.push_back(Events::Press);
                            //Input = Events::Press;
                        } else {
                            EventQueue.push_back(Events::Release);
                            //Input = Events::Release;
                        }
                    }
                }
            } else if (Event.type == SDL_MOUSEWHEEL) {
                if (Key == SDL_CUSTOM_WHEELDOWN && Event.wheel.y < 0) {
                    EventQueue.push_back(Events::Press);
                    //Input = Events::Press;
                } else if (Key == SDL_CUSTOM_WHEELUP && Event.wheel.y > 0) {
                    EventQueue.push_back(Events::Press);
                    //Input = Events::Press;
                }
            }
        }

        return Input;
    }

    void Bind::SetKey(SDL_Scancode Key) {
        this->Key = Key;
    }

    void Bind::SetModifier(SDL_Scancode Modifier) {
        this->Modifier = Modifier;
    }

    void Bind::SetRepeat(bool Repeat) {
        this->Repeat = Repeat;
    }

    Bind::~Bind() {
        Next = Previous = NULL;
        Key = Modifier = SDL_SCANCODE_UNKNOWN;
        Repeat = false;
    }
}
