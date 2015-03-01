VERSION 5.00
Begin VB.Form frmNewClass 
   Caption         =   "New Class"
   ClientHeight    =   3435
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   2775
   LinkTopic       =   "Form1"
   ScaleHeight     =   3435
   ScaleWidth      =   2775
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox Text1 
      Height          =   285
      Left            =   360
      TabIndex        =   4
      Top             =   1560
      Width           =   1935
   End
   Begin VB.CheckBox Check1 
      Caption         =   "inherits"
      Height          =   375
      Left            =   360
      TabIndex        =   3
      Top             =   720
      Width           =   1935
   End
   Begin VB.TextBox txtClassName 
      Height          =   375
      Left            =   240
      TabIndex        =   2
      Top             =   120
      Width           =   2175
   End
   Begin VB.CommandButton cmdCancel 
      Caption         =   "Cancel"
      Height          =   375
      Left            =   120
      TabIndex        =   1
      Top             =   3000
      Width           =   1215
   End
   Begin VB.CommandButton cmdCreate 
      Caption         =   "Create"
      Height          =   375
      Left            =   1440
      TabIndex        =   0
      Top             =   3000
      Width           =   1215
   End
   Begin VB.Label Label1 
      Caption         =   "from"
      Height          =   255
      Left            =   480
      TabIndex        =   5
      Top             =   1200
      Width           =   1455
   End
End
Attribute VB_Name = "frmNewClass"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Mutex As Long

Private Sub cmdCancel_Click()
    Unload Me
End Sub

Private Sub cmdCreate_Click()
    frmMain.treeClasses.Nodes.Add , tvwChild, txtClassName.Text, txtClassName.Text
    frmMain.real_f_string = "class " & txtClassName.Text
    If (Check1.Value) Then
        frmMain.real_f_string = frmMain.real_f_string & " inherits " & Text1.Text
    End If
    frmMain.real_f_string = frmMain.real_f_string & vbCrLf & "{"
    Unload Me
End Sub

Private Sub Form_Load()
    Mutex = CreateMutex(ByVal 0, 0, "CreateNewClass_Mutex")
    If Mutex = 0 Then Unload Me
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    CloseHandle Mutex
End Sub
