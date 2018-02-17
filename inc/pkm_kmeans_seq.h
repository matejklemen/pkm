#ifndef PKM_KMEANS_SEQ
#define PKM_KMEANS_SEQ

#include "../inc/pkm_kmeans_core.h"

void pkm_kmeans_sequential(pkm_data_point **pts, size_t num_pts, pkm_centroid **centrs, size_t num_centrs);

#endif