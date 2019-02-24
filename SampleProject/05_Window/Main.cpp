#include "Auto.h"
#include "Debug/DebugNew.h"

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include <cstdio>
#include <cstdlib>

using namespace Auto3D;

class WindowTest : public Object
{
    OBJECT(WindowTest);

public:
    void Run()
    {
        input = new Input();
        window = new Window();
        window->SetTitle("Window test");
        window->SetSize(IntVector2(800, 600), false, true);
        printf("Window opened\n");

        SubscribeToEvent(window->closeRequestEvent, &WindowTest::HandleCloseRequest);
        SubscribeToEvent(window->resizeEvent, &WindowTest::HandleResize);
        SubscribeToEvent(window->gainFocusEvent, &WindowTest::HandleGainFocus);
        SubscribeToEvent(window->loseFocusEvent, &WindowTest::HandleLoseFocus);
        SubscribeToEvent(window->minimizeEvent, &WindowTest::HandleMinimize);
        SubscribeToEvent(window->restoreEvent, &WindowTest::HandleRestore);
        SubscribeToEvent(input->mouseButtonEvent, &WindowTest::HandleMouseButton);
        SubscribeToEvent(input->mouseMoveEvent, &WindowTest::HandleMouseMove);
        SubscribeToEvent(input->keyEvent, &WindowTest::HandleKey);
        SubscribeToEvent(input->touchBeginEvent, &WindowTest::HandleTouchBegin);
        SubscribeToEvent(input->touchMoveEvent, &WindowTest::HandleTouchMove);
        SubscribeToEvent(input->touchEndEvent, &WindowTest::HandleTouchEnd);

        while (window->IsOpen())
        {
            input->Update();
            Thread::Sleep(1);
        }
        
        printf("Window closed\n");
    }

    void HandleCloseRequest(Event& /* event */)
    {
        printf("Close button pressed\n");
        window->Close();
    }

    void HandleResize(WindowResizeEvent& event)
    {
        printf("Window resized to %d %d\n", event.size.x, event.size.y);
    }

    void HandleGainFocus(Event& /* event */)
    {
        printf("Window gained focus\n");
    }

    void HandleLoseFocus(Event& /* event */)
    {
        printf("Window lost focus\n");
    }

    void HandleMinimize(Event& /* event */)
    {
        printf("Window minimized\n");
    }

    void HandleRestore(Event& /* event */)
    {
        printf("Window restored\n");
    }

    void HandleMouseMove(MouseMoveEvent& event)
    {
        printf("Mouse position %d %d delta %d %d\n", event.position.x, event.position.y, event.delta.x, event.delta.y);
    }

    void HandleMouseButton(MouseButtonEvent& event)
    {
        printf("Mouse button %d state %d\n", event.button, event.pressed ? 1 : 0);
    }

    void HandleKey(KeyEvent& event)
    {
        printf("Key code %d rawcode %d state %d\n", event.keyCode, event.rawKeyCode, event.pressed ? 1 : 0);
    }

    void HandleTouchBegin(TouchBeginEvent& event)
    {
        printf("Touch begin id %d position %d %d\n", event.id, event.position.x, event.position.y);
    }

    void HandleTouchMove(TouchMoveEvent& event)
    {
        printf("Touch move id %d position %d %d\n", event.id, event.position.x, event.position.y);
    }

    void HandleTouchEnd(TouchEndEvent& event)
    {
        printf("Touch end id %d position %d %d\n", event.id, event.position.x, event.position.y);
    }

    AutoPtr<Input> input;
    AutoPtr<Window> window;
};

int main()
{
    #ifdef _MSC_VER
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif
    
    WindowTest test;
    test.Run();

    return 0;
}