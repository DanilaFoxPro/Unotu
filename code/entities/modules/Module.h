#ifndef __UNOTU_ENTITIES_MODULES_MODULE_H_
#define __UNOTU_ENTITIES_MODULES_MODULE_H_

/**
 * @brief Should define general reusable functionality.
 * 
 * Base class for modules, which should define general functionality
 * that can be re-used across different classes.
 * 
 */
struct module
{
	module() = default;
	virtual ~module() = default;
};

#endif
