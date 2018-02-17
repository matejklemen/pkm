#include "../inc/pkm_kmeans_seq.h"

void kmeans_sequential_demo1(size_t num_pts, size_t num_dims, size_t num_centrs)
{
	pkm_data_point **pts = malloc(sizeof(pkm_data_point *) * num_pts);
	pkm_centroid **centrs = malloc(sizeof(pkm_centroid *) * num_centrs);

	// create data points
	for(int i = 0; i < num_pts; i++)
	{
		pts[i] = pkm_create_data_point(num_dims);

		// fill up vectors with random 'integers' (converted to floats) in interval [0...9]
		for(int j = 0; j < num_dims; j++)
		{
			int rand_num = rand() % 10;
			pts[i]->vec->data[j] = rand_num;
		}
	}

	// create centroids
	for(int i = 0; i < num_centrs; i++)
		centrs[i] = pkm_create_centroid(i, 0, num_dims);

	// run sequential kmeans
	pkm_kmeans_sequential(pts, num_pts, centrs, num_centrs);

	// print out the centroids
	for(int i = 0; i < num_centrs; i++)
		pkm_print_centroid(centrs[i]);

	// clean up
	for(int i = 0; i < num_pts; i++)
		pkm_free_data_point(pts[i]);

	for(int i = 0; i < num_centrs; i++)
		pkm_free_centroid(centrs[i]);

	free(pts);
}

int main()
{
	kmeans_sequential_demo1(10, 2, 3);

	return 0;
}