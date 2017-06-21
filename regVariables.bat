reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v MRAY_ENGINE /t REG_EXPAND_SZ /d "C:\Development\mrayEngine"
%reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v GSTREAMER_SDK_ROOT_X86 /t REG_EXPAND_SZ /d "C:\gstreamer\1.0\x86\"
%reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v GST_PLUGIN_PATH /t REG_EXPAND_SZ /d "C:\gstreamer\1.0\x86\lib\gstreamer-0.10"
%set PATH=%PATH%;"C:\gstreamer\1.0\x86\bin"