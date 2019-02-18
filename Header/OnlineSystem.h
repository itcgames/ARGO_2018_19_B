#pragma once
#include "System.h"
#include "ClientSocket.h"
#include "OnlineSendComponent.h"
#include "OnlineInputComponent.h"

using std::vector;
using std::string;

class OnlineSystem : public System {
public:
	struct LobbyInfo {
		string name;
		string players;
	};
	OnlineSystem() {};
	virtual ~OnlineSystem() {}
	void addComponent(Component *);
	void addSendingPlayer(OnlineSendComponent*);
	void addReceivingPlayer(OnlineInputComponent*);

	void update(double dt);

	void SendCommands();
	void ReceiveCommands();

	bool ConnectToServer();

	vector<LobbyInfo> getLobbies(); /////////////////

	bool isConnected = false;



private:
	ClientSocket* m_Socket;
	vector<OnlineSendComponent*> m_sendingPlayers;
	vector<OnlineInputComponent*> m_receivingPlayers;
};