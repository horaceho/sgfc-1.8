/**************************************************************************
***
*** Project: SGF Syntax Checker & Converter
***	File:	 all.h
***
*** Copyright (C) 1996-2004 by Arno Hollosi
*** (see 'main.c' for more copyright information)
***
*** Notes:	global definition of all #defines and structures
***
**************************************************************************/


/*#define DIRTY_FREE */	/* don't call free() on program exit
						** because it takes a LOT of time and the OS or the
						** compiler clean up anyway (which is much faster)
						*/

#define VERSION_UNIX				/* Mutual exclusiv defines */
/* #define VERSION_MAC		*/		/* for generating machine dependent code */
/* #define VERSION_MSDOS	*/		/* see README for details */

/* #define VERSION_NO_MAIN */		/* In case you've written a new main()
									** e.g. for writing a mouse-interface
									** (drag & drop shell)
									*/

#define EOLCHAR '\n'	/* EndOfLine-Character
						** '\n' for UNIX, AMIGA (SGF standard)
						** '\r' for MAC
						** 0    for PC's (0 gets translated to "\r\n")
						*/


typedef unsigned char	UCHAR;
typedef unsigned short	USHORT;
typedef unsigned long	ULONG;

#ifndef TRUE
#define TRUE	1
#endif
#ifndef FALSE
#define FALSE	0
#endif

/* order must match order in sgf_token[] !! */

typedef enum {
		TKN_NONE = -1, TKN_UNKNOWN,
		TKN_B,  TKN_W,  TKN_AB, TKN_AW, TKN_AE, TKN_N,  TKN_C,
		TKN_BL, TKN_WL, TKN_OB, TKN_OW,
		TKN_FF, TKN_GM, TKN_SZ, TKN_ST, TKN_CA, TKN_AP,
		TKN_GN, TKN_GC, TKN_PB, TKN_PW, TKN_BR, TKN_WR,
		TKN_PC, TKN_DT, TKN_RE,	TKN_KM, TKN_KI, TKN_HA, TKN_TM, TKN_EV,
		TKN_RO, TKN_SO, TKN_US, TKN_BT, TKN_WT, TKN_RU, TKN_AN, TKN_OT,
		TKN_ON, TKN_CP,
		TKN_L,	TKN_LB, TKN_AR, TKN_LN, TKN_M,  TKN_MA, TKN_TR, TKN_CR,
		TKN_TB, TKN_TW, TKN_SQ,	TKN_SL, TKN_DD,
		TKN_PL, TKN_V,  TKN_GB, TKN_GW, TKN_UC, TKN_DM, TKN_TE,
		TKN_BM, TKN_DO, TKN_IT, TKN_HO,
		TKN_KO, TKN_FG, TKN_MN, TKN_VW, TKN_PM,
		/* properties not part of FF4 */
		TKN_CH, TKN_SI, TKN_BS, TKN_WS, TKN_ID, TKN_TC, TKN_OM, TKN_OP,
		TKN_OV, TKN_LT, TKN_RG, TKN_SC, TKN_SE, TKN_EL, TKN_EX
	} token;

#define EMPTY	0
#define BLACK	0x0f
#define WHITE	0xf0	/* WHITE = ~BLACK */

#define FF12	0x03
#define FF3		0x04
#define FF4		0x08

#define PVT_LIST		0x0001
#define PVT_CPLIST		0x0002	/* compressed point list */
#define PVT_EMPTY		0x0004	/* empty value allowed */
#define PVT_COMPOSE		0x0008
#define PVT_WEAKCOMPOSE	0x0010	/* weak: value may or may not be composed */
#define PVT_SIMPLE		0x0020
#define PVT_DEL_EMPTY	0x0040	/* empty values get removed */
#define PVT_CHECK_EMPTY 0x0080	/* empty values are checked by Check_xxx */
#define SPLIT_SAVE		0x0100	/* splitting with '\' (only text values) */
#define DOUBLE_MERGE	0x0200
#define DELETE_PROP		0x0400	/* used by -y option */
#define TYPE_GO			0x0800
#define TYPE_MOVE		0x1000
#define TYPE_SETUP		0x2000
#define TYPE_ROOT		0x4000
#define TYPE_GINFO		0x8000

#define ST_ADDSTONE		0x0001	/* defines are used in BoardStatus.markup */
#define ST_MARKUP		0x0002
#define ST_LABEL		0x0004
#define ST_TERRITORY	0x0008
#define ST_DIMMED		0x0010

#define ST_ANN_GEN		0x0100	/* defines are used in BoardStatus.annotate */
#define ST_ANN_MOVE		0x0200
#define ST_ANN_TE		0x0400
#define ST_ANN_BM		0x0800
#define ST_MOVE			0x1000
#define ST_KO			0x2000

#define ST_OBSOLETE		0x8000	/* obsolete props which are converted */


#define MAX_BOARDSIZE	52

#define MAX_REORDER_VARIATIONS 100


struct BoardStatus
{
	USHORT annotate;		/* flags for annotation props, etc. */
	char *ginfo;			/* pointer into buffer of first GINFO property */
	int bwidth;				/* copy of sgfc->info->bwidth */
	char *board;
	int bsize;				/* board size in bytes */
	USHORT *markup;
	int msize;				/* markup size in bytes */
	char mrkp_chngd;		/* markup field changed */
};

#define MXY(x,y) ((y)*st->bwidth + (x))


struct PropValue
{
	struct PropValue *next;		/* list */
	struct PropValue *prev;

	char *buffer;
	char *value, *value2;		/* value2 for compose value type */
};


struct Property
{
	struct Property *next;		/* list */
	struct Property *prev;
	UCHAR  priority;			/* for sorting properties within a node */

	token id;
	char *idstr;				/* id string (necessary because of TKN_UNKNOWN) */
	USHORT flags;				/* copy of sgf_token[].flags (may get changed though) */
	char *buffer;

	struct PropValue *value;	/* value list head */
	struct PropValue *valend;
};


struct Node
{
	struct Node *next;			/* list */
	struct Node *prev;

	struct Node *parent;		/* tree */
	struct Node *child;
	struct Node *sibling;

	struct Property *prop;		/* prop list head */
	struct Property *last;

	char *buffer;
};


struct TreeInfo
{
	struct TreeInfo *next;	/* list */
	struct TreeInfo *prev;

	int num;			/* number of game tree (sequence) */
	int FF;				/* File format of GameTree */
	int GM;				/* Type of game */
	int bwidth;			/* Board width  */
	int bheight;		/* Board height */

	struct Node *root;	/* root node of this tree */
};


struct SGFInfo
{
	struct Node *first;	/* node list head */
	struct Node *tail;

	struct TreeInfo *tree;	/* Info for GameTrees */
	struct TreeInfo *last;
	struct TreeInfo *info;	/* pointer to info for current GameTree */

	struct Node *root;	/* first root node (tree) */

	char *name;			/* file name */
	FILE *file;			/* file handle */
	char *buffer;		/* file buffer */
	char *b_end;		/* file buffer end address */
	char *start;		/* start of SGF data within buffer */
	char *current;		/* actual read positon (cursor) in buffer */
};

#define SGF_EOF			(sgfc->current >= sgfc->b_end)

struct ListNode
{
	struct ListNode *next;
	struct ListNode *prev;
	UCHAR priority;		/* is only used by Enqueue function */
};

struct ListHead
{
	struct ListNode *first;
	struct ListNode *last;
};


struct SGFToken
{
	char *id;
	UCHAR priority;
	UCHAR ff;		/* file format */
	int (*CheckValue)(struct Property *, struct PropValue *);
	int (*Execute_Prop)(struct Node *, struct Property *, struct BoardStatus *);
	USHORT flags;
	USHORT data;	/* for Do_XXX */
};


/* Defines for KillChars / TestChars */

#define C_ISSPACE	((USHORT)0x01)
#define C_ISPUNCT	((USHORT)0x04)
#define C_IS8BIT	((USHORT)0x08)
#define C_ISALPHA	((USHORT)0x10)
#define C_inSET		((USHORT)0x20)
#define C_NOTinSET	((USHORT)0x40)

/* defines for error handling */

#define NO_ERROR		0x00000000L
#define FATAL_ERROR		0x40000000L
#define ERROR			0x20000000L
#define WARNING			0x10000000L
#define ERROR4			0x01000000L	/* error for FF[4] / warning otherwise */
#define CRITICAL		0x02000000L
#define WARNING_STRICT	0x04000000L /* error if strict checking, else warning */
#define ERROR_TYPE		0x70000000L
#define ERROR_NUM		0x00000fffL
#define E_VALUE			0x00010000L
#define SEARCH_POS		0x00020000L
#define E_ACCUMULATE	0x00040000L
#define E_MULTIPLE		0x00080000L
#define E_ERRNO			0x00100000L
#define E_DEL_DOUBLE	0x00200000L

#define E_SET_SGFINFO			(0xfff | ERROR)

#define E_OUTPUT	stdout			/* output channel for error messages */

/* defines of error codes		**
** has to match *error_mesg[]	*/

#define FE_TOO_MANY_FILES		(1 | FATAL_ERROR)
#define FE_UNKNOWN_OPTION		(2 | FATAL_ERROR)
#define FE_SOURCE_OPEN			(3 | FATAL_ERROR | E_ERRNO)
#define FE_SOURCE_READ			(4 | FATAL_ERROR | E_ERRNO)
#define FE_OUT_OF_MEMORY		(5 | FATAL_ERROR)
#define W_SGF_IN_HEADER			(6 | WARNING | CRITICAL | SEARCH_POS)
#define FE_NO_SGFDATA			(7 | FATAL_ERROR)
#define E_ILLEGAL_OUTSIDE_CHAR	(8 | ERROR | CRITICAL | SEARCH_POS | E_ACCUMULATE)
#define E_ILLEGAL_OUTSIDE_CHARS	(8 | ERROR | CRITICAL | SEARCH_POS | E_ACCUMULATE | E_MULTIPLE)
#define E_VARIATION_NESTING		(9 | ERROR | CRITICAL | SEARCH_POS)
#define E_UNEXPECTED_EOF		(10 | ERROR | CRITICAL | SEARCH_POS)

#define E_PROPID_TOO_LONG		(11 | ERROR | CRITICAL | SEARCH_POS)
#define E_EMPTY_VARIATION		(12 | ERROR | SEARCH_POS)
#define E_TOO_MANY_VALUES		(13 | ERROR | CRITICAL | SEARCH_POS)
#define E_BAD_VALUE_DELETED		(14 | ERROR | SEARCH_POS | E_VALUE)
#define E_BAD_VALUE_CORRECTED	(15 | ERROR | SEARCH_POS | E_VALUE)
#define E4_BAD_VALUE_CORRECTED	(15 | ERROR4 | SEARCH_POS | E_VALUE)
#define E_LC_IN_PROPID			(16 | ERROR | SEARCH_POS)
#define E_EMPTY_VALUE_DELETED	(17 | ERROR | SEARCH_POS)
#define W_EMPTY_VALUE_DELETED	(17 | WARNING | SEARCH_POS)
#define E_BAD_ROOT_PROP			(18 | ERROR | SEARCH_POS)
#define WCS_GAME_NOT_GO			(19 | WARNING_STRICT | CRITICAL | SEARCH_POS)
#define E_NO_PROP_VALUES		(20 | ERROR | CRITICAL | SEARCH_POS)

#define E_VARIATION_START		(21 | ERROR | CRITICAL | SEARCH_POS)
#define W_CTRL_BYTE_DELETED		(22 | WARNING | SEARCH_POS)
#define E_COMPOSE_EXPECTED		(23 | ERROR | SEARCH_POS | E_VALUE)
#define WS_MOVE_IN_ROOT			(24 | WARNING_STRICT | SEARCH_POS)
#define E_BAD_COMPOSE_CORRECTED	(25 | ERROR | SEARCH_POS | E_VALUE)
#define FE_DEST_FILE_OPEN		(26 | FATAL_ERROR | E_ERRNO)
#define FE_DEST_FILE_WRITE		(27 | FATAL_ERROR | E_ERRNO)
#define E_DOUBLE_PROP			(28 | ERROR | SEARCH_POS)
#define W_PROPERTY_DELETED		(29 | WARNING | SEARCH_POS)
#define E4_MOVE_SETUP_MIXED		(30 | ERROR4  | SEARCH_POS)

#define WS_LONG_PROPID			(31 | WARNING_STRICT | SEARCH_POS)
#define E_ROOTP_NOT_IN_ROOTN	(32 | ERROR | SEARCH_POS)
#define E4_FAULTY_GC			(33 | ERROR4 | SEARCH_POS | E_VALUE)
#define E_CRITICAL_NOT_SAVED	(34 | ERROR)
#define WS_UNKNOWN_PROPERTY		(35 | WARNING_STRICT | SEARCH_POS)
#define E_MISSING_SEMICOLON		(36 | ERROR | CRITICAL | SEARCH_POS)
#define E_TWO_MOVES_IN_NODE		(37 | ERROR | SEARCH_POS)
#define E_POSITION_NOT_UNIQUE	(38 | ERROR | SEARCH_POS | E_VALUE | E_DEL_DOUBLE)
#define WS_ADDSTONE_REDUNDANT	(39 | WARNING_STRICT|SEARCH_POS|E_DEL_DOUBLE)
#define WS_PROPERTY_NOT_IN_FF	(40 | WARNING_STRICT | SEARCH_POS)

#define E_ANNOTATE_NOT_UNIQUE	(41 | ERROR | SEARCH_POS)
#define E4_BM_TE_IN_NODE		(42 | ERROR4 | SEARCH_POS)
#define E_ANNOTATE_WITHOUT_MOVE (43 | ERROR | SEARCH_POS)
#define E4_GINFO_ALREADY_SET	(44 | ERROR4 | SEARCH_POS)
#define WS_ROOT_PROP_DIFFERS	(45 | WARNING_STRICT | SEARCH_POS)
#define FE_UNKNOWN_FILE_FORMAT	(46 | FATAL_ERROR | SEARCH_POS)
#define E_SQUARE_AS_RECTANGULAR	(47 | ERROR | SEARCH_POS)
#define FE_MISSING_SOURCE_FILE	(48 | FATAL_ERROR)
#define FE_BAD_PARAMETER		(49 | FATAL_ERROR)
#define E_BOARD_TOO_BIG			(50 | ERROR | SEARCH_POS)

#define E_VERSION_CONFLICT		(51 | ERROR | CRITICAL | SEARCH_POS)
#define E_BAD_VW_VALUES			(52 | ERROR | SEARCH_POS)
#define FE_DEST_NAME_TOO_LONG	(53 | FATAL_ERROR)
#define E_VALUES_WITHOUT_ID		(54 | ERROR | CRITICAL | SEARCH_POS)
#define W_EMPTY_NODE_DELETED	(55 | WARNING | SEARCH_POS)
#define W_VARLEVEL_UNCERTAIN	(56 | WARNING | SEARCH_POS)
#define W_VARLEVEL_CORRECTED	(57 | WARNING | SEARCH_POS)
#define WS_ILLEGAL_MOVE			(58 | WARNING_STRICT | SEARCH_POS)
#define W_INT_KOMI_FOUND		(59 | WARNING | SEARCH_POS)

#define E_MORE_THAN_ONE_TREE	(60 | ERROR)
#define W_HANDICAP_NOT_SETUP	(61 | WARNING | SEARCH_POS)
#define W_SETUP_AFTER_ROOT		(62 | WARNING | SEARCH_POS)
#define W_MOVE_OUT_OF_SEQUENCE	(63 | WARNING | SEARCH_POS)
#define E_TOO_MANY_VARIATIONS	(64 | ERROR | SEARCH_POS)
#define E_FF4_PASS_IN_OLD_FF	(65 | ERROR | SEARCH_POS)
#define E_NODE_OUSIDE_VAR		(66 | ERROR | CRITICAL | SEARCH_POS)
#define E_MISSING_NODE_START	(67 | ERROR | CRITICAL | SEARCH_POS)

#define MAX_ERROR_NUM			67
