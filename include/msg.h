
/*
 ****************************************************************
 *                                                              *
 *      Module: msg.h                                           *
 *      Description: The various messages tags                  *
 *      Version: 1.0 of 07/Aug/95                               *
 *      Author: Simon Gay and Juarez Muylaert                   *
 *                                                              *
 ****************************************************************
 */


# define	BAG_REQUEST		1         /* request a bag */
# define	BAG_RESPONSE		2         /* answer request */
# define	BAG_ADDRESS             3         /* address for copying   */
# define    	BAG_SIZE                4         /* size for copying */
# define    	BAG_DATA                5         /* contents of bag */
# define        BR_DOIT                 6         /* basic reaction requested to execute */
# define     	BR_RESPONSE             7         /* basic reaction response */
# define        BAG_SEND                8         /* sending a bag */
# define        BAG_PRINT               9         /* instruction to print the bag */
# define        BAG_INT                10
# define        BAG_END                11
# define        BAG_BOOL               12
# define        BAG_TUPLE_SIZE         13
# define        DIE                    14
