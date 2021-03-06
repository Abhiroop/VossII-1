//-------------------------------------------------------------------
// Copyright 2020 Carl-Johan Seger
// SPDX-License-Identifier: Apache-2.0
//-------------------------------------------------------------------

/********************************************************************
*                                                                   *
*     Original author: Carl-Johan Seger 1993                        *
*                                                                   *
*********************************************************************/
/* rec.h -- header for rec.c */

#ifndef _REC_H
#define _REC_H
#include "types.h"
#include "alloc.h"

#define REC_MAGIC_NBR	12349876
#define NAT_BLOCK_SIZE	4050	/* Leave some room for malloc's header */

typedef struct rec_struct {
	int		initialized;	/* Debugging help		   */
	unint		alignment;	/* Alignment requirement	   */
	unint		rec_size;	/* Size of each record		   */
	unint		blk_size;	/* Size of each block		   */
	unint		capacity;	/* Number of records in each block */
	pointer		first_blk;	/* First block allocated 	   */
	pointer		last_blk;	/* Last block allocated 	   */
	pointer		free_rec;	/* Free list of records 	   */
	unint		allocated;	/* Records allocated in last block */
	pointer 	cur_blk;	/* Used in FOR_REC macro	   */
	unint 		cur_rec;	/* Used in FOR_REC macro	   */
	bool 		pure;		/* Used in FOR_REC macro	   */
} rec_mgr, *rec_mgr_ptr;

#define FWD_BLK_PTR(p)	((*((pointer *) (p))))
#define BACK_BLK_PTR(p)	((*(((pointer *) (p))+1)))
#define REC_ADDR(blk,i)	((blk) + ((2*POINTER_SIZE) + ((i)*mp->rec_size)))


void	new_mgr(rec_mgr_ptr mp, unint size);
void	aligned_new_mgr(rec_mgr_ptr mp, unint size, unint alignment);
void	free_mgr(rec_mgr_ptr mp);
pointer	new_rec(rec_mgr_ptr mp);
int	ensure_pure_mgr(rec_mgr_ptr mp, string file, int line);
bool	owned_by_mgr(rec_mgr_ptr mp, pointer item);
void	free_rec(rec_mgr_ptr mp, pointer r);

#define FOR_REC(mp, type, Tp) for( ensure_pure_mgr(mp,__FILE__,__LINE__),   \
	     (mp)->cur_blk = (mp)->first_blk;				    \
	     (mp)->cur_blk != (pointer) NULL;				    \
	     (mp)->cur_blk = (pointer) FWD_BLK_PTR((mp)->cur_blk))	    \
	    for((mp)->cur_rec = 0,					    \
		Tp = (type) ((mp)->cur_blk + (2*POINTER_SIZE));		    \
		((mp)->cur_blk == (mp)->last_blk)?			    \
			(mp)->cur_rec < (mp)->allocated :		    \
			(mp)->cur_rec < (mp)->capacity;			    \
		(mp)->cur_rec++,					    \
		Tp = (type) (((mp)->cur_blk) +				    \
		     ((2*POINTER_SIZE) + (((mp)->cur_rec)*((mp)->rec_size)))))


#endif
