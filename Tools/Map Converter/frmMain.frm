VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "Form1"
   ClientHeight    =   6135
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6480
   LinkTopic       =   "Form1"
   ScaleHeight     =   6135
   ScaleWidth      =   6480
   StartUpPosition =   3  'Windows Default
   WindowState     =   2  'Maximized
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
Picture1.Picture = LoadPicture("C:\texcache_\2.bmp")
Kill "c:\texcache_\2.hmap"
Open "c:\texcache_\2.hmap" For Binary As #1
Dim x As long_t
Dim b As Byte
Dim n As color_t
Dim i As Long
Dim j As Long
Dim w As Long
w = 78
Put 1, , w
For i = 0 To w - 1
    For j = 0 To w - 1
        x.a = AveragePixel(Picture1.hdc, i, j)
        'Debug.Print x.a
        'Picture2.PSet (i, j), x.a
        LSet n = x
        b = n.r \ 5
        Put 1, , b
        'Debug.Print b
    Next j
Next i
Close #1
        
End Sub



