Attribute VB_Name = "Module1"
Option Explicit

Public Type node
    num_adj As Long
    posx As Single
    posy As Single
    weight As Single
    adj() As Long
End Type
Const noded As Long = 77
Public nodes(noded * noded) As node

Function calcindex(x As Long, y As Long, size As Long) As Long
    If (x < 0) Then
        calcindex = -1
        Exit Function
    End If
    If (y < 0) Then
        calcindex = -1
        Exit Function
    End If
    If (x >= size) Then
        calcindex = -1
        Exit Function
    End If
    If (y >= size) Then
        calcindex = -1
        Exit Function
    End If
    calcindex = x * size + y
End Function


Sub Main()
    Dim i As Long
    Dim j As Long
    
    DoEvents
    For i = 0 To noded - 1
        For j = 0 To noded - 1
            nodes(i * noded + j).posx = i
            nodes(i * noded + j).posy = j
            nodes(i * noded + j).weight = 0
            
        Next j
    Next i
    Dim l As Long
    Dim t As Long
    For i = 0 To noded - 1
        For j = 0 To noded - 1
            l = 0
            t = calcindex(i - 1, j, noded)
            If (t >= 0) Then
                ReDim Preserve nodes(calcindex(i, j, noded)).adj(l)
                nodes(calcindex(i, j, noded)).adj(l) = t
                l = l + 1
            End If
            t = calcindex(i - 1, j - 1, noded)
            If (t >= 0) Then
                ReDim Preserve nodes(calcindex(i, j, noded)).adj(l)
                nodes(calcindex(i, j, noded)).adj(l) = t
                l = l + 1
            End If
            t = calcindex(i, j - 1, noded)
            If (t >= 0) Then
                ReDim Preserve nodes(calcindex(i, j, noded)).adj(l)
                nodes(calcindex(i, j, noded)).adj(l) = t
                l = l + 1
            End If
            t = calcindex(i + 1, j, noded)
            If (t >= 0) Then
                ReDim Preserve nodes(calcindex(i, j, noded)).adj(l)
                nodes(calcindex(i, j, noded)).adj(l) = t
                l = l + 1
            End If
            t = calcindex(i + 1, j + 1, noded)
            If (t >= 0) Then
                ReDim Preserve nodes(calcindex(i, j, noded)).adj(l)
                nodes(calcindex(i, j, noded)).adj(l) = t
                l = l + 1
            End If
            t = calcindex(i, j + 1, noded)
            If (t >= 0) Then
                ReDim Preserve nodes(calcindex(i, j, noded)).adj(l)
                nodes(calcindex(i, j, noded)).adj(l) = t
                l = l + 1
            End If
            t = calcindex(i - 1, j + 1, noded)
            If (t >= 0) Then
                ReDim Preserve nodes(calcindex(i, j, noded)).adj(l)
                nodes(calcindex(i, j, noded)).adj(l) = t
                l = l + 1
            End If
            t = calcindex(i + 1, j - 1, noded)
            If (t >= 0) Then
                ReDim Preserve nodes(calcindex(i, j, noded)).adj(l)
                nodes(calcindex(i, j, noded)).adj(l) = t
                l = l + 1
            End If
            nodes(calcindex(i, j, noded)).num_adj = l
        Next j
    Next i
    writenodes "c:\pathing_map.map"
End Sub

Sub writenodes(file As String)
    Dim i As Long
    Open file For Binary As #1
    Put #1, , noded * noded
    Put #1, , noded
    
    For i = 0 To noded * noded - 1
        Put #1, , nodes(i).num_adj
        Put #1, , nodes(i).posx
        Put #1, , nodes(i).posy
        Put #1, , nodes(i).weight
        
    Next i
    
  
    Dim j As Long
    For i = 0 To noded * noded - 1
        For j = 0 To nodes(i).num_adj - 1
            Put #1, , nodes(i).adj(j)
            If nodes(i).adj(j) > noded * noded Then
                Debug.Print "Invalid index @ " & CStr(i) & ", " & j
            End If
        Next j
        
    Next i
    
    Close #1
End Sub

