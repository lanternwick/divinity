Attribute VB_Name = "modDX"
Const PI = 3.14519265358979
Public DirectX As New DirectX8
Public D3D As Direct3D8
Public D3DDevice As Direct3DDevice8

Public ThreeDView As Long
Public D3DPresentParams As D3DPRESENT_PARAMETERS


Public Function InitDirectX(hWnd As Long, SetWindowed As Boolean) As Boolean
    
    'On Error Resume Next
    
    
    Set D3D = DirectX.Direct3DCreate
    If D3D Is Nothing Then
        InitDirectX = False
        Exit Function
    End If
       
    Dim Mode As D3DDISPLAYMODE
    'D3D.GetAdapterDisplayMode D3DADAPTER_DEFAULT, Mode
    Mode.Format = D3DFMT_X8R8G8B8
    Mode.Width = 640
    Mode.Height = 480
    ScreenSizeX = 640
    ScreenSizeY = 480
    
    If SetWindowed Then
        D3DPresentParams.Windowed = 1
    End If
    
    D3DPresentParams.SwapEffect = D3DSWAPEFFECT_COPY_VSYNC
    D3DPresentParams.BackBufferFormat = Mode.Format
    If Not SetWindowed Then
        D3DPresentParams.BackBufferHeight = 480
        D3DPresentParams.BackBufferWidth = 640
    End If
    D3DPresentParams.hDeviceWindow = hWnd
    D3DPresentParams.EnableAutoDepthStencil = 1
    D3DPresentParams.AutoDepthStencilFormat = D3DFMT_D16_LOCKABLE
    
    
    Set D3DDevice = D3D.CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, D3DPresentParams)
    If D3DDevice Is Nothing Then
        InitDirectX = False
        Exit Function
    End If
    
    
    
End Function

Public Sub Init()
    
End Sub

