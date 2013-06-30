////////////////////////////////////////////////////////////////////////////////
#ifndef SPECIFICATION_H
#define SPECIFICATION_H
////////////////////////////////////////////////////////////////////////////////

struct Entity;

typedef void (*Constructor)(struct Entity*, const struct Entity*);
typedef void (*Destructor)(struct Entity*, const struct Entity*);

typedef struct Specification
{
  const struct Entity* prototype;
  Constructor constructor;
  Destructor destructor;
} Specification;

Specification CreateSpec(const struct Entity* prototype, Constructor ctor,
  Destructor dtor);
void AddSpecToEntity(struct Entity* dest, const Specification* spec);
void RemoveSpecFromEntity(struct Entity* dest, const Specification* spec);

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
