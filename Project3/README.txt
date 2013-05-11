Brian - May 11,2013
Changelog:
- Changed cacheBlock to a single array. A double array seems unneccessary for now 
  since each slot is represented by an integer as opposed to a bitstring.
- Direct-mapped caching seems to work, however some values are hardcoded for the 
  current testcase. I labeled each of these cases with 'TODO'
- Added a printCache function to that will print the contents of the cache.