VERSION 5.00
Begin VB.Form frmNewFunction 
   Caption         =   "New Function"
   ClientHeight    =   3825
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   3480
   LinkTopic       =   "Form1"
   ScaleHeight     =   3825
   ScaleWidth      =   3480
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdCancel 
      Caption         =   "Cancel"
      Height          =   495
      Left            =   960
      TabIndex        =   6
      Top             =   3240
      Width           =   1095
   End
   Begin VB.CommandButton cmdCreate 
      Caption         =   "Create"
      Height          =   495
      Left            =   2160
      TabIndex        =   5
      Top             =   3240
      Width           =   1215
   End
   Begin VB.CommandButton cmdAddArg 
      Caption         =   "Add Argument"
      Height          =   375
      Left            =   2160
      TabIndex        =   4
      Top             =   2760
      Width           =   1215
   End
   Begin VB.ComboBox cmbArgTypes 
      Height          =   315
      ItemData        =   "frmNewFunction.frx":0000
      Left            =   2160
      List            =   "frmNewFunction.frx":0010
      TabIndex        =   3
      Text            =   "float"
      Top             =   2400
      Width           =   1215
   End
   Begin VB.TextBox txtArgName 
      Height          =   315
      Left            =   120
      TabIndex        =   2
      Top             =   2400
      Width           =   1935
   End
   Begin VB.ListBox lstFunctionArgs 
      Height          =   1815
      Left            =   120
      TabIndex        =   1
      Top             =   480
      Width           =   3255
   End
   Begin VB.TextBox txtFunctionName 
      Height          =   285
      Left            =   120
      TabIndex        =   0
      Top             =   90
      Width           =   3255
   End
End
Attribute VB_Name = "frmNewFunction"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Mutex As Long

Private Sub cmdAddArg_Click()
    'If Len(txtArgName.Text) > 0 Then
        lstFunctionArgs.AddItem cmbArgTypes.Text
    'End If
    txtArgName.Text = ""
End Sub

Private Sub cmdCancel_Click()
    Unload Me
End Sub

Private Sub cmdCreate_Click()
    Dim function_string As String
    If Len(txtFunctionName.Text) = 0 Then
        Unload Me
        Exit Sub
    End If
    function_string = "function " & Chr(7) & txtFunctionName.Text & "("
    Dim i As Long
    For i = 0 To lstFunctionArgs.ListCount - 1
        If i = 0 Then
            function_string = function_string & lstFunctionArgs.List(i)
        Else
            function_string = function_string & ", " & lstFunctionArgs.List(i)
        End If
    Next i
    function_string = function_string & ");"
    Dim n As Node
    Set n = frmMain.treeClasses.Nodes.Add(frmMain.treeClasses.SelectedItem, tvwChild, function_string, function_string)
    For i = 0 To lstFunctionArgs.ListCount - 1
        'frmMain.treeClasses.Nodes.Add n, tvwChild, lstFunctionArgs.List(i), lstFunctionArgs.List(i)
    Next i
    frmMain.real_f_string = frmMain.real_f_string & vbCrLf & function_string & Chr(7)
    Unload Me
End Sub

Private Sub Form_Load()
    Mutex = CreateMutex(ByVal 0, 0, "CreateNewClass_Mutex")
    If Mutex = 0 Then Unload Me
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    CloseHandle Mutex
End Sub

