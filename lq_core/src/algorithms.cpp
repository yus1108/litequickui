#include "lq_core/algorithms.h"
#include "algorithms.hpp"

LQ_CORE_API lq_algorithm_skyline_t lq_algorithm_skyline_create(lq_uint16x2_t size, lq_uint16_t padding)
{
	lq_algorithm_skyline_t skyline = new lq_algorithm_skyline();
	skyline->size = size;
	skyline->padding = padding;
	skyline->nodes = { { 0, 0, size.x } };
	return skyline;
}

LQ_CORE_API void lq_algorithm_skyline_destroy(lq_algorithm_skyline_t skyline)
{
	LQ_DEBUG_ASSERT(skyline != NULL, "Skyline must not be null");
	delete skyline;
}

LQ_CORE_API lq_bool_t lq_algorithm_skyline_pack(lq_algorithm_skyline_t skyline, lq_uint16x2_t* out_position, lq_uint16x2_t size)
{
	lq_uint16x2_t best_position = { 0, std::numeric_limits<lq_uint16_t>::max() };
	lq_uint32_t bestIndex = 0;
	lq_bool_t found = lq_false;

	const lq_uint16x2_t paddedSize = lq_uint16x2_create(size.x + skyline->padding, size.y + skyline->padding);
	const lq_uint32_t nodeCount = static_cast<lq_uint32_t>(skyline->nodes.size());
	for (lq_uint32_t i = 0; i < nodeCount; i++)
	{
		lq_uint16_t y = std::numeric_limits<lq_uint16_t>::max();
		if (lq_algorithm_skyline_find_fit(skyline, &y, i, paddedSize))
		{
			if (y < best_position.y || (found && y == best_position.y && skyline->nodes[i].pos.x < skyline->nodes[bestIndex].pos.x))
			{
				best_position.y = y;
				best_position.x = skyline->nodes[i].pos.x;
				bestIndex = i;
				found = lq_true;
			}
		}
	}

	if (found == lq_false)
	{
		return lq_false;
	}

	out_position->x = best_position.x + skyline->padding / 2;
	out_position->y = best_position.y + skyline->padding / 2;

	lq_algorithm_skyline_insert(skyline, bestIndex, best_position, paddedSize);
	return lq_true;
}

lq_bool_t lq_algorithm_skyline_find_fit(const lq_algorithm_skyline_t skyline, lq_uint16_t* out_y, lq_uint32_t index, lq_uint16x2_t size)
{
	lq_uint16x2 pos = skyline->nodes[index].pos;

	// Check if the width fits in the current node
	if (pos.x + size.x > skyline->size.x) { return false; }

	lq_uint16_t widthLeft = size.x;
	lq_uint32_t i = index;

	lq_uint16_t maxY = pos.y;

	while (true)
	{
		// Check if out of bounds
		if (i >= skyline->nodes.size())
		{
			return false;
		}

		maxY = std::max(maxY, skyline->nodes[i].pos.y);
		if (maxY + size.y > skyline->size.y)
		{
			return false;
		}

		if (widthLeft <= skyline->nodes[i].width)
		{
			break;
		}

		widthLeft -= skyline->nodes[i].width;
		i++;
	}

	*out_y = maxY;
	return true;
}

void lq_algorithm_skyline_insert(lq_algorithm_skyline_t skyline, lq_uint32_t index, lq_uint16x2_t pos, lq_uint16x2_t size)
{
	LQ_DEBUG_ASSERT(pos.y <= UINT16_MAX, "Ensure no overflow");
	lq_algorithm_skyline_node_t new_node = { pos.x, static_cast<lq_uint16_t>(pos.y + size.y), size.x };
	skyline->nodes.insert(skyline->nodes.begin() + index, new_node);

	const lq_uint32_t nodeCount = static_cast<lq_uint32_t>(skyline->nodes.size());
	for (uint32_t i = index + 1; i < nodeCount;)
	{
		lq_algorithm_skyline_node_t& node = skyline->nodes[i];

		if (node.pos.x < pos.x + size.x)
		{
			lq_uint16_t shrink = pos.x + size.x - node.pos.x;
			if (node.width <= shrink)
			{
				skyline->nodes.erase(skyline->nodes.begin() + i);
				continue;
			}
			else
			{
				node.pos.x += shrink;
				node.width -= shrink;
				break;
			}
		}
		else
		{
			break;
		}
	}

	lq_algorithm_skyline_merge(skyline);
}

void lq_algorithm_skyline_merge(lq_algorithm_skyline_t skyline)
{
	for (size_t i = 0; i < skyline->nodes.size() - 1;)
	{
		if (skyline->nodes[i].pos.y == skyline->nodes[i + 1].pos.y)
		{
			skyline->nodes[i].width += skyline->nodes[i + 1].width;
			skyline->nodes.erase(skyline->nodes.begin() + i + 1);
		}
		else
		{
			++i;
		}
	}
}
