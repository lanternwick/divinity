VERSION 5.00
Begin VB.Form frmObjSelect 
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "Select Object Type"
   ClientHeight    =   3855
   ClientLeft      =   45
   ClientTop       =   285
   ClientWidth     =   2550
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3855
   ScaleWidth      =   2550
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   375
      Left            =   1320
      TabIndex        =   3
      Top             =   3360
      Width           =   1095
   End
   Begin VB.CommandButton cmdOK 
      Caption         =   "OK"
      Height          =   375
      Left            =   120
      TabIndex        =   2
      Top             =   3360
      Width           =   1095
   End
   Begin VB.TextBox txtSelect 
      Height          =   285
      Left            =   120
      TabIndex        =   1
      Top             =   3000
      Width           =   2295
   End
   Begin VB.ListBox lstObjs 
      Height          =   2790
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   2295
   End
End
Attribute VB_Name = "frmObjSelect"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public SelectedObject As String
Private Sub cmdCancel_Click()
    SelectedObject = ""
End Sub

Private Sub cmdOK_Click()
    Me.Hide
End Sub

Private Sub Form_Load()
    lstObjs.AddItem "Light"
    lstObjs.AddItem "Static Mesh"
    lstObjs.AddItem "Animated Mesh"
    lstObjs.AddItem "Bone"
    lstObjs.AddItem "Data"
    lstObjs.AddItem "Data Template"
End Sub

Private Sub lstObjs_Click()
    SelectedObject = lstObjs.Text
    txtSelect.Text = SelectedObject
End Sub
