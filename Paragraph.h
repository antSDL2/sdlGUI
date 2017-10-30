//Paragraph.h
#ifndef NEOGUI_PARAGRAPH_H
#define NEOGUI_PARAGRAPH_H

#include "List.h"
namespace AtGLui {
    class Paragraph: public List {
        //Members
        public:
            Paragraph *Next, *Previous;
        protected:
            bool Editable;
            int ShadowColor[3], Size, Styles;
            AtObjects::Vector2 ShadowOffset;
            std::string Font;
        //Access Methods
        public:
            Paragraph();
            ~Paragraph();
            std::string GetFont();
            int *GetShadowColor();
            AtObjects::Vector2 GetShadowOffset();
            int GetSize();
            int GetStyles();
            bool IsEditable();
        //Process Methods
        public:
            void SetEditable(bool Editable);
            void SetFont(std::string Font);
            void SetShadowColor(int ShadowColor[3]);
            void SetShadowOffset(float X, float Y);
            void SetSize(int Size);
            void SetStyle(int Styles);
    };
}

#endif
