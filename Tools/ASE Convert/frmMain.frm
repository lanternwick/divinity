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
    Dim mtext As String
    mtext = LoadTextFile("c:\texcache_\snow_corpse.ase")
    mtext = RemoveTabs(mtext)
    mtext = RemoveSpaces(mtext)
    
    Dim mesh As ParsedASE
    Dim b As Bone
    Dim i As Long
    'Do
    '    GetBoneFromString Text1.Text, i, b
    '    i = i + 1
    'Loop While b.Name <> ""
    Dim data As String
    i = 0
    mesh.ID = &HB83E9C38
    Do
        data = GetASESubsetFromString(mtext, i)
        If data = "" Then
            Exit Do
        End If
        i = i + 1
        ReDim Preserve mesh.Subsets(i - 1)
        ParseASEFromString data, mesh.Subsets(i - 1)
    Loop
    mesh.numSubsets = i

    'ParseASEFromString Text1.Text, mesh
    SaveParsedASE "c:\texcache_\snow_corpse.model", mesh
End Sub

Private Sub Form_Resize()
    Text1.Move 0, 0, ScaleWidth, ScaleHeight
End Sub
