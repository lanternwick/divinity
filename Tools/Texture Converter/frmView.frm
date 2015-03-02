VERSION 5.00
Begin VB.Form frmView 
   ClientHeight    =   6315
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7350
   LinkTopic       =   "Form1"
   ScaleHeight     =   6315
   ScaleWidth      =   7350
   Begin VB.PictureBox Picture1 
      Height          =   1575
      Left            =   2280
      ScaleHeight     =   1515
      ScaleWidth      =   1995
      TabIndex        =   2
      Top             =   1800
      Width           =   2055
   End
   Begin VB.VScrollBar VScroll1 
      Height          =   1215
      Left            =   5040
      TabIndex        =   1
      Top             =   2280
      Width           =   255
   End
   Begin VB.HScrollBar HScroll1 
      Height          =   255
      Left            =   0
      TabIndex        =   0
      Top             =   6000
      Width           =   1215
   End
End
Attribute VB_Name = "frmView"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_Load()
    HScroll1.Left = 0
    HScroll1.Top = Me.ScaleHeight - HScroll1.Height
    HScroll1.Width = Me.ScaleWidth - VScroll1.Width
    VScroll1.Top = 0
    VScroll1.Left = Me.ScaleWidth - VScroll1.Width
    VScroll1.Height = Me.ScaleHeight - HScroll1.Height
    Picture1.Move 0, 0, Me.ScaleWidth - VScroll1.Width, Me.ScaleHeight - HScroll1.Height
End Sub

Private Sub Form_Resize()
    HScroll1.Left = 0
    HScroll1.Top = Me.ScaleHeight - HScroll1.Height
    HScroll1.Width = Me.ScaleWidth - VScroll1.Width
    VScroll1.Top = 0
    VScroll1.Left = Me.ScaleWidth - VScroll1.Width
    VScroll1.Height = Me.ScaleHeight - HScroll1.Height
    Picture1.Move HScroll1.Value, VScroll1.Value, Me.ScaleWidth - VScroll1.Width, Me.ScaleHeight - HScroll1.Height
End Sub

Private Sub HScroll1_Change()
    Picture1.Move -HScroll1.Value, -VScroll1.Value, Me.ScaleWidth - VScroll1.Width, Me.ScaleHeight - HScroll1.Height
End Sub

Private Sub VScroll1_Change()
    Picture1.Move -HScroll1.Value, -VScroll1.Value, Me.ScaleWidth - VScroll1.Width, Me.ScaleHeight - HScroll1.Height
End Sub
