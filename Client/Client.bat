SET CPP_FLAGS=/c /MTd /Zi /Od /EHsc /D_DEBUG
SET EXE_LINK_FLAGS=/DEBUG

SET LIBS=UUID.lib Advapi32.lib Ole32.lib

cl %CPP_FLAGS% Mainprg.cpp	
cl %CPP_FLAGS% GUIDs.cpp

link %EXE_LINK_FLAGS% Mainprg.obj GUIDs.obj %LIBS%