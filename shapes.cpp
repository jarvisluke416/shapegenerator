#include <windows.h>
#include <vector>
#include <cstring>
#include <commdlg.h>
#include <cmath>


#define ID_ADD_CIRCLE 1001
#define ID_REMOVE_ALL 1002
#define ID_ADD_SQUARE 1003
#define ID_ADD_TRIANGLE 1004
#define ID_ADD_HEART 1005
#define ID_ADD_STAR 1006


enum ShapeType
{
    CIRCLE,
    SQUARE,
    TRIANGLE,
    HEART,
    STAR
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



void DrawShape(HDC hdc, Shape& shape)
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


    int size =
        (int)(100 * shape.scale);



    // ================= CIRCLE =================

    if(shape.type == CIRCLE)
    {
        Ellipse(
            hdc,
            shape.x,
            shape.y,
            shape.x + size,
            shape.y + size
        );
    }



    // ================= SQUARE =================

    if(shape.type == SQUARE)
    {
        Rectangle(
            hdc,
            shape.x,
            shape.y,
            shape.x + size,
            shape.y + size
        );
    }



    // ================= TRIANGLE =================

    if(shape.type == TRIANGLE)
    {
        POINT points[3];


        points[0].x =
            shape.x + size / 2;

        points[0].y =
            shape.y;



        points[1].x =
            shape.x;

        points[1].y =
            shape.y + size;



        points[2].x =
            shape.x + size;

        points[2].y =
            shape.y + size;



        Polygon(
            hdc,
            points,
            3
        );
    }





    // ================= HEART =================

    if(shape.type == HEART)
    {
        int x = shape.x;
        int y = shape.y;
        int s = size;


        POINT heart[13];


        heart[0] = {
            x + s / 2,
            y + s / 4
        };


        heart[1] = {
            x + s / 3,
            y - s / 8
        };


        heart[2] = {
            x,
            y
        };


        heart[3] = {
            x,
            y + s / 3
        };


        heart[4] = {
            x,
            y + s / 2
        };


        heart[5] = {
            x + s / 4,
            y + s * 3 / 4
        };


        heart[6] = {
            x + s / 2,
            y + s
        };


        heart[7] = {
            x + s * 3 / 4,
            y + s * 3 / 4
        };


        heart[8] = {
            x + s,
            y + s / 2
        };


        heart[9] = {
            x + s,
            y + s / 3
        };


        heart[10] = {
            x + s,
            y
        };


        heart[11] = {
            x + s * 2 / 3,
            y - s / 8
        };


        heart[12] = heart[0];


        MoveToEx(
            hdc,
            heart[0].x,
            heart[0].y,
            NULL
        );


        PolyBezier(
            hdc,
            heart,
            13
        );
    }






    // ================= STAR =================

    if(shape.type == STAR)
    {
        POINT points[10];


        int centerX =
            shape.x + size / 2;


        int centerY =
            shape.y + size / 2;



        double outerRadius =
            size / 2.0;


        double innerRadius =
            size / 5.0;



        double angle =
            -3.14159265358979323846 / 2;



        for(int i = 0; i < 10; i++)
        {
            double radius;


            if(i % 2 == 0)
                radius = outerRadius;
            else
                radius = innerRadius;



            points[i].x =
                centerX +
                (int)(cos(angle) * radius);



            points[i].y =
                centerY +
                (int)(sin(angle) * radius);



            angle +=
                3.14159265358979323846 / 5;
        }



        Polygon(
            hdc,
            points,
            10
        );
    }



    SelectObject(
        hdc,
        oldPen
    );


    DeleteObject(pen);
}






LRESULT CALLBACK WindowProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch(uMsg)
    {


        case WM_PAINT:
        {
            PAINTSTRUCT ps;


            HDC hdc =
                BeginPaint(
                    hwnd,
                    &ps
                );



            for(size_t i = 0; i < shapes.size(); i++)
            {
                DrawShape(
                    hdc,
                    shapes[i]
                );
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



                case ID_ADD_TRIANGLE:
                {
                    placingType = TRIANGLE;
                    placingShape = true;

                    InvalidateRect(
                        hwnd,
                        NULL,
                        FALSE
                    );

                    return 0;
                }



                case ID_ADD_HEART:
                {
                    placingType = HEART;
                    placingShape = true;

                    InvalidateRect(
                        hwnd,
                        NULL,
                        FALSE
                    );

                    return 0;
                }



                case ID_ADD_STAR:
                {
                    placingType = STAR;
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







        case WM_RBUTTONDOWN:
        {
            int mouseX =
                LOWORD(lParam);


            int mouseY =
                HIWORD(lParam);



            for(int i = (int)shapes.size() - 1; i >= 0; i--)
            {
                Shape& shape =
                    shapes[i];


                int size =
                    (int)(100 * shape.scale);



                if(
                    mouseX >= shape.x &&
                    mouseX <= shape.x + size &&
                    mouseY >= shape.y &&
                    mouseY <= shape.y + size
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
            "Circle Square Triangle Heart Star Editor",
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
        ID_ADD_TRIANGLE,
        "Add Triangle"
    );


    AppendMenu(
        hShapeMenu,
        MF_STRING,
        ID_ADD_HEART,
        "Add Heart"
    );


    AppendMenu(
        hShapeMenu,
        MF_STRING,
        ID_ADD_STAR,
        "Add Star"
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
