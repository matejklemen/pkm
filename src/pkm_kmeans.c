#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PKM_UNASSIGNED_CENTROID -1
#define PKM_DEFAULT_NUM_MEMBERS 32

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
	pkm_data_point **members;
	int num_members;
	int max_num_members;
	pkm_datatype *center;
} pkm_centroid;

void pkm_print_data_point(pkm_data_point *pt)
{
	printf("[");
	for(int i = 0; i < pt->vec->vec_len; i++)
		printf(" %f ", pt->vec->data[i]);
	printf("] [C:%d]\n", pt->cluster_id);
}

void pkm_print_centroid(pkm_centroid *centr)
{
	printf("Cluster %d, center: [", centr->cluster_id);
	// again assuming that all data points are of equal dimensions
	for(int i = 0; i < centr->members[0]->vec->vec_len; i++)
		printf(" %f ", centr->center[i]);
	printf("]\n");

	for(int i = 0; i < centr->num_members; i++)
	{
		printf("\t");
		pkm_print_data_point(centr->members[i]);
	}
}

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
		Don't forget to free the data at the end (dealt with in pkm_free_data_point(...)):
		- first, free(pt->vec->data)
		- second, free(pt->vec)
		- third, free(pt)
	*/
	pkm_datatype *data = malloc(sizeof(pkm_datatype) * vec_len);

	pkm_vector *vec = malloc(sizeof(pkm_vector));
	vec->data = data;
	vec->vec_len = vec_len;

	pkm_data_point *pt = malloc(sizeof(pkm_data_point));

	// on init, data points are not assigned to a cluster
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

pkm_centroid *pkm_create_centroid(int cluster_id, int num_members, size_t vec_len)
{
	/* Convention: if num_members is <=0, preallocate an array of default size (PKM_DEFAULT_NUM_MEMBERS) for centroid members.*/
	if(num_members <= 0)
		num_members = PKM_DEFAULT_NUM_MEMBERS;

	/*
		Don't forget to free the data at the end (dealt with in pkm_free_centroid(...)):
		- first, free(centr->members)
		- second, free(centr->center)
		- third, free(centr)
	*/

	// allocate centroid
	pkm_centroid *centr = malloc(sizeof(pkm_centroid));

	// allocate array of pointers to members of centroid
	pkm_data_point **members = malloc(sizeof(pkm_data_point *) * num_members);

	// allocate space for array that represents the center of centroid
	pkm_datatype *center_point = malloc(sizeof(pkm_datatype) * vec_len);

	centr->cluster_id = cluster_id;
	centr->members = members;
	centr->num_members = 0;
	centr->max_num_members = num_members;
	centr->center = center_point;

	return centr;
}

void pkm_free_centroid(pkm_centroid *centr)
{
	free(centr->members);
	free(centr->center);
	free(centr);
}

int main(int argc, char *argv[])
{
	printf("Parallel KMeans...\n");
	return 0;
}