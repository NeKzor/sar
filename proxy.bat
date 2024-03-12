REM This will copy the compiled proxy binary to the game bin.
REM Make sure sar.props has the correct path.

IF [%1]==[] (
	ECHO "[-] Missing first argument path to the compiled proxy binary."
	EXIT /b 1
)

IF [%2]==[] (
	ECHO "[-] Missing second argument path to the game directory."
	EXIT /b 1
)

SET "binary=%~1"
SET "game_bin=%~2\bin\win64"

REM Rename the original module so it still exists before overwriting it.
IF NOT EXIST "%game_bin%\scenefilecache_original.dll" (
	REN "%game_bin%\scenefilecache.dll" "scenefilecache_original.dll"

	IF errorlevel 1 (
		ECHO "[-] Failed to rename original scenefilecache.dll."
		EXIT /b 1
	)
)

XCOPY "%binary%" "%game_bin%\" /Y
