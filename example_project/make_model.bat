@REM Generate a new, artificial fractal DEM using Perlin noise
%PANGU_HOME%\bin\surfacemodeller -fractal create_surface.txt

@REM Generate an albedo map the same size as the base DEM
%PANGU_HOME%\bin\surfacemodeller -albedo create_albedomap.txt

@REM Generate a crater size frequency distribution graph from a power law
%PANGU_HOME%\bin\surfacemodeller -distribution create_crater_size_freq.txt

@REM Generate a list a of craters from the distribution and the region descriptor
%PANGU_HOME%\bin\surfacemodeller -crater create_craters.txt

@REM Generate a boulder size frequency distribution graph from a power law
%PANGU_HOME%\bin\surfacemodeller -distribution create_boulder_size_freq_dist.txt 

@REM Generate a list a of boulders from the distribution and the region descriptor
%PANGU_HOME%\bin\surfacemodeller -boulder create_boulders.txt

@REM Create a .pan file model
%PANGU_HOME%\bin\surfacemodeller -model model.txt

