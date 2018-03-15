//*********************************************************
//
// MIT License
// Copyright(c) 2018 Masafumi Takahashi / Shader.jp
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//*********************************************************

#pragma once

#include "DXSample.h"

using namespace DirectX;

// Note that while ComPtr is used to manage the lifetime of resources on the CPU,
// it has no understanding of the lifetime of resources on the GPU. Apps must account
// for the GPU lifetime of resources to avoid destroying objects that may still be
// referenced by the GPU.
// An example of this can be found in the class method: OnDestroy().
using Microsoft::WRL::ComPtr;

// Illustrate how to render to a target that supports HDR.
class D3D12HDRViewer : public DXSample
{
public:
	D3D12HDRViewer(UINT width, UINT height, std::wstring name);
	inline DXGI_FORMAT GetBackBufferFormat() { return m_swapChainFormats[m_currentSwapChainBitDepth]; }

    static const float HDRMetaDataPool[4][4];
	static const UINT FrameCount = 2;

protected:
	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnSizeChanged(UINT width, UINT height, bool minimized);
    virtual void OnWindowMoved(int xPos, int yPos);
	virtual void OnDestroy();
	virtual void OnKeyDown(UINT8 key);
    virtual void OnDisplayChanged();

private:
	static const float ClearColor[4];
	static const UINT TrianglesVertexCount = 18;

	// Vertex definitions.
	struct PresentVertex
	{
		XMFLOAT3 position;
		XMFLOAT2 uv;
	};

    struct DisplayChromacities
    {
        float RedX;
        float RedY;
        float GreenX;
        float GreenY;
        float BlueX;
        float BlueY;
        float WhiteX;
        float WhiteY;
    };

	enum PipelineStates
	{
		PalettePSO = 0,
		Present8bitPSO,
		Present10bitPSO,
		Present16bitPSO,
		PipelineStateCount
	};

	enum SwapChainBitDepth
	{
		_8 = 0,
		_10,
		_16,
		SwapChainBitDepthCount
	};

	enum RootConstants
	{
		ReferenceWhiteNits = 0,
		DisplayCurve,
		EVValue,
		HeatmapFlag,
		RootConstantsCount
	};

	enum DisplayCurve
	{
		sRGB = 0,	// The display expects an sRGB signal.
		ST2084,		// The display expects an HDR10 signal.
		None		// The display expects a linear signal.
	};

	enum TextureFromat
	{
		DDS = 0,
		OpenEXR,
		JXR,	// JPEG XR
		Unsupported
	};

	enum  DescriptorHeapOffset : uint32_t
	{
		RENDER_TARGET_OFFSET = 0,
		HDR_TEXTURE_HEAP_OFFSET,
		HEATMAP_HEAP_OFFSET,
		IMGUI_HEAP_OFFSET,
		HEAP_MAX,
	};


	// Pipeline objects.
	CD3DX12_VIEWPORT m_viewport;
	CD3DX12_RECT m_scissorRect;
	ComPtr<IDXGIFactory4> m_dxgiFactory;
	ComPtr<IDXGISwapChain4> m_swapChain;
	ComPtr<ID3D12Device> m_device;
	ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
	ComPtr<ID3D12Resource> m_intermediateRenderTarget;
	ComPtr<ID3D12CommandAllocator> m_commandAllocators[FrameCount];
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12RootSignature> m_rootSignature;
	ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	ComPtr<ID3D12DescriptorHeap> m_srvHeap;
	UINT m_rtvDescriptorSize;
	UINT m_srvDescriptorSize;
	DXGI_FORMAT m_swapChainFormats[SwapChainBitDepthCount];
	DXGI_COLOR_SPACE_TYPE m_currentSwapChainColorSpace;
	SwapChainBitDepth m_currentSwapChainBitDepth;
	bool m_swapChainFormatChanged;
	DXGI_FORMAT m_intermediateRenderTargetFormat;
	UINT m_dxgiFactoryFlags;

	// App resources.
	ComPtr<ID3D12PipelineState> m_pipelineStates[PipelineStateCount];
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	ComPtr<ID3D12Resource> m_vertexBuffer;
	ComPtr<ID3D12Resource> m_vertexBufferUpload;
	D3D12_VERTEX_BUFFER_VIEW m_presentVertexBufferView;
	UINT m_rootConstants[RootConstantsCount];
	float* m_rootConstantsF;
	bool m_enableEditWindow= true;
	bool m_enableDisplayInfo = true;
    UINT m_hdrMetaDataPoolIdx = 0;
	bool m_isHeatmap = false;
	bool m_openLoadDialog = false;
	
	// Color.
	bool m_hdrSupport = false;
	bool m_enableST2084 = false;
	float m_referenceWhiteNits = 80.0f;	// The reference brightness level of the display.

	// Synchronization objects.
	UINT m_frameIndex;
	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceValues[FrameCount];

	// Track the state of the window.
	// If it's minimized the app may decide not to render frames.
	bool m_windowVisible;
	bool m_windowedMode;
    bool m_in_sizechanging;

	//
	float m_evValue;
	bool m_isLoadTexture = false;
	std::wstring m_textureName;
	TextureFromat	m_format;

	void LoadPipeline();
	void LoadAssets();
	void LoadSizeDependentResources();
	XMFLOAT3 TransformVertex(XMFLOAT2 point, XMFLOAT2 offset);
	void RenderScene();
	void WaitForGpu();
	void MoveToNextFrame();
    void EnsureSwapChainColorSpace(SwapChainBitDepth d, bool enableST2084);
    void CheckDisplayHDRSupport();
    void SetHDRMetaData(float MaxOutputNits = 1000.0f, float MinOutputNits = 0.001f, float MaxCLL = 2000.0f, float MaxFALL = 500.0f);
    void UpdateSwapChainBuffer(UINT width, UINT height, DXGI_FORMAT format);

	//
	DXGI_OUTPUT_DESC1		m_outputdesc1;
	ComPtr<ID3D12Resource>	m_hdrTexture;
	ComPtr<ID3D12Resource>	m_heatmapTexture;

	void IMGuiUpdate();
	void OpenFile();
	HRESULT LoadTexture(std::wstring  filepath, const TextureFromat textureFormat, const uint32_t heapOffset);
};