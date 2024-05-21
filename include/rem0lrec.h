#ifndef rem0lrec_h
#define rem0lrec_h

#include "include/rem0rec.h"

/** The following function is used to get the offset to the nth
data field in an old-style record.
@param[in]   rec  record
@param[in]   n    index of the field
@param[out]  len  length of the field; UNIV_SQL_NULL if SQL null;
@return offset to the field */
ulint rec_get_nth_field_offs_old_low(const rec_t *rec, ulint n, ulint *len);

#endif