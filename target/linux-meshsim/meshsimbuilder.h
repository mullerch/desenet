#include <IBuilder>


class MeshSimBuilder : public IBuilder
{
public:
	// Constructor and destructor.
	MeshSimBuilder();
	virtual ~MeshSimBuilder();

	// IBuilder implementation.
	bool buildApplication( int argc = 0 , char ** argv = NULL );
};
