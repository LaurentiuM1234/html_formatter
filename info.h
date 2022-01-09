#ifndef __INFO__
#define __INFO__

#include "utils.h"

#define TYPE_SIZE 1000
#define ID_SIZE 1000
#define CONTENTS_SIZE 1000


/*Function used to alloc a structure of type TInfo
 @returns: NULL if any of the allocations failed or
 a pointer to the newly allocated structure*/
TInfo createInfo(void);

/*Function used to free the memory occupied by
 structure of type TInfo(safe delete)
 @params: info:->address of pointer to a structure
 of type TInfo*/
void deleteInfo(TInfo *info);

/*Function used to concat. the contents of a string
 to TInfo's content field
 @params: info:->info for which the concat. is done
          character:->string which will be concatenated*/
void catContent(TInfo info, char *character);

/*Function used to concat. the contents of a string
to TInfo's type field
@params: info:->info for which the concat. is done
         character:->string which will be concatenated*/
void catType(TInfo info, char *character);

/*Function used to set the id field in  TInfo structure
 @params: info:->pointer to structure in which the field
 setting is done
          id:->string containing the id*/
void setID(TInfo info, char *id);

/*Function used to remove the extra whitespaces from
 contents field
 @params: info:->info structure for which the formatting
 is done*/
void formatContents(TInfo info);

#endif
