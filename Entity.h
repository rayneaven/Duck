////////////////////////////////////////////////////////////////////////////////
#ifndef ENTITY_H
#define ENTITY_H
////////////////////////////////////////////////////////////////////////////////

//any function pointer type can be cast to any other function pointer type
typedef void (*Behavior)(void);

typedef struct Label
{
  const char* name;
  int byteOffset;
} Label;

typedef struct Entity
{
  char* data;
  Behavior* behaviors;
  Label* propertyLabels;
  Label* behaviorLabels;
  int numProperties;
  int numBehaviors;
  int propertyBytes;
} Entity;

//Functions I want

Entity* CreateEntity(int numBytes);
void AddBehaviorToEntity(Entity* entity, Behavior behavior, const char* label);
void AddPropertyToEntity(Entity* entity, char* data, int size, const char* label);
void RemoveBehaviorFromEntity(Entity* entity, const char* label);
void RemovePropertyFromEntity(Entity* entity, const char* label);
int EntityHasProperty(const Entity* entity, const char* label);
int EntityHasBehavior(const Entity* entity, const char* label);
void* GetPropertyOfEntity(const Entity* entity, const char* label);
Behavior GetBehaviorOfEntity(const Entity* entity, const char* label);

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////