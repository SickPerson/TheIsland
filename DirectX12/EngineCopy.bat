xcopy /s /y /d /exclude:exclude_list.txt ".\01.Project\Engine\*.h" ".\03.External\Include\Engine"
xcopy /s /y /d /exclude:exclude_list.txt ".\01.Project\Engine\*.inl" ".\03.External\Include\Engine"
xcopy /s /y /d /exclude:exclude_list.txt ".\01.Project\Engine\*.fx" ".\02.File\bin\content\Shader"



xcopy /s /y /d /exclude:exclude_list.txt ".\01.Project\Client\NaturalScript.h" ".\01.Project\Tool"
xcopy /s /y /d /exclude:exclude_list.txt ".\01.Project\Client\NaturalScript.cpp" ".\01.Project\Tool"
