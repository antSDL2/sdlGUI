//Main.cpp
#include <time.h>
#include <fstream>

#include <sdlApp/API.h>
#include <sdlApp/State.h>

#include <sdlUtility/Renderer.h>
#include <sdlUtility/Vector2.h>

#include "API.h"
#include "State.h"

using namespace sdlUtility;

int main (int argc, char *argv[]) {
    #ifdef DEBUG
        std::cout << "=== Debug Build ===============================================================" << std::endl;
    #else
        //Redirect cout and cerr
        std::ofstream Output("User/Logs/Console.log");
        std::streambuf *StandardCout = std::cout.rdbuf();
        std::streambuf *StandardCerr = std::cerr.rdbuf();

        std::cout.rdbuf(Output.rdbuf());
        std::cerr.rdbuf(Output.rdbuf());

        std::cout << "=== Release Build =============================================================" << std::endl;
    #endif

    //Feed
    srand(time(NULL));

    //Initialize Lua
    lua_State *Lua = luaL_newstate();
    if (Lua) {
        luaL_openlibs(Lua);
        std::cout << "Loaded Lua libraries." << std::endl;
    }

    //Application Instance
    sdlApp::State Application(Lua, "GLui Editor", "Graphics/Engine.png");
    sdlApp::API::Load(Lua, &Application);
    Application.SetResolution(800, 600);
    Application.AllowResize(true);

    if (Application.Start(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK)) {
        std::cout << "Started Application in " << Application.GetWidth() << "x" << Application.GetHeight() << "." << std::endl;
        Renderer::Who();
    }

    //Interface Instance
    sdlGUI::State Interface(Lua);
    sdlGUI::API::Load(Lua, &Interface);
    Interface.SetScaleMode(false);
    Interface.Initialize("Interface/", "Graphics/", "User/");
    Interface.LoadState("AtGUIEditor");

    //Main Loop
    Vector2 &Cursor = Interface.GetCursorPosition();
    do {
        //Input
        SDL_Event UserEvent;
        UserEvent.type = SDL_USEREVENT;

        SDL_Event Event;
        while (SDL_PollEvent(&Event)) {
            bool CanInput = false;

            if (Event.type == SDL_MOUSEMOTION) Cursor = Vector2(Event.motion.x, Event.motion.y); else {
                if (Event.type != SDL_MOUSEWHEEL) {
                    Event.motion.x = Cursor.X();
                    Event.motion.y = Cursor.Y();
                }
            }

            if (Event.type == SDL_MOUSEMOTION || Event.type == SDL_JOYAXISMOTION) {
                CanInput = Application.CanInput();
            } else CanInput = true;

            if (CanInput) {
                int Input = 0;
                Input = Application.Input(Event);
                if (!Input) {
                    Input = Interface.Input(Event);
                }
            }
        }

        //Process
        while (Application.CanProcess()) {
            SDL_PushEvent(&UserEvent);

            Interface.Process();
        }

        //Output
        float Interpolation = Application.GetInterpolation();

        Interface.Render(Interpolation);
    } while (Application.IsRunning());

    Interface.ClearState();

    Application.Close();

    std::cout << "===============================================================================" << std::endl;

    #ifdef DEBUG
    #else
        Output.close();

        //Reset cout and cerr
        std::cout.rdbuf(StandardCout);
        std::cerr.rdbuf(StandardCerr);
    #endif

    return 0;
}
