#pragma once
#include <IPhyTransceiver>


class MeshSimTransceiver : public IPhyTransceiver
{
public:
	// Constructor and destructor...
	MeshSimTransceiver();
	virtual ~MeshSimTransceiver();

	// IPhyTransceiver interface implementation.
	bool initialize();
	Mode mode() const;
	bool setMode( Mode mode );
	RadioChannel radioChannel() const;
	bool setRadioChannel( RadioChannel channel );
	FilterAddress receptionFilterAddress() const;
	bool setReceptionFilterAddress( const FilterAddress & address );
	bool autoAck() const ;
	bool setAutoAck( bool autoAck );
	bool setObserver( IPhyObserver * observer );
	bool send( const Frame & frame );

private:
	// Class uses the PIMP idiom for implementation.
	class Private;
	static Private * _pImpl;
};
