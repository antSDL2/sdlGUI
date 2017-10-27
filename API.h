//API.h
#ifndef NEOINTERFACE_API_H
#define NEOINTERFACE_API_H

#include "State.h"
#include <AtTools/AtTools.h>
#include <AtPhys/API.h>

namespace AtGLui {
    class API: protected AtPhys::API<State, Element> {
        //Members
        protected:
            static const luaL_Reg InterfaceMethods[], ElementMethods[];
            static const luaL_Reg BindMethods[], ButtonMethods[], DialogMethods[], FrameMethods[], ListMethods[], MessageMethods[], ParagraphMethods[], SliderMethods[], StatusBarMethods[];
        //Access Methods
        protected:
            API();
            ~API();
        //Process Methods
        public:
            static void Load(lua_State *Lua, State *State) {
                ObjectManager = State;

                if (ObjectManager == NULL) std::cerr << "(Interface/API.h) Load(): Interface pointer was not defined (NULL)." << std::endl;

                if (Lua == NULL) {
                    std::cerr << "(Interface/API.h) Load(): Lua pointer was not defined (NULL)." << std::endl;
                } else {
                    static const luaL_Reg Null[] {
                        {NULL, NULL}
                    };

                    //Interface
                    luaL_newlibtable(Lua, ObjectManagerMethods);
                    luaL_setfuncs(Lua, ObjectManagerMethods, 0);
                    luaL_setfuncs(Lua, InterfaceMethods, 0);
                    lua_setglobal(Lua, ObjectManagerName.c_str());

                    //GlobalEvents
                    lua_getglobal(Lua, "GlobalEvents");

                    if (lua_isnil(Lua, -1)) {
                        lua_newtable(Lua);
                    }

                    lua_newtable(Lua);
                    lua_setfield(Lua, -2, "DebugFocusChanged");

                    AtTools::Lua::ExecuteScript(Lua, "function Interface.OnGlobalEvent(Event, ...) for CallerName,CallbackValues in pairs(GlobalEvents[Event]) do local Caller = CallbackValues[0]; Caller.Callback = CallbackValues[1]; Caller.Callback(...); end end");
                    AtTools::Lua::ExecuteScript(Lua, "function Interface.RegisterEventHandler(Event, Caller, Value) if (Caller and Value and Event) then GlobalEvents[Event][Caller.ID] = {[0] = Caller, [1] = Value}; end end");
                    AtTools::Lua::ExecuteScript(Lua, "function Interface.RemoveEventHandlers(CallerID) for Event in pairs(GlobalEvents) do if (GlobalEvents[Event]) then GlobalEvents[Event][CallerID] = nil; end end end");

                    lua_setglobal(Lua, "GlobalEvents");

                    //Register classes
                    RegisterElementClass(Lua, Null, "Element");
                    RegisterElementClass(Lua, BindMethods, "Bind");
                    RegisterElementClass(Lua, ButtonMethods, "Button");
                    RegisterElementClass(Lua, DialogMethods, "Dialog");
                    RegisterElementClass(Lua, FrameMethods, "Frame");
                    RegisterElementClass(Lua, ListMethods, "List");
                    RegisterElementClass(Lua, MessageMethods, "Message");
                    RegisterElementClass(Lua, ParagraphMethods, "Paragraph");
                    RegisterElementClass(Lua, SliderMethods, "Slider");
                    RegisterElementClass(Lua, StatusBarMethods, "StatusBar");
                }
            }

        protected:
        //Common
            static void RegisterElementClass(lua_State *Lua, const luaL_Reg* Methods, std::string Type) {
                luaL_newmetatable(Lua, Type.c_str());
                luaL_setfuncs(Lua, ObjectMethods, 0);
                luaL_setfuncs(Lua, ElementMethods, 0);
                luaL_setfuncs(Lua, Methods, 0);
                lua_pushvalue(Lua, -1);
                lua_setfield(Lua, -1, "__index");
                lua_setglobal(Lua, Type.c_str());
            }

        //Interface
            //Access Functions
            static int ElementAllowsOtherInput(lua_State *Lua) {
                int Results = 1;
                bool AllowsOtherInput = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    Element *Object = GetObject<Element>(Lua);
                    if (Object) {
                        AllowsOtherInput = Object->AllowsOtherInput();
                    }
                } else std::cerr << "(Interface/API.h) ElementAllowsOtherInput(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushboolean(Lua, AllowsOtherInput);

                return Results;
            }

            static int ElementChildrenOffsetX(lua_State *Lua) {
                int Results = 1;
                int PaddingX = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    Element *Object = GetObject<Element>(Lua);
                    if (Object) {
                        PaddingX = Object->ChildrenOffsetX();
                    }
                } else std::cerr << "(Interface/API.h) ElementChildrenOffsetX(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushinteger(Lua, PaddingX);

                return Results;
            }

            static int ElementChildrenOffsetY(lua_State *Lua) {
                int Results = 1;
                int PaddingY = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    Element *Object = GetObject<Element>(Lua);
                    if (Object) {
                        PaddingY = Object->ChildrenOffsetY();
                    }
                } else std::cerr << "(Interface/API.h) ElementChildrenOffsetY(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushinteger(Lua, PaddingY);

                return Results;
            }

            static int GetCursorX(lua_State *Lua) {
                int Results = 1;
                int CursorX = 0;

                if (ObjectManager) {
                    AtPhys::Vector2 Cursor = ObjectManager->GetCursorPosition();
                    CursorX = Cursor.X();
                }

                lua_pushinteger(Lua, CursorX);

                return Results;
            }

            static int GetCursorY(lua_State *Lua) {
                int Results = 1;
                int CursorY = 0;

                if (ObjectManager) {
                    AtPhys::Vector2 Cursor = ObjectManager->GetCursorPosition();
                    CursorY = Cursor.Y();
                }

                lua_pushinteger(Lua, CursorY);

                return Results;
            }

            static int GetElementClass(lua_State *Lua) {
                int Results = 1;
                std::string Class;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 1) {
                        std::string ID = luaL_checkstring(Lua, 1);
                        Class = ObjectManager->GetElementClass(ID);
                    } else std::cerr << "(Interface/API.h) GetElementClass(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                lua_pushstring(Lua, Class.c_str());

                return Results;
            }

            static int GetFocusElement(lua_State *Lua) {
                int Results = 1;
                std::string ID;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    Element *Object = NULL;

                    if (Arguments == 1) {
                        Object = ObjectManager->GetFocus(true);
                    } else {
                        Object = ObjectManager->GetFocus();
                    }

                    if (Object) {
                        ID = Object->GetID();
                    }
                }

                lua_pushstring(Lua, ID.c_str());

                return Results;
            }

            static int IsShown(lua_State *Lua) {
                int Results = 1;
                bool IsShown = false;

                if (ObjectManager) {
                    IsShown = ObjectManager->IsShown();
                }

                lua_pushboolean(Lua, IsShown);

                return Results;
            }

            //Process Functions
            static int BrowseTextboxes(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    ObjectManager->BrowseTextboxes();
                }

                return Results;
            }

            static int DisableKeyBindings(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    ObjectManager->DisableKeyBindings();
                }

                return Results;
            }

            static int EnableKeyBindings(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    ObjectManager->EnableKeyBindings();
                }

                return Results;
            }

            static int Execute(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 1) {
                        std::string Script = lua_tostring(Lua, 1);

                        AtTools::Lua::ExecuteScript(Lua, Script.c_str());
                    } else std::cerr << "(Interface/API.h) Execute(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int Hide(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 0) {
                        ObjectManager->Hide();
                    } else std::cerr << "(Interface/API.h) Hide(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int LoadAddOnData(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 1) {
                        std::string AddOn = luaL_checkstring(Lua, 1);
                        ObjectManager->LoadAddOnData(AddOn);

                    } else std::cerr << "(Interface/API.h) LoadAddOnData(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int LoadCursor(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 1) {
                        std::string Location = lua_tostring(Lua, 1);
                        ObjectManager->LoadCursor(Location);
                    } else std::cerr << "(Interface/API.h) LoadCursor(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int MakeBind(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 3) {
                        std::string AddOn = lua_tostring(Lua, 1);
                        std::string Name = lua_tostring(Lua, 2);
                        std::string ParentName = lua_tostring(Lua, 3);
                        std::string ID = AddOn+"."+Name;

                        ObjectManager->MakeElement(Lists::Binds, AddOn, Name, ParentName);
                        MakeObjectInstance(Lua, ID, "Bind");

                        Results = 1;
                    } else std::cerr << "(Interface/API.h) MakeBind(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int MakeButton(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 3) {
                        std::string AddOn = lua_tostring(Lua, 1);
                        std::string Name = lua_tostring(Lua, 2);
                        std::string ParentName = lua_tostring(Lua, 3);
                        std::string ID = AddOn+"."+Name;

                        ObjectManager->MakeElement(Lists::Buttons, AddOn, Name, ParentName);
                        MakeObjectInstance(Lua, ID, "Button");

                        Results = 1;
                    } else std::cerr << "(Interface/API.h) MakeButton(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int MakeDialog(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 3) {
                        std::string AddOn = lua_tostring(Lua, 1);
                        std::string Name = lua_tostring(Lua, 2);
                        std::string ParentName = lua_tostring(Lua, 3);
                        std::string ID = AddOn+"."+Name;

                        ObjectManager->MakeElement(Lists::Dialogs, AddOn, Name, ParentName);
                        MakeObjectInstance(Lua, ID, "Dialog");

                        Results = 1;
                    } else std::cerr << "(Interface/API.h) MakeDialog(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int MakeFrame(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 3) {
                        std::string AddOn = lua_tostring(Lua, 1);
                        std::string Name = lua_tostring(Lua, 2);
                        std::string ParentName = lua_tostring(Lua, 3);
                        std::string ID = AddOn+"."+Name;

                        ObjectManager->MakeElement(Lists::Frames, AddOn, Name, ParentName);
                        MakeObjectInstance(Lua, ID, "Frame");

                        Results = 1;
                    } else std::cerr << "(Interface/API.h) MakeFrame(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int MakeList(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 3) {
                        std::string AddOn = lua_tostring(Lua, 1);
                        std::string Name = lua_tostring(Lua, 2);
                        std::string ParentName = lua_tostring(Lua, 3);
                        std::string ID = AddOn+"."+Name;

                        ObjectManager->MakeElement(Lists::Lists, AddOn, Name, ParentName);
                        MakeObjectInstance(Lua, ID, "List");

                        Results = 1;
                    } else std::cerr << "(Interface/API.h) MakeList(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int MakeMessage(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 3) {
                        std::string AddOn = lua_tostring(Lua, 1);
                        std::string Name = lua_tostring(Lua, 2);
                        std::string ParentName = lua_tostring(Lua, 3);
                        std::string ID = AddOn+"."+Name;

                        ObjectManager->MakeElement(Lists::Messages, AddOn, Name, ParentName);
                        MakeObjectInstance(Lua, ID, "Message");

                        Results = 1;
                    } else std::cerr << "(Interface/API.h) MakeMessage(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int MakeParagraph(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 3) {
                        std::string AddOn = lua_tostring(Lua, 1);
                        std::string Name = lua_tostring(Lua, 2);
                        std::string ParentName = lua_tostring(Lua, 3);
                        std::string ID = AddOn+"."+Name;

                        ObjectManager->MakeElement(Lists::Paragraphs, AddOn, Name, ParentName);
                        MakeObjectInstance(Lua, ID, "Paragraph");

                        Results = 1;
                    } else std::cerr << "(Interface/API.h) MakeParagraph(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int MakeSlider(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 3) {
                        std::string AddOn = lua_tostring(Lua, 1);
                        std::string Name = lua_tostring(Lua, 2);
                        std::string ParentName = lua_tostring(Lua, 3);
                        std::string ID = AddOn+"."+Name;

                        ObjectManager->MakeElement(Lists::Sliders, AddOn, Name, ParentName);
                        MakeObjectInstance(Lua, ID, "Slider");

                        Results = 1;
                    } else std::cerr << "(Interface/API.h) MakeSlider(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int MakeStatusBar(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 3) {
                        std::string AddOn = lua_tostring(Lua, 1);
                        std::string Name = lua_tostring(Lua, 2);
                        std::string ParentName = lua_tostring(Lua, 3);
                        std::string ID = AddOn+"."+Name;

                        ObjectManager->MakeElement(Lists::StatusBars, AddOn, Name, ParentName);
                        MakeObjectInstance(Lua, ID, "StatusBar");

                        Results = 1;
                    } else std::cerr << "(Interface/API.h) MakeStatusBar(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int RemoveBind(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 1) {
                        std::string ID = lua_tostring(Lua, 1);

                        ObjectManager->RemoveElement(Lists::Binds, ID);
                    } else std::cerr << "(Interface/API.h) RemoveBind(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int RemoveButton(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 1) {
                        std::string ID = lua_tostring(Lua, 1);

                        ObjectManager->RemoveElement(Lists::Buttons, ID);
                    } else std::cerr << "(Interface/API.h) RemoveButton(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int RemoveDialog(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 1) {
                        std::string ID = lua_tostring(Lua, 1);

                        ObjectManager->RemoveElement(Lists::Dialogs, ID);
                    } else std::cerr << "(Interface/API.h) RemoveDialog(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int RemoveFrame(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 1) {
                        std::string ID = lua_tostring(Lua, 1);

                        ObjectManager->RemoveElement(Lists::Frames, ID);
                    } else std::cerr << "(Interface/API.h) RemoveFrame(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int RemoveList(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 1) {
                        std::string ID = lua_tostring(Lua, 1);

                        ObjectManager->RemoveElement(Lists::Lists, ID);
                    } else std::cerr << "(Interface/API.h) RemoveList(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int RemoveMessage(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 1) {
                        std::string ID = lua_tostring(Lua, 1);

                        ObjectManager->RemoveElement(Lists::Messages, ID);
                    } else std::cerr << "(Interface/API.h) RemoveMessage(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int RemoveParagraph(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 1) {
                        std::string ID = lua_tostring(Lua, 1);

                        ObjectManager->RemoveElement(Lists::Paragraphs, ID);
                    } else std::cerr << "(Interface/API.h) RemoveParagraph(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int RemoveSlider(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 1) {
                        std::string ID = lua_tostring(Lua, 1);

                        ObjectManager->RemoveElement(Lists::Sliders, ID);
                    } else std::cerr << "(Interface/API.h) RemoveSlider(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int RemoveStatusBar(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 1) {
                        std::string ID = lua_tostring(Lua, 1);

                        ObjectManager->RemoveElement(Lists::StatusBars, ID);
                    } else std::cerr << "(Interface/API.h) RemoveStatusBar(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int ReleaseTextures(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    AtGfx::TextureManager *TextureManager = ObjectManager->GetTextureManager();
                    TextureManager->ReleaseTextures();
                }

                return Results;
            }

            static int SaveAddOnData(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    std::string AddOn = luaL_checkstring(Lua, 1);
                    std::string SaveData;

                    if (lua_istable(Lua, 2)) {
                        lua_pushnil(Lua);  /* first key */
                        while (lua_next(Lua, 2) != 0) {
                            std::string TableElement;
                            /* uses 'key' (at index -2) and 'value' (at index -1) */
                            int Type = lua_type(Lua, -2);
                            switch (Type) {
                                case LUA_TSTRING:
                                    TableElement = AddOn+".SaveData[\"" + lua_tostring(Lua, -2) + "\"]";
                                    SaveData = SaveData + "if (" + TableElement + " ~= nil) then " + TableElement + " = ";
                                    break;
                                case LUA_TBOOLEAN:
                                    break;
                                case LUA_TNUMBER:
                                    TableElement = AddOn+".SaveData[" + luaL_checkstring(Lua, -2) + "]";
                                    SaveData = SaveData + "if (" + TableElement + " ~= nil) then " + TableElement + " = ";
                                    break;
                                default:
                                    break;
                            }

                            Type = lua_type(Lua, -1);
                            switch (Type) {
                                case LUA_TSTRING:
                                    SaveData = SaveData + "\"" + lua_tostring(Lua, -1) + "\"; end\n";
                                    break;
                                case LUA_TBOOLEAN: {
                                    bool Value = lua_toboolean(Lua, -1);
                                    if (Value) {
                                        SaveData = SaveData + "true; end\n";
                                    } else {
                                        SaveData = SaveData + "false; end\n";
                                    }
                                }
                                    break;
                                case LUA_TNUMBER:
                                    SaveData = SaveData + luaL_checkstring(Lua, -1) + " end;\n";
                                    break;
                                default:
                                    break;
                            }

                            /* removes 'value'; keeps 'key' for next iteration */
                            lua_pop(Lua, 1);
                        }
                    }

                    ObjectManager->SaveAddOnData(AddOn, SaveData);
                }

                return Results;
            }

            static int Show(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 0) {
                        ObjectManager->Show();
                    } else std::cerr << "(Interface/API.h) Show(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int ShowCursor(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 1) {
                        bool State = lua_toboolean(Lua, 1);

                        SDL_ShowCursor(State);
                    } else std::cerr << "(Interface/API.h) ShowCursor(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int Toggle(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 0) {
                        if (ObjectManager) {
                            ObjectManager->Toggle();
                        }
                    } else std::cerr << "(Interface/API.h) Toggle(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

        //Element
            //Access Functions
            static int ElementClipsContents(lua_State *Lua) {
                int Results = 1;
                bool ClipsContents = false;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    Element *Object = GetObject<Element>(Lua);
                    if (Object) {
                        ClipsContents = Object->ClipsContents();
                    }
                } else std::cerr << "(Interface/API.h) ElementClipsContents(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushboolean(Lua, ClipsContents);

                return Results;
            }

            static int ElementCopiesInput(lua_State *Lua) {
                int Results = 1;
                bool CopiesInput = false;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    Element *Object = GetObject<Element>(Lua);
                    if (Object) {
                        CopiesInput = Object->CopiesInput();
                    }
                } else std::cerr << "(Interface/API.h) ElementCopiesInput(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushboolean(Lua, CopiesInput);

                return Results;
            }

            static int ElementGetClass(lua_State *Lua) {
                int Results = 1;
                std::string Class = "(None)";

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    Element *Object = GetObject<Element>(Lua);
                    if (Object) {
                        Class = ObjectManager->GetElementClass(Object->GetID());
                    }
                } else std::cerr << "(Interface/API.h) ElementGetClass(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushstring(Lua, Class.c_str());

                return Results;
            }

            static int ElementGetParent(lua_State *Lua) {
                int Results = 1;
                std::string ParentID = "(None)";

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    Element *Object = GetObject<Element>(Lua);
                    if (Object) {
                        Element *Parent = Object->GetParent();
                        if (Parent) {
                            ParentID = Parent->GetID();
                        }
                    }
                } else std::cerr << "(Interface/API.h) ElementGetParent(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushstring(Lua, ParentID.c_str());

                return Results;
            }

            static int ElementGetTexture(lua_State *Lua) {
                int Results = 1;
                std::string Texture;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    Element *Object = GetObject<Element>(Lua);
                    if (Object) {
                        AtGfx::Renderable *Renderable = Object->GetRenderable();
                        Texture = Renderable->GetTextureID();
                    }
                } else std::cerr << "(Interface/API.h) ElementGetTexture(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushstring(Lua, Texture.c_str());

                return Results;
            }

            static int ElementIsContainer(lua_State *Lua) {
                int Results = 1;
                bool IsContainer = false;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    Element *Object = GetObject<Element>(Lua);
                    if (Object) {
                        IsContainer = Object->IsContainer();
                    }

                } else std::cerr << "(Interface/API.h) ElementIsContainer(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushboolean(Lua, IsContainer);

                return Results;
            }

            static int ElementIsDisabled(lua_State *Lua) {
                int Results = 1;
                bool IsDisabled = false;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    Element *Object = GetObject<Element>(Lua);
                    if (Object) {
                        IsDisabled = Object->IsDisabled();
                    }

                } else std::cerr << "(Interface/API.h) ElementIsDisabled(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushboolean(Lua, IsDisabled);

                return Results;
            }

            static int ElementSharesInput(lua_State *Lua) {
                int Results = 1;
                bool SharesInput = false;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    Element *Object = GetObject<Element>(Lua);
                    if (Object) {
                        SharesInput = Object->SharesInput();
                    }

                } else std::cerr << "(Interface/API.h) ElementSharesInput(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushboolean(Lua, SharesInput);

                return Results;
            }

            //Process Functions
            static int ElementFocus(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 1 || Arguments == 2) {
                        Element *Object = GetObject<Element>(Lua);

                        if (Object) {
                            std::string ID = Object->GetID();

                            if (Arguments == 2) ObjectManager->SetFocus(ID, true); else ObjectManager->SetFocus(ID);
                        }
                    } else std::cerr << "(Interface/API.h) ElementFocus(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int ElementNew(lua_State *Lua) {
                int Results = 1;
                std::string ID;

                if (lua_gettop(Lua) == 1) {
                    ID = luaL_checkstring(Lua, 1);
                } else std::cerr << "(Interface/API.h) ElementNew(): Function called with invalid number of arguments (" << lua_gettop(Lua) << ")." << std::endl;

                MakeObjectInstance(Lua, ID, "Element");

                return Results;
            }

            static int ElementRelease(lua_State *Lua) {
                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 1 || Arguments == 2) {
                        Element *Object = GetObject<Element>(Lua);
                        if (Object) {
                            Element *FocusElement = NULL;
                            if (Arguments == 2) FocusElement = ObjectManager->GetFocus(true); else FocusElement = ObjectManager->GetFocus();

                            if (Object == FocusElement) {
                                if (Arguments == 2) ObjectManager->SetFocus("", true); else ObjectManager->SetFocus("");
                            }
                        }
                    } else std::cerr << "(Interface/API.h) ElementRelease(): Function called with invalid number of arguments (" << lua_gettop(Lua) << ")." << std::endl;
                }

                return 0;
            }

            static int ElementEnableOtherInput(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    bool State = lua_toboolean(Lua, 2);

                    Element *Object = GetObject<Element>(Lua);
                    if (Object) {
                        Object->EnableOtherInput(State);
                    }
                } else std::cerr << "(Interface/API.h) ElementEnableOtherInput(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ElementRemove(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    Element *Object = GetObject<Element>(Lua);
                    if (Object) {
                        std::string Class = ObjectManager->GetElementClass(Object->GetID());
                        int List = 0;

                        if (Class == "Bind") {
                            List = Lists::Binds;
                        } else if (Class == "Button") {
                            List = Lists::Buttons;
                        } else if (Class == "Dialog") {
                            List = Lists::Dialogs;
                        } else if (Class == "Frame") {
                            List = Lists::Frames;
                        } else if (Class == "List") {
                            List = Lists::Lists;
                        } else if (Class == "Message") {
                            List = Lists::Messages;
                        } else if (Class == "Paragraph") {
                            List = Lists::Paragraphs;
                        } else if (Class == "Slider") {
                            List = Lists::Sliders;
                        } else if (Class == "StatusBar") {
                            List = Lists::StatusBars;
                        }

                        ObjectManager->RemoveElement(List, Object->GetID());
                    }
                } else std::cerr << "(Interface/API.h) ElementRemove(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ElementSetColor(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    std::string ColorString = lua_tostring(Lua, 2);

                    int Color[4]; Color[0] = Color[1] = Color[2] = -1; Color[3] = 255;
                    for (int i = 0; i<3; i++) Color[i] = AtTools::Strings::StringTo<int>(ColorString, ' ', i);

                    Element *Object = GetObject<Element>(Lua);
                    if (Object) {
                        AtGfx::Renderable *Renderable = Object->GetRenderable();
                        Renderable->SetIdleColor(Color);
                    }
                } else std::cerr << "(Interface/API.h) ElementSetColor(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ElementSetClipContents(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    bool State = lua_toboolean(Lua, 2);

                    Element *Object = GetObject<Element>(Lua);
                    if (Object) {
                        Object->SetClipContents(State);
                    }
                } else std::cerr << "(Interface/API.h) ElementSetClipContents(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ElementSetContainer(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    bool State = lua_toboolean(Lua, 2);

                    Element *Object = GetObject<Element>(Lua);
                    if (Object) {
                        Object->SetContainer(State);
                    }
                } else std::cerr << "(Interface/API.h) ElementSetContainer(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ElementSetCopyInput(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    bool State = lua_toboolean(Lua, 2);

                    Element *Object = GetObject<Element>(Lua);
                    if (Object) {
                        Object->SetCopyInput(State);
                    }
                } else std::cerr << "(Interface/API.h) ElementSetCopyInput(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ElementSetDisabled(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    bool State = lua_toboolean(Lua, 2);

                    Element *Object = GetObject<Element>(Lua);
                    if (Object) {
                        Object->SetDisabled(State);
                    }
                } else std::cerr << "(Interface/API.h) ElementSetDisabled(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ElementSetFont(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 2) {
                        if (lua_istable(Lua, 1)) {
                            lua_getfield(Lua, 1, "ID");
                            std::string ID = luaL_checkstring(Lua, 3);
                            std::string Font = lua_tostring(Lua, 2);

                            ObjectManager->SetElementFont(ID, Font);
                        }
                    } else std::cerr << "(Interface/API.h) ElementSetFont(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int ElementSetShareInput(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    bool State = lua_toboolean(Lua, 2);

                    Element *Object = GetObject<Element>(Lua);
                    if (Object) {
                        Object->SetShareInput(State);
                    }
                } else std::cerr << "(Interface/API.h) ElementSetShareInput(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ElementSetTexture(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 2) {
                        if (lua_istable(Lua, 1)) {
                            lua_getfield(Lua, 1, "ID");
                            std::string ID = luaL_checkstring(Lua, 3);
                            std::string Texture = lua_tostring(Lua, 2);

                            ObjectManager->SetElementTexture(ID, Texture);
                        }
                    } else std::cerr << "(Interface/API.h) ElementSetTexture(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int ElementSetTransparency(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 2) {
                        float Transparency = lua_tonumber(Lua, 2);

                        Element *Object = GetObject<Element>(Lua);
                        if (Object) {
                            AtGfx::Renderable *Renderable = Object->GetRenderable();
                            Renderable->SetTransparency(Transparency);
                        }
                    } else std::cerr << "(Interface/API.h) ElementSetTransparency(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

        //Bind
            //Access Functions
            static int BindGetKey(lua_State *Lua) {
                int Results = 1;
                int Key = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    Bind *Object = GetObject<Bind>(Lua);
                    if (Object) {
                        Key = Object->GetKey();
                    }
                } else std::cerr << "(Interface/API.h) BindGetKey(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushinteger(Lua, Key);

                return Results;
            }

            static int BindGetModifier(lua_State *Lua) {
                int Results = 1;
                int Modifier = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    Bind *Object = GetObject<Bind>(Lua);
                    if (Object) {
                        Modifier = Object->GetModifier();
                    }
                } else std::cerr << "(Interface/API.h) BindGetModifier(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushinteger(Lua, Modifier);

                return Results;
            }

            //Process Functions
            static int BindNew(lua_State *Lua) {
                int Results = 1;
                std::string ID;

                if (lua_gettop(Lua) == 1) {
                    ID = luaL_checkstring(Lua, 1);
                } else std::cerr << "(Interface/API.h) BindNew(): Function called with invalid number of arguments (" << lua_gettop(Lua) << ")." << std::endl;

                MakeObjectInstance(Lua, ID, "Bind");

                return Results;
            }

            static int BindSetKey(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    Bind *Object = GetObject<Bind>(Lua);
                    if (Object) {
                        SDL_Scancode Key = (SDL_Scancode)lua_tointeger(Lua, 2);
                        Object->SetKey(Key);
                    }
                } else std::cerr << "(Interface/API.h) BindSetKey(): Function called with invalid number of arguments (" << lua_gettop(Lua) << ")." << std::endl;

                return Results;
            }

            static int BindSetModifier(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    Bind *Object = GetObject<Bind>(Lua);
                    if (Object) {
                        SDL_Scancode Modifier = (SDL_Scancode)lua_tointeger(Lua, 2);
                        Object->SetModifier(Modifier);
                    }
                } else std::cerr << "(Interface/API.h) BindSetModifier(): Function called with invalid number of arguments (" << lua_gettop(Lua) << ")." << std::endl;

                return Results;
            }
        //Button
            //Access Functions
            static int ButtonIsChecked(lua_State *Lua) {
                int Results = 1;
                bool Checked = false;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    Button *Object = GetObject<Button>(Lua);
                    if (Object) {
                        Checked = Object->IsChecked();
                    }
                } else std::cerr << "(Interface/API.h) ButtonIsChecked(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushboolean(Lua, Checked);

                return Results;
            }

            //Process Functions
            static int ButtonNew(lua_State *Lua) {
                int Results = 1;
                std::string ID;

                if (lua_gettop(Lua) == 1) {
                    ID = luaL_checkstring(Lua, 1);
                } else std::cerr << "(Interface/API.h) ButtonNew(): Function called with invalid number of arguments (" << lua_gettop(Lua) << ")." << std::endl;

                MakeObjectInstance(Lua, ID, "Button");

                return Results;
            }

            static int ButtonSetChecked(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    bool State = lua_toboolean(Lua, 2);

                    Button *Object = GetObject<Button>(Lua);
                    if (Object) {
                        Object->SetChecked(State);
                    }
                } else std::cerr << "(Interface/API.h) ButtonSetChecked(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

        //Frame
            //Process Functions
            static int FrameNew(lua_State *Lua) {
                int Results = 1;
                std::string ID;

                if (lua_gettop(Lua) == 1) {
                    ID = luaL_checkstring(Lua, 1);
                } else std::cerr << "(Interface/API.h) FrameNew(): Function called with invalid number of arguments (" << lua_gettop(Lua) << ")." << std::endl;

                MakeObjectInstance(Lua, ID, "Frame");

                return Results;
            }

        //List
            //Access Functions
            static int ListGetItems(lua_State *Lua) {
                int Results = 1;
                int Items = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    List *Object = GetObject<List>(Lua);
                    if (Object) {
                        Items = Object->GetItems();
                    }
                } else std::cerr << "(Interface/API.h) ListGetItems(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushinteger(Lua, Items);

                return Results;
            }

            static int ListGetSelectedItem(lua_State *Lua) {
                int Results = 1;

                std::string SelectedItemID;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    List *Object = GetObject<List>(Lua);
                    if (Object) {
                        Element *Element = Object->GetSelectedItem();
                        if (Element) {
                            SelectedItemID = Element->GetID();
                        }
                    }
                } else std::cerr << "(Interface/API.h) ListGetSelectedItem(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushstring(Lua, SelectedItemID.c_str());

                return Results;
            }

            //Process Functions
            static int ListNew(lua_State *Lua) {
                int Results = 1;
                std::string ID;

                if (lua_gettop(Lua) == 1) {
                    ID = luaL_checkstring(Lua, 1);
                } else std::cerr << "(Interface/API.h) ListNew(): Function called with invalid number of arguments (" << lua_gettop(Lua) << ")." << std::endl;

                MakeObjectInstance(Lua, ID, "List");

                return Results;
            }

            static int ListSetSelectedItem(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    std::string ID = lua_tostring(Lua, 2);

                    List *Object = GetObject<List>(Lua);
                    if (Object) {
                        Element *Element = NULL;
                        ObjectManager->Get(Element, ID);

                        if (Element) {
                            Object->SetSelectedItem(Element);
                        } else Object->SetSelectedItem(NULL);
                    }
                } else std::cerr << "(Interface/API.h) ListSetSelectedItem(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

        //Message
            //Access Functions
            static int MessageGetText(lua_State *Lua) {
                int Results = 1;
                std::string Text;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    Message *Object = GetObject<Message>(Lua);
                    if (Object) {
                        Text = Object->GetText();
                    }
                } else std::cerr << "(Interface/API.h) MessageGetText(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushstring(Lua, Text.c_str());

                return Results;
            }

            //Process Functions
            static int MessageNew(lua_State *Lua) {
                int Results = 1;
                std::string ID;

                if (lua_gettop(Lua) == 1) {
                    ID = luaL_checkstring(Lua, 1);
                } else std::cerr << "(Interface/API.h) MessageNew(): Function called with invalid number of arguments (" << lua_gettop(Lua) << ")." << std::endl;

                MakeObjectInstance(Lua, ID, "Message");

                return Results;
            }

            static int MessageSetSize(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    int Size = lua_tointeger(Lua, 2);

                    Message *Object = GetObject<Message>(Lua);
                    if (Object) {
                        Object->SetSize(Size);
                    }
                } else std::cerr << "(Interface/API.h) MessageSetSize(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int MessageSetText(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    std::string Text = lua_tostring(Lua, 2);

                    Message *Object = GetObject<Message>(Lua);
                    if (Object) {
                        Object->SetText(Text);
                    }
                } else std::cerr << "(Interface/API.h) MessageSetText(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int MessageSetShadowColor(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    Message *Object = GetObject<Message>(Lua);
                    if (Object) {
                        std::string ColorString = lua_tostring(Lua, 2);

                        int Color[3]; Color[0] = Color[1] = Color[2] = -1;
                        for (int i = 0; i<3; i++) Color[i] = AtTools::Strings::StringTo<int>(ColorString, ' ', i);

                        Object->SetShadowColor(Color);
                    }
                } else std::cerr << "(Interface/API.h) MessageSetShadowColor(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int MessageSetShadowOffset(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 3) {
                    Message *Object = GetObject<Message>(Lua);
                    if (Object) {
                        int X = lua_tointeger(Lua, 2);
                        int Y = lua_tointeger(Lua, 3);

                        Object->SetShadowOffset(X, Y);
                    }
                } else std::cerr << "(Interface/API.h) MessageSetShadowOffset(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

        //Paragraph
            //Process Functions
            static int ParagraphNew(lua_State *Lua) {
                int Results = 1;
                std::string ID;

                if (lua_gettop(Lua) == 1) {
                    ID = luaL_checkstring(Lua, 1);
                } else std::cerr << "(Interface/API.h) ParagraphNew(): Function called with invalid number of arguments (" << lua_gettop(Lua) << ")." << std::endl;

                MakeObjectInstance(Lua, ID, "Paragraph");

                return Results;
            }

            static int ParagraphSetText(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 2) {
                        if (lua_istable(Lua, 1)) {
                            lua_getfield(Lua, 1, "ID");
                            std::string ID = luaL_checkstring(Lua, 3);
                            std::string Text = lua_tostring(Lua, 2);
                            ObjectManager->LoadParagraphText(ID, Text);
                        }
                    } else std::cerr << "(Interface/API.h) ParagraphSetText(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

        //Slider
            //Access Functions
            static int SliderGetValue(lua_State *Lua) {
                int Results = 1;
                float Value = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    Slider *Object = GetObject<Slider>(Lua);
                    if (Object) {
                        Value = Object->GetSliderValue();
                    }
                } else std::cerr << "(Interface/API.h) SliderGetValue(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushnumber(Lua, Value);

                return Results;
            }

            //Process Functions
            static int SliderNew(lua_State *Lua) {
                int Results = 1;
                std::string ID;

                if (lua_gettop(Lua) == 1) {
                    ID = luaL_checkstring(Lua, 1);
                } else std::cerr << "(Interface/API.h) SliderNew(): Function called with invalid number of arguments (" << lua_gettop(Lua) << ")." << std::endl;

                MakeObjectInstance(Lua, ID, "Slider");

                return Results;
            }

            static int SliderSetMaximumValue(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    float MaximumValue = lua_tonumber(Lua, 2);

                    Slider *Object = GetObject<Slider>(Lua);
                    if (Object) {
                        Object->SetMaximumValue(MaximumValue);
                    }
                } else std::cerr << "(Interface/API.h) SliderSetMaximumValue(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int SliderSetMinimumValue(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    float MinimumValue = lua_tonumber(Lua, 2);

                    Slider *Object = GetObject<Slider>(Lua);
                    if (Object) {
                        Object->SetMinimumValue(MinimumValue);
                    }
                } else std::cerr << "(Interface/API.h) SliderSetMinimumValue(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int SliderSetValue(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    float Value = lua_tonumber(Lua, 2);

                    Slider *Object = GetObject<Slider>(Lua);
                    if (Object) {
                        Object->SetSliderValue(Value);
                    }
                } else std::cerr << "(Interface/API.h) SliderSetValue(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

        //StatusBar
            //Process Functions
            static int StatusBarNew(lua_State *Lua) {
                int Results = 1;
                std::string ID;

                if (lua_gettop(Lua) == 1) {
                    ID = luaL_checkstring(Lua, 1);
                } else std::cerr << "(Interface/API.h) StatusBarNew(): Function called with invalid number of arguments (" << lua_gettop(Lua) << ")." << std::endl;

                MakeObjectInstance(Lua, ID, "StatusBar");

                return Results;
            }

            static int StatusBarGetStatus(lua_State *Lua) {
                int Results = 1;
                float Status = 0.f;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    StatusBar *Object = GetObject<StatusBar>(Lua);
                    if (Object) {
                        Status = Object->GetStatus();
                    }
                } else std::cerr << "(Interface/API.h) StatusBarGetStatus(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushnumber(Lua, Status);

                return Results;
            }

            static int StatusBarSetStatus(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    float StatusPerCent = lua_tonumber(Lua, 2);

                    StatusBar *Object = GetObject<StatusBar>(Lua);
                    if (Object) {
                        Object->SetStatus(StatusPerCent);
                    }
                } else std::cerr << "(Interface/API.h) StatusBarSetStatus(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }
    };
}

#endif
