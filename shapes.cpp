#include <windows.h>
#include <vector>
#include <cstring>
#include <commdlg.h>


#define ID_ADD_CIRCLE 1001
#define ID_REMOVE_ALL 1002
#define ID_ADD_SQUARE 1003



enum ShapeType
{
    CIRCLE,
    SQUARE
};



struct Shape
{
    int x;
    int y;

    double scale;

    COLORREF color;

    ShapeType type;
};



std::vector<Shape> shapes;



bool placingShape = false;

ShapeType placingType = CIRCLE;


bool growing = false;
bool shrinking = false;



LRESULT CALLBACK WindowProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{

    switch(uMsg)
    {


        // ================= DRAW =================

        case WM_PAINT:
        {
            PAINTSTRUCT ps;

            HDC hdc = BeginPaint(
                hwnd,
                &ps
            );



            for(auto& shape : shapes)
            {

                HPEN pen = CreatePen(
                    PS_SOLID,
                    3,
                    shape.color
                );


                HPEN oldPen =
                    (HPEN)SelectObject(
                        hdc,
                        pen
                    );



                if(shape.type == CIRCLE)
                {
                    Ellipse(
                        hdc,
                        shape.x,
                        shape.y,
                        (int)(shape.x + 100 * shape.scale),
                        (int)(shape.y + 100 * shape.scale)
                    );
                }



                if(shape.type == SQUARE)
                {
                    Rectangle(
                        hdc,
                        shape.x,
                        shape.y,
                        (int)(shape.x + 100 * shape.scale),
                        (int)(shape.y + 100 * shape.scale)
                    );
                }



                SelectObject(
                    hdc,
                    oldPen
                );


                DeleteObject(pen);
            }



            if(placingShape)
            {

                const char text[] =
                    "Click where you want the shape";


                TextOut(
                    hdc,
                    10,
                    10,
                    text,
                    strlen(text)
                );

            }



            EndPaint(
                hwnd,
                &ps
            );


            return 0;
        }

        // ================= MENU =================

        case WM_COMMAND:
        {

            switch(LOWORD(wParam))
            {


                case ID_ADD_CIRCLE:
                {

                    placingType = CIRCLE;

                    placingShape = true;


                    InvalidateRect(
                        hwnd,
                        NULL,
                        FALSE
                    );


                    return 0;
                }



                case ID_ADD_SQUARE:
                {

                    placingType = SQUARE;

                    placingShape = true;


                    InvalidateRect(
                        hwnd,
                        NULL,
                        FALSE
                    );


                    return 0;
                }



                case ID_REMOVE_ALL:
                {

                    shapes.clear();


                    placingShape = false;


                    InvalidateRect(
                        hwnd,
                        NULL,
                        TRUE
                    );


                    return 0;
                }

            }

            break;
        }





        // ================= PLACE SHAPE =================

        case WM_LBUTTONDOWN:
        {

            if(placingShape)
            {

                Shape newShape;


                newShape.x =
                    LOWORD(lParam);


                newShape.y =
                    HIWORD(lParam);



                newShape.scale =
                    0.5;



                newShape.color =
                    RGB(
                        0,
                        0,
                        255
                    );



                newShape.type =
                    placingType;



                shapes.push_back(
                    newShape
                );



                placingShape = false;



                InvalidateRect(
                    hwnd,
                    NULL,
                    FALSE
                );

            }


            return 0;
        }






        // ================= COLOR =================

        case WM_RBUTTONDOWN:
        {

            int mouseX =
                LOWORD(lParam);


            int mouseY =
                HIWORD(lParam);



            for(auto& shape : shapes)
            {

                if(
                    mouseX >= shape.x &&
                    mouseX <= shape.x + 100 * shape.scale &&
                    mouseY >= shape.y &&
                    mouseY <= shape.y + 100 * shape.scale
                )
                {


                    CHOOSECOLOR cc = {};

                    COLORREF custom[16] = {};



                    cc.lStructSize =
                        sizeof(CHOOSECOLOR);


                    cc.hwndOwner =
                        hwnd;


                    cc.lpCustColors =
                        custom;


                    cc.rgbResult =
                        shape.color;


                    cc.Flags =
                        CC_RGBINIT;



                    if(ChooseColor(&cc))
                    {
                        shape.color =
                            cc.rgbResult;
                    }



                    InvalidateRect(
                        hwnd,
                        NULL,
                        FALSE
                    );


                    break;
                }

            }


            return 0;
        }







        // ================= KEYS =================

        case WM_KEYDOWN:
        {

            if(shapes.empty())
                break;



            Shape& shape =
                shapes.back();



            switch(wParam)
            {


                case VK_LEFT:
                    shape.x -= 10;
                    break;



                case VK_RIGHT:
                    shape.x += 10;
                    break;



                case VK_UP:
                    shape.y -= 10;
                    break;



                case VK_DOWN:
                    shape.y += 10;
                    break;



                case VK_ADD:

                case VK_OEM_PLUS:

                    growing = true;
                    break;



                case VK_SUBTRACT:

                case VK_OEM_MINUS:

                    shrinking = true;
                    break;

            }



            InvalidateRect(
                hwnd,
                NULL,
                FALSE
            );


            return 0;
        }






        case WM_KEYUP:
        {

            switch(wParam)
            {

                case VK_ADD:

                case VK_OEM_PLUS:

                    growing = false;
                    break;



                case VK_SUBTRACT:

                case VK_OEM_MINUS:

                    shrinking = false;
                    break;

            }


            return 0;
        }






        // ================= RESIZE TIMER =================

        case WM_TIMER:
        {

            if(!shapes.empty())
            {

                Shape& shape =
                    shapes.back();



                if(growing)
                    shape.scale += 0.02;



                if(shrinking)
                    shape.scale -= 0.02;



                if(shape.scale > 3.0)
                    shape.scale = 3.0;



                if(shape.scale < 0.1)
                    shape.scale = 0.1;



                InvalidateRect(
                    hwnd,
                    NULL,
                    FALSE
                );

            }


            return 0;
        }





        case WM_DESTROY:
        {

            KillTimer(
                hwnd,
                1
            );


            PostQuitMessage(0);


            return 0;
        }

    }



    return DefWindowProc(
        hwnd,
        uMsg,
        wParam,
        lParam
    );
}








int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE,
    LPSTR,
    int nCmdShow)
{


    const char CLASS_NAME[] =
        "ShapeEditor";



    WNDCLASS wc = {};



    wc.lpfnWndProc =
        WindowProc;


    wc.hInstance =
        hInstance;


    wc.lpszClassName =
        CLASS_NAME;


    wc.hbrBackground =
        (HBRUSH)(COLOR_WINDOW + 1);



    RegisterClass(&wc);






    HWND hwnd =
        CreateWindowEx(
            0,
            CLASS_NAME,
            "Circle and Square Editor",
            WS_OVERLAPPEDWINDOW,

            CW_USEDEFAULT,
            CW_USEDEFAULT,
            800,
            600,

            NULL,
            NULL,
            hInstance,
            NULL
        );



    if(hwnd == NULL)
        return 0;





    // ================= MENU =================


    HMENU hMenu =
        CreateMenu();



    HMENU hShapeMenu =
        CreatePopupMenu();



    AppendMenu(
        hShapeMenu,
        MF_STRING,
        ID_ADD_CIRCLE,
        "Add Circle"
    );



    AppendMenu(
        hShapeMenu,
        MF_STRING,
        ID_ADD_SQUARE,
        "Add Square"
    );



    AppendMenu(
        hShapeMenu,
        MF_STRING,
        ID_REMOVE_ALL,
        "Remove All Shapes"
    );



    AppendMenu(
        hMenu,
        MF_POPUP,
        (UINT_PTR)hShapeMenu,
        "Shapes"
    );



    SetMenu(
        hwnd,
        hMenu
    );





    ShowWindow(
        hwnd,
        nCmdShow
    );



    SetTimer(
        hwnd,
        1,
        16,
        NULL
    );





    MSG msg = {};



    while(GetMessage(
        &msg,
        NULL,
        0,
        0))
    {

        TranslateMessage(&msg);

        DispatchMessage(&msg);

    }

    return 0;
}
