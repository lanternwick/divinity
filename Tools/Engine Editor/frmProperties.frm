VERSION 5.00
Object = "{5E9E78A0-531B-11CF-91F6-C2863C385E30}#1.0#0"; "MSFLXGRD.OCX"
Begin VB.Form frmProperties 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Properties..."
   ClientHeight    =   7170
   ClientLeft      =   150
   ClientTop       =   720
   ClientWidth     =   4800
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   7170
   ScaleWidth      =   4800
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox Text1 
      Height          =   2535
      Left            =   360
      MultiLine       =   -1  'True
      TabIndex        =   10
      Text            =   "frmProperties.frx":0000
      Top             =   4440
      Width           =   3975
   End
   Begin VB.TextBox txtnewProp 
      Height          =   495
      Left            =   3240
      TabIndex        =   9
      Top             =   360
      Visible         =   0   'False
      Width           =   1215
   End
   Begin VB.ComboBox cmbParent 
      Height          =   315
      ItemData        =   "frmProperties.frx":0006
      Left            =   1440
      List            =   "frmProperties.frx":000D
      Sorted          =   -1  'True
      TabIndex        =   8
      Text            =   "Root"
      Top             =   480
      Width           =   1695
   End
   Begin VB.CommandButton cmdSet 
      Caption         =   "Set Value"
      Height          =   375
      Left            =   3480
      TabIndex        =   7
      Top             =   3960
      Visible         =   0   'False
      Width           =   975
   End
   Begin VB.TextBox txtEdit 
      Height          =   285
      Left            =   360
      TabIndex        =   6
      Top             =   3960
      Visible         =   0   'False
      Width           =   3015
   End
   Begin VB.TextBox txtType 
      Height          =   285
      Left            =   1440
      TabIndex        =   4
      Top             =   840
      Width           =   1695
   End
   Begin VB.TextBox txtName 
      Height          =   285
      Left            =   1440
      TabIndex        =   1
      Top             =   120
      Width           =   1695
   End
   Begin MSFlexGridLib.MSFlexGrid PropertiesGrid 
      Height          =   2535
      Left            =   360
      TabIndex        =   0
      Top             =   1320
      Width           =   3975
      _ExtentX        =   7011
      _ExtentY        =   4471
      _Version        =   393216
      FixedCols       =   0
      BackColorBkg    =   -2147483643
      AllowUserResizing=   1
      FormatString    =   "Property Name                      |Value                           "
   End
   Begin VB.Label lblType 
      Caption         =   "Object Type:"
      Height          =   255
      Left            =   360
      TabIndex        =   5
      Top             =   840
      Width           =   975
   End
   Begin VB.Label lblParent 
      Caption         =   "Object Parent:"
      Height          =   255
      Left            =   360
      TabIndex        =   3
      Top             =   480
      Width           =   1095
   End
   Begin VB.Label lblName 
      Caption         =   "Object Name:"
      Height          =   255
      Left            =   360
      TabIndex        =   2
      Top             =   120
      Width           =   975
   End
   Begin VB.Menu mnuFile 
      Caption         =   "File"
      Begin VB.Menu mnuAddprop 
         Caption         =   "Add Property"
      End
   End
End
Attribute VB_Name = "frmProperties"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private InCell As Boolean
Private CellX As Long
Private CellY As Long
Public index As Long
Private newProp As String

Private Sub cmbParent_Validate(Cancel As Boolean)
    Dim n As Node
    Dim n2 As Node
    Set n = frmMain.tvBrowse.Nodes.Item(SceneNodes(index).NodeName)
    For i = 1 To frmMain.tvBrowse.Nodes.Count
        If frmMain.tvBrowse.Nodes.Item(i).Text = cmbParent.Text Then
            Set n2 = frmMain.tvBrowse.Nodes.Item(i)
            Exit For
        End If
    Next i
    If n2 Is Nothing Then
        frmMain.tvBrowse.Nodes.Remove index
        Set n = frmMain.tvBrowse.Nodes.Add(, , SceneNodes(index).NodeName, SceneNodes(index).NodeName)
        Exit Sub
    End If
    Set n.Parent = n2
    
    
End Sub

Private Sub cmdSet_Click()
    Dim t As String
    Dim l As Long
    
    t = PropertiesGrid.TextMatrix(CellY, 0)
    t = t & "=""" & txtEdit.Text & """"
    SceneNodes(index).Block(CellY + 2) = t
    Parse
    InCell = False
    txtEdit.Visible = False
    cmdSet.Visible = False
End Sub

Private Sub Form_Load()
    PropertiesGrid.Refresh
    PropertiesGrid.Rows = 10
    txtName.Text = SceneNodes(index).NodeName
    txtType.Text = SceneNodes(index).NodeType
    Dim i As Long
    cmbParent.Clear
    cmbParent.AddItem "[None]"
    For i = 0 To UBound(SceneNodes) - 1
        If i <> index Then cmbParent.AddItem SceneNodes(i).NodeName
    Next i
    For i = 0 To cmbParent.ListCount - 1
        If cmbParent.List(i) = SceneNodes(index).NodeParent Then
            cmbParent.ListIndex = i
            Exit For
        End If
    Next i
    Parse
End Sub


Private Sub Parse()
    Dim i As Long
    Dim v As Variant
    PropertiesGrid.Rows = SceneNodes(index).numParams - 2
    PropertiesGrid.Clear
    PropertiesGrid.TextMatrix(0, 0) = "Parameter Name"
    PropertiesGrid.TextMatrix(0, 1) = "Value"
    For i = 3 To SceneNodes(index).numParams - 1
        v = Split(SceneNodes(index).Block(i), "=")
        PropertiesGrid.TextMatrix(i - 2, 0) = v(0)
        v = Split(SceneNodes(index).Block(i), """")
        PropertiesGrid.TextMatrix(i - 2, 1) = v(1)
    Next i
    
    Dim n As Node
    Dim n2 As Node
    Set n = frmMain.tvBrowse.Nodes.Item(SceneNodes(index).NodeName)
    
    'If SceneNodes(index).NodeParent <> "" Then
    '    Set n2 = frmMain.tvBrowse.Nodes.Item(cmbParent.Text)
    '    Set n.Parent = n2
    'End If
    SceneNodes(index).Block(1) = "Parent=""" & cmbParent.List(cmbParent.ListIndex) & """"
    SceneNodes(index).NodeParent = cmbParent.List(cmbParent.ListIndex)
    
    Text1.Text = ""
    For i = 0 To SceneNodes(index).numParams - 1
        Text1.Text = Text1.Text & SceneNodes(index).Block(i) & vbCrLf
    Next i
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Parse
End Sub

Private Sub mnuAddprop_Click()
    ReDim Preserve SceneNodes(index).Block(SceneNodes(index).numParams)
    PropertiesGrid.Rows = PropertiesGrid.Rows + 1
    PropertiesGrid.Row = PropertiesGrid.Rows - 1
    PropertiesGrid.Col = 0
    txtnewProp.Top = PropertiesGrid.CellTop + PropertiesGrid.Top
    txtnewProp.Left = PropertiesGrid.CellLeft + PropertiesGrid.Left
    txtnewProp.Width = PropertiesGrid.CellWidth
    txtnewProp.Height = PropertiesGrid.CellHeight
    txtnewProp.Visible = True
    txtnewProp.SetFocus
    
    
End Sub

Private Sub PropertiesGrid_DblClick()
    Dim temp As String
    Dim i As Long
    If InCell = False Then
        If PropertiesGrid.Col = 0 And PropertiesGrid.Rows > 0 Then
            If MsgBox("Are you sure you wish to delete this Parameter?", vbYesNo, "Delete Parameter?") = vbYes Then
                For i = PropertiesGrid.Row + 2 To SceneNodes(index).numParams - 2
                    SceneNodes(index).Block(i) = SceneNodes(index).Block(i + 1)
                Next i
                ReDim Preserve SceneNodes(index).Block(SceneNodes(index).numParams - 2)
                SceneNodes(index).numParams = SceneNodes(index).numParams - 1
            End If
            Parse
            Exit Sub
        End If
    End If
                 
                
    
    If InCell = False Then
        If PropertiesGrid.Col = 1 Then
            txtEdit.Text = PropertiesGrid.TextMatrix(PropertiesGrid.Row, PropertiesGrid.Col)
            txtEdit.Visible = True
            cmdSet.Visible = True
            InCell = True
            CellX = PropertiesGrid.Col
            CellY = PropertiesGrid.Row
        End If
    End If
    
End Sub

Private Sub PropertiesGrid_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
    If Button = 2 Then
        PopupMenu mnuFile
    End If
End Sub

Private Sub txtnewProp_KeyPress(KeyAscii As Integer)
    Dim i As Long
    If KeyAscii = Asc(vbCr) Then
            newProp = txtnewProp.Text & "="""""
            For i = 0 To SceneNodes(index).numParams
                If SceneNodes(index).Block(i) = "" Then
                    SceneNodes(index).Block(i) = newProp
                    Exit For
                End If
            Next i
            SceneNodes(index).numParams = SceneNodes(index).numParams + 1
            txtnewProp.Visible = False
            txtnewProp.Text = ""
            Parse
    End If
End Sub
