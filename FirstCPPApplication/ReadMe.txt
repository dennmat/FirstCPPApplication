========================
======Build Mode========
========================
========================

The basic idea behind build mode is to be able to create maps
using the client instead of writing them out by hand. In order
to make this easy, I'm just going to modify the existing client and
add commands that let you change tiles then write the new version
of the current map out to a file. So i'll need:
-some sort of buildmode boolean that lets me through walls
-a "[C]hange tile" command that will let me give all new information to a tile
-a "Wr[i]te map to file" command that writes the map out to a new file
-a "C[o]py and [P]aste" for tiles

