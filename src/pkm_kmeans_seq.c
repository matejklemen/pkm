#include "../inc/pkm_kmeans_core.h"
#include "../inc/pkm_kmeans_visualization.h"

#define TERMINATION_PROPORTION 0.01

void pkm_kmeans_sequential(pkm_data_point **pts, size_t num_pts, pkm_centroid **centrs, size_t num_centrs)
{
	unsigned int min_changes = ceil(TERMINATION_PROPORTION * num_pts);
	unsigned int curr_changes = min_changes + 1;

	pkm_random_init(pts, num_pts, centrs, num_centrs);

	while(curr_changes > min_changes)
	{
		curr_changes = 0;

		for(int i = 0; i < num_pts; i++)
			curr_changes += pkm_centroid_assignment(pts[i], centrs, num_centrs);

		for(int i = 0; i < num_centrs; i++)
			pkm_centroid_update(centrs[i]);
	}

	pkm_visualize_kmeans(centrs, num_centrs, 0, 1);
}