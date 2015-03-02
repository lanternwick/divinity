VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frmMain 
   AutoRedraw      =   -1  'True
   Caption         =   "Convert"
   ClientHeight    =   8145
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   10635
   LinkTopic       =   "Form1"
   ScaleHeight     =   8145
   ScaleWidth      =   10635
   Begin VB.TextBox txtLength 
      Height          =   285
      Left            =   7680
      TabIndex        =   17
      Text            =   "1.0"
      Top             =   3960
      Width           =   1335
   End
   Begin VB.CommandButton cmdAddPair 
      Caption         =   "Add Pair..."
      Height          =   375
      Left            =   4080
      TabIndex        =   16
      Top             =   1800
      Visible         =   0   'False
      Width           =   1935
   End
   Begin VB.ListBox lstPairs 
      Height          =   3375
      Left            =   7680
      TabIndex        =   15
      Top             =   120
      Width           =   2535
   End
   Begin VB.CheckBox chkAnim 
      Caption         =   "Animated Texture?"
      Height          =   375
      Left            =   5520
      TabIndex        =   14
      Top             =   7800
      Width           =   1815
   End
   Begin MSComDlg.CommonDialog browse 
      Left            =   4440
      Top             =   2040
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CommandButton cmdBrowseAlpha 
      Caption         =   "Browse..."
      Height          =   375
      Left            =   6240
      TabIndex        =   13
      Top             =   960
      Width           =   975
   End
   Begin VB.CommandButton cmdBrowseColor 
      Caption         =   "Browse..."
      Height          =   375
      Left            =   6240
      TabIndex        =   12
      Top             =   120
      Width           =   975
   End
   Begin VB.TextBox txtH 
      Height          =   375
      Left            =   5280
      TabIndex        =   11
      Text            =   "13"
      Top             =   4080
      Width           =   615
   End
   Begin VB.TextBox txtW 
      Height          =   375
      Left            =   4200
      TabIndex        =   10
      Text            =   "11"
      Top             =   4080
      Width           =   615
   End
   Begin VB.CommandButton cmdMakeFont 
      Caption         =   "Make Font"
      Height          =   375
      Left            =   4080
      TabIndex        =   9
      Top             =   4560
      Width           =   1935
   End
   Begin VB.TextBox txtAlpha 
      Height          =   285
      Left            =   4080
      TabIndex        =   8
      Text            =   "--Alpha Mask Filename--"
      Top             =   960
      Width           =   2055
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Load Alpha Mask"
      Height          =   375
      Left            =   4080
      TabIndex        =   7
      Top             =   1320
      Width           =   1935
   End
   Begin VB.PictureBox Picture3 
      AutoRedraw      =   -1  'True
      BackColor       =   &H00FF00FF&
      ForeColor       =   &H00FFFFFF&
      Height          =   3975
      Left            =   0
      ScaleHeight     =   261
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   261
      TabIndex        =   6
      Top             =   4080
      Width           =   3975
   End
   Begin VB.CommandButton cmdLoad 
      Caption         =   "Load Image"
      Height          =   375
      Left            =   4080
      TabIndex        =   5
      Top             =   480
      Width           =   1935
   End
   Begin VB.TextBox txtInput 
      Height          =   285
      Left            =   4080
      TabIndex        =   4
      Text            =   "--Input Filename--"
      Top             =   120
      Width           =   2055
   End
   Begin VB.TextBox txtFile 
      Height          =   285
      Left            =   4080
      TabIndex        =   3
      Text            =   "--Output Filename--"
      Top             =   3240
      Width           =   2055
   End
   Begin VB.CommandButton cmdProcess 
      Caption         =   "Process"
      Height          =   375
      Left            =   4080
      TabIndex        =   2
      Top             =   3600
      Width           =   1935
   End
   Begin VB.ComboBox cmbDim 
      Height          =   315
      Left            =   4080
      TabIndex        =   1
      Text            =   "--Select Dimensions--"
      Top             =   2880
      Width           =   2055
   End
   Begin VB.PictureBox Picture1 
      AutoRedraw      =   -1  'True
      BackColor       =   &H00FF00FF&
      BeginProperty Font 
         Name            =   "Verdana"
         Size            =   6.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   3975
      Left            =   0
      ScaleHeight     =   261
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   261
      TabIndex        =   0
      Top             =   0
      Width           =   3975
   End
   Begin VB.Label Label1 
      Caption         =   "Animation Length"
      Height          =   255
      Left            =   7680
      TabIndex        =   18
      Top             =   3600
      Width           =   2535
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal Length As Long)
Private HasAlphaMask As Boolean

Private Type color
    r As Byte
    g As Byte
    
    
    
    b As Byte
    a As Byte
End Type
    

Private Sub chkAnim_Click()
    If chkAnim.Value Then
        Me.Width = Me.Width + 3330
        chkAnim.Left = chkAnim.Left + 3330
        cmdAddPair.Visible = True
    Else
        Me.Width = Me.Width - 3330
        chkAnim.Left = chkAnim.Left - 3330
        cmdAddPair.Visible = False
    End If
End Sub

Private Sub cmdAddPair_Click()
    Dim s As String
    s = txtInput.Text & " & " & txtAlpha.Text
    lstPairs.AddItem s
End Sub

Private Sub cmdBrowseAlpha_Click()
    browse.DialogTitle = "Browse..."
    browse.Filter = "All|*.*"
    browse.ShowOpen
    txtAlpha = browse.FileName
End Sub

Private Sub cmdBrowseColor_Click()
    browse.DialogTitle = "Browse..."
    browse.Filter = "All|*.*"
    browse.ShowOpen
    txtInput = browse.FileName
End Sub

Private Sub cmdLoad_Click()
    Picture1.Picture = LoadPicture(txtInput.Text)
    Dim i As Long
    i = InStrRev(txtInput.Text, ".")
    txtFile.Text = Left$(txtInput.Text, i) & "texture"
End Sub

Private Sub cmdMakeFont_Click()
    Dim i As Long
    Picture1.Line (0, 0)-(255, 255), vbBlack, BF
    For i = Asc(" ") To 255
        Picture1.CurrentX = (((i - Asc(" ")) Mod 20)) * Val(txtW)
        Picture1.CurrentY = (((i - Asc(" ")) \ 20)) * Val(txtH)
        Picture1.Print Chr(i)
    Next i
    DoEvents
    Picture1.Picture = Picture1.Image
    Picture3.Picture = Picture1.Picture
    HasAlphaMask = True
    Dim n As Long
    Dim s As String
    n = InStr(1, cmbDim.Text, "x")
    '//If n = 0 Then Exit Sub
    s = Left(cmbDim.Text, n)
    n = Val(s)
    Open txtFile.Text For Binary As #1
    '//Put 1, , CLng(n - 1)
    Dim cellw As Long
    cellw = Val(txtH)
    Put #1, , cellw
    cellw = Val(txtW)
    Put #1, , cellw
        
    'Dim i As Long
    Dim j As Long
    Dim x As color
    For j = 0 To n - 1
        For i = 0 To n - 1
            If HasAlphaMask Then
                x = ComputePixel(i, j)
                
                Put 1, , x.b
                Put 1, , x.g
                Put 1, , x.r
                Put 1, , x.a
            Else
                'Put 1, , Picture1.Point(i, j)
                x.a = &HFF
                Put 1, , x.b
                Put 1, , x.g
                Put 1, , x.r
                Put 1, , x.a
            End If
            Picture2.PSet (i, j), RGB(x.r, x.g, x.b)
            'Put 1, , x
        Next i
        DoEvents
    Next j
    Close #1
End Sub

Private Sub cmdProcess_Click()
    If chkAnim.Value Then
        ProcessAnimated
    Else
        ProcessTexture
    End If
    
    MsgBox "Complete!", vbOKOnly
End Sub

Private Sub Command1_Click()
    Picture3.Picture = LoadPicture(txtAlpha.Text)
    HasAlphaMask = True
End Sub

Private Sub Form_Load()
    Me.Width = 7395
    Dim i As Long
    For i = 0 To 10
        cmbDim.AddItem 2 ^ i & "x" & 2 ^ i & " pixels"
    Next i
End Sub

Private Sub Text1_Change()

End Sub

Private Sub txtAlpha_Click()
    If txtAlpha.Text = "--Alpha Mask Filename--" Then txtAlpha.Text = ""
End Sub

Private Sub txtAlpha_LostFocus()
    If txtAlpha.Text = "" Then txtAlpha.Text = "--Alpha Mask Filename--"
End Sub

Private Sub txtFile_Click()
    If txtFile.Text = "--Output Filename--" Then txtFile.Text = ""
End Sub

Private Sub txtFile_LostFocus()
    If txtFile.Text = "" Then txtFile.Text = "--Output Filename--"
End Sub

Private Sub ProcessTexture()
    Dim n As Long
    Dim s As String
    n = InStr(1, cmbDim.Text, "x")
    '//If n = 0 Then Exit Sub
    s = Left(cmbDim.Text, n)
    n = Val(s)
    Open txtFile.Text For Binary As #1
    '//Put 1, , CLng(n - 1)
    Dim i As Long
    Dim j As Long
    Dim x As color
    For j = 0 To n - 1
        For i = 0 To n - 1
            If HasAlphaMask Then
                x = ComputePixel(i, j)
                
                Put 1, , x.b
                Put 1, , x.g
                Put 1, , x.r
                Put 1, , x.a
            Else
                'Put 1, , Picture1.Point(i, j)
                x.a = &HFF
                Put 1, , x.b
                Put 1, , x.g
                Put 1, , x.r
                Put 1, , x.a
            End If
            'Picture2.PSet (i, j), RGB(x.r, x.g, x.b)
            'Put 1, , x
        Next i
        DoEvents
    Next j
    Close #1
End Sub

Private Sub ProcessAnimated()
    Dim n As Long
    Dim s As String
    n = InStr(1, cmbDim.Text, "x")
    '//If n = 0 Then Exit Sub
    s = Left(cmbDim.Text, n)
    n = Val(s)
    Open txtFile.Text For Binary As #1
    '//Put 1, , CLng(n - 1)
    Dim files As Variant
    Dim i As Long
    Dim j As Long
    Dim x As color
    Dim k As Long
    k = lstPairs.ListCount
    Put 1, , k
    Dim rate As Single
    Put 1, , n
    rate = Val(txtLength.Text)
    Put 1, , rate
    For k = 0 To lstPairs.ListCount - 1
        files = Split(lstPairs.List(k), " & ")
        Picture1.Picture = LoadPicture(files(0))
        Picture3.Picture = LoadPicture(files(1))
        DoEvents
        For j = 0 To n - 1
            For i = 0 To n - 1
                If HasAlphaMask Then
                    x = ComputePixel(i, j)
                    
                    Put 1, , x.b
                    Put 1, , x.g
                    Put 1, , x.r
                    Put 1, , x.a
                Else
                    'Put 1, , Picture1.Point(i, j)
                    x.a = &HFF
                    Put 1, , x.b
                    Put 1, , x.g
                    Put 1, , x.r
                    Put 1, , x.a
                End If
                'Picture2.PSet (i, j), RGB(x.r, x.g, x.b)
                'Put 1, , x
            Next i
            DoEvents
        Next j
    Next k
    Close #1
End Sub

Private Sub txtInput_Click()
    If txtInput.Text = "--Input Filename--" Then txtInput.Text = ""
End Sub

Private Sub txtInput_LostFocus()
    If txtInput.Text = "" Then txtInput.Text = "--Input Filename--"
End Sub

Private Function ComputePixel(x As Long, y As Long) As color
    Dim c As Long
    Dim rcolor As color
    Dim acolor As color
    
    c = Picture1.Point(x, y)
    'Picture4.PSet (x, y), c
    CopyMemory rcolor, c, 3
    
    Dim c2 As Long
    c2 = Picture3.Point(x, y)
    CopyMemory acolor, c2, 4
    rcolor.a = acolor.r
    ComputePixel = rcolor
    
End Function
