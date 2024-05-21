
#ifndef inno_space_page_page_h
#define inno_space_page_page_h

#include "include/fil0fil.h"
#include "include/page0types.h"
#include "include/fil0types.h"
#include "include/fut0lst.h"
#include "include/mach_data.h"
#include "include/univ.i"

/*                      PAGE DIRECTORY
                        ==============
*/

typedef byte page_dir_slot_t;
typedef page_dir_slot_t page_dir_t;

/* Offset of the directory start down from the page end. We call the
slot with the highest file address directory start, as it points to
the first record in the list of records. */
constexpr uint32_t PAGE_DIR = FIL_PAGE_DATA_END;

/* We define a slot in the page directory as two bytes */
constexpr uint32_t PAGE_DIR_SLOT_SIZE = 2;

/* The offset of the physically lower end of the directory, counted from
page end, when the page is empty */
constexpr uint32_t PAGE_EMPTY_DIR_START = PAGE_DIR + 2 * PAGE_DIR_SLOT_SIZE;

/* The maximum and minimum number of records owned by a directory slot. The
number may drop below the minimum in the first and the last slot in the
directory. */
constexpr uint32_t PAGE_DIR_SLOT_MAX_N_OWNED = 8;
constexpr uint32_t PAGE_DIR_SLOT_MIN_N_OWNED = 4;

/* The infimum and supremum records are omitted from the compressed page.
On compress, we compare that the records are there, and on uncompress we
restore the records. */
/** Extra bytes of an infimum record */
static const byte infimum_extra[] = {
    0x01,          /* info_bits=0, n_owned=1 */
    0x00, 0x02     /* heap_no=0, status=2 */
    /* ?, ?     */ /* next=(first user rec, or supremum) */
};
/** Data bytes of an infimum record */
static const byte infimum_data[] = {
    0x69, 0x6e, 0x66, 0x69, 0x6d, 0x75, 0x6d, 0x00 /* "infimum\0" */
};
/** Extra bytes and data bytes of a supremum record */
static const byte supremum_extra_data[] = {
    /* 0x0?, */ /* info_bits=0, n_owned=1..8 */
    0x00,
    0x0b, /* heap_no=1, status=3 */
    0x00,
    0x00, /* next=0 */
    0x73,
    0x75,
    0x70,
    0x72,
    0x65,
    0x6d,
    0x75,
    0x6d /* "supremum" */
};

/** Gets the start of a page.
@param[in]  ptr     pointer to page frame
@return start of the page */
static inline page_t *page_align(const void *ptr);

/** Gets the offset within a page.
@param[in]  ptr     pointer to page frame
@return offset from the start of the page */
static inline ulint page_offset(const void *ptr);

/** Gets the offset of the first record on the page.
 @return offset of the first record in record list, relative from page */
static inline ulint page_get_infimum_offset(
    const page_t *page); /*!< in: page which must have record(s) */

static inline const byte *page_get_infimum_rec(const byte *page) {
  return page + page_get_infimum_offset(page);
}

/** Returns the nth record of the record list.
 This is the inverse function of page_rec_get_n_recs_before().
 @return nth record */
[[nodiscard]] const rec_t *page_rec_get_nth_const(
    const page_t *page, /*!< in: page */
    ulint nth);         /*!< in: nth record */

/** Returns the nth record of the record list.
This is the inverse function of page_rec_get_n_recs_before().
@param[in]      page    page
@param[in]      nth     nth record
@return nth record */
[[nodiscard]] inline rec_t *page_rec_get_nth(page_t *page, ulint nth);

#define page_dir_get_nth_slot(page, n) \
  ((page) + (UNIV_PAGE_SIZE - PAGE_DIR - (n + 1) * PAGE_DIR_SLOT_SIZE))

/*			PAGE HEADER
        ===========

Index page header starts at the first offset left free by the FIL-module */


#define PAGE_HEADER                                  \
  FSEG_PAGE_DATA /* index page header starts at this \
         offset */
/*-----------------------------*/
#define PAGE_N_DIR_SLOTS 0 /* number of slots in page directory */
#define PAGE_HEAP_TOP 2    /* pointer to record heap top */
#define PAGE_N_HEAP                                      \
  4                    /* number of records in the heap, \
                       bit 15=flag: new-style compact page format */
#define PAGE_FREE 6    /* pointer to start of page free record list */
#define PAGE_GARBAGE 8 /* number of bytes in deleted records */
#define PAGE_LAST_INSERT                                                \
  10                      /* pointer to the last inserted record, or    \
                          NULL if this info has been reset by a delete, \
                          for example */
#define PAGE_DIRECTION 12 /* last insert direction: PAGE_LEFT, ... */
#define PAGE_N_DIRECTION                                            \
  14                   /* number of consecutive inserts to the same \
                       direction */
#define PAGE_N_RECS 16 /* number of user records on the page */
#define PAGE_MAX_TRX_ID                             \
  18 /* highest id of a trx which may have modified \
     a record on the page; trx_id_t; defined only   \
     in secondary indexes and in the insert buffer  \
     tree */
#define PAGE_HEADER_PRIV_END                      \
  26 /* end of private data structure of the page \
     header which are set in a page create */
/*----*/
#define PAGE_LEVEL                                 \
  26 /* level of the node in an index tree; the    \
     leaf level is the level 0.  This field should \
     not be written to after page creation. */
#define PAGE_INDEX_ID                          \
  28 /* index id where the page belongs.       \
     This field should not be written to after \
     page creation. */

#define PAGE_BTR_SEG_LEAF                         \
  36 /* file segment header for the leaf pages in \
     a B-tree: defined only on the root page of a \
     B-tree, but not in the root of an ibuf tree */


/** The structure of a BLOB part header */
/* @{ */
/*--------------------------------------*/
#define BTR_BLOB_HDR_PART_LEN		0	/*!< BLOB part len on this
						page */
#define BTR_BLOB_HDR_NEXT_PAGE_NO	4	/*!< next BLOB part page no,
						FIL_NULL if none */
/*--------------------------------------*/
#define BTR_BLOB_HDR_SIZE		8	/*!< Size of a BLOB
						part header, in bytes */

/* blob first page data */
enum class BlobFirstPage {
  /** One byte of flag bits.  Currently only one bit (the least
  significant bit) is used, other 7 bits are available for future use.*/
  OFFSET_FLAGS = FIL_PAGE_DATA + 1,

  /** LOB version. 4 bytes.*/
  OFFSET_LOB_VERSION = OFFSET_FLAGS + 1,

  /** The latest transaction that modified this LOB. */
  OFFSET_LAST_TRX_ID = OFFSET_LOB_VERSION + 4,

  /** The latest transaction undo_no that modified this LOB. */
  OFFSET_LAST_UNDO_NO = OFFSET_LAST_TRX_ID + 6,

  /** Length of data stored in this page.  4 bytes. */
  OFFSET_DATA_LEN = OFFSET_LAST_UNDO_NO + 4,

  /** The trx that created the data stored in this page. */
  OFFSET_TRX_ID = OFFSET_DATA_LEN + 4,

  /** The offset where the list base node is located.  This is the list
  of LOB pages. */
  OFFSET_INDEX_LIST = OFFSET_TRX_ID + 6,
};

/* Blob index page */
enum class BlobIndexPage {
  OFFSET_VERSION = FIL_PAGE_DATA,
  LOB_PAGE_DATA = OFFSET_VERSION + 1
};

/* Blob data page */
enum class BlobDataPage {
  OFFSET_VERSION = FIL_PAGE_DATA,
  OFFSET_DATA_LEN = OFFSET_VERSION + 1,
  OFFSET_TRX_ID = OFFSET_DATA_LEN + 4,
  LOB_PAGE_DATA = OFFSET_TRX_ID + 6
};
/*-------------------------------------------------------------*/
/* Slot size */
#define TRX_RSEG_SLOT_SIZE 4

/* The offset of the rollback segment header on its page */
#define TRX_RSEG FSEG_PAGE_DATA

/* Transaction rollback segment header */
/*-------------------------------------------------------------*/
#define TRX_RSEG_MAX_SIZE                \
  0 /* Maximum allowed size for rollback \
    segment in pages */
#define TRX_RSEG_HISTORY_SIZE        \
  4 /* Number of file pages occupied \
    by the logs in the history list */
#define TRX_RSEG_HISTORY                  \
  8 /* The update undo logs for committed \
    transactions */
#define TRX_RSEG_FSEG_HEADER (8 + FLST_BASE_NODE_SIZE)
/* Header for the file segment where
this page is placed */
#define TRX_RSEG_UNDO_SLOTS (8 + FLST_BASE_NODE_SIZE + FSEG_HEADER_SIZE)
/* Undo log segment slots */
/*-------------------------------------------------------------*/

/** The undo log header. There can be several undo log headers on the first
page of an update undo log segment. */
/* @{ */
/*-------------------------------------------------------------*/
#define TRX_UNDO_TRX_ID 0 /*!< Transaction id */
#define TRX_UNDO_TRX_NO                  \
  8 /*!< Transaction number of the       \
    transaction; defined only if the log \
    is in a history list */
#define TRX_UNDO_DEL_MARKS                 \
  16 /*!< Defined only in an update undo   \
     log: TRUE if the transaction may have \
     done delete markings of records, and  \
     thus purge is necessary */
#define TRX_UNDO_LOG_START                    \
  18 /*!< Offset of the first undo log record \
     of this log on the header page; purge    \
     may remove undo log record from the      \
     log start, and therefore this is not     \
     necessarily the same as this log         \
     header end offset */
#define TRX_UNDO_XID_EXISTS                \
  20 /*!< TRUE if undo log header includes \
     X/Open XA transaction identification  \
     XID */
#define TRX_UNDO_DICT_TRANS                  \
  21 /*!< TRUE if the transaction is a table \
     create, index create, or drop           \
     transaction: in recovery                \
     the transaction cannot be rolled back   \
     in the usual way: a 'rollback' rather   \
     means dropping the created or dropped   \
     table, if it still exists */
#define TRX_UNDO_TABLE_ID                  \
  22 /*!< Id of the table if the preceding \
     field is TRUE. Note: deprecated */

/*!< Use following two fields to store last or penult statement undo no,
so that we can rollback the last statement before executing again from proxy.
Every field only occupy 4 btyes so that undo no can not bigger than UINT_MAX32.
If it does overflow, transaction resuming will disable for this transaction */
#define TRX_UNDO_LAST_STMT_UNDO_NO_1 TRX_UNDO_TABLE_ID
#define TRX_UNDO_LAST_STMT_UNDO_NO_2 (TRX_UNDO_LAST_STMT_UNDO_NO_1 + 4)

#define TRX_UNDO_NEXT_LOG                    \
  30 /*!< Offset of the next undo log header \
     on this page, 0 if none */
#define TRX_UNDO_PREV_LOG                 \
  32 /*!< Offset of the previous undo log \
     header on this page, 0 if none */
#define TRX_UNDO_HISTORY_NODE              \
  34 /*!< If the log is put to the history \
     list, the file list node is here */
/*-------------------------------------------------------------*/

/* Max number of rollback segments */
#define TRX_SYS_N_RSEGS 128

/** The offset of the Rollback Segment Directory header on an RSEG_ARRAY page */
#define RSEG_ARRAY_HEADER FSEG_PAGE_DATA

/** Rollback Segment Array Header */
/*------------------------------------------------------------- */
/** The RSEG ARRAY base version is a number derived from the string
'RSEG' [0x 52 53 45 47] for extra validation. Each new version
increments the base version by 1. */
#define RSEG_ARRAY_VERSION 0x52534547 + 1

/** The RSEG ARRAY version offset in the header. */
#define RSEG_ARRAY_VERSION_OFFSET 0

/** The current number of rollback segments being tracked in this array */
#define RSEG_ARRAY_SIZE_OFFSET 4

/** This is the pointer to the file segment inode that tracks this
rseg array page. */
#define RSEG_ARRAY_FSEG_HEADER_OFFSET 8

/** The start of the array of rollback segment header page numbers for this
undo tablespace. The potential size of this array is limited only by the
page size minus overhead. The actual size of the array is limited by
srv_rollback_segments. */
#define RSEG_ARRAY_PAGES_OFFSET (8 + FSEG_HEADER_SIZE)

/** Reserved space at the end of an RSEG_ARRAY page reserved for future use. */
#define RSEG_ARRAY_RESERVED_BYTES 200

/* Slot size of the array of rollback segment header page numbers */
#define RSEG_ARRAY_SLOT_SIZE 4
/*------------------------------------------------------------- */

/** The offset of the undo log page header on pages of the undo log */
#define TRX_UNDO_PAGE_HDR FSEG_PAGE_DATA
/*-------------------------------------------------------------*/
/** Transaction undo log page header offsets */
/* @{ */
#define TRX_UNDO_PAGE_TYPE  \
  0 /*!< TRX_UNDO_INSERT or \
    TRX_UNDO_UPDATE */
#define TRX_UNDO_PAGE_START               \
  2 /*!< Byte offset where the undo log   \
    records for the LATEST transaction    \
    start on this page (remember that     \
    in an update undo log, the first page \
    can contain several undo logs) */
#define TRX_UNDO_PAGE_FREE                 \
  4 /*!< On each page of the undo log this \
    field contains the byte offset of the  \
    first free byte on the page */
#define TRX_UNDO_PAGE_NODE               \
  6 /*!< The file list node in the chain \
    of undo log pages */
/*-------------------------------------------------------------*/
#define TRX_UNDO_PAGE_HDR_SIZE (6 + FLST_NODE_SIZE)
/*!< Size of the transaction undo
log page header, in bytes */
/* @} */

#define TRX_UNDO_SEG_HDR (TRX_UNDO_PAGE_HDR + TRX_UNDO_PAGE_HDR_SIZE)
/** Undo log segment header */
/* @{ */
/*-------------------------------------------------------------*/
#define TRX_UNDO_STATE 0 /*!< TRX_UNDO_ACTIVE, ... */

#define TRX_UNDO_LAST_LOG                   \
  2 /*!< Offset of the last undo log header \
    on the segment header page, 0 if        \
    none */

#define UNIV_PAGE_SIZE (16 * 1024)

/** Gets the page number.
 @return page number */
page_no_t page_get_page_no(const page_t *page); /*!< in: page */

/** Gets the tablespace identifier.
 @return space id */
space_id_t page_get_space_id(const page_t *page); /*!< in: page */

page_t *align_page(const void* ptr);

/** Reads the given header field. */
ulint page_header_get_field(const page_t *page, ulint field);        /*!< in: PAGE_LEVEL, ... */

/** Gets the number of records in the heap.
 *  @return number of user records */
ulint page_dir_get_n_heap(const page_t *page);

/** Gets the pointer to the next record on the page.
 @return pointer to next record */
rec_t *page_rec_get_next(rec_t *rec); /*!< in: pointer to record */

/************************************************************//**
Determine whether the page is a B-tree leaf.
@return true if the page is a B-tree leaf (PAGE_LEVEL = 0) */
bool
page_is_leaf(
/*=========*/
  const page_t*   page);   /*!< in: page */
#endif
