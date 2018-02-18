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

void pkm_visualize_kmeans(pkm_centroid **centrs, unsigned int dim1, unsigned int dim2)
{
	unsigned int height = 300;
	unsigned int width = 500;

	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
	cairo_t *cr = cairo_create(surface); // context

	cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);
	cairo_set_line_width(cr, 2);

	// set up white background
	cairo_set_source_rgb(cr, 255, 255, 255);
	cairo_rectangle(cr, 0, 0, width, height);
	cairo_fill(cr);
	cairo_stroke(cr);

	pkm_draw_axes(cr, width, height);

	cairo_destroy(cr);
	cairo_surface_write_to_png(surface, "test.png");
	cairo_surface_destroy(surface);
}