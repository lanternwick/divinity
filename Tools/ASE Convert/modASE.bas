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

Public Type ParsedASE
    ID As Long
    numMaterials As Long
    numFaces As Long
    numVertices As Long
    numIndices As Long
    Vertices() As DivVertex
    Indices() As Integer
    Materials() As Material
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
    Dim Pattern As String
    Pattern = "*HELPER_CLASS ""Bone"""
    Dim Loc As Long
    Dim eLoc As Long
    Dim num As Long
    Dim nodeStart As Long
    
    num = 0
    Loc = 1
    While num <= ID
        Loc = InStr(Loc + 1, data, Pattern)
        If Loc = 0 Then
            Exit Sub
        End If
        num = num + 1
    Wend
    
    Pattern = "*NODE_NAME """
    nodeStart = InStrRev(data, "{", Loc)
    Loc = InStr(nodeStart, data, Pattern)
    eLoc = InStr(Loc + Len(Pattern), data, """")
    ret.Name = Mid$(data, Loc + Len(Pattern), eLoc - (Loc + Len(Pattern)))
    Loc = InStr(Loc, data, "*NODE_PARENT """) ' get node parent but
    eLoc = InStr(nodeStart, data, "}")                ' make sure its in the
                                                  ' current block
    If Loc < eLoc Then
        Pattern = "*NODE_PARENT """
        eLoc = InStr(Loc + Len(Pattern), data, """")
        ret.Parent = Mid$(data, Loc + Len(Pattern), eLoc - (Loc + Len(Pattern)))
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

Sub GetVertexFromString(data As String, Pattern As String, ID As Long, ret As Vertex)
    Pattern = Replace(Pattern, "#", CStr(ID))
    Dim Loc As Long
    Dim l As Long
    Dim temp As Long
    Dim temp2 As Long
    Dim eLoc As Long
    Loc = InStr(1, data, Pattern)
    If Loc = 0 Then
        Exit Sub
    End If
    Loc = Loc + Len(Pattern)
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
    ret.y = CSng(vals(1))
    ret.z = CSng(vals(2))
    
End Sub

Sub GetFaceFromString(data As String, Pattern As String, ID As Long, ret As face)
    Pattern = Replace(Pattern, "#", CStr(ID))
    Dim Loc As Long
    Dim l As Long
    Dim temp As Long
    Dim temp2 As Long
    Dim eLoc As Long
    Loc = InStr(1, data, Pattern)
    If Loc = 0 Then
        Exit Sub
    End If
    Loc = Loc + Len(Pattern)
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
    Dim Pattern As String
    Pattern = "*MESH_FACENORMAL " & CStr(ID)
    Dim Loc As Long
    Dim eLoc As Long
    Loc = InStr(1, data, Pattern)
    eLoc = InStr(Loc, data, vbCrLf)
    Loc = Loc + Len(Pattern) + 1
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

Function ParseASEFromString(data As String, mesh As ParsedASE) As Long
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
    
    mesh.ID = &HB83E9C38
    
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

Sub SaveParsedASE(file As String, mesh As ParsedASE)
    On Error Resume Next
    Dim i As Long
    Kill file
    Open file For Binary As #1
    Put #1, , mesh.ID
    Put #1, , mesh.numVertices
    Put #1, , mesh.numIndices
    Put #1, , mesh.numFaces
    
    For i = 0 To mesh.numVertices - 1
        Put #1, , mesh.Vertices(i).x
        Put #1, , mesh.Vertices(i).z  ' Note: Swapping y and z values.
        Put #1, , mesh.Vertices(i).y  '  (0, 0, 1) is up in 3dsmax
        Put #1, , mesh.Vertices(i).nx '  (0, 1, 0) is my up vector
        Put #1, , mesh.Vertices(i).ny
        Put #1, , mesh.Vertices(i).nz
        Debug.Print "(" & mesh.Vertices(i).nx & ", " & mesh.Vertices(i).ny & ", " & mesh.Vertices(i).nz & ")"
        Debug.Print Sqr(mesh.Vertices(i).nx ^ 2 + mesh.Vertices(i).ny ^ 2 + mesh.Vertices(i).nz ^ 2)
        Put #1, , mesh.Vertices(i).tu
        Put #1, , mesh.Vertices(i).tv
    Next i
    
    For i = 0 To mesh.numIndices - 1
        Put #1, , mesh.Indices(i)
    Next i
    
    Put #1, , mesh.Materials(0)
    
    Close #1
End Sub

