Attribute VB_Name = "modStringFile"
Function GetValueFromString(data As String, Pattern As String, RetVal As Variant, Optional ID As Long) As Long
    If Not IsMissing(ID) Then
        Pattern = Replace(Pattern, "#", CStr(ID))
    End If
    Dim Loc As Long
    Dim eloc As Long
    Dim val As String
    Loc = InStr(1, data, Pattern)
    If Loc = 0 Then
        GetValueFromString = 0
        Exit Function
    End If
    
    Loc = Loc + Len(Pattern)
    
    eloc = InStr(Loc, data, " ")
    Dim temp As Long
    temp = InStr(Loc, data, vbCrLf)
    If eloc = 0 Or (temp < eloc And temp > 0) Then
        eloc = temp
    End If
    If eloc = 0 Then
        eloc = Len(data) + 1
    End If
    If eloc = 0 Then
        GetValueFromString = 0
        Exit Function
    End If
    
    
    val = Mid$(data, Loc, eloc - Loc)
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

Function LoadTextFile(file As String) As String
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
