VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "Comdlg32.ocx"
Begin VB.Form frmMdlView 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Model Viewer"
   ClientHeight    =   6555
   ClientLeft      =   150
   ClientTop       =   720
   ClientWidth     =   5535
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6555
   ScaleWidth      =   5535
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin MSComDlg.CommonDialog CD 
      Left            =   3240
      Top             =   3120
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Timer Timer1 
      Interval        =   100
      Left            =   2160
      Top             =   3000
   End
   Begin VB.PictureBox picViewport 
      Height          =   4695
      Left            =   120
      ScaleHeight     =   4635
      ScaleWidth      =   5235
      TabIndex        =   0
      Top             =   240
      Width           =   5295
   End
   Begin VB.Menu mnuFile 
      Caption         =   "File"
      Begin VB.Menu mnuImport 
         Caption         =   "Import"
         Begin VB.Menu mnuImportTOW 
            Caption         =   "TOW Model"
         End
         Begin VB.Menu mnuImportASE 
            Caption         =   "ASE"
         End
      End
      Begin VB.Menu mnuExport 
         Caption         =   "Export"
         Begin VB.Menu mnuExportTOW 
            Caption         =   "TOW Model"
         End
      End
   End
End
Attribute VB_Name = "frmMdlView"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private ASE As ParsedASE

Private LoadedModel As Long
Private ini As Boolean


Private Sub Form_Unload(Cancel As Integer)
    Set D3DDevice = Nothing
    ini = False
End Sub

Private Sub mnuExportTOW_Click()
    On Error GoTo errExit
    If LoadedModel = 0 Then Exit Sub
    CD.CancelError = True
    CD.Filter = "TOW Model Format|*.model"
    CD.ShowSave
    If LoadedModel = 1 Then
        SaveParsedASE CD.Filename, ASE
    End If
errExit:
End Sub

Private Sub mnuImportASE_Click()
    On Error GoTo errExit
    CD.CancelError = True
    CD.Filter = "ASCII Scene Export (3DS Max)|*.ase"
    CD.ShowOpen
    Dim d As String
    d = LoadTextFile(CD.Filename)
    d = RemoveTabs(d)
    d = RemoveSpaces(d)
    ParseASEFromString d, ASE
    LoadedModel = 1
errExit:
    
End Sub

Private Sub mnuImportTOW_Click()
    On Error GoTo errExit
    CD.CancelError = True
    CD.Filter = "TOW Model Format|*.model"
    CD.ShowOpen
    LoadParsedASE CD.Filename, ASE
    LoadedModel = 1
errExit:
End Sub

Private Sub Timer1_Timer()
    Static here As Boolean
    If Not here Then
        here = True
        Render
        here = False
    End If
End Sub

Private Sub Render()
    If Not ini Then
        Init
        ini = True
    End If
    Dim mat As D3DMATERIAL8
    SetMatrices
    D3DDevice.Clear ByVal 0, ByVal 0, D3DCLEAR_TARGET Or D3DCLEAR_ZBUFFER, 0, 1, 0
    
    D3DDevice.BeginScene
    
    D3DDevice.SetVertexShader D3DFVF_XYZ Or D3DFVF_NORMAL Or D3DFVF_TEX1
    
    D3DDevice.SetRenderState D3DRS_LIGHTING, True
    D3DDevice.SetRenderState D3DRS_AMBIENT, &HFFFFFF
    If LoadedModel = 1 Then
        LSet mat = ASE.Materials(0)
        D3DDevice.SetMaterial mat
        D3DDevice.DrawIndexedPrimitiveUP D3DPT_TRIANGLELIST, 0, ASE.numVertices, ASE.numFaces, ASE.Indices(0), D3DFMT_INDEX16, ASE.Vertices(0), Len(ASE.Vertices(0))
    End If
    
    D3DDevice.EndScene
    
    D3DDevice.Present ByVal 0, ByVal 0, 0, ByVal 0
End Sub

Private Sub Init()
    InitDirectX picViewport.hWnd, True
    LoadedModel = 0
End Sub

Private Sub SetMatrices()
    Dim p As D3DMATRIX
    Dim v As D3DMATRIX
    Dim a As D3DVECTOR
    Dim t As D3DVECTOR
    Dim u As D3DVECTOR
    u.x = 0
    u.z = 0
    u.y = 1
    a.x = -3
    a.y = 3
    a.z = -3
    t.x = 0
    t.y = 0
    t.z = 0
    
    Dim matProj As D3DMATRIX
    D3DXMatrixPerspectiveFovLH matProj, 3.14159 / 4, 1, 1, 1000
    D3DDevice.SetTransform D3DTS_PROJECTION, matProj
    
    'D3DXMatrixPerspectiveLH p, Picture1.ScaleWidth, Picture1.ScaleHeight, 0.1, 1000
    D3DXMatrixLookAtLH v, a, t, u
    'D3DDevice.SetTransform D3DTS_PROJECTION, p
    D3DDevice.SetTransform D3DTS_VIEW, v
    D3DXMatrixRotationY v, Timer
    D3DDevice.SetTransform D3DTS_WORLD, v
    
    Dim l As D3DLIGHT8
    l.Diffuse.a = 1#
    l.Diffuse.r = 1#
    l.Diffuse.g = 1#
    l.Diffuse.b = 1#

    l.Type = D3DLIGHT_POINT
    l.Attenuation0 = 1#
    l.Range = 4#

    l.Position.x = 1#
    l.Position.y = 1#
    l.Position.z = 0#

    l.Direction.x = 0#
    l.Direction.y = -1#
    l.Direction.z = 0#
    
    D3DDevice.SetLight 0, l
    D3DDevice.LightEnable 0, True
    
End Sub

