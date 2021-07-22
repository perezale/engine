// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * src/query.c
 *
 * High level data queries
 *
 * Copyright (C) 2018-2021 SCANOSS.COM
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "query.h"
#include "util.h"
#include "time.h"
#include "limits.h"
#include "ldb.h"
#include "scanoss.h"
#include "decrypt.h"
#include "ldb_client.h"
/* Obtain the first file name for the given file MD5 hash */
char *get_filename(char *md5)
{
	/* Convert md5 to bin */
	uint8_t md5bin[MD5_LEN];
	ldb_hex_to_bin(md5, MD5_LEN * 2, md5bin);

	/* Init record */
	uint8_t *record = calloc(LDB_MAX_REC_LN + 1, 1);

	/* Fetch first record */
	ldb_get_first_record(oss_file, md5bin, (void *) record);

	uint32_t recln = uint32_read(record);
	if (record)
	{
		memmove(record, record + 4, recln);
		record[recln] = 0;

		/* Decrypt data */
		decrypt_data(record, recln, "file", md5bin, md5bin + LDB_KEY_LN);
	}

	return (char *)record;
}

/* Handler function for get_url_record */
bool ldb_get_first_url_not_ignored(uint8_t *key, uint8_t *subkey, int subkey_ln, uint8_t *data, uint32_t datalen, int iteration, void *ptr)
{
	decrypt_data(data, datalen, "url", key, subkey);

	uint8_t *record = (uint8_t *) ptr;

	if (datalen) if (!ignored_asset_match(data + 4))
	{
		/* Not ignored, means copy record and exit */
		uint32_write(record, datalen);
		memcpy(record + 4, data, datalen);
		record[datalen + 4 + 1] = 0;
		return true;
	}

	return false;
}

/* Obtain the first available component record for the given MD5 hash */
void get_url_record(uint8_t *md5, uint8_t *record)
{
	/* Erase byte count */
	uint32_write(record, 0);


#ifndef LDB_CLIENT_MODE
	/* Fetch record */
	ldb_fetch_recordset(NULL, oss_url, md5, false, ldb_get_first_url_not_ignored, (void *) record);

	/* Erase record length prefix from record */
	uint32_t recln = uint32_read(record);
	if (recln) 
	{
		memmove(record, record+4, recln);
		record[recln] = 0;
	}
#else
		engine_funtion_t f;
		f.type = LDB_GET_RECORDS;
		f.table = TABLE_URLS;
		f.records_qty = 10;
		memset(f.key,0,sizeof(f.key));
		memcpy(f.key, md5, sizeof(f.key));
		ldb_response_t r = ldb_socket_request(LDB_HOST, LDB_PORT, &f);
		ldb_recorset_buffet_t buffer = {.index = (uint16_t*) r.response, .records = (ldb_recorset_t *) (r.response + sizeof(uint16_t))};

		for (uint32_t i = 0; i < *buffer.index; i++)
		{
			if(!ignored_asset_match(buffer.records[i].data))
			{
				memcpy(record, buffer.records[i].data, buffer.records[i].len);
				break;
			}
		}
#endif



}

/* Extracts component age in seconds from created date (1st CSV field in data) */
bool handle_get_component_age(uint8_t *key, uint8_t *subkey, int subkey_ln, \
uint8_t *data, uint32_t datalen, int iteration, void *ptr)
{
	long *age = (long *) ptr;

	decrypt_data(data, datalen, "purl", key, subkey);

	/* Expect at least a date*/
	if (datalen < 9) return false;

	/* Ignore purl relation records */
	if (!memcmp(data, "pkg:", 4)) return false;

	/* Extract created date (1st CSV field) from popularity record */
	char date[MAX_FIELD_LN] = "\0";
	extract_csv(date, (char *) data, 1, MAX_FIELD_LN);

	/* Expect date separators. Format 2009-03-21T22:32:25Z */
	if (date[4] != '-' || date[7] != '-') return false;

	/* Chop date string into year, month and day strings */
	date[4] = 0;
	date[7] = 0;
	date[10] = 0;

	/* Extract year */
	int year = atoi(date) - 1900;
	if (year < 0) return false;

	/* Extract year */
	int month = atoi(date + 5) - 1;
	if (month < 0) return false;

	/* Extract year */
	int day = atoi(date + 8);
	if (day < 0) return false;

	/* Fill time structure */
	struct tm t;
	time_t epoch;
	t.tm_year = year;
	t.tm_mon = month;
	t.tm_mday = day;
	t.tm_hour = 0;
	t.tm_min = 0;
	t.tm_sec = 0;
	t.tm_isdst = 0;
	epoch = mktime(&t);

	/* Keep the oldest date in case there are multiple sources */
	long seconds = (long) time (NULL) - (long) epoch;
	if (seconds > *age) *age = seconds;

	return false;
}

/* Return the age of a component in seconds */
int get_component_age(uint8_t *md5)
{
	if (!md5) return 0;

	/* Define purl table */
	struct ldb_table purl;
	strcpy(purl.db, "oss");
	strcpy(purl.table, "purl");
	purl.key_ln = 16;
	purl.rec_ln = 0;
	purl.ts_ln = 2;
	purl.tmp = false;

	/* Fetch record */
	long age = 0;
	ldb_fetch_recordset(NULL, purl, md5, false, handle_get_component_age, &age);

	return age;
}
