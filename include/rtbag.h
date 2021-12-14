/*
 ****************************************************************
 *                                                              *
 *      Module: rtbag.h                                         *
 *      Description: Run Time Bag Description                   *
 *      Version: 1.0 of 06/Jun/95                               *
 *      Author: Simon Gay and Juarez Muylaert                   *
 *                                                              *
 ****************************************************************
 */

# define	LOCKED		0x8000
# define        UNLOCKED	0x7fff

# define        LOCK_NODE(x)	(x |= LOCKED)
# define        UNLOCK_NODE(x)	(x &= UNLOCKED)
# define        NODE_LOCKED(x)	((x & LOCKED) == LOCKED)

# define 	NODE_TYPE(x)	(x & 0x7fff)

# define	TY_INT		0x000
# define	TY_BOOL		0x01
# define	TY_TUPLE	0x02
# define 	TY_IDENT	0x03

/* The run-time bag */

struct rt_bag {
	int rtb_tag;
	union rt_element *rtb_element;
	struct rt_bag *rtb_next;
	struct rt_bag *rtb_prev;
};

typedef struct rt_bag RT_BAG;

/* An element of a bag */

union rt_element {
	int rte_val;
	char *rte_name;            /* for patterns */
	struct rt_tuple *rte_tuple;
};

typedef union rt_element RT_ELEMENT;

struct rt_value {
	union rt_element *rtv_val;
	char *rtv_name;
	struct rt_value *rtv_next;
};

typedef struct rt_value RT_VALUE;

/* The element of a bag is a tuple */

struct rt_tuple {
	int rtt_size;
	struct rt_list *rtt_list;
};

typedef struct rt_tuple RT_TUPLE;

struct rt_list {
	int rtl_tag;
	union rt_element *rtl_element;
	struct rt_list *rtl_next;
};

typedef struct rt_list RT_LIST;

