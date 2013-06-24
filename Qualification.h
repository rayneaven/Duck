////////////////////////////////////////////////////////////////////////////////
#ifndef QUALIFICATION_H
#define QUALIFICATION_H
////////////////////////////////////////////////////////////////////////////////

//forward declaration
struct Entity;

typedef int (*Constraint)(const struct Entity*);

typedef struct Qualification
{
  const char** behaviors;
  const char** properties;
  Constraint* constraints;
  int numBehaviors;
  int numProperties;
  int numConstraints;
} Qualification;

Qualification CreateQualification(const char** behaviors, 
  const char** properties, Constraint* constraints, 
  int numBehaviors, int numProperties, int numConstraints);

//returns 1 if the Entity has the specified behaviors and properties,
//returns 0 otherwise
int IsOfType(const Qualification* qualification, const struct Entity* entity);

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
