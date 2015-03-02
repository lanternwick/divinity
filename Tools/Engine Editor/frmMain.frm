VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frmMain 
   Caption         =   "Divinity Engine Editor"
   ClientHeight    =   8310
   ClientLeft      =   285
   ClientTop       =   735
   ClientWidth     =   11400
   LinkTopic       =   "Form1"
   ScaleHeight     =   8310
   ScaleWidth      =   11400
   Begin MSComDlg.CommonDialog CD 
      Left            =   7080
      Top             =   3960
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin MSComctlLib.ImageList tvBrowse_Images 
      Left            =   7320
      Top             =   5160
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   16777215
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   8
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0000
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0352
            Key             =   "StaticMesh"
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":06A4
            Key             =   "DataTemplate"
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":09F6
            Key             =   "Data"
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0D48
            Key             =   "Light"
         EndProperty
         BeginProperty ListImage6 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":109A
            Key             =   "AnimatedMesh"
         EndProperty
         BeginProperty ListImage7 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":13EC
            Key             =   "Root"
         EndProperty
         BeginProperty ListImage8 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":173E
            Key             =   "Bone"
         EndProperty
      EndProperty
   End
   Begin MSComctlLib.TreeView tvBrowse 
      Height          =   10800
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   2895
      _ExtentX        =   5106
      _ExtentY        =   19050
      _Version        =   393217
      Indentation     =   265
      LineStyle       =   1
      Style           =   7
      ImageList       =   "tvBrowse_Images"
      Appearance      =   1
   End
   Begin VB.Menu mnuFile 
      Caption         =   "File"
      Begin VB.Menu mnuLoadScene 
         Caption         =   "Load Scene"
      End
      Begin VB.Menu mnuSaveAs 
         Caption         =   "Save Scene As..."
      End
      Begin VB.Menu mnuSep0 
         Caption         =   "-"
      End
      Begin VB.Menu mnuExit 
         Caption         =   "Exit"
      End
   End
   Begin VB.Menu mnuDefaultPopup 
      Caption         =   "DefPop"
      Visible         =   0   'False
      Begin VB.Menu mnuCreate 
         Caption         =   "Create Object..."
      End
   End
   Begin VB.Menu mnuItemPopup 
      Caption         =   "ObjPop"
      Visible         =   0   'False
      Begin VB.Menu mnuObjProperties 
         Caption         =   "Properties..."
      End
   End
   Begin VB.Menu mnuTools 
      Caption         =   "Tools"
      Begin VB.Menu mnuModelImport 
         Caption         =   "Model Viewer/Importer"
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
 
Private editname As String
Private currXonTree As Single
Private currYonTree As Single


Public Sub NewScene()
    tvBrowse.Nodes.Clear
    tvBrowse.Nodes.Add , , "Root", "Root", "Root"
    
End Sub

Private Sub Form_Load()
    NewScene
End Sub

Private Sub mnuCreate_Click()
    frmObjSelect.Show 1, Me
    ReDim Preserve SceneNodes(UBound(SceneNodes) + 1)
    CreateNewNode UBound(SceneNodes)
    tvBrowse.Nodes.Add "Root", tvwChild, frmObjSelect.SelectedObject, frmObjSelect.SelectedObject
End Sub

Private Sub mnuExit_Click()
    Unload Me
End Sub

Private Sub mnuLoadScene_Click()
    On Error GoTo errExit
    Dim d As String
    
    CD.CancelError = True
    CD.Filter = "*.*"
    CD.ShowOpen
    d = LoadTextFile(CD.Filename)
    d = RemoveTabs(d)
    d = RemoveSpaces(d)
    tvBrowse.Nodes.Clear
    ReDim SceneNodes(0)
    LoadSceneFromString d
errExit:
End Sub

Private Sub mnuModelImport_Click()
    frmMdlView.Show 1, Me
End Sub

Private Sub mnuObjProperties_Click()
    Dim i As Long
    For i = 0 To UBound(SceneNodes) - 1
        If SceneNodes(i).NodeName = tvBrowse.SelectedItem.Text Then
            frmProperties.index = i
            Exit For
        End If
    Next i
    frmProperties.Show 1, Me
End Sub

Private Sub mnuSaveAs_Click()
    On Error GoTo errExit
    Dim d As String
    
    CD.CancelError = True
    CD.Filter = "*.*"
    CD.ShowSave
    SaveSceneToFile CD.Filename
errExit:
End Sub

Private Sub tvBrowse_AfterLabelEdit(Cancel As Integer, NewString As String)
    Dim index As Long
    Dim i As Long
    Dim n As Node
    If Cancel Then Exit Sub
    Set n = tvBrowse.SelectedItem
    If n Is Nothing Then Exit Sub
    For i = 0 To tvBrowse.Nodes.Count - 1
        If n.Text = SceneNodes(i).NodeName Then
            index = i
            Exit For
        End If
    Next i
    SceneNodes(index).NodeName = NewString
    SceneNodes(index).Block(0) = "Name=""" & NewString & """"
    
End Sub


Private Sub tvBrowse_DblClick()
 
    Dim n As Node
    Set n = tvBrowse.HitTest(currXonTree, currYonTree)
    If n Is Nothing Then
        Exit Sub
    End If
    
    Dim i As Long
    For i = 0 To UBound(SceneNodes) - 1
        If SceneNodes(i).NodeName = n.Text Then
            frmProperties.index = i
            Exit For
        End If
    Next i
    frmProperties.Show 1, Me
 
End Sub

Private Sub tvBrowse_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
    Dim n As Node
    Set n = tvBrowse.HitTest(x, y)
    If Button = 2 Then
        If n Is Nothing Then
            PopupMenu mnuDefaultPopup
        Else
            PopupMenu mnuItemPopup
        End If
    End If
End Sub

Public Sub LoadSceneFromString(data As String)
    Dim n As Node
    Dim n2 As Node
    Dim i As Long
    Dim j As Long
    ParseSceneFromString data, SceneNodes
    tvBrowse.Nodes.Clear
    Dim AcceptedTypes() As String
    ReDim AcceptedTypes(tvBrowse_Images.ListImages.Count - 1)
    For i = 0 To tvBrowse_Images.ListImages.Count - 1
        AcceptedTypes(i) = tvBrowse_Images.ListImages.Item(i + 1).Key
    Next i
    For i = 0 To UBound(SceneNodes)
        Set n = tvBrowse.Nodes.Add(, , SceneNodes(i).NodeName, SceneNodes(i).NodeName)
        If SceneNodes(i).NodeParent <> "" Then
            Set n2 = tvBrowse.Nodes.Item(SceneNodes(i).NodeParent)
            Set n.Parent = n2
        End If
        For j = 0 To tvBrowse_Images.ListImages.Count - 1
            If SceneNodes(i).NodeType = AcceptedTypes(j) Then
                n.Image = AcceptedTypes(j)
                Exit For
            End If
        Next j
    Next i
End Sub

Public Sub SaveSceneToFile(Filename As String)
    Dim tData As String
    Dim i As Long
    Open Filename For Output As #1
    Print #1, UBound(SceneNodes) + 1
    For i = 0 To UBound(SceneNodes)
        Print #1, Replace(SceneNodes(i).NodeName, " ", "")
    Next i
        
    For i = 0 To UBound(SceneNodes)
        tData = Replace(SceneNodes(i).NodeName, " ", "")
        Print #1, tData & vbCrLf & "{"
        For j = 0 To SceneNodes(i).numParams - 1
            Print #1, SceneNodes(i).Block(j)
        Next j
        Print #1, "}" & vbCrLf
    Next i
    Close #1
End Sub

Private Sub tvBrowse_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
    currXonTree = x
    currYonTree = y
    
End Sub
