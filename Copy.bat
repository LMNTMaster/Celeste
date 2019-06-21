cd..
cd..

xcopy .\LMNTEngine\Include\*.h .\Engine\Include\ /e /y /d
xcopy .\LMNTEngine\Bin\*.* .\Engine\Bin\ /e /y /d /exclude:Exclude.txt
xcopy .\LMNTEngine\Bin\*.* .\LMNTClient\Bin\ /e /y /d /exclude:Exclude.txt
xcopy .\LMNTEngine\Bin\*.* .\LMNTTool\Bin\ /e /y /d /exclude:Exclude.txt