#ifndef IUPDATABLE_H
#define IUPDATABLE_H

#include "core/properties/iproperty.h"
#include "core/managers/managee.h"
#include "core/managers/updatemanager.h"

class UpdateManager;

class IUpdatable : public IProperty, public Managee<UpdateManager>
{
public:
    /// Constructor
	IUpdatable(const QString& name = "Basic Updatable");
	/// Destructor
	virtual ~IUpdatable();

	virtual void update(double elapsed_time) = 0;
};

#endif // IUPDATABLE_H
