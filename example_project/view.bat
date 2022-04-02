SET script_dir=%~dp0
cd %script_dir%

@REM Launch the viewer to view the model
%PANGU_HOME%\bin\viewer ^
	-noini ^
	-camera 4000 0 90 ^
	-target 0 0 0 ^
	model.pan ^
	%*

