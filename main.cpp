#include <xf/xf.h>
#include <IAir3TFactory>
#include <IBuilder>
#include <assert.h>


int main( int argc , char ** argv )
{
	// Prepare XF execution.
	XF_PREPARE;

	// Start the application.
	assert( IAir3TFactory::builder().buildApplication( argc , argv ) );

	// Initialize and start the XF.
	XF::init( 10 );
	XF::start();

	// Exit.
	return 0;
}
