#include "include/rem0rec.h"

ulint rec_get_next_offs(const rec_t *rec, ulint comp) {
  ulint field_value;
  static_assert(REC_NEXT_MASK == 0xFFFFUL, "REC_NEXT_MASK != 0xFFFFUL");
  static_assert(REC_NEXT_SHIFT == 0, "REC_NEXT_SHIFT != 0");

  field_value = mach_read_from_2(rec - REC_NEXT);

  if (comp) {
    /** Check if the result offset is still on the same page. We allow
    `field_value` to be interpreted as negative 16bit integer. This check does
    nothing for 64KB pages. */
    ut_ad(static_cast<uint16_t>(field_value +
                                ut_align_offset(rec, UNIV_PAGE_SIZE)));

    if (field_value == 0) {
      return (0);
    }

    /* There must be at least REC_N_NEW_EXTRA_BYTES + 1
    between each record. */
    ut_ad((field_value > REC_N_NEW_EXTRA_BYTES && field_value < 32768) ||
          field_value < (uint16_t)-REC_N_NEW_EXTRA_BYTES);

    return (ut_align_offset(rec + field_value, UNIV_PAGE_SIZE));
  } else {
    ut_ad(field_value < UNIV_PAGE_SIZE);

    return (field_value);
  }
}

/** Returns the offset of n - 1th field end if the record is stored in the
 1-byte offsets form. If the field is SQL null, the flag is ORed in the returned
 value. This function and the 2-byte counterpart are defined here because the
 C-compiler was not able to sum negative and positive constant offsets, and
 warned of constant arithmetic overflow within the compiler.
 @return offset of the start of the PREVIOUS field, SQL null flag ORed */
ulint rec_1_get_prev_field_end_info(
    const rec_t *rec, /*!< in: record */
    ulint n)          /*!< in: field index */
{
  ut_ad(rec_get_1byte_offs_flag(rec));
  ut_ad(n <= rec_get_n_fields_old_raw(rec));

  uint32_t version_length = 0;
  if (rec_old_is_versioned(rec)) {
    version_length = 1;
  }

  return (mach_read_from_1(rec - (REC_N_OLD_EXTRA_BYTES + version_length + n)));
}