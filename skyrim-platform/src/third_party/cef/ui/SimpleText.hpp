//#pragma comment(lib, "d2d1")
//#pragma comment(lib, "dwrite")

#include "DX11RenderHandler.hpp"
#include <D3D11Hook.hpp>
#include <d2d1.h>
#include <d3d11.h>
#include <dwrite.h>

namespace CEFUtils {

class SimpleText
{
public:
  SimpleText(const wchar_t* text, DX11RenderHandler::Renderer* apRenderer);

  void Draw();

  static HWND GetWindow(IDXGISwapChain* swapChain);

  void DiscardDeviceResources();

  void Render();
  void Create();
  void Reset();

private:
  IDWriteFactory* pDWriteFactory_;
  IDWriteTextFormat* pTextFormat_;

  const wchar_t* wszText_;
  UINT32 cTextLength_;

  ID2D1Factory* pD2DFactory_;
  ID2D1HwndRenderTarget* pRT_;
  ID2D1SolidColorBrush* pBlackBrush_;

  HWND hwnd_;

  HRESULT CreateDeviceIndependentResources();
  HRESULT CreateDeviceResources();

  RECT rc_;

  template <class T>
  void SafeRelease(T** ppT);

  float dpiScaleX_ = 72;
  float dpiScaleY_ = 72;

  DX11RenderHandler::Renderer* m_pRenderer;
};
}
