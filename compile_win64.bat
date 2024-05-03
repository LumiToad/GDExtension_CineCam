for /f "tokens=2 delims==" %%a in ('wmic OS Get localdatetime /value') do set "dt=%%a"
set "YY=%dt:~2,2%" & set "YYYY=%dt:~0,4%" & set "MM=%dt:~4,2%" & set "DD=%dt:~6,2%"
set "HH=%dt:~8,2%" & set "Min=%dt:~10,2%" & set "Sec=%dt:~12,2%"

set "datestamp=%YYYY%%MM%%DD%" & set "timestamp=%HH%%Min%%Sec%"
set "fullstamp=%YYYY%-%MM%-%DD%_%HH%-%Min%-%Sec%"

set "PATH=C:\Windows\System32;D:\Program Files\Python312\Scripts;D:\Program Files\Python312;D:\mingw32\bin"

echo WINDOWS > build_logs/windows/build_log_%fullstamp%.txt

echo. >> build_logs/windows/build_log_%fullstamp%.txt

set "PATH=C:\Windows\System32;D:\Program Files\Python312\Scripts;D:\Program Files\Python312;D:\mingw64\bin"

echo ---- >> build_logs/windows/build_log_%fullstamp%.txt
echo. >> build_logs/windows/build_log_%fullstamp%.txt
echo BUILD debug_win64 >> build_logs/windows/build_log_%fullstamp%.txt
scons platform=windows custom_api_file=extension_api.json >> build_logs/windows/build_log_%fullstamp%.txt
echo. >> build_logs/windows/build_log_%fullstamp%.txt
echo BUILD release_win64 >> build_logs/windows/build_log_%fullstamp%.txt
scons platform=windows target=template_release custom_api_file=extension_api.json >> build_logs/windows/build_log_%fullstamp%.txt
echo. >> build_logs/windows/build_log_%fullstamp%.txt

echo COMPLETE >> build_logs/windows/build_log_%fullstamp%.txt