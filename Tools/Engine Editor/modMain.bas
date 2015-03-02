Attribute VB_Name = "modMain"
Public SceneNodes() As NodeData

Public PropertiesObject As NodeData

Public Sub CreateNewNode(index As Long)
    SceneNodes(index).numParams = 3
    ReDim SceneNodes(index).Block(2)
    SceneNodes(index).Block(0) = "Name="""""
    SceneNodes(index).Block(1) = "Parent="""""
    SceneNodes(index).Block(2) = "Type="""""
End Sub

Public Sub Main()
    Dim aset As ParsedASEAnimationSet
    ParseASEAnimationSetFromASEFileList "x#C:\Program Files\3DSMAX3\Meshes\stick1.ase#C:\Program Files\3DSMAX3\Meshes\stick2.ase", aset
    SaveASEAnimationSet aset, "c:\set.animation"
    
End Sub
