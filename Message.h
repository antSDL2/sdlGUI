//Message.h
#ifndef NEOGUI_MSG_H
#define NEOGUI_MSG_H

#include "Element.h"
#include "Frame.h"
namespace AtGLui {
    class Message: public Element {
        //Members
        public:
            Message *Next, *Previous;
        protected:
            bool Editable, Focused;
            Frame *Cursor;
            int CharacterLimit, CursorPosition, CursorTimer, Repeat, RepeatTimer, ShadowColor[3], Size, Styles;
            AtObjects::Vector2 ShadowOffset;
            std::string Font;
        //Access Methods
        public:
            Message();
            ~Message();
            std::string GetFont();
            int GetSize();
            int GetStyles();
            std::string GetText();
            bool IsFocused();
            bool IsEditable();
        //Process Methods
        public:
            void Add(char C);
            void Erase(int Modifier = 0);
            void EraseFast(int Modifier = 0);
            void HoldKey(int Key);
            void MoveCursor(int Step);
            void MoveCursorFast(int Step);
            void ReleaseKey(int Key);
            void Render(float Interpolation);
            void SetCharacterLimit(int CharacterLimit);
            void SetCursor(Frame *Cursor);
            void SetEditable(bool Editable);
            void SetFocused(bool Focused);
            void SetFont(std::string Font);
            void SetShadowColor(int ShadowColor[3]);
            void SetShadowOffset(float X, float Y);
            void SetSize(int Size);
            void SetStyle(int Styles);
            void SetText(std::string Text);
        protected:
            int OnInput(SDL_Event &Event);
            void OnLeftButtonDown();
            void OnProcess();
            void OnRightButtonDown();
    };
}

#endif
