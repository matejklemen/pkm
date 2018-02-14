#include <stdio.h>
#include <stdlib.h>

typedef float pkm_datatype;
typedef struct pkm_data_point
{
	// which centroid this point belongs to
	int cluster_id;
	pkm_datatype *data;
} pkm_data_point;

typedef struct pkm_centroid
{
	int cluster_id;
	pkm_data_point *members;
	pkm_datatype *center;
} pkm_centroid;

int main(int argc, char *argv[])
{
	printf("Parallel KMeans...\n");
	return 0;
}