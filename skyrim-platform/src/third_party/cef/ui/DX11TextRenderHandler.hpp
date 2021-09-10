#pragma once

#include "DX11RenderHandler.hpp"
#include "MyRenderHandler.hpp"
#include "SimpleText.hpp"
#include <Signal.hpp>
#include <mutex>
#include <wrl.h>

namespace DirectX {
class SpriteBatch;
class CommonStates;
}

struct IDXGISwapChain;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11DeviceContext;
struct ID3D11Device;

namespace CEFUtils {
struct DX11TextRenderHandler : MyRenderHandler
{
  static bool& Visible()
  {
    static bool g_visible = true;
    return g_visible;
  }

  explicit DX11TextRenderHandler(
    DX11RenderHandler::Renderer* apRenderer) noexcept;
  virtual ~DX11TextRenderHandler();

  TP_NOCOPYMOVE(DX11TextRenderHandler);

  void Create() override;
  void Render() override;
  void Reset() override;

  void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
  void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type,
               const RectList& dirtyRects, const void* buffer, int width,
               int height) override;

  IMPLEMENT_REFCOUNTING(DX11TextRenderHandler);

protected:
  void GetRenderTargetSize();
  void CreateRenderTexture();

private:
  uint32_t m_width{ 0 };
  uint32_t m_height{ 0 };

  Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pTexture;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pCursorTexture;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureView;
  std::mutex m_textureLock;
  std::mutex m_createLock;
  bool isCreateLock = false;
  DX11RenderHandler::Renderer* m_pRenderer;

  Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pImmediateContext;

  std::unique_ptr<DirectX::SpriteBatch> m_pSpriteBatch;
  std::unique_ptr<DirectX::CommonStates> m_pStates;

  std::unique_ptr<SimpleText> _text;
};
}
