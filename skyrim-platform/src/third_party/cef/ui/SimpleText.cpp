#include "SimpleText.hpp"
#include <D3D11Hook.hpp>
#include <d2d1.h>
#include <d3d11.h>
#include <dwrite.h>

namespace CEFUtils {

//#include "MyRenderHandler.hpp"
HRESULT hr;

SimpleText::SimpleText(const wchar_t* text,
                       DX11RenderHandler::Renderer* apRenderer)
  : m_pRenderer(apRenderer)
{
  // hwnd_ = window;
  // hwnd_ = GetWindow(swapChain);
  wszText_ = text;
  cTextLength_ = (UINT32)wcslen(wszText_);
  // CreateDeviceIndependentResources();
  // CreateDeviceResources();
  Create();
}

void SimpleText::Draw()
{
  /* D2D1_RECT_F layoutRect =
     D2D1::RectF(static_cast<FLOAT>(rc_.left) / dpiScaleX_,
                 static_cast<FLOAT>(rc_.top) / dpiScaleY_,
                 static_cast<FLOAT>(rc_.right - rc_.left) / dpiScaleX_,
                 static_cast<FLOAT>(rc_.bottom - rc_.top) / dpiScaleY_);*/

  pRT_->DrawText(wszText_,      // The string to render.
                 cTextLength_,  // The string's length.
                 pTextFormat_,  // The text format.
                 D2D1::RectF(), // layoutRect,   // The region of the window
                                // where the text will be rendered.
                 pBlackBrush_ // The brush used to draw the text.
  );
}

void SimpleText::Render()
{
  if (!pRT_)
    Create();

  pRT_->BeginDraw();

  pRT_->SetTransform(D2D1::IdentityMatrix());

  pRT_->Clear(D2D1::ColorF(D2D1::ColorF::White));

  // Call the DrawText method of this class.
  Draw();

  hr = pRT_->EndDraw();

  if (FAILED(hr)) {
    DiscardDeviceResources();
  }
}

void SimpleText::Create()
{
  hwnd_ = GetWindow(m_pRenderer->GetSwapChain());
  if (FAILED(CreateDeviceIndependentResources()))
    return;
  if (FAILED(CreateDeviceResources()))
    return;
}

void SimpleText::Reset()
{
  DiscardDeviceResources();
  Create();
}

HWND SimpleText::GetWindow(IDXGISwapChain* swapChain)
{
  DXGI_SWAP_CHAIN_DESC desc{};
  desc.OutputWindow = nullptr;
  if (swapChain)
    swapChain->GetDesc(&desc);

  return desc.OutputWindow;
}

void SimpleText::DiscardDeviceResources()
{
  SafeRelease(&pRT_);
  SafeRelease(&pBlackBrush_);
}

HRESULT SimpleText::CreateDeviceIndependentResources()
{
  // creating factory
  hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory_);

  // creating interface
  if (SUCCEEDED(hr)) {
    hr =
      DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
                          reinterpret_cast<IUnknown**>(&pDWriteFactory_));
  }

  // creating text format
  if (SUCCEEDED(hr)) {
    hr = pDWriteFactory_->CreateTextFormat(
      L"Gabriola", // Font family name.
      NULL,        // Font collection (NULL sets it to use the system font
                   // collection).
      DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL,
      DWRITE_FONT_STRETCH_NORMAL, 72.0f, L"en-us", &pTextFormat_);
  }

  // Center align (horizontally) the text.
  if (SUCCEEDED(hr)) {
    hr = pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
  }

  if (SUCCEEDED(hr)) {
    hr =
      pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
  }

  return hr;
}

HRESULT SimpleText::CreateDeviceResources()
{
  RECT rc = { 0, 0, 1080, 1920 };
  // GetClientRect(hwnd_, &rc);

  D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

  if (!pRT_) {
    // Create a Direct2D render target.
    hr = pD2DFactory_->CreateHwndRenderTarget(
      D2D1::RenderTargetProperties(),
      D2D1::HwndRenderTargetProperties(hwnd_, size), &pRT_);

    // Create a black brush.
    if (SUCCEEDED(hr)) {
      hr = pRT_->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black),
                                       &pBlackBrush_);
    }
  }
  rc_ = rc;
  return hr;
}

template <class T>
void SimpleText::SafeRelease(T** ppT)
{
  if (*ppT) {
    (*ppT)->Release();
    *ppT = NULL;
  }
}
}
