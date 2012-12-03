#include "View"
#include <assert.h>

namespace Air3T
{
	View::View() : _observer( NULL )
	{}

	View::~View()
	{}

	bool View::setObserver( IViewObserver * observer )
	{
		_observer = observer;
		return observer;
	}
}
