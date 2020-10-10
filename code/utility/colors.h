#ifndef __COLORS_H_
#define __COLORS_H_

#include <types.h>

namespace color
{
        const rgb	black		= { 0.0f, 0.0f, 0.0f };
        const rgb	white		= { 1.0f, 1.0f, 1.0f };
        const rgba	transparent	= rgba( color::black, 0.0f );
        
        const rgb red		= { 1.0f, 0.0f, 0.0f };
        const rgb green		= { 0.0f, 1.0f, 0.0f };
        const rgb blue		= { 0.0f, 0.0f, 1.0f };
        
        const rgb gray				= { 0.5f, 0.5f, 0.5f };
        const rgb yellow			= { 1.0f, 1.0f, 0.0f };
        const rgb orange                        = { 1.0f, 0.5f, 0.0f };
        const rgb magenta			= { 1.0f, 0.0f, 1.0f };
        const rgb harvest_gold		= { 0.855f, 0.569f, 0.0f };
        const rgb light_sky_blue	= { 0.529f, 0.808f, 0.98f };
        const rgb pale_silver		= { 0.788f, 0.753f, 0.733f };
}// namespace color

#endif
