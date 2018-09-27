// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
 *	@file		stk_uuid.h
 *  @brief		uuid
 *	@par ¿˙ ∑º«¬º
 *				2016/10/28 ¥¥Ω®°£MengYaohui(Hoteamsoft)
**************************************************************************/
#ifndef _SVLLIB_UUID_H_
#define _SVLLIB_UUID_H_
	
namespace stk
{
	namespace uuid
	{
		/** @brief	uuid ˝æ›Ω·ππ	*/
		typedef struct _uuid_t
        {
            unsigned long		data1;
			unsigned short		data2;
			unsigned short		data3;
			unsigned char		data4[8];
		} uuid_t;
		/** @brief	 ±º‰Ω·ππ	*/
		typedef struct _timestamp_t
		{
			char	tm_sec;					/* Seconds after minute (0 ®C 59). */
			char	tm_min;					/* Minutes after hour (0 ®C 59). */
			char	tm_hour;				/* Hours after midnight (0 ®C 23). */
			char	tm_mday;				/* Day of month (1 ®C 31). */
			char	tm_mon;					/* Month (0 ®C 11; January = 0). */
			char	tm_wday;				/* Day of week (0 ®C 6; Sunday = 0). */
			short	tm_year;				/* Year (current year minus 1900). */
			short	tm_yday;				/* Day of year (0 ®C 365; January 1 = 0). */
			long	tm_fraction;			/* Fraction little than 1 second */
		} timestamp_t;

		/**
		 * Checks whether the given string matches the UUID format.
		 *    params:
		 *     [in] uuid - the potential UUID string
		 *    return 
		 *     TRUE if the given string is a UUID, FALSE otherwise
		 **/
		bool is_uuid_string(const char *uuid);

		/**
		 * Generates a new UUID. The UUID is a time-based time 1 UUID.
		 * A random per-process node identifier is used to avoid keeping global
		 * state and maintaining inter-process synchronization.
		 **/
		void uuid_create(uuid_t* uuid);

		/**
		 * Generates a new UUID string. The returned UUID is a time-based time 1 UUID.
		 * A random per-process node identifier is used to avoid keeping global
		 * state and maintaining inter-process synchronization.
		 *  return UUID string (newly allocated)
		 **/
		char *uuid_create_string(void);

		/**
		 * Generates a name-based (type 3) UUID string from the given external
		 * identifier. The special namespace UUID is used as the namespace of
		 * the generated UUID.
		 *  params
		 *     [in] external - the external identifier
		 *  return 
		 *     UUID string (newly allocated)
		 **/
		void uuid_create_external(const char *external, uuid_t* uuid);

		/**
		 * Translate a uuid_t to a uuid string
		 *  return UUID string
		 **/
		char *uuid_to_string(const uuid_t* uuid);

		/**
		 * Get timestamp from a UUID
		 **/
		void uuid_to_timestamp(const uuid_t* uuid, timestamp_t* time);


		/**
		 * Resurn a description of timestamp NOT including fraction
		 **/
		char* timestamp_to_string(const timestamp_t* time);

		/**
		 * Compare two UUID's lexically
		 *    return
		 *      -1   u1 is lexically before u2
		 *     0   u1 is equal to u2
		 *     1   u1 is lexically after u2
		*/
		int uuid_compare(const uuid_t *u1, const uuid_t *u2);

		/**
		 * Compare two UUID's temporally
		 *    return
		 *      -1   u1 is temporally before u2
		 *     0   u1 is equal to u2
		 *     1   u1 is temporally after u2
		*/
		int uuid_compare_time(const uuid_t *u1, const uuid_t *u2);
	}
}

#endif
