(Project of a) library to bring MVVM to WxWidgets, easy to bind to other languages.


# Naming convention
## Pointer ownership

* Every method ending in 'new' and returning a pointer will allocate memory for the
  returned object, and transfer ownership to the caller, leaving it the responsibilty
  of releasing it, generally through the associated _free method.

```
jg_class* jg_class_new();
void jg_class_free(jg_class* class_);
```
 * Method ending in '_init' will initialize an object on the stack, and the caller
   must generally clean resources allocated by the object itself through the matching
   '_cleanup' method

# Private struct, private fields.

Structs that have to be allocatable on the stack, but must be accessed through functions
should be declared as such.

# Documentation
 * You can ommit documentation for the first parameter of a function, if it's clear
   that the function is a 'method' associated with a given struct (TODO: make this
   more clear)
