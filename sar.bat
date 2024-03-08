REM This will copy the compiled sar binary to the game dir.
REM Make sure sar.props has the correct path.

IF [%1]==[] (
	ECHO "[-] Missing first argument path to the compiled sar binary."
	EXIT /b 1
)

IF [%2]==[] (
	ECHO "[-] Missing second argument path to the game directory."
	EXIT /b 1
)

SET "binary=%~1"
SET "game_bin=%~2\bin\win64"

XCOPY "%binary%" "%game_bin%\" /Y
