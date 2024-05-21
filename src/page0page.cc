#include "include/page0page.h"

/** Gets the page number.
 @return page number */
page_no_t page_get_page_no(const page_t *page) /*!< in: page */
{
  return (mach_read_from_4(page + FIL_PAGE_OFFSET));
}

/** Gets the tablespace identifier.
 @return space id */
space_id_t page_get_space_id(const page_t *page) /*!< in: page */
{
  return (mach_read_from_4(page + FIL_PAGE_ARCH_LOG_NO_OR_SPACE_ID));
}


page_t *align_page(const void* ptr) {
  return((page_t*)(((reinterpret_cast<uint64_t>(ptr))) & ~(UNIV_PAGE_SIZE - 1)));
}

/** Reads the given header field. */
ulint page_header_get_field(const page_t *page, /*!< in: page */
    ulint field)        /*!< in: PAGE_LEVEL, ... */
{
  return (mach_read_from_2(page + PAGE_HEADER + field));
}
/** Gets the number of records in the heap.
 *  @return number of user records */
ulint page_dir_get_n_heap(const page_t *page) /*!< in: index page */
{
  return (page_header_get_field(page, PAGE_N_HEAP) & PAGE_N_HEAP_MASK);
}

/** Gets the start of a page.
 @return start of the page */
static inline page_t *page_align(
    const void *ptr) /*!< in: pointer to page frame */
{
  return ((page_t *)((((ulint)ptr)) & ~(UNIV_PAGE_SIZE - 1)));
}
/** Gets the offset within a page.
 @return offset from the start of the page */
static inline ulint page_offset(
    const void *ptr) /*!< in: pointer to page frame */
{
  return (((ulint)ptr) & (UNIV_PAGE_SIZE - 1));
}

/** Used to check the consistency of a record on a page.
 @return true if succeed */
static inline bool page_rec_check(const rec_t *rec) /*!< in: record */
{
  const page_t *page = page_align(rec);

  ut_a(rec);

  ut_a(page_offset(rec) <= page_header_get_field(page, PAGE_HEAP_TOP));
  ut_a(page_offset(rec) >= PAGE_DATA);

  return true;
}

/** Gets the offset of the first record on the page.
 @return offset of the first record in record list, relative from page */
static inline ulint page_get_infimum_offset(
    const page_t *page) /*!< in: page which must have record(s) */
{
  ut_ad(page);
  ut_ad(!page_offset(page));

  if (page_is_comp(page)) {
    return (PAGE_NEW_INFIMUM);
  } else {
    return (PAGE_OLD_INFIMUM);
  }
}

/** Returns the nth record of the record list.
 This is the inverse function of page_rec_get_n_recs_before().
 @return nth record */
const rec_t *page_rec_get_nth_const(const page_t *page, /*!< in: page */
                                    ulint nth)          /*!< in: nth record */
{
  const page_dir_slot_t *slot;
  ulint i;
  ulint n_owned;
  const rec_t *rec;

  if (nth == 0) {
    return (page_get_infimum_rec(page));
  }

  ut_ad(nth < UNIV_PAGE_SIZE / (REC_N_NEW_EXTRA_BYTES + 1));

  for (i = 0;; i++) {
    slot = page_dir_get_nth_slot(page, i);
    n_owned = page_dir_slot_get_n_owned(slot);

    if (n_owned > nth) {
      break;
    } else {
      nth -= n_owned;
    }
  }

  ut_ad(i > 0);
  slot = page_dir_get_nth_slot(page, i - 1);
  rec = page_dir_slot_get_rec(slot);

  if (page_is_comp(page)) {
    do {
      rec = page_rec_get_next_low(rec, true);
      ut_ad(rec);
    } while (nth--);
  } else {
    do {
      rec = page_rec_get_next_low(rec, false);
      ut_ad(rec);
    } while (nth--);
  }

  return (rec);
}

/** Returns the nth record of the record list.
 This is the inverse function of page_rec_get_n_recs_before().
 @return nth record */
inline rec_t *page_rec_get_nth(page_t *page, /*!< in: page */
                                      ulint nth)    /*!< in: nth record */
{
  return ((rec_t *)page_rec_get_nth_const(page, nth));
}

/** Gets the pointer to the next record on the page.
 @return pointer to next record */
static inline const rec_t *page_rec_get_next_low(
    const rec_t *rec, /*!< in: pointer to record */
    ulint comp)       /*!< in: nonzero=compact page layout */
{
  ulint offs;
  const page_t *page;

  ut_ad(page_rec_check(rec));

  page = page_align(rec);

  offs = rec_get_next_offs(rec, comp);

  if (offs >= UNIV_PAGE_SIZE) {
    fprintf(stderr,
            "InnoDB: Next record offset is nonsensical %lu"
            " in record at offset %lu\n"
            "InnoDB: rec address %p, space id %lu, page %lu\n",
            (ulong)offs, (ulong)page_offset(rec), (void *)rec,
            (ulong)page_get_space_id(page), (ulong)page_get_page_no(page));
    ut_error;
  } else if (offs == 0) {
    return (nullptr);
  }

  return (page + offs);
}

/** Gets the pointer to the next record on the page.
 @return pointer to next record */
rec_t *page_rec_get_next(rec_t *rec) /*!< in: pointer to record */
{
  return ((rec_t *)page_rec_get_next_low(rec, page_rec_is_comp(rec)));
}

/************************************************************//**
Determine whether the page is a B-tree leaf.
@return true if the page is a B-tree leaf (PAGE_LEVEL = 0) */
bool
page_is_leaf(
/*=========*/
  const page_t*   page)   /*!< in: page */
{
  return(!*(const uint16*) (page + (PAGE_HEADER + PAGE_LEVEL)));
}
