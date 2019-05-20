TwentyOne Source Notes

-------------------------------------------------------------------------------------

NOTE: This code relies on the QCardU source available through the QCard2 link.

-------------------------------------------------------------------------------------

- There is a fairly nasty memory leak someplace in the program.
  (drains heavy after a couple of hands, ~2K every two or three hands)
  Hopefully this will be found and fixed.
  2001-05-31: Thought memory leak was fixed - it wasn't.
  2001-06-02: Still have memory leak (from 4 bytes to 2K per hand, clueless as
              to where it is right now).  It appears options dialog drains about
              16 bytes and about drains 4 bytes.  Why? 
- Rules the game plays by is in the main window comments.
- Source code is commented (I think fairly well :)).
- Used Sibyl 2.0 with FP4 to create and compile (though FP3 should work also).
- Did not create win32 objects for this program.

Note: Removed all forms (except the main) from the auto-create list in the projects
 settings dialog (none should have been there anyway).  2001-06-19.

M.G.S. (slack@attglobal.net)