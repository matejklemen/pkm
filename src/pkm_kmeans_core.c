#include "../inc/pkm_kmeans_core.h"
#include <math.h>

#define PKM_UNASSIGNED_CENTROID -1
#define PKM_DEFAULT_NUM_MEMBERS 32

void pkm_print_data_point(pkm_data_point *pt)
{
	// encountered an empty space in the array
	if(pt == NULL)
		return;

	printf("[");
	for(int i = 0; i < pt->vec->vec_len; i++)
		printf(" %f ", pt->vec->data[i]);
	printf("] [C:%d]\n", pt->cluster_id);
}

void pkm_print_centroid(pkm_centroid *centr)
{
	printf("Cluster %d, center: [", centr->cluster_id);
	// again assuming that all data points are of equal dimensions
	for(int i = 0; i < centr->center_len; i++)
		printf(" %f ", centr->center[i]);
	printf("]\n");

	for(int i = 0; i < centr->num_members; i++)
	{
		printf("\t");
		pkm_print_data_point(centr->members[i]);
	}
}

/*
	Copies data from 'src' to 'dst'.
	NOTE: does not perform any checks for out of bounds, that's the caller's duty.
*/
void pkm_copy_data(pkm_datatype *src, pkm_datatype *dst, size_t len)
{
	for(int i = 0; i < len; i++)
		dst[i] = src[i];
}

/* Euclidean distance */
pkm_datatype pkm_compute_dist(pkm_datatype *v1, pkm_datatype *v2, size_t vec_len)
{
	pkm_datatype dist = 0;

	// currently assuming vector sizes are equal
	for(int i = 0; i < vec_len; i++)
		dist += pow(v1[i] - v2[i], 2);

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
	pt->idx_arr = PKM_UNASSIGNED_CENTROID;

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
	// convention: if num_members is <=0, preallocate an array of default size (PKM_DEFAULT_NUM_MEMBERS) for centroid members
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
	centr->center_len = vec_len;

	return centr;
}

void pkm_free_centroid(pkm_centroid *centr)
{
	free(centr->members);
	free(centr->center);
	free(centr);
}

/*
	Adds 'new_pt' as a new member of centroid 'centr'. If members' list is full, allocate a bigger chunk of memory.
*/
void pkm_insert_member(pkm_centroid *centr, pkm_data_point *new_pt)
{
	/*
		Allocate twice as much memory for members as was allocated now.
		NOTE: might get out of hand when dealing with bigger amount of data
		(so think about how to allocate a more sensible amount of memory).
	*/
	if(centr->num_members == centr->max_num_members)
	{
		pkm_data_point **new_members = realloc(centr->members, centr->max_num_members << 1);

		centr->members = new_members;
		centr->max_num_members = centr->max_num_members << 1;
	}

	new_pt->idx_arr = centr->num_members;
	centr->members[centr->num_members] = new_pt;
	centr->num_members += 1;
}

void pkm_remove_member(pkm_centroid *centr, int idx)
{
	centr->members[idx] = NULL;
}

/* Find the centroid that is closest to data point 'pt'. */
void pkm_centroid_assignment(pkm_data_point *pt, pkm_centroid **centrs, size_t num_centrs)
{
	if(num_centrs == 0)
		return;

	int assigned_id;
	pkm_datatype min_dist, curr_dist;

	/*
		Avoid setting initial min distance to some sentinel value by either:
		- setting it to distance of point to current assigned centroid or
		- centroid 0 (if point has no assigned centroid yet)
	*/
	if(pt->cluster_id != PKM_UNASSIGNED_CENTROID)
	{
		assigned_id = pt->cluster_id;
		min_dist = pkm_compute_dist(pt->vec->data, centrs[pt->cluster_id]->center, pt->vec->vec_len);
	}
	else
	{
		assigned_id = 0;
		min_dist = pkm_compute_dist(pt->vec->data, centrs[0]->center, pt->vec->vec_len);
	}

	// find closest centroid
	for(int i = 0; i < num_centrs; i++)
	{
		curr_dist = pkm_compute_dist(pt->vec->data, centrs[i]->center, pt->vec->vec_len);

		if(curr_dist < min_dist)
		{
			min_dist = curr_dist;
			assigned_id = i;
		}
	}

	if(pt->cluster_id != PKM_UNASSIGNED_CENTROID)
		pkm_remove_member(centrs[pt->cluster_id], pt->idx_arr);
	
	pt->cluster_id = assigned_id;
	pkm_insert_member(centrs[assigned_id], pt);
}

void pkm_centroid_update(pkm_centroid *centr)
{
	int num_actual_points = 0;

	// reset center of centroid
	for(int i = 0; i < centr->center_len; i++)
		centr->center[i] = 0.0f;

	for(int i = 0; i < centr->num_members; i++)
	{
		if(centr->members[i] != NULL)
		{
			num_actual_points += 1;
			for(int j = 0; j < centr->center_len; j++)
				centr->center[j] += centr->members[i]->vec->data[j];
		}
	}

	// empty centroid, avoid dividing by zero
	if(num_actual_points == 0)
		return;

	for(int i = 0; i < centr->center_len; i++)
		centr->center[i] /= num_actual_points;
}

/*
	Assigns a data point as a center of a centroid (for each centroid).
	NOTE: if you want seeded random generation, set a seed before calling the function.
*/
void pkm_random_init(pkm_data_point **pts, unsigned int num_data_points, pkm_centroid **centrs, unsigned int num_centrs)
{
	/*
		Ad-hoc random number generation (without repetition).
		The downside of this approach is that this can't be parallelized
		(cluster initialization i dependent on cluster initialization (i - 1)).
		Another downside is that in practice, the last few generated numbers are
		very likely to be ..., (num_data_points - 2), (num_data_points - 1).
		The good thing about this approach is not having to allocate a 'boolean' array
		or shuffle an array to randomly generate numbers without repetition.
	*/

	unsigned int generated_idx = rand() % (num_data_points - num_centrs + 1);
	pkm_copy_data(pts[generated_idx]->vec->data, centrs[0]->center, centrs[0]->center_len);

	for(int i = 1; i < num_centrs; i++)
	{
		generated_idx = 1 + generated_idx + rand() % (num_data_points - generated_idx - num_centrs + i);
		pkm_copy_data(pts[generated_idx]->vec->data, centrs[i]->center, centrs[i]->center_len);
	}
}