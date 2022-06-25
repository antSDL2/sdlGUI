//Element.h
#ifndef NEOGUI_ELEMENT_H
#define NEOGUI_ELEMENT_H

#include <SDL2/SDL.h>
#include <sdlObjects/Object.h>
#include <sdlObjects/Renderable.h>

using namespace sdlUtility;

namespace sdlGUI {
    namespace Events {
        static const int Press              = 100;
        static const int Release            = 101;
    }

    namespace Orientation {
        const int Horizontal                = 1;
        const int Vertical                  = 2;
    }

    class Element: public sdlObjects::Object {
        //Members
        protected:
            Element *Parent;
            bool Container, ClipContents, CopyInput, Disabled, ShareInput;
            Vector2 Margin, Padding;
        //Access Methods
        public:
            Element();
            virtual ~Element();
            float ChildrenOffsetX();
            float ChildrenOffsetY();
            bool ClipsContents();
            bool CopiesInput();
            Element *GetParent();
            float GetMarginX();
            float GetMarginY();
            bool IsContainer();
            bool IsDisabled();
            float PaddingX();
            float PaddingY();
            bool SharesInput();
        protected:
            float InheritedHeight(int State = 1);
            float InheritedWidth(int State = 1);
        //Process Methods
        public:
            void AddPadding(float X, float Y);
            void OffsetChildren(int Axis, float Position);
            void Render(float Interpolation);
            void SetContainer(bool Container);
            void SetClipContents(bool ClipContents);
            void SetCopyInput(bool CopyInput);
            void SetDisabled(bool Disabled);
            void SetMargin(int X, int Y);
            void SetParent(Element *Parent);
            void SetShareInput(bool ShareInput);
        protected:
            void OnMouseIn();
            void OnMouseOut();
            void UpdateClipping();
    };
}

#endif
