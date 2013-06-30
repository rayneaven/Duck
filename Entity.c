#include "Entity.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Makes a new, empty entity
Entity* CreateEntity(int numBytes)
{
  Entity* entity;
  char* data; 

  entity = (Entity*) malloc(sizeof(Entity));
  data = (char*) malloc(numBytes);
  entity->data = data;
  entity->behaviors = NULL;
  entity->propertyLabels = NULL;
  entity->behaviorLabels  = NULL;
  entity->numProperties = 0;
  entity->numBehaviors = 0;
  entity->propertyBytes = numBytes;

  return entity;
}
// -----------------------------------------------------------------------------
// Adds a behavior to an entity
void AddBehaviorToEntity(Entity* entity, Behavior behavior, const char* name)
{
  Label* labels;
  Behavior* behaviors;
  int i;

  //first check if the behavior exists already
  if (EntityHasBehavior(entity, name))
    return;

  //allocate new memory for the the behaviors
  behaviors = (Behavior*) malloc(sizeof(Behavior) * (entity->numBehaviors + 1));
  //copy all the old behaviors into the new list of behaviors
  for (i = 0; i < entity->numBehaviors; ++i)
  {
    behaviors[i] = entity->behaviors[i];
  }
  //add new behavior onto the end of that list
  behaviors[i] = behavior;
  free(entity->behaviors);
  entity->behaviors = behaviors;
  ++(entity->numBehaviors);

  //allocate new memory for the behavior labels
  labels = (Label*) malloc(sizeof(Label) * (entity->numBehaviors));
  //copy all the old behavior labels into the new list of behavior labels
  for (i = 0; i < entity->numBehaviors - 1; ++i)
  {
    labels[i] = entity->behaviorLabels[i];
  }
  //add the new label to the new list of behavior labels
  labels[i].name = name;
  labels[i].byteOffset = (entity->numBehaviors - 1) * sizeof(Behavior);
  free(entity->behaviorLabels);
  entity->behaviorLabels = labels;
}
// -----------------------------------------------------------------------------
void RemoveBehaviorFromEntity(Entity* entity, const char* label)
{
  Behavior* behaviors;
  Label* labels;
  int i, indexToRemove;

  //find the index at which the behavior exists
  for (i = 0; i < entity->numBehaviors; ++i)
  {
    if (strcmp(entity->behaviorLabels[i].name, label) == 0)
      break;
  }
  //if it doesn't exist, return
  if (i == entity->numBehaviors)
    return;

  //allocate a new list of behaviors
  behaviors = (Behavior*) malloc(sizeof(Behavior) * (entity->numBehaviors - 1));
  //copy over all behaviors into the new list other than the one to remove
  indexToRemove = i;
  for (i = 0; i < entity->numBehaviors; ++i)
  {
    int index = i;
    if (i == indexToRemove)
      continue;
    else if (i > indexToRemove)
      --index;

    behaviors[index] = entity->behaviors[i];
  }
  //free the old list
  free(entity->behaviors);
  //assign the new one
  entity->behaviors = behaviors;

  //allocate a new list of behavior labels
  labels = (Label*) malloc(sizeof(Label) * (entity->numBehaviors - 1));
  //copy over all labels into the new list other than the one to remove
  for (i = 0; i < entity->numBehaviors; ++i)
  {
    int index = i;
    if (i == indexToRemove)
      continue;
    else if (i > indexToRemove)
      --index;

    labels[index] = entity->behaviorLabels[i];
  }
  //free the old list
  free(entity->behaviorLabels);
  //assign the new one
  entity->behaviorLabels = labels;
  //housekeeping
  --(entity->numBehaviors);
}
// -----------------------------------------------------------------------------
// Checks to see whether an entity has a given behavior
int EntityHasBehavior(const Entity* entity, const char* label)
{
  int i;

  for (i = 0; i < entity->numBehaviors; ++i)
  {
    if (strcmp(label, entity->behaviorLabels[i].name) == 0)
      return 1;
  }

  return 0;
}
// -----------------------------------------------------------------------------
// Adds a property to an entity
void AddPropertyToEntity(Entity* entity, char* data, int size, const char* label)
{
  char* newData;
  Label* labels;
  int i;

  //first check if the property exists, and if it does, return
  if (EntityHasProperty(entity, label))
    return;

  //allocate memory for the new data
  newData = (char*) malloc(sizeof(char) * (entity->propertyBytes + size));
  //copy the old data over
  memcpy(newData, entity->data, entity->propertyBytes);
  //copy the new data over
  memcpy(newData + entity->propertyBytes, data, size);
  //free the old data
  free(entity->data);

  //assign the new one
  entity->data = newData;

  //allocate memory for the new labels
  labels = (Label*) malloc(sizeof(Label) * (entity->numProperties + 1));
  //copy the old labels over
  for (i = 0; i < entity->numProperties; ++i)
  {
    labels[i] = entity->propertyLabels[i];
  }
  //make a new label
  labels[i].name = label;
  labels[i].byteOffset = entity->propertyBytes;

  //free the old data
  free(entity->propertyLabels);
  //assign the new one
  entity->propertyLabels = labels;

  //housekeeping
  ++(entity->numProperties);
  entity->propertyBytes += size;
}
// -----------------------------------------------------------------------------
// Removes a property from an entity
void RemovePropertyFromEntity(Entity* entity, const char* label)
{
  int i, offset, indexToRemove, removeBytes = 0;
  char* data;
  Label* labels;

  //find out how much memory is taken up by the given property
  for (i = 0; i < entity->numProperties; ++i)
  {
    if (strcmp(entity->propertyLabels[i].name, label) == 0)
    {
      offset = entity->propertyLabels[i].byteOffset;
      if (i == entity->numProperties - 1)
        removeBytes = entity->propertyBytes - offset;
      else
        removeBytes = entity->propertyLabels[i + 1].byteOffset - offset;
      break;
    }
  }

  //if we got all the way through the list, then the entity doesn't have the 
  //property, so we can just break out early
  if (i == entity->numProperties)
    return;

  indexToRemove = i;

  //if the number of bytes of memory to remove is greater than the amount of 
  //memory we have, something terrible has happened and we need to break out
  if (removeBytes > entity->propertyBytes)
    return;

  //allocate memory for the new properties
  data = (char*) malloc(sizeof(char) * (entity->propertyBytes - removeBytes));
  memmove(entity->data + offset, entity->data + offset + removeBytes, 
    removeBytes);
  //copy over the relevant memory
  memcpy(data, entity->data, entity->propertyBytes - removeBytes);
  //free the old memory
  free(entity->data);
  //assign the new block
  entity->data = data;

  //allocate memory for the new labels
  labels = (Label*) malloc(sizeof(Label) * (entity->numProperties - 1));
  //copy over all labels into the new list other than the one to remove
  for (i = 0; i < entity->numProperties; ++i)
  {
    int index = i;
    int offset = 0;
    if (i == indexToRemove)
      continue;
    else if (i > indexToRemove)
    {
      --index;
      offset = removeBytes;
    }

    labels[index] = entity->propertyLabels[i];
    labels[index].byteOffset -= offset;
  }

  //free the old list
  free(entity->propertyLabels);
  //assign the new one
  entity->propertyLabels = labels;
  //housekeeping
  --(entity->numProperties);
  entity->propertyBytes -= removeBytes;
}
// -----------------------------------------------------------------------------
// Checks to see whether an entity has a given property
int EntityHasProperty(const Entity* entity, const char* label)
{
  int i;

  for (i = 0; i < entity->numProperties; ++i)
  {
    if (strcmp(label, entity->propertyLabels[i].name) == 0)
      return 1;
  }

  return 0;
}
// -----------------------------------------------------------------------------
// Gets a pointer to an entity's property
// returns NULL if the entity doesn't have a property with the supplied name
void* GetPropertyOfEntity(const Entity* entity, const char* label)
{
  int i;
  int offset = 0;

  for (i = 0; i < entity->numProperties; ++i)
  {
    offset += entity->propertyLabels[i].byteOffset;

    if (strcmp(label, entity->propertyLabels[i].name) == 0)
      break;
  }

  if (i == entity->numProperties)
    return NULL;

  return entity->data + offset;
}
// -----------------------------------------------------------------------------
// Gets a pointer to an entity's behavior
// returns NULL if the entity doesn't have a property with the supplied name
Behavior GetBehaviorOfEntity(const Entity* entity, const char* label)
{
  int i;

  for (i = 0; i < entity->numBehaviors; ++i)
  {
    if (strcmp(label, entity->behaviorLabels[i].name) == 0)
      break;
  }

  if (i == entity->numProperties)
    return NULL;

  return entity->behaviors[i];
}
// -----------------------------------------------------------------------------
// Gets the size of an entity's property in number of bytes
// returns -1 if the entity does not have a property with the supplied name
int GetSizeOfProperty(const Entity* entity, const char* label)
{
  int i;

  for (i = 0; i < entity->numProperties; ++i)
  {
    if (strcmp(label, entity->propertyLabels[i].name) == 0)
      break;
  }

  if (i == entity->numProperties)
    return -1;

  if (i == entity->numProperties - 1)
    return entity->propertyBytes - entity->propertyLabels[i].byteOffset;

  return entity->propertyLabels[i + 1].byteOffset - 
    entity->propertyLabels[i].byteOffset;
}