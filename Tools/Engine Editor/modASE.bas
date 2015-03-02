Attribute VB_Name = "modASE"
Option Explicit


Public Type DivVertex
    x As Single
    y As Single
    z As Single
    nx As Single
    ny As Single
    nz As Single
    tu As Single
    tv As Single
End Type

Public Type ColorType
    r As Single
    g As Single
    b As Single
    a As Single
End Type

Public Type Material
    Diffuse As ColorType
    Ambient As ColorType
    Specular As ColorType
    Emissive As ColorType
    Power As Single
End Type

Public Type Vertex
    x As Single
    y As Single
    z As Single
End Type

Public Type face
    a As Integer
    b As Integer
    c As Integer
End Type

Public Type FaceNormalSet
    a As Vertex
    aID As Long
    b As Vertex
    bID As Long
    c As Vertex
    cID As Long
    face As Vertex
    faceID As Long
End Type

Public Type MatRow
    a As Single
    b As Single
    c As Single
    d As Single
End Type

Public Type Matrix
    a As MatRow
    b As MatRow
    c As MatRow
    d As MatRow
End Type

Public Type Bone
    Position As Vertex
    Parent As String
    Name As String
    Transform As Matrix
End Type

Public Type ParsedASESubset
    numFaces As Long
    numVertices As Long
    numIndices As Long
    Vertices() As DivVertex
    Indices() As Integer
    mat As Material
End Type

Public Type ParsedASE
    ID As Long
    numSubsets As Long
    Subsets() As ParsedASESubset
End Type

Public Type ParsedASEAnimation
    AnimName As String * 32
    numFrames As Long
    Frames() As ParsedASE
End Type

Public Type ParsedASEAnimationSet
    ID As Long
    numSets As Long
    Animations() As ParsedASEAnimation
End Type

Public Type ParsedASESkinned
    ID As Long
    numMaterials As Long
    numFaces As Long
    numVertices As Long
    numIndices As Long
    numFrames As Long
    numBones As Long
    Vertices() As DivVertex
    Indices() As Integer
    Materials() As Material
    Bones() As Vertex
    Matrices() As Matrix
End Type

Sub GetBoneFromString(data As String, ID As Long, ret As Bone)
    ret.Name = ""
    Dim pattern As String
    pattern = "*HELPER_CLASS ""Bone"""
    Dim Loc As Long
    Dim eLoc As Long
    Dim Num As Long
    Dim nodeStart As Long
    
    Num = 0
    Loc = 1
    While Num <= ID
        Loc = InStr(Loc + 1, data, pattern)
        If Loc = 0 Then
            Exit Sub
        End If
        Num = Num + 1
    Wend
    
    pattern = "*NODE_NAME """
    nodeStart = InStrRev(data, "{", Loc)
    Loc = InStr(nodeStart, data, pattern)
    eLoc = InStr(Loc + Len(pattern), data, """")
    ret.Name = Mid$(data, Loc + Len(pattern), eLoc - (Loc + Len(pattern)))
    Loc = InStr(Loc, data, "*NODE_PARENT """) ' get node parent but
    eLoc = InStr(nodeStart, data, "}")                ' make sure its in the
                                                  ' current block
    If Loc < eLoc Then
        pattern = "*NODE_PARENT """
        eLoc = InStr(Loc + Len(pattern), data, """")
        ret.Parent = Mid$(data, Loc + Len(pattern), eLoc - (Loc + Len(pattern)))
    Else
        ret.Parent = ""
    End If
    
    nodeStart = InStr(nodeStart + 1, data, "{")
    Loc = InStr(nodeStart, data, "*TM_ROW0 ")
    eLoc = InStr(Loc, data, vbCrLf)
    Dim val As String
    val = Mid(data, Loc, eLoc - Loc)
    Dim vals As Variant
    vals = Split(val)
    ret.Transform.a.a = CSng(vals(1))
    ret.Transform.a.b = CSng(vals(2))
    ret.Transform.a.c = CSng(vals(3))
    ret.Transform.a.d = 0
    Loc = InStr(nodeStart, data, "*TM_ROW1 ")
    eLoc = InStr(Loc, data, vbCrLf)

    val = Mid(data, Loc, eLoc - Loc)

    vals = Split(val)
    ret.Transform.b.a = CSng(vals(1))
    ret.Transform.b.b = CSng(vals(2))
    ret.Transform.b.c = CSng(vals(3))
    ret.Transform.b.d = 0
    Loc = InStr(nodeStart, data, "*TM_ROW2 ")
    eLoc = InStr(Loc, data, vbCrLf)
    
    val = Mid(data, Loc, eLoc - Loc)
    
    vals = Split(val)
    ret.Transform.c.a = CSng(vals(1))
    ret.Transform.c.b = CSng(vals(2))
    ret.Transform.c.c = CSng(vals(3))
    ret.Transform.c.d = 0
    
    Loc = InStr(nodeStart, data, "*TM_ROW3 ")
    eLoc = InStr(Loc, data, vbCrLf)
    
    val = Mid(data, Loc, eLoc - Loc)
    
    vals = Split(val)
    
    ret.Transform.d.a = CSng(vals(1))
    ret.Transform.d.b = CSng(vals(2))
    ret.Transform.d.c = CSng(vals(3))
    ret.Transform.d.d = 1
    
    ret.Position.x = ret.Transform.d.a
    ret.Position.y = ret.Transform.d.b
    ret.Position.z = ret.Transform.d.c
End Sub

Sub GetVertexFromString(data As String, pattern As String, ID As Long, ret As Vertex)
    pattern = Replace(pattern, "#", CStr(ID))
    Dim Loc As Long
    Dim l As Long
    Dim temp As Long
    Dim temp2 As Long
    Dim eLoc As Long
    Loc = InStr(1, data, pattern)
    If Loc = 0 Then
        Exit Sub
    End If
    Loc = Loc + Len(pattern)
    l = 3
    While l > 0
        temp2 = InStr(Loc + temp + 1, data, vbCrLf)
        eLoc = InStr(Loc + temp + 1, data, " ")
        If temp2 < eLoc Then eLoc = temp2
        temp = eLoc - Loc
        l = l - 1
    Wend
    Dim val As String
    val = Mid$(data, Loc, eLoc - Loc)
    Dim vals As Variant
    vals = Split(val)
    ret.x = CSng(vals(0))
    ret.z = CSng(vals(1))
    ret.y = CSng(vals(2))
    
End Sub

Sub GetFaceFromString(data As String, pattern As String, ID As Long, ret As face)
    pattern = Replace(pattern, "#", CStr(ID))
    Dim Loc As Long
    Dim l As Long
    Dim temp As Long
    Dim temp2 As Long
    Dim eLoc As Long
    Loc = InStr(1, data, pattern)
    If Loc = 0 Then
        Exit Sub
    End If
    Loc = Loc + Len(pattern)
    l = 6
    While l > 0
        temp2 = InStr(Loc + temp + 1, data, vbCrLf)
        eLoc = InStr(Loc + temp + 1, data, " ")
        If temp2 < eLoc Then eLoc = temp2
        temp = eLoc - Loc
        l = l - 1
    Wend
    Dim val As String
    val = Mid$(data, Loc, eLoc - Loc)
    Dim vals As Variant
    val = Trim(val)
    vals = Split(val)
    ret.a = CInt(vals(1))
    ret.b = CInt(vals(3))
    ret.c = CInt(vals(5))
    
End Sub

Sub GetFaceNormalsFromString(data As String, ID As Long, ret As FaceNormalSet)
    Dim pattern As String
    pattern = "*MESH_FACENORMAL " & CStr(ID)
    Dim Loc As Long
    Dim eLoc As Long
    Loc = InStr(1, data, pattern)
    eLoc = InStr(Loc, data, vbCrLf)
    If eLoc = 0 Then eLoc = Len(data)
    Loc = Loc + Len(pattern) + 1
    Dim val As String
    val = Mid$(data, Loc, eLoc - Loc)
    Dim vals As Variant
    vals = Split(val)
    ret.faceID = ID
    ret.face.x = CSng(vals(0))
    ret.face.z = CSng(vals(1))
    ret.face.y = CSng(vals(2))
    
    Loc = eLoc + 2 + Len("*MESH_VERTEXNORMAL ")
    eLoc = InStr(Loc, data, vbCrLf)
    val = Mid$(data, Loc, eLoc - Loc)
    val = Trim(val)
    vals = Split(val)
    ret.aID = CLng(vals(0))
    ret.a.x = CSng(vals(1))
    ret.a.z = CSng(vals(2))
    ret.a.y = CSng(vals(3))
    
    Loc = eLoc + 2 + Len("*MESH_VERTEXNORMAL ")
    eLoc = InStr(Loc, data, vbCrLf)
    val = Mid$(data, Loc, eLoc - Loc)
    val = Trim(val)
    vals = Split(val)
    ret.bID = CLng(vals(0))
    ret.b.x = CSng(vals(1))
    ret.b.z = CSng(vals(2))
    ret.b.y = CSng(vals(3))
    
    Loc = eLoc + 2 + Len("*MESH_VERTEXNORMAL ")
    eLoc = InStr(Loc, data, vbCrLf)
    val = Mid$(data, Loc, eLoc - Loc)
    val = Trim(val)
    vals = Split(val)
    ret.cID = CLng(vals(0))
    ret.c.x = CSng(vals(1))
    ret.c.z = CSng(vals(2))
    ret.c.y = CSng(vals(3))
    
End Sub

Sub ProcessVertexNormals(FaceNormals() As FaceNormalSet, numFaces As Long, vertexID As Long, ret As DivVertex)
    Dim i As Long
    Dim vTemp As DivVertex
    Dim v As Vertex
    Dim c As Long
    Dim p As Boolean
    For i = 0 To numFaces - 1
        p = False
        If FaceNormals(i).aID = vertexID Then
            p = True
            vTemp.nx = FaceNormals(i).a.x
            vTemp.ny = FaceNormals(i).a.y
            vTemp.nz = FaceNormals(i).a.z
        End If
        
        If FaceNormals(i).bID = vertexID Then
            p = True
            vTemp.nx = FaceNormals(i).b.x
            vTemp.ny = FaceNormals(i).b.y
            vTemp.nz = FaceNormals(i).b.z
        End If
        
        If FaceNormals(i).cID = vertexID Then
            p = True
            vTemp.nx = FaceNormals(i).c.x
            vTemp.ny = FaceNormals(i).c.y
            vTemp.nz = FaceNormals(i).c.z
        End If
        
        If p = True Then
            Exit For
        End If
    Next i
    
    ret.nx = vTemp.nx
    ret.ny = vTemp.ny
    ret.nz = vTemp.nz
    
End Sub

Function GenerateAnimationFromASEList(list As String, ByRef tmesh As ParsedASEAnimation) As Long
    Dim files As Variant
    files = Split(list, "#")
    
    'Dim mesh As ParsedASE
    
    Dim i As Long
    'Do
    '    GetBoneFromString Text1.Text, i, b
    '    i = i + 1
    'Loop While b.Name <> ""
    Dim data As String
    Dim mtext As String
    Dim k As Long
    tmesh.numFrames = UBound(files)
    ReDim Preserve tmesh.Frames(tmesh.numFrames - 1)
    tmesh.AnimName = files(0)
    For k = 0 To tmesh.numFrames - 1
        mtext = LoadTextFile(CStr(files(k + 1)))
        mtext = RemoveTabs(mtext)
        mtext = RemoveSpaces(mtext)
        i = 0
        tmesh.Frames(k).ID = &HB83E9C38
        Do
            data = GetASESubsetFromString(mtext, i)
            If data = "" Then
                Exit Do
            End If
            i = i + 1
            ReDim Preserve tmesh.Frames(k).Subsets(i - 1)
            ParseASEFromString data, tmesh.Frames(k).Subsets(i - 1)
        Loop
        tmesh.Frames(k).numSubsets = i
        
    Next k

End Function

Function ParseASEAnimationSetFromASEFileList(list As String, ByRef aset As ParsedASEAnimationSet)
    Dim anims As Variant
    anims = Split(list, "|")
    Dim files As Variant
    Dim x As Long
    x = UBound(anims) + 1
    Dim z As Long
    ReDim aset.Animations(x)
    aset.numSets = x
    For z = 0 To x - 1
        GenerateAnimationFromASEList CStr(anims(z)), aset.Animations(z)
    Next z
    aset.ID = &HC80AF461
End Function

Function SaveParsedASEToFile(ByRef mesh As ParsedASE, filenum As Long)
    
    Dim i As Long
    Put filenum, , mesh.numSubsets
    Dim j As Long
    For j = 0 To mesh.numSubsets - 1

        Put filenum, , mesh.Subsets(j).numVertices
        Put filenum, , mesh.Subsets(j).numIndices
        Put filenum, , mesh.Subsets(j).numFaces
        
        For i = 0 To mesh.Subsets(j).numVertices - 1
            Put filenum, , mesh.Subsets(j).Vertices(i).x
            Put filenum, , mesh.Subsets(j).Vertices(i).y
            Put filenum, , mesh.Subsets(j).Vertices(i).z
            Put filenum, , mesh.Subsets(j).Vertices(i).nx
            Put filenum, , mesh.Subsets(j).Vertices(i).ny
            Put filenum, , mesh.Subsets(j).Vertices(i).nz
            Put filenum, , mesh.Subsets(j).Vertices(i).tu
            Put filenum, , mesh.Subsets(j).Vertices(i).tv
        Next i
        
        For i = 0 To mesh.Subsets(j).numIndices - 1
            Put filenum, , mesh.Subsets(j).Indices(i)
        Next i
        
        Put filenum, , mesh.Subsets(j).mat
    Next j

End Function


Function SaveASEAnimationToFile(ByRef anim As ParsedASEAnimation, filenum As Long)
    Put 1, , anim.AnimName
    Put 1, , anim.numFrames
    Dim i As Long
    For i = 0 To anim.numFrames - 1
        SaveParsedASEToFile anim.Frames(i), filenum
    Next i
End Function


Function SaveASEAnimationSet(ByRef aset As ParsedASEAnimationSet, filename As String)
    Dim file As Long
    Open filename For Binary As #1
    Put 1, , aset.ID
    Put 1, , aset.numSets
    Dim i As Long
    For i = 0 To aset.numSets - 1
        SaveASEAnimationToFile aset.Animations(i), 1
    Next i
    Close 1
End Function

Function ParseASEFromString(data As String, ByRef mesh As ParsedASESubset) As Long
    Dim val As String
    Dim FaceNormals() As FaceNormalSet
    '// temporary stuff
    'mesh.numMaterials = 1
    Dim DontDoTextures As Boolean
    mesh.mat.Ambient.a = 1
    mesh.mat.Ambient.r = 1
    mesh.mat.Ambient.g = 1
    mesh.mat.Ambient.b = 1
    
    mesh.mat.Diffuse.a = 1
    mesh.mat.Diffuse.r = 1
    mesh.mat.Diffuse.g = 1
    mesh.mat.Diffuse.b = 1
    
    'mesh.ID = &HB83E9C38
    
    '// end temporary stuff
    
    GetValueFromString data, "*MESH_NUMVERTEX ", val
    mesh.numVertices = CLng(val)
    GetValueFromString data, "*MESH_NUMFACES ", val
    mesh.numFaces = CLng(val)
    GetValueFromString data, "*MESH_NUMTVERTEX ", val
    If mesh.numVertices <> CLng(val) Then
        DontDoTextures = True
    End If
    'mesh.numFaces = CLng(val)
    ReDim FaceNormals(mesh.numFaces)
    mesh.numIndices = mesh.numFaces * 3
    ReDim mesh.Vertices(mesh.numVertices)
    ReDim mesh.Indices(mesh.numIndices)
    Dim vTemp As Vertex
    Dim fTemp As face
    Dim i As Long
    For i = 0 To mesh.numVertices - 1
        GetVertexFromString data, "*MESH_VERTEX # ", i, vTemp
        mesh.Vertices(i).x = vTemp.x
        mesh.Vertices(i).y = vTemp.y
        mesh.Vertices(i).z = vTemp.z
        'If DontDoTextures = False Then
            GetVertexFromString data, "*MESH_TVERT # ", i, vTemp
            vTemp.x = Abs(vTemp.x)
            vTemp.y = Abs(vTemp.z)
            mesh.Vertices(i).tu = vTemp.x
            mesh.Vertices(i).tv = vTemp.y
        'End If
    Next i
    
    For i = 0 To mesh.numFaces - 1
        GetFaceFromString data, "*MESH_FACE #:", i, fTemp
        GetFaceNormalsFromString data, i, FaceNormals(i)
        mesh.Indices(i * 3) = fTemp.a
        mesh.Indices(i * 3 + 1) = fTemp.c
        mesh.Indices(i * 3 + 2) = fTemp.b
        
    Next i
    
    For i = 0 To mesh.numVertices - 1
        ProcessVertexNormals FaceNormals, mesh.numFaces, i, mesh.Vertices(i)
    Next i
    
End Function

Public Function GetASESubsetFromString(data As String, ByVal subSet As Long) As String
    Dim start As Long
    Dim numCurlys As Long
    While subSet + 1
        start = InStr(start + 1, data, "*GEOMOBJECT")
        subSet = subSet - 1
    Wend
    If start = 0 Then
        GetASESubsetFromString = ""
        Exit Function
    End If
    start = start + Len("*GEOMOBJECT ")
    Dim s As String
    Dim i As Long
    For i = start To Len(data)
        s = Mid$(data, i, 1)
        If s = "{" Then
            numCurlys = numCurlys + 1
        End If
        If s = "}" Then
            numCurlys = numCurlys - 1
        End If
        If numCurlys = 0 Then
            i = i + 1
            Exit For
        End If
    Next i
    
    GetASESubsetFromString = Mid(data, start + 3, i - start - 3)
    Open "C:\x.txt" For Output As #1
    Print #1, GetASESubsetFromString
    Close #1
End Function

Sub SaveParsedASE(file As String, mesh As ParsedASE)
    On Error Resume Next
    Dim i As Long
    Kill file
    Open file For Binary As #1
    Put #1, , mesh.ID
    Put #1, , mesh.numSubsets
    Dim j As Long
    For j = 0 To mesh.numSubsets

        Put #1, , mesh.Subsets(j).numVertices
        Put #1, , mesh.Subsets(j).numIndices
        Put #1, , mesh.Subsets(j).numFaces
        
        For i = 0 To mesh.Subsets(j).numVertices - 1
            Put #1, , mesh.Subsets(j).Vertices(i).x
            Put #1, , mesh.Subsets(j).Vertices(i).y
            Put #1, , mesh.Subsets(j).Vertices(i).z
            Put #1, , mesh.Subsets(j).Vertices(i).nx
            Put #1, , mesh.Subsets(j).Vertices(i).ny
            Put #1, , mesh.Subsets(j).Vertices(i).nz
            Put #1, , mesh.Subsets(j).Vertices(i).tu
            Put #1, , mesh.Subsets(j).Vertices(i).tv
        Next i
        
        For i = 0 To mesh.Subsets(j).numIndices - 1
            Put #1, , mesh.Subsets(j).Indices(i)
        Next i
        
        Put #1, , mesh.Subsets(j).mat
    Next j
    Close #1
End Sub

Sub LoadParsedASE(file As String, mesh As ParsedASE)
'    Dim i As Long
'    Open file For Binary As #1
'    Get #1, , mesh.ID
'    Get #1, , mesh.numVertices
'    Get #1, , mesh.numIndices
'    Get #1, , mesh.numFaces
'    ReDim mesh.Vertices(mesh.numVertices)
'    ReDim mesh.Indices(mesh.numIndices)
'    ReDim mesh.Materials(0)
'    For i = 0 To mesh.numVertices - 1
'        Get #1, , mesh.Vertices(i).x
'        Get #1, , mesh.Vertices(i).y
'        Get #1, , mesh.Vertices(i).z
'        Get #1, , mesh.Vertices(i).nx
'        Get #1, , mesh.Vertices(i).ny
'        Get #1, , mesh.Vertices(i).nz
'        Get #1, , mesh.Vertices(i).tu
'        Get #1, , mesh.Vertices(i).tv
'    Next i
'
'    For i = 0 To mesh.numIndices - 1
'        Get #1, , mesh.Indices(i)
'    Next i
'
'    Get #1, , mesh.Materials(0)
'
'    Close #1
End Sub

Function ParseSkinnedASEFromString(data As String, skinmesh As ParsedASESkinned)
    Dim val As String
    Dim FaceNormals() As FaceNormalSet
    '// temporary stuff
    mesh.numMaterials = 1
    ReDim mesh.Materials(0)
    mesh.Materials(0).Ambient.a = 1
    mesh.Materials(0).Ambient.r = 1
    mesh.Materials(0).Ambient.g = 1
    mesh.Materials(0).Ambient.b = 1
    
    mesh.Materials(0).Diffuse.a = 1
    mesh.Materials(0).Diffuse.r = 1
    mesh.Materials(0).Diffuse.g = 1
    mesh.Materials(0).Diffuse.b = 1
    
    mesh.ID = &HE29A7735
    
    '// end temporary stuff
    
    GetValueFromString data, "*MESH_NUMVERTEX ", val
    mesh.numVertices = CLng(val)
    GetValueFromString data, "*MESH_NUMFACES ", val
    mesh.numFaces = CLng(val)
    ReDim FaceNormals(mesh.numFaces)
    mesh.numIndices = mesh.numFaces * 3
    ReDim mesh.Vertices(mesh.numVertices)
    ReDim mesh.Indices(mesh.numIndices)
    Dim vTemp As Vertex
    Dim fTemp As face
    Dim i As Long
    For i = 0 To mesh.numVertices - 1
        GetVertexFromString data, "*MESH_VERTEX # ", i, vTemp
        mesh.Vertices(i).x = vTemp.x
        mesh.Vertices(i).y = vTemp.y
        mesh.Vertices(i).z = vTemp.z
    Next i
    
    For i = 0 To mesh.numFaces - 1
        GetFaceFromString data, "*MESH_FACE #:", i, fTemp
        GetFaceNormalsFromString data, i, FaceNormals(i)
        mesh.Indices(i * 3) = fTemp.a
        mesh.Indices(i * 3 + 1) = fTemp.c
        mesh.Indices(i * 3 + 2) = fTemp.b
        
    Next i
    
    For i = 0 To mesh.numVertices - 1
        ProcessVertexNormals FaceNormals, mesh.numFaces, i, mesh.Vertices(i)
    Next i
    
End Function
