/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "limine.h"

__attribute__((used, section(".limine_requests"))) static volatile uint64_t
	limine_base_revision[] = LIMINE_BASE_REVISION(6);

__attribute__((used, section(".limine_requests"))) static volatile struct
	limine_framebuffer_request framebuffer_request = {
		.id	  = LIMINE_FRAMEBUFFER_REQUEST_ID,
		.revision = 0
	};

__attribute__((used,
	section(".limine_requests_start"))) static volatile uint64_t
	limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end"))) static volatile uint64_t
	limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

static void
hcf(void)
{
	for (;;)
		asm("hlt");
}

void
kmain(void)
{
	if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false)
	{
		hcf();
	}

	if (framebuffer_request.response == NULL ||
		framebuffer_request.response->framebuffer_count < 1)
	{
		hcf();
	}

	struct limine_framebuffer *framebuffer =
		framebuffer_request.response->framebuffers[0];

	volatile uint32_t *fb_ptr = framebuffer->address;
	for (size_t y = 0; y < framebuffer->height; y++)
	{
		for (size_t x = 0; x < framebuffer->width; x++)
		{
			uint32_t r = (x * 255) / framebuffer->width;
			uint32_t g = (y * 255) / framebuffer->height;
			uint32_t b = 128; // Constant blue for flavor

			uint32_t color = (r << 16) | (g << 8) | b;

			fb_ptr[y * (framebuffer->pitch / 4) + x] = color;
		}
	}

	hcf();
}
