VERSION 5.00
Begin VB.Form frmMain 
   AutoRedraw      =   -1  'True
   Caption         =   "Pathing Map"
   ClientHeight    =   8715
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   9870
   LinkTopic       =   "Form1"
   ScaleHeight     =   8715
   ScaleWidth      =   9870
   StartUpPosition =   3  'Windows Default
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public Density As Long
Private Const Spacing As Long = 250

Private Const mWidth As Long = 32
Private Sub Form_Load()
    frmHgt.Show
    Density = CLng(InputBox("Density:"))
    Me.Show
    DrawPathing
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    Unload frmHgt
End Sub

Public Sub DrawPathing()
    Dim i As Long
    Dim j As Long
    
    For i = 0 To mWidth Step Density
        For j = 0 To mWidth Step Density
            frmMain.Circle ((i + 1) * Spacing, (j + 1) * Spacing), 75
        Next j
    Next i
    DoEvents
    drawmapping mWidth \ 2 + 1, mWidth \ 2 + 1, 5
End Sub

Public Sub drawmapping(x As Long, y As Long, depth As Long)
    'If x >= mWidth Or x < 1 Or y >= mWidth Or y < 1 Then
    '    GoTo ende
    'End If
    Dim col As Long
    Dim l As Single
    
    If depth = -1 Then Exit Sub
    l = depth / mWidth * 4
    col = RGB(l * 255, l * 255, l * 255)
    If (depth = 0) Then
        Line (x * Spacing, y * Spacing)-((x - ((depth + 1))) * Spacing, y * Spacing), col
        Line (x * Spacing, y * Spacing)-((x + ((depth + 1))) * Spacing, (y) * Spacing), col
        Line (x * Spacing, y * Spacing)-((x) * Spacing, (y - ((depth + 1))) * Spacing), col
        Line (x * Spacing, y * Spacing)-((x) * Spacing, (y + ((depth + 1))) * Spacing), col
        Line (x * Spacing, y * Spacing)-((x + (depth + 1)) * Spacing, (y - (depth + 1)) * Spacing)
        Line (x * Spacing, y * Spacing)-((x - (depth + 1)) * Spacing, (y - (depth + 1)) * Spacing)
        Line (x * Spacing, y * Spacing)-((x + (depth + 1)) * Spacing, (y + (depth + 1)) * Spacing)
        Line (x * Spacing, y * Spacing)-((x - (depth + 1)) * Spacing, (y + (depth + 1)) * Spacing)
        DoEvents
    End If
    
    drawmapping x - (depth * Density), y - (depth * Density), depth - 1
    drawmapping x + (depth * Density), y - (depth * Density), depth - 1
    
    drawmapping x - (depth * Density), y + (depth * Density), depth - 1
    drawmapping x + (depth * Density), y + (depth * Density), depth - 1
    
    'Line (x, y)-(x + Density, y + Density)
    'drawmapping x + Density, y + Density
    'Line (x, y)-(x - Density, y + Density)
    'drawmapping x + Density, y - Density
    'Line (x, y)-(x - density, y)
    'drawmapping x - density, y
    'Line (x, y)-(x - density, y)
    'drawmapping x - density, y
ende:
End Sub
