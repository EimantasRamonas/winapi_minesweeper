#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include "menu.h"

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{

    HWND hwnd;               /* This is the handle for our window */
    HWND game_hwnd;          // game window handle
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /// The Window structure
{
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;


    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

}
    /// Main program window
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Mano pirmas langas"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           800,                 /* The programs width */
           600,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           LoadMenu(hThisInstance, MAKEINTRESOURCE(IDR_MYMENU)),                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /// Game window

    /*
    game_hwnd = CreateWindowEx (
           0,                   // Extended possibilites for variation
           szClassName,         // Classname
           _T("Game window"),       // Title Text
           WS_OVERLAPPEDWINDOW, // default window
           CW_USEDEFAULT,       // Windows decides the position
           CW_USEDEFAULT,       // where the window ends up on the screen
           500,                 // The programs width
           500,                 // and height in pixels
           HWND_DESKTOP,        // The window is a child-window to desktop
           NULL,                // No menu
           hThisInstance,       // Program Instance handler
           NULL                 // No Window Creation data
           );
    */


    {   /// Main window buttons
    /// Play button
        HWND playButton = CreateWindow(
        "BUTTON",  // Predefined class; Unicode assumed
        "Play",      // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        300,         // x position
        200,         // y position
        200,        // Button width
        50,        // Button height
        hwnd,     // Parent window
        (HMENU) PLAY_BUTTON,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.


    /// Stats button
        HWND statsButton = CreateWindow(
        "BUTTON",  // Predefined class; Unicode assumed
        "Stats",      // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        300,         // x position
        275,         // y position
        200,        // Button width
        50,        // Button height
        hwnd,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.


    /// Settings button
        HWND settingsButton = CreateWindow(
        "BUTTON",  // Predefined class; Unicode assumed
        "Settings",      // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        300,         // x position
        350,         // y position
        200,        // Button width
        50,        // Button height
        hwnd,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.


    /// Exit button
        HWND exitButton = CreateWindow(
        "BUTTON",  // Predefined class; Unicode assumed
        "Exit",      // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
        300,         // x position
        425,         // y position
        200,        // Button width
        50,        // Button height
        hwnd,     // Parent window
        (HMENU) ID_FILE_EXIT,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.
    }


    /// Centering the Window
    {
    RECT rc;
    GetWindowRect ( hwnd, &rc ) ;
    int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right)/2;
    int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom)/2;
    SetWindowPos( hwnd, HWND_TOPMOST, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }


    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /// Message listener
    {
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
    }
}


/*  This function is called by the Windows function DispatchMessage()  */

    /// Message functionality
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_COMMAND:
            switch(LOWORD(wParam)){

                case PLAY_BUTTON:
                    // ShowWindow (hwnd, nCmdShow);


                    break;


                case ID_FILE_EXIT:
                    SendMessage(hwnd, WM_CLOSE, 0, 0);
                    break;

                case ID_STUFF_GO:
                    MessageBox(hwnd, "YAY, pavyko", "Antraste", MB_OK);
                    break;

            }
            break;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}


