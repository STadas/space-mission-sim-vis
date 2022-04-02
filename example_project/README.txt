This example was designed to demonstrate how to create a synthetic,
multi-resolution lunar surface with added craters, boulders and albedo map.

A fractal surface is first generated and used as a base DEM. Crater and
boulder size-frequency distributions are generated from power-law functions
and used to generate crater and boulder lists.

A synthetic albedo map is generated and added to the model.

A region descriptor is defined to specify two regions of higher resolution
and a .pan file is then created and viewed.

Features included in this example:
- synthetic DEM generation
- synthetic albedo map generation
- crater size-frequency distribution creation
- boulder size-frequency distribution creation
- generating a crater list
- generating a boulder list
- adding craters to a DEM based model
- adding boulders to a DEM based model
- adding regions of higher resolution to a DEM-based model.
- adding an albedo map to a DEM model
- PAN file creation
- launching the viewer

Scripts/batch files:
- clean:       delete all generated files
- make_model:  generate model
- runme:       generate and view model
- view:        view model
