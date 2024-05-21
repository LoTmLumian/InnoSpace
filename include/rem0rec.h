#ifndef rem0rec_h
#define rem0rec_h

#include "include/mach_data.h"
#include "include/rec.h"
#include "include/rem0types.h"
#include "include/udef.h"

/** The following function is used to get the offset of the
next chained record on the same page.
@param[in] rec  Physical record.
@param[in] comp Nonzero=compact page format.
@return the page offset of the next chained record, or 0 if none */
[[nodiscard]] ulint rec_get_next_offs(const rec_t *rec, ulint comp);

/** Returns the offset of n - 1th field end if the record is stored in the
 1-byte offsets form. If the field is SQL null, the flag is ORed in the returned
 value. This function and the 2-byte counterpart are defined here because the
 C-compiler was not able to sum negative and positive constant offsets, and
 warned of constant arithmetic overflow within the compiler.
 @return offset of the start of the PREVIOUS field, SQL null flag ORed */
ulint rec_1_get_prev_field_end_info(
  const rec_t *rec, /*!< in: record */
  ulint n);         /*!< in: field index */

#endif