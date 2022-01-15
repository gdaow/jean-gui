#ifndef _ULG_CLASS_H_
#define _ULG_CLASS_H_

typedef struct UlgObject UlgObject;
typedef struct UlgClass UlgClass;

typedef void* (*UlgConstructor)();
typedef void (*UlgDestructor)(void*);

UlgClass* ulg_class_create(const char* name, UlgConstructor constructor, UlgDestructor destructor);

#endif
