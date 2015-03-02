Attribute VB_Name = "modMutex"
Public Declare Function CreateMutex Lib "kernel32" Alias "CreateMutexA" (lpMutexAttributes As Any, ByVal bInitialOwner As Long, ByVal lpName As String) As Long
Public Declare Function WaitForSingleObject Lib "kernel32" (ByVal hHandle As Long, ByVal dwMilliseconds As Long) As Long
Public Declare Function ReleaseMutex Lib "kernel32" (ByVal hMutex As Long) As Long
Public Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long
Public Declare Function OpenMutex Lib "kernel32" Alias "OpenMutexA" (ByVal dwDesiredAccess As Long, ByVal bInheritHandle As Long, ByVal lpName As String) As Long

Public Const INFINITE = &HFFFF      '  Infinite timeout
Public Const SYNCHRONIZE = &H100000


