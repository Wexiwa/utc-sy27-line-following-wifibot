' Wizard for the SDK of RTMaps For Wifibot
'
' v1.0 2009/06 OM
'
' (c) 2009 Intempora S.A.

''''''''''' Create required objects '''''''''''''
Set FSO = CreateObject("Scripting.FileSystemObject")

'''''''''''' Common constants ''''''''''''''''''
Const solutionFileName = "RTMaps4Wifibot_sdk_vc2008.sln"
Const targetVC = "2008"
Const mainTitle = "RTMaps4Wifibot SDK Wizard"

Dim scriptPath, rootPath, templatePath, solutionFullPath, userSdkPath
scriptPath = Replace(WScript.ScriptFullName, WScript.ScriptName, "")
rootPath = FSO.GetParentFolderName(scriptPath)
userSdkPath = FSO.BuildPath(rootPath,"src")
solutionFullPath = FSO.BuildPath(rootPath, solutionFileName)
sharePath = FSO.BuildPath(rootPath,"share")

'''''' Check requirements ''''''''''''
'' Interactive mode
If WScript.Interactive = False Then
    QuitWithMessage "Script must be run in interactive mode.",-1
End If
'' Check if solution file is missing
If FSO.FileExists( solutionFullPath ) = false Then
    QuitWithMessage "Solution file ("&solutionFullPath&") is missing. Please check your installation",-1
End If

' Helper function to request an integer choice to the user in a valid range
Function RequestChoice(prompt, minAns, maxAns)
    Dim l_res, l_choice, l_isOK
    l_isOK = false
    Do While l_isOK = False
        l_res = InputBox(prompt,mainTitle)
        If IsNumeric(l_res) = True Then
            l_choice = Int(l_res)
            If l_choice >= minAns And l_choice <= maxAns Then
                l_isOK = true
            End If
        End If
        If l_isOK = False Then
            WScript.Echo "Wrong answer. Try again."
        End If
    Loop
    RequestChoice = l_choice
End Function

' Helper function to quit with a message and an error code
Sub QuitWithMessage(msg, errCode)
    WScript.Echo msg
    WScript.Quit(errCode)
End sub

' Create a new package
Sub CreateNewPackage
    Dim newGUID, newGUID2, packageName, projectFolder, subFolder
    Set TypeLib = CreateObject("Scriptlet.TypeLib")
    newGUID = TypeLib.Guid
    newGUID = Left(newGUID,Len(newGUID)-2)
    newGUID2 = TypeLib.Guid
    newGUID2 = Left(newGUID2,Len(newGUID2)-2)
    Set TypeLib = Nothing
    Set re = New RegExp
    re.Pattern = "^[a-z][a-z_0-9]*$"
    re.IgnoreCase = True
    re.Global = False
    Do While packageName = ""
        packageName = InputBox("Enter a new package name")
        If packageName = "" Then
            WScript.Quit(0)
        End If
        If false = re.Test(packageName) Then
            WScript.Echo "Project name contains invalid characters. Try again."
            packageName = ""
        End If
        projectFolder = FSO.BuildPath(userSdkPath, packageName & ".u")
        if FSO.FolderExists(projectFolder) = true Then
            WScript.Echo packageName & " package already exists. Try again."
            packageName = ""
        End If
    Loop
    Dim projectFolderObj
    Set projectFolderObj = FSO.CreateFolder(projectFolder)
    If projectFolderObj = "" Then
        QuitWithMessage "Failed to create package folder: " & projectFolder,-1
    End If
    Dim subFolders, srcFolder
    Set subFolders = projectFolderObj.SubFolders
    subFolders.Add("local_interfaces")
    Set srcFolder = subFolders.Add("src")
    Dim pckDefCpp, vcprojFileTemplate, vcprojFileTarget
    pckDefCpp = FSO.BuildPath(sharePath,"maps_package_definition.cpp")
    FSO.CopyFile pckDefCpp,srcFolder.Path & "\"
    ' Create the vcproj
    Dim vcprojFileTemplatePath, vcprojFileTargetPath,buffer
    vcprojFileTemplatePath = FSO.BuildPath(sharePath,"rtmaps_template_vc"&targetVC&".vcproj")
    vcprojFileTargetPath = FSO.BuildPath(projectFolderObj.Path,"rtmaps_"&packageName&"_vc"&targetVC&".vcproj")
    Set vcprojFileTemplate = FSO.OpenTextFile(vcprojFileTemplatePath,1)
    buffer = vcprojFileTemplate.ReadAll
    vcprojFileTemplate.Close
    re.Pattern = "\$\$GUID\$\$"
    re.Global = True
    re.IgnoreCase = False
    buffer = re.Replace(buffer,newGUID)
    re.Pattern = "\$\$PckName\$\$"
    buffer = re.Replace(buffer,packageName)
    Set vcprojFileTarget = FSO.CreateTextFile(vcprojFileTargetPath,True)
    vcprojFileTarget.Write(buffer)
    vcprojFileTarget.Close
    ' Add the project to the solution
    Dim solutionFile, tmpFileName, tmpFile, state
    Set solutionFile = FSO.OpenTextFile(solutionFullPath,1)
    tmpFileName = FSO.GetTempName
    Set tmpFile = FSO.CreateTextFile(tmpFileName,True)
    state = 0
    re.Pattern = "^# Visual Studio"
    re.Global = False
    re.IgnoreCase = False
    Do While solutionFile.AtEndOfStream <> True
        buffer = solutionFile.ReadLine
        Select Case state
        Case 0
            tmpFile.WriteLine(buffer)
            If re.Test(buffer) Then
                tmpFile.WriteLine("Project("""&newGUID2&""") = """&_
                         packageName&""", ""src\"&packageName&".u\rtmaps_"&packageName&"_vc"&_
                         targetVC&".vcproj"", """&newGUID&"""")
                tmpFile.WriteLine("EndProject")
                state = 1
                re.Pattern = "ProjectConfigurationPlatforms"
            End If
        Case 1
            tmpFile.WriteLine(buffer)
            If re.Test(buffer) Then
                tmpFile.WriteLine(vbTab&vbTab&newGUID&".Debug|Win32.ActiveCfg = Debug|Win32")
                tmpFile.WriteLine(vbTab&vbTab&newGUID&".Debug|Win32.Build.0 = Debug|Win32")
                tmpFile.WriteLine(vbTab&vbTab&newGUID&".Release|Win32.ActiveCfg = Release|Win32")
                tmpFile.WriteLine(vbTab&vbTab&newGUID&".Release|Win32.Build.0 = Release|Win32")
                state = 2
            End If
        Case Else
            tmpFile.WriteLine(buffer)
        End Select
    Loop
    solutionFile.Close
    tmpFile.Close
    FSO.CopyFile tmpFileName, solutionFullPath
    FSO.DeleteFile tmpFileName
    Set re = Nothing
End Sub


Sub CreateNewComponent
    ' List the available packages
    Dim rootFolders, f, re
    Set re = new RegExp
    re.Global = False
    re.IgnoreCase = True
    re.Pattern = "\.u$"
    Set srcFolders = FSO.GetFolder(userSdkPath).SubFolders
    If srcFolders.Count > 64 Then
        QuitWithMessage "You have exceeded the 64 max packages.", -1
    End If
    Dim myArr(64), idx, idx2
    idx = 0
    For Each f in srcFolders
        If re.Test(f.name) Then
            myArr(idx) = Left(f.name, Len(f.name) - 2)
            idx = idx + 1
        End If
    Next
    If idx = 0 Then
        QuitWithMessage "You must create a package first.", -1
    End If
    ' Select a package
    Dim choice, msg
    msg = "Choose the package to which you want to add a component:" & vbCrLf
    idx2 = 0
    Do While idx2 < idx
        msg = msg & " " & (idx2+1) & " - " & myArr(idx2) & vbCrLf
        idx2 = idx2 + 1
    Loop
    msg = msg & " 0 - Quit"
    choice = RequestChoice(msg,0,idx)
    If choice = 0 Then
        WScript.Quit(0)
    End If
    ' Component name
    Dim moduleName
    re.Pattern = "^[a-z][a-z_0-9]*$"
    re.Global = False
    re.IgnoreCase = True
    Do While moduleName = ""
        moduleName = InputBox("Enter the name of the new component:", mainTitle)
        If moduleName = "" Then
            WScript.Quit(0)
        End If
        If False = re.Test(moduleName) Then
            WScript.Echo "Module name contains invalid characters. Try again."
            moduleName = ""
        End If
    Loop
    Dim projectPath, srcPath, includePath, vcprojFullPath
    projectPath = FSO.BuildPath(userSdkPath, myArr(choice-1) & ".u")
    vcprojFullPath = FSO.BuildPath(projectPath,"rtmaps_"&myArr(choice-1)&"_vc"&targetVC&".vcproj")
    If FSO.FileExists(vcprojFullPath) = False Then
        QuitWithMessage "vcproj is missing. Aborting.",-1
    End If
    ' Create the templates files
    srcPath = FSO.BuildPath(projectPath, "src")
    If FSO.FolderExists(srcPath) = False Then
        QuitWithMessage "Package project is invalid.",-1
    End If
    includePath = FSO.BuildPath(projectPath, "local_interfaces")
    If FSO.FolderExists(includePath) = False Then
        QuitWithMessage "Package project is invalid.",-1
    End If
    Dim srcTemplateFullPath, includeTemplateFullPath,srcDestFullPath,includeDestFullPath
    srcTemplateFullPath = FSO.BuildPath(sharePath,"maps_ComponentTemplate.cpp")
    includeTemplateFullPath = FSO.BuildPath(sharePath,"maps_ComponentTemplate.h")
    srcDestFullPath = FSO.BuildPath(srcPath, "maps_"&moduleName&".cpp")
    includeDestFullPath = FSO.BuildPath(includePath, "maps_"&moduleName&".h")
    If FSO.FileExists(srcDestFullPath) Or FSO.FileExists(includeDestFullPath) Then
        QuitWithMessage "This module already exists.", -1
    End If
    Dim srcTemplateFile, includeTemplateFile,srcDestFile,includeDestFile
    Set srcTemplateFile = FSO.OpenTextFile(srcTemplateFullPath,1)
    Set includeTemplateFile = FSO.OpenTextFile(includeTemplateFullPath,1)
    Set srcDestFile = FSO.CreateTextFile(srcDestFullPath)
    Set includeDestFile = FSO.CreateTextFile(includeDestFullPath)
    Dim buffer
    re.Pattern = "ComponentTemplate"
    re.Global = True
    re.IgnoreCase = False
    Do While srcTemplateFile.AtEndOfStream <> True
        buffer = srcTemplateFile.ReadLine
        buffer = re.Replace(buffer,moduleName)
        srcDestFile.WriteLine(buffer)
    Loop
    srcTemplateFile.Close
    srcDestFile.Close
    Do While includeTemplateFile.AtEndOfStream <> True
        buffer = includeTemplateFile.ReadLine
        buffer = re.Replace(buffer,moduleName)
        includeDestFile.WriteLine(buffer)
    Loop
    includeTemplateFile.Close
    includeDestFile.Close
    ' Add the new sources to the vcproj
    set xmlDoc = CreateObject("Microsoft.XMLDOM")
    xmlDoc.Async = False
    xmlDoc.Load(vcprojFullPath)
    Set objNodeList = xmlDoc.getElementsByTagName("Files")
    If objNodeList.Length <> 1 Then
        QuitWithMessage "Project file is invalid (no Files section)",-1
    End If
    Set filesNode = objNodeList.Item(0) 'IXMLDOMElement
    ' Look for src path
    Dim srcFilterNode, includeFilterNode
    Set srcFilterNode = Nothing
    Set includeFilterNode = Nothing
    Set objNodeList = filesNode.getElementsByTagName("Filter")
    For Each n In objNodeList
        name = n.getAttribute("Name")
        If name = "src" Then
            Set srcFilterNode = n
        ElseIf name = "local_interfaces" Then
            Set includeFilterNode = n
        End If
    Next
    If srcFilterNode Is Nothing Then
        Set srcFilterNode = xmlDoc.createElement("Filter")
        srcFilterNode.setAttribute "Name", "src"
        filesNode.appendChild(srcFilterNode)
    End If
    If includeFilterNode Is Nothing Then
        Set includeFilterNode = xmlDoc.createElement("Filter")
        includeFilterNode.setAttribute "Name", "local_interfaces"
        filesNode.appendChild(includeFilterNode)
    End If

    Set srcElt = xmlDoc.createElement("File")
    srcElt.setAttribute "RelativePath", ".\src\maps_"&moduleName&".cpp"
    Set includeElt = xmlDoc.createElement("File")
    includeElt.setAttribute "RelativePath", ".\local_interfaces\maps_"&moduleName&".h"
    srcFilterNode.appendChild(srcElt)
    includeFilterNode.appendChild(includeElt)

    xmlDoc.save(vcprojFullPath)
    Set xmlDoc = Nothing
    Set re = Nothing
End Sub


'''''''''''''' Main script ''''''''''''''''''
Dim choice
choice = RequestChoice("What do you want to do:" & vbCrLf & _
               " 1 - Create a new package" & vbCrLf & _
               " 2 - Create a new component" & vbCrLf & _
               " 0 - Quit", 0, 2)
'WScript.Echo choice
Select Case choice
    Case 0    WScript.Quit(0)
    Case 1    CreateNewPackage
    Case 2    CreateNewComponent
    Case Else WScript.Quit(-1)
End Select
' Everything went ok
WScript.Quit(0)
