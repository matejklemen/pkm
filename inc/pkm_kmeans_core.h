#ifndef PKM_KMEANS_H
#define PKM_KMEANS_H

#include <stdlib.h>
#include <stdio.h>

typedef float pkm_datatype;

typedef struct pkm_vector
{
	pkm_datatype *data;
	size_t vec_len;
} pkm_vector;

typedef struct pkm_data_point
{
	// which centroid this point belongs to
	int cluster_id;
	pkm_vector *vec;
	// index on which this data point is stored in some centroid's members array
	int idx_arr;
} pkm_data_point;

typedef struct pkm_centroid
{
	int cluster_id;
	pkm_data_point **members;
	int num_members;
	int max_num_members;
	pkm_datatype *center;
	size_t center_len;
} pkm_centroid;

void pkm_print_data_point(pkm_data_point *pt);
void pkm_print_centroid(pkm_centroid *centr);

pkm_datatype pkm_compute_dist(pkm_datatype *v1, pkm_datatype *v2, size_t vec_len);

pkm_data_point *pkm_create_data_point(size_t vec_len);
void pkm_free_data_point(pkm_data_point *pt);

pkm_centroid *pkm_create_centroid(int cluster_id, int num_members, size_t vec_len);
void pkm_free_centroid(pkm_centroid *centr);

void pkm_centroid_assignment(pkm_data_point *pt, pkm_centroid **centrs, size_t num_centrs);
void pkm_centroid_update(pkm_centroid *centr);

void pkm_random_init(pkm_data_point **pts, unsigned int num_data_points, pkm_centroid **centrs, unsigned int num_centrs);

#endif