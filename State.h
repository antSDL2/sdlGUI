//State.h
#ifndef NEOGUI_STATE_H
#define NEOGUI_STATE_H

#include <Lua/lua.hpp>
#include <AtObjects/TextureManager.h>
#include <AtTools/AtTools.h>
#include <AtObjects/ObjectManager.h>
#include <AtXml/AtXml.h>
#include "Bind.h"
#include "Button.h"
#include "Dialog.h"
#include "Frame.h"
#include "List.h"
#include "Message.h"
#include "Paragraph.h"
#include "Slider.h"
#include "StatusBar.h"

namespace AtGLui {
    namespace Lists {
        static const int Binds          = 1;
        static const int Buttons        = 2;
        static const int Dialogs        = 3;
        static const int Frames         = 4;
        static const int Lists          = 5;
        static const int Messages       = 6;
        static const int Paragraphs     = 7;
        static const int Sliders        = 8;
        static const int StatusBars     = 9;
    }

    namespace Events {
        static const int DebugFocusChanged  = 1000;
    }

    class State: public AtObjects::ObjectManager<Element> {
        //Members
        protected:
            bool ScaleMode;
            std::map<std::string, bool> AddOnRequirements;
            std::map< std::string, std::vector<std::string> > FontIndex;
            std::map< std::string, std::string > StateIndex;
            AtObjects::List<std::string, Bind, Element> Binds;
            AtObjects::List<std::string, Button, Element> Buttons;
            AtObjects::List<std::string, Dialog, Element> Dialogs;
            AtObjects::List<std::string, Frame, Element> Frames;
            AtObjects::List<std::string, List, Element> Lists;
            AtObjects::List<std::string, Message, Element> Messages;
            AtObjects::List<std::string, Paragraph, Element> Paragraphs;
            AtObjects::List<std::string, Slider, Element> Sliders;
            AtObjects::List<std::string, StatusBar, Element> StatusBars;
            Element *Focus;
            Frame Interface;
            AtObjects::TextureManager TextureManager;
            AtObjects::Vector2 CursorPosition;
            SDL_Cursor *Cursor;
        //Access Methods
        public:
            State(lua_State *Lua);
            ~State();
            template <class Type> void Get(Type *&Object, std::string ID);
            std::string GetElementClass(std::string ID);
            AtObjects::Vector2 &GetCursorPosition();
            Element *GetFocus(bool State = false);
            AtObjects::TextureManager *GetTextureManager();
            bool IsShown();
        //Process Methods
        public:
            void BrowseTextboxes();
            void ClearState();
            void DisableKeyBindings();
            void EnableKeyBindings();
            void Hide();
            int Input(SDL_Event &Event);
            void Initialize(std::string AssetsDirectory, std::string GraphicsDirectory, std::string UserDirectory);
            void MakeElement(int List, std::string AddOn, std::string Name, std::string ParentName);
            void LoadAddOnData(std::string AddOn);
            void LoadCursor(std::string Name);
            void LoadParagraphText(std::string ID, std::string Text);
            void LoadState(std::string Location);
            void Process();
            void ReloadState();
            void RemoveElement(int List, std::string ID);
            void Render(float Interpolation);
            void ResetInput();
            void SaveAddOnData(std::string AddOn, std::string SaveData);
            void SetElementFont(std::string ID, std::string FontID);
            void SetFocus(std::string ID, bool State = false);
            void SetResolution(int Width, int Height);
            void SetScale(float ScaleX, float ScaleY);
            void SetScaleMode(bool ScaleMode);
            void SetElementTexture(std::string ID, std::string TextureID);
            void Show();
            void Toggle();
            void Who();
        protected:
            bool ElementAllowsDebug(Element *Element);
            void GetSharedAttributes(AtXml::Tag &Tag, Element *Element);
            void GetSpecificAttributes(AtXml::Tag &Tag, std::vector<std::string> &Data);
            void HandleEvents();
            template <class IDType, class Type, class InheritedType> void HandleListEvents(AtObjects::List<IDType, Type, InheritedType> &List);
            int InputFocus(SDL_Event &Event);
            template <class IDType, class Type, class InheritedType> int InputList(AtObjects::List<IDType, Type, InheritedType> &List, SDL_Event &Event, InheritedType *Parent = NULL);
            int InputRelatedTo(Element *Parent, SDL_Event &Event);
            void Load(std::string Name, Element *Parent = NULL, bool Embedded = false);
            void LoadElement(Element *Element, AtXml::Tag &Tag);
            void LoadFontIndex(std::string Path);
            void LoadStateIndex(std::string Path);
            void LoadTextureIndex(std::string Path);
            void MakeElement(int List, std::string AddOn, std::string Name, Element *Parent = NULL);
            template <class IDType, class Type, class InheritedType> void ProcessList(AtObjects::List<IDType, Type, InheritedType> &List, InheritedType *Parent = NULL);
            void ProcessRelatedTo(Element *Parent);
            void PushElementCallback(Element *Element, std::string Function);
            void ReloadMessageTextures();
            bool Remove(Element *Target);
            void RemoveRelatedTo(Element *Parent);
            template <class IDType, class Type, class InheritedType> void RenderList(float Interpolation, AtObjects::List<IDType, Type, InheritedType> &List, InheritedType *Parent = NULL);
            void RenderRelatedTo(Element *Parent, float Interpolation);
            void ResetAllExcept(int List, Element *Parent = NULL);
            void ResetRelatedTo(Element *Parent);
            void SetFocus(Element *Object, bool State = false);
            void SetParagraphFont(Paragraph *Paragraph, std::string FontID);
            void SetMessageFont(Message *Message, std::string FontID);
            void SetMessageFont(Message *Message, std::vector<std::string> &Data);
    };

    template <>
    inline void State::Get(Bind *&Bind, std::string ID) {
        Bind = Binds[ID];
    }

    template <>
    inline void State::Get(Button *&Button, std::string ID) {
        Button = Buttons[ID];
    }

    template <>
    inline void State::Get(Dialog *&Dialog, std::string ID) {
        Dialog = Dialogs[ID];
    }

    template <>
    inline void State::Get(Frame *&Frame, std::string ID) {
        if (ID == "Interface.Interface") Frame = &this->Interface; else Frame = Frames[ID];
    }

    template <>
    inline void State::Get(List *&List, std::string ID) {
        List = Lists[ID];
    }

    template <>
    inline void State::Get(Message *&Message, std::string ID) {
        Message = Messages[ID];
    }

    template <>
    inline void State::Get(Paragraph *&Paragraph, std::string ID) {
        Paragraph = Paragraphs[ID];
    }

    template <>
    inline void State::Get(Slider *&Slider, std::string ID) {
        Slider = Sliders[ID];
    }

    template <>
    inline void State::Get(StatusBar *&StatusBar, std::string ID) {
        StatusBar = StatusBars[ID];
    }

    template <>
    inline void State::Get(Element *&Element, std::string ID) {
        if (ID == "Interface.Interface") Element = &this->Interface; else Element = Objects[ID];
    }
}
#endif
