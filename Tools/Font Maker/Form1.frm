VERSION 5.00
Begin VB.Form Form1 
   AutoRedraw      =   -1  'True
   BackColor       =   &H00000000&
   Caption         =   "Form1"
   ClientHeight    =   4500
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   5550
   BeginProperty Font 
      Name            =   "Verdana"
      Size            =   8.25
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H00FFFFFF&
   LinkTopic       =   "Form1"
   ScaleHeight     =   300
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   370
   StartUpPosition =   3  'Windows Default
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_Load()
    Me.Show
    Dim i As Long
    For i = Asc(" ") To 255
        Me.CurrentX = (i Mod 20) * 12
        Me.CurrentY = ((i \ 20) - 1) * 15
        Me.Print Chr(i)
    Next i
    DoEvents
    Me.Picture = Me.Image
    Kill "c:\texcache\ttt.bmp"
    SavePicture Me.Picture, "c:\texcache\ttt.bmp"
End Sub
