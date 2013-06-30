#include "Specification.h"
#include "Entity.h"
#include <stddef.h>
#include <stdio.h>

Specification CreateSpec(const Entity* prototype, Constructor ctor,
  Destructor dtor)
{
  Specification spec;
  spec.prototype = prototype;
  spec.constructor = ctor;
  spec.destructor = dtor;
  return spec;
}
// -----------------------------------------------------------------------------
void AddSpecToEntity(Entity* dest, const Specification* spec)
{
  int i;

  //if there's a specialized way the user wants to set up the data,
  //let him do that
  if (spec->constructor != NULL)
  {
    spec->constructor(dest, spec->prototype);
    return;
  }

  //otherwise this is the default
  for (i = 0; i < spec->prototype->numProperties; ++i)
  {    
    const char* label = spec->prototype->propertyLabels[i].name;
    AddPropertyToEntity(
      dest, 
      (char*)GetPropertyOfEntity(spec->prototype, label),
      GetSizeOfProperty(spec->prototype, label),
      label);
  }

  for (i = 0; i < spec->prototype->numBehaviors; ++i)
  {
    const char* label = spec->prototype->behaviorLabels[i].name;
    AddBehaviorToEntity(dest, GetBehaviorOfEntity(spec->prototype, label), 
      label);
  }
}
// -----------------------------------------------------------------------------
void RemoveSpecFromEntity(Entity* dest, const Specification* spec)
{
  int i;

  //if there's a specialized way the user wants to delete the data,
  //let him do that
  if (spec->destructor != NULL)
  {
    spec->destructor(dest, spec->prototype);
    return;
  }

  //otherwise this is the default
  for (i = 0; i < spec->prototype->numProperties; ++i)
  {
    const char* label = spec->prototype->propertyLabels[i].name;
    RemovePropertyFromEntity(dest, label);
  }

  for (i = 0; i < spec->prototype->numBehaviors; ++i)
  {
    const char* label = spec->prototype->behaviorLabels[i].name;
    RemoveBehaviorFromEntity(dest, label);
  }
}