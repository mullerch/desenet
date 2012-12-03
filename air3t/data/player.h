#pragma once
#include <string>
#include <utils/Address>

namespace Air3T
{
	/**
	 * @ingroup data
	 * @brief Represents a remote Air3T-player that was found using DeSeNet advertisements.
	 */
	class Player
	{
	public:
		/**
		 * @brief Creates an empty player object.
		 */
		explicit Player();

		/**
		 * @brief Creates the player from the node id and the given nickname.
		 *
		 * @param id 		Id of the node the player is using to play.
		 * @param nickname 	The nickname the player set up.
		 */
		explicit Player( const Address<6> & id ,  const std::string & nickname = "" );

		/**
		 * @brief Destructor.
		 */
		virtual ~Player();

		/**
		 * @brief Returns a read-only reference to the ID of the player.
		 *
		 * This is the nothing other than the node id of the node the player is playing with.
		 *
		 * @return Reference to the player's ID.
		 */
		const Address<6> & id() const;

		/**
		 * @brief Changes the player id.
		 *
		 * @param id The new player id to use for the current player.
		 */
		void setId( const Address<6> & id );

		/**
		 * @brief Returns the nickname of the player as a C++ standard string.
		 *
		 * @return Nickname of the player.
		 */
		std::string nickname() const;

		/**
		 * @brief Changes the nickname of the player.
		 *
		 * @param nickname The nickname to set.
		 */
		void setNickname( const std::string & nickname );

	private:
		// The address of the node the player is using.
		Address<6> _id;

		// The player's nickname.
		std::string _nickname;
	};
}
