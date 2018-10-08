/***********************************************************************
		Created on	09/17/90 by CAD/CAM Systems Dept.
		Last updated on 01/30/94 by CAD/CAM Systems Dept.
		Copyright 1990,1991,1992,1993,1994 Nihon UNISYS EXCELUTIONS, Ltd.

		CRESTAM System Header file  (Crux.h)

***********************************************************************/
#ifndef	_TYPEDEF_
#define	_TYPEDEF_


#ifndef	INT
typedef	int	INT;
#endif
#ifndef	FLOAT
typedef	float			FLOAT;
#endif
#ifndef	DOUBLE
typedef	double			DOUBLE;
#endif
#ifndef	CHAR
typedef	char			CHAR;
#endif
#ifndef	SHORT
typedef	short		int	SHORT;
#endif
#ifndef	UINT
typedef	unsigned int	UINT;
#endif
#ifndef	USHORT
typedef	unsigned short	int	USHORT;
#endif
#ifndef	UCHAR
typedef	unsigned char		UCHAR;
#endif

#ifndef	VOID
#define	VOID			void
#endif

#ifdef	IBM
#ifdef abs
#undef abs
#endif
#define	abs(a)		( (a)<0   ? (-(a)) : (a) )
#else
//#define	abs(a)		( (a)<0   ? (-(a)) : (a) )
#endif	/* IBM */

#define	D3OFF		(0)		/* $OFF		*/
#define	D3ON		(1)		/* $ON		*/
#define	D3HDN		(2)		/* $HDN		*/


#endif	/* _TYPEDEF_ */
