#ifndef dict0boot_h
#define dict0boot_h

/** The ids for the basic system tables and their indexes */
constexpr uint32_t DICT_TABLES_ID = 1;
constexpr uint32_t DICT_COLUMNS_ID = 2;
constexpr uint32_t DICT_INDEXES_ID = 3;
constexpr uint32_t DICT_FIELDS_ID = 4;
/* The following is a secondary index on SYS_TABLES */
constexpr uint32_t DICT_TABLE_IDS_ID = 5;

/** the ids for tables etc. start from this number, except for basic system
 tables and their above defined indexes; ibuf tables and indexes are assigned
 as the id the number DICT_IBUF_ID_MIN plus the space id */
constexpr uint32_t DICT_HDR_FIRST_ID = 10;

/* The offset of the dictionary header on the page */
constexpr uint32_t DICT_HDR = FSEG_PAGE_DATA;

/*-------------------------------------------------------------*/
/* Dictionary header offsets */
/** The latest assigned row id */
constexpr uint32_t DICT_HDR_ROW_ID = 0;
/** The latest assigned table id */
constexpr uint32_t DICT_HDR_TABLE_ID = 8;
/** The latest assigned index id */
constexpr uint32_t DICT_HDR_INDEX_ID = 16;
/** The latest assigned space id,or 0*/
constexpr uint32_t DICT_HDR_MAX_SPACE_ID = 24;
/** Obsolete,always DICT_HDR_FIRST_ID*/
constexpr uint32_t DICT_HDR_MIX_ID_LOW = 28;
/** Root of SYS_TABLES clust index */
constexpr uint32_t DICT_HDR_TABLES = 32;
/** Root of SYS_TABLE_IDS sec index */
constexpr uint32_t DICT_HDR_TABLE_IDS = 36;
/** Root of SYS_COLUMNS clust index */
constexpr uint32_t DICT_HDR_COLUMNS = 40;
/** Root of SYS_INDEXES clust index */
constexpr uint32_t DICT_HDR_INDEXES = 44;
/** Root of SYS_FIELDS clust index */
constexpr uint32_t DICT_HDR_FIELDS = 48;

/* Segment header for the tablespace segment into which the dictionary header is
 created */
constexpr uint32_t DICT_HDR_FSEG_HEADER = 56;
/*-------------------------------------------------------------*/

/* The columns in SYS_TABLES */
enum dict_col_sys_tables_enum {
  DICT_COL__SYS_TABLES__NAME = 0,
  DICT_COL__SYS_TABLES__ID = 1,
  DICT_COL__SYS_TABLES__N_COLS = 2,
  DICT_COL__SYS_TABLES__TYPE = 3,
  DICT_COL__SYS_TABLES__MIX_ID = 4,
  DICT_COL__SYS_TABLES__MIX_LEN = 5,
  DICT_COL__SYS_TABLES__CLUSTER_ID = 6,
  DICT_COL__SYS_TABLES__SPACE = 7,
  DICT_NUM_COLS__SYS_TABLES = 8
};
/* The field numbers in the SYS_TABLES clustered index */
enum dict_fld_sys_tables_enum {
  DICT_FLD__SYS_TABLES__NAME = 0,
  DICT_FLD__SYS_TABLES__DB_TRX_ID = 1,
  DICT_FLD__SYS_TABLES__DB_ROLL_PTR = 2,
  DICT_FLD__SYS_TABLES__ID = 3,
  DICT_FLD__SYS_TABLES__N_COLS = 4,
  DICT_FLD__SYS_TABLES__TYPE = 5,
  DICT_FLD__SYS_TABLES__MIX_ID = 6,
  DICT_FLD__SYS_TABLES__MIX_LEN = 7,
  DICT_FLD__SYS_TABLES__CLUSTER_ID = 8,
  DICT_FLD__SYS_TABLES__SPACE = 9,
  DICT_NUM_FIELDS__SYS_TABLES = 10
};
/* The field numbers in the SYS_TABLE_IDS index */
enum dict_fld_sys_table_ids_enum {
  DICT_FLD__SYS_TABLE_IDS__ID = 0,
  DICT_FLD__SYS_TABLE_IDS__NAME = 1,
  DICT_NUM_FIELDS__SYS_TABLE_IDS = 2
};
/* The columns in SYS_COLUMNS */
enum dict_col_sys_columns_enum {
  DICT_COL__SYS_COLUMNS__TABLE_ID = 0,
  DICT_COL__SYS_COLUMNS__POS = 1,
  DICT_COL__SYS_COLUMNS__NAME = 2,
  DICT_COL__SYS_COLUMNS__MTYPE = 3,
  DICT_COL__SYS_COLUMNS__PRTYPE = 4,
  DICT_COL__SYS_COLUMNS__LEN = 5,
  DICT_COL__SYS_COLUMNS__PREC = 6,
  DICT_NUM_COLS__SYS_COLUMNS = 7
};
/* The field numbers in the SYS_COLUMNS clustered index */
enum dict_fld_sys_columns_enum {
  DICT_FLD__SYS_COLUMNS__TABLE_ID = 0,
  DICT_FLD__SYS_COLUMNS__POS = 1,
  DICT_FLD__SYS_COLUMNS__DB_TRX_ID = 2,
  DICT_FLD__SYS_COLUMNS__DB_ROLL_PTR = 3,
  DICT_FLD__SYS_COLUMNS__NAME = 4,
  DICT_FLD__SYS_COLUMNS__MTYPE = 5,
  DICT_FLD__SYS_COLUMNS__PRTYPE = 6,
  DICT_FLD__SYS_COLUMNS__LEN = 7,
  DICT_FLD__SYS_COLUMNS__PREC = 8,
  DICT_NUM_FIELDS__SYS_COLUMNS = 9
};
/* The columns in SYS_INDEXES */
enum dict_col_sys_indexes_enum {
  DICT_COL__SYS_INDEXES__TABLE_ID = 0,
  DICT_COL__SYS_INDEXES__ID = 1,
  DICT_COL__SYS_INDEXES__NAME = 2,
  DICT_COL__SYS_INDEXES__N_FIELDS = 3,
  DICT_COL__SYS_INDEXES__TYPE = 4,
  DICT_COL__SYS_INDEXES__SPACE = 5,
  DICT_COL__SYS_INDEXES__PAGE_NO = 6,
  DICT_COL__SYS_INDEXES__MERGE_THRESHOLD = 7,
  DICT_NUM_COLS__SYS_INDEXES = 8
};
/* The field numbers in the SYS_INDEXES clustered index */
enum dict_fld_sys_indexes_enum {
  DICT_FLD__SYS_INDEXES__TABLE_ID = 0,
  DICT_FLD__SYS_INDEXES__ID = 1,
  DICT_FLD__SYS_INDEXES__DB_TRX_ID = 2,
  DICT_FLD__SYS_INDEXES__DB_ROLL_PTR = 3,
  DICT_FLD__SYS_INDEXES__NAME = 4,
  DICT_FLD__SYS_INDEXES__N_FIELDS = 5,
  DICT_FLD__SYS_INDEXES__TYPE = 6,
  DICT_FLD__SYS_INDEXES__SPACE = 7,
  DICT_FLD__SYS_INDEXES__PAGE_NO = 8,
  DICT_FLD__SYS_INDEXES__MERGE_THRESHOLD = 9,
  DICT_NUM_FIELDS__SYS_INDEXES = 10
};
/* The columns in SYS_FIELDS */
enum dict_col_sys_fields_enum {
  DICT_COL__SYS_FIELDS__INDEX_ID = 0,
  DICT_COL__SYS_FIELDS__POS = 1,
  DICT_COL__SYS_FIELDS__COL_NAME = 2,
  DICT_NUM_COLS__SYS_FIELDS = 3
};
/* The field numbers in the SYS_FIELDS clustered index */
enum dict_fld_sys_fields_enum {
  DICT_FLD__SYS_FIELDS__INDEX_ID = 0,
  DICT_FLD__SYS_FIELDS__POS = 1,
  DICT_FLD__SYS_FIELDS__DB_TRX_ID = 2,
  DICT_FLD__SYS_FIELDS__DB_ROLL_PTR = 3,
  DICT_FLD__SYS_FIELDS__COL_NAME = 4,
  DICT_NUM_FIELDS__SYS_FIELDS = 5
};
/* The columns in SYS_FOREIGN */
enum dict_col_sys_foreign_enum {
  DICT_COL__SYS_FOREIGN__ID = 0,
  DICT_COL__SYS_FOREIGN__FOR_NAME = 1,
  DICT_COL__SYS_FOREIGN__REF_NAME = 2,
  DICT_COL__SYS_FOREIGN__N_COLS = 3,
  DICT_NUM_COLS__SYS_FOREIGN = 4
};
/* The field numbers in the SYS_FOREIGN clustered index */
enum dict_fld_sys_foreign_enum {
  DICT_FLD__SYS_FOREIGN__ID = 0,
  DICT_FLD__SYS_FOREIGN__DB_TRX_ID = 1,
  DICT_FLD__SYS_FOREIGN__DB_ROLL_PTR = 2,
  DICT_FLD__SYS_FOREIGN__FOR_NAME = 3,
  DICT_FLD__SYS_FOREIGN__REF_NAME = 4,
  DICT_FLD__SYS_FOREIGN__N_COLS = 5,
  DICT_NUM_FIELDS__SYS_FOREIGN = 6
};
/* The field numbers in the SYS_FOREIGN_FOR_NAME secondary index */
enum dict_fld_sys_foreign_for_name_enum {
  DICT_FLD__SYS_FOREIGN_FOR_NAME__NAME = 0,
  DICT_FLD__SYS_FOREIGN_FOR_NAME__ID = 1,
  DICT_NUM_FIELDS__SYS_FOREIGN_FOR_NAME = 2
};
/* The columns in SYS_FOREIGN_COLS */
enum dict_col_sys_foreign_cols_enum {
  DICT_COL__SYS_FOREIGN_COLS__ID = 0,
  DICT_COL__SYS_FOREIGN_COLS__POS = 1,
  DICT_COL__SYS_FOREIGN_COLS__FOR_COL_NAME = 2,
  DICT_COL__SYS_FOREIGN_COLS__REF_COL_NAME = 3,
  DICT_NUM_COLS__SYS_FOREIGN_COLS = 4
};
/* The field numbers in the SYS_FOREIGN_COLS clustered index */
enum dict_fld_sys_foreign_cols_enum {
  DICT_FLD__SYS_FOREIGN_COLS__ID = 0,
  DICT_FLD__SYS_FOREIGN_COLS__POS = 1,
  DICT_FLD__SYS_FOREIGN_COLS__DB_TRX_ID = 2,
  DICT_FLD__SYS_FOREIGN_COLS__DB_ROLL_PTR = 3,
  DICT_FLD__SYS_FOREIGN_COLS__FOR_COL_NAME = 4,
  DICT_FLD__SYS_FOREIGN_COLS__REF_COL_NAME = 5,
  DICT_NUM_FIELDS__SYS_FOREIGN_COLS = 6
};
/* The columns in SYS_TABLESPACES */
enum dict_col_sys_tablespaces_enum {
  DICT_COL__SYS_TABLESPACES__SPACE = 0,
  DICT_COL__SYS_TABLESPACES__NAME = 1,
  DICT_COL__SYS_TABLESPACES__FLAGS = 2,
  DICT_NUM_COLS__SYS_TABLESPACES = 3
};
/* The field numbers in the SYS_TABLESPACES clustered index */
enum dict_fld_sys_tablespaces_enum {
  DICT_FLD__SYS_TABLESPACES__SPACE = 0,
  DICT_FLD__SYS_TABLESPACES__DB_TRX_ID = 1,
  DICT_FLD__SYS_TABLESPACES__DB_ROLL_PTR = 2,
  DICT_FLD__SYS_TABLESPACES__NAME = 3,
  DICT_FLD__SYS_TABLESPACES__FLAGS = 4,
  DICT_NUM_FIELDS__SYS_TABLESPACES = 5
};
/* The columns in SYS_DATAFILES */
enum dict_col_sys_datafiles_enum {
  DICT_COL__SYS_DATAFILES__SPACE = 0,
  DICT_COL__SYS_DATAFILES__PATH = 1,
  DICT_NUM_COLS__SYS_DATAFILES = 2
};
/* The field numbers in the SYS_DATAFILES clustered index */
enum dict_fld_sys_datafiles_enum {
  DICT_FLD__SYS_DATAFILES__SPACE = 0,
  DICT_FLD__SYS_DATAFILES__DB_TRX_ID = 1,
  DICT_FLD__SYS_DATAFILES__DB_ROLL_PTR = 2,
  DICT_FLD__SYS_DATAFILES__PATH = 3,
  DICT_NUM_FIELDS__SYS_DATAFILES = 4
};

/* The columns in SYS_VIRTUAL */
enum dict_col_sys_virtual_enum {
  DICT_COL__SYS_VIRTUAL__TABLE_ID = 0,
  DICT_COL__SYS_VIRTUAL__POS = 1,
  DICT_COL__SYS_VIRTUAL__BASE_POS = 2,
  DICT_NUM_COLS__SYS_VIRTUAL = 3
};
/* The field numbers in the SYS_VIRTUAL clustered index */
enum dict_fld_sys_virtual_enum {
  DICT_FLD__SYS_VIRTUAL__TABLE_ID = 0,
  DICT_FLD__SYS_VIRTUAL__POS = 1,
  DICT_FLD__SYS_VIRTUAL__BASE_POS = 2,
  DICT_FLD__SYS_VIRTUAL__DB_TRX_ID = 3,
  DICT_FLD__SYS_VIRTUAL__DB_ROLL_PTR = 4,
  DICT_NUM_FIELDS__SYS_VIRTUAL = 5
};

/* A number of the columns above occur in multiple tables.  These are the
length of those fields. */
constexpr uint32_t DICT_FLD_LEN_SPACE = 4;
constexpr uint32_t DICT_FLD_LEN_FLAGS = 4;

/* When a row id which is zero modulo this number (which must be a power of
two) is assigned, the field DICT_HDR_ROW_ID on the dictionary header page is
updated */
constexpr uint32_t DICT_HDR_ROW_ID_WRITE_MARGIN = 256;


#endif