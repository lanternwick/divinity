Attribute VB_Name = "modDivData"
Public Type NodeData
    NodeName As String
    NodeParent As String
    NodeType As String
    BlockStart As Long
    BlockEnd As Long
    numParams As Long
    Block() As String
End Type
    
Sub GetNode(data As String, ID As String, NodeInfo As NodeData)
    Dim pattern As String
    Dim Block As String
    Dim s As Variant
    Dim Loc As Long
    Dim eLoc As Long
    Dim i As Long
    pattern = ID & vbCrLf & "{"
    Loc = InStr(1, data, pattern)
    If Loc = 0 Then
        NodeInfo.NodeName = ""
        Exit Sub
    End If
    Loc = Loc + Len(ID) + 3
    eLoc = InStr(Loc, data, "}") - 2
    Block = Mid$(data, Loc, eLoc - Loc)
    Block = Replace(Block, vbTab, "")
    s = Split(Block, vbCrLf)
    NodeInfo.numParams = UBound(s)
    ReDim NodeInfo.Block(NodeInfo.numParams - 1)
    For i = 1 To NodeInfo.numParams
        NodeInfo.Block(i - 1) = Trim(s(i))
    Next i
    s = Split(NodeInfo.Block(0), """")
    NodeInfo.NodeName = s(1)
    s = Split(NodeInfo.Block(1), """")
    NodeInfo.NodeParent = s(1)
    s = Split(NodeInfo.Block(2), """")
    NodeInfo.NodeType = s(1)
    
    
End Sub

Public Sub ParseSceneFromString(data As String, Nodes() As NodeData)
    Dim Num As Long
    Dim i As Long
    Num = CLng(GetLine(data, 0))
    ReDim Nodes(Num - 1)
    For i = 0 To Num - 1
        GetNode data, GetLine(data, 1 + i), Nodes(i)
    Next i
End Sub

