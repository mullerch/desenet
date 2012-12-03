
malloc / free using critical functions

This is a copy of the malloc supported by red-hat's newlib. It has been taken
out of the newlib (libc) to add protection arround malloc and free to be
able to use it in ISRs (interrupt service routines).

- When using this package you need to define following symbols for the 
  compilation process, when building the project:
  - INTERNAL_NEWLIB
  - DEFINE_MALLOC
  - DEFINE_FREE
  - DEFINE_REALLOC
  - DEFINE_CALLOC

The libc implementation of malloc and free will be overriden by the 
implementation given in the project.