//Slider.h
#ifndef NEOGUI_SLIDER_H
#define NEOGUI_SLIDER_H

#include "Element.h"
#include "Frame.h"
namespace AtGLui {
    namespace Sliders {
        static const int Standard    = 1;
        static const int Drag        = 2;
    }
    class Slider: public Element {
        //Members
        public:
            Slider *Next, *Previous;
        protected:
            float MaximumValue, MinimumValue, Status;
            Frame *Cursor;
            int Orientation, Type;
        //Access Methods
        public:
            Slider();
            ~Slider();
            int GetOrientation();
            float GetSliderValue();
            int GetType();
        //Process Methods
        public:
            void SetCursor(Frame *Cursor);
            void SetMaximumValue(float MaximumValue);
            void SetMinimumValue(float MinimumValue);
            void SetOrientation(int Orientation);
            void SetParentFrame(Frame *ParentFrame);
            void SetStatus(float Status);
            void SetSliderValue(float Value);
            void SetType(int Type);
        protected:
            int OnInput(SDL_Event &Event);
            void OnProcess();
    };
}

#endif

