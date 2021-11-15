#include "limits.h"

/**
  * @file limits.c
  * @date 13 Dec 2020 
  * @brief //TODO
 
  * //TODO Long description
  * @see https://github.com/scanoss/engine/blob/master/src/limits.c
  */

int scan_limit = 25; /** //TODO */

int consecutive_score = 4000;   /** //TODO */
int consecutive_hits = 4;       /** //TODO */
int consecutive_jump = 5;       /** //TODO */
int consecutive_threshold = 50; /** //TODO */

int range_tolerance = 5;  /** A maximum number of non-matched lines tolerated inside a matching range */
int min_match_lines = 10; /** Minimum number of lines matched for a match range to be acepted */
int min_match_hits  = 5;  /** Minimum number of snippet ID hits to produce a snippet match*/

const int rank_items = 1000; /** Number of items to evaluate in component and path rankings */

const int max_vulnerabilities = 50; /** Show only the first N vulnerabilities */
