#include "Qualification.h"
#include "Entity.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Qualification CreateQualification(const char** behaviors, 
  const char** properties, Constraint* constraints,
  int numBehaviors, int numProperties, int numConstraints)
{
  Qualification qual;
  qual.behaviors = behaviors;
  qual.properties = properties;
  qual.constraints = constraints;
  qual.numBehaviors = numBehaviors;
  qual.numProperties = numProperties;
  qual.numConstraints = numConstraints;
  return qual;
}
// -----------------------------------------------------------------------------
int IsOfType(const Qualification* qualification, const Entity* entity)
{
  int i;

  for (i = 0; i < qualification->numProperties; ++i)
  {
    if (!EntityHasProperty(entity, qualification->properties[i]))
      return 0;
  }

  for (i = 0; i < qualification->numBehaviors; ++i)
  {
    if (!EntityHasBehavior(entity, qualification->behaviors[i]))
      return 0;
  }

  for (i = 0; i < qualification->numConstraints; ++i)
  {
    if (!(qualification->constraints[i](entity)))
      return 0;
  }

  return 1;
}