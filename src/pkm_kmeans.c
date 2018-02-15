#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PKM_UNASSIGNED_CENTROID -1

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
} pkm_data_point;

typedef struct pkm_centroid
{
	int cluster_id;
	pkm_data_point *members;
	int num_members;
	pkm_datatype *center;
} pkm_centroid;

/* Euclidean distance */
pkm_datatype pkm_compute_dist(pkm_vector *v1, pkm_vector *v2)
{
	pkm_datatype dist = 0;

	// currently assuming vector sizes are equal
	for(int i = 0; i < v1->vec_len; i++)
		dist += pow(v1->data[i] - v2->data[i], 2);

	return sqrt(dist);
}

pkm_data_point *pkm_create_data_point(size_t vec_len)
{
	/*
		Don't forget to free the data at the end (dealt with in
		free_data_point(...)):
		- first, free(pt->vec->data)
		- second, free(pt->vec)
		- third, free(pt)
	*/
	pkm_datatype *data = malloc(sizeof(pkm_datatype) * vec_len);

	pkm_vector *vec = malloc(sizeof(pkm_vector));
	vec->data = data;
	vec->vec_len = vec_len;

	pkm_data_point *pt = malloc(sizeof(pkm_data_point));

	pt->cluster_id = PKM_UNASSIGNED_CENTROID;
	pt->vec = vec;

	return pt;
}

void pkm_free_data_point(pkm_data_point *pt)
{
	free(pt->vec->data);
	free(pt->vec);
	free(pt);
}

int main(int argc, char *argv[])
{
	printf("Parallel KMeans...\n");
	return 0;
}