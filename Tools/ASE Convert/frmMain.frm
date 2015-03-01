VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "Form1"
   ClientHeight    =   3930
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   5370
   LinkTopic       =   "Form1"
   ScaleHeight     =   3930
   ScaleWidth      =   5370
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox Text1 
      Height          =   3375
      Left            =   0
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   0
      Text            =   "frmMain.frx":0000
      Top             =   0
      Width           =   5055
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_Load()
    Text1.Text = LoadTextFile("c:\3dsmax3\meshes\tower.ase")
    Text1.Text = RemoveTabs(Text1.Text)
    Text1.Text = RemoveSpaces(Text1.Text)
    
    Dim mesh As ParsedASE
    Dim b As Bone
    Dim i As Long
    'Do
    '    GetBoneFromString Text1.Text, i, b
    '    i = i + 1
    'Loop While b.Name <> ""
    ParseASEFromString Text1.Text, mesh
    SaveParsedASE "c:\tower.model", mesh
End Sub

Private Sub Form_Resize()
    Text1.Move 0, 0, ScaleWidth, ScaleHeight
End Sub
