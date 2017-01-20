#include "Pch.h"
#include "SList.h"

void InitializeWindow(HINSTANCE instance, const std::wstring& className, const std::wstring windowTitle, int showCommand);
LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
void InitializeDirect3D();
void RenderFrame();
void CleanD3D();

// Handle to the window
HWND mWindowHandle;
// This struct holds information for the window class
WNDCLASSEX mWindow;

LONG mScreenWidth = 800;
LONG mScreenHeight = 600;

IDXGISwapChain *mSwapChain;
ID3D11Device *mDirect3DDevice;
ID3D11DeviceContext *mDirect3DDeviceContext;
ID3D11RenderTargetView *mBackBuffer;

DirectX::XMVECTORF32 BackgroundColor = { 0.392f, 0.584f, 0.929f, 1.0f };

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{	
	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(commandLine);

	std::wstring windowClassName = L"RenderingClass";

	InitializeWindow(instance, windowClassName, L"Test Window", showCommand);
	InitializeDirect3D();

	// This struct holds Windows event messages
	MSG msg;

	while (TRUE)
	{
		// Check to see if any messages are waiting in the queue
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Translate keystroke messages into the right format
			TranslateMessage(&msg);

			// Send the message to the WindowProc function
			DispatchMessage(&msg);

			// Check to see if it's time to quit
			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			//Game code will be run here
			RenderFrame();
		}
	}

	// Clean up DirectX and COM
	CleanD3D();

	// Return this part of the WM_QUIT message to Windows
	return static_cast<int>(msg.wParam);
}

void InitializeWindow(HINSTANCE instance, const std::wstring& className, const std::wstring windowTitle, int showCommand)
{
	// Clear out the window class for use
	ZeroMemory(&mWindow, sizeof(WNDCLASSEX));

	// Filling out the structure of the window class
	mWindow.cbSize = sizeof(WNDCLASSEX);
	mWindow.style = CS_HREDRAW | CS_VREDRAW;
	mWindow.lpfnWndProc = WindowProc;
	mWindow.hInstance = instance;
	mWindow.hCursor = LoadCursor(nullptr, IDC_ARROW);
	mWindow.hbrBackground = (HBRUSH)COLOR_WINDOW;
	mWindow.lpszClassName = className.c_str();

	// Register the window class
	RegisterClassEx(&mWindow);

	RECT windowRectangle = { 0, 0, mScreenWidth, mScreenHeight };	// Set the size, but not the position
	AdjustWindowRect(&windowRectangle, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the window and use the result as the handle
	mWindowHandle = CreateWindowEx(NULL,
		className.c_str(),									// Name of the window class
		windowTitle.c_str(),								// Title of the window
		WS_OVERLAPPEDWINDOW,								// Window style
		300,												// X-position of the window
		300,												// Y-position of the window
		windowRectangle.right - windowRectangle.left,		// Width of the window
		windowRectangle.bottom - windowRectangle.top,		// Height of the window
		nullptr,											// We have no parent window, nullptr
		nullptr,											// We aren't using menus, nullptr
		instance,											// Application handle
		nullptr);											// Used with multiple windows, nullptr

	// Display the window on the screen
	ShowWindow(mWindowHandle, showCommand);
}

void InitializeDirect3D()
{
	// Create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	// Clear out the struct for use
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	// Fill the swap chain struct
	swapChainDesc.BufferCount = 1;                                    // One back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // Use 32-bit color
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // How swap chain is to be used
	swapChainDesc.OutputWindow = mWindowHandle;                       // The window to be used
	swapChainDesc.SampleDesc.Count = 4;                               // How many multi samples for anti-aliasing
	swapChainDesc.Windowed = TRUE;                                    // Windowed/full-screen mode

	// Create a device, device context and the swap chain using the information in the swapChainDesc struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mSwapChain,
		&mDirect3DDevice,
		NULL,
		&mDirect3DDeviceContext);

	// Get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// Use the back buffer address to create the render target
	mDirect3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &mBackBuffer);
	pBackBuffer->Release();

	// Set the render target as the back buffer
	mDirect3DDeviceContext->OMSetRenderTargets(1, &mBackBuffer, NULL);

	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(mScreenWidth);
	viewport.Height = static_cast<float>(mScreenHeight);

	mDirect3DDeviceContext->RSSetViewports(1, &viewport);
}

// This is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// This message is read when the window is closed
	case WM_DESTROY:
	{
		// Close the application entirely
		PostQuitMessage(0);									//Sends the message WM_QUIT
		return 0;
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}

// This is the function that cleans up Direct3D and COM
void CleanD3D()
{
	// Close and release all existing COM objects
	mSwapChain->Release();
	mBackBuffer->Release();
	mDirect3DDevice->Release();
	mDirect3DDeviceContext->Release();
}

// This is the function used to render a single frame
void RenderFrame()
{
	// Clear the back buffer to a blue
	mDirect3DDeviceContext->ClearRenderTargetView(mBackBuffer, reinterpret_cast<const float*>(&BackgroundColor));

	// Switch the back buffer and the front buffer
	mSwapChain->Present(0, 0);
}