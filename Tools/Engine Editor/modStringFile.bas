Attribute VB_Name = "modStringFile"
Function GetValueFromString(data As String, pattern As String, RetVal As Variant, Optional ID As Long) As Long
    If Not IsMissing(ID) Then
        pattern = Replace(pattern, "#", CStr(ID))
    End If
    Dim Loc As Long
    Dim eLoc As Long
    Dim val As String
    Loc = InStr(1, data, pattern)
    If Loc = 0 Then
        GetValueFromString = 0
        Exit Function
    End If
    
    Loc = Loc + Len(pattern)
    
    eLoc = InStr(Loc, data, " ")
    Dim temp As Long
    temp = InStr(Loc, data, vbCrLf)
    If eLoc = 0 Or (temp < eLoc And temp > 0) Then
        eLoc = temp
    End If
    If eLoc = 0 Then
        eLoc = Len(data) + 1
    End If
    If eLoc = 0 Then
        GetValueFromString = 0
        Exit Function
    End If
    
    
    val = Mid(data, Loc, eLoc - Loc)
    RetVal = val
End Function

Function RemoveSpaces(Str As String) As String
    Dim temp As String
    temp = Str
    While InStr(1, temp, "  ")
        temp = Replace(temp, "  ", " ")
    Wend
    RemoveSpaces = temp
End Function

Function RemoveTabs(Str As String) As String
    Dim temp As String
    temp = Str
    While InStr(1, temp, vbTab)
        temp = Replace(temp, vbTab, " ")
    Wend
    RemoveTabs = temp
End Function

Function LoadTextFile(ByRef file As String) As String
    Dim temp As String
    Dim line As String
    Open file For Input As #1
    While Not EOF(1)
        Line Input #1, line
        temp = temp & line & vbCrLf
    Wend
    LoadTextFile = temp
    Close #1
End Function

Function GetLine(data As String, line As Long) As String
    Dim v As Variant
    v = Split(data, vbCrLf)
    If line > UBound(v) - 1 Then
        GetLine = ""
        Exit Function
    End If
    GetLine = v(line)
End Function

Function NumLines(data As String) As Long
    Dim Num As Long
    Dim d As String
    d = data
    d = Replace(d, vbCrLf, "")
    Num = (Len(data) - Len(d)) / 2
    NumLines = Num
End Function
