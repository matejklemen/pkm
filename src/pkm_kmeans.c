#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

int main(int argc, char *argv[])
{
	printf("Parallel KMeans...\n");
	return 0;
}