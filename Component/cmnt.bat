SET CPP_FLAGS=/c /MTd /Zi /Od /EHsc /D_DEBUG
SET DLL_LINK_FLAGS=/DLL /DEBUG

SET LIBS=UUID.lib Advapi32.lib Ole32.lib

cl %CPP_FLAGS% CFactory.cpp
cl %CPP_FLAGS% CEquationsSolver.cpp
cl %CPP_FLAGS% EquationsDll.cpp
cl %CPP_FLAGS% GUIDs.cpp
cl %CPP_FLAGS% Registry.cpp

link %DLL_LINK_FLAGS% EquationsDll.obj GUIDs.obj Registry.obj CFactory.obj CEquationsSolver.obj %LIBS% /DEF:EquationsDll.def
regsvr32 EquationsDll.dll