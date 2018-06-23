/**************************************************************************
***
*** Project: SGF Syntax Checker & Converter
***	File:	 all.h
***
*** Copyright (C) 1996-2014 by Arno Hollosi
*** (see 'main.c' for more copyright information)
***
*** Notes:	global definition of all #defines and structures
***
**************************************************************************/


/* #define VERSION_NO_MAIN */		/* In case you've written a new main()
									** e.g. for writing a mouse-interface
									** (drag & drop shell)
									*/

#define EOLCHAR '\n'	/* EndOfLine-Character
						** '\n' for UNIX, AMIGA (SGF standard)
						** '\r' for MAC
						*/


typedef unsigned char	U_CHAR;
typedef unsigned short	U_SHORT;
typedef unsigned long	U_LONG;

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

#define EMPTY	0u
#define BLACK	0x0fu
#define WHITE	0xf0u	/* WHITE = ~BLACK */

#define FF12	0x03u
#define FF3		0x04u
#define FF4		0x08u

#define PVT_LIST		0x0001u
#define PVT_CPLIST		0x0002u	/* compressed point list */
#define PVT_EMPTY		0x0004u	/* empty value allowed */
#define PVT_COMPOSE		0x0008u
#define PVT_WEAKCOMPOSE	0x0010u	/* weak: value may or may not be composed */
#define PVT_SIMPLE		0x0020u
#define PVT_DEL_EMPTY	0x0040u	/* empty values get removed */
#define PVT_CHECK_EMPTY 0x0080u	/* empty values are checked by Check_xxx */
#define SPLIT_SAVE		0x0100u	/* splitting with '\' (only text values) */
#define DOUBLE_MERGE	0x0200u
#define DELETE_PROP		0x0400u	/* used by -y option */
#define TYPE_GO			0x0800u
#define TYPE_MOVE		0x1000u
#define TYPE_SETUP		0x2000u
#define TYPE_ROOT		0x4000u
#define TYPE_GINFO		0x8000u

#define ST_ADDSTONE		0x0001u	/* defines are used in BoardStatus.markup */
#define ST_MARKUP		0x0002u
#define ST_LABEL		0x0004u
#define ST_TERRITORY	0x0008u
#define ST_DIMMED		0x0010u

#define ST_ANN_GEN		0x0100u	/* defines are used in BoardStatus.annotate */
#define ST_ANN_MOVE		0x0200u
#define ST_ANN_TE		0x0400u
#define ST_ANN_BM		0x0800u
#define ST_MOVE			0x1000u
#define ST_KO			0x2000u

#define ST_OBSOLETE		0x8000u	/* obsolete props which are converted */

#define PARSE_MOVE      0x0001u  /* flags for Parse_Move */
#define PARSE_POS       0x0002u

#define MAX_BOARDSIZE	52

#define MAX_REORDER_VARIATIONS 100


struct BoardStatus
{
	U_SHORT annotate;		/* flags for annotation props, etc. */
	char *ginfo;			/* pointer into buffer of first GINFO property */
	int bwidth;				/* copy of sgfc->info->bwidth */
	char *board;
	size_t bsize;				/* board size in bytes */
	U_SHORT *markup;
	size_t msize;				/* markup size in bytes */
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
	U_CHAR  priority;			/* for sorting properties within a node */

	token id;
	char *idstr;				/* id string (necessary because of TKN_UNKNOWN) */
	U_SHORT flags;				/* copy of sgf_token[].flags (may get changed though) */
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
	U_CHAR priority;		/* is only used by Enqueue function */
};

struct ListHead
{
	struct ListNode *first;
	struct ListNode *last;
};


struct SGFToken
{
	char *id;
	U_CHAR priority;
	U_CHAR ff;		/* file format */
	int (*CheckValue)(struct Property *, struct PropValue *);
	int (*Execute_Prop)(struct Node *, struct Property *, struct BoardStatus *);
	U_SHORT flags;
	U_SHORT data;	/* for Do_XXX */
};


/* Defines for KillChars / TestChars */

#define C_ISSPACE	((U_SHORT)0x01)
#define C_ISPUNCT	((U_SHORT)0x04)
#define C_IS8BIT	((U_SHORT)0x08)
#define C_ISALPHA	((U_SHORT)0x10)
#define C_inSET		((U_SHORT)0x20)
#define C_NOTinSET	((U_SHORT)0x40)

/* defines for error handling */

#define E_NO_ERROR		0x00000000UL
#define E_FATAL_ERROR	0x40000000UL
#define E_ERROR			0x20000000UL
#define E_WARNING		0x10000000UL
#define E_ERROR4		0x01000000UL	/* error for FF[4] / warning otherwise */
#define E_CRITICAL		0x02000000UL
#define E_WARNING_STRICT 0x04000000UL /* error if strict checking,else warning */
#define E_VALUE			0x00010000UL
#define E_SEARCHPOS		0x00020000UL
#define E_ACCUMULATE	0x00040000UL
#define E_MULTIPLE		0x00080000UL
#define E_ERRNO			0x00100000UL
#define E_DEL_DOUBLE	0x00200000UL

#define E_SET_SGFINFO			(0xfff | ERROR)
#define E_OUTPUT	stdout			/* output channel for error messages */

/* masks for error handling */
#define M_ERROR_TYPE	0x70000000UL
#define M_ERROR_NUM		0x00000fffUL

/* defines of E_ERROR codes		**
** has to match *E_ERROR_mesg[]	*/

#define FE_TOO_MANY_FILES		(1UL | E_FATAL_ERROR)
#define FE_UNKNOWN_OPTION		(2UL | E_FATAL_ERROR)
#define FE_SOURCE_OPEN			(3UL | E_FATAL_ERROR | E_ERRNO)
#define FE_SOURCE_READ			(4UL | E_FATAL_ERROR | E_ERRNO)
#define FE_OUT_OF_MEMORY		(5UL | E_FATAL_ERROR)
#define W_SGF_IN_HEADER			(6UL | E_WARNING | E_CRITICAL | E_SEARCHPOS)
#define FE_NO_SGFDATA			(7UL | E_FATAL_ERROR)
#define E_ILLEGAL_OUTSIDE_CHAR	(8UL | E_ERROR | E_CRITICAL | E_SEARCHPOS | E_ACCUMULATE)
#define E_ILLEGAL_OUTSIDE_CHARS	(8UL | E_ERROR | E_CRITICAL | E_SEARCHPOS | E_ACCUMULATE | E_MULTIPLE)
#define E_VARIATION_NESTING		(9UL | E_ERROR | E_CRITICAL | E_SEARCHPOS)
#define E_UNEXPECTED_EOF		(10UL | E_ERROR | E_CRITICAL | E_SEARCHPOS)

#define E_PROPID_TOO_LONG		(11UL | E_ERROR | E_CRITICAL | E_SEARCHPOS)
#define E_EMPTY_VARIATION		(12UL | E_ERROR | E_SEARCHPOS)
#define E_TOO_MANY_VALUES		(13UL | E_ERROR | E_CRITICAL | E_SEARCHPOS)
#define E_BAD_VALUE_DELETED		(14UL | E_ERROR | E_SEARCHPOS | E_VALUE)
#define E_BAD_VALUE_CORRECTED	(15UL | E_ERROR | E_SEARCHPOS | E_VALUE)
#define E4_BAD_VALUE_CORRECTED	(15UL | E_ERROR4 | E_SEARCHPOS | E_VALUE)
#define E_LC_IN_PROPID			(16UL | E_ERROR | E_SEARCHPOS)
#define E_EMPTY_VALUE_DELETED	(17UL | E_ERROR | E_SEARCHPOS)
#define W_EMPTY_VALUE_DELETED	(17UL | E_WARNING | E_SEARCHPOS)
#define E_BAD_ROOT_PROP			(18UL | E_ERROR | E_SEARCHPOS)
#define WCS_GAME_NOT_GO			(19UL | E_WARNING_STRICT | E_CRITICAL | E_SEARCHPOS)
#define E_NO_PROP_VALUES		(20UL | E_ERROR | E_CRITICAL | E_SEARCHPOS)

#define E_VARIATION_START		(21UL | E_ERROR | E_CRITICAL | E_SEARCHPOS)
#define W_CTRL_BYTE_DELETED		(22UL | E_WARNING | E_SEARCHPOS)
#define E_COMPOSE_EXPECTED		(23UL | E_ERROR | E_SEARCHPOS | E_VALUE)
#define WS_MOVE_IN_ROOT			(24UL | E_WARNING_STRICT | E_SEARCHPOS)
#define E_BAD_COMPOSE_CORRECTED	(25UL | E_ERROR | E_SEARCHPOS | E_VALUE)
#define FE_DEST_FILE_OPEN		(26UL | E_FATAL_ERROR | E_ERRNO)
#define FE_DEST_FILE_WRITE		(27UL | E_FATAL_ERROR | E_ERRNO)
#define E_DOUBLE_PROP			(28UL | E_ERROR | E_SEARCHPOS)
#define W_PROPERTY_DELETED		(29UL | E_WARNING | E_SEARCHPOS)
#define E4_MOVE_SETUP_MIXED		(30UL | E_ERROR4  | E_SEARCHPOS)

#define WS_LONG_PROPID			(31UL | E_WARNING_STRICT | E_SEARCHPOS)
#define E_ROOTP_NOT_IN_ROOTN	(32UL | E_ERROR | E_SEARCHPOS)
#define E4_FAULTY_GC			(33UL | E_ERROR4 | E_SEARCHPOS | E_VALUE)
#define E_CRITICAL_NOT_SAVED	(34UL | E_ERROR)
#define WS_UNKNOWN_PROPERTY		(35UL | E_WARNING_STRICT | E_SEARCHPOS)
#define E_MISSING_SEMICOLON		(36UL | E_ERROR | E_CRITICAL | E_SEARCHPOS)
#define E_TWO_MOVES_IN_NODE		(37UL | E_ERROR | E_SEARCHPOS)
#define E_POSITION_NOT_UNIQUE	(38UL | E_ERROR | E_SEARCHPOS | E_VALUE | E_DEL_DOUBLE)
#define WS_ADDSTONE_REDUNDANT	(39UL | E_WARNING_STRICT|E_SEARCHPOS|E_DEL_DOUBLE)
#define WS_PROPERTY_NOT_IN_FF	(40UL | E_WARNING_STRICT | E_SEARCHPOS)

#define E_ANNOTATE_NOT_UNIQUE	(41UL | E_ERROR | E_SEARCHPOS)
#define E4_BM_TE_IN_NODE		(42UL | E_ERROR4 | E_SEARCHPOS)
#define E_ANNOTATE_WITHOUT_MOVE (43UL | E_ERROR | E_SEARCHPOS)
#define E4_GINFO_ALREADY_SET	(44UL | E_ERROR4 | E_SEARCHPOS)
#define WS_ROOT_PROP_DIFFERS	(45UL | E_WARNING_STRICT | E_SEARCHPOS)
#define FE_UNKNOWN_FILE_FORMAT	(46UL | E_FATAL_ERROR | E_SEARCHPOS)
#define E_SQUARE_AS_RECTANGULAR	(47UL | E_ERROR | E_SEARCHPOS)
#define FE_MISSING_SOURCE_FILE	(48UL | E_FATAL_ERROR)
#define FE_BAD_PARAMETER		(49UL | E_FATAL_ERROR)
#define E_BOARD_TOO_BIG			(50UL | E_ERROR | E_SEARCHPOS)

#define E_VERSION_CONFLICT		(51UL | E_ERROR | E_CRITICAL | E_SEARCHPOS)
#define E_BAD_VW_VALUES			(52UL | E_ERROR | E_SEARCHPOS)
#define FE_DEST_NAME_TOO_LONG	(53UL | E_FATAL_ERROR)
#define E_VALUES_WITHOUT_ID		(54UL | E_ERROR | E_CRITICAL | E_SEARCHPOS)
#define W_EMPTY_NODE_DELETED	(55UL | E_WARNING | E_SEARCHPOS)
#define W_VARLEVEL_UNCERTAIN	(56UL | E_WARNING | E_SEARCHPOS)
#define W_VARLEVEL_CORRECTED	(57UL | E_WARNING | E_SEARCHPOS)
#define WS_ILLEGAL_MOVE			(58UL | E_WARNING_STRICT | E_SEARCHPOS)
#define W_INT_KOMI_FOUND		(59UL | E_WARNING | E_SEARCHPOS)

#define E_MORE_THAN_ONE_TREE	(60UL | E_ERROR)
#define W_HANDICAP_NOT_SETUP	(61UL | E_WARNING | E_SEARCHPOS)
#define W_SETUP_AFTER_ROOT		(62UL | E_WARNING | E_SEARCHPOS)
#define W_MOVE_OUT_OF_SEQUENCE	(63UL | E_WARNING | E_SEARCHPOS)
#define E_TOO_MANY_VARIATIONS	(64UL | E_ERROR | E_SEARCHPOS)
#define E_FF4_PASS_IN_OLD_FF	(65UL | E_ERROR | E_SEARCHPOS)
#define E_NODE_OUSIDE_VAR		(66UL | E_ERROR | E_CRITICAL | E_SEARCHPOS)
#define E_MISSING_NODE_START	(67UL | E_ERROR | E_CRITICAL | E_SEARCHPOS)
#define FE_UNKNOWN_LONG_OPTION	(68UL | E_FATAL_ERROR)

#define MAX_ERROR_NUM			68
