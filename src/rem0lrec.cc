#include "include/rem0lrec.h"

static inline ulint rec_1_get_field_start_offs_low(const rec_t *rec, ulint n) {
  ut_ad(rec_get_1byte_offs_flag(rec));
  ut_ad(n <= rec_get_n_fields_old_raw(rec));

  if (n == 0) {
    return (0);
  }

  return (rec_1_get_prev_field_end_info(rec, n) & ~REC_1BYTE_SQL_NULL_MASK);
}


static inline ulint rec_1_get_field_end_info_low(const rec_t *rec, ulint n) {
  ut_ad(rec_get_1byte_offs_flag(rec));
  ut_ad(n < rec_get_n_fields_old_raw(rec));

  uint32_t version_size = 0;
  if (rec_old_is_versioned(rec)) {
    version_size = 1;
  }

  return (
      mach_read_from_1(rec - (REC_N_OLD_EXTRA_BYTES + version_size + n + 1)));
}

/** Returns the offset of n - 1th field end if the record is stored in the
 2-byte offsets form. If the field is SQL null, the flag is ORed in the returned
 value.
 @return offset of the start of the PREVIOUS field, SQL null flag ORed */
static inline ulint rec_2_get_prev_field_end_info(
    const rec_t *rec, /*!< in: record */
    ulint n)          /*!< in: field index */
{
  ut_ad(!rec_get_1byte_offs_flag(rec));
  ut_ad(n <= rec_get_n_fields_old_raw(rec));

  uint32_t version_length = 0;
  if (rec_old_is_versioned(rec)) {
    version_length = 1;
  }

  return (
      mach_read_from_2(rec - (REC_N_OLD_EXTRA_BYTES + version_length + 2 * n)));
}

static inline ulint rec_2_get_field_start_offs_low(const rec_t *rec, ulint n) {
  ut_ad(!rec_get_1byte_offs_flag(rec));
  ut_ad(n <= rec_get_n_fields_old_raw(rec));

  if (n == 0) {
    return (0);
  }

  return (rec_2_get_prev_field_end_info(rec, n) &
          ~(REC_2BYTE_SQL_NULL_MASK | REC_2BYTE_EXTERN_MASK));
}

static inline ulint rec_2_get_field_end_info_low(const rec_t *rec, ulint n) {
  ut_ad(!rec_get_1byte_offs_flag(rec));
  ut_ad(n < rec_get_n_fields_old_raw(rec));

  uint32_t version_size = 0;
  if (rec_old_is_versioned(rec)) {
    version_size = 1;
  }

  return (mach_read_from_2(rec -
                           (REC_N_OLD_EXTRA_BYTES + version_size + 2 * n + 2)));
}

/** The following function is used to get the offset to the nth
data field in an old-style record.
@param[in]   rec  record
@param[in]   n    index of the field
@param[out]  len  length of the field; UNIV_SQL_NULL if SQL null;
@return offset to the field */
ulint rec_get_nth_field_offs_old_low(const rec_t *rec, ulint n, ulint *len) {
  ulint os;
  ulint next_os;

  ut_ad(len);
  ut_a(rec);
  ut_a(n < rec_get_n_fields_old_raw(rec));

  if (rec_get_1byte_offs_flag(rec)) {
    os = rec_1_get_field_start_offs_low(rec, n);

    next_os = rec_1_get_field_end_info_low(rec, n);

    if (next_os & REC_1BYTE_SQL_NULL_MASK) {
      *len = UNIV_SQL_NULL;

      return (os);
    }

    next_os = next_os & ~REC_1BYTE_SQL_NULL_MASK;
  } else {
    os = rec_2_get_field_start_offs_low(rec, n);

    next_os = rec_2_get_field_end_info_low(rec, n);

    if (next_os & REC_2BYTE_SQL_NULL_MASK) {
      *len = UNIV_SQL_NULL;

      return (os);
    }

    next_os = next_os & ~(REC_2BYTE_SQL_NULL_MASK | REC_2BYTE_EXTERN_MASK);
  }

  *len = next_os - os;

  ut_ad(*len < UNIV_PAGE_SIZE);

  return (os);
}

