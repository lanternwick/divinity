VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frmMain 
   Caption         =   "Form1"
   ClientHeight    =   8745
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   11670
   LinkTopic       =   "Form1"
   ScaleHeight     =   8745
   ScaleWidth      =   11670
   StartUpPosition =   3  'Windows Default
   WindowState     =   2  'Maximized
   Begin MSComDlg.CommonDialog cd 
      Left            =   5640
      Top             =   4080
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.PictureBox Picture1 
      Height          =   13335
      Left            =   0
      ScaleHeight     =   885
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   1077
      TabIndex        =   0
      Top             =   0
      Width           =   16215
   End
   Begin VB.PictureBox Picture2 
      Height          =   2175
      Left            =   360
      ScaleHeight     =   141
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   173
      TabIndex        =   1
      Top             =   2760
      Width           =   2655
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Declare Function GetPixel Lib "gdi32" (ByVal hdc As Long, ByVal x As Long, ByVal y As Long) As Long


Private Type long_t
    a As Long
End Type

Private Type color_t
    r As Byte
    b As Byte
    g As Byte
    n As Byte
End Type

Public Function AveragePixel(hdc As Long, x As Long, y As Long) As Long
    Dim a As color_t
    Dim b As color_t
    Dim c As color_t
    Dim d As color_t
    Dim e As color_t
    Dim al As long_t
    Dim bl As long_t
    Dim cl As long_t
    Dim dl As long_t
    Dim el As long_t
    al.a = GetPixel(hdc, x - 1, y)
    bl.a = GetPixel(hdc, x + 1, y)
    cl.a = GetPixel(hdc, x, y - 1)
    dl.a = GetPixel(hdc, x, y + 1)
    el.a = GetPixel(hdc, x, y)
    
    LSet a = al
    LSet b = bl
    LSet c = cl
    LSet d = dl
    LSet e = el
    
    a.r = CByte((CInt(a.r) + CInt(b.r) + CInt(c.r) + CInt(d.r) + CInt(e.r)) \ 5)
    a.g = CByte((CInt(a.g) + CInt(b.g) + CInt(c.g) + CInt(d.g) + CInt(e.g)) \ 5)
    a.b = CByte((CInt(a.b) + CInt(b.b) + CInt(c.b) + CInt(d.b) + CInt(e.b)) \ 5)
    LSet al = a
    AveragePixel = al.a
    
End Function


Private Sub Form_Load()
Me.Show
cd.ShowOpen
Dim s As String
s = cd.FileName
Dim nu As Long
nu = InStrRev(s, ".")
s = Left$(s, nu) & "hmap"
Picture1.Picture = LoadPicture(cd.FileName)
If Dir(s) <> "" Then
    Kill s
End If
Open s For Binary As #1
Dim x As long_t
Dim b As Byte
Dim n As color_t
Dim i As Long
Dim j As Long
Dim w As Long
w = 128
Put 1, , w
For i = 0 To w - 1
    For j = 0 To w - 1
        x.a = AveragePixel(Picture1.hdc, i, j)
        'Debug.Print x.a
        'Picture2.PSet (i, j), x.a
        LSet n = x
        b = CByte(IIf(Int(n.r) > 127, 255, n.r * 2))
        Put 1, , b
        'Debug.Print b
    Next j
Next i
Close #1
MsgBox "Complete!"
End Sub



