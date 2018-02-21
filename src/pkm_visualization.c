#include <cairo.h>
#include "../inc/pkm_kmeans_seq.h"

#define SPACE_EMPTY 25

void pkm_draw_axes(cairo_t *context, unsigned int width, unsigned int height)
{
	cairo_set_source_rgb(context, 0, 0, 0);
	cairo_set_line_width(context, 3);

	// x-axis
	cairo_move_to(context, 0 + SPACE_EMPTY, height / 2);
	cairo_line_to(context, width - SPACE_EMPTY, height / 2);
	// x-axis label
	cairo_show_text(context, "x1\0");

	// y-axis
	cairo_move_to(context, width / 2, 0 + SPACE_EMPTY);
	cairo_line_to(context, width / 2, height - SPACE_EMPTY);
	// y-axis label
	cairo_move_to(context, width / 2, 0 + SPACE_EMPTY);
	cairo_show_text(context, "x2\0");

	cairo_stroke(context);
}

void pkm_draw_point(cairo_t *context, unsigned int thickness,
					unsigned int pt_x, unsigned int pt_y,
					double color_r, double color_g, double color_b)
{
	cairo_set_source_rgb(context, color_r, color_g, color_b);
	cairo_arc(context, pt_x, pt_y, thickness, 0.0, 2 * M_PI);
	cairo_fill(context);
}

void pkm_visualize_kmeans(pkm_centroid **centrs, size_t num_centrs, unsigned int dim1, unsigned int dim2)
{
	unsigned int height = 300;
	unsigned int width = 500;
	unsigned int center_x = width / 2;
	unsigned int center_y = height / 2;

	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
	cairo_t *cr = cairo_create(surface);

	cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);
	cairo_set_line_width(cr, 2);

	// set up white background
	cairo_set_source_rgb(cr, 255, 255, 255);
	cairo_rectangle(cr, 0, 0, width, height);
	cairo_fill(cr);

	pkm_draw_axes(cr, width, height);

	// draw centroids
	for(int i = 0; i < num_centrs; i++)
		pkm_draw_point(cr, 5, center_x - centrs[i]->center[dim1] * 10, center_y - centrs[i]->center[dim2] * 10, 1.0 / i, i / num_centrs, 1 - i / num_centrs);

	//pkm_draw_point(cr, 2, center_x + 50, center_y + 50, 0.5, 0, 0);

	cairo_destroy(cr);
	cairo_surface_write_to_png(surface, "test.png");
	cairo_surface_destroy(surface);
}