#pragma once
#include "System.h"
#include "ClientSocket.h"
#include "OnlineSendComponent.h"
#include "OnlineInputComponent.h"
#include <Windows.h>
#include <math.h>

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

	void SendCommands(bool sync);
	void ReceiveCommands();

	bool ConnectToServer();

	vector<LobbyInfo> getLobbies(); 

	void makeHost();
	bool joinLobby(int lob);

	vector<int> getPlayers();

	void startGame();

	bool gameStarted = false;

	bool isConnected = false;
	bool m_isHost = false;

	int m_playerNumber = 1;

private:
	ClientSocket* m_Socket;
	int m_lobbyNumber = 0;
	vector<OnlineSendComponent*> m_sendingPlayers;
	vector<OnlineInputComponent*> m_receivingPlayers;
	//double syncRate = 0.2;
	//double tts = 0;//time to sync
};