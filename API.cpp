//API.cpp
#include "API.h"

namespace AtGLui {
    template <> std::string AtObjects::API<State, Element>::ObjectManagerName = "Interface";
    template <> State* AtObjects::API<State, Element>::ObjectManager = NULL;

    const luaL_Reg API::InterfaceMethods[] = {
        {"GetCursorX", API::GetCursorX},
        {"GetCursorY", API::GetCursorY},
        {"GetElementClass", API::GetElementClass},
        {"GetFocusElement", API::GetFocusElement},
        {"IsShown", API::IsShown},

        {"BrowseTextboxes", API::BrowseTextboxes},
        {"DisableKeyBindings", API::DisableKeyBindings},
        {"EnableKeyBindings", API::EnableKeyBindings},
        {"Execute", API::Execute},
        {"Hide", API::Hide},
        {"LoadAddOnData", API::LoadAddOnData},
        {"LoadCursor", API::LoadCursor},
        {"MakeBind", API::MakeBind},
        {"MakeButton", API::MakeButton},
        {"MakeDialog", API::MakeDialog},
        {"MakeFrame", API::MakeFrame},
        {"MakeList", API::MakeList},
        {"MakeMessage", API::MakeMessage},
        {"MakeParagraph", API::MakeParagraph},
        {"MakeSlider", API::MakeSlider},
        {"MakeStatusBar", API::MakeStatusBar},
        {"RemoveBind", API::RemoveBind},
        {"RemoveButton", API::RemoveButton},
        {"RemoveDialog", API::RemoveDialog},
        {"RemoveFrame", API::RemoveFrame},
        {"RemoveList", API::RemoveList},
        {"RemoveMessage", API::RemoveMessage},
        {"RemoveParagraph", API::RemoveParagraph},
        {"RemoveSlider", API::RemoveSlider},
        {"RemoveStatusBar", API::RemoveStatusBar},
        {"ReleaseTextures", API::ReleaseTextures},
        {"SaveAddOnData", API::SaveAddOnData},
        {"Show", API::Show},
        {"ShowCursor", API::ShowCursor},
        {"Toggle", API::Toggle},
        {NULL, NULL}
    };

    const luaL_Reg API::ElementMethods[] {
        {"AllowsOtherInput", API::ElementAllowsOtherInput},
        {"ChildrenOffsetX", API::ElementChildrenOffsetX},
        {"ChildrenOffsetY", API::ElementChildrenOffsetY},
        {"ClipsContents", API::ElementClipsContents},
        {"CopiesInput", API::ElementCopiesInput},
        {"GetClass", API::ElementGetClass},
        {"GetParent", API::ElementGetParent},
        {"GetTexture", API::ElementGetTexture},
        {"IsContainer", API::ElementIsContainer},
        {"IsDisabled", API::ElementIsDisabled},
        {"SharesInput", API::ElementSharesInput},

        {"EnableOtherInput", API::ElementEnableOtherInput},
        {"Focus", API::ElementFocus},
        {"New", API::ElementNew},
        {"Release", API::ElementRelease},
        {"Remove", API::ElementRemove},
        {"SetColor", API::ElementSetColor},
        {"SetClipContents", API::ElementSetClipContents},
        {"SetContainer", API::ElementSetContainer},
        {"SetCopyInput", API::ElementSetCopyInput},
        {"SetDisabled", API::ElementSetDisabled},
        {"SetFont", API::ElementSetFont},
        {"SetShareInput", API::ElementSetShareInput},
        {"SetTexture", API::ElementSetTexture},
        {"SetTransparency", API::ElementSetTransparency},
        {NULL, NULL}
    };

    const luaL_Reg API::BindMethods[] {
        {"GetKey", API::BindGetKey},
        {"GetModifier", API::BindGetModifier},

        {"New", API::BindNew},
        {"OnPress", API::ObjectOnEvent},
        {"OnRelease", API::ObjectOnEvent},
        {"SetKey", API::BindSetKey},
        {"SetModifier", API::BindSetModifier},
        {NULL, NULL}
    };

    const luaL_Reg API::ButtonMethods[] {
        {"IsChecked", API::ButtonIsChecked},

        {"New", API::ButtonNew},
        {"SetChecked", API::ButtonSetChecked},
        {NULL, NULL}
    };

    const luaL_Reg API::DialogMethods[] {
        {NULL, NULL}
    };

    const luaL_Reg API::FrameMethods[] {
        {"New", API::FrameNew},
        {"OnExit", API::ObjectOnEvent},
        {NULL, NULL}
    };

    const luaL_Reg API::ListMethods[] {
        {"GetItems", API::ListGetItems},
        {"GetSelectedItem", API::ListGetSelectedItem},

        {"New", API::ListNew},
        {"SetSelectedItem", API::ListSetSelectedItem},
        {NULL, NULL}
    };

    const luaL_Reg API::MessageMethods[] {
        {"GetText", API::MessageGetText},

        {"New", API::MessageNew},
        {"SetSize", API::MessageSetSize},
        {"SetText", API::MessageSetText},
        {"SetShadowColor", API::MessageSetShadowColor},
        {"SetShadowOffset", API::MessageSetShadowOffset},
        {NULL, NULL}
    };

    const luaL_Reg API::ParagraphMethods[] {
        {"New", API::ParagraphNew},
        {"SetText", API::ParagraphSetText},
        {NULL, NULL}
    };

    const luaL_Reg API::SliderMethods[] {
        {"GetValue", API::SliderGetValue},

        {"New", API::SliderNew},
        {"SetMaximumValue", API::SliderSetMaximumValue},
        {"SetMinimumValue", API::SliderSetMinimumValue},
        {"SetValue", API::SliderSetValue},
        {NULL, NULL}
    };

    const luaL_Reg API::StatusBarMethods[] {
        {"GetStatus", API::StatusBarGetStatus},

        {"New", API::StatusBarNew},
        {"SetStatus", API::StatusBarSetStatus},
        {NULL, NULL}
    };

    API::API() {}

    API::~API() {}
}
