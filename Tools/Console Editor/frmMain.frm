VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frmMain 
   Caption         =   "Console Editor"
   ClientHeight    =   4830
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   6570
   LinkTopic       =   "Form1"
   ScaleHeight     =   4830
   ScaleWidth      =   6570
   StartUpPosition =   3  'Windows Default
   Begin MSComDlg.CommonDialog cd 
      Left            =   3000
      Top             =   2160
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin MSComctlLib.TreeView treeClasses 
      Height          =   4815
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   2175
      _ExtentX        =   3836
      _ExtentY        =   8493
      _Version        =   393217
      Indentation     =   441
      LineStyle       =   1
      Sorted          =   -1  'True
      Style           =   7
      SingleSel       =   -1  'True
      Appearance      =   1
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuExport 
         Caption         =   "&Export"
      End
      Begin VB.Menu mnuSep0 
         Caption         =   "-"
      End
      Begin VB.Menu mnuQuit 
         Caption         =   "&Quit"
      End
   End
   Begin VB.Menu mnuClass 
      Caption         =   "&Class"
      Begin VB.Menu mnuNew 
         Caption         =   "&New"
      End
      Begin VB.Menu mnuAddFunction 
         Caption         =   "&Add Function"
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public real_f_string As String

Private Sub mnuAddFunction_Click()
    If treeClasses.Nodes.Count > 0 Then
        If treeClasses.SelectedItem.Parent Is Nothing Then
            AddFunction
        End If
    End If
End Sub

Private Sub mnuExport_Click()
    cd.ShowSave
    Open cd.FileName For Output As #1
    Print #1, real_f_string
    Print #1, "}"
    Close #1
End Sub

Private Sub mnuNew_Click()
    CreateNewClass
End Sub

Public Sub CreateNewClass()
    frmNewClass.Show
    Dim mut As Long
    mut = OpenMutex(SYNCHRONIZE, 0, "CreateNewClass_Mutex")
    WaitForSingleObject mut, INFINITE
    
End Sub

Public Sub AddFunction()
    frmNewFunction.Show
    Dim mut As Long
    mut = OpenMutex(SYNCHRONIZE, 0, "CreateNewFunction_Mutex")
    WaitForSingleObject mut, INFINITE
End Sub

Private Sub mnuQuit_Click()
    Unload Me
End Sub
