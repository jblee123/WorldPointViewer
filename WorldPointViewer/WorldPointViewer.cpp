#include <windows.h>
#include <windowsx.h>

#include <gdiplus.h>

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/Wglew.h>

#include "Matrix4Df.h"
#include "Vector.h"
#include "Vec3Df.h"
#include "Projection.h"

#include "GLPrograms.h"

#include "Globe.h"
#include "LineSegs.h"

#include "Camera.h"
#include "Utils.h"

struct Mouse {
    int last_x, last_y;
    bool l_btn_down, r_btn_down;

    Mouse() :
        last_x(-1),
        last_y(-1),
        l_btn_down(false),
        r_btn_down(false) {
    }
} g_mouse;

// Windows globals, defines, and prototypes
WCHAR szAppName[] = L"World Point Viewer";
HWND  ghWnd;
HDC   ghDC;
HGLRC ghRC;

const int GRID_WIDTH = 800;
const int GRID_HEIGHT = 800;

const int GRID_BUFFER = 100;
const int WIN_WIDTH = GRID_WIDTH + (6 * GRID_BUFFER);
const int WIN_HEIGHT = GRID_HEIGHT + (3 * GRID_BUFFER);

const float MOVE_AMOUNT = 5;
const float SPIN_AMOUNT = degToRad(5.0f);

LONG WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL setupPixelFormat(HDC);
void doCleanup(HWND);
void CALLBACK DrawTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

GLvoid resize(int width, int height);
void redoModelViewMatrix();
void redoProjectionMatrix(int width, int height);
void moveCameraByMouseMove(int x, int y, int last_x, int last_y);
void zoomCameraByMouseWheel(int delta);
void initializeGL();
GLvoid drawScene(int width, int height);
void createSwarm(int width, int height);
void setupData(int width, int height);

const int SWARM_SIZE = 500;

GLPrograms g_programs;

Globe g_globe(1);
LineSegs g_equator(Color{ 128, 128, 0, 255 });
LineSegs g_prime_meridian(Color{ 128, 128, 0, 255 });
std::vector<LineSegs> g_actual_points;
std::vector<LineSegs> g_approx_points;
std::vector<LineSegs> g_approx_offset_points;
std::vector<LineSegs> g_axis_points;

const UINT_PTR DRAW_TIMER_ID = 1;

Camera g_camera(
    vec3df::create(
        (float)(EARTH_EQUITORIAL_RADIUS * 1.5),
        0.0f,
        0.0f),
    degToRad(0), 0);

mat4df::Mat4Df g_modelView;
mat4df::Mat4Df g_projection;

//FrameRateCounter g_frameRateCounter(5000);

bool g_shiftPressed = false;
bool g_paused = false;

const bool CREATE_CONSOLE = true;

ULONG_PTR g_gdiplusToken;

int WINAPI WinMain(
    _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {

    MSG msg;
    WNDCLASS wndclass;

    // Register the frame class
    wndclass.style = 0;
    wndclass.lpfnWndProc = (WNDPROC)MainWndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, szAppName);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndclass.lpszMenuName = szAppName;
    wndclass.lpszClassName = szAppName;

    if (!::RegisterClass(&wndclass)) {
        return FALSE;
    }

    if (CREATE_CONSOLE) {
        ::AllocConsole();
        freopen("conin$", "r", stdin);
        freopen("conout$", "w", stdout);
        freopen("conout$", "w", stderr);
    }

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, nullptr);

    // Create the frame
    ghWnd = ::CreateWindow(szAppName,
        szAppName,
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WIN_WIDTH,
        WIN_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL);

    // make sure window was created
    if (!ghWnd) {
        return FALSE;
    }

    // show and update main window
    ::ShowWindow(ghWnd, nCmdShow);

    ::UpdateWindow(ghWnd);

    while (::GetMessage(&msg, NULL, 0, 0)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }
    return TRUE;
}

DWORD g_lastTime = 0;
void CALLBACK DrawTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    if (g_paused) {
        return;
    }

    ::KillTimer(hwnd, idEvent);

    DWORD currentTime = timeGetTime();

    const float TARGET_FPS = 60;
    const UINT TIME_PER_FRAME = (UINT)(1000 / TARGET_FPS);

    DWORD nextTime = (g_lastTime > 0) ?
        (g_lastTime + TIME_PER_FRAME) : (currentTime + TIME_PER_FRAME);
    DWORD delay = (nextTime >= currentTime) ? (nextTime - currentTime) : 0;
    ::SetTimer(hwnd, DRAW_TIMER_ID, delay, DrawTimerProc);

    //if (g_lastTime > 0) {
    //    printf("time: %u\n", currentTime - g_lastTime);
    //}
    g_lastTime = currentTime;

    RECT rect;
    ::GetWindowRect(hwnd, &rect);
    drawScene(rect.right - rect.left, rect.bottom - rect.top);
}

// main window procedure
LONG WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LONG lRet = 1;
    PAINTSTRUCT ps;
    RECT rect;

    switch (uMsg) {

    case WM_CREATE:
        ghDC = GetDC(hWnd);
        if (!setupPixelFormat(ghDC)) {
            PostQuitMessage(0);
        }

        initializeGL();
        GetClientRect(hWnd, &rect);
        resize(rect.right, rect.bottom); // added in leiu of passing dims to initialize
        g_programs.compilePrograms();

        setupData(rect.right, rect.bottom);

        ::SetTimer(hWnd, DRAW_TIMER_ID, 0, DrawTimerProc);

        break;

    case WM_PAINT:
        BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_SIZE:
        GetClientRect(hWnd, &rect);
        resize(rect.right, rect.bottom);
        break;

    case WM_CLOSE:
        doCleanup(hWnd);
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        doCleanup(hWnd);
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        switch (wParam) {
        case 'P':
            g_paused = !g_paused;
            break;

        //case 'W':
        //    if (g_shiftPressed) {
        //        g_camera.moveUp(MOVE_AMOUNT);
        //    }
        //    else {
        //        g_camera.moveForward(MOVE_AMOUNT);
        //    }
        //    redoModelViewMatrix();
        //    break;

        //case 'A':
        //    g_camera.moveSide(-MOVE_AMOUNT);
        //    redoModelViewMatrix();
        //    break;

        //case 'S':
        //    if (g_shiftPressed) {
        //        g_camera.moveUp(-MOVE_AMOUNT);
        //    }
        //    else {
        //        g_camera.moveForward(-MOVE_AMOUNT);
        //    }
        //    redoModelViewMatrix();
        //    break;

        //case 'D':
        //    g_camera.moveSide(MOVE_AMOUNT);
        //    redoModelViewMatrix();
        //    break;

        //case 'Q':
        //    g_camera.spinAroundFwd(SPIN_AMOUNT);
        //    redoModelViewMatrix();
        //    break;

        //case 'E':
        //    g_camera.spinAroundFwd(-SPIN_AMOUNT);
        //    redoModelViewMatrix();
        //    break;

        //case VK_UP:
        //    g_camera.spinAroundSide(-SPIN_AMOUNT);
        //    redoModelViewMatrix();
        //    break;

        //case VK_DOWN:
        //    g_camera.spinAroundSide(SPIN_AMOUNT);
        //    redoModelViewMatrix();
        //    break;

        //case VK_LEFT:
        //    //if (g_shiftPressed) {
        //    //    g_camera.spinAroundFwd(SPIN_AMOUNT);
        //    //}
        //    //else {
        //    g_camera.spinAroundUp(SPIN_AMOUNT);
        //    //}
        //    redoModelViewMatrix();
        //    break;

        //case VK_RIGHT:
        //    //if (g_shiftPressed) {
        //    //    g_camera.spinAroundFwd(-SPIN_AMOUNT);
        //    //}
        //    //else {
        //    g_camera.spinAroundUp(-SPIN_AMOUNT);
        //    //}
        //    redoModelViewMatrix();
        //    break;

        case VK_SHIFT:
            g_shiftPressed = true;
            break;
        }
        break;

    case WM_KEYUP:
        switch (wParam) {
        case VK_SHIFT:
            g_shiftPressed = false;
            break;
        }
        break;

    case WM_LBUTTONDOWN:
        g_mouse.l_btn_down = true;
        break;

    case WM_LBUTTONUP:
        g_mouse.l_btn_down = false;
        break;

    case WM_MOUSEMOVE: {
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);
        if (g_mouse.l_btn_down) {
            moveCameraByMouseMove(x, y, g_mouse.last_x, g_mouse.last_y);
        }
        g_mouse.last_x = x;
        g_mouse.last_y = y;
        break;
    }

    case WM_MOUSEWHEEL: {
        int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        zoomCameraByMouseWheel(delta);
        break;
    }

    default:
        lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
        break;
    }

    return lRet;
}

void doCleanup(HWND hWnd) {

    if (CREATE_CONSOLE) {
        ::FreeConsole();
    }

    g_globe.cleanup();

    g_equator.cleanup();
    g_prime_meridian.cleanup();

    for (auto& s : g_actual_points) {
        s.cleanup();
    }
    for (auto& s : g_approx_points) {
        s.cleanup();
    }
    for (auto& s : g_approx_offset_points) {
        s.cleanup();
    }
    for (auto& s : g_axis_points) {
        s.cleanup();
    }

    g_programs.cleanupPrograms();

    Gdiplus::GdiplusShutdown(g_gdiplusToken);

    if (ghRC) {
        wglDeleteContext(ghRC);
        ghRC = 0;
    }
    if (ghDC) {
        ReleaseDC(hWnd, ghDC);
        ghDC = 0;
    }
}

BOOL setupPixelFormat(HDC hdc) {
    PIXELFORMATDESCRIPTOR pfd;
    int pixelformat;

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.dwLayerMask = PFD_MAIN_PLANE;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 0;// 16;
    pfd.cAccumBits = 0;
    pfd.cStencilBits = 0;

    pixelformat = ChoosePixelFormat(hdc, &pfd);

    if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == 0) {
        MessageBox(NULL, L"ChoosePixelFormat failed", L"Error", MB_OK);
        return FALSE;
    }

    if (!SetPixelFormat(hdc, pixelformat, &pfd)) {
        MessageBox(NULL, L"SetPixelFormat failed", L"Error", MB_OK);
        return FALSE;
    }

    return TRUE;
}

// OpenGL code

GLvoid resize(int width, int height) {
    glViewport(0, 0, width, height);
    redoModelViewMatrix();
    redoProjectionMatrix(width, height);
}

void redoModelViewMatrix() {
    g_modelView = projection::createLookAt(
        g_camera.getPosition(), g_camera.getTarget(), g_camera.getUp());
}

void redoProjectionMatrix(int width, int height) {
    const float NEAR_DIST = 0.5f;
    const float FAR_DIST = (float)(EARTH_EQUITORIAL_RADIUS * 3.0);
    const float FOV = 70;

    g_projection = projection::createPerspective(FOV, (float)width, (float)height, NEAR_DIST, FAR_DIST);
}

void moveCameraByMouseMove(int x, int y, int last_x, int last_y) {
    int dx = x - last_x;
    int dy = y - last_y;

    float PIX_TO_DEG = 0.04f;
    auto pos = g_camera.getPosition();

    auto alt = pos.length() - EARTH_EQUITORIAL_RADIUS;
    auto scale_factor = 1.0;
    //auto scale_factor = alt / EARTH_EQUITORIAL_RADIUS;

    if (alt < 1000000) {
        PIX_TO_DEG = 0.001f;
    }

    pos = vec3df::rotateZ(pos, -degToRad(dx * PIX_TO_DEG) * scale_factor);

    float dx_for_atan = vec3df::create(pos(0), pos(1), 0).length();
    float dy_for_atan = pos(2);
    float angle = atan2(dy_for_atan, dx_for_atan);

    const float MAX_ANGLE = degToRad(89);

    float delta_angle = degToRad(dy * PIX_TO_DEG);
    if (angle + delta_angle > MAX_ANGLE) {
        delta_angle = MAX_ANGLE - angle;
    }
    if (angle + delta_angle < -MAX_ANGLE) {
        delta_angle = -MAX_ANGLE - angle;
    }
    auto axis = vec3df::cross(pos, vec3df::create(0, 0, 1));
    auto rot = mat4df::createRotationAbout(axis, delta_angle * scale_factor * 0.5);
    pos = mat4df::mul(rot, pos);

    g_camera.setPosition(pos);
}

void zoomCameraByMouseWheel(int delta) {
    const float ZOOM_FACTOR = 1.1f;
    auto pos = g_camera.getPosition();
    auto len = pos.length();

    //if (delta < 0) {
    //    pos *= ZOOM_FACTOR;
    //}
    //else if (delta > 0) {
    //    pos *= 1.0f / ZOOM_FACTOR;
    //}

    auto alt = len - EARTH_EQUITORIAL_RADIUS;
    if (delta < 0) {
        alt *= ZOOM_FACTOR;
        if (alt < 1) {
            alt = 1;
        }
    }
    else if (delta > 0) {
        alt *= 1.0f / ZOOM_FACTOR;
    }
    pos = pos.getUnit() * (EARTH_EQUITORIAL_RADIUS + alt);


    g_camera.setPosition(pos);
}

void initializeGL() {
    HGLRC tempContext = wglCreateContext(ghDC);
    wglMakeCurrent(ghDC, tempContext);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        // failed to initialize GLEW!
    }

    // My card currently only supports OpenGL 4.1 -- jbl
    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 1,
        WGL_CONTEXT_FLAGS_ARB, 0,
        0
    };

    if (wglewIsSupported("WGL_ARB_create_context") == 1) {
        ghRC = wglCreateContextAttribsARB(ghDC, 0, attribs);
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(tempContext);
        wglMakeCurrent(ghDC, ghRC);
    }
    else {
        // It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
        ghRC = tempContext;
    }

    int OpenGLVersion[2];
    glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
}

void setupData(int width, int height) {

    g_globe.setProgram(g_programs.getSimpleProg());
    g_globe.setup();

    const float RAD = (float)EARTH_EQUITORIAL_RADIUS;

    auto getPointFor = [&](int lat, int lon) {
        auto p = vec3df::create(1, 0, 0);
        float lat_f = degToRad((float)lat);
        float lon_f = degToRad((float)lon);

        p = vec3df::rotateY(p, lat_f);
        p = vec3df::rotateZ(p, lon_f);
        p *= RAD;

        return p;
    };

    std::vector<vec3df::Vec3Df> points;
    for (int lon = -180; lon <= 180; lon++) {
        auto p = getPointFor(0.0f, lon);
        points.push_back(p);
    }
    g_equator.setProgram(g_programs.getSimpleProg());
    g_equator.setup(points);
    points.clear();

    for (int lat = -90; lat <= 90; lat++) {
        auto p = getPointFor(lat, 0.0f);
        points.push_back(p);
    }
    g_prime_meridian.setProgram(g_programs.getSimpleProg());
    g_prime_meridian.setup(points);
    points.clear();

    auto tryAddPoints = [&](
        std::vector<LineSegs>& segs_list, Color color, std::vector<vec3df::Vec3Df>& points) {

        if (points.empty()) {
            return;
        }

        segs_list.push_back(LineSegs(color));
        LineSegs& segs = segs_list.back();
        segs.setProgram(g_programs.getSimpleProg());
        segs.setup(points);
        points.clear();
    };

    auto processFile = [&](
        const char* filename, std::vector<LineSegs>& segs_list, Color color) {

        std::vector<vec3df::Vec3Df> points;

        std::ifstream actual_points(filename);
        std::string str;
        while (std::getline(actual_points, str))
        {
            if (str.empty()) {
                tryAddPoints(segs_list, color, points);
            }
            else {
                size_t comma1 = str.find(",", 0);
                size_t comma2 = str.find(",", comma1 + 1);
                std::string str1 = str.substr(0, comma1);
                std::string str2 = str.substr(comma1 + 1, comma2 - comma1 - 1);
                std::string str3 = str.substr(comma2 + 1);

                vec3df::Vec3Df p = vec3df::create(
                    (float)atof(str1.c_str()), (float)atof(str2.c_str()), (float)atof(str3.c_str()));
                points.push_back(p);
            }
        }
        tryAddPoints(segs_list, color, points);
    };

    const Color ACTUAL_POINTS_COLOR = Color{ 255, 0, 0, 255 };
    const Color APPROX_POINTS_COLOR = Color{ 0, 255, 0, 255 };
    const Color APPROX_OFFSET_POINTS_COLOR = Color{ 0, 0, 255, 255 };
    const Color AXIS_POINTS_COLOR = Color{ 255, 255, 0, 255 };

    processFile("actual_points.txt", g_actual_points, ACTUAL_POINTS_COLOR);
    processFile("approx_points.txt", g_approx_points, APPROX_POINTS_COLOR);
    processFile("approx_offset_points.txt", g_approx_offset_points, APPROX_OFFSET_POINTS_COLOR);
    processFile("axis_points.txt", g_axis_points, AXIS_POINTS_COLOR);
}

unsigned int g_lastFrameRatePrintTime = 0;
void drawScene(int width, int height) {
    redoModelViewMatrix();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(true);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    g_globe.draw(g_modelView, g_projection);

    g_equator.draw(g_modelView, g_projection);
    g_prime_meridian.draw(g_modelView, g_projection);

    for (auto& s : g_actual_points) {
        s.draw(g_modelView, g_projection);
    }
    for (auto& s : g_approx_points) {
        s.draw(g_modelView, g_projection);
    }
    for (auto& s : g_approx_offset_points) {
        s.draw(g_modelView, g_projection);
    }
    for (auto& s : g_axis_points) {
        s.draw(g_modelView, g_projection);
    }

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    //DWORD currentTime = timeGetTime();
    //float frameRate = g_frameRateCounter.incorportateTime(currentTime);
    //if ((currentTime - g_lastFrameRatePrintTime) > 1000) {
    //    printf("fps: %f\n", frameRate);
    //    g_lastFrameRatePrintTime = currentTime;
    //}

    ::SwapBuffers(ghDC);
}
