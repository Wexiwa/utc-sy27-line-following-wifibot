@echo off

echo Setting environment variables...

set AdditionalIncludeDirectories="../../include";"./local_interfaces";
set AdditionalLibraryDirectories="../../lib";
set AdditionalDependencies=rtmaps_ring_buffers.lib;rtmaps4wifibot_pck.lib;
set OpencvDependencies_=cv210.lib;cvaux210.lib;cxcore210.lib;highgui210.lib;
set OpencvDebugDependencies_=cv210d.lib;cvaux210d.lib;cxcore210d.lib;highgui210d.lib;
set FileType_=.pck
rem set ProjectDir="C:\Users\Maxime\Dropbox\SY27\Projet\Ressources\cd_wifibot_lab_V3\cdlabRS232_V3\robotic_framework\RTMAPS\rtmaps4wifibot_sdk\"
set DebugCommand_="C:\Program Files\Intempora\RTMaps4Wifibot\bin\rtmaps4Wifibot.exe"
REM set COMPUTERNAME="localhost"

echo Sarting RTMaps4Wifibot_sdk_vc2008.sln...

START "" /wait "C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv.exe" RTMaps4Wifibot_sdk_vc2008.sln

echo Closing RTMaps4Wifibot_sdk_vc2008.sln...
