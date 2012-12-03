#include "Player"

namespace Air3T
{
	Player::Player()
	{}

	Player::Player( const Address<6> & id ,  const std::string & nickname ) : _id( id ) , _nickname( nickname )
	{}

	Player::~Player()
	{}

	const Address<6> & Player::id() const
	{
		return _id;
	}

	void Player::setId( const Address<6> & id )
	{
		_id = id;
	}

	std::string Player::nickname() const
	{
		return _nickname;
	}

	void Player::setNickname( const std::string & nickname )
	{
		_nickname = nickname;
	}
}
